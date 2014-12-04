/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD User options selection for a Kaveri FP3 platform solution system
 *
 * This file is placed in the user's platform directory and contains the
 * build option selections desired for that platform.
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
#include "Filecode.h"
#define FILECODE PLATFORM_SPECIFIC_OPTIONS_FILECODE


/* User makes option selections here
 * Comment out the items wanted to be included in the build.
 * Uncomment those items you with to REMOVE from the build.
 */
//#define BLDOPT_REMOVE_UDIMMS_SUPPORT           FALSE
#define BLDOPT_REMOVE_RDIMMS_SUPPORT           TRUE
#define BLDOPT_REMOVE_LRDIMMS_SUPPORT          TRUE
//#define BLDOPT_REMOVE_ECC_SUPPORT              TRUE
//#define BLDOPT_REMOVE_BANK_INTERLEAVE          TRUE
//#define BLDOPT_REMOVE_DCT_INTERLEAVE           TRUE
//#define BLDOPT_REMOVE_NODE_INTERLEAVE          TRUE
//#define BLDOPT_REMOVE_PARALLEL_TRAINING        TRUE
//#define BLDOPT_REMOVE_ONLINE_SPARE_SUPPORT     TRUE
//#define BLDOPT_REMOVE_MEM_RESTORE_SUPPORT      TRUE
//#define BLDOPT_REMOVE_MULTINODE_SUPPORT        TRUE
//#define BLDOPT_REMOVE_ACPI_PSTATES             TRUE
//#define BLDOPT_REMOVE_SRAT                     TRUE
//#define BLDOPT_REMOVE_SLIT                     TRUE
//#define BLDOPT_REMOVE_WHEA                     TRUE
//#define BLDOPT_REMOVE_DMI                      TRUE
//#define BLDOPT_REMOVE_DQS_TRAINING             TRUE
//#define BLDCFG_PCI_MMIO_SIZE                   256
//#define BLDCFG_PCI_MMIO_BASE                   0x00000000E0000000ull
//#define BLDOPT_REMOVE_GFX_RECOVERY             TRUE
#define BLDOPT_REMOVE_EARLY_SAMPLES            TRUE

/*  Process the options...
 * This file include MUST occur AFTER the user option selection settings
 */
#define AGESA_ENTRY_INIT_RESET                    TRUE
#define AGESA_ENTRY_INIT_RECOVERY                 TRUE
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
  // NUMBER_OF_DIMMS_SUPPORTED(SocketID, ChannelID, NumberOfDimmSlotsPerChannel)
  //      Specifies the number of DIMMs (DRAM devices) per channel.
  //      Note: The "NumberOfDimmSlotsPerChannel" parameter includes both slotted and soldered-down DIMMs
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
  PSO_END
};

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

