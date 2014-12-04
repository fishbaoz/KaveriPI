/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD User options selection for a Kaveri FP3 platform solution system
 *
 * This file is placed in the user's platform directory and contains the
 * build option selections desired for that platform.
 *
 * For Information about this file, see @ref platforminstall.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Core
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 */
/*****************************************************************************
 *
 * Copyright 2008 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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
 * ***************************************************************************
 */
#include  "AGESA.h"
#include  "CommonReturns.h"
#include "Filecode.h"
#define FILECODE PLATFORM_SPECIFIC_OPTIONS_FILECODE
/* User makes option selections here
 * Comment out the items wanted to be included in the build.
 * Uncomment those items you wish to REMOVE from the build.
 */
//#define BLDOPT_REMOVE_UDIMMS_SUPPORT           TRUE
#define BLDOPT_REMOVE_RDIMMS_SUPPORT           TRUE
#define BLDOPT_REMOVE_LRDIMMS_SUPPORT          TRUE
//#define BLDOPT_REMOVE_ECC_SUPPORT              TRUE
//#define BLDOPT_REMOVE_BANK_INTERLEAVE          TRUE
//#define BLDOPT_REMOVE_DCT_INTERLEAVE           TRUE
#define BLDOPT_REMOVE_NODE_INTERLEAVE          TRUE
#define BLDOPT_REMOVE_PARALLEL_TRAINING        TRUE
#define BLDOPT_REMOVE_ONLINE_SPARE_SUPPORT     TRUE
//#define BLDOPT_REMOVE_MEM_RESTORE_SUPPORT      TRUE
#define BLDOPT_REMOVE_MULTISOCKET_SUPPORT        TRUE
//#define BLDOPT_REMOVE_ACPI_PSTATES             TRUE
#define BLDOPT_REMOVE_SRAT                     TRUE
#define BLDOPT_REMOVE_SLIT                     TRUE
#define BLDOPT_REMOVE_WHEA                     TRUE
//#define BLDOPT_REMOVE_DMI                      TRUE
#define BLDOPT_REMOVE_EARLY_SAMPLES            FALSE
//#define BLDCFG_REMOVE_ACPI_PSTATES_PPC               TRUE
//#define BLDCFG_REMOVE_ACPI_PSTATES_PCT               TRUE
//#define BLDCFG_REMOVE_ACPI_PSTATES_PSD               TRUE
//#define BLDCFG_REMOVE_ACPI_PSTATES_PSS               TRUE
//#define BLDCFG_REMOVE_ACPI_PSTATES_XPSS              TRUE
//#define BLDCFG_NB_PSTATES_SUPPORTED                  FALSE

//This element selects whether P-States should be forced to be independent,
// as reported by the ACPI _PSD object. For single-link processors,
// setting TRUE for OS to support this feature.
//#define BLDCFG_FORCE_INDEPENDENT_PSD_OBJECT  TRUE

/* Build configuration values here.
 */
// Specify the default values for the VRM controlling the VDD plane.
#define BLDCFG_VRM_CURRENT_LIMIT                 55000
#define BLDCFG_VRM_LOW_POWER_THRESHOLD           0
#define BLDCFG_VRM_MAXIMUM_CURRENT_LIMIT         80000
#define BLDCFG_PLAT_NUM_IO_APICS                 3
#define BLDCFG_CORE_LEVELING_MODE                CORE_LEVEL_LOWEST
#define BLDCFG_MEM_INIT_PSTATE                   0

// Specify the default values for the VRM controlling the VDDNB plane.
// If not specified, the values used for the core VRM will be applied
#define BLDCFG_VRM_NB_CURRENT_LIMIT              50000
//#define BLDCFG_VRM_NB_LOW_POWER_THRESHOLD         1  // Zero - disable NBPSI_L, Non-zero - enable NBPSI_L
#define BLDCFG_VRM_NB_MAXIMUM_CURRENT_LIMIT      65000
//#define BLDCFG_VRM_NB_HIGH_SPEED_ENABLE           0

// Override the default strings used for AGESA ACPI table OEM fields.
// These strings must conform to applicable ACPI rules for OEM fields.
// Default strings are 'AMD   ' and 'AGESA   '
//#define  BLDCFG_ACPI_SET_OEM_ID                   ' ',' ',' ',' ',' ',' '             // This string must be 6 characters long.
//#define  BLDCFG_ACPI_SET_OEM_TABLE_ID             ' ',' ',' ',' ',' ',' ',' ',' '     // This string must be 8 characters long.

#define BLDCFG_AMD_PLATFORM_TYPE                  AMD_PLATFORM_MOBILE

#define BLDCFG_DIMM_TYPE_USED_IN_MIXED_CONFIG     DDR3_TECHNOLOGY
#define BLDCFG_MEMORY_BUS_FREQUENCY_LIMIT         DDR2133_FREQUENCY
#define BLDCFG_MEMORY_MODE_UNGANGED               TRUE
#define BLDCFG_MEMORY_QUAD_RANK_CAPABLE           TRUE
#define BLDCFG_MEMORY_QUADRANK_TYPE               QUADRANK_UNBUFFERED
#define BLDCFG_MEMORY_RDIMM_CAPABLE               FALSE
#define BLDCFG_MEMORY_UDIMM_CAPABLE               TRUE
#define BLDCFG_MEMORY_SODIMM_CAPABLE              TRUE
#define BLDCFG_MEMORY_ENABLE_BANK_INTERLEAVING    TRUE
#define BLDCFG_MEMORY_ENABLE_NODE_INTERLEAVING    FALSE
#define BLDCFG_MEMORY_CHANNEL_INTERLEAVING        TRUE
#define BLDCFG_MEMORY_POWER_DOWN                  TRUE
#define BLDCFG_POWER_DOWN_MODE                    POWER_DOWN_BY_CHIP_SELECT
#define BLDCFG_ONLINE_SPARE                       FALSE
#define BLDCFG_BANK_SWIZZLE                       TRUE
#define BLDCFG_TIMING_MODE_SELECT                 TIMING_MODE_AUTO
#define BLDCFG_MEMORY_CLOCK_SELECT                DDR800_FREQUENCY
#define BLDCFG_DQS_TRAINING_CONTROL               TRUE
#define BLDCFG_IGNORE_SPD_CHECKSUM                FALSE
#define BLDCFG_USE_BURST_MODE                     FALSE
#define BLDCFG_MEMORY_ALL_CLOCKS_ON               FALSE
#define BLDCFG_ENABLE_ECC_FEATURE                 TRUE
#define BLDCFG_ECC_REDIRECTION                    FALSE
#define BLDCFG_SCRUB_DRAM_RATE                    0
#define BLDCFG_SCRUB_L2_RATE                      0
#define BLDCFG_SCRUB_L3_RATE                      0
#define BLDCFG_SCRUB_IC_RATE                      0
#define BLDCFG_SCRUB_DC_RATE                      0
#define BLDCFG_ECC_SYNC_FLOOD                     TRUE
#define BLDCFG_ECC_SYMBOL_SIZE                    4
#define BLDCFG_HEAP_DRAM_ADDRESS                  0xB0000ul
#define BLDCFG_1GB_ALIGN                          FALSE
#define BLDCFG_VRM_HIGH_SPEED_ENABLE              TRUE
#define BLDCFG_PLATFORM_CSTATE_IO_BASE_ADDRESS    0x1770
#define BLDCFG_UMA_ALIGNMENT                      UMA_4MB_ALIGNED
//#define BLDCFG_PLATFORM_CPB_MODE                  CpbModeDisabled
//#define BLDCFG_PROCESSOR_SCOPE_NAME0              'C'
//#define BLDCFG_PROCESSOR_SCOPE_NAME1              '0'
#define BLDCFG_PLATFORM_CSTATE_MODE               CStateModeDisabled        // ENH395294
//#define BLDCFG_PLATFORM_CSTATE_OPDATA             0
#define BLDCFG_CFG_ABM_SUPPORT                    TRUE

/*  Process the options...
 * This file include MUST occur AFTER the user option selection settings
 */
#define AGESA_ENTRY_INIT_RESET                    FALSE
#define AGESA_ENTRY_INIT_RECOVERY                 FALSE
#define AGESA_ENTRY_INIT_EARLY                    TRUE
#define AGESA_ENTRY_INIT_POST                     TRUE
#define AGESA_ENTRY_INIT_ENV                      TRUE
#define AGESA_ENTRY_INIT_MID                      TRUE
#define AGESA_ENTRY_INIT_LATE                     TRUE
#define AGESA_ENTRY_INIT_S3SAVE                   TRUE
#define AGESA_ENTRY_INIT_RESUME                   TRUE
#define AGESA_ENTRY_INIT_LATE_RESTORE             FALSE
#define AGESA_ENTRY_INIT_FINAL_RESTORE            TRUE
#define AGESA_ENTRY_INIT_GENERAL_SERVICES         TRUE
/*
 * Customized OEM build configurations for FCH component
 */
// #define BLDCFG_SMBUS0_BASE_ADDRESS            0xB00
// #define BLDCFG_SMBUS1_BASE_ADDRESS            0xB20
// #define BLDCFG_SIO_PME_BASE_ADDRESS           0xE00
// #define BLDCFG_ACPI_PM1_EVT_BLOCK_ADDRESS     0x400
// #define BLDCFG_ACPI_PM1_CNT_BLOCK_ADDRESS     0x404
// #define BLDCFG_ACPI_PM_TMR_BLOCK_ADDRESS      0x408
// #define BLDCFG_ACPI_CPU_CNT_BLOCK_ADDRESS     0x410
// #define BLDCFG_ACPI_GPE0_BLOCK_ADDRESS        0x420
// #define BLDCFG_SPI_BASE_ADDRESS               0xFEC10000ul
// #define BLDCFG_WATCHDOG_TIMER_BASE            0xFEC000F0ul
// #define BLDCFG_HPET_BASE_ADDRESS              0xFED00000ul
// #define BLDCFG_SMI_CMD_PORT_ADDRESS           0xB0
// #define BLDCFG_ACPI_PMA_BLK_ADDRESS           0xFE00
// #define BLDCFG_ROM_BASE_ADDRESS               0xFED61000ul
// #define BLDCFG_AZALIA_SSID                    0x780D1022ul
// #define BLDCFG_SMBUS_SSID                     0x780B1022ul
// #define BLDCFG_IDE_SSID                       0x780C1022ul
// #define BLDCFG_SATA_AHCI_SSID                 0x78011022ul
// #define BLDCFG_SATA_IDE_SSID                  0x78001022ul
// #define BLDCFG_SATA_RAID5_SSID                0x78031022ul
// #define BLDCFG_SATA_RAID_SSID                 0x78021022ul
// #define BLDCFG_EHCI_SSID                      0x78081022ul
// #define BLDCFG_OHCI_SSID                      0x78071022ul
// #define BLDCFG_LPC_SSID                       0x780E1022ul
// #define BLDCFG_SD_SSID                        0x78061022ul
// #define BLDCFG_XHCI_SSID                      0x78121022ul
// #define BLDCFG_FCH_PORT80_BEHIND_PCIB         FALSE
// #define BLDCFG_FCH_ENABLE_ACPI_SLEEP_TRAP     TRUE
// #define BLDCFG_FCH_GPP_LINK_CONFIG            PortA4
// #define BLDCFG_FCH_GPP_PORT0_PRESENT          FALSE
// #define BLDCFG_FCH_GPP_PORT1_PRESENT          FALSE
// #define BLDCFG_FCH_GPP_PORT2_PRESENT          FALSE
// #define BLDCFG_FCH_GPP_PORT3_PRESENT          FALSE
// #define BLDCFG_FCH_GPP_PORT0_HOTPLUG          FALSE
// #define BLDCFG_FCH_GPP_PORT1_HOTPLUG          FALSE
// #define BLDCFG_FCH_GPP_PORT2_HOTPLUG          FALSE
// #define BLDCFG_FCH_GPP_PORT3_HOTPLUG          FALSE

CONST AP_MTRR_SETTINGS ROMDATA KaveriApMtrrSettingsList[] =
{
  { AMD_AP_MTRR_FIX64k_00000, 0x1E1E1E1E1E1E1E1E },
  { AMD_AP_MTRR_FIX16k_80000, 0x1E1E1E1E1E1E1E1E },
  { AMD_AP_MTRR_FIX16k_A0000, 0x0000000000000000 },
  { AMD_AP_MTRR_FIX4k_C0000, 0x0000000000000000 },
  { AMD_AP_MTRR_FIX4k_C8000, 0x0000000000000000 },
  { AMD_AP_MTRR_FIX4k_D0000, 0x0000000000000000 },
  { AMD_AP_MTRR_FIX4k_D8000, 0x0000000000000000 },
  { AMD_AP_MTRR_FIX4k_E0000, 0x1818181818181818 },
  { AMD_AP_MTRR_FIX4k_E8000, 0x1818181818181818 },
  { AMD_AP_MTRR_FIX4k_F0000, 0x1818181818181818 },
  { AMD_AP_MTRR_FIX4k_F8000, 0x1818181818181818 },
  { CPU_LIST_TERMINAL }
};

#define BLDCFG_AP_MTRR_SETTINGS_LIST &KaveriApMtrrSettingsList



#include  "KaveriFp3Install.h"



/*----------------------------------------------------------------------------------------

 *                        CUSTOMER OVERIDES MEMORY TABLE

 *----------------------------------------------------------------------------------------

 */



/*

 *  Platform Specific Overriding Table allows IBV/OEM to pass in platform information to AGESA

 *  (e.g. MemClk routing, the number of DIMM slots per channel,...). If PlatformSpecificTable

 *  is populated, AGESA will base its settings on the data from the table. Otherwise, it will

 *  use its default conservative settings.

 */

CONST PSO_ENTRY ROMDATA DefaultPlatformMemoryConfiguration[] = {

  //

  // The following macros are supported (use comma to separate macros):

  //

  // MEMCLK_DIS_MAP(SocketID, ChannelID, MemClkDisBit0CSMap,..., MemClkDisBit7CSMap)

  //      The MemClk pins are identified based on BKDG definition of Fn2x88[MemClkDis] bitmap.

  //      AGESA will base on this value to disable unused MemClk to save power.

  //      Example:

  //      BKDG definition of Fn2x88[MemClkDis] bitmap for AM3 package is like below:

  //           Bit AM3/S1g3 pin name

  //           0   M[B,A]_CLK_H/L[0]

  //           1   M[B,A]_CLK_H/L[1]

  //           2   M[B,A]_CLK_H/L[2]

  //           3   M[B,A]_CLK_H/L[3]

  //           4   M[B,A]_CLK_H/L[4]

  //           5   M[B,A]_CLK_H/L[5]

  //           6   M[B,A]_CLK_H/L[6]

  //           7   M[B,A]_CLK_H/L[7]

  //      And platform has the following routing:

  //           CS0   M[B,A]_CLK_H/L[4]

  //           CS1   M[B,A]_CLK_H/L[2]

  //           CS2   M[B,A]_CLK_H/L[3]

  //           CS3   M[B,A]_CLK_H/L[5]

  //      Then platform can specify the following macro:

  //      MEMCLK_DIS_MAP(ANY_SOCKET, ANY_CHANNEL, 0x00, 0x00, 0x02, 0x04, 0x01, 0x08, 0x00, 0x00)

  //

  // CKE_TRI_MAP(SocketID, ChannelID, CKETriBit0CSMap, CKETriBit1CSMap,..., CKETriBit3CSMap)

  //      The CKE pins are identified based on BKDG definition of Fn2x9C_0C[CKETri] bitmap.

  //      AGESA will base on this value to tristate unused CKE to save power.

  //

  // ODT_TRI_MAP(SocketID, ChannelID, ODTTriBit0CSMap,..., ODTTriBit3CSMap)

  //      The ODT pins are identified based on BKDG definition of Fn2x9C_0C[ODTTri] bitmap.

  //      AGESA will base on this value to tristate unused ODT pins to save power.

  //

  // CS_TRI_MAP(SocketID, ChannelID, CSTriBit0CSMap,..., CSTriBit7CSMap)

  //      The Chip select pins are identified based on BKDG definition of Fn2x9C_0C[ChipSelTri] bitmap.

  //      AGESA will base on this value to tristate unused Chip select to save power.

  //

  // NUMBER_OF_DIMMS_SUPPORTED(SocketID, ChannelID, NumberOfDimmSlotsPerChannel)

  //      Specifies the number of DIMMs (DRAM devices) per channel.

  //      Note: The "NumberOfDimmSlotsPerChannel" parameter includes both slotted and soldered-down DIMMs

  //

  // NUMBER_OF_CHIP_SELECTS_SUPPORTED(SocketID, ChannelID, NumberOfChipSelectsPerChannel)

  //      Specifies the number of Chip selects per channel.

  //

  // NUMBER_OF_CHANNELS_SUPPORTED(SocketID, NumberOfChannelsPerSocket)

  //      Specifies the number of channels per socket.

  //

  // OVERRIDE_DDR_BUS_SPEED(SocketID, ChannelID, USER_MEMORY_TIMING_MODE, MEMORY_BUS_SPEED)

  //      Specifies DDR bus speed of channel ChannelID on socket SocketID.

  //

  // DRAM_TECHNOLOGY(SocketID, TECHNOLOGY_TYPE)

  //      Specifies the DRAM technology type of socket SocketID (DDR2, DDR3,...)

  //

  // WRITE_LEVELING_SEED(SocketID, ChannelID, DimmID, Byte0Seed, Byte1Seed, Byte2Seed, Byte3Seed, Byte4Seed, Byte5Seed,

  //      Byte6Seed, Byte7Seed, ByteEccSeed)

  //      Specifies the write leveling seed for a channel of a socket.

  //

  // HW_RXEN_SEED(SocketID, ChannelID, DimmID, Byte0Seed, Byte1Seed, Byte2Seed, Byte3Seed, Byte4Seed, Byte5Seed,

  //      Byte6Seed, Byte7Seed, ByteEccSeed)

  //      Speicifes the HW RXEN training seed for a channel of a socket

  //

  NUMBER_OF_DIMMS_SUPPORTED (ANY_SOCKET, ANY_CHANNEL, 1),

  NUMBER_OF_CHANNELS_SUPPORTED (ANY_SOCKET, 2),

  // Signal routing for AMD reference board

  MEMCLK_DIS_MAP (ANY_SOCKET, ANY_CHANNEL, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),

  CKE_TRI_MAP (ANY_SOCKET, ANY_CHANNEL, 0x05, 0x0A, 0xFF, 0xFF),

  ODT_TRI_MAP (ANY_SOCKET, ANY_CHANNEL, 0x01, 0x02, 0x00, 0x00),

  CS_TRI_MAP (ANY_SOCKET, ANY_CHANNEL, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),

  // CPUID KV
  CPU_FAMILY_TO_OVERRIDE (0x00630F00),
  NUMBER_OF_DIMMS_SUPPORTED (ANY_SOCKET, ANY_CHANNEL, 2),
  NUMBER_OF_CHANNELS_SUPPORTED (ANY_SOCKET, 2),
  MEMCLK_DIS_MAP (ANY_SOCKET, ANY_CHANNEL, 0x01, 0x02, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00),
  CKE_TRI_MAP (ANY_SOCKET, ANY_CHANNEL, 0x05, 0x0A, 0xFF, 0xFF),
  ODT_TRI_MAP (ANY_SOCKET, ANY_CHANNEL, 0x01, 0x04, 0x02, 0x08),
  CS_TRI_MAP (ANY_SOCKET, ANY_CHANNEL, 0x01, 0x02, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00),

  PSO_END

};



/*

 * These tables are optional and may be used to adjust memory timing settings

 */

#include "mm.h"

#include "mn.h"



// Customer table

UINT8 AGESA_MEM_TABLE_KV[][sizeof (MEM_TABLE_ALIAS)] =

{

 // Hardcoded Memory Training Values



 // The following macro should be used to override training values for your platform

 //

 // DQSACCESS(MTAfterDqsRwPosTrn, MTNodes, MTDcts, MTDIMMs, BFRdDqsDly, MTOverride, 0x00, 0x04, 0x08, 0x0c, 0x10, 0x14, 0x18, 0x1c, 0x20),

 //

 //   NOTE:

 //   The following training hardcode values are example values that were taken from a tilapia motherboard

 //   with a particular DIMM configuration.  To hardcode your own values, uncomment the appropriate line in

 //   the table and replace the byte lane values with your own.

 //

 //                                                                               ------------------ BYTE LANES ----------------------

 //                                                                                BL0   BL1   BL2   BL3   BL4   BL5   BL6   Bl7   ECC

 // Write Data Timing

 // DQSACCESS(MTAfterHwWLTrnP2, MTNode0, MTDct0, MTDIMM0, BFWrDatDly, MTOverride, 0x1D, 0x20, 0x26, 0x2B, 0x37, 0x3A, 0x3e, 0x3F, 0x30),// DCT0, DIMM0

 // DQSACCESS(MTAfterHwWLTrnP2, MTNode0, MTDct0, MTDIMM1, BFWrDatDly, MTOverride, 0x1D, 0x00, 0x06, 0x0B, 0x17, 0x1A, 0x1E, 0x1F, 0x10),// DCT0, DIMM1

 // DQSACCESS(MTAfterHwWLTrnP2, MTNode0, MTDct1, MTDIMM0, BFWrDatDly, MTOverride, 0x18, 0x1D, 0x27, 0x2B, 0x3B, 0x3B, 0x3E, 0x3E, 0x30),// DCT1, DIMM0

 // DQSACCESS(MTAfterHwWLTrnP2, MTNode0, MTDct1, MTDIMM1, BFWrDatDly, MTOverride, 0x18, 0x1D, 0x1C, 0x0B, 0x17, 0x1A, 0x1D, 0x1C, 0x10),// DCT1, DIMM1



 // DQS Receiver Enable

 // DQSACCESS(MTAfterSwRxEnTrn, MTNode0, MTDct0, MTDIMM0, BFRcvEnDly, MTOverride, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),// DCT0, DIMM0

 // DQSACCESS(MTAfterSwRxEnTrn, MTNode0, MTDct0, MTDIMM1, BFRcvEnDly, MTOverride, 0x7C, 0x7D, 0x7E, 0x81, 0x88, 0x8F, 0x96, 0x9F, 0x84),// DCT0, DIMM1

 // DQSACCESS(MTAfterSwRxEnTrn, MTNode0, MTDct1, MTDIMM0, BFRcvEnDly, MTOverride, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),// DCT1, DIMM0

 // DQSACCESS(MTAfterSwRxEnTrn, MTNode0, MTDct1, MTDIMM1, BFRcvEnDly, MTOverride, 0x1C, 0x1D, 0x1E, 0x01, 0x08, 0x0F, 0x16, 0x1F, 0x04),// DCT1, DIMM1



 // Write DQS Delays

 // DQSACCESS(MTAfterDqsRwPosTrn, MTNode0, MTDct0, MTDIMM0, BFWrDqsDly, MTOverride, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),// DCT0, DIMM0

 // DQSACCESS(MTAfterDqsRwPosTrn, MTNode0, MTDct0, MTDIMM1, BFWrDqsDly, MTOverride, 0x06, 0x0D, 0x12, 0x1A, 0x25, 0x28, 0x2C, 0x2C, 0x44),// DCT0, DIMM1

 // DQSACCESS(MTAfterDqsRwPosTrn, MTNode0, MTDct1, MTDIMM0, BFWrDqsDly, MTOverride, 0x07, 0x0E, 0x14, 0x1B, 0x24, 0x29, 0x2B, 0x2C, 0x1F),// DCT1, DIMM0

 // DQSACCESS(MTAfterDqsRwPosTrn, MTNode0, MTDct1, MTDIMM1, BFWrDqsDly, MTOverride, 0x07, 0x0C, 0x14, 0x19, 0x25, 0x28, 0x2B, 0x2B, 0x1A),// DCT1, DIMM1



 // Read DQS Delays

 // DQSACCESS(MTAfterDqsRwPosTrn, MTNode0, MTDct0, MTDIMM0, BFRdDqsDly, MTOverride, 0x10, 0x10, 0x0E, 0x10, 0x10, 0x10, 0x10, 0x0E, 0x10),// DCT0, DIMM0

 // DQSACCESS(MTAfterDqsRwPosTrn, MTNode0, MTDct0, MTDIMM1, BFRdDqsDly, MTOverride, 0x10, 0x10, 0x0E, 0x10, 0x10, 0x10, 0x10, 0x1E, 0x10),// DCT0, DIMM1

 // DQSACCESS(MTAfterDqsRwPosTrn, MTNode0, MTDct1, MTDIMM0, BFRdDqsDly, MTOverride, 0x10, 0x10, 0x0E, 0x10, 0x10, 0x10, 0x10, 0x1E, 0x10),// DCT1, DIMM0

 // DQSACCESS(MTAfterDqsRwPosTrn, MTNode0, MTDct1, MTDIMM1, BFRdDqsDly, MTOverride, 0x10, 0x10, 0x0E, 0x10, 0x10, 0x10, 0x10, 0x1E, 0x10),// DCT1, DIMM1

 //--------------------------------------------------------------------------------------------------------------------------------------------------

 // TABLE END

  NBACCESS (MTEnd, 0,  0, 0, 0, 0),      // End of Table

};

UINT8 SizeOfTableKV = sizeof (AGESA_MEM_TABLE_KV) / sizeof (AGESA_MEM_TABLE_KV[0]);



/* ***************************************************************************

 *   Optional User code to be included into the AGESA build

 *    These may be 32-bit call-out routines...

 */

//AGESA_STATUS

//AgesaReadSpd (

//  IN        UINTN                 FcnData,

//  IN OUT    AGESA_READ_SPD_PARAMS *ReadSpd

//  )

//{

//  /* platform code to read an SPD...  */

//  return Status;

//}


