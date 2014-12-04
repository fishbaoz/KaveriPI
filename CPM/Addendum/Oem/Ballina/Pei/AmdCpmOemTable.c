/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM OEM tables, and callback function.
 *
 * Contains code that defines OEM tables and callback function to override
 * OEM table on run time.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  OEM
 * @e \$Revision: 289766 $   @e \$Date: 2014-04-15 09:44:01 -0500 (Tue, 15 Apr 2014) $
 *
 */
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

#include <AmdCpmPei.h>
#include <AmdCpmOem.h>
#include "AmdCpmOemInitPeim.h"


//
// OEM CPM Table Definition
//

//
// Platform Id Table: Get Board Id from GPIO pins
//
AMD_CPM_PLATFORM_ID_TABLE           gCpmPlatformIdTable = {
  {CPM_SIGNATURE_GET_PLATFORM_ID, sizeof (gCpmPlatformIdTable) / sizeof (UINT8), 0, 0, 0, 1},
  {
    14,                                         // BOARD_ID0
    15,                                         // BOARD_ID1
    16,                                         // BOARD_ID2
    17,                                         // BOARD_ID3
    18,                                         // BOARD_ID4
    19,                                         // BOARD_ID5
    0xFF
  }
};

//
// Convert Table from Board Id to Platform Id
//
AMD_CPM_PLATFORM_ID_CONVERT_TABLE   gCpmPlatformIdConvertTable = {
  {CPM_SIGNATURE_GET_PLATFORM_ID_CONVERT, sizeof (gCpmPlatformIdConvertTable) / sizeof (UINT8), 0, 0, 0, 1},
  {
    {CPM_CPU_REVISION_ID_KV,  0x0000, 0x0000,    0x0000}, // Board Id -> Platform Id
    {CPM_CPU_REVISION_ID_TN,  0x0000, 0x0000,    0x0001}, // Board Id -> Platform Id
    0xFFFF,
  }
};

//
// Pre-Init Table
//
AMD_CPM_PRE_INIT_TABLE              gCpmPreInitTable = {
  {CPM_SIGNATURE_PRE_INIT, sizeof (gCpmPreInitTable) / sizeof (UINT8), 0, 0, 0, 0x01},
  {
    {   0x00,   0x03,   0xEA,   0xFE,   0x01    },          // PM_RegEA[0]: PCIDisable = 1
    {   0x00,   0x03,   0x2E,   0xF9,   0x00    },          // PM_Reg2E[2:1]: Smbus0Sel = 0
//  {   0x00,   0x03,   0x3C,   0x7F,   0x80    },          // PM_Reg3C[7]: SmartVoltEnable = 1
    {   0x00,   0x03,   0xBE,   0xED,   0x12    },          // PM_RegBE[1,4]: Enable KbRst
    {   0x01,   0xA3,   0x78,   0xF7,   0x00    },          // LPC Reg78[3]: Disable LDRQ1#
    {   0x02,   0xC3,   0xE4,   0x8F,   0x00    },          // APU_MISC Reg1E4[6:4] = 0
    {   0x01,   0xC3,   0xA4,   0x00,   0xEF    },          // APU_MISC RegA4 = 0xEF
    {   0x01,   0xC3,   0xA5,   0x00,   0x0F    },          // APU_MISC RegA5 = 0x0F
    0xFF,
  }
};

//
// GPIO PreInit Table
//
AMD_CPM_GPIO_INIT_TABLE             gCpmGpioPreInitTable = {
  {CPM_SIGNATURE_GPIO_PRE_INIT, sizeof (gCpmGpioPreInitTable) / sizeof (UINT8), 0, 0, 0, 0x01},
  {
    GPIO_DEFINITION(32,     GPIO_FUNCTION_1,    GPIO_INPUT,         GPIO_PU_EN,     GPIO_STICKY_DIS),    // INTE#/GPIO32 -> DET0
    GPIO_DEFINITION(34,     GPIO_FUNCTION_1,    GPIO_INPUT,         GPIO_PU_EN,     GPIO_STICKY_DIS),    // INTG#/GPIO34 -> DET2
    GPIO_DEFINITION(191,    GPIO_FUNCTION_0,    GPIO_OUTPUT_HIGH,   GPIO_PU_EN,     GPIO_STICKY_EN),     // PE_GPIO0
    0xFF,
  }
};

//
// GPIO Init Table
//
AMD_CPM_GPIO_INIT_TABLE             gCpmGpioInitTable = {
  {CPM_SIGNATURE_GPIO_INIT, sizeof (gCpmGpioInitTable) / sizeof (UINT8), 0, 0, 0, 0x01},
  {
    GPIO_DEFINITION(0,      GPIO_FUNCTION_0,    GPIO_OUTPUT_HIGH,   GPIO_PU_EN,     GPIO_STICKY_EN),    // MPCIE_RST1#
    GPIO_DEFINITION(1,      GPIO_FUNCTION_0,    GPIO_OUTPUT_HIGH,   GPIO_PU_EN,     GPIO_STICKY_EN),    // MPCIE_RST2#
    GPIO_DEFINITION(2,      GPIO_FUNCTION_0,    GPIO_OUTPUT_HIGH,   GPIO_PU_EN,     GPIO_STICKY_EN),    // DMC_RST0#
    GPIO_DEFINITION(4,      GPIO_FUNCTION_0,    GPIO_OUTPUT_HIGH,   GPIO_PU_EN,     GPIO_STICKY_EN),    // MPCIE_RST_DT#
    GPIO_DEFINITION(7,      GPIO_FUNCTION_0,    GPIO_NA,            GPIO_PU_EN,     GPIO_STICKY_EN),    // BT_ON
    GPIO_DEFINITION(8,      GPIO_FUNCTION_0,    GPIO_NA,            GPIO_PD_EN,     GPIO_STICKY_DIS),   // PEX_STD_SW#
    GPIO_DEFINITION(12,     GPIO_FUNCTION_0,    GPIO_NA,            GPIO_PU_EN,     GPIO_STICKY_EN),    // WL_DISABLE#
    GPIO_DEFINITION(13,     GPIO_FUNCTION_0,    GPIO_NA,            GPIO_PU_EN,     GPIO_STICKY_EN),    // WU_DISABLE#
    GPIO_DEFINITION(22,     GPIO_FUNCTION_0,    GPIO_NA,            GPIO_PU_EN,     GPIO_STICKY_EN),    // FCH_PWR_LV
    GPIO_DEFINITION(25,     GPIO_FUNCTION_0,    GPIO_OUTPUT_HIGH,   GPIO_PU_EN,     GPIO_STICKY_EN),    // PCIE_RST#_LAN
    GPIO_DEFINITION(27,     GPIO_FUNCTION_0,    GPIO_OUTPUT_HIGH,   GPIO_PU_EN,     GPIO_STICKY_EN),    // DDI3_PCIE_RST#
    GPIO_DEFINITION(41,     GPIO_FUNCTION_1,    GPIO_NA,            GPIO_PU_EN,     GPIO_STICKY_DIS),   // FCH_PCIE_PE2_CLKREQ#
    GPIO_DEFINITION(42,     GPIO_FUNCTION_1,    GPIO_NA,            GPIO_PU_EN,     GPIO_STICKY_DIS),   // FCH_PCIE_DT_CLKREQ#
    GPIO_DEFINITION(52,     GPIO_FUNCTION_0,    GPIO_NA,            GPIO_PU_EN,     GPIO_STICKY_DIS),   // FANOUT0
    GPIO_DEFINITION(53,     GPIO_FUNCTION_1,    GPIO_OUTPUT_HIGH,   GPIO_PU_EN,     GPIO_STICKY_EN),    // MPCIE_RST_XPRESS#
    GPIO_DEFINITION(54,     GPIO_FUNCTION_1,    GPIO_OUTPUT_HIGH,   GPIO_PU_EN,     GPIO_STICKY_DIS),   // FCH_PROCHOT#_C
    GPIO_DEFINITION(55,     GPIO_FUNCTION_3,    GPIO_NA,            GPIO_PD_EN,     GPIO_STICKY_DIS),   // MXM_PWR_EN
    GPIO_DEFINITION(57,     GPIO_FUNCTION_1,    GPIO_INPUT,         GPIO_PU_EN,     GPIO_STICKY_DIS),   // MLDIR
    GPIO_DEFINITION(59,     GPIO_FUNCTION_2,    GPIO_INPUT,         GPIO_PD_EN,     GPIO_STICKY_DIS),   // FFS_INT1
    GPIO_DEFINITION(171,    GPIO_FUNCTION_1,    GPIO_NA,            GPIO_PU_EN,     GPIO_STICKY_EN),    // ODD_PWR
    GPIO_DEFINITION(172,    GPIO_FUNCTION_1,    GPIO_INPUT,         GPIO_PU_EN,     GPIO_STICKY_DIS),   // DMC_PRESENT#
    GPIO_DEFINITION(175,    GPIO_FUNCTION_1,    GPIO_NA,            GPIO_PD_EN,     GPIO_STICKY_EN),    // DMC_PD
    GPIO_DEFINITION(176,    GPIO_FUNCTION_1,    GPIO_NA,            GPIO_PD_EN,     GPIO_STICKY_EN),    // MPCIE_PD1
    GPIO_DEFINITION(177,    GPIO_FUNCTION_1,    GPIO_NA,            GPIO_PD_EN,     GPIO_STICKY_EN),    // MPCIE_PD2
    GPIO_DEFINITION(189,    GPIO_FUNCTION_0,    GPIO_NA,            GPIO_PU_EN,     GPIO_STICKY_EN),    // MEM_1V25#
    GPIO_DEFINITION(190,    GPIO_FUNCTION_0,    GPIO_NA,            GPIO_PU_EN,     GPIO_STICKY_EN),    // MEM_1V5#
    GPIO_DEFINITION(191,    GPIO_FUNCTION_0,    GPIO_NA,            GPIO_PU_EN,     GPIO_STICKY_EN),    // PE_GPIO0
    GPIO_DEFINITION(192,    GPIO_FUNCTION_0,    GPIO_NA,            GPIO_PD_EN,     GPIO_STICKY_EN),    // PE_GPIO1
    GPIO_DEFINITION(197,    GPIO_FUNCTION_2,    GPIO_NA,            GPIO_PD_EN,     GPIO_STICKY_EN),    // VDDR_VDDP_VCTRL
    GPIO_DEFINITION(198,    GPIO_FUNCTION_0,    GPIO_OUTPUT_HIGH,   GPIO_PU_EN,     GPIO_STICKY_EN),    // HDD2_PWR
    GPIO_DEFINITION(199,    GPIO_FUNCTION_0,    GPIO_NA,            GPIO_PU_EN,     GPIO_STICKY_EN),    // WP_DISABLE#
    GPIO_DEFINITION(200,    GPIO_FUNCTION_0,    GPIO_OUTPUT_HIGH,   GPIO_PU_EN,     GPIO_STICKY_EN),    // HDD0_PWR

    GPIO_DEFINITION(0x64,   GPIO_FUNCTION_2,    GPIO_NA,            GPIO_NA,        GPIO_STICKY_DIS),   // FCH_PCIE_RST#
    GPIO_DEFINITION(0x66,   GPIO_FUNCTION_1,    GPIO_NA,            GPIO_PU_EN,     GPIO_STICKY_DIS),   // FCH_ODD_DA
    GPIO_DEFINITION(0x67,   GPIO_FUNCTION_1,    GPIO_OUTPUT_LOW,    GPIO_PD_EN,     GPIO_STICKY_EN),    // VGA_PD
    GPIO_DEFINITION(0x6B,   GPIO_FUNCTION_1,    GPIO_NA,            GPIO_PU_PD_DIS, GPIO_STICKY_DIS),   // DP_HPD_DIG
    GPIO_DEFINITION(0x6C,   GPIO_FUNCTION_1,    GPIO_NA,            GPIO_NA,        GPIO_STICKY_DIS),   // WF_RADIO
    GPIO_DEFINITION(0x6D,   GPIO_FUNCTION_1,    GPIO_NA,            GPIO_PU_EN,     GPIO_STICKY_DIS),   // LID_CLOSED#
    GPIO_DEFINITION(0x6E,   GPIO_FUNCTION_1,    GPIO_NA,            GPIO_NA,        GPIO_STICKY_DIS),   // TALERT#_FCH
    GPIO_DEFINITION(0x6F,   GPIO_FUNCTION_1,    GPIO_NA,            GPIO_NA,        GPIO_STICKY_DIS),   // AC_PRES_OK#
    GPIO_DEFINITION(0x70,   GPIO_FUNCTION_1,    GPIO_NA,            GPIO_PU_EN,     GPIO_STICKY_DIS),   // ODD_PLUGIN#
    GPIO_DEFINITION(0x77,   GPIO_FUNCTION_0,    GPIO_NA,            GPIO_PD_EN,     GPIO_STICKY_DIS),   // FFS_INT2

    0xFF,
  }
};

//
// GEVENT Init Table
//
AMD_CPM_GEVENT_INIT_TABLE           gCpmGeventInitTable = {
  {CPM_SIGNATURE_GEVENT_INIT, sizeof (gCpmGeventInitTable) / sizeof (UINT8), 0, 0, 0, 0x01},
  { //                          EventEnable     SciTrigE        SciLevl         SmiSciEn        SciS0En         SciMap      SmiTrig     SmiControl
    GEVENT_DEFINITION ( 0x00,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_00,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x01,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_01,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x02,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_02,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x03,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_03,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x04,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_04,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x05,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_05,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x06,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_06,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x07,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_07,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x08,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_08,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x09,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_09,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x0A,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_10,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x0B,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_11,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x0C,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_12,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x0D,   EVENT_ENABLE,   SCITRIG_AUTO,   SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_13,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x0E,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_14,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x0F,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_15,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x10,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_16,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x11,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_17,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x12,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_18,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x13,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_19,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x14,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_20,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x15,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_21,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x16,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_22,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    GEVENT_DEFINITION ( 0x17,   EVENT_ENABLE,   SCITRIG_LOW,    SCILEVEL_EDGE,  SMISCI_DISABLE, SCIS0_DISABLE,  SCIMAP_23,  SMITRIG_HI, SMICONTROL_DISABLE  ),
    0xFF,
  }
};

//
// Set Mem Voltage
//
AMD_CPM_GPIO_MEM_VOLTAGE_TABLE  gCpmSetMemVoltage = {
  {CPM_SIGNATURE_SET_MEM_VOLTAGE, sizeof (gCpmSetMemVoltage) / sizeof (UINT8), 0, 0 ,0, 0x01 },
  {                                 //          MEM_1V5#, MEM_1V25#
    {1, 190, 0, 189, 1},            // 1.5V         0       1
    {2, 190, 1, 189, 1},            // 1.35V        1       1
    {3, 190, 1, 189, 0},            // 1.25V        1       0
    0xFF,
  }
};

//
// Set Vddp/Vddr Voltage
//
AMD_CPM_GPIO_VDDP_VDDR_VOLTAGE_TABLE  gCpmSetVddpVddrVoltage = {
  {CPM_SIGNATURE_SET_VDDP_VDDR_VOLTAGE, sizeof (gCpmSetVddpVddrVoltage) / sizeof (UINT8), 0, 0 ,0, 0x01 },
  {
    {0, 197, 1},                    // 0.95V
    {1, 197, 0},                    // 1.05V
    0xFF,
  }
};

//
// CPM GPIO Module
//

AMD_CPM_GPIO_DEVICE_CONFIG_TABLE    gCpmGpioDeviceConfigTable = {
  {CPM_SIGNATURE_GPIO_DEVICE_CONFIG, sizeof (gCpmGpioDeviceConfigTable) / sizeof (UINT8), 0, 0, 0, 0x0F},
  {
    //                      DeviceId                   Enable         Assert      Deassert   Hotplug
    GPIO_DEVICE_DEFINITION (DEVICE_ID_ODD,          CPM_DEVICE_AUTO,    0,          0,          0   ),
    GPIO_DEVICE_DEFINITION (DEVICE_ID_DMC,          CPM_DEVICE_AUTO,    0,          0,          0   ),
    GPIO_DEVICE_DEFINITION (DEVICE_ID_MPCIE1,       CPM_DEVICE_ON,      0,          0,          0   ),
    GPIO_DEVICE_DEFINITION (DEVICE_ID_MPCIE2,       CPM_DEVICE_ON,      0,          0,          0   ),
    GPIO_DEVICE_DEFINITION (DEVICE_ID_MXM,          CPM_DEVICE_AUTO,    1,          0,          0   ),
    GPIO_DEVICE_DEFINITION (DEVICE_ID_BT,           CPM_DEVICE_ON,      0,          0,          0   ),
    GPIO_DEVICE_DEFINITION (DEVICE_ID_SWINGMODE,    CPM_DEVICE_ON,      0,          0,          0   ),
    GPIO_DEVICE_DEFINITION (DEVICE_ID_POWERLEVEL,   CPM_DEVICE_ON,      0,          0,          0   ),
    GPIO_DEVICE_DEFINITION (DEVICE_ID_VGAMUXSEL,    CPM_DEVICE_ON,      0,          0,          0   ),
    0xFF,
  }
};

//
// CPM Device Detection Table
//
AMD_CPM_GPIO_DEVICE_DETECTION_TABLE gCpmGpioDeviceDetectionTable = {
  {CPM_SIGNATURE_GPIO_DEVICE_DETECTION, sizeof (gCpmGpioDeviceDetectionTable) / sizeof (UINT8), 0, 0, 0, 0x01},
  {
    {   DEVICE_ID_ODD,          CPM_TYPE_GPIO_1,    112,    0,      0,      0   },          // ODD_PLUGIN#: GEVENT16#
    {   DEVICE_ID_DMC,          CPM_TYPE_GPIO_1,    172,    0,      0,      0   },          // DMC_PRESENT#: GPIO172
    {   DEVICE_ID_MXM,          CPM_TYPE_GPIO_2,    32,     0,      34,     0   },          // MXM_PRESENT1: GPIO32. MXM_PRESENT2: GPIO34
    {   DEVICE_ID_EXPRESSCARD,  CPM_TYPE_GPIO_1,    101,    0,      0,      0   },          // PCIE_EXPCARD_PWREN#: GEVENT5
    0xFF,
  }
};

//
// CPM Device Reset Table
//
AMD_CPM_GPIO_DEVICE_RESET_TABLE gCpmGpioDeviceResetTable = {
  {CPM_SIGNATURE_GPIO_DEVICE_RESET, sizeof (gCpmGpioDeviceResetTable) / sizeof (UINT8), 0, 0, 0, 0x01},
  {
    //     DeviceId                  Mode               Type                    Num Value       InitFlag;
    {   DEVICE_ID_DMC,          CPM_RESET_ASSERT,   CPM_RESET_GPIO,     CPM_GPIO_PIN(2, 0),         0   },      // DMC_RST0#
    {   DEVICE_ID_MPCIE1,       CPM_RESET_ASSERT,   CPM_RESET_GPIO,     CPM_GPIO_PIN(0, 0),         0   },      // MPCIE_RST1#
    {   DEVICE_ID_MPCIE2,       CPM_RESET_ASSERT,   CPM_RESET_GPIO,     CPM_GPIO_PIN(1, 0),         0   },      // MPCIE_RST2#
    {   DEVICE_ID_MXM,          CPM_RESET_ASSERT,   CPM_RESET_GPIO,     CPM_GPIO_PIN(191, 0),       0   },      // PE_GPIO0#
    {   DEVICE_ID_DT,           CPM_RESET_ASSERT,   CPM_RESET_GPIO,     CPM_GPIO_PIN(4, 0),         0   },      // MPCIE_RST_DT#
    {   DEVICE_ID_LAN,          CPM_RESET_ASSERT,   CPM_RESET_GPIO,     CPM_GPIO_PIN(25, 0),        0   },      // PCIE_RST_LAN#
    {   DEVICE_ID_DDI3,         CPM_RESET_ASSERT,   CPM_RESET_GPIO,     CPM_GPIO_PIN(27, 0),        0   },      // DDI3_PCIE_RST#
    {   DEVICE_ID_EXPRESSCARD,  CPM_RESET_ASSERT,   CPM_RESET_GPIO,     CPM_GPIO_PIN(53, 0),        0   },      // MPCIE_RST_XPRESS#

    {   DEVICE_ID_DMC,          CPM_RESET_DEASSERT, CPM_RESET_GPIO,     CPM_GPIO_PIN(2, 1),         0   },      // DMC_RST0#
    {   DEVICE_ID_MPCIE1,       CPM_RESET_DEASSERT, CPM_RESET_GPIO,     CPM_GPIO_PIN(0, 1),         0   },      // MPCIE_RST1#
    {   DEVICE_ID_MPCIE2,       CPM_RESET_DEASSERT, CPM_RESET_GPIO,     CPM_GPIO_PIN(1, 1),         0   },      // MPCIE_RST2#
    {   DEVICE_ID_MXM,          CPM_RESET_DEASSERT, CPM_RESET_GPIO,     CPM_GPIO_PIN(191, 1),       0   },      // PE_GPIO0#
    {   DEVICE_ID_DT,           CPM_RESET_DEASSERT, CPM_RESET_GPIO,     CPM_GPIO_PIN(4, 1),         0   },      // MPCIE_RST_DT#
    {   DEVICE_ID_LAN,          CPM_RESET_DEASSERT, CPM_RESET_GPIO,     CPM_GPIO_PIN(25, 1),        0   },      // PCIE_RST_LAN#
    {   DEVICE_ID_DDI3,         CPM_RESET_DEASSERT, CPM_RESET_GPIO,     CPM_GPIO_PIN(27, 1),        0   },      // DDI3_PCIE_RST#
    {   DEVICE_ID_EXPRESSCARD,  CPM_RESET_DEASSERT, CPM_RESET_GPIO,     CPM_GPIO_PIN(53, 1),        0   },      // MPCIE_RST_XPRESS#

    0xFF,
  }
};

//
// CPM GPIO Device Init Table (Power On/Off)
//
AMD_CPM_GPIO_DEVICE_POWER_TABLE gCpmGpioDevicePowerTable = {
  {CPM_SIGNATURE_GPIO_DEVICE_POWER, sizeof (gCpmGpioDevicePowerTable) / sizeof (UINT8), 0, 0, 0, 0x01},
  {
    //  DeviceId                Mode                Type                Config                  InitFlag;
    {   DEVICE_ID_ODD,          CPM_POWER_OFF,      CPM_POWER_SET,      CPM_GPIO_PIN(171, 0),   0   },      // ODD_PWR
    {   DEVICE_ID_DMC,          CPM_POWER_OFF,      CPM_POWER_SET,      CPM_GPIO_PIN(175, 1),   0   },      // DMC_PD
    {   DEVICE_ID_MXM,          CPM_POWER_OFF,      CPM_POWER_SET,      CPM_GPIO_PIN(55, 0),    0   },      // MXM_PWR_EN
//  {   DEVICE_ID_MXM,          CPM_POWER_OFF,      CPM_POWER_DELAY,    3000,                   0   },      // MXM Delay 3ms
    {   DEVICE_ID_MXM,          CPM_POWER_OFF,      CPM_POWER_SET,      CPM_GPIO_PIN(192, 0),   0   },      // PE_GPIO1
    {   DEVICE_ID_SWINGMODE,    CPM_POWER_OFF,      CPM_POWER_SET,      CPM_GPIO_PIN(8, 0),     0   },      // PEX_STD_SW#: Standard
    {   DEVICE_ID_POWERLEVEL,   CPM_POWER_OFF,      CPM_POWER_SET,      CPM_GPIO_PIN(22, 0),    0   },      // FCH_PWR_LV: Battery
    {   DEVICE_ID_VGAMUXSEL,    CPM_POWER_OFF,      CPM_POWER_SET,      CPM_GPIO_PIN(24, 1),    0   },      // VGA_MUX_SEL: MXM
    {   DEVICE_ID_BT,           CPM_POWER_OFF,      CPM_POWER_SET,      CPM_GPIO_PIN(7, 0),     3   },      // BT_ON
    {   DEVICE_ID_WIRELESS,     CPM_POWER_OFF,      CPM_POWER_SET,      CPM_GPIO_PIN (176, 1),  3   },
    {   DEVICE_ID_WIRELESS,     CPM_POWER_OFF,      CPM_POWER_SET,      CPM_GPIO_PIN (177, 1),  3   },
    {   DEVICE_ID_RADIO,        CPM_POWER_OFF,      CPM_POWER_SET,      CPM_GPIO_PIN(12, 0),    3   },      // WL_DISABLE#
    {   DEVICE_ID_RADIO,        CPM_POWER_OFF,      CPM_POWER_SET,      CPM_GPIO_PIN(13, 0),    3   },      // WU_DISABLE#
    {   DEVICE_ID_RADIO,        CPM_POWER_OFF,      CPM_POWER_SET,      CPM_GPIO_PIN(199, 0),   3   },      // WP_DISABLE#

    {   DEVICE_ID_ODD,          CPM_POWER_ON,       CPM_POWER_SET,      CPM_GPIO_PIN(171, 1),   0   },      // ODD_PWR
    {   DEVICE_ID_DMC,          CPM_POWER_ON,       CPM_POWER_SET,      CPM_GPIO_PIN(175, 0),   0   },      // DMC_PD
    {   DEVICE_ID_MXM,          CPM_POWER_ON,       CPM_POWER_SET,      CPM_GPIO_PIN(192, 1),   0   },      // PE_GPIO1
    {   DEVICE_ID_MXM,          CPM_POWER_ON,       CPM_POWER_DELAY,    3000,                   0   },      // MXM Delay 3ms
    {   DEVICE_ID_MXM,          CPM_POWER_ON,       CPM_POWER_SET,      CPM_GPIO_PIN(55, 1),    0   },      // MXM_PWR_EN
    {   DEVICE_ID_MXM,          CPM_POWER_ON,       CPM_POWER_WAIT,     CPM_GPIO_PIN(51, 1),    0   },      // MXM_PWRGD
    {   DEVICE_ID_MXM,          CPM_POWER_ON,       CPM_POWER_DELAY,    200,                    0   },      // MXM Delay 200us
    {   DEVICE_ID_SWINGMODE,    CPM_POWER_ON,       CPM_POWER_SET,      CPM_GPIO_PIN(8, 1),     0   },      // PEX_STD_SW#: Half
    {   DEVICE_ID_POWERLEVEL,   CPM_POWER_ON,       CPM_POWER_SET,      CPM_GPIO_PIN(22, 1),    0   },      // FCH_PWR_LV: AC
    {   DEVICE_ID_VGAMUXSEL,    CPM_POWER_ON,       CPM_POWER_SET,      CPM_GPIO_PIN(24, 0),    0   },      // VGA_MUX_SEL: FCH
    {   DEVICE_ID_BT,           CPM_POWER_ON,       CPM_POWER_SET,      CPM_GPIO_PIN(7, 1),     3   },      // BT_ON
    {   DEVICE_ID_WIRELESS,     CPM_POWER_ON,       CPM_POWER_SET,      CPM_GPIO_PIN (176, 0),  3   },      // 3: GPIO_DEVICE_INIT_STAGE_DUMMY
    {   DEVICE_ID_WIRELESS,     CPM_POWER_ON,       CPM_POWER_SET,      CPM_GPIO_PIN (177, 0),  3   },
    {   DEVICE_ID_RADIO,        CPM_POWER_ON,       CPM_POWER_SET,      CPM_GPIO_PIN(12, 1),    3   },      // WL_DISABLE#
    {   DEVICE_ID_RADIO,        CPM_POWER_ON,       CPM_POWER_SET,      CPM_GPIO_PIN(13, 1),    3   },      // WU_DISABLE#
    {   DEVICE_ID_RADIO,        CPM_POWER_ON,       CPM_POWER_SET,      CPM_GPIO_PIN(199, 1),   3   },      // WP_DISABLE#

    0xFF,
  }
};

//
// PCIE Clock Table
//
AMD_CPM_PCIE_CLOCK_TABLE    gCpmPcieClockTable = {
  {CPM_SIGNATURE_PCIE_CLOCK, sizeof (gCpmPcieClockTable) / sizeof (UINT8), 0, 0, 0x01, 0x0F},
  {
    //  ClkId           ClkReq          ClkIdExt        ClkReqExt       DeviceId                Device  Function    SlotCheck   SpecialFunctionId;
    {   GPP_CLK0,       CLK_REQ0,       SRC_CLK0,       CLK_REQ0,       DEVICE_ID_EXPRESSCARD,  3,      4,          NON_SLOT_CHECK,     0    },     // EXPRESS CARD
    {   GPP_CLK1,       CLK_ENABLE,     SRC_CLK4,       CLK_ENABLE,     DEVICE_ID_DDI3,         3,      5,          SLOT_CHECK,         0    },     // DDI SLOT3
    {   GPP_CLK2,       CLK_REQ2,       SRC_CLK5,       CLK_REQ5,       DEVICE_ID_DMC,          21,     1,          SLOT_CHECK,         0    },     // DMC
    {   GPP_CLK3,       CLK_REQ3,       SRC_CLK1,       CLK_REQ1,       DEVICE_ID_LAN,          3,      1,          SLOT_CHECK,         0    },     // LAN
    {   GPP_CLK4,       CLK_REQ4,       SRC_CLK2,       CLK_REQ2,       DEVICE_ID_MPCIE1,       3,      2,          SLOT_CHECK,         0    },     // Mini PCIE1
    {   GPP_CLK5,       CLK_ENABLE,     SRC_CLK6,       CLK_REQ6,       DEVICE_ID_DT,           21,     3,          SLOT_CHECK,         0    },     // DT X1 PCIE
    {   GPP_CLK6,       CLK_DISABLE,    SRC_CLK7,       CLK_DISABLE,    0xFF,                   0,      0,          NON_SLOT_CHECK,     0    },     // N/A
    {   GPP_CLK7,       CLK_DISABLE,    SRC_CLK9,       CLK_ENABLE,     0xFF,                   0,      0,          NON_SLOT_CHECK,     0    },     // N/A
    {   GPP_CLK8,       CLK_REQ8,       SRC_CLK3,       CLK_REQ3,       DEVICE_ID_MPCIE2,       3,      3,          SLOT_CHECK,         0    },     // Mini PCIE2
    {   GPP_CLK9,       CLK_REQGFX,     SRC_CLK8,       CLK_REQ8,       DEVICE_ID_MXM,          2,      1,          SLOT_CHECK,         0    },     // MXM
    {   CPM_CLKID_OSCOUT2_OUTOFF, CLK_DISABLE, SRC_SKIP, 0,             0xFF,                   0,      0,          NON_SLOT_CHECK,     0    },     // OSCOUT2
    0xFF,
  }
};

AMD_CPM_PCIE_CLOCK_TABLE    gCpmPcieClockTable2 = {
  {CPM_SIGNATURE_PCIE_CLOCK, sizeof (gCpmPcieClockTable2) / sizeof (UINT8), 0, 0, 0x02, 0x0F},
  {
    //  ClkId           ClkReq          ClkIdExt        ClkReqExt       DeviceId                Device  Function    SlotCheck   SpecialFunctionId;
    {   GPP_CLK0,       CLK_REQ0,       SRC_CLK0,       CLK_REQ0,       DEVICE_ID_EXPRESSCARD,  7,      0,          NON_SLOT_CHECK,     0    },     // EXPRESS CARD
    {   GPP_CLK1,       CLK_DISABLE,    SRC_CLK4,       CLK_DISABLE,    DEVICE_ID_DDI3,         0,      0,          NON_SLOT_CHECK,     0    },     // DDI SLOT3
    {   GPP_CLK2,       CLK_REQ2,       SRC_CLK5,       CLK_REQ5,       DEVICE_ID_DMC,          21,     1,          SLOT_CHECK,         0    },     // DMC
    {   GPP_CLK3,       CLK_REQ3,       SRC_CLK1,       CLK_REQ1,       DEVICE_ID_LAN,          4,      0,          SLOT_CHECK,         0    },     // LAN
    {   GPP_CLK4,       CLK_REQ4,       SRC_CLK2,       CLK_REQ2,       DEVICE_ID_MPCIE1,       5,      0,          SLOT_CHECK,         0    },     // Mini PCIE1
    {   GPP_CLK5,       CLK_ENABLE,     SRC_CLK6,       CLK_REQ6,       DEVICE_ID_DT,           21,     3,          SLOT_CHECK,         0    },     // DT X1 PCIE
    {   GPP_CLK6,       CLK_DISABLE,    SRC_CLK7,       CLK_DISABLE,    0xFF,                   0,      0,          NON_SLOT_CHECK,     0    },     // N/A
    {   GPP_CLK7,       CLK_DISABLE,    SRC_CLK9,       CLK_ENABLE,     0xFF,                   0,      0,          NON_SLOT_CHECK,     0    },     // N/A
    {   GPP_CLK8,       CLK_REQ8,       SRC_CLK3,       CLK_REQ3,       DEVICE_ID_MPCIE2,       6,      0,          SLOT_CHECK,         0    },     // Mini PCIE2
    {   GPP_CLK9,       CLK_REQGFX,     SRC_CLK8,       CLK_REQ8,       DEVICE_ID_MXM,          2,      0,          SLOT_CHECK,         0    },     // MXM
    {   CPM_CLKID_OSCOUT2_OUTOFF, CLK_DISABLE, SRC_SKIP, 0,             0xFF,                   0,      0,          NON_SLOT_CHECK,     0    },     // OSCOUT2
    0xFF,
  }
};

//
// External ClkGen Table
//
AMD_CPM_EXT_CLKGEN_TABLE    gCpmExtClkGenTable = {
  {CPM_SIGNATURE_EXT_CLKGEN, sizeof (gCpmExtClkGenTable) / sizeof (UINT8), 0, 0, 0, 0x01},
  0,                              // Smbus Select: 0: Smbus0. 1: Smbus1
  0x69,                           // Smbus Address: 0xD2
  {
    //  Function    Offset      AndMask     OrMask
    {   0x00,       0x04,       0xF9,       0x02    },      // SS Disable
    {   0x01,       0x04,       0xF9,       0x06    },      // SS Enable

    {   0x80,       0x02,       0xFE,       0x00    },      // Clk0 Disable
    {   0x81,       0x02,       0xFD,       0x00    },      // Clk1 Disable
    {   0x82,       0x02,       0xFB,       0x00    },      // Clk2 Disable
    {   0x83,       0x02,       0xF7,       0x00    },      // Clk3 Disable
    {   0x84,       0x02,       0xEF,       0x00    },      // Clk4 Disable
    {   0x85,       0x02,       0xDF,       0x00    },      // Clk5 Disable
    {   0x86,       0x02,       0xBF,       0x00    },      // Clk6 Disable
    {   0x87,       0x02,       0x7F,       0x00    },      // Clk7 Disable
    {   0x88,       0x01,       0xFE,       0x00    },      // Clk8 Disable
    {   0x89,       0x01,       0xFD,       0x00    },      // Clk9 Disable
    {   0x8A,       0x01,       0xFB,       0x00    },      // Clk10 Disable
    {   0x8B,       0x01,       0xF7,       0x00    },      // Clk11 Disable

    {   0x90,       0x04,       0xBF,       0x40    },      // ClkREQ0 Enable
    {   0x91,       0x04,       0x7F,       0x80    },      // ClkREQ1 Enable
    {   0x92,       0x03,       0xBF,       0x40    },      // ClkREQ2 Enable
    {   0x93,       0x03,       0x7F,       0x80    },      // ClkREQ3 Enable
    {   0x94,       0x01,       0xEF,       0x10    },      // ClkREQ4 Enable
    {   0x95,       0x01,       0xDF,       0x20    },      // ClkREQ5 Enable
    {   0x96,       0x01,       0xBF,       0x40    },      // ClkREQ6 Enable
    {   0x97,       0x01,       0x7F,       0x80    },      // ClkREQ7 Enable
    {   0x98,       0x0B,       0xFE,       0x01    },      // ClkREQ8 Enable
    {   0x99,       0x0B,       0xFD,       0x02    },      // ClkREQ9 Enable
    {   0x9A,       0x0B,       0xFB,       0x04    },      // ClkREQ10 Enable
    {   0x9B,       0x0B,       0xF7,       0x08    },      // ClkREQ11 Enable

    0xFF,
  }
};

//
// CPM Express Card Table
//
AMD_CPM_EXPRESS_CARD_TABLE          gCpmExpressCardTable = {
  {CPM_SIGNATURE_PCIE_EXPRESS_CARD, sizeof (gCpmExpressCardTable) / sizeof (UINT8), 0, 0, 0x01, 0x01},
  3,                      // Device Number of PCIE Bridge
  4,                      // Function Number of PCIE Bridge
  5,                      // GEVENT Pin 5
  DEVICE_ID_EXPRESSCARD,  // Device Id
};

AMD_CPM_EXPRESS_CARD_TABLE          gCpmExpressCardTable2 = {
  {CPM_SIGNATURE_PCIE_EXPRESS_CARD, sizeof (gCpmExpressCardTable2) / sizeof (UINT8), 0, 0, 0x02, 0x01},
  7,                      // Device Number of PCIE Bridge
  0,                      // Function Number of PCIE Bridge
  5,                      // GEVENT Pin 5
  DEVICE_ID_EXPRESSCARD,  // Device Id
};

//
// CPM Wireless Button Table
//
AMD_CPM_WIRELESS_BUTTON_TABLE       gCpmWirelessButtonTable = {
  {CPM_SIGNATURE_WIRELESS_BUTTON, sizeof (gCpmWirelessButtonTable) / sizeof (UINT8), 0, 0, 0x01, 0x00000001},
  {
    {3, 2},
    {3, 3},
  },
  12,                     // GEVENT Pin 12
  DEVICE_ID_RADIO,        // Device Id ro control radio
  DEVICE_ID_WIRELESS,     // Device Id to control power
  DEVICE_ID_BT            // Device Id to control BT
};

AMD_CPM_WIRELESS_BUTTON_TABLE       gCpmWirelessButtonTable2 = {
  {CPM_SIGNATURE_WIRELESS_BUTTON, sizeof (gCpmWirelessButtonTable) / sizeof (UINT8), 0, 0, 0x02, 0x00000001},
  {
    {5, 0},
    {6, 0},
  },
  12,                     // GEVENT Pin 12
  DEVICE_ID_RADIO,        // Device Id ro control radio
  DEVICE_ID_WIRELESS,     // Device Id to control power
  DEVICE_ID_BT            // Device Id to control BT
};

//
// CPM Thermal Fan Control Table
//
AMD_CPM_ACPI_THERMAL_FAN_TABLE      gCpmAcpiThermalFan = {
  {CPM_SIGNATURE_ACPI_THERMAL_FAN, sizeof (gCpmAcpiThermalFan) / sizeof (UINT8), 0, 0, 0, 0x01},
  {
    0x0E,                       // EventPin
    0x00,                       // SbFanCtrlId
  },
  {
    105,                        // CpuCRT
    98,                         // CpuPSV
    80,                         // CpuAC0
    50,                         // CpuAC1
    0,                          // CpuAC2
    0,                          // CpuAC3
    100,                        // CpuAL0
    40,                         // CpuAL1
    0,                          // CpuAL2
    0,                          // CpuAL3
    0,                          // ThermalSensor
    12,                         // HysteresisInfo
    5                           // HysteresisInfoPsv
  },
};

//
// CPM Display Feature Module
//

//
// CPM Device Path Table
//
AMD_CPM_DEVICE_PATH_TABLE gCpmDevicePathTable = {
  {CPM_SIGNATURE_DEVICE_PATH, sizeof (gCpmDevicePathTable) / sizeof (UINT8), 0, 0, 0, 0x01},
  {
    {0x80000003, 0x00, {0, 0}, {1, 0}},                                     // PowerXpress|HCF, iGPU, (0,0,0)/(1,0)
    {0x80000003, 0x01, {2, 0}, {0, 0}, DEVICE_ID_VGAMUXSEL, CPM_POWER_OFF}, // PowerXpress|HCF, dGPU, (0,2,0)/(0,0)
    {0x80000003, 0x01, {3, 0}, {0, 0}, DEVICE_ID_VGAMUXSEL, CPM_POWER_OFF}, // PowerXpress|HCF, dGPU, (0,3,0)/(0,0)
    {0x80000003, 0x01, {2, 1}, {0, 0}, DEVICE_ID_VGAMUXSEL, CPM_POWER_OFF}, // PowerXpress|HCF, dGPU, (0,3,0)/(0,0)
    {0x80000003, 0x01, {2, 2}, {0, 0}, DEVICE_ID_VGAMUXSEL, CPM_POWER_OFF}, // PowerXpress|HCF, dGPU, (0,3,0)/(0,0)
    {0x00000000, 0x00, {0, 0}, {0, 0}},
  }
};

//
// CPM Specific Ssid Table
//
AMD_CPM_SPECIFIC_SSID_TABLE gCpmSpecificSsidTable = {
  {CPM_SIGNATURE_SPECIFIC_SSID, sizeof (gCpmSpecificSsidTable) / sizeof (UINT8), 0, 0, 0, 0x01},
  {
    {   0x1002,   0x6602    },      // Opal XTX
    {   0x1002,   0x6604    },      // Opal XT
    {   0x1002,   0x6605    },      // Opal PRO
    {   0x1002,   0x6707    },      // Opal LE
    {   0x1002,   0x6664    },      // Jet XT
    {   0x1002,   0x6665    },      // Jet PRO
    {   0x1002,   0x6667    },      // Jet UL
    {   0x1002,   0x666F    },      // Jet LE

    {   0x1002,   0x95C4    },
    {   0x1002,   0x9553    },
    {   0x1002,   0x9480    },
    {   0x1002,   0x68B0    },
    {   0x1002,   0x68E0    },      // MXM
    {   0x1002,   0x68E5    },      // MXM-Robson-Cedar     6300M
    {   0x1002,   0x6741    },      // MXM-Turks-Whistler   6600M
    {   0x1002,   0x6742    },      // MXM-Turks-Whistler   6600M
    {   0x1002,   0x6760    },      // MXM-Caisos-Seymour   6470M
    {   0x1002,   0x6800    },      // 7800
    {   0x1002,   0x6801    },
    {   0x1002,   0x6820    },      // 7700
    {   0x1002,   0x6821    },
    {   0x1002,   0x6823    },
    {   0x1002,   0x6824    },
    {   0x1002,   0x6825    },
    {   0x1002,   0x6826    },
    {   0x1002,   0x6827    },
    {   0x1002,   0x682D    },
    {   0x1002,   0x682E    },
    {   0x1002,   0x6840    },      // Lombok/Thames
    {   0x1002,   0x6841    },
    {   0x1002,   0x6842    },

    {   0x1002,   0x1304    },      // Kaveri
    {   0x1002,   0x1305    },
    {   0x1002,   0x1306    },
    {   0x1002,   0x1307    },
    {   0x1002,   0x1308    },
    {   0x1002,   0x1309    },
    {   0x1002,   0x130A    },
    {   0x1002,   0x130B    },
    {   0x1002,   0x130C    },
    {   0x1002,   0x130D    },
    {   0x1002,   0x130E    },
    {   0x1002,   0x130F    },
    {   0x1002,   0x1310    },
    {   0x1002,   0x1311    },
    {   0x1002,   0x1312    },
    {   0x1002,   0x1313    },
    {   0x1002,   0x1315    },
    {   0x1002,   0x1316    },
    {   0x1002,   0x1317    },
    {   0x1002,   0x1318    },
    {   0x1002,   0x1319    },
    {   0x1002,   0x131A    },
    {   0x1002,   0x131B    },
    {   0x1002,   0x131C    },
    {   0x1002,   0x131D    },
    {   0x1002,   0x131E    },
    {   0x1002,   0x131F    },

    {   0x1002,   0x9900    },      // Trinity
    {   0x1002,   0x9901    },
    {   0x1002,   0x9902    },
    {   0x1002,   0x9903    },
    {   0x1002,   0x9904    },
    {   0x1002,   0x9905    },
    {   0x1002,   0x9906    },
    {   0x1002,   0x9907    },
    {   0x1002,   0x9908    },
    {   0x1002,   0x9909    },
    {   0x1002,   0x990A    },
    {   0x1002,   0x990B    },
    {   0x1002,   0x990C    },
    {   0x1002,   0x990D    },
    {   0x1002,   0x990D    },
    {   0x1002,   0x990E    },
    {   0x1002,   0x990F    },
    {   0x1002,   0x9910    },
    {   0x1002,   0x9911    },
    {   0x1002,   0x9912    },
    {   0x1002,   0x9913    },
    {   0x1002,   0x9914    },
    {   0x1002,   0x9915    },
    {   0x1002,   0x9916    },
    {   0x1002,   0x9917    },
    {   0x1002,   0x9918    },
    {   0x1002,   0x9919    },
    {   0x1002,   0x991A    },
    {   0x1002,   0x991B    },
    {   0x1002,   0x991C    },
    {   0x1002,   0x991D    },
    {   0x1002,   0x991E    },
    {   0x1002,   0x991F    },
    {   0x1002,   0x9911    },
    {   0x1002,   0x9911    },
    {   0x1002,   0x9911    },
    {   0x1002,   0x9911    },
    {   0x1002,   0x9911    },
    {   0x1002,   0x9911    },
    {   0x1002,   0x9990    },
    {   0x1002,   0x9991    },
    {   0x1002,   0x9992    },
    {   0x1002,   0x9993    },
    {   0x1002,   0x9994    },
    {   0x1002,   0x9995    },
    {   0x1002,   0x9996    },
    {   0x1002,   0x9997    },
    {   0x1002,   0x9998    },
    {   0x1002,   0x9999    },
    {   0x1002,   0x999A    },
    {   0x1002,   0x999B    },
    {   0x1002,   0x999C    },
    {   0x1002,   0x999D    },
    {   0x1002,   0x999E    },
    {   0x1002,   0x999F    },
    {   0x1002,   0x99A0    },
    {   0x1002,   0x99A1    },
    {   0x1002,   0x99A2    },
    {   0x1002,   0x99A3    },
    {   0x1002,   0x99A4    },
    {   0x1002,   0x99A5    },
    {   0x1002,   0x99A6    },
    {   0x1002,   0x99A7    },
    {   0x1002,   0x99A8    },
    {   0x1002,   0x99A9    },
    {   0x1002,   0x99AA    },
    {   0x1002,   0x99AB    },
    {   0x1002,   0x99AC    },
    {   0x1002,   0x99AD    },
    {   0x1002,   0x99AE    },
    {   0x1002,   0x99AF    },

    {   0xFFFF,   0xFFFF    },      //End of Table
  }
};

//
// CPM Rebrand Dual Graphics Ssid Table
//
AMD_CPM_REBRAND_DUAL_GRAPHICS_SSID_TABLE gCpmRebrandDualGraphcisSsidTable = {
  {CPM_SIGNATURE_REBRAND_DUAL_GRAPHICS_SSID, sizeof (gCpmRebrandDualGraphcisSsidTable) / sizeof (UINT8), 0, 0, 0, 0x01},
  {
    {0x1002,  0x1309, 0},
    {0x1002,  0x130A, 0},
    {0x1002,  0x130B, 0},
    {0x1002,  0x130C, 0},
    {0x1002,  0x130D, 0},
    {0x1002,  0x130E, 0},

    {0x1002,  0x6900, 1},
    {0x1002,  0x6901, 1},
    {0x1002,  0x6664, 1},
    {0x1002,  0x6665, 1},
    {0x1002,  0x6667, 1},
    {0x1002,  0x6604, 1},
    {0x1002,  0x6605, 1},

    {0xFFFF,  0xFFFF, 1},      //End of Table
  }
};

//
// CPM Display Feature Table
//
AMD_CPM_DISPLAY_FEATURE_TABLE gCpmDisplayFeatureTable = {
  {CPM_SIGNATURE_DISPLAY_FEATURE, sizeof (gCpmDisplayFeatureTable) / sizeof (UINT8), 0, 0, 0, 0x0F},
  0,                                          // FunctionDisableMask
  DEVICE_ID_MXM,                              // MXM Device Id
  0xFF,                                       // MXM_OVERT#
  0,                                          // Forced Power State Id if MXM_OVERT# != 0xFF
  0x0B,                                       // Display Connect Event
  0,                                          // Device Id for Docking Pin
  0,                                          // MuxFlag
  0,                                          // No Display Mux
  0,                                          //
  10,                                         // AtpxConnector8Number
  {                                           // AtpxConnector8
    {0x05, 0x00, 0x00, 0x0110},                 // Connector #0: LCD1 on iGPU
    {0x05, 0x01, 0x00, 0x0100},                 // Connector #1: CRT1 on iGPU
    {0x07, 0x03, 0x00, 0x0210},                 // Connector #2: DFP1 on iGPU
    {0x07, 0x07, 0x00, 0x0220},                 // Connector #3: DFP2 on iGPU
    {0x00, 0x09, 0x00, 0x0230},                 // Connector #4: DFP3 on iGPU
    {0x01, 0x00, 0x01, 0x0110},                 // Connector #5: LCD1 on dGPU
    {0x01, 0x01, 0x01, 0x0100},                 // Connector #6: CRT1 on dGPU
    {0x03, 0x03, 0x01, 0x0210},                 // Connector #7: DFP1 on dGPU
    {0x03, 0x07, 0x01, 0x0220},                 // Connector #8: DFP2 on dGPU
    {0x00, 0x09, 0x01, 0x0230},                 // Connector #9: DFP3 on dGPU
  },
  0,                                          // AtpxConnector9Number
  {                                           // AtpxConnector9
    {0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00},
  },
  0x55,                                       // AtifSupportedNotificationMask;
  7,                                          // AtifDeviceCombinationNumber;
  {
    0x01, 0x02, 0x08, 0x80, 0x03, 0x09, 0x81    // AtifDeviceCombinationBuffer[20];
  },
  {
    0x6C, 0x00,                               // WORD Structure Size : 0x6A
    0x00, 0x00,                               // WORD Flags          : Reserved
    0x00,                                     // BYTE        Error Code     : 0x00
    0x64,                                     // BYTE        AC Level       : 100%
    0x20,                                     // BYTE        DC Level       : 32%
    0x0C,                                     // BYTE        Minimum signal : 12
    0xFF,                                     // BYTE        Maximum signal : 255
    0x31,                                     // BYTE        Count          : 49
    0x02, 0x0E,                               // BYTE-BYTE    First Lumi/Signal: 2% - 14
    0x04, 0x10,                               // BYTE-BYTE
    0x06, 0x12,                               // BYTE-BYTE
    0x08, 0x15,                               // BYTE-BYTE
    0x0A, 0x17,                               // BYTE-BYTE
    0x0C, 0x1A,                               // BYTE-BYTE
    0x0E, 0x1D,                               // BYTE-BYTE
    0x10, 0x20,                               // BYTE-BYTE
    0x12, 0x23,                               // BYTE-BYTE
    0x14, 0x26,                               // BYTE-BYTE
    0x16, 0x29,                               // BYTE-BYTE
    0x18, 0x2C,                               // BYTE-BYTE
    0x1A, 0x30,                               // BYTE-BYTE
    0x1C, 0x34,                               // BYTE-BYTE
    0x1E, 0x37,                               // BYTE-BYTE
    0x20, 0x3B,                               // BYTE-BYTE
    0x22, 0x3E,                               // BYTE-BYTE
    0x24, 0x43,                               // BYTE-BYTE
    0x26, 0x47,                               // BYTE-BYTE
    0x28, 0x4B,                               // BYTE-BYTE
    0x2A, 0x50,                               // BYTE-BYTE
    0x2C, 0x54,                               // BYTE-BYTE
    0x2E, 0x58,                               // BYTE-BYTE
    0x30, 0x5D,                               // BYTE-BYTE
    0x32, 0x62,                               // BYTE-BYTE
    0x34, 0x67,                               // BYTE-BYTE
    0x36, 0x6C,                               // BYTE-BYTE
    0x38, 0x71,                               // BYTE-BYTE
    0x3A, 0x76,                               // BYTE-BYTE
    0x3C, 0x7B,                               // BYTE-BYTE
    0x3E, 0x81,                               // BYTE-BYTE
    0x40, 0x87,                               // BYTE-BYTE
    0x42, 0x8C,                               // BYTE-BYTE
    0x44, 0x92,                               // BYTE-BYTE
    0x46, 0x98,                               // BYTE-BYTE
    0x48, 0x9E,                               // BYTE-BYTE
    0x4A, 0xA4,                               // BYTE-BYTE
    0x4C, 0xAB,                               // BYTE-BYTE
    0x4E, 0xB1,                               // BYTE-BYTE
    0x50, 0xB7,                               // BYTE-BYTE
    0x52, 0xBE,                               // BYTE-BYTE
    0x54, 0xC5,                               // BYTE-BYTE
    0x56, 0xCC,                               // BYTE-BYTE
    0x58, 0xD3,                               // BYTE-BYTE
    0x5A, 0xDA,                               // BYTE-BYTE
    0x5C, 0xE1,                               // BYTE-BYTE
    0x5E, 0xE8,                               // BYTE-BYTE
    0x60, 0xF0,                               // BYTE-BYTE
    0x62, 0xF7                                // BYTE-BYTE   Last Lumi/Signal: 98% - 250
  }
};

//
// CPM Zero Power Odd Table
//
AMD_CPM_ZERO_POWER_ODD_TABLE    gCpmZeroPowerOddTable = {
  {CPM_SIGNATURE_ZERO_POWER_ODD, sizeof (gCpmZeroPowerOddTable) / sizeof (UINT8), 0, 0, 0, 0x01},
  DEVICE_ID_ODD,      // Gpio pin
  0x06,               // Gevent pin for FCH_ODD_DA
  0x10,               // Gevent pin for ODD_PLUGIN# Detect
  0x1F,               // Dummy Event
  0x0E,               // SATA Mode Mask
  0x01,               // SATA Port Number
};

//
// Save Context Table
//
AMD_CPM_SAVE_CONTEXT_TABLE           gCpmSaveContextTable = {
  {CPM_SIGNATURE_SAVE_CONTEXT, sizeof (gCpmSaveContextTable) / sizeof (UINT8), 0, 0, 0, 0x01},
  0x05,               // BufferType
  0x50,               // BufferOffset
  0x10,               // BufferSize
};

//
// Adaptive S4 Table
//
AMD_CPM_ADAPTIVE_S4_TABLE           gCpmAdaptiveS4Table = {
  {CPM_SIGNATURE_ADAPTIVE_S4, sizeof (gCpmAdaptiveS4Table) / sizeof (UINT8), 0, 0, 0, 0x01},
  0x05,               // BufferType
  0x40,               // BufferOffset
  0x0C,               // BufferSize
  0xE0                // EcRamOffset
};

// OEM CPM Table definition end


//
// Convert Table between Register Value and DDI Card Id
//
UINT8   DdiCard[] = {
  0x10,   0x00,
  0x20,   0x02,
  0x30,   0x04,
  0x40,   0x03,
  0x50,   0x01,
  0x60,   0x06,
  0x64,   0x0C,
  0x70,   0x06,
  0xFF,   0xFF
};

/*----------------------------------------------------------------------------------------*/
/**
 * Get DDI Card Id
 *
 * This function reads register value of SMBUS device on DDI card and converts it to
 * pre-defined DDI Card Id.
 *
 * @param[in]     This           Pointer to AMD CPM Table PPI
 * @param[in]     Smbus          Smbus Select. 0: Smbus0. 1: Smbus1.
 * @param[in]     Address        Smbus Address
 *
 * @retval        DDI Card Id
 *
 */
UINT8
GetDdiCardId (
  IN       AMD_CPM_TABLE_PPI   *This,
  IN       UINT8               Smbus,
  IN       UINT8               Address
  )
{
  EFI_STATUS      Status;
  UINT8           Data;
  UINT8           index;
  UINT8           Value;

  Value = 0xFF;
  Status = This->CommonFunction.ReadSmbus (This, Smbus, Address, 0, 1, &Data);
  if (!EFI_ERROR (Status)) {
    for (index = 0; index < sizeof (DdiCard); index += 2) {
      if (DdiCard[index] == Data) {
        Value = DdiCard[index + 1];
        break;
      }
    }
  }
  return Value;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Detect PCIe Devices Automatically
 *
 * This function patches PCIe Topology Override Table by detecting PCIe devices
 * automatically.
 *
 * @param[in]     AmdCpmTablePpi                Pointer to AMD CPM Table PPI
 * @param[in]     PcieTopologyOverrideTablePtr  Pointer to PCIe Topology Override Table
 * @param[in]     PcieTopologyTablePtr          Pointer to PCIe Topology Table
 *
 */
VOID
DetectPcieDevices (
  IN       AMD_CPM_TABLE_PPI                       *AmdCpmTablePpi,
  IN       AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_TABLE    *PcieTopologyOverrideTablePtr,
  IN       AMD_CPM_PCIE_TOPOLOGY_TABLE             *PcieTopologyTablePtr
  )
{
  AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_ITEM     *ItemPtr;
  UINT8                                   i;
  UINT8                                   DP_2;
  UINT8                                   DP_3;
  UINT8                                   DP_4;
  UINT8                                   DP_5;

  if (PcieTopologyOverrideTablePtr) {
    ItemPtr = &PcieTopologyOverrideTablePtr->Item[0];

    i = GetDdiCardId (AmdCpmTablePpi, 0, 0x20);       // DP_A
    if (i != 0xFF) {
      ItemPtr->Flag.Raw   = 0x93;                     // Valid, IsDdi, DdiTypeOverride, EnableOverride
      ItemPtr->Offset     = 0;
      ItemPtr->Enable     = PcieDdiEngine;
      ItemPtr->DdiType    = i;
    } else {
      ItemPtr->Flag.Raw   = 0x91;                     // Valid, IsDdi, EnableOverride
      ItemPtr->Offset     = 0;
      ItemPtr->Enable     = PcieUnusedEngine;
    }
    ItemPtr++;

    if (!AmdCpmTablePpi->CommonFunction.DetectDevice (AmdCpmTablePpi, DEVICE_ID_DMC, NULL)) {
      ItemPtr->Flag.Raw   = 0x93;                     // Valid, IsDdi, DdiTypeOverride, EnableOverride
      ItemPtr->Offset     = 1;
      ItemPtr->Enable     = PcieDdiEngine;
      ItemPtr->DdiType    = ConnectorTypeNutmegDpToVga;
    } else {
      i = GetDdiCardId (AmdCpmTablePpi, 1, 0x22);     // DP_B: DMC
      if (i != 0xFF) {
        ItemPtr->Flag.Raw   = 0x93;                   // Valid, IsDdi, DdiTypeOverride, EnableOverride
        ItemPtr->Offset     = 1;
        ItemPtr->Enable     = PcieDdiEngine;
        ItemPtr->DdiType    = i;
      } else {
        i = GetDdiCardId (AmdCpmTablePpi, 1, 0x23);   // DP_B: DMC anyother type card
        if (i != 0xFF) {
          ItemPtr->Flag.Raw   = 0x93;                 // Valid, IsDdi, DdiTypeOverride, EnableOverride
          ItemPtr->Offset     = 1;
          ItemPtr->Enable     = PcieDdiEngine;
          ItemPtr->DdiType    = i;
        } else {
          ItemPtr->Flag.Raw   = 0x93;                 // Valid, IsDdi, DdiTypeOverride, EnableOverride
          ItemPtr->Offset     = 1;
          ItemPtr->Enable     = PcieDdiEngine;
          ItemPtr->DdiType    = ConnectorTypeNutmegDpToVga;
        }
      }
    }
    ItemPtr++;

    i = GetDdiCardId (AmdCpmTablePpi, 0, 0x21);       // DP_C
    if (i != 0xFF) {
      ItemPtr->Flag.Raw   = 0x93;                     // Valid, IsDdi, DdiTypeOverride, EnableOverride
      ItemPtr->Offset     = 2;
      ItemPtr->Enable     = PcieDdiEngine;
      ItemPtr->DdiType    = i;
    } else {
      ItemPtr->Flag.Raw   = 0x91;                     // Valid, IsDdi, EnableOverride
      ItemPtr->Offset     = 2;
      ItemPtr->Enable     = PcieUnusedEngine;
    }
    ItemPtr++;

    if (AmdCpmTablePpi->CommonFunction.GetCpuRevisionId (AmdCpmTablePpi) == CPM_CPU_REVISION_ID_KV) {
      i = GetDdiCardId (AmdCpmTablePpi, 0, 0x22);       // DP_D
      if (i != 0xFF) {
        ItemPtr->Flag.Raw   = 0x93;                     // Valid, IsDdi, DdiTypeOverride, EnableOverride
        ItemPtr->Offset     = 3;
        ItemPtr->Enable     = PcieDdiEngine;
        ItemPtr->DdiType    = i;
      } else {
        ItemPtr->Flag.Raw   = 0x91;                     // Valid, IsDdi, EnableOverride
        ItemPtr->Offset     = 3;
        ItemPtr->Enable     = PcieUnusedEngine;
        ItemPtr++;
        ItemPtr->Flag.Raw   = 0x81;                   // Valid, EnableOverride
        ItemPtr->Offset     = 6;
        ItemPtr->Enable     = PciePortEngine;
        ItemPtr++;
      }
      ItemPtr++;
    }

    if (AmdCpmTablePpi->CommonFunction.DetectDevice (AmdCpmTablePpi, DEVICE_ID_MXM, NULL)) {
      DP_2 = GetDdiCardId (AmdCpmTablePpi, 0, 0x24);  // DP_2
      if (DP_2 != 0xFF) {
        ItemPtr->Flag.Raw   = 0x93;                   // Valid, IsDdi, DdiTypeOverride, EnableOverride
        ItemPtr->Offset     = 4;
        ItemPtr->Enable     = PcieDdiEngine;
        ItemPtr->DdiType    = DP_2;
        if (DP_2 != 3) {
          ItemPtr->Flag.Config.LaneOverride = 1;
          ItemPtr->StartLane  = (UINT8) PcieTopologyTablePtr->Ddi[ItemPtr->Offset].EngineData.StartLane + 4;
          ItemPtr->EndLane    = (UINT8) PcieTopologyTablePtr->Ddi[ItemPtr->Offset].EngineData.EndLane;
        }
      } else {
        ItemPtr->Flag.Raw   = 0x91;                   // Valid, IsDdi, EnableOverride
        ItemPtr->Offset     = 4;
        ItemPtr->Enable     = PcieUnusedEngine;
      }
      ItemPtr++;

      DP_3 = GetDdiCardId (AmdCpmTablePpi, 0, 0x25);  // DP_3
      if (DP_3 != 0xFF) {
        ItemPtr->Flag.Raw   = 0x93;                   // Valid, IsDdi, DdiTypeOverride, EnableOverride
        ItemPtr->Offset     = 5;
        ItemPtr->Enable     = PcieDdiEngine;
        ItemPtr->DdiType    = DP_3;
        if (DP_3 != 3) {
          ItemPtr->Flag.Config.LaneOverride = 1;
          ItemPtr->StartLane  = (UINT8) PcieTopologyTablePtr->Ddi[ItemPtr->Offset].EngineData.StartLane;
          ItemPtr->EndLane    = (UINT8) PcieTopologyTablePtr->Ddi[ItemPtr->Offset].EngineData.EndLane - 4;
        } else {
          ItemPtr--;
          ItemPtr->Flag.Raw   = 0x91;                 // Valid, IsDdi, EnableOverride
          ItemPtr->Offset     = 4;
          ItemPtr->Enable     = PcieUnusedEngine;
          ItemPtr++;
        }
      } else {
        ItemPtr->Flag.Raw   = 0x91;                   // Valid, IsDdi, EnableOverride
        ItemPtr->Offset     = 5;
        ItemPtr->Enable     = PcieUnusedEngine;
      }
      ItemPtr++;

      DP_4 = GetDdiCardId (AmdCpmTablePpi, 0, 0x26);  // DP_4
      if (DP_4 != 0xFF) {
        ItemPtr->Flag.Raw   = 0x93;                   // Valid, IsDdi, DdiTypeOverride, EnableOverride
        ItemPtr->Offset     = 6;
        ItemPtr->Enable     = PcieDdiEngine;
        ItemPtr->DdiType    = DP_4;
        if (DP_4 != 3) {
          DP_5 = GetDdiCardId (AmdCpmTablePpi, 0, 0x27);  // DP_4
          if (DP_5 == 3) {
            ItemPtr->DdiType    = DP_5;
          } else {
            ItemPtr->Flag.Config.LaneOverride = 1;
            ItemPtr->StartLane  = (UINT8) PcieTopologyTablePtr->Ddi[ItemPtr->Offset].EngineData.StartLane + 4;
            ItemPtr->EndLane    = (UINT8) PcieTopologyTablePtr->Ddi[ItemPtr->Offset].EngineData.EndLane;
          }
        }
      } else {
        ItemPtr->Flag.Raw   = 0x91;                   // Valid, IsDdi, EnableOverride
        ItemPtr->Offset     = 6;
        ItemPtr->Enable     = PcieUnusedEngine;
      }
      ItemPtr++;

      if (DP_2 == 0xFF && DP_3 == 0xFF && DP_4 == 0xFF) {
        ItemPtr->Flag.Raw   = 0x81;                   // Valid, EnableOverride
        ItemPtr->Offset     = 0;
        ItemPtr->Enable     = PciePortEngine;
        ItemPtr++;

        ItemPtr->Flag.Raw   = 0x81;                   // Valid, EnableOverride
        ItemPtr->Offset     = 1;
        ItemPtr->Enable     = PcieUnusedEngine;
        ItemPtr++;
      } else {
        ItemPtr->Flag.Raw   = 0x81;                   // Valid, EnableOverride
        ItemPtr->Offset     = 0;
        ItemPtr->Enable     = PcieUnusedEngine;
        ItemPtr++;

        ItemPtr->Flag.Raw   = 0x81;                   // Valid, EnableOverride
        ItemPtr->Offset     = 1;
        ItemPtr->Enable     = PcieUnusedEngine;
        ItemPtr++;
      }
    } else {
      ItemPtr->Flag.Raw   = 0x91;                     // Valid, IsDdi, EnableOverride
      ItemPtr->Offset     = 4;
      ItemPtr->Enable     = PcieUnusedEngine;
      ItemPtr++;

      ItemPtr->Flag.Raw   = 0x91;                     // Valid, IsDdi, EnableOverride
      ItemPtr->Offset     = 5;
      ItemPtr->Enable     = PcieUnusedEngine;
      ItemPtr++;

      ItemPtr->Flag.Raw   = 0x91;                     // Valid, IsDdi, EnableOverride
      ItemPtr->Offset     = 6;
      ItemPtr->Enable     = PcieUnusedEngine;
      ItemPtr++;
    }
    ItemPtr->Flag.Raw = 0xFF;
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Update GPIO Device Config Table
 *
 * @param[in]     GpioDeviceConfigTablePtr  Pointer to GPIO Device Config Table
 * @param[in]     DeviceId                  Device Id to update
 * @param[in]     Mode                      Value. 0: Disable. 1: Enable
 *
 */
VOID
SetDevice (
  IN       AMD_CPM_GPIO_DEVICE_CONFIG_TABLE     *GpioDeviceConfigTablePtr,
  IN       UINT8                                DeviceId,
  IN       UINT8                                Mode
  )
{
  UINT8   Index;
  UINT8   GpioDeviceId;
  for (Index = 0; Index < AMD_GPIO_DEVICE_SIZE; Index ++) {
    GpioDeviceId = GpioDeviceConfigTablePtr->DeviceList[Index].DeviceId;
    if (GpioDeviceId == 0xFF || GpioDeviceId == 0x00) {
      break;
    }
    if (GpioDeviceId == DeviceId) {
      GpioDeviceConfigTablePtr->DeviceList[Index].Config.Setting.Enable = Mode;
      break;
    }
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * CPM Override Function After AMD CPM Table PPI
 *
 * This function updates CPM OEM Tables according to setup options or the value to be detected
 * on run time after AMD CPM Table PPI is installed.
 *
 * @param[in]     PeiServices    Pointer to Pei Services
 *
 * @retval        EFI_SUCCESS    Function initialized successfully
 * @retval        EFI_ERROR      Function failed (see error for more details)
 *
 */
EFI_STATUS
EFIAPI
CpmTableOverride (
  IN       EFI_PEI_SERVICES       **PeiServices
  )
{
  EFI_STATUS                      Status;
  AMD_CPM_TABLE_PPI               *AmdCpmTablePpi;
  AMD_CPM_DISPLAY_FEATURE_TABLE   *DisplayFeatureTablePtr;
  AMD_CPM_MAIN_TABLE              *MainTablePtr;
  AMD_CPM_PCIE_CLOCK_TABLE        *PcieClockTablePtr;
  AMD_CPM_GPIO_DEVICE_CONFIG_TABLE      *GpioDeviceConfigTablePtr;
  AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_TABLE  *PcieTopologyOverrideTablePtr;
  AMD_CPM_PCIE_TOPOLOGY_TABLE     *PcieTopologyTablePtr;
  CPM_OEM_SETUP_OPTION            OemSetupOption;
  UINT8                           Index;

  Status = (*PeiServices)->LocatePpi (
                           PeiServices,
                           &gAmdCpmTablePpiGuid,
                           0,
                           NULL,
                           &AmdCpmTablePpi
                           );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  MainTablePtr                  = AmdCpmTablePpi->MainTablePtr;
  DisplayFeatureTablePtr        = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_DISPLAY_FEATURE);
  PcieClockTablePtr             = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_PCIE_CLOCK);
  PcieTopologyTablePtr          = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_PCIE_TOPOLOGY);
  GpioDeviceConfigTablePtr      = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_GPIO_DEVICE_CONFIG);
  PcieTopologyOverrideTablePtr  = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_PCIE_TOPOLOGY_OVERRIDE);

  Status = CpmOemSetupOption (PeiServices, &OemSetupOption);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  MainTablePtr->DisplayFeature.Raw &= 0xFFFFFC00;

  switch (OemSetupOption.SpecialVgaFeature) {
  case 1: // HCF_dGPU
    MainTablePtr->DisplayFeature.Raw |= (1 << DFC_HYPER_CROSS_FIRE);
    break;
  case 2: // Surround View
    MainTablePtr->DisplayFeature.Raw |= (1 << DFC_SURROUND_VIEW);
    break;
  case 3: //PX
    if (OemSetupOption.PowerExpressDynamicMode) {
      MainTablePtr->DisplayFeature.Raw |= OemSetupOption.PowerExpressDynamicMode << 1;
    } else {
      MainTablePtr->DisplayFeature.Raw |= BIT0;
    }
    if (OemSetupOption.PX56Support) {
      MainTablePtr->DisplayFeature.Raw |= BIT12;
      if (OemSetupOption.PX56Support == 2) {
        MainTablePtr->DisplayFeature.Raw |= BIT14;
      }
    }
    break;
  }
#ifdef CPM_SKIP_SSID_OVERRIDE_CONTROL
  if (OemSetupOption.SkipSsidOverride) {
    DisplayFeatureTablePtr->FunctionDisableMask |= 1;
  }
#endif

  if ((OemSetupOption.PrimaryVideoAdaptor == 2)) {
    MainTablePtr->DisplayFeature.Raw |= 1 << 8;
  }

  if (OemSetupOption.BrightnessControlMethod == 1) {
    MainTablePtr->DisplayFeature.Raw |= 1 << 9;
  }

  if (OemSetupOption.DisplayOutput == 0) {
    SetDevice (GpioDeviceConfigTablePtr, DEVICE_ID_VGAMUXSEL, CPM_DEVICE_ON);
  } else {
    SetDevice (GpioDeviceConfigTablePtr, DEVICE_ID_VGAMUXSEL, CPM_DEVICE_OFF);
  }

  if (OemSetupOption.BlueToothEn) {
    SetDevice (GpioDeviceConfigTablePtr, DEVICE_ID_BT, CPM_DEVICE_OFF);
  } else {
    SetDevice (GpioDeviceConfigTablePtr, DEVICE_ID_BT, CPM_DEVICE_ON);
  }

  MainTablePtr->ZeroPowerOddEn = 0;
  if (OemSetupOption.ZeroPowerOddEn) {
    MainTablePtr->ZeroPowerOddEn = BIT0 | BIT1 | BIT3;
  }
  if (OemSetupOption.SystemBootWithPS0 == 0) {
    MainTablePtr->ZeroPowerOddEn |= BIT2;
  }

  MainTablePtr->UnusedGppClkOffEn = OemSetupOption.UnusedGppClkOff;
  MainTablePtr->ExtClkGen         = OemSetupOption.SpreadSpectrumSwitch;

  MainTablePtr->AcpiThermalFanEn  = OemSetupOption.AcpiThermalFanEn ? 0 : 1;
  MainTablePtr->AdaptiveS4En      = OemSetupOption.AdaptiveS4En;
  MainTablePtr->WirelessButtonEn  = OemSetupOption.WirelessSwitch + 1;
  MainTablePtr->Ec.Config.S5PlusEn = 1;

  DetectPcieDevices (AmdCpmTablePpi, PcieTopologyOverrideTablePtr, PcieTopologyTablePtr);

  if (PcieClockTablePtr && OemSetupOption.UnusedGppClkOff && !OemSetupOption.ClockRequest) {
    Index = 0;
    while (PcieClockTablePtr->Item[Index].ClkId != 0xFF) {
      if (PcieClockTablePtr->Item[Index].ClkReq != CLK_DISABLE) {
        PcieClockTablePtr->Item[Index].ClkReq = CLK_ENABLE;
      }
      if (PcieClockTablePtr->Item[Index].ClkReqExt != CLK_DISABLE) {
        PcieClockTablePtr->Item[Index].ClkReqExt = CLK_ENABLE;
      }
      Index++;
    }
  }

  return Status;
}
