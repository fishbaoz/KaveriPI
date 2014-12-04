/* $NoKeywords:$ */
/**
 * @file
 *
 * Install of build options for a Kaveri FP3 platform solution
 *
 * This file generates the defaults tables for the "Kaveri FP3" platform solution
 * set of processors. The documented build options are imported from a user
 * controlled file for processing.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Core
 * @e \$Revision: 290557 $   @e \$Date: 2014-04-23 11:10:31 -0500 (Wed, 23 Apr 2014) $
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
 *
 ***************************************************************************/

#include "cpuRegisters.h"
#include "cpuFamRegisters.h"
#include "cpuFamilyTranslation.h"
#include "AdvancedApi.h"
#include "heapManager.h"
#include "CreateStruct.h"
#include "cpuFeatures.h"
#include "Table.h"
#include "CommonReturns.h"
#include "cpuEarlyInit.h"
#include "cpuLateInit.h"
#include "GnbInterface.h"

/*****************************************************************************
 *   Define the RELEASE VERSION string
 *
 * The Release Version string should identify the next planned release.
 * When a branch is made in preparation for a release, the release manager
 * should change/confirm that the branch version of this file contains the
 * string matching the desired version for the release. The trunk version of
 * the file should always contain a trailing 'X'. This will make sure that a
 * development build from trunk will not be confused for a released version.
 * The release manager will need to remove the trailing 'X' and update the
 * version string as appropriate for the release. The trunk copy of this file
 * should also be updated/incremented for the next expected version, + trailing 'X'
 ****************************************************************************/

/*
 * CRITICAL!
 *
 * This file is copied into the binaryPI directory to ensure that the modified
 * PlatformInstall.h is used rather than the original copy in the Includes
 * directory.
 */

// This is the delivery package title, "KaveriPI        "
                  // This string MUST be exactly 16 characters long
#define AGESA_PACKAGE_STRING  {'K', 'a', 'v', 'e', 'r', 'i', 'P', 'I', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}

                  // This is the release version number of the AGESA component
                  // This string MUST be exactly 12 characters long
#define AGESA_VERSION_STRING  {'V', '1', '.', '1', '.', '0', '.', '5', ' ', ' ', ' ', ' '}


// The Kaveri FP3 solution is defined to be family 0x15 models 0x30 - 0x3F in the FP3 sockets.
#define INSTALL_FP3_SOCKET_SUPPORT           TRUE
#define INSTALL_FAMILY_15_MODEL_3x_SUPPORT   TRUE

#ifdef BLDOPT_REMOVE_FAMILY_15_MODEL_3x_SUPPORT
  #if BLDOPT_REMOVE_FAMILY_15_MODEL_3x_SUPPORT == TRUE
    #undef INSTALL_FAMILY_15_MODEL_3x_SUPPORT
    #define INSTALL_FAMILY_15_MODEL_3x_SUPPORT     FALSE
  #endif
#endif

// The following definitions specify the default values for various parameters in which there are
// no clearly defined defaults to be used in the common file.  The values below are based on product
// and BKDG content, please consult the AGESA Memory team for consultation.
#define DFLT_SCRUB_DRAM_RATE            (0)
#define DFLT_SCRUB_L2_RATE              (0)
#define DFLT_SCRUB_L3_RATE              (0)
#define DFLT_SCRUB_IC_RATE              (0)
#define DFLT_SCRUB_DC_RATE              (0)
#define DFLT_MEMORY_QUADRANK_TYPE       QUADRANK_UNBUFFERED
#define DFLT_VRM_SLEW_RATE              (5000)


#define DFLT_SMBUS0_BASE_ADDRESS            0xB00
#define DFLT_SMBUS1_BASE_ADDRESS            0xB20
#define DFLT_SIO_PME_BASE_ADDRESS           0xE00
#define DFLT_ACPI_PM1_EVT_BLOCK_ADDRESS     0x400
#define DFLT_ACPI_PM1_CNT_BLOCK_ADDRESS     0x404
#define DFLT_ACPI_PM_TMR_BLOCK_ADDRESS      0x408
#define DFLT_ACPI_CPU_CNT_BLOCK_ADDRESS     0x410
#define DFLT_ACPI_GPE0_BLOCK_ADDRESS        0x420
#define DFLT_SPI_BASE_ADDRESS               0xFEC10000ul
#define DFLT_WATCHDOG_TIMER_BASE_ADDRESS    0xFEC000F0ul
#define DFLT_HPET_BASE_ADDRESS              0xFED00000ul
#define DFLT_SMI_CMD_PORT                   0xB0
#define DFLT_ACPI_PMA_CNT_BLK_ADDRESS       0xFE00
#define DFLT_GEC_BASE_ADDRESS               0xFED61000ul
#define DFLT_AZALIA_SSID                    0x780D1022ul
#define DFLT_SMBUS_SSID                     0x780B1022ul
#define DFLT_IDE_SSID                       0x780C1022ul
#define DFLT_SATA_AHCI_SSID                 0x78011022ul
#define DFLT_SATA_IDE_SSID                  0x78001022ul
#define DFLT_SATA_RAID5_SSID                0x78031022ul
#define DFLT_SATA_RAID_SSID                 0x78021022ul
#define DFLT_EHCI_SSID                      0x78081022ul
#define DFLT_OHCI_SSID                      0x78071022ul
#define DFLT_LPC_SSID                       0x780E1022ul
#define DFLT_SD_SSID                        0x78061022ul
#define DFLT_XHCI_SSID                      0x78121022ul
#define DFLT_FCH_PORT80_BEHIND_PCIB         FALSE
#define DFLT_FCH_ENABLE_ACPI_SLEEP_TRAP     TRUE
#define DFLT_FCH_GPP_LINK_CONFIG            PortA4
#define DFLT_FCH_GPP_PORT0_PRESENT          FALSE
#define DFLT_FCH_GPP_PORT1_PRESENT          FALSE
#define DFLT_FCH_GPP_PORT2_PRESENT          FALSE
#define DFLT_FCH_GPP_PORT3_PRESENT          FALSE
#define DFLT_FCH_GPP_PORT0_HOTPLUG          FALSE
#define DFLT_FCH_GPP_PORT1_HOTPLUG          FALSE
#define DFLT_FCH_GPP_PORT2_HOTPLUG          FALSE
#define DFLT_FCH_GPP_PORT3_HOTPLUG          FALSE
// Instantiate all solution relevant data.
#include <PlatformInstall.h>
