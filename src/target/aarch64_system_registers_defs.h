/**************************************************************************
*   Copyright (c) 2023 Qualcomm Innovation Center, Inc.                   *
*   All rights reserved.                                                  * 
*   SPDX-License-Identifier: GPL-2.0-or-later                             * 
*								          *
***************************************************************************/

static const struct aarch64_system_register aarch64_system_registers[] = {
         {
                 	"OSDTRRX_EL1",
                 	"OS Lock Data Transfer Register, Receive",
                 	"10", "000", "0000", "0000", "010",
                 	0x8002,
                 	{
                    		{"None", 31, 0},
                 	}
           },
           {
                   "MDCCINT_EL1",
                   "Monitor DCC Interrupt Enable Register",
                   "10", "000", "0000", "0010", "000",
                   0x8010,
                   {
                           {"RX", 30, 30},
                           {"TX", 29, 29},
                   }
           },
           {
                   "MDSCR_EL1",
                   "Monitor Debug System Control Register",
                   "10", "000", "0000", "0010", "010",
                   0x8012,
                   {
                           {"TFO", 31, 31},
                           {"RXfull", 30, 30},
                           {"TXfull", 29, 29},
                           {"RXO", 27, 27},
                           {"TXU", 26, 26},
                           {"INTdis", 23, 22},
                           {"TDA", 21, 21},
                           {"SC2", 19, 19},
                           {"None", 18, 16},
                           {"MDE", 15, 15},
                           {"HDE", 14, 14},
                           {"KDE", 13, 13},
                           {"TDCC", 12, 12},
                           {"ERR", 6, 6},
                           {"SS", 0, 0},
                   }
           },
           {
                   "OSDTRTX_EL1",
                   "OS Lock Data Transfer Register, Transmit",
                   "10", "000", "0000", "0011", "010",
                   0x801a,
                   {
                           {"None", 31, 0},
                   }
           },
           {
                   "OSECCR_EL1",
                   "OS Lock Exception Catch Control Register",
                   "10", "000", "0000", "0110", "010",
                   0x8032,
                   {
                           {"EDECCR", 31, 0},
                   }
           },
           {
                   "DBGBVR0_EL1",
                   "Debug Breakpoint Value Registers",
                   "10", "000", "0000", "0000", "100",
                   0x8004,
                   {
                           {"RESS[14:4]", 63, 53},
                           {"VA[52:49]", 52, 49},
                           {"RESS[3:0]", 52, 49},
                           {"VA[48:2]", 48, 2},
                   }
           },
           {
                   "DBGBVR1_EL1",
                   "Debug Breakpoint Value Registers",
                   "10", "000", "0000", "0001", "100",
                   0x800c,
                   {
                           {"RESS[14:4]", 63, 53},
                           {"VA[52:49]", 52, 49},
                           {"RESS[3:0]", 52, 49},
                           {"VA[48:2]", 48, 2},
                   }
           },
           {
                   "DBGBVR2_EL1",
                   "Debug Breakpoint Value Registers",
                   "10", "000", "0000", "0010", "100",
                   0x8014,
                   {
                           {"RESS[14:4]", 63, 53},
                           {"VA[52:49]", 52, 49},
                           {"RESS[3:0]", 52, 49},
                           {"VA[48:2]", 48, 2},
                   }
           },
           {
                   "DBGBVR3_EL1",
                  "Debug Breakpoint Value Registers",
                  "10", "000", "0000", "0011", "100",
                  0x801c,
                  {
                          {"RESS[14:4]", 63, 53},
                          {"VA[52:49]", 52, 49},
                          {"RESS[3:0]", 52, 49},
                          {"VA[48:2]", 48, 2},
                  }
          },
          {
                  "DBGBVR4_EL1",
                  "Debug Breakpoint Value Registers",
                  "10", "000", "0000", "0100", "100",
                  0x8024,
                  {
                          {"RESS[14:4]", 63, 53},
                          {"VA[52:49]", 52, 49},
                          {"RESS[3:0]", 52, 49},
                          {"VA[48:2]", 48, 2},
                  }
          },
          {
                  "DBGBVR5_EL1",
                  "Debug Breakpoint Value Registers",
                  "10", "000", "0000", "0101", "100",
                  0x802c,
                  {
                          {"RESS[14:4]", 63, 53},
                          {"VA[52:49]", 52, 49},
                          {"RESS[3:0]", 52, 49},
                          {"VA[48:2]", 48, 2},
                  }
          },
          {
                  "DBGBVR6_EL1",
                 "Debug Breakpoint Value Registers",
                 "10", "000", "0000", "0110", "100",
                 0x8034,
                 {
                         {"RESS[14:4]", 63, 53},
                         {"VA[52:49]", 52, 49},
                         {"RESS[3:0]", 52, 49},
                         {"VA[48:2]", 48, 2},
                 }
         },
         {
                 "DBGBVR7_EL1",
                 "Debug Breakpoint Value Registers",
                 "10", "000", "0000", "0111", "100",
                 0x803c,
                 {
                         {"RESS[14:4]", 63, 53},
                         {"VA[52:49]", 52, 49},
                         {"RESS[3:0]", 52, 49},
                         {"VA[48:2]", 48, 2},
                 }
         },
         {
                 "DBGBVR8_EL1",
                 "Debug Breakpoint Value Registers",
                 "10", "000", "0000", "1000", "100",
                 0x8044,
                 {
                         {"RESS[14:4]", 63, 53},
                         {"VA[52:49]", 52, 49},
                         {"RESS[3:0]", 52, 49},
                         {"VA[48:2]", 48, 2},
                 }
         },
         {
                 "DBGBVR9_EL1",
                 "Debug Breakpoint Value Registers",
                 "10", "000", "0000", "1001", "100",
                 0x804c,
                 {
                         {"RESS[14:4]", 63, 53},
                         {"VA[52:49]", 52, 49},
                         {"RESS[3:0]", 52, 49},
                         {"VA[48:2]", 48, 2},
                 }
         },
         {
                 "DBGBVR10_EL1",
                 "Debug Breakpoint Value Registers",
                 "10", "000", "0000", "1010", "100",
                 0x8054,
                 {
                         {"RESS[14:4]", 63, 53},
                         {"VA[52:49]", 52, 49},
                         {"RESS[3:0]", 52, 49},
                         {"VA[48:2]", 48, 2},
                 }
         },
         {
                 "DBGBVR11_EL1",
                 "Debug Breakpoint Value Registers",
                 "10", "000", "0000", "1011", "100",
                 0x805c,
                 {
                         {"RESS[14:4]", 63, 53},
                         {"VA[52:49]", 52, 49},
                         {"RESS[3:0]", 52, 49},
                         {"VA[48:2]", 48, 2},
                 }
         },
         {
                 "DBGBVR12_EL1",
                 "Debug Breakpoint Value Registers",
                 "10", "000", "0000", "1100", "100",
                 0x8064,
                 {
                         {"RESS[14:4]", 63, 53},
                         {"VA[52:49]", 52, 49},
                         {"RESS[3:0]", 52, 49},
                         {"VA[48:2]", 48, 2},
                 }
         },
         {
                 "DBGBVR13_EL1",
                 "Debug Breakpoint Value Registers",
                 "10", "000", "0000", "1101", "100",
                 0x806c,
                 {
                         {"RESS[14:4]", 63, 53},
                         {"VA[52:49]", 52, 49},
                         {"RESS[3:0]", 52, 49},
                         {"VA[48:2]", 48, 2},
                 }
         },
         {
                 "DBGBVR14_EL1",
                 "Debug Breakpoint Value Registers",
                 "10", "000", "0000", "1110", "100",
                 0x8074,
                 {
                         {"RESS[14:4]", 63, 53},
                         {"VA[52:49]", 52, 49},
                         {"RESS[3:0]", 52, 49},
                         {"VA[48:2]", 48, 2},
                 }
         },
         {
                 "DBGBVR15_EL1",
                 "Debug Breakpoint Value Registers",
                 "10", "000", "0000", "1111", "100",
                 0x807c,
                 {
                         {"RESS[14:4]", 63, 53},
                         {"VA[52:49]", 52, 49},
                         {"RESS[3:0]", 52, 49},
                         {"VA[48:2]", 48, 2},
                 }
         },
         {
                 "DBGBCR0_EL1",
                 "Debug Breakpoint Control Registers",
                 "10", "000", "0000", "0000", "101",
                 0x8005,
                 {
                         {"SSCE", 29, 29},
                         {"BT", 23, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 8, 5},
                         {"PMC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGBCR1_EL1",
                 "Debug Breakpoint Control Registers",
                 "10", "000", "0000", "0001", "101",
                 0x800d,
                 {
                         {"SSCE", 29, 29},
                         {"BT", 23, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 8, 5},
                         {"PMC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGBCR2_EL1",
                 "Debug Breakpoint Control Registers",
                 "10", "000", "0000", "0010", "101",
                 0x8015,
                 {
                         {"SSCE", 29, 29},
                         {"BT", 23, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 8, 5},
                         {"PMC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGBCR3_EL1",
                 "Debug Breakpoint Control Registers",
                 "10", "000", "0000", "0011", "101",
                 0x801d,
                 {
                         {"SSCE", 29, 29},
                         {"BT", 23, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 8, 5},
                         {"PMC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGBCR4_EL1",
                 "Debug Breakpoint Control Registers",
                 "10", "000", "0000", "0100", "101",
                 0x8025,
                 {
                         {"SSCE", 29, 29},
                         {"BT", 23, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 8, 5},
                         {"PMC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGBCR5_EL1",
                 "Debug Breakpoint Control Registers",
                 "10", "000", "0000", "0101", "101",
                 0x802d,
                 {
                         {"SSCE", 29, 29},
                         {"BT", 23, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 8, 5},
                         {"PMC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGBCR6_EL1",
                 "Debug Breakpoint Control Registers",
                 "10", "000", "0000", "0110", "101",
                 0x8035,
                 {
                         {"SSCE", 29, 29},
                         {"BT", 23, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 8, 5},
                         {"PMC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGBCR7_EL1",
                 "Debug Breakpoint Control Registers",
                 "10", "000", "0000", "0111", "101",
                 0x803d,
                 {
                         {"SSCE", 29, 29},
                         {"BT", 23, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 8, 5},
                         {"PMC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGBCR8_EL1",
                 "Debug Breakpoint Control Registers",
                 "10", "000", "0000", "1000", "101",
                 0x8045,
                 {
                         {"SSCE", 29, 29},
                         {"BT", 23, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 8, 5},
                         {"PMC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGBCR9_EL1",
                 "Debug Breakpoint Control Registers",
                 "10", "000", "0000", "1001", "101",
                 0x804d,
                 {
                         {"SSCE", 29, 29},
                         {"BT", 23, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 8, 5},
                         {"PMC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGBCR10_EL1",
                 "Debug Breakpoint Control Registers",
                 "10", "000", "0000", "1010", "101",
                 0x8055,
                 {
                         {"SSCE", 29, 29},
                         {"BT", 23, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 8, 5},
                         {"PMC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGBCR11_EL1",
                 "Debug Breakpoint Control Registers",
                 "10", "000", "0000", "1011", "101",
                 0x805d,
                 {
                         {"SSCE", 29, 29},
                         {"BT", 23, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 8, 5},
                         {"PMC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGBCR12_EL1",
                 "Debug Breakpoint Control Registers",
                 "10", "000", "0000", "1100", "101",
                 0x8065,
                 {
                         {"SSCE", 29, 29},
                         {"BT", 23, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 8, 5},
                         {"PMC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGBCR13_EL1",
                 "Debug Breakpoint Control Registers",
                 "10", "000", "0000", "1101", "101",
                 0x806d,
                 {
                         {"SSCE", 29, 29},
                         {"BT", 23, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 8, 5},
                         {"PMC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGBCR14_EL1",
                 "Debug Breakpoint Control Registers",
                 "10", "000", "0000", "1110", "101",
                 0x8075,
                 {
                         {"SSCE", 29, 29},
                         {"BT", 23, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 8, 5},
                         {"PMC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGBCR15_EL1",
                 "Debug Breakpoint Control Registers",
                 "10", "000", "0000", "1111", "101",
                 0x807d,
                 {
                         {"SSCE", 29, 29},
                         {"BT", 23, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 8, 5},
                         {"PMC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGWVR0_EL1",
                 "Debug Watchpoint Value Registers",
                 "10", "000", "0000", "0000", "110",
                 0x8006,
                 {
                         {"RESS[14:4]", 63, 53},
                         {"VA[52:49]", 52, 49},
                         {"RESS[3:0]", 52, 49},
                         {"VA[48:2]", 48, 2},
                 }
         },
         {
                 "DBGWVR1_EL1",
                 "Debug Watchpoint Value Registers",
                 "10", "000", "0000", "0001", "110",
                 0x800e,
                 {
                         {"RESS[14:4]", 63, 53},
                         {"VA[52:49]", 52, 49},
                         {"RESS[3:0]", 52, 49},
                         {"VA[48:2]", 48, 2},
                 }
         },
         {
                 "DBGWVR2_EL1",
                 "Debug Watchpoint Value Registers",
                 "10", "000", "0000", "0010", "110",
                 0x8016,
                 {
                         {"RESS[14:4]", 63, 53},
                         {"VA[52:49]", 52, 49},
                         {"RESS[3:0]", 52, 49},
                         {"VA[48:2]", 48, 2},
                 }
         },
         {
                 "DBGWVR3_EL1",
                 "Debug Watchpoint Value Registers",
                 "10", "000", "0000", "0011", "110",
                 0x801e,
                 {
                         {"RESS[14:4]", 63, 53},
                         {"VA[52:49]", 52, 49},
                         {"RESS[3:0]", 52, 49},
                         {"VA[48:2]", 48, 2},
                 }
         },
         {
                 "DBGWVR4_EL1",
                 "Debug Watchpoint Value Registers",
                 "10", "000", "0000", "0100", "110",
                 0x8026,
                 {
                         {"RESS[14:4]", 63, 53},
                         {"VA[52:49]", 52, 49},
                         {"RESS[3:0]", 52, 49},
                         {"VA[48:2]", 48, 2},
                 }
         },
         {
                 "DBGWVR5_EL1",
                 "Debug Watchpoint Value Registers",
                 "10", "000", "0000", "0101", "110",
                 0x802e,
                 {
                         {"RESS[14:4]", 63, 53},
                         {"VA[52:49]", 52, 49},
                         {"RESS[3:0]", 52, 49},
                         {"VA[48:2]", 48, 2},
                 }
         },
         {
                 "DBGWVR6_EL1",
                 "Debug Watchpoint Value Registers",
                 "10", "000", "0000", "0110", "110",
                 0x8036,
                 {
                         {"RESS[14:4]", 63, 53},
                         {"VA[52:49]", 52, 49},
                         {"RESS[3:0]", 52, 49},
                         {"VA[48:2]", 48, 2},
                 }
         },
         {
                 "DBGWVR7_EL1",
                 "Debug Watchpoint Value Registers",
                 "10", "000", "0000", "0111", "110",
                 0x803e,
                 {
                         {"RESS[14:4]", 63, 53},
                         {"VA[52:49]", 52, 49},
                         {"RESS[3:0]", 52, 49},
                         {"VA[48:2]", 48, 2},
                 }
         },
         {
                 "DBGWVR8_EL1",
                 "Debug Watchpoint Value Registers",
                 "10", "000", "0000", "1000", "110",
                 0x8046,
                 {
                         {"RESS[14:4]", 63, 53},
                         {"VA[52:49]", 52, 49},
                         {"RESS[3:0]", 52, 49},
                         {"VA[48:2]", 48, 2},
                 }
         },
         {
                 "DBGWVR9_EL1",
                 "Debug Watchpoint Value Registers",
                 "10", "000", "0000", "1001", "110",
                 0x804e,
                 {
                         {"RESS[14:4]", 63, 53},
                         {"VA[52:49]", 52, 49},
                         {"RESS[3:0]", 52, 49},
                         {"VA[48:2]", 48, 2},
                 }
         },
         {
                 "DBGWVR10_EL1",
                 "Debug Watchpoint Value Registers",
                 "10", "000", "0000", "1010", "110",
                 0x8056,
                 {
                         {"RESS[14:4]", 63, 53},
                         {"VA[52:49]", 52, 49},
                         {"RESS[3:0]", 52, 49},
                         {"VA[48:2]", 48, 2},
                 }
         },
         {
                 "DBGWVR11_EL1",
                 "Debug Watchpoint Value Registers",
                 "10", "000", "0000", "1011", "110",
                 0x805e,
                 {
                         {"RESS[14:4]", 63, 53},
                         {"VA[52:49]", 52, 49},
                         {"RESS[3:0]", 52, 49},
                         {"VA[48:2]", 48, 2},
                 }
         },
         {
                 "DBGWVR12_EL1",
                 "Debug Watchpoint Value Registers",
                 "10", "000", "0000", "1100", "110",
                 0x8066,
                 {
                         {"RESS[14:4]", 63, 53},
                         {"VA[52:49]", 52, 49},
                         {"RESS[3:0]", 52, 49},
                         {"VA[48:2]", 48, 2},
                 }
         },
         {
                 "DBGWVR13_EL1",
                 "Debug Watchpoint Value Registers",
                 "10", "000", "0000", "1101", "110",
                 0x806e,
                 {
                         {"RESS[14:4]", 63, 53},
                         {"VA[52:49]", 52, 49},
                         {"RESS[3:0]", 52, 49},
                         {"VA[48:2]", 48, 2},
                 }
         },
         {
                 "DBGWVR14_EL1",
                 "Debug Watchpoint Value Registers",
                 "10", "000", "0000", "1110", "110",
                 0x8076,
                 {
                         {"RESS[14:4]", 63, 53},
                         {"VA[52:49]", 52, 49},
                         {"RESS[3:0]", 52, 49},
                         {"VA[48:2]", 48, 2},
                 }
         },
         {
                 "DBGWVR15_EL1",
                 "Debug Watchpoint Value Registers",
                 "10", "000", "0000", "1111", "110",
                 0x807e,
                 {
                         {"RESS[14:4]", 63, 53},
                         {"VA[52:49]", 52, 49},
                         {"RESS[3:0]", 52, 49},
                         {"VA[48:2]", 48, 2},
                 }
         },
         {
                 "DBGWCR0_EL1",
                 "Debug Watchpoint Control Registers",
                 "10", "000", "0000", "0000", "111",
                 0x8007,
                 {
                         {"SSCE", 29, 29},
                         {"MASK", 28, 24},
                         {"WT", 20, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 12, 5},
                         {"LSC", 4, 3},
                         {"PAC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGWCR1_EL1",
                 "Debug Watchpoint Control Registers",
                 "10", "000", "0000", "0001", "111",
                 0x800f,
                 {
                         {"SSCE", 29, 29},
                         {"MASK", 28, 24},
                         {"WT", 20, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 12, 5},
                         {"LSC", 4, 3},
                         {"PAC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGWCR2_EL1",
                 "Debug Watchpoint Control Registers",
                 "10", "000", "0000", "0010", "111",
                 0x8017,
                 {
                         {"SSCE", 29, 29},
                         {"MASK", 28, 24},
                         {"WT", 20, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 12, 5},
                         {"LSC", 4, 3},
                         {"PAC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGWCR3_EL1",
                 "Debug Watchpoint Control Registers",
                 "10", "000", "0000", "0011", "111",
                 0x801f,
                 {
                         {"SSCE", 29, 29},
                         {"MASK", 28, 24},
                         {"WT", 20, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 12, 5},
                         {"LSC", 4, 3},
                         {"PAC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGWCR4_EL1",
                 "Debug Watchpoint Control Registers",
                 "10", "000", "0000", "0100", "111",
                 0x8027,
                 {
                         {"SSCE", 29, 29},
                         {"MASK", 28, 24},
                         {"WT", 20, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 12, 5},
                         {"LSC", 4, 3},
                         {"PAC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGWCR5_EL1",
                 "Debug Watchpoint Control Registers",
                 "10", "000", "0000", "0101", "111",
                 0x802f,
                 {
                         {"SSCE", 29, 29},
                         {"MASK", 28, 24},
                         {"WT", 20, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 12, 5},
                         {"LSC", 4, 3},
                         {"PAC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGWCR6_EL1",
                 "Debug Watchpoint Control Registers",
                 "10", "000", "0000", "0110", "111",
                 0x8037,
                 {
                         {"SSCE", 29, 29},
                         {"MASK", 28, 24},
                         {"WT", 20, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 12, 5},
                         {"LSC", 4, 3},
                         {"PAC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGWCR7_EL1",
                 "Debug Watchpoint Control Registers",
                 "10", "000", "0000", "0111", "111",
                 0x803f,
                 {
                         {"SSCE", 29, 29},
                         {"MASK", 28, 24},
                         {"WT", 20, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 12, 5},
                         {"LSC", 4, 3},
                         {"PAC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGWCR8_EL1",
                 "Debug Watchpoint Control Registers",
                 "10", "000", "0000", "1000", "111",
                 0x8047,
                 {
                         {"SSCE", 29, 29},
                         {"MASK", 28, 24},
                         {"WT", 20, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 12, 5},
                         {"LSC", 4, 3},
                         {"PAC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGWCR9_EL1",
                 "Debug Watchpoint Control Registers",
                 "10", "000", "0000", "1001", "111",
                 0x804f,
                 {
                         {"SSCE", 29, 29},
                         {"MASK", 28, 24},
                         {"WT", 20, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 12, 5},
                         {"LSC", 4, 3},
                         {"PAC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGWCR10_EL1",
                 "Debug Watchpoint Control Registers",
                 "10", "000", "0000", "1010", "111",
                 0x8057,
                 {
                         {"SSCE", 29, 29},
                         {"MASK", 28, 24},
                         {"WT", 20, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 12, 5},
                         {"LSC", 4, 3},
                         {"PAC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGWCR11_EL1",
                 "Debug Watchpoint Control Registers",
                 "10", "000", "0000", "1011", "111",
                 0x805f,
                 {
                         {"SSCE", 29, 29},
                         {"MASK", 28, 24},
                         {"WT", 20, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 12, 5},
                         {"LSC", 4, 3},
                         {"PAC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGWCR12_EL1",
                 "Debug Watchpoint Control Registers",
                 "10", "000", "0000", "1100", "111",
                 0x8067,
                 {
                         {"SSCE", 29, 29},
                         {"MASK", 28, 24},
                         {"WT", 20, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 12, 5},
                         {"LSC", 4, 3},
                         {"PAC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGWCR13_EL1",
                 "Debug Watchpoint Control Registers",
                 "10", "000", "0000", "1101", "111",
                 0x806f,
                 {
                         {"SSCE", 29, 29},
                         {"MASK", 28, 24},
                         {"WT", 20, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 12, 5},
                         {"LSC", 4, 3},
                         {"PAC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGWCR14_EL1",
                 "Debug Watchpoint Control Registers",
                 "10", "000", "0000", "1110", "111",
                 0x8077,
                 {
                         {"SSCE", 29, 29},
                         {"MASK", 28, 24},
                         {"WT", 20, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 12, 5},
                         {"LSC", 4, 3},
                         {"PAC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "DBGWCR15_EL1",
                 "Debug Watchpoint Control Registers",
                 "10", "000", "0000", "1111", "111",
                 0x807f,
                 {
                         {"SSCE", 29, 29},
                         {"MASK", 28, 24},
                         {"WT", 20, 20},
                         {"LBN", 19, 16},
                         {"SSC", 15, 14},
                         {"HMC", 13, 13},
                         {"BAS", 12, 5},
                         {"LSC", 4, 3},
                         {"PAC", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "MDRAR_EL1",
                 "Monitor Debug ROM Address Register",
                 "10", "000", "0001", "0000", "000",
                 0x8080,
                 {
                         {"ROMADDR", 51, 12},
                         {"Valid", 1, 0},
                 }
         },
         {
                 "OSLAR_EL1",
                 "OS Lock Access Register",
                 "10", "000", "0001", "0000", "100",
                 0x8084,
                 {
                         {"OSLK", 0, 0},
                 }
         },
         {
                 "OSLSR_EL1",
                 "OS Lock Status Register",
                 "10", "000", "0001", "0001", "100",
                 0x808c,
                 {
                         {"OSLM", 3, 3},
                         {"nTT", 2, 2},
                         {"OSLK", 1, 1},
                         {"OSLM[0]", 0, 0},
                 }
         },
         {
                 "OSDLR_EL1",
                 "OS Double Lock Register",
                 "10", "000", "0001", "0011", "100",
                 0x809c,
                 {
                         {"DLK", 0, 0},
                         {"None", 0, 0},
                 }
         },
         {
                 "DBGPRCR_EL1",
                 "Debug Power Control Register",
                 "10", "000", "0001", "0100", "100",
                 0x80a4,
                 {
                         {"CORENPDRQ", 0, 0},
                         {"CORENPDRQ", 0, 0},
                 }
         },
         {
                 "DBGCLAIMSET_EL1",
                 "Debug CLAIM Tag Set register",
                 "10", "000", "0111", "1000", "110",
                 0x83c6,
                 {
                         {"None", 31, 8},
                         {"CLAIM", 7, 0},
                 }
         },
         {
                 "DBGCLAIMCLR_EL1",
                 "Debug CLAIM Tag Clear register",
                 "10", "000", "0111", "1001", "110",
                 0x83ce,
                 {
                         {"None", 31, 8},
                         {"CLAIM", 7, 0},
                 }
         },
         {
                 "DBGAUTHSTATUS_EL1",
                 "Debug Authentication Status register",
                 "10", "000", "0111", "1110", "110",
                 0x83f6,
                 {
                         {"RTNID", 27, 26},
                         {"RTID", 25, 24},
                         {"RLNID", 15, 14},
                         {"RLID", 13, 12},
                         {"SNID", 7, 6},
                         {"SNID", 7, 6},
                         {"SID", 5, 4},
                         {"NSNID", 3, 2},
                         {"NSNID", 3, 2},
                         {"NSID", 1, 0},
                 }
         },
         {
                 "TRCTRACEIDR",
                 "Trace ID Register",
                 "10", "001", "0000", "0000", "001",
                 0x8801,
                 {
                         {"TRACEID", 6, 0},
                 }
         },
         {
                 "TRCVICTLR",
                 "ViewInst Main Control Register",
                 "10", "001", "0000", "0000", "010",
                 0x8802,
                 {
                         {"EXLEVEL_RL_EL2", 26, 26},
                         {"EXLEVEL_RL_EL1", 25, 25},
                         {"EXLEVEL_RL_EL0", 24, 24},
                         {"EXLEVEL_NS_EL2", 22, 22},
                         {"EXLEVEL_NS_EL1", 21, 21},
                         {"EXLEVEL_NS_EL0", 20, 20},
                         {"EXLEVEL_S_EL3", 19, 19},
                         {"EXLEVEL_S_EL2", 18, 18},
                         {"EXLEVEL_S_EL1", 17, 17},
                         {"EXLEVEL_S_EL0", 16, 16},
                         {"TRCERR", 11, 11},
                         {"TRCRESET", 10, 10},
                         {"SSSTATUS", 9, 9},
                         {"EVENT_TYPE", 7, 7},
                         {"EVENT_SEL", 4, 0},
                         {"Reserved", 4, 0},
                 }
         },
         {
                 "TRCIDR8",
                 "ID Register 8",
                 "10", "001", "0000", "0000", "110",
                 0x8806,
                 {
                         {"MAXSPEC", 31, 0},
                 }
         },
         {
                 "TRCIMSPEC0",
                 "IMP DEF Register 0",
                 "10", "001", "0000", "0000", "111",
                 0x8807,
                 {
                         {"EN", 7, 4},
                         {"SUPPORT", 3, 0},
                 }
         },
         {
                 "TRCPRGCTLR",
                 "Programming Control Register",
                 "10", "001", "0000", "0001", "000",
                 0x8808,
                 {
                         {"EN", 0, 0},
                 }
         },
         {
                 "TRCQCTLR",
                 "Q Element Control Register",
                 "10", "001", "0000", "0001", "001",
                 0x8809,
                 {
                         {"MODE", 8, 8},
                         {"RANGE[<m>]", 7, 0},
                 }
         },
         {
                 "TRCVIIECTLR",
                 "ViewInst Include/Exclude Control Register",
                 "10", "001", "0000", "0001", "010",
                 0x880a,
                 {
                         {"EXCLUDE[<m>]", 23, 16},
                         {"INCLUDE[<m>]", 7, 0},
                 }
         },
         {
                 "TRCIDR9",
                 "ID Register 9",
                 "10", "001", "0000", "0001", "110",
                 0x880e,
                 {
                         {"NUMP0KEY", 31, 0},
                 }
         },
         {
                 "TRCVISSCTLR",
                 "ViewInst Start/Stop Control Register",
                 "10", "001", "0000", "0010", "010",
                 0x8812,
                 {
                         {"STOP[<m>]", 31, 16},
                         {"START[<m>]", 15, 0},
                 }
         },
         {
                 "TRCIDR10",
                 "ID Register 10",
                 "10", "001", "0000", "0010", "110",
                 0x8816,
                 {
                         {"NUMP1KEY", 31, 0},
                 }
         },
         {
                 "TRCSTATR",
                 "Trace Status Register",
                 "10", "001", "0000", "0011", "000",
                 0x8818,
                 {
                         {"PMSTABLE", 1, 1},
                         {"IDLE", 0, 0},
                 }
         },
         {
                 "TRCVIPCSSCTLR",
                 "ViewInst Start/Stop PE Comparator Control Register",
                 "10", "001", "0000", "0011", "010",
                 0x881a,
                 {
                         {"STOP[<m>]", 23, 16},
                         {"START[<m>]", 7, 0},
                 }
         },
         {
                 "TRCIDR11",
                 "ID Register 11",
                 "10", "001", "0000", "0011", "110",
                 0x881e,
                 {
                         {"NUMP1SPC", 31, 0},
                 }
         },
         {
                 "TRCSEQEVR0",
                 "Sequencer State Transition Control Register <n>",
                 "10", "001", "0000", "0000", "100",
                 0x8804,
                 {
                         {"B_TYPE", 15, 15},
                         {"B_SEL", 12, 8},
                         {"F_TYPE", 7, 7},
                         {"F_SEL", 4, 0},
                 }
         },
         {
                 "TRCSEQEVR1",
                 "Sequencer State Transition Control Register <n>",
                 "10", "001", "0000", "0001", "100",
                 0x880c,
                 {
                         {"B_TYPE", 15, 15},
                         {"B_SEL", 12, 8},
                         {"F_TYPE", 7, 7},
                         {"F_SEL", 4, 0},
                 }
         },
         {
                 "TRCSEQEVR2",
                 "Sequencer State Transition Control Register <n>",
                 "10", "001", "0000", "0010", "100",
                 0x8814,
                 {
                         {"B_TYPE", 15, 15},
                         {"B_SEL", 12, 8},
                         {"F_TYPE", 7, 7},
                         {"F_SEL", 4, 0},
                 }
         },
         {
                 "TRCSEQEVR3",
                 "Sequencer State Transition Control Register <n>",
                 "10", "001", "0000", "0011", "100",
                 0x881c,
                 {
                         {"B_TYPE", 15, 15},
                         {"B_SEL", 12, 8},
                         {"F_TYPE", 7, 7},
                         {"F_SEL", 4, 0},
                 }
         },
         {
                 "TRCCNTRLDVR0",
                 "Counter Reload Value Register <n>",
                 "10", "001", "0000", "0000", "101",
                 0x8805,
                 {
                         {"VALUE", 15, 0},
                 }
         },
         {
                 "TRCCNTRLDVR1",
                 "Counter Reload Value Register <n>",
                 "10", "001", "0000", "0001", "101",
                 0x880d,
                 {
                         {"VALUE", 15, 0},
                 }
         },
         {
                 "TRCCNTRLDVR2",
                 "Counter Reload Value Register <n>",
                 "10", "001", "0000", "0010", "101",
                 0x8815,
                 {
                         {"VALUE", 15, 0},
                 }
         },
         {
                 "TRCCNTRLDVR3",
                 "Counter Reload Value Register <n>",
                 "10", "001", "0000", "0011", "101",
                 0x881d,
                 {
                         {"VALUE", 15, 0},
                 }
         },
         {
                 "TRCCONFIGR",
                 "Trace Configuration Register",
                 "10", "001", "0000", "0100", "000",
                 0x8820,
                 {
                         {"VMIDOPT", 15, 15},
                         {"QE", 14, 13},
                         {"QE", 14, 13},
                         {"QE", 14, 13},
                         {"RS", 12, 12},
                         {"TS", 11, 11},
                         {"VMID", 7, 7},
                         {"CID", 6, 6},
                         {"CCI", 4, 4},
                         {"BB", 3, 3},
                 }
         },
         {
                 "TRCIDR12",
                 "ID Register 12",
                 "10", "001", "0000", "0100", "110",
                 0x8826,
                 {
                         {"NUMCONDKEY", 31, 0},
                 }
         },
         {
                 "TRCIDR13",
                 "ID Register 13",
                 "10", "001", "0000", "0101", "110",
                 0x882e,
                 {
                         {"NUMCONDSPC", 31, 0},
                 }
         },
         {
                 "TRCAUXCTLR",
                 "Auxiliary Control Register",
                 "10", "001", "0000", "0110", "000",
                 0x8830,
                 {
                         {"IMPLEMENTATION DEFINED", 31, 0},
                 }
         },
         {
                 "TRCSEQRSTEVR",
                 "Sequencer Reset Control Register",
                 "10", "001", "0000", "0110", "100",
                 0x8834,
                 {
                         {"RST_TYPE", 7, 7},
                         {"RST_SEL", 4, 0},
                 }
         },
         {
                 "TRCSEQSTR",
                 "Sequencer State Register",
                 "10", "001", "0000", "0111", "100",
                 0x883c,
                 {
                         {"STATE", 1, 0},
                 }
         },
         {
                 "TRCCNTCTLR0",
                 "Counter Control Register <n>",
                 "10", "001", "0000", "0100", "101",
                 0x8825,
                 {
                         {"CNTCHAIN", 17, 17},
                         {"RLDSELF", 16, 16},
                         {"RLDEVENT_TYPE", 15, 15},
                         {"RLDEVENT_SEL", 12, 8},
                         {"CNTEVENT_TYPE", 7, 7},
                         {"CNTEVENT_SEL", 4, 0},
                 }
         },
         {
                 "TRCCNTCTLR1",
                 "Counter Control Register <n>",
                 "10", "001", "0000", "0101", "101",
                 0x882d,
                 {
                         {"CNTCHAIN", 17, 17},
                         {"RLDSELF", 16, 16},
                         {"RLDEVENT_TYPE", 15, 15},
                         {"RLDEVENT_SEL", 12, 8},
                         {"CNTEVENT_TYPE", 7, 7},
                         {"CNTEVENT_SEL", 4, 0},
                 }
         },
         {
                 "TRCCNTCTLR2",
                 "Counter Control Register <n>",
                 "10", "001", "0000", "0110", "101",
                 0x8835,
                 {
                         {"CNTCHAIN", 17, 17},
                         {"RLDSELF", 16, 16},
                         {"RLDEVENT_TYPE", 15, 15},
                         {"RLDEVENT_SEL", 12, 8},
                         {"CNTEVENT_TYPE", 7, 7},
                         {"CNTEVENT_SEL", 4, 0},
                 }
         },
         {
                 "TRCCNTCTLR3",
                 "Counter Control Register <n>",
                 "10", "001", "0000", "0111", "101",
                 0x883d,
                 {
                         {"CNTCHAIN", 17, 17},
                         {"RLDSELF", 16, 16},
                         {"RLDEVENT_TYPE", 15, 15},
                         {"RLDEVENT_SEL", 12, 8},
                         {"CNTEVENT_TYPE", 7, 7},
                         {"CNTEVENT_SEL", 4, 0},
                 }
         },
         {
                 "TRCIMSPEC0",
                 "IMP DEF Register <n>",
                 "10", "001", "0000", "0000", "111",
                 0x8807,
                 {
                         {"IMPLEMENTATION DEFINED", 31, 0},
                 }
         },
         {
                 "TRCIMSPEC1",
                 "IMP DEF Register <n>",
                 "10", "001", "0000", "0001", "111",
                 0x880f,
                 {
                         {"IMPLEMENTATION DEFINED", 31, 0},
                 }
         },
         {
                 "TRCIMSPEC2",
                 "IMP DEF Register <n>",
                 "10", "001", "0000", "0010", "111",
                 0x8817,
                 {
                         {"IMPLEMENTATION DEFINED", 31, 0},
                 }
         },
         {
                 "TRCIMSPEC3",
                 "IMP DEF Register <n>",
                 "10", "001", "0000", "0011", "111",
                 0x881f,
                 {
                         {"IMPLEMENTATION DEFINED", 31, 0},
                 }
         },
         {
                 "TRCIMSPEC4",
                 "IMP DEF Register <n>",
                 "10", "001", "0000", "0100", "111",
                 0x8827,
                 {
                         {"IMPLEMENTATION DEFINED", 31, 0},
                 }
         },
         {
                 "TRCIMSPEC5",
                 "IMP DEF Register <n>",
                 "10", "001", "0000", "0101", "111",
                 0x882f,
                 {
                         {"IMPLEMENTATION DEFINED", 31, 0},
                 }
         },
         {
                 "TRCIMSPEC6",
                 "IMP DEF Register <n>",
                 "10", "001", "0000", "0110", "111",
                 0x8837,
                 {
                         {"IMPLEMENTATION DEFINED", 31, 0},
                 }
         },
         {
                 "TRCIMSPEC7",
                 "IMP DEF Register <n>",
                 "10", "001", "0000", "0111", "111",
                 0x883f,
                 {
                         {"IMPLEMENTATION DEFINED", 31, 0},
                 }
         },
         {
                 "TRCEVENTCTL0R",
                 "Event Control 0 Register",
                 "10", "001", "0000", "1000", "000",
                 0x8840,
                 {
                         {"EVENT3_TYPE", 31, 31},
                         {"EVENT3_SEL", 28, 24},
                         {"EVENT2_TYPE", 23, 23},
                         {"EVENT2_SEL", 20, 16},
                         {"EVENT1_TYPE", 15, 15},
                         {"EVENT1_SEL", 12, 8},
                         {"EVENT0_TYPE", 7, 7},
                         {"EVENT0_SEL", 4, 0},
                 }
         },
         {
                 "TRCIDR0",
                 "ID Register 0",
                 "10", "001", "0000", "1000", "111",
                 0x8847,
                 {
                         {"COMMTRANS", 30, 30},
                         {"COMMOPT", 29, 29},
                         {"TSSIZE", 28, 24},
                         {"TSMARK", 23, 23},
                         {"TRCEXDATA", 17, 17},
                         {"QSUPP", 16, 15},
                         {"QFILT", 14, 14},
                         {"CONDTYPE", 13, 12},
                         {"NUMEVENT", 11, 10},
                         {"NUMEVENT", 11, 10},
                         {"RETSTACK", 9, 9},
                         {"TRCCCI", 7, 7},
                         {"TRCCOND", 6, 6},
                         {"TRCBB", 5, 5},
                         {"TRCDATA", 4, 3},
                         {"INSTP0", 2, 1},
                 }
         },
         {
                 "TRCEVENTCTL1R",
                 "Event Control 1 Register",
                 "10", "001", "0000", "1001", "000",
                 0x8848,
                 {
                         {"LPOVERRIDE", 12, 12},
                         {"ATB", 11, 11},
                         {"INSTEN[<m>]", 3, 0},
                 }
         },
         {
                 "TRCIDR1",
                 "ID Register 1",
                 "10", "001", "0000", "1001", "111",
                 0x884f,
                 {
                         {"DESIGNER", 31, 24},
                         {"TRCARCHMAJ", 11, 8},
                         {"TRCARCHMIN", 7, 4},
                         {"REVISION", 3, 0},
                 }
         },
         {
                 "TRCRSR",
                 "Resources Status Register",
                 "10", "001", "0000", "1010", "000",
                 0x8850,
                 {
                         {"TA", 12, 12},
                         {"EVENT[<m>]", 11, 8},
                         {"EXTIN[<m>]", 3, 0},
                 }
         },
         {
                 "TRCIDR2",
                 "ID Register 2",
                 "10", "001", "0000", "1010", "111",
                 0x8857,
                 {
                         {"WFXMODE", 31, 31},
                         {"VMIDOPT", 30, 29},
                         {"CCSIZE", 28, 25},
                         {"DVSIZE", 24, 20},
                         {"DASIZE", 19, 15},
                         {"VMIDSIZE", 14, 10},
                         {"CIDSIZE", 9, 5},
                         {"IASIZE", 4, 0},
                 }
         },
         {
                 "TRCSTALLCTLR",
                 "Stall Control Register",
                 "10", "001", "0000", "1011", "000",
                 0x8858,
                 {
                         {"NOOVERFLOW", 13, 13},
                         {"ISTALL", 8, 8},
                         {"LEVEL", 3, 0},
                 }
         },
         {
                 "TRCIDR3",
                 "ID Register 3",
                 "10", "001", "0000", "1011", "111",
                 0x885f,
                 {
                         {"NOOVERFLOW", 31, 31},
                         {"NUMPROC", 30, 28},
                         {"SYSSTALL", 27, 27},
                         {"STALLCTL", 26, 26},
                         {"SYNCPR", 25, 25},
                         {"TRCERR", 24, 24},
                         {"EXLEVEL_NS_EL2", 22, 22},
                         {"EXLEVEL_NS_EL1", 21, 21},
                         {"EXLEVEL_NS_EL0", 20, 20},
                         {"EXLEVEL_S_EL3", 19, 19},
                         {"EXLEVEL_S_EL2", 18, 18},
                         {"EXLEVEL_S_EL1", 17, 17},
                         {"EXLEVEL_S_EL0", 16, 16},
                         {"NUMPROC[4:3]", 13, 12},
                         {"CCITMIN", 11, 0},
                 }
         },
         {
                 "TRCEXTINSELR0",
                 "External Input Select Register <n>",
                 "10", "001", "0000", "1000", "100",
                 0x8844,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "TRCEXTINSELR1",
                 "External Input Select Register <n>",
                 "10", "001", "0000", "1001", "100",
                 0x884c,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "TRCEXTINSELR2",
                 "External Input Select Register <n>",
                 "10", "001", "0000", "1010", "100",
                 0x8854,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "TRCEXTINSELR3",
                 "External Input Select Register <n>",
                 "10", "001", "0000", "1011", "100",
                 0x885c,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "TRCCNTVR0",
                 "Counter Value Register <n>",
                 "10", "001", "0000", "1000", "101",
                 0x8845,
                 {
                         {"VALUE", 15, 0},
                 }
         },
         {
                 "TRCCNTVR1",
                 "Counter Value Register <n>",
                 "10", "001", "0000", "1001", "101",
                 0x884d,
                 {
                         {"VALUE", 15, 0},
                 }
         },
         {
                 "TRCCNTVR2",
                 "Counter Value Register <n>",
                 "10", "001", "0000", "1010", "101",
                 0x8855,
                 {
                         {"VALUE", 15, 0},
                 }
         },
         {
                 "TRCCNTVR3",
                 "Counter Value Register <n>",
                 "10", "001", "0000", "1011", "101",
                 0x885d,
                 {
                         {"VALUE", 15, 0},
                 }
         },
         {
                 "TRCTSCTLR",
                 "Timestamp Control Register",
                 "10", "001", "0000", "1100", "000",
                 0x8860,
                 {
                         {"EVENT_TYPE", 7, 7},
                         {"EVENT_SEL", 4, 0},
                 }
         },
         {
                 "TRCIDR4",
                 "ID Register 4",
                 "10", "001", "0000", "1100", "111",
                 0x8867,
                 {
                         {"NUMVMIDC", 31, 28},
                         {"NUMCIDC", 27, 24},
                         {"NUMSSCC", 23, 20},
                         {"NUMRSPAIR", 19, 16},
                         {"NUMPC", 15, 12},
                         {"SUPPDAC", 8, 8},
                         {"NUMDVC", 7, 4},
                         {"NUMACPAIRS", 3, 0},
                 }
         },
         {
                 "TRCSYNCPR",
                 "Synchronization Period Register",
                 "10", "001", "0000", "1101", "000",
                 0x8868,
                 {
                         {"PERIOD", 4, 0},
                 }
         },
         {
                 "TRCIDR5",
                 "ID Register 5",
                 "10", "001", "0000", "1101", "111",
                 0x886f,
                 {
                         {"NUMCNTR", 30, 28},
                         {"NUMSEQSTATE", 27, 25},
                         {"LPOVERRIDE", 23, 23},
                         {"ATBTRIG", 22, 22},
                         {"TRACEIDSIZE", 21, 16},
                         {"NUMEXTINSEL", 11, 9},
                         {"NUMEXTIN", 8, 0},
                 }
         },
         {
                 "TRCCCCTLR",
                 "Cycle Count Control Register",
                 "10", "001", "0000", "1110", "000",
                 0x8870,
                 {
                         {"THRESHOLD", 11, 0},
                 }
         },
         {
                 "TRCIDR6",
                 "ID Register 6",
                 "10", "001", "0000", "1110", "111",
                 0x8877,
                 {
                         {"EXLEVEL_RL_EL2", 2, 2},
                         {"EXLEVEL_RL_EL1", 1, 1},
                         {"EXLEVEL_RL_EL0", 0, 0},
                 }
         },
         {
                 "TRCBBCTLR",
                 "Branch Broadcast Control Register",
                 "10", "001", "0000", "1111", "000",
                 0x8878,
                 {
                         {"MODE", 8, 8},
                         {"RANGE[<m>]", 7, 0},
                 }
         },
         {
                 "TRCIDR7",
                 "ID Register 7",
                 "10", "001", "0000", "1111", "111",
                 0x887f,
                 {
                 }
         },
         {
                 "TRCOSLSR",
                 "Trace OS Lock Status Register",
                 "10", "001", "0001", "0001", "100",
                 0x888c,
                 {
                         {"OSLM", 4, 3},
                         {"OSLK", 1, 1},
                         {"OSLM[0]", 0, 0},
                 }
         },
         {
                 "TRCSSCCR0",
                 "Single-shot Comparator Control Register <n>",
                 "10", "001", "0001", "0000", "010",
                 0x8882,
                 {
                         {"RST", 24, 24},
                         {"ARC[<m>]", 23, 16},
                         {"SAC[<m>]", 15, 0},
                 }
         },
         {
                 "TRCSSCCR1",
                 "Single-shot Comparator Control Register <n>",
                 "10", "001", "0001", "0001", "010",
                 0x888a,
                 {
                         {"RST", 24, 24},
                         {"ARC[<m>]", 23, 16},
                         {"SAC[<m>]", 15, 0},
                 }
         },
         {
                 "TRCSSCCR2",
                 "Single-shot Comparator Control Register <n>",
                 "10", "001", "0001", "0010", "010",
                 0x8892,
                 {
                         {"RST", 24, 24},
                         {"ARC[<m>]", 23, 16},
                         {"SAC[<m>]", 15, 0},
                 }
         },
         {
                 "TRCSSCCR3",
                 "Single-shot Comparator Control Register <n>",
                 "10", "001", "0001", "0011", "010",
                 0x889a,
                 {
                         {"RST", 24, 24},
                         {"ARC[<m>]", 23, 16},
                         {"SAC[<m>]", 15, 0},
                 }
         },
         {
                 "TRCSSCCR4",
                 "Single-shot Comparator Control Register <n>",
                 "10", "001", "0001", "0100", "010",
                 0x88a2,
                 {
                         {"RST", 24, 24},
                         {"ARC[<m>]", 23, 16},
                         {"SAC[<m>]", 15, 0},
                 }
         },
         {
                 "TRCSSCCR5",
                 "Single-shot Comparator Control Register <n>",
                 "10", "001", "0001", "0101", "010",
                 0x88aa,
                 {
                         {"RST", 24, 24},
                         {"ARC[<m>]", 23, 16},
                         {"SAC[<m>]", 15, 0},
                 }
         },
         {
                 "TRCSSCCR6",
                 "Single-shot Comparator Control Register <n>",
                 "10", "001", "0001", "0110", "010",
                 0x88b2,
                 {
                         {"RST", 24, 24},
                         {"ARC[<m>]", 23, 16},
                         {"SAC[<m>]", 15, 0},
                 }
         },
         {
                 "TRCSSCCR7",
                 "Single-shot Comparator Control Register <n>",
                 "10", "001", "0001", "0111", "010",
                 0x88ba,
                 {
                         {"RST", 24, 24},
                         {"ARC[<m>]", 23, 16},
                         {"SAC[<m>]", 15, 0},
                 }
         },
         {
                 "TRCSSPCICR0",
                 "Single-shot Processing Element Comparator Input Control Register <n>",
                 "10", "001", "0001", "0000", "011",
                 0x8883,
                 {
                         {"PC[<m>]", 7, 0},
                 }
         },
         {
                 "TRCSSPCICR1",
                 "Single-shot Processing Element Comparator Input Control Register <n>",
                 "10", "001", "0001", "0001", "011",
                 0x888b,
                 {
                         {"PC[<m>]", 7, 0},
                 }
         },
         {
                 "TRCSSPCICR2",
                 "Single-shot Processing Element Comparator Input Control Register <n>",
                 "10", "001", "0001", "0010", "011",
                 0x8893,
                 {
                         {"PC[<m>]", 7, 0},
                 }
         },
         {
                 "TRCSSPCICR3",
                 "Single-shot Processing Element Comparator Input Control Register <n>",
                 "10", "001", "0001", "0011", "011",
                 0x889b,
                 {
                         {"PC[<m>]", 7, 0},
                 }
         },
         {
                 "TRCSSPCICR4",
                 "Single-shot Processing Element Comparator Input Control Register <n>",
                 "10", "001", "0001", "0100", "011",
                 0x88a3,
                 {
                         {"PC[<m>]", 7, 0},
                 }
         },
         {
                 "TRCSSPCICR5",
                 "Single-shot Processing Element Comparator Input Control Register <n>",
                 "10", "001", "0001", "0101", "011",
                 0x88ab,
                 {
                         {"PC[<m>]", 7, 0},
                 }
         },
         {
                 "TRCSSPCICR6",
                 "Single-shot Processing Element Comparator Input Control Register <n>",
                 "10", "001", "0001", "0110", "011",
                 0x88b3,
                 {
                         {"PC[<m>]", 7, 0},
                 }
         },
         {
                 "TRCSSPCICR7",
                 "Single-shot Processing Element Comparator Input Control Register <n>",
                 "10", "001", "0001", "0111", "011",
                 0x88bb,
                 {
                         {"PC[<m>]", 7, 0},
                 }
         },
         {
                 "TRCSSCSR0",
                 "Single-shot Comparator Control Status Register <n>",
                 "10", "001", "0001", "1000", "010",
                 0x88c2,
                 {
                         {"STATUS", 31, 31},
                         {"PENDING", 30, 30},
                         {"PC", 3, 3},
                         {"DV", 2, 2},
                         {"DA", 1, 1},
                         {"INST", 0, 0},
                 }
         },
         {
                 "TRCSSCSR1",
                 "Single-shot Comparator Control Status Register <n>",
                 "10", "001", "0001", "1001", "010",
                 0x88ca,
                 {
                         {"STATUS", 31, 31},
                         {"PENDING", 30, 30},
                         {"PC", 3, 3},
                         {"DV", 2, 2},
                         {"DA", 1, 1},
                         {"INST", 0, 0},
                 }
         },
         {
                 "TRCSSCSR2",
                 "Single-shot Comparator Control Status Register <n>",
                 "10", "001", "0001", "1010", "010",
                 0x88d2,
                 {
                         {"STATUS", 31, 31},
                         {"PENDING", 30, 30},
                         {"PC", 3, 3},
                         {"DV", 2, 2},
                         {"DA", 1, 1},
                         {"INST", 0, 0},
                 }
         },
         {
                 "TRCSSCSR3",
                 "Single-shot Comparator Control Status Register <n>",
                 "10", "001", "0001", "1011", "010",
                 0x88da,
                 {
                         {"STATUS", 31, 31},
                         {"PENDING", 30, 30},
                         {"PC", 3, 3},
                         {"DV", 2, 2},
                         {"DA", 1, 1},
                         {"INST", 0, 0},
                 }
         },
         {
                 "TRCSSCSR4",
                 "Single-shot Comparator Control Status Register <n>",
                 "10", "001", "0001", "1100", "010",
                 0x88e2,
                 {
                         {"STATUS", 31, 31},
                         {"PENDING", 30, 30},
                         {"PC", 3, 3},
                         {"DV", 2, 2},
                         {"DA", 1, 1},
                         {"INST", 0, 0},
                 }
         },
         {
                 "TRCSSCSR5",
                 "Single-shot Comparator Control Status Register <n>",
                 "10", "001", "0001", "1101", "010",
                 0x88ea,
                 {
                         {"STATUS", 31, 31},
                         {"PENDING", 30, 30},
                         {"PC", 3, 3},
                         {"DV", 2, 2},
                         {"DA", 1, 1},
                         {"INST", 0, 0},
                 }
         },
         {
                 "TRCSSCSR6",
                 "Single-shot Comparator Control Status Register <n>",
                 "10", "001", "0001", "1110", "010",
                 0x88f2,
                 {
                         {"STATUS", 31, 31},
                         {"PENDING", 30, 30},
                         {"PC", 3, 3},
                         {"DV", 2, 2},
                         {"DA", 1, 1},
                         {"INST", 0, 0},
                 }
         },
         {
                 "TRCSSCSR7",
                 "Single-shot Comparator Control Status Register <n>",
                 "10", "001", "0001", "1111", "010",
                 0x88fa,
                 {
                         {"STATUS", 31, 31},
                         {"PENDING", 30, 30},
                         {"PC", 3, 3},
                         {"DV", 2, 2},
                         {"DA", 1, 1},
                         {"INST", 0, 0},
                 }
         },
         {
                 "TRCRSCTLR0",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "0000", "000",
                 0x8880,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR1",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "0000", "001",
                 0x8881,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR2",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "0001", "000",
                 0x8888,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR3",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "0001", "001",
                 0x8889,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR4",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "0010", "000",
                 0x8890,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR5",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "0010", "001",
                 0x8891,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR6",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "0011", "000",
                 0x8898,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR7",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "0011", "001",
                 0x8899,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR8",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "0100", "000",
                 0x88a0,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR9",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "0100", "001",
                 0x88a1,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR10",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "0101", "000",
                 0x88a8,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR11",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "0101", "001",
                 0x88a9,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR12",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "0110", "000",
                 0x88b0,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR13",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "0110", "001",
                 0x88b1,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR14",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "0111", "000",
                 0x88b8,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR15",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "0111", "001",
                 0x88b9,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR16",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "1000", "000",
                 0x88c0,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR17",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "1000", "001",
                 0x88c1,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR18",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "1001", "000",
                 0x88c8,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR19",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "1001", "001",
                 0x88c9,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR20",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "1010", "000",
                 0x88d0,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR21",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "1010", "001",
                 0x88d1,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR22",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "1011", "000",
                 0x88d8,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR23",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "1011", "001",
                 0x88d9,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR24",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "1100", "000",
                 0x88e0,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR25",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "1100", "001",
                 0x88e1,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR26",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "1101", "000",
                 0x88e8,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR27",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "1101", "001",
                 0x88e9,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR28",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "1110", "000",
                 0x88f0,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR29",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "1110", "001",
                 0x88f1,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR30",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "1111", "000",
                 0x88f8,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCRSCTLR31",
                 "Resource Selection Control Register <n>",
                 "10", "001", "0001", "1111", "001",
                 0x88f9,
                 {
                         {"PAIRINV", 21, 21},
                         {"INV", 20, 20},
                         {"GROUP", 19, 16},
                         {"SELECT", 15, 0},
                 }
         },
         {
                 "TRCACVR0",
                 "Address Comparator Value Register <n>",
                 "10", "001", "0010", "0000", "000",
                 0x8900,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "TRCACVR1",
                 "Address Comparator Value Register <n>",
                 "10", "001", "0010", "0000", "001",
                 0x8901,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "TRCACVR2",
                 "Address Comparator Value Register <n>",
                 "10", "001", "0010", "0010", "000",
                 0x8910,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "TRCACVR3",
                 "Address Comparator Value Register <n>",
                 "10", "001", "0010", "0010", "001",
                 0x8911,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "TRCACVR4",
                 "Address Comparator Value Register <n>",
                 "10", "001", "0010", "0100", "000",
                 0x8920,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "TRCACVR5",
                 "Address Comparator Value Register <n>",
                 "10", "001", "0010", "0100", "001",
                 0x8921,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "TRCACVR6",
                 "Address Comparator Value Register <n>",
                 "10", "001", "0010", "0110", "000",
                 0x8930,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "TRCACVR7",
                 "Address Comparator Value Register <n>",
                 "10", "001", "0010", "0110", "001",
                 0x8931,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "TRCACVR8",
                 "Address Comparator Value Register <n>",
                 "10", "001", "0010", "1000", "000",
                 0x8940,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "TRCACVR9",
                 "Address Comparator Value Register <n>",
                 "10", "001", "0010", "1000", "001",
                 0x8941,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "TRCACVR10",
                 "Address Comparator Value Register <n>",
                 "10", "001", "0010", "1010", "000",
                 0x8950,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "TRCACVR11",
                 "Address Comparator Value Register <n>",
                 "10", "001", "0010", "1010", "001",
                 0x8951,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "TRCACVR12",
                 "Address Comparator Value Register <n>",
                 "10", "001", "0010", "1100", "000",
                 0x8960,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "TRCACVR13",
                 "Address Comparator Value Register <n>",
                 "10", "001", "0010", "1100", "001",
                 0x8961,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "TRCACVR14",
                 "Address Comparator Value Register <n>",
                 "10", "001", "0010", "1110", "000",
                 0x8970,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "TRCACVR15",
                 "Address Comparator Value Register <n>",
                 "10", "001", "0010", "1110", "001",
                 0x8971,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "TRCACATR0",
                 "Address Comparator Access Type Register <n>",
                 "10", "001", "0010", "0000", "010",
                 0x8902,
                 {
                         {"EXLEVEL_RL_EL2", 18, 18},
                         {"EXLEVEL_RL_EL1", 17, 17},
                         {"EXLEVEL_RL_EL0", 16, 16},
                         {"EXLEVEL_NS_EL2", 14, 14},
                         {"EXLEVEL_NS_EL1", 13, 13},
                         {"EXLEVEL_NS_EL0", 12, 12},
                         {"EXLEVEL_S_EL3", 11, 11},
                         {"EXLEVEL_S_EL2", 10, 10},
                         {"EXLEVEL_S_EL1", 9, 9},
                         {"EXLEVEL_S_EL0", 8, 8},
                         {"CONTEXT", 6, 4},
                         {"CONTEXTTYPE", 3, 2},
                 }
         },
         {
                 "TRCACATR1",
                 "Address Comparator Access Type Register <n>",
                 "10", "001", "0010", "0000", "011",
                 0x8903,
                 {
                         {"EXLEVEL_RL_EL2", 18, 18},
                         {"EXLEVEL_RL_EL1", 17, 17},
                         {"EXLEVEL_RL_EL0", 16, 16},
                         {"EXLEVEL_NS_EL2", 14, 14},
                         {"EXLEVEL_NS_EL1", 13, 13},
                         {"EXLEVEL_NS_EL0", 12, 12},
                         {"EXLEVEL_S_EL3", 11, 11},
                         {"EXLEVEL_S_EL2", 10, 10},
                         {"EXLEVEL_S_EL1", 9, 9},
                         {"EXLEVEL_S_EL0", 8, 8},
                         {"CONTEXT", 6, 4},
                         {"CONTEXTTYPE", 3, 2},
                 }
         },
         {
                 "TRCACATR2",
                 "Address Comparator Access Type Register <n>",
                 "10", "001", "0010", "0010", "010",
                 0x8912,
                 {
                         {"EXLEVEL_RL_EL2", 18, 18},
                         {"EXLEVEL_RL_EL1", 17, 17},
                         {"EXLEVEL_RL_EL0", 16, 16},
                         {"EXLEVEL_NS_EL2", 14, 14},
                         {"EXLEVEL_NS_EL1", 13, 13},
                         {"EXLEVEL_NS_EL0", 12, 12},
                         {"EXLEVEL_S_EL3", 11, 11},
                         {"EXLEVEL_S_EL2", 10, 10},
                         {"EXLEVEL_S_EL1", 9, 9},
                         {"EXLEVEL_S_EL0", 8, 8},
                         {"CONTEXT", 6, 4},
                         {"CONTEXTTYPE", 3, 2},
                 }
         },
         {
                 "TRCACATR3",
                 "Address Comparator Access Type Register <n>",
                 "10", "001", "0010", "0010", "011",
                 0x8913,
                 {
                         {"EXLEVEL_RL_EL2", 18, 18},
                         {"EXLEVEL_RL_EL1", 17, 17},
                         {"EXLEVEL_RL_EL0", 16, 16},
                         {"EXLEVEL_NS_EL2", 14, 14},
                         {"EXLEVEL_NS_EL1", 13, 13},
                         {"EXLEVEL_NS_EL0", 12, 12},
                         {"EXLEVEL_S_EL3", 11, 11},
                         {"EXLEVEL_S_EL2", 10, 10},
                         {"EXLEVEL_S_EL1", 9, 9},
                         {"EXLEVEL_S_EL0", 8, 8},
                         {"CONTEXT", 6, 4},
                         {"CONTEXTTYPE", 3, 2},
                 }
         },
         {
                 "TRCACATR4",
                 "Address Comparator Access Type Register <n>",
                 "10", "001", "0010", "0100", "010",
                 0x8922,
                 {
                         {"EXLEVEL_RL_EL2", 18, 18},
                         {"EXLEVEL_RL_EL1", 17, 17},
                         {"EXLEVEL_RL_EL0", 16, 16},
                         {"EXLEVEL_NS_EL2", 14, 14},
                         {"EXLEVEL_NS_EL1", 13, 13},
                         {"EXLEVEL_NS_EL0", 12, 12},
                         {"EXLEVEL_S_EL3", 11, 11},
                         {"EXLEVEL_S_EL2", 10, 10},
                         {"EXLEVEL_S_EL1", 9, 9},
                         {"EXLEVEL_S_EL0", 8, 8},
                         {"CONTEXT", 6, 4},
                         {"CONTEXTTYPE", 3, 2},
                 }
         },
         {
                 "TRCACATR5",
                 "Address Comparator Access Type Register <n>",
                 "10", "001", "0010", "0100", "011",
                 0x8923,
                 {
                         {"EXLEVEL_RL_EL2", 18, 18},
                         {"EXLEVEL_RL_EL1", 17, 17},
                         {"EXLEVEL_RL_EL0", 16, 16},
                         {"EXLEVEL_NS_EL2", 14, 14},
                         {"EXLEVEL_NS_EL1", 13, 13},
                         {"EXLEVEL_NS_EL0", 12, 12},
                         {"EXLEVEL_S_EL3", 11, 11},
                         {"EXLEVEL_S_EL2", 10, 10},
                         {"EXLEVEL_S_EL1", 9, 9},
                         {"EXLEVEL_S_EL0", 8, 8},
                         {"CONTEXT", 6, 4},
                         {"CONTEXTTYPE", 3, 2},
                 }
         },
         {
                 "TRCACATR6",
                 "Address Comparator Access Type Register <n>",
                 "10", "001", "0010", "0110", "010",
                 0x8932,
                 {
                         {"EXLEVEL_RL_EL2", 18, 18},
                         {"EXLEVEL_RL_EL1", 17, 17},
                         {"EXLEVEL_RL_EL0", 16, 16},
                         {"EXLEVEL_NS_EL2", 14, 14},
                         {"EXLEVEL_NS_EL1", 13, 13},
                         {"EXLEVEL_NS_EL0", 12, 12},
                         {"EXLEVEL_S_EL3", 11, 11},
                         {"EXLEVEL_S_EL2", 10, 10},
                         {"EXLEVEL_S_EL1", 9, 9},
                         {"EXLEVEL_S_EL0", 8, 8},
                         {"CONTEXT", 6, 4},
                         {"CONTEXTTYPE", 3, 2},
                 }
         },
         {
                 "TRCACATR7",
                 "Address Comparator Access Type Register <n>",
                 "10", "001", "0010", "0110", "011",
                 0x8933,
                 {
                         {"EXLEVEL_RL_EL2", 18, 18},
                         {"EXLEVEL_RL_EL1", 17, 17},
                         {"EXLEVEL_RL_EL0", 16, 16},
                         {"EXLEVEL_NS_EL2", 14, 14},
                         {"EXLEVEL_NS_EL1", 13, 13},
                         {"EXLEVEL_NS_EL0", 12, 12},
                         {"EXLEVEL_S_EL3", 11, 11},
                         {"EXLEVEL_S_EL2", 10, 10},
                         {"EXLEVEL_S_EL1", 9, 9},
                         {"EXLEVEL_S_EL0", 8, 8},
                         {"CONTEXT", 6, 4},
                         {"CONTEXTTYPE", 3, 2},
                 }
         },
         {
                 "TRCACATR8",
                 "Address Comparator Access Type Register <n>",
                 "10", "001", "0010", "1000", "010",
                 0x8942,
                 {
                         {"EXLEVEL_RL_EL2", 18, 18},
                         {"EXLEVEL_RL_EL1", 17, 17},
                         {"EXLEVEL_RL_EL0", 16, 16},
                         {"EXLEVEL_NS_EL2", 14, 14},
                         {"EXLEVEL_NS_EL1", 13, 13},
                         {"EXLEVEL_NS_EL0", 12, 12},
                         {"EXLEVEL_S_EL3", 11, 11},
                         {"EXLEVEL_S_EL2", 10, 10},
                         {"EXLEVEL_S_EL1", 9, 9},
                         {"EXLEVEL_S_EL0", 8, 8},
                         {"CONTEXT", 6, 4},
                         {"CONTEXTTYPE", 3, 2},
                 }
         },
         {
                 "TRCACATR9",
                 "Address Comparator Access Type Register <n>",
                 "10", "001", "0010", "1000", "011",
                 0x8943,
                 {
                         {"EXLEVEL_RL_EL2", 18, 18},
                         {"EXLEVEL_RL_EL1", 17, 17},
                         {"EXLEVEL_RL_EL0", 16, 16},
                         {"EXLEVEL_NS_EL2", 14, 14},
                         {"EXLEVEL_NS_EL1", 13, 13},
                         {"EXLEVEL_NS_EL0", 12, 12},
                         {"EXLEVEL_S_EL3", 11, 11},
                         {"EXLEVEL_S_EL2", 10, 10},
                         {"EXLEVEL_S_EL1", 9, 9},
                         {"EXLEVEL_S_EL0", 8, 8},
                         {"CONTEXT", 6, 4},
                         {"CONTEXTTYPE", 3, 2},
                 }
         },
         {
                 "TRCACATR10",
                 "Address Comparator Access Type Register <n>",
                 "10", "001", "0010", "1010", "010",
                 0x8952,
                 {
                         {"EXLEVEL_RL_EL2", 18, 18},
                         {"EXLEVEL_RL_EL1", 17, 17},
                         {"EXLEVEL_RL_EL0", 16, 16},
                         {"EXLEVEL_NS_EL2", 14, 14},
                         {"EXLEVEL_NS_EL1", 13, 13},
                         {"EXLEVEL_NS_EL0", 12, 12},
                         {"EXLEVEL_S_EL3", 11, 11},
                         {"EXLEVEL_S_EL2", 10, 10},
                         {"EXLEVEL_S_EL1", 9, 9},
                         {"EXLEVEL_S_EL0", 8, 8},
                         {"CONTEXT", 6, 4},
                         {"CONTEXTTYPE", 3, 2},
                 }
         },
         {
                 "TRCACATR11",
                 "Address Comparator Access Type Register <n>",
                 "10", "001", "0010", "1010", "011",
                 0x8953,
                 {
                         {"EXLEVEL_RL_EL2", 18, 18},
                         {"EXLEVEL_RL_EL1", 17, 17},
                         {"EXLEVEL_RL_EL0", 16, 16},
                         {"EXLEVEL_NS_EL2", 14, 14},
                         {"EXLEVEL_NS_EL1", 13, 13},
                         {"EXLEVEL_NS_EL0", 12, 12},
                         {"EXLEVEL_S_EL3", 11, 11},
                         {"EXLEVEL_S_EL2", 10, 10},
                         {"EXLEVEL_S_EL1", 9, 9},
                         {"EXLEVEL_S_EL0", 8, 8},
                         {"CONTEXT", 6, 4},
                         {"CONTEXTTYPE", 3, 2},
                 }
         },
         {
                 "TRCACATR12",
                 "Address Comparator Access Type Register <n>",
                 "10", "001", "0010", "1100", "010",
                 0x8962,
                 {
                         {"EXLEVEL_RL_EL2", 18, 18},
                         {"EXLEVEL_RL_EL1", 17, 17},
                         {"EXLEVEL_RL_EL0", 16, 16},
                         {"EXLEVEL_NS_EL2", 14, 14},
                         {"EXLEVEL_NS_EL1", 13, 13},
                         {"EXLEVEL_NS_EL0", 12, 12},
                         {"EXLEVEL_S_EL3", 11, 11},
                         {"EXLEVEL_S_EL2", 10, 10},
                         {"EXLEVEL_S_EL1", 9, 9},
                         {"EXLEVEL_S_EL0", 8, 8},
                         {"CONTEXT", 6, 4},
                         {"CONTEXTTYPE", 3, 2},
                 }
         },
         {
                 "TRCACATR13",
                 "Address Comparator Access Type Register <n>",
                 "10", "001", "0010", "1100", "011",
                 0x8963,
                 {
                         {"EXLEVEL_RL_EL2", 18, 18},
                         {"EXLEVEL_RL_EL1", 17, 17},
                         {"EXLEVEL_RL_EL0", 16, 16},
                         {"EXLEVEL_NS_EL2", 14, 14},
                         {"EXLEVEL_NS_EL1", 13, 13},
                         {"EXLEVEL_NS_EL0", 12, 12},
                         {"EXLEVEL_S_EL3", 11, 11},
                         {"EXLEVEL_S_EL2", 10, 10},
                         {"EXLEVEL_S_EL1", 9, 9},
                         {"EXLEVEL_S_EL0", 8, 8},
                         {"CONTEXT", 6, 4},
                         {"CONTEXTTYPE", 3, 2},
                 }
         },
         {
                 "TRCACATR14",
                 "Address Comparator Access Type Register <n>",
                 "10", "001", "0010", "1110", "010",
                 0x8972,
                 {
                         {"EXLEVEL_RL_EL2", 18, 18},
                         {"EXLEVEL_RL_EL1", 17, 17},
                         {"EXLEVEL_RL_EL0", 16, 16},
                         {"EXLEVEL_NS_EL2", 14, 14},
                         {"EXLEVEL_NS_EL1", 13, 13},
                         {"EXLEVEL_NS_EL0", 12, 12},
                         {"EXLEVEL_S_EL3", 11, 11},
                         {"EXLEVEL_S_EL2", 10, 10},
                         {"EXLEVEL_S_EL1", 9, 9},
                         {"EXLEVEL_S_EL0", 8, 8},
                         {"CONTEXT", 6, 4},
                         {"CONTEXTTYPE", 3, 2},
                 }
         },
         {
                 "TRCACATR15",
                 "Address Comparator Access Type Register <n>",
                 "10", "001", "0010", "1110", "011",
                 0x8973,
                 {
                         {"EXLEVEL_RL_EL2", 18, 18},
                         {"EXLEVEL_RL_EL1", 17, 17},
                         {"EXLEVEL_RL_EL0", 16, 16},
                         {"EXLEVEL_NS_EL2", 14, 14},
                         {"EXLEVEL_NS_EL1", 13, 13},
                         {"EXLEVEL_NS_EL0", 12, 12},
                         {"EXLEVEL_S_EL3", 11, 11},
                         {"EXLEVEL_S_EL2", 10, 10},
                         {"EXLEVEL_S_EL1", 9, 9},
                         {"EXLEVEL_S_EL0", 8, 8},
                         {"CONTEXT", 6, 4},
                         {"CONTEXTTYPE", 3, 2},
                 }
         },
         {
                 "TRCCIDCCTLR0",
                 "Context Identifier Comparator Control Register 0",
                 "10", "001", "0011", "0000", "010",
                 0x8982,
                 {
                         {"COMP3[<m>]", 31, 24},
                         {"COMP2[<m>]", 23, 16},
                         {"COMP1[<m>]", 15, 8},
                         {"COMP0[<m>]", 7, 0},
                 }
         },
         {
                 "TRCCIDCCTLR1",
                 "Context Identifier Comparator Control Register 1",
                 "10", "001", "0011", "0001", "010",
                 0x898a,
                 {
                         {"COMP7[<m>]", 31, 24},
                         {"COMP6[<m>]", 23, 16},
                         {"COMP5[<m>]", 15, 8},
                         {"COMP4[<m>]", 7, 0},
                 }
         },
         {
                 "TRCVMIDCCTLR0",
                 "Virtual Context Identifier Comparator Control Register 0",
                 "10", "001", "0011", "0010", "010",
                 0x8992,
                 {
                         {"COMP3[<m>]", 31, 24},
                         {"COMP2[<m>]", 23, 16},
                         {"COMP1[<m>]", 15, 8},
                         {"COMP0[<m>]", 7, 0},
                 }
         },
         {
                 "TRCVMIDCCTLR1",
                 "Virtual Context Identifier Comparator Control Register 1",
                 "10", "001", "0011", "0011", "010",
                 0x899a,
                 {
                         {"COMP7[<m>]", 31, 24},
                         {"COMP6[<m>]", 23, 16},
                         {"COMP5[<m>]", 15, 8},
                         {"COMP4[<m>]", 7, 0},
                 }
         },
         {
                 "TRCCIDCVR0",
                 "Context Identifier Comparator Value Registers <n>",
                 "10", "001", "0011", "0000", "000",
                 0x8980,
                 {
                         {"VALUE", 63, 0},
                 }
         },
         {
                 "TRCCIDCVR1",
                 "Context Identifier Comparator Value Registers <n>",
                 "10", "001", "0011", "0010", "000",
                 0x8990,
                 {
                         {"VALUE", 63, 0},
                 }
         },
         {
                 "TRCCIDCVR2",
                 "Context Identifier Comparator Value Registers <n>",
                 "10", "001", "0011", "0100", "000",
                 0x89a0,
                 {
                         {"VALUE", 63, 0},
                 }
         },
         {
                 "TRCCIDCVR3",
                 "Context Identifier Comparator Value Registers <n>",
                 "10", "001", "0011", "0110", "000",
                 0x89b0,
                 {
                         {"VALUE", 63, 0},
                 }
         },
         {
                 "TRCCIDCVR4",
                 "Context Identifier Comparator Value Registers <n>",
                 "10", "001", "0011", "1000", "000",
                 0x89c0,
                 {
                         {"VALUE", 63, 0},
                 }
         },
         {
                 "TRCCIDCVR5",
                 "Context Identifier Comparator Value Registers <n>",
                 "10", "001", "0011", "1010", "000",
                 0x89d0,
                 {
                         {"VALUE", 63, 0},
                 }
         },
         {
                 "TRCCIDCVR6",
                 "Context Identifier Comparator Value Registers <n>",
                 "10", "001", "0011", "1100", "000",
                 0x89e0,
                 {
                         {"VALUE", 63, 0},
                 }
         },
         {
                 "TRCCIDCVR7",
                 "Context Identifier Comparator Value Registers <n>",
                 "10", "001", "0011", "1110", "000",
                 0x89f0,
                 {
                         {"VALUE", 63, 0},
                 }
         },
         {
                 "TRCVMIDCVR0",
                 "Virtual Context Identifier Comparator Value Register <n>",
                 "10", "001", "0011", "0000", "001",
                 0x8981,
                 {
                         {"VALUE", 63, 0},
                 }
         },
         {
                 "TRCVMIDCVR1",
                 "Virtual Context Identifier Comparator Value Register <n>",
                 "10", "001", "0011", "0010", "001",
                 0x8991,
                 {
                         {"VALUE", 63, 0},
                 }
         },
         {
                 "TRCVMIDCVR2",
                 "Virtual Context Identifier Comparator Value Register <n>",
                 "10", "001", "0011", "0100", "001",
                 0x89a1,
                 {
                         {"VALUE", 63, 0},
                 }
         },
         {
                 "TRCVMIDCVR3",
                 "Virtual Context Identifier Comparator Value Register <n>",
                 "10", "001", "0011", "0110", "001",
                 0x89b1,
                 {
                         {"VALUE", 63, 0},
                 }
         },
         {
                 "TRCVMIDCVR4",
                 "Virtual Context Identifier Comparator Value Register <n>",
                 "10", "001", "0011", "1000", "001",
                 0x89c1,
                 {
                         {"VALUE", 63, 0},
                 }
         },
         {
                 "TRCVMIDCVR5",
                 "Virtual Context Identifier Comparator Value Register <n>",
                 "10", "001", "0011", "1010", "001",
                 0x89d1,
                 {
                         {"VALUE", 63, 0},
                 }
         },
         {
                 "TRCVMIDCVR6",
                 "Virtual Context Identifier Comparator Value Register <n>",
                 "10", "001", "0011", "1100", "001",
                 0x89e1,
                 {
                         {"VALUE", 63, 0},
                 }
         },
         {
                 "TRCVMIDCVR7",
                 "Virtual Context Identifier Comparator Value Register <n>",
                 "10", "001", "0011", "1110", "001",
                 0x89f1,
                 {
                         {"VALUE", 63, 0},
                 }
         },
         {
                 "TRCDEVID",
                 "Device Configuration Register",
                 "10", "001", "0111", "0010", "111",
                 0x8b97,
                 {
                 }
         },
         {
                 "TRCCLAIMSET",
                 "Claim Tag Set Register",
                 "10", "001", "0111", "1000", "110",
                 0x8bc6,
                 {
                         {"SET[<m>]", 31, 0},
                 }
         },
         {
                 "TRCCLAIMCLR",
                 "Claim Tag Clear Register",
                 "10", "001", "0111", "1001", "110",
                 0x8bce,
                 {
                         {"CLR[<m>]", 31, 0},
                 }
         },
         {
                 "TRCAUTHSTATUS",
                 "Authentication Status Register",
                 "10", "001", "0111", "1110", "110",
                 0x8bf6,
                 {
                         {"RTNID", 27, 26},
                         {"RTID", 25, 24},
                         {"RLNID", 15, 14},
                         {"RLID", 13, 12},
                         {"HNID", 11, 10},
                         {"HID", 9, 8},
                         {"SNID", 7, 6},
                         {"SID", 5, 4},
                         {"NSNID", 3, 2},
                         {"NSID", 1, 0},
                 }
         },
         {
                 "TRCDEVARCH",
                 "Device Architecture Register",
                 "10", "001", "0111", "1111", "110",
                 0x8bfe,
                 {
                         {"ARCHITECT", 31, 21},
                         {"PRESENT", 20, 20},
                         {"REVISION", 19, 16},
                         {"ARCHVER", 15, 12},
                         {"ARCHPART", 11, 0},
                 }
         },
         {
                 "BRBINF0_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "0000", "000",
                 0x8c00,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF1_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "0000", "100",
                 0x8c04,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF2_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "0001", "000",
                 0x8c08,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF3_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "0001", "100",
                 0x8c0c,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF4_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "0010", "000",
                 0x8c10,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF5_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "0010", "100",
                 0x8c14,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF6_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "0011", "000",
                 0x8c18,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF7_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "0011", "100",
                 0x8c1c,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF8_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "0100", "000",
                 0x8c20,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF9_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "0100", "100",
                 0x8c24,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF10_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "0101", "000",
                 0x8c28,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF11_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "0101", "100",
                 0x8c2c,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF12_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "0110", "000",
                 0x8c30,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF13_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "0110", "100",
                 0x8c34,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF14_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "0111", "000",
                 0x8c38,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF15_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "0111", "100",
                 0x8c3c,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF16_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "1000", "000",
                 0x8c40,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF17_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "1000", "100",
                 0x8c44,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF18_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "1001", "000",
                 0x8c48,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF19_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "1001", "100",
                 0x8c4c,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF20_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "1010", "000",
                 0x8c50,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF21_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "1010", "100",
                 0x8c54,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF22_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "1011", "000",
                 0x8c58,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF23_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "1011", "100",
                 0x8c5c,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF24_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "1100", "000",
                 0x8c60,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF25_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "1100", "100",
                 0x8c64,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF26_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "1101", "000",
                 0x8c68,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF27_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "1101", "100",
                 0x8c6c,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF28_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "1110", "000",
                 0x8c70,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF29_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "1110", "100",
                 0x8c74,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF30_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "1111", "000",
                 0x8c78,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBINF31_EL1",
                 "Branch Record Buffer Information Register <n>",
                 "10", "001", "1000", "1111", "100",
                 0x8c7c,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBSRC0_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "0000", "001",
                 0x8c01,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC1_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "0000", "101",
                 0x8c05,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC2_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "0001", "001",
                 0x8c09,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC3_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "0001", "101",
                 0x8c0d,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC4_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "0010", "001",
                 0x8c11,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC5_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "0010", "101",
                 0x8c15,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC6_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "0011", "001",
                 0x8c19,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC7_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "0011", "101",
                 0x8c1d,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC8_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "0100", "001",
                 0x8c21,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC9_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "0100", "101",
                 0x8c25,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC10_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "0101", "001",
                 0x8c29,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC11_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "0101", "101",
                 0x8c2d,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC12_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "0110", "001",
                 0x8c31,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC13_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "0110", "101",
                 0x8c35,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC14_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "0111", "001",
                 0x8c39,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC15_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "0111", "101",
                 0x8c3d,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC16_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "1000", "001",
                 0x8c41,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC17_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "1000", "101",
                 0x8c45,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC18_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "1001", "001",
                 0x8c49,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC19_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "1001", "101",
                 0x8c4d,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC20_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "1010", "001",
                 0x8c51,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC21_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "1010", "101",
                 0x8c55,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC22_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "1011", "001",
                 0x8c59,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC23_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "1011", "101",
                 0x8c5d,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC24_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "1100", "001",
                 0x8c61,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC25_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "1100", "101",
                 0x8c65,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC26_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "1101", "001",
                 0x8c69,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC27_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "1101", "101",
                 0x8c6d,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC28_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "1110", "001",
                 0x8c71,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC29_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "1110", "101",
                 0x8c75,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC30_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "1111", "001",
                 0x8c79,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBSRC31_EL1",
                 "Branch Record Buffer Source Address Register <n>",
                 "10", "001", "1000", "1111", "101",
                 0x8c7d,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT0_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "0000", "010",
                 0x8c02,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT1_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "0000", "110",
                 0x8c06,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT2_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "0001", "010",
                 0x8c0a,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT3_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "0001", "110",
                 0x8c0e,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT4_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "0010", "010",
                 0x8c12,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT5_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "0010", "110",
                 0x8c16,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT6_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "0011", "010",
                 0x8c1a,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT7_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "0011", "110",
                 0x8c1e,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT8_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "0100", "010",
                 0x8c22,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT9_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "0100", "110",
                 0x8c26,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT10_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "0101", "010",
                 0x8c2a,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT11_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "0101", "110",
                 0x8c2e,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT12_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "0110", "010",
                 0x8c32,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT13_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "0110", "110",
                 0x8c36,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT14_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "0111", "010",
                 0x8c3a,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT15_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "0111", "110",
                 0x8c3e,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT16_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "1000", "010",
                 0x8c42,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT17_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "1000", "110",
                 0x8c46,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT18_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "1001", "010",
                 0x8c4a,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT19_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "1001", "110",
                 0x8c4e,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT20_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "1010", "010",
                 0x8c52,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT21_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "1010", "110",
                 0x8c56,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT22_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "1011", "010",
                 0x8c5a,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT23_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "1011", "110",
                 0x8c5e,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT24_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "1100", "010",
                 0x8c62,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT25_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "1100", "110",
                 0x8c66,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT26_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "1101", "010",
                 0x8c6a,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT27_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "1101", "110",
                 0x8c6e,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT28_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "1110", "010",
                 0x8c72,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT29_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "1110", "110",
                 0x8c76,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT30_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "1111", "010",
                 0x8c7a,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGT31_EL1",
                 "Branch Record Buffer Target Address Register <n>",
                 "10", "001", "1000", "1111", "110",
                 0x8c7e,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBCR_EL1",
                 "Branch Record Buffer Control Register (EL1)",
                 "10", "001", "1001", "0000", "000",
                 0x8c80,
                 {
                         {"EXCEPTION", 23, 23},
                         {"ERTN", 22, 22},
                         {"FZP", 8, 8},
                         {"TS", 6, 5},
                         {"MPRED", 4, 4},
                         {"CC", 3, 3},
                         {"E1BRE", 1, 1},
                         {"E0BRE", 0, 0},
                 }
         },
         {
                 "BRBFCR_EL1",
                 "Branch Record Buffer Function Control Register",
                 "10", "001", "1001", "0000", "001",
                 0x8c81,
                 {
                         {"BANK", 29, 28},
                         {"CONDDIR", 22, 22},
                         {"DIRCALL", 21, 21},
                         {"INDCALL", 20, 20},
                         {"RTN", 19, 19},
                         {"INDIRECT", 18, 18},
                         {"DIRECT", 17, 17},
                         {"EnI", 16, 16},
                         {"PAUSED", 7, 7},
                         {"LASTFAILED", 6, 6},
                 }
         },
         {
                 "BRBTS_EL1",
                 "Branch Record Buffer Timestamp Register",
                 "10", "001", "1001", "0000", "010",
                 0x8c82,
                 {
                         {"TS", 63, 0},
                 }
         },
         {
                 "BRBINFINJ_EL1",
                 "Branch Record Buffer Information Injection Register",
                 "10", "001", "1001", "0001", "000",
                 0x8c88,
                 {
                         {"CCU", 46, 46},
                         {"CC", 45, 32},
                         {"LASTFAILED", 17, 17},
                         {"T", 16, 16},
                         {"TYPE", 13, 8},
                         {"EL", 7, 6},
                         {"MPRED", 5, 5},
                         {"VALID", 1, 0},
                 }
         },
         {
                 "BRBSRCINJ_EL1",
                 "Branch Record Buffer Source Address Injection Register",
                 "10", "001", "1001", "0001", "001",
                 0x8c89,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBTGTINJ_EL1",
                 "Branch Record Buffer Target Address Injection Register",
                 "10", "001", "1001", "0001", "010",
                 0x8c8a,
                 {
                         {"ADDRESS", 63, 0},
                 }
         },
         {
                 "BRBIDR0_EL1",
                 "Branch Record Buffer ID0 Register",
                 "10", "001", "1001", "0010", "000",
                 0x8c90,
                 {
                         {"CC", 15, 12},
                         {"FORMAT", 11, 8},
                         {"NUMREC", 7, 0},
                 }
         },
         {
                 "MDCCSR_EL0",
                 "Monitor DCC Status Register",
                 "10", "011", "0000", "0001", "000",
                 0x9808,
                 {
                         {"RXfull", 30, 30},
                         {"TXfull", 29, 29},
                         {"None", 18, 15},
                         {"None", 12, 12},
                         {"None", 5, 2},
                 }
         },
         {
                 "DBGDTR_EL0",
                 "Debug Data Transfer Register, half-duplex",
                 "10", "011", "0000", "0100", "000",
                 0x9820,
                 {
                         {"HighWord", 63, 32},
                         {"LowWord", 31, 0},
                 }
         },
         {
                 "DBGDTRRX_EL0",
                 "Debug Data Transfer Register, Receive",
                 "10", "011", "0000", "0101", "000",
                 0x9828,
                 {
                         {"None", 31, 0},
                 }
         },
         {
                 "DBGDTRTX_EL0",
                 "Debug Data Transfer Register, Transmit",
                 "10", "011", "0000", "0101", "000",
                 0x9828,
                 {
                         {"None", 31, 0},
                 }
         },
         {
                 "DBGVCR32_EL2",
                 "Debug Vector Catch Register",
                 "10", "100", "0000", "0111", "000",
                 0xa038,
                 {
                         {"NSF", 31, 31},
                         {"NSI", 30, 30},
                         {"NSD", 28, 28},
                         {"NSP", 27, 27},
                         {"NSS", 26, 26},
                         {"NSU", 25, 25},
                         {"SF", 7, 7},
                         {"SI", 6, 6},
                         {"SD", 4, 4},
                         {"SP", 3, 3},
                         {"SS", 2, 2},
                         {"SU", 1, 1},
                 }
         },
         {
                 "BRBCR_EL2",
                 "Branch Record Buffer Control Register (EL2)",
                 "10", "100", "1001", "0000", "000",
                 0xa480,
                 {
                         {"EXCEPTION", 23, 23},
                         {"ERTN", 22, 22},
                         {"FZP", 8, 8},
                         {"TS", 6, 5},
                         {"MPRED", 4, 4},
                         {"CC", 3, 3},
                         {"E2BRE", 1, 1},
                         {"E0HBRE", 0, 0},
                 }
         },
         {
                 "MIDR_EL1",
                 "Main ID Register",
                 "11", "000", "0000", "0000", "000",
                 0xc000,
                 {
                         {"Implementer", 31, 24},
                         {"Variant", 23, 20},
                         {"Architecture", 19, 16},
                         {"PartNum", 15, 4},
                         {"Revision", 3, 0},
                 }
         },
         {
                 "MPIDR_EL1",
                 "Multiprocessor Affinity Register",
                 "11", "000", "0000", "0000", "101",
                 0xc005,
                 {
                         {"Aff3", 39, 32},
                         {"U", 30, 30},
                         {"MT", 24, 24},
                         {"Aff2", 23, 16},
                         {"Aff1", 15, 8},
                         {"Aff0", 7, 0},
                 }
         },
         {
                 "REVIDR_EL1",
                 "Revision ID Register",
                 "11", "000", "0000", "0000", "110",
                 0xc006,
                 {
                         {"IMPLEMENTATION DEFINED", 63, 0},
                 }
         },
         {
                 "ID_PFR0_EL1",
                 "AArch32 Processor Feature Register 0",
                 "11", "000", "0000", "0001", "000",
                 0xc008,
                 {
                         {"RAS", 31, 28},
                         {"DIT", 27, 24},
                         {"AMU", 23, 20},
                         {"CSV2", 19, 16},
                         {"State3", 15, 12},
                         {"State2", 11, 8},
                         {"State1", 7, 4},
                         {"State0", 3, 0},
                 }
         },
         {
                 "ID_PFR1_EL1",
                 "AArch32 Processor Feature Register 1",
                 "11", "000", "0000", "0001", "001",
                 0xc009,
                 {
                         {"GIC", 31, 28},
                         {"Virt_frac", 27, 24},
                         {"Sec_frac", 23, 20},
                         {"GenTimer", 19, 16},
                         {"Virtualization", 15, 12},
                         {"MProgMod", 11, 8},
                         {"Security", 7, 4},
                         {"ProgMod", 3, 0},
                 }
         },
         {
                 "ID_DFR0_EL1",
                 "AArch32 Debug Feature Register 0",
                 "11", "000", "0000", "0001", "010",
                 0xc00a,
                 {
                         {"TraceFilt", 31, 28},
                         {"PerfMon", 27, 24},
                         {"MProfDbg", 23, 20},
                         {"MMapTrc", 19, 16},
                         {"CopTrc", 15, 12},
                         {"MMapDbg", 11, 8},
                         {"CopSDbg", 7, 4},
                         {"CopDbg", 3, 0},
                 }
         },
         {
                 "ID_AFR0_EL1",
                 "AArch32 Auxiliary Feature Register 0",
                 "11", "000", "0000", "0001", "011",
                 0xc00b,
                 {
                         {"IMPLEMENTATION DEFINED", 15, 12},
                         {"IMPLEMENTATION DEFINED", 11, 8},
                         {"IMPLEMENTATION DEFINED", 7, 4},
                         {"IMPLEMENTATION DEFINED", 3, 0},
                 }
         },
         {
                 "ID_MMFR0_EL1",
                 "AArch32 Memory Model Feature Register 0",
                 "11", "000", "0000", "0001", "100",
                 0xc00c,
                 {
                         {"InnerShr", 31, 28},
                         {"FCSE", 27, 24},
                         {"AuxReg", 23, 20},
                         {"TCM", 19, 16},
                         {"ShareLvl", 15, 12},
                         {"OuterShr", 11, 8},
                         {"PMSA", 7, 4},
                         {"VMSA", 3, 0},
                 }
         },
         {
                 "ID_MMFR1_EL1",
                 "AArch32 Memory Model Feature Register 1",
                 "11", "000", "0000", "0001", "101",
                 0xc00d,
                 {
                         {"BPred", 31, 28},
                         {"L1TstCln", 27, 24},
                         {"L1Uni", 23, 20},
                         {"L1Hvd", 19, 16},
                         {"L1UniSW", 15, 12},
                         {"L1HvdSW", 11, 8},
                         {"L1UniVA", 7, 4},
                         {"L1HvdVA", 3, 0},
                 }
         },
         {
                 "ID_MMFR2_EL1",
                 "AArch32 Memory Model Feature Register 2",
                 "11", "000", "0000", "0001", "110",
                 0xc00e,
                 {
                         {"HWAccFlg", 31, 28},
                         {"WFIStall", 27, 24},
                         {"MemBarr", 23, 20},
                         {"UniTLB", 19, 16},
                         {"HvdTLB", 15, 12},
                         {"L1HvdRng", 11, 8},
                         {"L1HvdBG", 7, 4},
                         {"L1HvdFG", 3, 0},
                 }
         },
         {
                 "ID_MMFR3_EL1",
                 "AArch32 Memory Model Feature Register 3",
                 "11", "000", "0000", "0001", "111",
                 0xc00f,
                 {
                         {"Supersec", 31, 28},
                         {"CMemSz", 27, 24},
                         {"CohWalk", 23, 20},
                         {"PAN", 19, 16},
                         {"MaintBcst", 15, 12},
                         {"BPMaint", 11, 8},
                         {"CMaintSW", 7, 4},
                         {"CMaintVA", 3, 0},
                 }
         },
         {
                 "ID_ISAR0_EL1",
                 "AArch32 Instruction Set Attribute Register 0",
                 "11", "000", "0000", "0010", "000",
                 0xc010,
                 {
                         {"Divide", 27, 24},
                         {"Debug", 23, 20},
                         {"Coproc", 19, 16},
                         {"CmpBranch", 15, 12},
                         {"BitField", 11, 8},
                         {"BitCount", 7, 4},
                         {"Swap", 3, 0},
                 }
         },
         {
                 "ID_ISAR1_EL1",
                 "AArch32 Instruction Set Attribute Register 1",
                 "11", "000", "0000", "0010", "001",
                 0xc011,
                 {
                         {"Jazelle", 31, 28},
                         {"Interwork", 27, 24},
                         {"Immediate", 23, 20},
                         {"IfThen", 19, 16},
                         {"Extend", 15, 12},
                         {"Except_AR", 11, 8},
                         {"Except", 7, 4},
                         {"Endian", 3, 0},
                 }
         },
         {
                 "ID_ISAR2_EL1",
                 "AArch32 Instruction Set Attribute Register 2",
                 "11", "000", "0000", "0010", "010",
                 0xc012,
                 {
                         {"Reversal", 31, 28},
                         {"PSR_AR", 27, 24},
                         {"MultU", 23, 20},
                         {"MultS", 19, 16},
                         {"Mult", 15, 12},
                         {"MultiAccessInt", 11, 8},
                         {"MemHint", 7, 4},
                         {"LoadStore", 3, 0},
                 }
         },
         {
                 "ID_ISAR3_EL1",
                 "AArch32 Instruction Set Attribute Register 3",
                 "11", "000", "0000", "0010", "011",
                 0xc013,
                 {
                         {"T32EE", 31, 28},
                         {"TrueNOP", 27, 24},
                         {"T32Copy", 23, 20},
                         {"TabBranch", 19, 16},
                         {"SynchPrim", 15, 12},
                         {"SVC", 11, 8},
                         {"SIMD", 7, 4},
                         {"Saturate", 3, 0},
                 }
         },
         {
                 "ID_ISAR4_EL1",
                 "AArch32 Instruction Set Attribute Register 4",
                 "11", "000", "0000", "0010", "100",
                 0xc014,
                 {
                         {"SWP_frac", 31, 28},
                         {"PSR_M", 27, 24},
                         {"SynchPrim_frac", 23, 20},
                         {"Barrier", 19, 16},
                         {"SMC", 15, 12},
                         {"Writeback", 11, 8},
                         {"WithShifts", 7, 4},
                         {"Unpriv", 3, 0},
                 }
         },
         {
                 "ID_ISAR5_EL1",
                 "AArch32 Instruction Set Attribute Register 5",
                 "11", "000", "0000", "0010", "101",
                 0xc015,
                 {
                         {"VCMA", 31, 28},
                         {"RDM", 27, 24},
                         {"CRC32", 19, 16},
                         {"SHA2", 15, 12},
                         {"SHA1", 11, 8},
                         {"AES", 7, 4},
                         {"SEVL", 3, 0},
                 }
         },
         {
                 "ID_MMFR4_EL1",
                 "AArch32 Memory Model Feature Register 4",
                 "11", "000", "0000", "0010", "110",
                 0xc016,
                 {
                         {"EVT", 31, 28},
                         {"CCIDX", 27, 24},
                         {"LSM", 23, 20},
                         {"HPDS", 19, 16},
                         {"CnP", 15, 12},
                         {"XNX", 11, 8},
                         {"AC2", 7, 4},
                         {"SpecSEI", 3, 0},
                 }
         },
         {
                 "ID_ISAR6_EL1",
                 "AArch32 Instruction Set Attribute Register 6",
                 "11", "000", "0000", "0010", "111",
                 0xc017,
                 {
                         {"I8MM", 27, 24},
                         {"BF16", 23, 20},
                         {"SPECRES", 19, 16},
                         {"SB", 15, 12},
                         {"FHM", 11, 8},
                         {"DP", 7, 4},
                         {"JSCVT", 3, 0},
                 }
         },
         {
                 "MVFR0_EL1",
                 "AArch32 Media and VFP Feature Register 0",
                 "11", "000", "0000", "0011", "000",
                 0xc018,
                 {
                         {"FPRound", 31, 28},
                         {"FPShVec", 27, 24},
                         {"FPSqrt", 23, 20},
                         {"FPDivide", 19, 16},
                         {"FPTrap", 15, 12},
                         {"FPDP", 11, 8},
                         {"FPSP", 7, 4},
                         {"SIMDReg", 3, 0},
                 }
         },
         {
                 "MVFR1_EL1",
                 "AArch32 Media and VFP Feature Register 1",
                 "11", "000", "0000", "0011", "001",
                 0xc019,
                 {
                         {"SIMDFMAC", 31, 28},
                         {"FPHP", 27, 24},
                         {"SIMDHP", 23, 20},
                         {"SIMDSP", 19, 16},
                         {"SIMDInt", 15, 12},
                         {"SIMDLS", 11, 8},
                         {"FPDNaN", 7, 4},
                         {"FPFtZ", 3, 0},
                 }
         },
         {
                 "MVFR2_EL1",
                 "AArch32 Media and VFP Feature Register 2",
                 "11", "000", "0000", "0011", "010",
                 0xc01a,
                 {
                         {"FPMisc", 7, 4},
                         {"SIMDMisc", 3, 0},
                 }
         },
         {
                 "ID_PFR2_EL1",
                 "AArch32 Processor Feature Register 2",
                 "11", "000", "0000", "0011", "100",
                 0xc01c,
                 {
                         {"RAS_frac", 11, 8},
                         {"SSBS", 7, 4},
                         {"CSV3", 3, 0},
                 }
         },
         {
                 "ID_DFR1_EL1",
                 "Debug Feature Register 1",
                 "11", "000", "0000", "0011", "101",
                 0xc01d,
                 {
                         {"MTPMU", 3, 0},
                 }
         },
         {
                 "ID_MMFR5_EL1",
                 "AArch32 Memory Model Feature Register 5",
                 "11", "000", "0000", "0011", "110",
                 0xc01e,
                 {
                         {"nTLBPA", 7, 4},
                         {"ETS", 3, 0},
                 }
         },
         {
                 "ID_AA64PFR0_EL1",
                 "AArch64 Processor Feature Register 0",
                 "11", "000", "0000", "0100", "000",
                 0xc020,
                 {
                         {"CSV3", 63, 60},
                         {"CSV2", 59, 56},
                         {"RME", 55, 52},
                         {"DIT", 51, 48},
                         {"AMU", 47, 44},
                         {"MPAM", 43, 40},
                         {"SEL2", 39, 36},
                         {"SVE", 35, 32},
                         {"RAS", 31, 28},
                         {"GIC", 27, 24},
                         {"AdvSIMD", 23, 20},
                         {"FP", 19, 16},
                         {"EL3", 15, 12},
                         {"EL2", 11, 8},
                         {"EL1", 7, 4},
                         {"EL0", 3, 0},
                 }
         },
         {
                 "ID_AA64PFR1_EL1",
                 "AArch64 Processor Feature Register 1",
                 "11", "000", "0000", "0100", "001",
                 0xc021,
                 {
                         {"CSV2_frac", 35, 32},
                         {"SME", 27, 24},
                         {"MPAM_frac", 19, 16},
                         {"RAS_frac", 15, 12},
                         {"MTE", 11, 8},
                         {"SSBS", 7, 4},
                         {"BT", 3, 0},
                 }
         },
         {
                 "ID_AA64ZFR0_EL1",
                 "SVE Feature ID register 0",
                 "11", "000", "0000", "0100", "100",
                 0xc024,
                 {
                         {"F64MM", 59, 56},
                         {"F32MM", 55, 52},
                         {"I8MM", 47, 44},
                         {"SM4", 43, 40},
                         {"SHA3", 35, 32},
                         {"BF16", 23, 20},
                         {"BitPerm", 19, 16},
                         {"AES", 7, 4},
                         {"SVEver", 3, 0},
                 }
         },
         {
                 "ID_AA64SMFR0_EL1",
                 "SME Feature ID register 0",
                 "11", "000", "0000", "0100", "101",
                 0xc025,
                 {
                         {"I16I64", 55, 52},
                         {"F64F64", 48, 48},
                         {"I8I32", 39, 36},
                         {"F16F32", 35, 35},
                         {"B16F32", 34, 34},
                         {"F32F32", 32, 32},
                 }
         },
         {
                 "ID_AA64DFR0_EL1",
                 "AArch64 Debug Feature Register 0",
                 "11", "000", "0000", "0101", "000",
                 0xc028,
                 {
                         {"BRBE", 55, 52},
                         {"MTPMU", 51, 48},
                         {"TraceBuffer", 47, 44},
                         {"TraceFilt", 43, 40},
                         {"DoubleLock", 39, 36},
                         {"PMSVer", 35, 32},
                         {"CTX_CMPs", 31, 28},
                         {"WRPs", 23, 20},
                         {"BRPs", 15, 12},
                         {"PMUVer", 11, 8},
                         {"TraceVer", 7, 4},
                         {"DebugVer", 3, 0},
                 }
         },
         {
                 "ID_AA64DFR1_EL1",
                 "AArch64 Debug Feature Register 1",
                 "11", "000", "0000", "0101", "001",
                 0xc029,
                 {
                 }
         },
         {
                 "ID_AA64AFR0_EL1",
                 "AArch64 Auxiliary Feature Register 0",
                 "11", "000", "0000", "0101", "100",
                 0xc02c,
                 {
                         {"IMPLEMENTATION DEFINED", 31, 28},
                         {"IMPLEMENTATION DEFINED", 27, 24},
                         {"IMPLEMENTATION DEFINED", 23, 20},
                         {"IMPLEMENTATION DEFINED", 19, 16},
                         {"IMPLEMENTATION DEFINED", 15, 12},
                         {"IMPLEMENTATION DEFINED", 11, 8},
                         {"IMPLEMENTATION DEFINED", 7, 4},
                         {"IMPLEMENTATION DEFINED", 3, 0},
                 }
         },
         {
                 "ID_AA64AFR1_EL1",
                 "AArch64 Auxiliary Feature Register 1",
                 "11", "000", "0000", "0101", "101",
                 0xc02d,
                 {
                 }
         },
         {
                 "ID_AA64ISAR0_EL1",
                 "AArch64 Instruction Set Attribute Register 0",
                 "11", "000", "0000", "0110", "000",
                 0xc030,
                 {
                         {"RNDR", 63, 60},
                         {"TLB", 59, 56},
                         {"TS", 55, 52},
                         {"FHM", 51, 48},
                         {"DP", 47, 44},
                         {"SM4", 43, 40},
                         {"SM3", 39, 36},
                         {"SHA3", 35, 32},
                         {"RDM", 31, 28},
                         {"TME", 27, 24},
                         {"Atomic", 23, 20},
                         {"CRC32", 19, 16},
                         {"SHA2", 15, 12},
                         {"SHA1", 11, 8},
                         {"AES", 7, 4},
                 }
         },
         {
                 "ID_AA64ISAR1_EL1",
                 "AArch64 Instruction Set Attribute Register 1",
                 "11", "000", "0000", "0110", "001",
                 0xc031,
                 {
                         {"LS64", 63, 60},
                         {"XS", 59, 56},
                         {"I8MM", 55, 52},
                         {"DGH", 51, 48},
                         {"BF16", 47, 44},
                         {"SPECRES", 43, 40},
                         {"SB", 39, 36},
                         {"FRINTTS", 35, 32},
                         {"GPI", 31, 28},
                         {"GPA", 27, 24},
                         {"LRCPC", 23, 20},
                         {"FCMA", 19, 16},
                         {"JSCVT", 15, 12},
                         {"API", 11, 8},
                         {"APA", 7, 4},
                         {"DPB", 3, 0},
                 }
         },
         {
                 "ID_AA64ISAR2_EL1",
                 "AArch64 Instruction Set Attribute Register 2",
                 "11", "000", "0000", "0110", "010",
                 0xc032,
                 {
                         {"RPRES", 7, 4},
                         {"WFxT", 3, 0},
                 }
         },
         {
                 "ID_AA64MMFR0_EL1",
                 "AArch64 Memory Model Feature Register 0",
                 "11", "000", "0000", "0111", "000",
                 0xc038,
                 {
                         {"ECV", 63, 60},
                         {"FGT", 59, 56},
                         {"ExS", 47, 44},
                         {"TGran4_2", 43, 40},
                         {"TGran64_2", 39, 36},
                         {"TGran16_2", 35, 32},
                         {"TGran4", 31, 28},
                         {"TGran64", 27, 24},
                         {"TGran16", 23, 20},
                         {"BigEndEL0", 19, 16},
                         {"SNSMem", 15, 12},
                         {"BigEnd", 11, 8},
                         {"ASIDBits", 7, 4},
                         {"PARange", 3, 0},
                 }
         },
         {
                 "ID_AA64MMFR1_EL1",
                 "AArch64 Memory Model Feature Register 1",
                 "11", "000", "0000", "0111", "001",
                 0xc039,
                 {
                         {"nTLBPA", 51, 48},
                         {"AFP", 47, 44},
                         {"HCX", 43, 40},
                         {"ETS", 39, 36},
                         {"TWED", 35, 32},
                         {"XNX", 31, 28},
                         {"SpecSEI", 27, 24},
                         {"PAN", 23, 20},
                         {"LO", 19, 16},
                         {"HPDS", 15, 12},
                         {"VH", 11, 8},
                         {"VMIDBits", 7, 4},
                         {"HAFDBS", 3, 0},
                 }
         },
         {
                 "ID_AA64MMFR2_EL1",
                 "AArch64 Memory Model Feature Register 2",
                 "11", "000", "0000", "0111", "010",
                 0xc03a,
                 {
                         {"E0PD", 63, 60},
                         {"EVT", 59, 56},
                         {"BBM", 55, 52},
                         {"TTL", 51, 48},
                         {"FWB", 43, 40},
                         {"IDS", 39, 36},
                         {"AT", 35, 32},
                         {"ST", 31, 28},
                         {"NV", 27, 24},
                         {"CCIDX", 23, 20},
                         {"VARange", 19, 16},
                         {"IESB", 15, 12},
                         {"LSM", 11, 8},
                         {"UAO", 7, 4},
                         {"CnP", 3, 0},
                 }
         },
         {
                 "SCTLR_EL1",
                 "System Control Register (EL1)",
                 "11", "000", "0001", "0000", "000",
                 0xc080,
                 {
                         {"EnTP2", 60, 60},
                         {"EPAN", 57, 57},
                         {"EnALS", 56, 56},
                         {"EnAS0", 55, 55},
                         {"EnASR", 54, 54},
                         {"TME", 53, 53},
                         {"TME0", 52, 52},
                         {"TMT", 51, 51},
                         {"TMT0", 50, 50},
                         {"TWEDEL", 49, 46},
                         {"TWEDEn", 45, 45},
                         {"DSSBS", 44, 44},
                         {"ATA", 43, 43},
                         {"ATA0", 42, 42},
                         {"TCF", 41, 40},
                         {"TCF0", 39, 38},
                         {"ITFSB", 37, 37},
                         {"BT1", 36, 36},
                         {"BT0", 35, 35},
                         {"EnIA", 31, 31},
                         {"EnIB", 30, 30},
                         {"LSMAOE", 29, 29},
                         {"nTLSMD", 28, 28},
                         {"EnDA", 27, 27},
                         {"UCI", 26, 26},
                         {"EE", 25, 25},
                         {"E0E", 24, 24},
                         {"SPAN", 23, 23},
                         {"EIS", 22, 22},
                         {"IESB", 21, 21},
                         {"TSCXT", 20, 20},
                         {"WXN", 19, 19},
                         {"nTWE", 18, 18},
                         {"nTWI", 16, 16},
                         {"UCT", 15, 15},
                         {"DZE", 14, 14},
                         {"EnDB", 13, 13},
                         {"I", 12, 12},
                         {"EOS", 11, 11},
                         {"EnRCTX", 10, 10},
                         {"UMA", 9, 9},
                         {"SED", 8, 8},
                         {"ITD", 7, 7},
                         {"nAA", 6, 6},
                         {"CP15BEN", 5, 5},
                         {"SA0", 4, 4},
                         {"SA", 3, 3},
                         {"C", 2, 2},
                         {"A", 1, 1},
                         {"M", 0, 0},
                 }
         },
         {
                 "ACTLR_EL1",
                 "Auxiliary Control Register (EL1)",
                 "11", "000", "0001", "0000", "001",
                 0xc081,
                 {
                         {"IMPLEMENTATION DEFINED", 63, 0},
                 }
         },
         {
                 "CPACR_EL1",
                 "Architectural Feature Access Control Register",
                 "11", "000", "0001", "0000", "010",
                 0xc082,
                 {
                         {"TTA", 28, 28},
                         {"SMEN", 25, 24},
                         {"FPEN", 21, 20},
                         {"ZEN", 17, 16},
                 }
         },
         {
                 "RGSR_EL1",
                 "Random Allocation Tag Seed Register.",
                 "11", "000", "0001", "0000", "101",
                 0xc085,
                 {
                         {"SEED", 23, 8},
                         {"TAG", 3, 0},
                 }
         },
         {
                 "GCR_EL1",
                 "Tag Control Register.",
                 "11", "000", "0001", "0000", "110",
                 0xc086,
                 {
                         {"RRND", 16, 16},
                         {"Exclude", 15, 0},
                 }
         },
         {
                 "ZCR_EL1",
                 "SVE Control Register (EL1)",
                 "11", "000", "0001", "0010", "000",
                 0xc090,
                 {
                         {"None", 8, 4},
                         {"LEN", 3, 0},
                 }
         },
         {
                 "TRFCR_EL1",
                 "Trace Filter Control Register (EL1)",
                 "11", "000", "0001", "0010", "001",
                 0xc091,
                 {
                         {"TS", 6, 5},
                         {"E1TRE", 1, 1},
                         {"E0TRE", 0, 0},
                 }
         },
         {
                 "SMPRI_EL1",
                 "Streaming Mode Priority Register",
                 "11", "000", "0001", "0010", "100",
                 0xc094,
                 {
                         {"Priority", 3, 0},
                 }
         },
         {
                 "SMCR_EL1",
                 "SME Control Register (EL1)",
                 "11", "000", "0001", "0010", "110",
                 0xc096,
                 {
                         {"None", 8, 4},
                         {"LEN", 3, 0},
                 }
         },
         {
                 "TTBR0_EL1",
                 "Translation Table Base Register 0 (EL1)",
                 "11", "000", "0010", "0000", "000",
                 0xc100,
                 {
                         {"ASID", 63, 48},
                         {"BADDR[47:1]", 47, 1},
                         {"CnP", 0, 0},
                 }
         },
         {
                 "TTBR1_EL1",
                 "Translation Table Base Register 1 (EL1)",
                 "11", "000", "0010", "0000", "001",
                 0xc101,
                 {
                         {"ASID", 63, 48},
                         {"BADDR[47:1]", 47, 1},
                         {"CnP", 0, 0},
                 }
         },
         {
                 "TCR_EL1",
                 "Translation Control Register (EL1)",
                 "11", "000", "0010", "0000", "010",
                 0xc102,
                 {
                         {"DS", 59, 59},
                         {"TCMA1", 58, 58},
                         {"TCMA0", 57, 57},
                         {"E0PD1", 56, 56},
                         {"E0PD0", 55, 55},
                         {"NFD1", 54, 54},
                         {"NFD0", 53, 53},
                         {"TBID1", 52, 52},
                         {"TBID0", 51, 51},
                         {"HWU162", 50, 50},
                         {"None", 50, 50},
                         {"HWU161", 49, 49},
                         {"None", 49, 49},
                         {"HWU160", 48, 48},
                         {"None", 48, 48},
                         {"HWU159", 47, 47},
                         {"None", 47, 47},
                         {"HWU062", 46, 46},
                         {"None", 46, 46},
                         {"HWU061", 45, 45},
                         {"None", 45, 45},
                         {"HWU060", 44, 44},
                         {"None", 44, 44},
                         {"HWU059", 43, 43},
                         {"None", 43, 43},
                         {"HPD1", 42, 42},
                         {"HPD0", 41, 41},
                         {"HD", 40, 40},
                         {"HA", 39, 39},
                         {"TBI1", 38, 38},
                         {"TBI0", 37, 37},
                         {"AS", 36, 36},
                         {"IPS", 34, 32},
                         {"TG1", 31, 30},
                         {"SH1", 29, 28},
                         {"ORGN1", 27, 26},
                         {"IRGN1", 25, 24},
                         {"EPD1", 23, 23},
                         {"A1", 22, 22},
                         {"T1SZ", 21, 16},
                         {"TG0", 15, 14},
                         {"SH0", 13, 12},
                         {"ORGN0", 11, 10},
                         {"IRGN0", 9, 8},
                         {"EPD0", 7, 7},
                         {"T0SZ", 5, 0},
                 }
         },
         {
                 "APIAKeyLo_EL1",
                 "Pointer Authentication Key A for Instruction (bits[63:0])",
                 "11", "000", "0010", "0001", "000",
                 0xc108,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "APIAKeyHi_EL1",
                 "Pointer Authentication Key A for Instruction (bits[127:64])",
                 "11", "000", "0010", "0001", "001",
                 0xc109,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "APIBKeyLo_EL1",
                 "Pointer Authentication Key B for Instruction (bits[63:0])",
                 "11", "000", "0010", "0001", "010",
                 0xc10a,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "APIBKeyHi_EL1",
                 "Pointer Authentication Key B for Instruction (bits[127:64])",
                 "11", "000", "0010", "0001", "011",
                 0xc10b,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "APDAKeyLo_EL1",
                 "Pointer Authentication Key A for Data (bits[63:0])",
                 "11", "000", "0010", "0010", "000",
                 0xc110,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "APDAKeyHi_EL1",
                 "Pointer Authentication Key A for Data (bits[127:64])",
                 "11", "000", "0010", "0010", "001",
                 0xc111,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "APDBKeyLo_EL1",
                 "Pointer Authentication Key B for Data (bits[63:0])",
                 "11", "000", "0010", "0010", "010",
                 0xc112,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "APDBKeyHi_EL1",
                 "Pointer Authentication Key B for Data (bits[127:64])",
                 "11", "000", "0010", "0010", "011",
                 0xc113,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "APGAKeyLo_EL1",
                 "Pointer Authentication Key A for Code (bits[63:0])",
                 "11", "000", "0010", "0011", "000",
                 0xc118,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "APGAKeyHi_EL1",
                 "Pointer Authentication Key A for Code (bits[127:64])",
                 "11", "000", "0010", "0011", "001",
                 0xc119,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "SPSR_EL1",
                 "Saved Program Status Register (EL1)",
                 "11", "000", "0100", "0000", "000",
                 0xc200,
                 {
                         {"N", 31, 31},
                         {"Z", 30, 30},
                         {"C", 29, 29},
                         {"V", 28, 28},
                         {"Q", 27, 27},
                         {"IT", 26, 25},
                         {"DIT", 24, 24},
                         {"SSBS", 23, 23},
                         {"PAN", 22, 22},
                         {"SS", 21, 21},
                         {"IL", 20, 20},
                         {"GE", 19, 16},
                         {"IT[7:2]", 15, 10},
                         {"E", 9, 9},
                         {"A", 8, 8},
                         {"I", 7, 7},
                         {"F", 6, 6},
                         {"T", 5, 5},
                         {"M[4]", 4, 4},
                         {"M[3:0]", 3, 0},
                 }
         },
         {
                 "ELR_EL1",
                 "Exception Link Register (EL1)",
                 "11", "000", "0100", "0000", "001",
                 0xc201,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "SP_EL0",
                 "Stack Pointer (EL0)",
                 "11", "000", "0100", "0001", "000",
                 0xc208,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "SPSel",
                 "Stack Pointer Select",
                 "11", "000", "0100", "0010", "000",
                 0xc210,
                 {
                         {"SP", 0, 0},
                 }
         },
         {
                 "SPSel",
                 "Stack Pointer Select",
                 "00", "000", "0100", "0000", "101",
                 0x205,
                 {
                         {"SP", 0, 0},
                 }
         },
         {
                 "CurrentEL",
                 "Current Exception Level",
                 "11", "000", "0100", "0010", "010",
                 0xc212,
                 {
                         {"EL", 3, 2},
                 }
         },
         {
                 "PAN",
                 "Privileged Access Never",
                 "11", "000", "0100", "0010", "011",
                 0xc213,
                 {
                         {"PAN", 22, 22},
                 }
         },
         {
                 "PAN",
                 "Privileged Access Never",
                 "00", "000", "0100", "0000", "100",
                 0x204,
                 {
                         {"PAN", 22, 22},
                 }
         },
         {
                 "UAO",
                 "User Access Override",
                 "11", "000", "0100", "0010", "100",
                 0xc214,
                 {
                         {"UAO", 23, 23},
                 }
         },
         {
                 "UAO",
                 "User Access Override",
                 "00", "000", "0100", "0000", "011",
                 0x203,
                 {
                         {"UAO", 23, 23},
                 }
         },
         {
                 "ICC_PMR_EL1",
                 "Interrupt Controller Interrupt Priority Mask Register",
                 "11", "000", "0100", "0110", "000",
                 0xc230,
                 {
                         {"Priority", 7, 0},
                 }
         },
         {
                 "AFSR0_EL1",
                 "Auxiliary Fault Status Register 0 (EL1)",
                 "11", "000", "0101", "0001", "000",
                 0xc288,
                 {
                         {"IMPLEMENTATION DEFINED", 63, 0},
                 }
         },
         {
                 "AFSR1_EL1",
                 "Auxiliary Fault Status Register 1 (EL1)",
                 "11", "000", "0101", "0001", "001",
                 0xc289,
                 {
                         {"IMPLEMENTATION DEFINED", 63, 0},
                 }
         },
         {
                 "ESR_EL1",
                 "Exception Syndrome Register (EL1)",
                 "11", "000", "0101", "0010", "000",
                 0xc290,
                 {
                         {"ISS2", 36, 32},
                         {"EC", 31, 26},
                         {"IL", 25, 25},
                         {"ISS", 24, 0},
                 }
         },
         {
                 "ERRIDR_EL1",
                 "Error Record ID Register",
                 "11", "000", "0101", "0011", "000",
                 0xc298,
                 {
                         {"NUM", 15, 0},
                 }
         },
         {
                 "ERRSELR_EL1",
                 "Error Record Select Register",
                 "11", "000", "0101", "0011", "001",
                 0xc299,
                 {
                         {"SEL", 15, 0},
                 }
         },
         {
                 "ERXFR_EL1",
                 "Selected Error Record Feature Register",
                 "11", "000", "0101", "0100", "000",
                 0xc2a0,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "ERXCTLR_EL1",
                 "Selected Error Record Control Register",
                 "11", "000", "0101", "0100", "001",
                 0xc2a1,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "ERXSTATUS_EL1",
                 "Selected Error Record Primary Status Register",
                 "11", "000", "0101", "0100", "010",
                 0xc2a2,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "ERXADDR_EL1",
                 "Selected Error Record Address Register",
                 "11", "000", "0101", "0100", "011",
                 0xc2a3,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "ERXPFGF_EL1",
                 "Selected Pseudo-fault Generation Feature register",
                 "11", "000", "0101", "0100", "100",
                 0xc2a4,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "ERXPFGCTL_EL1",
                 "Selected Pseudo-fault Generation Control register",
                 "11", "000", "0101", "0100", "101",
                 0xc2a5,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "ERXPFGCDN_EL1",
                 "Selected Pseudo-fault Generation Countdown register",
                 "11", "000", "0101", "0100", "110",
                 0xc2a6,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "ERXMISC0_EL1",
                 "Selected Error Record Miscellaneous Register 0",
                 "11", "000", "0101", "0101", "000",
                 0xc2a8,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "ERXMISC1_EL1",
                 "Selected Error Record Miscellaneous Register 1",
                 "11", "000", "0101", "0101", "001",
                 0xc2a9,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "ERXMISC2_EL1",
                 "Selected Error Record Miscellaneous Register 2",
                 "11", "000", "0101", "0101", "010",
                 0xc2aa,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "ERXMISC3_EL1",
                 "Selected Error Record Miscellaneous Register 3",
                 "11", "000", "0101", "0101", "011",
                 0xc2ab,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "TFSR_EL1",
                 "Tag Fault Status Register (EL1)",
                 "11", "000", "0101", "0110", "000",
                 0xc2b0,
                 {
                         {"TF1", 1, 1},
                         {"TF0", 0, 0},
                 }
         },
         {
                 "TFSRE0_EL1",
                 "Tag Fault Status Register (EL0).",
                 "11", "000", "0101", "0110", "001",
                 0xc2b1,
                 {
                         {"TF1", 1, 1},
                         {"TF0", 0, 0},
                 }
         },
         {
                 "FAR_EL1",
                 "Fault Address Register (EL1)",
                 "11", "000", "0110", "0000", "000",
                 0xc300,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PAR_EL1",
                 "Physical Address Register",
                 "11", "000", "0111", "0100", "000",
                 0xc3a0,
                 {
                         {"ATTR", 63, 56},
                         {"PA[51:48]", 51, 48},
                         {"PA[47:12]", 47, 12},
                         {"NSE", 11, 11},
                         {"IMPLEMENTATION DEFINED", 10, 10},
                         {"NS", 9, 9},
                         {"NS", 9, 9},
                         {"SH", 8, 7},
                         {"F", 0, 0},
                 }
         },
         {
                 "PMSCR_EL1",
                 "Statistical Profiling Control Register (EL1)",
                 "11", "000", "1001", "1001", "000",
                 0xc4c8,
                 {
                         {"PCT", 7, 6},
                         {"PCT", 7, 6},
                         {"TS", 5, 5},
                         {"PA", 4, 4},
                         {"CX", 3, 3},
                         {"E1SPE", 1, 1},
                         {"E0SPE", 0, 0},
                 }
         },
         {
                 "PMSNEVFR_EL1",
                 "Sampling Inverted Event Filter Register",
                 "11", "000", "1001", "1001", "001",
                 0xc4c9,
                 {
                         {"E[<x>]", 63, 48},
                         {"E[63]", 63, 63},
                         {"E[62]", 62, 62},
                         {"E[61]", 61, 61},
                         {"E[60]", 60, 60},
                         {"E[59]", 59, 59},
                         {"E[58]", 58, 58},
                         {"E[57]", 57, 57},
                         {"E[56]", 56, 56},
                         {"E[55]", 55, 55},
                         {"E[54]", 54, 54},
                         {"E[53]", 53, 53},
                         {"E[52]", 52, 52},
                         {"E[51]", 51, 51},
                         {"E[50]", 50, 50},
                         {"E[49]", 49, 49},
                         {"E[48]", 48, 48},
                         {"None", 47, 32},
                         {"E[31]", 31, 31},
                         {"E[30]", 30, 30},
                         {"E[29]", 29, 29},
                         {"E[28]", 28, 28},
                         {"E[27]", 27, 27},
                         {"E[26]", 26, 26},
                         {"E[25]", 25, 25},
                         {"E[24]", 24, 24},
                         {"None", 23, 19},
                         {"E[18]", 18, 18},
                         {"None", 18, 18},
                         {"E[17]", 17, 17},
                         {"None", 17, 17},
                         {"E[16]", 16, 16},
                         {"None", 16, 16},
                         {"E[15]", 15, 15},
                         {"E[14]", 14, 14},
                         {"E[13]", 13, 13},
                         {"E[12]", 12, 12},
                         {"E[11]", 11, 11},
                         {"None", 11, 11},
                         {"None", 10, 8},
                         {"E[7]", 7, 7},
                         {"E[6]", 6, 6},
                         {"E[5]", 5, 5},
                         {"None", 4, 4},
                         {"E[3]", 3, 3},
                         {"None", 2, 2},
                         {"E[1]", 1, 1},
                         {"None", 1, 1},
                         {"None", 0, 0},
                 }
         },
         {
                 "PMSICR_EL1",
                 "Sampling Interval Counter Register",
                 "11", "000", "1001", "1001", "010",
                 0xc4ca,
                 {
                         {"ECOUNT", 63, 56},
                         {"COUNT", 31, 0},
                 }
         },
         {
                 "PMSIRR_EL1",
                 "Sampling Interval Reload Register",
                 "11", "000", "1001", "1001", "011",
                 0xc4cb,
                 {
                         {"INTERVAL", 31, 8},
                         {"RND", 0, 0},
                 }
         },
         {
                 "PMSFCR_EL1",
                 "Sampling Filter Control Register",
                 "11", "000", "1001", "1001", "100",
                 0xc4cc,
                 {
                         {"ST", 18, 18},
                         {"LD", 17, 17},
                         {"B", 16, 16},
                         {"FnE", 3, 3},
                         {"FL", 2, 2},
                         {"FT", 1, 1},
                         {"FE", 0, 0},
                 }
         },
         {
                 "PMSEVFR_EL1",
                 "Sampling Event Filter Register",
                 "11", "000", "1001", "1001", "101",
                 0xc4cd,
                 {
                         {"E[<x>]", 63, 48},
                         {"E[63]", 63, 63},
                         {"E[62]", 62, 62},
                         {"E[61]", 61, 61},
                         {"E[60]", 60, 60},
                         {"E[59]", 59, 59},
                         {"E[58]", 58, 58},
                         {"E[57]", 57, 57},
                         {"E[56]", 56, 56},
                         {"E[55]", 55, 55},
                         {"E[54]", 54, 54},
                         {"E[53]", 53, 53},
                         {"E[52]", 52, 52},
                         {"E[51]", 51, 51},
                         {"E[50]", 50, 50},
                         {"E[49]", 49, 49},
                         {"E[48]", 48, 48},
                         {"None", 47, 32},
                         {"E[31]", 31, 31},
                         {"E[30]", 30, 30},
                         {"E[29]", 29, 29},
                         {"E[28]", 28, 28},
                         {"E[27]", 27, 27},
                         {"E[26]", 26, 26},
                         {"E[25]", 25, 25},
                         {"E[24]", 24, 24},
                         {"None", 23, 19},
                         {"E[18]", 18, 18},
                         {"None", 18, 18},
                         {"E[17]", 17, 17},
                         {"None", 17, 17},
                         {"E[16]", 16, 16},
                         {"None", 16, 16},
                         {"E[15]", 15, 15},
                         {"E[14]", 14, 14},
                         {"E[13]", 13, 13},
                         {"E[12]", 12, 12},
                         {"E[11]", 11, 11},
                         {"None", 11, 11},
                         {"None", 10, 8},
                         {"E[7]", 7, 7},
                         {"E[6]", 6, 6},
                         {"None", 6, 6},
                         {"E[5]", 5, 5},
                         {"None", 4, 4},
                         {"E[3]", 3, 3},
                         {"None", 2, 2},
                         {"E[1]", 1, 1},
                         {"None", 1, 1},
                         {"None", 0, 0},
                 }
         },
         {
                 "PMSLATFR_EL1",
                 "Sampling Latency Filter Register",
                 "11", "000", "1001", "1001", "110",
                 0xc4ce,
                 {
                         {"MINLAT", 11, 0},
                 }
         },
         {
                 "PMSIDR_EL1",
                 "Sampling Profiling ID Register",
                 "11", "000", "1001", "1001", "111",
                 0xc4cf,
                 {
                         {"PBT", 24, 24},
                         {"Format", 23, 20},
                         {"None", 23, 20},
                         {"CountSize", 19, 16},
                         {"MaxSize", 15, 12},
                         {"Interval", 11, 8},
                         {"FnE", 6, 6},
                         {"ERnd", 5, 5},
                         {"LDS", 4, 4},
                         {"ArchInst", 3, 3},
                         {"FL", 2, 2},
                         {"FT", 1, 1},
                         {"FE", 0, 0},
                 }
         },
         {
                 "PMBLIMITR_EL1",
                 "Profiling Buffer Limit Address Register",
                 "11", "000", "1001", "1010", "000",
                 0xc4d0,
                 {
                         {"LIMIT", 63, 12},
                         {"PMFZ", 5, 5},
                         {"FM", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "PMBPTR_EL1",
                 "Profiling Buffer Write Pointer Register",
                 "11", "000", "1001", "1010", "001",
                 0xc4d1,
                 {
                         {"PTR", 63, 0},
                 }
         },
         {
                 "PMBSR_EL1",
                 "Profiling Buffer Status/syndrome Register",
                 "11", "000", "1001", "1010", "011",
                 0xc4d3,
                 {
                         {"EC", 31, 26},
                         {"DL", 19, 19},
                         {"EA", 18, 18},
                         {"S", 17, 17},
                         {"COLL", 16, 16},
                         {"MSS", 15, 0},
                 }
         },
         {
                 "PMBIDR_EL1",
                 "Profiling Buffer ID Register",
                 "11", "000", "1001", "1010", "111",
                 0xc4d7,
                 {
                         {"F", 5, 5},
                         {"P", 4, 4},
                         {"Align", 3, 0},
                 }
         },
         {
                 "TRBLIMITR_EL1",
                 "Trace Buffer Limit Address Register",
                 "11", "000", "1001", "1011", "000",
                 0xc4d8,
                 {
                         {"LIMIT", 63, 12},
                         {"nVM", 5, 5},
                         {"TM", 4, 3},
                         {"FM", 2, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "TRBPTR_EL1",
                 "Trace Buffer Write Pointer Register",
                 "11", "000", "1001", "1011", "001",
                 0xc4d9,
                 {
                         {"PTR", 63, 0},
                 }
         },
         {
                 "TRBBASER_EL1",
                 "Trace Buffer Base Address Register",
                 "11", "000", "1001", "1011", "010",
                 0xc4da,
                 {
                         {"BASE", 63, 12},
                 }
         },
         {
                 "TRBSR_EL1",
                 "Trace Buffer Status/syndrome Register",
                 "11", "000", "1001", "1011", "011",
                 0xc4db,
                 {
                         {"EC", 31, 26},
                         {"IRQ", 22, 22},
                         {"TRG", 21, 21},
                         {"WRAP", 20, 20},
                         {"EA", 18, 18},
                         {"S", 17, 17},
                         {"MSS", 15, 0},
                 }
         },
         {
                 "TRBMAR_EL1",
                 "Trace Buffer Memory Attribute Register",
                 "11", "000", "1001", "1011", "100",
                 0xc4dc,
                 {
                         {"SH", 9, 8},
                         {"Attr[7:4]", 7, 4},
                         {"Attr[7:4]", 7, 4},
                         {"Attr[3:0]", 3, 0},
                         {"Attr[3:0]", 3, 0},
                 }
         },
         {
                 "TRBTRG_EL1",
                 "Trace Buffer Trigger Counter Register",
                 "11", "000", "1001", "1011", "110",
                 0xc4de,
                 {
                         {"TRG", 31, 0},
                 }
         },
         {
                 "TRBIDR_EL1",
                 "Trace Buffer ID Register",
                 "11", "000", "1001", "1011", "111",
                 0xc4df,
                 {
                         {"F", 5, 5},
                         {"P", 4, 4},
                         {"Align", 3, 0},
                 }
         },
         {
                 "PMINTENSET_EL1",
                 "Performance Monitors Interrupt Enable Set register",
                 "11", "000", "1001", "1110", "001",
                 0xc4f1,
                 {
                         {"C", 31, 31},
                         {"P<n>", 30, 0},
                 }
         },
         {
                 "PMINTENCLR_EL1",
                 "Performance Monitors Interrupt Enable Clear register",
                 "11", "000", "1001", "1110", "010",
                 0xc4f2,
                 {
                         {"C", 31, 31},
                         {"P<n>", 30, 0},
                 }
         },
         {
                 "PMMIR_EL1",
                 "Performance Monitors Machine Identification Register",
                 "11", "000", "1001", "1110", "110",
                 0xc4f6,
                 {
                         {"BUS_WIDTH", 19, 16},
                         {"BUS_SLOTS", 15, 8},
                         {"SLOTS", 7, 0},
                 }
         },
         {
                 "MAIR_EL1",
                 "Memory Attribute Indirection Register (EL1)",
                 "11", "000", "1010", "0010", "000",
                 0xc510,
                 {
                         {"Attr<n>", 63, 0},
                 }
         },
         {
                 "AMAIR_EL1",
                 "Auxiliary Memory Attribute Indirection Register (EL1)",
                 "11", "000", "1010", "0011", "000",
                 0xc518,
                 {
                         {"IMPLEMENTATION DEFINED", 63, 0},
                 }
         },
         {
                 "LORSA_EL1",
                 "LORegion Start Address (EL1)",
                 "11", "000", "1010", "0100", "000",
                 0xc520,
                 {
                         {"SA", 51, 16},
                         {"Valid", 0, 0},
                 }
         },
         {
                 "LOREA_EL1",
                 "LORegion End Address (EL1)",
                 "11", "000", "1010", "0100", "001",
                 0xc521,
                 {
                         {"EA[51:48]", 51, 48},
                         {"EA[47:16]", 47, 16},
                 }
         },
         {
                 "LORN_EL1",
                 "LORegion Number (EL1)",
                 "11", "000", "1010", "0100", "010",
                 0xc522,
                 {
                         {"Num", 7, 0},
                 }
         },
         {
                 "LORC_EL1",
                 "LORegion Control (EL1)",
                 "11", "000", "1010", "0100", "011",
                 0xc523,
                 {
                         {"DS", 9, 2},
                         {"EN", 0, 0},
                 }
         },
         {
                 "MPAMIDR_EL1",
                 "MPAM ID Register (EL1)",
                 "11", "000", "1010", "0100", "100",
                 0xc524,
                 {
                         {"HAS_SDEFLT", 61, 61},
                         {"HAS_FORCE_NS", 60, 60},
                         {"SP4", 59, 59},
                         {"HAS_TIDR", 58, 58},
                         {"HAS_ALTSP", 57, 57},
                         {"PMG_MAX", 39, 32},
                         {"VPMR_MAX", 20, 18},
                         {"None", 20, 18},
                         {"HAS_HCR", 17, 17},
                         {"PARTID_MAX", 15, 0},
                 }
         },
         {
                 "LORID_EL1",
                 "LORegionID (EL1)",
                 "11", "000", "1010", "0100", "111",
                 0xc527,
                 {
                         {"LD", 23, 16},
                         {"LR", 7, 0},
                 }
         },
         {
                 "MPAM1_EL1",
                 "MPAM1 Register (EL1)",
                 "11", "000", "1010", "0101", "000",
                 0xc528,
                 {
                         {"MPAMEN", 63, 63},
                         {"FORCED_NS", 60, 60},
                         {"ALTSP_FRCD", 54, 54},
                         {"PMG_D", 47, 40},
                         {"PMG_I", 39, 32},
                         {"PARTID_D", 31, 16},
                         {"PARTID_I", 15, 0},
                 }
         },
         {
                 "MPAM0_EL1",
                 "MPAM0 Register (EL1)",
                 "11", "000", "1010", "0101", "001",
                 0xc529,
                 {
                         {"PMG_D", 47, 40},
                         {"PMG_I", 39, 32},
                         {"PARTID_D", 31, 16},
                         {"PARTID_I", 15, 0},
                 }
         },
         {
                 "MPAMSM_EL1",
                 "MPAM Streaming Mode Register",
                 "11", "000", "1010", "0101", "011",
                 0xc52b,
                 {
                         {"PMG_D", 47, 40},
                         {"PARTID_D", 31, 16},
                 }
         },
         {
                 "VBAR_EL1",
                 "Vector Base Address Register (EL1)",
                 "11", "000", "1100", "0000", "000",
                 0xc600,
                 {
                         {"None", 63, 11},
                 }
         },
         {
                 "RVBAR_EL1",
                 "Reset Vector Base Address Register (if EL2 and EL3 not implemented)",
                 "11", "000", "1100", "0000", "001",
                 0xc601,
                 {
                         {"ResetAddress", 63, 0},
                 }
         },
         {
                 "RMR_EL1",
                 "Reset Management Register (EL1)",
                 "11", "000", "1100", "0000", "010",
                 0xc602,
                 {
                         {"RR", 1, 1},
                         {"AA64", 0, 0},
                         {"None", 0, 0},
                 }
         },
         {
                 "ISR_EL1",
                 "Interrupt Status Register",
                 "11", "000", "1100", "0001", "000",
                 0xc608,
                 {
                         {"A", 8, 8},
                         {"I", 7, 7},
                         {"F", 6, 6},
                 }
         },
         {
                 "DISR_EL1",
                 "Deferred Interrupt Status Register",
                 "11", "000", "1100", "0001", "001",
                 0xc609,
                 {
                         {"A", 31, 31},
                         {"IDS", 24, 24},
                         {"AET", 12, 10},
                         {"EA", 9, 9},
                         {"DFSC", 5, 0},
                 }
         },
         {
                 "ICC_IAR0_EL1",
                 "Interrupt Controller Interrupt Acknowledge Register 0",
                 "11", "000", "1100", "1000", "000",
                 0xc640,
                 {
                         {"INTID", 23, 0},
                 }
         },
         {
                 "ICC_EOIR0_EL1",
                 "Interrupt Controller End Of Interrupt Register 0",
                 "11", "000", "1100", "1000", "001",
                 0xc641,
                 {
                         {"INTID", 23, 0},
                 }
         },
         {
                 "ICC_HPPIR0_EL1",
                 "Interrupt Controller Highest Priority Pending Interrupt Register 0",
                 "11", "000", "1100", "1000", "010",
                 0xc642,
                 {
                         {"INTID", 23, 0},
                 }
         },
         {
                 "ICC_BPR0_EL1",
                 "Interrupt Controller Binary Point Register 0",
                 "11", "000", "1100", "1000", "011",
                 0xc643,
                 {
                         {"BinaryPoint", 2, 0},
                 }
         },
         {
                 "ICC_AP0R0_EL1",
                 "Interrupt Controller Active Priorities Group 0 Registers",
                 "11", "000", "1100", "1000", "100",
                 0xc644,
                 {
                         {"IMPLEMENTATION DEFINED", 31, 0},
                 }
         },
         {
                 "ICC_AP0R1_EL1",
                 "Interrupt Controller Active Priorities Group 0 Registers",
                 "11", "000", "1100", "1000", "101",
                 0xc645,
                 {
                         {"IMPLEMENTATION DEFINED", 31, 0},
                 }
         },
         {
                 "ICC_AP0R2_EL1",
                 "Interrupt Controller Active Priorities Group 0 Registers",
                 "11", "000", "1100", "1000", "110",
                 0xc646,
                 {
                         {"IMPLEMENTATION DEFINED", 31, 0},
                 }
         },
         {
                 "ICC_AP0R3_EL1",
                 "Interrupt Controller Active Priorities Group 0 Registers",
                 "11", "000", "1100", "1000", "111",
                 0xc647,
                 {
                         {"IMPLEMENTATION DEFINED", 31, 0},
                 }
         },
         {
                 "ICC_AP1R0_EL1",
                 "Interrupt Controller Active Priorities Group 1 Registers",
                 "11", "000", "1100", "1001", "000",
                 0xc648,
                 {
                         {"IMPLEMENTATION DEFINED", 31, 0},
                 }
         },
         {
                 "ICC_AP1R1_EL1",
                 "Interrupt Controller Active Priorities Group 1 Registers",
                 "11", "000", "1100", "1001", "001",
                 0xc649,
                 {
                         {"IMPLEMENTATION DEFINED", 31, 0},
                 }
         },
         {
                 "ICC_AP1R2_EL1",
                 "Interrupt Controller Active Priorities Group 1 Registers",
                 "11", "000", "1100", "1001", "010",
                 0xc64a,
                 {
                         {"IMPLEMENTATION DEFINED", 31, 0},
                 }
         },
         {
                 "ICC_AP1R3_EL1",
                 "Interrupt Controller Active Priorities Group 1 Registers",
                 "11", "000", "1100", "1001", "011",
                 0xc64b,
                 {
                         {"IMPLEMENTATION DEFINED", 31, 0},
                 }
         },
         {
                 "ICC_DIR_EL1",
                 "Interrupt Controller Deactivate Interrupt Register",
                 "11", "000", "1100", "1011", "001",
                 0xc659,
                 {
                         {"INTID", 23, 0},
                 }
         },
         {
                 "ICC_RPR_EL1",
                 "Interrupt Controller Running Priority Register",
                 "11", "000", "1100", "1011", "011",
                 0xc65b,
                 {
                         {"Priority", 7, 0},
                 }
         },
         {
                 "ICC_SGI1R_EL1",
                 "Interrupt Controller Software Generated Interrupt Group 1 Register",
                 "11", "000", "1100", "1011", "101",
                 0xc65d,
                 {
                         {"Aff3", 55, 48},
                         {"RS", 47, 44},
                         {"IRM", 40, 40},
                         {"Aff2", 39, 32},
                         {"INTID", 27, 24},
                         {"Aff1", 23, 16},
                         {"TargetList", 15, 0},
                 }
         },
         {
                 "ICC_ASGI1R_EL1",
                 "Interrupt Controller Alias Software Generated Interrupt Group 1 Register",
                 "11", "000", "1100", "1011", "110",
                 0xc65e,
                 {
                         {"Aff3", 55, 48},
                         {"RS", 47, 44},
                         {"IRM", 40, 40},
                         {"Aff2", 39, 32},
                         {"INTID", 27, 24},
                         {"Aff1", 23, 16},
                         {"TargetList", 15, 0},
                 }
         },
         {
                 "ICC_SGI0R_EL1",
                 "Interrupt Controller Software Generated Interrupt Group 0 Register",
                 "11", "000", "1100", "1011", "111",
                 0xc65f,
                 {
                         {"Aff3", 55, 48},
                         {"RS", 47, 44},
                         {"IRM", 40, 40},
                         {"Aff2", 39, 32},
                         {"INTID", 27, 24},
                         {"Aff1", 23, 16},
                         {"TargetList", 15, 0},
                 }
         },
         {
                 "ICC_IAR1_EL1",
                 "Interrupt Controller Interrupt Acknowledge Register 1",
                 "11", "000", "1100", "1100", "000",
                 0xc660,
                 {
                         {"INTID", 23, 0},
                 }
         },
         {
                 "ICC_EOIR1_EL1",
                 "Interrupt Controller End Of Interrupt Register 1",
                 "11", "000", "1100", "1100", "001",
                 0xc661,
                 {
                         {"INTID", 23, 0},
                 }
         },
         {
                 "ICC_HPPIR1_EL1",
                 "Interrupt Controller Highest Priority Pending Interrupt Register 1",
                 "11", "000", "1100", "1100", "010",
                 0xc662,
                 {
                         {"INTID", 23, 0},
                 }
         },
         {
                 "ICC_BPR1_EL1",
                 "Interrupt Controller Binary Point Register 1",
                 "11", "000", "1100", "1100", "011",
                 0xc663,
                 {
                         {"BinaryPoint", 2, 0},
                 }
         },
         {
                 "ICC_CTLR_EL1",
                 "Interrupt Controller Control Register (EL1)",
                 "11", "000", "1100", "1100", "100",
                 0xc664,
                 {
                         {"ExtRange", 19, 19},
                         {"RSS", 18, 18},
                         {"A3V", 15, 15},
                         {"SEIS", 14, 14},
                         {"IDbits", 13, 11},
                         {"PRIbits", 10, 8},
                         {"PMHE", 6, 6},
                         {"EOImode", 1, 1},
                         {"CBPR", 0, 0},
                 }
         },
         {
                 "ICC_SRE_EL1",
                 "Interrupt Controller System Register Enable register (EL1)",
                 "11", "000", "1100", "1100", "101",
                 0xc665,
                 {
                         {"DIB", 2, 2},
                         {"DFB", 1, 1},
                         {"SRE", 0, 0},
                 }
         },
         {
                 "ICC_IGRPEN0_EL1",
                 "Interrupt Controller Interrupt Group 0 Enable register",
                 "11", "000", "1100", "1100", "110",
                 0xc666,
                 {
                         {"Enable", 0, 0},
                 }
         },
         {
                 "ICC_IGRPEN1_EL1",
                 "Interrupt Controller Interrupt Group 1 Enable register",
                 "11", "000", "1100", "1100", "111",
                 0xc667,
                 {
                         {"Enable", 0, 0},
                 }
         },
         {
                 "CONTEXTIDR_EL1",
                 "Context ID Register (EL1)",
                 "11", "000", "1101", "0000", "001",
                 0xc681,
                 {
                         {"PROCID", 31, 0},
                 }
         },
         {
                 "TPIDR_EL1",
                 "EL1 Software Thread ID Register",
                 "11", "000", "1101", "0000", "100",
                 0xc684,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "ACCDATA_EL1",
                 "Accelerator Data",
                 "11", "000", "1101", "0000", "101",
                 0xc685,
                 {
                         {"ACCDATA", 31, 0},
                 }
         },
         {
                 "SCXTNUM_EL1",
                 "EL1 Read/Write Software Context Number",
                 "11", "000", "1101", "0000", "111",
                 0xc687,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "CNTKCTL_EL1",
                 "Counter-timer Kernel Control register",
                 "11", "000", "1110", "0001", "000",
                 0xc708,
                 {
                         {"EVNTIS", 17, 17},
                         {"EL0PTEN", 9, 9},
                         {"EL0VTEN", 8, 8},
                         {"EVNTI", 7, 4},
                         {"EVNTDIR", 3, 3},
                         {"EVNTEN", 2, 2},
                         {"EL0VCTEN", 1, 1},
                         {"EL0PCTEN", 0, 0},
                 }
         },
         {
                 "CCSIDR_EL1",
                 "Current Cache Size ID Register",
                 "11", "001", "0000", "0000", "000",
                 0xc800,
                 {
                         {"NumSets", 55, 32},
                         {"Associativity", 23, 3},
                         {"LineSize", 2, 0},
                 }
         },
         {
                 "CLIDR_EL1",
                 "Cache Level ID Register",
                 "11", "001", "0000", "0000", "001",
                 0xc801,
                 {
                         {"Ttype<n>", 46, 33},
                         {"ICB", 32, 30},
                         {"LoUU", 29, 27},
                         {"LoC", 26, 24},
                         {"LoUIS", 23, 21},
                         {"Ctype<n>", 20, 0},
                 }
         },
         {
                 "CCSIDR2_EL1",
                 "Current Cache Size ID Register 2",
                 "11", "001", "0000", "0000", "010",
                 0xc802,
                 {
                         {"NumSets", 23, 0},
                 }
         },
         {
                 "GMID_EL1",
                 "Multiple tag transfer ID register",
                 "11", "001", "0000", "0000", "100",
                 0xc804,
                 {
                         {"BS", 3, 0},
                 }
         },
         {
                 "SMIDR_EL1",
                 "Streaming Mode Identification Register",
                 "11", "001", "0000", "0000", "110",
                 0xc806,
                 {
                         {"Implementer", 31, 24},
                         {"Revision", 23, 16},
                         {"SMPS", 15, 15},
                         {"Affinity", 11, 0},
                 }
         },
         {
                 "AIDR_EL1",
                 "Auxiliary ID Register",
                 "11", "001", "0000", "0000", "111",
                 0xc807,
                 {
                         {"IMPLEMENTATION DEFINED", 63, 0},
                 }
         },
         {
                 "CSSELR_EL1",
                 "Cache Size Selection Register",
                 "11", "010", "0000", "0000", "000",
                 0xd000,
                 {
                         {"TnD", 4, 4},
                         {"Level", 3, 1},
                         {"InD", 0, 0},
                 }
         },
         {
                 "CTR_EL0",
                 "Cache Type Register",
                 "11", "011", "0000", "0000", "001",
                 0xd801,
                 {
                         {"TminLine", 37, 32},
                         {"DIC", 29, 29},
                         {"IDC", 28, 28},
                         {"CWG", 27, 24},
                         {"ERG", 23, 20},
                         {"DminLine", 19, 16},
                         {"L1Ip", 15, 14},
                         {"IminLine", 3, 0},
                 }
         },
         {
                 "DCZID_EL0",
                 "Data Cache Zero ID register",
                 "11", "011", "0000", "0000", "111",
                 0xd807,
                 {
                         {"DZP", 4, 4},
                         {"BS", 3, 0},
                 }
         },
         {
                 "RNDR",
                 "Random Number",
                 "11", "011", "0010", "0100", "000",
                 0xd920,
                 {
                         {"RNDR", 63, 0},
                 }
         },
         {
                 "RNDRRS",
                 "Reseeded Random Number",
                 "11", "011", "0010", "0100", "001",
                 0xd921,
                 {
                         {"RNDRRS", 63, 0},
                 }
         },
         {
                 "NZCV",
                 "Condition Flags",
                 "11", "011", "0100", "0010", "000",
                 0xda10,
                 {
                         {"N", 31, 31},
                         {"Z", 30, 30},
                         {"C", 29, 29},
                         {"V", 28, 28},
                 }
         },
         {
                 "DAIF",
                 "Interrupt Mask Bits",
                 "11", "011", "0100", "0010", "001",
                 0xda11,
                 {
                         {"D", 9, 9},
                         {"A", 8, 8},
                         {"I", 7, 7},
                         {"F", 6, 6},
                 }
         },
         {
                 "SVCR",
                 "Streaming Vector Control Register",
                 "11", "011", "0100", "0010", "010",
                 0xda12,
                 {
                         {"ZA", 1, 1},
                         {"SM", 0, 0},
                 }
         },
         {
                 "DIT",
                 "Data Independent Timing",
                 "11", "011", "0100", "0010", "101",
                 0xda15,
                 {
                         {"DIT", 24, 24},
                 }
         },
         {
                 "DIT",
                 "Data Independent Timing",
                 "00", "011", "0100", "0000", "010",
                 0x1a02,
                 {
                         {"DIT", 24, 24},
                 }
         },
         {
                 "SSBS",
                 "Speculative Store Bypass Safe",
                 "11", "011", "0100", "0010", "110",
                 0xda16,
                 {
                         {"SSBS", 12, 12},
                 }
         },
         {
                 "SSBS",
                 "Speculative Store Bypass Safe",
                 "00", "011", "0100", "0000", "001",
                 0x1a01,
                 {
                         {"SSBS", 12, 12},
                 }
         },
         {
                 "TCO",
                 "Tag Check Override",
                 "11", "011", "0100", "0010", "111",
                 0xda17,
                 {
                         {"TCO", 25, 25},
                 }
         },
         {
                 "TCO",
                 "Tag Check Override",
                 "00", "011", "0100", "0000", "100",
                 0x1a04,
                 {
                         {"TCO", 25, 25},
                 }
         },
         {
                 "FPCR",
                 "Floating-point Control Register",
                 "11", "011", "0100", "0100", "000",
                 0xda20,
                 {
                         {"AHP", 26, 26},
                         {"DN", 25, 25},
                         {"FZ", 24, 24},
                         {"RMode", 23, 22},
                         {"Stride", 21, 20},
                         {"FZ16", 19, 19},
                         {"Len", 18, 16},
                         {"IDE", 15, 15},
                         {"EBF", 13, 13},
                         {"IXE", 12, 12},
                         {"UFE", 11, 11},
                         {"OFE", 10, 10},
                         {"DZE", 9, 9},
                         {"IOE", 8, 8},
                         {"NEP", 2, 2},
                         {"AH", 1, 1},
                         {"FIZ", 0, 0},
                 }
         },
         {
                 "FPSR",
                 "Floating-point Status Register",
                 "11", "011", "0100", "0100", "001",
                 0xda21,
                 {
                         {"N", 31, 31},
                         {"Z", 30, 30},
                         {"C", 29, 29},
                         {"V", 28, 28},
                         {"QC", 27, 27},
                         {"IDC", 7, 7},
                         {"IXC", 4, 4},
                         {"UFC", 3, 3},
                         {"OFC", 2, 2},
                         {"DZC", 1, 1},
                         {"IOC", 0, 0},
                 }
         },
         {
                 "DSPSR_EL0",
                 "Debug Saved Program Status Register",
                 "11", "011", "0100", "0101", "000",
                 0xda28,
                 {
                         {"N", 31, 31},
                         {"Z", 30, 30},
                         {"C", 29, 29},
                         {"V", 28, 28},
                         {"Q", 27, 27},
                         {"IT", 26, 25},
                         {"DIT", 24, 24},
                         {"SSBS", 23, 23},
                         {"PAN", 22, 22},
                         {"SS", 21, 21},
                         {"IL", 20, 20},
                         {"GE", 19, 16},
                         {"IT[7:2]", 15, 10},
                         {"E", 9, 9},
                         {"A", 8, 8},
                         {"I", 7, 7},
                         {"F", 6, 6},
                         {"T", 5, 5},
                         {"M[4]", 4, 4},
                         {"M[3:0]", 3, 0},
                 }
         },
         {
                 "DLR_EL0",
                 "Debug Link Register",
                 "11", "011", "0100", "0101", "001",
                 0xda29,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMCR_EL0",
                 "Performance Monitors Control Register",
                 "11", "011", "1001", "1100", "000",
                 0xdce0,
                 {
                         {"FZS", 32, 32},
                         {"IMP", 31, 24},
                         {"None", 31, 24},
                         {"IDCODE", 23, 16},
                         {"N", 15, 11},
                         {"FZO", 9, 9},
                         {"LP", 7, 7},
                         {"LC", 6, 6},
                         {"DP", 5, 5},
                         {"X", 4, 4},
                         {"None", 4, 4},
                         {"D", 3, 3},
                         {"C", 2, 2},
                         {"P", 1, 1},
                         {"E", 0, 0},
                 }
         },
         {
                 "PMCNTENSET_EL0",
                 "Performance Monitors Count Enable Set register",
                 "11", "011", "1001", "1100", "001",
                 0xdce1,
                 {
                         {"C", 31, 31},
                         {"P<n>", 30, 0},
                 }
         },
         {
                 "PMCNTENCLR_EL0",
                 "Performance Monitors Count Enable Clear register",
                 "11", "011", "1001", "1100", "010",
                 0xdce2,
                 {
                         {"C", 31, 31},
                         {"P<n>", 30, 0},
                 }
         },
         {
                 "PMOVSCLR_EL0",
                 "Performance Monitors Overflow Flag Status Clear Register",
                 "11", "011", "1001", "1100", "011",
                 0xdce3,
                 {
                         {"C", 31, 31},
                         {"P<n>", 30, 0},
                 }
         },
         {
                 "PMSWINC_EL0",
                 "Performance Monitors Software Increment register",
                 "11", "011", "1001", "1100", "100",
                 0xdce4,
                 {
                         {"P<n>", 30, 0},
                 }
         },
         {
                 "PMSELR_EL0",
                 "Performance Monitors Event Counter Selection Register",
                 "11", "011", "1001", "1100", "101",
                 0xdce5,
                 {
                         {"SEL", 4, 0},
                 }
         },
         {
                 "PMCEID0_EL0",
                 "Performance Monitors Common Event Identification register 0",
                 "11", "011", "1001", "1100", "110",
                 0xdce6,
                 {
                         {"IDhi<n>", 63, 32},
                         {"ID<n>", 31, 0},
                 }
         },
         {
                 "PMCEID1_EL0",
                 "Performance Monitors Common Event Identification register 1",
                 "11", "011", "1001", "1100", "111",
                 0xdce7,
                 {
                         {"IDhi<n>", 63, 32},
                         {"ID<n>", 31, 0},
                 }
         },
         {
                 "PMCCNTR_EL0",
                 "Performance Monitors Cycle Count Register",
                 "11", "011", "1001", "1101", "000",
                 0xdce8,
                 {
                         {"CCNT", 63, 0},
                 }
         },
         {
                 "PMXEVTYPER_EL0",
                 "Performance Monitors Selected Event Type Register",
                 "11", "011", "1001", "1101", "001",
                 0xdce9,
                 {
                         {"None", 31, 0},
                 }
         },
         {
                 "PMXEVCNTR_EL0",
                 "Performance Monitors Selected Event Count Register",
                 "11", "011", "1001", "1101", "010",
                 0xdcea,
                 {
                         {"PMEVCNTR<n>", 63, 0},
                 }
         },
         {
                 "PMUSERENR_EL0",
                 "Performance Monitors User Enable Register",
                 "11", "011", "1001", "1110", "000",
                 0xdcf0,
                 {
                         {"ER", 3, 3},
                         {"CR", 2, 2},
                         {"SW", 1, 1},
                         {"EN", 0, 0},
                 }
         },
         {
                 "PMOVSSET_EL0",
                 "Performance Monitors Overflow Flag Status Set register",
                 "11", "011", "1001", "1110", "011",
                 0xdcf3,
                 {
                         {"C", 31, 31},
                         {"P<n>", 30, 0},
                 }
         },
         {
                 "TPIDR_EL0",
                 "EL0 Read/Write Software Thread ID Register",
                 "11", "011", "1101", "0000", "010",
                 0xde82,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "TPIDRRO_EL0",
                 "EL0 Read-Only Software Thread ID Register",
                 "11", "011", "1101", "0000", "011",
                 0xde83,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "TPIDR2_EL0",
                 "EL0 Read/Write Software Thread ID Register 2",
                 "11", "011", "1101", "0000", "101",
                 0xde85,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "SCXTNUM_EL0",
                 "EL0 Read/Write Software Context Number",
                 "11", "011", "1101", "0000", "111",
                 0xde87,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMCR_EL0",
                 "Activity Monitors Control Register",
                 "11", "011", "1101", "0010", "000",
                 0xde90,
                 {
                         {"CG1RZ", 17, 17},
                         {"HDBG", 10, 10},
                 }
         },
         {
                 "AMCFGR_EL0",
                 "Activity Monitors Configuration Register",
                 "11", "011", "1101", "0010", "001",
                 0xde91,
                 {
                         {"NCG", 31, 28},
                         {"HDBG", 24, 24},
                         {"None", 23, 14},
                         {"SIZE", 13, 8},
                         {"N", 7, 0},
                 }
         },
         {
                 "AMCGCR_EL0",
                 "Activity Monitors Counter Group Configuration Register",
                 "11", "011", "1101", "0010", "010",
                 0xde92,
                 {
                         {"CG1NC", 15, 8},
                         {"CG0NC", 7, 0},
                 }
         },
         {
                 "AMUSERENR_EL0",
                 "Activity Monitors User Enable Register",
                 "11", "011", "1101", "0010", "011",
                 0xde93,
                 {
                         {"EN", 0, 0},
                 }
         },
         {
                 "AMCNTENCLR0_EL0",
                 "Activity Monitors Count Enable Clear Register 0",
                 "11", "011", "1101", "0010", "100",
                 0xde94,
                 {
                         {"None", 15, 4},
                         {"P<n>", 3, 0},
                 }
         },
         {
                 "AMCNTENSET0_EL0",
                 "Activity Monitors Count Enable Set Register 0",
                 "11", "011", "1101", "0010", "101",
                 0xde95,
                 {
                         {"None", 15, 4},
                         {"P<n>", 3, 0},
                 }
         },
         {
                 "AMCG1IDR_EL0",
                 "Activity Monitors Counter Group 1 Identification Register",
                 "11", "011", "1101", "0010", "110",
                 0xde96,
                 {
                         {"AMEVCNTOFF1<n>_EL2", 31, 16},
                         {"AMEVCNTR1<n>_EL0", 15, 0},
                 }
         },
         {
                 "AMCNTENCLR1_EL0",
                 "Activity Monitors Count Enable Clear Register 1",
                 "11", "011", "1101", "0011", "000",
                 0xde98,
                 {
                         {"P<n>", 15, 0},
                 }
         },
         {
                 "AMCNTENSET1_EL0",
                 "Activity Monitors Count Enable Set Register 1",
                 "11", "011", "1101", "0011", "001",
                 0xde99,
                 {
                         {"P<n>", 15, 0},
                 }
         },
         {
                 "AMEVCNTR00_EL0",
                 "Activity Monitors Event Counter Registers 0",
                 "11", "011", "1101", "0100", "000",
                 0xdea0,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR01_EL0",
                 "Activity Monitors Event Counter Registers 0",
                 "11", "011", "1101", "0100", "001",
                 0xdea1,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR02_EL0",
                 "Activity Monitors Event Counter Registers 0",
                 "11", "011", "1101", "0100", "010",
                 0xdea2,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR03_EL0",
                 "Activity Monitors Event Counter Registers 0",
                 "11", "011", "1101", "0100", "011",
                 0xdea3,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR04_EL0",
                 "Activity Monitors Event Counter Registers 0",
                 "11", "011", "1101", "0100", "100",
                 0xdea4,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR05_EL0",
                 "Activity Monitors Event Counter Registers 0",
                 "11", "011", "1101", "0100", "101",
                 0xdea5,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR06_EL0",
                 "Activity Monitors Event Counter Registers 0",
                 "11", "011", "1101", "0100", "110",
                 0xdea6,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR07_EL0",
                 "Activity Monitors Event Counter Registers 0",
                 "11", "011", "1101", "0100", "111",
                 0xdea7,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR08_EL0",
                 "Activity Monitors Event Counter Registers 0",
                 "11", "011", "1101", "0101", "000",
                 0xdea8,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR09_EL0",
                 "Activity Monitors Event Counter Registers 0",
                 "11", "011", "1101", "0101", "001",
                 0xdea9,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR010_EL0",
                 "Activity Monitors Event Counter Registers 0",
                 "11", "011", "1101", "0101", "010",
                 0xdeaa,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR011_EL0",
                 "Activity Monitors Event Counter Registers 0",
                 "11", "011", "1101", "0101", "011",
                 0xdeab,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR012_EL0",
                 "Activity Monitors Event Counter Registers 0",
                 "11", "011", "1101", "0101", "100",
                 0xdeac,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR013_EL0",
                 "Activity Monitors Event Counter Registers 0",
                 "11", "011", "1101", "0101", "101",
                 0xdead,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR014_EL0",
                 "Activity Monitors Event Counter Registers 0",
                 "11", "011", "1101", "0101", "110",
                 0xdeae,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR015_EL0",
                 "Activity Monitors Event Counter Registers 0",
                 "11", "011", "1101", "0101", "111",
                 0xdeaf,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVTYPER00_EL0",
                 "Activity Monitors Event Type Registers 0",
                 "11", "011", "1101", "0110", "000",
                 0xdeb0,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER01_EL0",
                 "Activity Monitors Event Type Registers 0",
                 "11", "011", "1101", "0110", "001",
                 0xdeb1,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER02_EL0",
                 "Activity Monitors Event Type Registers 0",
                 "11", "011", "1101", "0110", "010",
                 0xdeb2,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER03_EL0",
                 "Activity Monitors Event Type Registers 0",
                 "11", "011", "1101", "0110", "011",
                 0xdeb3,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER04_EL0",
                 "Activity Monitors Event Type Registers 0",
                 "11", "011", "1101", "0110", "100",
                 0xdeb4,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER05_EL0",
                 "Activity Monitors Event Type Registers 0",
                 "11", "011", "1101", "0110", "101",
                 0xdeb5,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER06_EL0",
                 "Activity Monitors Event Type Registers 0",
                 "11", "011", "1101", "0110", "110",
                 0xdeb6,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER07_EL0",
                 "Activity Monitors Event Type Registers 0",
                 "11", "011", "1101", "0110", "111",
                 0xdeb7,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER08_EL0",
                 "Activity Monitors Event Type Registers 0",
                 "11", "011", "1101", "0111", "000",
                 0xdeb8,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER09_EL0",
                 "Activity Monitors Event Type Registers 0",
                 "11", "011", "1101", "0111", "001",
                 0xdeb9,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER010_EL0",
                 "Activity Monitors Event Type Registers 0",
                 "11", "011", "1101", "0111", "010",
                 0xdeba,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER011_EL0",
                 "Activity Monitors Event Type Registers 0",
                 "11", "011", "1101", "0111", "011",
                 0xdebb,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER012_EL0",
                 "Activity Monitors Event Type Registers 0",
                 "11", "011", "1101", "0111", "100",
                 0xdebc,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER013_EL0",
                 "Activity Monitors Event Type Registers 0",
                 "11", "011", "1101", "0111", "101",
                 0xdebd,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER014_EL0",
                 "Activity Monitors Event Type Registers 0",
                 "11", "011", "1101", "0111", "110",
                 0xdebe,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER015_EL0",
                 "Activity Monitors Event Type Registers 0",
                 "11", "011", "1101", "0111", "111",
                 0xdebf,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVCNTR10_EL0",
                 "Activity Monitors Event Counter Registers 1",
                 "11", "011", "1101", "1100", "000",
                 0xdee0,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR11_EL0",
                 "Activity Monitors Event Counter Registers 1",
                 "11", "011", "1101", "1100", "001",
                 0xdee1,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR12_EL0",
                 "Activity Monitors Event Counter Registers 1",
                 "11", "011", "1101", "1100", "010",
                 0xdee2,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR13_EL0",
                 "Activity Monitors Event Counter Registers 1",
                 "11", "011", "1101", "1100", "011",
                 0xdee3,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR14_EL0",
                 "Activity Monitors Event Counter Registers 1",
                 "11", "011", "1101", "1100", "100",
                 0xdee4,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR15_EL0",
                 "Activity Monitors Event Counter Registers 1",
                 "11", "011", "1101", "1100", "101",
                 0xdee5,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR16_EL0",
                 "Activity Monitors Event Counter Registers 1",
                 "11", "011", "1101", "1100", "110",
                 0xdee6,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR17_EL0",
                 "Activity Monitors Event Counter Registers 1",
                 "11", "011", "1101", "1100", "111",
                 0xdee7,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR18_EL0",
                 "Activity Monitors Event Counter Registers 1",
                 "11", "011", "1101", "1101", "000",
                 0xdee8,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR19_EL0",
                 "Activity Monitors Event Counter Registers 1",
                 "11", "011", "1101", "1101", "001",
                 0xdee9,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR110_EL0",
                 "Activity Monitors Event Counter Registers 1",
                 "11", "011", "1101", "1101", "010",
                 0xdeea,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR111_EL0",
                 "Activity Monitors Event Counter Registers 1",
                 "11", "011", "1101", "1101", "011",
                 0xdeeb,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR112_EL0",
                 "Activity Monitors Event Counter Registers 1",
                 "11", "011", "1101", "1101", "100",
                 0xdeec,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR113_EL0",
                 "Activity Monitors Event Counter Registers 1",
                 "11", "011", "1101", "1101", "101",
                 0xdeed,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR114_EL0",
                 "Activity Monitors Event Counter Registers 1",
                 "11", "011", "1101", "1101", "110",
                 0xdeee,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVCNTR115_EL0",
                 "Activity Monitors Event Counter Registers 1",
                 "11", "011", "1101", "1101", "111",
                 0xdeef,
                 {
                         {"ACNT", 63, 0},
                 }
         },
         {
                 "AMEVTYPER10_EL0",
                 "Activity Monitors Event Type Registers 1",
                 "11", "011", "1101", "1110", "000",
                 0xdef0,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER11_EL0",
                 "Activity Monitors Event Type Registers 1",
                 "11", "011", "1101", "1110", "001",
                 0xdef1,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER12_EL0",
                 "Activity Monitors Event Type Registers 1",
                 "11", "011", "1101", "1110", "010",
                 0xdef2,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER13_EL0",
                 "Activity Monitors Event Type Registers 1",
                 "11", "011", "1101", "1110", "011",
                 0xdef3,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER14_EL0",
                 "Activity Monitors Event Type Registers 1",
                 "11", "011", "1101", "1110", "100",
                 0xdef4,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER15_EL0",
                 "Activity Monitors Event Type Registers 1",
                 "11", "011", "1101", "1110", "101",
                 0xdef5,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER16_EL0",
                 "Activity Monitors Event Type Registers 1",
                 "11", "011", "1101", "1110", "110",
                 0xdef6,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER17_EL0",
                 "Activity Monitors Event Type Registers 1",
                 "11", "011", "1101", "1110", "111",
                 0xdef7,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER18_EL0",
                 "Activity Monitors Event Type Registers 1",
                 "11", "011", "1101", "1111", "000",
                 0xdef8,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER19_EL0",
                 "Activity Monitors Event Type Registers 1",
                 "11", "011", "1101", "1111", "001",
                 0xdef9,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER110_EL0",
                 "Activity Monitors Event Type Registers 1",
                 "11", "011", "1101", "1111", "010",
                 0xdefa,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER111_EL0",
                 "Activity Monitors Event Type Registers 1",
                 "11", "011", "1101", "1111", "011",
                 0xdefb,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER112_EL0",
                 "Activity Monitors Event Type Registers 1",
                 "11", "011", "1101", "1111", "100",
                 0xdefc,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER113_EL0",
                 "Activity Monitors Event Type Registers 1",
                 "11", "011", "1101", "1111", "101",
                 0xdefd,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER114_EL0",
                 "Activity Monitors Event Type Registers 1",
                 "11", "011", "1101", "1111", "110",
                 0xdefe,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "AMEVTYPER115_EL0",
                 "Activity Monitors Event Type Registers 1",
                 "11", "011", "1101", "1111", "111",
                 0xdeff,
                 {
                         {"evtCount", 15, 0},
                 }
         },
         {
                 "CNTFRQ_EL0",
                 "Counter-timer Frequency register",
                 "11", "011", "1110", "0000", "000",
                 0xdf00,
                 {
                         {"None", 31, 0},
                 }
         },
         {
                 "CNTPCT_EL0",
                 "Counter-timer Physical Count register",
                 "11", "011", "1110", "0000", "001",
                 0xdf01,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "CNTVCT_EL0",
                 "Counter-timer Virtual Count register",
                 "11", "011", "1110", "0000", "010",
                 0xdf02,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "CNTPCTSS_EL0",
                 "Counter-timer Self-Synchronized Physical Count register",
                 "11", "011", "1110", "0000", "101",
                 0xdf05,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "CNTVCTSS_EL0",
                 "Counter-timer Self-Synchronized Virtual Count register",
                 "11", "011", "1110", "0000", "110",
                 0xdf06,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "CNTP_TVAL_EL0",
                 "Counter-timer Physical Timer TimerValue register",
                 "11", "011", "1110", "0010", "000",
                 0xdf10,
                 {
                         {"TimerValue", 31, 0},
                 }
         },
         {
                 "CNTP_CTL_EL0",
                 "Counter-timer Physical Timer Control register",
                 "11", "011", "1110", "0010", "001",
                 0xdf11,
                 {
                         {"ISTATUS", 2, 2},
                         {"IMASK", 1, 1},
                         {"ENABLE", 0, 0},
                 }
         },
         {
                 "CNTP_CVAL_EL0",
                 "Counter-timer Physical Timer CompareValue register",
                 "11", "011", "1110", "0010", "010",
                 0xdf12,
                 {
                         {"CompareValue", 63, 0},
                 }
         },
         {
                 "CNTV_TVAL_EL0",
                 "Counter-timer Virtual Timer TimerValue register",
                 "11", "011", "1110", "0011", "000",
                 0xdf18,
                 {
                         {"TimerValue", 31, 0},
                 }
         },
         {
                 "CNTV_CTL_EL0",
                 "Counter-timer Virtual Timer Control register",
                 "11", "011", "1110", "0011", "001",
                 0xdf19,
                 {
                         {"ISTATUS", 2, 2},
                         {"IMASK", 1, 1},
                         {"ENABLE", 0, 0},
                 }
         },
         {
                 "CNTV_CVAL_EL0",
                 "Counter-timer Virtual Timer CompareValue register",
                 "11", "011", "1110", "0011", "010",
                 0xdf1a,
                 {
                         {"CompareValue", 63, 0},
                 }
         },
         {
                 "PMEVCNTR0_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1000", "000",
                 0xdf40,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR1_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1000", "001",
                 0xdf41,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR2_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1000", "010",
                 0xdf42,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR3_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1000", "011",
                 0xdf43,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR4_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1000", "100",
                 0xdf44,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR5_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1000", "101",
                 0xdf45,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR6_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1000", "110",
                 0xdf46,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR7_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1000", "111",
                 0xdf47,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR8_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1001", "000",
                 0xdf48,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR9_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1001", "001",
                 0xdf49,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR10_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1001", "010",
                 0xdf4a,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR11_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1001", "011",
                 0xdf4b,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR12_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1001", "100",
                 0xdf4c,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR13_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1001", "101",
                 0xdf4d,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR14_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1001", "110",
                 0xdf4e,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR15_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1001", "111",
                 0xdf4f,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR16_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1010", "000",
                 0xdf50,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR17_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1010", "001",
                 0xdf51,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR18_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1010", "010",
                 0xdf52,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR19_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1010", "011",
                 0xdf53,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR20_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1010", "100",
                 0xdf54,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR21_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1010", "101",
                 0xdf55,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR22_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1010", "110",
                 0xdf56,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR23_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1010", "111",
                 0xdf57,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR24_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1011", "000",
                 0xdf58,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR25_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1011", "001",
                 0xdf59,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR26_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1011", "010",
                 0xdf5a,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR27_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1011", "011",
                 0xdf5b,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR28_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1011", "100",
                 0xdf5c,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR29_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1011", "101",
                 0xdf5d,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR30_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1011", "110",
                 0xdf5e,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMEVCNTR31_EL0",
                 "Performance Monitors Event Count Registers",
                 "11", "011", "1110", "1011", "111",
                 0xdf5f,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "PMCCFILTR_EL0",
                 "Performance Monitors Cycle Count Filter Register",
                 "11", "011", "1110", "1111", "111",
                 0xdf7f,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                 }
         },
         {
                 "PMEVTYPER0_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1100", "000",
                 0xdf60,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER1_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1100", "001",
                 0xdf61,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER2_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1100", "010",
                 0xdf62,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER3_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1100", "011",
                 0xdf63,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER4_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1100", "100",
                 0xdf64,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER5_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1100", "101",
                 0xdf65,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER6_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1100", "110",
                 0xdf66,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER7_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1100", "111",
                 0xdf67,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER8_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1101", "000",
                 0xdf68,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER9_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1101", "001",
                 0xdf69,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER10_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1101", "010",
                 0xdf6a,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER11_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1101", "011",
                 0xdf6b,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER12_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1101", "100",
                 0xdf6c,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER13_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1101", "101",
                 0xdf6d,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER14_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1101", "110",
                 0xdf6e,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER15_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1101", "111",
                 0xdf6f,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER16_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1110", "000",
                 0xdf70,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER17_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1110", "001",
                 0xdf71,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER18_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1110", "010",
                 0xdf72,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER19_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1110", "011",
                 0xdf73,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER20_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1110", "100",
                 0xdf74,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER21_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1110", "101",
                 0xdf75,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER22_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1110", "110",
                 0xdf76,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER23_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1110", "111",
                 0xdf77,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER24_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1111", "000",
                 0xdf78,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER25_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1111", "001",
                 0xdf79,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER26_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1111", "010",
                 0xdf7a,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER27_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1111", "011",
                 0xdf7b,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER28_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1111", "100",
                 0xdf7c,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER29_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1111", "101",
                 0xdf7d,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER30_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1111", "110",
                 0xdf7e,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "PMEVTYPER31_EL0",
                 "Performance Monitors Event Type Registers",
                 "11", "011", "1110", "1111", "111",
                 0xdf7f,
                 {
                         {"P", 31, 31},
                         {"U", 30, 30},
                         {"NSK", 29, 29},
                         {"NSU", 28, 28},
                         {"NSH", 27, 27},
                         {"M", 26, 26},
                         {"MT", 25, 25},
                         {"SH", 24, 24},
                         {"T", 23, 23},
                         {"RLK", 22, 22},
                         {"RLU", 21, 21},
                         {"RLH", 20, 20},
                         {"evtCount[15:10]", 15, 10},
                         {"evtCount[9:0]", 9, 0},
                 }
         },
         {
                 "VPIDR_EL2",
                 "Virtualization Processor ID Register",
                 "11", "100", "0000", "0000", "000",
                 0xe000,
                 {
                         {"Implementer", 31, 24},
                         {"Variant", 23, 20},
                         {"Architecture", 19, 16},
                         {"PartNum", 15, 4},
                         {"Revision", 3, 0},
                 }
         },
         {
                 "VMPIDR_EL2",
                 "Virtualization Multiprocessor ID Register",
                 "11", "100", "0000", "0000", "101",
                 0xe005,
                 {
                         {"Aff3", 39, 32},
                         {"U", 30, 30},
                         {"MT", 24, 24},
                         {"Aff2", 23, 16},
                         {"Aff1", 15, 8},
                         {"Aff0", 7, 0},
                 }
         },
         {
                 "SCTLR_EL2",
                 "System Control Register (EL2)",
                 "11", "100", "0001", "0000", "000",
                 0xe080,
                 {
                         {"EnTP2", 60, 60},
                         {"EPAN", 57, 57},
                         {"EnALS", 56, 56},
                         {"EnAS0", 55, 55},
                         {"EnASR", 54, 54},
                         {"TME", 53, 53},
                         {"TME0", 52, 52},
                         {"TMT", 51, 51},
                         {"TMT0", 50, 50},
                         {"TWEDEL", 49, 46},
                         {"TWEDEn", 45, 45},
                         {"DSSBS", 44, 44},
                         {"ATA", 43, 43},
                         {"ATA0", 42, 42},
                         {"TCF", 41, 40},
                         {"TCF0", 39, 38},
                         {"ITFSB", 37, 37},
                         {"BT", 36, 36},
                         {"BT0", 35, 35},
                         {"EnIA", 31, 31},
                         {"EnIB", 30, 30},
                         {"LSMAOE", 29, 29},
                         {"nTLSMD", 28, 28},
                         {"EnDA", 27, 27},
                         {"UCI", 26, 26},
                         {"EE", 25, 25},
                         {"E0E", 24, 24},
                         {"SPAN", 23, 23},
                         {"EIS", 22, 22},
                         {"IESB", 21, 21},
                         {"TSCXT", 20, 20},
                         {"WXN", 19, 19},
                         {"nTWE", 18, 18},
                         {"nTWI", 16, 16},
                         {"UCT", 15, 15},
                         {"DZE", 14, 14},
                         {"EnDB", 13, 13},
                         {"I", 12, 12},
                         {"EOS", 11, 11},
                         {"EnRCTX", 10, 10},
                         {"SED", 8, 8},
                         {"ITD", 7, 7},
                         {"nAA", 6, 6},
                         {"CP15BEN", 5, 5},
                         {"SA0", 4, 4},
                         {"SA", 3, 3},
                         {"C", 2, 2},
                         {"A", 1, 1},
                         {"M", 0, 0},
                 }
         },
         {
                 "ACTLR_EL2",
                 "Auxiliary Control Register (EL2)",
                 "11", "100", "0001", "0000", "001",
                 0xe081,
                 {
                         {"IMPLEMENTATION DEFINED", 63, 0},
                 }
         },
         {
                 "HCR_EL2",
                 "Hypervisor Configuration Register",
                 "11", "100", "0001", "0001", "000",
                 0xe088,
                 {
                         {"TWEDEL", 63, 60},
                         {"TWEDEn", 59, 59},
                         {"TID5", 58, 58},
                         {"DCT", 57, 57},
                         {"ATA", 56, 56},
                         {"TTLBOS", 55, 55},
                         {"TTLBIS", 54, 54},
                         {"EnSCXT", 53, 53},
                         {"TOCU", 52, 52},
                         {"AMVOFFEN", 51, 51},
                         {"TICAB", 50, 50},
                         {"TID4", 49, 49},
                         {"GPF", 48, 48},
                         {"FIEN", 47, 47},
                         {"FWB", 46, 46},
                         {"NV2", 45, 45},
                         {"AT", 44, 44},
                         {"NV1", 43, 43},
                         {"NV1", 43, 43},
                         {"NV", 42, 42},
                         {"NV", 42, 42},
                         {"API", 41, 41},
                         {"APK", 40, 40},
                         {"TME", 39, 39},
                         {"MIOCNCE", 38, 38},
                         {"TEA", 37, 37},
                         {"TERR", 36, 36},
                         {"TLOR", 35, 35},
                         {"E2H", 34, 34},
                         {"ID", 33, 33},
                         {"CD", 32, 32},
                         {"RW", 31, 31},
                         {"None", 31, 31},
                         {"TRVM", 30, 30},
                         {"HCD", 29, 29},
                         {"TDZ", 28, 28},
                         {"TGE", 27, 27},
                         {"TVM", 26, 26},
                         {"TTLB", 25, 25},
                         {"TPU", 24, 24},
                         {"TPCP", 23, 23},
                         {"TPC", 23, 23},
                         {"TSW", 22, 22},
                         {"TACR", 21, 21},
                         {"TIDCP", 20, 20},
                         {"TSC", 19, 19},
                         {"TID3", 18, 18},
                         {"TID2", 17, 17},
                         {"TID1", 16, 16},
                         {"TID0", 15, 15},
                         {"TWE", 14, 14},
                         {"TWI", 13, 13},
                         {"DC", 12, 12},
                         {"BSU", 11, 10},
                         {"FB", 9, 9},
                         {"VSE", 8, 8},
                         {"VI", 7, 7},
                         {"VF", 6, 6},
                         {"AMO", 5, 5},
                         {"IMO", 4, 4},
                         {"FMO", 3, 3},
                         {"PTW", 2, 2},
                         {"SWIO", 1, 1},
                         {"VM", 0, 0}
                 }
         },
         {
                 "MDCR_EL2",
                 "Monitor Debug Configuration Register (EL2)",
                 "11", "100", "0001", "0001", "001",
                 0xe089,
                 {
                         {"HPMFZS", 36, 36},
                         {"HPMFZO", 29, 29},
                         {"MTPME", 28, 28},
                         {"TDCC", 27, 27},
                         {"HLP", 26, 26},
                         {"E2TB", 25, 24},
                         {"HCCD", 23, 23},
                         {"TTRF", 19, 19},
                         {"HPMD", 17, 17},
                         {"HPMD", 17, 17},
                         {"TPMS", 14, 14},
                         {"E2PB", 13, 12},
                         {"TDRA", 11, 11},
                         {"TDOSA", 10, 10},
                         {"TDOSA", 10, 10},
                         {"TDA", 9, 9},
                         {"TDE", 8, 8},
                         {"HPME", 7, 7},
                         {"TPM", 6, 6},
                         {"TPMCR", 5, 5},
                         {"HPMN", 4, 0},
                 }
         },
         {
                 "CPTR_EL2",
                 "Architectural Feature Trap Register (EL2)",
                 "11", "100", "0001", "0001", "010",
                 0xe08a,
                 {
                         {"TCPAC", 31, 31},
                         {"TAM", 30, 30},
                         {"TTA", 28, 28},
                         {"SMEN", 25, 24},
                         {"FPEN", 21, 20},
                         {"ZEN", 17, 16},
                 }
         },
         {
                 "HSTR_EL2",
                 "Hypervisor System Trap Register",
                 "11", "100", "0001", "0001", "011",
                 0xe08b,
                 {
                         {"T<n>", 15, 15},
                         {"T15", 15, 15},
                         {"T13", 13, 13},
                         {"T12", 12, 12},
                         {"T11", 11, 11},
                         {"T10", 10, 10},
                         {"T9", 9, 9},
                         {"T8", 8, 8},
                         {"T7", 7, 7},
                         {"T6", 6, 6},
                         {"T5", 5, 5},
                         {"T3", 3, 3},
                         {"T2", 2, 2},
                         {"T1", 1, 1},
                         {"T0", 0, 0},
                 }
         },
         {
                 "HFGRTR_EL2",
                 "Hypervisor Fine-Grained Read Trap Register",
                 "11", "100", "0001", "0001", "100",
                 0xe08c,
                 {
                         {"nTPIDR2_EL0", 55, 55},
                         {"nSMPRI_EL1", 54, 54},
                         {"nACCDATA_EL1", 50, 50},
                         {"ERXADDR_EL1", 49, 49},
                         {"ERXPFGCDN_EL1", 48, 48},
                         {"ERXPFGCTL_EL1", 47, 47},
                         {"ERXPFGF_EL1", 46, 46},
                         {"ERXMISCn_EL1", 45, 45},
                         {"ERXSTATUS_EL1", 44, 44},
                         {"ERXCTLR_EL1", 43, 43},
                         {"ERXFR_EL1", 42, 42},
                         {"ERRSELR_EL1", 41, 41},
                         {"ERRIDR_EL1", 40, 40},
                         {"ICC_IGRPENn_EL1", 39, 39},
                         {"VBAR_EL1", 38, 38},
                         {"TTBR1_EL1", 37, 37},
                         {"TTBR0_EL1", 36, 36},
                         {"TPIDR_EL0", 35, 35},
                         {"TPIDRRO_EL0", 34, 34},
                         {"TPIDR_EL1", 33, 33},
                         {"TCR_EL1", 32, 32},
                         {"SCXTNUM_EL0", 31, 31},
                         {"SCXTNUM_EL1", 30, 30},
                         {"SCTLR_EL1", 29, 29},
                         {"REVIDR_EL1", 28, 28},
                         {"PAR_EL1", 27, 27},
                         {"MPIDR_EL1", 26, 26},
                         {"MIDR_EL1", 25, 25},
                         {"MAIR_EL1", 24, 24},
                         {"LORSA_EL1", 23, 23},
                         {"LORN_EL1", 22, 22},
                         {"LORID_EL1", 21, 21},
                         {"LOREA_EL1", 20, 20},
                         {"LORC_EL1", 19, 19},
                         {"ISR_EL1", 18, 18},
                         {"FAR_EL1", 17, 17},
                         {"ESR_EL1", 16, 16},
                         {"DCZID_EL0", 15, 15},
                         {"CTR_EL0", 14, 14},
                         {"CSSELR_EL1", 13, 13},
                         {"CPACR_EL1", 12, 12},
                         {"CONTEXTIDR_EL1", 11, 11},
                         {"CLIDR_EL1", 10, 10},
                         {"CCSIDR_EL1", 9, 9},
                         {"APIBKey", 8, 8},
                         {"APIAKey", 7, 7},
                         {"APGAKey", 6, 6},
                         {"APDBKey", 5, 5},
                         {"APDAKey", 4, 4},
                         {"AMAIR_EL1", 3, 3},
                         {"AIDR_EL1", 2, 2},
                         {"AFSR1_EL1", 1, 1},
                         {"AFSR0_EL1", 0, 0},
                 }
         },
         {
                 "HFGWTR_EL2",
                 "Hypervisor Fine-Grained Write Trap Register",
                 "11", "100", "0001", "0001", "101",
                 0xe08d,
                 {
                         {"nTPIDR2_EL0", 55, 55},
                         {"nSMPRI_EL1", 54, 54},
                         {"nACCDATA_EL1", 50, 50},
                         {"ERXADDR_EL1", 49, 49},
                         {"ERXPFGCDN_EL1", 48, 48},
                         {"ERXPFGCTL_EL1", 47, 47},
                         {"ERXMISCn_EL1", 45, 45},
                         {"ERXSTATUS_EL1", 44, 44},
                         {"ERXCTLR_EL1", 43, 43},
                         {"ERRSELR_EL1", 41, 41},
                         {"ICC_IGRPENn_EL1", 39, 39},
                         {"VBAR_EL1", 38, 38},
                         {"TTBR1_EL1", 37, 37},
                         {"TTBR0_EL1", 36, 36},
                         {"TPIDR_EL0", 35, 35},
                         {"TPIDRRO_EL0", 34, 34},
                         {"TPIDR_EL1", 33, 33},
                         {"TCR_EL1", 32, 32},
                         {"SCXTNUM_EL0", 31, 31},
                         {"SCXTNUM_EL1", 30, 30},
                         {"SCTLR_EL1", 29, 29},
                         {"PAR_EL1", 27, 27},
                         {"MAIR_EL1", 24, 24},
                         {"LORSA_EL1", 23, 23},
                         {"LORN_EL1", 22, 22},
                         {"LOREA_EL1", 20, 20},
                         {"LORC_EL1", 19, 19},
                         {"FAR_EL1", 17, 17},
                         {"ESR_EL1", 16, 16},
                         {"CSSELR_EL1", 13, 13},
                         {"CPACR_EL1", 12, 12},
                         {"CONTEXTIDR_EL1", 11, 11},
                         {"APIBKey", 8, 8},
                         {"APIAKey", 7, 7},
                         {"APGAKey", 6, 6},
                         {"APDBKey", 5, 5},
                         {"APDAKey", 4, 4},
                         {"AMAIR_EL1", 3, 3},
                         {"AFSR1_EL1", 1, 1},
                         {"AFSR0_EL1", 0, 0},
                 }
         },
         {
                 "HFGITR_EL2",
                 "Hypervisor Fine-Grained Instruction Trap Register",
                 "11", "100", "0001", "0001", "110",
                 0xe08e,
                 {
                         {"nBRBIALL", 56, 56},
                         {"nBRBINJ", 55, 55},
                         {"DCCVAC", 54, 54},
                         {"SVC_EL1", 53, 53},
                         {"SVC_EL0", 52, 52},
                         {"ERET", 51, 51},
                         {"CPPRCTX", 50, 50},
                         {"DVPRCTX", 49, 49},
                         {"CFPRCTX", 48, 48},
                         {"TLBIVAALE1", 47, 47},
                         {"TLBIVALE1", 46, 46},
                         {"TLBIVAAE1", 45, 45},
                         {"TLBIASIDE1", 44, 44},
                         {"TLBIVAE1", 43, 43},
                         {"TLBIVMALLE1", 42, 42},
                         {"TLBIRVAALE1", 41, 41},
                         {"TLBIRVALE1", 40, 40},
                         {"TLBIRVAAE1", 39, 39},
                         {"TLBIRVAE1", 38, 38},
                         {"TLBIRVAALE1IS", 37, 37},
                         {"TLBIRVALE1IS", 36, 36},
                         {"TLBIRVAAE1IS", 35, 35},
                         {"TLBIRVAE1IS", 34, 34},
                         {"TLBIVAALE1IS", 33, 33},
                         {"TLBIVALE1IS", 32, 32},
                         {"TLBIVAAE1IS", 31, 31},
                         {"TLBIASIDE1IS", 30, 30},
                         {"TLBIVAE1IS", 29, 29},
                         {"TLBIVMALLE1IS", 28, 28},
                         {"TLBIRVAALE1OS", 27, 27},
                         {"TLBIRVALE1OS", 26, 26},
                         {"TLBIRVAAE1OS", 25, 25},
                         {"TLBIRVAE1OS", 24, 24},
                         {"TLBIVAALE1OS", 23, 23},
                         {"TLBIVALE1OS", 22, 22},
                         {"TLBIVAAE1OS", 21, 21},
                         {"TLBIASIDE1OS", 20, 20},
                         {"TLBIVAE1OS", 19, 19},
                         {"TLBIVMALLE1OS", 18, 18},
                         {"ATS1E1WP", 17, 17},
                         {"ATS1E1RP", 16, 16},
                         {"ATS1E0W", 15, 15},
                         {"ATS1E0R", 14, 14},
                         {"ATS1E1W", 13, 13},
                         {"ATS1E1R", 12, 12},
                         {"DCZVA", 11, 11},
                         {"DCCIVAC", 10, 10},
                         {"DCCVADP", 9, 9},
                         {"DCCVAP", 8, 8},
                         {"DCCVAU", 7, 7},
                         {"DCCISW", 6, 6},
                         {"DCCSW", 5, 5},
                         {"DCISW", 4, 4},
                         {"DCIVAC", 3, 3},
                         {"ICIVAU", 2, 2},
                         {"ICIALLU", 1, 1},
                         {"ICIALLUIS", 0, 0},
                 }
         },
         {
                 "HACR_EL2",
                 "Hypervisor Auxiliary Control Register",
                 "11", "100", "0001", "0001", "111",
                 0xe08f,
                 {
                         {"IMPLEMENTATION DEFINED", 63, 0},
                 }
         },
         {
                 "ZCR_EL2",
                 "SVE Control Register (EL2)",
                 "11", "100", "0001", "0010", "000",
                 0xe090,
                 {
                         {"None", 8, 4},
                         {"LEN", 3, 0},
                 }
         },
         {
                 "TRFCR_EL2",
                 "Trace Filter Control Register (EL2)",
                 "11", "100", "0001", "0010", "001",
                 0xe091,
                 {
                         {"TS", 6, 5},
                         {"CX", 3, 3},
                         {"E2TRE", 1, 1},
                         {"E0HTRE", 0, 0},
                 }
         },
         {
                 "HCRX_EL2",
                 "Extended Hypervisor Configuration Register",
                 "11", "100", "0001", "0010", "010",
                 0xe092,
                 {
                         {"SMPME", 5, 5},
                         {"FGTnXS", 4, 4},
                         {"FnXS", 3, 3},
                         {"EnASR", 2, 2},
                         {"EnALS", 1, 1},
                         {"EnAS0", 0, 0},
                 }
         },
         {
                 "SMPRIMAP_EL2",
                 "Streaming Mode Priority Mapping Register",
                 "11", "100", "0001", "0010", "101",
                 0xe095,
                 {
                         {"P15", 63, 60},
                         {"P14", 59, 56},
                         {"P13", 55, 52},
                         {"P12", 51, 48},
                         {"P11", 47, 44},
                         {"P10", 43, 40},
                         {"P9", 39, 36},
                         {"P8", 35, 32},
                         {"P7", 31, 28},
                         {"P6", 27, 24},
                         {"P5", 23, 20},
                         {"P4", 19, 16},
                         {"P3", 15, 12},
                         {"P2", 11, 8},
                         {"P1", 7, 4},
                         {"P0", 3, 0},
                 }
         },
         {
                 "SMCR_EL2",
                 "SME Control Register (EL2)",
                 "11", "100", "0001", "0010", "110",
                 0xe096,
                 {
                         {"None", 8, 4},
                         {"LEN", 3, 0},
                 }
         },
         {
                 "SDER32_EL2",
                 "AArch32 Secure Debug Enable Register",
                 "11", "100", "0001", "0011", "001",
                 0xe099,
                 {
                         {"SUNIDEN", 1, 1},
                         {"SUIDEN", 0, 0},
                 }
         },
         {
                 "TTBR0_EL2",
                 "Translation Table Base Register 0 (EL2)",
                 "11", "100", "0010", "0000", "000",
                 0xe100,
                 {
                         {"ASID", 63, 48},
                         {"BADDR[47:1]", 47, 1},
                         {"CnP", 0, 0},
                 }
         },
         {
                 "TTBR1_EL2",
                 "Translation Table Base Register 1 (EL2)",
                 "11", "100", "0010", "0000", "001",
                 0xe101,
                 {
                         {"ASID", 63, 48},
                         {"BADDR[47:1]", 47, 1},
                         {"CnP", 0, 0},
                 }
         },
         {
                 "TCR_EL2",
                 "Translation Control Register (EL2)",
                 "11", "100", "0010", "0000", "010",
                 0xe102,
                 {
                         {"DS", 32, 32},
                         {"TCMA", 30, 30},
                         {"TBID", 29, 29},
                         {"HWU62", 28, 28},
                         {"None", 28, 28},
                         {"HWU61", 27, 27},
                         {"None", 27, 27},
                         {"HWU60", 26, 26},
                         {"None", 26, 26},
                         {"HWU59", 25, 25},
                         {"None", 25, 25},
                         {"HPD", 24, 24},
                         {"HD", 22, 22},
                         {"HA", 21, 21},
                         {"TBI", 20, 20},
                         {"PS", 18, 16},
                         {"TG0", 15, 14},
                         {"SH0", 13, 12},
                         {"ORGN0", 11, 10},
                         {"IRGN0", 9, 8},
                         {"T0SZ", 5, 0},
                 }
         },
         {
                 "VTTBR_EL2",
                 "Virtualization Translation Table Base Register",
                 "11", "100", "0010", "0001", "000",
                 0xe108,
                 {
                         {"VMID", 63, 48},
                         {"BADDR", 47, 1},
                         {"CnP", 0, 0},
                 }
         },
         {
                 "VTCR_EL2",
                 "Virtualization Translation Control Register",
                 "11", "100", "0010", "0001", "010",
                 0xe10a,
                 {
                         {"SL2", 33, 33},
                         {"DS", 32, 32},
                         {"NSA", 30, 30},
                         {"NSW", 29, 29},
                         {"HWU62", 28, 28},
                         {"None", 28, 28},
                         {"HWU61", 27, 27},
                         {"None", 27, 27},
                         {"HWU60", 26, 26},
                         {"None", 26, 26},
                         {"HWU59", 25, 25},
                         {"None", 25, 25},
                         {"HD", 22, 22},
                         {"HA", 21, 21},
                         {"VS", 19, 19},
                         {"PS", 18, 16},
                         {"TG0", 15, 14},
                         {"SH0", 13, 12},
                         {"ORGN0", 11, 10},
                         {"IRGN0", 9, 8},
                         {"SL0", 7, 6},
                         {"SL0", 7, 6},
                         {"T0SZ", 5, 0},
                 }
         },
         {
                 "VNCR_EL2",
                 "Virtual Nested Control Register",
                 "11", "100", "0010", "0010", "000",
                 0xe110,
                 {
                         {"RESS", 63, 53},
                         {"BADDR", 52, 12},
                 }
         },
         {
                 "VSTTBR_EL2",
                 "Virtualization Secure Translation Table Base Register",
                 "11", "100", "0010", "0110", "000",
                 0xe130,
                 {
                         {"BADDR", 47, 1},
                         {"CnP", 0, 0},
                 }
         },
         {
                 "VSTCR_EL2",
                 "Virtualization Secure Translation Control Register",
                 "11", "100", "0010", "0110", "010",
                 0xe132,
                 {
                         {"SL2", 33, 33},
                         {"SA", 30, 30},
                         {"SW", 29, 29},
                         {"TG0", 15, 14},
                         {"SL0", 7, 6},
                         {"SL0", 7, 6},
                         {"T0SZ", 5, 0},
                 }
         },
         {
                 "DACR32_EL2",
                 "Domain Access Control Register",
                 "11", "100", "0011", "0000", "000",
                 0xe180,
                 {
                         {"D<n>", 31, 0},
                 }
         },
         {
                 "HDFGRTR_EL2",
                 "Hypervisor Debug Fine-Grained Read Trap Register",
                 "11", "100", "0011", "0001", "100",
                 0xe18c,
                 {
                         {"PMBIDR_EL1", 63, 63},
                         {"nPMSNEVFR_EL1", 62, 62},
                         {"nBRBDATA", 61, 61},
                         {"nBRBCTL", 60, 60},
                         {"nBRBIDR", 59, 59},
                         {"PMCEIDn_EL0", 58, 58},
                         {"PMUSERENR_EL0", 57, 57},
                         {"TRBTRG_EL1", 56, 56},
                         {"TRBSR_EL1", 55, 55},
                         {"TRBPTR_EL1", 54, 54},
                         {"TRBMAR_EL1", 53, 53},
                         {"TRBLIMITR_EL1", 52, 52},
                         {"TRBIDR_EL1", 51, 51},
                         {"TRBBASER_EL1", 50, 50},
                         {"TRCVICTLR", 48, 48},
                         {"TRCSTATR", 47, 47},
                         {"TRCSSCSRn", 46, 46},
                         {"TRCSEQSTR", 45, 45},
                         {"TRCPRGCTLR", 44, 44},
                         {"TRCOSLSR", 43, 43},
                         {"TRCIMSPECn", 41, 41},
                         {"TRCID", 40, 40},
                         {"TRCCNTVRn", 37, 37},
                         {"TRCCLAIM", 36, 36},
                         {"TRCAUXCTLR", 35, 35},
                         {"TRCAUTHSTATUS", 34, 34},
                         {"TRC", 33, 33},
                         {"PMSLATFR_EL1", 32, 32},
                         {"PMSIRR_EL1", 31, 31},
                         {"PMSIDR_EL1", 30, 30},
                         {"PMSICR_EL1", 29, 29},
                         {"PMSFCR_EL1", 28, 28},
                         {"PMSEVFR_EL1", 27, 27},
                         {"PMSCR_EL1", 26, 26},
                         {"PMBSR_EL1", 25, 25},
                         {"PMBPTR_EL1", 24, 24},
                         {"PMBLIMITR_EL1", 23, 23},
                         {"PMMIR_EL1", 22, 22},
                         {"PMSELR_EL0", 19, 19},
                         {"PMOVS", 18, 18},
                         {"PMINTEN", 17, 17},
                         {"PMCNTEN", 16, 16},
                         {"PMCCNTR_EL0", 15, 15},
                         {"PMCCFILTR_EL0", 14, 14},
                         {"PMEVTYPERn_EL0", 13, 13},
                         {"PMEVCNTRn_EL0", 12, 12},
                         {"OSDLR_EL1", 11, 11},
                         {"OSECCR_EL1", 10, 10},
                         {"OSLSR_EL1", 9, 9},
                         {"DBGPRCR_EL1", 7, 7},
                         {"DBGAUTHSTATUS_EL1", 6, 6},
                         {"DBGCLAIM", 5, 5},
                         {"MDSCR_EL1", 4, 4},
                         {"DBGWVRn_EL1", 3, 3},
                         {"DBGWCRn_EL1", 2, 2},
                         {"DBGBVRn_EL1", 1, 1},
                         {"DBGBCRn_EL1", 0, 0},
                 }
         },
         {
                 "HDFGWTR_EL2",
                 "Hypervisor Debug Fine-Grained Write Trap Register",
                 "11", "100", "0011", "0001", "101",
                 0xe18d,
                 {
                         {"nPMSNEVFR_EL1", 62, 62},
                         {"nBRBDATA", 61, 61},
                         {"nBRBCTL", 60, 60},
                         {"PMUSERENR_EL0", 57, 57},
                         {"TRBTRG_EL1", 56, 56},
                         {"TRBSR_EL1", 55, 55},
                         {"TRBPTR_EL1", 54, 54},
                         {"TRBMAR_EL1", 53, 53},
                         {"TRBLIMITR_EL1", 52, 52},
                         {"TRBBASER_EL1", 50, 50},
                         {"TRFCR_EL1", 49, 49},
                         {"TRCVICTLR", 48, 48},
                         {"TRCSSCSRn", 46, 46},
                         {"TRCSEQSTR", 45, 45},
                         {"TRCPRGCTLR", 44, 44},
                         {"TRCOSLAR", 42, 42},
                         {"TRCIMSPECn", 41, 41},
                         {"TRCCNTVRn", 37, 37},
                         {"TRCCLAIM", 36, 36},
                         {"TRCAUXCTLR", 35, 35},
                         {"TRC", 33, 33},
                         {"PMSLATFR_EL1", 32, 32},
                         {"PMSIRR_EL1", 31, 31},
                         {"PMSICR_EL1", 29, 29},
                         {"PMSFCR_EL1", 28, 28},
                         {"PMSEVFR_EL1", 27, 27},
                         {"PMSCR_EL1", 26, 26},
                         {"PMBSR_EL1", 25, 25},
                         {"PMBPTR_EL1", 24, 24},
                         {"PMBLIMITR_EL1", 23, 23},
                         {"PMCR_EL0", 21, 21},
                         {"PMSWINC_EL0", 20, 20},
                         {"PMSELR_EL0", 19, 19},
                         {"PMOVS", 18, 18},
                         {"PMINTEN", 17, 17},
                         {"PMCNTEN", 16, 16},
                         {"PMCCNTR_EL0", 15, 15},
                         {"PMCCFILTR_EL0", 14, 14},
                         {"PMEVTYPERn_EL0", 13, 13},
                         {"PMEVCNTRn_EL0", 12, 12},
                         {"OSDLR_EL1", 11, 11},
                         {"OSECCR_EL1", 10, 10},
                         {"OSLAR_EL1", 8, 8},
                         {"DBGPRCR_EL1", 7, 7},
                         {"DBGCLAIM", 5, 5},
                         {"MDSCR_EL1", 4, 4},
                         {"DBGWVRn_EL1", 3, 3},
                         {"DBGWCRn_EL1", 2, 2},
                         {"DBGBVRn_EL1", 1, 1},
                         {"DBGBCRn_EL1", 0, 0},
                 }
         },
         {
                 "HAFGRTR_EL2",
                 "Hypervisor Activity Monitors Fine-Grained Read Trap Register",
                 "11", "100", "0011", "0001", "110",
                 0xe18e,
                 {
                         {"AMEVTYPER1<x>_EL0", 49, 49},
                         {"AMEVTYPER115_EL0", 49, 49},
                         {"AMEVCNTR1<x>_EL0", 48, 48},
                         {"AMEVCNTR115_EL0", 48, 48},
                         {"AMEVTYPER114_EL0", 47, 47},
                         {"AMEVCNTR114_EL0", 46, 46},
                         {"AMEVTYPER113_EL0", 45, 45},
                         {"AMEVCNTR113_EL0", 44, 44},
                         {"AMEVTYPER112_EL0", 43, 43},
                         {"AMEVCNTR112_EL0", 42, 42},
                         {"AMEVTYPER111_EL0", 41, 41},
                         {"AMEVCNTR111_EL0", 40, 40},
                         {"AMEVTYPER110_EL0", 39, 39},
                         {"AMEVCNTR110_EL0", 38, 38},
                         {"AMEVTYPER19_EL0", 37, 37},
                         {"AMEVCNTR19_EL0", 36, 36},
                         {"AMEVTYPER18_EL0", 35, 35},
                         {"AMEVCNTR18_EL0", 34, 34},
                         {"AMEVTYPER17_EL0", 33, 33},
                         {"AMEVCNTR17_EL0", 32, 32},
                         {"AMEVTYPER16_EL0", 31, 31},
                         {"AMEVCNTR16_EL0", 30, 30},
                         {"AMEVTYPER15_EL0", 29, 29},
                         {"AMEVCNTR15_EL0", 28, 28},
                         {"AMEVTYPER14_EL0", 27, 27},
                         {"AMEVCNTR14_EL0", 26, 26},
                         {"AMEVTYPER13_EL0", 25, 25},
                         {"AMEVCNTR13_EL0", 24, 24},
                         {"AMEVTYPER12_EL0", 23, 23},
                         {"AMEVCNTR12_EL0", 22, 22},
                         {"AMEVTYPER11_EL0", 21, 21},
                         {"AMEVCNTR11_EL0", 20, 20},
                         {"AMEVTYPER10_EL0", 19, 19},
                         {"AMEVCNTR10_EL0", 18, 18},
                         {"AMCNTEN<x>", 17, 17},
                         {"AMCNTEN1", 17, 17},
                         {"AMEVCNTR0<x>_EL0", 4, 1},
                         {"AMCNTEN0", 0, 0},
                 }
         },
         {
                 "SPSR_EL2",
                 "Saved Program Status Register (EL2)",
                 "11", "100", "0100", "0000", "000",
                 0xe200,
                 {
                         {"N", 31, 31},
                         {"Z", 30, 30},
                         {"C", 29, 29},
                         {"V", 28, 28},
                         {"Q", 27, 27},
                         {"IT", 26, 25},
                         {"DIT", 24, 24},
                         {"SSBS", 23, 23},
                         {"PAN", 22, 22},
                         {"SS", 21, 21},
                         {"IL", 20, 20},
                         {"GE", 19, 16},
                         {"IT[7:2]", 15, 10},
                         {"E", 9, 9},
                         {"A", 8, 8},
                         {"I", 7, 7},
                         {"F", 6, 6},
                         {"T", 5, 5},
                         {"M[4]", 4, 4},
                         {"M[3:0]", 3, 0},
                 }
         },
         {
                 "ELR_EL2",
                 "Exception Link Register (EL2)",
                 "11", "100", "0100", "0000", "001",
                 0xe201,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "SP_EL1",
                 "Stack Pointer (EL1)",
                 "11", "100", "0100", "0001", "000",
                 0xe208,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "SPSR_irq",
                 "Saved Program Status Register (IRQ mode)",
                 "11", "100", "0100", "0011", "000",
                 0xe218,
                 {
                         {"N", 31, 31},
                         {"Z", 30, 30},
                         {"C", 29, 29},
                         {"V", 28, 28},
                         {"Q", 27, 27},
                         {"IT", 26, 25},
                         {"J", 24, 24},
                         {"SSBS", 23, 23},
                         {"PAN", 22, 22},
                         {"DIT", 21, 21},
                         {"IL", 20, 20},
                         {"GE", 19, 16},
                         {"IT[7:2]", 15, 10},
                         {"E", 9, 9},
                         {"A", 8, 8},
                         {"I", 7, 7},
                         {"F", 6, 6},
                         {"T", 5, 5},
                         {"M[4:0]", 4, 0},
                 }
         },
         {
                 "SPSR_abt",
                 "Saved Program Status Register (Abort mode)",
                 "11", "100", "0100", "0011", "001",
                 0xe219,
                 {
                         {"N", 31, 31},
                         {"Z", 30, 30},
                         {"C", 29, 29},
                         {"V", 28, 28},
                         {"Q", 27, 27},
                         {"IT", 26, 25},
                         {"J", 24, 24},
                         {"SSBS", 23, 23},
                         {"PAN", 22, 22},
                         {"DIT", 21, 21},
                         {"IL", 20, 20},
                         {"GE", 19, 16},
                         {"IT[7:2]", 15, 10},
                         {"E", 9, 9},
                         {"A", 8, 8},
                         {"I", 7, 7},
                         {"F", 6, 6},
                         {"T", 5, 5},
                         {"M[4:0]", 4, 0},
                 }
         },
         {
                 "SPSR_und",
                 "Saved Program Status Register (Undefined mode)",
                 "11", "100", "0100", "0011", "010",
                 0xe21a,
                 {
                         {"N", 31, 31},
                         {"Z", 30, 30},
                         {"C", 29, 29},
                         {"V", 28, 28},
                         {"Q", 27, 27},
                         {"IT", 26, 25},
                         {"J", 24, 24},
                         {"SSBS", 23, 23},
                         {"PAN", 22, 22},
                         {"DIT", 21, 21},
                         {"IL", 20, 20},
                         {"GE", 19, 16},
                         {"IT[7:2]", 15, 10},
                         {"E", 9, 9},
                         {"A", 8, 8},
                         {"I", 7, 7},
                         {"F", 6, 6},
                         {"T", 5, 5},
                         {"M[4:0]", 4, 0},
                 }
         },
         {
                 "SPSR_fiq",
                 "Saved Program Status Register (FIQ mode)",
                 "11", "100", "0100", "0011", "011",
                 0xe21b,
                 {
                         {"N", 31, 31},
                         {"Z", 30, 30},
                         {"C", 29, 29},
                         {"V", 28, 28},
                         {"Q", 27, 27},
                         {"IT", 26, 25},
                         {"J", 24, 24},
                         {"SSBS", 23, 23},
                         {"PAN", 22, 22},
                         {"DIT", 21, 21},
                         {"IL", 20, 20},
                         {"GE", 19, 16},
                         {"IT[7:2]", 15, 10},
                         {"E", 9, 9},
                         {"A", 8, 8},
                         {"I", 7, 7},
                         {"F", 6, 6},
                         {"T", 5, 5},
                         {"M[4:0]", 4, 0},
                 }
         },
         {
                 "IFSR32_EL2",
                 "Instruction Fault Status Register (EL2)",
                 "11", "100", "0101", "0000", "001",
                 0xe281,
                 {
                         {"FnV", 16, 16},
                         {"ExT", 12, 12},
                         {"FS", 10, 10},
                         {"LPAE", 9, 9},
                         {"FS[3:0]", 3, 0},
                 }
         },
         {
                 "AFSR0_EL2",
                 "Auxiliary Fault Status Register 0 (EL2)",
                 "11", "100", "0101", "0001", "000",
                 0xe288,
                 {
                         {"IMPLEMENTATION DEFINED", 63, 0},
                 }
         },
         {
                 "AFSR1_EL2",
                 "Auxiliary Fault Status Register 1 (EL2)",
                 "11", "100", "0101", "0001", "001",
                 0xe289,
                 {
                         {"IMPLEMENTATION DEFINED", 63, 0},
                 }
         },
         {
                 "ESR_EL2",
                 "Exception Syndrome Register (EL2)",
                 "11", "100", "0101", "0010", "000",
                 0xe290,
                 {
                         {"ISS2", 36, 32},
                         {"EC", 31, 26},
                         {"IL", 25, 25},
                         {"ISS", 24, 0},
                 }
         },
         {
                 "VSESR_EL2",
                 "Virtual SError Exception Syndrome Register",
                 "11", "100", "0101", "0010", "011",
                 0xe293,
                 {
                         {"AET", 15, 14},
                         {"ExT", 12, 12},
                 }
         },
         {
                 "FPEXC32_EL2",
                 "Floating-Point Exception Control register",
                 "11", "100", "0101", "0011", "000",
                 0xe298,
                 {
                         {"EX", 31, 31},
                         {"EN", 30, 30},
                         {"DEX", 29, 29},
                         {"FP2V", 28, 28},
                         {"VV", 27, 27},
                         {"TFV", 26, 26},
                         {"VECITR", 10, 8},
                         {"IDF", 7, 7},
                         {"IXF", 4, 4},
                         {"UFF", 3, 3},
                         {"OFF", 2, 2},
                         {"DZF", 1, 1},
                         {"IOF", 0, 0},
                 }
         },
         {
                 "TFSR_EL2",
                 "Tag Fault Status Register (EL2)",
                 "11", "100", "0101", "0110", "000",
                 0xe2b0,
                 {
                         {"TF1", 1, 1},
                         {"TF0", 0, 0},
                 }
         },
         {
                 "FAR_EL2",
                 "Fault Address Register (EL2)",
                 "11", "100", "0110", "0000", "000",
                 0xe300,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "HPFAR_EL2",
                 "Hypervisor IPA Fault Address Register",
                 "11", "100", "0110", "0000", "100",
                 0xe304,
                 {
                         {"NS", 63, 63},
                         {"FIPA", 43, 4},
                 }
         },
         {
                 "PMSCR_EL2",
                 "Statistical Profiling Control Register (EL2)",
                 "11", "100", "1001", "1001", "000",
                 0xe4c8,
                 {
                         {"PCT", 7, 6},
                         {"TS", 5, 5},
                         {"PA", 4, 4},
                         {"CX", 3, 3},
                         {"E2SPE", 1, 1},
                         {"E0HSPE", 0, 0},
                 }
         },
         {
                 "MAIR_EL2",
                 "Memory Attribute Indirection Register (EL2)",
                 "11", "100", "1010", "0010", "000",
                 0xe510,
                 {
                         {"Attr<n>", 63, 0},
                 }
         },
         {
                 "AMAIR_EL2",
                 "Auxiliary Memory Attribute Indirection Register (EL2)",
                 "11", "100", "1010", "0011", "000",
                 0xe518,
                 {
                         {"IMPLEMENTATION DEFINED", 63, 0},
                 }
         },
         {
                 "MPAMHCR_EL2",
                 "MPAM Hypervisor Control Register (EL2)",
                 "11", "100", "1010", "0100", "000",
                 0xe520,
                 {
                         {"TRAP_MPAMIDR_EL1", 31, 31},
                         {"GSTAPP_PLK", 8, 8},
                         {"EL1_VPMEN", 1, 1},
                         {"EL0_VPMEN", 0, 0},
                 }
         },
         {
                 "MPAMVPMV_EL2",
                 "MPAM Virtual Partition Mapping Valid Register",
                 "11", "100", "1010", "0100", "001",
                 0xe521,
                 {
                         {"VPM_V<m>", 31, 0},
                 }
         },
         {
                 "MPAM2_EL2",
                 "MPAM2 Register (EL2)",
                 "11", "100", "1010", "0101", "000",
                 0xe528,
                 {
                         {"MPAMEN", 63, 63},
                         {"TIDR", 58, 58},
                         {"ALTSP_HFC", 56, 56},
                         {"ALTSP_EL2", 55, 55},
                         {"ALTSP_FRCD", 54, 54},
                         {"EnMPAMSM", 50, 50},
                         {"TRAPMPAM0EL1", 49, 49},
                         {"TRAPMPAM1EL1", 48, 48},
                         {"PMG_D", 47, 40},
                         {"PMG_I", 39, 32},
                         {"PARTID_D", 31, 16},
                         {"PARTID_I", 15, 0},
                 }
         },
         {
                 "MPAMVPM0_EL2",
                 "MPAM Virtual PARTID Mapping Register 0",
                 "11", "100", "1010", "0110", "000",
                 0xe530,
                 {
                         {"PhyPARTID3", 63, 48},
                         {"PhyPARTID2", 47, 32},
                         {"PhyPARTID1", 31, 16},
                         {"PhyPARTID0", 15, 0},
                 }
         },
         {
                 "MPAMVPM1_EL2",
                 "MPAM Virtual PARTID Mapping Register 1",
                 "11", "100", "1010", "0110", "001",
                 0xe531,
                 {
                         {"PhyPARTID7", 63, 48},
                         {"PhyPARTID6", 47, 32},
                         {"PhyPARTID5", 31, 16},
                         {"PhyPARTID4", 15, 0},
                 }
         },
         {
                 "MPAMVPM2_EL2",
                 "MPAM Virtual PARTID Mapping Register 2",
                 "11", "100", "1010", "0110", "010",
                 0xe532,
                 {
                         {"PhyPARTID11", 63, 48},
                         {"PhyPARTID10", 47, 32},
                         {"PhyPARTID9", 31, 16},
                         {"PhyPARTID8", 15, 0},
                 }
         },
         {
                 "MPAMVPM3_EL2",
                 "MPAM Virtual PARTID Mapping Register 3",
                 "11", "100", "1010", "0110", "011",
                 0xe533,
                 {
                         {"PhyPARTID15", 63, 48},
                         {"PhyPARTID14", 47, 32},
                         {"PhyPARTID13", 31, 16},
                         {"PhyPARTID12", 15, 0},
                 }
         },
         {
                 "MPAMVPM4_EL2",
                 "MPAM Virtual PARTID Mapping Register 4",
                 "11", "100", "1010", "0110", "100",
                 0xe534,
                 {
                         {"PhyPARTID19", 63, 48},
                         {"PhyPARTID18", 47, 32},
                         {"PhyPARTID17", 31, 16},
                         {"PhyPARTID16", 15, 0},
                 }
         },
         {
                 "MPAMVPM5_EL2",
                 "MPAM Virtual PARTID Mapping Register 5",
                 "11", "100", "1010", "0110", "101",
                 0xe535,
                 {
                         {"PhyPARTID23", 63, 48},
                         {"PhyPARTID22", 47, 32},
                         {"PhyPARTID21", 31, 16},
                         {"PhyPARTID20", 15, 0},
                 }
         },
         {
                 "MPAMVPM6_EL2",
                 "MPAM Virtual PARTID Mapping Register 6",
                 "11", "100", "1010", "0110", "110",
                 0xe536,
                 {
                         {"PhyPARTID27", 63, 48},
                         {"PhyPARTID26", 47, 32},
                         {"PhyPARTID25", 31, 16},
                         {"PhyPARTID24", 15, 0},
                 }
         },
         {
                 "MPAMVPM7_EL2",
                 "MPAM Virtual PARTID Mapping Register 7",
                 "11", "100", "1010", "0110", "111",
                 0xe537,
                 {
                         {"PhyPARTID31", 63, 48},
                         {"PhyPARTID30", 47, 32},
                         {"PhyPARTID29", 31, 16},
                         {"PhyPARTID28", 15, 0},
                 }
         },
         {
                 "VBAR_EL2",
                 "Vector Base Address Register (EL2)",
                 "11", "100", "1100", "0000", "000",
                 0xe600,
                 {
                         {"None", 63, 11},
                 }
         },
         {
                 "RVBAR_EL2",
                 "Reset Vector Base Address Register (if EL3 not implemented)",
                 "11", "100", "1100", "0000", "001",
                 0xe601,
                 {
                         {"ResetAddress", 63, 0},
                 }
         },
         {
                 "RMR_EL2",
                 "Reset Management Register (EL2)",
                 "11", "100", "1100", "0000", "010",
                 0xe602,
                 {
                         {"RR", 1, 1},
                         {"AA64", 0, 0},
                         {"None", 0, 0},
                 }
         },
         {
                 "VDISR_EL2",
                 "Virtual Deferred Interrupt Status Register",
                 "11", "100", "1100", "0001", "001",
                 0xe609,
                 {
                         {"A", 31, 31},
                         {"IDS", 24, 24},
                         {"ISS", 23, 0},
                 }
         },
         {
                 "ICH_AP0R0_EL2",
                 "Interrupt Controller Hyp Active Priorities Group 0 Registers",
                 "11", "100", "1100", "1000", "000",
                 0xe640,
                 {
                         {"P<x>", 31, 0},
                 }
         },
         {
                 "ICH_AP0R1_EL2",
                 "Interrupt Controller Hyp Active Priorities Group 0 Registers",
                 "11", "100", "1100", "1000", "001",
                 0xe641,
                 {
                         {"P<x>", 31, 0},
                 }
         },
         {
                 "ICH_AP0R2_EL2",
                 "Interrupt Controller Hyp Active Priorities Group 0 Registers",
                 "11", "100", "1100", "1000", "010",
                 0xe642,
                 {
                         {"P<x>", 31, 0},
                 }
         },
         {
                 "ICH_AP0R3_EL2",
                 "Interrupt Controller Hyp Active Priorities Group 0 Registers",
                 "11", "100", "1100", "1000", "011",
                 0xe643,
                 {
                         {"P<x>", 31, 0},
                 }
         },
         {
                 "ICH_AP1R0_EL2",
                 "Interrupt Controller Hyp Active Priorities Group 1 Registers",
                 "11", "100", "1100", "1001", "000",
                 0xe648,
                 {
                         {"P<x>", 31, 0},
                 }
         },
         {
                 "ICH_AP1R1_EL2",
                 "Interrupt Controller Hyp Active Priorities Group 1 Registers",
                 "11", "100", "1100", "1001", "001",
                 0xe649,
                 {
                         {"P<x>", 31, 0},
                 }
         },
         {
                 "ICH_AP1R2_EL2",
                 "Interrupt Controller Hyp Active Priorities Group 1 Registers",
                 "11", "100", "1100", "1001", "010",
                 0xe64a,
                 {
                         {"P<x>", 31, 0},
                 }
         },
         {
                 "ICH_AP1R3_EL2",
                 "Interrupt Controller Hyp Active Priorities Group 1 Registers",
                 "11", "100", "1100", "1001", "011",
                 0xe64b,
                 {
                         {"P<x>", 31, 0},
                 }
         },
         {
                 "ICC_SRE_EL2",
                 "Interrupt Controller System Register Enable register (EL2)",
                 "11", "100", "1100", "1001", "101",
                 0xe64d,
                 {
                         {"Enable", 3, 3},
                         {"DIB", 2, 2},
                         {"DFB", 1, 1},
                         {"SRE", 0, 0},
                 }
         },
         {
                 "ICH_HCR_EL2",
                 "Interrupt Controller Hyp Control Register",
                 "11", "100", "1100", "1011", "000",
                 0xe658,
                 {
                         {"EOIcount", 31, 27},
                         {"DVIM", 15, 15},
                         {"TDIR", 14, 14},
                         {"TSEI", 13, 13},
                         {"TALL1", 12, 12},
                         {"TALL0", 11, 11},
                         {"TC", 10, 10},
                         {"vSGIEOICount", 8, 8},
                         {"VGrp1DIE", 7, 7},
                         {"VGrp1EIE", 6, 6},
                         {"VGrp0DIE", 5, 5},
                         {"VGrp0EIE", 4, 4},
                         {"NPIE", 3, 3},
                         {"LRENPIE", 2, 2},
                         {"UIE", 1, 1},
                         {"En", 0, 0},
                 }
         },
         {
                 "ICH_VTR_EL2",
                 "Interrupt Controller VGIC Type Register",
                 "11", "100", "1100", "1011", "001",
                 0xe659,
                 {
                         {"PRIbits", 31, 29},
                         {"PREbits", 28, 26},
                         {"IDbits", 25, 23},
                         {"SEIS", 22, 22},
                         {"A3V", 21, 21},
                         {"nV4", 20, 20},
                         {"TDS", 19, 19},
                         {"DVIM", 18, 18},
                         {"ListRegs", 4, 0},
                 }
         },
         {
                 "ICH_MISR_EL2",
                 "Interrupt Controller Maintenance Interrupt State Register",
                 "11", "100", "1100", "1011", "010",
                 0xe65a,
                 {
                         {"VGrp1D", 7, 7},
                         {"VGrp1E", 6, 6},
                         {"VGrp0D", 5, 5},
                         {"VGrp0E", 4, 4},
                         {"NP", 3, 3},
                         {"LRENP", 2, 2},
                         {"U", 1, 1},
                         {"EOI", 0, 0},
                 }
         },
         {
                 "ICH_EISR_EL2",
                 "Interrupt Controller End of Interrupt Status Register",
                 "11", "100", "1100", "1011", "011",
                 0xe65b,
                 {
                         {"Status<n>", 15, 0},
                 }
         },
         {
                 "ICH_ELRSR_EL2",
                 "Interrupt Controller Empty List Register Status Register",
                 "11", "100", "1100", "1011", "101",
                 0xe65d,
                 {
                         {"Status<n>", 15, 0},
                 }
         },
         {
                 "ICH_VMCR_EL2",
                 "Interrupt Controller Virtual Machine Control Register",
                 "11", "100", "1100", "1011", "111",
                 0xe65f,
                 {
                         {"VPMR", 31, 24},
                         {"VBPR0", 23, 21},
                         {"VBPR1", 20, 18},
                         {"VEOIM", 9, 9},
                         {"VCBPR", 4, 4},
                         {"VFIQEn", 3, 3},
                         {"VAckCtl", 2, 2},
                         {"VENG1", 1, 1},
                         {"VENG0", 0, 0},
                 }
         },
         {
                 "ICH_LR0_EL2",
                 "Interrupt Controller List Registers",
                 "11", "100", "1100", "1100", "000",
                 0xe660,
                 {
                         {"State", 63, 62},
                         {"HW", 61, 61},
                         {"Group", 60, 60},
                         {"Priority", 55, 48},
                         {"pINTID", 44, 32},
                         {"vINTID", 31, 0},
                 }
         },
         {
                 "ICH_LR1_EL2",
                 "Interrupt Controller List Registers",
                 "11", "100", "1100", "1100", "001",
                 0xe661,
                 {
                         {"State", 63, 62},
                         {"HW", 61, 61},
                         {"Group", 60, 60},
                         {"Priority", 55, 48},
                         {"pINTID", 44, 32},
                         {"vINTID", 31, 0},
                 }
         },
         {
                 "ICH_LR2_EL2",
                 "Interrupt Controller List Registers",
                 "11", "100", "1100", "1100", "010",
                 0xe662,
                 {
                         {"State", 63, 62},
                         {"HW", 61, 61},
                         {"Group", 60, 60},
                         {"Priority", 55, 48},
                         {"pINTID", 44, 32},
                         {"vINTID", 31, 0},
                 }
         },
         {
                 "ICH_LR3_EL2",
                 "Interrupt Controller List Registers",
                 "11", "100", "1100", "1100", "011",
                 0xe663,
                 {
                         {"State", 63, 62},
                         {"HW", 61, 61},
                         {"Group", 60, 60},
                         {"Priority", 55, 48},
                         {"pINTID", 44, 32},
                         {"vINTID", 31, 0},
                 }
         },
         {
                 "ICH_LR4_EL2",
                 "Interrupt Controller List Registers",
                 "11", "100", "1100", "1100", "100",
                 0xe664,
                 {
                         {"State", 63, 62},
                         {"HW", 61, 61},
                         {"Group", 60, 60},
                         {"Priority", 55, 48},
                         {"pINTID", 44, 32},
                         {"vINTID", 31, 0},
                 }
         },
         {
                 "ICH_LR5_EL2",
                 "Interrupt Controller List Registers",
                 "11", "100", "1100", "1100", "101",
                 0xe665,
                 {
                         {"State", 63, 62},
                         {"HW", 61, 61},
                         {"Group", 60, 60},
                         {"Priority", 55, 48},
                         {"pINTID", 44, 32},
                         {"vINTID", 31, 0},
                 }
         },
         {
                 "ICH_LR6_EL2",
                 "Interrupt Controller List Registers",
                 "11", "100", "1100", "1100", "110",
                 0xe666,
                 {
                         {"State", 63, 62},
                         {"HW", 61, 61},
                         {"Group", 60, 60},
                         {"Priority", 55, 48},
                         {"pINTID", 44, 32},
                         {"vINTID", 31, 0},
                 }
         },
         {
                 "ICH_LR7_EL2",
                 "Interrupt Controller List Registers",
                 "11", "100", "1100", "1100", "111",
                 0xe667,
                 {
                         {"State", 63, 62},
                         {"HW", 61, 61},
                         {"Group", 60, 60},
                         {"Priority", 55, 48},
                         {"pINTID", 44, 32},
                         {"vINTID", 31, 0},
                 }
         },
         {
                 "ICH_LR8_EL2",
                 "Interrupt Controller List Registers",
                 "11", "100", "1100", "1101", "000",
                 0xe668,
                 {
                         {"State", 63, 62},
                         {"HW", 61, 61},
                         {"Group", 60, 60},
                         {"Priority", 55, 48},
                         {"pINTID", 44, 32},
                         {"vINTID", 31, 0},
                 }
         },
         {
                 "ICH_LR9_EL2",
                 "Interrupt Controller List Registers",
                 "11", "100", "1100", "1101", "001",
                 0xe669,
                 {
                         {"State", 63, 62},
                         {"HW", 61, 61},
                         {"Group", 60, 60},
                         {"Priority", 55, 48},
                         {"pINTID", 44, 32},
                         {"vINTID", 31, 0},
                 }
         },
         {
                 "ICH_LR10_EL2",
                 "Interrupt Controller List Registers",
                 "11", "100", "1100", "1101", "010",
                 0xe66a,
                 {
                         {"State", 63, 62},
                         {"HW", 61, 61},
                         {"Group", 60, 60},
                         {"Priority", 55, 48},
                         {"pINTID", 44, 32},
                         {"vINTID", 31, 0},
                 }
         },
         {
                 "ICH_LR11_EL2",
                 "Interrupt Controller List Registers",
                 "11", "100", "1100", "1101", "011",
                 0xe66b,
                 {
                         {"State", 63, 62},
                         {"HW", 61, 61},
                         {"Group", 60, 60},
                         {"Priority", 55, 48},
                         {"pINTID", 44, 32},
                         {"vINTID", 31, 0},
                 }
         },
         {
                 "ICH_LR12_EL2",
                 "Interrupt Controller List Registers",
                 "11", "100", "1100", "1101", "100",
                 0xe66c,
                 {
                         {"State", 63, 62},
                         {"HW", 61, 61},
                         {"Group", 60, 60},
                         {"Priority", 55, 48},
                         {"pINTID", 44, 32},
                         {"vINTID", 31, 0},
                 }
         },
         {
                 "ICH_LR13_EL2",
                 "Interrupt Controller List Registers",
                 "11", "100", "1100", "1101", "101",
                 0xe66d,
                 {
                         {"State", 63, 62},
                         {"HW", 61, 61},
                         {"Group", 60, 60},
                         {"Priority", 55, 48},
                         {"pINTID", 44, 32},
                         {"vINTID", 31, 0},
                 }
         },
         {
                 "ICH_LR14_EL2",
                 "Interrupt Controller List Registers",
                 "11", "100", "1100", "1101", "110",
                 0xe66e,
                 {
                         {"State", 63, 62},
                         {"HW", 61, 61},
                         {"Group", 60, 60},
                         {"Priority", 55, 48},
                         {"pINTID", 44, 32},
                         {"vINTID", 31, 0},
                 }
         },
         {
                 "ICH_LR15_EL2",
                 "Interrupt Controller List Registers",
                 "11", "100", "1100", "1101", "111",
                 0xe66f,
                 {
                         {"State", 63, 62},
                         {"HW", 61, 61},
                         {"Group", 60, 60},
                         {"Priority", 55, 48},
                         {"pINTID", 44, 32},
                         {"vINTID", 31, 0},
                 }
         },
         {
                 "CONTEXTIDR_EL2",
                 "Context ID Register (EL2)",
                 "11", "100", "1101", "0000", "001",
                 0xe681,
                 {
                         {"PROCID", 31, 0},
                 }
         },
         {
                 "TPIDR_EL2",
                 "EL2 Software Thread ID Register",
                 "11", "100", "1101", "0000", "010",
                 0xe682,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "SCXTNUM_EL2",
                 "EL2 Read/Write Software Context Number",
                 "11", "100", "1101", "0000", "111",
                 0xe687,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF00_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 0",
                 "11", "100", "1101", "1000", "000",
                 0xe6c0,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF01_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 0",
                 "11", "100", "1101", "1000", "001",
                 0xe6c1,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF02_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 0",
                 "11", "100", "1101", "1000", "010",
                 0xe6c2,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF03_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 0",
                 "11", "100", "1101", "1000", "011",
                 0xe6c3,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF04_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 0",
                 "11", "100", "1101", "1000", "100",
                 0xe6c4,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF05_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 0",
                 "11", "100", "1101", "1000", "101",
                 0xe6c5,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF06_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 0",
                 "11", "100", "1101", "1000", "110",
                 0xe6c6,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF07_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 0",
                 "11", "100", "1101", "1000", "111",
                 0xe6c7,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF08_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 0",
                 "11", "100", "1101", "1001", "000",
                 0xe6c8,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF09_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 0",
                 "11", "100", "1101", "1001", "001",
                 0xe6c9,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF010_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 0",
                 "11", "100", "1101", "1001", "010",
                 0xe6ca,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF011_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 0",
                 "11", "100", "1101", "1001", "011",
                 0xe6cb,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF012_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 0",
                 "11", "100", "1101", "1001", "100",
                 0xe6cc,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF013_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 0",
                 "11", "100", "1101", "1001", "101",
                 0xe6cd,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF014_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 0",
                 "11", "100", "1101", "1001", "110",
                 0xe6ce,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF015_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 0",
                 "11", "100", "1101", "1001", "111",
                 0xe6cf,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF10_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 1",
                 "11", "100", "1101", "1010", "000",
                 0xe6d0,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF11_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 1",
                 "11", "100", "1101", "1010", "001",
                 0xe6d1,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF12_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 1",
                 "11", "100", "1101", "1010", "010",
                 0xe6d2,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF13_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 1",
                 "11", "100", "1101", "1010", "011",
                 0xe6d3,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF14_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 1",
                 "11", "100", "1101", "1010", "100",
                 0xe6d4,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF15_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 1",
                 "11", "100", "1101", "1010", "101",
                 0xe6d5,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF16_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 1",
                 "11", "100", "1101", "1010", "110",
                 0xe6d6,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF17_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 1",
                 "11", "100", "1101", "1010", "111",
                 0xe6d7,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF18_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 1",
                 "11", "100", "1101", "1011", "000",
                 0xe6d8,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF19_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 1",
                 "11", "100", "1101", "1011", "001",
                 0xe6d9,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF110_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 1",
                 "11", "100", "1101", "1011", "010",
                 0xe6da,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF111_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 1",
                 "11", "100", "1101", "1011", "011",
                 0xe6db,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF112_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 1",
                 "11", "100", "1101", "1011", "100",
                 0xe6dc,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF113_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 1",
                 "11", "100", "1101", "1011", "101",
                 0xe6dd,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF114_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 1",
                 "11", "100", "1101", "1011", "110",
                 0xe6de,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "AMEVCNTVOFF115_EL2",
                 "Activity Monitors Event Counter Virtual Offset Registers 1",
                 "11", "100", "1101", "1011", "111",
                 0xe6df,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "CNTVOFF_EL2",
                 "Counter-timer Virtual Offset register",
                 "11", "100", "1110", "0000", "011",
                 0xe703,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "CNTPOFF_EL2",
                 "Counter-timer Physical Offset register",
                 "11", "100", "1110", "0000", "110",
                 0xe706,
                 {
                         {"None", 63, 0},
                 }
         },
         {
                 "CNTHCTL_EL2",
                 "Counter-timer Hypervisor Control register",
                 "11", "100", "1110", "0001", "000",
                 0xe708,
                 {
                         {"CNTPMASK", 19, 19},
                         {"CNTVMASK", 18, 18},
                         {"EVNTIS", 17, 17},
                         {"EL1NVVCT", 16, 16},
                         {"EL1NVPCT", 15, 15},
                         {"EL1TVCT", 14, 14},
                         {"EL1TVT", 13, 13},
                         {"ECV", 12, 12},
                         {"EL1PTEN", 11, 11},
                         {"EL1PCTEN", 10, 10},
                         {"EL0PTEN", 9, 9},
                         {"EL0VTEN", 8, 8},
                         {"EVNTI", 7, 4},
                         {"EVNTDIR", 3, 3},
                         {"EVNTEN", 2, 2},
                         {"EL0VCTEN", 1, 1},
                         {"EL0PCTEN", 0, 0},
                 }
         },
         {
                 "CNTHP_TVAL_EL2",
                 "Counter-timer Physical Timer TimerValue register (EL2)",
                 "11", "100", "1110", "0010", "000",
                 0xe710,
                 {
                         {"TimerValue", 31, 0},
                 }
         },
         {
                 "CNTHP_CTL_EL2",
                 "Counter-timer Hypervisor Physical Timer Control register",
                 "11", "100", "1110", "0010", "001",
                 0xe711,
                 {
                         {"ISTATUS", 2, 2},
                         {"IMASK", 1, 1},
                         {"ENABLE", 0, 0},
                 }
         },
         {
                 "CNTHP_CVAL_EL2",
                 "Counter-timer Physical Timer CompareValue register (EL2)",
                 "11", "100", "1110", "0010", "010",
                 0xe712,
                 {
                         {"CompareValue", 63, 0},
                 }
         },
         {
                 "CNTHV_TVAL_EL2",
                 "Counter-timer Virtual Timer TimerValue Register (EL2)",
                 "11", "100", "1110", "0011", "000",
                 0xe718,
                 {
                         {"TimerValue", 31, 0},
                 }
         },
         {
                 "CNTHV_CTL_EL2",
                 "Counter-timer Virtual Timer Control register (EL2)",
                 "11", "100", "1110", "0011", "001",
                 0xe719,
                 {
                         {"ISTATUS", 2, 2},
                         {"IMASK", 1, 1},
                         {"ENABLE", 0, 0},
                 }
         },
        {
                "CNTHV_CVAL_EL2",
                "Counter-timer Virtual Timer CompareValue register (EL2)",
                "11", "100", "1110", "0011", "010",
                0xe71a,
                {
                        {"CompareValue", 63, 0},
                }
        },
        {
                "CNTHVS_TVAL_EL2",
                "Counter-timer Secure Virtual Timer TimerValue register (EL2)",
                "11", "100", "1110", "0100", "000",
                0xe720,
                {
                        {"TimerValue", 31, 0},
                }
        },
        {
                "CNTHVS_CTL_EL2",
                "Counter-timer Secure Virtual Timer Control register (EL2)",
                "11", "100", "1110", "0100", "001",
                0xe721,
                {
                        {"ISTATUS", 2, 2},
                        {"IMASK", 1, 1},
                        {"ENABLE", 0, 0},
                }
        },
        {
                "CNTHVS_CVAL_EL2",
                "Counter-timer Secure Virtual Timer CompareValue register (EL2)",
                "11", "100", "1110", "0100", "010",
                0xe722,
                {
                        {"CompareValue", 63, 0},
                }
        },
        {
                "CNTHPS_TVAL_EL2",
                "Counter-timer Secure Physical Timer TimerValue register (EL2)",
                "11", "100", "1110", "0101", "000",
                0xe728,
                {
                        {"TimerValue", 31, 0},
                }
        },
        {
                "CNTHPS_CTL_EL2",
                "Counter-timer Secure Physical Timer Control register (EL2)",
                "11", "100", "1110", "0101", "001",
                0xe729,
                {
                        {"ISTATUS", 2, 2},
                        {"IMASK", 1, 1},
                        {"ENABLE", 0, 0},
                }
        },
        {
                "CNTHPS_CVAL_EL2",
                "Counter-timer Secure Physical Timer CompareValue register (EL2)",
                "11", "100", "1110", "0101", "010",
                0xe72a,
                {
                        {"CompareValue", 63, 0},
                }
        },
        {
                "SCTLR_EL3",
                "System Control Register (EL3)",
                "11", "110", "0001", "0000", "000",
                0xf080,
                {
                        {"TME", 53, 53},
                        {"TMT", 51, 51},
                        {"DSSBS", 44, 44},
                        {"ATA", 43, 43},
                        {"TCF", 41, 40},
                        {"ITFSB", 37, 37},
                        {"BT", 36, 36},
                        {"EnIA", 31, 31},
                        {"EnIB", 30, 30},
                        {"EnDA", 27, 27},
                        {"EE", 25, 25},
                        {"EIS", 22, 22},
                        {"IESB", 21, 21},
                        {"WXN", 19, 19},
                        {"EnDB", 13, 13},
                        {"I", 12, 12},
                        {"EOS", 11, 11},
                        {"nAA", 6, 6},
                        {"SA", 3, 3},
                        {"C", 2, 2},
                        {"A", 1, 1},
                        {"M", 0, 0},
                }
        },
        {
                "ACTLR_EL3",
                "Auxiliary Control Register (EL3)",
                "11", "110", "0001", "0000", "001",
                0xf081,
                {
                        {"IMPLEMENTATION DEFINED", 63, 0},
                }
        },
        {
                "SCR_EL3",
                "Secure Configuration Register",
                "11", "110", "0001", "0001", "000",
                0xf088,
                {
                        {"NSE", 62, 62},
                        {"GPF", 48, 48},
                        {"EnTP2", 41, 41},
                        {"HXEn", 38, 38},
                        {"ADEn", 37, 37},
                        {"EnAS0", 36, 36},
                        {"AMVOFFEN", 35, 35},
                        {"TME", 34, 34},
                        {"TWEDEL", 33, 30},
                        {"TWEDEn", 29, 29},
                        {"ECVEn", 28, 28},
                        {"FGTEn", 27, 27},
                        {"ATA", 26, 26},
                        {"EnSCXT", 25, 25},
                        {"FIEN", 21, 21},
                        {"NMEA", 20, 20},
                        {"EASE", 19, 19},
                        {"EEL2", 18, 18},
                        {"API", 17, 17},
                        {"API", 17, 17},
                        {"APK", 16, 16},
                        {"TERR", 15, 15},
                        {"TLOR", 14, 14},
                        {"TWE", 13, 13},
                        {"TWI", 12, 12},
                        {"ST", 11, 11},
                        {"RW", 10, 10},
                        {"None", 10, 10},
                        {"SIF", 9, 9},
                        {"SIF", 9, 9},
                        {"HCE", 8, 8},
                        {"SMD", 7, 7},
                        {"EA", 3, 3},
                        {"FIQ", 2, 2},
                        {"IRQ", 1, 1},
                        {"NS", 0, 0},
                        {"NS", 0, 0},
                }
        },
        {
                "SDER32_EL3",
                "AArch32 Secure Debug Enable Register",
                "11", "110", "0001", "0001", "001",
                0xf089,
                {
                        {"SUNIDEN", 1, 1},
                        {"SUIDEN", 0, 0},
                }
        },
        {
                "CPTR_EL3",
                "Architectural Feature Trap Register (EL3)",
                "11", "110", "0001", "0001", "010",
                0xf08a,
                {
                        {"TCPAC", 31, 31},
                        {"TAM", 30, 30},
                        {"TTA", 20, 20},
                        {"ESM", 12, 12},
                        {"TFP", 10, 10},
                        {"EZ", 8, 8},
                }
        },
        {
                "ZCR_EL3",
                "SVE Control Register (EL3)",
                "11", "110", "0001", "0010", "000",
                0xf090,
                {
                        {"None", 8, 4},
                        {"LEN", 3, 0},
                }
        },
        {
                "SMCR_EL3",
                "SME Control Register (EL3)",
                "11", "110", "0001", "0010", "110",
                0xf096,
                {
                        {"None", 8, 4},
                        {"LEN", 3, 0},
                }
        },
        {
                "MDCR_EL3",
                "Monitor Debug Configuration Register (EL3)",
                "11", "110", "0001", "0011", "001",
                0xf099,
                {
                        {"EnPMSN", 36, 36},
                        {"MPMX", 35, 35},
                        {"MCCD", 34, 34},
                        {"SBRBE", 33, 32},
                        {"MTPME", 28, 28},
                        {"TDCC", 27, 27},
                        {"NSTBE", 26, 26},
                        {"NSTB", 25, 24},
                        {"NSTB", 25, 24},
                        {"SCCD", 23, 23},
                        {"ETAD", 22, 22},
                        {"ETAD", 22, 22},
                        {"EPMAD", 21, 21},
                        {"EPMAD", 21, 21},
                        {"EPMAD", 21, 21},
                        {"EDAD", 20, 20},
                        {"EDAD", 20, 20},
                        {"EDAD", 20, 20},
                        {"EDAD", 20, 20},
                        {"TTRF", 19, 19},
                        {"STE", 18, 18},
                        {"SPME", 17, 17},
                        {"SPME", 17, 17},
                        {"SPME", 17, 17},
                        {"SDD", 16, 16},
                        {"SPD32", 15, 14},
                        {"NSPB", 13, 12},
                        {"NSPB", 13, 12},
                        {"NSPBE", 11, 11},
                        {"TDOSA", 10, 10},
                        {"TDOSA", 10, 10},
                        {"TDA", 9, 9},
                        {"TPM", 6, 6},
                        {"EDADE", 4, 4},
                        {"ETADE", 3, 3},
                        {"EPMADE", 2, 2},
                        {"RTTE", 1, 1},
                        {"RLTE", 0, 0},
                }
        },
        {
                "TTBR0_EL3",
                "Translation Table Base Register 0 (EL3)",
                "11", "110", "0010", "0000", "000",
                0xf100,
                {
                        {"BADDR[47:1]", 47, 1},
                        {"CnP", 0, 0},
                }
        },
        {
                "TCR_EL3",
                "Translation Control Register (EL3)",
                "11", "110", "0010", "0000", "010",
                0xf102,
                {
                        {"DS", 32, 32},
                        {"TCMA", 30, 30},
                        {"TBID", 29, 29},
                        {"HWU62", 28, 28},
                        {"None", 28, 28},
                        {"HWU61", 27, 27},
                        {"None", 27, 27},
                        {"HWU60", 26, 26},
                        {"None", 26, 26},
                        {"HWU59", 25, 25},
                        {"None", 25, 25},
                        {"HPD", 24, 24},
                        {"HD", 22, 22},
                        {"HA", 21, 21},
                        {"TBI", 20, 20},
                        {"PS", 18, 16},
                        {"TG0", 15, 14},
                        {"SH0", 13, 12},
                        {"ORGN0", 11, 10},
                        {"IRGN0", 9, 8},
                        {"T0SZ", 5, 0},
                }
        },
        {
                "GPTBR_EL3",
                "Granule Protection Table Base Register",
                "11", "110", "0010", "0001", "100",
                0xf10c,
                {
                        {"BADDR", 39, 0},
                }
        },
        {
                "GPCCR_EL3",
                "Granule Protection Check Control Register (EL3)",
                "11", "110", "0010", "0001", "110",
                0xf10e,
                {
                        {"L0GPTSZ", 23, 20},
                        {"GPCP", 17, 17},
                        {"GPC", 16, 16},
                        {"PGS", 15, 14},
                        {"SH", 13, 12},
                        {"ORGN", 11, 10},
                        {"IRGN", 9, 8},
                        {"PPS", 2, 0},
                }
        },
        {
                "SPSR_EL3",
                "Saved Program Status Register (EL3)",
                "11", "110", "0100", "0000", "000",
                0xf200,
                {
                        {"N", 31, 31},
                        {"Z", 30, 30},
                        {"C", 29, 29},
                        {"V", 28, 28},
                        {"Q", 27, 27},
                        {"IT", 26, 25},
                        {"DIT", 24, 24},
                        {"SSBS", 23, 23},
                        {"PAN", 22, 22},
                        {"SS", 21, 21},
                        {"IL", 20, 20},
                        {"GE", 19, 16},
                        {"IT[7:2]", 15, 10},
                        {"E", 9, 9},
                        {"A", 8, 8},
                        {"I", 7, 7},
                        {"F", 6, 6},
                        {"T", 5, 5},
                        {"M[4]", 4, 4},
                        {"M[3:0]", 3, 0},
                }
        },
        {
                "ELR_EL3",
                "Exception Link Register (EL3)",
                "11", "110", "0100", "0000", "001",
                0xf201,
                {
                        {"None", 63, 0},
                }
        },
        {
                "SP_EL2",
                "Stack Pointer (EL2)",
                "11", "110", "0100", "0001", "000",
                0xf208,
                {
                        {"None", 63, 0},
                }
        },
        {
                "AFSR0_EL3",
                "Auxiliary Fault Status Register 0 (EL3)",
                "11", "110", "0101", "0001", "000",
                0xf288,
                {
                        {"IMPLEMENTATION DEFINED", 63, 0},
                }
        },
        {
                "AFSR1_EL3",
                "Auxiliary Fault Status Register 1 (EL3)",
                "11", "110", "0101", "0001", "001",
                0xf289,
                {
                        {"IMPLEMENTATION DEFINED", 63, 0},
                }
        },
        {
                "ESR_EL3",
                "Exception Syndrome Register (EL3)",
                "11", "110", "0101", "0010", "000",
                0xf290,
                {
                        {"ISS2", 36, 32},
                        {"EC", 31, 26},
                        {"IL", 25, 25},
                        {"ISS", 24, 0},
                }
        },
        {
                "TFSR_EL3",
                "Tag Fault Status Register (EL3)",
                "11", "110", "0101", "0110", "000",
                0xf2b0,
                {
                        {"TF0", 0, 0},
                }
        },
        {
                "FAR_EL3",
                "Fault Address Register (EL3)",
                "11", "110", "0110", "0000", "000",
                0xf300,
                {
                        {"None", 63, 0},
                }
        },
        {
                "MFAR_EL3",
                "PA Fault Address Register",
                "11", "110", "0110", "0000", "101",
                0xf305,
                {
                        {"NS", 63, 63},
                        {"NSE", 62, 62},
                        {"FPA[51:48]", 51, 48},
                        {"FPA[47:12]", 47, 12},
                }
        },
        {
                "MAIR_EL3",
                "Memory Attribute Indirection Register (EL3)",
                "11", "110", "1010", "0010", "000",
                0xf510,
                {
                        {"Attr<n>", 63, 0},
                }
        },
        {
                "AMAIR_EL3",
                "Auxiliary Memory Attribute Indirection Register (EL3)",
                "11", "110", "1010", "0011", "000",
                0xf518,
                {
                        {"IMPLEMENTATION DEFINED", 63, 0},
                }
        },
        {
                "MPAM3_EL3",
                "MPAM3 Register (EL3)",
                "11", "110", "1010", "0101", "000",
                0xf528,
                {
                        {"MPAMEN", 63, 63},
                        {"TRAPLOWER", 62, 62},
                        {"SDEFLT", 61, 61},
                        {"FORCE_NS", 60, 60},
                        {"ALTSP_HEN", 57, 57},
                        {"ALTSP_HFC", 56, 56},
                        {"ALTSP_EL3", 55, 55},
                        {"RT_ALTSP_NS", 52, 52},
                        {"PMG_D", 47, 40},
                        {"PMG_I", 39, 32},
                        {"PARTID_D", 31, 16},
                        {"PARTID_I", 15, 0},
                }
        },
        {
                "VBAR_EL3",
                "Vector Base Address Register (EL3)",
                "11", "110", "1100", "0000", "000",
                0xf600,
                {
                        {"None", 63, 11},
                }
        },
        {
                "RVBAR_EL3",
                "Reset Vector Base Address Register (if EL3 implemented)",
                "11", "110", "1100", "0000", "001",
                0xf601,
                {
                        {"ResetAddress", 63, 0},
                }
        },
        {
                "RMR_EL3",
                "Reset Management Register (EL3)",
                "11", "110", "1100", "0000", "010",
                0xf602,
                {
                        {"RR", 1, 1},
                        {"AA64", 0, 0},
                        {"None", 0, 0},
                }
        },
        {
                "ICC_CTLR_EL3",
                "Interrupt Controller Control Register (EL3)",
                "11", "110", "1100", "1100", "100",
                0xf664,
                {
                        {"ExtRange", 19, 19},
                        {"RSS", 18, 18},
                        {"nDS", 17, 17},
                        {"A3V", 15, 15},
                        {"SEIS", 14, 14},
                        {"IDbits", 13, 11},
                        {"PRIbits", 10, 8},
                        {"PMHE", 6, 6},
                        {"RM", 5, 5},
                        {"EOImode_EL1NS", 4, 4},
                        {"EOImode_EL1S", 3, 3},
                        {"EOImode_EL3", 2, 2},
                        {"CBPR_EL1NS", 1, 1},
                        {"CBPR_EL1S", 0, 0},
                }
        },
        {
                "ICC_SRE_EL3",
                "Interrupt Controller System Register Enable register (EL3)",
                "11", "110", "1100", "1100", "101",
                0xf665,
                {
                        {"Enable", 3, 3},
                        {"DIB", 2, 2},
                        {"DFB", 1, 1},
                        {"SRE", 0, 0},
                }
        },
        {
                "ICC_IGRPEN1_EL3",
                "Interrupt Controller Interrupt Group 1 Enable register (EL3)",
                "11", "110", "1100", "1100", "111",
                0xf667,
                {
                        {"EnableGrp1S", 1, 1},
                        {"EnableGrp1NS", 0, 0},
                }
        },
        {
                "TPIDR_EL3",
                "EL3 Software Thread ID Register",
                "11", "110", "1101", "0000", "010",
                0xf682,
                {
                        {"None", 63, 0},
                }
        },
        {
                "SCXTNUM_EL3",
                "EL3 Read/Write Software Context Number",
                "11", "110", "1101", "0000", "111",
                0xf687,
                {
                        {"None", 63, 0},
                }
        },
        {
                "CNTPS_TVAL_EL1",
                "Counter-timer Physical Secure Timer TimerValue register",
                "11", "111", "1110", "0010", "000",
                0xff10,
                {
                        {"TimerValue", 31, 0},
                }
        },
        {
                "CNTPS_CTL_EL1",
                "Counter-timer Physical Secure Timer Control register",
                "11", "111", "1110", "0010", "001",
                0xff11,
                {
                        {"ISTATUS", 2, 2},
                        {"IMASK", 1, 1},
                        {"ENABLE", 0, 0},
                }
        },
        {
                "CNTPS_CVAL_EL1",
                "Counter-timer Physical Secure Timer CompareValue register",
                "11", "111", "1110", "0010", "010",
                0xff12,
                {
                        {"CompareValue", 63, 0},
                }
        },
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, { { NULL, 0, 0 } } }
};