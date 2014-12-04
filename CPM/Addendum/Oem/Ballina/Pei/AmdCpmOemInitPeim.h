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

#ifndef AMD_CPM_SETUP_OPTION_POWER_XPRESS_DYNAMIC_MODE
#define AMD_CPM_SETUP_OPTION_POWER_XPRESS_DYNAMIC_MODE  PowerExpressDynamicMode
#endif

#ifndef AMD_CPM_SETUP_OPTION_PRIMARY_VIDEO_ADAPTOR
#define AMD_CPM_SETUP_OPTION_PRIMARY_VIDEO_ADAPTOR      PrimaryVideoAdaptor
#endif

#ifndef AMD_CPM_SETUP_OPTION_DISPLAY_OUTPUT
#define AMD_CPM_SETUP_OPTION_DISPLAY_OUTPUT             DisplayOutput
#endif

#ifndef AMD_CPM_SETUP_OPTION_BRIGHTNESS_CONTROL_METHOD
#define AMD_CPM_SETUP_OPTION_BRIGHTNESS_CONTROL_METHOD  BrightnessControlMethod
#endif

#ifndef AMD_CPM_SETUP_OPTION_BLUE_TOOTH_EN
#define AMD_CPM_SETUP_OPTION_BLUE_TOOTH_EN              BlueToothEn
#endif

#ifndef AMD_CPM_SETUP_OPTION_ZERO_POWER_ODD_EN
#define AMD_CPM_SETUP_OPTION_ZERO_POWER_ODD_EN          ZeroPowerOddEn
#endif

#ifndef AMD_CPM_SETUP_OPTION_UNUSED_GPP_CLOCK_OFF
#define AMD_CPM_SETUP_OPTION_UNUSED_GPP_CLOCK_OFF       UnusedGppClkOff
#endif

#ifndef AMD_CPM_SETUP_OPTION_SYSTEM_BOOT_WITH_PS0
#define AMD_CPM_SETUP_OPTION_SYSTEM_BOOT_WITH_PS0       amdSystemBootWithPS0
#endif

#ifndef AMD_CPM_SETUP_OPTION_ACPI_THERMAL_FAN_EN
#define AMD_CPM_SETUP_OPTION_ACPI_THERMAL_FAN_EN        ThermalFanControl
#endif

#ifndef AMD_CPM_SETUP_OPTION_ADAPTIVE_S4_EN
#define AMD_CPM_SETUP_OPTION_ADAPTIVE_S4_EN             AdaptiveS4En
#endif

#ifndef AMD_CPM_SETUP_OPTION_SPREAD_SPECTRUM_SWITCH
#define AMD_CPM_SETUP_OPTION_SPREAD_SPECTRUM_SWITCH     SpreadSpectrumSwitch
#endif

#ifndef AMD_CPM_SETUP_OPTION_WIRELESS_SWITCH
#define AMD_CPM_SETUP_OPTION_WIRELESS_SWITCH            WireLessSwitch
#endif

#ifndef AMD_CPM_SETUP_OPTION_POWERXPRESS_5_6
#define AMD_CPM_SETUP_OPTION_POWERXPRESS_5_6            PowerExpressFiveSix
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
#define SCITRIG_AUTO                3
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

/// Physical or virtual on-board device Id.

typedef enum {
  DEVICE_ID_ODD =                     0x01,                       ///< Device Id for ODD
  DEVICE_ID_DMC =                     0x02,                       ///< Device Id for DMC
  DEVICE_ID_MPCIE1 =                  0x03,                       ///< Device Id for Mini PCIE 1
  DEVICE_ID_MPCIE2 =                  0x04,                       ///< Device Id for Mini PCIE 2
  DEVICE_ID_MXM =                     0x05,                       ///< Device Id for MXM
  DEVICE_ID_DT =                      0x06,                       ///< Device Id for DT MPCIE
  DEVICE_ID_LAN =                     0x07,                       ///< Device Id for On-board LAN
  DEVICE_ID_DDI3 =                    0x08,                       ///< Device Id for DDI3
  DEVICE_ID_EXPRESSCARD =             0x09,                       ///< Device Id for Express Card
  DEVICE_ID_HDD0 =                    0x0A,                       ///< Device Id for HDD0
  DEVICE_ID_HDD2 =                    0x0B,                       ///< Device Id for HDD2
  DEVICE_ID_BT =                      0x0C,                       ///< Device Id for BlueTooth
  DEVICE_ID_WIRELESS =                0x0D,                       ///< Device Id for WLAN, WWAN, Mini PCIE on DMC slot
  DEVICE_ID_RADIO =                   0x0E,                       ///< Device Id for Radio of WLAN, WWAN, Mini PCIE on DMC slot
  DEVICE_ID_SWINGMODE =               0x10,                       ///< Device Id for virtual device to switch PEX_STD_SW#
  DEVICE_ID_POWERLEVEL =              0x11,                       ///< Device Id for virtual device to switch FCH_PWR_LV
  DEVICE_ID_VGAMUXSEL =               0x12,                       ///< Device Id for virtual device to switch VGA_MUX_SEL
  DEVICE_ID_APU_SB_UMI =              0x13,                       ///< Device Id for SB UMI
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
  SRC_SKIP =                          0xFE,                       ///< SKIP
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
  UINT8                 PowerExpressDynamicMode;                  ///< PX Dynamic Mode
                                                                  ///<  0: Disabled
                                                                  ///<  1: dGPU Power Down
                                                                  ///<  2: PX ULPS mode
  UINT8                 PrimaryVideoAdaptor;                      ///< Primary Video Adaptor
                                                                  ///<  0: Auto
                                                                  ///<  1: Int Graphics (IGD)
  UINT8                 DisplayOutput;                            ///< Display Output
                                                                  ///<  0: FCH
                                                                  ///<  1: MXM
  UINT8                 BrightnessControlMethod;                  ///< Brightness Control Method
                                                                  ///<  0: Video BIOS
                                                                  ///<  1: VGA driver
  UINT8                 BlueToothEn;                              ///< BlurTooth Device
                                                                  ///<  0: Auto
                                                                  ///<  1: Disabled
  UINT8                 ZeroPowerOddEn;                           ///< AMD ODD Zero Power
                                                                  ///<  0: Disabled
                                                                  ///<  1: Enabled
  UINT8                 SystemBootWithPS0;                        ///< System Boot with PS0
                                                                  ///<  0: Yes
                                                                  ///<  1: No
  UINT8                 UnusedGppClkOff;                          ///< Unused GPP Clocks Off
                                                                  ///<  0: Disabled
                                                                  ///<  1: Enabled
  UINT8                 ClockRequest;                             ///< Clock Request
                                                                  ///<  0: Disable
                                                                  ///<  1: Enable
  UINT8                 AcpiThermalFanEn;                         ///< ACPI Thermal Fan Control
                                                                  ///<  0: Disabled
                                                                  ///<  1: Enabled
  UINT8                 AdaptiveS4En;                             ///< Adaptive S4 Control
                                                                  ///<  0: Disabled
                                                                  ///<  1: Enabled with EC Method
                                                                  ///<  2: Enabled with RTC Method
  UINT8                 SpreadSpectrumSwitch;                     ///< External Spread Spectrum Control
                                                                  ///<  0: Disabled
                                                                  ///<  1: Enabled
  UINT8                 WirelessSwitch;                           ///< Wireless Switch
                                                                  ///<  0: Radio Off
                                                                  ///<  1: Power down
  UINT8                 PX56Support;                              ///< PowerXpress 5.6 support
                                                                  ///<  0: Disable
                                                                  ///<  1: Enable
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

#endif
