/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family_15 TN early sample support.
 *
 * Provides the code and data required to support pre-production silicon.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x15/TN
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
 *****************************************************************************
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
#include "Ids.h"
#include "cpuRegisters.h"
#include "cpuEarlyInit.h"
#include "cpuFamilyTranslation.h"
#include "cpuF15PowerMgmt.h"
#include "cpuF15TnPowerMgmt.h"
#include "GeneralServices.h"
#include "Filecode.h"
#define FILECODE PROC_CPU_FAMILY_0X15_TN_F15TNEARLYSAMPLES_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
typedef union {
  UINT64           RawData;
  PATCH_LOADER_MSR BitFields;
} PATCH_LOADER;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

BOOLEAN
F15TnEarlySamplesLoadMicrocode (
  IN       MICROCODE_PATCH    *MicrocodePatchPtr,
  IN OUT   AMD_CONFIG_PARAMS  *StdHeader
  );

BOOLEAN
F15TnEarlySamplesLoadMicrocodePatch (
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

BOOLEAN
F15TnEarlySamplePstatePowerWorkaround (
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

BOOLEAN
F15TnEarlySampleIsNbPstateEn (
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

BOOLEAN
F15TnEarlySampleSkipNbPsLo (
  IN       UINT32            *NbPstateStatus,
  IN       UINT32            *NbPstateControl,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          D A T A    D E C L A R A T I O N S
 *----------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 *  Update microcode patch in current processor.
 *
 *  Then reads the patch id, and compare it to the expected, in the Microprocessor
 *  patch block.
 *
 *  @param[in] StdHeader   - Config handle for library and services.
 *
 *  @retval    TRUE   - Patch Loaded Successfully.
 *  @retval    FALSE  - Patch Did Not Get Loaded.
 *
 */
BOOLEAN
F15TnEarlySamplesLoadMicrocodePatch (
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8    PatchNumber;
  UINT8    TotalPatches;
  UINT16   ProcessorEquivalentId;
  BOOLEAN  Status;
  MICROCODE_PATCH **MicrocodePatchPtr;
  CPU_SPECIFIC_SERVICES *FamilySpecificServices;
  MICROCODE_PATCH     *ForceLoadMicrocodePatchPtr;
  Status = FALSE;

  if (IsCoreComputeUnitPrimary (FirstCoreIsComputeUnitPrimary, StdHeader)) {
    // Get the patch pointer
    GetCpuServicesOfCurrentCore (&FamilySpecificServices, StdHeader);
    FamilySpecificServices->GetMicroCodePatchesStruct (FamilySpecificServices, (VOID **) &MicrocodePatchPtr, &TotalPatches, StdHeader);
    ForceLoadMicrocodePatchPtr = NULL;
    IDS_SKIP_HOOK (IDS_UCODE, &ForceLoadMicrocodePatchPtr, StdHeader) {
      if (ForceLoadMicrocodePatchPtr == NULL) {
        // Get the processor microcode path equivalent ID
        if (GetPatchEquivalentId (&ProcessorEquivalentId, StdHeader)) {
          // parse the patch table to see if we have one for the current cpu
          for (PatchNumber = 0; PatchNumber < TotalPatches; PatchNumber++) {
            if (ValidateMicrocode (MicrocodePatchPtr[PatchNumber], ProcessorEquivalentId, StdHeader)) {
              if (F15TnEarlySamplesLoadMicrocode (MicrocodePatchPtr[PatchNumber], StdHeader)) {
                Status = TRUE;
              } else {
                PutEventLog (AGESA_ERROR,
                             CPU_ERROR_MICRO_CODE_PATCH_IS_NOT_LOADED,
                             0, 0, 0, 0, StdHeader);
              }
              break; // Once we find a microcode patch that matches the processor, exit the for loop
            }
          }
        }
      } else {
        IDS_HDT_CONSOLE (CPU_TRACE, "  Force Ucode loaded from BVM binary offset %x\n", ForceLoadMicrocodePatchPtr);
        if (F15TnEarlySamplesLoadMicrocode (ForceLoadMicrocodePatchPtr, StdHeader)) {
          Status = TRUE;
        } else {
          PutEventLog (AGESA_ERROR,
                       CPU_ERROR_MICRO_CODE_PATCH_IS_NOT_LOADED,
                       0, 0, 0, 0, StdHeader);
        }
      }
    }
  }
  return Status;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  F15TnEarlySamplesLoadMicrocode
 *
 *    Update microcode patch in current processor, then reads the
 *    patch id, and compare it to the expected, in the Microprocessor
 *    patch block.
 *
 *    Note: This is a special version of the normal LoadMicrocode()
 *    function which lives in cpuMicrocodePatch.c. This version
 *    implements a workaround (on Tn-A0 only) before applying the
 *    microcode patch.
 *
 *    @param[in]       MicrocodePatchPtr  - Pointer to Microcode Patch.
 *    @param[in,out]   StdHeader          - Pointer to AMD_CONFIG_PARAMS struct.
 *
 *    @retval          TRUE  - Patch Loaded Successfully.
 *    @retval          FALSE - Patch Did Not Get Loaded.
 *
 */
BOOLEAN
F15TnEarlySamplesLoadMicrocode (
  IN       MICROCODE_PATCH    *MicrocodePatchPtr,
  IN OUT   AMD_CONFIG_PARAMS  *StdHeader
  )
{
  UINT32       MicrocodeVersion;
  UINT64       MsrData;
  UINT64       CuCfgMsrValue;
  PATCH_LOADER PatchLoaderMsr;
  CPU_LOGICAL_ID LogicalId;

  // Load microcode patch into CPU
  GetLogicalIdOfCurrentCore (&LogicalId, StdHeader);
  PatchLoaderMsr.RawData = (UINT64) MicrocodePatchPtr;
  PatchLoaderMsr.BitFields.SBZ = 0;
  // Check if this CPU is TN-A0, expected fix in TN-A1
  if ((LogicalId.Revision & AMD_F15_TN_A0) != 0) {
    LibAmdMsrRead (MSR_BR_FROM, &MsrData, StdHeader);

    // Erratum #655, only applicable to TN-A0.
    // Set MSR C001_1023[1] = 1b prior to writing to MSR C001_1020
    LibAmdMsrRead (MSR_CU_CFG, &CuCfgMsrValue, StdHeader);
    MsrData = CuCfgMsrValue | BIT1;
    LibAmdMsrWrite (MSR_CU_CFG, &MsrData, StdHeader);
  }

  LibAmdMsrWrite (MSR_PATCH_LOADER, &PatchLoaderMsr.RawData, StdHeader);

  if ((LogicalId.Revision & AMD_F15_TN_A0) != 0) {
    // Erratum #655
    // Restore MSR C001_1023[1] = previous setting
    LibAmdMsrWrite (MSR_CU_CFG, &CuCfgMsrValue, StdHeader);
  }

  // Do ucode patch Authentication
  // Read microcode version back from CPU, determine if
  // it is the same patch level as contained in the source
  // microprocessor patch block passed in
  GetMicrocodeVersion (&MicrocodeVersion, StdHeader);
  if (MicrocodeVersion == MicrocodePatchPtr->PatchID) {
    return (TRUE);
  } else {
    return (FALSE);
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Workaround for Pstate Power for TN-A0 Early Sample
 *
 * This function workarounds the power duplication of P-states.
 *
 * @param[in]  StdHeader               Config handle for library and services.
 *
 * @retval     TRUE   - P-state power has been workarounded.
 * @retval     FALSE  - P-state remain untouched.
 *
 */
BOOLEAN
F15TnEarlySamplePstatePowerWorkaround (
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  PSTATE_MSR            PstateMsr;
  PSTATE_MSR            PstateMsr2;
  UINT32                PstateIndex;
  UINT32                PstateIndex2;
  BOOLEAN               Adjustment;

  PCI_ADDR              PciAddress;
  UINT32                PciData;
  CPU_LOGICAL_ID        LogicalId;

  // Qualifiers of this workaround:
  // * If the part is not a TN-A0 with F3x1FC[21] = 1 then apply this fix.
  GetLogicalIdOfCurrentCore (&LogicalId, StdHeader);
  if ((LogicalId.Revision & AMD_F15_TN_A0) != 0) {
    PciAddress.AddressValue = MAKE_SBDFO (0, 0, 24, FUNC_3, 0x1FC);
    LibAmdPciRead (AccessWidth32, PciAddress, &PciData, StdHeader);
    if ((PciData & BIT21) != 0) {
      // This part doesn't need this workaround.
      return FALSE;
    }
  } else {
    // This part doesn't need this workaround.
    return FALSE;
  }

  ASSERT (NM_PS_REG > 1);
  for (PstateIndex = 0; PstateIndex < (NM_PS_REG - 1); PstateIndex++) {
    Adjustment = FALSE;
    LibAmdMsrRead (PstateIndex + PS_REG_BASE, (UINT64 *) &PstateMsr, StdHeader);
    if (PstateMsr.PsEnable == 1) {
      for (PstateIndex2 = PstateIndex + 1; PstateIndex2 < NM_PS_REG; PstateIndex2++) {
        LibAmdMsrRead (PstateIndex2 + PS_REG_BASE, (UINT64 *) &PstateMsr2, StdHeader);
        if (PstateMsr2.PsEnable == 1) {
          // Do CpuVid adjustment if applicable.
          if (PstateMsr.CpuVid > PstateMsr2.CpuVid) {
            PstateMsr2.CpuVid = PstateMsr.CpuVid;
            Adjustment = TRUE;
          }
          // Do IddValue adjustment if applicable.
          if ((PstateMsr.IddValue <= PstateMsr2.IddValue) && (PstateMsr.IddDiv == PstateMsr2.IddDiv)) {
            PstateMsr2.IddValue = PstateMsr.IddValue - 1;
            Adjustment = TRUE;
          }
          if (Adjustment == TRUE) {
            LibAmdMsrWrite (PstateIndex2 + PS_REG_BASE, (UINT64 *) &PstateMsr2, StdHeader);
            break;
          }
        }
      }
    }
  }
  return TRUE;
}


/*---------------------------------------------------------------------------------------*/
/**
 * Workaround for IsNbPstateEnabled for TN-A0 Early Sample
 *
 * If TN A0 & F3x1FC[21] == 0, NbPstate is not enabled
 *
 * @param[in]  StdHeader               Config handle for library and services.
 *
 * @retval     TRUE   - continue to check is NbPstate enabled
 * @retval     FALSE  - return FALSE, no need to check other condition.
 *
 */
BOOLEAN
F15TnEarlySampleIsNbPstateEn (
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32   PrctInfo;
  PCI_ADDR PciAddress;
  CPU_LOGICAL_ID LogicalId;

  GetLogicalIdOfCurrentCore (&LogicalId, StdHeader);
  // Check if this CPU is TN A0 & F3x1FC[21] == 0
  if ((LogicalId.Revision & AMD_F15_TN_A0) != 0) {
    PciAddress.AddressValue = MAKE_SBDFO (0, 0, 24, FUNC_3, PRCT_INFO_REG);
    LibAmdPciRead (AccessWidth32, PciAddress, &PrctInfo, StdHeader);
    if ((PrctInfo & BIT21) == 0) {
      return FALSE;
    }
  }
  return TRUE;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  SkipNbPsLoPart workaround
 *
 *  @param[in] NbPstateStatus  - NbPstate status register value.
 *  @param[in] NbPstateControl - NbPstate control register value.
 *  @param[in] StdHeader       - Config handle for library and services.
 *
 *  @retval    TRUE   - Skip NbPstateLo program
 *  @retval    FALSE  - Do NbPstateLo program
 *
 */
BOOLEAN
F15TnEarlySampleSkipNbPsLo (
  IN       UINT32            *NbPstateStatus,
  IN       UINT32            *NbPstateControl,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  if ((((NB_PSTATE_STS_REGISTER *) NbPstateStatus)->StartupNbPstate != ((NB_PSTATE_CTRL_REGISTER *) NbPstateControl)->NbPstateHi) &&
      (((NB_PSTATE_STS_REGISTER *) NbPstateStatus)->StartupNbPstate != ((NB_PSTATE_CTRL_REGISTER *) NbPstateControl)->NbPstateLo)) {
    return TRUE;
  }
  return FALSE;
}

/*---------------------------------------------------------------------------------------
 *                           L O C A L    F U N C T I O N S
 *---------------------------------------------------------------------------------------
 */

