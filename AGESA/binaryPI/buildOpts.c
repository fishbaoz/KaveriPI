/*****************************************************************************
 *
 * Copyright (C) 2008 - 2014, Advanced Micro Devices, Inc.
 *               2013 - 2014, Sage Electronic Engineering, LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Advanced Micro Devices, Inc. nor the names of
 *       its contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ADVANCED MICRO DEVICES, INC. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * AMD User options selection for a Family 15 Model 30 platform solution system
 *
 * This file is placed in the user's platform directory and contains the
 * build option selections desired for that platform.
 *
 * For Information about this file, see @ref platforminstall.
 */

#include "Porting.h"
#include "AGESA.h"
#include "Ids.h"
//#include "CommonReturns.h"
#include "Filecode.h"
#define FILECODE PLATFORM_SPECIFIC_OPTIONS_FILECODE

/*
 * Getting something that works in C and CPP for an arg that may or may
 * not be defined is tricky.  Here, if we have "#define CONFIG_BOOGER 1"
 * we match on the placeholder define, insert the "0," for arg1 and generate
 * the triplet (0, 1, 0).  Then the last step cherry picks the 2nd arg (a one).
 * When CONFIG_BOOGER is not defined, we generate a (... 1, 0) pair, and when
 * the last step cherry picks the 2nd arg, we get a zero.
 */
#define __ARG_PLACEHOLDER_1 0,
#define config_enabled(cfg) _config_enabled(cfg)
#define _config_enabled(value) __config_enabled(__ARG_PLACEHOLDER_##value)
#define __config_enabled(arg1_or_junk) ___config_enabled(arg1_or_junk 1, 0, 0)
#define ___config_enabled(__ignored, val, ...) val

#define IS_ENABLED(option) config_enabled(option)

/*  Select the cpu family.  */
#define INSTALL_FAMILY_10_SUPPORT FALSE
#define INSTALL_FAMILY_12_SUPPORT FALSE
#define INSTALL_FAMILY_14_SUPPORT FALSE


/*  Select the cpu socket type.  */
#define INSTALL_G34_SOCKET_SUPPORT  FALSE
#define INSTALL_C32_SOCKET_SUPPORT  FALSE
#define INSTALL_S1G3_SOCKET_SUPPORT FALSE
#define INSTALL_S1G4_SOCKET_SUPPORT FALSE
#define INSTALL_ASB2_SOCKET_SUPPORT FALSE
#define INSTALL_FS1_SOCKET_SUPPORT  FALSE
#define INSTALL_FM1_SOCKET_SUPPORT  FALSE
#define INSTALL_FP2_SOCKET_SUPPORT  FALSE
#define INSTALL_FT1_SOCKET_SUPPORT  FALSE
#define INSTALL_AM3_SOCKET_SUPPORT  FALSE
//INSTALL_FS1_SOCKET_SUPPORT
//INSTALL_FS1_SOCKET_SUPPORT
#define INSTALL_FM2_SOCKET_SUPPORT  FALSE
#define INSTALL_FM2r2_SOCKET_SUPPORT FALSE
#define INSTALL_FP3_SOCKET_SUPPORT  TRUE

#define INSTALL_FAMILY_15_MODEL_1x_SUPPORT FALSE
#define INSTALL_FAMILY_15_MODEL_3x_SUPPORT TRUE

#ifdef BLDOPT_REMOVE_FAMILY_15_MODEL_1x_SUPPORT
  #if BLDOPT_REMOVE_FAMILY_15_MODEL_1x_SUPPORT == TRUE
    #undef INSTALL_FAMILY_15_MODEL_1x_SUPPORT
    #define INSTALL_FAMILY_15_MODEL_1x_SUPPORT     FALSE
  #endif
#endif

#ifdef BLDOPT_REMOVE_FAMILY_15_MODEL_3x_SUPPORT
  #if BLDOPT_REMOVE_FAMILY_15_MODEL_3x_SUPPORT == TRUE
    #undef INSTALL_FAMILY_15_MODEL_3x_SUPPORT
    #define INSTALL_FAMILY_15_MODEL_3x_SUPPORT     FALSE
  #endif
#endif

#define IDSOPT_IDS_ENABLED                 TRUE
#define IDSOPT_TRACING_ENABLED             TRUE
#define IDSOPT_TRACING_CONSOLE_SERIALPORT  TRUE
#define IDSOPT_SERIAL_PORT                 3F8

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
//#define BLDOPT_REMOVE_SRAT                     TRUE
//#define BLDOPT_REMOVE_SLIT                     TRUE
//#define BLDOPT_REMOVE_WHEA                     TRUE
//#define BLDOPT_REMOVE_CDIT                     TRUE
//#define BLDOPT_REMOVE_DMI                      TRUE
#define BLDOPT_REMOVE_EARLY_SAMPLES            TRUE
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

#define BLDCFG_PCI_MMIO_BASE	0xF8000000
#define BLDCFG_PCI_MMIO_SIZE	64

/* Build configuration values here.
 */
// Specify the default values for the VRM controlling the VDD plane.
#define BLDCFG_VRM_CURRENT_LIMIT                 55000
#define BLDCFG_VRM_LOW_POWER_THRESHOLD           0
#define BLDCFG_VRM_MAXIMUM_CURRENT_LIMIT         80000
#define BLDCFG_PLAT_NUM_IO_APICS                 3
#define BLDCFG_GNB_IOAPIC_ADDRESS                0xFEC20000
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

#define BLDCFG_AMD_PLATFORM_TYPE                  AMD_PLATFORM_DESKTOP

#define BLDCFG_DIMM_TYPE_USED_IN_MIXED_CONFIG     DDR3_TECHNOLOGY
#define BLDCFG_MEMORY_BUS_FREQUENCY_LIMIT         DDR2133_FREQUENCY
#define BLDCFG_MEMORY_MODE_UNGANGED               TRUE
#define BLDCFG_MEMORY_QUAD_RANK_CAPABLE           FALSE
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
#define BLDCFG_ECC_REDIRECTION                    TRUE
#define BLDCFG_SCRUB_DRAM_RATE                    0
#define BLDCFG_SCRUB_L2_RATE                      0
#define BLDCFG_SCRUB_L3_RATE                      0
#define BLDCFG_SCRUB_IC_RATE                      0
#define BLDCFG_SCRUB_DC_RATE                      0
#define BLDCFG_ECC_SYNC_FLOOD                     FALSE
#define BLDCFG_ECC_SYMBOL_SIZE                    4
#define BLDCFG_HEAP_DRAM_ADDRESS                  0xB0000ul
#define BLDCFG_1GB_ALIGN                          FALSE
#define BLDCFG_VRM_HIGH_SPEED_ENABLE              TRUE
#define BLDCFG_PLATFORM_CSTATE_IO_BASE_ADDRESS    0x1770
#define BLDCFG_PMU_TRAINING_MODE                  PMU_TRAIN_1D
#define BLDOPT_REMOVE_ALIB                        FALSE
#define BLDCFG_CFG_LCD_BACK_LIGHT_CONTROL         200
#define BLDCFG_CFG_ABM_SUPPORT                    FALSE

// Specify the default values for the VRM controlling the VDDNB plane.
// If not specified, the values used for the core VRM will be applied
#define BLDCFG_VRM_NB_CURRENT_LIMIT               0xc350 //0xa028  // Not currently used on Trinity
#define BLDCFG_VRM_NB_LOW_POWER_THRESHOLD         0 //0x4e20  // Zero - disable NBPSI_L, Non-zero - enable NBPSI_L
#define BLDCFG_VRM_NB_MAXIMUM_CURRENT_LIMIT      0xfde8 //0xea60
#define BLDCFG_VRM_NB_SVI_OCP_LEVEL		 0x17700
#define BLDCFG_VRM_NB_SLEW_RATE                  0x1388 //0x2710 //  5000 // Used in calculating the VSRampSlamTime
#define BLDCFG_VRM_NB_ADDITIONAL_DELAY            0  // Not currently used on Trinity
//#define BLDCFG_VRM_NB_HIGH_SPEED_ENABLE           0  // Not currently used on Trinity
//#define BLDCFG_VRM_NB_INRUSH_CURRENT_LIMIT        0  // Not currently used on Trinity

//#define BLDCFG_VRM_NB_CURRENT_LIMIT               60000

#define BLDCFG_UMA_ALIGNMENT                      UMA_4MB_ALIGNED
#define BLDCFG_UMA_ALLOCATION_MODE                UMA_AUTO
//#define BLDCFG_UMA_ALLOCATION_SIZE              0x1000//0x1800//0x1000 /* (1000 << 16) = 256M*/
//#define BLDCFG_UMA_ALLOCATION_SIZE                0x0400//64M
#define BLDCFG_UMA_ABOVE4G_SUPPORT                FALSE
#define BLDCFG_CFG_GNB_HD_AUDIO                   TRUE
//#define BLDCFG_IGPU_SUBSYSTEM_ID                  OEM_IGPU_SSID
//#define BLDCFG_IGPU_HD_AUDIO_SUBSYSTEM_ID         OEM_IGPU_HD_AUDIO_SSID
//#define BLFCFG_APU_PCIE_PORTS_SUBSYSTEM_ID        OEM_APU_PCIE_PORTS_SSID

#define BLDCFG_PLATFORM_CPB_MODE                  CpbModeAuto
#define BLDCFG_PROCESSOR_SCOPE_NAME0              'P'
#define BLDCFG_PROCESSOR_SCOPE_NAME1              '0'
#define BLDCFG_PLATFORM_CSTATE_MODE               CStateModeC6       // ENH395294
//#define BLDCFG_PLATFORM_CSTATE_OPDATA             0

#define BLDCFG_IOMMU_SUPPORT                      TRUE

/*
 *   Process the options...
 */
#define AGESA_ENTRY_INIT_RESET                    TRUE
#define AGESA_ENTRY_INIT_RECOVERY                 FALSE
#define AGESA_ENTRY_INIT_EARLY                    TRUE
#define AGESA_ENTRY_INIT_POST                     TRUE
#define AGESA_ENTRY_INIT_ENV                      TRUE
#define AGESA_ENTRY_INIT_MID                      TRUE
#define AGESA_ENTRY_INIT_LATE                     TRUE
#define AGESA_ENTRY_INIT_S3SAVE                   TRUE
#define AGESA_ENTRY_INIT_RESUME                   TRUE
#define AGESA_ENTRY_INIT_LATE_RESTORE             TRUE
#define AGESA_ENTRY_INIT_FINAL_RESTORE            FALSE
#define AGESA_ENTRY_INIT_GENERAL_SERVICES         TRUE

/*
 * Customized OEM build configurations for FCH component
 */
#define DFLT_SMBUS0_BASE_ADDRESS            0xB00
#define DFLT_SMBUS1_BASE_ADDRESS            0xB20
#define DFLT_SIO_PME_BASE_ADDRESS           0xE00
#define DFLT_ACPI_PM1_EVT_BLOCK_ADDRESS     0x800
#define DFLT_ACPI_PM1_CNT_BLOCK_ADDRESS     0x804
#define DFLT_ACPI_PM_TMR_BLOCK_ADDRESS      0x808
#define DFLT_ACPI_CPU_CNT_BLOCK_ADDRESS     0x810
#define DFLT_ACPI_GPE0_BLOCK_ADDRESS        0x820
#define DFLT_SPI_BASE_ADDRESS               0xFEC10000
#define DFLT_WATCHDOG_TIMER_BASE_ADDRESS    0xFEC000F0
#define DFLT_HPET_BASE_ADDRESS              0xFED00000
#define DFLT_SMI_CMD_PORT                   0xB0
#define DFLT_ACPI_PMA_CNT_BLK_ADDRESS       0xFE00
#define DFLT_GEC_BASE_ADDRESS               0xFED61000
#define DFLT_AZALIA_SSID                    0x780D1022
#define DFLT_SMBUS_SSID                     0x780B1022
#define DFLT_IDE_SSID                       0x780C1022
#define DFLT_SATA_AHCI_SSID                 0x78011022
#define DFLT_SATA_IDE_SSID                  0x78001022
#define DFLT_SATA_RAID5_SSID                0x78031022
#define DFLT_SATA_RAID_SSID                 0x78021022
#define DFLT_EHCI_SSID                      0x78081022
#define DFLT_OHCI_SSID                      0x78071022
#define DFLT_LPC_SSID                       0x780E1022
#define DFLT_SD_SSID                        0x78061022
#define DFLT_XHCI_SSID                      0x78121022
#define DFLT_FCH_PORT80_BEHIND_PCIB         FALSE
#define DFLT_FCH_ENABLE_ACPI_SLEEP_TRAP     TRUE
#define DFLT_FCH_GPP_LINK_CONFIG            PortA1B1C1D1
#define DFLT_FCH_GPP_PORT0_PRESENT          TRUE
#define DFLT_FCH_GPP_PORT1_PRESENT          TRUE
#define DFLT_FCH_GPP_PORT2_PRESENT          TRUE
#define DFLT_FCH_GPP_PORT3_PRESENT          TRUE
#define DFLT_FCH_GPP_PORT0_HOTPLUG          FALSE
#define DFLT_FCH_GPP_PORT1_HOTPLUG          FALSE
#define DFLT_FCH_GPP_PORT2_HOTPLUG          FALSE
#define DFLT_FCH_GPP_PORT3_HOTPLUG          FALSE
//#define BLDCFG_IR_PIN_CONTROL	0x33

CONST AP_MTRR_SETTINGS ROMDATA ApMtrrSettingsList[] =
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

#define BLDCFG_AP_MTRR_SETTINGS_LIST &ApMtrrSettingsList

/* MEMORY_BUS_SPEED */
#define DDR400_FREQUENCY   200     ///< DDR 400
#define DDR533_FREQUENCY   266     ///< DDR 533
#define DDR667_FREQUENCY   333     ///< DDR 667
#define DDR800_FREQUENCY   400     ///< DDR 800
#define DDR1066_FREQUENCY   533    ///< DDR 1066
#define DDR1333_FREQUENCY   667    ///< DDR 1333
#define DDR1600_FREQUENCY   800    ///< DDR 1600
#define DDR1866_FREQUENCY   933    ///< DDR 1866
#define DDR2100_FREQUENCY   1050   ///< DDR 2100
#define DDR2133_FREQUENCY   1066   ///< DDR 2133
#define DDR2400_FREQUENCY   1200   ///< DDR 2400
#define UNSUPPORTED_DDR_FREQUENCY	1201 ///< Highest limit of DDR frequency

/* QUANDRANK_TYPE*/
#define QUADRANK_REGISTERED				0 ///< Quadrank registered DIMM
#define QUADRANK_UNBUFFERED				1 ///< Quadrank unbuffered DIMM

/* USER_MEMORY_TIMING_MODE */
#define TIMING_MODE_AUTO				0 ///< Use best rate possible
#define TIMING_MODE_LIMITED				1 ///< Set user top limit
#define TIMING_MODE_SPECIFIC			2 ///< Set user specified speed

/* POWER_DOWN_MODE */
#define POWER_DOWN_BY_CHANNEL			0 ///< Channel power down mode
#define POWER_DOWN_BY_CHIP_SELECT		1 ///< Chip select power down mode

GPIO_CONTROL   gpio[] = {
	#if !PCIE_DP4_MUX
	{50, Function0, 0x08},
	#endif
	{166, Function0, 0x18},	/* TODO: pull up. */
	{-1}
};
#define BLDCFG_FCH_GPIO_CONTROL_LIST           (gpio)

#include "FieldAccessors.h"
#if INSTALL_FM2r2_SOCKET_SUPPORT || INSTALL_FM2_SOCKET_SUPPORT
#include  "KaveriFm2r2Install.h"
#else
#include  "KaveriFp3Install.h"
#endif

BOOLEAN
IdsErrorStop (
  IN       UINT32            filecode
  )
{
	IDS_DEADLOOP();
	return FALSE;
}

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

   NUMBER_OF_DIMMS_SUPPORTED (ANY_SOCKET, ANY_CHANNEL, 2),
   NUMBER_OF_CHANNELS_SUPPORTED (ANY_SOCKET, 2),

//   // Signal routing for AMD reference board
//   MEMCLK_DIS_MAP (ANY_SOCKET, ANY_CHANNEL, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),
//   CKE_TRI_MAP (ANY_SOCKET, ANY_CHANNEL, 0x05, 0x0A, 0xFF, 0xFF),
//   ODT_TRI_MAP (ANY_SOCKET, ANY_CHANNEL, 0x01, 0x02, 0x00, 0x00),
//   CS_TRI_MAP (ANY_SOCKET, ANY_CHANNEL, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),

   // CPUID KV
//   CPU_FAMILY_TO_OVERRIDE (0x00630F01),
   MEMCLK_DIS_MAP (ANY_SOCKET, ANY_CHANNEL, 0x04, 0x01, 0x02, 0x08, 0x00, 0x00, 0x00, 0x00),
   CKE_TRI_MAP (ANY_SOCKET, ANY_CHANNEL, 0x01, 0x02, 0x04, 0x08),
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
