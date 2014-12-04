/* $NoKeywords:$ */
/**
 * @file
 *
 * Install of all appropriate CPU family specific support.
 *
 * This file generates the defaults tables for all family specific
 * combinations.
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
 *
 ***************************************************************************/

/*  Default all CPU Specific Service members to off.  They
    will be enabled as needed by cross referencing families
    with entry points in the family / model install files.  */
#define USES_REGISTER_TABLES        FALSE
#define BASE_FAMILY_PCI             FALSE
#define MODEL_SPECIFIC_PCI          FALSE
#define BASE_FAMILY_MSR             FALSE
#define MODEL_SPECIFIC_MSR          FALSE
#define BASE_FAMILY_HT_PCI          FALSE
#define MODEL_SPECIFIC_HT_PCI       FALSE
#define BASE_FAMILY_WORKAROUNDS     FALSE

/*
 * Pull in family specific services based on entry point
 */
#if AGESA_ENTRY_INIT_RESET == TRUE
#endif

#if AGESA_ENTRY_INIT_RECOVERY == TRUE
  #undef USES_REGISTER_TABLES
  #define USES_REGISTER_TABLES        TRUE
  #undef BASE_FAMILY_PCI
  #define BASE_FAMILY_PCI             TRUE
  #undef MODEL_SPECIFIC_PCI
  #define MODEL_SPECIFIC_PCI          TRUE
  #undef BASE_FAMILY_MSR
  #define BASE_FAMILY_MSR             TRUE
  #undef MODEL_SPECIFIC_MSR
  #define MODEL_SPECIFIC_MSR          TRUE
#endif

#if AGESA_ENTRY_INIT_EARLY == TRUE
  #undef USES_REGISTER_TABLES
  #define USES_REGISTER_TABLES        TRUE
  #undef BASE_FAMILY_PCI
  #define BASE_FAMILY_PCI             TRUE
  #undef MODEL_SPECIFIC_PCI
  #define MODEL_SPECIFIC_PCI          TRUE
  #undef BASE_FAMILY_MSR
  #define BASE_FAMILY_MSR             TRUE
  #undef MODEL_SPECIFIC_MSR
  #define MODEL_SPECIFIC_MSR          TRUE
  #undef BASE_FAMILY_HT_PCI
  #define BASE_FAMILY_HT_PCI          TRUE
  #undef MODEL_SPECIFIC_HT_PCI
  #define MODEL_SPECIFIC_HT_PCI       TRUE
  #undef BASE_FAMILY_WORKAROUNDS
  #define BASE_FAMILY_WORKAROUNDS     TRUE
#endif

#if AGESA_ENTRY_INIT_POST == TRUE
#endif

#if AGESA_ENTRY_INIT_ENV == TRUE
#endif

#if AGESA_ENTRY_INIT_MID == TRUE
#endif

#if AGESA_ENTRY_INIT_LATE == TRUE
#endif

#if AGESA_ENTRY_INIT_S3SAVE == TRUE
#endif

#if AGESA_ENTRY_INIT_RESUME == TRUE
#endif

#if AGESA_ENTRY_INIT_LATE_RESTORE == TRUE
#endif

#if AGESA_ENTRY_INIT_GENERAL_SERVICES == TRUE
#endif

/*
 * Initialize PCI MMIO mask to 0
 */
#define FAMILY_MMIO_BASE_MASK (0ull)


/*
 * Initialize all families to disabled
 */
#define OPT_F15_TABLE

#define OPT_F15_ID_TABLE


/*
 * Install family specific support
 */
#if (OPTION_FAMILY15H_TN == TRUE) || (OPTION_FAMILY15H_KV == TRUE)
  #include  "OptionFamily15hInstall.h"
#endif

/*
 * Process PCI MMIO mask
 */

// If size is 0, but base is not, break the build.
#if (CFG_PCI_MMIO_BASE != 0) && (CFG_PCI_MMIO_SIZE == 0)
  #error    BLDCFG: Invalid PCI MMIO size -- acceptable values are 1, 2, 4, 8, 16, 32, 64, 128, and 256
#endif

// If base is 0, but size is not, break the build.
#if (CFG_PCI_MMIO_BASE == 0) && (CFG_PCI_MMIO_SIZE != 0)
  #error    BLDCFG: Invalid PCI MMIO base -- must be 8MB or greater
#endif

#if (CFG_PCI_MMIO_BASE != 0) && (CFG_PCI_MMIO_SIZE != 0)
  // Both are non-zero, begin further processing.

  // Heap runs from 4MB to 8MB. Disallow any addresses below 8MB.
  #if (CFG_PCI_MMIO_BASE < 0x800000)
    #error    BLDCFG: Invalid PCI MMIO base -- must be 8MB or greater
  #endif

  // Break the build if the address is too high for the enabled families.
  #if ((CFG_PCI_MMIO_BASE & FAMILY_MMIO_BASE_MASK) != 0)
    #error       BLDCFG: Invalid PCI MMIO base address for the installed CPU families
  #endif

  // If the size parameter is not valid, break the build.
  #if (CFG_PCI_MMIO_SIZE != 1) && (CFG_PCI_MMIO_SIZE != 2) && (CFG_PCI_MMIO_SIZE != 4) && (CFG_PCI_MMIO_SIZE != 8) && (CFG_PCI_MMIO_SIZE != 16)
    #if (CFG_PCI_MMIO_SIZE != 32) && (CFG_PCI_MMIO_SIZE != 64) && (CFG_PCI_MMIO_SIZE != 128) && (CFG_PCI_MMIO_SIZE != 256)
      #error      BLDCFG: Invalid PCI MMIO size -- acceptable values are 1, 2, 4, 8, 16, 32, 64, 128, and 256
    #endif
  #endif

  #define PCI_MMIO_ALIGNMENT ((0x100000ul * CFG_PCI_MMIO_SIZE) - 1)
  // If the base is not aligned according to size, break the build.
  #if ((CFG_PCI_MMIO_BASE & PCI_MMIO_ALIGNMENT) != 0)
    #error      BLDCFG: Invalid PCI MMIO base -- must be properly aligned according to MMIO size
  #endif
  #undef PCI_MMIO_ALIGNMENT
#endif

/*
 * Process sockets / modules
 */
#ifndef ADVCFG_PLATFORM_SOCKETS
  #error BLDOPT Set Family supported sockets.
#endif
#ifndef ADVCFG_PLATFORM_MODULES
  #error BLDOPT Set Family supported modules.
#endif

CONST OPTIONS_CONFIG_TOPOLOGY ROMDATA TopologyConfiguration =
{
  ADVCFG_PLATFORM_SOCKETS,
  ADVCFG_PLATFORM_MODULES
};

/*
 * Instantiate global data needed for processor identification
 */
CONST CPU_SPECIFIC_SERVICES_XLAT ROMDATA CpuSupportedFamiliesArray[] =
{
  OPT_F15_TABLE
};

CONST CPU_FAMILY_SUPPORT_TABLE ROMDATA CpuSupportedFamiliesTable =
{
  (sizeof (CpuSupportedFamiliesArray) / sizeof (CPU_SPECIFIC_SERVICES_XLAT)),
  &CpuSupportedFamiliesArray[0]
};


CONST CPU_LOGICAL_ID_FAMILY_XLAT ROMDATA CpuSupportedFamilyIdArray[] =
{
  OPT_F15_ID_TABLE
};

CONST CPU_FAMILY_ID_XLAT_TABLE ROMDATA CpuSupportedFamilyIdTable =
{
  (sizeof (CpuSupportedFamilyIdArray) / sizeof (CPU_LOGICAL_ID_FAMILY_XLAT)),
  CpuSupportedFamilyIdArray
};
