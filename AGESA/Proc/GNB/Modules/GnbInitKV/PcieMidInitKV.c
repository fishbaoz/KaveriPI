/* $NoKeywords:$ */
/**
 * @file
 *
 * PCIe mid post initialization.
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
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
* ***************************************************************************
*
*/
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include  "AGESA.h"
#include  "amdlib.h"
#include  "Ids.h"
#include  "Gnb.h"
#include  "GnbPcie.h"
#include  "GnbPcieConfig.h"
#include  "GnbPcieInitLibV1.h"
#include  "GnbPcieInitLibV4.h"
#include  "GnbPcieInitLibV5.h"
#include  "GnbFamServices.h"
#include  "GnbRegistersKV.h"
#include  "PcieLibKV.h"
#include  "GnbLibPciAcc.h"
#include  "PciePowerGateKV.h"
#include  "PciePortServicesV4.h"
#include  "cpuFamilyTranslation.h"
#include  "PcieComplexDataKV.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITKV_PCIEMIDINITKV_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

extern CONST PCIE_PORT_REGISTER_TABLE_HEADER ROMDATA PortInitMidTableKV;

#define ABS(X) (((X) < 0) ? (-(X)) : (X))

#define EI_CYCLING_TIME     10
#define PLL_RAMP_UP_TIME    22
#define LS2_EXIT_TIME       1
#define RECOVER_TO_L0_TIME  5

#define NUMBER_OF_LANES     40
#define NUMBER_OF_WRAPPERS  4
#define NUMBER_OF_SUBLINKS  NUMBER_OF_LANES / 4


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

/// PCIe Sublink Characterization Structure
typedef struct {
  UINT8   GppPortCount;             ///< Count of Gpp ports on a given sublink
  UINT8   MaxGenCapability;         ///< Max gen capability of ports on a given sublink
} PCIe_SUBLINK_INFO;

/// PCIe Wrapper Characterization Structure
typedef struct {
  UINT8   MinAspmL1ExitLatency;     ///< Minimum ASPM L1 Exit Latency on wrapper
  UINT8   ActualL1Latency;          ///< Actual L1 Latency value for wrapper
  UINT8   L1ExitLatencyValue;       ///< Register value to program for L1ExitLatency
  BOOLEAN AnyDevFailPllpdnb;        ///< Any device on wrapper cannot support PLLPDNB
  BOOLEAN DisableL1OnWrapper;       ///< Disable L1 on all ports on this wrapper
} PCIe_WRAPPER_INFO;

/// PCIe Characterization Buffer
typedef struct {
  PCIe_SUBLINK_INFO SublinkInfo[NUMBER_OF_SUBLINKS];
  PCIe_WRAPPER_INFO WrapperInfo[NUMBER_OF_WRAPPERS];
} PCIe_INFO_BUFFER;


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 * Callback to characterize all active ports / sublinks
 *
 *
 *
 *
 * @param[in]       Engine          Pointer to engine config descriptor
 * @param[in, out]  Buffer
 * @param[in]       Pcie            Pointer to global PCIe configuration
 *
 */

VOID
STATIC
PcieMidPortCharacterizationCallbackKV (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIe_SUBLINK_INFO       *SublinkInfo;
  PCIe_WRAPPER_INFO       *WrapperInfo;
  PCIe_ASPM_LATENCY_INFO  LinkLatencyInfo;
  DxFxxE4_xA4_STRUCT      DxFxxE4_xA4;

  SublinkInfo = &(((PCIe_INFO_BUFFER *)Buffer)->SublinkInfo[MIN (Engine->EngineData.StartLane, Engine->EngineData.EndLane) / 4]);
  WrapperInfo = &(((PCIe_INFO_BUFFER *)Buffer)->WrapperInfo[PcieConfigGetParentWrapper (Engine)->WrapId]);

  LinkLatencyInfo.MaxL1ExitLatency = 255;
  LinkLatencyInfo.MaxL0sExitLatency = 255;
  // Count x1/x2 GPP port on each sublink
  if (ABS (Engine->EngineData.StartLane - Engine->EngineData.EndLane) <= 2) {
    SublinkInfo->GppPortCount += 1;
  }
  // Determine max gen capability of this engine and sublink
  if (PcieConfigCheckPortStatus (Engine, INIT_STATUS_PCIE_TRAINING_SUCCESS)) {
    // Port is trained, use device capability
    DxFxxE4_xA4.Value = PciePortRegisterRead (Engine, DxFxxE4_xA4_ADDRESS, Pcie);
    if (DxFxxE4_xA4.Field.LcOtherSideSupportsGen2 == 1) {
      SublinkInfo->MaxGenCapability = Gen2;
    }
    // Determine minimum L1 exit latency for this engine
    PcieAspmGetMinExitLatencyV5 (Engine->Type.Port.Address, &LinkLatencyInfo, GnbLibGetHeader (Pcie));
    if (LinkLatencyInfo.MaxL1ExitLatency < WrapperInfo->MinAspmL1ExitLatency) {
      WrapperInfo->MinAspmL1ExitLatency = LinkLatencyInfo.MaxL1ExitLatency;
    }
  } else {
    // Hotplug port - use max supported speed
    if (Engine->Type.Port.PortData.LinkSpeedCapability > SublinkInfo->MaxGenCapability) {
      SublinkInfo->MaxGenCapability = Engine->Type.Port.PortData.LinkSpeedCapability;
    }
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback to characterize all wrappers
 *
 *
 *
 *
 * @param[in]       Wrapper         Pointer to wrapper config descriptor
 * @param[in, out]  Buffer
 * @param[in]       Pcie            Pointer to global PCIe configuration
 * @retval          AGESA_SUCCESS   Topology successfully mapped
 *
 */

AGESA_STATUS
STATIC
PcieMidWrapperCharacterizationCallbackKV (
  IN       PCIe_WRAPPER_CONFIG   *Wrapper,
  IN       VOID                  *Buffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIe_WRAPPER_INFO       *WrapperInfo;
  UINT8                   Pif;
  D0F0xE4_PIF_0012_STRUCT D0F0xE4_PIF_0012;
  UINT8                   ActualL1Latency;

  WrapperInfo = &(((PCIe_INFO_BUFFER *)Buffer)->WrapperInfo[Wrapper->WrapId]);

  WrapperInfo->ActualL1Latency = EI_CYCLING_TIME + PLL_RAMP_UP_TIME + LS2_EXIT_TIME + RECOVER_TO_L0_TIME;
  if (WrapperInfo->MinAspmL1ExitLatency < WrapperInfo->ActualL1Latency) {
    WrapperInfo->ActualL1Latency -= EI_CYCLING_TIME;
    for (Pif = 0; Pif < Wrapper->NumberOfPIFs; Pif++) {
      PcieRegisterRMW (
        Wrapper,
        PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0010_ADDRESS),
        D0F0xE4_PIF_0010_EiDetCycleMode_MASK,
        (0 << D0F0xE4_PIF_0010_EiDetCycleMode_OFFSET),
        TRUE,
        Pcie
        );
    }
  }
  if (WrapperInfo->MinAspmL1ExitLatency < WrapperInfo->ActualL1Latency) {
    WrapperInfo->ActualL1Latency -= PLL_RAMP_UP_TIME;
    D0F0xE4_PIF_0012.Value = PcieRegisterRead (
                               Wrapper,
                               PIF_SPACE (Wrapper->WrapId, 0, D0F0xE4_PIF_0012_ADDRESS),
                               Pcie
                               );
    D0F0xE4_PIF_0012.Field.PllPowerStateInOff = PifPowerStateL0;
    D0F0xE4_PIF_0012.Field.PllPowerStateInTxs2 = PifPowerStateL0;

    for (Pif = 0; Pif < Wrapper->NumberOfPIFs; Pif++) {
      PcieRegisterWrite (
        Wrapper,
        PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0012_ADDRESS),
        D0F0xE4_PIF_0012.Value,
        TRUE,
        Pcie
      );
      PcieRegisterWrite (
        Wrapper,
        PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0012_ADDRESS + 1),
        D0F0xE4_PIF_0012.Value,
        TRUE,
        Pcie
      );
    }
    WrapperInfo->AnyDevFailPllpdnb = TRUE;
  }
  if (WrapperInfo->MinAspmL1ExitLatency < WrapperInfo->ActualL1Latency) {
    WrapperInfo->DisableL1OnWrapper = TRUE;
  }
  ActualL1Latency = WrapperInfo->ActualL1Latency >> 1;
  WrapperInfo->L1ExitLatencyValue = 1;
  while (ActualL1Latency != 0) {
    ActualL1Latency = ActualL1Latency >> 1;
    WrapperInfo->L1ExitLatencyValue++;
  }
  return (AGESA_SUCCESS);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Pcie Mid Characterization
 *
 *   Mid PCIe Characterization
 *
 * @param[in, out]  PcieInfoBuffer      Pointer to PCIe characterization info
 * @param[in]       Pcie                Pointer to global PCIe configuration
 * @retval          AGESA_SUCCESS       Topology successfully mapped
 * @retval          AGESA_ERROR         Topology can not be mapped
 */

AGESA_STATUS
STATIC
PcieMidCharacterizationKV (
  IN OUT   PCIe_INFO_BUFFER      *PcieInfoBuffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  AGESA_STATUS          AgesaStatus;
  AGESA_STATUS          Status;
  UINT8                 Index;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieMidCharacterizationKV Enter\n");
  AgesaStatus = AGESA_SUCCESS;

  // Set initial stats for all PcieInfoBuffer fields
  for (Index = 0; Index < NUMBER_OF_SUBLINKS; Index++) {
    ((PCIe_INFO_BUFFER *) PcieInfoBuffer)->SublinkInfo[Index].GppPortCount = 0;
    ((PCIe_INFO_BUFFER *) PcieInfoBuffer)->SublinkInfo[Index].MaxGenCapability = Gen1;
  }
  for (Index = 0; Index < NUMBER_OF_WRAPPERS; Index++) {
    ((PCIe_INFO_BUFFER *) PcieInfoBuffer)->WrapperInfo[Index].MinAspmL1ExitLatency = 255;
    ((PCIe_INFO_BUFFER *) PcieInfoBuffer)->WrapperInfo[Index].AnyDevFailPllpdnb = FALSE;
    ((PCIe_INFO_BUFFER *) PcieInfoBuffer)->WrapperInfo[Index].DisableL1OnWrapper = FALSE;
  }

  PcieConfigRunProcForAllEngines (
    DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
    PcieMidPortCharacterizationCallbackKV,
    (VOID *) PcieInfoBuffer,
    Pcie
    );

  Status = PcieConfigRunProcForAllWrappers (
             DESCRIPTOR_ALL_WRAPPERS,
             PcieMidWrapperCharacterizationCallbackKV,
             (VOID *) PcieInfoBuffer,
             Pcie);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieMidCharacterizationKV Exit [0x%x]\n", AgesaStatus);
  return  AgesaStatus;
}



/*----------------------------------------------------------------------------------------*/
/**
 * Callback to init various features on all active ports
 *
 *
 *
 *
 * @param[in]       Engine          Pointer to engine config descriptor
 * @param[in, out]  Buffer          Pointer to PCIe_INFO_BUFFER populated by characterization
 * @param[in]       Pcie            Pointer to global PCIe configuration
 *
 */

VOID
STATIC
PcieMidPortInitCallbackKV (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  DxFxx68_STRUCT          DxFxx68;
  D0F0xE4_PIF_0012_STRUCT D0F0xE4_PIF_0012;
  PCIe_SUBLINK_INFO       *SublinkInfo;
  PCIe_WRAPPER_INFO       *WrapperInfo;
  PCIe_WRAPPER_CONFIG     *Wrapper;
  CPU_LOGICAL_ID          LogicalId;
  UINT8                   Count;
  UINT8                   Nibble;

  PciePortProgramRegisterTable (PortInitMidTableKV.Table, PortInitMidTableKV.Length, Engine, TRUE, Pcie);
  if (PcieConfigCheckPortStatus (Engine, INIT_STATUS_PCIE_TRAINING_SUCCESS) || Engine->Type.Port.PortData.LinkHotplug != HotplugDisabled) {
    PcieEnableSlotPowerLimitV5 (Engine, Pcie);
    if (GnbFmCheckIommuPresent ((GNB_HANDLE*) PcieConfigGetParentSilicon (Engine), GnbLibGetHeader (Pcie))) {
      PcieInitPortForIommuV4 (Engine, Pcie);
    }
    // After GFX link is trained up and before ASPM is enabled, AGESA needs to check link width,
    // if it equals to x16, then apply the following change to GFX port:
    // Per port register 0xA1 - PCIE LC TRAINING CONTROL, bit16 - LC_EXTEND_WAIT_FOR_SKP = 1
    GnbLibPciRead (
      Engine->Type.Port.Address.AddressValue | DxFxx68_ADDRESS,
      AccessWidth32,
      &DxFxx68,
      GnbLibGetHeader (Pcie)
      );
    if (DxFxx68.Field.NegotiatedLinkWidth == 16) {
      PciePortRegisterRMW (
        Engine,
        DxFxxE4_xA1_ADDRESS,
        DxFxxE4_xA1_LcExtendWaitForSkp_MASK,
        (1 << DxFxxE4_xA1_LcExtendWaitForSkp_OFFSET),
        TRUE,
        Pcie
        );
    }
  }

  Wrapper = PcieConfigGetParentWrapper (Engine);
  SublinkInfo = &(((PCIe_INFO_BUFFER *)Buffer)->SublinkInfo[MIN (Engine->EngineData.StartLane, Engine->EngineData.EndLane) / 4]);
  WrapperInfo = &(((PCIe_INFO_BUFFER *)Buffer)->WrapperInfo[Wrapper->WrapId]);
  GetLogicalIdOfCurrentCore (&LogicalId, (AMD_CONFIG_PARAMS *)Pcie->StdHeader);
  // Check if this CPU is KV A0
  // UBTS468566
  if ((LogicalId.Revision & AMD_F15_KV_A0) != 0) {

    Count = SublinkInfo->GppPortCount;
    IDS_HDT_CONSOLE (GNB_TRACE, "x1x2 PortCount = %02x\n", Count);

    if (Count == 2) {
      // If number of GPP ports under the same sublink is 2, Delay L1 Exit (prolong minimum time spent in L1)
      PciePortRegisterRMW (
        Engine,
        DxFxxE4_xA0_ADDRESS,
        DxFxxE4_xA0_LcDelayCount_MASK |
        DxFxxE4_xA0_LcDelayL1Exit_MASK,
        (0 << DxFxxE4_xA0_LcDelayCount_OFFSET) |
        (1 << DxFxxE4_xA0_LcDelayL1Exit_OFFSET),
        TRUE,
        Pcie
        );
    } else if (Count > 2) {
      // If number of GPP ports > 2
      if (SublinkInfo->MaxGenCapability > Gen1) {
        // If at least 1 GPP is Gen2 capable, Disable PLL Power down feature
        Wrapper = PcieConfigGetParentWrapper (Engine);
        Nibble = (UINT8) ((MIN (Engine->EngineData.StartLane, Engine->EngineData.EndLane) % 8) / 4);
        // Only PSD and PPD can have x1/x2 links, so we assume that PIF number is always 0
        D0F0xE4_PIF_0012.Value = PcieRegisterRead (
                                   Wrapper,
                                   PIF_SPACE (Wrapper->WrapId, 0, D0F0xE4_PIF_0012_ADDRESS + Nibble),
                                   Pcie
                                   );

        D0F0xE4_PIF_0012.Field.PllPowerStateInOff = PifPowerStateL0;
        D0F0xE4_PIF_0012.Field.PllPowerStateInTxs2 = PifPowerStateL0;

        PcieRegisterWrite (
          Wrapper,
          PIF_SPACE (Wrapper->WrapId, 0, D0F0xE4_PIF_0012_ADDRESS + Nibble),
          D0F0xE4_PIF_0012.Value,
          TRUE,
          Pcie
          );
      } else {
        // All ports are only Gen1
        PciePortRegisterRMW (
          Engine,
          DxFxxE4_xC0_ADDRESS,
          DxFxxE4_xC0_StrapMedyTSxCount_MASK,
          0x2 << DxFxxE4_xC0_StrapMedyTSxCount_OFFSET,
          TRUE,
          Pcie
          );
      }
    }
  }
  PcieRegisterRMW (
    Wrapper,
    WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_0802_ADDRESS + (0x100 * Engine->Type.Port.PortId)),
    D0F0xE4_WRAP_0802_StrapBifL1ExitLatency_MASK,
    (WrapperInfo->L1ExitLatencyValue << D0F0xE4_WRAP_0802_StrapBifL1ExitLatency_OFFSET),
    TRUE,
    Pcie
    );
  if (WrapperInfo->DisableL1OnWrapper == TRUE) {
    Engine->Type.Port.PortData.LinkAspm &= ~(AspmL1);
  }
  PcieEnableAspm (Engine,  Pcie);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Master procedure to init various features on all active ports
 *
 *
 *
 *
 * @param[in, out]  PcieInfoBuffer      Pointer to PCIe characterization info
 * @param[in]       Pcie                Pointer to global PCIe configuration
 * @retval          AGESA_STATUS
 *
 */

AGESA_STATUS
STATIC
PcieMidPortInitKV (
  IN OUT   PCIe_INFO_BUFFER      *PcieInfoBuffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  AGESA_STATUS         Status;
  PCIE_LINK_SPEED_CAP  GlobalSpeedCap;

  Status = AGESA_SUCCESS;

  PcieConfigRunProcForAllEngines (
    DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
    PcieMidPortInitCallbackKV,
    PcieInfoBuffer,
    Pcie
    );

  GlobalSpeedCap = PcieUtilGlobalGenCapability (
                     PCIE_PORT_GEN_CAP_BOOT | PCIE_GLOBAL_GEN_CAP_TRAINED_PORTS | PCIE_GLOBAL_GEN_CAP_HOTPLUG_PORTS,
                     Pcie
                     );

  PcieSetVoltageKV (GlobalSpeedCap, Pcie);
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Per wrapper Pcie Late Init.
 *
 *
 * @param[in]  Wrapper         Pointer to wrapper configuration descriptor
 * @param[in]  Buffer          Pointer buffer
 * @param[in]  Pcie            Pointer to global PCIe configuration
 */
AGESA_STATUS
STATIC
PcieMidInitCallbackKV (
  IN       PCIe_WRAPPER_CONFIG           *Wrapper,
  IN OUT   VOID                          *Buffer,
  IN       PCIe_PLATFORM_CONFIG          *Pcie
  )
{
  PciePwrPowerDownUnusedLanesKV (Wrapper, Pcie);
  PciePwrClockGatingV5 (Wrapper, Pcie);
  PcieLockRegisters (Wrapper, Pcie);

  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Pcie Late Init
 *
 *   Late PCIe initialization
 *
 * @param[in, out]  PcieInfoBuffer      Pointer to PCIe characterization info
 * @param[in]       Pcie                Pointer to global PCIe configuration
 * @retval          AGESA_SUCCESS       Topology successfully mapped
 * @retval          AGESA_ERROR         Topology can not be mapped
 */

AGESA_STATUS
STATIC
PcieMidInitKV (
  IN OUT   PCIe_INFO_BUFFER      *PcieInfoBuffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  AGESA_STATUS          AgesaStatus;
  AGESA_STATUS          Status;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieMidInitKV Enter\n");
  AgesaStatus = AGESA_SUCCESS;

  Status = PcieConfigRunProcForAllWrappers (DESCRIPTOR_ALL_WRAPPERS, PcieMidInitCallbackKV, (VOID *) PcieInfoBuffer, Pcie);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  Status = PciePowerGateKV (Pcie);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieMidInitKV Exit [0x%x]\n", AgesaStatus);
  return  AgesaStatus;
}

/*----------------------------------------------------------------------------------------*/
/**
 * PCIe Mid Init
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */
AGESA_STATUS
PcieMidInterfaceKV (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS          AgesaStatus;
  AGESA_STATUS          Status;
  PCIe_PLATFORM_CONFIG  *Pcie;
  PCIe_INFO_BUFFER      PcieInfoBuffer;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieMidInterfaceKV Enter\n");
  AgesaStatus = AGESA_SUCCESS;
  Status = PcieLocateConfigurationData (StdHeader, &Pcie);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);
  if (Status == AGESA_SUCCESS) {
    PciePortsVisibilityControlV5 (UnhidePorts, Pcie);

    Status = PcieMidCharacterizationKV (&PcieInfoBuffer, Pcie);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    ASSERT (Status == AGESA_SUCCESS);

    Status = PcieMidPortInitKV (&PcieInfoBuffer, Pcie);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    ASSERT (Status == AGESA_SUCCESS);

    Status = PcieMidInitKV (&PcieInfoBuffer, Pcie);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    ASSERT (Status == AGESA_SUCCESS);

    PciePortsVisibilityControlV5 (HidePorts, Pcie);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieMidInterfaceKV Exit [0x%x]\n", AgesaStatus);
  return  AgesaStatus;
}
