/***************************************************************************
 *   Copyright (C) 2015 by David Ung                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 ***************************************************************************/
// extern int is_spurious_breakpoint;

#ifndef OPENOCD_TARGET_HEXAGON_H
#define OPENOCD_TARGET_HEXAGON_H

#include "armv8.h"
#define RSPVERSION 0x1

/*******************************Common MACRO for Feature control********************************************/

typedef struct Q6_global
{
	double execution_time;
	double buffer_execution;
	uint32_t refresh_indicator;
    uint32_t revision_num;

	uint32_t debug_thread_id;
	int selected_process;
	uint16_t total_process;
	uint64_t hexagon_debug_process_id;

	uint32_t qurtk_vtlb_main_addr;
	target_addr_t qurtk_vtlb_entries; 
	target_addr_t QURTK_vtlb_revision;
    uint64_t bitmap_addr;
} Q6_global;

typedef struct current_hexagon_target  
{
    bool hexagon_adsp;
    bool hexagon_cdsp;
    bool hexagon_mpss;
    bool hexagon;
} current_hexagon_target;

#define ClkEnRegs 8

/* define this macro when we need to fetch VTLB using MEMW interface */
//#define HEXAGON_VTLB_MEMW 



#define _VTLB_ENABLED
#define _KODIAK_TARGET
#define BREAKPOINT_THREAD_SELECT
#define HEXAGON_VTLB_MEM_PHYS
// #define _DEBUG_HEXAGON_
/***************************************************************************/

#define HEXAGON_VTLB_NEW_ARCH

#define HEXAGON_COMMON_MAGIC 0xD2D2D2D2   //TDB

/* HEXAGON_MSS_QDSP6SS_WDOG_CTL value is same for Kodiak , Cedros and BITRA */
#define  HEXAGON_MSS_QDSP6SS_WDOG_DISABLE  0x0
#define  HEXAGON_MSS_QDSP6SS_WDOG_ENABLE   0x7



#define HEXAGON_MAX_ISDB_REG 17

/** ISDB register offsets w.r.t Hexagon debug_base (Sec:13.5) **/
#define HEXAGON_ISDB_ISDBST            0x00
#define HEXAGON_ISDB_ISDBCFG0          0x04
#define HEXAGON_ISDB_ISDBCFG1          0x08
#define HEXAGON_ISDB_BRKPTINFO         0x0C
#define HEXAGON_ISDB_BRKPTPC0          0x10
#define HEXAGON_ISDB_BRKPTCFG0         0x14
#define HEXAGON_ISDB_BRKPTPC1          0x18
#define HEXAGON_ISDB_BRKPTCFG1         0x1C
#define HEXAGON_ISDB_STFINST           0x20
#define HEXAGON_ISDB_ISDBMBXIN         0x24
#define HEXAGON_ISDB_ISDBMBXOUT        0x28
#define HEXAGON_ISDB_ISDBCMD           0x2C
#define HEXAGON_ISDB_ISDBEN            0x30
#define HEXAGON_ISDB_ISDBVER           0x34
#define HEXAGON_ISDB_COREVER           0x38
#define HEXAGON_ISDB_ISDBGPR           0x3C
#define HEXAGON_ISDB_ISDBCST           0x40

#define HEXAGON_ALL_ISDB_REG           0xAA

/** SYSCFG register field **/
#define SYSCFG_L2NRA                   (0x1 << 22)
#define SYSCFG_L2NWA                   (0x1 << 21)
#define SYSCFG_L2WB                    (0x1 << 23)


/** ISDB status register fields **/
#define ISDBST_ISDB_READY              (0x1 << 0)
#define ISDBST_ISDB_MAILBOX_OUT        (0x1 << 1)
#define ISDBST_ISDB_MAILBOX_IN         (0x1 << 2)
#define ISDBST_ISDB_CMD_STATUS         (0x1 << 4)
#define ISDBST_STUFF_CMD_STATUS        (0x1 << 5)
#define ISDBST_DEBUG_MODE_STATUS       (0xFF << 8)
#define ISDBST_OFF_MODE_STATUS         (0xF << 16)
#define ISDBST_WAITRUN_MODE_STATUS     (0xF << 24)

/** ISDB configuration register0 fields **/
#define ISDBCFG0_ETMBRKPT_TNUM_MASK    (0x3F << 0)
#define ISDBCFG0_EXTBRKPT_TNUM_MASK    (0xF << 8)
#define ISDBCFG0_EXTRESUME_TNUM_MASK   (0xF << 16)
#define ISDBCFG0_GLOBAL_INT_DISABLE    (0x1 << 24)
#define ISDBCFG0_HW_BREAKPOINT0        (0x1 << 25)
#define ISDBCFG0_HW_BREAKPOINT1        (0x1 << 26)


/** ISDB configuration register1 fields **/
#define ISDBCFG1_BRK_EVENT_TNUM_MASK   (0xF << 0)
#define ISDBCFG1_SWBRKPT_TNUM_MASK     (0xF << 8)
#define ISDBCFG1_BREAK0_TNUM_MASK      (0xF << 16)
#define ISDBCFG1_BREAK1_TNUM_MASK      (0xF << 24)


/** Break point information register fields **/
#define BRKPTINFO_THREAD0_BRKPT_SOURCE (0x7 << 0)
#define BRKPTINFO_THREAD1_BRKPT_SOURCE (0x7 << 3)
#define BRKPTINFO_THREAD2_BRKPT_SOURCE (0x7 << 6)
#define BRKPTINFO_THREAD3_BRKPT_SOURCE (0x7 << 9)
#define BRKPTINFO_THREAD4_BRKPT_SOURCE (0x7 << 12)
#define BRKPTINFO_THREAD5_BRKPT_SOURCE (0x7 << 15)

/** Break point PC register fields **/
#define BRKPTPC_0_1_BRKPTPC        (0x3FFFFFFF << 2)

/** ISDB Command register fields **/
#define ISDBCMD_CMD_BREAK              (0x1 << 0)
#define ISDBCMD_CMD_RESUME             (0x2 << 0)
#define ISDBCMD_CMD_ISTEP              (0x3 << 0)
#define ISDBCMD_CMD_STUFF              (0x4 << 0)
#define ISDBCMD_CMD_RST_ASSRT          (0x5 << 0)
#define ISDBCMD_CMD_INTRPT             (0x6 << 0)
#define ISDBCMD_CMD_RST_RELEASE        (0x7 << 0)

#define ISDBCMD_USER_LVL               (0x0 << 6)
#define ISDBCMD_GUEST_LVL              (0x1 << 6)
#define ISDBCMD_MONITOR_LVL            (0x2 << 6)
#define ISDBCMD_TNUM_MASK_2		        (0x3 << 8)
#define ISDBCMD_TNUM_MASK_4             (0xF << 8)
#define ISDBCMD_TNUM_MASK_6             (0x3F << 8)
#define ISDBCMD_TNUM_MASK_8	            (0xFF << 8)
// #define ISDBCMD_TNUM_RESUME_ALL_THREADS(A)    (((1 << (A)) - 1) << 8)
#define ISDBCMD_TNUM_MASK_THREAD(A)    (0x1 << (8+A))
 

/** ISDB Enable register fields **/
#define ISDBEN_APB_ISDB_EN             (0x1 << 0)
#define ISDBEN_ISDB_CLK_OFF            (0x1 << 1)
#define ISDBEN_SECURE_EN               (0x1 << 2)
#define ISDBEN_ISDB_TRUSTED            (0x1 << 3)
#define ISDBEN_ISDB_PREVNT_PWRDWN      (0x1 << 4)
#define ISDBEN_SECURITY_DFD_EN_STATUS  (0x1 << 5)
#define ISDBEN_SECURITY_ETM_EN_STATUS  (0x1 << 6)


/** ISDB Version register fields **/
#define COREVER_ISA            (0xFF << 0)
#define COREVER_ID             (0xFF << 8)
#define COREVER_LAYER          (0xF << 16)
#define COREVER_UID            (0xF << 20)
#define COREVER_METAL          (0xFF << 24)



/** ISDB Core Status register fields **/
#define ISDBCST_ALLWIAT        (0x1 << 1)
#define ISDBCST_RESET_OR_PC    (0x1 << 2)
#define ISDBCST_AXIM_BUS_ISO   (0x1 << 3)
#define ISDBCST_AXIM2_BUS_ISO  (0x1 << 4)

/** page table related macros **/
#define VIRT_PAGE(hi)  (hi & 0xfffff)
#define PAGE_VALS(lo)  (lo == 1 ? "4KB" : (lo == 2 ? "16KB" : (lo == 4 ? "64KB" : (lo == 8 ? "256KB" : (lo == 16 ? "1MB" : (lo == 32 ? "4MB" : "16MB"))))))
#define ASID(hi)       ((hi >> 20) & 0x7F)
// #define ASID(hi)       ((hi >> 11) & 0x7E)
//  1111 1 111 1110
#define P_READ(lo)     ((lo >> 29) & 0x01)
#define P_WRITE(lo)    ((lo >> 30) & 0x01)
#define P_EXEC(lo)     ((lo >> 31) & 0x01)
#define P_USER(lo)     ((lo >> 28) & 0x01)
#define P_CCCC(lo)     ((lo >> 24) & 0x0f)
#define P_S(lo)        ((lo >> 0) & 0x01)
#define P_V(hi)        ((hi >> 31) & 0x01)
#define P_G(hi)        ((hi >> 30) & 0x01)
#define P_EP(hi)       ((hi >> 29) & 0x01)
#define P_A1(hi)       ((hi >> 28) & 0x01)
#define P_A0(hi)       ((hi >> 27) & 0x01)

#define PAGE_SIZE(lo, mask)  PAGE_VALS((lo & mask))

#define HEXAGON_PAGE_SIZE_4K         (0x00001000)
#define HEXAGON_PAGE_SIZE_16K        (0x00004000)
#define HEXAGON_PAGE_SIZE_64K        (0x00010000)
#define HEXAGON_PAGE_SIZE_256K       (0x00040000)
#define HEXAGON_PAGE_SIZE_1M         (0x00100000)
#define HEXAGON_PAGE_SIZE_4M         (0x00400000)
#define HEXAGON_PAGE_SIZE_16M        (0x01000000)


enum hexagon_hw_thread {
    HEXAGON_HW_THREAD0=0,
    HEXAGON_HW_THREAD1,
    HEXAGON_HW_THREAD2,
    HEXAGON_HW_THREAD3,
    HEXAGON_HW_THREAD4,
    HEXAGON_HW_THREAD5,
    HEXAGON_HW_THREAD_ALL
};

enum hexagon_registers {
/** General Purpose Registers(32) **/ 
    HEXAGON_R0 = 0,
    HEXAGON_R1,
    HEXAGON_R2,
    HEXAGON_R3,
    HEXAGON_R4,
    HEXAGON_R5,
    HEXAGON_R6,
    HEXAGON_R7,
    HEXAGON_R8,
    HEXAGON_R9,
    HEXAGON_R10,
    HEXAGON_R11,
    HEXAGON_R12,
    HEXAGON_R13,
    HEXAGON_R14,
    HEXAGON_R15,
    HEXAGON_R16,
    HEXAGON_R17,
    HEXAGON_R18,
    HEXAGON_R19,
    HEXAGON_R20,
    HEXAGON_R21,
    HEXAGON_R22,
    HEXAGON_R23,
    HEXAGON_R24,
    HEXAGON_R25,
    HEXAGON_R26,
    HEXAGON_R27,
    HEXAGON_R28,
    HEXAGON_SP,
    HEXAGON_FP,  // 30
    HEXAGON_LR      = 31,
    HEXAGON_GPR_MAX = 32,

/** Control Registers(32) **/
    HEXAGON_SA0 = 32,      /* C0  */
    HEXAGON_LC0,           /* C1  */
    HEXAGON_SA1,           /* C2  */
    HEXAGON_LC1,           /* C3  */
    HEXAGON_P30,           /* C4  */
    HEXAGON_C5_RESRV,      /* C5 - reserved */
    HEXAGON_M0,            /* C6  */
    HEXAGON_M1,            /* C7  */
    HEXAGON_USR,           /* C8  */
    HEXAGON_PC,            /* C9  */
    HEXAGON_UGP,           /* C10 */
    HEXAGON_GP,            /* C11 */
    HEXAGON_CS0,           /* C12 */
    HEXAGON_CS1,           /* C13 */
    HEXAGON_UPCYCLLO,      /* C14 */
    HEXAGON_UPCYCLHI,      /* C15 */
    HEXAGON_FLMT,          /* C16 */
    HEXAGON_FKEY,          /* C17 */
    HEXAGON_PKTCNTLO,      /* C18 */
    HEXAGON_PKTCNTHI,      /* C19 */
    HEXAGON_C20_RESRV,     /* C20 - reserved */
    HEXAGON_C21_RESRV,     /* C21 - reserved */
    HEXAGON_C22_RESRV,     /* C22 - reserved */
    HEXAGON_C23_RESRV,     /* C23 - reserved */
    HEXAGON_C24_RESRV,     /* C24 - reserved */
    HEXAGON_C25_RESRV,     /* C25 - reserved */
    HEXAGON_C26_RESRV,     /* C26 - reserved */
    HEXAGON_C27_RESRV,     /* C27 - reserved */
    HEXAGON_C28_RESRV,     /* C28 - reserved */
    HEXAGON_C29_RESRV,     /* C29 - reserved */        
    HEXAGON_UTMRLO,        /* C30 */
    HEXAGON_UTMRHI = 63,   /* C31 */
    HEXAGON_CTRL_MAX = 64,

/** Monitor Mode per-thread Control Registers(16) **/   
    HEXAGON_SGP0 = 64,     /* S0  */
    HEXAGON_SGP1,          /* S1  */
    HEXAGON_STID,          /* S2  */
    HEXAGON_ELR,           /* S3  */
    HEXAGON_BADVA0,        /* S4  */
    HEXAGON_BADVA1,        /* S5  */
    HEXAGON_SSR,           /* S6  */
    HEXAGON_CCR,           /* S7  */
    HEXAGON_HTID,          /* S8  */
    HEXAGON_BADVA,         /* S9  */
    HEXAGON_IMASK,         /* S10 */
    HEXAGON_GEVB,          /* S11 */
    HEXAGON_S12_RESRV,     /* S12 - reserved */
    HEXAGON_S13_RESRV,     /* S13 - reserved */
    HEXAGON_S14_RESRV,     /* S14 - reserved */
    HEXAGON_S15_RESRV = 79,/* S15 - reserved */    
    HEXAGON_MMODE_PERTHRD_MAX = 80,

/** Monitor Mode Global Control Registers(16) **/
    HEXAGON_EVB = 80,      /* S16 */
    HEXAGON_MODECTL,       /* S17 */
    HEXAGON_SYSCFG,        /* S18 */
    HEXAGON_S19_RESRV,     /* S19 - reserved */
    HEXAGON_IPENDAD,       /* S20 */
    HEXAGON_VID,           /* S21 */
    HEXAGON_VID1,          /* S22 */
    HEXAGON_BESTWAIT,      /* S23 */
    HEXAGON_S24_RESRV,     /* S24 - reserved  */
    HEXAGON_SCHDCFG,       /* S25 */
    HEXAGON_S26_RESRV,     /* S26 - reserved  */
    HEXAGON_CFGBASE,       /* S27 */
    HEXAGON_DIAG,          /* S28 */
    HEXAGON_REV,           /* S29 */
    HEXAGON_PCYCLELO,      /* S30 */
    HEXAGON_PCYCLEHI = 95, /* S31 */
    HEXAGON_MMODE_GLOBAL_MAX = 96,

/** ISDB registers(12) **/
    HEXAGON_ISDBST = 96,   /* S32 */
    HEXAGON_ISDBCF0,       /* S33 */
    HEXAGON_ISDBCFG1,      /* S34 */
    HEXAGON_LIVELOCK,      /* S35 */
    HEXAGON_BRKPTPC0,      /* S36 */
    HEXAGON_BRKPTCFG0,     /* S37 */
    HEXAGON_BRKPTPC1,      /* S38 */
    HEXAGON_BRKPTCFG1,     /* S39 */
    HEXAGON_ISDBMBXIN,     /* S40 */
    HEXAGON_ISDBMBXOUT,    /* S41 */
    HEXAGON_ISDBEN,        /* S42 */
    HEXAGON_ISDBGPR,       /* S43 */

/** Perf Monitor registers(15) **/
    HEXAGON_PMUCNT4,       /* S44 */
    HEXAGON_PMUCNT5,       /* S45 */
    HEXAGON_PMUCNT6,       /* S46 */
    HEXAGON_PMUCNT7,       /* S47 */
    HEXAGON_PMUCNT0,       /* S48 */
    HEXAGON_PMUCNT1,       /* S49 */
    HEXAGON_PMUCNT2,       /* S50 */
    HEXAGON_PMUCNT3,       /* S51 */
    HEXAGON_PMUEVTCFG,     /* S52 */
    HEXAGON_PMUSTID0,      /* S53 */
    HEXAGON_PMUEVTCFG1,    /* S54 */
    HEXAGON_PMUSTID1,      /* S55 */   
    HEXAGON_TIMERLO,       /* S56 */
    HEXAGON_TIMERHI,       /* S57 */
    HEXAGON_PMUCFG1,       /* S58 */
    
    HEXAGON_S59_RESRV,     /* S59 */
    HEXAGON_S60_RESRV,     /* S60 */
    HEXAGON_S61_RESRV,     /* S61 */
    HEXAGON_S62_RESRV,     /* S62 */
    HEXAGON_S63_RESRV,     /* S63 */

    HEXAGON_LAST_REG = 128
};


/* Hexagon Registers info */
#define HEXAGON_GPR_REGS   HEXAGON_GPR_MAX
#define HEXAGON_PER_THREAD_CTRL_REGS (HEXAGON_MMODE_PERTHRD_MAX - HEXAGON_GPR_MAX)
#define HEXAGON_PER_THREAD_REGS HEXAGON_MMODE_PERTHRD_MAX
#define HEXAGON_GLOBAL_REGS (HEXAGON_MMODE_GLOBAL_MAX - HEXAGON_MMODE_PERTHRD_MAX)
#define HEXAGON_NUM_REGS HEXAGON_MMODE_GLOBAL_MAX

#define HEXAGON_PER_THREAD_RESV_REGS 15
#define HEXAGON_GLOBAL_RESV_REGS 3
#define HEXAGON_VALID_PER_THREAD_REGS (HEXAGON_MMODE_PERTHRD_MAX - HEXAGON_PER_THREAD_RESV_REGS)
#define HEXAGON_VALID_GLOBAL_REGS (HEXAGON_MMODE_GLOBAL_MAX-HEXAGON_MMODE_PERTHRD_MAX - HEXAGON_GLOBAL_RESV_REGS)


#define hexagon_pack_isdbcmd(cmd, prilvl, thrdmsk) ((cmd) | (prilvl) | (thrdmsk))


typedef struct {
    unsigned id;
    const char *name;
    unsigned bits;
    enum reg_type type;
    const char *group;
    const char *feature;
    struct reg_data_type *data_type;
} hexagon_reg;


#define HEXAGON_MAX_HW_BPS 2
#define HEXAGON_MAX_SW_BPS 8



/******************/
/******************/

#define PAGE_SIZE_4KB          0x1000
#define PAGE_SIZE_4KB_LEVEL0_BITS  39
#define PAGE_SIZE_4KB_LEVEL1_BITS  30
#define PAGE_SIZE_4KB_LEVEL2_BITS  21
#define PAGE_SIZE_4KB_LEVEL3_BITS  12

#define PAGE_SIZE_4KB_LEVEL0_MASK  ((0x1FFULL) << PAGE_SIZE_4KB_LEVEL0_BITS)
#define PAGE_SIZE_4KB_LEVEL1_MASK  ((0x1FFULL) << PAGE_SIZE_4KB_LEVEL1_BITS)
#define PAGE_SIZE_4KB_LEVEL2_MASK  ((0x1FFULL) << PAGE_SIZE_4KB_LEVEL2_BITS)
#define PAGE_SIZE_4KB_LEVEL3_MASK  ((0x1FFULL) << PAGE_SIZE_4KB_LEVEL3_BITS)

#define PAGE_SIZE_4KB_TRBBASE_MASK 0xFFFFFFFFF000


enum hexagon_opcode {
    READ_REG_R0,
    READ_REG_R1,
    READ_REG_R2,
    READ_REG_R3,
    READ_REG_R4,
    READ_REG_R5,
    READ_REG_R6,
    READ_REG_R7,
    READ_REG_R8,
    READ_REG_R9,
    READ_REG_R10,
    READ_REG_R11,
    READ_REG_R12,
    READ_REG_R13,
    READ_REG_R14,
    READ_REG_R15,
    READ_REG_R16,
    READ_REG_R17,
    READ_REG_R18,
    READ_REG_R19,
    READ_REG_R20,
    READ_REG_R21,
    READ_REG_R22,
    READ_REG_R23,
    READ_REG_R24,
    READ_REG_R25,
    READ_REG_R26,
    READ_REG_R27,
    READ_REG_R28,
    READ_REG_R29,
    READ_REG_R30,
    READ_REG_R31,
    BRKPT,
    HEXA_OPCODE_MAX
};




static const uint32_t hexagon_opcodes[HEXA_OPCODE_MAX] = {
        [READ_REG_R0]  = 0x6700c029,
        [READ_REG_R1]  = 0x6701c029,
        [READ_REG_R2]  = 0x6702c029,
        [READ_REG_R3]  = 0x6703c029,
        [READ_REG_R4]  = 0x6704c029,
        [READ_REG_R5]  = 0x6705c029,
        [READ_REG_R6]  = 0x6706c029,
        [READ_REG_R7]  = 0x6707c029,
        [READ_REG_R8]  = 0x6708c029,
        [READ_REG_R9]  = 0x6709c029,
        [READ_REG_R10] = 0x670ac029,
        [READ_REG_R11] = 0x670bc029,
        [READ_REG_R12] = 0x670cc029,
        [READ_REG_R13] = 0x670dc029,
        [READ_REG_R14] = 0x670ec029,
        [READ_REG_R15] = 0x670fc029,
        [READ_REG_R16] = 0x6710c029,
        [READ_REG_R17] = 0x6711c029,
        [READ_REG_R18] = 0x6712c029,
        [READ_REG_R19] = 0x6713c029,
        [READ_REG_R20] = 0x6714c029,
        [READ_REG_R21] = 0x6715c029,
        [READ_REG_R22] = 0x6716c029,
        [READ_REG_R23] = 0x6717c029,
        [READ_REG_R24] = 0x6718c029,
        [READ_REG_R25] = 0x6719c029,
        [READ_REG_R26] = 0x671ac029,
        [READ_REG_R27] = 0x671bc029,
        [READ_REG_R28] = 0x671cc029,
        [READ_REG_R29] = 0x671dc029,
        [READ_REG_R30] = 0x671ec029,
        [READ_REG_R31] = 0x671fc029,
        [BRKPT]            = 0x6c20c000,
};

/** All H/w threads on some debub reason ? **/
#define HEXA_DEBUG_ENTRY(x) (x & 0x7)

/* Cause of entry into debug mode */
#define HEXA_DBG_HWBRKPT0      (0x0)
#define HEXA_DBG_HWBRKPT1      (0x1)
#define HEXA_DBG_SWBRKPT       (0x2)
#define HEXA_DBG_ETMBRKPT      (0x3)
#define HEXA_DBG_APBBRKPT      (0x4)
#define HEXA_DBG_EXTBRKPT      (0x5)


struct hexagon_brp {
    int used;
    int type;
    target_addr_t value;
    uint32_t control;
    uint8_t BRPn;
};


enum hexagon_isrmasking_mode {
    HEXAGON_ISRMASK_OFF,
    HEXAGON_ISRMASK_ON,
};

union fourbyte {
  uint32_t word;
  uint16_t hword[2];
  uint8_t  byte[4];
};

//  forward declaration for usage in hexa_info
struct hexa_info;

struct hexa_reg {

    /** Invoke before a series of instruction operations */
    int (*prepare)( hexagon_reg *);

    /** Invoke after a series of instruction operations */
    int (*finish)( hexagon_reg *);

    /** Runs one instruction. */
    int (*instr_execute)( hexagon_reg *, uint32_t opcode);

    /** Runs one instruction, writing data to R0 before execution. */
    int (*instr_write_data_r0)( hexagon_reg *,
            uint32_t opcode, uint32_t data);


    /** Runs one instruction, reading data from r0 after execution. */
    int (*instr_read_data_r0)( hexagon_reg *,
            uint32_t opcode, uint32_t *data);


    struct reg *(*hexagon_reg_current)(struct hexa_info *hexa_info,
            unsigned int regnum, struct reg_cache *cache);
};


struct hexa_bp {
    unsigned number;
    uint32_t address;
    uint32_t control;
    /* true if hardware state needs flushing */
    bool dirty;
};


struct hexa_brkpt {
    /* BREAKPOINT SUPPORT */

    /**
     * Enables one breakpoint or watchpoint by writing to the
     * hardware registers.  The specified breakpoint/watchpoint
     * must currently be disabled.  Indices 0..15 are used for
     * breakpoints; indices 16..31 are for watchpoints.
     */
    int (*hwbp_enable)(struct hexa_brkpt *, unsigned index_value,
            uint32_t addr, uint32_t control);

    int (*swbp_enable)(struct hexa_brkpt *, unsigned index_value,
            uint32_t addr, uint32_t control);

    /**
     * Disables one breakpoint or watchpoint by clearing its
     * hardware control registers.  Indices are the same ones
     * accepted by bpwp_enable().
     */
    int (*hwbp_disable)(struct hexa_brkpt *, unsigned index_value);
    int (*swbp_disable)(struct hexa_brkpt *, unsigned index_value);

    /* The breakpoint and watchpoint arrays are private to the
     * DPM infrastructure.  There are nbp indices in the dbp
     * array.  There are nwp indices in the dwp array.
     */

    unsigned nhwbp;
    struct hexa_bp *dhwbp;
    unsigned nswbp;
    struct hexa_bp *dswbp;
};


struct hexagon_mmu_common {
    
    uint32_t mmu_enabled;
    uint32_t instrution_cache_enabled;
    uint32_t data_cache_enabled;
};

struct hexa_info {

    /* Hold the last read ISDB registers values */
    uint32_t isdb_ver;
    uint32_t corever;
    uint32_t isdb_enable;
    uint32_t isdb_status;
    uint32_t isdb_cstatus;
    uint32_t brkptinfo;

    /** Backpointer to the target. */
    struct target *target;

    /** pointer for arch **/
    void *arch_info;

    /** For targets conforming to ARM Debug Interface v5,
     * this handle references the Debug Access Port (DAP)
     * used to make requests to the target.
     */
    struct adiv5_dap *dap;
    // uint32_t debug_base; - mem_ap_read/writes all expect debug_base to be target_addr_t type
    target_addr_t debug_base;
    struct adiv5_ap *debug_ap;
    struct reg_cache *core_cache;

    /** Handle to the PC; valid in all core modes. */
    struct reg *pc;

    struct hexa_reg reg;

    /** Retrieve all Hw thread registers, for display. */
    int (*full_context)(struct target *target);

    /** Retrieve a single Hw thread  register. */
    int (*read_core_reg)(struct target *target, struct reg *r, int regnum, uint32_t hwthrd);
    int (*write_core_reg)(struct target *target, int regnum, uint32_t hwthrd, uint32_t value);
    const int *map;
    const uint32_t *opcodes;
    struct hexa_brkpt brkpt;
    struct hexagon_mmu_common hexagon_mmu;

    /* last run-control command issued to this target (resume, halt, step) */
    enum run_control_op last_run_control_op;
};


struct hexagon_common {

    struct hexa_info hexa_info;
    int common_magic;
    /* Context information */
    uint32_t system_control_reg;
    uint32_t system_control_reg_curr;
    /* Breakpoint register pairs */
    int brp_num_context;
    int brp_num;
    int brp_num_available;
    struct hexagon_brp *brp_list;
    enum hexagon_isrmasking_mode isrmasking_mode;
};

struct hex_reg {
    int num;
    uint32_t hwthrd;
    struct target *target;
    struct hexa_info *hexa_info;
    uint8_t value[16];
};
/*
 * private target configuration items
 */





static inline struct hexagon_common *
target_to_hexagon(struct target *target)
{
    return container_of(target->arch_info, struct hexagon_common, hexa_info);
}

/* this enum for page  size */
enum hexagon_page_size
{
    HEXAGON_TLB_PAGE_SIZE_4KB,
    HEXAGON_TLB_PAGE_SIZE_16KB,
    HEXAGON_TLB_PAGE_SIZE_64KB,
    HEXAGON_TLB_PAGE_SIZE_256KB,
    HEXAGON_TLB_PAGE_SIZE_1MB,
    HEXAGON_TLB_PAGE_SIZE_4MB,
    HEXAGON_TLB_PAGE_SIZE_16MB,
};

/* data structure contains the data of TLB entry parsing*/

typedef struct
{
    uint64_t phy_add_low;
    uint64_t phy_add_high;
    uint32_t virt_add_low;
    uint32_t virt_add_high;
    uint32_t virt_tlb_raw_data;
    uint32_t phys_tlb_raw_data;
    uint32_t virt_page : 20;
    uint32_t phy_page  : 24;
    enum hexagon_page_size page_size;
    uint8_t asid : 7;
    uint8_t CCCC : 4;
    uint8_t validbit  : 1;
    uint8_t globalbit : 1;
    uint8_t EP : 1;
    uint8_t A0 : 1;
    uint8_t A1 : 1;
    uint8_t S  : 1;
    uint8_t R  : 1;
    uint8_t W  : 1;
    uint8_t X  : 1;
    uint8_t U  : 1;
} tlb_entries;

#ifdef HEXAGON_VTLB_NEW_ARCH
typedef struct    
{
     unsigned vtlb_version:4;                  // version info
     unsigned vtlb_table_type:3;               // Descriptor type
     unsigned next_table_addr_is_virtual:1;    // next element is virtual or physical
     unsigned long long next_table_addr:36;    // table address where the descriptor type elements start from
     unsigned table_entries:20;                // Number of entries of the descriptor type
} qurtk_vtlb_table_descriptor;
#endif

/* data structure contains the data related to QURTK_vtlb_main for fetching vtlb entries */
typedef struct 
{
    uint32_t vtlb_previous_counter;
    uint32_t vtlb_current_counter;
    uint32_t vtlb_no_of_entries;
    uint32_t vtlb_no_of_entries1;
    uint32_t valid_vtlb_no_of_entries;
    uint32_t QURTK_vtlb_main_VA;
    uint32_t QURTK_vtlb_main_PA; 
    uint32_t QURTK_VTLB_DATA_VA;
    uint32_t QURTK_VTLB_DATA_PA;
    #ifdef HEXAGON_VTLB_NEW_ARCH
    qurtk_vtlb_table_descriptor qurtk_vtlb_main;
    qurtk_vtlb_table_descriptor qurtk_vtlb_main_next;
    #endif
   
}vtlb_data;

/* data structure for Virtual Page entry parsing */
union pg_tlbhi_t{
    struct{
        unsigned int vir_addr   :20;
        unsigned int asid    :7;
        unsigned int a0      :1;
        unsigned int a1      :1;
        unsigned int ep      :1;
        unsigned int global  :1;
        unsigned int valid   :1;
    } info;
    unsigned int raw;
};

/* data structure for Physical Page entry parsing */
union pg_tlblo_t{
    struct{
        unsigned int phys_addr  :24;
        unsigned int cache      :4;
        unsigned int usr        :1;
        unsigned int perm       :3;
    } info;
    unsigned int raw;
};
/*
 * hexagon-untrusted
 * Data structures and variable are included here
 */
#define ISDBMAILBOX_SIZE 4
#define UNTRUSTED_PROTOCOL_VERSION 1
typedef struct {
    bool has_packet;
    bool has_checksum;
    bool has_rsp;
    char current_packet[1024];
    char current_checksum[2];
    uint16_t packet_len;
    bool header_sent;
    uint32_t current_process_id;
    uint32_t process_id_list[10];
    uint32_t current_session_id;
    struct target *target;
} untrusted_mode;

typedef struct {
    uint8_t protocol;
    uint8_t header_len;
    uint16_t payload_len;
} untrusted_essential;

typedef struct {
    uint32_t process_id;
    uint32_t rsp_ver;
    uint32_t session_id;
} untrusted_header;

typedef struct{
    uint16_t len;
    uint16_t bytes_to_read;
    uint32_t *payload;
} untrusted_payload;

typedef struct{
    char process_name[128];
    uint16_t pid;
} process_pd;
/*
hexagon-Untrusted mode function prototypes
*/
int hexagon_untrusted_update_packet(char *packet, int len);
int hexagon_untrusted_update_packet_checksum(char *checksum);
int hexagon_untrusted_forward_rsp(struct target *target, char *response, uint16_t *response_len);

#endif  /* OPENOCD_TARGET_HEXAGON_H */
