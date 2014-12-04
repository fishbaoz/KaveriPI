/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM OEM API, and related functions.
 *
 * Contains the table definition for Pcie Clock.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  OEM
 * @e \$Revision: 281158 $   @e \$Date: 2013-12-17 20:36:04 -0600 (Tue, 17 Dec 2013) $
 *
/*****************************************************************************
 *
 * Copyright 2012 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
 *
 * AMD is granting you permission to use this software (the Materials)
 * pursuant to the terms and conditions of your Software License Agreement
 * with AMD.  This header does *NOT* give you permission to use the Materials
 * or any rights under AMD's intellectual property.  Your use of any portion
 * of these Materials shall constitute your acceptance of those terms and
 * conditions.  If you do not agree to the terms and conditions of the Software
 * License Agreement, please do not use any portion of these Materials.
 *
 * CONFIDENTIALITY:  The Materials and all other information, identified as
 * confidential and provided to you by AMD shall be kept confidential in
 * accordance with the terms and conditions of the Software License Agreement.
 *
 * LIMITATION OF LIABILITY: THE MATERIALS AND ANY OTHER RELATED INFORMATION
 * PROVIDED TO YOU BY AMD ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE,
 * OR WARRANTIES ARISING FROM CONDUCT, COURSE OF DEALING, OR USAGE OF TRADE.
 * IN NO EVENT SHALL AMD OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES WHATSOEVER
 * (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF PROFITS, BUSINESS
 * INTERRUPTION, OR LOSS OF INFORMATION) ARISING OUT OF AMD'S NEGLIGENCE,
 * GROSS NEGLIGENCE, THE USE OF OR INABILITY TO USE THE MATERIALS OR ANY OTHER
 * RELATED INFORMATION PROVIDED TO YOU BY AMD, EVEN IF AMD HAS BEEN ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES.  BECAUSE SOME JURISDICTIONS PROHIBIT THE
 * EXCLUSION OR LIMITATION OF LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES,
 * THE ABOVE LIMITATION MAY NOT APPLY TO YOU.
 *
 * AMD does not assume any responsibility for any errors which may appear in
 * the Materials or any other related information provided to you by AMD, or
 * result from use of the Materials or any related information.
 *
 * You agree that you will not reverse engineer or decompile the Materials.
 *
 * NO SUPPORT OBLIGATION: AMD is not obligated to furnish, support, or make any
 * further information, software, technical information, know-how, or show-how
 * available to you.  Additionally, AMD retains the right to modify the
 * Materials at any time, without notice, and is not obligated to provide such
 * modified Materials to you.
 *
 * U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are provided with
 * "RESTRICTED RIGHTS." Use, duplication, or disclosure by the Government is
 * subject to the restrictions as set forth in FAR 52.227-14 and
 * DFAR252.227-7013, et seq., or its successor.  Use of the Materials by the
 * Government constitutes acknowledgement of AMD's proprietary rights in them.
 *
 * EXPORT ASSURANCE:  You agree and certify that neither the Materials, nor any
 * direct product thereof will be exported directly or indirectly, into any
 * country prohibited by the United States Export Administration Act and the
 * regulations thereunder, without the required authorization from the U.S.
 * government nor will be used for any purpose prohibited by the same.
 ******************************************************************************
 */
#ifndef _PCIE_CLOCK_TABLE_H_
#define _PCIE_CLOCK_TABLE_H_
#include "AmdCpmOemInitPeim.h"
//
// PCIE Clock Table
//
AMD_CPM_PCIE_CLOCK_TABLE    gCpmPcieClockTable = {
  {CPM_SIGNATURE_PCIE_CLOCK, sizeof (gCpmPcieClockTable) / sizeof (UINT8), 0, 0, 0x05, 0x0F},
  {
    //  ClkId           ClkReq          ClkIdExt        ClkReqExt       DeviceId            Device            Function           SlotCheck           SpecialFunctionId;
    {   GPP_CLK0,       CLK_REQ3,       SRC_CLK0,       CLK_REQ0,       DEVICE_ID_GFX1,     APU_GFX1_DEV_KV,  APU_GFX1_FUNC_KV,  NON_SLOT_CHECK,     0    },     // APU GFX 0-7
    {   GPP_CLK1,       CLK_REQ4,       SRC_CLK4,       CLK_DISABLE,    DEVICE_ID_APU_GPP,  APU_GPP0_DEV_KV,  APU_GPP0_FUNC_KV,  SLOT_CHECK,         0    },     // APU GPP x4 slot
    {   GPP_CLK2,       CLK_REQ2,       SRC_CLK6,       CLK_REQ6,       DEVICE_ID_SB_GPP3,  FCH_GPP3_DEV,     FCH_GPP3_FUNC,     SLOT_CHECK,         0    },     // FCH GPP3 x1 Mini PCIE
    {   GPP_CLK3,       CLK_REQ6,       SRC_CLK1,       CLK_REQ1,       DEVICE_ID_SB_GPP0,  FCH_GPP0_DEV,     FCH_GPP0_FUNC,     SLOT_CHECK,         0    },     // FCH GPP0 x1 slot
    {   GPP_CLK4,       CLK_ENABLE,     SRC_CLK2,       CLK_REQ2,       0xFF,               FCH_GPP2_DEV,     FCH_GPP2_FUNC,     NON_SLOT_CHECK,     0    },     // FCH GPP2 On Board LAN
    {   GPP_CLK5,       CLK_REQ8,       SRC_CLK5,       CLK_REQ5,       DEVICE_ID_SB_GPP1,  FCH_GPP1_DEV,     FCH_GPP1_FUNC,     SLOT_CHECK,         0    },     // FCH GPP1 x1 slot
    {   GPP_CLK6,       CLK_DISABLE,    SRC_CLK7,       CLK_DISABLE,    0xFF,               0,                0,                 NON_SLOT_CHECK,     0    },     // N/A
    {   GPP_CLK7,       CLK_DISABLE,    SRC_CLK9,       CLK_ENABLE,     0xFF,               0,                0,                 NON_SLOT_CHECK,     0    },     // N/A
    {   GPP_CLK8,       CLK_DISABLE,    SRC_CLK3,       CLK_REQ3,       0xFF,               0,                0,                 SLOT_CHECK,         0    },     // N/A
    {   GPP_CLK9,       CLK_REQ0,       SRC_CLK8,       CLK_REQ8,       DEVICE_ID_GFX0,     APU_GFX0_DEV_KV,  APU_GFX0_FUNC_KV,  SLOT_CHECK,         0    },     // APU GFX 0-15/8-15
    0xFF,
  }
};

AMD_CPM_PCIE_CLOCK_TABLE    gCpmPcieClockTable2 = {
  {CPM_SIGNATURE_PCIE_CLOCK, sizeof (gCpmPcieClockTable2) / sizeof (UINT8), 0, 0, 0x02, 0x0F},
  {
    //  ClkId           ClkReq          ClkIdExt        ClkReqExt       DeviceId            Device  Function  SlotCheck           SpecialFunctionId;
    {   GPP_CLK0,       CLK_REQ3,       SRC_CLK0,       CLK_REQ0,       DEVICE_ID_GFX1,     APU_GFX1_DEV_TN,  APU_GFX1_FUNC_TN,  NON_SLOT_CHECK,     0    },     // APU GFX 0-7
    {   GPP_CLK1,       CLK_REQ4,       SRC_CLK4,       CLK_DISABLE,    DEVICE_ID_APU_GPP,  APU_GPP0_DEV_TN,  APU_GPP0_FUNC_TN,  SLOT_CHECK,         0    },     // APU GPP x4 slot
    {   GPP_CLK2,       CLK_REQ2,       SRC_CLK6,       CLK_REQ6,       DEVICE_ID_SB_GPP3,  FCH_GPP3_DEV,     FCH_GPP3_FUNC,     SLOT_CHECK,         0    },     // FCH GPP3 x1 Mini PCIE
    {   GPP_CLK3,       CLK_REQ6,       SRC_CLK1,       CLK_REQ1,       DEVICE_ID_SB_GPP0,  FCH_GPP0_DEV,     FCH_GPP0_FUNC,     SLOT_CHECK,         0    },     // FCH GPP0 x1 slot
    {   GPP_CLK4,       CLK_ENABLE,     SRC_CLK2,       CLK_REQ2,       0xFF,               FCH_GPP2_DEV,     FCH_GPP2_FUNC,     NON_SLOT_CHECK,     0    },     // FCH GPP2 On Board LAN
    {   GPP_CLK5,       CLK_REQ8,       SRC_CLK5,       CLK_REQ5,       DEVICE_ID_SB_GPP0,  FCH_GPP1_DEV,     FCH_GPP1_FUNC,     SLOT_CHECK,         0    },     // FCH GPP1 x1 slot
    {   GPP_CLK6,       CLK_DISABLE,    SRC_CLK7,       CLK_DISABLE,    0xFF,               0,                0,                 NON_SLOT_CHECK,     0    },     // N/A
    {   GPP_CLK7,       CLK_DISABLE,    SRC_CLK9,       CLK_ENABLE,     0xFF,               0,                0,                 NON_SLOT_CHECK,     0    },     // N/A
    {   GPP_CLK8,       CLK_DISABLE,    SRC_CLK3,       CLK_REQ3,       0xFF,               0,                0,                 SLOT_CHECK,         0    },     // N/A
    {   GPP_CLK9,       CLK_REQ0,       SRC_CLK8,       CLK_REQ8,       DEVICE_ID_GFX0,     APU_GFX0_DEV_TN,  APU_GFX0_FUNC_TN,  SLOT_CHECK,         0    },     // APU GFX 0-15/8-15
    0xFF,
  }
};

//
// External ClkGen Table
//
AMD_CPM_EXT_CLKGEN_TABLE    gCpmExtClkGenTable = {
  {CPM_SIGNATURE_EXT_CLKGEN, sizeof (gCpmExtClkGenTable) / sizeof (UINT8), 0, 0, 0, 0x0F},
  0,                              // Smbus Select: 0: Smbus0. 1: Smbus1
  0x69,                           // Smbus Address: 0xD2
  {
    //  Function    Offset      AndMask     OrMask
//    {   0x80,       0x02,       0xFE,       0x00    },      // Clk0 Disable
//    {   0x81,       0x02,       0xFD,       0x00    },      // Clk1 Disable
//    {   0x82,       0x02,       0xFB,       0x00    },      // Clk2 Disable
//    {   0x83,       0x02,       0xF7,       0x00    },      // Clk3 Disable
//    {   0x84,       0x02,       0xFE,       0x00    },      // Clk4 Disable
//    {   0x85,       0x02,       0xEF,       0x00    },      // Clk5 Disable
//    {   0x86,       0x02,       0xDF,       0x00    },      // Clk6 Disable
//    {   0x87,       0x02,       0xBF,       0x00    },      // Clk7 Disable
//    {   0x88,       0x02,       0x7F,       0x00    },      // Clk8 Disable
//    {   0x89,       0x01,       0xFE,       0x00    },      // Clk9 Disable
//    {   0x8A,       0x01,       0xFD,       0x00    },      // Clk10 Disable
//    {   0x8B,       0x01,       0xFB,       0x00    },      // Clk11 Disable

//    {   0x90,       0x04,       0xBF,       0x40    },      // ClkREQ0 Enable
//    {   0x91,       0x04,       0x7F,       0x80    },      // ClkREQ1 Enable
//    {   0x92,       0x03,       0xBF,       0x40    },      // ClkREQ2 Enable
//    {   0x93,       0x03,       0x7F,       0x80    },      // ClkREQ3 Enable
//    {   0x94,       0x01,       0xEF,       0x10    },      // ClkREQ4 Enable
//    {   0x95,       0x01,       0xDF,       0x20    },      // ClkREQ5 Enable
//    {   0x96,       0x01,       0xBF,       0x40    },      // ClkREQ6 Enable
//    {   0x97,       0x01,       0x7F,       0x80    },      // ClkREQ7 Enable
//    {   0x98,       0x0B,       0xFE,       0x01    },      // ClkREQ8 Enable
//    {   0x99,       0x0B,       0xFD,       0x02    },      // ClkREQ9 Enable
//    {   0x9A,       0x0B,       0xFB,       0x04    },      // ClkREQ10 Enable
//    {   0x9B,       0x0B,       0xF7,       0x08    },      // ClkREQ11 Enable
    0xFF,
  }
};
#endif