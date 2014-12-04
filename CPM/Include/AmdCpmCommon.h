/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM structures and definitions
 *
 * Contains AMD CPM Common Interface
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  Include
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

#ifndef _AMD_CPM_COMMON_H_
#define _AMD_CPM_COMMON_H_

#pragma pack(push)

#include <AGESA.h>
#include <AmdCpmPlatform.h>
#include <AmdCpmDefine.h>
#include <AmdCpmFunction.h>
#include <AmdCpmBootTimeRecordDescription.h>

// #define AMD_CPM_KERNEL_VERSION                 0xnnnm
// CPM Kernel Main Version  = nnn
// CPM Kernel Sub Version   = m
#define AMD_CPM_KERNEL_VERSION                    0x0302

#ifndef AMD_MAX_CPU_CORE_NUMBERS
  #define AMD_MAX_CPU_CORE_NUMBERS                0x8
#endif

#ifndef AMD_CHECK_TOOL_SIG
  #define AMD_CHECK_TOOL_SIG                      0x4B435324ul
#endif


#ifndef AMD_DISPLAY_DEVICE_PATH_SIZE
  #define AMD_DISPLAY_DEVICE_PATH_SIZE            0x08
#endif

#ifndef AMD_SPECIFIC_SSID_DEVICE_SIZE
  #define AMD_SPECIFIC_SSID_DEVICE_SIZE           0x100
#endif

#ifndef AMD_REBAND_DUAL_GRAPHICS_SSID_DEVICE_SIZE
  #define AMD_REBAND_DUAL_GRAPHICS_SSID_DEVICE_SIZE 0x20
#endif

#define AMD_VGA_DOD_BUFFER_SIZE_MAXIMUM           0x20
#ifndef AMD_VGA_DOD_BUFFER_SIZE
  #define AMD_VGA_DOD_BUFFER_SIZE                 AMD_VGA_DOD_BUFFER_SIZE_MAXIMUM
#else
  #if AMD_VGA_DOD_BUFFER_SIZE > AMD_VGA_DOD_BUFFER_SIZE_MAXIMUM
    #undef  AMD_VGA_DOD_BUFFER_SIZE
    #define AMD_VGA_DOD_BUFFER_SIZE               AMD_VGA_DOD_BUFFER_SIZE_MAXIMUM
  #endif
#endif

#define AMD_LCD_BRIGHTNESS_LEVEL_BUFFER_SIZE_MAXIMUM    100
#ifndef AMD_LCD_BRIGHTNESS_LEVEL_BUFFER_SIZE
  #define AMD_LCD_BRIGHTNESS_LEVEL_BUFFER_SIZE    AMD_LCD_BRIGHTNESS_LEVEL_BUFFER_SIZE_MAXIMUM
#else
  #if AMD_LCD_BRIGHTNESS_LEVEL_BUFFER_SIZE > AMD_LCD_BRIGHTNESS_LEVEL_BUFFER_SIZE_MAXIMUM
    #undef  AMD_LCD_BRIGHTNESS_LEVEL_BUFFER_SIZE
    #define AMD_LCD_BRIGHTNESS_LEVEL_BUFFER_SIZE  AMD_LCD_BRIGHTNESS_LEVEL_BUFFER_SIZE_MAXIMUM
  #endif
#endif

#define AMD_ATPX_CONNECTOR_8_SIZE_MAXIMUM         20
#define AMD_ATPX_CONNECTOR_8_SIZE                 AMD_ATPX_CONNECTOR_8_SIZE_MAXIMUM

#define AMD_ATPX_CONNECTOR_9_SIZE_MAXIMUM         20
#define AMD_ATPX_CONNECTOR_9_SIZE                 AMD_ATPX_CONNECTOR_9_SIZE_MAXIMUM

#define AMD_ATIF_DEVICE_COMBINATION_BUFFER_SIZE_MAXIMUM   20
#define AMD_ATIF_DEVICE_COMBINATION_BUFFER_SIZE   AMD_ATIF_DEVICE_COMBINATION_BUFFER_SIZE_MAXIMUM


/// The signatures of CPM table

typedef enum {
  CPM_SIGNATURE_MAIN_TABLE =                  'MPC$',         ///< The signature of AMD_CPM_MAIN_TABLE

  CPM_SIGNATURE_GET_PLATFORM_ID =             '00A$',         ///< The signature of AMD_CPM_PLATFORM_ID_TABLE
  CPM_SIGNATURE_GET_PLATFORM_ID_CONVERT =     '10A$',         ///< The signature of AMD_CPM_PLATFORM_ID_CONVERT_TABLE
  CPM_SIGNATURE_PRE_INIT =                    '20A$',         ///< The signature of AMD_CPM_PRE_INIT_TABLE
  CPM_SIGNATURE_SAVE_CONTEXT =                '30A$',         ///< The signature of AMD_CPM_SAVE_CONTEXT_TABLE
  CPM_SIGNATURE_INIT_FLAG =                   '40A$',         ///< The signature of AMD_CPM_INIT_FLAG_TABLE

  CPM_SIGNATURE_GPIO_INIT =                   '01A$',         ///< The signature of AMD_CPM_GPIO_INIT_TABLE
  CPM_SIGNATURE_GEVENT_INIT =                 '11A$',         ///< The signature of AMD_CPM_GEVENT_INIT_TABLE
  CPM_SIGNATURE_GPIO_DEVICE_CONFIG =          '21A$',         ///< The signature of AMD_CPM_GPIO_DEVICE_CONFIG_TABLE
  CPM_SIGNATURE_GPIO_DEVICE_POWER =           '31A$',         ///< The signature of AMD_CPM_GPIO_DEVICE_POWER_TABLE
  CPM_SIGNATURE_GPIO_DEVICE_DETECTION =       '41A$',         ///< The signature of AMD_CPM_GPIO_DEVICE_DETECTION_TABLE
  CPM_SIGNATURE_GPIO_DEVICE_RESET =           '51A$',         ///< The signature of AMD_CPM_GPIO_DEVICE_RESET_TABLE
  CPM_SIGNATURE_SET_MEM_VOLTAGE =             '61A$',         ///< The signature of AMD_CPM_GPIO_MEM_VOLTAGE_TABLE
  CPM_SIGNATURE_PCIE_CLOCK =                  '71A$',         ///< The signature of AMD_CPM_PCIE_CLOCK_TABLE
  CPM_SIGNATURE_EXT_CLKGEN =                  '81A$',         ///< The signature of AMD_CPM_EXT_CLKGEN_TABLE
  CPM_SIGNATURE_GPIO_PRE_INIT =               '91A$',         ///< The signature of AMD_CPM_GPIO_PRE_INIT_TABLE

  CPM_SIGNATURE_PCIE_TOPOLOGY =               '02A$',         ///< The signature of AMD_CPM_PCIE_TOPOLOGY_TABLE
  CPM_SIGNATURE_PCIE_TOPOLOGY_OVERRIDE =      '12A$',         ///< The signature of AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_TABLE
  CPM_SIGNATURE_PCIE_EXPRESS_CARD =           '22A$',         ///< The signature of AMD_CPM_EXPRESS_CARD_TABLE
  CPM_SIGNATURE_WIRELESS_BUTTON =             '32A$',         ///< The signature of AMD_CPM_WIRELESS_BUTTON_TABLE
  CPM_SIGNATURE_SET_VDDP_VDDR_VOLTAGE =       '42A$',         ///< The signature of AMD_CPM_GPIO_VDDP_VDDR_VOLTAGE_TABLE

  CPM_SIGNATURE_EC_INIT =                     '03A$',         ///< The signature of AMD_CPM_EC_INIT_TABLE

  CPM_SIGNATURE_ACPI_THERMAL_FAN =            '04A$',         ///< The signature of AMD_CPM_ACPI_THERMAL_FAN_TABLE

  CPM_SIGNATURE_ZERO_POWER_ODD =              '05A$',         ///< The signature of AMD_CPM_ZERO_POWER_ODD_TABLE

  CPM_SIGNATURE_LPC_UART =                    '06A$',         ///< The signature of AMD_CPM_LPC_UART_TABLE
  CPM_SIGNATURE_PROCHOT =                     '07A$',         ///< The signature of AMD_CPM_PROCHOT_TABLE

  CPM_SIGNATURE_DISPLAY_FEATURE =             '08A$',         ///< The signature of AMD_CPM_DISPLAY_FEATURE_TABLE
  CPM_SIGNATURE_DEVICE_PATH =                 '18A$',         ///< The signature of AMD_CPM_DEVICE_PATH_TABLE
  CPM_SIGNATURE_SPECIFIC_SSID =               '28A$',         ///< The signature of AMD_CPM_SPECIFIC_SSID_TABLE
  CPM_SIGNATURE_REBRAND_DUAL_GRAPHICS_SSID =  '38A$',         ///< The signature of AMD_CPM_REBRAND_DUAL_GRAPHICS_SSID_TABLE

  CPM_SIGNATURE_TDP_LIMIT_CHANGE =            '09A$',         ///< The signature of AMD_CPM_TDP_LIMIT_CHANGE_TABLE
  CPM_SIGNATURE_SMI_CHECK_TOOL =              '0AA$',         ///< The signature of AMD_CPM_SMI_CHECK_TOOL_TABLE

  CPM_SIGNATURE_ADAPTIVE_S4 =                 '0BA$',         ///< The signature of AMD_CPM_ADAPTIVE_S4_TABLE

} AMD_CPM_TABLE_SIGNATURE;


/// The value of special clock id

typedef enum {
  CPM_CLKID_APU_CLK =                         0x80,           ///< APU_CLK Power Down Enable
  CPM_CLKID_DISP2_CLK =                       0x81,           ///< DISP2_CLK Power Down Enable
  CPM_CLKID_PCIE_RCLK_OUTPUT =                0x82,           ///< PCIE_RCLK_Output Power Down Enable
  CPM_CLKID_DISP_CLK =                        0x83,           ///< DISP_CLK Power Down Enable
  CPM_CLKID_PCIE_RCLK =                       0x84,           ///< PCIE_RCLK Power Down Enable
  CPM_CLKID_CLOCK_BUFFER_BIAS =               0x85,           ///< Clock Buffer Bias Power Down Enable
  CPM_CLKID_OSCOUT2_OUTOFF =                  0x86,           ///< OSCOUT2 Power Down Enable
} AMD_CPM_CLOCK_ID;


/// The value of Boot Mode

typedef enum {
  CPM_BOOT_MODE_S0 =                          0x00,           ///< BOOT ON S0
  CPM_BOOT_MODE_S1 =                          0x01,           ///< BOOT ON S1 RESUME
  CPM_BOOT_MODE_S3 =                          0x03,           ///< BOOT ON S3 RESUME
  CPM_BOOT_MODE_S4 =                          0x04,           ///< BOOT ON S4 RESUME
  CPM_BOOT_MODE_S5 =                          0x05,           ///< BOOT ON S5 RESUME
} AMD_CPM_BOOT_MODE;

/// The value of CPU Revision ID

typedef enum {
  CPM_CPU_REVISION_ID_TN =                    0x00,           ///< CPU Revision ID for TN
  CPM_CPU_REVISION_ID_ON,                                     ///< CPU Revision ID for ON
  CPM_CPU_REVISION_ID_KV,                                     ///< CPU Revision ID for KV
  CPM_CPU_REVISION_ID_KB,                                     ///< CPU Revision ID for KB
  CPM_CPU_REVISION_ID_ML,                                     ///< CPU Revision ID for ML
  CPM_CPU_REVISION_ID_CZ,                                     ///< CPU Revision ID for CZ
} AMD_CPM_CPU_REVISION_ID;

/// The value of PCIE Revision ID

typedef enum {
  CPM_PCIE_REVISION_ID_TN =                   0x00,           ///< PCIE Revision ID for TN
  CPM_PCIE_REVISION_ID_ON,                                    ///< PCIE Revision ID for ON
  CPM_PCIE_REVISION_ID_KV,                                    ///< PCIE Revision ID for KV
  CPM_PCIE_REVISION_ID_KB,                                    ///< PCIE Revision ID for KB
  CPM_PCIE_REVISION_ID_ML,                                    ///< PCIE Revision ID for ML
  CPM_PCIE_REVISION_ID_CZ,                                    ///< PCIE Revision ID for CZ
} AMD_CPM_PCIE_REVISION_ID;

/// The value of CPU Revision ID

typedef enum {
  CPM_FCH_REVISION_ID_DEFAULT =               0x00,           ///< FCH Revision ID for Default
  CPM_FCH_REVISION_ID_KB =                    0x01,           ///< FCH Revision ID for KB
  CPM_FCH_REVISION_ID_ML =                    0x02,           ///< FCH Revision ID for ML
  CPM_FCH_REVISION_ID_CZ =                    0x03,           ///< FCH Revision ID for CZ
} AMD_CPM_FCH_REVISION_ID;

/// The value of the stage to load AMD CPM Pre Init Table

typedef enum {
  CPM_PRE_INIT_STAGE_0 =                      0,              ///< Stage 0 to load Pre Init Table
  CPM_PRE_INIT_STAGE_1 =                      1,              ///< Stage 1 to load Pre Init Table
} AMD_CPM_PRE_INIT_STAGE;

/// Configuration values for CPM table attribute

typedef enum {
  CPM_PEI =                                   BIT0,           ///< Used by CPM PEI driver
  CPM_DXE =                                   BIT1,           ///< Used by CPM DXE driver
  CPM_SMM =                                   BIT2,           ///< Used by CPM SMM driver
  CPM_OVERRIDE =                              BIT3,           ///< Modified by CPM driver
} AMD_CPM_TABLE_ATTRIBUTE;


/// CPM table header

typedef struct {
  UINT32                          TableSignature;             ///< Signature of CPM table
  UINT16                          TableSize;                  ///< Table size
  UINT8                           FormatRevision;             ///< Revision of table format
  UINT8                           ContentRevision;            ///< Revision of table contect
  UINT32                          PlatformMask;               ///< The mask of platform table supports
  UINT32                          Attribute;                  ///< Table attribute
} AMD_CPM_TABLE_COMMON_HEADER;


/// Table pointer

typedef union {
  VOID                            *Pointer;                   ///< Table pointer
  UINT64                          Raw;                        ///< Table pointer value
} AMD_CPM_POINTER;


/// Device and function number of PCI device

typedef struct {
  UINT8                           Device:5;                 ///< PCI Device Number
  UINT8                           Function:3;               ///< PCI Function Number
} AMD_CPM_PCI_DEVICE_FUNCTION;


/// Item of CPM table list

typedef struct {
  UINT32                          TableId;                    ///< Signature of table
  UINT8                           Flag;                       ///< Location of table. 0: ROM. 1: RAM
  AMD_CPM_POINTER                 SubTable;                   ///< Pointer of table
} AMD_CPM_TABLE_ITEM;


/// CPM table list

typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                         ///< Table header
  UINT32                          Number;                         ///< Table number to be recorded in the list
  UINT32                          Size;                           ///< Table size
  AMD_CPM_TABLE_ITEM              Item[AMD_TABLE_LIST_ITEM_SIZE]; ///< The array of CPM table
} AMD_CPM_TABLE_LIST;


/// GPIO pin list for platform Id

typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                               ///< Table header
  UINT16                          GpioPin[AMD_PLATFORM_ID_TABLE_SIZE];  ///< The array of GPIO pin for platform Id
} AMD_CPM_PLATFORM_ID_TABLE;


/// Platform Id converting item

typedef struct {
  UINT8                           CpuRevisionId;              ///< CPU Revision ID
  UINT16                          OriginalIdMask;             ///< Platform Id mask from platform Id table
  UINT16                          OriginalId;                 ///< Platform Id from platform Id table
  UINT16                          ConvertedId;                ///< Platform Id which is used to check whether
                                                              ///<   CPM table supports current platform
} AMD_CPM_PLATFORM_ID_CONVERT_ITEM;


/// Platform Id mapping table

typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER         Header;                                   ///< Table Header
  AMD_CPM_PLATFORM_ID_CONVERT_ITEM    Item[AMD_PLATFORM_ID_CONVERT_TABLE_SIZE]; ///< Platform Id mapping list
} AMD_CPM_PLATFORM_ID_CONVERT_TABLE;


/// The table definition for early initialization

typedef struct {
  UINT8                           Type;                       ///< Register type. 0: FCH MMIO. 1: PCI
  UINT8                           Select;                     ///< Register sub-type
  UINT8                           Offset;                     ///< Register offset
  UINT8                           AndMask;                    ///< AND mask
  UINT8                           OrMask;                     ///< OR mask
  UINT8                           Stage;                      ///< Stage number
} AMD_CPM_PRE_SETTING_ITEM;


/// Register table to be initialized in the earliest stage
typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                     ///< Table header
  AMD_CPM_PRE_SETTING_ITEM        Item[AMD_PRE_INIT_SIZE];    ///< Register setting
} AMD_CPM_PRE_INIT_TABLE;

/// Save context definition table to define the area to save CPM context
typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                     ///< Table header
  UINT8                           BufferType;                 ///< Buffer Type. 5: BIOS RAM. Other: Reserved.
  UINT8                           BufferOffset;               ///< Buffer Offset
  UINT8                           BufferSize;                 ///< Buffer Size
} AMD_CPM_SAVE_CONTEXT_TABLE;


/// Data structure of Pre-defined Save Context Area
typedef struct {
  UINT32                          PcieDeviceStatus;           ///< The status of PCIe device on APU
  UINT32                          PcieClockSlotStatus;        ///< The status of PCIe device slot
  UINT8                           WirelessButtonStatus;       ///< The status of wireless button
  UINT8                           BootMode;                   ///< Current Boot Mode
} AMD_CPM_PREDEFINED_SAVE_CONTEXT;

//
// The table definition for GPIO
//
//  AMD_CPM_GPIO_INIT_TABLE
//  AMD_CPM_GEVENT_INIT_TABLE
//  AMD_CPM_GPIO_DEVICE_CONFIG_TABLE
//  AMD_CPM_GPIO_DEVICE_POWER_TABLE
//  AMD_CPM_GPIO_DEVICE_DETECTION_TABLE
//  AMD_CPM_GPIO_DEVICE_RESET_TABLE
//


/// GPIO Pin

typedef struct {
  UINT16                          Pin;                        ///< GPIO pin number
  UINT8                           Value;                      ///< GPIO pin value
} AMD_CPM_GPIO_PIN;


/// Defintion of GPIO setting

typedef union {
  UINT16                          Raw;                        ///< GPIO setting value
  struct {                                                    ///<
    UINT16                        Out:1;                      ///< Output state
    UINT16                        OutEnB:1;                   ///< Output enable
    UINT16                        PullUpSel:1;                ///< Pull up select: 0: 4K. 1: 8K
    UINT16                        SetEnB:1;                   ///< Gate of Out and OutEnB
    UINT16                        Sticky:1;                   ///< Sticky enable
    UINT16                        PullUp:1;                   ///< Pull up enable
    UINT16                        PullDown:1;                 ///< Pull down enable
    UINT16                        PresetEn:1;                 ///< Gate of Sticky, PullUp & PullDown
    UINT16                        IoMux:3;                    ///< Multi-function IO pin function select of GPIO
    UINT16                        IoMuxEn:1;                  ///< Gate of IoMux
    UINT16                        DrvStrengthSel:2;           ///< Drive Strength Select: 0: 4mA. 1: 8mA. 2: 12mA. 3: 16mA
    UINT16                        Reserved2:2;                ///< Reserved
  }                               Gpio;                       ///< Bit mapping for GPIO setting
} AMD_CPM_GPIO_SETTING;


/// GPIO setting item
typedef struct {
  UINT16                          Pin;                        ///< GPIO pin number
  AMD_CPM_GPIO_SETTING            Setting;                    ///< GPIO setting
} AMD_CPM_GPIO_ITEM;


/// GPIO init table

typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                       ///< Table header
  AMD_CPM_GPIO_ITEM               GpioList[AMD_GPIO_ITEM_SIZE]; ///< GPIO setting list
} AMD_CPM_GPIO_INIT_TABLE;


/// The definition of GEVENT setting

typedef union {
  UINT16                          Raw;                        ///< GEVENT setting value
  struct {                                                    ///<
    UINT16                        EventEnable:1;              ///< EventEnable. 0: Disable, 1: Enable
    UINT16                        SciTrig:1;                  ///< SciTrig. 0: Falling Edge, 1: Rising Edge
    UINT16                        SciLevl:1;                  ///< SciLevl.0 trigger mode. 0: Edge trigger, 1: Level Trigger
    UINT16                        SmiSciEn:1;                 ///< SmiSciEn. 0: Not send SMI, 1: Send SMI
    UINT16                        SciS0En:1;                  ///< SciS0En. 0: Disable, 1: Enable
    UINT16                        SciMap:5;                   ///< SciMap. 0000b->1111b
    UINT16                        SciTrigAuto:1;              ///< SciTrigAuto. 1: Disable. 0: Enable.
    UINT16                        SmiTrig:1;                  ///< SmiTrig. 0: Active Low, 1: Active High
    UINT16                        SmiControl:4;               ///< SmiControl. 0: Disable, 1: SMI 2: NMI 3: IRQ13
  }                               Gevent;                     ///< Bit mapping for GEVENT setting
  struct {                                                    ///<
    UINT16                        DebounceTmrOut:4;           ///< Specifies the debounce timer out number
    UINT16                        DebounceTmrOutUnit:1;       ///< 0: 30.5us (One RtcClk period), 1: 122us (four RtcClk periods)
    UINT16                        DebounceCntrl:2;            ///< 00b: No debounce, 01b: Preserve low glitch
                                                              ///< 10b: Preserve high glitch, 11b: Remove glitch
    UINT16                        Reserved:1;                 ///< Reserved
    UINT16                        LevelTrig:1;                ///< 0: Edge trigger, 1: Level trigger
    UINT16                        ActiveLevel:2;              ///< 00b: Active High. 01b: Active Low. 10b: Active on both edges if LevelTrig=0
    UINT16                        InterruptEnable:2;          ///< [0]: Enable interrupt status, [1]: Enable interrupt delivery
    UINT16                        WakeCntrl:3;                ///< [0]: Enable wake in S0I3 state, [1]: Enable wake in S3 state, [2]: Enable wake in S4/S5 state
  }                               Gpio;                       ///< Bit mapping for GPIO interrupt setting
} AMD_CPM_GEVENT_SETTING;


/// GEVENT setting item

typedef struct {
  UINT16                          Pin;                        ///< GEVENT pin number
  AMD_CPM_GEVENT_SETTING          Setting;                    ///< GEVENT setting
} AMD_CPM_GEVENT_ITEM;


/// GEVENT init table

typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                           ///< Table header
  AMD_CPM_GEVENT_ITEM             GeventList[AMD_GEVENT_ITEM_SIZE]; ///< GEVENT setting list
} AMD_CPM_GEVENT_INIT_TABLE;


/// Configuration of Device which is controlled by GPIO pin

typedef struct {
  UINT8                           DeviceId;           ///< Device Id
  union {
    UINT8                         Raw;                ///< Device Config Value
    struct {
      UINT8                       Enable:2;           ///< Flag of power state. 0: Disable. 1: Enable. 2: Auto Detection
      UINT8                       ResetAssert:1;      ///< Flag to assert reset pin
      UINT8                       ResetDeassert:1;    ///< Flag to de-assert reset pin
      UINT8                       Reserved:4;         ///< Reserved
    }                             Setting;            ///< Bit mapping of Device Config
  }                               Config;             ///< Device Config
} AMD_CPM_GPIO_DEVICE_CONFIG;


/// Device config table

typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                           ///< Table header
  AMD_CPM_GPIO_DEVICE_CONFIG      DeviceList[AMD_GPIO_DEVICE_SIZE]; ///< Device config list
} AMD_CPM_GPIO_DEVICE_CONFIG_TABLE;


/// Device detection method

typedef struct {
  UINT8                           DeviceId;           ///< Device Id
  UINT8                           Type;               ///< Detection type. 0: One GPIO pin. 1: Two GPIO pins. 2: Special Pin
  UINT16                          PinNum1;            ///< Pin number of GPIO 1
  UINT8                           Value1;             ///< Value of GPIO 1
  UINT16                          PinNum2;            ///< Pin number of GPIO 2
  UINT8                           Value2;             ///< Value of GPIO 2
  UINT16                          PinNum3;            ///< Pin number of GPIO 3
  UINT8                           Value3;             ///< Value of GPIO 3
} AMD_CPM_GPIO_DEVICE_DETECTION;


/// Device Detection Table

typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                                           ///< Table header
  AMD_CPM_GPIO_DEVICE_DETECTION   DeviceDetectionList[AMD_GPIO_DEVICE_DETECT_SIZE]; ///< Device Detection List
} AMD_CPM_GPIO_DEVICE_DETECTION_TABLE;


/// Device Reset Table

typedef struct {
  UINT8                           DeviceId;           ///< Device Id
  UINT8                           Mode;               ///< Reset mode
                                                      ///<   0: Reset Assert.
                                                      ///<   1: Reset De-assert
                                                      ///<   2: Delay between Assert and Deassert
  UINT8                           Type;               ///< Register type
                                                      ///<   0: GPIO.
                                                      ///<   1: Special pin.if Mode = 0 or 1
  union {
    UINT32                        Stall;              ///< Delay
    AMD_CPM_GPIO_PIN              Gpio;               ///< GPIO pin
  }                               Config;             ///< Setting
  UINT8                           InitFlag;           ///< Init flag in post
} AMD_CPM_GPIO_DEVICE_RESET;


/// Configuration values for CPM GPIO Device Init Flag

typedef enum {
  GPIO_DEVICE_INIT_DISABLE =      0,                  ///< GPIO Device does not need to be initialized or is controlled by GPIO Device COnfig
  GPIO_DEVICE_INIT_STAGE_1 =      1,                  ///< GPIO Device needs to be initialized on stage 1
  GPIO_DEVICE_INIT_STAGE_2 =      2,                  ///< GPIO Device needs to be initialized on stage 2
  GPIO_DEVICE_INIT_STAGE_DUMMY =  3,                  ///< GPIO Device does not need to be initialized in BIOS post
} AMD_CPM_GPIO_DEVICE_INIT_FLAG;


/// Device Reset Table

typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                                       ///< Table header
  AMD_CPM_GPIO_DEVICE_RESET       DeviceResetList[AMD_GPIO_DEVICE_RESET_SIZE];  ///< Device reset list
} AMD_CPM_GPIO_DEVICE_RESET_TABLE;

/// Item of device power on / off sequence

typedef struct {
  UINT8                           DeviceId;           ///< Device Id
  UINT8                           Mode;               ///< Device Power Mode. 1: Power On. 0: Power Off
  UINT8                           Type;               ///< Device Power Item. 0: Set GPIO. 1: Wait GPIO. 2: Add Delay
  union {
    UINT32                        Stall;              ///< Delay
    AMD_CPM_GPIO_PIN              SetGpio;            ///< Set GPIO pin
    AMD_CPM_GPIO_PIN              WaitGpio;           ///< Wait for GPIO pin to some value
  }                               Config;             ///< Dvice Power Item Setting
  UINT8                           InitFlag;           ///< Init flag in post
} AMD_CPM_GPIO_DEVICE_POWER;

/// GPIO Device Init Flag Table

typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                                       ///< Table header
  UINT8                           PowerInitFlag[AMD_GPIO_DEVICE_POWER_SIZE];    ///< Init Flag for Power Sequence
  UINT8                           ResetInitFlag[AMD_GPIO_DEVICE_RESET_SIZE];    ///< Init Flag for Reset Sequence
} AMD_CPM_INIT_FLAG_TABLE;

/// Device Power Sequence Table

typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                                       ///< Table header
  AMD_CPM_GPIO_DEVICE_POWER       DevicePowerList[AMD_GPIO_DEVICE_POWER_SIZE];  ///< Device power sequence list
} AMD_CPM_GPIO_DEVICE_POWER_TABLE;


/// GPIO setting for one memory voltage (VDDIO)

typedef struct {
  UINT8                           Voltage;            ///< DDR3Voltage
  UINT16                          GpioPin1;           ///< GPIO pin 1
  UINT8                           Value1;             ///< Value of GPIO pin 1
  UINT16                          GpioPin2;           ///< GPIO pin 2
  UINT8                           Value2;             ///< Value of GPIO pin 2
} AMD_CPM_GPIO_MEM_VOLTAGE_ITEM;


/// Memory voltage table

typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                           ///< Table header
  AMD_CPM_GPIO_MEM_VOLTAGE_ITEM   Item[AMD_GPIO_MEM_VOLTAGE_SIZE];  ///< GPIO setting list for memory voltage
} AMD_CPM_GPIO_MEM_VOLTAGE_TABLE;


/// GPIO setting for VDDP/VDDR voltage

typedef struct {
  UINT8                           Voltage;            ///< VDDP/VDDR Voltage
  UINT16                          GpioPin1;           ///< GPIO pin
  UINT8                           Value1;             ///< Value of GPIO pin
} AMD_CPM_GPIO_VDDP_VDDR_VOLTAGE_ITEM;

/// VDDP/VDDR voltage table

typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                           ///< Table header
  AMD_CPM_GPIO_VDDP_VDDR_VOLTAGE_ITEM Item[AMD_GPIO_VDDP_VDDR_VOLTAGE_SIZE];  ///< GPIO setting list for memory voltage
} AMD_CPM_GPIO_VDDP_VDDR_VOLTAGE_TABLE;


/// PCIe Clock Setting

typedef struct _AMD_CPM_PCIE_CLOCK_ITEM {
  UINT8                           ClkId;              ///< FCH PCIe Clock
  UINT8                           ClkReq;             ///< FCH PCIe ClkReq
  UINT8                           ClkIdExt;           ///< External Clock Source
  UINT8                           ClkReqExt;          ///< External ClkReq
  UINT8                           DeviceId;           ///< Device Id. No Device Id if 0xFF
  UINT8                           Device;             ///< Device Number of PCIe bridge
  UINT8                           Function;           ///< Function Number of PCIe bridge
  UINT8                           SlotCheck;          ///< Slot Check Flag
  UINT32                          SpecialFunctionId;  ///< Id of Special Function
} AMD_CPM_PCIE_CLOCK_ITEM;

/// PCIe Clock Table

typedef struct _AMD_CPM_PCIE_CLOCK_TABLE {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                     ///< Table header
  AMD_CPM_PCIE_CLOCK_ITEM         Item[AMD_PCIE_CLOCK_SIZE];  ///< PCIe clock setting list
} AMD_CPM_PCIE_CLOCK_TABLE;


/// External ClkGen Register Setting Item

typedef struct _AMD_CPM_EXT_CLKGEN_ITEM {
  UINT8                           Function;           ///< External ClkGen Setting Item Type
                                                      ///<   0x00~0x7F: Initial Sequence Id
                                                      ///<   0x80~0x8F: Clock Disable Sequence
                                                      ///<   0x90~0x9F: ClkReq Enable Sequence
  UINT8                           Offset;             ///< Register Offset
  UINT8                           AndMask;            ///< AND Mask
  UINT8                           OrMask;             ///< Or Mask
} AMD_CPM_EXT_CLKGEN_ITEM;


/// External ClkGen Table

typedef struct _AMD_CPM_EXT_CLKGEN_TABLE {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                     ///< Table header
  UINT8                           SmbusSelect;                ///< SMBUS Number
  UINT8                           SmbusAddress;               ///< SMBUS Address
  AMD_CPM_EXT_CLKGEN_ITEM         Item[AMD_EXT_CLKGEN_SIZE];  ///< External ClkGen Register Setting List
} AMD_CPM_EXT_CLKGEN_TABLE;


/// PCIE Topology Table

typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                               ///< Table header
  UINT32                          SocketId;                             ///< Socket Id
  PCIe_PORT_DESCRIPTOR            Port[AMD_PCIE_PORT_DESCRIPTOR_SIZE];  ///< PCIe Port Descriptor List
  PCIe_DDI_DESCRIPTOR             Ddi[AMD_PCIE_DDI_DESCRIPTOR_SIZE];    ///< PCIe DDI Descriptor List
} AMD_CPM_PCIE_TOPOLOGY_TABLE;


/// The override table definition for PCIE Topology

typedef struct {
  union {
    UINT8                         Raw;                        ///< Value of Flag
    struct {
      UINT8                       EnableOverride:1;           ///< Override Enable field of descriptor
      UINT8                       DdiTypeOverride:1;          ///< Override DDI type
      UINT8                       LaneOverride:1;             ///< Override StartLane and EndLane
      UINT8                       PortPresentOverride:1;      ///< Override PortPresent
      UINT8                       IsDdi:1;                    ///< This item is used to override Port or DDI descriptor
                                                              ///<   0: Port Descriptor
                                                              ///<   1: DDI Descriptor
      UINT8                       Reserved:2;                 ///< Reserved
      UINT8                       Valid:1;                    ///< Valid Flag
    }                             Config;                     ///< Bitmap of Flag
  }                               Flag;                       ///< Flag of PCIe Topology override item
  UINT8                           Offset;                     ///< Offset of Port Descriptor or DDI Dscriptor list
  UINT8                           Enable;                     ///< Descriptor Enable
  UINT8                           DdiType;                    ///< Ddi Type
  UINT8                           PortPresent;                ///< Port Present
  UINT8                           StartLane;                  ///< Start Lane
  UINT8                           EndLane;                    ///< End Lane
} AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_ITEM;


/// PCIE Topoplogy Override Table

typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER         Header;                                 ///< Table header
  AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_ITEM Item[AMD_PCIE_TOPOLOGY_OVERRIDE_SIZE];  ///< Override Item List
} AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_TABLE;


/// Express Card Table

typedef struct _AMD_CPM_EXPRESS_CARD_TABLE {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                         ///< Table header
  UINT8                           Device;                         ///< Device Number of PCIe Bridge
  UINT8                           Function;                       ///< Function Number of PCIe Bridge
  UINT8                           EventPin;                       ///< GEVENT Pin
  UINT8                           DeviceId;                       ///< Device Id
} AMD_CPM_EXPRESS_CARD_TABLE;


/// Wireless Button Table

typedef struct _AMD_CPM_WIRELESS_BUTTON_TABLE {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                         ///< Table header
  AMD_CPM_PCI_DEVICE_FUNCTION     Bridge[4];                      ///< Device & Function Number of PCIe Bridge
  UINT8                           EventPin;                       ///< GEVENT Pin
  UINT8                           DeviceIdRadio;                  ///< Device Id to control radio
  UINT8                           DeviceIdPower;                  ///< Device Id to control power
  UINT8                           DeviceIdOther;                  ///< Device Id to control other device
} AMD_CPM_WIRELESS_BUTTON_TABLE;


/// The table definition for Acpi Thermal Fan Control

typedef struct {
  UINT8                           CpuCRT;                         ///< Critical temperature threshold
  UINT8                           CpuPSV;                         ///< Passive temperature threshold
  UINT8                           CpuAC0;                         ///< TemperatureThreshold 0
  UINT8                           CpuAC1;                         ///< TemperatureThreshold 1
  UINT8                           CpuAC2;                         ///< TemperatureThreshold 2
  UINT8                           CpuAC3;                         ///< TemperatureThreshold 3
  UINT8                           CpuAL0;                         ///< FanSpeedPwmLevel 0
  UINT8                           CpuAL1;                         ///< FanSpeedPwmLevel 1
  UINT8                           CpuAL2;                         ///< FanSpeedPwmLevel 2
  UINT8                           CpuAL3;                         ///< FanSpeedPwmLevel 3
  UINT8                           ThermalSensor;                  ///< Thermal Sensor
  UINT8                           HysteresisInfo;                 ///< HysteresisInfo for active cooling
  UINT8                           HysteresisInfoPsv;              ///< HysteresisInfo for Passive cooling
} AMD_CPM_FAN_POLICY;


/// Fan Hardware Config

typedef struct {
  UINT8                           EventPin;                       ///< GEVENT Pin
  UINT8                           FanNum;                         ///< SB Fan control fields
} AMD_CPM_FAN_HW_CONFIG;


/// ACPI Thermal Fan Table

typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                         ///< Table header
  AMD_CPM_FAN_HW_CONFIG           FanHwConfig;                    ///< Fan HW Config
  AMD_CPM_FAN_POLICY              FanPolicy;                      ///< Fan Policy
} AMD_CPM_ACPI_THERMAL_FAN_TABLE;


/// Convert from Device Id of SATA controller to SATA mode mask

typedef struct {
  UINT16                          DeviceId;                   ///< Device Id of SATA controller
  UINT8                           Mask;                       ///< SATA Mode Mask
} AMD_CPM_SATA_MODE_MASK;


/// Zero Power Odd Table

typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                     ///< Table header
  UINT8                           DeviceId;                   ///< Device Id
  UINT8                           EventPin1;                  ///< Event Pin Number for FCH_ODD_DA
  UINT8                           EventPin2;                  ///< Event Pin Number for ODD_PLUGIN#
  UINT8                           EventPin3;                  ///< Dummy Event
  UINT8                           SataModeSupportMask;        ///< Mask of SATA mode to support
                                                              ///< BIT0: IDE Mode
                                                              ///< BIT1: AHCI Mode
                                                              ///< BIT2: RAID Mode
                                                              ///< BIT3: AMD AHCI Mode
  UINT8                           SataPortId;                 ///< Sata Port Number for Odd
} AMD_CPM_ZERO_POWER_ODD_TABLE;


/// The table definition for LPC UART

typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                     ///< Table header
  UINT8                           ChipId;                     ///< Chip Id. 0: SMSC_1036
  UINT8                           LpcIndex;                   ///< Index of LPC register. 0: 2E/2F. 1: 4E/4F
  UINT16                          Address;                    ///< Uart base register
  UINT8                           Irq;                        ///< IRQ
} AMD_CPM_LPC_UART_TABLE;


/// The table definition for PROCHOT

typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                     ///< Table header
  UINT8                           FanNum;                     ///< Pin Number of FanOut
  UINT8                           Freq;                       ///< Frequency Setting for FanOut
} AMD_CPM_PROCHOT_TABLE;


/// Display Feature Config

typedef union {
  UINT32                        Raw;                        ///< Display feature config value
  struct {                                                  ///<
    UINT32                      PowerXpressFixedMode:1;     ///< BIT0: PowerXpress Fixed Mode
    UINT32                      PowerXpressDynamicMode:2;   ///< BIT1-2: PowerXpress Dynamic Mode
    UINT32                      HyperCrossFire:1;           ///< BIT3: HyperCrossFire
    UINT32                      SurroundView:1;             ///< BIT4: SurroundView
    UINT32                      Reserved1:3;                ///< BIT5-7: Reserved
    UINT32                      IsDgpuPrimary:1;            ///< BIT8: IsDgpuPrimary
    UINT32                      IsBrightnessByDriver:1;     ///< BIT9: IsBrightnessControlledByDriver
    UINT32                      DisableDgpuAudioInPX:1;     ///< BIT10: Disable Audio Device in DGPU if PowerXpress Mode is enabled
    UINT32                      DualGraphicsNotSupported:1; ///< BIT11: Dual Graphics is not supported
    UINT32                      DgpuDisplayOutput:1;        ///< BIT12: Discrete Graphics can drive display outputs
    UINT32                      SpecialPostIgpu:1;          ///< BIT13: Force Special Post for Igpu if Dgpu is primary
    UINT32                      PulseGeneratorSupport:1;    ///< BIT14: Plug-in/unplug interrupts will be generated on every
                                                            ///<        display plug-in/unplug event
    UINT32                      RebrandDualGraphics:1;      ///< BIT15: Rebranding a dual graphics feature by overriding the SSID of
                                                            ///<        dGPU device to the DID of iGPU
    UINT32                      Reserved2:16;               ///< BIT16-31: Reserved
  }                             Config;                     ///< Display feature config
} AMD_CPM_DISPLAY_FEATURE_CONFIG;


/// Display Feature Support Mask

typedef union {
  UINT32                        Raw;                        ///< Display Feature support value
  struct {                                                  ///<
    UINT32                      PowerXpress:1;              ///< BIT0: PowerXpress Fixed Mode
    UINT32                      HyperCrossFire:1;           ///< BIT1: HyperCrossFire
    UINT32                      SurroundView:1;             ///< BIT2: SurroundView
    UINT32                      Reserved1:13;               ///< BIT3-15: Reserved
    UINT32                      Bus:8;                      ///< BIT16-23: Reserved
    UINT32                      Reserved2:4;                ///< BIT24-28: Reserved
    UINT32                      Removable:1;                ///< BIT28: Removable in PX mode
    UINT32                      Vga:1;                      ///< BIt29: VgaEn
    UINT32                      Exist:1;                    ///< BIT30: Exist
    UINT32                      Valid:1;                    ///< BIT31: Valid
  }                             Mask;                       ///< Bit mapping for display feature support
} AMD_CPM_DISPLAY_FEATURE_SUPPORT;


/// Device Path Item Definition

typedef struct {
  AMD_CPM_DISPLAY_FEATURE_SUPPORT FeatureMask;              ///< See AMD_CPM_DISPLAY_FEATURE_SUPPORT
  UINT8                           IsDgpu;                   ///< Is Igpu or Dgpu. 0: iGpu; 1: dGpu
  AMD_CPM_PCI_DEVICE_FUNCTION     Bridge;                   ///< Device and function number of PCIe Bridge
  AMD_CPM_PCI_DEVICE_FUNCTION     Device;                   ///< Device and function number of Display Device
  UINT8                           DeviceId;                 ///< Device Id to control GPIO
  UINT8                           Mode;                     ///< Power mode
} AMD_CPM_DEVICE_PATH_ITEM;


/// Device Path Table

typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                             ///< Table header
  AMD_CPM_DEVICE_PATH_ITEM        Path[AMD_DISPLAY_DEVICE_PATH_SIZE]; /// Device Path List
} AMD_CPM_DEVICE_PATH_TABLE;

/// Speccific SSID Item Definition

typedef struct {
  UINT16                          VendorId;                   ///< Vendor Id
  UINT16                          DeviceId;                   ///< Device Id
} AMD_CPM_SPECIFIC_SSID_ITEM;


/// Specific SSID Table
typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                               ///< Table header
  AMD_CPM_SPECIFIC_SSID_ITEM      Item[AMD_SPECIFIC_SSID_DEVICE_SIZE];  ///< VendorId and Device Id List for Specific SSID
} AMD_CPM_SPECIFIC_SSID_TABLE;


/// Rebrand Dual Graphics SSID Item Definition

typedef struct {
  UINT16                          VendorId;                   ///< Vendor Id
  UINT16                          DeviceId;                   ///< Device Id
  UINT8                           IsDgpu;                     ///< Is Igpu or Dgpu. 0: iGpu; 1: dGpu
} AMD_CPM_REBRAND_DUAL_GRAPHICS_SSID_ITEM;


/// Rebrand Dual Graphics SSID Table
typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER             Header;                                           ///< Table header
  AMD_CPM_REBRAND_DUAL_GRAPHICS_SSID_ITEM Item[AMD_REBAND_DUAL_GRAPHICS_SSID_DEVICE_SIZE];  ///< VendorId and Device Id List for Rebrand Dual Graphics SSID
} AMD_CPM_REBRAND_DUAL_GRAPHICS_SSID_TABLE;


/// Display Information Structure for ATPX Sub-Function 8

typedef struct {
  UINT8                           Flags;                                ///< Flags
  UINT8                           AtifId;                               ///< ATIF Id
  UINT8                           AdaptorId;                            ///< Adaptor Id
  UINT16                          AcpiId;                               ///< ACPI Id
} AMD_CPM_DISPLAY_CONNECTOR_8;

/// Display Information Structure for ATPX Sub-Function 8

typedef struct {
  UINT8                           AtifId;                               ///< ATIF Id
  UINT8                           HpdPortId;                            ///< HPD Port Id
  UINT8                           DdcPortId;                            ///< DDC Port Id
} AMD_CPM_DISPLAY_CONNECTOR_9;


/// Structure for Graphic Device

typedef struct {
  UINT32                          Flags;                                ///< Flags
  UINT16                          Bus;                                  ///< Bus Number
  UINT16                          Device;                               ///< Device Number
} AMD_CPM_GFX_DEVICE;

/// Structure for ATIF Device List

typedef struct {
  UINT16                          NumDevice;                            ///< Device Number
  UINT16                          DeviceStructSize;                     ///< Device Structure Size
  AMD_CPM_GFX_DEVICE              Device[3];                            ///< Device List
} AMD_CPM_GFX_DETECT;


/// Display Feature Table

typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                                                         ///< Table header
  UINT8                           FunctionDisableMask;                                            ///< BIT0: Disable SetSsid. BIT1: Disable AddSsdt
  UINT8                           MxmDeviceId;                                                    ///< Device Id for MXM
  UINT8                           MxmOverTempEvent;                                               ///< GEVENT pin number for MXM_OVERT#
  UINT8                           MxmOverTempStateId;                                             ///< Forced Power State Id if MXM_OVERT# is low
  UINT8                           DisplayConnectEvent;                                            ///< GEVENT pin number for Discrete GPU display connect/disconnect event
  UINT8                           DockingDeviceId;                                                ///< Bit[0-6]: Device Id for docking detection if Bit7 = 0
                                                                                                  ///<           Forced docking status if Bit7 = 1
  UINT8                           MuxFlag;                                                        ///< The flag for Mux-Based Power Xpress
  UINT8                           DisplayMuxDeviceId;                                             ///< Device Id for Display Mux
  UINT8                           I2xMuxDeviceId;                                                 ///<
  UINT8                           AtpxConnector8Number;                                           ///< Number of reported display connectors in ATPX sub-function 8
  AMD_CPM_DISPLAY_CONNECTOR_8     AtpxConnector8[AMD_ATPX_CONNECTOR_8_SIZE];                      ///< The Connector information for ATPX sub-function 8
  UINT8                           AtpxConnector9Number;                                           ///< Number of reported display connectors in ATPX sub-function 9
  AMD_CPM_DISPLAY_CONNECTOR_9     AtpxConnector9[AMD_ATPX_CONNECTOR_9_SIZE];                      ///< The Connector information for ATPX sub-function 9
  UINT32                          AtifSupportedNotificationMask;                                  ///< Supported Notifications Mask in ATIF sub-function 0
  UINT8                           AtifDeviceCombinationNumber;                                    ///< The number of Display Device Combination
  UINT8                           AtifDeviceCombinationBuffer[AMD_ATIF_DEVICE_COMBINATION_BUFFER_SIZE]; ///< The data of Display Device Combination
  UINT8                           Atif16Buffer[0x100];                                            ///< The data for Query Brightness Transfer Characteristics
} AMD_CPM_DISPLAY_FEATURE_TABLE;



/// Definition of CPM EC config

typedef union {
  UINT8                           Raw;                        ///< EC Config Value
  struct {                                                    ///<
    UINT8                         AcDcSwitchEn:1;             ///< AC/DC Switch Enable
    UINT8                         ColdBootCyclingEn:1;        ///< Cold Boot Cycling Enable
    UINT8                         S5PlusEn:1;                 ///< S5+ Support
    UINT8                         Reserved:5;                 ///< Reserved
  }                               Config;                     ///< Bitmap of EC Config
} AMD_CPM_EC_CONFIG;


/// EC Init Table

typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                     ///< Table header
  UINT8                           AcTimer;                    ///< AC Timer in minute
  UINT8                           DcTimer;                    ///< DC Timer in minute
  UINT8                           OnTimer;                    ///< Power On Timer in second
} AMD_CPM_EC_INIT_TABLE;


/// TDP Limit Change Table

typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                     ///< Table header
  UINT8                           SwSciBit;                   ///< Soft Sci Bit
  UINT8                           SwSmiId;                    ///< Command to trigger SW SMI
  UINT16                          SwSmiRegister;              ///< SW SMI Register
} AMD_CPM_TDP_LIMIT_CHANGE_TABLE;


/// SMI Check Tool Table
typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                     ///< Table header
  UINT8                           SwSmiId;                    ///< Command to trigger SW SMI
  UINT16                          SwSmiRegister;              ///< SW SMI Register
} AMD_CPM_SMI_CHECK_TOOL_TABLE;


/// Local SMI Status

typedef struct {
  UINT8                           ApicId;                             ///< Core apic id
  UINT32                          LocalSmiStatus;                     ///< SMMFEC4 Local SMI Status
} AMD_CPM_LOCAL_SMI_STATUS;


/// SMI Data

typedef struct {
  UINT32                          Signature;                          ///< Signature "$SCK"
  UINT8                           SmiCpuCnt;                          ///< Total number of cores
  AMD_CPM_LOCAL_SMI_STATUS        SmiSts[AMD_MAX_CPU_CORE_NUMBERS];   ///< X-Total number of cores. Holds "LocalSMIStatus" structure for each core.
} AMD_CPM_SMI_DATA;


/// Adaptive S4 Table

typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                             ///< Table header
  UINT8                           BufferType;                         ///< Buffer Type. 5: BIOS RAM. Other: Reserved.
  UINT8                           BufferOffset;                       ///< Buffer Offset
  UINT8                           BufferSize;                         ///< Buffer Size
  UINT8                           EcRamOffset;                        ///< Offset of EC RAM
} AMD_CPM_ADAPTIVE_S4_TABLE;

/// AMD CPM Main Table

typedef struct {
  AMD_CPM_TABLE_COMMON_HEADER     Header;                         ///< Table header
  UINT8                           PlatformName[32];               ///< Platform name
  UINT8                           BiosType;                       ///< BIOS type
  UINT16                          CurrentPlatformId;              ///< Current Platform Id
  UINT32                          PcieMemIoBaseAddr;              ///< PcieMemIoBaseAddr
  UINT32                          AcpiMemIoBaseAddr;              ///< AcpiMemIoBaseAddr
  AMD_CPM_POINTER                 Service;                        ///< Reserved for internal use
  AMD_CPM_POINTER                 TableInRomList;                 ///< Reserved for internal use
  AMD_CPM_POINTER                 TableInRamList;                 ///< Reserved for internal use
  AMD_CPM_POINTER                 TableInHobList;                 ///< Reserved for internal use
  AMD_CPM_POINTER                 HobTablePtr;                    ///< Reserved for internal use
  AMD_CPM_DISPLAY_FEATURE_CONFIG  DisplayFeature;                 ///< Display feature config
  UINT8                           ZeroPowerOddEn;                 ///< ZeroPowerOdd Config. BIT0: ZeroPowerOddEn.
                                                                  ///<                      BIT1: OddHotplugEn.
                                                                  ///<                      BIT2: SystemBootWithPS0.
                                                                  ///<                      BIT3: Enable _PRW
                                                                  ///<                      BIT4: Enable port reset workaround
  UINT8                           AcpiThermalFanEn;               ///< ACPI thermal fan config
  UINT8                           ExtClkGen;                      ///< External ClkGen Config. 0x00~0x7F
  UINT8                           UnusedGppClkOffEn;              ///< Config to turn off unused GPP clock
  UINT8                           AdaptiveS4En;                   ///< Adaptive S4 Config. 0: Disable. 1: Enable EC Method. 2: Enable RTC Method.
  UINT8                           WirelessButtonEn;               ///< Wireless Button Config. 0: Disable. 1: Radio Off. 2: Power Off
  AMD_CPM_EC_CONFIG               Ec;                             ///< External EC config
  UINT8                           TdpLimitChangeEn;               ///< TdpLimitChangeEn
  UINT8                           SmiCheckToolEn;                 ///< SmiCheckToolEn
  UINT8                           LpcUartEn;                      ///< LpcUartEn
  UINT8                           ProchotEn;                      ///< ProchotEn
} AMD_CPM_MAIN_TABLE;


/// Header of CPM Hob table

typedef struct {
  AMD_CPM_POINTER                 HeaderPtr;                  ///< Table header
  AMD_CPM_POINTER                 MainTablePtr;               ///< Pointer of CPM main table
  UINT32                          Revision;                   ///< CPM Revision
  UINT32                          BufferItem;                 ///< Available Table Number
  UINT32                          BufferOffset;               ///< Offset of Available Buffer
  UINT32                          BufferSize;                 ///< Size of Available Table
} AMD_CPM_HOB_HEADER;


/// Structure of PCI PFA
typedef union {
  UINT16                          Raw;                        ///< PCI Pfa value
  struct {                                                    ///<
    UINT8                         Function:3;                 ///< PCI Function Number
    UINT8                         Device:5;                   ///< PCI Device Number
    UINT8                         Bus;                        ///< PCI Bus Number
  }                               Pfa;                        ///< PCI Pfa
} AMD_CPM_PCI_PFA;


/// CPM NV Data Table

typedef struct {
  UINT32                          CpmVersion;                 ///< CPM Revision
  UINT32                          CpmPcieMmioBaseAddr;        ///< PcieMmioBaseAddress
  UINT32                          CpmAcpiMmioBaseAddr;        ///< AcpiMmioBaseAddress
  UINT8                           CpmSbChipId;                ///< SbChipId
  UINT8                           CpmChipId[3];               ///< ChipId

  UINT32                          CpmDisplayFeatureConfig;    ///< DisplayFeatureConfig
  UINT16                          CpmiGpuP2pBridgePfa;        ///< iGpuP2pBridgePfa
  UINT16                          CpmiGpuP2pDevicePfa;        ///< iGpuP2pDevicePfa
  UINT16                          CpmdGpuP2pBridgePfa;        ///< dGpuP2pBridgePfa
  UINT16                          CpmdGpuP2pDevicePfa;        ///< dGpuP2pDevicePfa
  UINT8                           CpmdGpuAspmLxEnable;        ///< dGpuAspmLxEnable
  UINT8                           CpmdGpuAudioDisable;        ///< dGpuAudioDisable

  UINT32                          CpmAtpxSupportedFunctionMask; ///< AtpxSupportedFunctionMask
  UINT32                          CpmAtpxFlags;               ///< AtpxFlags
  UINT32                          CpmAtcsSupportedFunctionMask; ///< AtcsSupportedFunctionMask;
  UINT32                          CpmAtifSupportedNotificationMask; ///< AtifSupportedNotificationMask
  UINT32                          CpmAtifFunctionBitVector;   ///< AtifFunctionBitVector
  UINT32                          CpmAtifFlags;               ///< AtifFlags
  UINT8                           CpmAtifDeviceListBuffer[28];  ///< AtifDeviceListBuffer
  UINT32                          CpmAtrmRomSize;             ///< VBIOS image size
  UINT8                           CpmAtrmRomImage[0x10000];   ///< VBIOS image

  UINT64                          CpmSmiCheckCheckToolData;   ///< Smi Check Tool Data Pointer

  UINT32                          CpmMainTable;               ///< Offset of AMD_CPM_MAIN_TABLE
  UINT32                          CpmDeviceDetectionTable;    ///< Offset of AMD_CPM_GPIO_DEVICE_DETECTION_TABLE
  UINT32                          CpmDeviceResetTable;        ///< Offset of AMD_CPM_GPIO_DEVICE_RESET_TABLE
  UINT32                          CpmDevicePowerTable;        ///< Offset of AMD_CPM_GPIO_DEVICE_POWER_TABLE
  UINT32                          CpmWirelessButtonTable;     ///< Offset of AMD_CPM_WIRELESS_BUTTON_TABLE
  UINT32                          CpmSaveContextTable;        ///< Offset of AMD_CPM_SAVE_CONTEXT_TABLE
  UINT32                          CpmExpressCardTable;        ///< Offset of AMD_CPM_EXPRESS_CARD_TABLE
  UINT32                          CpmDisplayFeatureTable;     ///< Offset of AMD_CPM_DISPLAY_FEATURE_TABLE
  UINT32                          CpmZeroPowerOddTable;       ///< Offset of AMD_CPM_ZERO_POWER_ODD_TABLE
  UINT32                          CpmAdaptiveS4Table;         ///< Offset of AMD_CPM_ADAPTIVE_S4_TABLE
  UINT32                          CpmAcpiThermalFanTable;     ///< Offset of AMD_CPM_ACPI_THERMAL_FAN_TABLE
} AMD_CPM_NV_DATA_STRUCT;


/// Structure for Display Feature Private Data

typedef struct {
  VOID                            *DisplayFeatureTablePtr;    ///< Pointer of Display Feature Table
  VOID                            *DevicePathTablePtr;        ///< Pointer of Device Path Table
  VOID                            *SpecificSsidTablePtr;      ///< Pointer of Specific SSID Table
  VOID                            *RebrandDualGraphicsSsidTablePtr; ///< Pointer of Rebrand Dual Graphics SSID Table
  AMD_CPM_DISPLAY_FEATURE_SUPPORT CurrentFeature;             ///< Current Display Feature Support
  AMD_CPM_PCI_PFA                 GfxBridgePfa[3];            ///< GfxBridgePfa. [0]: iGpu Pfa. [1]: dGpu Pfa [2]: dGpu Pfa
  AMD_CPM_PCI_PFA                 GfxDevicePfa[3];            ///< GfxDevicePfa. [0]: iGpu Pfa. [1]: dGpu Pfa [2]: dGpu Pfa
  UINT32                          VBiosImage;                 ///< VBIOS Image Address
  UINT32                          VBiosImageSize;             ///< VBIOS Image Size
  BOOLEAN                         VBiosFlag;                  ///< VBIOS Flag. [1]: dGPU VBIOS Rom.
  UINT32                          Ssid;                       ///< SSID for GPU
  UINT32                          Ssid2;                      ///< SSID for DGPU in when rebrand dual graphics is enabled
} CPM_DISPLAY_FEATURE_PRIVATE;

/// Structure for FCH Strap Data

typedef struct {
  UINT32      ImcEnable:1;                                    ///< Imc status. 0: Disable, 1: Enable
  UINT32      InternalClock:1;                                ///< Internal clock status. 0: 25Mhz crystal clock, 1: 100Mhz PCI-E clock
  UINT32      S5PlusSupport:1;                                ///< S5+ support. 0: not support. 1: support.
} AMD_CPM_STRAP_SETTING;

/// Convert from PCIe device and function number to ASL name

typedef struct {
  UINT8     PcieRevision;     ///< PCIe bridge revision
  UINT8     Device;           ///< Device number of PCIe bridge
  UINT8     Function;         ///< Function number of PCIe bridge
  UINT8     NameId;           ///< ASL name id of PCIe bridge
  UINT32    Name;             ///< ASL name of PCIe bridge
} PCIE_BRIDGE_NAME;

/// Structure for CPU revision

typedef struct {
  UINT32    Mask;             ///< CPU Id Mask
  UINT32    Value;            ///< Value
  UINT8     CpuRevision;      ///< Cpu Revision
  UINT8     PcieRevision;     ///< PCIe bridge revision
} CPU_REVISION_ITEM;

/// Structure for Chip Id

typedef struct {
  UINT8     Cpu;              ///< CPU/APU Chip Id
  UINT8     Sb;               ///< SB Chip Id
  UINT8     Reserved[6];      ///<
} AMD_CPM_CHIP_ID;

#pragma pack(pop)

#endif
