/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family_15 Kaveri Pstate feature support functions.
 *
 * Provides the functions necessary to initialize the Pstate feature.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x15/KV
 * @e \$Revision: 290541 $   @e \$Date: 2014-04-23 10:15:07 -0500 (Wed, 23 Apr 2014) $
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
#include "GeneralServices.h"
#include "cpuPstateTables.h"
#include "cpuRegisters.h"
#include "Table.h"
#include "cpuFamilyTranslation.h"
#include "cpuFamRegisters.h"
#include "cpuF15Utilities.h"
#include "cpuF15KvUtilities.h"
#include "cpuF15PowerMgmt.h"
#include "cpuF15KvPowerMgmt.h"
#include "CommonReturns.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_CPU_FAMILY_0X15_KV_CPUF15KVPSTATE_FILECODE


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
F15KvGetPowerStepValueInTime (
  IN OUT   UINT32  *PowerStepPtr
  );

VOID
STATIC
F15KvGetPllValueInTime (
  IN OUT   UINT32  *PllLockTimePtr
  );

AGESA_STATUS
STATIC
F15KvGetFrequencyXlatRegInfo (
  IN       PSTATE_CPU_FAMILY_SERVICES  *PstateCpuServices,
  IN       UINT8                  PStateNumber,
  IN       UINT32                 Frequency,
     OUT   UINT32                 *CpuFidPtr,
     OUT   UINT32                 *CpuDidPtr1,
     OUT   UINT32                 *CpuDidPtr2,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );

AGESA_STATUS
F15KvGetPstateTransLatency (
  IN       PSTATE_CPU_FAMILY_SERVICES *PstateCpuServices,
  IN       PSTATE_LEVELING        *PStateLevelingBufferStructPtr,
  IN       PCI_ADDR               *PciAddress,
     OUT   UINT32                 *TransitionLatency,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );

AGESA_STATUS
F15KvGetPstateFrequency (
  IN       PSTATE_CPU_FAMILY_SERVICES *PstateCpuServices,
  IN       UINT8 StateNumber,
     OUT   UINT32 *FrequencyInMHz,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

AGESA_STATUS
F15KvGetPstatePower (
  IN       PSTATE_CPU_FAMILY_SERVICES *PstateCpuServices,
  IN       UINT8 StateNumber,
     OUT   UINT32 *PowerInMw,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

AGESA_STATUS
F15KvGetPstateMaxState (
  IN       PSTATE_CPU_FAMILY_SERVICES *PstateCpuServices,
     OUT   UINT32              *MaxPStateNumber,
     OUT   UINT8               *NumberOfBoostStates,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  );

AGESA_STATUS
F15KvGetPstateRegisterInfo (
  IN       PSTATE_CPU_FAMILY_SERVICES *PstateCpuServices,
  IN       UINT32              PState,
     OUT   BOOLEAN             *PStateEnabled,
  IN OUT   UINT32              *IddVal,
  IN OUT   UINT32              *IddDiv,
     OUT   UINT32              *SwPstateNumber,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  );



/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 *  Family specific call to check if Pstate PSD is dependent.
 *
 * @param[in]     PstateCpuServices  Pstate CPU services.
 * @param[in,out] PlatformConfig     Contains the runtime modifiable feature input data.
 * @param[in]     StdHeader          Config Handle for library, services.
 *
 * @retval       TRUE               PSD is dependent.
 * @retval       FALSE              PSD is independent.
 *
 */
BOOLEAN
STATIC
F15KvIsPstatePsdDependent (
  IN       PSTATE_CPU_FAMILY_SERVICES *PstateCpuServices,
  IN OUT   PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  BOOLEAN                PsdIsDependent;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F15KvIsPstatePsdDependent\n");

  // Family 15h Kaveri defaults to dependent PSD; allow Platform Configuration to
  // overwrite the default setting.
  PsdIsDependent = TRUE;
  if (PlatformConfig->ForcePstateIndependent == TRUE) {
    PsdIsDependent = FALSE;
  }

  IDS_HDT_CONSOLE (CPU_TRACE, "    P-state PSD is dependent: %d\n", PsdIsDependent);
  return PsdIsDependent;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Family specific call to get Pstate Transition Latency.
 *
 *  Calculate TransitionLatency by power step value and pll value.
 *
 *  @param[in]     PstateCpuServices                 Pstate CPU services.
 *  @param[in]     PStateLevelingBufferStructPtr     Pstate row data buffer pointer
 *  @param[in]     PciAddress                        Pci address
 *  @param[out]    TransitionLatency                 The transition latency.
 *  @param[in]     StdHeader                         Header for library and services
 *
 *  @retval      AGESA_SUCCESS Always succeeds.
 */
AGESA_STATUS
F15KvGetPstateTransLatency (
  IN       PSTATE_CPU_FAMILY_SERVICES *PstateCpuServices,
  IN       PSTATE_LEVELING        *PStateLevelingBufferStructPtr,
  IN       PCI_ADDR               *PciAddress,
     OUT   UINT32                 *TransitionLatency,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT32               CpuFid;
  UINT32               HwP0CpuFid;
  UINT32               PowerStepDown;
  UINT32               PowerStepUp;
  UINT32               PllLockTime;
  UINT32               LocalPciRegister;
  UINT32               k;
  UINT32               PStateMaxValueOnCurrentCore;
  UINT32               TransAndBusMastLatency;
  UINT64               MsrData;
  BOOLEAN              CpuFidSameFlag;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F15KvGetPstateTransLatency\n");

  CpuFidSameFlag = TRUE;
  HwP0CpuFid = 0;

  // Get PStateMaxValue
  PciAddress->Address.Register = CPTC2_REG;
  PciAddress->Address.Function = FUNC_3;
  LibAmdPciRead (AccessWidth32, *PciAddress, &LocalPciRegister, StdHeader);
  PStateMaxValueOnCurrentCore = ((CLK_PWR_TIMING_CTRL2_REGISTER *) &LocalPciRegister)->PstateMaxVal;

  //
  //Check if MSRC001_00[6B:64][CpuFid] is the same value for all P-states
  //
  for (k = 0; k <= PStateMaxValueOnCurrentCore; k++) {
    LibAmdMsrRead ((MSR_PSTATE_0 + k), &MsrData, StdHeader);
    CpuFid = (UINT32) (((PSTATE_MSR *) &MsrData)->CpuFid);
    if (k == 0) {
      HwP0CpuFid = CpuFid;
    }
    if (HwP0CpuFid != CpuFid) {
      CpuFidSameFlag = FALSE;
      break;
    }
  }

  PciAddress->Address.Register = CPTC0_REG;
  PciAddress->Address.Function = FUNC_3;
  LibAmdPciRead (AccessWidth32, *PciAddress, &LocalPciRegister, StdHeader);

  // PowerStepDown - Bits 20:23
  PowerStepDown = ((CLK_PWR_TIMING_CTRL_REGISTER *) &LocalPciRegister)->PowerStepDown;

  // PowerStepUp   - Bits 24:27
  PowerStepUp = ((CLK_PWR_TIMING_CTRL_REGISTER *) &LocalPciRegister)->PowerStepUp;

  // Convert the raw numbers in TempVar8_a and TempVar8_b into time
  F15KvGetPowerStepValueInTime (&PowerStepDown);
  F15KvGetPowerStepValueInTime (&PowerStepUp);

  //
  //(15 * (F3xD4[PowerStepDown] + F3xD4[PowerStepUp]) /1000) us
  //
  TransAndBusMastLatency =
  (15 * (PowerStepDown + PowerStepUp) + 999) / 1000;

  if (!CpuFidSameFlag) {
    //
    //+ F3xA0[PllLockTime]
    //
    PciAddress->Address.Register = PW_CTL_MISC_REG;
    LibAmdPciRead (AccessWidth32, *PciAddress, &LocalPciRegister, StdHeader);

    PllLockTime = ((POWER_CTRL_MISC_REGISTER *) &LocalPciRegister)->PllLockTime;
    F15KvGetPllValueInTime (&PllLockTime);
    TransAndBusMastLatency += PllLockTime;
  }

  *TransitionLatency = TransAndBusMastLatency;

  return (AGESA_SUCCESS);
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Family specific call to calculates the frequency in megahertz of the desired P-state.
 *
 *  @param[in]   PstateCpuServices        Pstate CPU services.
 *  @param[in]   StateNumber              The P-State to analyze.
 *  @param[out]  FrequencyInMHz           The P-State's frequency in MegaHertz
 *  @param[in]   StdHeader                Header for library and services
 *
 *  @retval      AGESA_SUCCESS   Always Succeeds.
 */
AGESA_STATUS
F15KvGetPstateFrequency (
  IN       PSTATE_CPU_FAMILY_SERVICES *PstateCpuServices,
  IN       UINT8 StateNumber,
     OUT   UINT32 *FrequencyInMHz,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8 TempValue;
  UINT32 CpuDid;
  UINT32 CpuFid;
  UINT64 LocalMsrRegister;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F15KvGetPstateFrequency - P%d\n", StateNumber);

  ASSERT (StateNumber < NM_PS_REG);
  LibAmdMsrRead (PS_REG_BASE + (UINT32) StateNumber, &LocalMsrRegister, StdHeader);
  ASSERT (((PSTATE_MSR *) &LocalMsrRegister)->PsEnable == 1);
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
  *FrequencyInMHz = (100 * (CpuFid + 0x10) / TempValue);
  IDS_HDT_CONSOLE (CPU_TRACE, "    FrequencyInMHz=%d, CpuFid=%d, CpuDid=%d\n", *FrequencyInMHz, CpuFid, CpuDid);

  return (AGESA_SUCCESS);
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Family specific call to calculates the power in milliWatts of the desired P-state.
 *
 *  @param[in]     PstateCpuServices      Pstate CPU services.
 *  @param[in]   StateNumber              Which P-state to analyze
 *  @param[out]  PowerInMw                The Power in milliWatts of that P-State
 *  @param[in]   StdHeader                Header for library and services
 *
 *  @retval      AGESA_SUCCESS Always succeeds.
 */
AGESA_STATUS
F15KvGetPstatePower (
  IN       PSTATE_CPU_FAMILY_SERVICES *PstateCpuServices,
  IN       UINT8 StateNumber,
     OUT   UINT32 *PowerInMw,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32  CpuVid;
  UINT32  IddValue;
  UINT32  IddDiv;
  UINT32  V_x100000;
  UINT32  Power;
  UINT64  LocalMsrRegister;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F15KvGetPstatePower - P%d\n", StateNumber);

  ASSERT (StateNumber < NM_PS_REG);
  LibAmdMsrRead (PS_REG_BASE + (UINT32) StateNumber, &LocalMsrRegister, StdHeader);
  ASSERT (((PSTATE_MSR *) &LocalMsrRegister)->PsEnable == 1);
  CpuVid = (UINT32) (((PSTATE_MSR *) &LocalMsrRegister)->CpuVid);
  IddValue = (UINT32) (((PSTATE_MSR *) &LocalMsrRegister)->IddValue);
  IddDiv = (UINT32) (((PSTATE_MSR *) &LocalMsrRegister)->IddDiv);

  if (CpuVid >= 0xF8) {
    V_x100000 = 0;
  } else {
    V_x100000 = 155000L - (625L * CpuVid);
  }

  Power = V_x100000 * IddValue;

  switch (IddDiv) {
  case 0:
    *PowerInMw = Power / 100L;
    break;
  case 1:
    *PowerInMw = Power / 1000L;
    break;
  case 2:
    *PowerInMw = Power / 10000L;
    break;
  default:
    // IddDiv is set to an undefined value.  This is due to either a misfused CPU, or
    // an invalid P-state MSR write.
    ASSERT (FALSE);
    *PowerInMw = 0;
    break;
  }
  IDS_HDT_CONSOLE (CPU_TRACE, "  PowerInMw=%d, CpuVid=%d, IddValue=%d, IddDiv=%d\n", *PowerInMw, CpuVid, IddValue, IddDiv);

  return (AGESA_SUCCESS);
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Family specific call to get CPU pstate max state.
 *
 *  @param[in]     PstateCpuServices      Pstate CPU services.
 *  @param[out]    MaxPStateNumber        The max hw pstate value on the current socket.
 *  @param[out]    NumberOfBoostStates    The number of boosted P-states on the current socket.
 *  @param[in]     StdHeader              Handle of Header for calling lib functions and services.
 *
 *  @retval      AGESA_SUCCESS Always succeeds.
 */
AGESA_STATUS
F15KvGetPstateMaxState (
  IN       PSTATE_CPU_FAMILY_SERVICES *PstateCpuServices,
     OUT   UINT32              *MaxPStateNumber,
     OUT   UINT8               *NumberOfBoostStates,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32               NumBoostStates;
  UINT64               MsrValue;
  UINT32               LocalPciRegister;
  PCI_ADDR             PciAddress;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F15KvGetPstateMaxState\n");

  LocalPciRegister = 0;

  // For F15 Kaveri CPU, skip boosted p-state. The boosted p-state number = F4x15C[NumBoostStates].
  PciAddress.AddressValue = CPB_CTRL_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, &LocalPciRegister, StdHeader); // F4x15C

  NumBoostStates = ((CPB_CTRL_REGISTER *) &LocalPciRegister)->NumBoostStates;
  *NumberOfBoostStates = (UINT8) NumBoostStates;

  //
  // Read PstateMaxVal [6:4] from MSR C001_0061
  // So, we will know the max pstate state in this socket.
  //
  LibAmdMsrRead (MSR_PSTATE_CURRENT_LIMIT, &MsrValue, StdHeader);
  *MaxPStateNumber = (UINT32) (((PSTATE_CURLIM_MSR *) &MsrValue)->PstateMaxVal) + NumBoostStates;
  IDS_HDT_CONSOLE (CPU_TRACE, "    MaxPStateNumber=%d, NumBoostStates=%d\n", *MaxPStateNumber, NumBoostStates);

  return (AGESA_SUCCESS);
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Family specific call to get CPU pstate register information.
 *
 *  @param[in]     PstateCpuServices         Pstate CPU services.
 *  @param[in]     PState                    Input Pstate number for query.
 *  @param[out]    PStateEnabled             Boolean flag return pstate enable.
 *  @param[in,out] IddVal                    Pstate current value.
 *  @param[in,out] IddDiv                    Pstate current divisor.
 *  @param[out]    SwPstateNumber            Software P-state number.
 *  @param[in]     StdHeader                 Handle of Header for calling lib functions and services.
 *
 *  @retval      AGESA_SUCCESS Always succeeds.
 */
AGESA_STATUS
F15KvGetPstateRegisterInfo (
  IN       PSTATE_CPU_FAMILY_SERVICES *PstateCpuServices,
  IN       UINT32              PState,
     OUT   BOOLEAN             *PStateEnabled,
  IN OUT   UINT32              *IddVal,
  IN OUT   UINT32              *IddDiv,
     OUT   UINT32              *SwPstateNumber,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32    LocalPciRegister;
  UINT64    LocalMsrRegister;
  PCI_ADDR  PciAddress;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F15KvGetPstateRegisterInfo - P%d\n", PState);

  ASSERT (PState < NM_PS_REG);

  // For F15 Kaveri CPU, skip boosted p-state. The boosted p-state number = F4x15C[NumBoostStates].
  PciAddress.AddressValue = CPB_CTRL_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, &LocalPciRegister, StdHeader); // F4x15C

  // Read PSTATE MSRs
  LibAmdMsrRead (PS_REG_BASE + (UINT32) PState, &LocalMsrRegister, StdHeader);

  *SwPstateNumber = PState;

  if (((PSTATE_MSR *) &LocalMsrRegister)->PsEnable == 1) {
    // PState enable = bit 63
    *PStateEnabled = TRUE;
    //
    // Check input pstate belongs to Boosted-Pstate, if yes, return *PStateEnabled = FALSE.
    //
    if (PState < ((CPB_CTRL_REGISTER *) &LocalPciRegister)->NumBoostStates) {
      *PStateEnabled = FALSE;
    } else {
      *SwPstateNumber = PState - ((CPB_CTRL_REGISTER *) &LocalPciRegister)->NumBoostStates;
    }
    IDS_HDT_CONSOLE (CPU_TRACE, "  Pstate %d is enabled. SwPstateNumber=%d\n", PState, *SwPstateNumber);
  } else {
    *PStateEnabled = FALSE;
  }

  // Bits 39:32 (high 32 bits [7:0])
  *IddVal = (UINT32) ((PSTATE_MSR *) &LocalMsrRegister)->IddValue;
  // Bits 41:40 (high 32 bits [9:8])
  *IddDiv = (UINT32) ((PSTATE_MSR *) &LocalMsrRegister)->IddDiv;

  IDS_HDT_CONSOLE (CPU_TRACE, "    IddVal=%d, IddDiv=%d\n", *IddVal, *IddDiv);
  return (AGESA_SUCCESS);
}


CONST PSTATE_CPU_FAMILY_SERVICES ROMDATA F15KvPstateServices =
{
  0,
  (PF_PSTATE_PSD_IS_NEEDED) CommonReturnTrue,
  F15KvIsPstatePsdDependent,
  F15KvGetPstateTransLatency,
  F15KvGetPstateFrequency,
  (PF_CPU_SET_PSTATE_LEVELING_REG) CommonReturnAgesaSuccess,
  F15KvGetPstatePower,
  F15KvGetPstateMaxState,
  F15KvGetPstateRegisterInfo
};


/*---------------------------------------------------------------------------------------*/
/**
 *  Workaround for KV that must be applied just prior to microcode patch being loaded.
 *
 *  @param[in]     FamilyServices         The current Family Specific Services.
 *  @param[in]     EarlyParams            Service parameters.
 *  @param[in]     StdHeader              Config handle for library and services.
 *
 */
VOID
F15KvApplyVddWorkaroundAtEarly (
  IN       CPU_SPECIFIC_SERVICES  *FamilyServices,
  IN       AMD_CPU_EARLY_PARAMS   *EarlyParams,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT32                                CurrentPstate;
  UINT32                                MaxDefinedPstate;
  UINT32                                MaxUnaffectedPstate;
  PCI_ADDR                              PciAddr;
  PSTATE_MSR                            PstateRegister;
  PSTATE_MSR                            PrevPstate;
  AGESA_STATUS                          IgnoredSts;
  HTC_REGISTER                          Htc;
  NB_CAPS_REGISTER                      NbCaps;
  CPB_CTRL_REGISTER                     CpbCtrl;
  MISC_VOLTAGE_REGISTER                 MiscVolt;
  CLK_PWR_TIMING_CTRL1_REGISTER         Cptc1;
  F15_KV_CLK_PWR_TIMING_CTRL2_REGISTER  Cptc2;

  if (IsBsp (StdHeader, &IgnoredSts)) {
    PciAddr.AddressValue = CPTC1_PCI_ADDR;
    LibAmdPciRead (AccessWidth32, PciAddr, (VOID *) &Cptc1, StdHeader);
    if (Cptc1.Reserved2 == 0) {
      MaxUnaffectedPstate = 0;
      PrevPstate.CpuVid = 0;
      PciAddr.AddressValue = MISC_VOLTAGES_PCI_ADDR;
      LibAmdPciRead (AccessWidth32, PciAddr, (VOID *) &MiscVolt, StdHeader);
      PciAddr.AddressValue = CPB_CONTROL_REG_PCI_ADDR;
      LibAmdPciRead (AccessWidth32, PciAddr, (VOID *) &CpbCtrl, StdHeader);
      PciAddr.AddressValue = CPTC2_PCI_ADDR;
      LibAmdPciRead (AccessWidth32, PciAddr, (VOID *) &Cptc2, StdHeader);

      MaxDefinedPstate = Cptc2.HwPstateMaxVal;
      PciAddr.AddressValue = NB_CAPS_PCI_ADDR;
      LibAmdPciRead (AccessWidth32, PciAddr, (VOID *) &NbCaps, StdHeader);
      if (NbCaps.HtcCapable == 1) {
        PciAddr.AddressValue = HTC_PCI_ADDR;
        LibAmdPciRead (AccessWidth32, PciAddr, (VOID *) &Htc, StdHeader);
        if ((Htc.HtcTmpLmt != 0) && (Htc.HtcPstateLimit > Cptc2.HwPstateMaxVal)) {
          MaxDefinedPstate = Htc.HtcPstateLimit;
        }
      }

      for (CurrentPstate = 0; CurrentPstate <= MaxDefinedPstate; CurrentPstate++) {
        LibAmdMsrRead (PS_REG_BASE + CurrentPstate, (UINT64 *) &PstateRegister, StdHeader);
        if ((PstateRegister.CpuVid < (MiscVolt.MaxVid + 4)) ||
            ((CurrentPstate != 0) && (MaxUnaffectedPstate == CurrentPstate) && (PstateRegister.CpuVid < (PrevPstate.CpuVid + 4))) ||
            ((CpbCtrl.BoostLock == 1) && (CurrentPstate < CpbCtrl.NumBoostStates))) {
          MaxUnaffectedPstate = CurrentPstate + 1;
        } else {
          PstateRegister.CpuVid -= 4;
          LibAmdMsrWrite (PS_REG_BASE + CurrentPstate, (UINT64 *) &PstateRegister, StdHeader);
        }
        PrevPstate = PstateRegister;
      }
      PciAddr.AddressValue = CPTC1_PCI_ADDR;
      Cptc1.Reserved1 = MaxUnaffectedPstate;
      Cptc1.Reserved2 = 1;
      LibAmdPciWrite (AccessWidth32, PciAddr, (VOID *) &Cptc1, StdHeader);
    }
  }
}


/*---------------------------------------------------------------------------------------
 *                          L O C A L    F U N C T I O N S
 *---------------------------------------------------------------------------------------
 */


/**
 *---------------------------------------------------------------------------------------
 *
 *  F15KvGetPowerStepValueInTime
 *
 *  Description:
 *    Convert power step value in time
 *
 *  Parameters:
 *    @param[out]     *PowerStepPtr
 *
 *    @retval       VOID
 *
 *---------------------------------------------------------------------------------------
 **/
VOID
STATIC
F15KvGetPowerStepValueInTime (
  IN OUT   UINT32  *PowerStepPtr
  )
{
  UINT32 TempVar_a;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F15KvGetPowerStepValueInTime\n");

  TempVar_a = *PowerStepPtr;

  if (TempVar_a < 0x4) {
    *PowerStepPtr = 400 - (TempVar_a * 100);
  } else if (TempVar_a <  0x9) {
    *PowerStepPtr = 130 - (TempVar_a * 10);
  } else {
    *PowerStepPtr = 90 - (TempVar_a * 5);
  }
  IDS_HDT_CONSOLE (CPU_TRACE, "  PowerStepPtr=%d\n", *PowerStepPtr);
}

/**
 *---------------------------------------------------------------------------------------
 *
 *  F15KvGetPllValueInTime
 *
 *  Description:
 *    Convert PLL Value in time
 *
 *  Parameters:
 *    @param[out]     *PllLockTimePtr
 *
 *    @retval       VOID
 *
 *---------------------------------------------------------------------------------------
 **/
VOID
STATIC
F15KvGetPllValueInTime (
  IN OUT   UINT32  *PllLockTimePtr
  )
{
  IDS_HDT_CONSOLE (CPU_TRACE, "  F15KvGetPllValueInTime\n");

  if (*PllLockTimePtr < 4) {
    *PllLockTimePtr = *PllLockTimePtr + 1;
  } else if (*PllLockTimePtr == 4) {
    *PllLockTimePtr = 8;
  } else if (*PllLockTimePtr == 5) {
    *PllLockTimePtr = 16;
  } else
    *PllLockTimePtr = 0;
  IDS_HDT_CONSOLE (CPU_TRACE, "    PllLockTimePtr=%d\n", *PllLockTimePtr);
}

/*---------------------------------------------------------------------------------------*/
/**
 *    This function will return the CpuFid and CpuDid in MHz, using the formula
 *    described in the BKDG MSRC001_00[68:64] P-State [4:0] Registers:bit 8:0
 *
 *    @param[in]       PstateCpuServices        The current Family Specific Services.
 *    @param[in]       PStateNumber             P-state number to check.
 *    @param[in]       Frequency                Leveled target frequency for PStateNumber.
 *    @param[out]      *CpuFidPtr               New leveled FID.
 *    @param[out]      *CpuDidPtr1              New leveled DID info 1.
 *    @param[out]      *CpuDidPtr2              New leveled DID info 2.
 *    @param[in]       *StdHeader               Header for library and services.
 *
 *    @retval       AGESA_WARNING           This P-State does not need to be modified.
 *    @retval       AGESA_SUCCESS           This P-State must be modified to be level.
 */
AGESA_STATUS
STATIC
F15KvGetFrequencyXlatRegInfo (
  IN       PSTATE_CPU_FAMILY_SERVICES  *PstateCpuServices,
  IN       UINT8                  PStateNumber,
  IN       UINT32                 Frequency,
     OUT   UINT32                 *CpuFidPtr,
     OUT   UINT32                 *CpuDidPtr1,
     OUT   UINT32                 *CpuDidPtr2,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT32 i;
  UINT32 j;
  AGESA_STATUS Status;
  UINT32 FrequencyInMHz;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F15KvGetFrequencyXlatRegInfo - PstateNumber=%d, Frequency=%d\n", PStateNumber, Frequency);

  FrequencyInMHz = 0;
  *CpuDidPtr2 = 0xFFFF;

  Status = AGESA_SUCCESS;

  PstateCpuServices->GetPstateFrequency (PstateCpuServices, PStateNumber, &FrequencyInMHz, StdHeader);
  if (FrequencyInMHz == Frequency) {
    Status |= AGESA_WARNING;
  }

  // CPU Frequency = 100 MHz * (CpuFid + 10h) / (2^CpuDid)
  // In this for loop i = 2^CpuDid


  for (i = 1; i < 17; (i += i)) {
    for (j = 0; j < 64; j++) {
      if (Frequency == ((100 * (j + 0x10)) / i )) {
        *CpuFidPtr = j;
        if (i == 1) {
          *CpuDidPtr1 = 0;
        } else if (i == 2) {
          *CpuDidPtr1 = 1;
        } else if (i == 4) {
          *CpuDidPtr1 = 2;
        } else if (i == 8) {
          *CpuDidPtr1 = 3;
        } else if (i == 16) {
          *CpuDidPtr1 = 4;
        } else {
          *CpuFidPtr = 0xFFFF;
          *CpuDidPtr1 = 0xFFFF;
        }
        IDS_HDT_CONSOLE (CPU_TRACE, "  CpuFidPtr=%d, CpuDidPtr1=0x%x, CpuDidPtr2=0x%x\n", *CpuFidPtr, *CpuDidPtr1, *CpuDidPtr2);
        // Success
        return Status;
      }
    }
  }

  // Error Condition
  *CpuFidPtr = 0x00FF;
  *CpuDidPtr1 = 0x00FF;
  *CpuDidPtr2 = 0x00FF;

  IDS_HDT_CONSOLE (CPU_TRACE, "  CpuFidPtr=%d, CpuDidPtr1=0x%x, CpuDidPtr2=0x%x\n", *CpuFidPtr, *CpuDidPtr1, *CpuDidPtr2);
  return AGESA_ERROR;
}

