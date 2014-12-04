/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM OEM API, and related functions.
 *
 * Contains the table definition for GPIO Device.
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
#ifndef _GPIO_DEVICES_TABLE_H_
#define _GPIO_DEVICES_TABLE_H_
#include "AmdCpmOemInitPeim.h"

//
// CPM GPIO Module
//

//
// This table is used to define the initialize sequence of the on board devices
//
AMD_CPM_GPIO_DEVICE_CONFIG_TABLE    gCpmGpioDeviceConfigTable = {
  {CPM_SIGNATURE_GPIO_DEVICE_CONFIG, sizeof (gCpmGpioDeviceConfigTable) / sizeof (UINT8), 0, 0, 0, 0x0F},
  {
    //                      DeviceId                Enable              Assert  Deassert   Hotplug
    GPIO_DEVICE_DEFINITION (DEVICE_ID_GFX0,         CPM_DEVICE_ON,      1,      1,         0      ),
    GPIO_DEVICE_DEFINITION (DEVICE_ID_GFX1,         CPM_DEVICE_ON,      1,      1,         0      ),
    GPIO_DEVICE_DEFINITION (DEVICE_ID_APU_GPP,      CPM_DEVICE_ON,      0,      0,         0      ),
    GPIO_DEVICE_DEFINITION (DEVICE_ID_GFX_LINK_CFG, CPM_DEVICE_ON,      0,      0,         0      ),
    GPIO_DEVICE_DEFINITION (DEVICE_ID_LAN,          CPM_DEVICE_ON,      0,      0,         0      ),
    GPIO_DEVICE_DEFINITION (DEVICE_ID_MPCIE,        CPM_DEVICE_ON,      0,      0,         0      ),
    GPIO_DEVICE_DEFINITION (DEVICE_ID_WLAN_RF,      CPM_DEVICE_ON,      0,      0,         0      ),
    GPIO_DEVICE_DEFINITION (DEVICE_ID_FRONT_AUDIO,  CPM_DEVICE_AUTO,    0,      0,         0      ),
    0xFF,
  }
};

//
// CPM Device Detection Table
//
AMD_CPM_GPIO_DEVICE_DETECTION_TABLE gCpmGpioDeviceDetectionTable = {
  {CPM_SIGNATURE_GPIO_DEVICE_DETECTION, sizeof (gCpmGpioDeviceDetectionTable) / sizeof (UINT8), 0, 0, 0, 0x0F},
  { //  Device_ID                Type                PinNum1 Value1  PinNum2 Value2  PinNum3 Value3
    {   DEVICE_ID_PCIE_CARD,     CPM_TYPE_GPIO_3_OR,  32,    1,      33,    1,       34,    1   },
//    {   DEVICE_ID_CARD0,         CPM_TYPE_GPIO_1,     32,    1,      0,      0,      0,      0   },
//    {   DEVICE_ID_CARD1,         CPM_TYPE_GPIO_1,     33,    1,      0,      0,      0,      0   },
//    {   DEVICE_ID_CARD2,         CPM_TYPE_GPIO_1,     34,    1,      0,      0,      0,      0   },
    {   DEVICE_ID_DUNGLE_DP2,    CPM_TYPE_GPIO_1,    191,    0,      0,      0,      0,      0   },
    {   DEVICE_ID_DUNGLE_DP3,    CPM_TYPE_GPIO_1,    192,    0,      0,      0,      0,      0   },
    {   DEVICE_ID_DUNGLE_DP4,    CPM_TYPE_GPIO_1,    197,    0,      0,      0,      0,      0   },
    {   DEVICE_ID_DUNGLE_DP5,    CPM_TYPE_GPIO_1,    198,    0,      0,      0,      0,      0   },
    {   DEVICE_ID_FRONT_AUDIO,   CPM_TYPE_GPIO_1,     59,    0,      0,      0,      0,      0   },
    {   DEVICE_ID_DAP,           CPM_TYPE_GPIO_1,     17,    0,      0,      0,      0,      0   },
    0xFF,
  }
};

//
// CPM Device Reset Table
//
AMD_CPM_GPIO_DEVICE_RESET_TABLE gCpmGpioDeviceResetTable = {
  {CPM_SIGNATURE_GPIO_DEVICE_RESET, sizeof (gCpmGpioDeviceResetTable) / sizeof (UINT8), 0, 0, 0, 0x0F},
  {
    //     DeviceId                  Mode               Type                    Num Value       InitFlag;
    {   DEVICE_ID_GFX0,          CPM_RESET_ASSERT,   CPM_RESET_GPIO,     CPM_GPIO_PIN (175, 0),      0   },
    {   DEVICE_ID_GFX1,          CPM_RESET_ASSERT,   CPM_RESET_GPIO,     CPM_GPIO_PIN (176, 0),      0   },
    {   DEVICE_ID_APU_GPP,       CPM_RESET_ASSERT,   CPM_RESET_GPIO,     CPM_GPIO_PIN (177, 0),      0   },

    {   DEVICE_ID_GFX0,          CPM_RESET_DELAY,    0,                  500,                        0   },
    {   DEVICE_ID_GFX1,          CPM_RESET_DELAY,    0,                  500,                        0   },

    {   DEVICE_ID_GFX0,          CPM_RESET_DEASSERT, CPM_RESET_GPIO,     CPM_GPIO_PIN (175, 1),      0   },
    {   DEVICE_ID_GFX1,          CPM_RESET_DEASSERT, CPM_RESET_GPIO,     CPM_GPIO_PIN (176, 1),      0   },
    {   DEVICE_ID_APU_GPP,       CPM_RESET_DEASSERT, CPM_RESET_GPIO,     CPM_GPIO_PIN (177, 1),      0   },
    0xFF,
  }
};

//
// CPM GPIO Device Init Table (Power On/Off)
//
AMD_CPM_GPIO_DEVICE_POWER_TABLE gCpmGpioDevicePowerTable = {
  {CPM_SIGNATURE_GPIO_DEVICE_POWER, sizeof (gCpmGpioDevicePowerTable) / sizeof (UINT8), 0, 0, 0, 0x0F},
  {
    //  DeviceId                Mode                Type                Config                  InitFlag;
//    {   DEVICE_ID_DDI,          CPM_POWER_OFF,      CPM_POWER_SET,      CPM_GPIO_PIN(50, 0),    0   },      // PCIe DDI card
    {   DEVICE_ID_GFX_LINK_CFG, CPM_POWER_OFF,      CPM_POWER_SET,      CPM_GPIO_PIN(5, 0),     0   },      // GFX link configuration
    {   DEVICE_ID_LAN,          CPM_POWER_OFF,      CPM_POWER_SET,      CPM_GPIO_PIN(183, 1),   0   },      // On Board LAN
    {   DEVICE_ID_MPCIE,        CPM_POWER_OFF,      CPM_POWER_SET,      CPM_GPIO_PIN(1, 0),     0   },      // MPCIE_PWRON
    {   DEVICE_ID_WLAN_RF,      CPM_POWER_OFF,      CPM_POWER_SET,      CPM_GPIO_PIN(0, 0),     0   },      // MPCIE WL_DISABLE#

//    {   DEVICE_ID_DDI,          CPM_POWER_ON,       CPM_POWER_SET,      CPM_GPIO_PIN(50, 1),    0   },      // PCIe DDI card
    {   DEVICE_ID_GFX_LINK_CFG, CPM_POWER_ON,       CPM_POWER_SET,      CPM_GPIO_PIN(5, 1),     0   },      // GFX link configuration
    {   DEVICE_ID_LAN,          CPM_POWER_ON,       CPM_POWER_SET,      CPM_GPIO_PIN(183, 0),   0   },      // On Board LAN
    {   DEVICE_ID_MPCIE,        CPM_POWER_ON,       CPM_POWER_SET,      CPM_GPIO_PIN(1, 1),     0   },      // MPCIE_PWRON
    {   DEVICE_ID_WLAN_RF,      CPM_POWER_ON,       CPM_POWER_SET,      CPM_GPIO_PIN(0, 1),     0   },      // MPCIE WL_DISABLE#
    0xFF,
  }
};
#endif