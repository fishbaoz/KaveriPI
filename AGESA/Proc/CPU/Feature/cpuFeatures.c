/* $NoKeywords:$ */
/**
 * @file
 *
 * Implement general feature dispatcher.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU
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
 *                            M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AGESA.h"
#include "amdlib.h"
#include "Ids.h"
#include "GeneralServices.h"
#include "cpuFeatures.h"
#include "cpuFamilyTranslation.h"
#include "heapManager.h"
#include "Filecode.h"
CODE_GROUP (G1_PEICC)
RDATA_GROUP (G1_PEICC)
#define FILECODE PROC_CPU_FEATURE_CPUFEATURES_FILECODE
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
 *            E X P O R T E D    F U N C T I O N S - External General Services API
 *----------------------------------------------------------------------------------------
 */
extern CONST CPU_FEATURE_DESCRIPTOR* ROMDATA SupportedCpuFeatureList[];

/**
 * Update the status of a specific feature
 *
 * Update the status (is enabled or not) in heap
 *
 * @param[in]       Feature        Indicates the desired feature.
 * @param[in]       IsEnabled      Indicates the desired feature
 * @param[in]       StdHeader      Standard AMD configuration parameters.
 *
 */
VOID
UpdateFeatureStatusInHeap (
  IN       DISPATCHABLE_CPU_FEATURES Feature,
  IN       BOOLEAN IsEnabled,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8 *CpuFeatureIsEnabledHeap;
  LOCATE_HEAP_PTR LocateHeap;

  ASSERT (Feature < MaxCpuFeature);

  CpuFeatureIsEnabledHeap = NULL;
  LocateHeap.BufferHandle = AMD_IS_FEATURE_ENABLED;
  if (HeapLocateBuffer (&LocateHeap, StdHeader) == AGESA_SUCCESS) {
    CpuFeatureIsEnabledHeap = LocateHeap.BufferPtr;
  }
  if (CpuFeatureIsEnabledHeap != NULL) {
    CpuFeatureIsEnabledHeap[Feature] = (UINT8) IsEnabled;
  }
}

/**
 * Determines if a specific feature is or will be enabled.
 *
 * This code traverses the feature list until a match is
 * found, then invokes the 'IsEnabled' function of the
 * feature.
 *
 * @param[in]       Feature        Indicates the desired feature.
 * @param[in]       PlatformConfig Contains the runtime modifiable feature input data.
 * @param[in]       StdHeader      Standard AMD configuration parameters.
 *
 * @retval         TRUE      Feature is or will be enabled
 * @retval         FALSE     Feature is not enabled
 */
BOOLEAN
IsFeatureEnabled (
  IN       DISPATCHABLE_CPU_FEATURES Feature,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8 *CpuFeatureIsEnabledHeap;
  UINTN i;
  LOCATE_HEAP_PTR LocateHeap;

  ASSERT (Feature < MaxCpuFeature);

  CpuFeatureIsEnabledHeap = NULL;
  LocateHeap.BufferHandle = AMD_IS_FEATURE_ENABLED;
  if (HeapLocateBuffer (&LocateHeap, StdHeader) == AGESA_SUCCESS) {
    CpuFeatureIsEnabledHeap = LocateHeap.BufferPtr;
  }
  if (CpuFeatureIsEnabledHeap != NULL) {
    if (CpuFeatureIsEnabledHeap[Feature] != CPU_FEATURE_UNDECIDED) {
      return ((BOOLEAN) CpuFeatureIsEnabledHeap[Feature]);
    }
  }

  for (i = 0; SupportedCpuFeatureList[i] != NULL; i++) {
    if (SupportedCpuFeatureList[i]->Feature == Feature) {
      return (SupportedCpuFeatureList[i]->IsEnabled (PlatformConfig, StdHeader));
    }
  }
  return FALSE;
}

/**
 * Dispatches all features needing to perform some initialization at
 * this time point.
 *
 * This routine searches the feature table for features needing to
 * run at this time point, and invokes them.
 *
 * @param[in]      EntryPoint     Timepoint designator
 * @param[in]      PlatformConfig Contains the runtime modifiable feature input data.
 * @param[in]      StdHeader      Standard AMD configuration parameters.
 *
 * @return         The most severe status of any called service.
 */
AGESA_STATUS
DispatchCpuFeatures (
  IN       UINT64                 EntryPoint,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT8        *CpuFeatureIsEnabledHeap;
  UINTN        i;
  BOOLEAN      FeatureIsEnabled;
  BOOLEAN      ResultDecided;
  AGESA_STATUS AgesaStatus;
  AGESA_STATUS CalledStatus;
  AGESA_STATUS IgnoredStatus;
  LOCATE_HEAP_PTR LocateHeap;
  ALLOCATE_HEAP_PARAMS AllocateHeap;


  AgesaStatus = AGESA_SUCCESS;

  CpuFeatureIsEnabledHeap = NULL;
  LocateHeap.BufferHandle = AMD_IS_FEATURE_ENABLED;
  if (HeapLocateBuffer (&LocateHeap, StdHeader) == AGESA_SUCCESS) {
    CpuFeatureIsEnabledHeap = LocateHeap.BufferPtr;
  } else {
    AllocateHeap.BufferHandle = AMD_IS_FEATURE_ENABLED;
    AllocateHeap.RequestedBufferSize = MaxCpuFeature;
    AllocateHeap.Persist = HEAP_SYSTEM_MEM;
    if (HeapAllocateBuffer (&AllocateHeap, StdHeader) == AGESA_SUCCESS) {
      CpuFeatureIsEnabledHeap = AllocateHeap.BufferPtr;
      // Initialize all feature as NOT_KNOW_YET
      LibAmdMemFill ((VOID *) CpuFeatureIsEnabledHeap, CPU_FEATURE_UNDECIDED, MaxCpuFeature, StdHeader);
    }
  }

  if (IsBsp (StdHeader, &IgnoredStatus)) {
    for (i = 0; SupportedCpuFeatureList[i] != NULL; i++) {
      if ((SupportedCpuFeatureList[i]->EntryPoint & EntryPoint) != 0) {
        ResultDecided = FALSE;
        FeatureIsEnabled = FALSE;
        IDS_SKIP_HOOK (IDS_CPU_FEAT, (CPU_FEATURE_DESCRIPTOR *) SupportedCpuFeatureList[i], StdHeader) {
          if (CpuFeatureIsEnabledHeap != NULL) {
            // Try to get isFeatureEnabled from heap
            if (CpuFeatureIsEnabledHeap[SupportedCpuFeatureList[i]->Feature] != CPU_FEATURE_UNDECIDED) {
              FeatureIsEnabled = (BOOLEAN) (CpuFeatureIsEnabledHeap[SupportedCpuFeatureList[i]->Feature]);
              ResultDecided = TRUE;
            }
          }
          if (!ResultDecided) {
            // Get isFeatureEnabled by running IsEnabled
            FeatureIsEnabled = SupportedCpuFeatureList[i]->IsEnabled (PlatformConfig, StdHeader);
            if (CpuFeatureIsEnabledHeap != NULL) {
              CpuFeatureIsEnabledHeap[SupportedCpuFeatureList[i]->Feature] = (UINT8) FeatureIsEnabled;
            }
          }
          if (FeatureIsEnabled) {
            CalledStatus = SupportedCpuFeatureList[i]->InitializeFeature (EntryPoint, PlatformConfig, StdHeader);
            if (CalledStatus > AgesaStatus) {
              AgesaStatus = CalledStatus;
            }
          }
        }
      }
    }
  }
  return AgesaStatus;
}
