/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family_15 KV SCS Support
 *
 * Family support for Software Configuration Settings (SCS) feature
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x15/KV
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
#include "cpuRegisters.h"
#include "heapManager.h"
#include "cpuFeatures.h"
#include "cpuServices.h"
#include "GeneralServices.h"
#include "cpuFamilyTranslation.h"
#include "cpuApicUtilities.h"
#include "CommonReturns.h"
#include "cpuScs.h"
#include "cpuF15KvScs.h"
#include "cpuF15PowerMgmt.h"
#include "cpuF15KvPowerMgmt.h"
#include "Filecode.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)

#define FILECODE PROC_CPU_FAMILY_0X15_KV_CPUF15KVSCS_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 *  Call to calculate P0 frequency.
 *
 *  @param[out]  P0FrequencyInMHz         The P-State's frequency in MegaHertz
 *  @param[in]   StdHeader                Header for library and services
 *
 */
VOID
F15KvGetP0Frequency (
     OUT   UINT32            *P0FrequencyInMHz,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8 TempValue;
  UINT32 CpuDid;
  UINT32 CpuFid;
  UINT64 LocalMsrRegister;

  LibAmdMsrRead (MSR_PSTATE_0, &LocalMsrRegister, StdHeader);
  CpuDid = (UINT32) (((PSTATE_MSR *) &LocalMsrRegister)->CpuDid);
  CpuFid = (UINT32) (((PSTATE_MSR *) &LocalMsrRegister)->CpuFid);

  switch (CpuDid) {
  case 0:
    TempValue = 1;
    break;
  case 1:
    TempValue = 2;
    break;
  case 2:
    TempValue = 4;
    break;
  case 3:
    TempValue = 8;
    break;
  case 4:
    TempValue  =  16;
    break;
  default:
    // CpuDid is set to an undefined value.  This is due to either a misfused CPU, or
    // an invalid P-state MSR write.
    ASSERT (FALSE);
    TempValue = 1;
    break;
  }
  *P0FrequencyInMHz = (100 * (CpuFid + 0x10) / TempValue);

}

/*---------------------------------------------------------------------------------------*/
/**
 * Procedure to write the CACMON weight registers.
 * @param[in]  SmuCpuInfoPtr          Pointer to the weights
 * @param[in]  StdHeader              Config handle for library and services.
 *
 */
VOID
STATIC
WriteTheWeights (
  IN       VOID                       *SmuCpuInfoPtr,
  IN       AMD_CONFIG_PARAMS          *StdHeader
  )
{
  C001_1072_MSR       PowerMonitorWeights;
  UINT32              i;

  if (IsCoreComputeUnitPrimary (FirstCoreIsComputeUnitPrimary, StdHeader)) {
  // Process table and program the Power Monitor Weights (MSRC001_1072)
    for (i = 0; i < ((SMU_RAM_CPU_WEIGHTS_INFO *) SmuCpuInfoPtr)->CountApmWeights; i++) {
      PowerMonitorWeights.ArrayIndex = i;
      PowerMonitorWeights.WriteCmd = 1;
      PowerMonitorWeights.WriteData = ((SMU_RAM_CPU_WEIGHTS_INFO *) SmuCpuInfoPtr)->MSRApmWeights[i].Weight;

      LibAmdMsrWrite (MSR_C001_1072, (UINT64 *) (&PowerMonitorWeights), StdHeader);
    }
  }
}


/*---------------------------------------------------------------------------------------*/
/**
 * Family specific code for programming weights.
 *
 * @param[in]  *SmuCpuInfoPtr          Pointer to Smu data.
 * @param[in]  StdHeader               Config handle for library and services.
 *
 * @retval     AGESA_SUCCESS           Normalizer and weights programmed
 * @retval     AGESA_ERROR             Unable to locate Smu data, or error in Smu data
 *
 */
AGESA_STATUS
STATIC
F15KvProgramWeights (
  IN       SMU_RAM_CPU_INFO       *SmuCpuInfoPtr,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  AP_TASK             TaskPtr;

  // If we get valid data, but number of counts is incorrect for Kaveri, panic !
  ASSERT (SmuCpuInfoPtr->CountApmWeights == WEIGHTS);
  if (SmuCpuInfoPtr->CountApmWeights != WEIGHTS) {
    PutEventLog (AGESA_ERROR,
                 (CPU_EVENT_SCS_INITIALIZATION_ERROR + AGESA_SCS_WEIGHTS_MISMATCH),
                 0, 0, 0, 0, StdHeader);
    return AGESA_ERROR;
  }

  TaskPtr.FuncAddress.PfApTaskI = WriteTheWeights;
  TaskPtr.DataTransfer.DataSizeInDwords = (UINT16) (1 + SmuCpuInfoPtr->CountApmWeights);
  TaskPtr.DataTransfer.DataPtr = &(SmuCpuInfoPtr->CountApmWeights);
  TaskPtr.DataTransfer.DataTransferFlags = 0;
  TaskPtr.ExeFlags = WAIT_FOR_CORE;
  ApUtilRunCodeOnAllLocalCoresAtEarly (&TaskPtr, StdHeader, NULL);

  return AGESA_SUCCESS;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write normalizer.
 *
 * @param[in]  ExponentAndMantissa    Pointer to data
 * @param[in]  StdHeader              Config handle for library and services.
 *
 */
VOID
STATIC
WriteTheNormalizer (
  IN       VOID                *ExponentAndMantissa,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  C001_1073_MSR               LocalMsrRegister;

  if (IsCoreComputeUnitPrimary (FirstCoreIsComputeUnitPrimary, StdHeader)) {
    LibAmdMsrRead (MSR_C001_1073, (UINT64 *) (&LocalMsrRegister), StdHeader);

    //Write normalizer as 5-bit exponent and 5 bit mantissa to MSRC001_1073[Exp/Man].
    LocalMsrRegister.Exponent = ((NORMALIZER *) ExponentAndMantissa)->Exponent;
    LocalMsrRegister.Mantissa = ((NORMALIZER *) ExponentAndMantissa)->Mantissa;
    LibAmdMsrWrite (MSR_C001_1073, (UINT64 *) (&LocalMsrRegister), StdHeader);

    // Toggle the Ready bit to ensure proper operation.
    LocalMsrRegister.Ready = 0;
    LibAmdMsrWrite (MSR_C001_1073, (UINT64 *) (&LocalMsrRegister), StdHeader);

    LocalMsrRegister.Ready = 1;
    LibAmdMsrWrite (MSR_C001_1073, (UINT64 *) (&LocalMsrRegister), StdHeader);

    IDS_SKIP_HOOK (IDS_MSR_ACCESS_OVERRIDE, NULL, StdHeader) {
      LocalMsrRegister.Lock = 1;
      LibAmdMsrWrite (MSR_C001_1073, (UINT64 *) (&LocalMsrRegister), StdHeader);
    }
  }

}

/*---------------------------------------------------------------------------------------*/
/**
 * Family specific code for writing normalizer.
 *
 * @param[in]  FiveBitExponent         The current CPU's family services.
 * @param[in]  FiveBitMantissa         Timepoint designator.
 * @param[in]  StdHeader               Config handle for library and services.
 *
 */
VOID
STATIC
F15KvWriteNormalizer (
  IN       UINT8               FiveBitExponent,
  IN       UINT8               FiveBitMantissa,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  AP_TASK             TaskPtr;
  NORMALIZER          ExponentAndMantissa;

  ExponentAndMantissa.Exponent = FiveBitExponent;
  ExponentAndMantissa.Mantissa = FiveBitMantissa;

  TaskPtr.FuncAddress.PfApTaskI = WriteTheNormalizer;
  TaskPtr.DataTransfer.DataSizeInDwords = 1;
  TaskPtr.DataTransfer.DataPtr = &(ExponentAndMantissa);
  TaskPtr.DataTransfer.DataTransferFlags = 0;
  TaskPtr.ExeFlags = WAIT_FOR_CORE;
  ApUtilRunCodeOnAllLocalCoresAtEarly (&TaskPtr, StdHeader, NULL);

}

CONST SCS_FAMILY_SERVICES ROMDATA F15KvScsSupport =
{
  0,
  (PF_SCS_IS_SUPPORTED) CommonReturnTrue,
  F15KvGetP0Frequency,
  F15KvProgramWeights,
  F15KvWriteNormalizer
};
