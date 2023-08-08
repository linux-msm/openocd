/**************************************************************************
*	Copyright (c) 2023 Qualcomm Innovation Center, Inc.                   *
*   All rights reserved.                                                  * 
*   SPDX-License-Identifier: GPL-2.0-or-later                             * 
*																		  *
***************************************************************************/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "breakpoints.h"
#include "aarch64.h"
#include "register.h"
#include "target_request.h"
#include "target_type.h"
#include "armv8_opcodes.h"
#include "armv8_cache.h"
#include "arm_semihosting.h"
#include "jtag/interface.h"
#include "smp.h"
#include <helper/time_support.h>
#include "arm_adi_v5.h"
#include "jtag/swd.h"
#include "hexagon.h"
# include<time.h>

clock_t start, end, func_start, func_end;
clock_t start_buffer, end_buffer;

#define ClkEnRegs 8

double execution_time=0;
double buffer_execution=0;

extern uint64_t thread_id_thread_select;
extern uint64_t breakpoint_address_thread_select;
extern int is_spurious_breakpoint;
uint64_t loop_count = 1000;
static uint8_t vtlb_initialized = 0;

/*	 
To facilitate any subsystem specific debug enablement 
such as enable clocks for ADSP debug
*/
current_hexagon_target  current_hexagon_subsystem = {
	.hexagon_adsp = false,
	.hexagon_mpss = false,
	.hexagon_cdsp = false,
	.hexagon = false,
};


static bool mmu_init=false;
// static bool vtlb_enabled_cdsp=false;
static bool multi_thr_enabled=false;
/****************************Global /static variable declartions**************************/

//  mem_ap_read_atomic_u32 requires uint32_t
uint32_t hvx_register[32][16];
process_pd process_list[4];
untrusted_mode hexagon_untrusted_current_state;
//  first defined here
bool is_hexagon_untrusted = false;
static int spid;

uint32_t qurtk_vtlb_main_addr = 0x0; 
target_addr_t qurtk_vtlb_entries = 0x0; 
target_addr_t bitmap_addr ;
target_addr_t QURTK_vtlb_revision=0x0;
// target_addr_t vtlb_entries ;
bool bitmap_init=false;
extern void decToBinary(unsigned int n, unsigned int binaryNum[]);
int hexagon_update_modified_vtlb_entry(struct target *target);

Q6_global Q6_global_settings ={
	.selected_process= -1,
	.total_process = 0,
	.refresh_indicator = 0x0,
	.revision_num = 0x0,
	.hexagon_debug_process_id = 0x2,
	.qurtk_vtlb_main_addr = 0x0,
	.qurtk_vtlb_entries = 0x0,
	// .bitmap_array_addr = 0x0,
	.QURTK_vtlb_revision = 0x0,
    .bitmap_addr = 0x0,
};

typedef struct hexagon_config
{
    uint32_t maxHwThreads;
    uint32_t qpss6WDOGCtl;
    uint32_t numTlbEntries;
	uint32_t clkEnAddr[8];
	uint32_t etmClkenAddr;
	uint32_t etmResetAddr;
} hexagon_config;

hexagon_config gHexConfig =
{
		.maxHwThreads = 1,
		.qpss6WDOGCtl = 0x4104004,
		.numTlbEntries = 192,
		.etmClkenAddr = 0x86988000,
		.etmResetAddr = 0x86988008,

		.clkEnAddr[0] = 0x147004,  //GCC_LPASS_CFG_NOC_SWAY_CBCR
		.clkEnAddr[1] = 0x147010,  //GCC_LPASS_AT_CBCR
		.clkEnAddr[2] = 0x338901C, //LPASS_Q6_AHBM_CBCR
		.clkEnAddr[3] = 0x3000038, //LPASS_QDSP6SS_XO_CBCR
		.clkEnAddr[4] = 0x338a00c, //LPASS_AT_CBCR
		.clkEnAddr[5] = 0x338a014, //LPASS_PCLKDBG_CBCR
		.clkEnAddr[6] = 0x3000018, //LPASS_QDSP6SS_DBG_CFG
		.clkEnAddr[7] = 0x338A058, //LPASS_DBG_TSCTR_CBCR

};
uint32_t (*gpPerHwThrdReg)[HEXAGON_PER_THREAD_REGS] = {0};
static uint32_t **gpSbpHaltedThreadsPC;
char (*gpHexagonThreadNameArray)[20];

tlb_entries *gpHexagonTlbEntries;
uint64_t qurtk_vtlb_bitmap= 0x0; 


static int initConfig(hexagon_config *pHexCfg)
{
    uint32_t i;
    char a = '0';

    LOG_INFO("InitConfig is called here");

	gpHexagonThreadNameArray = malloc((pHexCfg->maxHwThreads + 1) * sizeof(*gpHexagonThreadNameArray));
	gpPerHwThrdReg = (uint32_t(*)[HEXAGON_PER_THREAD_REGS])malloc(pHexCfg->maxHwThreads * sizeof(*gpPerHwThrdReg));
	// since we need array of pointers, we need size of pointer not integer
	gpSbpHaltedThreadsPC = (uint32_t **)malloc(pHexCfg->maxHwThreads * sizeof(uint32_t *));
	gpHexagonTlbEntries = (tlb_entries *)malloc(pHexCfg->numTlbEntries * sizeof(tlb_entries));
	
	if (gpHexagonThreadNameArray == NULL || gpPerHwThrdReg == NULL || gpSbpHaltedThreadsPC == NULL || gpHexagonTlbEntries == NULL)
	{
		if (gpHexagonThreadNameArray != NULL)
			free(gpHexagonThreadNameArray);

		if (gpPerHwThrdReg != NULL)
			free(gpPerHwThrdReg);

		if (gpSbpHaltedThreadsPC != NULL)
			free(gpSbpHaltedThreadsPC);
		if (gpHexagonTlbEntries != NULL)
				free(gpHexagonTlbEntries);
		return ERROR_FAIL;
	}
	for (i = 0; i < pHexCfg->maxHwThreads; i++, a++)
	{
		strcpy(gpHexagonThreadNameArray[i], "HW-Thrd-");
		gpHexagonThreadNameArray[i][8] = a;
		gpHexagonThreadNameArray[i][9] = '\0';
	}
	strcpy(gpHexagonThreadNameArray[i], "GLOBAL");

	memset(gpSbpHaltedThreadsPC, 0, (pHexCfg->maxHwThreads * sizeof(uint32_t *)));
	memset(gpPerHwThrdReg, 0, (pHexCfg->maxHwThreads * sizeof(*gpPerHwThrdReg)));
	memset(gpHexagonTlbEntries, 0, (pHexCfg->numTlbEntries * sizeof(tlb_entries)));

	return ERROR_OK;
}

static void deinitConfig(hexagon_config *pHexCfg)
{
	if (gpHexagonThreadNameArray != NULL)
		free(gpHexagonThreadNameArray);

	if (gpPerHwThrdReg != NULL)
		free(gpPerHwThrdReg);

	if (gpSbpHaltedThreadsPC != NULL)
		free(gpSbpHaltedThreadsPC);

	if (gpHexagonTlbEntries != NULL)
		free(gpHexagonTlbEntries);

	return;
}

static const hexagon_reg hexagon_per_hwt_regs[] = {
/** General Purpose Registers **/ 
	{ HEXAGON_R0,  		"R0",  		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R1,  		"R1",  		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R2,  		"R2",  		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R3,  		"R3",  		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R4,  		"R4",  		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R5,  		"R5",  		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R6,  		"R6",  		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R7,  		"R7",  		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R8,  		"R8",  		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R9,  		"R9",  		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R10, 		"R10", 		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R11, 		"R11", 		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R12, 		"R12", 		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R13, 		"R13", 		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R14, 		"R14", 		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R15, 		"R15", 		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R16, 		"R16", 		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R17, 		"R17", 		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R18, 		"R18", 		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R19, 		"R19", 		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R20, 		"R20", 		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R21, 		"R21", 		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R22, 		"R22", 		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R23, 		"R23", 		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R24, 		"R24", 		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R25, 		"R25", 		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R26, 		"R26", 		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R27, 		"R27", 		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_R28, 		"R28",	 	32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_SP, 		"R29", 		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_FP, 		"R30",	 	32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
    	{ HEXAGON_LR, 		"R31", 		32, REG_TYPE_UINT32, "general", "org.gnu.gdb.hexagon.core", NULL},
/** Control Registers **/
	{ HEXAGON_SA0, 		"SA0", 		32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_LC0, 		"LC0", 		32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_SA1,  	"SA1",  	32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_LC1,  	"LC1", 	 	32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_P30,  	"P3:0",  	32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
/** C5: reserved **/
	{ HEXAGON_C5_RESRV, "C5_RESRV",	32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_M0, 		"M0",  		32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_M1,  		"M1",  		32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_USR,  	"USR",  	32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_PC,  		"PC",  		32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_UGP,  	"UGP",  	32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_GP,  		"GP",  		32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_CS0,  	"CS0",  	32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_CS1,  	"CS1",  	32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_UPCYCLLO,"UPCYCLELO", 32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_UPCYCLHI,"UPCYCLEHI", 32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_FLMT, 	"FLMT", 	32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_FKEY, 	"FKEY", 	32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_PKTCNTLO, "PKTCNTLO", 32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_PKTCNTHI, "PKTCNTHI", 32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
/** C20-29: reserved **/
	{ HEXAGON_C20_RESRV,"C5_RESRV",	32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_C21_RESRV,"C5_RESRV",	32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_C22_RESRV,"C5_RESRV",	32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_C23_RESRV,"C5_RESRV",	32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_C24_RESRV,"C5_RESRV",	32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_C25_RESRV,"C5_RESRV",	32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_C26_RESRV,"C5_RESRV",	32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_C27_RESRV,"C5_RESRV",	32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_C28_RESRV,"C5_RESRV",	32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_C29_RESRV,"C5_RESRV",	32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_UTMRLO, 	"UTMRLO", 	32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_UTMRHI, 	"UTMRHI", 	32, REG_TYPE_UINT32, "control", "org.gnu.gdb.hexagon.core", NULL},
/** Monitor Mode per-thread Control Registers **/	
	{ HEXAGON_SGP0, 	"SGP0", 	32, REG_TYPE_UINT32, "monitor", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_SGP1, 	"SGP1", 	32, REG_TYPE_UINT32, "monitor", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_STID, 	"STID", 	32, REG_TYPE_UINT32, "monitor", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_ELR,		"ELR",		32, REG_TYPE_UINT32, "monitor", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_BADVA0,	"BADVA0",	32, REG_TYPE_UINT32, "monitor", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_BADVA1,	"BADVA1",	32, REG_TYPE_UINT32, "monitor", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_SSR, 		"SSR", 		32, REG_TYPE_UINT32, "monitor", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_CCR, 		"CCR", 		32, REG_TYPE_UINT32, "monitor", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_HTID, 	"HTID", 	32, REG_TYPE_UINT32, "monitor", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_BADVA,	"BADVA",	32, REG_TYPE_UINT32, "monitor", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_IMASK, 	"IMASK", 	32, REG_TYPE_UINT32, "monitor", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_GEVB, 	"GEVB", 	32, REG_TYPE_UINT32, "monitor", "org.gnu.gdb.hexagon.core", NULL},
/** S12 - S15: reserved **/
	{ HEXAGON_S12_RESRV,"S12_RESRV",32, REG_TYPE_UINT32, "monitor", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_S13_RESRV,"S13_RESRV",32, REG_TYPE_UINT32, "monitor", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_S14_RESRV,"S14_RESRV",32, REG_TYPE_UINT32, "monitor", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_S15_RESRV,"S15_RESRV",32, REG_TYPE_UINT32, "monitor", "org.gnu.gdb.hexagon.core", NULL},


};

uint32_t global_reg[HEXAGON_MMODE_GLOBAL_MAX-HEXAGON_MMODE_PERTHRD_MAX]={0};


static const hexagon_reg hexagon_global_regs[] = {
/** Monitor Mode Global Control Registers **/
	{ HEXAGON_EVB, 		"EVB", 		32, REG_TYPE_UINT32, "global", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_MODECTL,	"MODECTL",	32, REG_TYPE_UINT32, "global", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_SYSCFG, 	"SYSCFG", 	32, REG_TYPE_UINT32, "global", "org.gnu.gdb.hexagon.core", NULL},
/** 19: reserved **/
	{ HEXAGON_S19_RESRV,"S19_RESRV",32, REG_TYPE_UINT32, "global", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_IPENDAD,	"IPENDAD",	32, REG_TYPE_UINT32, "global", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_VID,		"VID",		32, REG_TYPE_UINT32, "global", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_VID1, 	"VID1", 	32, REG_TYPE_UINT32, "global", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_BESTWAIT, "BESTWAIT", 32, REG_TYPE_UINT32, "global", "org.gnu.gdb.hexagon.core", NULL},
/** 24: reserved **/
	{ HEXAGON_S24_RESRV,"S24_RESRV",32, REG_TYPE_UINT32, "global", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_SCHDCFG,	"SCHDCFG",	32, REG_TYPE_UINT32, "global", "org.gnu.gdb.hexagon.core", NULL},
/** 26: reserved **/	
	{ HEXAGON_S26_RESRV,"S26_RESRV",32, REG_TYPE_UINT32, "global", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_CFGBASE,  "CFGBASE", 	32, REG_TYPE_UINT32, "global", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_DIAG, 	"DIAG", 	32, REG_TYPE_UINT32, "global", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_REV, 		"REV", 		32, REG_TYPE_UINT32, "global", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_PCYCLELO, "PCYCLELO", 32, REG_TYPE_UINT32, "global", "org.gnu.gdb.hexagon.core", NULL},
	{ HEXAGON_PCYCLEHI, "PCYCLEHI", 32, REG_TYPE_UINT32, "global", "org.gnu.gdb.hexagon.core", NULL},
};

/********************************************/
/** Stuff instruction for register reading **/
/********************************************/


/* R0-R31  registers */
uint32_t stuff_inst_gpr_read[] = {
	    0x6700c029,	/* isdbmbxout = r0  */
		0x6701c029,	/* isdbmbxout = r1  */
		0x6702c029,	/* isdbmbxout = r2  */
		0x6703c029,	/* isdbmbxout = r3  */
		0x6704c029,	/* isdbmbxout = r4  */
		0x6705c029,	/* isdbmbxout = r5  */
		0x6706c029,	/* isdbmbxout = r6  */
		0x6707c029,	/* isdbmbxout = r7  */
		0x6708c029,	/* isdbmbxout = r8  */
		0x6709c029,	/* isdbmbxout = r9  */
		0x670ac029,	/* isdbmbxout = r10 */
		0x670bc029,	/* isdbmbxout = r11 */
		0x670cc029,	/* isdbmbxout = r12 */
		0x670dc029,	/* isdbmbxout = r13 */
		0x670ec029,	/* isdbmbxout = r14 */
		0x670fc029,	/* isdbmbxout = r15 */
		0x6710c029,	/* isdbmbxout = r16 */
		0x6711c029,	/* isdbmbxout = r17 */
		0x6712c029,	/* isdbmbxout = r18 */
		0x6713c029,	/* isdbmbxout = r19 */
		0x6714c029,	/* isdbmbxout = r20 */
		0x6715c029,	/* isdbmbxout = r21 */
		0x6716c029,	/* isdbmbxout = r22 */
		0x6717c029,	/* isdbmbxout = r23 */
		0x6718c029,	/* isdbmbxout = r24 */
		0x6719c029,	/* isdbmbxout = r25 */
		0x671ac029,	/* isdbmbxout = r26 */
		0x671bc029,	/* isdbmbxout = r27 */
		0x671cc029,	/* isdbmbxout = r28 */
		0x671dc029,	/* isdbmbxout = r29 */
		0x671ec029,	/* isdbmbxout = r30 */
		0x671fc029  /* isdbmbxout = r31 */
};

/* Control registers */
uint32_t stuff_inst_ctrl_reg_read[][2] =  {
		{0x6a00c007,0x6707c029},	/* r7 = sa0, isdbmbxout = r7 */
		{0x6a01c007,0x6707c029},	/* r7 = lc0, isdbmbxout = r7 */
		{0x6a02c007,0x6707c029}, 	/* r7 = sa1, isdbmbxout = r7 */
		{0x6a03c007,0x6707c029},	/* r7 = lc1, isdbmbxout = r7 */
		{0x6a04c007,0x6707c029}, 	/* r7 = p3:0 isdbmbxout = r7 */
		{0x0, 0x0},					/* Invalid: C5 reserved 	 */
		{0x6a06c007,0x6707c029},	/* r7 = m0,  isdbmbxout = r7 */
		{0x6a07c007,0x6707c029}, 	/* r7 = m1,  isdbmbxout = r7 */
		{0x6a08c007,0x6707c029},	/* r7 = usr, isdbmbxout = r7 */
		{0x6a09c007,0x6707c029},	/* r7 = pc,  isdbmbxout = r7 */ 
		{0x6a0ac007,0x6707c029},	/* r7 = ugp, isdbmbxout = r7 */
		{0x6a0bc007,0x6707c029}, 	/* r7 = gp,  isdbmbxout = r7 */
		{0x6a0cc007,0x6707c029},	/* r7 = cs0, isdbmbxout = r7 */
		{0x6a0dc007,0x6707c029}, 	/* r7 = cs1, isdbmbxout = r7 */
		{0x6a0ec007,0x6707c029}, 	/* r7 = upcyclelo, isdbmbxout = r7 */
		{0x6a0fc007,0x6707c029}, 	/* r7 = upcyclehi, isdbmbxout = r7 */
		{0x6a10c007,0x6707c029}, 	/* r7 = framelimit,isdbmbxout = r7 */
		{0x6a11c007,0x6707c029},  	/* r7 = framekey,  isdbmbxout = r7 */
		{0x6a12c007,0x6707c029},	/* r7 = pktcountlo,isdbmbxout = r7 */	
		{0x6a13c007,0x6707c029}, 	/* r7 = pktcounthi,isdbmbxout = r7 */
		{0x0, 0x0}, 				/* Invalid: C20 reserved 	 */
		{0x0, 0x0}, 				/* Invalid: C21 reserved 	 */
		{0x0, 0x0}, 				/* Invalid: C22 reserved 	 */
		{0x0, 0x0}, 				/* Invalid: C23 reserved 	 */
		{0x0, 0x0}, 				/* Invalid: C24 reserved	 */
		{0x0, 0x0}, 				/* Invalid: C25 reserved	 */
		{0x0, 0x0}, 				/* Invalid: C26 reserved	 */
		{0x0, 0x0}, 				/* Invalid: C27 reserved	 */
		{0x0, 0x0}, 				/* Invalid: C28 reserved	 */
		{0x0, 0x0}, 				/* Invalid: C29 reserved	 */
		{0x6a1ec007,0x6707c029},  	/* r7 = utimerlo,  isdbmbxout = r7 */
		{0x6a1fc007,0x6707c029} 	/* r7 = utimerhi,  isdbmbxout = r7 */
};

/* Per thread control registers */
uint32_t stuff_inst_mmode_reg_read[][2] = { 
		{0x6e80c007,0x6707c029},	/* r7 = sgp0, isdbmbxout = r7  */
		{0x6e81c007,0x6707c029},	/* r7 = sgp1, isdbmbxout = r7  */
		{0x6e82c007,0x6707c029}, 	/* r7 = stid, isdbmbxout = r7  */
		{0x6e83c007,0x6707c029},	/* r7 = elr,  isdbmbxout = r7  */
		{0x6e84c007,0x6707c029}, 	/* r7 = badva0,isdbmbxout = r7 */
		{0x6e85c007,0x6707c029},	/* r7 = badva1,isdbmbxout = r7 */
		{0x6e86c007,0x6707c029}, 	/* r7 = ssr,  isdbmbxout = r7  */
		{0x6e87c007,0x6707c029},	/* r7 = ccr,  isdbmbxout = r7  */
		{0x6e88c007,0x6707c029}, 	/* r7 = htid, isdbmbxout = r7  */
		{0x6e89c007,0x6707c029},	/* r7 = badva,isdbmbxout = r7  */
		{0x6e8ac007,0x6707c029}, 	/* r7 = imask,isdbmbxout = r7  */
		{0x6e8bc007,0x6707c029},	/* r7 = gevb, isdbmbxout = r7  */
		{0x0, 0x0}, 			/* Invalid: S12 reserved */
		{0x0, 0x0}, 			/* Invalid: S13 reserved */
		{0x0, 0x0}, 			/* Invalid: S14 reserved */
		{0x0, 0x0}, 			/* Invalid: S15 reserved */		
};

 uint32_t stuff_inst_mmode_imask_reg_read[][3] = {
	 {0x7800c027,0x6607c007,0x6707c029}, /* r7 = #1, r7 = getimask(r7), isdbmbxout = r7 */
	 {0x7800c047,0x6607c007,0x6707c029}, /* r7 = #2, r7 = getimask(r7), isdbmbxout = r7 */
	 {0x7800c087,0x6607c007,0x6707c029}, /* r7 = #4, r7 = getimask(r7), isdbmbxout = r7 */
	 {0x7800c107,0x6607c007,0x6707c029}, /* r7 = #8, r7 = getimask(r7), isdbmbxout = r7 */
	 {0x7800c207,0x6607c007,0x6707c029}, /* r7 = #16,r7 = getimask(r7), isdbmbxout = r7 */
	 {0x7800c407,0x6607c007,0x6707c029}  /* r7 = #32,r7 = getimask(r7), isdbmbxout = r7 */

}; 

/* Global control registers */
uint32_t stuff_inst_global_reg_read[][2] = {
		{0x6e90c007,0x6707c029}, /* r7 = evb, isdbmbxout = r7 */	
		{0x6e91c007,0x6707c029}, /* r7 = modectl, isdbmbxout = r7 */	
		{0x6e92c007,0x6707c029}, /* r7 = syscfg, isdbmbxout = r7 */	
		{0x0, 0x0}, 			 /* Invalid: S19 reserved	  */			
		{0x6e94c007,0x6707c029}, /* r7 = ipendad, isdbmbxout = r7 */
		{0x6e95c007,0x6707c029}, /* r7 = vid, isdbmbxout = r7 */	
		{0x6e96c007,0x6707c029}, /* r7 = vid1, isdbmbxout = r7 */	
		{0x6e97c007,0x6707c029}, /* r7 = bestwait, isdbmbxout = r7 */
		{0x0, 0x0}, 			 /* Invalid: S24 reserved	  */			
		{0x6e99c007,0x6707c029}, /* r7 = schedcfg, isdbmbxout = r7 */	
		{0x0, 0x0}, 			 /* Invalid: S26 reserved	  */			
		{0x6e9bc007,0x6707c029}, /* r7 = cfgbase, isdbmbxout = r7 */	
		{0x6e9cc007,0x6707c029}, /* r7 = diag, isdbmbxout = r7 */	
		{0x6e9dc007,0x6707c029}, /* r7 = rev, isdbmbxout = r7 */	
		{0x6e9ec007,0x6707c029}, /* r7 = pcyclelo, isdbmbxout = r7 */	
		{0x6e9fc007,0x6707c029}	 /* r7 = pcyclehi, isdbmbxout = r7 */	
};
	


uint32_t stuff_inst_sfr[][2] = 
		{{0x7800c027,0x6607c007},
		 {0x7800c047,0x6607c007},
		 {0x7800c087,0x6607c007},
		 {0x7800c107,0x6607c007},
		 {0x7800c207,0x6607c007},
		 {0x7800c407,0x6607c007},
		 {0x6a0ac007,0x6707c029},
		 {0x6a0bc007,0x6707c029},
		 {0x6e86c007,0x6707c029},
		 {0x6e84c007,0x6707c029},
		 {0x6e85c007,0x6707c029},
		 {0x6e92c007,0x6707c029},
		 {0x6e91c007,0x6707c029},
		 {0x6e9ec007,0x6707c029},
		 {0x6e9fc007,0x6707c029},
		 {0x6e90c007,0x6707c029},
		 {0x6e9cc007,0x6707c029},
		 {0x6e94c007,0x6707c029},
		 {0x6e9dc007,0x6707c029},
		 {0x6e9bc007,0x6707c029}};

/********************************************/
/** Stuff instruction for register writing **/
/********************************************/


/* R0-R31  registers */
uint32_t stuff_inst_gpr_write[] = {
	    	0x6ea8c000,	/* r0  = isdbmbxin */
		0x6ea8c001,	/* r1  = isdbmbxin */
		0x6ea8c002,	/* r2  = isdbmbxin */
	    	0x6ea8c003,	/* r3  = isdbmbxin */
		0x6ea8c004,	/* r4  = isdbmbxin */
		0x6ea8c005,	/* r5  = isdbmbxin */
	    	0x6ea8c006,	/* r6  = isdbmbxin */
		0x6ea8c007,	/* r7  = isdbmbxin */
		0x6ea8c008,	/* r8  = isdbmbxin */
	    	0x6ea8c009,	/* r9  = isdbmbxin */
		0x6ea8c00A,	/* r10 = isdbmbxin */
		0x6ea8c00B,	/* r11 = isdbmbxin */
	    	0x6ea8c00C,	/* r12 = isdbmbxin */
		0x6ea8c00D,	/* r13 = isdbmbxin */
		0x6ea8c00E,	/* r14 = isdbmbxin */
	    	0x6ea8c00F,	/* r15 = isdbmbxin */
		0x6ea8c010,	/* r16 = isdbmbxin */
		0x6ea8c011,	/* r17 = isdbmbxin */
		0x6ea8c012,	/* r18 = isdbmbxin */
		0x6ea8c013,	/* r19 = isdbmbxin */
		0x6ea8c014,	/* r20 = isdbmbxin */
		0x6ea8c015,	/* r21 = isdbmbxin */		
		0x6ea8c016,	/* r22 = isdbmbxin */
		0x6ea8c017,	/* r23 = isdbmbxin */
		0x6ea8c018,	/* r24 = isdbmbxin */
		0x6ea8c019,	/* r25 = isdbmbxin */		
		0x6ea8c01A,	/* r26 = isdbmbxin */
		0x6ea8c01B,	/* r27 = isdbmbxin */		
		0x6ea8c01C,	/* r28 = isdbmbxin */
		0x6ea8c01D,	/* r29 = isdbmbxin */
		0x6ea8c01E,	/* r30 = isdbmbxin */
		0x6ea8c01F,	/* r31 = isdbmbxin */	
};


/* Control registers */
uint32_t stuff_inst_ctrl_reg_write[][2] =  {
	{0x6ea8c007, 0x6227c000},	/* r7 = isdbmbxin, sa0 = r7 */
	{0x6ea8c007, 0x6227c001},	/* r7 = isdbmbxin, lc0 = r7 */
	{0x6ea8c007, 0x6227c002}, 	/* r7 = isdbmbxin, sa1 = r7 */
	{0x6ea8c007, 0x6227c003}, 	/* r7 = isdbmbxin, lc1 = r7 */
	{0x6ea8c007, 0x6227c004}, 	/* r7 = isdbmbxin, p3:0 = r7*/
	{0x0, 0x0}, 				/* Invalid: C5 reserved */
	{0x6ea8c007, 0x6227c006}, 	/* r7 = isdbmbxin, m0 = r7  */
	{0x6ea8c007, 0x6227c007}, 	/* r7 = isdbmbxin, m1 = r7  */
	{0x6ea8c007, 0x6227c008}, 	/* r7 = isdbmbxin, usr = r7 */
	{0x6ea8c007, 0x5287c000},	/* r7 = isdbmbxin, jump r7 (PC=r7)  */ 
	{0x6ea8c007, 0x6227c00a}, 	/* r7 = isdbmbxin, ugp = r7 */
	{0x6ea8c007, 0x6227c00b}, 	/* r7 = isdbmbxin, gp = r7  */
	{0x6ea8c007, 0x6227c00c}, 	/* r7 = isdbmbxin, cs0 = r7 */
	{0x6ea8c007, 0x6227c00d}, 	/* r7 = isdbmbxin, cs1 = r7 */
	{0x6ea8c007, 0x6227c00e}, 	/* r7 = isdbmbxin, upcyclelo = r7 */
	{0x6ea8c007, 0x6227c00f}, 	/* r7 = isdbmbxin, upcyclehi = r7 */
	{0x6ea8c007, 0x6227c010}, 	/* r7 = isdbmbxin, framelimit= r7 */
	{0x6ea8c007, 0x6227c011}, 	/* r7 = isdbmbxin, framekey = r7 */
	{0x6ea8c007, 0x6227c012},	/* r7 = isdbmbxin, pktcountlo = r7 */
	{0x6ea8c007, 0x6227c013}, 	/* r7 = isdbmbxin, pktcounthi = r7 */
	{0x0, 0x0}, 				/* Invalid: C20 reserved 	 */
	{0x0, 0x0}, 				/* Invalid: C21 reserved 	 */
	{0x0, 0x0}, 				/* Invalid: C22 reserved 	 */
	{0x0, 0x0}, 				/* Invalid: C23 reserved 	 */
	{0x0, 0x0}, 				/* Invalid: C24 reserved	 */
	{0x0, 0x0}, 				/* Invalid: C25 reserved	 */
	{0x0, 0x0}, 				/* Invalid: C26 reserved	 */
	{0x0, 0x0}, 				/* Invalid: C27 reserved	 */
	{0x0, 0x0}, 				/* Invalid: C28 reserved	 */
	{0x0, 0x0}, 				/* Invalid: C29 reserved	 */
	{0x6ea8c007, 0x6227c01e}, 	/* r7 = isdbmbxin, utimerlo = r7  */
	{0x6ea8c007, 0x6227c01f}, 	/* r7 = isdbmbxin, utimerhi = r7 */
};


/* Per thread control registers */
uint32_t stuff_inst_mmode_reg_write[][2] = { 
	{0x6ea8c007, 0x6707c000}, 	/* r7 = isdbmbxin, sgp0 = r7 */
	{0x6ea8c007, 0x6707c001}, 	/* r7 = isdbmbxin, sgp1 = r7 */
	{0x6ea8c007, 0x6707c002},	/* r7 = isdbmbxin, stid = r7 */
	{0x6ea8c007, 0x6707c003}, 	/* r7 = isdbmbxin, elr = r7  */
	{0x6ea8c007, 0x6707c004}, 	/* r7 = isdbmbxin, badva0 = r7 */
	{0x6ea8c007, 0x6707c005},	/* r7 = isdbmbxin, badva1 = r7 */
	{0x6ea8c007, 0x6707c006}, 	/* r7 = isdbmbxin, ssr = r7 */
	{0x6ea8c007, 0x6707c007}, 	/* r7 = isdbmbxin, ccr = r7 */
	{0x0, 0x0}, 				/* Invalid: S12 reserved */
	{0x0, 0x0}, 				/* Invalid: S13 reserved */
	{0x0, 0x0}, 				/* Invalid: S14 reserved */
	{0x0, 0x0}, 				/* Invalid: S15 reserved */	
};


/* Per thread control registers */
uint32_t stuff_inst_global_reg_write[][2] = { 
	{0x6ea8c007, 0x6707c010},   /* r7 = isdbmbxin, evb = r7 */		
	{0x0, 0x0}, 				/* modectl: read only */
	{0x6ea8c007, 0x6707c012},   /* r7 = isdbmbxin, syscfg = r7 */	
	{0x0, 0x0}, 				/* Invalid: S19 reserved */
	{0x0, 0x0}, 				/* ipendad: read only */
	{0x6ea8c007, 0x6707c015},   /* r7 = isdbmbxin, vid = r7 */	
	{0x6ea8c007, 0x6707c016},   /* r7 = isdbmbxin, vid1 = r7 */
	{0x6ea8c007, 0x6707c017}, 	/* r7 = isdbmbxin, bestwait = r7 */
	{0x0, 0x0}, 				/* Invalid: S24 reserved */
	{0x0, 0x0}, 				/* schdcfg: read only */
	{0x0, 0x0}, 				/* Invalid: S26 reserved */
	{0x0, 0x0},					/* cfgbase: read only  */
	{0x6ea8c007, 0x6707c01c}, 	/* r7 = isdbmbxin, diag = r7 */
	{0x0, 0x0},					/* Rev: read only */
	{0x6ea8c007, 0x6707c01e},   /* r7 = isdbmbxin, pcyclelo = r7 */
	{0x6ea8c007, 0x6707c01f}, 	/* r7 = isdbmbxin, pcyclehi = r7 */	
};


static int hexagon_get_core_reg(struct reg *reg);
static int hexagon_set_core_reg(struct reg *reg, uint8_t *buf);

static const struct reg_arch_type hexagon_reg_type = {
	.get = hexagon_get_core_reg,
	.set = hexagon_set_core_reg,
};
uint32_t hexagon_r0_used_stuff = 0, hexagon_r1_used_stuff = 0, 
	hexagon_r2_used_stuff = 0, hexagon_r7_used_stuff = 0;
uint32_t hexagon_r30_used_stuff = 0,  hexagon_r31_used_stuff = 0;

#ifdef  _HEXAGON_TARGET_TIME_PROFILING
int64_t hexagon_time_start = 0,hexagon_time_total = 0;
#endif

vtlb_data hexagon_vtlb_data = {0};
tlb_entries * hexagon_vtlb_entries = NULL;
struct adiv5_ap * debug_axi_ap = NULL;
static uint32_t hexagon_syscfg_reg; 

//Global data to store/keep track of the initial PC value once threads are halted through software breakpoint, value are cleared once resume happens.
static uint8_t sbp_step_executed = 0;	//Just a bool variable


/****************************Function declartions***********************************************/
static int hexagon_poll(struct target *target);
int  hexagon_arch_state(struct target *target);
static int hexagon_resume(struct target *target, int current, target_addr_t address,
	int handle_breakpoints, int debug_execution);
static int hexagon_step(struct target *target, int current, target_addr_t address,
			int handle_breakpoints);
static int hexagon_halt(struct target *target);
const char *hexagon_get_gdb_arch(struct target *target);
int hexagon_get_gdb_reg_list(struct target *target,
	struct reg **reg_list[], int *reg_list_size,
	enum target_register_class reg_class);

static int hexagon_read_memory(struct target *target, target_addr_t address,
	uint32_t size, uint32_t count, uint8_t *buffer);
static int hexagon_write_memory(struct target *target, target_addr_t address,
	uint32_t size, uint32_t count, const uint8_t *buffer);
static int hexagon_read_buffer (struct target *target, target_addr_t address,
		uint32_t size, uint8_t *buffer);
static int hexagon_write_buffer (struct target *target, target_addr_t address,
		uint32_t size, const uint8_t *buffer);
static int hexagon_add_breakpoint(struct target *target,
	struct breakpoint *breakpoint);
static int hexagon_remove_breakpoint(struct target *target, struct breakpoint *breakpoint);
static int hexagon_target_create(struct target *target, Jim_Interp *interp);
static int hexagon_jim_configure(struct target *target, struct jim_getopt_info *goi);
static int hexagon_init_debug_access(struct target *target);
static int hexagon_examine_first(struct target *target);
static int hexagon_examine(struct target *target);
static int hexagon_init_target(struct command_context *cmd_ctx,
	struct target *target);
static void hexagon_deinit_target(struct target *target);
static int hexagon_virt2phys(struct target *target, target_addr_t virt,
			     target_addr_t *phys);
static int hexagon_mmu(struct target *target, int *enabled);
static int hexagon_init_arch_info(struct target *target,
	struct hexagon_common *hexagon, struct adiv5_dap *dap);
static int hexagon_handle_target_request(void *priv);
static uint64_t hexagon_etm_on(struct target *target);
static void hexagon_wait_loop(void);
static int hexagon_brkpt_setup(struct hexagon_common *hexagon);
static int hexagon_reg_setup(struct hexagon_common *hexagon);
static int hexagon_read_core_reg(struct target *target, struct reg *r, int regnum, uint32_t hwthrd);
static int hexagon_write_core_reg(struct target *target, int regnum, uint32_t hwthrd, uint32_t value);
struct reg *hexagon_reg_current(struct hexa_info *hexa_info, unsigned int regnum, 
	struct reg_cache *cache);
struct reg_cache *hexagon_build_reg_cache(struct target *target, uint32_t hwthrd);
static int hexagon_check_state_one(struct target *target,
                                        uint64_t mask, bool *halted, uint32_t *debug_thread);
int hexagon_read_current_registers(struct target *target, uint32_t hwthrd);
void hexagon_update_sp_pc_fp_gdb_server(unsigned int hwthrd, unsigned int * pc , 
		unsigned int * fp, unsigned int * sp);
int hexagon_read_tlb_entry(struct target *target);
static void hexagon_update_tlb_entry_in_structure(uint64_t tlb_phy, uint64_t tlb_virtual, uint64_t index);

#ifdef  _HEXAGON_TARGET_TIME_PROFILING
void hexagon_start_time_cal_ms(void);
void hexagon_end_time_cal_ms(void);
#endif
static int hexagon_dump_hwthrd_reg(struct target *target, uint32_t hwthrd);
int hexagon_read_gpr_registers(struct target *target, uint32_t hwthrd);
int hexagon_read_ctrl_registers(struct target *target, uint32_t hwthrd);
int hexagon_read_mmode_registers(struct target *target, uint32_t hwthrd);
int hexagon_read_imask_register(struct target *target, uint32_t hwthrd);
static int hexagon_restore_stuff_used_reg(struct target *target);
int hexagon_read_global_ctrl_registers(struct target *target);
void hexagon_debug_reason(struct target *target, uint64_t brkptinfo);
static int hexagon_write_gpr_register(struct target *target, int regnum, uint32_t hwthrd, uint32_t value);
static int hexagon_write_ctrl_register(struct target *target, int regnum, uint32_t hwthrd, uint32_t value);
static int hexagon_write_global_ctrl_register(struct target *target, int regnum, uint32_t hwthrd, uint32_t value);
unsigned int get_phys_page(unsigned int lo, unsigned int hi, unsigned int mask);
unsigned int get_phys_mask(unsigned int tlblo);
static unsigned int hexagon_ct0(unsigned int d);
static unsigned int hexagon_clrbit(unsigned int d, unsigned int bit);
static unsigned int QURT_getPhysAddr_v2(uint64_t pg_tlblo, uint64_t pg_tlbhi);
static int hexagon_search_virtadd_in_tlb(uint64_t virt, target_addr_t *phys);
static int hexagon_search_virtadd_in_vtlb(struct target *target, uint64_t virt_add, target_addr_t *phys);
static void hexagon_memw_phys_read(struct target *target, target_addr_t phy_address, uint32_t *value);

static void hexagon_memw_read(struct target *target, uint64_t virt_address, uint32_t *value);

static void hexagon_stuff_reg_restore(struct target *target);

static void hexagon_populate_vtlb_entries(struct target *target);
static void hexagon_update_vtlb_entry_in_structure(uint64_t tlb_phy, uint64_t tlb_virtual, uint64_t index);
static void hexagon_populate_vtlb_refresh_entries(struct target *target);
// static void hexagon_stuff_reg_restore_r7(struct target *target);
static void hexagon_memw_phys_read_buffer(struct target *target,target_addr_t phy_address, uint32_t size, uint8_t * buffer);
static int hexagon_write_syscfg_register(struct target *target,uint32_t value);
static int hexagon_read_syscfg_register(struct target *target);
static int hexagon_memw_write(struct target *target, uint64_t virt_address, uint32_t value, uint32_t size);
static int hexagon_memw_write_buffer(struct target *target, uint64_t virt_address, uint32_t size, const uint8_t *buffer);
static int hexagon_set_breakpoint(struct target *target, struct breakpoint *breakpoint, uint64_t bpconfig);
static int hexagon_setup_isdb_config(struct target *target, uint64_t old_isdbcfg0, uint8_t hbp_num);
static int hexagon_memw_write_instruction_memory(struct target *target, uint64_t virt_address, uint32_t value, uint8_t flag);
static int hexagon_unset_breakpoint(struct target *target, struct breakpoint *breakpoint);
// static void hexagon_stuff_reg_restore(struct target *target);
static uint32_t hexagon_print_pc(struct target *target);
static int hexagon_dump_isdb_reg(struct hexa_info *hexa_info);
static int hexagon_read_BRKPT_through_stuff(struct target *target, uint32_t hwthrd);
static void hexagon_populate_vtlb_data(struct target *target);
static void hexagon_print_vtlb_entries(void);
uint32_t  hexagon_no_of_hw_threads(void);
static void hexagon_hw_watchdog_disable(struct target *target);
static void hexagon_enable_clock(struct target *target);
static int hexagon_read_ISDB(struct target *target, uint32_t isdbsts, uint64_t stuffcmdStatusCheck);

/***************************** UNTRUSTED MODE Function declarations ***********************************************/
//   parameters should explicitly specify void to indicate that it takes no arguments.
int hexagon_untrusted_concat_rsp(void);
int hexagon_untrusted_write_to_mailboxin(struct target *target, uint32_t value);
uint64_t min_u(uint64_t a, uint64_t b);
int hexagon_untrusted_read_to_mailboxout(struct target *target, uint32_t *value);
int hexagon_untrusted_poll_isdbst_set_bit(struct target *target, uint8_t bit);

int hexagon_untrusted_poll_isdbst_unset_bit(struct target *target, uint8_t bit);
int send_isdb_interrupt(struct target *target);
int hexagon_untrusted_mode(void);
static uint32_t hexagon_untrusted_convert_essential_header(untrusted_essential header);
void current_debug_thread(uint32_t selected_thread);
// 
/**********************************Function Definitions*****************************************/
/* this function is used to fetch SP , PC and FP value for given HW thread from gdb server  */
void hexagon_update_sp_pc_fp_gdb_server(unsigned int hwthrd, unsigned int * pc , 
		unsigned int * fp, unsigned int * sp)
{


	*pc = gpPerHwThrdReg[hwthrd][HEXAGON_PC];
	*fp = gpPerHwThrdReg[hwthrd][HEXAGON_FP];
	*sp = gpPerHwThrdReg[hwthrd][HEXAGON_SP];

}

uint32_t  hexagon_no_of_hw_threads(void)
{

#if 1

    uint32_t temp;

    if (multi_thr_enabled)
        temp = gHexConfig.maxHwThreads;
    else
        temp = 1;
    LOG_DEBUG("hexagon_no_of_hw_threads  = 0x%x  ", temp);

    return temp;
#endif

	
}
static int hexagon_virt2phys(struct target *target, target_addr_t virt, target_addr_t *phys)
{
	int ret_val = 0;
    uint64_t virt_add = 0;
	virt_add = virt;

#ifdef _VTLB_ENABLED
	LOG_INFO ("virtual address : 0x%llx ", virt_add);
    if (mmu_init)
    {
		ret_val = hexagon_search_virtadd_in_tlb( virt_add, phys);
		if(ret_val == ERROR_OK)
			return ret_val;
	
		ret_val = hexagon_search_virtadd_in_vtlb(target, virt_add, phys);

		LOG_INFO ("virtual address : 0x%llx ; physical address :0x%llx", virt_add, *phys);
    }
#endif

	return ret_val;
}

/* this function is use to convert the Virtual address to physical address using TLB entries*/
static int hexagon_search_virtadd_in_tlb(uint64_t virt, target_addr_t *phys)
{
	uint32_t i;
    uint64_t offset = 0;

	for (i=0;  i  <  gHexConfig.numTlbEntries;  i++)
	{
		if( (virt >= gpHexagonTlbEntries[i].virt_add_low)  &&  (virt <= gpHexagonTlbEntries[i].virt_add_high))
		{
			if(gpHexagonTlbEntries[i].globalbit)
			{
				offset = virt - gpHexagonTlbEntries[i].virt_add_low;
				*phys = gpHexagonTlbEntries[i].phy_add_low + offset;
				LOG_DEBUG("V Bit = 0x%x; G Bit = 0x%x; ASID = 0x%x ", gpHexagonTlbEntries[i].validbit, gpHexagonTlbEntries[i].globalbit, gpHexagonTlbEntries[i].asid );

                LOG_DEBUG("i=%d; virt =0x%llx ; virt_add_low = 0x%x; phy_add_low = 0x%llx  ",i, virt, gpHexagonTlbEntries[i].virt_add_low, gpHexagonTlbEntries[i].phy_add_low);
                LOG_DEBUG("Physical address  = 0x%llx  ",*phys);

				return ERROR_OK;
			}
			else
			{
				offset= virt - gpHexagonTlbEntries[i].virt_add_low;
				*phys = gpHexagonTlbEntries[i].phy_add_low + offset;
				LOG_DEBUG("V Bit = 0x%x; G Bit = 0x%x; ASID = 0x%x ", gpHexagonTlbEntries[i].validbit, gpHexagonTlbEntries[i].globalbit, gpHexagonTlbEntries[i].asid );

                // LOG_DEBUG("Physical address  = 0x%x  ",*phys);
                LOG_DEBUG("i=%d; virt =0x%llx ; virt_add_low = 0x%x; phy_add_low = 0x%llx  ",i, virt, gpHexagonTlbEntries[i].virt_add_low, gpHexagonTlbEntries[i].phy_add_low);
				return ERROR_OK;

			}
		}
	}
	if(i  ==  gHexConfig.numTlbEntries)
	{
	    *phys = 0;
        LOG_DEBUG("Entry not found in TLB for virt address = 0x%llx  ", virt);
		return ERROR_FAIL;
	}

	return ERROR_OK;
}

/* this function is use to convert the Virtual address to physical address using VTLB entries*/
static int hexagon_search_virtadd_in_vtlb(struct target *target, uint64_t virt_add, target_addr_t *phys)
{
    uint64_t offset = 0, i;
	tlb_entries *temp = NULL;

//	hexagon_populate_vtlb_refresh_entries(target);

	if(hexagon_vtlb_data.vtlb_no_of_entries == 0)
	{
		hexagon_populate_vtlb_data(target);
	}
	for (i=0; i  <  hexagon_vtlb_data.valid_vtlb_no_of_entries; i++)
	{
		temp = hexagon_vtlb_entries + i;
		if( virt_add >= temp->virt_add_low  &&  virt_add <= temp->virt_add_high)
		{
			if(temp->globalbit)
			{
				offset= virt_add - temp->virt_add_low;
				*phys = temp->phy_add_low + offset;
				LOG_DEBUG("V Bit = 0x%x; G Bit = 0x%x; ASID = 0x%x ", temp->validbit, temp->globalbit, temp->asid );
				LOG_DEBUG("Physical address  = 0x%x  ", (uint32_t)*phys);
				return ERROR_OK;
			}
			else
			{
				offset= virt_add - temp->virt_add_low;
				*phys = temp->phy_add_low + offset;
				LOG_DEBUG("V Bit = 0x%x; G Bit = 0x%x; ASID = 0x%x ", temp->validbit, temp->globalbit, temp->asid );
				LOG_DEBUG("Physical address  = 0x%x  ", (uint32_t)*phys);
				return ERROR_OK;
			}
		}
	}
	if(i == hexagon_vtlb_data.valid_vtlb_no_of_entries)
	{
		*phys = 0;
        LOG_DEBUG("Entry not found in Vtable virt address  = 0x%llx  ", virt_add);
		return ERROR_FAIL;
	}

	return ERROR_OK;
}

/* This function refresh the VTLB entries in case of Halt / step in etc*/
static void hexagon_populate_vtlb_refresh_entries(struct target *target)
{
	int retval = ERROR_OK;

	LOG_DEBUG("hexagon_populate_vtlb_refresh_entries  Enter");

	if(hexagon_vtlb_data.vtlb_no_of_entries == 0)
	{
			hexagon_populate_vtlb_data(target);
			return;
	}

#ifdef HEXAGON_VTLB_OLD_ARCH
#ifdef HEXAGON_VTLB_AXI
	if(debug_axi_ap == NULL)
		hexagon_initialize_axi_ap(target);

	retval = mem_ap_read_buf(debug_axi_ap,
		(uint8_t *)&hexagon_vtlb_data.vtlb_current_counter, 4, 1, hexagon_vtlb_data.QURTK_vtlb_main_PA-16);
	
	if (retval != ERROR_OK)
	{
		LOG_DEBUG("mem_ap_read_buf API failed to read");
		return;
	}
	if(hexagon_vtlb_data.vtlb_previous_counter == hexagon_vtlb_data.vtlb_current_counter)
	{
		return;
	}
	retval = mem_ap_read_buf(debug_axi_ap,(uint8_t *)&hexagon_vtlb_data.vtlb_no_of_entries, 4, 1,
							hexagon_vtlb_data.QURTK_VTLB_DATA_PA);
	if (retval != ERROR_OK)
	{
		LOG_DEBUG("mem_ap_read_buf API failed to read");
		return;
	}
#endif

#ifdef HEXAGON_VTLB_MEM_PHYS
		hexagon_memw_phys_read(target,hexagon_vtlb_data.QURTK_vtlb_main_PA-16, &hexagon_vtlb_data.vtlb_current_counter);
		if(hexagon_vtlb_data.vtlb_previous_counter == hexagon_vtlb_data.vtlb_current_counter)
		{
			return;
		}
		hexagon_memw_phys_read(target,hexagon_vtlb_data.QURTK_VTLB_DATA_PA, &hexagon_vtlb_data.vtlb_no_of_entries);
#endif

#ifdef HEXAGON_VTLB_MEMW
		hexagon_memw_read(target,hexagon_vtlb_data.QURTK_vtlb_main_VA-16, &hexagon_vtlb_data.vtlb_current_counter);
		if(hexagon_vtlb_data.vtlb_previous_counter == hexagon_vtlb_data.vtlb_current_counter)
		{
			return;
		}
		hexagon_memw_read(target,hexagon_vtlb_data.QURTK_VTLB_DATA_VA, &hexagon_vtlb_data.vtlb_no_of_entries);
#endif
#endif 

#ifdef HEXAGON_VTLB_NEW_ARCH
    // uint64_t output[2] = {0};
    uint32_t output[2] = {0};

	hexagon_memw_read(target,hexagon_vtlb_data.QURTK_vtlb_main_VA-16,
						&hexagon_vtlb_data.vtlb_current_counter);
	if(hexagon_vtlb_data.vtlb_previous_counter == hexagon_vtlb_data.vtlb_current_counter)
	{
		return;
	}
	hexagon_memw_read(target,hexagon_vtlb_data.QURTK_vtlb_main_VA, &output[0]);
	hexagon_memw_read(target,hexagon_vtlb_data.QURTK_vtlb_main_VA+4, &output[1]);

    // memcpy(&hexagon_vtlb_data.qurtk_vtlb_main, &output, 8);
    // LOG_DEBUG("hexagon_vtlb_data.qurtk_vtlb_main.next_table_addr = 0x%x ",
    //           hexagon_vtlb_data.qurtk_vtlb_main.next_table_addr);

	hexagon_memw_read(target,hexagon_vtlb_data.qurtk_vtlb_main.next_table_addr, &output[0]);
	hexagon_memw_read(target,hexagon_vtlb_data.qurtk_vtlb_main.next_table_addr+4, &output[1]);
    // memcpy(&hexagon_vtlb_data.qurtk_vtlb_main_next, &output, 8);
	LOG_DEBUG("hexagon_vtlb_data.qurtk_vtlb_main_next.table_entries= 0x%x ",
				hexagon_vtlb_data.qurtk_vtlb_main_next.table_entries);
	hexagon_vtlb_data.vtlb_no_of_entries =  hexagon_vtlb_data.qurtk_vtlb_main_next.table_entries;
#endif


	LOG_DEBUG("hexagon_vtlb_data.vtlb_no_of_entries = 0x%x and retval = %d",hexagon_vtlb_data.vtlb_no_of_entries,retval);

	if (hexagon_vtlb_entries == NULL)
	{
		hexagon_vtlb_entries = malloc(sizeof(tlb_entries) * hexagon_vtlb_data.vtlb_no_of_entries);
		memset(hexagon_vtlb_entries, 0, sizeof(tlb_entries) * hexagon_vtlb_data.vtlb_no_of_entries);
	}
	else
	{
		free(hexagon_vtlb_entries);
		hexagon_vtlb_entries = malloc(sizeof(tlb_entries) * hexagon_vtlb_data.vtlb_no_of_entries);
		memset(hexagon_vtlb_entries, 0, sizeof(tlb_entries) * hexagon_vtlb_data.vtlb_no_of_entries);
	}
	hexagon_populate_vtlb_entries(target);
	LOG_DEBUG("hexagon_populate_vtlb_refresh_entries  Exit");
}


/* This function used to initialize the AXI-AP */
static void hexagon_initialize_axi_ap(struct target *target)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct adiv5_dap *swddp = hexa_info->dap;
	int retval,i;
	
	LOG_DEBUG("hexagon_initialize_axi_ap  Enter");
	retval = enable_dbg_sys_pwr(swddp);
	
	if(debug_axi_ap == NULL)
	{
		/* Search for the AXI-AP - it is needed for access to memory */
		retval = dap_find_get_ap(swddp, AP_TYPE_AXI_AP, &debug_axi_ap);
		if (retval != ERROR_OK) 
		{
			for(i=0; i < 10; i++)
			{
				hexagon_wait_loop();
				retval = dap_find_get_ap(swddp, AP_TYPE_AXI_AP, &debug_axi_ap);
				if(retval == ERROR_OK)
					break;
			}
			if (retval != ERROR_OK) 
			{
				LOG_DEBUG("Could not find AP_TYPE_AXI_AP for debug access");
				LOG_DEBUG("Critical connection Error, Please reboot the device and try to attach again");
				return;
			}
		}
		retval = enable_dbg_sys_pwr(swddp);
		retval = mem_ap_init(debug_axi_ap);
        if (retval != ERROR_OK)
        {
			LOG_DEBUG("Could not initialize the AP_TYPE_AXI_AP");
			return;
		}
		debug_axi_ap->memaccess_tck = 10;
	}
	LOG_DEBUG("hexagon_initialize_axi_ap  Exit");
}


/* This function populate the VTLB global data like no of entries in VTLB, counter and allocate VTLB memory for entries */
static void hexagon_populate_vtlb_data(struct target *target)
{

	LOG_DEBUG("hexagon_populate_vtlb_data  Enter");

#ifdef HEXAGON_VTLB_OLD_ARCH
	int retval;

	#ifdef HEXAGON_VTLB_AXI
		if(debug_axi_ap == NULL)
			hexagon_initialize_axi_ap(target);
	#endif



	hexagon_vtlb_data.valid_vtlb_no_of_entries = 0;

	#ifdef HEXAGON_VTLB_AXI
		retval = mem_ap_read_buf(debug_axi_ap,
			(uint8_t *)&hexagon_vtlb_data.vtlb_current_counter, 4, 1, hexagon_vtlb_data.QURTK_vtlb_main_PA-16);
		if (retval != ERROR_OK)
		{
			LOG_DEBUG("mem_ap_read_buf API failed to read");
			return;
		}
		hexagon_vtlb_data.vtlb_previous_counter = hexagon_vtlb_data.vtlb_current_counter;
		LOG_DEBUG("vtlb_current_counter = 0x%x and retval = %d",hexagon_vtlb_data.vtlb_current_counter,retval);

		retval = mem_ap_read_buf(debug_axi_ap,(uint8_t *)&hexagon_vtlb_data.vtlb_no_of_entries, 4, 1, 
								hexagon_vtlb_data.QURTK_VTLB_DATA_PA);
		if (retval != ERROR_OK)
		{
			LOG_DEBUG("mem_ap_read_buf API failed to read");
			return;
		}
		LOG_DEBUG("vtlb_no_of_entries = 0x%x and retval = %d",hexagon_vtlb_data.vtlb_no_of_entries,retval);
	#endif

	#ifdef HEXAGON_VTLB_MEM_PHYS
		hexagon_memw_phys_read(target,hexagon_vtlb_data.QURTK_vtlb_main_PA-16,
									&hexagon_vtlb_data.vtlb_current_counter);
		hexagon_vtlb_data.vtlb_previous_counter = hexagon_vtlb_data.vtlb_current_counter;
		LOG_DEBUG("vtlb_current_counter = 0x%x ",hexagon_vtlb_data.vtlb_current_counter);
		hexagon_memw_phys_read(target,hexagon_vtlb_data.QURTK_VTLB_DATA_PA, 
									&hexagon_vtlb_data.vtlb_no_of_entries);
		LOG_DEBUG("vtlb_no_of_entries = 0x%x ",hexagon_vtlb_data.vtlb_no_of_entries);
	#endif

	#ifdef HEXAGON_VTLB_MEMW
		hexagon_memw_read(target,hexagon_vtlb_data.QURTK_vtlb_main_VA-16,
								&hexagon_vtlb_data.vtlb_current_counter);
		hexagon_vtlb_data.vtlb_previous_counter = hexagon_vtlb_data.vtlb_current_counter;
		LOG_DEBUG("hexagon_vtlb_data.vtlb_current_counter = 0x%x ",hexagon_vtlb_data.vtlb_current_counter);
		hexagon_memw_read(target,hexagon_vtlb_data.QURTK_VTLB_DATA_VA, 
								&hexagon_vtlb_data.vtlb_no_of_entries);
		LOG_DEBUG("hexagon_vtlb_data.vtlb_no_of_entries = 0x%x ",hexagon_vtlb_data.vtlb_no_of_entries);
	#endif
	
	if (hexagon_vtlb_entries == NULL)
	{
		hexagon_vtlb_entries = malloc(sizeof(tlb_entries) * hexagon_vtlb_data.vtlb_no_of_entries);
		memset(hexagon_vtlb_entries, 0, sizeof(tlb_entries) * hexagon_vtlb_data.vtlb_no_of_entries);
	}

#endif 

#ifdef HEXAGON_VTLB_NEW_ARCH

	//QURTK_vtlb_main           D:FE111F5C--FE111F5F

	// uint32_t qurtk_vtlb_main_addr = hexagon_vtlb_data.QURTK_vtlb_main_VA;
	hexagon_vtlb_data.valid_vtlb_no_of_entries = 0;
    uint64_t temp, temp1, temp2;
	uint32_t output[2] = {0};

    LOG_DEBUG("qurtk_vtlb_main_addr   = 0x%x ",qurtk_vtlb_main_addr);
	hexagon_memw_read(target, qurtk_vtlb_main_addr, &hexagon_vtlb_data.QURTK_vtlb_main_VA );
	LOG_DEBUG("hexagon_vtlb_data.QURTK_vtlb_main_VA   = 0x%x ",hexagon_vtlb_data.QURTK_vtlb_main_VA );
	
	hexagon_memw_read(target,hexagon_vtlb_data.QURTK_vtlb_main_VA-16,
						&hexagon_vtlb_data.vtlb_current_counter);
	hexagon_vtlb_data.vtlb_previous_counter = hexagon_vtlb_data.vtlb_current_counter;
	LOG_DEBUG("vtlb_current_counter = 0x%x ",hexagon_vtlb_data.vtlb_current_counter);

	hexagon_memw_read(target,hexagon_vtlb_data.QURTK_vtlb_main_VA, &output[0]);
	hexagon_memw_read(target,hexagon_vtlb_data.QURTK_vtlb_main_VA+4, &output[1]);

	temp = output[0] >> 8;
    	temp1 = (output[1] & 0xFFF)<<24;
	temp2 = temp | temp1;
	hexagon_vtlb_data.qurtk_vtlb_main.next_table_addr = temp2;
	
	// next_table_addr is of type long long unsigned int:36 
    // hence using format specifier llx which is for long long unsigned int (64 bit)
	LOG_DEBUG("hexagon_vtlb_data.qurtk_vtlb_main.next_table_addr = 0x%llx ",
				(unsigned long long)hexagon_vtlb_data.qurtk_vtlb_main.next_table_addr);

	hexagon_memw_read(target,hexagon_vtlb_data.qurtk_vtlb_main.next_table_addr, &output[0]);
	hexagon_memw_read(target,hexagon_vtlb_data.qurtk_vtlb_main.next_table_addr+4, &output[1]);
	//memcpy(&hexagon_vtlb_data.qurtk_vtlb_main_next, &output, 8);

	temp2 = output[1] >>12;
	hexagon_vtlb_data.qurtk_vtlb_main_next.table_entries =  temp2 ;
	
	LOG_DEBUG("hexagon_vtlb_data.qurtk_vtlb_main_next.table_entries= 0x%x ",
				hexagon_vtlb_data.qurtk_vtlb_main_next.table_entries);
	hexagon_vtlb_data.vtlb_no_of_entries =  hexagon_vtlb_data.qurtk_vtlb_main_next.table_entries;

	if (hexagon_vtlb_entries == NULL)
	{
		hexagon_vtlb_entries = malloc(sizeof(tlb_entries) * hexagon_vtlb_data.vtlb_no_of_entries);
		memset(hexagon_vtlb_entries, 0, sizeof(tlb_entries) * hexagon_vtlb_data.vtlb_no_of_entries);
	}
#endif

	hexagon_populate_vtlb_entries(target);
	LOG_DEBUG("hexagon_populate_vtlb_data  Exit");
}

/* This function populate the VTLB entries from QURTK_VTLB_DATA*/
static void hexagon_populate_vtlb_entries(struct target *target)
{
	uint32_t output[2] = {0},i,address;
	
	LOG_DEBUG("hexagon_populate_vtlb_entries  Enter");
	
#ifdef	_HEXAGON_TARGET_TIME_PROFILING
		hexagon_start_time_cal_ms();
#endif


#ifdef HEXAGON_VTLB_OLD_ARCH
	int retval = ERROR_OK;

	#ifdef _BITRA_TARGET
		address =  hexagon_vtlb_data.QURTK_VTLB_DATA_PA+0x40E0;
		#ifdef HEXAGON_VTLB_MEMW
			address =  hexagon_vtlb_data.QURTK_VTLB_DATA_VA+0x40E0;
		#endif
	#endif 

	#ifdef _CEDROS_TARGET
		address =  hexagon_vtlb_data.QURTK_VTLB_DATA_PA+0x40E0;
		#ifdef HEXAGON_VTLB_MEMW
			address =  hexagon_vtlb_data.QURTK_VTLB_DATA_VA+0x40E0;
		#endif
	#endif 


	#ifdef HEXAGON_VTLB_AXI
		if(debug_axi_ap == NULL)
			hexagon_initialize_axi_ap(target);
	#endif
	hexagon_vtlb_data.valid_vtlb_no_of_entries = 0;

	#ifdef HEXAGON_VTLB_AXI
		for (i=0; i < hexagon_vtlb_data.vtlb_no_of_entries; i++)
		{
			retval = mem_ap_read_buf(debug_axi_ap,(uint8_t *)&output, 4, 2, address + 8*i);
			if (retval != ERROR_OK)
			{
				LOG_DEBUG("mem_ap_read_buf API failed to read");
				continue;
			}
			hexagon_update_vtlb_entry_in_structure(output[0], output[1],i);
		}
	#endif
	
	#ifdef HEXAGON_VTLB_MEM_PHYS
		for (i=0; i < hexagon_vtlb_data.vtlb_no_of_entries; i++)
		{
			hexagon_memw_phys_read(target,address, &output[0]);
			hexagon_memw_phys_read(target,address+4, &output[1]);
		//	LOG_DEBUG("Phy_page = 0x%08x and virtual page  = 0x%08x " , output[0],output[1]);
			hexagon_update_vtlb_entry_in_structure(output[0], output[1],i);
			address= address + 8;
		}
	#endif

	#ifdef HEXAGON_VTLB_MEMW
		for (i=0; i < hexagon_vtlb_data.vtlb_no_of_entries; i++)
		{
			hexagon_memw_read(target,address, &output[0]);
			hexagon_memw_read(target,address+4, &output[1]);
		//	LOG_DEBUG("Phy_page = 0x%08x and virtual page  = 0x%08x " , output[0],output[1]);
			hexagon_update_vtlb_entry_in_structure(output[0], output[1],i);
			address= address + 8;
		}
	#endif
#endif 

#ifdef HEXAGON_VTLB_NEW_ARCH
	address =  hexagon_vtlb_data.qurtk_vtlb_main.next_table_addr + 8;
	for (i=0; i < hexagon_vtlb_data.vtlb_no_of_entries; i++)
	{
		hexagon_memw_read(target,address, &output[0]);
		hexagon_memw_read(target,address+4, &output[1]);
        LOG_DEBUG("raw start range = 0x%08x and raw end range  = 0x%08x " , output[0],output[1]);
		hexagon_update_vtlb_entry_in_structure(output[0], output[1],i);
		address= address + 8;
	}

#endif

	hexagon_print_vtlb_entries();
	
	#ifdef  _HEXAGON_TARGET_TIME_PROFILING
		hexagon_end_time_cal_ms();
		LOG_DEBUG("Total time taken  %" PRId64 "ms", hexagon_time_total);
	#endif

	LOG_DEBUG("hexagon_populate_vtlb_entries  Exit");


}

/* This function print the VTLB entries*/
static void hexagon_print_vtlb_entries(void)
{
	tlb_entries * temp = NULL;
    uint64_t i;
	LOG_DEBUG("Printing the VTLB content");

	for (i=0; i < hexagon_vtlb_data.valid_vtlb_no_of_entries; i++) 
	{
		temp = hexagon_vtlb_entries + i;
		/*LOG_DEBUG("VA = 0x%x --> PA = 0x%x ", temp->virt_tlb_raw_data,temp->phys_tlb_raw_data);
		LOG_DEBUG("VA Page = 0x%08x PA Page =  0x%08x and Page size = %d", temp->virt_page,temp->phy_page,temp->page_size);*/
        LOG_DEBUG("VA = 0x%08x -- 0x%08x and PA = 0x%llx -- 0x%llx", temp->virt_add_low, temp->virt_add_high,
                  temp->phy_add_low, temp->phy_add_high);
    }
}


/* this function update the tlb entry in global structure  hexagon_tlb_entries */
static void hexagon_update_vtlb_entry_in_structure(uint64_t tlb_phy, uint64_t tlb_virtual, uint64_t index)
{
    uint64_t mask, size = 0, virt_page = 0, phy_page = 0, virt_add = 0, phy_add =0;
	char * page_size = 0;
	tlb_entries *temp = NULL;
	union pg_tlblo_t  tlblo;
	union pg_tlbhi_t  tlbhi;

	//LOG_DEBUG("hexagon_update_vtlb_entry_in_structure  Enter");
	if ((index >= hexagon_vtlb_data.vtlb_no_of_entries) || (hexagon_vtlb_data.valid_vtlb_no_of_entries >= hexagon_vtlb_data.vtlb_no_of_entries))
	{
        // LOG_DEBUG("Index is greater than hexagon_vtlb_data.vtlb_no_of_entries");
		return;
	}
	virt_page = VIRT_PAGE(tlb_virtual);
	// previously used a mask, here we are right shifting
	// virt_page = tlb_virtual >> 12;

	if(virt_page == 0x0 )
		return;
	tlblo.raw = tlb_phy;
	tlbhi.raw = tlb_virtual;
	virt_add = virt_page << 12;

	phy_page = QURT_getPhysAddr_v2(tlblo.raw, tlbhi.raw);
	phy_add = phy_page << 12;

	if((virt_add == 0x0) || (phy_add == 0x0) )
	{
		//LOG_DEBUG("Virtual / Physical  address is NULL for raw phy= 0x%08x and raw virtual = 0x%08x " , tlb_phy,tlb_virtual);
		return;
	}

	temp = hexagon_vtlb_entries  + hexagon_vtlb_data.valid_vtlb_no_of_entries;
	mask = get_phys_mask(tlb_phy);
	temp->virt_tlb_raw_data = tlb_virtual;
	temp->phys_tlb_raw_data = tlb_phy;
	temp->phy_page = phy_page; 
	temp->virt_page = virt_page;
	temp->asid = ASID(tlb_virtual);
	LOG_DEBUG("SID = 0x%x ", temp->asid);

	temp->asid = ASID(tlbhi.raw);
	LOG_DEBUG("SID = 0x%x ", temp->asid);

	temp->R = P_READ(tlb_phy);
	temp->W = P_WRITE(tlb_phy);
	temp->X= P_EXEC(tlb_phy);
	temp->U = P_USER(tlb_phy);
	temp->CCCC = P_CCCC(tlb_phy);
	temp->S = P_S(tlb_phy);
	temp->validbit = P_V(tlb_virtual);
	temp->globalbit= P_G(tlb_virtual);
	temp->EP = P_EP(tlb_virtual);
	temp->A1 = P_A1(tlb_virtual);
	temp->A0 = P_A0(tlb_virtual);
	page_size = PAGE_SIZE(tlb_phy, mask);
	hexagon_vtlb_data.valid_vtlb_no_of_entries++;


	if (strcmp(page_size, "4KB") == 0)
	{
		temp->page_size =  HEXAGON_TLB_PAGE_SIZE_4KB;
		size = HEXAGON_PAGE_SIZE_4K -1;
	}
	else if (strcmp(page_size, "16KB") == 0) 
	{
		temp->page_size =  HEXAGON_TLB_PAGE_SIZE_16KB;
		size = HEXAGON_PAGE_SIZE_16K-1;
	}
	else if (strcmp(page_size, "64KB") == 0) 
	{
		temp->page_size =  HEXAGON_TLB_PAGE_SIZE_64KB;
		size = HEXAGON_PAGE_SIZE_64K-1;
	}
	else if (strcmp(page_size, "256KB") == 0) 
	{
		temp->page_size =  HEXAGON_TLB_PAGE_SIZE_256KB;
		size = HEXAGON_PAGE_SIZE_256K-1;
	}
	else if (strcmp(page_size, "1MB") == 0) 
	{
		temp->page_size =  HEXAGON_TLB_PAGE_SIZE_1MB;
		size = HEXAGON_PAGE_SIZE_1M -1;
	}
	else if (strcmp(page_size, "4MB") == 0) 
	{
		temp->page_size =  HEXAGON_TLB_PAGE_SIZE_4MB;
		size = HEXAGON_PAGE_SIZE_4M -1;
	}
	else if (strcmp(page_size, "16MB") == 0) 
	{
		temp->page_size =  HEXAGON_TLB_PAGE_SIZE_16MB;
		size = HEXAGON_PAGE_SIZE_16M -1;
	}

	temp->virt_add_low= virt_add;
	temp->virt_add_high = virt_add + size;

	// temp->phy_add_low = temp->phy_page << 12;
	// temp->phy_add_high = temp->phy_add_low + size;

	temp->phy_add_low = (uint64_t) temp->phy_page << 12;
	temp->phy_add_high = (uint64_t) temp->phy_add_low + size;

    // LOG_DEBUG("Phy after left shift = 0x%llx",temp->phy_add_low);
    // LOG_DEBUG("Phy page address before shift = 0x%llx",temp->phy_page);
	LOG_DEBUG("V bit =0x%x G bit = 0x%x ASID = 0x%x ", temp->validbit, temp->globalbit, temp->asid);
    LOG_DEBUG("VA = 0x%x -- 0x%x ; PA = 0x%llx -- 0x%llx", temp->virt_add_low,temp->virt_add_high, temp->phy_add_low,temp->phy_add_high);
	//LOG_DEBUG("hexagon_update_vtlb_entry_in_structure Exit");
}
static void hexagon_memw_read_buffer(struct target *target, uint64_t virt_address, uint32_t size, uint8_t *buffer)
{
    uint32_t count, i;
    uint8_t *temp;
    if (virt_address == 0x0)
    {
        LOG_DEBUG("virt_address address passed as NULL");
        return;
    }
    if ((size % 4) == 0)
    {
        count = size / 4;
        temp = buffer;
        for (i = 0; i < count; i++)
        {
            hexagon_memw_read(target, virt_address + 4 * i, (uint32_t *)temp);
            temp = temp + 4;
        }
    }
    else
    {
        LOG_DEBUG("size is not multiple of 4 bytes");
    }
}

/* this interface is to read the  memory via memw interface*/
static void hexagon_memw_read(struct target *target, uint64_t virt_address, uint32_t *value)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
    uint32_t isdb_mmode_cmd, isdb_cmd_status, isdbsts;
    uint64_t stuff_inst[] = {0x6ea8c000, 0x9180c007, 0x6707c029};
	/* Stuff instruction 0x6ea8c000-->{r0 =isdbmbxin} 0x9180c007-->{r7 = memw(r0+#0) } 0x6707c029-->{isdbmbxout=r7}*/
	int retval, i;
	
    isdb_mmode_cmd = 0x184;
	// isdb_mmode_cmd = hexagon_pack_isdbcmd(ISDBCMD_CMD_STUFF,ISDBCMD_MONITOR_LVL,
							// ISDBCMD_TNUM_MASK_THREAD(0));

	retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_ISDBMBXIN,virt_address);

	if (retval != ERROR_OK) 
			LOG_DEBUG("HEXAGON_ISDB_ISDBMBXIN return value is not OK");
	for (i=0; i < 3; i++)
	{
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst[i]);
		if (retval != ERROR_OK) 
				LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");

		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
		if (retval != ERROR_OK) 
            LOG_DEBUG("HEXAGON_ISDB_ISDBCMD  return value is not OK");
		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
		if (retval != ERROR_OK) 
				LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
				
		/* if isdb_cmd_status 0 in cmd was successfull in case of 1 failed */ 
	    isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
		if(isdb_cmd_status)
		{
			 LOG_DEBUG("ISDBcommand failed in monitor mode for i = %d", i);
			 return ;
		}
	}
	i=0;
	while(!(isdbsts & ISDBST_ISDB_MAILBOX_OUT))
	{

	    retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
		i++;
		if (i == 10)
			break;
	}
	if(!(isdbsts & ISDBST_ISDB_MAILBOX_OUT))
	{
		LOG_DEBUG("ISDBST status not set for mailbox so skiping reading it");
		return;
	}
	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBMBXOUT, value);
	
	hexagon_wait_loop();
    hexagon_wait_loop();

	if (retval != ERROR_OK) 
		LOG_DEBUG("HEXAGON_ISDB_ISDBMBXOUT read failed ");
}


/* This function is used to read memory word using memw_phys instruction  
	In this function we passed the physical address as an argument*/
// static void hexagon_memw_phys_read(struct target *target, target_addr_t phy_address, uint64_t *value)
static void hexagon_memw_phys_read(struct target *target, target_addr_t phy_address, uint32_t *value)

{
	struct hexagon_common * hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct adiv5_dap *swddp = hexa_info->dap;
    uint32_t isdb_mmode_cmd = 0x184, isdb_cmd_status, isdbsts;
    uint64_t phy_add[2], t_phy_address = 0;
	uint32_t stuff_inst[] = {0x6ea8c000,0x6ea8c001,0x9200e107,0x6707c029};
	int retval,i;
	
	/* Stuff Inst  0x6ea8c000-->{r0 = isdbmbxin} , 0x6ea8c001-->{r1 = isdbmbxin} ,0x9200e107-->{r7 = memw_phys(r0,r1)}
		0x6707c029-->{isdbmbxout = r7}  */

	t_phy_address = phy_address;
	if(t_phy_address == 0x0)
	{
		LOG_DEBUG("Physical address passed as NULL");
		return;
	}
	hexagon_r0_used_stuff = 1;
   	 hexagon_r1_used_stuff = 1;
	hexagon_r7_used_stuff = 1;

	phy_add[0] = t_phy_address & 0x7ff;
	phy_add[1] = t_phy_address >> 11;
    LOG_DEBUG("physical address= 0x%llx phy_add[0] = 0x%llx  phy_add[1] = 0x%llx",phy_address, phy_add[0],phy_add[1]);
	
	
	for (i=0 ; i < 2; i++)
	{
	// /*	
		retval = enable_dbg_sys_pwr(swddp);
		if (retval != ERROR_OK) 
				LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
	// */

		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_ISDBMBXIN,phy_add[i]);
		if (retval != ERROR_OK) 
				LOG_DEBUG("HEXAGON_ISDB_ISDBMBXIN return value is not OK");

		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst[i]);
		if (retval != ERROR_OK) 
				LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");

		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
		if (retval != ERROR_OK) 
				LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
		
		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
		if (retval != ERROR_OK) 
				LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
				
		/* if isdb_cmd_status 0 in cmd was successfull in case of 1 failed */ 
		    isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
		   if(isdb_cmd_status)
		   {
			 LOG_DEBUG("ISDB command failed in monitor mode for i = %d", i);
			 return ;
		  }
	}
	for (i=2; i < 4; i++)
	{
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst[i]);
		if (retval != ERROR_OK) 
				LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");

		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
		if (retval != ERROR_OK) 
				LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
		if (retval != ERROR_OK) 
				LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
				
		/* if isdb_cmd_status 0 in cmd was successfull in case of 1 failed */ 
	    isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
		if(isdb_cmd_status)
		{
			 LOG_DEBUG("ISDB command failed in monitor mode for i = %d", i);
			 return ;
		}
	}
	i=0;
	while(!(isdbsts & ISDBST_ISDB_MAILBOX_OUT))
	{

	    retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
		i++;
		if (i==10)
			break;
		// hexagon_wait_loop();
	}
	if(!(isdbsts & ISDBST_ISDB_MAILBOX_OUT))
	{
		LOG_DEBUG("ISDBST status not set for mailbox so skiping reading it isdbsts = 0x%x",isdbsts);
		return;
	}
	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBMBXOUT, value);
	if (retval != ERROR_OK) 
		LOG_DEBUG("HEXAGON_ISDB_ISDBMBXOUT read failed ");

}


static int hexagon_remove_breakpoint(struct target *target, struct breakpoint *breakpoint)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	LOG_DEBUG("Entering %s\n",__FUNCTION__);
	
#if 0
		if (target->state != TARGET_HALTED) {
			LOG_WARNING("target not halted");
			return ERROR_TARGET_NOT_HALTED;
		}
#endif
		if (breakpoint->is_set) 
		{
			hexagon_unset_breakpoint(target, breakpoint);
			if (breakpoint->type == BKPT_HARD)
				hexagon->brp_num_available++;
		}
		return ERROR_OK;
}

static int hexagon_unset_breakpoint(struct target *target, struct breakpoint *breakpoint)
{
	LOG_DEBUG("Entering %s\n",__FUNCTION__);
	int retval;
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct hexagon_brp *brp_list = hexagon->brp_list;


    if (!breakpoint->is_set)
    {
		LOG_WARNING("breakpoint not set");
		return ERROR_OK;
	}

    if (breakpoint->type == BKPT_HARD)
    {
		if ((breakpoint->address != 0) && (breakpoint->asid != 0)) 
		{
			LOG_DEBUG(" %s\t ----------------- %d\n",__FUNCTION__, __LINE__);
			//todo: this condition is true for linked BP, Need to explore more if same is applicable for hexagon??
		} 
		else 
		{
			LOG_DEBUG(" %s\t ----------------- %d\n",__FUNCTION__, __LINE__);
			int brp_i = breakpoint->is_set - 1;
            if ((brp_i < 0) || (brp_i >= hexagon->brp_num))
            {
				LOG_DEBUG("Invalid BRP number in breakpoint");
				return ERROR_OK;
			}
//			LOG_DEBUG("rbp %i control 0x%0" PRIx32 " value 0x%0" PRIx64, brp_i,
//				brp_list[brp_i].control, brp_list[brp_i].value);
			brp_list[brp_i].used = 0;
			brp_list[brp_i].value = 0;
			brp_list[brp_i].control = 0;

			int retrycount = 5;
			retval = ERROR_OK;

            do
            {
				if(brp_i<1)
				{
					//HW BP 0 config
					//todo: Migrate this work to a helper function. //bp_write_helper()
					retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_BRKPTPC0, brp_list[brp_i].value);
				}
				else
				{
					retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_BRKPTPC1, brp_list[brp_i].value);
				}

				if (retval != ERROR_OK) 
				{
					LOG_WARNING("BRKPTPC write failed Error code: %d", retval);
				}
				--retrycount;

			} while((retval != ERROR_OK) && (retrycount>0));

			if (retval != ERROR_OK) 
			{
				//After 5 retries we are unable to set the HW bp then return from here
				LOG_DEBUG("BRKPTPC write failed after 5 retries 0x%llx", brp_list[brp_i].value);
				return retval;
			}

			//HW breakpoint config/settings
			retval = ERROR_OK;
			retrycount = 5;

            do
            {
                if (brp_i < 1)
                {
					//todo: Migrate this work to a helper function. //bp_write_helper()
					retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_BRKPTCFG0, brp_list[brp_i].control);
				}
                else
                {
					retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_BRKPTCFG1, brp_list[brp_i].control);
				}

				if (retval != ERROR_OK) {
					LOG_WARNING("BRKPTCFG write failed Error code: %d", retval);
				}
				--retrycount;

			}
			while((retval != ERROR_OK) && (retrycount>0));

            if (retval != ERROR_OK)
            {
				//if after 5 retries we are unable to set the HW bp then return from here
				LOG_DEBUG("BRKPTCFG failed after 5 retries 0x%x", brp_list[brp_i].control);
				return retval;
		}
			breakpoint->is_set = 0;
			return ERROR_OK;
		}
	}
	else
	{
		LOG_DEBUG(" %s\t ----------------- %d\n",__FUNCTION__, __LINE__);
		//TODO: Delete sw bp
		union fourbyte val;
		
		val.byte[0] = breakpoint->orig_instr[0];
		val.byte[1] = breakpoint->orig_instr[1];
		val.byte[2] = breakpoint->orig_instr[2];
		val.byte[3] = breakpoint->orig_instr[3];
		
		LOG_DEBUG("Removing brkpt, writing back breakpoint->orig_instr = 0x%x", val.word);
		retval = hexagon_memw_write_instruction_memory(target, breakpoint->address, val.word, 1);	//replacing the original instruction in place of brkpt instruction
		//todo: check the endianness
		if (retval != ERROR_OK) 
			LOG_DEBUG("hexagon_memw_write_instruction_memory return value is not OK");
	}
	// hexagon_sync(target);



	breakpoint->is_set = 0;
	return ERROR_OK;
}


static int hexagon_add_breakpoint(struct target *target, struct breakpoint *breakpoint)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	uint64_t bpconfig = 0;
	
	bpconfig |=(1<<17); 	//BRKPTPC match enable


	if ((breakpoint->type == BKPT_HARD) && (hexagon->brp_num_available  <  1)) 
	{
		LOG_DEBUG("no hardware breakpoint available");
		return ERROR_TARGET_RESOURCE_NOT_AVAILABLE;
	}
	if (breakpoint->type == BKPT_HARD)
		hexagon->brp_num_available--;
	
	return hexagon_set_breakpoint(target, breakpoint, bpconfig);	//address match enable
}

static int hexagon_setup_isdb_config(struct target* target, uint64_t old_isdbcfg0, uint8_t hbp_num)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	int32_t isdbcfg0 = old_isdbcfg0 , retval = ERROR_OK;


	isdbcfg0|=0x3f3f3f;
    if (hbp_num == 0)
    {
		isdbcfg0|=(1<<25);	  //HW BP0 enable
	}
    else if (hbp_num == 1)
    {
		isdbcfg0|=(1<<26);	   //HW BP1 enable
	}
		
	LOG_DEBUG("Writing isdbcfg0...	 isdbcfg0 = 0x%x", isdbcfg0);

	retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
				hexa_info->debug_base + HEXAGON_ISDB_ISDBCFG0, isdbcfg0);
    if (retval != ERROR_OK)
    {
			LOG_DEBUG("ISDBCFG0 write failed 0x%x", isdbcfg0);
			return retval;
	}
	//disabling these lines as T32+EUD HBP logs doesn't do any write on isdbcfg1, Can enable later.
	int32_t isdbcfg1 = 0;
	retval = ERROR_OK;
	isdbcfg1|= 0x3F3F3F00;	   //HW break0 & break1 TNUM mask SW Break TNUM Mask

	LOG_DEBUG("Writing isdbcfg1...	 isdbcfg1 = 0x%x", isdbcfg1);
	retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
				hexa_info->debug_base + HEXAGON_ISDB_ISDBCFG1, isdbcfg1);
    if (retval != ERROR_OK)
    {
			LOG_DEBUG("ISDBCFG1 write failed 0x%x", isdbcfg1);
			return retval;
	}
	LOG_DEBUG("Write sccessful for HEXAGON_ISDB_ISDBCFG0 & HEXAGON_ISDB_ISDBCFG1");

	return ERROR_OK;
}

	
/** Setup hardware Breakpoint Register Pair bpconfig parameter will be considered for setting up on-chip breakpoints, otherwise ignored **/
static int hexagon_set_breakpoint(struct target *target, struct breakpoint *breakpoint, uint64_t bpconfig)
{
	LOG_DEBUG("Entering %s\n",__FUNCTION__);
	int retval;
	int brp_i = 0;
	uint32_t isdbcfg0 = 0;
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct hexagon_brp *brp_list = hexagon->brp_list;
	struct adiv5_dap *swddp = hexa_info->dap;
	
    if (breakpoint->is_set)
    {
		LOG_WARNING("breakpoint already set");
		return ERROR_OK;
	}
	
	retval = ERROR_OK;
	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBCFG0, &isdbcfg0);
    if (retval != ERROR_OK)
    {
		LOG_DEBUG("ISDBCFG0 read failed 0x%x", isdbcfg0);
		return retval;
	}
	LOG_DEBUG("Reading isdbcfg0...	 isdbcfg0 = 0x%x", isdbcfg0);

    if (breakpoint->type == BKPT_HARD)
    {
		
		LOG_DEBUG(" %s\t :-------: %d breakpoint->type == BKPT_HARD",__FUNCTION__, __LINE__);
        if (breakpoint->length != 4)
        {
			LOG_DEBUG("bug: breakpoint length should be 4");	//REVIEW: Not applicable if this check has been already done, also NMI bp address can be VA, VA+ASID, PA.
			return ERROR_FAIL;
		}
		LOG_DEBUG(" %s\t :-------: %d\n",__FUNCTION__, __LINE__);
				
		LOG_DEBUG(" %s\t :-------: %d\t brp_i = %d\n",__FUNCTION__, __LINE__, brp_i);
		LOG_DEBUG(" %s\t :-------: %d\t brp_list = %p\n",__FUNCTION__, __LINE__, (void *) brp_list);
		// trying to print struct hexagon_brp which does not have a valid datatype
		// instead we can print the value of the breakpoint
		LOG_DEBUG(" %s\t :-------: %d\t brp_list[brp_i] = %llx\n",__FUNCTION__, __LINE__, brp_list[brp_i].value);
		LOG_DEBUG(" %s\t :-------: %d\t brp_list[brp_i].used = %x\n",__FUNCTION__, __LINE__, brp_list[brp_i].used);
		
        while ((brp_list[brp_i].used) && (brp_i < hexagon->brp_num))
        { // brp_i < 2; 2 HW BP supported till hexagon V71, should have a MACRO for future versions
			brp_i++;
		}
        if (brp_i >= hexagon->brp_num)
        {
			LOG_DEBUG("ERROR Can't add more HW breakpoints");
			return ERROR_TARGET_RESOURCE_NOT_AVAILABLE;
		}
		breakpoint->is_set = brp_i + 1;		//REVIEW: NMI Significance of breakpoint->is_set field, from the intuition, it should be 1/0. Keeping as it is AARCH64

		brp_list[brp_i].used = 1;
		brp_list[brp_i].value = breakpoint->address & 0xFFFFFFFFFFFFFFFC;		//keeping 64 bit for now, last 2 bit is 0 for 32bit alignment
		brp_list[brp_i].control = bpconfig; 									//REVIEW: assuming this will hold the value of BRKPTCFG0/1 for HW bp
//		bpt_value = brp_list[brp_i].value;

		//TODO: Check if T32 checks for system halted then sets HW BP, or during threads in RUN mode it sets the BP
		int retrycount;
		uint64_t brkptpc, brkptcfg;

		brkptpc = brkptcfg = retrycount = 0;
		retval = ERROR_OK;

		//HW breakpoint PC address write
		retrycount = 5;
				
	// /*	
		retval = enable_dbg_sys_pwr(swddp);
		if (retval != ERROR_OK) {
			LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
		}
	// }*/
		//HW breakpoint config/settings
		retval = ERROR_OK;
		brkptcfg = brp_list[brp_i].control;
		LOG_DEBUG(" %s\t :-------: %d\t :------------: brkptcfg = %llx\n",__FUNCTION__, __LINE__, brkptcfg);
		retrycount = 5;
		
		retval = enable_dbg_sys_pwr(swddp);
        // if (retval != ERROR_OK)
        // {
		// 	LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
		// }
        do
        {
            if (brp_i < 1)
            {
				//HW BP 0 config
				//todo: Migrate this work to a helper function. //bp_write_helper()
				retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_BRKPTCFG0, brkptcfg);
			}
            else
            {
				retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_BRKPTCFG1, brkptcfg);
		}

            if (retval != ERROR_OK)
            {
				LOG_WARNING("BRKPTCFG write failed Error code: %d", retval);
			}
			--retrycount;

			}while((retval != ERROR_OK) && (retrycount>0));

        if (retval != ERROR_OK)
        {
			//if after 5 retries we are unable to set the HW bp then return from here
			LOG_DEBUG("BRKPTCFG failed after 5 retries 0x%llx", brkptcfg);
		return retval;
		}
			
	brkptpc = brp_list[brp_i].value;
	LOG_DEBUG(" %s\t :-------: %d\t :------------: brkptpc = %llx\n",__FUNCTION__, __LINE__, brkptpc);
	retval = ERROR_OK;
	
	do 
	{
			if(brp_i<1)
			{
				//HW BP 0 PC
					//todo: Migrate this work to a helper function. //bp_write_helper()
					retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_BRKPTPC0, brkptpc);
				hexagon_setup_isdb_config(target, isdbcfg0, 0);
			}
            else
            {
				retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_BRKPTPC1, brkptpc);
				hexagon_setup_isdb_config(target, isdbcfg0, 1);
			}

            if (retval != ERROR_OK)
            {
				LOG_WARNING("BRKPTPC write failed Error code: %d", retval);
	}
		--retrycount;

	}while((retval != ERROR_OK) && (retrycount>0));
			
        if (retval != ERROR_OK)
        {
		//After 5 retries we are unable to set the HW bp then return from here
		LOG_DEBUG("BRKPTPC write failed after 5 retries 0x%llx", brkptpc);
		return retval;
	}
	
//	hexagon_dump_isdb_reg(&hexagon->hexa_info);
//	hexagon_read_BRKPT_through_stuff(target);
	
	} 
    else if (breakpoint->type == BKPT_SOFT)
    {
		retval = ERROR_OK;
		
		uint64_t first_instrn_addr, current_addr;
		first_instrn_addr = 0;
		current_addr = breakpoint->address;
//		uint8_t code_arr[20]= {0};
//		uint8_t fwd_code_arr[4]= {0};
		uint8_t bkwd_code_arr[4]= {0}, packet_counter = 0;
		union fourbyte  val;
		
		LOG_DEBUG("In software BP ");
        if ((breakpoint->length != 4) && (breakpoint->address == 0))
        {
			LOG_DEBUG("bug: breakpoint length should be 4 for sw breakpoints/address should not be null");
			return ERROR_FAIL;
		}
		LOG_DEBUG("Breakpoint address received from gdb address = 0x%llx", current_addr);

		val.byte[0] = val.byte[1] = val.byte[2] = val.byte[3] = 0;
		//find First instruction in the packet, There's no specific bit pattern for first instruction of current packet,
		//find out last instruction in previous packet & the next instruction will be first instruction of the current packet
		first_instrn_addr = current_addr;
		uint8_t first_instrn_found = 0;
		while(!first_instrn_found)
		{
			packet_counter++;
			first_instrn_addr-=4;	//start looking from the next address from the current instruction
			retval = ERROR_OK;
			retval = hexagon_read_buffer (target, first_instrn_addr, 4, bkwd_code_arr);
			
			if(packet_counter >= 10)
			{
				LOG_DEBUG("First instruction not found after 10 memory read, unable to set software berakpoint");
				return ERROR_FAIL;
			}
			if (retval != ERROR_OK)
			{ 
				LOG_DEBUG("hexagon_memw_write_instruction_memory return value is not OK retval = %d", retval);
				continue;
			}
			LOG_DEBUG("the Opcode present at address 0x%llx is 0x%x%x%x%x", \
				first_instrn_addr, bkwd_code_arr[3], bkwd_code_arr[2], bkwd_code_arr[1], bkwd_code_arr[0]);
			
			val.byte[0] = bkwd_code_arr[0];
			val.byte[1] = bkwd_code_arr[1];
			val.byte[2] = bkwd_code_arr[2];
			val.byte[3] = bkwd_code_arr[3];
			
			if(((val.word & (3<<14))>>14) == 0b11)
			{
				first_instrn_addr+=4;
				LOG_DEBUG("First instruction of the packet found at address 0x%llx", first_instrn_addr);
				first_instrn_found = 1;
			}

		}
		retval = ERROR_OK;
		retval = hexagon_read_buffer(target, first_instrn_addr, 4, bkwd_code_arr);
		if (retval != ERROR_OK) 
			LOG_DEBUG("hexagon_memw_write_instruction_memory return value is not OK retval = %d", retval);
		
		//replacing first instruction address instead of the breakpoint->address, because once the packet execution will start, 
		//instruction decoder will pick up/decode brkpt instruction & assume that it's the only instruction in the packet. 
		//so there's no need of replacing the rest of the instructions in the packet
		
		retval = hexagon_memw_write_instruction_memory(target, first_instrn_addr, 0x6c20c000, 0);	//value will be ignored hard coded value is 0x6c20c000	{	 brkpt }
		if (retval != ERROR_OK) 
			LOG_DEBUG("hexagon_memw_write_instruction_memory return value is not OK");
		
		
		//Modifying the breakpoint attributes passed by GDB.
		//Todo: verify this
		//orig_instr[0-3] = bkwd_code_arr[0-3]
		breakpoint->is_set = 1;
		memcpy(breakpoint->orig_instr, bkwd_code_arr, 4);
		breakpoint->address = first_instrn_addr;
		
		hexagon_setup_isdb_config(target, isdbcfg0, 0xFF);
		
		//debug purpose to check if the brkpt instruction is written correctly.
		
		retval = ERROR_OK;
		// hexagon_sync(target);

		retval = hexagon_read_buffer (target, first_instrn_addr, 4, bkwd_code_arr);


        if (retval != ERROR_OK)
        {
			LOG_DEBUG("hexagon_memw_read_instruction_memory return value is not OK retval = %d", retval);
			
	}
		LOG_DEBUG("Debug: after writing breakpoint The Opcode present at address 0x%llx is 0x%x%x%x%x", \
			first_instrn_addr, bkwd_code_arr[3], bkwd_code_arr[2], bkwd_code_arr[1], bkwd_code_arr[0]);
		
		//		buf_set_u32(code, 0, 32, opcode);
		/*
		 * //TODO: 0x6c20c000  {	brkpt } opcode for brkpt instruction, Prior to this ISDBCFG1[11:8] SWBRKPT TNUM mask should be programmed correctly
		//todo: following sequence is followed for setting a sw bp
		//1. Read sysconfig to determine the cache status l1, l2 enabled/disabled??
		//2. Imp bits L2CFG?? L2NWA?? L2NRA?? L2WB??
		 * Implement read_memory(target, address, size, count, buffer);
		 * 13.8.2.1.3 Reading from L1 data cache plus L2 cache plus backing memory
			Load data and return the cached copy if a hit, and the bus copy if a miss, but do not allocate a line
			on a miss. This allows the debugger to non-intrusively inspect the processor�s view of the data. To
			perform this task, the debugger is to use the following procedure:
			1. Set (1) the SYSCFG[L2NRA] bit to force no-read-allocate in L2 cache.
			2. Set (1) the SYSCFG[L2NWA] bit to force no-write-allocate in L2 cache.
			3. Clear (0) the SYSCFG[L2WB] bit to force write-through in L2 cache.
			4. Ensure that the L1 and L2 caches are enabled.
			5. Execute a memw_phys instruction to read the physical address.
			6. This instruction returns the version in cache if found, but it does not allocate a line if missed.
			Because L1 cache and L2 cache are disabled, it reads the backing store.
		//3. replace PC/symbol address opcode with "brkpt"
		//4. Program ISDBCFG1[11:8] sw bp TNUM mask
		//5. resume the threads.
		 *
		 * To Keep In Mind:
		 * brkpt instructions cannot be packetized with other instructions.
		 *
		 *
//		*/

		return ERROR_OK;
	}
	

	return retval;
}

/*This function write to instruction memory in this function if flag is passed as 1 means we are passing the value to write otherwise 
we need to use the brpkt instruction opcode to write into the meory*/

static int hexagon_memw_write_instruction_memory(struct target *target,uint64_t virt_address, uint32_t value, uint8_t flag)
{
	

	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct adiv5_dap *swddp = hexa_info->dap;
	uint32_t isdb_mmode_cmd = 0x184, isdb_cmd_status,isdbsts;
	uint64_t stuff_inst[] = {0x6ea8c000,0x6ea8c007,0xa180c700,0xa000c000,0xa800c000,0x56c0c000,0x57c0c002};
	uint64_t address[2];
	int retval = 0, i = 0;
	// target_addr_t phy_addr =0 ;

	/* Stuff instruction  
	0x6ea8c000-->{r0 = isdbmbxin } 0x6ea8c007-->{r7 = isdbmbxin } 0xa180c700-->{memw(r0+#0) = r7}
	0xa000c000-->{dccleana(r0) } 0xa800c000-->{barrier } 0x56c0c000-->{icinva(r0) } 0x57c0c002-->{isync} */

	#ifdef  _HEXAGON_TARGET_TIME_PROFILING
		hexagon_start_time_cal_ms();
	#endif
	
	LOG_DEBUG("hexagon_memw_write_instruction_memory  Enter");

    // if (mmu_init)
	// 	retval=  hexagon_virt2phys(target, virt_address, &phy_addr);
    // else
    //     phy_addr = virt_address;

	// if(retval == ERROR_FAIL)
	// {
	// 	LOG_DEBUG("There is no TLB mapping for virtual address  = 0x%p ", address);
	// 	return retval;
	// }
	hexagon_r0_used_stuff = 1;
	hexagon_r7_used_stuff = 1;

	if(!flag)
		value = 0x6c20c000;   //brpkt instruction opcode
	
	address[0]= virt_address;
	address[1]= value;

    LOG_DEBUG("virt_address =  0x%llx and value = 0x%x", virt_address, value);
	// /*
	retval = enable_dbg_sys_pwr(swddp);
	// if (retval != ERROR_OK) 
	// 		LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
	// */

	for(i = 0;  i < 2;  i++)
	{
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
								hexa_info->debug_base + HEXAGON_ISDB_ISDBMBXIN, address[i]);
		if (retval != ERROR_OK) 
				LOG_DEBUG("HEXAGON_ISDB_ISDBMBXIN return value is not OK");
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
								hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst[i]);
		if (retval != ERROR_OK) 
				LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
								hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
		if (retval != ERROR_OK) 
				LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
		if (retval != ERROR_OK) 
				LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
					
		/* if isdb_cmd_status 0 in cmd was successfull in case of 1 failed */ 
	   	 isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
		if(isdb_cmd_status)
		{
			 LOG_DEBUG("ISDBcommand failed in monitor mode for i = %d", i);
			 return ERROR_FAIL;
		}
	}
	
	for(i = 2; i < 7;  i++)
	{
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
								hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst[i]);
		if (retval != ERROR_OK) 
				LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
								hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
		if (retval != ERROR_OK) 
				LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
		if (retval != ERROR_OK) 
				LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
					
		/* if isdb_cmd_status 0 in cmd was successfull in case of 1 failed */ 
	    	isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
		if(isdb_cmd_status)
		{
			 LOG_DEBUG("ISDBcommand failed in monitor mode" );
			 return ERROR_FAIL;
		}
	}
	#ifdef  _HEXAGON_TARGET_TIME_PROFILING
    hexagon_end_time_cal_ms();
		LOG_DEBUG("Total time taken  %" PRId64 "ms", hexagon_time_total);
	#endif

	LOG_DEBUG("hexagon_memw_write_instruction_memory Exit");
	return ERROR_OK;
}


static int hexagon_write_buffer (struct target *target, target_addr_t address,
		uint32_t size, const uint8_t *buffer)
{
	int retval = ERROR_OK;
	target_addr_t phy_addr = 0;
	uint32_t value= 0;
	
	if(address == 0x0)
	{
		LOG_DEBUG("Virtual address passed as NULL");
		return ERROR_FAIL;
	}
	
	#ifdef  _HEXAGON_TARGET_TIME_PROFILING
			hexagon_start_time_cal_ms();
	#endif
		
    if (!mmu_init)
	{	
		LOG_INFO("writes can be performed only after MMUs are enabled, memory write failed");
		return retval;
	}

	LOG_DEBUG("address  = 0x%llx and size = %d , phy add = 0x%llx", address,size,phy_addr);
		
	if(retval == ERROR_FAIL)
	{
		LOG_DEBUG("There is no TLB mapping for virtual address	= 0x%llx ", address);
		return retval;
	}
	if (size == 3 )
	{
		memcpy(&value, buffer,2);
		retval = hexagon_memw_write(target,address, value, 2);

		value = 0;
		//  requires a pointer, not the value of buffer[2]
		memcpy(&value, &buffer[2],1);
		retval = hexagon_memw_write(target,address, value, 1);

	}
	else if(size <= 4)
	{
		memcpy(&value, buffer,size);
		retval = hexagon_memw_write(target,address, value,size);

	}
	else if ((size % 4) == 0)
	{
		retval = hexagon_memw_write_buffer(target, address, size, buffer);

	}
		
	#ifdef  _HEXAGON_TARGET_TIME_PROFILING
			hexagon_end_time_cal_ms();
			LOG_DEBUG("Total time taken  %" PRId64 "ms", hexagon_time_total);
	#endif

	

	return retval;
}

/*this function is use to write memory buffer using memw interface */
static int hexagon_memw_write_buffer(struct target *target, uint64_t virt_address, uint32_t size, const uint8_t  * buffer)
{
	uint32_t count,i,value,retval = ERROR_OK;
	// Changed temp to const uint8_t * to preserve the const qualifier and avoid warning
	const uint8_t * temp;

	count = size/4;
	temp = buffer;

	for (i=0; i < count;  i++)
	{
		memcpy(&value, temp, 4);
		retval = hexagon_memw_write(target, virt_address + 4*i , value, 4);
		temp = temp+4;
	}
	return retval;
	

}


/*This function used to write memory using memw interface  */
static int hexagon_memw_write(struct target *target,uint64_t virt_address, uint32_t value, uint32_t size)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct adiv5_dap *swddp = hexa_info->dap;
	uint32_t isdb_mmode_cmd = 0x184, isdbsts = 0,sys_cfg;
	uint32_t stuff_inst[] = {0x6ea8c000, 0x6ea8c007, 0xa180c700, 0xa000c000, 0xa800c000, 0xa840c000};
	// uint32_t stuff_inst[] = {0x6ea8c01f, 0x6ea8c01f, 0xab1edf08, 0x6ea8c01e, 0x6ea8c01f, 0x921ffe1f, 0x671fc029 ,0xa800c000, 0xa840c000 };

    uint64_t address[2];
	int retval,i = 0;

	/* 
	opcodes[0x7190] <+176>:  0x6ea8c01e  {  r30 = isdbmbxin }
	opcodes[0x7194] <+180>:  0x6ea8c01f  {  r31 = isdbmbxin }
	opcodes[0x7198] <+184>:  0xab1edf08  {  memb(r30++#0x1) = r31 }
	opcodes[0x719c] <+188>:  0x6ea8c01e  {  r30 = isdbmbxin }
	opcodes[0x71a0] <+192>:  0x6ea8c01f  {  r31 = isdbmbxin }
	opcodes[0x71a4] <+196>:  0x921ffe1f  {  r31 = memw_phys(r31,r30) }
	opcodes[0x71a8] <+200>:  0x671fc029  {  isdbmbxout = r31 }

	Stuff inst  
	0x6ea8c01f-->{r30 = isdbmbxin }
	0x6ea8c01f-->{r31 = isdbmbxin} 
	0xa180c700-->{memb(r0+#0) = r31 } 
	0xa000c000-->{dccleana(r0) } 0xa800c000-->{barrier } 0xa840c000-->{syncht } 

	Stuff inst  0x6ea8c000-->{r0 = isdbmbxin }0x6ea8c007-->{r7 = isdbmbxin} 
	0xa180c700-->{memw(r0+#0) = r7 } 0xa000c000-->{dccleana(r0) } 0xa800c000-->{barrier } 0xa840c000-->{syncht } 
	*/

	LOG_DEBUG("hexagon_memw_write Enter");


	LOG_DEBUG("size is %d", size);

	if(size == 1)
	{
		stuff_inst[2] = 0xa100c700;  /* {memb(r0+#0) = r7} */
		// stuff_inst[2] = 0xab1edf08;
		// LOG_DEBUG("size is %d", size );

		// retval =  hexagon_virt2phys(target, virt_address, &address[0]);		

	}
	if(size == 2)
	{
		stuff_inst[2] = 0xa140c700;  /* {memh(r0+#0) = r7} */
		// LOG_DEBUG("size is %d", size );
		// address[0]= virt_address;ss


	}
	LOG_DEBUG("address is 0x%llx\nvalue to be written is 0x%x", virt_address, value );

	address[0]= virt_address;
	address[1]= value;

	// LOG_DEBUG("address = 0x%x, value = 0x%x", address, value);
	LOG_DEBUG("address[0] = 0x%llx, address[1] = 0x%llx", address[0], address[1]);

	// /*
	retval = enable_dbg_sys_pwr(swddp);
	// if (retval != ERROR_OK) 
	// 		LOG_DEBUG("enable_dbg_sys_pwr return value is not OK"); 
	// */

	hexagon_r0_used_stuff = 1;
	hexagon_r7_used_stuff = 1;

	retval = hexagon_read_syscfg_register(target);
	if(retval == ERROR_OK)
	{
		sys_cfg = hexagon_syscfg_reg;
		sys_cfg = sys_cfg | SYSCFG_L2NRA | SYSCFG_L2NWA;
		sys_cfg = sys_cfg & ~(SYSCFG_L2WB);
		LOG_DEBUG("Writing value in SYSCFG  = 0x%x ", sys_cfg);
		hexagon_write_syscfg_register(target,sys_cfg);
		
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
								hexa_info->debug_base + HEXAGON_ISDB_STFINST, 0xa840c000);
		if (retval != ERROR_OK) 
				LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
								hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
		if (retval != ERROR_OK) 
				LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
		
		retval = hexagon_read_ISDB(target, isdbsts, ISDBST_ISDB_CMD_STATUS);

	} 
	for(i = 0; i < 2;  i++)
	{
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
								hexa_info->debug_base + HEXAGON_ISDB_ISDBMBXIN, address[i]);
		LOG_DEBUG ("writing address[i] as 0x%llx", address[i]);
	
		if (retval != ERROR_OK) 
				LOG_DEBUG("HEXAGON_ISDB_ISDBMBXIN return value is not OK");
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
								hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst[i]);
		if (retval != ERROR_OK) 
				LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
								hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
		if (retval != ERROR_OK) 
				LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
		
		retval = hexagon_read_ISDB(target, isdbsts, ISDBST_ISDB_CMD_STATUS);

	}
	for(i = 2; i < 6 ; i++)
	{
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
								hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst[i]);
		if (retval != ERROR_OK) 
				LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
								hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
		if (retval != ERROR_OK) 
				LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");

		
		retval = hexagon_read_ISDB(target, isdbsts, ISDBST_ISDB_CMD_STATUS);
	}
	LOG_DEBUG("hexagon_memw_write  Exit");
	return ERROR_OK;
}


static int hexagon_read_buffer (struct target *target, target_addr_t address,
		uint32_t size, uint8_t *buffer)
{
	int retval = ERROR_OK;
    long long int phy_addr = 0;
	uint32_t value;

#ifdef  HEXAGON_DEBUG_LOGS
    start_buffer = clock();
#endif
	
	

	if(address == 0x0)
	{
		LOG_DEBUG("Virtual address passed as NULL");
		return ERROR_FAIL;
	}
	#ifdef  _HEXAGON_TARGET_TIME_PROFILING
			hexagon_start_time_cal_ms();
	#endif
		
    if (mmu_init) //after QURTOS_INIT only memw reads with VA
	{
		// retval=  hexagon_virt2phys(target, address,&phy_addr);

		if(size < 4)
		{

			hexagon_memw_read(target, address, &value);
			LOG_DEBUG("retrieved value using  hexagon_memw_read for address = 0x%llx \
						 size < 4  = 0x%x and value = 0x%x", address, size, value);
			switch(size)
			{
				case 1:
					buffer[0] = (uint8_t) (value >> 0);
					break;
				case 2: 
					target_buffer_set_u16(target, buffer, value);
					break;
				case 3:
					target_buffer_set_u24(target, buffer, value);
					break;
			}
		}
		else if (size == 4)
		{
			hexagon_memw_read(target, address, &value);
			LOG_DEBUG("retrieved value using  hexagon_memw_read for address = 0x%llx \
						 size == 4  = 0x%x and value = 0x%x ", address, size, value);

			target_buffer_set_u32(target, buffer , value);
		}
		else if ((size % 4) == 0 )
		{
			hexagon_memw_read_buffer(target,address,size, buffer);
			LOG_DEBUG("hexagon_memw_read_buffer for address = 0x%llx for size = 0x%x", address, size);

		}
	}

    else if (!mmu_init) // this is for SW_ENTRY till QURTOS_init 
    {  	
		phy_addr = address;

		//LOG_DEBUG("hexagon_read_buffer address  = 0x%x and size = %u , phy add = 0x%x", (uint32_t)address,size,(uint32_t)phy_addr);
	
		if(retval == ERROR_FAIL)
		{
			LOG_DEBUG("There is no TLB mapping for virtual address	= 0x%x ", (uint32_t)address);
			return retval;
		}
	
		if(size < 4)
		{

			hexagon_memw_phys_read(target,phy_addr, &value);
			LOG_DEBUG("retrieved value using  hexagon_memw_phys_read for address = 0x%llx \
						 size < 4  = 0x%x and value = 0x%x ", phy_addr, size, value);
			switch(size)
			{
				case 1:
					buffer[0] = (uint8_t) (value >> 0);
					break;
				case 2: 
					target_buffer_set_u16(target, buffer, value);
					break;
				case 3:
					target_buffer_set_u24(target, buffer, value);
					break;
			}
		}
		else if (size == 4)
		{
			hexagon_memw_phys_read(target,phy_addr, &value);
			LOG_DEBUG("retrieved value using  hexagon_memw_phys_read for address = 0x%llx \
						 size == 4  = 0x%x and value = 0x%x ", phy_addr, size, value);
			target_buffer_set_u32(target, buffer , value);
		}
		else if ((size % 4) == 0 )
		{
			hexagon_memw_phys_read_buffer(target,phy_addr,size, buffer);
			LOG_DEBUG("hexagon_memw_phys_read_buffer for address = 0x%llx for size = 0x%x", address, size);

		}
	}
#ifdef HEXAGON_DEBUG_LOGS
    end_buffer = clock();
    buffer_execution = ((double)(end_buffer - start_buffer))/CLOCKS_PER_SEC;
    // LOG_INFO("AFTER read_buffer %lf", buffer_execution);
#endif

	

	return retval;
}

/* This function is used to read memory of request size using memw_phys  instruction  
	In this function we passed the physical address as an argument */
static void hexagon_memw_phys_read_buffer(struct target *target,target_addr_t phy_address, uint32_t size, uint8_t * buffer)
{
	uint32_t count, i;
	uint8_t * temp;

	

	if(phy_address == 0x0)
	{
		LOG_DEBUG("Physical address passed as NULL");
		return;
	}
	if ((size % 4) == 0)
	{
		count = size/4;
		temp = buffer;
		for (i=0; i < count; i++)
		{
			// hexagon_memw_phys_read(target, phy_address+ 4*i, (uint64_t*) temp);
			hexagon_memw_phys_read(target, phy_address+ 4*i, (uint32_t *) temp);
			temp = temp+4;
		}
	}
	else
	{
		LOG_DEBUG("size is not multiple of 4 bytes");
	}

}

static int hexagon_read_syscfg_register(struct target *target)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct adiv5_dap *swddp = hexa_info->dap;
	int retval,i=0;
    uint32_t isdbsts,isdb_mmode_cmd, isdb_cmd_status;
    /*Stuff inst  {r7 = syscfg}  {isdbmbxout = r7} */

	hexagon_r7_used_stuff  = 1;
	//LOG_DEBUG("Enter in hexagon_read_syscfg_register");

	isdb_mmode_cmd = hexagon_pack_isdbcmd(ISDBCMD_CMD_STUFF,ISDBCMD_MONITOR_LVL,
							ISDBCMD_TNUM_MASK_THREAD(0));

	

	/* Monitor mode */
	// /*
	retval = enable_dbg_sys_pwr(swddp);
	if (retval != ERROR_OK) 
		LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
	// */

	retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
	hexa_info->debug_base + HEXAGON_ISDB_STFINST, 0x6e92c007);
	if (retval != ERROR_OK) 
		LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");

	retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
				hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
	if (retval != ERROR_OK) 
		LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");

	/* wait till the stuff instruction is executed */
	// hexagon_wait_loop();
	
	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
	if (retval != ERROR_OK) 
		LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);

	/* 0 - cmd sucessfull, 1 - failure */
	isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
	if (isdb_cmd_status)
	{
		 LOG_DEBUG("ISDBcommand failed in monitor mode");
		 return ERROR_OK;
	}
	/*there are 2 stuff instruction here programming second inst */
	// /*
	retval = enable_dbg_sys_pwr(swddp);
	// if (retval != ERROR_OK) 
	// 	LOG_DEBUG("enable_dbg_sys_pwr return value is not OK"); 
	// */

	retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
			  hexa_info->debug_base + HEXAGON_ISDB_STFINST,0x6707c029);
	if (retval != ERROR_OK) 
		LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");

	retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
	
	if (retval != ERROR_OK) 
		LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");

	/* wait till the stuff instruction is executed */
	// hexagon_wait_loop();
				
	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
	
	if (retval != ERROR_OK) 
		LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
	

	/* 0 - cmd success, 1 - cmd failure */ 
	isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
	if (isdb_cmd_status)
	{
		 LOG_DEBUG("ISDBcommand failed in monitor mode");
		 return ERROR_OK;
	}
	while(!(isdbsts & ISDBST_ISDB_MAILBOX_OUT))
	{
		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);

		hexagon_wait_loop();
		i++;
		if (i==10)
			break;
	}

	if(!(isdbsts & ISDBST_ISDB_MAILBOX_OUT))
	{
		LOG_DEBUG("ISDBST status not set for mailbox so skiping reading SYSCFG register ");
		return ERROR_FAIL;
	}
	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBMBXOUT, &hexagon_syscfg_reg);
	if (retval != ERROR_OK) 
	{
        LOG_DEBUG("HEXAGON_ISDB_ISDBMBXOUT read failed 0x%x", hexagon_syscfg_reg);
	}
	//LOG_DEBUG("SYSCFG register value =  0x%x", hexagon_syscfg_reg);
	
	return ERROR_OK;
}


/* This function is used to write value in syscfg register */
static int hexagon_write_syscfg_register(struct target *target,uint32_t value)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct adiv5_dap *swddp = hexa_info->dap;
	int retval,i;
	uint32_t isdbsts;
	uint32_t isdb_mmode_cmd, isdb_cmd_status;
	uint32_t stuff_inst[] = {0x6ea8c007  ,0x6707c012,0x57c0c002,0xa840c000};
	/* Stuff instruction  0x6ea8c007-->{r7 = isdbmbxin } 0x6707c012-->{syscfg = r7} 0x57c0c002--> { isync }, 0xa840c000-->  { syncht } */
	

	hexagon_r7_used_stuff = 1;



	/* Monitor mode */
	// /*
	retval = enable_dbg_sys_pwr(swddp);
	if (retval != ERROR_OK) 
		LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
	// */
	
	isdb_mmode_cmd = hexagon_pack_isdbcmd(ISDBCMD_CMD_STUFF, ISDBCMD_MONITOR_LVL,
		  								ISDBCMD_TNUM_MASK_THREAD(0));
	


	retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
	hexa_info->debug_base + HEXAGON_ISDB_ISDBMBXIN, value);


	if (retval != ERROR_OK) 
        LOG_DEBUG("HEXAGON_ISDB_ISDBMBXIN return value is not OK");

	for(i = 0;  i < 4;  i++)
	{


		retval = enable_dbg_sys_pwr(swddp);
		if (retval != ERROR_OK) 
			LOG_DEBUG("enable_dbg_sys_pwr return value is not OK"); 
		// */
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst[i]);



		if (retval != ERROR_OK) 
			LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");

		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
				hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
		if (retval != ERROR_OK) 
			LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");


		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
		if (retval != ERROR_OK) 
			LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);

		/* 0 - cmd sucessfull, 1 - failure */
		isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
		if (isdb_cmd_status)
		{
			 LOG_DEBUG("ISDBcommand failed in monitor mode");
			 return ERROR_FAIL;
		}
	}

	return ERROR_OK;
		
}






// /* This function is used to restore r7 used during stuff */
// static void hexagon_stuff_reg_restore_r7(struct target *target)
// {	
	
// 	hexagon_write_gpr_register(target, HEXAGON_R7, HEXAGON_HW_THREAD0, 
// 					gpPerHwThrdReg[HEXAGON_HW_THREAD0][HEXAGON_R7]);
	
// }

static int hexagon_read_memory(struct target *target, target_addr_t address,
	uint32_t size, uint32_t count, uint8_t *buffer)
{
	int retval = ERROR_OK;
	LOG_DEBUG("address  = 0x%llx and size = %u,count=%u  ", address,size,count);
	return retval;
}

static int hexagon_write_memory(struct target *target, target_addr_t address,
	uint32_t size, uint32_t count, const uint8_t *buffer)
{
	int retval = ERROR_OK;
	
	LOG_DEBUG("address  = 0x%llx and size = %u,count=%u  ", address,size,count);
	

	return retval;
}


int hexagon_get_gdb_reg_list(struct target *target,
	struct reg **reg_list[], int *reg_list_size,
	enum target_register_class reg_class)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct reg_cache *cache, *global;
	
	int i, x;
	
	switch (reg_class) 
	{
		case REG_CLASS_GENERAL:
		*reg_list_size = HEXAGON_GPR_REGS * 2;	//try with 1 hw thread.
		*reg_list = malloc(sizeof(struct reg *) * (*reg_list_size));
		memset(*reg_list, 0, sizeof(struct reg *) * (*reg_list_size));
		/* dump GPR for all Hw thread */
		x = 0;
		cache = hexa_info->core_cache;
		while ((cache->next != NULL) && (x < *reg_list_size))
		{
			for (i = 0; i < HEXAGON_GPR_REGS ; i++)
			{
				(*reg_list)[x++] = hexagon_reg_current(hexa_info, i, cache);
			}
			cache = cache->next;
		}


		return ERROR_OK;
		
		case REG_CLASS_ALL: 	
		
				/** dump all registers of all HW thrd */
			*reg_list_size = (HEXAGON_VALID_PER_THREAD_REGS + HEXAGON_VALID_GLOBAL_REGS) 
																	*gHexConfig.maxHwThreads;
			*reg_list = malloc(sizeof(struct reg *) * (*reg_list_size));
			memset(*reg_list, 0, sizeof(struct reg *) * (*reg_list_size));
			LOG_DEBUG("reg_list_size  is 0x%x, number of threads =0x%x, regs per thread = 0x%x",*reg_list_size, gHexConfig.maxHwThreads, (HEXAGON_VALID_PER_THREAD_REGS + HEXAGON_VALID_GLOBAL_REGS));
			// LOG_DEBUG("reg_list_size  is 0x%x",*reg_list_size);

		
				/* go to the global register node */
			global = hexa_info->core_cache;
			while (global->next != NULL)
			{
					global = global->next;
			}
		
				/* dump per thread + global registers for all Hw threads */
			x = 0;
			cache = hexa_info->core_cache;
			while ((cache->next != NULL) && (x < *reg_list_size))
			{
				/* reg_list[0] - reg_list[64],excluding reserved regs */
				for (i = 0; (i < HEXAGON_PER_THREAD_REGS) && (x < *reg_list_size); i++)
				{
					/* skip reserve registers */
					if ((i == HEXAGON_C5_RESRV) || (i >= HEXAGON_C20_RESRV && i <= HEXAGON_C29_RESRV) ||
						(i >= HEXAGON_S12_RESRV && i <= HEXAGON_S15_RESRV))
						continue;
					
					(*reg_list)[x++] = hexagon_reg_current(hexa_info, i, cache);
				}
	
				/* reg_list[65] - reg_list[77],excluding reserved regs */
				for (; (i < HEXAGON_MMODE_GLOBAL_MAX) && (x < *reg_list_size); i++)
				{
					/* skip reserve registers */
					if ((i == HEXAGON_S19_RESRV) || (i == HEXAGON_S24_RESRV) || 
															(i == HEXAGON_S26_RESRV))
						continue;
	
					(*reg_list)[x++] = hexagon_reg_current(hexa_info, (i - HEXAGON_EVB), global);
				}
	
				cache = cache->next;
			}
			return ERROR_OK;
	
		default:
			LOG_DEBUG("not a valid register class type in query.");
			return ERROR_FAIL;
	}
	
	

}

const char *hexagon_get_gdb_arch(struct target *target)
{
	return  "hexagon";
}


static int hexagon_halt(struct target *target)
{
	// static uint8_t vtlb_initialized = 0;
	int retval = ERROR_OK;
	uint64_t prev_target_state, counter=0, sys_cfg;
	bool halted = false; 
	uint32_t debug_thread = 0;

	int64_t now, then = timeval_ms();
	LOG_DEBUG("hexagon_halt");
	
	//retval = enable_dbg_sys_pwr(swddp);
	retval = hexagon_check_state_one(target, ISDBST_DEBUG_MODE_STATUS, &halted, &debug_thread);
	if (halted == false)
	{
		retval = hexagon_init_debug_access(target);
		if (retval != ERROR_OK) {
			LOG_DEBUG("hexagon_init_debug_access API  failed");
			return retval;
		}
		prev_target_state = target->state;
		retval = hexagon_check_state_one(target, ISDBST_DEBUG_MODE_STATUS, &halted, &debug_thread);
		if (retval != ERROR_OK) {
			LOG_DEBUG("hexagon_check_state_one API	failed");
			return retval;
		}
		if(halted == true)
		{
			/* We have a halting debug event */
			target->state = TARGET_HALTED;
			//enum target_debug_reason debug_reason = target->debug_reason;

// #ifdef _VTLB_ENABLED
			// hexagon_sync(target);
			if (mmu_init)
				if(!vtlb_initialized)
				{
					LOG_DEBUG("hexagon_populate_vtlb_data ");

					hexagon_populate_vtlb_data(target);
					if(hexagon_vtlb_data.vtlb_no_of_entries > 0)
						vtlb_initialized = 1;
				}
// #endif
					
			LOG_DEBUG("hexagon_halt  target->debug_reason =%d",target->debug_reason);

			hexagon_read_current_registers(target, gHexConfig.maxHwThreads);
			LOG_DEBUG("Target %s halted ", target_name(target));
	
			if (retval != ERROR_OK)
				return retval;
            switch (prev_target_state)
            {
				case TARGET_RUNNING:
				case TARGET_UNKNOWN:
				case TARGET_RESET:
					target_call_event_callbacks(target, TARGET_EVENT_HALTED);
					break;
				case TARGET_DEBUG_RUNNING:
					target_call_event_callbacks(target, TARGET_EVENT_DEBUG_HALTED);
					break;
				default:
					target_call_event_callbacks(target, TARGET_EVENT_HALTED);
					break;
			}
		}
	}
	
	for (;;) 
	{
		LOG_DEBUG("hexagon_halt foor loop enter");
		if (counter > 20)
			break;
		retval = hexagon_check_state_one(target, ISDBST_DEBUG_MODE_STATUS, &halted, &debug_thread);
		if ((retval != ERROR_OK) || (halted == true))
			break; 
		//then += 50000;
		then += 1000;
		now = timeval_ms();
		if (now > then) {
			retval = ERROR_TARGET_TIMEOUT;
			LOG_DEBUG("target %s timeout in halt, then 0x%llx - now 0x%llx", target_name(target), then, now);
			break;
		}
		counter++;
	}
		/* need to enable this code part later */
	retval = hexagon_read_syscfg_register(target);
	if(retval == ERROR_OK)
	{
		sys_cfg = hexagon_syscfg_reg;
		sys_cfg = sys_cfg | SYSCFG_L2NRA | SYSCFG_L2NWA;
		sys_cfg = sys_cfg & ~(SYSCFG_L2WB);
		//LOG_DEBUG("Writing value in SYSCFG  = 0x%x ", sys_cfg);
		hexagon_write_syscfg_register(target, sys_cfg);
	}
    if (mmu_init)
	{
		if(vtlb_initialized > 1)
		{
			LOG_DEBUG("hexagon_populate_vtlb_refresh_entries");

			hexagon_populate_vtlb_refresh_entries(target);
		}
		else
		{
			if(hexagon_vtlb_data.vtlb_no_of_entries > 0)
				vtlb_initialized++;
		}
	}
	
	return retval;
}

#ifdef  _HEXAGON_TARGET_TIME_PROFILING
void hexagon_start_time_cal_ms(void)
{
	hexagon_time_start = timeval_ms();
}

void hexagon_end_time_cal_ms(void)
{
	hexagon_time_total = timeval_ms() - hexagon_time_start;
}
#endif

int hexagon_read_tlb_entry(struct target *target)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct adiv5_dap *swddp = hexa_info->dap;
    uint32_t isdbsts;
    // uint64_t read_val[1][2];
	uint32_t read_val[1][2];
	int retval, i=0;
	uint32_t k=0;
	uint32_t isdb_mmode_cmd = 0x184, isdb_cmd_status;
	
	uint32_t stuff_inst[1][4] ={{0x7800c022,0x6c42c000,0x6700c029,0x6701c029}};
	/*stuff inst 0x7800c022-->{r2 = #1},0x6c42c000-->{r1:0 = tlbr(r2)},0x6700c029-->{isdbmbxout = r0},0x6701c029-->{isdbmbxout = r1 } */
										
	LOG_DEBUG("hexagon_read_tlb_entry Enter");
	
	hexagon_r0_used_stuff = 1;
	hexagon_r1_used_stuff = 1;
	hexagon_r2_used_stuff = 1;
	
	#ifdef  _HEXAGON_TARGET_TIME_PROFILING
			hexagon_start_time_cal_ms();
	#endif
	
	for(k=0; k < gHexConfig.numTlbEntries; k++)
	{
		retval = enable_dbg_sys_pwr(swddp);
		if (retval != ERROR_OK) {
			LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
		}
		// */
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst[0][0] + k*0x20 );
		if (retval != ERROR_OK) {
			LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
		}
        hexagon_wait_loop();
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
		if (retval != ERROR_OK) {
				LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
		}
		/* if isdb_cmd_status 0 in cmd was successfull in case of 1 failed */ 
		isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
		if(isdb_cmd_status)
		{
            LOG_DEBUG("ISDBcommand failed in monitor mode retrying");
			continue;

        	// retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
            //                              hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);   
            // isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
            // if (isdb_cmd_status)
			// 		continue;
		}
				
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst[0][1]);
		if (retval != ERROR_OK)
		{
			LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
		}
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
        if (retval != ERROR_OK)
        {
            LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
		}
		/* if isdb_cmd_status 0 in cmd was successfull in case of 1 failed */ 
		isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
		if(isdb_cmd_status)
		{
			LOG_DEBUG("ISDBcommand failed in monitor mode");
			continue;
		}
				
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst[0][2]);
        if (retval != ERROR_OK)
        {
            LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
				}
				retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
        if (retval != ERROR_OK)
        {
            LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
			}
	
		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
        if (retval != ERROR_OK)
        {
			LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
		}
	
		/* if isdb_cmd_status 0 in cmd was successfull in case of 1 failed */ 
		isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
		if(isdb_cmd_status)
		{
			LOG_DEBUG("ISDBcommand failed in monitor mode for k = %d",k);
			continue;
		}
		while(!(isdbsts & ISDBST_ISDB_MAILBOX_OUT))
		{
				
			retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
								hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
			i++;
			if (i==100)
				break;
			hexagon_wait_loop();
		}
				
		i = 0;
		if(!(isdbsts & ISDBST_ISDB_MAILBOX_OUT))
		{
			LOG_DEBUG("ISDBST status not set for mailbox for k= %d",k);
			continue;
		}
	
		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_ISDBMBXOUT, &read_val[0][0]);
		if (retval != ERROR_OK) 
		{
			LOG_DEBUG("HEXAGON_ISDB_ISDBMBXOUT read failed 0x%x", read_val[0][0]);
		}
				
		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
								hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
	
		/* procedure to read the second time mailbox resgiter*/
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst[0][3]);
        if (retval != ERROR_OK)
        {
			LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
		}
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
        if (retval != ERROR_OK)
        {
			LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
		}
	
		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
        if (retval != ERROR_OK)
        {
			LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
		}
	
		/* if isdb_cmd_status 0 in cmd was successfull in case of 1 failed */ 
		isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
		if(isdb_cmd_status)
		{
			LOG_DEBUG("ISDBcommand failed in monitor mode for k = %d",k);
			continue;
		}
		
		while(!(isdbsts & ISDBST_ISDB_MAILBOX_OUT))
		{
			retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
								hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
	
			i++;
			if (i==100)
				break;
			hexagon_wait_loop();
		}
		i = 0;
		if(!(isdbsts & ISDBST_ISDB_MAILBOX_OUT))
		{
			LOG_DEBUG("ISDBST status not set for mailbox for k= %d",k);
			continue;
		}
	
		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_ISDBMBXOUT, &read_val[0][1]);
		if (retval != ERROR_OK) 
		{
			LOG_DEBUG("HEXAGON_ISDB_ISDBMBXOUT read failed 0x%x", read_val[0][1]);
		}
		hexagon_update_tlb_entry_in_structure(read_val[0][0], read_val[0][1] ,k);
	}
	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
			//LOG_DEBUG("HEXAGON_ISDB_ISDBMBXOUT value 0x%x", read_val[1]);

	// LOG_DEBUG("Printing TLB Entries" );
	for (k=0; k < gHexConfig.numTlbEntries; k++)
	{
        LOG_DEBUG("VA raw = 0x%x --> PA raw  = 0x%x ", \
			gpHexagonTlbEntries[k].virt_tlb_raw_data, gpHexagonTlbEntries[k].phys_tlb_raw_data);

        LOG_DEBUG("VA = 0x%x -- 0x%x and PA = 0x%llx -- 0x%llx", \
				 gpHexagonTlbEntries[k].virt_add_low, gpHexagonTlbEntries[k].virt_add_high, \
				 gpHexagonTlbEntries[k].phy_add_low, gpHexagonTlbEntries[k].phy_add_high);
	}

	#ifdef  _HEXAGON_TARGET_TIME_PROFILING
			hexagon_end_time_cal_ms();
			LOG_DEBUG("Total time taken  %" PRId64 "ms", hexagon_time_total);
	#endif
	return ERROR_OK;

}

static unsigned int QURT_getPhysAddr_v2(uint64_t pg_tlblo, uint64_t pg_tlbhi)
 {
    union pg_tlblo_t tlblo;
    union pg_tlbhi_t tlbhi;
    tlblo.raw = pg_tlblo;
    tlbhi.raw = pg_tlbhi;

	
	
    return ((hexagon_clrbit((unsigned int)tlblo.info.phys_addr + ((unsigned int)tlbhi.info.ep << 24),
                                 (unsigned int)(hexagon_ct0((unsigned int)tlblo.info.phys_addr & 0x7f))) >>
             1) &
            0xffffff);
}
 
static unsigned int hexagon_clrbit(unsigned int d, unsigned int bit)
{
	
	
     return (d & (~(1<<bit)));
}

static unsigned int hexagon_ct0(unsigned int d)
{
	
	int i;
    for (i = 0; i < 32; i++)
    {
        if ((d & (1 << i)) != 0)
        {
			  return i;
		  }
	  }
	
	return 32;
}

/* this function is related to page table and get the mask from physical page */
unsigned int get_phys_mask(unsigned int tlblo)
{
	int i = 0;
	unsigned int mask = 0;

	
	for(i = 0; i <= 7; i++)
	{
		mask |= (0x01 << i);

		// first set bit defines mask and page size
		if(tlblo & (0x01 << i))
		{
			return mask;
		}
	}

	
	return 0;
}

/* this function is related to page table and get the physical page */

unsigned int get_phys_page(unsigned int lo, unsigned int hi, unsigned int mask)
{
	// get bits [34:12] defined at [23:1]
	unsigned int pp = ((lo >> 1) & 0x7fffff);
	// add bit [35]

	
	if(hi & (0x01 << 29))
		pp |= 0x800000;

	// mask page size bits
	pp &= ~(mask >> 1);
	
	
	return pp;
}


/* this function update the tlb entry in global structure  hexagon_tlb_entries */
static void  hexagon_update_tlb_entry_in_structure(uint64_t tlb_phy, uint64_t tlb_virtual , uint64_t index)
{
    uint64_t mask, size = 0, virt_add = 0, phy_add = 0;
	char * page_size;
    static int i;
	union pg_tlblo_t tlblo;
    union pg_tlbhi_t tlbhi;


	if (index >= gHexConfig.numTlbEntries)
	{
        LOG_DEBUG("Index is greater than HEXAGON_TLB_ENTRIES_NUM");
		return;
	}
	tlblo.raw = tlb_phy;
    tlbhi.raw = tlb_virtual;
    gpHexagonTlbEntries[index].phy_page = QURT_getPhysAddr_v2(tlb_phy, tlb_virtual);
    gpHexagonTlbEntries[index].virt_page = VIRT_PAGE(tlb_virtual);
    virt_add = gpHexagonTlbEntries[index].virt_page << 12;
    phy_add = gpHexagonTlbEntries[index].phy_page << 12;

    if ((virt_add == 0x0) || (phy_add == 0x0))
    {
        return;
    }
    if (!i)
    {
        LOG_DEBUG("size of  tlb_entries  = 0x%llx  ", sizeof(tlb_entries));
        i++;
    }
	mask = get_phys_mask(tlb_phy);
	gpHexagonTlbEntries[index].virt_tlb_raw_data = tlb_virtual;
	gpHexagonTlbEntries[index].phys_tlb_raw_data = tlb_phy;
	gpHexagonTlbEntries[index].phy_page = QURT_getPhysAddr_v2(tlblo.raw, tlbhi.raw );
	gpHexagonTlbEntries[index].virt_page = VIRT_PAGE(tlb_virtual);
	
	gpHexagonTlbEntries[index].asid = ASID(tlb_virtual);
	LOG_DEBUG("SID = 0x%x ", gpHexagonTlbEntries[index].asid);

	gpHexagonTlbEntries[index].asid = ASID(tlbhi.raw);
    LOG_DEBUG("SID = 0x%x ", gpHexagonTlbEntries[index].asid);

	gpHexagonTlbEntries[index].R = P_READ(tlb_phy);
	gpHexagonTlbEntries[index].W = P_WRITE(tlb_phy);
	gpHexagonTlbEntries[index].X= P_EXEC(tlb_phy);
	gpHexagonTlbEntries[index].U = P_USER(tlb_phy);
	gpHexagonTlbEntries[index].CCCC = P_CCCC(tlb_phy);
	gpHexagonTlbEntries[index].S = P_S(tlb_phy);
	gpHexagonTlbEntries[index].validbit = P_V(tlb_virtual);
	gpHexagonTlbEntries[index].globalbit = P_G(tlb_virtual);
	gpHexagonTlbEntries[index].EP = P_EP(tlb_virtual);
	gpHexagonTlbEntries[index].A1 = P_A1(tlb_virtual);
	gpHexagonTlbEntries[index].A0= P_A0(tlb_virtual);
	page_size = PAGE_SIZE(tlb_phy, mask);

    LOG_DEBUG("SID = 0x%x ", gpHexagonTlbEntries[index].asid);
    LOG_DEBUG("virt_add = 0x%llx ", virt_add);
    LOG_DEBUG("phys_add = 0x%llx ", phy_add);
	if (strcmp(page_size, "4KB") == 0)
	{
		gpHexagonTlbEntries[index].page_size =  HEXAGON_TLB_PAGE_SIZE_4KB;
		size = HEXAGON_PAGE_SIZE_16K - 1;
	}
	else if (strcmp(page_size, "16KB") == 0) 
	{
		gpHexagonTlbEntries[index].page_size =  HEXAGON_TLB_PAGE_SIZE_16KB;
		size = HEXAGON_PAGE_SIZE_16K - 1;
	}
	else if (strcmp(page_size, "64KB") == 0) 
	{
		gpHexagonTlbEntries[index].page_size =  HEXAGON_TLB_PAGE_SIZE_64KB;
		size = HEXAGON_PAGE_SIZE_64K - 1;
	}
	else if (strcmp(page_size, "256KB") == 0) 
	{
		gpHexagonTlbEntries[index].page_size =  HEXAGON_TLB_PAGE_SIZE_256KB;
		size = HEXAGON_PAGE_SIZE_256K - 1;
	}
	else if (strcmp(page_size, "1MB") == 0) 
	{
		gpHexagonTlbEntries[index].page_size =  HEXAGON_TLB_PAGE_SIZE_1MB;
		size = HEXAGON_PAGE_SIZE_1M - 1;
	}
	else if (strcmp(page_size, "4MB") == 0)
	{
		gpHexagonTlbEntries[index].page_size =  HEXAGON_TLB_PAGE_SIZE_4MB;
		size = HEXAGON_PAGE_SIZE_4M - 1;
	}
	else if (strcmp(page_size, "16MB") == 0)
	{
		gpHexagonTlbEntries[index].page_size =  HEXAGON_TLB_PAGE_SIZE_16MB;
		size = HEXAGON_PAGE_SIZE_16M - 1;
	}
	gpHexagonTlbEntries[index].virt_add_low= gpHexagonTlbEntries[index].virt_page << 12;
	gpHexagonTlbEntries[index].virt_add_high = gpHexagonTlbEntries[index].virt_add_low + size;
	
	gpHexagonTlbEntries[index].phy_add_low = (uint64_t) gpHexagonTlbEntries[index].phy_page << 12;
	gpHexagonTlbEntries[index].phy_add_high = (uint64_t) (gpHexagonTlbEntries[index].phy_add_low) + size;
	
	LOG_DEBUG("V bit =0x%x G bit = 0x%x ASID = 0x%x ",\
		 gpHexagonTlbEntries[index].validbit, gpHexagonTlbEntries[index].globalbit, gpHexagonTlbEntries[index].asid);

    LOG_DEBUG("VA = 0x%x -- 0x%x and PA = 0x%llx -- 0x%llx",\
		 gpHexagonTlbEntries[index].virt_add_low,gpHexagonTlbEntries[index].virt_add_high,gpHexagonTlbEntries[index].phy_add_low,gpHexagonTlbEntries[index].phy_add_high);

	

}
static int hexagon_dump_hwthrd_reg(struct target *target, uint32_t hwthrd)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	
	struct reg_cache *cache;
	uint64_t  i;
    uint64_t current_pc_thread_select;

	if (hwthrd == gHexConfig.maxHwThreads)
	{
		/* dump registers for all Hw thread */
		cache = hexa_info->core_cache;
		while (cache->next != NULL)
		{
			for (i = 0; i < HEXAGON_PER_THREAD_REGS ; i++)
			{
                current_pc_thread_select = *((uint64_t *)cache->reg_list[41].value);
				
            #ifdef BREAKPOINT_THREAD_SELECT
                if (((unsigned int)current_pc_thread_select == (unsigned int)breakpoint_address_thread_select))
                {   
                    thread_id_thread_select = *((uint64_t *)cache->reg_list[72].value);

                }
            #endif
				LOG_DEBUG("%s : %s = 0x%x", cache->name, 
					cache->reg_list[i].name, *(cache->reg_list[i].value));
			}
			cache = cache->next;
		}

		/* dump the global registers */
		for (i = 0; i < HEXAGON_GLOBAL_REGS ; i++)
		{
			LOG_DEBUG("%s : %s = 0x%x", cache->name, 
				cache->reg_list[i].name, *(cache->reg_list[i].value));
		}
	}
	else
	{
		/* dump registers for the requested Hw thread */
		cache = hexa_info->core_cache;
		i= 0;
		while ((cache != NULL) && i < hwthrd)
		{
			cache = cache->next;
			i++;
		}
		
		for (i = 0; i < HEXAGON_PER_THREAD_REGS ; i++)
		{
			LOG_DEBUG("%s : %s = 0x%x", cache->name, 
				cache->reg_list[i].name, *(cache->reg_list[i].value));
		}			
	}
    LOG_DEBUG("exiting hexagon_dump_hwthrd_reg");
	return ERROR_OK;	
}
#if 1
/** Read registers of the the current context **/
int hexagon_read_gpr_registers(struct target *target, uint32_t hwthrd)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct adiv5_dap *swddp = hexa_info->dap;
	struct reg_cache *cache;
	int retval,j,k=0, loop,x;
	uint32_t isdb_mmode_cmd,isdb_cmd_status,isdbsts, i=0;
	
	#ifdef  _DEBUG_HEXAGON_
    LOG_DEBUG("hexagon_read_gpr_registers entry - hw thrd: %d", hwthrd);
	#endif
	
	#ifdef  _HEXAGON_TARGET_TIME_PROFILING
		hexagon_start_time_cal_ms();
	#endif

	cache = hexa_info->core_cache;
	if (hwthrd == gHexConfig.maxHwThreads)
	{
		loop = gHexConfig.maxHwThreads;
		i = 0;
	}
	else
	{
		loop = 1;
		i= 0;
		while ((cache != NULL) && i < hwthrd)
		{
			cache = cache->next;
			i++;
		}
	}
	
		/* Get registers per Hw thread */
	for (x = 0; x < loop && cache != NULL; x++, i++, cache=cache->next)
	{
		/* pack the ISDB command for the relevant Hw thread */
		isdb_mmode_cmd = hexagon_pack_isdbcmd(ISDBCMD_CMD_STUFF,ISDBCMD_MONITOR_LVL,
													ISDBCMD_TNUM_MASK_THREAD(i));
		#ifdef  _DEBUG_HEXAGON_
		LOG_DEBUG("hexagon_read_gpr_registers  isdb_mmode_cmd = %d and i = %d", isdb_mmode_cmd, i);
		#endif
	
		for (j = 0; j < HEXAGON_GPR_REGS; j++)
		{
			/* monitor  mode */
			// /*	
			retval = enable_dbg_sys_pwr(swddp);
			if (retval != ERROR_OK) {
				LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
			} 
			// */
			
			retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
				hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst_gpr_read[j]);
			if (retval != ERROR_OK) {
				LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
			}
			retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
					hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
			if (retval != ERROR_OK) {
				LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
			}

			/* wait till the stuff instruction is executed */
			hexagon_wait_loop();
			
			retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
					hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
			if (retval != ERROR_OK) 
			{
				LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
			}


			/* if isdb_cmd_status 0 in cmd was successfull in case of 1 failed */ 
			isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;

			if (isdb_cmd_status)
			{
					LOG_DEBUG("ISDBcommand failed in user mode");
					return ERROR_OK;
			}
			else
				{
				while(!(isdbsts & ISDBST_ISDB_MAILBOX_OUT))
				{
					retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);

					hexagon_wait_loop();
					#ifdef  _DEBUG_HEXAGON_
					LOG_DEBUG("ISDB status read for Mboxout %d time", j);
					#endif
					
					k++;
					if (k==10)
						break;
				}
				#ifdef  _DEBUG_HEXAGON_
				LOG_DEBUG("ISDBST status before reading MBXOUT = 0x%x, thrd = %d, reg = %d", isdbsts, x, j);
				#endif

				k = 0;
				if(!(isdbsts & ISDBST_ISDB_MAILBOX_OUT))
				{
					LOG_DEBUG("ISDBST status not set for mailbox so skiping reading it for register R%d", j);
					continue;
				}


				retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_ISDBMBXOUT, &gpPerHwThrdReg[x][j]);
				if (retval != ERROR_OK) 
				{
					LOG_DEBUG("HEXAGON_ISDB_ISDBMBXOUT read failed 0x%x", gpPerHwThrdReg[x][j]);
				}

				// Cast to uint8_t * to match the type of cache->reg_list[j].value 
				cache->reg_list[j].value = (uint8_t *) &gpPerHwThrdReg[x][j];
				cache->reg_list[j].valid = true;
			
				retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);

				#ifdef  _DEBUG_HEXAGON_
				LOG_DEBUG("ISDBST status after reading MBXOUT = 0x%x, thrd = %d, reg = %d", isdbsts, x, j);
				LOG_DEBUG("HEXAGON_ISDB_ISDBMBXOUT value 0x%x", gpPerHwThrdReg[x][j]);
				#endif
				}
			}
		}

	#ifdef  _HEXAGON_TARGET_TIME_PROFILING
		hexagon_end_time_cal_ms();
		LOG_DEBUG("Total time taken  %" PRId64 "ms", hexagon_time_total);
	#endif

	#ifdef  _DEBUG_HEXAGON_
    		LOG_DEBUG("hexagon_read_gpr_registers exit - hw thrd: %d", hwthrd);
	#endif

	
	return ERROR_OK;
}
#endif
#if 0 // new gpr logic
/** Read registers of the the current context **/
int hexagon_read_gpr_registers(struct target *target, uint32_t hwthrd)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct adiv5_dap *swddp = hexa_info->dap;
	struct reg_cache *cache;
	int retval, j, k=0, loop,x;
	uint32_t isdb_mmode_cmd,isdb_cmd_status,isdbsts, i = 0;
	
	cache = hexa_info->core_cache;
	if (hwthrd == gHexConfig.maxHwThreads)
	{
		loop = gHexConfig.maxHwThreads;
		i = 0;
	}
	else
	{
		loop = 1;
		i= 0;
		while ((cache != NULL) && i < hwthrd)
		{
			cache = cache->next;
			i++;
		}
	}
	
		/* Get registers per Hw thread */
	for (x = 0; x < loop && cache != NULL; x++, i++, cache=cache->next)
	{
		/* pack the ISDB command for the relevant Hw thread */
		isdb_mmode_cmd = hexagon_pack_isdbcmd(ISDBCMD_CMD_STUFF,ISDBCMD_MONITOR_LVL,
													ISDBCMD_TNUM_MASK_THREAD(i));

		for (j = 0; j < HEXAGON_GPR_REGS; j++)
		{

			retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
				hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst_gpr_read[j]);
			if (retval != ERROR_OK) {
				LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
			}
			retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
					hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
			if (retval != ERROR_OK) {
				LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
			}

			/* wait till the stuff instruction is executed */
			hexagon_wait_loop();
			
			/* if isdb_cmd_status 0 in cmd was successfull in case of 1 failed */ 
			isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;

			if (isdb_cmd_status)
			{
					LOG_DEBUG("ISDBcommand failed in user mode");
					return ERROR_OK;
			}
			else
				{
				while(!(isdbsts & ISDBST_ISDB_MAILBOX_OUT))
				{
					retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
					
					k++;
					if (k==10)
						break;
				}

				k = 0;
				if(!(isdbsts & ISDBST_ISDB_MAILBOX_OUT))
				{
					LOG_DEBUG("ISDBST status not set for mailbox so skiping reading it for register R%d", j);
					continue;
				}

				retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_ISDBMBXOUT, &gpPerHwThrdReg[x][j]);
				if (retval != ERROR_OK) 
				{
					LOG_DEBUG("HEXAGON_ISDB_ISDBMBXOUT read failed 0x%x", gpPerHwThrdReg[x][j]);
				}


				cache->reg_list[j].value = &gpPerHwThrdReg[x][j];
				cache->reg_list[j].valid = true;
			
				retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);

				#ifdef  _DEBUG_HEXAGON_
				LOG_DEBUG("ISDBST status after reading MBXOUT = 0x%x, thrd = %d, reg = %d", isdbsts, x, j);
				LOG_DEBUG("HEXAGON_ISDB_ISDBMBXOUT value 0x%x", gpPerHwThrdReg[x][j]);
				#endif
				}
			}
		}

	#ifdef  _HEXAGON_TARGET_TIME_PROFILING
		hexagon_end_time_cal_ms();
		LOG_DEBUG("Total time taken  %" PRId64 "ms", hexagon_time_total);
	#endif

	#ifdef  _DEBUG_HEXAGON_
    	LOG_DEBUG("hexagon_read_gpr_registers exit - hw thrd: %d", hwthrd);
	#endif

	
	return ERROR_OK;
}
#endif
/** Read registers of the the current context **/
int hexagon_read_ctrl_registers(struct target *target, uint32_t hwthrd)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct adiv5_dap *swddp = hexa_info->dap;
	struct reg_cache *cache;
	int retval, j, k,l=0, x, loop;
	uint32_t isdbsts,i=0;
	uint32_t isdb_mmode_cmd, isdb_cmd_status;

	#ifdef  _DEBUG_HEXAGON_
    LOG_DEBUG("hexagon_read_ctrl_registers entry - hw thrd: %d", hwthrd);
	#endif

	#ifdef  _HEXAGON_TARGET_TIME_PROFILING
		hexagon_start_time_cal_ms();
	#endif
	
	cache = hexa_info->core_cache;
	if (hwthrd == gHexConfig.maxHwThreads)
	{
		loop = gHexConfig.maxHwThreads;
		i = 0;
	}
	else
	{
		loop = 1;
		i= 0;
		while ((cache != NULL) && i < hwthrd)
		{
			cache = cache->next;
			i++;
		}
	}

		/* Get per Hw thread control registers */
	for (x = 0; x < loop && cache != NULL; x++, i++, cache=cache->next)
	{
		/* pack the ISDB command for the relevant Hw thread */
		isdb_mmode_cmd = hexagon_pack_isdbcmd(ISDBCMD_CMD_STUFF,ISDBCMD_MONITOR_LVL,
													ISDBCMD_TNUM_MASK_THREAD(i));
		#ifdef  _DEBUG_HEXAGON_
		LOG_DEBUG("hexagon_read_ctrl_registers  isdb_mmode_cmd = %d and i = %d", isdb_mmode_cmd, i);
		#endif
		/* Mark r7 as dirty as it is being used for stuff instruction */
		cache->reg_list[HEXAGON_R7].dirty = true;
		
		//k = 0;
		for (j = HEXAGON_SA0, k = 0; j < HEXAGON_CTRL_MAX; j++, k++)
		{

			if ((j == HEXAGON_C5_RESRV) || (j >= HEXAGON_C20_RESRV && j <= HEXAGON_C29_RESRV))
			{
			//k++;
				continue;
			}

			
			retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
					hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst_ctrl_reg_read[k][0]);
			if (retval != ERROR_OK) {
				LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
			}
			retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
					hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
			if (retval != ERROR_OK) {
				LOG_DEBUG("HEXAGON_ISDB_ISDBCMD_CDSP return value is not OK");
			}

			/* wait till the stuff instruction is executed */
			hexagon_wait_loop();
			
			retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
					hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
			if (retval != ERROR_OK) {
				LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
			}


			/* 0 - cmd sucessfull, 1 - failure */ 
			isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
			if (isdb_cmd_status)
			{
					LOG_DEBUG("ISDBcommand failed in Monitor mode");
					return ERROR_OK;
			}
			else
			{
				/*there are 2 stuff instruction here programming second inst */
				// /*
				retval = enable_dbg_sys_pwr(swddp);
				if (retval != ERROR_OK) {
					LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
				}
				// */

				retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst_ctrl_reg_read[k][1]);
			if (retval != ERROR_OK)
			{
				LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
				}

				retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
			if (retval != ERROR_OK)
			{
				LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
				}

				/* wait till the stuff instruction is executed */
				hexagon_wait_loop();
							
				retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
			if (retval != ERROR_OK)
			{
					LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
				}
				#ifdef  _DEBUG_HEXAGON_
				LOG_DEBUG("ISDBST read status after 2nd stuff inst 0x%x", isdbsts);
				#endif
				
				/* 0 - cmd success, 1 - cmd failure */ 
				isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
				if (isdb_cmd_status)
				{
						LOG_DEBUG("ISDBcommand failed in Monitor mode");
						return ERROR_OK;
				}
				else
				{			
					while(!(isdbsts & ISDBST_ISDB_MAILBOX_OUT))
					{
						retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);

						hexagon_wait_loop();
						#ifdef  _DEBUG_HEXAGON_
						LOG_DEBUG("ISDB status read for Mboxout %d time", j);
						#endif
							
						l++;
						if (l==10)
							break;
					}
					#ifdef  _DEBUG_HEXAGON_
					LOG_DEBUG("ISDBST status before reading MBXOUT = 0x%x, thrd = %d, reg = %d", isdbsts, x, j);
					#endif

					l = 0;
					if(!(isdbsts & ISDBST_ISDB_MAILBOX_OUT))
					{
						LOG_DEBUG("ISDBST status not set for mailbox so skiping reading it for register R%d", j);
						continue;
					}

					retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_ISDBMBXOUT, &gpPerHwThrdReg[x][j]);
					if (retval != ERROR_OK) 
					{
						LOG_DEBUG("HEXAGON_ISDB_ISDBMBXOUT_CDSP read failed 0x%x", gpPerHwThrdReg[x][j]);
					}

					// Cast to uint8_t * to match the type of cache->reg_list[j].value 
					cache->reg_list[j].value = (uint8_t *)&gpPerHwThrdReg[x][j];
					cache->reg_list[j].valid = true;
						
					retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);

					#ifdef  _DEBUG_HEXAGON_
					LOG_DEBUG("ISDBST status after reading MBXOUT = 0x%x, thrd = %d, reg = %d", isdbsts, x, j);
					LOG_DEBUG("HEXAGON_ISDB_ISDBMBXOUT value 0x%x", gpPerHwThrdReg[x][j]);
					#endif
				}
			}

		}
	}

	#ifdef  _HEXAGON_TARGET_TIME_PROFILING
		hexagon_end_time_cal_ms();
		LOG_DEBUG("Total time taken  %" PRId64 "ms", hexagon_time_total);
	#endif
	#ifdef  _DEBUG_HEXAGON_
	LOG_DEBUG("hexagon_read_ctrl_registers exit - hw thrd: %d", hwthrd);
	#endif

	
	return ERROR_OK;
		
}


int hexagon_read_mmode_registers(struct target *target, uint32_t hwthrd)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct adiv5_dap *swddp = hexa_info->dap;
	struct reg_cache *cache;
	int retval , j, k,l=0, x, loop;
	uint32_t isdbsts,i=0;
	uint32_t isdb_mmode_cmd, isdb_cmd_status;

	#ifdef  _DEBUG_HEXAGON_
	LOG_DEBUG("hexagon_read_mmode_registers entry - hw thrd: %d", hwthrd);
	#endif

	
	#ifdef  _HEXAGON_TARGET_TIME_PROFILING
		hexagon_start_time_cal_ms();
	#endif

	cache = hexa_info->core_cache;
	if (hwthrd == gHexConfig.maxHwThreads)
	{
		loop = gHexConfig.maxHwThreads;
		i = 0;
	}
	else
	{
		loop = 1;
		i= 0;
		while ((cache != NULL) && i < hwthrd)
		{
			cache = cache->next;
			i++;
		}
	}
	/* Get per Hw thread control registers */
	for (x = 0; x < loop && cache != NULL; x++, i++, cache=cache->next)
	{
		/* pack the ISDB command for the relevant Hw thread */
		isdb_mmode_cmd = hexagon_pack_isdbcmd(ISDBCMD_CMD_STUFF, ISDBCMD_MONITOR_LVL,
								ISDBCMD_TNUM_MASK_THREAD(i));
		#ifdef  _DEBUG_HEXAGON_
		LOG_DEBUG("hexagon_read_mmode_registers  isdb_mmode_cmd = %d and i = %d", isdb_mmode_cmd, i);
		#endif
		/* Mark r7 as dirty as it is being used for stuff instruction */
		cache->reg_list[HEXAGON_R7].dirty = true;
			
		for (j = HEXAGON_SGP0, k = 0; j < HEXAGON_MMODE_PERTHRD_MAX; j++, k++)
		{
			if (j >= HEXAGON_S12_RESRV && j <= HEXAGON_S15_RESRV)
			{
				continue;
			}
			/* Monitor mode */
			// /*
			retval = enable_dbg_sys_pwr(swddp);
			if (retval != ERROR_OK) {
				LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
			}
			// */
	
			retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
					hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst_mmode_reg_read[k][0]);
			if (retval != ERROR_OK) {
				LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
			}
			retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
					hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
            if (retval != ERROR_OK)
            {
				LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
			}

			/* wait till the stuff instruction is executed */
			hexagon_wait_loop();
			
			retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
					hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
            if (retval != ERROR_OK)
            {
				LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
			}


			/* 0 - cmd sucessfull, 1 - failure */
			isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
			if (isdb_cmd_status)
			{
				 LOG_DEBUG("ISDBcommand failed in Monitor mode");
				 return ERROR_OK;
			}
			else
			{
				/*there are 2 stuff instruction here programming second inst */
				// /*
				retval = enable_dbg_sys_pwr(swddp);
				if (retval != ERROR_OK) {
					LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
				}
				// */

				retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
						  hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst_mmode_reg_read[k][1]);
                if (retval != ERROR_OK)
                {
					LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
				}

				retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
                if (retval != ERROR_OK)
                {
					LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
				}

				/* wait till the stuff instruction is executed */
				hexagon_wait_loop();
							
				retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
										hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
                if (retval != ERROR_OK)
                {
					LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
				}
				#ifdef  _DEBUG_HEXAGON_
				LOG_DEBUG("ISDBST read status after 2nd stuff inst 0x%x", isdbsts);
				#endif
				
					/* 0 - cmd success, 1 - cmd failure */ 
    				isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
				if (isdb_cmd_status)
				{
	    			 LOG_DEBUG("ISDBcommand failed in monitor  mode");
					 return ERROR_OK;
				}
				else
				{			
					while(!(isdbsts & ISDBST_ISDB_MAILBOX_OUT))
					{
						retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);

						hexagon_wait_loop();

						#ifdef  _DEBUG_HEXAGON_
						LOG_DEBUG("ISDB status read for Mboxout %d time", j);
						#endif
							
						l++;
						if (l==10)
							break;
					}
					#ifdef  _DEBUG_HEXAGON_
					LOG_DEBUG("ISDBST status before reading MBXOUT = 0x%x, thrd = %d, reg = %d", isdbsts, x, j);
					#endif

					l = 0;
					if(!(isdbsts & ISDBST_ISDB_MAILBOX_OUT))
					{
						LOG_DEBUG("ISDBST status not set for mailbox so skiping reading it for register R%d", j);
						continue;
					}

					retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_ISDBMBXOUT, &gpPerHwThrdReg[x][j]);
					if (retval != ERROR_OK) 
					{
						LOG_DEBUG("HEXAGON_ISDB_ISDBMBXOUT read failed 0x%x", gpPerHwThrdReg[x][j]);
					}

					// Cast to uint8_t * to match the type of cache->reg_list[j].value 
					cache->reg_list[j].value = (uint8_t *)&gpPerHwThrdReg[x][j];
					cache->reg_list[j].valid = true;	
						
					retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);

					#ifdef  _DEBUG_HEXAGON_
					LOG_DEBUG("ISDBST status after reading MBXOUT = 0x%x, thrd = %d, reg = %d", isdbsts, x, j);
					LOG_DEBUG("HEXAGON_ISDB_ISDBMBXOUT value 0x%x", gpPerHwThrdReg[x][j]);
					#endif
				}
			}
		}
	}

	hexagon_read_imask_register(target, hwthrd);
	#ifdef  _HEXAGON_TARGET_TIME_PROFILING
		hexagon_end_time_cal_ms();
		LOG_DEBUG("Total time taken  %" PRId64 "ms", hexagon_time_total);
	#endif

	#ifdef  _DEBUG_HEXAGON_
	LOG_DEBUG("hexagon_read_mmode_registers exit - hw thrd: %d", hwthrd);
	#endif

	
	return ERROR_OK;
}

int hexagon_read_imask_register(struct target *target, uint32_t hwthrd)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct adiv5_dap *swddp = hexa_info->dap;
	struct reg_cache *cache;
	int retval, k,l=0, x, loop;
	uint32_t isdbsts,i=0;
	uint32_t isdb_mmode_cmd, isdb_cmd_status;

	#ifdef  _DEBUG_HEXAGON_
	LOG_DEBUG("hexagon_read_imask_register entry - hw thrd: %d", hwthrd);
	#endif

	#ifdef  _HEXAGON_TARGET_TIME_PROFILING
		hexagon_start_time_cal_ms();
	#endif

	
	
	cache = hexa_info->core_cache;
	if (hwthrd == gHexConfig.maxHwThreads)
	{
		loop = gHexConfig.maxHwThreads;
		k = 0;
	}
	else
	{
		loop = 1;
		i= 0;
		while ((cache != NULL) && i < hwthrd)
		{
			cache = cache->next;
			i++;
		}
		k = i;
	}

	/* Get per Hw thread control registers */
	for (x = 0, k = 0; x < loop && cache != NULL; x++, cache=cache->next, k++)
	{
		/* pack the ISDB command for the relevant Hw thread */
		isdb_mmode_cmd = hexagon_pack_isdbcmd(ISDBCMD_CMD_STUFF, ISDBCMD_MONITOR_LVL,
						ISDBCMD_TNUM_MASK_THREAD(k));
		//LOG_DEBUG("hexagon_read_imask_register  isdb_mmode_cmd = %d and i = %d", isdb_mmode_cmd, k);
		/* Monitor mode */
		// /*
		retval = enable_dbg_sys_pwr(swddp);
		if (retval != ERROR_OK) {
			LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
		}
		// */
		
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
				hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst_mmode_imask_reg_read[k][0]);
        if (retval != ERROR_OK)
        {
			LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
		}
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
				hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
        if (retval != ERROR_OK)
        {
			LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
		}

		/* wait till the stuff instruction is executed */
		hexagon_wait_loop();
		
		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
				hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
        if (retval != ERROR_OK)
        {
			LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
		}


		/* 0 - cmd sucessfull, 1 - failure */
		isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
		if (isdb_cmd_status)
		{
			 LOG_DEBUG("ISDBcommand failed in Monitor mode");
			 return ERROR_OK;
		}
		else
		{
			/*there are 2 stuff instruction here programming second inst */
			// /*
			retval = enable_dbg_sys_pwr(swddp);
			if (retval != ERROR_OK) {
				LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
			}
			// */

			retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
					  hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst_mmode_imask_reg_read[k][1]);
            if (retval != ERROR_OK)
            {
				LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
			}

			retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
					hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
            if (retval != ERROR_OK)
            {
				LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
			}

			/* wait till the stuff instruction is executed */
			hexagon_wait_loop();
						
			retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
					hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
            if (retval != ERROR_OK)
            {
				LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
			}
			#ifdef  _DEBUG_HEXAGON_
			LOG_DEBUG("ISDBST read status after 2nd stuff inst 0x%x", isdbsts);
			#endif
			
			/* 0 - cmd success, 1 - cmd failure */ 
			isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
			if (isdb_cmd_status)
			{
				 LOG_DEBUG("ISDBcommand failed in monitor mode");
				 return ERROR_OK;
			}
			else
			{

				/*there are 3 stuff instruction here programming second inst */
				// /*	
				retval = enable_dbg_sys_pwr(swddp);
				if (retval != ERROR_OK) {
					LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
				} 
				// */

				retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
						  hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst_mmode_imask_reg_read[k][2]);
                if (retval != ERROR_OK)
                {
					LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
				}

				retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
                if (retval != ERROR_OK)
                {
					LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
				}

				/* wait till the stuff instruction is executed */
				hexagon_wait_loop();
							
				retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
                if (retval != ERROR_OK)
                {
					LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
				}
				#ifdef  _DEBUG_HEXAGON_
				LOG_DEBUG("ISDBST read status after 2nd stuff inst 0x%x", isdbsts);
				#endif
				
				/* 0 - cmd success, 1 - cmd failure */ 
				isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
				if (isdb_cmd_status)
				{
					 LOG_DEBUG("ISDBcommand failed in monitor mode");
					 return ERROR_OK;
				}
				else
				{
					while(!(isdbsts & ISDBST_ISDB_MAILBOX_OUT))
					{
						retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);

						hexagon_wait_loop();
						#ifdef  _DEBUG_HEXAGON_
						LOG_DEBUG("ISDB status read for Mboxout %d time", x);
						#endif
							
						l++;
						if (l==10)
							break;
					}
					#ifdef  _DEBUG_HEXAGON_
					LOG_DEBUG("ISDBST status before reading MBXOUT = 0x%x, thrd = %d, reg = %d", isdbsts, k, HEXAGON_IMASK);
					#endif

					l = 0;
					if(!(isdbsts & ISDBST_ISDB_MAILBOX_OUT))
					{
						LOG_DEBUG("ISDBST status not set for mailbox so skiping reading it for register R%d", HEXAGON_IMASK);
						continue;
					}

					retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_ISDBMBXOUT, &gpPerHwThrdReg[k][HEXAGON_IMASK]);
					if (retval != ERROR_OK) 
					{
						LOG_DEBUG("HEXAGON_ISDB_ISDBMBXOUT read failed 0x%x", gpPerHwThrdReg[k][HEXAGON_IMASK]);
					}

				// Cast to uint8_t * to match the type of cache->reg_list[HEXAGON_IMASK].value 
				cache->reg_list[HEXAGON_IMASK].value = (uint8_t *) &gpPerHwThrdReg[k][HEXAGON_IMASK];
					
				retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
				#ifdef  _DEBUG_HEXAGON_
				LOG_DEBUG("ISDBST status after reading MBXOUT = 0x%x, thrd = %d, reg = %d", isdbsts, k, HEXAGON_IMASK);
				LOG_DEBUG("HEXAGON_ISDB_ISDBMBXOUT value 0x%x", gpPerHwThrdReg[k][HEXAGON_IMASK]);
				#endif
			}
			}
		}
	}

	#ifdef  _HEXAGON_TARGET_TIME_PROFILING
		hexagon_end_time_cal_ms();
		LOG_DEBUG("Total time taken  %" PRId64 "ms", hexagon_time_total);
	#endif

	#ifdef  _DEBUG_HEXAGON_
	LOG_DEBUG("hexagon_read_imask_register exit - hw thrd: %d", hwthrd);
	#endif

	
	return ERROR_OK;	
}
static int hexagon_restore_stuff_used_reg(struct target *target)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct reg_cache *cache;
	int i=0;

	
    LOG_DEBUG("hexagon_restore_stuff_used_reg");
	
	i= 0;
	cache = hexa_info->core_cache;
	while (cache != NULL)
	{
		/* check whether R7 is being used for stuff instruction */
		if (cache->reg_list[HEXAGON_R7].dirty == true)
		{
			LOG_DEBUG("Writing modifed R7");
		
			hexagon_write_core_reg(target, HEXAGON_R7, i, *((uint64_t*)cache->reg_list[HEXAGON_R7].value));
			cache->reg_list[HEXAGON_R7].dirty = false;
		}
			cache = cache->next;
			i++;
	}

	
	return ERROR_OK;
}

/** Read registers of the the current context **/
int hexagon_read_global_ctrl_registers(struct target *target)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct adiv5_dap *swddp = hexa_info->dap;
	struct reg_cache *cache;
	int retval ,j,l=0, x;
	uint32_t isdb_mmode_cmd, isdb_cmd_status,isdbsts;

	#ifdef  _DEBUG_HEXAGON_
	LOG_DEBUG("hexagon_read_global_ctrl_registers  entry");
	#endif

	#ifdef  _HEXAGON_TARGET_TIME_PROFILING
		hexagon_start_time_cal_ms();
	#endif

	/* global */
	cache = hexa_info->core_cache;
	while ((cache->next != NULL))
	{
		cache = cache->next;
	}

	isdb_mmode_cmd = hexagon_pack_isdbcmd(ISDBCMD_CMD_STUFF, ISDBCMD_MONITOR_LVL,
												ISDBCMD_TNUM_MASK_THREAD(0));
	#ifdef  _DEBUG_HEXAGON_
	LOG_DEBUG("hexagon_read_global_ctrl_registers  isdb_mmode_cmd = %d ", isdb_mmode_cmd);
	#endif

	/* Mark r7 as dirty as it is being used for stuff instruction */
	cache->reg_list[HEXAGON_R7].dirty = true;
	
	for (x = HEXAGON_EVB, j = 0; x < HEXAGON_MMODE_GLOBAL_MAX; x++, j++)
	{
		if ((x == HEXAGON_S19_RESRV) || (x == HEXAGON_S24_RESRV) || (x == HEXAGON_S26_RESRV))
		{
			continue;
		}

		/* Monitor mode */
		// /*
		retval = enable_dbg_sys_pwr(swddp);
		if (retval != ERROR_OK) {
			LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
		} 
		// */
		
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst_global_reg_read[j][0]);
        if (retval != ERROR_OK)
        {
            LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
		}
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
				hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
        if (retval != ERROR_OK)
        {
            LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
		}

		/* wait till the stuff instruction is executed */
		hexagon_wait_loop();
		
		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
				hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
        if (retval != ERROR_OK)
        {
			LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
		}


		/* 0 - cmd sucessfull, 1 - failure */
		isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
		if (isdb_cmd_status)
		{
			 LOG_DEBUG("ISDBcommand failed in Monitor mode");
			 return ERROR_OK;
		}
		else
		{
			/*there are 2 stuff instruction here programming second inst */
			// /*
			retval = enable_dbg_sys_pwr(swddp);
			if (retval != ERROR_OK) {
				LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
			} 
			// */

			retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
					  hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst_global_reg_read[j][1]);
            if (retval != ERROR_OK)
            {
				LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
			}

			retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
					hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
            if (retval != ERROR_OK)
            {
				LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
			}

			/* wait till the stuff instruction is executed */
			hexagon_wait_loop();
						
			retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
					hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
            if (retval != ERROR_OK)
            {
				LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
			}
			#ifdef  _DEBUG_HEXAGON_
			LOG_DEBUG("ISDBST read status after 2nd stuff inst 0x%x", isdbsts);
			#endif
			
			/* 0 - cmd success, 1 - cmd failure */ 
			isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
			if (isdb_cmd_status)
			{
				 LOG_DEBUG("ISDB command failed in monitor mode");
				 return ERROR_OK;
			}
			else
			{			
				while(!(isdbsts & ISDBST_ISDB_MAILBOX_OUT))
				{
					retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);

					hexagon_wait_loop();

					#ifdef  _DEBUG_HEXAGON_
					LOG_DEBUG("ISDB status read for Mboxout %d time", x);
					#endif
						
					l++;
					if (l==10)
						break;
				}

				#ifdef  _DEBUG_HEXAGON_
				LOG_DEBUG("ISDBST status before reading MBXOUT = 0x%x, reg = %d", isdbsts, x);
				#endif

				l = 0;
				if(!(isdbsts & ISDBST_ISDB_MAILBOX_OUT))
				{
					LOG_DEBUG("ISDBST status not set for mailbox so skiping reading it for register R%d", x);
					continue;
				}

				retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_ISDBMBXOUT, &global_reg[j]);
				if (retval != ERROR_OK) 
				{
					LOG_DEBUG("HEXAGON_ISDB_ISDBMBXOUT read failed 0x%x", global_reg[j]);
				}

				// Cast to uint8_t * to match the type of cache->reg_list[j].value 
				cache->reg_list[j].value = (uint8_t *) &global_reg[j];
				cache->reg_list[j].valid = true;
					
				retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
				#ifdef  _DEBUG_HEXAGON_
				LOG_DEBUG("ISDBST status after reading MBXOUT = 0x%x,  reg = %d", isdbsts, x);
				LOG_DEBUG("HEXAGON_ISDB_ISDBMBXOUT value 0x%x", global_reg[j]);
				#endif
			}
		}
	}

	#ifdef  _HEXAGON_TARGET_TIME_PROFILING
		hexagon_end_time_cal_ms();
		LOG_DEBUG("Total time taken  %" PRId64 "ms", hexagon_time_total);
	#endif

	#ifdef  _DEBUG_HEXAGON_
	LOG_DEBUG("hexagon_read_global_ctrl_registers  exit");
	#endif
	return ERROR_OK;	
}
int hexagon_write_hvx_registers(struct target *target, uint32_t value)
{
    struct hexagon_common *hexagon = target_to_hexagon(target);
    struct hexa_info *hexa_info = &hexagon->hexa_info;
    int retval, j = 0;
    uint32_t isdbsts,isdb_mmode_cmd, isdb_cmd_status;
    uint64_t opcode = 0x19a0e020;

#ifdef _HEXAGON_TARGET_TIME_PROFILING
    hexagon_start_time_cal_ms();
#endif
	


    isdb_mmode_cmd = hexagon_pack_isdbcmd(ISDBCMD_CMD_STUFF, ISDBCMD_MONITOR_LVL,
                                               ISDBCMD_TNUM_MASK_THREAD(0));

    for (j = 0; j < 32; j++)
    {
        retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
                                        hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
        if (retval != ERROR_OK)
            LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
        LOG_DEBUG("ISDB status before ISDBMBXIN write 0x%x", isdbsts);
        retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
                                         hexa_info->debug_base + HEXAGON_ISDB_ISDBMBXIN, 0x9);
        if (retval != ERROR_OK)
            LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
        retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
                                         hexa_info->debug_base + HEXAGON_ISDB_STFINST, 0x6ea8c000);
        if (retval != ERROR_OK)
            LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
        retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
                                        hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
        if (retval != ERROR_OK)
            LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
        retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
                                         hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
        if (retval != ERROR_OK)
            LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
        hexagon_wait_loop();
        retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
                                        hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
        if (retval != ERROR_OK)
            LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
        isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
        if (isdb_cmd_status)
        {
            LOG_DEBUG("ISDB command failed in monitor mode");
            return ERROR_OK;
        }
        retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
                                         hexa_info->debug_base + HEXAGON_ISDB_STFINST, opcode + j);
        if (retval != ERROR_OK)
            LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
        retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
                                        hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
        if (retval != ERROR_OK)
            LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
        LOG_DEBUG("ISDB status after opcode + j  write 0x%x", isdbsts);
        retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
                                         hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
        if (retval != ERROR_OK)
            LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
        hexagon_wait_loop();
        retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
                                        hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
        if (retval != ERROR_OK)
            LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
        isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
        if (isdb_cmd_status)
        {
            LOG_DEBUG("ISDB command failed in monitor  mode");
        }
    }
#ifdef _HEXAGON_TARGET_TIME_PROFILING
    hexagon_end_time_cal_ms();
    LOG_DEBUG("Total time taken  %" PRId64 "ms", hexagon_time_total);
#endif


	

    return ERROR_OK;
}
int hexagon_read_hvx_registers(struct target *target)
{
    struct hexagon_common *hexagon = target_to_hexagon(target);
    struct hexa_info *hexa_info = &hexagon->hexa_info;
    int retval, i, j, l = 0;
    uint32_t isdb_mmode_cmd, isdb_cmd_status, isdbsts;
    uint64_t vextract_opcode_v0 = 0x9200c021;
    hexagon_r0_used_stuff = 1;
    hexagon_r1_used_stuff = 1;
    LOG_DEBUG("hexagon_read_hvx_registers  entry");
#ifdef _HEXAGON_TARGET_TIME_PROFILING
    hexagon_start_time_cal_ms();
#endif
    isdb_mmode_cmd = hexagon_pack_isdbcmd(ISDBCMD_CMD_STUFF, ISDBCMD_MONITOR_LVL,
                                               ISDBCMD_TNUM_MASK_THREAD(0));
#ifdef _DEBUG_HEXAGON_
    LOG_DEBUG("hexagon_read_hvx_registers   isdb_mmode_cmd = %d ", isdb_mmode_cmd);
#endif
    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 16; j++)
        {
            hvx_register[i][j] = 32670;
        }
    }
    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 16; j++)
        {
            retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
                                            hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
            if (retval != ERROR_OK)
                LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
#ifdef _DEBUG_HEXAGON_
            LOG_DEBUG("ISDB status before ISDBMBXIN write 0x%x", isdbsts);
#endif
            retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
                                             hexa_info->debug_base + HEXAGON_ISDB_ISDBMBXIN, j * 4);
            if (retval != ERROR_OK)
                LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
            retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
                                             hexa_info->debug_base + HEXAGON_ISDB_STFINST, 0x6ea8c000);
            if (retval != ERROR_OK)
                LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
            retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
                                             hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
            if (retval != ERROR_OK)
                LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
            hexagon_wait_loop();
            retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
                                             hexa_info->debug_base + HEXAGON_ISDB_STFINST, vextract_opcode_v0 + 100 * i);
            if (retval != ERROR_OK)
                LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
            retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
                                             hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
            if (retval != ERROR_OK)
                LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
            hexagon_wait_loop();
            retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
                                             hexa_info->debug_base + HEXAGON_ISDB_STFINST, 0x6701c029);
            if (retval != ERROR_OK)
                LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
            retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
                                             hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
            if (retval != ERROR_OK)
                LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
            hexagon_wait_loop();
            retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
                                            hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
            if (retval != ERROR_OK)
                LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
#ifdef _DEBUG_HEXAGON_
            LOG_DEBUG("ISDBST read status after 2nd stuff inst 0x%x", isdbsts);
#endif
            isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
            if (isdb_cmd_status)
            {
                LOG_DEBUG("ISDB command failed in monitor mode");
                return ERROR_OK;
            }
            else
            {
                while (!(isdbsts & ISDBST_ISDB_MAILBOX_OUT))
                {
                    retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
                                                    hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
                    l++;
                    if (l == 10)
                        break;
                }
#ifdef _DEBUG_HEXAGON_
                LOG_DEBUG("ISDBST status before reading MBXOUT = 0x%x, reg = %d", isdbsts, j);
#endif
                l = 0;
                if (!(isdbsts & ISDBST_ISDB_MAILBOX_OUT))
                {
                    LOG_DEBUG("ISDBST status not set for mailbox so skiping reading it for register R%d", j);
                    continue;
                }
                retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
                                                hexa_info->debug_base + HEXAGON_ISDB_ISDBMBXOUT, &hvx_register[i][j]);
                if (retval != ERROR_OK)
                {
                    LOG_DEBUG("HEXAGON_ISDB_ISDBMBXOUT read failed 0x%x", hvx_register[i][j]);
                }
            }
        }
    }
#ifdef _HEXAGON_TARGET_TIME_PROFILING
    hexagon_end_time_cal_ms();
    LOG_DEBUG("Total time taken  %" PRId64 "ms", hexagon_time_total);
#endif
    LOG_DEBUG("******Printing HVX registers*****");
    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 16; j++)
        {
            LOG_DEBUG("hvx_register[%d][%d]  =  0x%x", i, j, hvx_register[i][j]);
        }
    }
#ifdef _DEBUG_HEXAGON_
    LOG_DEBUG("hexagon_read_hvx_registers  exit");
#endif
    return ERROR_OK;
}
int hexagon_read_current_registers(struct target *target, uint32_t hwthrd)
{
	


	/* read general purpose registers (R0-R1) */
	hexagon_read_gpr_registers(target, hwthrd);

	/* read per thread control registers */
	hexagon_read_ctrl_registers(target, hwthrd);

	/* read per thread monitor mode control registers */
	hexagon_read_mmode_registers(target, hwthrd);

	/* read global control registers */
	hexagon_read_global_ctrl_registers(target);

	/* Restore the register used stuff instruction */
	hexagon_restore_stuff_used_reg(target);
	
	LOG_DEBUG("Dumping Registers %s", target_name(target));
	hexagon_dump_hwthrd_reg(target, gHexConfig.maxHwThreads);
	if (mmu_init)
		hexagon_read_tlb_entry(target);


	

	return ERROR_OK;

}
static uint32_t hexagon_print_pc(struct target *target)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct reg_cache *cache;
	uint32_t i;

	/* dump registers for the requested HW thread */
	cache = hexa_info->core_cache;
	for ( i = 0 ; i  <  gHexConfig.maxHwThreads;  i++)
	{
		LOG_DEBUG("Value of PC  %s : %s = 0x%x", cache->name, cache->reg_list[HEXAGON_PC].name, 
					*(cache->reg_list[HEXAGON_PC].value));
		cache = cache->next;
	}
	cache = hexa_info->core_cache;
	/* return PC value for HW thread 0 but printing it for all HW thread*/
	return  *(cache->reg_list[HEXAGON_PC].value);
}

void current_debug_thread (uint32_t selected_thread)
{ 
	//comes as input from gdb_server with +1
	// debug_thread_id = selected_thread - 1;
	thread_id_thread_select = selected_thread - 1;
	LOG_DEBUG("Current debug thread is %lld", thread_id_thread_select);
	return;
}
static int hexagon_step(struct target *target, int current, target_addr_t address,
			int handle_breakpoints)
{

#ifdef HEXAGON_DEBUG_LOGS
    start = clock();
#endif
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct adiv5_dap *swddp = hexa_info->dap;
	int retval = ERROR_OK;
	uint64_t isdbcmd;
    uint32_t output = 0, isdbsts = 0, isdb_cmd_status = 0;

#ifdef HEXAGON_DEBUG_LOGS
    LOG_INFO("Inside Step function");
#endif
	retval = isdbcmd = isdbsts = 0;

	if (target->state != TARGET_HALTED)
	{
		LOG_DEBUG("Step is requested when target is not halted, polling again");
		hexagon_poll(target);
		if (target->state != TARGET_HALTED)
			return ERROR_TARGET_NOT_HALTED;
	}
#ifdef HEXAGON_DEBUG_LOGS
    func_start = clock();
#endif
	hexagon_stuff_reg_restore(target);
#ifdef HEXAGON_DEBUG_LOGS
    func_end = clock();
    func_time = ((double)(func_end - func_start))/CLOCKS_PER_SEC;
    LOG_INFO("hexagon_stuff_reg_restore:  %lf", func_time);
#endif

#ifdef HEXAGON_DEBUG_LOGS
    func_start = clock();
#endif

	if (multi_thr_enabled == true)
	{
		isdbcmd |= ISDBCMD_CMD_ISTEP;
		isdbcmd |= ISDBCMD_MONITOR_LVL;
		isdbcmd |= ISDBCMD_TNUM_MASK_THREAD(thread_id_thread_select);	
	}
	else
	{

		isdbcmd=0x183;
	}
	retval = enable_dbg_sys_pwr(swddp);
	retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdbcmd);

	if (retval != ERROR_OK) 
	{
		LOG_DEBUG("ISDCMD write failed 0x%llx", isdbcmd);
		return retval;
	}
	/* Wait for some time  to enable ISDB clk */
	hexagon_wait_loop();
	
		
	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
				hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
	if (retval != ERROR_OK) {
		LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
	}
	
	/* if isdb_cmd_status 0 in cmd was successfull in case of 1 failed */ 
	isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
	if(isdb_cmd_status)
	{
		 LOG_DEBUG("ISDB command failed returning from hexagon_step function ");
		 return ERROR_FAIL;
	}
	
#ifdef  _HEXAGON_TARGET_TIME_PROFILING
		hexagon_end_time_cal_ms();
		LOG_DEBUG("Total time taken  %" PRId64 "ms", hexagon_time_total);
#endif
	
	target->debug_reason = DBG_REASON_SINGLESTEP;
	
	//hexagon_read_current_registers(target, HEXAGON_HW_THREAD0);
#ifdef HEXAGON_DEBUG_LOGS
    func_start = clock();
#endif    
	/* read general purpose registers (R0-R1) */
	hexagon_read_gpr_registers(target, gHexConfig.maxHwThreads);
#ifdef HEXAGON_DEBUG_LOGS
    func_end = clock();
    func_time = ((double)(func_end - func_start))/CLOCKS_PER_SEC;
    LOG_INFO("hexagon_read_gpr_registers:  %lf", func_time);
#endif
	
#ifdef HEXAGON_DEBUG_LOGS
    func_start = clock();
#endif     
	/* read per thread control registers */
	hexagon_read_ctrl_registers(target, gHexConfig.maxHwThreads);
	
#ifdef HEXAGON_DEBUG_LOGS
    func_end = clock();
    func_time = ((double)(func_end - func_start))/CLOCKS_PER_SEC;
    LOG_INFO("hexagon_read_ctrl_registers:  %lf", func_time);
#endif
	
#ifdef HEXAGON_DEBUG_LOGS
    func_start = clock();
#endif         
	/* read per thread monitor mode control registers */
	// hexagon_read_mmode_registers(target, gHexConfig.maxHwThreads);
#ifdef HEXAGON_DEBUG_LOGS
    func_end = clock();
    func_time = ((double)(func_end - func_start))/CLOCKS_PER_SEC;
    LOG_INFO("hexagon_read_mmode_registers:  %lf", func_time);
#endif

#ifdef HEXAGON_DEBUG_LOGS
    func_start = clock();
#endif       
	/* read global control registers */
	// hexagon_read_global_ctrl_registers(target);
#ifdef HEXAGON_DEBUG_LOGS
    func_end = clock();
    func_time = ((double)(func_end - func_start))/CLOCKS_PER_SEC;
    LOG_INFO("hexagon_read_global_ctrl_registers:  %lf", func_time);
#endif

	/* Restore the register used stuff instruction */
#ifdef HEXAGON_DEBUG_LOGS
    func_start = clock();
#endif        
	hexagon_restore_stuff_used_reg(target);
#ifdef HEXAGON_DEBUG_LOGS
    func_end = clock();
    func_time = ((double)(func_end - func_start))/CLOCKS_PER_SEC;
    LOG_INFO("hexagon_restore_stuff_used_reg:  %lf", func_time);
// #endif

    double before_vtlb;
    end = clock();
    before_vtlb = ((double)(end - start))/CLOCKS_PER_SEC;
    LOG_INFO("before_vtlb:  %lf", before_vtlb);
#endif

// #ifdef HEXAGON_DEBUG_LOGS
//     func_start = clock();
// #endif       
	hexagon_read_tlb_entry(target);
// #ifdef HEXAGON_DEBUG_LOGS
//     func_end = clock();
//     func_time = ((double)(func_end - func_start))/CLOCKS_PER_SEC;
// #endif
    if (QURTK_vtlb_revision != 0x0)
    {
        Q6_global_settings.refresh_indicator = QURTK_vtlb_revision;
        hexagon_memw_read (target, Q6_global_settings.refresh_indicator, &output);
        Q6_global_settings.refresh_indicator = output;
        hexagon_memw_read (target, Q6_global_settings.refresh_indicator, &output);
        #ifdef HEXAGON_DEBUG_LOGS
            LOG_INFO("QURTK_vtlb_revision at 0x%x is 0x%x and the previous revision number is 0x%x", Q6_global_settings.refresh_indicator, output, Q6_global_settings.revision_num );
        #endif     
    }
    if (mmu_init)
    {   
        if (bitmap_init == false) // before bitmap array has been read once and cleared, we need to populate all entries once
            {
        #ifdef HEXAGON_DEBUG_LOGS
                LOG_INFO("Populating all entries");
        #endif    
			hexagon_populate_vtlb_refresh_entries(target);
            }
            else if (output != Q6_global_settings.revision_num) 
            {
				
                Q6_global_settings.revision_num = output;
                retval = hexagon_update_modified_vtlb_entry(target);
            }
            // else
            // {
            //     LOG_INFO("no vtlb update needed");
            // }
		// #ifdef HEXAGON_DEBUG_LOGS
            // end = clock();
            // execution_time = ((double)(end - start ))/CLOCKS_PER_SEC;
            // LOG_INFO("Overall execution:  %lf", execution_time);
		// #endif
    }
	sbp_step_executed = 1;

	//LOG_DEBUG("Exiting %s\n",__FUNCTION__);

	return ERROR_OK;
}

int hexagon_update_modified_vtlb_entry(struct target *target)
{
    int retval;
    // uint64_t output;
	uint32_t output;

    unsigned int vtlb_entry_count = hexagon_vtlb_data.vtlb_no_of_entries;
    uint64_t vtlb_entries_read[vtlb_entry_count];
    unsigned int vtlb_notify_size;
    vtlb_notify_size = (vtlb_entry_count % 32)? ((vtlb_entry_count/32) +1) :  (vtlb_entry_count/32);
    unsigned int bitmap_array_contents[vtlb_notify_size];
    unsigned int size_vtlb_binary = (vtlb_entry_count*4);
    // target_addr_t bitmap_addr_temp ;
	uint32_t bitmap_addr_temp;
    // target_addr_t vtlb_entries_temp;
	uint32_t vtlb_entries_temp;

    unsigned int* final_str = (unsigned int *) calloc(size_vtlb_binary, sizeof(unsigned int));
    unsigned int final_str_ind = 0;

    hexagon_memw_read(target, bitmap_addr, &bitmap_addr_temp);
    for (unsigned int i = 0; i < vtlb_notify_size; i++)
    {   
        hexagon_memw_read(target, bitmap_addr_temp, &output);
        bitmap_array_contents[i] = output;
        bitmap_addr_temp = bitmap_addr_temp + 4;
    } 
#ifdef HEXAGON_DEBUG_LOGS
    LOG_INFO("size of bitmap : %d",sizeof(bitmap_array_contents) / sizeof(bitmap_array_contents[0]));
#endif   
    for(unsigned int i = 0; i < (vtlb_notify_size); i++) 
    {
        unsigned int binaryNum[32] = {0};
        decToBinary(bitmap_array_contents[i], binaryNum);
        for(int j = 0; j < 32; j++) 
        {
            final_str[final_str_ind] = binaryNum[j];
            final_str_ind++;
        }
    }
    hexagon_memw_read(target, qurtk_vtlb_entries, &vtlb_entries_temp);
    for(unsigned int i = 0; i < size_vtlb_binary; i++)
    {
        if (final_str[i]!=0)
        {   
            if (i < vtlb_entry_count)
            {
                hexagon_memw_read(target, vtlb_entries_temp, &output);
                vtlb_entries_read[i] = output;
                vtlb_entries_temp = vtlb_entries_temp + 4; 
                hexagon_memw_read(target, vtlb_entries_temp, &output);
                vtlb_entries_read[i+1] = output;
                vtlb_entries_temp = vtlb_entries_temp + 4;
            }
            if (i < (sizeof(vtlb_entries_read) / sizeof(vtlb_entries_read[0])))
            {
                hexagon_update_vtlb_entry_in_structure(vtlb_entries_read[i],vtlb_entries_read[i+1],i);
            }
        }
        else
        {
                vtlb_entries_temp = vtlb_entries_temp + 8; 
        }
        i++;
    }
    free(final_str);
    bitmap_addr = qurtk_vtlb_bitmap;
    hexagon_memw_read(target, bitmap_addr, &bitmap_addr_temp);
    for (unsigned int i = 0; i < hexagon_vtlb_data.vtlb_no_of_entries/32; i++)
    {   
        retval = hexagon_memw_write(target, bitmap_addr_temp, 0x0 , 4);
		if (retval != ERROR_OK)
			LOG_DEBUG("bitmap write failed");
        bitmap_addr_temp = bitmap_addr_temp + 4;
    }


	

    return ERROR_OK;
}
static int hexagon_dump_isdb_reg(struct hexa_info *hexa_info)
{
	struct adiv5_dap *swddp = hexa_info->dap;
	uint32_t reg_value;
	int retval = ERROR_OK, i;

	/* dump the all registers */ 
	retval = enable_dbg_sys_pwr(swddp);
	for (i = 0; i < HEXAGON_MAX_ISDB_REG; i++)
	{
		reg_value = 0;
		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
					hexa_info->debug_base + (i*4), &reg_value);

        if (retval != ERROR_OK)
        {
			LOG_DEBUG("ISDB reg(addr: 0x%x) read failed", (i*4));
			return retval;
		}	
		#ifdef  _DEBUG_HEXAGON_
		LOG_DEBUG("ISDB reg dump: ISDB reg(addr: 0x%x) -> 0x%x", reg_value);
		#endif
	}

	

	return retval;
}

static int hexagon_resume(struct target *target, int current, target_addr_t address,
	int handle_breakpoints, int debug_execution)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct adiv5_dap *swddp = hexa_info->dap;
	int retval = ERROR_OK;
	uint64_t isdbcmd,  isdb_cmd_status;
	uint32_t isdbsts;
	static int retry_cnt = 0;
	// hexagon_sync(target);
	
	hexagon_dump_isdb_reg(&hexagon->hexa_info);
    for (uint32_t j = 0; j < gHexConfig.maxHwThreads; j++)
    {
        hexagon_read_BRKPT_through_stuff(target,j);
    }
	LOG_DEBUG("Entering, Params passed: current = %d  :  address = 0x%llx  :handle_breakpoints = %d  :	debug_execution = %d", 
				current, address, handle_breakpoints, debug_execution);
	if (target->state != TARGET_HALTED)
	{
		LOG_DEBUG("Resume is requested when target is not halted, polling again");
		hexagon_poll(target);
		if (target->state != TARGET_HALTED)
			return ERROR_TARGET_NOT_HALTED;
	}

#ifdef  _HEXAGON_TARGET_TIME_PROFILING
		hexagon_start_time_cal_ms();
#endif
	
	if(((hexa_info->brkptinfo & BRKPTINFO_THREAD0_BRKPT_SOURCE)>>(0)) == HEXA_DBG_SWBRKPT){
		
		LOG_DEBUG("Halted reason HEXA_DBG_SWBRKPT");
		
		//basic sanity before proceeding
		struct breakpoint *current_breakpoint = target->breakpoints;
		uint8_t PC_matched_with_sbp_addr = 0;
		struct reg_cache *cache = hexa_info->core_cache;
		uint8_t i = 0;
		
		while(current_breakpoint != NULL)
		{
            LOG_DEBUG("Current PC : 0x%x ", *((uint32_t *)cache->reg_list[HEXAGON_PC].value));
			LOG_DEBUG("current_breakpoint->address = 0x%llx", current_breakpoint->address);
			LOG_DEBUG("current_breakpoint->type = 0x%x", current_breakpoint->type);
			LOG_DEBUG("current_breakpoint->is_set = 0x%x", current_breakpoint->is_set);
			LOG_DEBUG("current_breakpoint->orig_instr = 0x%hhn", current_breakpoint->orig_instr);
			LOG_DEBUG("current_breakpoint->next = 0x%p",(void *) current_breakpoint->next);
			i = 0;
			cache = hexa_info->core_cache;
			while ((cache != NULL))
			{
				LOG_DEBUG("*((uint32_t*)cache->reg_list[HEXAGON_PC].value) = 0x%x", *((uint32_t*)cache->reg_list[HEXAGON_PC].value));
                if (*((uint32_t *)cache->reg_list[HEXAGON_PC].value) == ((current_breakpoint->address)))
				{
					LOG_DEBUG("breakpoint address match with PC found in the bp-list, Replacing the original instruction in place of breakpoint");
					PC_matched_with_sbp_addr = 1;
					break;
				}
					
				cache = cache->next;
				i++;
			}
			if(PC_matched_with_sbp_addr)
				break;
				
		//	LOG_DEBUG("Reached here" );
			current_breakpoint = current_breakpoint->next;
		//	LOG_DEBUG("Reached here" );
			
			if(current_breakpoint)
			{
				LOG_DEBUG("current_breakpoint->address = 0x%llx", current_breakpoint->address);
				LOG_DEBUG("current_breakpoint->type = 0x%x", current_breakpoint->type);
				LOG_DEBUG("current_breakpoint->is_set = 0x%x", current_breakpoint->is_set);
				LOG_DEBUG("current_breakpoint->orig_instr = 0x%p", (void *)current_breakpoint->orig_instr);
				LOG_DEBUG("current_breakpoint->next = 0x%p", (void *) current_breakpoint->next);
			}
			//LOG_DEBUG("Reached here" );
		}
		if (multi_thr_enabled==false)
			isdbcmd=0x182;
		
		if(!PC_matched_with_sbp_addr)
		{
            LOG_INFO("breakpoint not found in the list, trying to force resume");
            if (retval == ERROR_OK)
                retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
                                     hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdbcmd);
			if (retval == ERROR_OK)
			{
				goto resumed;
			}
            return retval;
		}
		

		retval = hexagon_write_ctrl_register(target, HEXAGON_PC, i, current_breakpoint->address);
		
		//do a step 0x11 isdbcmd
		if(!sbp_step_executed)
		{
			retval = hexagon_step(target, 1, 0x00, 0); //current = 1: continue on current pc, otherwise continue at <address> modified the PC so step from the current PC
		}
		if (retval != ERROR_OK) 
			LOG_DEBUG("Step command failed");

		//Replace original instrn with brkpt instruction
		retval = ERROR_OK;		
		
		retval = hexagon_memw_write_instruction_memory(target, current_breakpoint->address, 0x6c20c000, 0);;
		if (retval != ERROR_OK) 
			LOG_DEBUG("hexagon_memw_write_instruction_memory return value is not OK");
		//continue
		LOG_DEBUG("All steps are successful before continuing in case of sw BP");
		
	}
retry:
	//Todo: check the position of retry label, in case of very frequent breakpoints, whether it
	//		needs to be placed above the SW preakpoint preprocessing or it's current place, i.e Just before continue cmd

	retval = isdbsts = isdb_cmd_status = 0;
	hexagon_stuff_reg_restore(target);

	if (multi_thr_enabled == true)
	{
		if(gHexConfig.maxHwThreads == 2)
		{
			/* Send ISDB command(Resume) to resume all halted threads*/
			isdbcmd |= ISDBCMD_CMD_RESUME;
			isdbcmd |= ISDBCMD_MONITOR_LVL;
			isdbcmd |= ISDBCMD_TNUM_MASK_2;
		}
		else if(gHexConfig.maxHwThreads == 4)
		{
			/* Send ISDB command(Resume) to resume all halted threads*/
			isdbcmd |= ISDBCMD_CMD_RESUME;
			isdbcmd |= ISDBCMD_MONITOR_LVL;
			isdbcmd |= ISDBCMD_TNUM_MASK_4;
			
		}
		else if(gHexConfig.maxHwThreads == 6)
		{
			/* Send ISDB command(Resume) to resume all halted threads*/
			isdbcmd |= ISDBCMD_CMD_RESUME;
			isdbcmd |= ISDBCMD_MONITOR_LVL;
			isdbcmd |= ISDBCMD_TNUM_MASK_6;
		}
		else if(gHexConfig.maxHwThreads == 8)
		{
			/* Send ISDB command(Resume) to resume all halted threads*/
			isdbcmd |= ISDBCMD_CMD_RESUME;
			isdbcmd |= ISDBCMD_MONITOR_LVL;
			isdbcmd |= ISDBCMD_TNUM_MASK_8;
		}
	
	}
	else
	{
		// single thread resume
		isdbcmd = 0x182;
	}

	LOG_DEBUG("Writing HEXAGON_ISDB_ISDBCMD = 0x%llx, to resume target", isdbcmd);
	// retval = enable_dbg_sys_pwr(swddp);

	retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdbcmd);
	if (retval != ERROR_OK) 
	{
		LOG_DEBUG("ISDCMD write failed 0x%llx", isdbcmd);
		return retval;
	}
	retval = hexagon_read_ISDB(target, isdbsts, ISDBST_ISDB_CMD_STATUS);
	
	uint8_t isdbst_read_retry_cnt = 0;
	while(isdbst_read_retry_cnt < 4)
	{
		
		if(!((isdbsts & ISDBST_DEBUG_MODE_STATUS) >> 8))
		//  here debug mode status bits are expected to be 0 when resumed
		{
			LOG_DEBUG("target resumed; ISDBST_DEBUG_MODE_STATUS is 0x%x", ((isdbsts & ISDBST_DEBUG_MODE_STATUS)>>8));
			LOG_DEBUG("ISDBST_DEBUG_MODE_STATUS value after trying target resume: 0x%x", ((isdbsts & ISDBST_DEBUG_MODE_STATUS)>>8));
			LOG_DEBUG("ISDBST_WAITRUN_MODE_STATUS value after trying target resume: 0x%x", ((isdbsts & ISDBST_WAITRUN_MODE_STATUS)>>24));
			goto resumed;
		}
		retval = enable_dbg_sys_pwr(swddp);
		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
		LOG_DEBUG("HEXAGON_ISDB_ISDBST value after trying target resume: 0x%x", isdbsts);

		isdbst_read_retry_cnt++;
	}
	
	LOG_DEBUG("ISDBST_DEBUG_MODE_STATUS value after trying target resume: 0x%x", ((isdbsts & ISDBST_DEBUG_MODE_STATUS)>>8));
	LOG_DEBUG("ISDBST_WAITRUN_MODE_STATUS value after trying target resume: 0x%x", ((isdbsts & ISDBST_WAITRUN_MODE_STATUS)>>24));
	
	if((isdbsts & ISDBST_DEBUG_MODE_STATUS)>>8)
	//  if it is nonzero

	{
		LOG_DEBUG("Unable to resume target; ISDBST_DEBUG_MODE_STATUS is nonzero 0x%x", ((isdbsts & ISDBST_DEBUG_MODE_STATUS)>>8));

		//handle retry here
		retry_cnt++;
        if (retry_cnt < 4)
        {
			goto retry;
		}
		LOG_DEBUG("Unable to resume target After 10 Retry");
		hexagon_read_gpr_registers(target, gHexConfig.maxHwThreads);
		hexagon_read_ctrl_registers(target, gHexConfig.maxHwThreads);

		return ERROR_TARGET_FAILURE;		
	}
	//LOG_DEBUG("hexagon_halt  target->debug_reason =%d",target->debug_reason);

resumed:
	LOG_DEBUG("Target %s resumed and PC 0x%x", target_name(target),hexagon_print_pc(target));
	
	hexagon_write_syscfg_register(target,hexagon_syscfg_reg);
	hexagon_stuff_reg_restore(target);
	target->debug_reason = DBG_REASON_NOTHALTED;
	
	/* Wait for some time  to enable ISDB clk */
	hexagon_wait_loop();

	// LOG_INFO("finished long wait");
	if (!debug_execution)
	{
		target->state = TARGET_RUNNING;
		target_call_event_callbacks(target, TARGET_EVENT_RESUMED);
		LOG_DEBUG(" !debug_execution : target resumed at 0x%x" , hexagon_print_pc(target));
	} 
	else 
	{
		target->state = TARGET_DEBUG_RUNNING;
		target_call_event_callbacks(target, TARGET_EVENT_DEBUG_RESUMED);
		LOG_DEBUG("target debug resumed at 0x%x" , hexagon_print_pc(target));
	}	

	
#ifdef  _HEXAGON_TARGET_TIME_PROFILING
		hexagon_end_time_cal_ms();
		LOG_DEBUG("Total time taken  %" PRId64 "ms", hexagon_time_total);
#endif
	
	
	//resetting the global data structures to keep track of Business logic involved in SW BP

	for (uint32_t j=0; j < gHexConfig.maxHwThreads;  j++)
	{
		gpSbpHaltedThreadsPC[j] = 0;
	}

	sbp_step_executed = 0;
	return ERROR_OK;
}

static int hexagon_read_BRKPT_through_stuff(struct target *target, uint32_t hwthrd)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct adiv5_dap *swddp = hexa_info->dap;
    // uint32_t isdb_mmode_cmd = 0x0;

	int retval , i=0;
	uint64_t stuff_inst[][4] = {{0x6ea4c007, 0x6707c029},
						{0x6ea6c007, 0x6707c029},
						{0x6ea5c007, 0x6707c029},
						{0x6ea7c007, 0x6707c029}};		//r7=PC0,PC1,CFG0,CFG1; isdbmbxout = r7;
	uint32_t read_val[4] = {};
	uint32_t isdbsts;
	uint32_t isdb_mmode_cmd = 0x184;
    // isdb_mmode_cmd = hexagon_pack_isdbcmd(ISDBCMD_CMD_STUFF, ISDBCMD_MONITOR_LVL,
    //                                             ISDBCMD_TNUM_MASK_THREAD(hwthrd));
    uint32_t  isdb_cmd_status;

	LOG_DEBUG("%s ------ %d\n",__FUNCTION__,__LINE__);

	

				
	/*there are 2 stuff instruction, here programming first inst */
	// /*
	retval = enable_dbg_sys_pwr(swddp);
	if (retval != ERROR_OK) {
			LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
	}
	// */
	
	for (int j = 0; j < 4; j++)
	{
		// /*
		retval = enable_dbg_sys_pwr(swddp);
		if (retval != ERROR_OK) {
			LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
		}
		// */
		/*there are 2 stuff instruction, here programming first inst */
		
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
					hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst[j][0]);
        if (retval != ERROR_OK)
        {
				LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK for stuff_inst[j][0],j=%d",j);
		}
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
                                         hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
        if (retval != ERROR_OK)
        {
				LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK for stuff_inst[j][0],j=%d",j);
		}
	
		/* wait till the stuff instruction is executed */
		hexagon_wait_loop();
				
		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
				hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
        if (retval != ERROR_OK)
        {
			LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
		}
		if (isdbsts == 0x0)
		{
			LOG_DEBUG("ERROR: ISDBSTS reg has become 0x0");
			// return ERROR_FAIL;
		}
			
		LOG_DEBUG("ISDBST read status after first stuff inst j = %d, 0x%x", j,isdbsts);
		/* if isdb_cmd_status 0 in cmd was successfull in case of 1 failed */ 
		isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
	
		if (isdb_cmd_status)
		{
			LOG_DEBUG("ISDBcommand failed in monitor  mode");
			// /*
			retval = enable_dbg_sys_pwr(swddp);
			if (retval != ERROR_OK) {
					LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
			}
			// */
			
			retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst[j][0]);
            if (retval != ERROR_OK)
            {
				LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK for stuff_inst[j][0],j= %d",j);
			}
			retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
                                             hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
            if (retval != ERROR_OK)
            {
                LOG_DEBUG("HEXAGON_ISDB_ISDBCMD isdb_mmode_cmd return value is not OK for stuff_inst[j][0],j=%d", j);
			}
			retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
					hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
            if (retval != ERROR_OK)
            {
					LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
				
			}
			LOG_DEBUG("ISDBST read status 0x%x", isdbsts);
			/* if isdb_cmd_status 0 in cmd was successfull in case of 1 failed */ 
			isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
	
			if (isdb_cmd_status)
			{
				LOG_DEBUG("ISDBcommand failed in user mode");
			        //  /* 
				retval = enable_dbg_sys_pwr(swddp);
				if (retval != ERROR_OK) {
						LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
				}
				// */
				
				retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst[j][0]);
                if (retval != ERROR_OK)
                {
						LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
				}
				retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
                                                 hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
                if (retval != ERROR_OK)
                {
                    LOG_DEBUG("HEXAGON_ISDB_ISDBCMD isdb_mmode_cmd return value is not OK");
				}
				retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
                if (retval != ERROR_OK)
                {
						LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
					
				}
				LOG_DEBUG("ISDBST read status 0x%x", isdbsts);
				/* if isdb_cmd_status 0 in cmd was successfull in case of 1 failed */ 
				isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
				if(isdb_cmd_status)
				{
					 LOG_DEBUG("ISDBcommand failed in Guest mode");
					 return ERROR_OK;
				}
			}
		}
		/*there are 2 stuff instruction, here programming second inst */
		// /*
		retval = enable_dbg_sys_pwr(swddp);
		if (retval != ERROR_OK) {
				LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
		}
		// */
		
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
					hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst[j][1]);
        if (retval != ERROR_OK)
        {
				LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
		}
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
                                         hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
        if (retval != ERROR_OK)
        {
				LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
		}
		/* wait till the stuff instruction is executed */
		hexagon_wait_loop();
					
		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
				hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
        if (retval != ERROR_OK)
        {
				LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
		}
		LOG_DEBUG("ISDBST read status after 2nd stuff inst 0x%x", isdbsts);
		/* if isdb_cmd_status 0 in cmd was successfull in case of 1 failed */ 
		isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
		if (isdb_cmd_status)
		{
			LOG_DEBUG("ISDBcommand failed in monitor mode");
			// /*
			retval = enable_dbg_sys_pwr(swddp);
			if (retval != ERROR_OK) {
					LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
			}
			// */
			
			retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst[j][1]);
            if (retval != ERROR_OK)
            {
					LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
			}
			retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
                                             hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
            if (retval != ERROR_OK)
            {
                LOG_DEBUG("HEXAGON_ISDB_ISDBCMD isdb_mmode_cmd return value is not OK");
			}
	
			/* wait till the stuff instruction is executed */
			hexagon_wait_loop();
			
			retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
					hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
            if (retval != ERROR_OK)
            {
					LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
				
			}
			LOG_DEBUG("ISDBST read status 0x%x", isdbsts);
			/* if isdb_cmd_status 0 in cmd was successfull in case of 1 failed */ 
			isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
	
			if(isdb_cmd_status)
			{
				  LOG_DEBUG("ISDBcommand failed in user  mode");
				// /* 
				retval = enable_dbg_sys_pwr(swddp);
				if (retval != ERROR_OK) {
						LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
				}
				// */
				
				retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
							hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst[j][1]);
                if (retval != ERROR_OK)
                {
						LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
				}
				retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
                                                 hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
                if (retval != ERROR_OK)
                {
                    LOG_DEBUG("HEXAGON_ISDB_ISDBCMD isdb_mmode_cmd return value is not OK");
				}
	
				/* wait till the stuff instruction is executed */
				hexagon_wait_loop();
			
				retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
						hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
                if (retval != ERROR_OK)
                {
						LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
					
				}
				LOG_DEBUG("ISDBST read status 0x%x", isdbsts);
				/* if isdb_cmd_status 0 in cmd was successfull in case of 1 failed */ 
				isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
				if(isdb_cmd_status)
				{
					   LOG_DEBUG("ISDBcommand failed in Guest  mode");
					 return ERROR_OK;
				}
			}
		}
		retval = enable_dbg_sys_pwr(swddp);
				if (retval != ERROR_OK) {
						LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
				}
		/*reading mailboxout status in ISDBST register */
		while(!(isdbsts & ISDBST_ISDB_MAILBOX_OUT))
		{
			retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
				hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
				/* wait till the stuff instruction is executed */
				hexagon_wait_loop();
			i++;
			if (i==10)
				break;
		}
		LOG_DEBUG("ISDBST status value before  reading mailbox register 0x%x, i = %d", isdbsts,i);
		i = 0;
		if(!(isdbsts & ISDBST_ISDB_MAILBOX_OUT))
		{
			LOG_DEBUG("ISDBST status not set for mailbox so skiping reading it for iteration %d", j);
			continue;
		}
		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
				hexa_info->debug_base + HEXAGON_ISDB_ISDBMBXOUT, &read_val[j]);
		if (retval != ERROR_OK) 
		{
			LOG_DEBUG("HEXAGON_ISDB_ISDBMBXOUT read failed 0x%x", read_val[j]);
		}
		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
				hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
		LOG_DEBUG("HEXAGON_ISDB_ISDBMBXOUT value 0x%x", read_val[j]);
		LOG_DEBUG("ISDBST status value after  reading mailbox register 0x%x ", isdbsts);
	}
	retval = enable_dbg_sys_pwr(swddp);
	if (retval != ERROR_OK) {
		LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
	}

	return ERROR_OK;
}

static int hexagon_check_state_one(struct target *target,
		uint64_t mask, bool *halted, uint32_t *debug_thread)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct adiv5_dap *swddp = hexa_info->dap;
	uint32_t isdbsts = 0;
	int retval;

	if (halted == NULL || debug_thread == NULL )
	{
		LOG_DEBUG(" Fail: halted : %d, debug_thread 0x%x", *halted, *debug_thread);
		return ERROR_FAIL;
	}
	
	/*	Check ISDB status for threads in  debug mode */
	retval = enable_dbg_sys_pwr(swddp);
	
	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
    if (retval != ERROR_OK)
    {
		LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
		return retval;
	}
	if (halted)
	{
		*debug_thread = (isdbsts & mask) >> 8;
		*halted = (*debug_thread) > 0 ? true : false;
	}
	//LOG_INFO("ISDB STATUS: 0x%x, halted 0x%x, debug_thread 0x%x", isdbsts, *halted, isdbsts); 
	/*
	if(!*halted)
	{
		LOG_INFO("ISDB STATUS: 0x%x, halted 0x%x, debug_thread 0x%x", isdbsts, *halted, isdbsts); 
	}*/

	

	return ERROR_OK;
}
static int hexagon_debug_entry(struct target *target)
{
	// static uint8_t vtlb_initialized = 0;
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct adiv5_dap *swddp = hexa_info->dap;
	int retval = ERROR_OK;
	uint32_t brkptinfo = 0;
	uint64_t *thrd_src;

	thrd_src = (uint64_t *) malloc ( gHexConfig.maxHwThreads * sizeof(uint64_t) );
	if (thrd_src == NULL) {
        LOG_DEBUG("Memory allocation for thrd_src failed");
        return ERROR_FAIL;
    }
	//  initialize thrd_src
	memset(thrd_src, 0, gHexConfig.maxHwThreads * sizeof(uint64_t));

	LOG_DEBUG("hexagon_debug_entry  %s", target_name(target));

	retval = enable_dbg_sys_pwr(swddp);
	
	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_BRKPTINFO, &brkptinfo);

	if (retval != ERROR_OK) {
		LOG_DEBUG("BRKPTINFO read failed 0x%x", brkptinfo);
		free(thrd_src);
		return retval;
	}
	
	LOG_DEBUG("TARGET_HALTED, HEXAGON_ISDB_BRKPTINFO = 0x%x \n",brkptinfo);

	if(gHexConfig.maxHwThreads == 2)
	{
		thrd_src[0] = ((brkptinfo & BRKPTINFO_THREAD0_BRKPT_SOURCE)>>(0));
		thrd_src[1] = ((brkptinfo & BRKPTINFO_THREAD1_BRKPT_SOURCE)>>(3));
	}
	else 	if(gHexConfig.maxHwThreads == 4)
	{
		thrd_src[0] = ((brkptinfo & BRKPTINFO_THREAD0_BRKPT_SOURCE)>>(0));
		thrd_src[1] = ((brkptinfo & BRKPTINFO_THREAD1_BRKPT_SOURCE)>>(3));
		thrd_src[2] = ((brkptinfo & BRKPTINFO_THREAD2_BRKPT_SOURCE)>>(6));
		thrd_src[3] =  ((brkptinfo & BRKPTINFO_THREAD3_BRKPT_SOURCE)>>(9));
	}
	else if (gHexConfig.maxHwThreads == 6)
	{
		thrd_src[0] = ((brkptinfo & BRKPTINFO_THREAD0_BRKPT_SOURCE)>>(0));
		thrd_src[1] = ((brkptinfo & BRKPTINFO_THREAD1_BRKPT_SOURCE)>>(3));
		thrd_src[2] = ((brkptinfo & BRKPTINFO_THREAD2_BRKPT_SOURCE)>>(6));
		thrd_src[3] =  ((brkptinfo & BRKPTINFO_THREAD3_BRKPT_SOURCE)>>(9));
		thrd_src[4] =   ((brkptinfo & BRKPTINFO_THREAD4_BRKPT_SOURCE)>>(12));
		thrd_src[5] =  ((brkptinfo & BRKPTINFO_THREAD5_BRKPT_SOURCE)>>(15));
	}
		
	for(uint32_t i =0; i < gHexConfig.maxHwThreads; i++){
			
			LOG_DEBUG("Debug reason  thrd_src[%d]= %llx", i,thrd_src[i]);

			switch(thrd_src[i])
			{
				case 0b000:
					LOG_DEBUG("Thread num %d has hit Hardware breakpoint 0\n", i);
					LOG_INFO("HW Breakpoint 0 hit" );
					break;
					
				case 0b001:
					LOG_DEBUG("Thread num %d has hit Hardware breakpoint 1\n", i);
					LOG_INFO("HW Breakpoint 1 hit" );
					break;
				
				case 0b010:
					LOG_DEBUG("Thread num %d has hexecuted BRKPT instruction\n", i);
					LOG_INFO("SW Breakpoint hit" );
					break;
					
				case 0b011:
					LOG_DEBUG("Thread num %d has hit ETM Breakpoint\n", i);
					break;
					
				case 0b100:
					LOG_DEBUG("Thread num %d has hit APB Breakpoint\n", i);
					break;
					
				case 0b101:
					LOG_DEBUG("Thread num %d has External breakpoint\n", i);
					break;
					
				default:
					LOG_DEBUG("Default case: Thread num %d Breakpoint source = %llx\n", i, thrd_src[i]);
					break;		
			}	
		}
	
	/* save the current BP info */
	hexa_info->brkptinfo = brkptinfo;
	
	/* Examine debug reason */
	//hexagon_debug_reason(target, brkptinfo);
	
	//Assuming all threads are halted for the same reason.
	hexagon_debug_reason(target, thrd_src[0]);
	hexagon_read_current_registers(target, gHexConfig.maxHwThreads);
	
//  san - update mmu on halt
#if 1
	// hexagon_sync(target,);
		// if (mmu_init)

	if (multi_thr_enabled)
		if(!vtlb_initialized)
		{
			LOG_DEBUG("hexagon_populate_vtlb_data ");

			hexagon_populate_vtlb_data(target);
			if(hexagon_vtlb_data.vtlb_no_of_entries > 0)
				vtlb_initialized = 1;
		}
// #endif    
	// if (mmu_init)
		if(vtlb_initialized > 1)
		{
			LOG_DEBUG("hexagon_populate_vtlb_refresh_entries");

			hexagon_populate_vtlb_refresh_entries(target);
		}
		else
		{
			if(hexagon_vtlb_data.vtlb_no_of_entries > 0)
				vtlb_initialized++;
		}
	
#endif
	
	//*****************************************************//
	if(((hexa_info->brkptinfo & BRKPTINFO_THREAD0_BRKPT_SOURCE)>>(0)) == HEXA_DBG_SWBRKPT)
	{
		
		LOG_DEBUG("Halted reason HEXA_DBG_SWBRKPT");
		struct breakpoint *current_breakpoint = target->breakpoints;
		uint8_t PC_matched_with_sbp_addr = 0;
		struct reg_cache *cache = hexa_info->core_cache;
		uint8_t i = 0;

		for( i =0; i < gHexConfig.maxHwThreads; i++)
		{
			// Assign the address of gpPerHwThrdReg which  is uint32_t * to gpSbpHaltedThreadsPC is uint32_t ** 
			gpSbpHaltedThreadsPC[i] = &gpPerHwThrdReg[i][HEXAGON_PC];
		}
		
		cache = hexa_info->core_cache;
		while(current_breakpoint != NULL)
		{
		
			LOG_DEBUG("current_breakpoint->address = 0x%llx", current_breakpoint->address);
			LOG_DEBUG("current_breakpoint->type = 0x%x", current_breakpoint->type);
			LOG_DEBUG("current_breakpoint->is_set = 0x%x", current_breakpoint->is_set);
			LOG_DEBUG("current_breakpoint->orig_instr = 0x%hhn", current_breakpoint->orig_instr);
			LOG_DEBUG("current_breakpoint->next = 0x%p", (void *) current_breakpoint->next);
			i = 0;
			cache = hexa_info->core_cache;
			while ( i <  gHexConfig.maxHwThreads)
			{
				LOG_DEBUG("*((uint64_t*)cache->reg_list[HEXAGON_PC].value) = 0x%p", gpSbpHaltedThreadsPC[i]);
				// comparison between pointer and integer
                if (*gpSbpHaltedThreadsPC[i] == ((current_breakpoint->address) + 4))
                {
					LOG_DEBUG("breakpoint address match with PC found in the bp-list, Replacing the original instruction in place of breakpoint");
					PC_matched_with_sbp_addr = 1;
					break;
				}
				cache = cache->next;
				i++;
			}
			if(PC_matched_with_sbp_addr)
				break;

			current_breakpoint = current_breakpoint->next;
			LOG_DEBUG("Reached here" );
			
			if(current_breakpoint)
			{
					LOG_DEBUG("current_breakpoint->address = 0x%llx", current_breakpoint->address);
					LOG_DEBUG("current_breakpoint->type = 0x%x", current_breakpoint->type);
					LOG_DEBUG("current_breakpoint->is_set = 0x%x", current_breakpoint->is_set);
					LOG_DEBUG("current_breakpoint->orig_instr = 0x%hhn", current_breakpoint->orig_instr);
					LOG_DEBUG("current_breakpoint->next = 0x%p", (void *) current_breakpoint->next);
			}
			LOG_DEBUG("Reached here" );
		}
		if(!PC_matched_with_sbp_addr)
		{
			LOG_DEBUG("breakpoint not found in the list, returning with ERROR_FAIL");
			free(thrd_src);
			return ERROR_FAIL;
		}
		//set PC to PC-4
		retval = hexagon_write_ctrl_register(target, HEXAGON_PC, i, current_breakpoint->address);
		if (retval != ERROR_OK) {
			LOG_DEBUG("hexagon_write_ctrl_register HEXAGON_PC return value is not OK, returning with ERROR_FAIL");
			free(thrd_src);
			return ERROR_OK;
		}
		//Replace breakpoint instrn with original instruction
		retval = ERROR_OK;
		union fourbyte val;
		
		val.byte[0] = current_breakpoint->orig_instr[0];
		val.byte[1] = current_breakpoint->orig_instr[1];
		val.byte[2] = current_breakpoint->orig_instr[2];
		val.byte[3] = current_breakpoint->orig_instr[3];
		
		
		retval = hexagon_memw_write_instruction_memory(target, current_breakpoint->address, val.word, 1);
		if (retval != ERROR_OK) {
			LOG_DEBUG("Replacing original instruction, hexagon_memw_write_instruction_memory return value is not OK. Returning with ERROR_FAIL");
			free(thrd_src);
			return ERROR_FAIL;
		}
	}	

	free(thrd_src);
	return ERROR_OK;
}
void hexagon_debug_reason(struct target *target, uint64_t brkptinfo)
{

	


	/* Examine debug reason */
	switch (HEXA_DEBUG_ENTRY(brkptinfo)) 
	{
		case HEXA_DBG_EXTBRKPT:	
			target->debug_reason = DBG_REASON_DBGRQ;
            LOG_DEBUG("DBG_REASON_DBGRQ");
			break;
			
		case HEXA_DBG_HWBRKPT0:	
		case HEXA_DBG_HWBRKPT1: 
		case HEXA_DBG_SWBRKPT:
 	 	case HEXA_DBG_ETMBRKPT:
	 	case HEXA_DBG_APBBRKPT:
			target->debug_reason = DBG_REASON_BREAKPOINT;
            LOG_DEBUG("DBG_REASON_BREAKPOINT");
				break;
		default:
			target->debug_reason = DBG_REASON_UNDEFINED;
            LOG_DEBUG("DBG_REASON_UNDEFINED");
			break;
	}

	

}


static int hexagon_poll(struct target *target)
{
	enum target_state prev_target_state;
	int retval = ERROR_OK;
	uint32_t debug_thread = 0;
	bool halted = false;

	retval = hexagon_check_state_one(target, ISDBST_DEBUG_MODE_STATUS, &halted, &debug_thread);
#ifdef HEXAGON_DEBUG
	LOG_DEBUG("hexagon_check_state_one returned %d",retval);
#endif

	//if (retval != ERROR_OK)
		//return retval;
	static int cnt = 0;
	if((++cnt % 1000)==0){
		LOG_DEBUG("At FUNCTION:%s\t LINE:%d\n",__FUNCTION__, __LINE__);
	}

	
	if (halted == true) 
	{
		prev_target_state = target->state;
		// if((cnt%500)==0)
		if((cnt%100)==0)
		{
			LOG_DEBUG("At FUNCTION:%s\t LINE:%d, prev_target_state = %d",__FUNCTION__, __LINE__, prev_target_state);

			LOG_DEBUG("At FUNCTION:%s\t LINE:%d, (halted == true)",__FUNCTION__, __LINE__);
		}
		
		if (prev_target_state != TARGET_HALTED) 
		{
			enum target_debug_reason debug_reason = target->debug_reason;
			LOG_DEBUG("At FUNCTION:%s\t LINE:%d, prev_target_state = %d",__FUNCTION__, __LINE__, prev_target_state);
			LOG_DEBUG("At FUNCTION:%s\t LINE:%d, prev_target_state != TARGET_HALTED",__FUNCTION__, __LINE__);
			LOG_DEBUG("At FUNCTION:%s\t LINE:%d, target->debug_reason = %d",__FUNCTION__, __LINE__, debug_reason);

			/* We have a halting debug event */
			target->state = TARGET_HALTED;
			LOG_DEBUG("Target %s halted", target_name(target));
			
			retval = hexagon_debug_entry(target);

			if (retval != ERROR_OK)
				return retval;

			//if (target->smp)
				//hexagon_update_halt_gdb(target, debug_reason);
			LOG_DEBUG("entering switch case, prev state:%s\t LINE:%d, prev_target_state = %d",__FUNCTION__, __LINE__, prev_target_state);

			switch (prev_target_state) 
			{
				case TARGET_RUNNING:
				case TARGET_UNKNOWN:
				case TARGET_RESET:
					LOG_DEBUG("At FUNCTION:%s\t LINE:%d, Calling target_call_event_callbacks(target, TARGET_EVENT_HALTED)",__FUNCTION__, __LINE__);
					target_call_event_callbacks(target, TARGET_EVENT_HALTED);
					break;
				case TARGET_DEBUG_RUNNING:
					target_call_event_callbacks(target, TARGET_EVENT_DEBUG_HALTED);
					break;
				default:
					break;
			}
	   }
	} 
	else
	{
		target->state = TARGET_RUNNING;
	}

	

	return retval;
}

int  hexagon_arch_state(struct target *target)
{
	assert(target != NULL);
	// return target->arch_info;
#ifdef HEXAGON_DEBUG
	LOG_DEBUG("hexagon_arch_state returning ERROR_OK");
#endif


	

	

	return ERROR_OK;
}

/*
 * Basic debug access, very low level assumes state is saved
 */
static int hexagon_init_debug_access(struct target *target)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct adiv5_dap *swddp = hexa_info->dap;
	int retval = ERROR_OK;
	uint64_t isdbcmd, isdbcsts;
	uint32_t isdbsts = 0, bpinfo =0;

	retval = isdbcmd = isdbsts = isdbcsts = bpinfo = 0;

	LOG_DEBUG("hexagon_init_debug_access");
	// LOG_INFO("%s", target_name(target));

#ifdef HEXAGON_DEBUG
	LOG_DEBUG("Number of threads is %d",gHexConfig.maxHwThreads);
#endif

	if(gHexConfig.maxHwThreads == 2)
	{
		isdbcmd = hexagon_pack_isdbcmd(ISDBCMD_CMD_BREAK,ISDBCMD_USER_LVL,
							ISDBCMD_TNUM_MASK_2);
	}	
	else if(gHexConfig.maxHwThreads == 4)
	{
		isdbcmd = hexagon_pack_isdbcmd(ISDBCMD_CMD_BREAK,ISDBCMD_USER_LVL,
							ISDBCMD_TNUM_MASK_4);
	}
	else 	if(gHexConfig.maxHwThreads == 6)
	{
		isdbcmd = hexagon_pack_isdbcmd(ISDBCMD_CMD_BREAK, ISDBCMD_USER_LVL,
							ISDBCMD_TNUM_MASK_6);
	}
#ifdef HEXAGON_DEBUG
	LOG_DEBUG("halting with isdbcmd : 0x%x",isdbcmd);
#endif

	retval = enable_dbg_sys_pwr(swddp);
	retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdbcmd);
#ifdef HEXAGON_DEBUG
	LOG_DEBUG(" isdbcmd ap write done: 0x%x",isdbcmd);
#endif


    if (retval != ERROR_OK)
    {
		LOG_DEBUG("ISDCMD write failed 0x%llx", isdbcmd);
		return retval;
	}

		/* Wait for some time  to enable ISDB clk */
	hexagon_wait_loop();
#ifdef HEXAGON_DEBUG
	LOG_DEBUG(" hexagon_wait_loop");
#endif
	/*	Check ISDB status for threads entering debug mode */
	retval = enable_dbg_sys_pwr(swddp);
	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
    if (retval != ERROR_OK)
    {
			LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
			return retval;
	}
	
	/* check ISDB core status - reset/PC to access ISDB*/
	retval = enable_dbg_sys_pwr(swddp);

	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_BRKPTINFO, &bpinfo);
    if (retval != ERROR_OK)
    {
		LOG_DEBUG("ISDBEN read failed 0x%x", bpinfo);
		return retval;
	}
	//LOG_INFO("Finally: ISDBstatus 0x%x, brkpt info 0x%x ", isdbcmd, isdbsts, bpinfo);
	LOG_INFO("Finally: ISDBstatus 0x%x, brkpt info 0x%x ",	isdbsts, bpinfo);
	LOG_INFO("Threads entered debug mode(ISDB status 0x%x)", isdbsts);
	target->debug_reason = DBG_REASON_DBGRQ;
#ifdef HEXAGON_DEBUG
	LOG_DEBUG(" exiting hexagon_init_debug_access");
#endif

	return ERROR_OK;
}


COMMAND_HANDLER(hexagon_handle_cache_info_command)
{
#ifdef HEXAGON_DEBUG
	LOG_DEBUG(" hexagon_handle_cache_info_command returning with ERROR OK");
#endif
	//struct target *target = get_current_target(CMD_CTX);
	return ERROR_OK;
}


COMMAND_HANDLER(hexagon_handle_dbginit_command)
{
	struct target *target = get_current_target(CMD_CTX);
    if (!target_was_examined(target))
    {
		LOG_DEBUG("target not examined yet");
		return ERROR_FAIL;
	}
#ifdef HEXAGON_DEBUG
	LOG_DEBUG(" target was examined exiting hexagon_handle_dbginit_command ");
#endif
	return hexagon_init_debug_access(target);
}

COMMAND_HANDLER(hexagon_mask_interrupts_command)
{
	//struct target *target = get_current_target(CMD_CTX);
#ifdef HEXAGON_DEBUG
	LOG_DEBUG(" hexagon_mask_interrupts_command returning with ERROR OK");
#endif
	return ERROR_OK;
}

COMMAND_HANDLER(hexagon_set_QURTK_vtlb_main_command)
{
	target_addr_t addr = 0;

	if (CMD_ARGC > 1)
		return ERROR_COMMAND_SYNTAX_ERROR;

    if (CMD_ARGC == 1)
    {
		COMMAND_PARSE_ADDRESS(CMD_ARGV[0], addr);
        // hexagon_vtlb_data.QURTK_vtlb_main_VA = addr;
        qurtk_vtlb_main_addr = addr;
		command_print(CMD, " QURTK_vtlb_main = 0x%llx", addr);
		LOG_INFO("QURTK_VTLB_main = 0x%llx", addr);
	}

	return ERROR_OK;
}
COMMAND_HANDLER(hexagon_set_qurtk_vtlb_data_command)
{
	target_addr_t addr = 0;

	if (CMD_ARGC > 1)
		return ERROR_COMMAND_SYNTAX_ERROR;

	if (CMD_ARGC == 1) {
		COMMAND_PARSE_ADDRESS(CMD_ARGV[0], addr);
		hexagon_vtlb_data.QURTK_VTLB_DATA_VA=addr;
		command_print(CMD, "QURTK_VTLB_DATA = 0x%llx", addr);
		LOG_INFO("QURTK_VTLB_DATA = 0x%llx", addr);
	}

	return ERROR_OK;
}
static target_addr_t threads;
COMMAND_HANDLER(hexagon_SetHWthreads)
{

	if (CMD_ARGC > 1)
		return ERROR_COMMAND_SYNTAX_ERROR;

	if (CMD_ARGC == 1)
	{
		COMMAND_PARSE_ADDRESS(CMD_ARGV[0], threads);
		gHexConfig.maxHwThreads = threads;
		command_print(CMD, " HEXAGON_MAX_HW_THREADS_MODEM = %d", gHexConfig.maxHwThreads);
		LOG_INFO("HEXAGON_MAX_HW_THREADS_MODEM = %d", gHexConfig.maxHwThreads);
	}

	return ERROR_OK;
}
COMMAND_HANDLER(hexagon_SetQPSS6WDOGCTL)
{
	target_addr_t WDOG = 0;

	if (CMD_ARGC > 1)
		return ERROR_COMMAND_SYNTAX_ERROR;

	if (CMD_ARGC == 1)
	{
		COMMAND_PARSE_ADDRESS(CMD_ARGV[0], WDOG);
		gHexConfig.qpss6WDOGCtl = WDOG;
		command_print(CMD, " HEXAGON_MSS_QDSP6SS_WDOG_CTL = 0x%llx", WDOG);
		LOG_INFO("HEXAGON_MSS_QDSP6SS_WDOG_CTL = 0x%llx", WDOG);
	}

	return ERROR_OK;
}

COMMAND_HANDLER(hexagon_SetNumTLBEntries)
{
	target_addr_t numTLB = 0;

	if (CMD_ARGC > 1)
		return ERROR_COMMAND_SYNTAX_ERROR;

	if (CMD_ARGC == 1)
	{
		COMMAND_PARSE_ADDRESS(CMD_ARGV[0], numTLB);
		gHexConfig.numTlbEntries = numTLB;
		command_print(CMD, " HEXAGON_TLB_ENTRIES_NUM = %lld", numTLB);
		LOG_INFO("HEXAGON_TLB_ENTRIES_NUM = %lld", numTLB);
	}

	return ERROR_OK;
}
COMMAND_HANDLER(hexagon_initConfig)
{
	initConfig(&gHexConfig);
	LOG_INFO("initiating with : ");
	LOG_INFO("HEXAGON_MAX_HW_THREADS_MODEM = %d", gHexConfig.maxHwThreads);
	LOG_INFO("HEXAGON_TLB_ENTRIES_NUM = %d", gHexConfig.numTlbEntries);
	LOG_INFO("HEXAGON_MSS_QDSP6SS_WDOG_CTL = 0x%x", gHexConfig.qpss6WDOGCtl);

	return ERROR_OK;
}

COMMAND_HANDLER(hexagon_vtlbRefresh)
{
    struct target *target = get_current_target(CMD_CTX);
    start = clock();
    hexagon_populate_vtlb_refresh_entries(target);
    end = clock();
    execution_time = ((double)(end - start))/CLOCKS_PER_SEC;
    LOG_INFO("time taken for refresh_tlb_entries  %lf", execution_time);
    return ERROR_OK;
}   
void decToBinary(unsigned int n, unsigned int binaryNum[])
{
    int i = 0;
    while (n > 0 && i < 32) {
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }
}
COMMAND_HANDLER(hexagon_set_vtlb_params)
{
    // uint64_t output;
	uint32_t output;
    unsigned int vtlb_entry_count = hexagon_vtlb_data.vtlb_no_of_entries ;
    uint64_t vtlb_entries_read[vtlb_entry_count];
    unsigned int vtlb_notify_size;
    vtlb_notify_size = (vtlb_entry_count%32)? ((vtlb_entry_count/32)+1): vtlb_entry_count/32;
    unsigned int bitmap_array_contents[vtlb_notify_size];
    unsigned int size_vtlb_binary = (vtlb_entry_count*4);
    // target_addr_t bitmap_addr_temp ;
    // target_addr_t vtlb_entries_temp;
	uint32_t bitmap_addr_temp, vtlb_entries_temp;
    unsigned int* final_str = (unsigned int *)calloc(size_vtlb_binary, sizeof(unsigned int));
    unsigned int final_str_ind = 0;
    struct target *target = get_current_target(CMD_CTX);
    target_addr_t vtlb_entries;

    if (CMD_ARGC < 2)
    {
        return ERROR_COMMAND_SYNTAX_ERROR;
    }
    else if (CMD_ARGC == 2)
    {
        COMMAND_PARSE_ADDRESS(CMD_ARGV[0], bitmap_addr);
        command_print(CMD, " bitmap addr = 0x%llx", bitmap_addr);
        qurtk_vtlb_bitmap = bitmap_addr;
        COMMAND_PARSE_ADDRESS(CMD_ARGV[1], vtlb_entries);
        qurtk_vtlb_entries = vtlb_entries;
        command_print(CMD, " QURTK_vtlb_entries = 0x%llx", vtlb_entries);
    }
    hexagon_memw_read(target, bitmap_addr, &bitmap_addr_temp);
    for (unsigned int i = 0; i < vtlb_notify_size; i++)
    {   
        hexagon_memw_read(target, bitmap_addr_temp, &output);
        bitmap_array_contents[i] = output;
        bitmap_addr_temp = bitmap_addr_temp + 4;
    } 
    for(unsigned int i = 0; i < (vtlb_notify_size); i++) 
    {
        unsigned int binaryNum[32] = {0};
        decToBinary(bitmap_array_contents[i], binaryNum);
        for(int j = 0; j < 32; j++) 
	{
            final_str[final_str_ind] = binaryNum[j];
            final_str_ind++;
        }
    }
    hexagon_memw_read(target, qurtk_vtlb_entries, &vtlb_entries_temp);
    for(unsigned int i = 0; i < final_str_ind; i++)
    {
        if (final_str[i] !=0)
        {    
            if (i < vtlb_entry_count)
            {
                hexagon_memw_read(target, vtlb_entries_temp, &output);
                vtlb_entries_read[i] = output;
                vtlb_entries_temp = vtlb_entries_temp + 4; 
                hexagon_memw_read(target, vtlb_entries_temp, &output);
                vtlb_entries_read[i+1] = output;
                LOG_INFO("0x%llx\t", vtlb_entries_read[i+1]);
                vtlb_entries_temp = vtlb_entries_temp + 4;
            }
            if (i < (sizeof(vtlb_entries_read) / sizeof(vtlb_entries_read[0])))
            {
                hexagon_update_vtlb_entry_in_structure(vtlb_entries_read[i],vtlb_entries_read[i+1],i);
            }
        }
        else
        {
            vtlb_entries_temp = vtlb_entries_temp + 8;
        }
        i++;
    }
    free(final_str);
    bitmap_init = true;
    return ERROR_OK;
}
COMMAND_HANDLER(hexagon_set_revision_addr)
{
    target_addr_t input;
    uint32_t output;
    struct target *target = get_current_target(CMD_CTX);

    if (CMD_ARGC > 1)
        return ERROR_COMMAND_SYNTAX_ERROR;

    if (CMD_ARGC == 1)
    {
        COMMAND_PARSE_ADDRESS(CMD_ARGV[0], input);
        QURTK_vtlb_revision = input;
        command_print(CMD, " QURTK_vtlb_revision = 0x%llx", QURTK_vtlb_revision);
    }
	// QURTK_vtlb_revision addr is read from elf
	// to which we perform a read that returns refresh indicator address
	// returned address is 32 bit
    LOG_INFO("QURTK_vtlb_revision  at = 0x%llx ", QURTK_vtlb_revision);
    hexagon_memw_read (target, QURTK_vtlb_revision, &output);
    Q6_global_settings.refresh_indicator = output;
    hexagon_memw_read (target, Q6_global_settings.refresh_indicator, &output);
    Q6_global_settings.revision_num = output;
    LOG_INFO("revision num is = 0x%x ", Q6_global_settings.revision_num);
    return ERROR_OK;
}

COMMAND_HANDLER(hexagon_mmu_init)
{
    if (CMD_ARGC > 1)
        return ERROR_COMMAND_SYNTAX_ERROR;

    if (CMD_ARGC == 0)
    {
        mmu_init = true;    
		vtlb_initialized = 1;

        command_print(CMD, "only vtlb logic is initialized");
		LOG_INFO("only vtlb logic initialized");

    }

    return ERROR_OK;
}

COMMAND_HANDLER(hexagon_multi_th_init)
{

    if (CMD_ARGC > 0)
        return ERROR_COMMAND_SYNTAX_ERROR;

    if (CMD_ARGC == 0)
    {
        mmu_init = true;    
        multi_thr_enabled=true;
        gHexConfig.maxHwThreads = threads;

        command_print(CMD, "vtlb logic and multithreading mode is initialized ");
		LOG_INFO("multithreading mode initialized");

    }

    return ERROR_OK;
}

COMMAND_HANDLER(hexagon_refresh_registers)
{
    struct target *target = get_current_target(CMD_CTX);
    if (CMD_ARGC > 0)
	{
        return ERROR_COMMAND_SYNTAX_ERROR;
	}
	hexagon_read_gpr_registers(target, gHexConfig.maxHwThreads);
	hexagon_read_ctrl_registers(target, gHexConfig.maxHwThreads);

    return ERROR_OK;
}

COMMAND_HANDLER(hexagon_clear_bitmap_array)
{
    uint32_t output;
    int retval;
    struct target *target = get_current_target(CMD_CTX);
    bitmap_addr = qurtk_vtlb_bitmap;
    // target_addr_t bitmap_addr_temp ;
	uint32_t bitmap_addr_temp;
    LOG_INFO("QURTK_vtlb_bitmap array at = 0x%llx ", bitmap_addr);
    hexagon_memw_read(target, bitmap_addr, &bitmap_addr_temp);

    for (unsigned int i = 0; i < hexagon_vtlb_data.vtlb_no_of_entries/32; i++)
    {   
        retval = hexagon_memw_write(target, bitmap_addr_temp, 0x0 , 4);
		if (retval != ERROR_OK)
		{
			LOG_DEBUG("clear bitmap write failed");
		}
        bitmap_addr_temp = bitmap_addr_temp + 4;
    } 

    bitmap_addr = qurtk_vtlb_bitmap;
    hexagon_memw_read(target, bitmap_addr, &bitmap_addr_temp);

    for (unsigned int i = 0; i < hexagon_vtlb_data.vtlb_no_of_entries/32; i++)
    {   
        hexagon_memw_read(target, bitmap_addr_temp, &output);
        LOG_INFO("bitmap content at 0x%x is 0x%x",bitmap_addr_temp, output);
        bitmap_addr_temp = bitmap_addr_temp + 4;
    } 

    return ERROR_OK;
}
COMMAND_HANDLER(hexagon_APB_RW)
{
    uint32_t output;
    int retval;
    struct target *target = get_current_target(CMD_CTX);
    struct hexagon_common *hexagon = target->arch_info;
    struct hexa_info *hexa_info = &hexagon->hexa_info;
    target_addr_t addr, value;


	if (CMD_ARGC > 2)
        return ERROR_COMMAND_SYNTAX_ERROR;
    else if (CMD_ARGC == 1)
    {
        COMMAND_PARSE_ADDRESS(CMD_ARGV[0], addr);
        command_print(CMD, " reg_addr = 0x%llx",  addr);
        retval = mem_ap_read_atomic_u32(hexa_info->debug_ap, addr , &output);
        LOG_INFO("output read is 0x%x", output);
    }
    else if (CMD_ARGC == 2)
    {
        COMMAND_PARSE_ADDRESS(CMD_ARGV[0], addr);
        command_print(CMD, " reg_addr = 0x%llx", addr);
        retval = mem_ap_read_atomic_u32(hexa_info->debug_ap, addr, &output);
        LOG_INFO("output read is 0x%x", output);
        COMMAND_PARSE_ADDRESS(CMD_ARGV[1], value);
        command_print(CMD, " value = 0x%llx", value);
        LOG_INFO("value = 0x%llx ", value);
        retval = mem_ap_write_atomic_u32(hexa_info->debug_ap, addr, value);
        LOG_INFO("value written is  0x%llx", value);
	}

    retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
                                    addr, &output);
	if (retval != ERROR_OK)
	{
		LOG_DEBUG ("read failed for addr : 0x%llx", addr);
	}

    LOG_INFO("output read is 0x%x", output);

    return ERROR_OK;
}
COMMAND_HANDLER(hexagon_dap_RW)
{
    uint32_t output;
    int retval;
    struct target *target = get_current_target(CMD_CTX);
    struct hexagon_common *hexagon = target->arch_info;
    struct hexa_info *hexa_info = &hexagon->hexa_info;
    target_addr_t reg_addr, value;

    if (CMD_ARGC > 2)
        return ERROR_COMMAND_SYNTAX_ERROR;
    else if (CMD_ARGC == 1)
    {
        COMMAND_PARSE_ADDRESS(CMD_ARGV[0], reg_addr);
        command_print(CMD, " reg_addr = 0x%llx", hexa_info->debug_base + reg_addr);
        retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
                                        hexa_info->debug_base + reg_addr , &output);
        LOG_INFO("output read is 0x%x", output);
    }
    else if (CMD_ARGC == 2)
    {
        COMMAND_PARSE_ADDRESS(CMD_ARGV[0], reg_addr);
        command_print(CMD, " reg_addr = 0x%llx", hexa_info->debug_base + reg_addr);
        retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
                                        hexa_info->debug_base + reg_addr, &output);
        LOG_INFO("output read is 0x%x", output);
        COMMAND_PARSE_ADDRESS(CMD_ARGV[1], value);
        command_print(CMD, " value = 0x%llx", value);
        LOG_INFO("value = 0x%llx ", value);
        retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
                                        hexa_info->debug_base + reg_addr, value);
        LOG_INFO("value written is  0x%llx", value);
	}

    retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
                                        hexa_info->debug_base + reg_addr, &output);
	if (retval != ERROR_OK)
	{
		LOG_DEBUG ("read failed for addr : 0x%llx", reg_addr);
	}
    LOG_INFO("output read is 0x%x", output);
	return ERROR_OK;
}
COMMAND_HANDLER(hexagon_ReadClkRegs)
{
	target_addr_t Reg_addr[8];
	uint32_t i;

	for (i = 0; i <= 7 ; i++)
	{
		COMMAND_PARSE_ADDRESS(CMD_ARGV[i], Reg_addr[i]);
		gHexConfig.clkEnAddr[i] = Reg_addr[i];
		command_print(CMD, " Register address = 0x%llx", Reg_addr[i]);
		LOG_INFO("Register address = 0x%llx", Reg_addr[i]);
	}
	current_hexagon_subsystem.hexagon_adsp = true;
	return ERROR_OK;
}
COMMAND_HANDLER(hexagon_SetETMclkAddress)
{
    target_addr_t etm_clk_en_Addr=0;
    target_addr_t etm_clk_Res_Addr = 0;

    COMMAND_PARSE_ADDRESS(CMD_ARGV[0], etm_clk_en_Addr);
    gHexConfig.etmClkenAddr = etm_clk_en_Addr;
    command_print(CMD, " ETM clock enable address = 0x%x", gHexConfig.etmClkenAddr);
    LOG_INFO(" ETM clock enable address = 0x%x", gHexConfig.etmClkenAddr);
    COMMAND_PARSE_ADDRESS(CMD_ARGV[1], etm_clk_Res_Addr);
    gHexConfig.etmResetAddr = etm_clk_Res_Addr;
    command_print(CMD, " ETM clock Reset address = 0x%x", gHexConfig.etmResetAddr);
    LOG_INFO(" ETM clock reset address = 0x%x", gHexConfig.etmResetAddr);

    // }

    return ERROR_OK;
}

COMMAND_HANDLER(hexagon_reset_spurious_brkpt)
{
	is_spurious_breakpoint = 1;
	LOG_INFO("is_spurious_breakpoint = 0x%d ", is_spurious_breakpoint);
	return ERROR_OK;
}

COMMAND_HANDLER (hexagon_wait_time)
{	
	target_addr_t input_loop_count;
	if (CMD_ARGC > 2)
        return ERROR_COMMAND_SYNTAX_ERROR;
    else if (CMD_ARGC == 1)
    {
        COMMAND_PARSE_ADDRESS(CMD_ARGV[0], input_loop_count);
        command_print(CMD, " loop count = %lld", input_loop_count);
		loop_count = input_loop_count;
        LOG_INFO("loop count is %lld", loop_count);
    }
	LOG_INFO("loop_count = %lld ", loop_count);
	return ERROR_OK;
}
COMMAND_HANDLER(hexagon_mem_dap)
{
    int retval;
    // clock_t start_buffer, end_buffer;
    struct target *target = get_current_target(CMD_CTX);
    struct hexagon_common *hexagon = target->arch_info;
    struct hexa_info *hexa_info = &hexagon->hexa_info;
    uint64_t virt_addr = 0 , phy_addr=0, value = 0;
	uint32_t read_value =  0, isdbsts = 0;


    if (CMD_ARGC > 2)
        return ERROR_COMMAND_SYNTAX_ERROR;

	// hexagon_populate_vtlb_entries(target);
	// hexagon_read_tlb_entry(target);

	
    if (CMD_ARGC == 0)
    {
		LOG_INFO ("DUMPING VTLB ENTRIES");
		hexagon_print_vtlb_entries();
		LOG_INFO ("DUMPING TLB ENTRIES");
		hexagon_read_tlb_entry(target);
		return ERROR_OK;
    }

    if (CMD_ARGC == 1)
    {
        COMMAND_PARSE_ADDRESS(CMD_ARGV[0], virt_addr);
        command_print(CMD, " virt_addr = 0x%llx", virt_addr);
        LOG_INFO("virt_addr = 0x%llx ", virt_addr);
		retval =  hexagon_virt2phys(target, virt_addr, &phy_addr);

		hexagon_memw_phys_read(target, phy_addr, &read_value);

		// LOG_INFO (" phy_addr is  : 0x%x", phy_addr);
		LOG_INFO ("0x%llx", phy_addr);

		return ERROR_OK;

    }

	if (CMD_ARGC == 2)
    {
		COMMAND_PARSE_ADDRESS(CMD_ARGV[0], virt_addr);
        command_print(CMD, " virt_addr = 0x%llx", virt_addr);
        LOG_INFO("virt_addr = 0x%llx ", virt_addr);

		COMMAND_PARSE_ADDRESS(CMD_ARGV[1], value);
        command_print(CMD, " value = 0x%llx", value);
        LOG_INFO("value = 0x%llx ", value);
		// retval = hexagon_new_memw_write(target, virt_addr, value, 1);
		// hexagon_stuff_reg_restore(target);
		retval = hexagon_memw_write(target, virt_addr, value, 1);

		LOG_DEBUG("hexagon_memw_write  Exit");
    }


    retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
                                        hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);

	if (retval != ERROR_OK)
            LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
	

    return ERROR_OK;
}
COMMAND_HANDLER(hexagon_handle_untrusted_command)
{
	struct target *target = get_current_target(CMD_CTX);
	if (!target_was_examined(target))
	{
		LOG_DEBUG("target not examined yet");
		return ERROR_FAIL;
	}
	return hexagon_untrusted_mode();
}
static const struct command_registration hexagon_exec_command_handlers[] = {
	{
		.name = "cache_info",
		.handler = hexagon_handle_cache_info_command,
		.mode = COMMAND_EXEC,
		.help = "display information about hexagon caches",
        .usage = "",
    },
	{
        .name = "mem_dap",
        .handler = hexagon_mem_dap,
        .mode = COMMAND_ANY,
        .help = "memw virtual read",
        .usage = "[]",
    },
	{
        .name = "spurious_brkpt_reset",
        .handler = hexagon_reset_spurious_brkpt,
        .mode = COMMAND_EXEC,
        .help = "to help ignore or consider the first breakpoint sent from lldb while establishing gdb connection",
		.usage = "",
	},
    {
        .name = "mmu_init",
        .handler = hexagon_mmu_init,
        .mode = COMMAND_EXEC,
        .help = "vtlb has been initialized",
		.usage = "",
	},
	{
		.name = "dbginit",
		.handler = hexagon_handle_dbginit_command,
		.mode = COMMAND_EXEC,
		.help = "Initialize Hexagon debug",
		.usage = "",
	},
	{
		.name = "maskisr",
		.handler = hexagon_mask_interrupts_command,
		.mode = COMMAND_ANY,
		.help = "mask hexagon interrupts during single-step",
		.usage = "['on'|'off']",
	},
	{
		.name = "SetVTLBMainAddress",
		.handler = hexagon_set_QURTK_vtlb_main_command,
		.mode = COMMAND_ANY,
		.help = "Return vtlb main address from the elf file",
		.usage = "[address]",
	},
	{
		.name = "SetVTLBDataAddress",
		.handler = hexagon_set_qurtk_vtlb_data_command,
		.mode = COMMAND_ANY,
		.help = "Return vtlb data address from the elf file",
		.usage = "[address]",
	},
	{
		.name = "SetMaxHWThreads",
		.handler = hexagon_SetHWthreads,
		.mode = COMMAND_ANY,
		.help = "Set the maximum number of hardware threads",
		.usage = "[Number]",
	},
	{
        .name = "setRevision",
        .handler = hexagon_set_revision_addr,
        .mode = COMMAND_ANY,
        .help = "Set the revision address",
        .usage = "[Number]",
    },
    {
		.name = "SetWDOGCTL",
		.handler = hexagon_SetQPSS6WDOGCTL,
		.mode = COMMAND_ANY,
		.help = "Set Watchdog Control address",
		.usage = "[address]",
	},
	{
		.name = "SetNumTLBEntries",
		.handler = hexagon_SetNumTLBEntries,
		.mode = COMMAND_ANY,
		.help = "Set Number of TLB Entries",
		.usage = "[Number]",
	},
	{
        .name = "refreshVTLB",
        .handler = hexagon_vtlbRefresh,
        .mode = COMMAND_ANY,
        .help = "Refresh VTLB entries",
        .usage = "[Number]",
    },
    {
        .name = "setVTLBParam",
        .handler = hexagon_set_vtlb_params,
        .mode = COMMAND_ANY,
        .help = "set vtlb entries",
        .usage = "[]",
    },
    {   
        .name = "clear_vtlb_entries",
        .handler = hexagon_clear_bitmap_array,
        .mode = COMMAND_ANY,
        .help = "clear bitmap array",
        .usage = "[]",
    },
    // {
    //     .name = "mem_dap",
    //     .handler = hexagon_mem_dap,
    //     .mode = COMMAND_ANY,
    //     .help = "memw virtual read",
    //     .usage = "[]",
    // },
    {
		.name = "initConfig",
		.handler = hexagon_initConfig,
		.mode = COMMAND_ANY,
		.help = "Initiate with the set values",
		.usage = "[]",
	},
	{
        .name = "DAP",
        .handler = hexagon_dap_RW,
        .mode = COMMAND_ANY,
        .help = "find modified vtlb",
        .usage = "[]",
    },
    {   
        .name = "RegRefresh",
        .handler = hexagon_refresh_registers,
		.mode = COMMAND_ANY,
        .help = "refresh all registers and system status",
        .usage = "[]",
	},
	{
		.name = "EnableClockRegs",
		.handler = hexagon_ReadClkRegs,
		.mode = COMMAND_ANY,
		.help = "Enable AXI by setting register addresses",
		.usage = "[address1] [address2] [address3] [address4] [address5] [address6] [address7] [address8]",
	},
	{
		.name = "SetEtmClkAddress",
		.handler = hexagon_SetETMclkAddress,
		.mode = COMMAND_ANY,
		.help = "Set ETM addresses",
		.usage = "[address]",
	},
	{
        .name = "multi_th_init",
        .handler = hexagon_multi_th_init,
        .mode = COMMAND_EXEC,
        .help = "vtlb with multithreading mode has been initialized",
        .usage = "",
    },
	{
        .name = "time_delay",
        .handler = hexagon_wait_time,
        .mode = COMMAND_EXEC,
        .help = "delay period between DAP writes to ISDB register writes",
        .usage = "",
	},
	{
        .name = "APB",
        .handler = hexagon_APB_RW,
        .mode = COMMAND_ANY,
        .help = "Reads/Writes via APB",
        .usage = "[]",
    },
	/*Hexagon untrusted mode*/
	{
		.name = "untrusted",
		.handler = hexagon_handle_untrusted_command,
		.mode = COMMAND_EXEC,
		.help = "Initialize Hexagon in untrusted mode",
		.usage = "",
	},
	{
		.chain = smp_command_handlers,
	},
	COMMAND_REGISTRATION_DONE};

static const struct command_registration hexagon_command_handlers[] = {
	{
		.name = "hexagon",
		.mode = COMMAND_ANY,
		.help = "hexagon command group",
		.usage = "command <> <>",
		.chain = hexagon_exec_command_handlers,
	},
    COMMAND_REGISTRATION_DONE
};
enum aarch64_cfg_param
{
	CFG_CTI,
};

static const struct jim_nvp nvp_config_opts[] = 
{
	{ .name = "-cti", .value = CFG_CTI },
	{ .name = NULL, .value = -1 }
};
struct hexagon_private_config 
{
    struct adiv5_private_config adiv5_config;
    struct arm_cti *cti;
};
static int hexagon_handle_target_request(void *priv)
{
    struct target *target = priv;
    struct hexagon_common *hexagon = target->arch_info;
    struct hexa_info *hexa_info = &hexagon->hexa_info;
    int retval;
    static int cnt = 0;
    uint32_t *thrd_src;

    thrd_src = (uint32_t *) malloc(gHexConfig.maxHwThreads * sizeof(uint32_t));

    if ((++cnt % 1000) == 0)
        LOG_DEBUG("hexagon_handle_target_request exam=%d dbg=%d state=%d ", target->examined, target->dbg_msg_enabled, target->state); 

    if (target->state == TARGET_HALTED && ((cnt % 200) == 0))
    {
		
        LOG_DEBUG("TARGET_HALTED\n"); 
        // Check the halt reason

        uint32_t brkptinfo = 0;
        retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
                hexa_info->debug_base + HEXAGON_ISDB_BRKPTINFO, &brkptinfo); 
        if (retval != ERROR_OK) {
            LOG_DEBUG("Could not read brkptinfo, retval = %d", retval);
            return retval;
        }

        LOG_DEBUG("TARGET_HALTED, HEXAGON_ISDB_BRKPTINFO = 0x%x \n", brkptinfo);

        if (gHexConfig.maxHwThreads == 2)
        {
            thrd_src[0] = ((brkptinfo & BRKPTINFO_THREAD0_BRKPT_SOURCE) >> (0));
            thrd_src[1] = ((brkptinfo & BRKPTINFO_THREAD1_BRKPT_SOURCE) >> (3));
        }
        else if (gHexConfig.maxHwThreads == 4)
        {
            thrd_src[0] = ((brkptinfo & BRKPTINFO_THREAD0_BRKPT_SOURCE) >> (0));
            thrd_src[1] = ((brkptinfo & BRKPTINFO_THREAD1_BRKPT_SOURCE) >> (3));
            thrd_src[2] = ((brkptinfo & BRKPTINFO_THREAD2_BRKPT_SOURCE) >> (6));
            thrd_src[3] = ((brkptinfo & BRKPTINFO_THREAD3_BRKPT_SOURCE) >> (9));
        }
        else if (gHexConfig.maxHwThreads == 6)
        {
            thrd_src[0] = ((brkptinfo & BRKPTINFO_THREAD0_BRKPT_SOURCE) >> (0));
            thrd_src[1] = ((brkptinfo & BRKPTINFO_THREAD1_BRKPT_SOURCE) >> (3));
            thrd_src[2] = ((brkptinfo & BRKPTINFO_THREAD2_BRKPT_SOURCE) >> (6));
            thrd_src[3] = ((brkptinfo & BRKPTINFO_THREAD3_BRKPT_SOURCE) >> (9));
            thrd_src[4] = ((brkptinfo & BRKPTINFO_THREAD4_BRKPT_SOURCE) >> (12));
            thrd_src[5] = ((brkptinfo & BRKPTINFO_THREAD5_BRKPT_SOURCE) >> (15));
        }

        for (uint32_t i = 0; i < gHexConfig.maxHwThreads; i++)
        {
            LOG_DEBUG("Debug reason  thrd_src[%d]= %x", i, thrd_src[i]);

            switch (thrd_src[i])
            {
                case 0b000:
                    LOG_DEBUG("Thread num %d has hit Hardware breakpoint 0\n", i);
                    target->debug_reason = DBG_REASON_BREAKPOINT;
                    break;

                case 0b001:
                    LOG_DEBUG("Thread num %d has hit Hardware breakpoint 1\n", i);
                    target->debug_reason = DBG_REASON_BREAKPOINT;
                    break;

                case 0b010:
                    LOG_DEBUG("Thread num %d has executed BRKPT instruction\n", i);
                    target->debug_reason = DBG_REASON_BREAKPOINT;
                    break;

                case 0b011:
                    LOG_DEBUG("Thread num %d has hit ETM Breakpoint\n", i);
                    break;

                case 0b100:
                    LOG_DEBUG("Thread num %d has hit APB Breakpoint\n", i);
                    break;

                case 0b101:
                    LOG_DEBUG("Thread num %d has External breakpoint\n", i);
                    break;

                default:
                    LOG_DEBUG("Default case: Thread num %d Breakpoint source = %x\n", i, thrd_src[i]);
                    break;        
            }    
        }
    }

    if (!target_was_examined(target))
        return ERROR_OK;
    if (!target->dbg_msg_enabled)
        return ERROR_OK;
    free(thrd_src);
    return ERROR_OK;
}

static int hexagon_init_arch_info(struct target *target,
	struct hexagon_common *hexagon, struct adiv5_dap *dap)
{
	struct hexa_info *hexa_info = &hexagon->hexa_info;

      LOG_INFO("hexagon_init_arch_info");

	hexagon->common_magic = HEXAGON_COMMON_MAGIC; 
	hexa_info->dap = dap;
	hexa_info->arch_info = hexagon;
	target->arch_info = hexagon;
	hexa_info->target= target;
	memset(global_reg, 0, sizeof(global_reg));   
	target_register_timer_callback(hexagon_handle_target_request, 5,
		TARGET_TIMER_TYPE_PERIODIC, target);

	return ERROR_OK;
}


static int hexagon_target_create(struct target *target, Jim_Interp *interp)
{

	struct hexagon_private_config *pc = target->private_config;
	struct hexagon_common *hexagon;

	LOG_INFO("hexagon_target_create");
	
	if (adiv5_verify_config(&pc->adiv5_config) != ERROR_OK)
		return ERROR_FAIL;

	hexagon = calloc(1, sizeof(struct hexagon_common));
	if (hexagon == NULL) 
	{
		LOG_DEBUG("Out of memory");
		return ERROR_FAIL;
	}

	return hexagon_init_arch_info(target, hexagon, pc->adiv5_config.dap);

}

static int hexagon_jim_configure(struct target *target, struct jim_getopt_info *goi)
{
	struct hexagon_private_config *pc;
	struct jim_nvp *n;
	int e;

	LOG_INFO("hexagon_jim_configure");

	pc = (struct hexagon_private_config *)target->private_config;
	if (pc == NULL) 
	{
			pc = calloc(1, sizeof(struct hexagon_private_config));
			target->private_config = pc;
	}

	/*
	 * Call adiv5_jim_configure() to parse the common DAP options
	 * It will return JIM_CONTINUE if it didn't find any known
	 * options, JIM_OK if it correctly parsed the topmost option
	 * and JIM_ERR if an error occured during parameter evaluation.
	 * For JIM_CONTINUE, we check our own params.
	 */
	e = adiv5_jim_configure(target, goi);
	if (e != JIM_CONTINUE)
		return e;

	/* parse config or cget options ... */
	if (goi->argc > 0) {
		Jim_SetEmptyResult(goi->interp);

		/* check first if topmost item is for us */
		e = jim_nvp_name2value_obj(goi->interp, nvp_config_opts,
				goi->argv[0], &n);
		if (e != JIM_OK)
			return JIM_CONTINUE;

		e = jim_getopt_obj(goi, NULL);
		if (e != JIM_OK)
			return e;

		switch (n->value) 
		{
			case CFG_CTI:
			{
				if (goi->isconfigure) 
				{
					Jim_Obj *o_cti;
					struct arm_cti *cti;
					e = jim_getopt_obj(goi, &o_cti);
					if (e != JIM_OK)
						return e;
					cti = cti_instance_by_jim_obj(goi->interp, o_cti);
					if (cti == NULL) 
					{
						Jim_SetResultString(goi->interp, "CTI name invalid!", -1);
						return JIM_ERR;
					}
					pc->cti = cti;
				} 
				else 
				{
					if (goi->argc != 0) 
					{
						Jim_WrongNumArgs(goi->interp,
								goi->argc, goi->argv,
								"NO PARAMS");
						return JIM_ERR;
					}

					if (pc == NULL || pc->cti == NULL) 
					{
						Jim_SetResultString(goi->interp, "CTI not configured", -1);
						return JIM_ERR;
					}
					Jim_SetResultString(goi->interp, arm_cti_name(pc->cti), -1);
				}
				break;
			}

		default:
			return JIM_CONTINUE;
		}
	}

	return JIM_OK;

}

static void hexagon_wait_loop(void)
{
 	uint16_t i, loop = 0;

	for (i = 0; i < loop_count; i++)
	{
		if ((i % 1000) == 0)
			//LOG_DEBUG("in hexagon_wait_loop()");
			loop++;
	}
}

/* This function enable the ETM */
static uint64_t hexagon_etm_on(struct target *target)
{
	struct hexagon_common *hexagon = target->arch_info;
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct adiv5_dap *swddp = hexa_info->dap;
	static int initialized;
	uint64_t retval = ERROR_OK;
	uint32_t tmp;

	// gHexConfig.etmResetAddr = 0x86988008,
	// gHexConfig.etmClkenAddr = 0x86988000,


	if(!initialized)
	{
		retval = enable_dbg_sys_pwr(swddp);
		LOG_INFO("Enabling ETM ");
		tmp = 0x3;
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap, gHexConfig.etmClkenAddr, tmp);

        if (retval != ERROR_OK)
        {
			LOG_DEBUG("unable to enable ETM clk 0x%x", tmp);
			return retval;
		}
		hexagon_wait_loop();
		tmp = 0x1;
		// retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,etm_clk_reset_addr, tmp);
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap, gHexConfig.etmResetAddr , tmp);

        if (retval != ERROR_OK)
        {
			LOG_DEBUG("unable to reset ETM 0x%x", tmp);
			return retval;
		}
		hexagon_wait_loop();
		tmp = 0x0;
        retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,gHexConfig.etmResetAddr, tmp);

		if (retval != ERROR_OK)
        {
			LOG_DEBUG("APB unlock fail");
			return retval;
		}
		hexagon_wait_loop();
		LOG_INFO("After Enabling ETM ");
		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap, gHexConfig.etmClkenAddr , &tmp);
        if (retval != ERROR_OK)
        {
			LOG_DEBUG("unable to read ETM clk 0x%x", tmp);
			return retval;
		}
		LOG_DEBUG("ETM clk 0x%x", tmp);
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,gHexConfig.etmResetAddr, tmp);
        if (retval != ERROR_OK)
        {
			LOG_DEBUG("unable to read ETM reset 0x%x", tmp);
			return retval;
		}
		LOG_DEBUG("ETM reset 0x%x", tmp);
		initialized = 1;
	}
	return retval;
	
}
/*  This function is used to disable the watchdog during debug */
static void hexagon_hw_watchdog_disable(struct target *target)
{
	int retval;
	uint32_t temp;

	LOG_DEBUG("hexagon_hw_watchdog_disable  enter "); 
		
	if(debug_axi_ap == NULL)
		hexagon_initialize_axi_ap(target);

	if(debug_axi_ap == NULL)
	{
		LOG_DEBUG("debug_axi_ap not available so watchdog is not disabled "); 
		return;
	}
	retval = mem_ap_read_buf(debug_axi_ap,(uint8_t *)&temp, 4, 1, gHexConfig.qpss6WDOGCtl);
	if(retval != ERROR_OK)
		LOG_DEBUG("read api failed"); 
	
	LOG_DEBUG("HEXAGON_QDSP6SS_WDOG_CTL read value  = 0x%x ",temp);
	
	retval = mem_ap_write_atomic_u32(debug_axi_ap,
				gHexConfig.qpss6WDOGCtl, HEXAGON_MSS_QDSP6SS_WDOG_DISABLE);
	
	if(retval != ERROR_OK)
		LOG_DEBUG("WDOG disabled failed"); 
		
	hexagon_wait_loop();
	retval = mem_ap_read_buf(debug_axi_ap,(uint8_t *)&temp, 4, 1, gHexConfig.qpss6WDOGCtl);
	if(retval != ERROR_OK)
		LOG_DEBUG("read api failed"); 
	
	LOG_DEBUG("HEXAGON_QDSP6SS_WDOG_CTL  read value after write  = 0x%x ",temp); 
	LOG_INFO("hexagon_hw_watchdog_disable  exit "); 

}

/*  This function is used to enable  the clocks  needed for  debug */
static void hexagon_enable_clock(struct target *target)
{
	int retval;

	LOG_DEBUG("hexagon_enable_clock  enter "); 
		
	if(debug_axi_ap == NULL)
		hexagon_initialize_axi_ap(target);

	if(debug_axi_ap == NULL)
	{
		LOG_DEBUG("debug_axi_ap not available so clock is not enabled "); 
		return;
	}
	retval = mem_ap_write_atomic_u32(debug_axi_ap, gHexConfig.clkEnAddr[0], 0x20008001);
	if (retval != ERROR_OK)
		LOG_DEBUG("unable to perform enable clock register write");
	retval = mem_ap_write_atomic_u32(debug_axi_ap, gHexConfig.clkEnAddr[1], 0x20008001);
	if (retval != ERROR_OK)
		LOG_DEBUG("unable to perform enable clock register write");
	retval = mem_ap_write_atomic_u32(debug_axi_ap, gHexConfig.clkEnAddr[2], 0x1);
	if (retval != ERROR_OK)
		LOG_DEBUG("unable to perform enable clock register write");
	retval = mem_ap_write_atomic_u32(debug_axi_ap, gHexConfig.clkEnAddr[3], 0x1);
	if (retval != ERROR_OK)
		LOG_DEBUG("unable to perform enable clock register write");
	retval = mem_ap_write_atomic_u32(debug_axi_ap, gHexConfig.clkEnAddr[4], 0x1);
	if (retval != ERROR_OK)
		LOG_DEBUG("unable to perform enable clock register write");
	retval = mem_ap_write_atomic_u32(debug_axi_ap, gHexConfig.clkEnAddr[5], 0x1);
	if (retval != ERROR_OK)
		LOG_DEBUG("unable to perform enable clock register write");
	retval = mem_ap_write_atomic_u32(debug_axi_ap, gHexConfig.clkEnAddr[6], 0x20000001);
	if (retval != ERROR_OK)
		LOG_DEBUG("unable to perform enable clock register write");
	retval = mem_ap_write_atomic_u32(debug_axi_ap, gHexConfig.clkEnAddr[7], 0x1);
	if (retval != ERROR_OK)
		LOG_DEBUG("unable to perform enable clock register write");

	LOG_INFO("hexagon_enable_clock  exit "); 

}


static int hexagon_examine_first(struct target *target)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);

	//LOG_DEBUG("Entering %s\n",__FUNCTION__);

	LOG_DEBUG("Number of bp available brp_num_available= %d", hexagon->brp_num_available);
	LOG_DEBUG("Number of bp available brp_num= %d", hexagon->brp_num);
	LOG_DEBUG("Number of bp available brp_num_context= %d", hexagon->brp_num_context);
	//Hard coding brp_num_available = 2 for now, need to find a permanent solution. *hexagon = target_to_hexagon(target); should give valid data.
	hexagon->brp_num_available = 2; 			//this value is constant to future ref
	hexagon->brp_num = 2;						//upon setting/removing HW breakpoint this value will be decreased/increased 
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	//struct hexagon_private_config *pc;
	struct adiv5_dap *swddp = hexa_info->dap;
	int retval = ERROR_OK;
	uint32_t isdben,isdbver,isdbcstat,isdbst, corever, tmp;
	isdben = isdbver = isdbcstat = isdbst = corever = tmp = 0;

	// only required for ADSP debug
	if (current_hexagon_subsystem.hexagon_adsp == true)
	{
		LOG_INFO("ADSP clock enablement");
		hexagon_enable_clock(target);
	}
	/* Search for the APB-AB - it is needed to access debug registers */
	retval = dap_find_get_ap(swddp, AP_TYPE_APB_AP, &hexa_info->debug_ap);
	if (retval != ERROR_OK) {
		LOG_DEBUG("Could not find APB-AP for debug access");
		return retval;
	}

	retval = enable_dbg_sys_pwr(swddp);

	retval = mem_ap_init(hexa_info->debug_ap);
	if (retval != ERROR_OK) {
		LOG_DEBUG("Could not initialize the APB-AP");
		return retval;
	}

	hexa_info->debug_ap->memaccess_tck = 10;

	if (!target->dbgbase_set) {

		/* TDB */
		uint64_t dbgbase;
		/* Get ROM Table base */
		uint32_t apid;
		int32_t coreidx = target->coreid;
		
		retval = dap_get_debugbase(hexa_info->debug_ap, &dbgbase, &apid);
		if (retval != ERROR_OK)
			return retval;
		/* Lookup 0x15 -- Processor DAP */
		retval = dap_lookup_cs_component(hexa_info->debug_ap, 0x15,
				&hexa_info->debug_base, coreidx);
		if (retval != ERROR_OK)
			return retval;
		LOG_INFO("Detected core %x dbgbase: %llx apid: %x",\
					 coreidx, hexa_info->debug_base, apid);
	} else
		/** debug base = 0x0x86809000 **/
		hexa_info->debug_base = target->dbgbase;

	hexagon_wait_loop();

	/* Unlocking the APB address space by writing to debug base + 0xFB0 with  0xc5acce55 */
	retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,hexa_info->debug_base + 0xFB0, 0xc5acce55);
	if(retval != ERROR_OK)
		LOG_DEBUG("hexa_info->debug_base + 0xFB0	write failed"); 
	LOG_INFO("After writing 0x%llx	with value	0xC5ACCE55", hexa_info->debug_base+ 0xFB0); 

	/* Need to wait some time after writing to register to take it effect */
	hexagon_wait_loop(); 

	retval = hexagon_etm_on(target);
	if (retval != ERROR_OK)
		LOG_DEBUG("ETM enablement fail"); 
	

	retval = enable_dbg_sys_pwr(swddp);
	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
				hexa_info->debug_base + HEXAGON_ISDB_ISDBCST, &isdbcstat);
	if (retval != ERROR_OK) {
			LOG_DEBUG("ISDBCST read failed 0x%x", isdbcstat);
		return retval;
	}
	if (isdbcstat == 4)
	{
	LOG_ERROR("Q6 Processor is in reset or power collapse, please wait until the core is active");
	return ERROR_FAIL;
	}
	
	retval = hexagon_etm_on(target);
	if (retval != ERROR_OK)
		LOG_DEBUG("ETM enablement fail");

		/** check ISDB version details **/
	retval = enable_dbg_sys_pwr(swddp);
	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBVER, &isdbver);
	if (retval != ERROR_OK) {
		LOG_DEBUG("ISDBVER read failed 0x%x", isdbver);
		return retval;
	}
	LOG_DEBUG(" ISDBver 0x%x  ", isdbver);
		

		/** check core version details **/
	retval = enable_dbg_sys_pwr(swddp);
	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_COREVER, &corever);
	if (retval != ERROR_OK) {
		LOG_DEBUG("COREVER read failed 0x%x", corever);
		return retval;
	}
	LOG_DEBUG(" core ver 0x%x ", corever);



	retval = enable_dbg_sys_pwr(swddp);
	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBEN, &isdben);
	if (retval != ERROR_OK)
	{
		LOG_DEBUG("ISDBEN read failed 0x%x", isdben);
		return retval;
	}
	LOG_DEBUG("After ISDBEN read 0x%x ", isdben);


	/** check secure_EN, ISDB_trusted to enable APB_ISDB **/
	if (((isdben & ISDBEN_APB_ISDB_EN) != ISDBEN_APB_ISDB_EN) ||
		((isdben & ISDBEN_ISDB_PREVNT_PWRDWN) != ISDBEN_ISDB_PREVNT_PWRDWN))  
	{	 /** trusted, secure, clk on, apb enabl **/
		hexa_info->isdb_enable = true;
		isdben |= ISDBEN_APB_ISDB_EN;
		isdben |= ISDBEN_ISDB_PREVNT_PWRDWN;

		retval = enable_dbg_sys_pwr(swddp);
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
				hexa_info->debug_base + HEXAGON_ISDB_ISDBEN, isdben);
	if (retval != ERROR_OK)
	{
			LOG_DEBUG("ISDBEN write failed 0x%x", isdben);
			return retval;
		}
		LOG_DEBUG("After ISDBEN write  0x%x", isdben);
	}
	else 
	{
		LOG_DEBUG("ISDB is already enabled 0x%x", isdben);
	}


	hexagon_wait_loop();

	retval = enable_dbg_sys_pwr(swddp);
	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBEN, &isdben);
	if (retval != ERROR_OK)
	{
		LOG_DEBUG("ISDBEN read failed 0x%x", isdben);
		return retval;
	}
	LOG_DEBUG("After ISDBEN read 0x%x ", isdben);



	retval = enable_dbg_sys_pwr(swddp);

	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbst);
	if (retval != ERROR_OK)
	{
			LOG_DEBUG("ISDB status read failed 0x%x", isdbst);
			return retval;
	}
	LOG_DEBUG("After ISDB status  0x%x ", isdbst);
	

	

//#if 0

	
	//---------------Hexagon breakpoint global data structure setup---------------//
	hexagon->brp_num = 2;
	hexagon->brp_num_available = hexagon->brp_num;
	hexagon->brp_list = calloc(hexagon->brp_num, sizeof(struct hexagon_brp));
	for (int i = 0; i < hexagon->brp_num; i++) 
	{
		hexagon->brp_list[i].used = 0;
		hexagon->brp_list[i].type = BRP_NORMAL;
		hexagon->brp_list[i].value = 0;
		hexagon->brp_list[i].control = 0;
		hexagon->brp_list[i].BRPn = i;
	}

	LOG_DEBUG("Configured %i hw breakpoints", hexagon->brp_num);
		
	//-----------------------------------------------------------------------//

	//hexagon_setup_isdb_config(target);
	//hexagon breakpoint config setup	


	hexa_info->isdb_ver = isdbver;
	hexa_info->corever= corever;
	if((isdben == 0x0) && (isdbver == 0x0) &&  (corever == 0x0))
	{
		LOG_ERROR("Not able to communicate with device , Reboot the device and try again");
	}
	
	LOG_DEBUG("Finally: isdben 0x%x, isdbver 0x%x, corever 0x%x, isdbstatus 0x%x", isdben, isdbver, corever, isdbst);

	/* set up registers, breakpoint */
	retval = hexagon_reg_setup(hexagon);
	retval = hexagon_brkpt_setup(hexagon);

	target->state = TARGET_UNKNOWN;
	target->debug_reason = DBG_REASON_NOTHALTED;
	hexagon->isrmasking_mode = HEXAGON_ISRMASK_ON;
	target_set_examined(target);
	LOG_INFO("%s: examination pass\n", target_name(target));

	hexagon_hw_watchdog_disable(target);
	// LOG_INFO("checking STEP latency issues");
	//hexagon_populate_vtlb_data(target);
	return ERROR_OK;
}



static int hexagon_examine(struct target *target)
{
	int retval = ERROR_OK;
	

	LOG_DEBUG("hexagon_examine() ");
		
	/* don't re-probe hardware after each reset */
	if (!target_was_examined(target))
		retval = hexagon_examine_first(target);

	
	return retval;
}

static int hexagon_brkpt_setup(struct hexagon_common *hexagon)
{
	int retval = ERROR_OK;

	

	
	return retval;
}
static int hexagon_reg_setup(struct hexagon_common *hexagon)
{
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct hexa_reg *reg = &hexa_info->reg;
	struct target *target = hexa_info->target;
	struct reg_cache *cache;
	struct reg_cache *list = NULL;
	int retval = ERROR_OK;
	uint32_t i;

	

	if (hexa_info->core_cache != NULL)
	{
		LOG_DEBUG("registers are already built...so skipping");
		return ERROR_OK;
	}

	/* no of Hw threads reg + 1 global reg */
	for (i = 0; i <= gHexConfig.maxHwThreads; i++)
	{
		if ((cache = hexagon_build_reg_cache(target,i)) == NULL)
				return ERROR_FAIL;

		cache->name = gpHexagonThreadNameArray[i];
		LOG_DEBUG("hexagon thread name [i] = %s", gpHexagonThreadNameArray[i]);
		if (hexa_info->core_cache == NULL)
		{
			hexa_info->core_cache = cache;
		}
		else 
		{
			list->next = cache;
		}
		list = cache;
	}

	hexa_info->full_context = NULL;
	hexa_info->read_core_reg = hexagon_read_core_reg;
	hexa_info->write_core_reg = hexagon_write_core_reg;
	reg->hexagon_reg_current = hexagon_reg_current;


	
	return retval;
}


static int hexagon_get_core_reg(struct reg *reg)
{
	struct hex_reg *arch_info = reg->arch_info; 
	struct target *target = arch_info->target;
	struct hexa_info *hexa_info = arch_info->hexa_info;


	

	LOG_DEBUG("get_core_reg: target state %d, hw-thrd %d, reg %d", 
							target->state, arch_info->hwthrd, arch_info->num);
	
	if (target->state != TARGET_HALTED)
		return ERROR_TARGET_NOT_HALTED;


	

	return hexa_info->read_core_reg(target, reg, arch_info->num, arch_info->hwthrd);
}

static int hexagon_set_core_reg(struct reg *reg, uint8_t *buf)
{
	struct hex_reg *arch_info = reg->arch_info; 
	struct target *target = arch_info->target;
	struct hexa_info *hexa_info = arch_info->hexa_info;
	uint32_t value;
	

	memcpy(&value, buf, 4);
	
	// LOG_INFO("set_core_reg: target state %d, hw-thrd %d, reg %d, value 0x%x",
	// 					target->state, arch_info->hwthrd, arch_info->num, value);
	
	if (target->state != TARGET_HALTED)
		return ERROR_TARGET_NOT_HALTED;

			

	

	return hexa_info->write_core_reg(target, arch_info->num, arch_info->hwthrd, value);  
	
}


/** Builds cache of architecturally defined registers.  */
struct reg_cache *hexagon_build_reg_cache(struct target *target, uint32_t hwthrd)
{
	struct hexagon_common *hexa_common = target_to_hexagon(target);
	struct hexa_info *hexa = &hexa_common->hexa_info;
	int num_regs;
	struct reg_cache **cache_p = register_get_last_cache_p(&target->reg_cache);
	struct reg_cache *cache;
	struct reg *reg_list;
	struct hex_reg *arch_info;
	struct reg_feature *feature;
	int i;
	
    LOG_DEBUG("hexagon_build_reg_cache enter");
	/* Build the process context cache */
	cache = malloc(sizeof(struct reg_cache));

	if (hwthrd == gHexConfig.maxHwThreads+1)
	{
		/* Create for global regisers */
		num_regs = HEXAGON_MMODE_GLOBAL_MAX - HEXAGON_MMODE_PERTHRD_MAX;
		reg_list = calloc(num_regs, sizeof(struct reg));
		arch_info = calloc(num_regs, sizeof(struct hex_reg));
		cache->next = NULL;
		cache->reg_list = reg_list;
		cache->num_regs = num_regs;
	
		for (i = 0; i < num_regs; i++)
		{
			arch_info[i].num = hexagon_global_regs[i].id;
			arch_info[i].hwthrd = gHexConfig.maxHwThreads;
			arch_info[i].target = target;
			arch_info[i].hexa_info= hexa;
			arch_info[i].value[0] = 0xDE; //default value
			arch_info[i].value[1] = 0xAD; //default value

			reg_list[i].name = hexagon_global_regs[i].name;
			reg_list[i].size = hexagon_global_regs[i].bits;
			reg_list[i].value = &arch_info[i].value[0];
			reg_list[i].type = &hexagon_reg_type;
			reg_list[i].arch_info = &arch_info[i];
			reg_list[i].group = hexagon_global_regs[i].group;
			reg_list[i].number = i;
			reg_list[i].exist = true;
			reg_list[i].caller_save = true;	/* gdb defaults to true */
			//reg_list[i].valid = true;

			feature = calloc(1, sizeof(struct reg_feature));
			if (feature)
			{
				feature->name = hexagon_global_regs[i].feature;
				reg_list[i].feature = feature;
			} 
			else
			LOG_DEBUG("unable to allocate feature list");

			reg_list[i].reg_data_type = calloc(1, sizeof(struct reg_data_type));
			
			if (reg_list[i].reg_data_type)
			{
				reg_list[i].reg_data_type->type = hexagon_global_regs[i].type;
			}
			else

			LOG_DEBUG("unable to allocate reg type list");

		}

		LOG_DEBUG("hexagon_build_reg_cache global reg allocation done");
			
	}
	else
	{
		/** Create GPR, CTRL, Monitor Mode (per thread) registers */
		num_regs = HEXAGON_PER_THREAD_REGS;
		reg_list = calloc(num_regs, sizeof(struct reg));
		arch_info = calloc(num_regs, sizeof(struct hex_reg));
		cache->next = NULL;
		cache->reg_list = reg_list;
		cache->num_regs = num_regs;		
		for (i = 0; i < num_regs; i++) 
		{
			arch_info[i].num = hexagon_per_hwt_regs[i].id;
			arch_info[i].hwthrd = hwthrd;
			arch_info[i].target = target;
			arch_info[i].hexa_info= hexa;
			//arch_info[i].value[0] = 0xDEAD; //default values
			arch_info[i].value[0] = 0xDE; //default value
			arch_info[i].value[1] = 0xAD; //default value
			reg_list[i].name = hexagon_per_hwt_regs[i].name;
			reg_list[i].size = hexagon_per_hwt_regs[i].bits;
			reg_list[i].value = &arch_info[i].value[0];
			reg_list[i].type = &hexagon_reg_type;
			reg_list[i].arch_info = &arch_info[i];
			reg_list[i].group = hexagon_per_hwt_regs[i].group;
			reg_list[i].number = i;
			reg_list[i].exist = true;
			reg_list[i].caller_save = true;	/* gdb defaults to true */
			//reg_list[i].valid = true;

			feature = calloc(1, sizeof(struct reg_feature));
			if (feature)
			{
				feature->name = hexagon_per_hwt_regs[i].feature;
				reg_list[i].feature = feature;
			} 
			else
				LOG_DEBUG("unable to allocate feature list");

			reg_list[i].reg_data_type = calloc(1, sizeof(struct reg_data_type));
			
			if (reg_list[i].reg_data_type)
			{
				reg_list[i].reg_data_type->type = hexagon_per_hwt_regs[i].type;
			} else
				LOG_DEBUG("unable to allocate reg type list");
			
	  	   }
		
			LOG_DEBUG(" hexagon_build_reg_cache per thread allocation done");
	}

	LOG_DEBUG("hexagon_build_reg_cache exit");

	(*cache_p) = cache;
	return cache;
}

struct reg *hexagon_reg_current(struct hexa_info *hexa_info, unsigned int regnum,  struct reg_cache *cache)
{
	struct reg *r;

	


	if (regnum >= HEXAGON_MMODE_GLOBAL_MAX)
		return NULL;
	
	r = cache->reg_list + regnum;  //TDB for all Hw threads.



	
	return r;
}

static int hexagon_write_core_reg(struct target *target, int regnum, uint32_t hwthrd, uint32_t value)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct reg_cache *cache;
	int retval=ERROR_OK;
	uint64_t i;


	

	if (regnum < HEXAGON_R0 || regnum >= HEXAGON_MMODE_GLOBAL_MAX)
		return ERROR_COMMAND_SYNTAX_ERROR;

	#ifdef  _DEBUG_HEXAGON_
		LOG_DEBUG("hexagon_write_core_reg: %d, Thrd: %d, value %d", regnum, hwthrd, value);	
	#endif

	/* Get the register of the requested Hw thread */
	cache = hexa_info->core_cache;
	i= 0;
	while ((cache != NULL) && i < hwthrd)
	{
		cache = cache->next;
		i++;
	}

	/* update the register value locally */
	*((uint8_t*)cache->reg_list[regnum].value) = value;
	cache->reg_list[regnum].dirty = true;
	cache->reg_list[regnum].valid = true;

	#ifdef  _DEBUG_HEXAGON_
		LOG_DEBUG("Before writing:%d", regnum); 
	#endif
	
	/* Write the register value to device */
	if (regnum < HEXAGON_GPR_MAX)
	{
		hexagon_write_gpr_register(target, regnum, hwthrd, value);
	}
	else if (regnum < HEXAGON_CTRL_MAX)
	{
		hexagon_write_ctrl_register(target, regnum, hwthrd, value);
	}
	else if ((regnum < HEXAGON_MMODE_PERTHRD_MAX))
	{
		LOG_DEBUG("This reg:%d write is no supported", regnum);
	}
	else if (regnum < HEXAGON_MMODE_GLOBAL_MAX)
	{
		hexagon_write_global_ctrl_register(target, regnum, hwthrd, value);
	}
	else
	{
		LOG_DEBUG("Invalid register to write reg:%d", regnum);	
	}

	
	return retval; 
}
int hexagon_sync(struct target *target,uint32_t hwthrd)
{
    uint32_t isdb_mmode_cmd, isdbsts,retval;
    uint64_t stuff_inst[] = {0xa840c000,  0x56c0d000, 0x57c0c002 };
	//{0xa840c000 -->{syncht }, 0x56c0d000 -->{ickill}, 0x57c0c002 -->{isync} }
    struct hexagon_common *hexagon = target_to_hexagon(target);
    struct hexa_info *hexa_info = &hexagon->hexa_info;

	isdb_mmode_cmd = hexagon_pack_isdbcmd(ISDBCMD_CMD_STUFF, ISDBCMD_MONITOR_LVL,
                                                ISDBCMD_TNUM_MASK_THREAD(hwthrd));

	

    for (int i = 0; i < 2; i++) 
    {
        retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
                                         hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst[i]);
        if (retval != ERROR_OK)
            LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
        retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
                                         hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
        if (retval != ERROR_OK)
            LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
        retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
                                        hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
        if (retval != ERROR_OK)
            LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
        retval = isdbsts & ISDBST_ISDB_CMD_STATUS;
        if (retval!=ERROR_OK)
        {
            LOG_DEBUG("ISDBcommand failed in monitor mode");
            return ERROR_FAIL;
        }
    }


	
    return ERROR_OK;
}
static int hexagon_write_global_ctrl_register(struct target *target, int regnum, uint32_t hwthrd, uint32_t value)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct adiv5_dap *swddp = hexa_info->dap;

	//struct adiv5_dap *swddp = hexa_info->dap;
	int retval , i=0;
	uint32_t isdbsts;
	uint32_t isdb_mmode_cmd, isdb_cmd_status;


	

	#ifdef  _DEBUG_HEXAGON_
    LOG_DEBUG("hexagon_write_global_ctrl_register: hw thrd: %d, regnum %d, value %d", hwthrd, regnum, value);
	#endif

	if ((regnum == HEXAGON_MODECTL) || (regnum == HEXAGON_S19_RESRV) || (regnum == HEXAGON_IPENDAD) || 
		(regnum >= HEXAGON_S24_RESRV && regnum <= HEXAGON_CFGBASE) || ((regnum == HEXAGON_REV)))
	{
		LOG_DEBUG("Invalid: reserved/readonly register : %d", regnum);
		return ERROR_FAIL;
	}
	#ifdef  _HEXAGON_TARGET_TIME_PROFILING
		hexagon_start_time_cal_ms();
	#endif
	isdb_mmode_cmd = hexagon_pack_isdbcmd(ISDBCMD_CMD_STUFF, ISDBCMD_MONITOR_LVL,
											ISDBCMD_TNUM_MASK_THREAD(hwthrd));

	// /*
	retval = enable_dbg_sys_pwr(swddp);
	if (retval != ERROR_OK) {
		LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
	}
	// */

	/* run the 1st instruction */
	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
	if (retval != ERROR_OK) {
		LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
	}
	#ifdef  _DEBUG_HEXAGON_
	LOG_DEBUG("ISDB status before ISDBMBXIN write 0x%x", isdbsts);
	#endif
	

	retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBMBXIN, value);
	if (retval != ERROR_OK) {
		LOG_DEBUG("HEXAGON_ISDB_ISDBMBXIN_CDSP return value is not OK");
	}

	//  ensure that regnum-HEXAGON_SGP0 is not out of bounds before accessing it
	if ((regnum - HEXAGON_SGP0) >= 0 && (regnum - HEXAGON_SGP0) < 16) {
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap, hexa_info->debug_base + HEXAGON_ISDB_STFINST,
										stuff_inst_global_reg_write[regnum-HEXAGON_SGP0][0]);
		if (retval != ERROR_OK) {
			LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
		}
	} else {
		LOG_ERROR("Array index out of bounds: %d", regnum - HEXAGON_SGP0);
	}

	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
	if (retval != ERROR_OK) {
		LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
	}
	#ifdef  _DEBUG_HEXAGON_
	LOG_DEBUG("ISDB status after ISDBMBXIN write 0x%x", isdbsts);
	#endif

	retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
	if (retval != ERROR_OK) {
		LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
	}

	/* wait till the stuff instruction is executed */
	hexagon_wait_loop();

	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
	if (retval != ERROR_OK) {
		LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
	}
	#ifdef  _DEBUG_HEXAGON_
	LOG_DEBUG("ISDB status after ISDBMBXIN write 0x%x", isdbsts);
	#endif

	/* if isdb_cmd_status 0 in cmd was successfull in case of 1 failed */ 
	isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;

	if (isdb_cmd_status)
	{
		 LOG_DEBUG("ISDBcommand failed in monitor  mode");
		 return ERROR_OK;
	}
	else
	{	
		if (isdbsts & ISDBST_ISDB_MAILBOX_IN)
		{
			i = 0;
			while (isdbsts & ISDBST_ISDB_MAILBOX_IN)
			{

				retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
					hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);

				hexagon_wait_loop();
				#ifdef  _DEBUG_HEXAGON_
				LOG_DEBUG("ISDB status read for Mboxin %d time", i);
				#endif
				i++;
				if (i==10)
					break;
			}
			if(isdbsts & ISDBST_ISDB_MAILBOX_IN)
			{
				LOG_DEBUG("ISDBST status not cleared for ISDBMBXIN, so write failure for register R%d", regnum);
				return 	ERROR_FAIL;	
			}
		}
		/* run the 2nd instruction */
		if ((regnum - HEXAGON_SGP0) >= 0 && (regnum - HEXAGON_SGP0) < 16) {
			retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,hexa_info->debug_base + HEXAGON_ISDB_STFINST, 
									stuff_inst_global_reg_write[regnum - HEXAGON_SGP0][1]);
			if (retval != ERROR_OK) {
				LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
			}
		} else {
			LOG_ERROR("Array index out of bounds: %d", regnum - HEXAGON_SGP0);
		}

		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
				hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
		if (retval != ERROR_OK) {
			LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
		}

		/* wait till the stuff instruction is executed */
		hexagon_wait_loop();
	
		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
				hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
		if (retval != ERROR_OK) {
			LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
		}


		/* if isdb_cmd_status 0 in cmd was successfull in case of 1 failed */ 
		isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
		if (isdb_cmd_status)
		{
			 LOG_DEBUG("ISDBcommand failed in monitor  mode");
			 return ERROR_OK;
		}
	}
	global_reg[regnum-HEXAGON_EVB]  = value;
	#ifdef  _HEXAGON_TARGET_TIME_PROFILING
		hexagon_end_time_cal_ms();
		LOG_DEBUG("Total time taken  %" PRId64 "ms", hexagon_time_total);
	#endif


	
	return ERROR_OK;
}

static int hexagon_write_ctrl_register(struct target *target, int regnum, uint32_t hwthrd, uint32_t value)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct adiv5_dap *swddp = hexa_info->dap;
	int retval , i=0;
	uint32_t isdbsts;
	uint32_t isdb_mmode_cmd, isdb_cmd_status;

	#ifdef  _DEBUG_HEXAGON_
    LOG_DEBUG("hexagon_write_ctrl_register: hw thrd: %d, regnum %d, value 0x%x", hwthrd, regnum, value);
	#endif


	

	if ((regnum == HEXAGON_C5_RESRV) || (regnum >= HEXAGON_C20_RESRV && regnum <= HEXAGON_C20_RESRV))
	{
		LOG_DEBUG("Invalid: reserved register : %d", regnum);
		return ERROR_FAIL;
	}

	isdb_mmode_cmd = hexagon_pack_isdbcmd(ISDBCMD_CMD_STUFF, ISDBCMD_MONITOR_LVL,
					ISDBCMD_TNUM_MASK_THREAD(hwthrd));

	// /*
	retval = enable_dbg_sys_pwr(swddp);
	if (retval != ERROR_OK) {
		LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
	}
	// */

	/* run the 1st instruction */
	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
	if (retval != ERROR_OK) {
		LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
	}
	#ifdef  _DEBUG_HEXAGON_
		LOG_DEBUG("ISDB status before ISDBMBXIN write 0x%x", isdbsts);
	#endif
	

	retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBMBXIN, value);
	if (retval != ERROR_OK) {
		LOG_DEBUG("HEXAGON_ISDB_ISDBMBXIN return value is not OK");
	}

	retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,hexa_info->debug_base + HEXAGON_ISDB_STFINST, 
								stuff_inst_ctrl_reg_write[regnum-HEXAGON_SA0][0]);
	if (retval != ERROR_OK) {
		LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
	}


	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
	if (retval != ERROR_OK) {
		LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
	}
	#ifdef  _DEBUG_HEXAGON_
		LOG_DEBUG("ISDB status after ISDBMBXIN write 0x%x", isdbsts);
	#endif
       
	retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
	if (retval != ERROR_OK) {
		LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
	}

	/* wait till the stuff instruction is executed */
	hexagon_wait_loop();

	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
	if (retval != ERROR_OK) {
		LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
	}
	#ifdef  _DEBUG_HEXAGON_
		LOG_DEBUG("ISDB status after ISDBCMD run 0x%x", isdbsts);
	#endif


	/* if isdb_cmd_status 0 in cmd was successfull in case of 1 failed */ 
	isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;

	if (isdb_cmd_status)
	{
		 LOG_DEBUG("ISDBcommand failed in monitor  mode");
		 return ERROR_OK;
	}
	else
	{	
		if(isdbsts & ISDBST_ISDB_MAILBOX_IN)
		{
			i = 0;
			while(isdbsts & ISDBST_ISDB_MAILBOX_IN)
			{
				retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
					hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);

				hexagon_wait_loop();
				#ifdef  _DEBUG_HEXAGON_
					LOG_DEBUG("ISDB status read for Mboxin 0x%x time", isdbsts);
				#endif
					
				i++;
				if (i==10)
					break;
			}
			if(isdbsts & ISDBST_ISDB_MAILBOX_IN)
			{
				LOG_DEBUG("ISDBST status not cleared for ISDBMBXIN, write failure for register R%d", regnum);
				return 	ERROR_FAIL;
			}
		}

		/* run the 2nd instruction */
		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,hexa_info->debug_base + HEXAGON_ISDB_STFINST,
							stuff_inst_ctrl_reg_write[regnum-HEXAGON_SA0][1]);
		if (retval != ERROR_OK) {
			LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
		}

		retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
				hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
        if (retval != ERROR_OK)
        {
			LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
		}

		/* wait till the stuff instruction is executed */
		hexagon_wait_loop();
	
		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
				hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
        if (retval != ERROR_OK)
        {
			LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
		}
		/* if isdb_cmd_status 0 in cmd was successfull in case of 1 failed */ 
		isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;

		if (isdb_cmd_status)
		{
			 LOG_DEBUG("ISDBcommand failed in monitor  mode");
			 return ERROR_OK;
		}
	}
	gpPerHwThrdReg[hwthrd][regnum] = value;
	
	#ifdef  _HEXAGON_TARGET_TIME_PROFILING
		hexagon_end_time_cal_ms();
		LOG_DEBUG("Total time taken  %" PRId64 "ms", hexagon_time_total);
	#endif


	
	return ERROR_OK;
}

static int hexagon_write_gpr_register(struct target *target, int regnum, uint32_t hwthrd, uint32_t value)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct adiv5_dap *swddp = hexa_info->dap;
	int retval , i=0;
	uint32_t isdbsts;
    uint32_t isdb_mmode_cmd, isdb_cmd_status;

	#ifdef  _DEBUG_HEXAGON_
    	LOG_DEBUG("hexagon_write_gpr_register: hw thrd: %d, regnum %d, value 0x%x", hwthrd, regnum, value);
	#endif


	

    isdb_mmode_cmd = hexagon_pack_isdbcmd(ISDBCMD_CMD_STUFF, ISDBCMD_MONITOR_LVL,
											ISDBCMD_TNUM_MASK_THREAD(hwthrd));

	#ifdef  _HEXAGON_TARGET_TIME_PROFILING
		hexagon_start_time_cal_ms();
	#endif
	
	// /*
	retval = enable_dbg_sys_pwr(swddp);
	if (retval != ERROR_OK) {
		LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
	}
	// */

	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
	if (retval != ERROR_OK) {
		LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
	}
	#ifdef  _DEBUG_HEXAGON_
		LOG_DEBUG("ISDB status before ISDBMBXIN write 0x%x", isdbsts);
	#endif
	
	retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBMBXIN, value);
	if (retval != ERROR_OK) {
		LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
	}
	retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_STFINST, stuff_inst_gpr_write[regnum]);
    if (retval != ERROR_OK)
    {
		LOG_DEBUG("HEXAGON_ISDB_STFINST return value is not OK");
	}

	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
    if (retval != ERROR_OK)
    {
		LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
	}
	#ifdef  _DEBUG_HEXAGON_
		LOG_DEBUG("ISDB status after ISDBMBXIN write 0x%x", isdbsts);
	#endif

	retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
                                     hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
    if (retval != ERROR_OK)
    {
		LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
	}

	/* wait till the stuff instruction is executed */
	hexagon_wait_loop();

	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
			hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
    if (retval != ERROR_OK)
    {
		LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
	}
	#ifdef  _DEBUG_HEXAGON_
		LOG_DEBUG("ISDB status after ISDBCMD write 0x%x", isdbsts);
	#endif


	/* if isdb_cmd_status 0 in cmd was successfull in case of 1 failed */ 
	isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;

	if (isdb_cmd_status)
	{
		 LOG_DEBUG("ISDBcommand failed in user mode");
		 return ERROR_OK;
	}
	else if(isdbsts & ISDBST_ISDB_MAILBOX_IN)
	{
		i = 0;
		while (isdbsts & ISDBST_ISDB_MAILBOX_IN)
		{
			retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
				hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);

			hexagon_wait_loop();
			#ifdef  _DEBUG_HEXAGON_
				LOG_DEBUG("ISDB status read for Mboxin 0x%x time", isdbsts);
			#endif 
				
			i++;
			if (i==10)
				break;
		}
		if(isdbsts & ISDBST_ISDB_MAILBOX_IN)
		{
			LOG_DEBUG("ISDBST status not cleared for ISDBMBXIN, write failure for register R%d", regnum);
			return 	ERROR_FAIL;	
		}
	}
	gpPerHwThrdReg[hwthrd][regnum] = value;
	#ifdef  _HEXAGON_TARGET_TIME_PROFILING
		hexagon_end_time_cal_ms();
		LOG_DEBUG("Total time taken  %" PRId64 "ms", hexagon_time_total);
	#endif

	
	return ERROR_OK;
}

static int hexagon_read_core_reg(struct target *target, struct reg *r, int regnum, uint32_t hwthrd)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct reg_cache *cache;
	int retval=ERROR_OK;
	uint64_t i;

	

	// LOG_INFO("reading reg:%d, hw_thrd %d ", regnum, hwthrd);

	if (regnum < 0 || regnum >= HEXAGON_MMODE_GLOBAL_MAX)
		return ERROR_COMMAND_SYNTAX_ERROR;

	/* Read the register of the requested Hw thread */
	cache = hexa_info->core_cache;
	i= 0;
	while ((cache->next != NULL) && i < hwthrd)
	{
		cache = cache->next;
		i++;
	}
	r->value = (uint8_t*) cache->reg_list[regnum].value;
	r->valid = true;
	r->dirty = false;


	
	return retval;
}


static int hexagon_init_target(struct command_context *cmd_ctx,
	struct target *target)
{
	LOG_INFO(" hexagon_init_target");
	return ERROR_OK;
}
static void hexagon_deinit_target(struct target *target)
{

	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info hexa_info = hexagon->hexa_info;
	struct reg_cache *cache, *cache1;
	

	free(hexagon->brp_list);
	free(target->private_config);
	
 	cache = cache1= hexa_info.core_cache;
	while(cache != NULL)
	{
		free(cache->reg_list->arch_info);
		free(cache->reg_list);
		cache1 = cache->next;
		free(cache);
		cache = cache1;
	}
	free(hexa_info.core_cache);
	
	free(hexagon);
    deinitConfig(&gHexConfig);
	
	if(hexagon_vtlb_entries)
		free(hexagon_vtlb_entries);

	

}

static int hexagon_read_ISDB(struct target *target, uint32_t isdbsts, uint64_t stuffcmdStatusCheck)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	int retval , count=0;
	uint32_t isdbsts_new = isdbsts, isdb_cmd_status;

	while (isdbsts == isdbsts_new && count <= 1000)
	{	
		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
					hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts_new);

		if (retval != ERROR_OK) 
		{
			LOG_DEBUG("ISDBST read failed 0x%x", isdbsts_new);
			return retval;
		}
	}
	if (count>= 1000)
	{
		LOG_DEBUG("ISDBST COMMAND failed 0x%x", isdbsts_new);
		return ERROR_FAIL;
	}
	LOG_DEBUG("HEXAGON_ISDB_ISDBST value : 0x%x after ISDBSTUFF COMMAND : 0x%llx", isdbsts_new , stuffcmdStatusCheck);
	isdb_cmd_status = isdbsts_new & stuffcmdStatusCheck;

	if(isdb_cmd_status)
	{
		LOG_DEBUG("ISDBcommand  failed ");
	}
	/* if isdb_cmd_status 0 in cmd was successfull in case of 1 failed */ 
	LOG_DEBUG("HEXAGON_ISDB_ISDBCMD write successful ");

	return ERROR_OK;
}


static int hexagon_mmu(struct target *target, int *enabled)
{
	uint64_t syscfg,retval;

	retval = hexagon_read_syscfg_register(target);
	// hexagon_stuff_reg_restore_r7(target);
	hexagon_stuff_reg_restore(target);
	if(retval == ERROR_OK)
		syscfg= hexagon_syscfg_reg;
	else
		syscfg = global_reg[2];

	struct hexagon_mmu_common * mmu = (struct hexagon_mmu_common *)&(target_to_hexagon(target)->hexa_info.hexagon_mmu);
	
	if (target->state != TARGET_HALTED) {
		LOG_DEBUG("%s: target %s not halted", __func__, target_name(target));
		return ERROR_TARGET_INVALID;
	}
	LOG_DEBUG("SYSCFG register is  0x%llx", syscfg);
	if(syscfg & 1)
		mmu->mmu_enabled = 1;
	if(syscfg & (1<<1))
		mmu->instrution_cache_enabled = 1;
	if(syscfg & (1<<2))
		mmu->data_cache_enabled = 1;
	
	*enabled = target_to_hexagon(target)->hexa_info.hexagon_mmu.mmu_enabled;

	
	
	return ERROR_OK;
}

/* This function is used to restore r0, r1, r2 and r7 used during stuff instrcution*/
// static void hexagon_stuff_reg_restore(struct target *target)
static void hexagon_stuff_reg_restore(struct target *target)

{
	int retval;
	
	if(hexagon_r0_used_stuff)
	{
		retval = hexagon_write_gpr_register(target, HEXAGON_R0, HEXAGON_HW_THREAD0, 
											gpPerHwThrdReg[HEXAGON_HW_THREAD0][HEXAGON_R0]);
		if(retval == ERROR_OK)
			hexagon_r0_used_stuff = 0;
		else
		{
			retval = hexagon_write_gpr_register(target, HEXAGON_R0, HEXAGON_HW_THREAD0, 
											gpPerHwThrdReg[HEXAGON_HW_THREAD0][HEXAGON_R0]);
			if(retval == ERROR_OK)
				hexagon_r0_used_stuff = 0;
			else 
				LOG_DEBUG("Error occured during restoring R0 value used for stuff ");
		}
	}
	if(hexagon_r1_used_stuff)
	{
		retval = hexagon_write_gpr_register(target, HEXAGON_R1, HEXAGON_HW_THREAD0, 
										gpPerHwThrdReg[HEXAGON_HW_THREAD0][HEXAGON_R1]);
		if(retval == ERROR_OK)
			hexagon_r1_used_stuff = 0;
		else
		{
			retval = hexagon_write_gpr_register(target, HEXAGON_R1, HEXAGON_HW_THREAD0, 
										gpPerHwThrdReg[HEXAGON_HW_THREAD0][HEXAGON_R1]);
			if(retval == ERROR_OK)
				hexagon_r1_used_stuff = 0;
			else
				LOG_DEBUG("Error occured during restoring R1 value used for stuff ");
		}
	}
	if(hexagon_r2_used_stuff)
	{
		retval = hexagon_write_gpr_register(target, HEXAGON_R2, HEXAGON_HW_THREAD0, 
										gpPerHwThrdReg[HEXAGON_HW_THREAD0][HEXAGON_R2]);
		if(retval == ERROR_OK)
			hexagon_r2_used_stuff = 0;
		else
		{
			retval = hexagon_write_gpr_register(target, HEXAGON_R2, HEXAGON_HW_THREAD0, 
										gpPerHwThrdReg[HEXAGON_HW_THREAD0][HEXAGON_R2]);
			if(retval == ERROR_OK)
				hexagon_r2_used_stuff = 0;
			else
				LOG_DEBUG("Error occured during restoring R2 value used for stuff ");
		}
	}
	if(hexagon_r7_used_stuff)
	{
		retval= hexagon_write_gpr_register(target, HEXAGON_R7, HEXAGON_HW_THREAD0, 
										gpPerHwThrdReg[HEXAGON_HW_THREAD0][HEXAGON_R7]);
		if(retval == ERROR_OK)
			hexagon_r7_used_stuff = 0;
		else
		{
			retval= hexagon_write_gpr_register(target, HEXAGON_R7, HEXAGON_HW_THREAD0, 
										gpPerHwThrdReg[HEXAGON_HW_THREAD0][HEXAGON_R7]);
			if(retval == ERROR_OK)
				hexagon_r7_used_stuff = 0;
			else
				LOG_DEBUG("Error occured during restoring R7 value used for stuff ");
		}
	}
	if(hexagon_r30_used_stuff)
	{
		retval= hexagon_write_gpr_register(target, HEXAGON_FP, HEXAGON_HW_THREAD0, 
										gpPerHwThrdReg[HEXAGON_HW_THREAD0][HEXAGON_FP]);
		if(retval == ERROR_OK)
			hexagon_r30_used_stuff = 0;
		else
		{
			retval= hexagon_write_gpr_register(target, HEXAGON_FP, HEXAGON_HW_THREAD0, 
										gpPerHwThrdReg[HEXAGON_HW_THREAD0][HEXAGON_FP]);
			if(retval == ERROR_OK)
				hexagon_r30_used_stuff = 0;
			else
				LOG_DEBUG("Error occured during restoring R7 value used for stuff ");
		}
	}
	if(hexagon_r31_used_stuff)
	{
		retval= hexagon_write_gpr_register(target, HEXAGON_LR, HEXAGON_HW_THREAD0, 
										gpPerHwThrdReg[HEXAGON_HW_THREAD0][HEXAGON_LR]);
		if(retval == ERROR_OK)
			hexagon_r31_used_stuff = 0;
		else
		{
			retval= hexagon_write_gpr_register(target, HEXAGON_LR, HEXAGON_HW_THREAD0, 
										gpPerHwThrdReg[HEXAGON_HW_THREAD0][HEXAGON_LR]);
			if(retval == ERROR_OK)
				hexagon_r31_used_stuff = 0;
			else
				LOG_DEBUG("Error occured during restoring R7 value used for stuff ");
		}
	}
	
}
/*================================================================*/
/*hexagon-untrusted code changes*/
/*================================================================*/
int hexagon_untrusted_update_packet(char *packet, int len)
{
	int retval;
	if (packet == NULL)
		return ERROR_FAIL;
	// Adding initial $ to each RSP packet
	hexagon_untrusted_current_state.current_packet[0] = '$';
	for (int i = 1; i <= len; i++)
	{
		hexagon_untrusted_current_state.current_packet[i] = packet[i - 1];
	}
	hexagon_untrusted_current_state.packet_len = len + 1;
	// Updating the end of the string in the RSP packet
	hexagon_untrusted_current_state.current_packet[len + 1] = '\0';
	hexagon_untrusted_current_state.has_packet = true;
	if (hexagon_untrusted_current_state.has_checksum)
	{
		retval = hexagon_untrusted_concat_rsp();
		if (retval != ERROR_OK)
			return retval;
	}
	// LOG_INFO("Hexagon untrusted RSP packet : %s\n",hexagon_untrusted_current_state.current_packet);
	return ERROR_OK;
}


/* function hexagon_untrusted_update_packet_checksum
 * args:
 * checksum : it contains the checksum recieved on GDB connection
 *			 for the RSP packet.
 */
int hexagon_untrusted_update_packet_checksum(char *checksum)
{
	int retval = 0;
	// LOG_INFO("Hexagon untrusted RSP packet checksum entered\n");

	hexagon_untrusted_current_state.current_checksum[0] = checksum[0];
	hexagon_untrusted_current_state.current_checksum[1] = checksum[1];
	hexagon_untrusted_current_state.has_checksum = true;
	if (hexagon_untrusted_current_state.has_packet)
	{
		retval = hexagon_untrusted_concat_rsp();
	}
	// LOG_INFO("Hexagon untrusted RSP packet checksum  exit: %c%c\n",checksum[0],checksum[1]);
	if (retval != ERROR_OK)
		LOG_DEBUG("rsp concatentation failed");
	return ERROR_OK;
}
/* function hexagon_untrusted_concat_rsp
 * This function will concatinate the packet and Checksum
 *	packet after this function will look like in this format
 *	$<packet>#<checksum>
 */
int hexagon_untrusted_concat_rsp()
{
	// LOG_DEBUG("Hexagon untrusted concatinated the RSP entered\n");

	if (!(hexagon_untrusted_current_state.has_checksum &&
		  hexagon_untrusted_current_state.has_packet))
	{
		LOG_DEBUG("Hexagon untrusted RSP packet or checksum not available\n");
		return ERROR_FAIL;
	}
	int len = hexagon_untrusted_current_state.packet_len;
	hexagon_untrusted_current_state.current_packet[len] = '#';

	hexagon_untrusted_current_state.current_packet[len + 1] =
		hexagon_untrusted_current_state.current_checksum[0];
	hexagon_untrusted_current_state.current_packet[len + 2] =
		hexagon_untrusted_current_state.current_checksum[1];
	hexagon_untrusted_current_state.current_packet[len + 3] = '\0';

	hexagon_untrusted_current_state.packet_len = len + 3;
	hexagon_untrusted_current_state.has_packet = false;
	hexagon_untrusted_current_state.has_checksum = false;
	hexagon_untrusted_current_state.has_rsp = true;
	// LOG_DEBUG("Hexagon untrusted concatinated the RSP : %s\n",
	//						hexagon_untrusted_current_state.current_packet);

	return ERROR_OK;
}

/* function hexagon_untrusted_write_to_mailboxin
 * writes the 32bit value to the mailboxin register
 * args:
 * target : target variable for accessing the connected target
 * val : value which need to written on the register
 */
int hexagon_untrusted_write_to_mailboxin(struct target *target, uint32_t value)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	int retval;
	uint32_t isdbst_after = 0x0;
	uint32_t isdbsts = 0x0;

	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
									hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);

	// retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
	// 								hexa_info->debug_base + HEXAGON_ISDB_ISDBMBXIN, &isdbmbx);

	// LOG_INFO("ISDBMBX before write to mailbox : 0x%x", isdbmbx);
	LOG_DEBUG("ISDBSTS before write to mailbox : 0x%x", isdbsts);

	retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
									 hexa_info->debug_base + HEXAGON_ISDB_ISDBMBXIN, value);


	if (retval != ERROR_OK)
	{
		LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
		return retval;
	}


	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
									hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbst_after);


	// LOG_INFO("ISDBMBX after write to mailbox : 0x%x", isdbmbx);
	LOG_DEBUG("ISDBSTS after write to mailbox : 0x%x", isdbst_after);

	if (retval != ERROR_OK)
	{
		LOG_DEBUG("ISDBST read failed 0x%x", isdbst_after);
		return retval;
	}
	retval = send_isdb_interrupt(target);
	if (retval != ERROR_OK)
	{
		LOG_DEBUG("Unable to Interrupt!\n");
		return retval;
	}
	return ERROR_OK;
}

/*function hexagon_untrusted_read_to_mailboxout
 * this function will write the 32bit value to the ISDBMAILBOXOUT register
 * args
 * target : target variable to access the connected target
 * value :  value to be written to the register
 */
int hexagon_untrusted_read_to_mailboxout(struct target *target, uint32_t *value)
{

	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	int retval;
	uint32_t isdbsts;

	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
									hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);

	LOG_DEBUG("ISDBSTS before read from mailboxout : 0x%x", isdbsts);

	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
									hexa_info->debug_base + HEXAGON_ISDB_ISDBMBXOUT, value);
	

	if (retval != ERROR_OK)
	{
		LOG_DEBUG("HEXAGON_ISDB_ISDBMBXOUT read failed 0x%x", *value);
		return retval;
	}
	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
									hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);

	

	if (retval != ERROR_OK)
	{
		LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);
		return retval;
	}
	retval = send_isdb_interrupt(target);
	if (retval != ERROR_OK)
	{
		LOG_DEBUG("Unable to Interrupt!\n");
		return retval;
	}
	return ERROR_OK;
}



/*function hexagon_untrusted_convert_essential_header
 * convert the essential header information to the 32bit integer
 * args
 * header: essential header structure containing the info
 */

static uint32_t hexagon_untrusted_convert_essential_header(untrusted_essential header)
{
    uint32_t value = 0x0;
    value |= header.payload_len;
    value = (value << 8) | header.header_len;
    value = (value << 8) | header.protocol;

    return value;
}

/*function hexagon_untrusted_convert_to_essential_header
 * convert 32bit integer to the essential header information
 * args
 * val : 32bit value to convert
 * header: essential header structure pointer to store result
 */
static int hexagon_untrusted_convert_to_essential_header(uint32_t val, untrusted_essential *header)
{
	// LOG_DEBUG("value before conversion is %u\n",val);
	header->protocol = (uint8_t)(val & 0xff);
	val >>= 8;
	header->header_len = (uint8_t)(val & 0xff);
	val >>= 8;
	header->payload_len = (uint16_t)(val & 0xffff);
	return ERROR_OK;
}
/* function
 */
int hexagon_untrusted_poll_isdbst_set_bit(struct target *target, uint8_t bit)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	uint32_t isdbsts, check_bit = 0x1;
	int retval;
	while (check_bit > 0)
	{
		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
										hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);

		

		if (retval != ERROR_OK)
		{
			LOG_DEBUG("Unable to read ISDBST register.\n");
			return retval;
		}
		check_bit = isdbsts & bit;
	}
	return ERROR_OK;
}

/* function
 */
int hexagon_untrusted_poll_isdbst_unset_bit(struct target *target, uint8_t bit)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	uint32_t isdbsts, check_bit = 0x0;
	int retval;
	while (check_bit == 0)
	{
		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
										hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);
		

		if (retval != ERROR_OK)
		{
			LOG_ERROR("Unable to read ISDBST register.\n");
			return retval;
		}
		check_bit = isdbsts & bit;
	}
	return ERROR_OK;
}
/* function

*/
uint64_t min_u(uint64_t a, uint64_t b)
{
	if (a > b)
		return b;
	return a;
}
/*function
 */
int send_isdb_interrupt(struct target *target)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	struct adiv5_dap *swddp = hexa_info->dap;
	uint32_t isdb_mmode_cmd = ISDBCMD_CMD_INTRPT | ISDBCMD_MONITOR_LVL;
	uint32_t isdb_cmd_status, isdbsts;
	int retval;
	retval = enable_dbg_sys_pwr(swddp);
	if (retval != ERROR_OK)
		LOG_DEBUG("enable_dbg_sys_pwr return value is not OK");
	retval = mem_ap_write_atomic_u32(hexa_info->debug_ap,
									 hexa_info->debug_base + HEXAGON_ISDB_ISDBCMD, isdb_mmode_cmd);
	

	if (retval != ERROR_OK)
		LOG_DEBUG("HEXAGON_ISDB_ISDBCMD return value is not OK");
	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
									hexa_info->debug_base + HEXAGON_ISDB_ISDBST, &isdbsts);

	

	if (retval != ERROR_OK)
		LOG_DEBUG("ISDBST read failed 0x%x", isdbsts);

	/* if isdb_cmd_status 0 in cmd was successfull in case of 1 failed */
	isdb_cmd_status = isdbsts & ISDBST_ISDB_CMD_STATUS;
	if (isdb_cmd_status)
	{
		LOG_DEBUG("ISDBcommand failed to interrupt");
		return ERROR_FAIL;
	}
	LOG_DEBUG("Raised Interrupt successfully!\n");
	return ERROR_OK;
}

/*function
 */
int hexagon_untrusted_listen_for_rsp(struct target *target, char *s, uint16_t *res_len)
{
	int retval, i = 0;
	uint32_t read_val;
	uint64_t calculated_checksum = 0;
    uint32_t mbxOutVal;
	uint32_t isdbsts = 0;
	untrusted_essential *header;
    struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;


	// fix for checksum mismatch
# if 1
	/* Read the essential header */
	LOG_DEBUG("Reading the essential header...\n");

	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
									hexa_info->debug_base + HEXAGON_ISDB_ISDBST,  &isdbsts);
	LOG_DEBUG("ISDBST before poll 0x%x", isdbsts);

	retval = hexagon_untrusted_poll_isdbst_unset_bit(target, ISDBST_ISDB_MAILBOX_OUT);
	LOG_DEBUG("ISDBST after poll 0x%x", isdbsts);
#endif


	/* Read the essential header */
	LOG_DEBUG("Reading the essential header...\n");
	// retval = hexagon_untrusted_poll_isdbst_unset_bit(target, ISDBST_ISDB_MAILBOX_OUT);
	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
									hexa_info->debug_base + ISDBST_ISDB_MAILBOX_OUT, &mbxOutVal);
	

	LOG_DEBUG("Mailbox out value is :0x%x\n",mbxOutVal);

	retval = hexagon_untrusted_read_to_mailboxout(target, &read_val);
	calculated_checksum += (read_val & 0xff) + ((read_val >> 8) & 0xff) + ((read_val >> 16) & 0xff) + ((read_val >> 24) & 0xff);
	
	retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
									hexa_info->debug_base + ISDBST_ISDB_MAILBOX_OUT, &mbxOutVal);
	

	LOG_DEBUG("Mailbox out value is :0x%x\n", mbxOutVal);
	LOG_DEBUG("Successfully read the essential header!\n");
	if (retval != ERROR_OK)
	{
		LOG_DEBUG("Error in reading the ISDBMailboxout register\n");
		return retval;
	}
	header = (untrusted_essential *)malloc(sizeof(untrusted_essential));

	retval = hexagon_untrusted_convert_to_essential_header(read_val, header);
	
	/* Read process ID */
	if (header->header_len >= 8)
	{
		retval = hexagon_untrusted_poll_isdbst_unset_bit(target, ISDBST_ISDB_MAILBOX_OUT);
		retval = hexagon_untrusted_read_to_mailboxout(target, &read_val);
		calculated_checksum += (read_val & 0xff) + ((read_val >> 8) & 0xff) + ((read_val >> 16) & 0xff) + ((read_val >> 24) & 0xff);
		LOG_DEBUG("Successfully read the process id header!\n");
		if (retval != ERROR_OK)
		{
			LOG_DEBUG("Error in reading the ISDBMailboxout register\n");
			return retval;
		}
		// if(Q6_global_settings.selected_process != read_val){
		// 	LOG_DEBUG("Process id is not same!\n");
		// 	// return ERROR_FAIL;
		// }
	}

	uint16_t len = (header->payload_len / ISDBMAILBOX_SIZE);
	if ((header->payload_len % ISDBMAILBOX_SIZE) != 0)
	{
		len += 1;
	}
	char *rsp_packet;
	uint16_t current = 0;
	rsp_packet = (char *)malloc(sizeof(char) * header->payload_len);

	for (i = 0; i < len; i++)
	{

		retval = hexagon_untrusted_poll_isdbst_unset_bit(target, ISDBST_ISDB_MAILBOX_OUT);
		retval = hexagon_untrusted_read_to_mailboxout(target, &read_val);
		calculated_checksum += (read_val & 0xff) + ((read_val >> 8) & 0xff) + ((read_val >> 16) & 0xff) + ((read_val >> 24) & 0xff);
		for (int j = 0; j < 4; j++)
		{
			char c = (char)(read_val & 0xff);
			rsp_packet[current] = c;
			read_val >>= 8;
			current++;
		}
	}

	calculated_checksum = calculated_checksum % 256;
	retval = hexagon_untrusted_poll_isdbst_unset_bit(target, ISDBST_ISDB_MAILBOX_OUT);
	retval = hexagon_untrusted_read_to_mailboxout(target, &read_val);
	if (read_val != calculated_checksum)
	{
		LOG_DEBUG("Error in reading. Checksum don't match!!! read_val : 0x%x calculated_checksum : 0x%llx \n",read_val,calculated_checksum );
		
	}

	*res_len = header->payload_len;
	memcpy(s, rsp_packet, *res_len);

	free(rsp_packet);
	return ERROR_OK;
}

static int hexagon_untrusted_send_rsp(struct target *target, char *rsppkt, uint16_t len, char *response, uint16_t *response_len, int header_len)
{
	untrusted_payload *payload;
	untrusted_essential *essential_header;
	uint16_t number_of_payload_packets = len / ISDBMAILBOX_SIZE;
	uint32_t pid_s = 0;
	uint32_t rsp_v;
	int retval;
	uint32_t write_value;
	uint32_t buffer, stride;
	uint32_t cha;
	uint16_t i = 0;
	int p = 0;
	uint32_t calculated_checksum = 0;

	if (len % ISDBMAILBOX_SIZE != 0)
	{
		number_of_payload_packets += 1;
	}

	essential_header = (untrusted_essential *)malloc(sizeof(untrusted_essential));
	if (essential_header == NULL)
	{
		LOG_DEBUG("Insufficient memory\n");
		return ERROR_FAIL;
	}

	payload = (untrusted_payload *)malloc(sizeof(untrusted_payload));
	if (payload == NULL)
	{
		LOG_DEBUG("Insufficient memory\n");
		return ERROR_FAIL;
	}

	LOG_DEBUG("Packet to be converted is %s",rsppkt);

	// Initializing the payload data structure
	payload->len = number_of_payload_packets;
	payload->bytes_to_read = len;
	payload->payload = (uint32_t *)malloc(sizeof(uint32_t) * number_of_payload_packets);
	
	while (i < payload->bytes_to_read)
	{
		stride = min_u(ISDBMAILBOX_SIZE, payload->bytes_to_read - i);
		buffer = 0x0;
		for (uint32_t j = 0; j < stride; j++)
		{
			cha = (uint32_t)rsppkt[i + j];
			buffer |= (cha << (8 * j));
		}
		i += ISDBMAILBOX_SIZE;
		payload->payload[p++] = buffer;
	}
	essential_header->protocol = UNTRUSTED_PROTOCOL_VERSION;
	if (header_len == -1)
	{
		essential_header->header_len = 12;
		pid_s = 0xFFFFFFFF;
		rsp_v = RSPVERSION;
	}
	else
	{
		essential_header->header_len = 8;
		pid_s = Q6_global_settings.selected_process;
		// LOG_DEBUG("PID set to : %lld",pid_s);
	}


	essential_header->payload_len = payload->bytes_to_read;
	/*Sending the essential header to the ISDBMBXIN*/
	// waiting if already content there
	// retval=hexagon_untrusted_poll_isdbst_set_bit(target,0x2);
	write_value = hexagon_untrusted_convert_essential_header(*(essential_header));
	retval = hexagon_untrusted_poll_isdbst_set_bit(target, ISDBST_ISDB_MAILBOX_IN);

	// write crash
	LOG_DEBUG("writing to isdb value 0x%x",write_value);
	retval = hexagon_untrusted_write_to_mailboxin(target, write_value);
	
	

	calculated_checksum += (write_value & 0xff) + ((write_value >> 8) & 0xff) + ((write_value >> 16) & 0xff) + ((write_value >> 24) & 0xff);
	if (retval != ERROR_OK)
	{
		LOG_DEBUG("Unable to send packet on the ISDBMBXIN channel\n");
		return retval;
	}

	retval = hexagon_untrusted_poll_isdbst_set_bit(target, ISDBST_ISDB_MAILBOX_IN);
	// write crash
	LOG_DEBUG("writing to isdb value 0x%x",pid_s);
	retval = hexagon_untrusted_write_to_mailboxin(target, pid_s);
	
	

	calculated_checksum += (pid_s & 0xff) + ((pid_s >> 8) & 0xff) + ((pid_s >> 16) & 0xff) + ((pid_s >> 24) & 0xff);
	if (retval != ERROR_OK)
	{
		LOG_DEBUG("Unable to send packet on the ISDBMBXIN channel\n");
		return retval;
	}

	if (essential_header->header_len == 12)
	{
		retval = hexagon_untrusted_poll_isdbst_set_bit(target, ISDBST_ISDB_MAILBOX_IN);

		// write crash
		LOG_DEBUG("writing to isdb value 0x%x",rsp_v);
		retval = hexagon_untrusted_write_to_mailboxin(target, rsp_v);
		
		

		calculated_checksum += (rsp_v & 0xff) + ((rsp_v >> 8) & 0xff) + ((rsp_v >> 16) & 0xff) + ((rsp_v >> 24) & 0xff);
		if (retval != ERROR_OK)
		{
			LOG_DEBUG("Unable to send packet on the ISDBMBXIN channel\n");
			return retval;
		}
	}

	
	/*Sending the RSP packet as the payload*/
	for (i = 0; i < payload->len; i++)
	{
		write_value = payload->payload[i];


		retval = hexagon_untrusted_poll_isdbst_set_bit(target, ISDBST_ISDB_MAILBOX_IN);

		// write crash
		LOG_DEBUG("writing to isdb value 0x%x",write_value);
		retval = hexagon_untrusted_write_to_mailboxin(target, write_value);
		
		

		calculated_checksum += (write_value & 0xff) + ((write_value >> 8) & 0xff) + ((write_value >> 16) & 0xff) + ((write_value >> 24) & 0xff);
		// // 
		if (retval != ERROR_OK)
		{
			LOG_DEBUG("Unable to send packet on the ISDBMBXIN channel\n");
			return retval;
		}
	}

	/* Sending checksum value */
	calculated_checksum = calculated_checksum % 256;
	retval = hexagon_untrusted_poll_isdbst_set_bit(target, ISDBST_ISDB_MAILBOX_IN);
	// write crash

	LOG_DEBUG("writing to isdb value 0x%x",calculated_checksum);
	retval = hexagon_untrusted_write_to_mailboxin(target, calculated_checksum);
	
	

	if (retval != ERROR_OK)
	{
		LOG_DEBUG("Unable to send checksum packet on the ISDBMBXIN channel\n");
		return retval;
	}

	free(payload->payload);
	free(payload);
	free(essential_header);

	
	

	LOG_DEBUG("Listening for the RSP...\n");
	retval = hexagon_untrusted_listen_for_rsp(target, response, response_len);

	return ERROR_OK;
}

static int convert_to_int(char *s)
{
	int len = strlen(s);
	int index = -1;
	for (int i = 0; i < len; i++)
	{
		if (s[i] != '0')
		{
			index = i;
			break;
		}
	}
	if (index == -1)
		return 0;
	return strtoul(s + index, NULL, 16);
}
static int load_process_list(char *process_list_response)
{
	char *ret;
	char *pid_index;
	char *comma_index;
	char *scolon_index;
	char p[16];
	// int spid;
	Q6_global_settings.selected_process = -1;
	int r=0x0;
	char *token = strtok(process_list_response, ";");
	while (token != NULL)
	{
		ret = strstr(token, "pid");
		if (ret)
		{
			pid_index = strchr(token, ':');
			comma_index = strchr(token, ',');
			r = strlen(pid_index) - 1 - strlen(comma_index);
			memcpy(p, pid_index + 1, r);
			p[r] = '\0';
			process_list[Q6_global_settings.total_process].pid = convert_to_int(p);
			scolon_index = strchr(comma_index, ':');
			r = strlen(scolon_index) - 1;
			memcpy(process_list[Q6_global_settings.total_process].process_name, scolon_index + 1, r);
			process_list[Q6_global_settings.total_process].process_name[r] = '\0';
			Q6_global_settings.total_process++;
		}
		token = strtok(NULL, ";");
	}
	printf("====================Process Selection Menu====================\n");
	printf("\tProcess Id\tProcess Name\n");


	for (int i = 0; i < Q6_global_settings.total_process; i++)
	{
		printf("\t%u\t\t%s\n", process_list[i].pid, process_list[i].process_name);
	}
	for (int i = 0; i < Q6_global_settings.total_process; i++) 
	{

		if (strcmp(process_list[i].process_name, "_ASID0_") != 0) 
		{
			Q6_global_settings.selected_process = process_list[i].pid;
			LOG_INFO("Selected process to debug is: %s pid : %d \n", process_list[i].process_name,process_list[i].pid);
			break;
		}
	}
	
	if (Q6_global_settings.selected_process == -1)
	{
		printf("\nError: No process is selected for debugging!\n");
		return ERROR_FAIL;
	}

	return ERROR_OK;
}
/* function

*/
int hexagon_untrusted_forward_rsp(struct target *target, char *response, uint16_t *response_len)
{
	int retval;
	uint16_t len;

	LOG_DEBUG("untrusted rsp packet forwarding");
	if (!hexagon_untrusted_current_state.has_rsp)
	{
		LOG_DEBUG("RSP packet is not available to forward\n");
		return ERROR_FAIL;
	}
	if (strncmp(hexagon_untrusted_current_state.current_packet, "$QStartNoAckMode", 16) == 0)
	{
		char s[256];
		uint16_t res_len = 0x0;
		
		retval = hexagon_untrusted_send_rsp(target, "$qProcessList#ec", 17, s, &res_len, -1);
		if (retval != ERROR_OK)
		{
			LOG_DEBUG("Not able to send initial process packet!\n");
			return retval;
		}

		LOG_DEBUG("Response recieved from qprocesslist is:  %s and the length is:  %u\n", s, res_len);
		retval = load_process_list(s);
		for (int i = 0; i < Q6_global_settings.total_process; i++)
		{
			if (Q6_global_settings.hexagon_debug_process_id == process_list[i].pid)
			{
				LOG_DEBUG("Process ID to debug is : %d\n", process_list[i].pid);
				break;
			}
		}
	}
	len = hexagon_untrusted_current_state.packet_len;
	// (char *) and char (*)[1024] point to the start address of the array
	// Cast the address of the array to char * to match the function's expected parameter type
	retval = hexagon_untrusted_send_rsp(target, (char *)&(hexagon_untrusted_current_state.current_packet), len, response, response_len, 1);

	hexagon_untrusted_current_state.has_rsp = false;
	hexagon_untrusted_current_state.has_packet = false;
	hexagon_untrusted_current_state.has_checksum = false;
	return ERROR_OK;
}
int hexagon_untrusted_mode()
{
	is_hexagon_untrusted = true;
	spid = 2;
	LOG_DEBUG("Mode changed to hexagon untrusted!\n");
	return ERROR_OK;
}

int hexagon_poll_ISDB(struct target *target, uint64_t isdbsts_old, uint64_t stuffcmdStatusCheck)
{
	struct hexagon_common *hexagon = target_to_hexagon(target);
	struct hexa_info *hexa_info = &hexagon->hexa_info;
	int retval , count=0;
	uint64_t isdbsts_new = isdbsts_old, isdb_cmd_status;

	isdb_cmd_status = isdbsts_old & stuffcmdStatusCheck;
	goto bit_check;
	
	//  loop while isdb status is not changing
	while (isdbsts_old == isdbsts_new && count <= 1000)
	{	
		retval = mem_ap_read_atomic_u32(hexa_info->debug_ap,
					hexa_info->debug_base + HEXAGON_ISDB_ISDBST, (uint32_t *) &isdbsts_new);

		hexagon_wait_loop();
		if (retval != ERROR_OK) 
		{
			return retval;
		}
		count++;
	}
	//  timed out waiting for isdb status to change

	if (count>= 1000)
	{
		LOG_DEBUG("ISDBST COMMAND failed 0x%llx", isdbsts_new);
		return ERROR_FAIL;
	}
	// LOG_DEBUG("HEXAGON_ISDB_ISDBST value : 0x%x after ISDBSTUFF COMMAND : 0x%x", isdbsts_new , stuffcmdStatusCheck);
	// isdb_cmd_status = isdbsts_new & ISDBST_ISDB_CMD_STATUS_ADSP;
	isdb_cmd_status = isdbsts_new & stuffcmdStatusCheck;
bit_check :

	//  isdbstatus command bit if not set means success
	if (stuffcmdStatusCheck == ISDBST_ISDB_CMD_STATUS && isdb_cmd_status == 0)
	{
		LOG_DEBUG("ISDBST_ISDB_CMD_STATUS success, previous isdbsts :  0x%llx current isdbsts : 0x%llx",isdbsts_old,isdbsts_new );

		return ERROR_OK;
	}
	if (stuffcmdStatusCheck == ISDBST_STUFF_CMD_STATUS && isdb_cmd_status == 0)
	{
		LOG_DEBUG("ISDBST_STUFF_CMD_STATUS success, previous isdbsts :  0x%llx current isdbsts : 0x%llx",isdbsts_old, isdbsts_new );

		return ERROR_OK;
	}
	//  isdbstatus command bit if set means success for mailbox
	if (stuffcmdStatusCheck == ISDBST_ISDB_MAILBOX_OUT && isdb_cmd_status == 1)
	{
		LOG_DEBUG("ISDBST_ISDB_MAILBOX_OUT is full, previous isdbsts :  0x%llx current isdbsts : 0x%llx",isdbsts_old,isdbsts_new );
		return ERROR_OK;
	}
	else
	{
		return ERROR_FAIL;
	}
}

struct target_type hexagon_target = {
	.name = "hexagon",
	.poll = hexagon_poll,
	.arch_state = hexagon_arch_state,
	.target_request_data  = NULL,
	.resume = hexagon_resume,
	.step = hexagon_step,
	.halt = hexagon_halt,
	.assert_reset = NULL,
	.deassert_reset = NULL,
	.soft_reset_halt = NULL,
	.get_gdb_arch = hexagon_get_gdb_arch,
	.get_gdb_reg_list = hexagon_get_gdb_reg_list,
	.get_gdb_reg_list_noread = hexagon_get_gdb_reg_list,
	.read_memory = hexagon_read_memory,
	.write_memory = hexagon_write_memory,
	.read_buffer = hexagon_read_buffer,
	.write_buffer = hexagon_write_buffer,
	.checksum_memory = NULL,
	.blank_check_memory = NULL,
	.add_breakpoint = hexagon_add_breakpoint,
	.add_context_breakpoint = NULL,
	.add_hybrid_breakpoint = NULL,
	.remove_breakpoint = hexagon_remove_breakpoint,
	.add_watchpoint = NULL,
	.remove_watchpoint = NULL,
	.hit_watchpoint = NULL,
	.commands = hexagon_command_handlers,
	.target_create = hexagon_target_create,
	.target_jim_configure = hexagon_jim_configure,
	.target_jim_commands = NULL,
	.examine = hexagon_examine,
	.init_target = hexagon_init_target,
	.deinit_target = hexagon_deinit_target,
	.virt2phys = hexagon_virt2phys,
	.mmu = hexagon_mmu,
	.check_reset = NULL,
	.get_gdb_fileio_info = NULL,
	.gdb_fileio_end = NULL,
	.profiling = NULL,
	.address_bits = NULL

};
