/**************************************************************************
 *	Copyright (c) 2023 Qualcomm Innovation Center, Inc.                   *
 *   All rights reserved.                                                  *
 *   SPDX-License-Identifier: GPL-2.0-or-later                             *
 *																		  *
 ***************************************************************************/


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef KILLPROCESS
#define KILLPROCESS
#endif

#ifdef KILLPROCESS
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#endif

#if (defined(__GNUC__) && defined(__unix__))
#include <dlfcn.h>
#endif

#include <jtag/interface.h>
#include "bitbang.h"
#include "hello.h"
#include <helper/time_support.h>

#include "eud/inc/eud_api.h"

#define OPENOCD_ENV 1
#define SWD_BITBANG_CLK_SHFT 0
#define SWD_BITBANG_DI_SHFT 1
#define SWD_BITBANG_RCTLR_SRST_N_SHFT 2
#define SWD_BITBANG_GPIO_DI_OE 3
#define SWD_BITBANG_GPIO_SRST_N_SHFT 4
#define SWD_BITBANG_GPIO_TRST_N_SHFT 5
#define SWD_BITBANG_DAP_TRST_N_SHFT 6
#define EUD_SUCCESS 0

#define FLUSH_OPTION_NULL 0
#define FLUSH_OPTION_TRUE 1

#define EUD_SWD_FREQ_120_MHz 0x0
#define EUD_SWD_FREQ_80_MHz 0x1

#if 0
#define EUD_CMD_COUNT_LOGGING 1

#endif
// for unix system dlsym lib API will be used to map to exported functions
#if (defined(__GNUC__) && defined(__unix__))
#define LOAD_EXPORTED_FUNC dlsym
#endif
// for windows system GetProcAddress lib API will be used to map to exported functions
#if defined(__WIN32) || defined(__WIN64)
#define LOAD_EXPORTED_FUNC GetProcAddress
#endif

// Function pointers for exported functions in EUD.DLL
// Note: Here f is suffix for functions names exported in EUD.DLL

static SwdEudDevice *gpSWDDevice = NULL;
static uint32_t gDeviceId = 0;

static uint32_t cmd_count = 0;
uint32_t dummy_read;
uint64_t periodic_seq_timeout = 3500;
bool is_pwrupNeeded = false;

#define CTRLSTAT_CHECK_ENABLED 1
#define CTRLSTAT_READ_ENABLED 1


static void trigger_pwr_on_sequence(void);
static void eud_ensure_dbg_sys_pwr_is_on(void);
static EUD_ERR_t handle_SwdAckFault(uint32_t APnDP, uint32_t A2_3, uint32_t *value, bool RnW);

static inline EUD_ERR_t eudWriteWrapper(uint32_t APnDP, uint32_t A2_3, uint32_t val)
{
     return swd_write(gpSWDDevice, APnDP, A2_3, val);
}

static inline EUD_ERR_t eudReadWrapper(uint32_t APnDP, uint32_t A2_3, uint32_t *readVal)
{
    return swd_read(gpSWDDevice, APnDP, A2_3, readVal);
}

static inline void eudGetJtagIDwrapper(uint32_t *DPIDR)
{
    swd_get_jtag_id(gpSWDDevice, DPIDR);
}

static inline EUD_ERR_t eudBitBangWrapper(uint32_t swd_bitbang_value, uint32_t *return_val)
{
    return swd_bitbang(gpSWDDevice, swd_bitbang_value, return_val);
}

static inline EUD_ERR_t eudSetFrequencyWrapper(uint32_t freqVal)
{
    return swd_set_frequency(gpSWDDevice, freqVal);
}


#ifdef KILLPROCESS
static void kill_process(void);
static void kill_process(void)
{
    // pid_t ppid = getppid();
    // pid_t pid  = getpid();
    // LOG_DEBUG ("PPID = %u, PID = %u", ppid, pid);
    // kill (pid, SIGINT);
    raise(SIGINT);
}
#endif

#if 1
int eud_switch_seq(enum swd_special_seq seq)
{
    EUD_ERR_t err = EUD_SUCCESS;
    // LOG_DEBUG("eud_switch_seq\n");
    switch (seq)
    {
    case JTAG_TO_SWD:
        err = jtag_to_swd(gpSWDDevice);
        if (err != EUD_SUCCESS)
            return ERROR_FAIL;
        break;

    case LINE_RESET:
    case SWD_TO_JTAG:
    case SWD_TO_DORMANT:
    case DORMANT_TO_SWD:
    default:
        return ERROR_FAIL;
        break;
    }

    return ERROR_OK;
}
#endif

#if CTRLSTAT_CHECK_ENABLED

static int eud_swd_run(void)
{
    EUD_ERR_t err = EUD_SUCCESS;

#ifdef EUD_CMD_COUNT_LOGGING

    LOG_DEBUG("eud_swd_run: cmd_count: %d", cmd_count);
#endif

    swd_flush (gpSWDDevice);

    if (err == SWD_ERR_SWD_ACK_FAULT_DETECTED)
    {
        err = handle_SwdAckFault(0, 0, NULL, true); // dummy params ok for QUTS
    }

    if (err != EUD_SUCCESS)
    {
        LOG_ERROR("swdflushbuffers FAILED!!");
    }

    return err;
}

static inline void write_abort_1f(void)
{
    eudWriteWrapper(0, 0, 0x1f); // abort write 0x1f
}

static inline void write_abort_4(void)
{
    eudWriteWrapper(0, 0, 0x4); // abort write 0x4
}

static void trigger_pwr_on_sequence(void)
{

    EUD_ERR_t err = EUD_SUCCESS;
    uint32_t APnDP = 0;
    uint32_t A2_3 = 0;
    uint32_t val = 0;

    APnDP = 0;
    A2_3 = 1;
    val = 0;

    val = (uint32_t)(0x50000000);
    err = eudWriteWrapper(APnDP, A2_3, val); // write 0x5000000 to ctrl stat

    if (err != EUD_SUCCESS)
    {
        LOG_ERROR("CTRLSTAT Write in triggger sequence FAILED!! %x", err);
    }

    return;
}

static void eud_ensure_dbg_sys_pwr_is_on(void)
{
    static int call_count = 0;

    if (call_count > 500)
    {
        return;
    }
    else
    {
        call_count++;
    }
    //LOG_DEBUG(" eud_ensure_dbg_sys_pwr_is_on");
    write_abort_4();
    trigger_pwr_on_sequence();
    return;
}

#endif

static EUD_ERR_t handle_SwdAckFault(uint32_t APnDP, uint32_t A2_3, uint32_t *value, bool RnW)
{

    EUD_ERR_t err = EUD_SUCCESS;
    uint32_t _val;

    if (value)
        _val = *value;

    // retry scenario
    write_abort_1f();
    trigger_pwr_on_sequence();
    if (RnW) // Read case
    {
        LOG_DEBUG("Entered read ack fault");
        err = eudReadWrapper(APnDP, A2_3, &_val);
    }
    else // write case
    {
        LOG_DEBUG("Entered write ack fault");
        err = eudWriteWrapper(APnDP, A2_3, _val);
    }

    if (value)
        *value = _val;

    return err;
}

static inline void triggerPeriodicSequence(void)
{
    if (periodic_seq_timeout == 0)
    {
        write_abort_4();
        trigger_pwr_on_sequence();
    }
    else 
    {
        if (cmd_count > periodic_seq_timeout)
        {
            write_abort_4();
            trigger_pwr_on_sequence();
            cmd_count = 0;
        }
    }
}

static void eud_swd_read(uint8_t cmd, uint32_t *value, uint32_t ap_delay_hint)
{
    cmd_count++;
    // LOG_DEBUG("swdread call \n");
    uint32_t APnDP = 0;
    uint32_t A2_3 = 0;
    EUD_ERR_t err = EUD_SUCCESS;

    (void)ap_delay_hint;
    APnDP = (cmd >> 1) & 0x1;
    A2_3 = (cmd >> 3) & 0x3;

    if (ap_delay_hint > 0)
    {

        uint32_t i;
        for (i = 0; i < 2000; i++)
        {
        }
    }

#if CTRLSTAT_CHECK_ENABLED
    // Dont touch ctrlstat register, if DAPIDR or ctrlstat is being read
    if (!(((APnDP == 0) && ((A2_3 == 0) || (A2_3 == 1)))))
    {
        eud_ensure_dbg_sys_pwr_is_on();
        triggerPeriodicSequence();
    }

    /*Below power up request is required & triggered during monitor reset*/
    if (is_pwrupNeeded && !(((APnDP == 0) && ((A2_3 == 0) || (A2_3 == 1)))))
    {
        write_abort_4();
        trigger_pwr_on_sequence();
    }
#endif

    if (value == NULL)
    {
        err = eudReadWrapper(APnDP, A2_3, &dummy_read);

        if (err == SWD_ERR_SWD_ACK_FAULT_DETECTED)
        {
            err = handle_SwdAckFault(APnDP, A2_3, &dummy_read, true);
        }
    }
    else
    {
        err = eudReadWrapper(APnDP, A2_3, value);

        if (err == SWD_ERR_SWD_ACK_FAULT_DETECTED)
        {
            err = handle_SwdAckFault(APnDP, A2_3, value, true);
        }
    }

    if (err != EUD_SUCCESS)
    {
        LOG_ERROR("SWD Read FAILED!! , %X value %d", err, *value);
#ifdef KILLPROCESS
        if (err != SWD_ERR_SWD_ACK_WAIT_DETECTED)
        {
            LOG_ERROR("Going to kill!! , %X", err);
            kill_process();
        }
#endif
    }
    //LOG_DEBUG("swdread  end \n");
    return;
}

static void eud_swd_write(uint8_t cmd, uint32_t value, uint32_t ap_delay_hint)
{
    cmd_count++;
    // LOG_DEBUG("swd write call \n");
    uint32_t APnDP = 0;
    uint32_t A2_3 = 0;
    EUD_ERR_t err;
    // static int j;

    (void)ap_delay_hint;
    APnDP = (cmd >> 1) & 0x1;
    A2_3 = (cmd >> 3) & 0x3;

    if (ap_delay_hint > 0)
    {
        uint32_t i;
        for (i = 0; i < 20000; i++)
        {
        }
    }

#if CTRLSTAT_CHECK_ENABLED

    // Dont touch ctrlstat register, if DAPIDR or ctrlstat is being read
    if (!(((APnDP == 0) && ((A2_3 == 0) || (A2_3 == 1)))))
    {
        eud_ensure_dbg_sys_pwr_is_on();
        triggerPeriodicSequence();
    }

        /*Below power up request is required & triggered during monitor reset*/
    if (is_pwrupNeeded && !(((APnDP == 0) && ((A2_3 == 0) || (A2_3 == 1)))))
    {
        write_abort_4();
        trigger_pwr_on_sequence();
    }
#endif

    // cmd_count += 5;

    // LOG_DEBUG("swd write wrapper call \n");

    err = eudWriteWrapper(APnDP, A2_3, value);

    if (err == SWD_ERR_SWD_ACK_FAULT_DETECTED)
    {
        LOG_DEBUG("swdread wrapper SWD_ERR_SWD_ACK_FAULT_DETECTED \n");
        err = handle_SwdAckFault(APnDP, A2_3, &value, false);
    }

    // LOG_DEBUG("swd write wrapper end \n");

    if (err != EUD_SUCCESS)
        LOG_ERROR("SWD Write FAILED!!");

    // LOG_DEBUG("swd write end \n");
    return;
}

int eud_AssertReset(void)
{
    is_pwrupNeeded = true;
    EUD_ERR_t err;
    uint32_t return_val = 0; // DPIDR = 0;
    uint32_t DPIDR = 0;      // DPIDR = 0;

    eudGetJtagIDwrapper(&DPIDR);

    // Assert reset and tap reset
    uint32_t swd_bitbang_value =
        SWD_BITBANG_CLK_BMSK_DEASSERT +
        SWD_BITBANG_DI_BMSK_DEASSERT +
        SWD_BITBANG_RCTLR_SRST_BMSK_ASSERT + // this is the reset value
        SWD_BITBANG_GPIO_DI_OE_DEASSERT +
        SWD_BITBANG_GPIO_SRST_BMSK_DEASSERT +
        SWD_BITBANG_GPIO_TRST_BMSK_DEASSERT +
        SWD_BITBANG_DAP_TRST_BMSK_ASSERT; // Also reset TAP

    LOG_DEBUG("OCD: Bitbang eud_AssertReset wrapper call val: %" PRIu32 " \n", swd_bitbang_value);
    err = eudBitBangWrapper(swd_bitbang_value, &return_val);
    LOG_DEBUG("OCD: Bitbang eud_AssertReset wrapper end \n");

    // De-assert tap reset but keep SRST asserted.
    swd_bitbang_value =
        SWD_BITBANG_CLK_BMSK_DEASSERT +
        SWD_BITBANG_DI_BMSK_DEASSERT +
        SWD_BITBANG_RCTLR_SRST_BMSK_ASSERT + // this is the reset value
        SWD_BITBANG_GPIO_DI_OE_DEASSERT +
        SWD_BITBANG_GPIO_SRST_BMSK_DEASSERT +
        SWD_BITBANG_GPIO_TRST_BMSK_DEASSERT +
        SWD_BITBANG_DAP_TRST_BMSK_DEASSERT; // Deassert TAP reset

    LOG_DEBUG("OCD: Bitbang eud_AssertReset wrapper call val: %" PRIu32 " \n", swd_bitbang_value);
    err = eudBitBangWrapper(swd_bitbang_value, &return_val);
    LOG_DEBUG("OCD: Bitbang eud_AssertReset wrapper end \n");

    if (err != ERROR_OK)
    {
        printf("%s error: line %d\n", __FUNCTION__, __LINE__);
        return err;
    }

    eudGetJtagIDwrapper(&DPIDR);

    printf(" From inside assert api, before jtag to swd DPIDR: %x\n", DPIDR);

    return err;
}

int eud_DeAssertReset(void)
{
    EUD_ERR_t err;
    uint32_t return_val = 0;
    // deassert reset
    uint32_t swd_bitbang_value =
        SWD_BITBANG_CLK_BMSK_DEASSERT +
        SWD_BITBANG_DI_BMSK_DEASSERT +
        SWD_BITBANG_RCTLR_SRST_BMSK_DEASSERT + // this is thede reset value
        SWD_BITBANG_GPIO_DI_OE_DEASSERT +
        SWD_BITBANG_GPIO_SRST_BMSK_DEASSERT +
        SWD_BITBANG_GPIO_TRST_BMSK_DEASSERT +
        SWD_BITBANG_DAP_TRST_BMSK_DEASSERT;

    LOG_DEBUG("OCD: Bitbang eud_AssertReset wrapper call val: %" PRIu32 " \n", swd_bitbang_value);
    err = eudBitBangWrapper(swd_bitbang_value, &return_val);
    LOG_DEBUG("OCD: Bitbang eud_AssertReset wrapper end \n");

    is_pwrupNeeded = false;
    for (int i = 0; i < 10; i++)
    {
        trigger_pwr_on_sequence();
    }
    if (err != ERROR_OK)
    {
        printf("%s error: line %d\n", __FUNCTION__, __LINE__);
        return err;
    }

    return err;
}

int eud_reset(int trst, int srst)
{
    LOG_DEBUG("eud_reset call \n");
    int retval = EUD_SUCCESS;

    if (srst == 1)
    {
        retval = eud_AssertReset();

        if (retval)
        {
            printf("libeud_reset call libusb_AssertReset, error code %x\n", retval);
            return retval;
        }

        printf("libeud_reset call libusb_AssertReset successfully\n");
    }
    else
    {

        retval = eud_DeAssertReset();

        if (retval)
        {
            printf("libeud_reset call libusb_DeAssertReset, error code %x\n", retval);
            return retval;
        }

        printf("libeud_reset call libusb_DeAssertReset successfully\n");
    }

    return retval;
}


static int eud_khz(int khz, int *speed)
{
    LOG_DEBUG("eud_khz %d", khz);
    if (khz >= 0 && khz <= 1875)
        *speed = 0x6;
    else if (khz > 1875 && khz <= 3750)
        *speed = 0x5;
    else if (khz > 3750 && khz <= 7500)
        *speed = 0x4;
    else if (khz > 7500 && khz <= 10000)
        *speed = 0x3;
    else if (khz > 10000 && khz <= 15000)
        *speed = 0x2;

    // hard code speed to 15 mhz
    //	*speed=2;
    return ERROR_OK;
    /*
        - 0x0:  30 MHz	not supported
        - 0x1:  20 MHz	not supported
        - 0x2:  15 MHz
        - 0x3:  10 MHz
        - 0x4:  7.5 MHz
        - 0x5:  3.75 MHz
        - 0x6:  1.875 MHz
        - 0x7:  938 kHz	not supported
        - 0x8:  469 kHz	not supported
        - 0x9:  234 kHz	not supported
        - 0xA:  117 kHz	not supported
        - 0xB:  59 kHz	not supported
        - 0xC:  30 kHz	not supported
    */

#if 0
	if (khz >= 0  && khz <= 117)
		*jtag_speed = 0xC;     // EUD_SWD_FREQ_0_117_MHz
	else if (khz > 117  && khz <= 234)
		*jtag_speed = 0xB;     // EUD_SWD_FREQ_0_234_MHz
	else if (khz > 234  && khz <= 469)
			*jtag_speed = 0xA; // EUD_SWD_FREQ_0_469_MHz
	else if (khz > 469  && khz <= 938)
			*jtag_speed = 0x9; // EUD_SWD_FREQ_0_938_MHz
	else if (khz > 938  && khz <= 1875)
			*jtag_speed = 0x8; // EUD_SWD_FREQ_1_875_MHz
	else if (khz > 1875  && khz <= 3750)
			*jtag_speed = 0x7; // EUD_SWD_FREQ_3_75_MHz
	else if (khz > 3750  && khz <= 7500)
			*jtag_speed = 0x6; // EUD_SWD_FREQ_7_5_MHz
	else if (khz > 7500  && khz <= 15000)
			*jtag_speed = 0x5; // EUD_SWD_FREQ_15_MHz
	else if (khz > 15000  && khz <= 30000)
			*jtag_speed = 0x4; // EUD_SWD_FREQ_30_MHz
	else if (khz > 30000  && khz <= 40000)
			*jtag_speed = 0x3; // EUD_SWD_FREQ_40_MHz
	else if (khz > 40000  && khz <= 60000)
			*jtag_speed = 0x2; // EUD_SWD_FREQ_60_MHz
	else if (khz > 60000  && khz <= 80000)
			*jtag_speed = 0x1; // EUD_SWD_FREQ_80_MHz
	else if (khz > 80000)
			*jtag_speed = 0x0; // EUD_SWD_FREQ_120_MHz
#endif

    // return ERROR_OK;
}

#ifdef EUD_SPEED_DIV
static int eud_speed_div(int speed, int *khz)
{
	if (speed == 0)
		*khz = 0;
	else
		*khz = 64000/speed;

	return ERROR_OK;
}
#endif

static int eud_set_speed(int speed)
{
    // LOG_DEBUG("eud_set_speed call \n");
    EUD_ERR_t err = EUD_SUCCESS;

    err = eudSetFrequencyWrapper(0x4);

    if (err != EUD_SUCCESS)
        return ERROR_FAIL;

    return ERROR_OK;
}

static int eud_init(void)
{
    // Read EUD version in use
    uint32_t major_rev = 0;
    uint32_t minor_rev = 0;
    uint32_t spin_rev = 0;
    eud_get_version(&major_rev, &minor_rev, &spin_rev);
    LOG_INFO("Using EUD %u.%u.%u", major_rev, minor_rev, spin_rev);

    return ERROR_OK;
}

static int eud_quit(void)
{
    return ERROR_OK;
}


static int eud_swd_init(void)
{
    EUD_ERR_t err = EUD_SUCCESS;

    uint32_t arr[100] = {0};
    uint32_t len = 0;

    err = get_device_id_array(arr, &len);
    if (err != EUD_SUCCESS)
        return ERROR_FAIL;

    gDeviceId = arr[0];
    gpSWDDevice = eud_initialize_device_swd(gDeviceId, OPENOCD_ENV, &err);

    if (err != EUD_SUCCESS)
        return ERROR_FAIL;

    return err;
}

#define FLUSH_OPTION_NULL 0
#define FLUSH_OPTION_TRUE 1

COMMAND_HANDLER(eud_trigger_seq)
{
    COMMAND_PARSE_ADDRESS(CMD_ARGV[0], periodic_seq_timeout);
	LOG_INFO("periodic_seq_timeout = 0x%lx ", periodic_seq_timeout);
	return ERROR_OK;
}

static const struct command_registration eud_exec_command_handlers[] = 
{
    {
		.name = "trigger_seq",
		.handler = eud_trigger_seq,
		.mode = COMMAND_EXEC,
		.help = "set periodic trigger frequency",
	.usage = "",
    },

    COMMAND_REGISTRATION_DONE
};

static const struct command_registration eud_command_handlers[] = {
    {
		.name  = "eud",
		.mode  = COMMAND_ANY,
		.help  = "EUD interface driver commands",
		.chain = eud_exec_command_handlers,
		.usage = "",
    },
	COMMAND_REGISTRATION_DONE,
};


/* The eud driver is used to easily check the code path
 * where the target is unresponsive.
 */
static struct swd_driver eud_swd_driver = 
{
    .init = eud_swd_init,
    .switch_seq = eud_switch_seq,
    .read_reg = eud_swd_read,
    .write_reg = eud_swd_write,
    .run = eud_swd_run,
    .trace = NULL
};

static const char *const eud_transports[] = {"swd", "jtag", NULL};

struct adapter_driver eud_adapter_driver = {
    .name = "eud",
    .transports = eud_transports,
    .commands = eud_command_handlers,

    .init = eud_init,
    .quit = eud_quit,
    .reset = eud_reset,
    .speed = eud_set_speed,
    .khz = eud_khz,
    .speed_div = NULL, // eud_speed_div,
    .power_dropout = NULL,
    .srst_asserted = NULL,
    .config_trace = NULL,
    .poll_trace = NULL,
    .swd_ops = &eud_swd_driver,
};
