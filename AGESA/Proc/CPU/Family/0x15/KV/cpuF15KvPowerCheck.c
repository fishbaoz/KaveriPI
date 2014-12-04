/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family_15 KV P-State power check
 *
 * Performs the "Processor-Systemboard Power Delivery Compatibility Check" as
 * described in the BKDG.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x15/KV
 * @e \$Revision: 285514 $   @e \$Date: 2014-02-21 14:05:56 -0600 (Fri, 21 Feb 2014) $
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
#include "cpuF15PowerMgmt.h"
#include "cpuF15KvPowerMgmt.h"
#include "cpuRegisters.h"
#include "cpuApicUtilities.h"
#include "cpuFamilyTranslation.h"
#include "cpuF15PowerCheck.h"
#include "cpuF15KvPowerCheck.h"
#include "cpuF15KvUtilities.h"
#include "GeneralServices.h"
#include "cpuFeatures.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_CPU_FAMILY_0X15_KV_CPUF15KVPOWERCHECK_FILECODE

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
VOID
STATIC
F15KvTransitionPstateCore (
  IN       VOID *StateNumber,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

VOID
STATIC
F15KvPmPwrChkCopyPstate (
  IN     UINT8 Dest,
  IN     UINT8 Src,
  IN     AMD_CONFIG_PARAMS *StdHeader
  );

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 * Family 15h core 0 entry point for performing the family 15h Processor-
 * Systemboard Power Delivery Check.
 *
 * The steps are as follows:
 *    1. Starting with P0, loop through all P-states until a passing state is
 *       found.  A passing state is one in which the current required by the
 *       CPU is less than the maximum amount of current that the system can
 *       provide to the CPU.  If P0 is under the limit, no further action is
 *       necessary.
 *    2. If at least one P-State is under the limit & at least one P-State is
 *       over the limit, the BIOS must:
 *       a. If the processor's current P-State is disabled by the power check,
 *          then the BIOS must request a transition to an enabled P-state
 *          using MSRC001_0062[PstateCmd] and wait for MSRC001_0063[CurPstate]
 *          to reflect the new value.
 *       b. Copy the contents of the enabled P-state MSRs to the highest
 *          performance P-state locations.
 *       c. Request a P-state transition to the P-state MSR containing the
 *          COF/VID values currently applied.
 *       d. If a subset of boosted P-states are disabled, then copy the contents
 *          of the highest performance boosted P-state still enabled to the
 *          boosted P-states that have been disabled.
 *       e. If all boosted P-states are disabled, then program D18F4x15C[BoostSrc]
 *          to zero.
 *       f. Adjust the following P-state parameters affected by the P-state
 *          MSR copy by subtracting the number of P-states that are disabled
 *          by the power check.
 *          1. F3x64[HtcPstateLimit]
 *          2. F3x68[SwPstateLimit]
 *          3. F3xDC[PstateMaxVal]
 *    3. If all P-States are over the limit, the BIOS must:
 *       a. If the processor's current P-State is !=F3xDC[PstateMaxVal], then
 *          write F3xDC[PstateMaxVal] to MSRC001_0062[PstateCmd] and wait for
 *          MSRC001_0063[CurPstate] to reflect the new value.
 *       b. If MSRC001_0061[PstateMaxVal]!=000b, copy the contents of the P-state
 *          MSR pointed to by F3xDC[PstateMaxVal] to the software P0 MSR.
 *          Write 000b to MSRC001_0062[PstateCmd] and wait for MSRC001_0063
 *          [CurPstate] to reflect the new value.
 *       c. Adjust the following P-state parameters to zero:
 *          1. F3x64[HtcPstateLimit]
 *          2. F3x68[SwPstateLimit]
 *          3. F3xDC[PstateMaxVal]
 *       d. Program D18F4x15C[BoostSrc] to zero.
 *
 * @param[in]  FamilySpecificServices  The current Family Specific Services.
 * @param[in]  CpuEarlyParams          Service parameters
 * @param[in]  StdHeader               Config handle for library and services.
 *
 */
VOID
F15KvPmPwrCheck (
  IN       CPU_SPECIFIC_SERVICES *FamilySpecificServices,
  IN       AMD_CPU_EARLY_PARAMS  *CpuEarlyParams,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  UINT8                                 Core;
  UINT8                                 CurrentSwPstate;
  UINT8                                 i;
  UINT8                                 InitialPsMaxVal;
  UINT8                                 NewSwP0;
  UINT8                                 NumberOfBoostStates;
  UINT8                                 NumberOfDisabledBoostStates;
  UINT8                                 NumberOfDisabledSoftwarePstates;
  UINT8                                 NumberOfSoftwarePstates;
  UINT8                                 PsMaxValOnEntry;
  UINT8                                 Pstate;
  UINT32                                ActiveCores;
  UINT32                                LocalPciRegister;
  UINT32                                ProcIddMax;
  UINT64                                LocalMsrRegister;
  BOOLEAN                               AllowablePstateFound;
  BOOLEAN                               HwPstateMaxValAdjusted;
  AP_TASK                               ApTask;
  PCI_ADDR                              PciAddress;
  HTC_REGISTER                          HtcReg;
  NB_CAPS_REGISTER                      NbCaps;
  SW_PS_LIMIT_REGISTER                  StcReg;
  F15_CPB_CTRL_REGISTER                 CpbReg;
  F15_KV_CLK_PWR_TIMING_CTRL2_REGISTER  ClkPwrTimingCtrl2;

  // Starting with P0, loop through all P-states until a passing state is
  // found.  A passing state is one in which the current required by the
  // CPU is less than the maximum amount of current that the system can
  // provide to the CPU.  If P0 is under the limit, no further action is
  // necessary.
  PciAddress.AddressValue = CPB_CTRL_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, &CpbReg, StdHeader); // F4x15C
  NumberOfBoostStates = (UINT8) CpbReg.NumBoostStates;

  PciAddress.AddressValue = CPTC2_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, &ClkPwrTimingCtrl2, StdHeader); // F3xDC
  PsMaxValOnEntry = (UINT8) ClkPwrTimingCtrl2.HwPstateMaxVal;
  NumberOfSoftwarePstates = (PsMaxValOnEntry + 1) - NumberOfBoostStates;

  // Start by analyzing the boosted P-states
  NumberOfDisabledSoftwarePstates = 0;
  NumberOfDisabledBoostStates = 0;
  AllowablePstateFound = FALSE;
  for (Pstate = 0; Pstate < NumberOfBoostStates; Pstate++) {
    if (FamilySpecificServices->GetProcIddMax (FamilySpecificServices, Pstate, &ProcIddMax, StdHeader)) {
      if (ProcIddMax > CpuEarlyParams->PlatformConfig.VrmProperties[CoreVrm].MaximumCurrentLimit) {
        // Add to event log the Pstate that exceeded the current limit
        PutEventLog (AGESA_WARNING,
                     CPU_EVENT_PM_PSTATE_OVERCURRENT,
                     0, Pstate, 0, 0, StdHeader);
      } else {
        // Indicate that an enabled P-state has been found to be legal
        AllowablePstateFound = TRUE;
        break;
      }
    }
    // Increment whether or not PsEn is set
    NumberOfDisabledBoostStates++;
  }

  if (AllowablePstateFound) {
    // An enabled boost state has been found to be under the board limit
    if (NumberOfDisabledBoostStates != 0) {
      // A subset of boost states are out of bounds
      // Disable by copying the highest legal boost state upwards
      for (i = 0; i < NumberOfDisabledBoostStates; i++) {
        F15KvPmPwrChkCopyPstate (i, NumberOfDisabledBoostStates, StdHeader);
      }
    }
  } else {
    // All boosted P-states are disabled
    // 1. Program D18F4x15C[BoostSrc] to zero.
    PciAddress.AddressValue = CPB_CTRL_PCI_ADDR;
    CpbReg.BoostSrc = 0;
    LibAmdPciWrite (AccessWidth32, PciAddress, &CpbReg, StdHeader); // F4x15C
    // 2. Clear PsEn for all boost P-states
    for (i = 0; i < NumberOfBoostStates; i++) {
      FamilySpecificServices->DisablePstate (FamilySpecificServices, i, StdHeader);
    }
    // 3. Update the result of isFeatureEnabled in heap.
    UpdateFeatureStatusInHeap (CoreBoost, FALSE, StdHeader);

    // Now assess the software P-states
    for (Pstate = NumberOfBoostStates; Pstate < NM_PS_REG; Pstate++) {
      if (FamilySpecificServices->GetProcIddMax (FamilySpecificServices, Pstate, &ProcIddMax, StdHeader)) {
        if (ProcIddMax > CpuEarlyParams->PlatformConfig.VrmProperties[CoreVrm].MaximumCurrentLimit) {
          // Add to event log the Pstate that exceeded the current limit
          PutEventLog (AGESA_WARNING,
                       CPU_EVENT_PM_PSTATE_OVERCURRENT,
                       0, Pstate, 0, 0, StdHeader);
          NumberOfDisabledSoftwarePstates++;
        } else {
          AllowablePstateFound = TRUE;
          break;
        }
      } else {
        // Exit the loop on finding the first software P-state with PsEn = 0.
        break;
      }
    }
    if (!AllowablePstateFound) {
      // All P-states have been found to draw too much current for the platform
      PutEventLog (AGESA_FATAL,
                   CPU_EVENT_PM_ALL_PSTATE_OVERCURRENT,
                   0, 0, 0, 0, StdHeader);
      // Adjust the number of disabled P-states so the remainder of the code behaves as if Pmin was legal
      ASSERT (NumberOfDisabledSoftwarePstates != 0);
      NumberOfDisabledSoftwarePstates--;
      // At least one software P-state must be defined
      ASSERT (Pstate != NumberOfBoostStates);
      Pstate--;
    }
    if (NumberOfDisabledSoftwarePstates != 0) {
      // The new software P0 is located at the first legal P-state found
      NewSwP0 = Pstate;

      // Prepare ApTask for P-state transitions
      ApTask.FuncAddress.PfApTaskI = F15KvTransitionPstateCore;
      ApTask.DataTransfer.DataSizeInDwords = 1;
      ApTask.DataTransfer.DataPtr = &CurrentSwPstate;
      ApTask.DataTransfer.DataTransferFlags = 0;
      ApTask.ExeFlags = 0;
      GetActiveCoresInCurrentSocket (&ActiveCores, StdHeader);

      // Check for NB Pstates
      LibAmdMsrRead ((PS_REG_BASE + NewSwP0), &LocalMsrRegister, StdHeader);
      if (((PSTATE_MSR *) &LocalMsrRegister)->NbPstate == 1) {
        // If the new software P0 state indexes the low NB P-state, disable NB P-states for memory training
        CpuEarlyParams->PlatformConfig.PlatformProfile.PlatformPowerPolicy = Performance;
        F15KvNbPstateDis (FamilySpecificServices, CpuEarlyParams, StdHeader);
      }

      LibAmdMsrRead (MSR_COFVID_STS, &LocalMsrRegister, StdHeader);
      if (((COFVID_STS_MSR *) &LocalMsrRegister)->CurPstate < NewSwP0) {
        // The current P-state is illegal.  Transition to the new software P0
        CurrentSwPstate = NewSwP0 - NumberOfBoostStates;
        for (Core = ((UINT8) ActiveCores - 1); Core > 0; Core--) {
          ApUtilRunCodeOnSocketCore (0, Core, &ApTask, StdHeader);
        }
        ApUtilTaskOnExecutingCore (&ApTask, StdHeader, (VOID *) CpuEarlyParams);
      } else {
        // The current P-state is legal.  Convert from HW to SW numbering
        CurrentSwPstate = (UINT8) ((COFVID_STS_MSR *) &LocalMsrRegister)->CurPstate - NumberOfBoostStates;
      }

      // Shift all legal hardware P-states up to SW P0 - Pn (includes hidden HTC if present)
      for (i = NumberOfBoostStates; (i + NumberOfDisabledSoftwarePstates) < NM_PS_REG; i++) {
        F15KvPmPwrChkCopyPstate (i, (i + NumberOfDisabledSoftwarePstates), StdHeader);
      }

      // Determine the new location of the current P-state and transition to it
      ASSERT (CurrentSwPstate >= NumberOfDisabledSoftwarePstates);
      CurrentSwPstate = CurrentSwPstate - NumberOfDisabledSoftwarePstates;
      for (Core = ((UINT8) ActiveCores - 1); Core > 0; Core--) {
        ApUtilRunCodeOnSocketCore (0, Core, &ApTask, StdHeader);
      }
      ApUtilTaskOnExecutingCore (&ApTask, StdHeader, (VOID *) CpuEarlyParams);

      // Disable all remaining P-states
      for (i = NumberOfBoostStates + NumberOfSoftwarePstates - NumberOfDisabledSoftwarePstates; i < NM_PS_REG; i++) {
        FamilySpecificServices->DisablePstate (FamilySpecificServices, i, StdHeader);
      }

      // Adjust the values of HTC / STC limits and HwPstateMaxVal since software P-states have shifted
      PciAddress.AddressValue = HTC_PCI_ADDR;
      LibAmdPciRead (AccessWidth32, PciAddress, (VOID *) &HtcReg, StdHeader); // F3x64
      HtcReg.HtcPstateLimit = (HtcReg.HtcPstateLimit > NumberOfDisabledSoftwarePstates) ? HtcReg.HtcPstateLimit - NumberOfDisabledSoftwarePstates : NumberOfBoostStates;
      LibAmdPciWrite (AccessWidth32, PciAddress, (VOID *) &HtcReg, StdHeader); // F3x64

      PciAddress.Address.Register = SW_PS_LIMIT_REG;
      LibAmdPciRead (AccessWidth32, PciAddress, (VOID *) &StcReg, StdHeader); // F3x68
      StcReg.SwPstateLimit = (StcReg.SwPstateLimit > NumberOfDisabledSoftwarePstates) ? StcReg.SwPstateLimit - NumberOfDisabledSoftwarePstates : NumberOfBoostStates;
      LibAmdPciWrite (AccessWidth32, PciAddress, (VOID *) &StcReg, StdHeader); // F3x68

      PciAddress.Address.Register = CPTC2_REG;
      LibAmdPciRead (AccessWidth32, PciAddress, (VOID *) &ClkPwrTimingCtrl2, StdHeader); // F3xDC
      ClkPwrTimingCtrl2.HwPstateMaxVal = (ClkPwrTimingCtrl2.HwPstateMaxVal > NumberOfDisabledSoftwarePstates) ? ClkPwrTimingCtrl2.HwPstateMaxVal - NumberOfDisabledSoftwarePstates : NumberOfBoostStates;
      LibAmdPciWrite (AccessWidth32, PciAddress, (VOID *) &ClkPwrTimingCtrl2, StdHeader); // F3xDC
    }
  }

  // HTC / STC do not persist across a warm reset.  Determine whether or not P-states have been adjusted on previous boots
  HwPstateMaxValAdjusted = FALSE;
  PciAddress.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0xB8);
  LocalPciRegister = 0xC0106010;
  LibAmdPciWrite (AccessWidth32, PciAddress, &LocalPciRegister, StdHeader);
  PciAddress.Address.Register = 0xBC;
  LibAmdPciRead (AccessWidth32, PciAddress, &LocalPciRegister, StdHeader);
  LocalPciRegister = ((LocalPciRegister >> 17) & 0xFF);
  if (LocalPciRegister != 0) {
    InitialPsMaxVal = LibAmdBitScanReverse (LocalPciRegister);
    if (PsMaxValOnEntry != InitialPsMaxVal) {
      // Pstates have previously been disabled.  HwPstateMaxVal persists, so use it.
      HwPstateMaxValAdjusted = TRUE;
      PciAddress.AddressValue = CPTC2_PCI_ADDR;
      LibAmdPciRead (AccessWidth32, PciAddress, &LocalPciRegister, StdHeader);
      LocalPciRegister = ClkPwrTimingCtrl2.HwPstateMaxVal;

      PciAddress.Address.Register = HTC_REG;
      LibAmdPciRead (AccessWidth32, PciAddress, &HtcReg, StdHeader);
      HtcReg.HtcPstateLimit = LocalPciRegister;
      LibAmdPciWrite (AccessWidth32, PciAddress, &HtcReg, StdHeader);

      PciAddress.Address.Register = SW_PS_LIMIT_REG;
      LibAmdPciRead (AccessWidth32, PciAddress, &StcReg, StdHeader);
      StcReg.SwPstateLimit = LocalPciRegister;
      LibAmdPciWrite (AccessWidth32, PciAddress, &StcReg, StdHeader);
    }
  }

  // Apply the BKDG recommendation for HwPstateMaxVal here to close the window in which a warm reset could cause a logic problem
  if (!HwPstateMaxValAdjusted) {
    PciAddress.AddressValue = NB_CAPS_PCI_ADDR;
    LibAmdPciRead (AccessWidth32, PciAddress, &NbCaps, StdHeader); // D18F3xE8
    if (NbCaps.HtcCapable == 1) {
      PciAddress.AddressValue = HTC_PCI_ADDR;
      LibAmdPciRead (AccessWidth32, PciAddress, &HtcReg, StdHeader); // D18F3x64
      if (HtcReg.HtcTmpLmt != 0) {
        PciAddress.AddressValue = CPTC2_PCI_ADDR;
        LibAmdPciRead (AccessWidth32, PciAddress, &ClkPwrTimingCtrl2, StdHeader); // D18F3xDC
        if (HtcReg.HtcPstateLimit > ClkPwrTimingCtrl2.HwPstateMaxVal) {
          ClkPwrTimingCtrl2.HwPstateMaxVal = HtcReg.HtcPstateLimit;
          LibAmdPciWrite (AccessWidth32, PciAddress, &ClkPwrTimingCtrl2, StdHeader); // D18F3xDC
        }
      }
    }
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Copies the contents of one P-State MSR to another.
 *
 * @param[in]  Dest              Destination p-state number
 * @param[in]  Src               Source p-state number
 * @param[in]  StdHeader         Config handle for library and services
 *
 */
VOID
STATIC
F15KvPmPwrChkCopyPstate (
  IN       UINT8 Dest,
  IN       UINT8 Src,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT64 LocalMsrRegister;

  LibAmdMsrRead ((UINT32) (PS_REG_BASE + Src), &LocalMsrRegister, StdHeader);
  LibAmdMsrWrite ((UINT32) (PS_REG_BASE + Dest), &LocalMsrRegister, StdHeader);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Core-level transition Pstate
 *
 * Transitions the executing core to the desired P-state.
 *
 * @param[in]  StateNumber          The new P-State to make effective.
 * @param[in]  StdHeader            Config handle for library and services.
 *
 */
VOID
STATIC
F15KvTransitionPstateCore (
  IN       VOID *StateNumber,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  BOOLEAN               WaitForTransition;
  CPU_SPECIFIC_SERVICES *FamilySpecificServices;

  GetCpuServicesOfCurrentCore (&FamilySpecificServices, StdHeader);

  WaitForTransition = IsCoreComputeUnitPrimary (FirstCoreIsComputeUnitPrimary, StdHeader) ? TRUE : FALSE;

  FamilySpecificServices->TransitionPstate (FamilySpecificServices, *((UINT8 *) StateNumber), WaitForTransition, StdHeader);
}
