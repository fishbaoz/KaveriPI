/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM OEM API, and related functions.
 *
 * Contains OEM code that defined CPM tables before CPM init.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  OEM
 * @e \$Revision: 290554 $   @e \$Date: 2014-04-23 10:58:35 -0500 (Wed, 23 Apr 2014) $
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

#ifndef _AMD_CPM_OEM_INIT_PEIM_H_
#define _AMD_CPM_OEM_INIT_PEIM_H_

#define AMD_CPM_OEM_REVISION    0x1105

#ifndef AMD_CPM_SETUP_OPTION_SPECIAL_VGA_FEATURE
#define AMD_CPM_SETUP_OPTION_SPECIAL_VGA_FEATURE        SpecialVgaFeature
#endif

#ifndef AMD_CPM_SETUP_OPTION_PRIMARY_VIDEO_ADAPTOR
#define AMD_CPM_SETUP_OPTION_PRIMARY_VIDEO_ADAPTOR      PrimaryVideoAdaptor
#endif

#ifndef AMD_CPM_SETUP_OPTION_UNUSED_GPP_CLOCK_OFF
#define AMD_CPM_SETUP_OPTION_UNUSED_GPP_CLOCK_OFF       UnusedGppClkOff
#endif

#ifndef AMD_CPM_SETUP_OPTION_ACPI_THERMAL_FAN_EN
#define AMD_CPM_SETUP_OPTION_ACPI_THERMAL_FAN_EN        ThermalFanControl
#endif

#ifndef AMD_CPM_SETUP_OPTION_GFX_LINK_CFG
#define AMD_CPM_SETUP_OPTION_GFX_LINK_CFG               GfxLinkCfg
#endif

#ifndef AMD_CPM_SETUP_OPTION_ON_BOARD_LAN_EN
#define AMD_CPM_SETUP_OPTION_ON_BOARD_LAN_EN            OnboardLanSwitch
#endif

#ifndef AMD_CPM_SETUP_OPTION_MINI_PCIE_EN
#define AMD_CPM_SETUP_OPTION_MINI_PCIE_EN               MiniPcieSlotEnable
#endif

#ifndef AMD_CPM_SETUP_OPTION_ADAPTIVE_S4_EN
#define AMD_CPM_SETUP_OPTION_ADAPTIVE_S4_EN             AdaptiveS4En
#endif

#ifndef AMD_CPM_SETUP_OPTION_WIRELESS_LAN_RF_EN
#define AMD_CPM_SETUP_OPTION_WIRELESS_LAN_RF_EN         WirelessLanRfEnable
#endif

#ifndef AMD_CPM_SETUP_OPTION_PCIE_DDI_DETECTION
#define AMD_CPM_SETUP_OPTION_PCIE_DDI_DETECTION         PcieDdiDetection
#endif

#ifndef AMD_CPM_SETUP_OPTION_CLKREQ
#define AMD_CPM_SETUP_OPTION_CLKREQ                     ClkReq
#endif

#ifdef CPM_SKIP_SSID_OVERRIDE_CONTROL
#ifndef AMD_CPM_SETUP_OPTION_SKIP_SSID_OVERRIDE
#define AMD_CPM_SETUP_OPTION_SKIP_SSID_OVERRIDE         SkipSsidOverride
#endif
#endif

#define GPIO_STICKY_DIS     0
#define GPIO_STICKY_EN      1
#define GPIO_FUNCTION_0     0
#define GPIO_FUNCTION_1     1
#define GPIO_FUNCTION_2     2
#define GPIO_FUNCTION_3     3
#define GPIO_FUNCTION_4     4
#define GPIO_FUNCTION_5     5
#define GPIO_FUNCTION_6     6
#define GPIO_FUNCTION_7     7
#define GPIO_PD_EN          0
#define GPIO_PU_EN          1
#define GPIO_PU_PD_DIS      2
#define GPIO_OUTPUT_LOW     0
#define GPIO_OUTPUT_HIGH    1
#define GPIO_INPUT          2
#define GPIO_NA             3

#define EVENT_DISABLE               0
#define EVENT_ENABLE                1
#define SCITRIG_LOW                 0
#define SCITRIG_HI                  1
#define SCILEVEL_EDGE               0
#define SCILEVEL_LEVEL              1
#define SMISCI_DISABLE              0
#define SMISCI_ENABLE               1
#define SCIS0_DISABLE               0
#define SCIS0_ENABLE                1
#define SCIMAP_00                   0
#define SCIMAP_01                   1
#define SCIMAP_02                   2
#define SCIMAP_03                   3
#define SCIMAP_04                   4
#define SCIMAP_05                   5
#define SCIMAP_06                   6
#define SCIMAP_07                   7
#define SCIMAP_08                   8
#define SCIMAP_09                   9
#define SCIMAP_10                   10
#define SCIMAP_11                   11
#define SCIMAP_12                   12
#define SCIMAP_13                   13
#define SCIMAP_14                   14
#define SCIMAP_15                   15
#define SCIMAP_16                   16
#define SCIMAP_17                   17
#define SCIMAP_18                   18
#define SCIMAP_19                   19
#define SCIMAP_20                   20
#define SCIMAP_21                   21
#define SCIMAP_22                   22
#define SCIMAP_23                   23
#define SMITRIG_LOW                 0
#define SMITRIG_HI                  1
#define SMICONTROL_DISABLE          0
#define SMICONTROL_SMI              1
#define SMICONTROL_NMI              2
#define SMICONTROL_IRQ13            3

//
// FCH GPP
//
#ifndef FCH_GPP0_BUS
  #define FCH_GPP0_BUS                  0
#endif
#ifndef FCH_GPP0_DEV
  #define FCH_GPP0_DEV                  21
#endif
#ifndef FCH_GPP0_FUNC
  #define FCH_GPP0_FUNC                 0
#endif
#ifndef FCH_GPP1_BUS
  #define FCH_GPP1_BUS                  0
#endif
#ifndef FCH_GPP1_DEV
  #define FCH_GPP1_DEV                  21
#endif
#ifndef FCH_GPP1_FUNC
  #define FCH_GPP1_FUNC                 1
#endif
#ifndef FCH_GPP2_BUS
  #define FCH_GPP2_BUS                  0
#endif
#ifndef FCH_GPP2_DEV
  #define FCH_GPP2_DEV                  21
#endif
#ifndef FCH_GPP2_FUNC
  #define FCH_GPP2_FUNC                 2
#endif
#ifndef FCH_GPP3_BUS
  #define FCH_GPP3_BUS                  0
#endif
#ifndef FCH_GPP3_DEV
  #define FCH_GPP3_DEV                  21
#endif
#ifndef FCH_GPP3_FUNC
  #define FCH_GPP3_FUNC                 3
#endif

//
// Trinity APU GFX/GPP
//
#ifndef APU_GFX0_BUS_TN
  #define APU_GFX0_BUS_TN               0
#endif
#ifndef APU_GFX0_DEV_TN
  #define APU_GFX0_DEV_TN               2
#endif
#ifndef APU_GFX0_FUNC_TN
  #define APU_GFX0_FUNC_TN              0
#endif
#ifndef APU_GFX1_BUS_TN
  #define APU_GFX1_BUS_TN               0
#endif
#ifndef APU_GFX1_DEV_TN
  #define APU_GFX1_DEV_TN               3
#endif
#ifndef APU_GFX1_FUNC_TN
  #define APU_GFX1_FUNC_TN              0
#endif
#ifndef APU_GPP0_BUS_TN
  #define APU_GPP0_BUS_TN               0
#endif
#ifndef APU_GPP0_DEV_TN
  #define APU_GPP0_DEV_TN               4
#endif
#ifndef APU_GPP0_FUNC_TN
  #define APU_GPP0_FUNC_TN              0
#endif
#ifndef APU_GPP1_BUS_TN
  #define APU_GPP1_BUS_TN               0
#endif
#ifndef APU_GPP1_DEV_TN
  #define APU_GPP1_DEV_TN               5
#endif
#ifndef APU_GPP1_FUNC_TN
  #define APU_GPP1_FUNC_TN              0
#endif
#ifndef APU_GPP2_BUS_TN
  #define APU_GPP2_BUS_TN               0
#endif
#ifndef APU_GPP2_DEV_TN
  #define APU_GPP2_DEV_TN               6
#endif
#ifndef APU_GPP2_FUNC_TN
  #define APU_GPP2_FUNC_TN              0
#endif
#ifndef APU_GPP3_BUS_TN
  #define APU_GPP3_BUS_TN               0
#endif
#ifndef APU_GPP3_DEV_TN
  #define APU_GPP3_DEV_TN               7
#endif
#ifndef APU_GPP3_FUNC_TN
  #define APU_GPP3_FUNC_TN              0
#endif

#define APU_FCH_UMI_BUS_TN              0
#define APU_FCH_UMI_DEV_TN              8
#define APU_FCH_UMI_FUNC_TN             0

#define APU_GFX0_DF_TN                  (APU_GFX0_DEV_TN << 3) + APU_GFX0_FUNC_TN
#define APU_GFX1_DF_TN                  (APU_GFX1_DEV_TN << 3) + APU_GFX1_FUNC_TN
#define APU_GPP0_DF_TN                  (APU_GPP0_DEV_TN << 3) + APU_GPP0_FUNC_TN
#define APU_GPP1_DF_TN                  (APU_GPP1_DEV_TN << 3) + APU_GPP1_FUNC_TN
#define APU_GPP2_DF_TN                  (APU_GPP2_DEV_TN << 3) + APU_GPP2_FUNC_TN
#define APU_GPP3_DF_TN                  (APU_GPP3_DEV_TN << 3) + APU_GPP3_FUNC_TN

//
// Kaveri APU GFX/GPP
//
#ifndef APU_GFX0_BUS_KV
  #define APU_GFX0_BUS_KV               0
#endif
#ifndef APU_GFX0_DEV_KV
  #define APU_GFX0_DEV_KV               2
#endif
#ifndef APU_GFX0_FUNC_KV
  #define APU_GFX0_FUNC_KV              1
#endif
#ifndef APU_GFX1_BUS_KV
  #define APU_GFX1_BUS_KV               0
#endif
#ifndef APU_GFX1_DEV_KV
  #define APU_GFX1_DEV_KV               2
#endif
#ifndef APU_GFX1_FUNC_KV
  #define APU_GFX1_FUNC_KV              2
#endif
#ifndef APU_GPP0_BUS_KV
  #define APU_GPP0_BUS_KV               0
#endif
#ifndef APU_GPP0_DEV_KV
  #define APU_GPP0_DEV_KV               3
#endif
#ifndef APU_GPP0_FUNC_KV
  #define APU_GPP0_FUNC_KV              1
#endif
#ifndef APU_GPP1_BUS_KV
  #define APU_GPP1_BUS_KV               0
#endif
#ifndef APU_GPP1_DEV_KV
  #define APU_GPP1_DEV_KV               3
#endif
#ifndef APU_GPP1_FUNC_KV
  #define APU_GPP1_FUNC_KV              2
#endif
#ifndef APU_GPP2_BUS_KV
  #define APU_GPP2_BUS_KV               0
#endif
#ifndef APU_GPP2_DEV_KV
  #define APU_GPP2_DEV_KV               3
#endif
#ifndef APU_GPP2_FUNC_KV
  #define APU_GPP2_FUNC_KV              3
#endif
#ifndef APU_GPP3_BUS_KV
  #define APU_GPP3_BUS_KV               0
#endif
#ifndef APU_GPP3_DEV_KV
  #define APU_GPP3_DEV_KV               3
#endif
#ifndef APU_GPP3_FUNC_KV
  #define APU_GPP3_FUNC_KV              4
#endif

#define APU_FCH_UMI_BUS_KV              0
#define APU_FCH_UMI_DEV_KV              4
#define APU_FCH_UMI_FUNC_KV             1

#define APU_GFX0_DF_KV                  (APU_GFX0_DEV_KV << 3) + APU_GFX0_FUNC_KV
#define APU_GFX1_DF_KV                  (APU_GFX1_DEV_KV << 3) + APU_GFX1_FUNC_KV
#define APU_GPP0_DF_KV                  (APU_GPP0_DEV_KV << 3) + APU_GPP0_FUNC_KV
#define APU_GPP1_DF_KV                  (APU_GPP1_DEV_KV << 3) + APU_GPP1_FUNC_KV
#define APU_GPP2_DF_KV                  (APU_GPP2_DEV_KV << 3) + APU_GPP2_FUNC_KV
#define APU_GPP3_DF_KV                  (APU_GPP3_DEV_KV << 3) + APU_GPP3_FUNC_KV

//
// iGPU/dGPU
//
#define INTERNAL_VGA_BUS                0 //1
#define INTERNAL_VGA_DEV                1 //5
#define INTERNAL_VGA_FUNC               0

#define EXTERNAL_VGA_BUS                1
#define EXTERNAL_VGA_DEV                0
#define EXTERNAL_VGA_FUNC               0

//
// Triniti APU Lane ID mapping
//
#define PCIe_PORT_DESCRIPTOR_GFX0_TN    0
#define PCIe_PORT_DESCRIPTOR_GFX1_TN    PCIe_PORT_DESCRIPTOR_GFX0_TN + 1

#define PCIE_GPP_IDX_START_TN           PCIe_PORT_DESCRIPTOR_GFX1_TN + 1
#define PCIe_PORT_DESCRIPTOR_GPP0_TN    PCIE_GPP_IDX_START_TN
#define PCIe_PORT_DESCRIPTOR_GPP1_TN    PCIe_PORT_DESCRIPTOR_GPP0_TN + 1
#define PCIe_PORT_DESCRIPTOR_GPP2_TN    PCIe_PORT_DESCRIPTOR_GPP1_TN + 1
#define PCIe_PORT_DESCRIPTOR_GPP3_TN    PCIe_PORT_DESCRIPTOR_GPP2_TN + 1
#define PCIe_PORT_DESCRIPTOR_UMI_TN     PCIe_PORT_DESCRIPTOR_GPP3_TN + 1 // APU <-> Hudson

#define PCIe_DDI_DESCRIPTOR_DP0_TN      0
#define PCIe_DDI_DESCRIPTOR_DP1_TN      PCIe_DDI_DESCRIPTOR_DP0_TN + 1
#define PCIe_DDI_DESCRIPTOR_DP2_TN      PCIe_DDI_DESCRIPTOR_DP1_TN + 1
// number of Display port: DP_0, DP_1, DP_2
#define DP_TOTAL_TN                     PCIe_DDI_DESCRIPTOR_DP2_TN - PCIe_DDI_DESCRIPTOR_DP0_TN + 1

#define DDI_IDX_START_TN                PCIe_DDI_DESCRIPTOR_DP2_TN + 1
#define PCIe_DDI_DESCRIPTOR_DDI0_TN     DDI_IDX_START_TN
#define PCIe_DDI_DESCRIPTOR_DDI1_TN     PCIe_DDI_DESCRIPTOR_DDI0_TN + 1
#define PCIe_DDI_DESCRIPTOR_DDI2_TN     PCIe_DDI_DESCRIPTOR_DDI1_TN + 1
#define PCIe_DDI_DESCRIPTOR_DDI3_TN     PCIe_DDI_DESCRIPTOR_DDI2_TN + 1
#define DDI_PORT_TOTAL_TN               PCIe_DDI_DESCRIPTOR_DDI3_TN - PCIe_DDI_DESCRIPTOR_DDI0_TN + 1

#define GFX0_LANE_START_TN              23
#define GFX0_LANE_END_TN                8
#define GFX1_LANE_START_TN              15
#define GFX1_LANE_END_TN                8
#define DP0_LANE_START_TN               24
#define DP0_LANE_END_TN                 27
#define DP1_LANE_START_TN               28
#define DP1_LANE_END_TN                 31
#define DP2_LANE_START_TN               32
#define DP2_LANE_END_TN                 35
#define DP_DLDVI_LANE_START_TN          28
#define DP_DLDVI_LANE_END_TN            35
#define P_GPP0_LANE_START_TN            4
#define P_GPP0_LANE_END_TN              4
#define P_GPP1_LANE_START_TN            5
#define P_GPP1_LANE_END_TN              5
#define P_GPP2_LANE_START_TN            6
#define P_GPP2_LANE_END_TN              6
#define P_GPP3_LANE_START_TN            7
#define P_GPP3_LANE_END_TN              7
#define DDI0_LANE_START_TN              23
#define DDI0_LANE_END_TN                20
#define DDI1_LANE_START_TN              19
#define DDI1_LANE_END_TN                16
#define DDI2_LANE_START_TN              15
#define DDI2_LANE_END_TN                12
#define DDI3_LANE_START_TN              11
#define DDI3_LANE_END_TN                8
#define APU_FCH_UMI_LANE_START_TN       0
#define APU_FCH_UMI_LANE_END_TN         3

//
// Kaveri APU Lane ID mapping
//
#define PCIe_PORT_DESCRIPTOR_GFX0_KV    0
#define PCIe_PORT_DESCRIPTOR_GFX1_KV    PCIe_PORT_DESCRIPTOR_GFX0_KV + 1

#define PCIE_GPP_IDX_START_KV           PCIe_PORT_DESCRIPTOR_GFX1_KV + 1
#define PCIe_PORT_DESCRIPTOR_GPP0_KV    PCIE_GPP_IDX_START_KV
#define PCIe_PORT_DESCRIPTOR_GPP1_KV    PCIe_PORT_DESCRIPTOR_GPP0_KV + 1
#define PCIe_PORT_DESCRIPTOR_GPP2_KV    PCIe_PORT_DESCRIPTOR_GPP1_KV + 1
#define PCIe_PORT_DESCRIPTOR_GPP3_KV    PCIe_PORT_DESCRIPTOR_GPP2_KV + 1
#define PCIe_PORT_DESCRIPTOR_UMI_KV     PCIe_PORT_DESCRIPTOR_GPP3_KV + 1  // APU <-> Hudson

#define PCIe_DDI_DESCRIPTOR_DP0_KV      0
#define PCIe_DDI_DESCRIPTOR_DP1_KV      PCIe_DDI_DESCRIPTOR_DP0_KV + 1
#define PCIe_DDI_DESCRIPTOR_DP2_KV      PCIe_DDI_DESCRIPTOR_DP1_KV + 1
//#define PCIe_DDI_DESCRIPTOR_DP3_KV      PCIe_DDI_DESCRIPTOR_DP2_KV + 1
#define DP_TOTAL_KV                     PCIe_DDI_DESCRIPTOR_DP2_KV - PCIe_DDI_DESCRIPTOR_DP0_KV + 1

#define DDI_IDX_START_KV                PCIe_DDI_DESCRIPTOR_DP2_KV + 1
#define PCIe_DDI_DESCRIPTOR_DDI0_KV     DDI_IDX_START_KV
#define PCIe_DDI_DESCRIPTOR_DDI1_KV     PCIe_DDI_DESCRIPTOR_DDI0_KV + 1
#define PCIe_DDI_DESCRIPTOR_DDI2_KV     PCIe_DDI_DESCRIPTOR_DDI1_KV + 1
#define PCIe_DDI_DESCRIPTOR_DDI3_KV     PCIe_DDI_DESCRIPTOR_DDI2_KV + 1
#define DDI_PORT_TOTAL_KV               PCIe_DDI_DESCRIPTOR_DDI3_KV - PCIe_DDI_DESCRIPTOR_DDI0_KV + 1

#define GFX0_LANE_START_KV              31
#define GFX0_LANE_END_KV                16
#define GFX1_LANE_START_KV              23
#define GFX1_LANE_END_KV                16
#define DP0_LANE_START_KV               4
#define DP0_LANE_END_KV                 7
//#define DP1_LANE_START_KV               32
//#define DP1_LANE_END_KV                 35
#define DP1_LANE_START_KV               12
#define DP1_LANE_END_KV                 15
//#define DP2_LANE_START_KV               36
//#define DP2_LANE_END_KV                 39
#define DP2_LANE_START_KV               32
#define DP2_LANE_END_KV                 35
#define DP_DLDVI_LANE_START_KV          32
#define DP_DLDVI_LANE_END_KV            39
//#define DP3_LANE_START_KV               12
//#define DP3_LANE_END_KV                 15
#define P_GPP0_LANE_START_KV            8
#define P_GPP0_LANE_END_KV              8
#define P_GPP1_LANE_START_KV            9
#define P_GPP1_LANE_END_KV              9
#define P_GPP2_LANE_START_KV            10
#define P_GPP2_LANE_END_KV              10
#define P_GPP3_LANE_START_KV            11
#define P_GPP3_LANE_END_KV              11
#define DDI0_LANE_START_KV              31
#define DDI0_LANE_END_KV                28
#define DDI1_LANE_START_KV              27
#define DDI1_LANE_END_KV                24
#define DDI2_LANE_START_KV              23
#define DDI2_LANE_END_KV                20
#define DDI3_LANE_START_KV              19
#define DDI3_LANE_END_KV                16
#define APU_FCH_UMI_LANE_START_KV       0
#define APU_FCH_UMI_LANE_END_KV         3


/// Physical or virtual on-board device Id.

typedef enum {
  DEVICE_ID_GFX0   =                  0x01,                       ///< Device Id for GFX0
  DEVICE_ID_GFX1,                                                 ///< Device Id for GFX1
  DEVICE_ID_APU_GPP,                                              ///< Device Id for APU GPP
  DEVICE_ID_SB_GPP0,                                              ///< Device Id for SB GPP0
  DEVICE_ID_SB_GPP1,                                              ///< Device Id for SB GPP1
  DEVICE_ID_SB_GPP2,                                              ///< Device Id for SB GPP2
  DEVICE_ID_SB_GPP3,                                              ///< Device Id for SB GPP3
  DEVICE_ID_DP1_HP,                                               ///< Device Id for DP1 hot plug detect
  DEVICE_ID_MPCIE,                                                ///< Device Id for Mini PCIE
  DEVICE_ID_LAN,                                                  ///< Device Id for On Board LAN
  DEVICE_ID_FRONT_AUDIO,                                          ///< Device Id for Front Audio
  DEVICE_ID_WLAN_RF,                                              ///< Device Id for WLAN
  DEVICE_ID_PCIE_CARD,                                            ///< Device Id for PCIE card
//  DEVICE_ID_CARD0,                                                ///< Device Id for PCIE card PinNum1
//  DEVICE_ID_CARD1,                                                ///< Device Id for PCIE card PinNum2
//  DEVICE_ID_CARD2,                                                ///< Device Id for PCIE card PinNum3
  DEVICE_ID_DDI,                                                  ///< Device Id for DDI card
  DEVICE_ID_DUNGLE_DP2,                                           ///< Device Id for DP2 attached device
  DEVICE_ID_DUNGLE_DP3,                                           ///< Device Id for DP3 attached device
  DEVICE_ID_DUNGLE_DP4,                                           ///< Device Id for DP4 attached device
  DEVICE_ID_DUNGLE_DP5,                                           ///< Device Id for DP5 attached device
  DEVICE_ID_VGAMUXSEL,                                            ///< Device Id for VGA MUX
  DEVICE_ID_GFX_LINK_CFG,                                         ///< Device Id for GFX link configuration
  DEVICE_ID_DAP,                                                  ///< Device Id for Bantry DAP board
  DEVICE_ID_APU_SB_UMI,                                           ///< Device Id for APU GPP
} CPM_GPIO_DEVICE_ID;


/// Configuration values for GppClk in Fch

typedef enum {
  GPP_CLK0 =                          0x00,                       ///< GPP_CLK0
  GPP_CLK1,                                                       ///< GPP_CLK1
  GPP_CLK2,                                                       ///< GPP_CLK2
  GPP_CLK3,                                                       ///< GPP_CLK3
  GPP_CLK4,                                                       ///< GPP_CLK4
  GPP_CLK5,                                                       ///< GPP_CLK5
  GPP_CLK6,                                                       ///< GPP_CLK6
  GPP_CLK7,                                                       ///< GPP_CLK7
  GPP_CLK8,                                                       ///< GPP_CLK8
  GPP_CLK9,                                                       ///< GFX_CLK
} CPM_GPP_CLK;

/// Configuration values for SrcClk in ClkGen

typedef enum {
  SRC_CLK0 =                          0x00,                       ///< SRC0
  SRC_CLK1,                                                       ///< SRC1
  SRC_CLK2,                                                       ///< SRC2
  SRC_CLK3,                                                       ///< SRC3
  SRC_CLK4,                                                       ///< SRC4
  SRC_CLK5,                                                       ///< SRC5
  SRC_CLK6,                                                       ///< SRC6
  SRC_CLK7,                                                       ///< SRC7
  SRC_CLK8,                                                       ///< SRC8
  SRC_CLK9,                                                       ///< SRC9
} CPM_SRC_CLK;

/// Configuration values for ClkReq

typedef enum {
  CLK_DISABLE =                       0x00,                       ///< CLK_DISABLE
  CLK_REQ0,                                                       ///< CLK_REQ0
  CLK_REQ1,                                                       ///< CLK_REQ1
  CLK_REQ2,                                                       ///< CLK_REQ2
  CLK_REQ3,                                                       ///< CLK_REQ3
  CLK_REQ4,                                                       ///< CLK_REQ4
  CLK_REQ5,                                                       ///< CLK_REQ5
  CLK_REQ6,                                                       ///< CLK_REQ6
  CLK_REQ7,                                                       ///< CLK_REQ7
  CLK_REQ8,                                                       ///< CLK_REQ8
  CLK_REQGFX,                                                     ///< CLK_REQG
  CLK_ENABLE =                        0xFF,                       ///< CLK_ENABLE
} CPM_CLK_REQ;

/// Configuration values for Slot Check

typedef enum {
  NON_SLOT_CHECK =                    0x00,                       ///< Do not check the device on slot
  SLOT_CHECK,                                                     ///< Check PCI space of the device on slot
} CPM_SLOT_CHECK;


/// Structure for Variables to be used for CPM

typedef struct {
  UINT8                 SpecialVgaFeature;                        ///< Special Feature
                                                                  ///<  0: Disabled
                                                                  ///<  3: PowerXpress
  UINT8                 PrimaryVideoAdaptor;                      ///< Primary Video Adaptor
                                                                  ///<  0: Auto
                                                                  ///<  1: Int Graphics (IGD)
  UINT8                 UnusedGppClkOff;                          ///< Unused GPP Clocks Off
                                                                  ///<  0: Disabled
                                                                  ///<  1: Enabled
  UINT8                 ClockRequest;                             ///< Clock Request
                                                                  ///<  0: Disable
                                                                  ///<  1: Enable
  UINT8                 AcpiThermalFanEn;                         ///< ACPI Thermal Fan Control
                                                                  ///<  0: Disabled
                                                                  ///<  1: Enabled
  UINT8                 GfxLinkCfg;                               ///< GFX link configuration
                                                                  ///<  0: 1 x16 PCIe GFX slot
                                                                  ///<  1: 2 x8 PCIe GFX slots
  UINT8                 OnboardLanSwitch;                         ///< On Board Lan control
                                                                  ///<  0: Disabled
                                                                  ///<  1: Enabled
  UINT8                 MiniPcieSlotEnable;                       ///< Mini PCIe slot control
                                                                  ///<  0: Disabled
                                                                  ///<  1: Enabled
  UINT8                 WirelessLanRfEnable;                      ///< Wireless Lan RF control
                                                                  ///<  0: Disabled
                                                                  ///<  1: Enabled
  UINT8                 PcieDdiDetection;                         ///< DDI port detection
                                                                  ///<  0: Disabled
                                                                  ///<  1: Enabled
  UINT8                 AdaptiveS4En;                             ///< Adaptive S4 Control
                                                                  ///<  0: Disabled
                                                                  ///<  1: Enabled with EC Method
                                                                  ///<  2: Enabled with RTC Method
#ifdef CPM_SKIP_SSID_OVERRIDE_CONTROL
  UINT8                 SkipSsidOverride;                         ///< PowerXpress 5.6 support
                                                                  ///<  0: Disable
                                                                  ///<  1: Enable
#endif
} CPM_OEM_SETUP_OPTION;

EFI_STATUS
EFIAPI
CpmOemSetupOption (
  IN       EFI_PEI_SERVICES       **PeiServices,
  IN       CPM_OEM_SETUP_OPTION   *SetupOption
  );

//
// DDI card type
//
#define DDI_CARD_TYPE_DP                0x10
#define DDI_CARD_TYPE_DVI               0x20
#define DDI_CARD_TYPE_HDMI              0x30
#define DDI_CARD_TYPE_DL_DVI            0x40
#define DDI_CARD_TYPE_EDP               0x50
#define DDI_CARD_TYPE_LVDS              0x60
#define DDI_CARD_TYPE_EDP_LVDS          0x64    // 3rd party eDP-to-LVDS translator which require AMD SW init
#define DDI_CARD_TYPE_AUTO              0x70
#define DDI_CARD_TYPE_UNKNOWN           0xFF
#define INVALID_CONNECTOR_TYPE          0xFF

/// Registers' access type

typedef enum {
  SB_MMIO_ACCESS,                               ///< FCH MMIO register
  PCI_REG_ACCESS,                               ///< PCI register offset 0x00 ~ 0xFF
  EXT_PCI_REG_ACCESS,                           ///< PCI register offset over 0x100
} CPM_REGISTER_ACCESS_TYPE;

//
// Flags of AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_ITEM
//
#define ENABLE_OVERRIDE_ENABLE          1
#define DDI_TYPE_OVERRIDE_ENABLE        2       // 0 means PCIe port override
#define LANE_OVERRIDE_ENABLE            4
#define PORT_PRESENT_OVERRIDE_ENABLE    8
#define IS_DDI_DESCRIPTOR               0x10    // 0 means PCIe Port Descriptor
#define VALID_OVERRIDE                  0x80

/// The offset in PCIe Port Descriptor List

typedef enum {
  PCIE_PORT_GFX0_OFFSET = 0,                    ///< Offset of GFX0 port
  PCIE_PORT_GFX1_OFFSET,                        ///< Offset of GFX1 port
  PCIE_PORT_GPP0_OFFSET,                        ///< Offset of GPP0 port
  PCIE_PORT_GPP1_OFFSET,                        ///< Offset of GPP1 port
  PCIE_PORT_GPP2_OFFSET,                        ///< Offset of GPP2 port
  PCIE_PORT_GPP3_OFFSET,                        ///< Offset of GPP3 port
} CPM_PCIE_PORT_DESCRIPRTO_OFFSET;

/// The offset in PCIe DDI Descriptor List

typedef enum {
  PCIE_DDI_PORT0_OFFSET = 0,                    ///< DP0
  PCIE_DDI_PORT1_OFFSET,                        ///< DP1
  PCIE_DDI_PORT2_OFFSET,                        ///< DP2
  PCIE_DDI_PORT3_OFFSET,                        ///< DP3 for DL_DVI
  PCIE_GFX_DDI_PORT0_OFFSET,                    ///< DP4 (via GFX[15:12]
  PCIE_GFX_DDI_PORT1_OFFSET,                    ///< DP5 (via GFX[11:8])
  PCIE_GFX_DDI_PORT2_OFFSET,                    ///< DP6 (via GFX[7:4])
  PCIE_GFX_DDI_PORT3_OFFSET,                    ///< N/A (via GFX[3:0])
} CPM_PCIE_DDI_DESCRIPTOR_OFFSET;

/// Offset of Display Feature Config

typedef enum {
  DFC_PX_FIXED_MODE            = 0,             ///< PowerXpress Fixed Mode
  DFC_PX_DYNAMIC_MODE          = 1,             ///< PowerXpress Dynamic Mode
  DFC_HYPER_CROSS_FIRE         = 3,             ///< Hyper Cross Fire
  DFC_SURROUND_VIEW            = 4,             ///< Surround View
  DFC_DGPU_PRIMARY             = 8,             ///< Primary Display
  DFC_BRIGHNESS_BY_DRIVER      = 9,             ///< Brightness Controlled by Driver
  DFC_DISABLE_DGPU_AUDIO_IN_PX = 10,            ///< Disable dGPU Audio in PowerXpress
} CPM_DISPLAY_FEATURE_CONFIG_OFFSET;

// AMD_CPM_DISPLAY_FEATURE_SUPPORT
#define  DFS_POWER_EXPRESS              1
#define  DFS_HYPER_CROSS_FIRE           2
#define  DFS_SURROUND_VIEW              4
#define  DFS_REMOVABLE                  0x10000000
#define  DFS_VGA                        0x20000000
#define  DFS_EXIST                      0x40000000
#define  DFS_VALID                      0x80000000

#define  DFS_iGPU                       0
#define  DFS_dGPU                       1

//
// AMD_CPM_DISPLAY_FEATURE_TABLE
//
#define DISPLAY_SWITCH_REQUEST                  1
#define EXPANSION_MODE_CHANGE_REQUEST           2
#define THERMAL_STATE_CHANGE_REQUEST            4
#define FORCE_POWER_STATE_CHANGE_REQUEST        8
#define SYSTEM_POWER_SOURCE_CHANGE_REQUEST      0x10
#define DISPLAY_CONFIGUARTION_CHANGE_REQUEST    0x20
#define PX_GRAPHICS_SWITCH_TOGGLE_REQUEST       0x40
#define PANEL_BRIGHTNESS_CHANGE_REQUEST         0x80
#define DGPU_DISPLAY_CON_DISCON_EVENT           0x100

#define IGPU_AS_PRIMARY                 1
#define DGPU_AS_PRIMARY                 2

// Smbus address of Display Port via x16 GFX slot
#define GFX_DP0_I2C_ADDRESS             0x48
#define GFX_DP1_I2C_ADDRESS             0x4A
#define GFX_DP2_I2C_ADDRESS             0x4C
#define GFX_DP3_I2C_ADDRESS             0x4E

// PCIe card type
#define CARD_B962                       0
#define CARD_B964                       1
#define CARD_B966                       2
#define CARD_B967                       3
#define CARD_C527                       4
#define CARD_VGA                        7
#define CARD_UNKNOWN                    0xFF

// PCIE DDI card for CMOS PcieDdiDetection
#define PCIE_DDI_AUTO                   1
#define PCIE_DDI_4X4_DP                 2
#define PCIE_DDI_4X4_HDMI               3
#define PCIE_DDI_4X4_DVI                4
#define PCIE_DDI_2X8_DL_DVI             5

#define GFX_X16                         0
#define GFX_2X8                         1

#define FEATURE_DISABLE                 0
#define FEATURE_ENABLE                  1
#define SWITCH_OFF                      0
#define SWITCH_ON                       1

#define GPIO_VALUE(function, output, pullup, sticky) \
  (function ? 0x800 : 0) + (function << 8) + (output < 3 ? (output < 2 ? output : 0x02) : 0x08) + \
  ((sticky == 1) || (pullup < 3) ? ((sticky == 1 ? 0x10 : 0) + (pullup == 0 ? 0x40 : 0) + \
  (pullup == 1 ? 0x20 : 0) + 0x80) : 0)

// Bantry uses SMBUS 1
#define GFX_PCIE_SMBUS_NO               1    // GFX PCIE slot SMBUS numver
#endif
