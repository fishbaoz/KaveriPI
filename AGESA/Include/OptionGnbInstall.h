/* $NoKeywords:$ */
/**
 * @file
 *
 * Install of build option: GNB
 *
 * Contains AMD AGESA install macros and test conditions. Output is the
 * defaults tables reflecting the User's build options selection.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Options
 * @e \$Revision: 290085 $   @e \$Date: 2014-04-18 13:53:57 -0500 (Fri, 18 Apr 2014) $
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

#ifndef _OPTION_GNB_INSTALL_H_
#define _OPTION_GNB_INSTALL_H_

#include "S3SaveState.h"
/*  This option is designed to be included into the platform solution install
 *  file. The platform solution install file will define the options status.
 *  Check to validate the definition
 */

//---------------------------------------------------------------------------------------------------
// Family installation
//---------------------------------------------------------------------------------------------------



#define GNB_TYPE_TN   FALSE
#define GNB_TYPE_KV   FALSE

#if (OPTION_FAMILY15H_TN == TRUE)
  #undef  GNB_TYPE_TN
  #define GNB_TYPE_TN  TRUE
#endif

#if (OPTION_FAMILY15H_KV == TRUE)
  #undef  GNB_TYPE_KV
  #define GNB_TYPE_KV  TRUE
#endif

#if (GNB_TYPE_KV == TRUE || GNB_TYPE_TN == TRUE)
//---------------------------------------------------------------------------------------------------
// Service installation
//---------------------------------------------------------------------------------------------------

  #include "Gnb.h"
  #include "GnbPcie.h"
  #include "GnbGfx.h"

  #define SERVICES_POINTER  NULL
  #if (GNB_TYPE_TN == TRUE)
    #include "GnbInitTNInstall.h"
  #endif
  #if (GNB_TYPE_KV == TRUE)
    #include "GnbInitKVInstall.h"
  #endif
  GNB_SERVICE   *ServiceTable = SERVICES_POINTER;

//---------------------------------------------------------------------------------------------------
// Common BUILD options
// These Common Build options apply to all families
//---------------------------------------------------------------------------------------------------

  #ifndef CFG_IGFX_AS_PCIE_EP
    #define CFG_IGFX_AS_PCIE_EP                           TRUE
  #endif

  #ifndef CFG_GNB_PCIE_LINK_RECEIVER_DETECTION_POOLING
    #define CFG_GNB_PCIE_LINK_RECEIVER_DETECTION_POOLING  (60 * 1000)
  #endif

  #ifndef CFG_GNB_PCIE_LINK_L0_POOLING
    #define CFG_GNB_PCIE_LINK_L0_POOLING                  (60 * 1000)
  #endif

  #ifndef CFG_GNB_PCIE_LINK_GPIO_RESET_ASSERT_TIME
    #define CFG_GNB_PCIE_LINK_GPIO_RESET_ASSERT_TIME      (2 * 1000)
  #endif

  #ifndef CFG_GNB_PCIE_LINK_RESET_TO_TRAINING_TIME
    #define CFG_GNB_PCIE_LINK_RESET_TO_TRAINING_TIME      (2 * 1000)
  #endif

  #ifdef BLDCFG_PCIE_TRAINING_ALGORITHM
    #define CFG_GNB_PCIE_TRAINING_ALGORITHM               BLDCFG_PCIE_TRAINING_ALGORITHM
  #else
    #define CFG_GNB_PCIE_TRAINING_ALGORITHM               PcieTrainingStandard
  #endif

  #ifndef CFG_GNB_PCIE_POWERGATING_FLAGS
    #define CFG_GNB_PCIE_POWERGATING_FLAGS                0
  #endif

  #ifndef CFG_PCIE_ASPM_BLACK_LIST_ENABLE
    #define CFG_PCIE_ASPM_BLACK_LIST_ENABLE               TRUE
  #endif

  #ifndef CFG_MAX_PAYLOAD_ENABLE
    #define CFG_MAX_PAYLOAD_ENABLE                        TRUE
  #endif

  #ifndef CFG_GNB_IVRS_RELATIVE_ADDR_NAMES_SUPPORT
    #define CFG_GNB_IVRS_RELATIVE_ADDR_NAMES_SUPPORT      FALSE
  #endif

  #ifndef CFG_GNB_LOAD_REAL_FUSE
    #define  CFG_GNB_LOAD_REAL_FUSE                       TRUE
  #endif

  GNB_BUILD_OPTIONS ROMDATA GnbBuildOptions = {
    CFG_IGFX_AS_PCIE_EP,
    CFG_GNB_PCIE_LINK_RECEIVER_DETECTION_POOLING,
    CFG_GNB_PCIE_LINK_L0_POOLING,
    CFG_GNB_PCIE_LINK_GPIO_RESET_ASSERT_TIME,
    CFG_GNB_PCIE_LINK_RESET_TO_TRAINING_TIME,
    CFG_GNB_PCIE_TRAINING_ALGORITHM,
    CFG_GNB_PCIE_POWERGATING_FLAGS,
    CFG_PCIE_ASPM_BLACK_LIST_ENABLE,
    CFG_MAX_PAYLOAD_ENABLE,
    CFG_GNB_IVRS_RELATIVE_ADDR_NAMES_SUPPORT,
    CFG_ACPI_SET_OEM_ID,
    CFG_ACPI_SET_OEM_TABLE_ID,
    CFG_GNB_LOAD_REAL_FUSE
  };

//---------------------------------------------------------------------------------------------------
// Family-specific common BUILD options
// The build options are used in all families, but may be configured differently for different families
// CFG_XXXX will override default value if CFG_XXXX is defined
// CFG_XXXX_FM will override CFG_XXXX if CFG_XXXX_FM is defined (Where _FM is family, _TN, _KV, ...
// If neither CFG_XXXX_FM or CFG_XXXX is defined, then the family specific default will be defined
// within the family-specific definition areas below
//---------------------------------------------------------------------------------------------------

  #ifdef CFG_SCS_SUPPORT
    #ifndef CFG_SCS_SUPPORT_KV
      #define CFG_SCS_SUPPORT_KV                            CFG_SCS_SUPPORT
    #endif
  #endif

  #ifdef CFG_UMA_STEERING
    #ifndef CFG_UMA_STEERING_TN
      #define CFG_UMA_STEERING_TN                           CFG_UMA_STEERING
    #endif
    #ifndef CFG_UMA_STEERING_KV
      #define CFG_UMA_STEERING_KV                           CFG_UMA_STEERING
    #endif
  #endif

  #ifdef CFG_GMC_POWER_GATING
    #ifndef CFG_GMC_POWER_GATING_TN
      #define CFG_GMC_POWER_GATING_TN                       CFG_GMC_POWER_GATING
    #endif
    #ifndef CFG_GMC_POWER_GATING_KV
      #define CFG_GMC_POWER_GATING_KV                       CFG_GMC_POWER_GATING
    #endif
  #endif

  #ifdef CFG_GMC_CLOCK_GATING
    #ifndef CFG_GMC_CLOCK_GATING_TN
      #define CFG_GMC_CLOCK_GATING_TN                      CFG_GMC_CLOCK_GATING
    #endif
    #ifndef CFG_GMC_CLOCK_GATING_KV
      #define CFG_GMC_CLOCK_GATING_KV                      CFG_GMC_CLOCK_GATING
    #endif
  #endif

  #ifdef CFG_ORB_DYN_WAKE_ENABLE
    #ifndef CFG_ORB_DYN_WAKE_ENABLE_TN
      #define CFG_ORB_DYN_WAKE_ENABLE_TN                   CFG_ORB_DYN_WAKE_ENABLE
    #endif
    #ifndef CFG_ORB_DYN_WAKE_ENABLE_KV
      #define CFG_ORB_DYN_WAKE_ENABLE_KV                   CFG_ORB_DYN_WAKE_ENABLE
    #endif
  #endif

  #ifdef CFG_ORB_CLOCK_GATING_ENABLE
    #ifndef CFG_ORB_CLOCK_GATING_ENABLE_TN
      #define CFG_ORB_CLOCK_GATING_ENABLE_TN               CFG_ORB_CLOCK_GATING_ENABLE
    #endif
    #ifndef CFG_ORB_CLOCK_GATING_ENABLE_KV
      #define CFG_ORB_CLOCK_GATING_ENABLE_KV               CFG_ORB_CLOCK_GATING_ENABLE
    #endif
  #endif

  #ifdef CFG_IOMMU_L1_CLOCK_GATING_ENABLE
    #ifndef CFG_IOMMU_L1_CLOCK_GATING_ENABLE_TN
      #define CFG_IOMMU_L1_CLOCK_GATING_ENABLE_TN          CFG_IOMMU_L1_CLOCK_GATING_ENABLE
    #endif
    #ifndef CFG_IOMMU_L1_CLOCK_GATING_ENABLE_KV
      #define CFG_IOMMU_L1_CLOCK_GATING_ENABLE_KV          CFG_IOMMU_L1_CLOCK_GATING_ENABLE
    #endif
  #endif

  #ifdef CFG_IOMMU_L2_CLOCK_GATING_ENABLE
    #ifndef CFG_IOMMU_L2_CLOCK_GATING_ENABLE_TN
      #define CFG_IOMMU_L2_CLOCK_GATING_ENABLE_TN          CFG_IOMMU_L2_CLOCK_GATING_ENABLE
    #endif
    #ifndef CFG_IOMMU_L2_CLOCK_GATING_ENABLE_KV
      #define CFG_IOMMU_L2_CLOCK_GATING_ENABLE_KV          CFG_IOMMU_L2_CLOCK_GATING_ENABLE
    #endif
  #endif

  #ifdef CFG_LCLK_DEEP_SLEEP_EN
    #ifndef CFG_LCLK_DEEP_SLEEP_EN_TN
      #define CFG_LCLK_DEEP_SLEEP_EN_TN                    CFG_LCLK_DEEP_SLEEP_EN
    #endif
    #ifndef CFG_LCLK_DEEP_SLEEP_EN_KV
      #define CFG_LCLK_DEEP_SLEEP_EN_KV                    CFG_LCLK_DEEP_SLEEP_EN
    #endif
  #endif

  #ifdef CFG_LCLK_DPM_EN
    #ifndef CFG_LCLK_DPM_EN_TN
      #define CFG_LCLK_DPM_EN_TN                           CFG_LCLK_DPM_EN
    #endif
    #ifndef CFG_LCLK_DPM_EN_KV
      #define CFG_LCLK_DPM_EN_KV                           CFG_LCLK_DPM_EN
    #endif
  #endif

  #ifdef CFG_IOC_LCLK_CLOCK_GATING_ENABLE
    #ifndef CFG_IOC_LCLK_CLOCK_GATING_ENABLE_TN
      #define CFG_IOC_LCLK_CLOCK_GATING_ENABLE_TN          CFG_IOC_LCLK_CLOCK_GATING_ENABLE
    #endif
    #ifndef CFG_IOC_LCLK_CLOCK_GATING_ENABLE_KV
      #define CFG_IOC_LCLK_CLOCK_GATING_ENABLE_KV          CFG_IOC_LCLK_CLOCK_GATING_ENABLE
    #endif
  #endif

  #ifdef CFG_GNB_BAPM_SUPPORT
    #ifndef CFG_GNB_BAPM_SUPPORT_TN
      #define CFG_GNB_BAPM_SUPPORT_TN                      CFG_GNB_BAPM_SUPPORT
    #endif
    #ifndef CFG_GNB_BAPM_SUPPORT_KV
      #define CFG_GNB_BAPM_SUPPORT_KV                      CFG_GNB_BAPM_SUPPORT
    #endif
  #endif

  #ifdef CFG_SMU_SERVICE_BIT_MAP
    #ifndef CFG_SMU_SERVICE_BIT_MAP_KV
      #define CFG_SMU_SERVICE_BIT_MAP_KV                     CFG_SMU_SERVICE_BIT_MAP
    #endif
  #endif

  #ifdef CFG_IGFX_IOMMU_V1_DIS
    #ifndef CFG_IGFX_IOMMU_V1_DIS_KV
      #define CFG_IGFX_IOMMU_V1_DIS_KV                     CFG_GNB_IGFX_IOMMU_DIS
    #endif
  #endif

  #if  (FCH_SUPPORT == TRUE)
    #ifndef CFG_PCIE_REFCLK_SPREAD_SPECTRUM_TN
      #define CFG_PCIE_REFCLK_SPREAD_SPECTRUM_TN        36
    #endif
    #ifndef CFG_PCIE_REFCLK_SPREAD_SPECTRUM_KV
      #define CFG_PCIE_REFCLK_SPREAD_SPECTRUM_KV        375
    #endif
  #else
    #define CFG_PCIE_REFCLK_SPREAD_SPECTRUM_TN          0
    #define CFG_PCIE_REFCLK_SPREAD_SPECTRUM_KV          0
  #endif



//---------------------------------------------------------------------------------------------------
// TN/RL-specific common BUILD options
//---------------------------------------------------------------------------------------------------
  #if (GNB_TYPE_TN == TRUE)

    // Common configuration defaults for TN
    #ifndef CFG_UMA_STEERING_TN
      #define CFG_UMA_STEERING_TN                       DefaultGarlic
    #endif

    #ifndef CFG_GMC_POWER_GATING_TN
      #define CFG_GMC_POWER_GATING_TN                   GmcPowerGatingWithStutter
    #endif

    #ifndef CFG_GMC_CLOCK_GATING_TN
      #define CFG_GMC_CLOCK_GATING_TN                   TRUE
    #endif

    #ifndef CFG_ORB_DYN_WAKE_ENABLE_TN
      #define CFG_ORB_DYN_WAKE_ENABLE_TN                TRUE
    #endif

    #ifndef CFG_ORB_CLOCK_GATING_ENABLE_TN
      #define CFG_ORB_CLOCK_GATING_ENABLE_TN            TRUE
    #endif

    #ifndef CFG_IOMMU_L1_CLOCK_GATING_ENABLE_TN
      #define CFG_IOMMU_L1_CLOCK_GATING_ENABLE_TN       TRUE
    #endif

    #ifndef CFG_IOMMU_L2_CLOCK_GATING_ENABLE_TN
      #define CFG_IOMMU_L2_CLOCK_GATING_ENABLE_TN       TRUE
    #endif

    #ifndef CFG_LCLK_DEEP_SLEEP_EN_TN
      #define CFG_LCLK_DEEP_SLEEP_EN_TN                 FALSE
    #endif

    #ifndef CFG_LCLK_DPM_EN_TN
      #define CFG_LCLK_DPM_EN_TN                        TRUE
    #endif

    #ifndef CFG_IOC_LCLK_CLOCK_GATING_ENABLE_TN
      #define CFG_IOC_LCLK_CLOCK_GATING_ENABLE_TN       TRUE
    #endif

    #ifndef CFG_GNB_BAPM_SUPPORT_TN
      #define CFG_GNB_BAPM_SUPPORT_TN                   TRUE
    #endif

    // TN-only configuration defaults
    #ifndef CFG_NBDPM_ENABLE_TN
      #define CFG_NBDPM_ENABLE_TN                       TRUE
    #endif

    #ifndef CFG_SMU_SCLK_CLOCK_GATING_ENABLE_TN
      #define CFG_SMU_SCLK_CLOCK_GATING_ENABLE_TN       TRUE
    #endif

    #ifndef CFG_UNUSED_SIMD_POWERGATING_ENABLE_TN
      #define CFG_UNUSED_SIMD_POWERGATING_ENABLE_TN     TRUE
    #endif

    #ifndef CFG_UNUSED_RB_POWERGATING_ENABLE_TN
      #define CFG_UNUSED_RB_POWERGATING_ENABLE_TN       FALSE
    #endif

    #ifndef CFG_LOADLINE_ENABLE_TN
      #define CFG_LOADLINE_ENABLE_TN                    TRUE
    #endif

    #ifndef CFG_LINK_BW_NOTIFICATION_EN_TN
      #define CFG_LINK_BW_NOTIFICATION_EN_TN            TRUE
    #endif

    GNB_BUILD_OPTIONS_TN ROMDATA GnbBuildOptionsTN = {
      {                                                 // GNB_BUILD_OPTIONS_COMMON
        FALSE,                                          // CFG_SCS_SUPPORT NOT SUPPORTED ON TN
        CFG_UMA_STEERING_TN,
        CFG_GMC_POWER_GATING_TN,
        CFG_GMC_CLOCK_GATING_TN,
        CFG_ORB_DYN_WAKE_ENABLE_TN,
        CFG_ORB_CLOCK_GATING_ENABLE_TN,
        CFG_IOMMU_L1_CLOCK_GATING_ENABLE_TN,
        CFG_IOMMU_L2_CLOCK_GATING_ENABLE_TN,
        CFG_LCLK_DEEP_SLEEP_EN_TN,
        CFG_LCLK_DPM_EN_TN,
        CFG_IOC_LCLK_CLOCK_GATING_ENABLE_TN,
        CFG_GNB_BAPM_SUPPORT_TN,
        FALSE,
        CFG_PCIE_REFCLK_SPREAD_SPECTRUM_TN
      },
      CFG_NBDPM_ENABLE_TN,
      CFG_SMU_SCLK_CLOCK_GATING_ENABLE_TN,
      CFG_UNUSED_SIMD_POWERGATING_ENABLE_TN,
      CFG_UNUSED_RB_POWERGATING_ENABLE_TN,
      CFG_LOADLINE_ENABLE_TN,
      CFG_LINK_BW_NOTIFICATION_EN_TN
    };
  #endif

//---------------------------------------------------------------------------------------------------
// KV-specific common BUILD options
//---------------------------------------------------------------------------------------------------
  #if (GNB_TYPE_KV == TRUE)

    // Common configuration defaults for KV
    #ifndef CFG_SCS_SUPPORT_KV
      #define CFG_SCS_SUPPORT_KV                        TRUE
    #endif

    #ifndef CFG_UMA_STEERING_KV
      #define CFG_UMA_STEERING_KV                       DefaultGarlic
    #endif

    #ifndef CFG_GMC_POWER_GATING_KV
      #define CFG_GMC_POWER_GATING_KV                   GmcPowerGatingWithStutter
    #endif

    #ifndef CFG_GMC_CLOCK_GATING_KV
      #define CFG_GMC_CLOCK_GATING_KV                   TRUE
    #endif

    #ifndef CFG_ORB_DYN_WAKE_ENABLE_KV
      #define CFG_ORB_DYN_WAKE_ENABLE_KV                TRUE
    #endif

    #ifndef CFG_ORB_CLOCK_GATING_ENABLE_KV
      #define CFG_ORB_CLOCK_GATING_ENABLE_KV            TRUE
    #endif

    #ifndef CFG_IOMMU_L1_CLOCK_GATING_ENABLE_KV
      #define CFG_IOMMU_L1_CLOCK_GATING_ENABLE_KV       TRUE
    #endif

    #ifndef CFG_IOMMU_L2_CLOCK_GATING_ENABLE_KV
      #define CFG_IOMMU_L2_CLOCK_GATING_ENABLE_KV       TRUE
    #endif

    #ifndef CFG_LCLK_DEEP_SLEEP_EN_KV
      #define CFG_LCLK_DEEP_SLEEP_EN_KV                 TRUE
    #endif

    #ifndef CFG_LCLK_DPM_EN_KV
      #define CFG_LCLK_DPM_EN_KV                        TRUE
    #endif

    #ifndef CFG_IOC_LCLK_CLOCK_GATING_ENABLE_KV
      #define CFG_IOC_LCLK_CLOCK_GATING_ENABLE_KV       TRUE
    #endif

    #ifndef CFG_GNB_BAPM_SUPPORT_KV
      #define CFG_GNB_BAPM_SUPPORT_KV                   FALSE
    #endif

    // KV-only configuration defaults
    #ifndef CFG_NBDPM_ENABLE_KV
      #define CFG_NBDPM_ENABLE_KV                       FALSE
    #endif

    #ifndef CFG_CHUB_CLOCK_GATING_KV
      #define CFG_CHUB_CLOCK_GATING_KV                  TRUE
    #endif

    #ifndef CFG_ACP_CLOCK_GATING_KV
      #define CFG_ACP_CLOCK_GATING_KV                   TRUE
    #endif

    #ifndef CFG_SMU_SERVICE_BIT_MAP_KV
      #define CFG_SMU_SERVICE_BIT_MAP_KV                0xFFFFFFFFul
    #endif

    #ifndef CFG_IGFX_IOMMU_V1_DIS_KV
      #define CFG_IGFX_IOMMU_V1_DIS_KV                  FALSE
    #endif

    GNB_BUILD_OPTIONS_KV ROMDATA GnbBuildOptionsKV = {
      {                                                 // GNB_BUILD_OPTIONS_COMMON
        CFG_SCS_SUPPORT_KV,
        CFG_UMA_STEERING_KV,
        CFG_GMC_POWER_GATING_KV,
        CFG_GMC_CLOCK_GATING_KV,
        CFG_ORB_DYN_WAKE_ENABLE_KV,
        CFG_ORB_CLOCK_GATING_ENABLE_KV,
        CFG_IOMMU_L1_CLOCK_GATING_ENABLE_KV,
        CFG_IOMMU_L2_CLOCK_GATING_ENABLE_KV,
        CFG_LCLK_DEEP_SLEEP_EN_KV,
        CFG_LCLK_DPM_EN_KV,
        CFG_IOC_LCLK_CLOCK_GATING_ENABLE_KV,
        CFG_GNB_BAPM_SUPPORT_KV,
        FALSE,
        CFG_PCIE_REFCLK_SPREAD_SPECTRUM_KV
      },
      CFG_NBDPM_ENABLE_KV,
      CFG_CHUB_CLOCK_GATING_KV,
      CFG_ACP_CLOCK_GATING_KV,
      CFG_SMU_SERVICE_BIT_MAP_KV,
      CFG_IGFX_IOMMU_V1_DIS_KV
    };
  #endif

  //---------------------------------------------------------------------------------------------------
  // SMU Firmware
  //---------------------------------------------------------------------------------------------------

  #if (GNB_TYPE_TN == TRUE)
    #ifndef CFG_INCLUDE_RL_FW
      #define CFG_INCLUDE_RL_FW TRUE
    #endif
    #ifndef CFG_INCLUDE_TN_FW
      #define CFG_INCLUDE_TN_FW TRUE
    #endif
    #if (CFG_INCLUDE_TN_FW == TRUE)
      extern UINT32   FirmwareTN[];
      #define TN_FIRMWARE_POINTER                         &FirmwareTN[0]
    #else
      #define TN_FIRMWARE_POINTER                         NULL
    #endif
    #if (CFG_INCLUDE_RL_FW == TRUE)
      extern UINT32   FirmwareRL[];
      #define RL_FIRMWARE_POINTER                         &FirmwareRL[0]
    #else
      #define RL_FIRMWARE_POINTER                         NULL
    #endif
    GNB_FW_PTRS ROMDATA GnbFwPtrs = {
      TN_FIRMWARE_POINTER,
      RL_FIRMWARE_POINTER
    };
  #endif


  //---------------------------------------------------------------------------------------------------
  // Module entries
  //---------------------------------------------------------------------------------------------------

  #if (AGESA_ENTRY_INIT_EARLY == TRUE)
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_NB_EARLY_INIT
      #define OPTION_NB_EARLY_INIT TRUE
    #endif
    #if (OPTION_NB_EARLY_INIT == TRUE) && (GNB_TYPE_TN == TRUE)
      OPTION_GNB_FEATURE                                  GnbEarlyInterfaceTN;
      #define OPTION_GNBEARLYINTERFACETN_ENTRY            {AMD_FAMILY_TN, GnbEarlyInterfaceTN, TpGnbEarlyInterface},
    #else
      #define OPTION_GNBEARLYINTERFACETN_ENTRY
    #endif
    #if (OPTION_NB_EARLY_INIT == TRUE) && (GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                  GnbEarlyInterfaceKV;
      #define OPTION_GNBEARLYINTERFACEKV_ENTRY            {AMD_FAMILY_KV, GnbEarlyInterfaceKV, TpGnbEarlyInterface},
    #else
      #define OPTION_GNBEARLYINTERFACEKV_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_CONFIG_MAP
      #define OPTION_PCIE_CONFIG_MAP TRUE
    #endif
    #if (OPTION_PCIE_CONFIG_MAP == TRUE) && (GNB_TYPE_TN == TRUE || GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                 PcieConfigurationMap;
      #define OPTION_PCIECONFIGURATIONMAP_ENTRY          {AMD_FAMILY_TN | AMD_FAMILY_KV, PcieConfigurationMap, TpGnbPcieConfigurationMap},
    #else
      #define OPTION_PCIECONFIGURATIONMAP_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_EARLY_INIT
      #define OPTION_PCIE_EARLY_INIT TRUE
    #endif
    #if (OPTION_PCIE_EARLY_INIT == TRUE) && (GNB_TYPE_TN == TRUE)
      OPTION_GNB_FEATURE                                  PcieEarlyInterfaceTN;
      #define OPTION_PCIEEARLYINTERFACETN_ENTRY           {AMD_FAMILY_TN, PcieEarlyInterfaceTN, TpGnbPcieEarlyInterface},
    #else
      #define  OPTION_PCIEEARLYINTERFACETN_ENTRY
    #endif
    #if (OPTION_PCIE_EARLY_INIT == TRUE) && (GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                  PcieEarlyInterfaceKV;
      #define OPTION_PCIEEARLYINTERFACEKV_ENTRY           {AMD_FAMILY_KV, PcieEarlyInterfaceKV, TpGnbPcieEarlyInterface},
    #else
      #define OPTION_PCIEEARLYINTERFACEKV_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    OPTION_GNB_CONFIGURATION  GnbEarlyFeatureTable[] = {
      OPTION_GNBEARLYINTERFACETN_ENTRY
      OPTION_GNBEARLYINTERFACEKV_ENTRY
      OPTION_PCIECONFIGURATIONMAP_ENTRY
      OPTION_PCIEEARLYINTERFACETN_ENTRY
      OPTION_PCIEEARLYINTERFACEKV_ENTRY
      {0, NULL, EndGnbTestPoints}
    };

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_CONFIG_INIT
      #define OPTION_PCIE_CONFIG_INIT TRUE
    #endif
    #if (OPTION_PCIE_CONFIG_INIT == TRUE) && (GNB_TYPE_TN == TRUE || GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                  PcieConfigurationInit;
      #define OPTION_PCIECONFIGURATIONINIT_ENTRY          {AMD_FAMILY_TN | AMD_FAMILY_KV , PcieConfigurationInit, TpGnbEarlierPcieConfigurationInit},
    #else
      #define OPTION_PCIECONFIGURATIONINIT_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_NB_EARLIER_INIT
      #define OPTION_NB_EARLIER_INIT TRUE
    #endif
    #if (OPTION_NB_EARLIER_INIT == TRUE) && (GNB_TYPE_TN == TRUE)
      OPTION_GNB_FEATURE                                  GnbEarlierInterfaceTN;
      #define OPTION_GNBEARLIERINTERFACETN_ENTRY          {AMD_FAMILY_TN, GnbEarlierInterfaceTN, TpGnbEarlierInterface},
    #else
      #define OPTION_GNBEARLIERINTERFACETN_ENTRY
    #endif
    #if (OPTION_NB_EARLIER_INIT == TRUE) && (GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                  GnbEarlierInterfaceKV;
      #define OPTION_GNBEARLIERINTERFACEKV_ENTRY          {AMD_FAMILY_KV, GnbEarlierInterfaceKV, TpGnbEarlierInterface},
    #else
      #define OPTION_GNBEARLIERINTERFACEKV_ENTRY
    #endif

    #if (OPTION_NB_EARLIER_INIT == TRUE) && (GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                  GnbScsInterfaceKV;
      #define OPTION_GNBSCSINTERFACEKV_ENTRY          {AMD_FAMILY_KV, GnbScsInterfaceKV, TpGnbEarlierInterface},
    #else
      #define OPTION_GNBSCSINTERFACEKV_ENTRY
    #endif

    OPTION_GNB_CONFIGURATION  GnbEarlierFeatureTable[] = {
      OPTION_PCIECONFIGURATIONINIT_ENTRY
      OPTION_GNBEARLIERINTERFACETN_ENTRY
      OPTION_GNBEARLIERINTERFACEKV_ENTRY
      OPTION_GNBSCSINTERFACEKV_ENTRY
      {0, NULL, EndGnbTestPoints}
    };
  #endif

  #if (AGESA_ENTRY_INIT_POST == TRUE)
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GFX_CONFIG_POST_INIT
      #define OPTION_GFX_CONFIG_POST_INIT TRUE
    #endif
    #if (OPTION_GFX_CONFIG_POST_INIT == TRUE) && (GNB_TYPE_TN == TRUE || GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                  GfxConfigPostInterface;
      #define OPTION_GFXCONFIGPOSTINTERFACE_ENTRY         {AMD_FAMILY_TN | AMD_FAMILY_KV, GfxConfigPostInterface, TpGnbGfxConfigPostInterface},
    #else
      #define OPTION_GFXCONFIGPOSTINTERFACE_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GFX_POST_INIT
      #define OPTION_GFX_POST_INIT TRUE
    #endif
    #if (OPTION_GFX_POST_INIT == TRUE) && (GNB_TYPE_TN == TRUE)
      OPTION_GNB_FEATURE                                  GfxPostInterfaceTN;
      #define OPTION_GFXPOSTINTERFACETN_ENTRY             {AMD_FAMILY_TN, GfxPostInterfaceTN, TpGnbGfxPostInterface},
    #else
      #define OPTION_GFXPOSTINTERFACETN_ENTRY
    #endif
    #if (OPTION_GFX_POST_INIT == TRUE) && (GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                  GfxPostInterfaceKV;
      #define OPTION_GFXPOSTINTERFACEKV_ENTRY             {AMD_FAMILY_KV, GfxPostInterfaceKV, TpGnbGfxPostInterface},
    #else
      #define OPTION_GFXPOSTINTERFACEKV_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_NB_POST_INIT
      #define OPTION_NB_POST_INIT TRUE
    #endif
    #if (OPTION_NB_POST_INIT == TRUE) && (GNB_TYPE_TN == TRUE)
      OPTION_GNB_FEATURE                                  GnbPostInterfaceTN;
      #define OPTION_GNBPOSTINTERFACETN_ENTRY             {AMD_FAMILY_TN, GnbPostInterfaceTN, TpGnbPostInterface},
    #else
      #define OPTION_GNBPOSTINTERFACETN_ENTRY
    #endif
    #if (OPTION_NB_POST_INIT == TRUE) && (GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                  GnbPostInterfaceKV;
      #define OPTION_GNBPOSTINTERFACEKV_ENTRY             {AMD_FAMILY_KV, GnbPostInterfaceKV, TpGnbPostInterface},
    #else
      #define OPTION_GNBPOSTINTERFACEKV_ENTRY
    #endif

    //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_POST_EARLY_INIT
      #define OPTION_PCIE_POST_EARLY_INIT TRUE
    #endif
    #if (OPTION_PCIE_POST_EARLY_INIT == TRUE) && (GNB_TYPE_TN == TRUE)
      OPTION_GNB_FEATURE                                  PciePostEarlyInterfaceTN;
      #define OPTION_PCIEPOSTEARLYINTERFACETN_ENTRY       {AMD_FAMILY_TN, PciePostEarlyInterfaceTN, TpGnbPciePostEarlyInterface},
    #else
      #define OPTION_PCIEPOSTEARLYINTERFACETN_ENTRY
    #endif
    #if (OPTION_PCIE_POST_EARLY_INIT == TRUE) && (GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                  PciePostEarlyInterfaceKV;
      #define OPTION_PCIEPOSTEARLYINTERFACEKV_ENTRY       {AMD_FAMILY_KV, PciePostEarlyInterfaceKV, TpGnbPciePostEarlyInterface},
    #else
      #define OPTION_PCIEPOSTEARLYINTERFACEKV_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_POST_INIT
      #define OPTION_PCIE_POST_INIT TRUE
    #endif
    #if (OPTION_PCIE_POST_INIT == TRUE) && (GNB_TYPE_TN == TRUE)
      OPTION_GNB_FEATURE                                  PciePostInterfaceTN;
      #define OPTION_PCIEPOSTINTERFACETN_ENTRY            {AMD_FAMILY_TN, PciePostInterfaceTN, TpGnbPciePostInterface},
    #else
      #define OPTION_PCIEPOSTINTERFACETN_ENTRY
    #endif
    #if (OPTION_PCIE_POST_INIT == TRUE) && (GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                  PciePostInterfaceKV;
      #define OPTION_PCIEPOSTINTERFACEKV_ENTRY            {AMD_FAMILY_KV, PciePostInterfaceKV, TpGnbPciePostInterface},
    #else
      #define OPTION_PCIEPOSTINTERFACEKV_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    OPTION_GNB_CONFIGURATION  GnbPostFeatureTable[] = {
      OPTION_PCIEPOSTEARLYINTERFACETN_ENTRY
      OPTION_PCIEPOSTEARLYINTERFACEKV_ENTRY
      OPTION_GFXCONFIGPOSTINTERFACE_ENTRY
      OPTION_GFXPOSTINTERFACETN_ENTRY
      OPTION_GFXPOSTINTERFACEKV_ENTRY
      {0, NULL, EndGnbTestPoints}
    };

    OPTION_GNB_CONFIGURATION  GnbPostAfterDramFeatureTable[] = {
      OPTION_GNBPOSTINTERFACETN_ENTRY
      OPTION_GNBPOSTINTERFACEKV_ENTRY
      OPTION_PCIEPOSTINTERFACETN_ENTRY
      OPTION_PCIEPOSTINTERFACEKV_ENTRY
      {0, NULL, EndGnbTestPoints}
    };
  #endif

  #if (AGESA_ENTRY_INIT_ENV == TRUE)
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_NB_ENV_INIT
      #define OPTION_NB_ENV_INIT TRUE
    #endif
    #if (OPTION_NB_ENV_INIT == TRUE) && (GNB_TYPE_TN == TRUE)
      OPTION_GNB_FEATURE                                  GnbEnvInterfaceTN;
      #define OPTION_GNBENVINTERFACETN_ENTRY              {AMD_FAMILY_TN, GnbEnvInterfaceTN, TpGnbEnvInterface},
    #else
      #define OPTION_GNBENVINTERFACETN_ENTRY
    #endif
    #if (OPTION_NB_ENV_INIT == TRUE) && (GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                  GnbEnvInterfaceKV;
      #define OPTION_GNBENVINTERFACEKV_ENTRY              {AMD_FAMILY_KV, GnbEnvInterfaceKV, TpGnbEnvInterface},
    #else
      #define OPTION_GNBENVINTERFACEKV_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GFX_CONFIG_ENV_INIT
      #define OPTION_GFX_CONFIG_ENV_INIT TRUE
    #endif
    #if (OPTION_GFX_CONFIG_ENV_INIT == TRUE) && (GNB_TYPE_TN == TRUE || GNB_TYPE_KV == TRUE )
      OPTION_GNB_FEATURE                                   GfxConfigEnvInterface;
      #define OPTION_GFXCONFIGENVINTERFACE_ENTRY          {AMD_FAMILY_TN | AMD_FAMILY_KV, GfxConfigEnvInterface, TpGnbGfxConfigEnvInterface},
    #else
      #define  OPTION_GFXCONFIGENVINTERFACE_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GFX_ENV_INIT
      #define OPTION_GFX_ENV_INIT TRUE
    #endif
    #if (OPTION_GFX_ENV_INIT == TRUE) && (GNB_TYPE_TN == TRUE)
      OPTION_GNB_FEATURE                                  GfxEnvInterfaceTN;
      #define OPTION_GFXENVINTERFACETN_ENTRY              {AMD_FAMILY_TN, GfxEnvInterfaceTN, TpGnbGfxEnvInterface},
    #else
      #define OPTION_GFXENVINTERFACETN_ENTRY
    #endif
    #if (OPTION_GFX_ENV_INIT == TRUE) && (GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                  GfxEnvInterfaceKV;
      #define OPTION_GFXENVINTERFACEKV_ENTRY              {AMD_FAMILY_KV, GfxEnvInterfaceKV, TpGnbGfxEnvInterface},
    #else
      #define OPTION_GFXENVINTERFACEKV_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_ENV_INIT
      #define OPTION_PCIE_ENV_INIT TRUE
    #endif
    #if (OPTION_PCIE_ENV_INIT == TRUE) && (GNB_TYPE_TN == TRUE)
      OPTION_GNB_FEATURE PcieEnvInterfaceTN;
      #define OPTION_PCIEENVINTERFACETN_ENTRY             {AMD_FAMILY_TN, PcieEnvInterfaceTN, TpGnbPcieEnvInterface},
    #else
      #define OPTION_PCIEENVINTERFACETN_ENTRY
    #endif
    #if (OPTION_PCIE_ENV_INIT == TRUE) && (GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                  PcieEnvInterfaceKV;
      #define OPTION_PCIEENVINTERFACEKV_ENTRY             {AMD_FAMILY_KV, PcieEnvInterfaceKV, TpGnbPcieEnvInterface},
    #else
      #define OPTION_PCIEENVINTERFACEKV_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------

    OPTION_GNB_CONFIGURATION  GnbEnvFeatureTable[] = {
      OPTION_GNBENVINTERFACETN_ENTRY
      OPTION_GNBENVINTERFACEKV_ENTRY
      OPTION_PCIEENVINTERFACETN_ENTRY
      OPTION_PCIEENVINTERFACEKV_ENTRY
      OPTION_GFXCONFIGENVINTERFACE_ENTRY
      OPTION_GFXENVINTERFACETN_ENTRY
      OPTION_GFXENVINTERFACEKV_ENTRY
      {0, NULL, EndGnbTestPoints}
    };
  #endif

  #if (AGESA_ENTRY_INIT_MID == TRUE)
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GFX_MID_INIT
      #define OPTION_GFX_MID_INIT TRUE
    #endif
    #if (OPTION_GFX_MID_INIT == TRUE) && (GNB_TYPE_TN == TRUE)
      OPTION_GNB_FEATURE                                  GfxMidInterfaceTN;
      #define OPTION_GFXMIDINTERFACETN_ENTRY              {AMD_FAMILY_TN, GfxMidInterfaceTN, TpGnbGfxMidInterface},
    #else
      #define OPTION_GFXMIDINTERFACETN_ENTRY
    #endif
    #if (OPTION_GFX_MID_INIT == TRUE) && (GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                  GfxMidInterfaceKV;
      #define OPTION_GFXMIDINTERFACEKV_ENTRY              {AMD_FAMILY_KV, GfxMidInterfaceKV, TpGnbGfxMidInterface},
    #else
      #define OPTION_GFXMIDINTERFACEKV_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GFX_INTEGRATED_TABLE_INIT
      #define OPTION_GFX_INTEGRATED_TABLE_INIT TRUE
    #endif
    #if (OPTION_GFX_INTEGRATED_TABLE_INIT == TRUE) && (GNB_TYPE_TN == TRUE)
      OPTION_GNB_FEATURE                                  GfxIntInfoTableInterfaceTN;
      #define OPTION_GFXINTINFOTABLEINTERFACETN_ENTRY     {AMD_FAMILY_TN, GfxIntInfoTableInterfaceTN},
    #else
      #define OPTION_GFXINTINFOTABLEINTERFACETN_ENTRY
    #endif
    #if (OPTION_GFX_INTEGRATED_TABLE_INIT == TRUE) && (GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                  GfxIntInfoTableInterfaceKV;
      #define OPTION_GFXINTINFOTABLEINTERFACEKV_ENTRY     {AMD_FAMILY_KV, GfxIntInfoTableInterfaceKV},
    #else
      #define OPTION_GFXINTINFOTABLEINTERFACEKV_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIe_MID_INIT
      #define OPTION_PCIe_MID_INIT TRUE
    #endif
    #if (OPTION_PCIe_MID_INIT == TRUE) && (GNB_TYPE_TN == TRUE)
      OPTION_GNB_FEATURE                                  PcieMidInterfaceTN;
      #define OPTION_PCIEMIDINTERFACETN_ENTRY             {AMD_FAMILY_TN, PcieMidInterfaceTN, TpPcieMidInterface},
    #else
      #define OPTION_PCIEMIDINTERFACETN_ENTRY
    #endif
    #if (OPTION_PCIe_MID_INIT == TRUE) && (GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                  PcieMidInterfaceKV;
      #define OPTION_PCIEMIDINTERFACEKV_ENTRY             {AMD_FAMILY_KV, PcieMidInterfaceKV, TpPcieMidInterface},
    #else
      #define OPTION_PCIEMIDINTERFACEKV_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_NB_MID_INIT
      #define OPTION_NB_MID_INIT TRUE
    #endif
    #if (OPTION_NB_MID_INIT == TRUE) && (GNB_TYPE_TN == TRUE)
      OPTION_GNB_FEATURE                                  GnbMidInterfaceTN;
      #define OPTION_GNBMIDINTERFACETN_ENTRY              {AMD_FAMILY_TN, GnbMidInterfaceTN, TpGnbMidInterface},
    #else
      #define OPTION_GNBMIDINTERFACETN_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GFX_CONFIG_POST_INIT
      #define OPTION_GFX_CONFIG_POST_INIT TRUE
    #endif
    #if (OPTION_GFX_CONFIG_POST_INIT == TRUE) && (GNB_TYPE_TN == TRUE || GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                  GfxConfigMidInterface;
      #define OPTION_GFXCONFIGMIDINTERFACE_ENTRY         {AMD_FAMILY_TN | AMD_FAMILY_KV, GfxConfigMidInterface, TpGnbGfxConfigMidInterface},
    #else
      #define OPTION_GFXCONFIGMIDINTERFACE_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #if (OPTION_NB_MID_INIT == TRUE) && (GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                  GnbMidInterfaceKV;
      #define OPTION_GNBMIDINTERFACEKV_ENTRY              {AMD_FAMILY_KV, GnbMidInterfaceKV, TpGnbMidInterface},
    #else
      #define OPTION_GNBMIDINTERFACEKV_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_MAXPAYLOAD_INTERFACE
      #define OPTION_PCIE_MAXPAYLOAD_INTERFACE TRUE
    #endif
    #if (OPTION_PCIE_MAXPAYLOAD_INTERFACE == TRUE) && (GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                  PcieMaxPayloadInterface;
      #define OPTION_PCIEMAXPAYLOADINTERFACE_ENTRY        {AMD_FAMILY_TN | AMD_FAMILY_KV, PcieMaxPayloadInterface, TpGnbPcieMaxPayloadInterface},
    #else
      #define OPTION_PCIEMAXPAYLOADINTERFACE_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_CLK_PM_INTERFACE
      #define OPTION_PCIE_CLK_PM_INTERFACE FALSE
      #if (GNB_TYPE_TN == TRUE  &&  (OPTION_FS1_SOCKET_SUPPORT == TRUE || OPTION_FP1_SOCKET_SUPPORT == TRUE))
        #undef  OPTION_PCIE_CLK_PM_INTERFACE
        #define OPTION_PCIE_CLK_PM_INTERFACE TRUE
      #endif
      #if (GNB_TYPE_KV == TRUE)
        #undef  OPTION_PCIE_CLK_PM_INTERFACE
        #define OPTION_PCIE_CLK_PM_INTERFACE TRUE
      #endif
    #endif

    #if (OPTION_PCIE_CLK_PM_INTERFACE == TRUE) && (GNB_TYPE_TN == TRUE || GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                  PcieClkPmInterface;
      #define OPTION_PCIECLKPMINTERFACE_ENTRY             {AMD_FAMILY_TN | AMD_FAMILY_KV, PcieClkPmInterface, TpGnbPcieClkPmInterface},
    #else
      #define OPTION_PCIECLKPMINTERFACE_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_ASPM_INTERFACE
      #define OPTION_PCIE_ASPM_INTERFACE TRUE
    #endif
    #if (OPTION_PCIE_ASPM_INTERFACE == TRUE) && (GNB_TYPE_TN == TRUE || GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                  PcieAspmInterface;
      #define OPTION_PCIEASPMINTERFACE_ENTRY              {AMD_FAMILY_TN | AMD_FAMILY_KV, PcieAspmInterface, TpGnbPcieAspmInterface},
    #else
      #define OPTION_PCIEASPMINTERFACE_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GNB_IOAPIC_INTERFACE
      #define OPTION_GNB_IOAPIC_INTERFACE TRUE
    #endif
    #if (OPTION_GNB_IOAPIC_INTERFACE == TRUE) && (GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                  GnbNbIoapicInterface;
      #define OPTION_GNBNBIOAPICINTERFACE_ENTRY          {AMD_FAMILY_KV, GnbNbIoapicInterface, TpGnbNbIoapicInterface},
    #else
      #define OPTION_GNBNBIOAPICINTERFACE_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_PROGRAM_COMMON_CLOCK_INTERFACE
      #define OPTION_PCIE_PROGRAM_COMMON_CLOCK_INTERFACE TRUE
    #endif
    #if (OPTION_PCIE_PROGRAM_COMMON_CLOCK_INTERFACE == TRUE) && (GNB_TYPE_TN == TRUE || GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                  PcieCommClkCfgInterface;
      #define OPTION_PCIECOMMCLKCFGINTERFACE_ENTRY              {AMD_FAMILY_TN | AMD_FAMILY_KV, PcieCommClkCfgInterface, TpGnbPcieClkPmInterface},
    #else
      #define OPTION_PCIECOMMCLKCFGINTERFACE_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    OPTION_GNB_CONFIGURATION  GnbMidFeatureTable[] = {
      OPTION_GFXCONFIGMIDINTERFACE_ENTRY
      OPTION_GFXMIDINTERFACETN_ENTRY
      OPTION_GFXMIDINTERFACEKV_ENTRY
      OPTION_GFXINTINFOTABLEINTERFACETN_ENTRY
      OPTION_GFXINTINFOTABLEINTERFACEKV_ENTRY
      OPTION_PCIEMIDINTERFACETN_ENTRY
      OPTION_PCIEMIDINTERFACEKV_ENTRY
      OPTION_GNBMIDINTERFACETN_ENTRY
      OPTION_GNBMIDINTERFACEKV_ENTRY
      OPTION_PCIEMAXPAYLOADINTERFACE_ENTRY
      OPTION_PCIECLKPMINTERFACE_ENTRY
      OPTION_PCIEASPMINTERFACE_ENTRY
      OPTION_GNBNBIOAPICINTERFACE_ENTRY
      OPTION_PCIECOMMCLKCFGINTERFACE_ENTRY
      {0, NULL, EndGnbTestPoints}
    };
  #endif

  #if (AGESA_ENTRY_INIT_LATE == TRUE)
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_ALIB
      #define OPTION_ALIB FALSE
    #endif
    #if (OPTION_ALIB == TRUE) && (GNB_TYPE_TN == TRUE || GNB_TYPE_KV == TRUE)
      #define ALIB_CALL_TABLE
      #if (GNB_TYPE_TN == TRUE)
        #if ((OPTION_FM2_SOCKET_SUPPORT == TRUE) || (OPTION_FM2r2_SOCKET_SUPPORT == TRUE))
          extern F_ALIB_UPDATE PcieAlibUpdatePcieMmioInfo;
          extern F_ALIB_GET PcieAlibGetBaseTableTNFM2;
          F_ALIB_GET  *AlibGetBaseTable = PcieAlibGetBaseTableTNFM2;
          #undef ALIB_CALL_TABLE
          #define ALIB_CALL_TABLE PcieAlibUpdatePcieMmioInfo,
        #else
          extern F_ALIB_UPDATE PcieAlibUpdatePcieMmioInfo;
          extern F_ALIB_GET PcieAlibGetBaseTableTNFS1;
          F_ALIB_GET  *AlibGetBaseTable = PcieAlibGetBaseTableTNFS1;
          extern F_ALIB_UPDATE PcieAlibUpdateVoltageInfo;
          extern F_ALIB_UPDATE PcieAlibUpdatePcieInfo;
          #undef ALIB_CALL_TABLE
          #define ALIB_CALL_TABLE PcieAlibUpdatePcieMmioInfo, \
                                  PcieAlibUpdateVoltageInfo, \
                                  PcieAlibUpdatePcieInfo,
        #endif
      #endif

      F_ALIB_UPDATE* AlibDispatchTable [] = {
        ALIB_CALL_TABLE
        NULL
      };

      #if (((GNB_TYPE_KV == TRUE) && (OPTION_FM2r2_SOCKET_SUPPORT != TRUE)) || ((GNB_TYPE_KV == TRUE) && (OPTION_FM2r2_SOCKET_SUPPORT != TRUE)))
        OPTION_GNB_FEATURE_V2     PcieAlibUpdateVoltageData;
        #define OPTION_ALIB_PCIEUPDATEVOLTAGEDATA     {AMD_FAMILY_KV, PcieAlibUpdateVoltageData, TpGnbAlibDispatchFeature},
        OPTION_GNB_FEATURE_V2     PcieAlibUpdatePcieData;
        #define OPTION_ALIB_PCIEUPDATEPCIEDATA     {AMD_FAMILY_KV, PcieAlibUpdatePcieData, TpGnbAlibDispatchFeature},
      #else
        #define OPTION_ALIB_PCIEUPDATEVOLTAGEDATA
        #define OPTION_ALIB_PCIEUPDATEPCIEDATA
      #endif

      OPTION_GNB_CONFIGURATION_V2 AlibDispatchTableV2 [] = {
        OPTION_ALIB_PCIEUPDATEVOLTAGEDATA
        OPTION_ALIB_PCIEUPDATEPCIEDATA
        {0, NULL, EndGnbTestPoints}
      };

      #if (GNB_TYPE_TN == TRUE)
        OPTION_GNB_FEATURE                                  PcieAlibFeature;
        #define OPTION_PCIEALIBFEATURE_ENTRY                {AMD_FAMILY_TN, PcieAlibFeature, TpGnbPcieAlibFeature},
      #endif
      #if (GNB_TYPE_KV == TRUE)
        OPTION_GNB_FEATURE                                  PcieAlibV2Feature;
        #define OPTION_PCIEALIBV2FEATURE_ENTRY              {AMD_FAMILY_KV, PcieAlibV2Feature, TpGnbPcieAlibFeature},
      #endif
    #else
      F_ALIB_GET  *AlibGetBaseTable = NULL;
      F_ALIB_UPDATE* AlibDispatchTable [] = {
        NULL
      };
      OPTION_GNB_CONFIGURATION_V2 AlibDispatchTableV2 [] = {
        {0, NULL, EndGnbTestPoints}
      };
      #define OPTION_PCIEALIBFEATURE_ENTRY
      #define OPTION_PCIEALIBV2FEATURE_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_IOMMU_ACPI_IVRS
      #if (CFG_IOMMU_SUPPORT == TRUE)
        #define OPTION_IOMMU_ACPI_IVRS TRUE
      #else
        #define OPTION_IOMMU_ACPI_IVRS FALSE
      #endif
    #endif
    #if (OPTION_IOMMU_ACPI_IVRS == TRUE) && (GNB_TYPE_TN == TRUE || GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                  GnbIommuIvrsTable;
      #define OPTIONIOMMUACPIIVRSLATE_ENTRY               {AMD_FAMILY_TN | AMD_FAMILY_KV, GnbIommuIvrsTable},
    #else
      #define OPTIONIOMMUACPIIVRSLATE_ENTRY
    #endif
    #if (CFG_IOMMU_SUPPORT == TRUE) && (GNB_TYPE_TN == TRUE || GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                  GnbIommuScratchMemoryRangeInterface;
      #define OPTIONIOMMUSCRATCHMEMORYLATE_ENTRY          {AMD_FAMILY_TN | AMD_FAMILY_KV, GnbIommuScratchMemoryRangeInterface, TpGnbIommuIvrsTable},
    #else
      #define OPTIONIOMMUSCRATCHMEMORYLATE_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    OPTION_GNB_CONFIGURATION  GnbLateFeatureTable[] = {
      #if (GNB_TYPE_TN == TRUE)
        OPTION_PCIEALIBFEATURE_ENTRY
      #endif
      #if (GNB_TYPE_KV == TRUE)
        OPTION_PCIEALIBV2FEATURE_ENTRY
      #endif
      OPTIONIOMMUSCRATCHMEMORYLATE_ENTRY
      OPTIONIOMMUACPIIVRSLATE_ENTRY
      {0, NULL, EndGnbTestPoints}
    };
  #endif

  #if (AGESA_ENTRY_INIT_S3SAVE == TRUE)
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GFX_INIT_SVIEW
      #define OPTION_GFX_INIT_SVIEW TRUE
    #endif
    #if (OPTION_GFX_INIT_SVIEW == TRUE) && (GNB_TYPE_TN == TRUE || GNB_TYPE_KV == TRUE)
      OPTION_GNB_FEATURE                                  GfxInitSview;
      #define OPTION_GFXINITSVIEW_ENTRY                   {AMD_FAMILY_TN | AMD_FAMILY_KV, GfxInitSview},
    #else
      #define OPTION_GFXINITSVIEW_ENTRY
    #endif

    OPTION_GNB_CONFIGURATION  GnbS3SaveFeatureTable[] = {
      OPTION_GFXINITSVIEW_ENTRY
      {0, NULL, EndGnbTestPoints}
    };
  #endif

  #if  (GNB_TYPE_TN == TRUE || GNB_TYPE_KV == TRUE)
    #define GNBS3RESTOREV4
    #define GNBS3RESTOREV7
    #if (GNB_TYPE_TN == TRUE)
      S3_DISPATCH_FUNCTION  GnbSmuServiceRequestV4S3Script;
      #undef  GNBS3RESTOREV4
      #define GNBS3RESTOREV4 {GnbSmuServiceRequestV4S3Script_ID, GnbSmuServiceRequestV4S3Script},
    #endif
    #if (GNB_TYPE_KV == TRUE)
      S3_DISPATCH_FUNCTION  GnbSmuServiceRequestV7S3Script;
      #undef  GNBS3RESTOREV7
      #define GNBS3RESTOREV7 {GnbSmuServiceRequestV7S3Script_ID, GnbSmuServiceRequestV7S3Script},
    #endif
    S3_DISPATCH_FUNCTION  GnbLibStallS3Script;
    #define PCIELATERESTORETN
    #define PCIELATERESTOREKV
    #define GFXSCLKRESTORETN
    #if (GNB_TYPE_TN == TRUE)
      S3_DISPATCH_FUNCTION  PcieLateRestoreInitTNS3Script;
      S3_DISPATCH_FUNCTION  GfxRequestSclkTNS3Script;
      #undef  PCIELATERESTORETN
      #define PCIELATERESTORETN {PcieLateRestoreTNS3Script_ID, PcieLateRestoreInitTNS3Script},
      #undef  GFXSCLKRESTORETN
      #define GFXSCLKRESTORETN  {GfxRequestSclkTNS3Script_ID,  GfxRequestSclkTNS3Script     },
    #endif
    #if (GNB_TYPE_KV == TRUE)
      S3_DISPATCH_FUNCTION  PcieLateRestoreInitKVS3Script;
      #undef  PCIELATERESTOREKV
      #define PCIELATERESTOREKV {PcieLateRestoreKVS3Script_ID, PcieLateRestoreInitKVS3Script},
    #endif
    #define GNB_S3_DISPATCH_FUNCTION_TABLE \
    GNBS3RESTOREV4      \
    GNBS3RESTOREV7      \
    PCIELATERESTORETN \
    GFXSCLKRESTORETN  \
    PCIELATERESTOREKV \
    {GnbLibStallS3Script_ID, GnbLibStallS3Script},
  #endif

#endif
#endif  // _OPTION_GNB_INSTALL_H_
