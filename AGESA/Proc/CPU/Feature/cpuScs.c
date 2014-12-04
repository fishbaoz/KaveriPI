/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD AGESA CPU Software Configuration Settings (SCS) feature support code.
 *
 * Contains code that declares the AGESA CPU SCS related APIs
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
#include "amdlib.h"
#include "heapManager.h"
#include "GeneralServices.h"
#include "cpuFamilyTranslation.h"
#include "cpuApicUtilities.h"
#include "cpuFeatures.h"
#include "cpuScs.h"
#include "Filecode.h"
#include "cpuPstateTables.h"
CODE_GROUP (G1_PEICC)
RDATA_GROUP (G1_PEICC)

#define FILECODE PROC_CPU_FEATURE_CPUSCS_FILECODE

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
extern CPU_FAMILY_SUPPORT_TABLE           ScsFamilyServiceTable;
extern CPU_FAMILY_SUPPORT_TABLE           PstateFamilyServiceTable;

/*---------------------------------------------------------------------------------------*/
/**
 *  Is Software Configuration Settings (SCS) supported
 *
 * @param[in]    PlatformConfig     Contains the runtime modifiable feature input data.
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @retval       TRUE               SCS is supported.
 * @retval       FALSE              SCS is not supported.
 *
 */
BOOLEAN
STATIC
IsScsFeatureEnabled (
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  BOOLEAN               IsEnabled;
  SCS_FAMILY_SERVICES   *FamilyServices;

  IsEnabled = TRUE;

  GetFeatureServicesOfCurrentCore (&ScsFamilyServiceTable, (CONST VOID **)&FamilyServices, StdHeader);
  if ((FamilyServices == NULL) || (!FamilyServices->IsScsSupported (FamilyServices, PlatformConfig, StdHeader))) {
    IsEnabled = FALSE;
  }
  return IsEnabled;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Procedure to raise a floating point number to an integer power. This procedure
 * mimics the pow() procedure in C maths library.
 *
 * @param[in]  x                       Floating point base
 * @param[in]  y                       Integer exponent
 *
 * @return     The result              x^y
 *
 */
DOUBLE
Power (
  IN       DOUBLE x,
  IN       INT32  y
  )
{
  DOUBLE Temp;

  if (y == 0) {
    return 1;
  }
  Temp = Power (x, (y / 2));
  if ((y % 2) == 0) {
    return Temp * Temp;
  } else {
    if (y > 0) {
      return x * Temp * Temp;
    } else  {
      ASSERT (x != 0)
      return (Temp * Temp) / x;
    }
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Match the calculated constant with a value generated from 5 bit exponent/mantissa
 * to allow programming of the hardware
 *
 * @param[in]      EnCnt                   Calculated value.
 * @param[out]     Mantissa                Mantissa.
 * @param[in,out]  Exponent                Exponent.
 * @param[in,out]  DifferenceLess          Calculated difference.
 *
 * @retval         TRUE                    Match found.
 * @retval         FALSE                   Match not found.
 *
 */
BOOLEAN
DoMath (
  IN       DOUBLE   EnCnt,
     OUT   UINT8    *Mantissa,
  IN OUT   UINT8    *Exponent,
  IN OUT   DOUBLE   *DifferenceLess
  )
{
  UINT8   M;
  UINT8   E;
  DOUBLE  Normalizer;
  DOUBLE  x;
  BOOLEAN Result;
  DOUBLE  DifferenceGreater;

  E = *Exponent;
  Result = FALSE;

  for (M = 0; M < 32; M++) {
    // Calculate a 64 bit FP from 5 bit exponent and mantissa.
    x = (1 + (((DOUBLE) (M)) / 32));
    Normalizer = x * Power (2, (UINT32) (E - 15));
    // If value calculated from 5 bit exponent/mantissa  == value calculated from hardware, we have our 5 bit exponent and mantissa
    // to write to the encode register.
    if (Normalizer == EnCnt) {
      *Mantissa = M;
      Result = TRUE;
      break;
    } else {
      // Keep track of difference if value calculated from 5 bit exponent/mantissa  < value calculated from hardware.
      if (EnCnt > Normalizer) {
        *DifferenceLess = EnCnt - Normalizer;
      } else {
        // Value calculated from 5 bit exponent/mantissa  > value calculated from hardware, so value calculated from hardware
        // is between last 5 bit exponent/mantissa and this one.
        DifferenceGreater = Normalizer - EnCnt;
        // Decide which of the two 5 bit exponent/mantissa values is closest to value calculated from hardware.
        if (DifferenceGreater > *DifferenceLess) {
          if (M == 0) {
            M = 31;
            E -= 1;
            *Exponent = E;
          } else {
            M -= 1;
          }
        }
        *Mantissa = M;
        Result = TRUE;
        break;
      }
    }
  }
  return Result;

}

/*---------------------------------------------------------------------------------------*/
/**
 *  Initialize Software Configuration Services (SCS)
 *
 * @param[in]    EntryPoint         Timepoint designator.
 * @param[in]    PlatformConfig     Contains the runtime modifiable feature input data.
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @retval       AGESA_SUCCESS      SCS initialization completed successfully.
 * @retval       AGESA_ERROR        SCS initialization failed.
 *
 */
AGESA_STATUS
STATIC
InitializeScsFeature (
  IN       UINT64                 EntryPoint,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  AGESA_STATUS  CalledStatus;
  SCS_FAMILY_SERVICES   *ScsFamilyServices;
  ALLOCATE_HEAP_PARAMS AllocateHeapParams;
  LOCATE_HEAP_PTR     LocateHeapParams;
  DOUBLE              EnCntConstant;
  DOUBLE              BoostTimeConstant;
  C001_1073_MSR       LocalMsrRegister;
  DOUBLE              P0FrequencyInGHz;
  UINT32              P0FrequencyInMHz;
  DOUBLE              EnCnt;
  DOUBLE              DifferenceLess;
  UINT8               FiveBitMantissa;
  UINT8               FiveBitExponent;
  SMU_RAM_CPU_INFO    *SmuCpuInfoPtr;
  PCI_ADDR            PciAddress;
  SAMPLE_AND_RESIDENCY_TIMER_REGISTER SampleAndResidency;
  CPU_LOGICAL_ID      CpuLogicalId;

  CalledStatus = AGESA_UNSUPPORTED;
  if ((EntryPoint & CPU_FEAT_BEFORE_PM_INIT) != 0) {
    // Get frequency of HWP0 before power delivery compatibility check; have to use our own procedure here
    // as we may be getting the frequency of a disabled pstate, and family procedure will ASSERT if
    // frequency is requested for a disabled pstate.
    ScsFamilyServices = NULL;
    GetFeatureServicesOfCurrentCore (&ScsFamilyServiceTable, (CONST VOID **)&ScsFamilyServices, StdHeader);
    ASSERT (ScsFamilyServices != NULL);

    ScsFamilyServices->GetP0Frequency (&P0FrequencyInMHz, StdHeader);

    AllocateHeapParams.BufferHandle = AMD_SCS_HWP0_FREQ;
    AllocateHeapParams.Persist = HEAP_LOCAL_CACHE;
    AllocateHeapParams.RequestedBufferSize = sizeof (P0FrequencyInMHz);
    CalledStatus = HeapAllocateBuffer (&AllocateHeapParams, StdHeader);
    ASSERT (CalledStatus == AGESA_SUCCESS);
    if (CalledStatus != AGESA_SUCCESS) {
      return CalledStatus;
    }

    *((UINT32 *) (AllocateHeapParams.BufferPtr)) = P0FrequencyInMHz;
  } else if ((EntryPoint & CPU_FEAT_AFTER_PM_INIT) != 0) {
    AGESA_TESTPOINT (TpProcCpuEntryScs, StdHeader);
    IDS_HDT_CONSOLE (CPU_TRACE, "SCS Initialization\n");

    ScsFamilyServices = NULL;
    GetFeatureServicesOfCurrentCore (&ScsFamilyServiceTable, (CONST VOID **)&ScsFamilyServices, StdHeader);
    ASSERT (ScsFamilyServices != NULL);

    LibAmdFinit ();

    // Find selected table and constant(s) from the heap.
    LocateHeapParams.BufferHandle = AMD_SCS_SMU_RAM_INFO;
    if (HeapLocateBuffer (&LocateHeapParams, StdHeader) != AGESA_SUCCESS) {
      PutEventLog (AGESA_ERROR,
                   (CPU_EVENT_SCS_INITIALIZATION_ERROR + AGESA_SCS_HEAP_ENTRY_MISSING),
                   0, 0, 0, 0, StdHeader);
      return AGESA_ERROR;
    }

    SmuCpuInfoPtr = (SMU_RAM_CPU_INFO *) LocateHeapParams.BufferPtr;

    // If we return an empty buffer, assume Smu returned no data.
    if (SmuCpuInfoPtr->CountApmWeights == 0) {
      PutEventLog (AGESA_ERROR,
                   (CPU_EVENT_SCS_INITIALIZATION_ERROR + AGESA_SCS_BUFFER_EMPTY),
                   0, 0, 0, 0, StdHeader);
      return AGESA_ERROR;
    }

    // Program weights
    CalledStatus = ScsFamilyServices->ProgramWeights (SmuCpuInfoPtr, StdHeader);
    if (CalledStatus != AGESA_SUCCESS) {
      return (UINT32) CalledStatus;
    }

    // Gather required items from hardware
    // The value below is transferred from the SMU.
    EnCntConstant = SmuCpuInfoPtr->CountConstant;
    //EnCntConstant = 428.7709;                         //Uncomment this line to get realistic math without SMU data.

    // The value below is transferred from the SMU.
    BoostTimeConstant = SmuCpuInfoPtr->BoostConstant;
    //BoostTimeConstant = 1.07;                         //Uncomment this line to get realistic math without SMU data.

    LibAmdMsrRead (MSR_C001_1073, (UINT64 *) (&LocalMsrRegister), StdHeader);
    GetLogicalIdOfCurrentCore (&CpuLogicalId, StdHeader);
    if (CpuLogicalId.Family & AMD_FAMILY_15_KV) {
      PciAddress.AddressValue = EXTENDED_PRODUCT_INFORMATION_PCI_ADDR;
      LibAmdPciRead (AccessWidth32, PciAddress, &SampleAndResidency, StdHeader);
      if (SampleAndResidency.CSampleTimer == 0) {
        SampleAndResidency.CSampleTimer = 2;
      }
    } else {
      PciAddress.AddressValue = SAMPLE_AND_RESIDENCY_TIMER_PCI_ADDR;
      LibAmdPciRead (AccessWidth32, PciAddress, &SampleAndResidency, StdHeader);
    }
    ASSERT (SampleAndResidency.CSampleTimer != 0);

    LocateHeapParams.BufferHandle = AMD_SCS_HWP0_FREQ;
    if (HeapLocateBuffer (&LocateHeapParams, StdHeader) != AGESA_SUCCESS) {
      return AGESA_ERROR;
    }

    P0FrequencyInMHz = *((UINT32 *) LocateHeapParams.BufferPtr);

    // Convert from Mhz to GHz
    P0FrequencyInGHz = ((DOUBLE) (P0FrequencyInMHz)) / 1000;

    //1. Calculate normalizer
    EnCnt = (DOUBLE) (254 - LocalMsrRegister.Base - HEADROOM);
    EnCnt = EnCnt / (P0FrequencyInGHz * EnCntConstant * SampleAndResidency.CSampleTimer / 2);
    EnCnt = EnCnt * BoostTimeConstant;

    //2. Encode normalizer

    FiveBitMantissa = 0;
    DifferenceLess = 0;
    for (FiveBitExponent = 0; FiveBitExponent < 32; FiveBitExponent++) {
      if (DoMath (EnCnt, &FiveBitMantissa, &FiveBitExponent, &DifferenceLess)) {
        break;
      }
    }
    ASSERT (FiveBitExponent != 32);

    ScsFamilyServices->WriteNormalizer (FiveBitExponent, FiveBitMantissa, StdHeader);
  }
  return (UINT32) CalledStatus;
}

CONST CPU_FEATURE_DESCRIPTOR ROMDATA CpuFeatureScs =
{
  CpuScs,
  (CPU_FEAT_BEFORE_PM_INIT | CPU_FEAT_AFTER_PM_INIT),
  IsScsFeatureEnabled,
  InitializeScsFeature
};
