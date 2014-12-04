/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD AGESA CPU Prefetch Mode feature support code.
 *
 * Contains code that declares the AGESA CPU prefetch mode related APIs
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Feature
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
 ******************************************************************************
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
 ******************************************************************************
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AGESA.h"
#include "cpuFamilyTranslation.h"
#include "cpuFeatures.h"
#include "GeneralServices.h"
#include "cpuPrefetchMode.h"
#include "cpuLateInit.h"
#include "Filecode.h"
CODE_GROUP (G1_PEICC)
RDATA_GROUP (G1_PEICC)

#define FILECODE PROC_CPU_FEATURE_CPUPREFETCHMODE_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
extern CPU_FAMILY_SUPPORT_TABLE PrefetchModeFamilyServiceTable;

/*---------------------------------------------------------------------------------------*/
/**
 *  Should prefetch mode be enabled
 *
 * @param[in]    PlatformConfig     Contains the runtime modifiable feature input data.
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @retval       TRUE               prefetch mode is not 'AUTO'.
 * @retval       FALSE              prefetch mode is 'AUTO'
 *
 */
BOOLEAN
IsPrefetchModeFeatureEnabled (
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  BOOLEAN IsEnabled;

  IsEnabled = FALSE;

  if ((PlatformConfig->PlatformProfile.AdvancedPerformanceProfile.HardwarePrefetchMode != HARDWARE_PREFETCHER_AUTO) ||
      (PlatformConfig->PlatformProfile.AdvancedPerformanceProfile.SoftwarePrefetchMode != SOFTWARE_PREFETCHES_AUTO)) {
    IsEnabled = TRUE;
  }
  return IsEnabled;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Enable prefetch mode
 *
 * @param[in]    EntryPoint         Timepoint designator.
 * @param[in]    PlatformConfig     Contains the runtime modifiable feature input data.
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @retval       AGESA_SUCCESS      Always succeeds.
 *
 */
AGESA_STATUS
InitializePrefetchModeFeature (
  IN       UINT64                 EntryPoint,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  AP_EXE_PARAMS ApParams;
  AGESA_STATUS        AgesaStatus;
  AGESA_STATUS        CalledStatus;

  AgesaStatus = AGESA_SUCCESS;
  CalledStatus = AGESA_SUCCESS;

  IDS_HDT_CONSOLE (CPU_TRACE, "    Prefetch Mode is not AUTO\n");

  ApParams.StdHeader = *StdHeader;
  ApParams.FunctionNumber = AP_LATE_TASK_CPU_PREFETCH_MODE;
  ApParams.RelatedDataBlock = (VOID *) PlatformConfig;
  ApParams.RelatedBlockLength = sizeof (PLATFORM_CONFIGURATION);
  CalledStatus = RunLateApTaskOnAllAPs (&ApParams, StdHeader);
  if (CalledStatus > AgesaStatus) {
    AgesaStatus = CalledStatus;
  }
  AgesaStatus = CpuPrefetchModeApTask (&ApParams);
  if (CalledStatus > AgesaStatus) {
    AgesaStatus = CalledStatus;
  }
  return AgesaStatus;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CpuPrefetchModeApTask
 *
 *  Parameters:
 *    @param[in] ApExeParams   Handle to config for library and services.
 *
 *    @retval    AGESA_STATUS
 *
 *  Processing:
 *
 */
AGESA_STATUS
CpuPrefetchModeApTask (
  IN       AP_EXE_PARAMS *ApExeParams
  )
{
  UINT32      Socket;
  UINT32      Module;
  UINT32      Core;
  AGESA_STATUS IgnoredSts;
  AGESA_STATUS CalledStatus;
  PREFETCH_MODE_FAMILY_SERVICES *FamilyServices;

  CalledStatus = AGESA_ERROR;

  IdentifyCore (&(ApExeParams->StdHeader), &Socket, &Module, &Core, &IgnoredSts);
  GetFeatureServicesOfSocket (&PrefetchModeFamilyServiceTable, Socket, (CONST VOID **)&FamilyServices, &(ApExeParams->StdHeader));
  if (FamilyServices != NULL) {
    CalledStatus = FamilyServices->PrefetchModeControlOnCore (FamilyServices, (PLATFORM_CONFIGURATION *) (ApExeParams->RelatedDataBlock), &(ApExeParams->StdHeader));
  }
  return CalledStatus;
}

CONST CPU_FEATURE_DESCRIPTOR ROMDATA CpuFeaturePrefetchMode =
{
  PrefetchMode,
  (CPU_FEAT_INIT_LATE_END | CPU_FEAT_S3_LATE_RESTORE_END),
  IsPrefetchModeFeatureEnabled,
  InitializePrefetchModeFeature
};
