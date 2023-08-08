/**************************************************************************
*	Copyright (c) 2023 Qualcomm Innovation Center, Inc.                   *
*   All rights reserved.                                                  * 
*   SPDX-License-Identifier: GPL-2.0-or-later                             * 
*																		  *
***************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "aarch64.h"
#include "target_type.h"
#include "aarch64_system_registers.h"
#include "armv8_opcodes.h"

/* convert bitlen bits of v to string b
+   assuming b is at least bitlen+1 size */
static void tobin(char *b, uint32_t v, uint8_t bitlen)
{
    size_t j = 0;
    for (int i = bitlen - 1; i >= 0; i--)
    {
        if (v & (1 << i))
                b[j] = '1';
        else
                b[j] = '0';
        j++;
    }
    b[j] = 0;
}

// Gets the encoding from the opcode given to the command
static inline uint32_t get_aarch64_system_register_encoding( uint8_t op0, uint8_t op1, uint8_t crn, uint8_t crm, uint8_t op2)
{
    return (op0 << 14) | (op1 << 11) | (crn << 7) | (crm << 3) | op2;
}

// Gets the register details required for reading/writing to registers from the name provided with the command
static const struct aarch64_system_register *get_aarch64_system_register_from_name(const char *name)
{
    size_t i = 0;
    struct aarch64_system_register sysreg = aarch64_system_registers[0];

    while (sysreg.short_name)
    {
        if (strcasecmp(sysreg.short_name, name) == 0)
            return &aarch64_system_registers[i];
        i++;
        sysreg = aarch64_system_registers[i];
    }

    return NULL;
}

// Gets the register details required for reading/writing to registers from the encoding
static const struct aarch64_system_register *get_aarch64_system_register_from_encoding(uint32_t encoding)
{
    size_t i = 0;
    struct aarch64_system_register sysreg = aarch64_system_registers[i];

    while (sysreg.short_name)
    {
        if (sysreg.encoding == encoding)
                return &aarch64_system_registers[i];
        i++;
        sysreg = aarch64_system_registers[i];
    }

    return NULL;
}

// Gets the encoding required for reading/writing to registers from the command arguments
static int get_aarch64_system_register_encoding_from_command_arguments(struct command_invocation *cmd, uint32_t *encoding)
{
    uint8_t op0;
    uint8_t op1;
    uint8_t crn;
    uint8_t crm;
    uint8_t op2;

    COMMAND_PARSE_NUMBER(u8, CMD_ARGV[0], op0);
    COMMAND_PARSE_NUMBER(u8, CMD_ARGV[1], op1);
    COMMAND_PARSE_NUMBER(u8, CMD_ARGV[2], crn);
    COMMAND_PARSE_NUMBER(u8, CMD_ARGV[3], crm);
    COMMAND_PARSE_NUMBER(u8, CMD_ARGV[4], op2);

    if (op0 > 3)
    {
        command_print(cmd, "op0 should be <= 3");
        return ERROR_COMMAND_SYNTAX_ERROR;
    }
    if (op1 > 7)
    {
        command_print(cmd, "op1 should be <= 7");
        return ERROR_COMMAND_SYNTAX_ERROR;
    }
    if (crn > 15)
    {
        command_print(cmd, "CRn should be <= 15");
        return ERROR_COMMAND_SYNTAX_ERROR;
    }
    if (crm > 15)
    {
        command_print(cmd, "CRm should be <= 15");
        return ERROR_COMMAND_SYNTAX_ERROR;
    }
    if (op2 > 7)
    {
        command_print(cmd, "op2 should be <= 7");
        return ERROR_COMMAND_SYNTAX_ERROR;
    }

    *encoding = get_aarch64_system_register_encoding(op0, op1, crn, crm, op2);

    return ERROR_OK;
}

// Prints the list of system registers for armv8
static void print_aarch64_system_registers(struct command_invocation *cmd)
{
    size_t i = 0;
    struct aarch64_system_register sysreg = aarch64_system_registers[i];

    while (sysreg.short_name)
    {
        command_print(cmd, "%s: %s", sysreg.short_name, sysreg.long_name);
        i++;
        sysreg = aarch64_system_registers[i];
    }
}

// used for writing into the coprocessor registers
COMMAND_HANDLER(aarch64_handle_msr_command)
{
    struct target *target = get_current_target(CMD_CTX);
    struct arm *arm = target_to_arm(target);
    struct arm_dpm *dpm = arm->dpm;
    const struct aarch64_system_register *sysreg;
    uint64_t value;
    uint32_t encoding;
    int retval;

    if (target->state != TARGET_HALTED)
    {
        LOG_WARNING("target not halted");
        return ERROR_TARGET_NOT_HALTED;
    }

    if (CMD_ARGC == 0)
    {
        print_aarch64_system_registers(cmd);
        return ERROR_OK;
    }

    switch (CMD_ARGC)
    {
    case 6:
        COMMAND_PARSE_NUMBER(u64, CMD_ARGV[5], value);
        retval = get_aarch64_system_register_encoding_from_command_arguments(cmd, &encoding);
        
        if (retval != ERROR_OK)
            return ERROR_COMMAND_SYNTAX_ERROR;
        
        break;

    case 2:
        COMMAND_PARSE_NUMBER(u64, CMD_ARGV[1], value);
        sysreg = get_aarch64_system_register_from_name(CMD_ARGV[0]);
        if (!sysreg)
        {
            command_print(cmd, "system register %s is not supported", CMD_ARGV[0]);
            return ERROR_COMMAND_SYNTAX_ERROR;
        }
        encoding = sysreg->encoding;
        break;

    default:
        return ERROR_COMMAND_SYNTAX_ERROR;
    }

    retval = dpm->prepare(dpm);

    if (retval != ERROR_OK)
            return retval;

    /* rt is implictly 0 below in order to get this from arguments,
        * dpm should be modified as well */
    retval = dpm->instr_write_data_r0_64(dpm, ARMV8_MSR_GP(encoding, 0), value);
    /* (void) */ 
    dpm->finish(dpm);
    return retval;
}

// used for reading the coprocessor registers
COMMAND_HANDLER(aarch64_handle_mrs_command)
{
    struct target *target = get_current_target(CMD_CTX);
    struct arm *arm = target_to_arm(target);
    struct arm_dpm *dpm = arm->dpm;
    const struct aarch64_system_register *sysreg;
    bool decode_fields = false;
    uint32_t encoding;
    int retval;

    if (target->state != TARGET_HALTED)
    {
        LOG_WARNING("target not halted");
        return ERROR_TARGET_NOT_HALTED;
    }

    if (CMD_ARGC == 0)
    {
        print_aarch64_system_registers(cmd);
        return ERROR_OK;
    }

    switch (CMD_ARGC)
    {
    case 6:
    case 5:
        retval = get_aarch64_system_register_encoding_from_command_arguments(cmd, &encoding);
        if (retval != ERROR_OK)
            return ERROR_COMMAND_SYNTAX_ERROR;
        break;

    case 2:
    case 1:
        sysreg = get_aarch64_system_register_from_name(CMD_ARGV[0]);
        if (!sysreg)
        {
            command_print(cmd, "system register %s is not supported", CMD_ARGV[0]);
            return ERROR_COMMAND_SYNTAX_ERROR;
        }
        encoding = sysreg->encoding;
        break;

    default:
        return ERROR_COMMAND_SYNTAX_ERROR;
    }

    if (CMD_ARGC == 2 || CMD_ARGC == 6)
        decode_fields = !strcmp(CMD_ARGV[CMD_ARGC - 1], "decode");

    retval = dpm->prepare(dpm);

    if (retval != ERROR_OK)
        return retval;

    uint64_t value;

    /* rt is implictly 0 below in order to get this from arguments,
        * dpm should be modified as well */
    retval = dpm->instr_read_data_r0_64(dpm, ARMV8_MRS(encoding, 0), &value);
    if (retval == ERROR_OK)
    {
        /* this might return NULL if a register not defined above is
            * addressed */
        sysreg = get_aarch64_system_register_from_encoding(encoding);

        if (!sysreg)
        {
            char op0v[3];
            char op1v[4];
            char crnv[5];
            char crmv[5];
            char op2v[4];

            tobin(op0v, (encoding >> 14) & 0x3, 2);
            tobin(op1v, (encoding >> 11) & 0x7, 3);
            tobin(crnv, (encoding >> 7) & 0xF, 4);
            tobin(crmv, (encoding >> 3) & 0xF, 4);
            tobin(op2v, (encoding & 0x7), 3);

            command_print(cmd, "S%s_%s_%s_%s_%s: 0x%016" PRIx64, op0v, op1v, crnv, crmv, op2v, value);
        }

        else
        {
            command_print(cmd, "%s : 0x%016" PRIx64, sysreg->short_name, value);
            size_t i = 0;
            struct aarch64_system_register_field field = sysreg->fields[i];

            while (decode_fields && field.name)
            {
                uint8_t msb = field.msb;
                uint8_t lsb = field.lsb;

                uint64_t mask = (1 << (msb - lsb + 1)) - 1;
                uint64_t val = ((value >> lsb) & mask);
                char binval[64];
                tobin(binval, val, (msb - lsb + 1));
                command_print(cmd, ".%s =0x%" PRIx64 " =0b%s", field.name, val, binval);
                i++;
                field = sysreg->fields[i];
            }
        }
    }

    /* (void) */ 
    dpm->finish(dpm);
    return retval;
}

// new commands for reading or writing into coprocessor registers
const struct command_registration coproc_command_handlers[] = {
    {
        .name = "msr",
        .handler = aarch64_handle_msr_command,
        .mode = COMMAND_EXEC,
        .help = "write to system register",
        .usage = "[systemreg value|op0 op1 CRn CRm op2 value]",
    },
    {
        .name = "mrs",
        .handler = aarch64_handle_mrs_command,
        .mode = COMMAND_EXEC,
        .help = "read from system register",
        .usage = "[systemreg|op0 op1 CRn CRm op2] [decode]",
    },
    COMMAND_REGISTRATION_DONE
};