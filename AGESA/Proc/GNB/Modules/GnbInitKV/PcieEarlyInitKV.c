/* $NoKeywords:$ */
/**
 * @file
 *
 * PCIe early post initialization.
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 285511 $   @e \$Date: 2014-02-21 13:47:54 -0600 (Fri, 21 Feb 2014) $
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
#include  "GnbCommonLib.h"
#include  "GnbPcieConfig.h"
#include  "GnbPcieTrainingV2.h"
#include  "GnbPcieInitLibV1.h"
#include  "GnbPcieInitLibV4.h"
#include  "GnbPcieInitLibV5.h"
#include  "PcieLibKV.h"
#include  "PcieComplexDataKV.h"
#include  "GnbRegistersKV.h"
#include  "GnbRegisterAccKV.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITKV_PCIEEARLYINITKV_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
extern BUILD_OPT_CFG UserOptions;
extern CONST PCIE_HOST_REGISTER_TABLE_HEADER ROMDATA CoreInitTableKV;
extern CONST PCIE_HOST_REGISTER_TABLE_HEADER ROMDATA PcieInitEarlyTableKV;
extern CONST PCIE_HOST_REGISTER_TABLE_HEADER ROMDATA PhyLaneInitEarlyTableKV;
extern CONST PCIE_HOST_REGISTER_TABLE_HEADER ROMDATA PhyWrapperInitEarlyTableKV;
extern CONST PCIE_PORT_REGISTER_TABLE_HEADER ROMDATA PortInitEarlyTableKV;

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 * Set port device/function mapping
 *
 *
 *
 * @param[in]  Descriptor          Silicon descriptor
 * @param[in]  Buffer              Pointer to buffer
 * @param[in]  Pcie                Pointer to global PCIe configuration
 */
AGESA_STATUS
PciePortMapInitCallbackKV (
  IN       PCIe_DESCRIPTOR_HEADER            *Descriptor,
  IN OUT   VOID                              *Buffer,
  IN       PCIe_PLATFORM_CONFIG              *Pcie
  )
{
  PcieSetPortPciAddressMapKV ((PCIe_SILICON_CONFIG *) Descriptor);
  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * PHY lane parameter Init
 *
 *
 *
 * @param[in]  Wrapper             Pointer to wrapper config descriptor
 * @param[in]  Buffer              Pointer to buffer
 * @param[in]  Pcie                Pointer to global PCIe configuration
 */
AGESA_STATUS
PciePhyLaneInitInitCallbackKV (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      VOID                  *Buffer,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT8                   Phy;
  UINT8                   PhyLaneIndex;
  UINT8                   Lane;
  UINT32                  LaneBitmap;
  UINTN                   Index;

  IDS_HDT_CONSOLE (GNB_TRACE, "PciePhyLaneInitInitCallbackKV Enter\n");
  LaneBitmap = PcieUtilGetWrapperLaneBitMap (LANE_TYPE_PCIE_PHY_NATIVE, 0, Wrapper);
  for (Lane = 0; Lane < Wrapper->NumberOfLanes; ++Lane) {
    Phy = Lane / MAX_NUM_LANE_PER_PHY;
    PhyLaneIndex = Lane - Phy * MAX_NUM_LANE_PER_PHY;
    if ((LaneBitmap & (1 << Lane)) != 0) {
      for (Index = 0; Index < PhyLaneInitEarlyTableKV.Length; Index++) {
        UINT32 Value;
        Value = PcieRegisterRead (
                  Wrapper,
                  PHY_SPACE (Wrapper->WrapId, Phy, PhyLaneInitEarlyTableKV.Table[Index].Reg + (PhyLaneIndex * 0x80)),
                  Pcie
                  );
        Value &= (~PhyLaneInitEarlyTableKV.Table[Index].Mask);
        Value |= PhyLaneInitEarlyTableKV.Table[Index].Data;
        PcieRegisterWrite (
          Wrapper,
          PHY_SPACE (Wrapper->WrapId, Phy, PhyLaneInitEarlyTableKV.Table[Index].Reg + (PhyLaneIndex * 0x80)),
          Value,
          FALSE,
          Pcie
          );
      }
    }
    // RPR Step 1a
    if (PhyLaneIndex == 0) {
      PcieRegisterRMW (
        Wrapper,
        PIF_SPACE (Wrapper->WrapId, Phy, D0F0xE4_PIF_0010_ADDRESS),
        D0F0xE4_PIF_0010_DivInitEnable_MASK,
        (1 << D0F0xE4_PIF_0010_DivInitEnable_OFFSET),
        FALSE,
        Pcie
        );
    }
    PcieRegisterWrite (
      Wrapper,
      PIF_SPACE (Wrapper->WrapId, Phy, (D0F0xE4_PIF_0020_ADDRESS + PhyLaneIndex)),
      0x0,
      FALSE,
      Pcie
      );
  }
  for (Lane = 0; Lane < Wrapper->NumberOfLanes; Lane += MAX_NUM_LANE_PER_PHY) {
    Phy = Lane / MAX_NUM_LANE_PER_PHY;
    for (Index = 0; Index < PhyWrapperInitEarlyTableKV.Length; Index++) {
      UINT32 Value;
      Value = PcieRegisterRead (
                Wrapper,
                PHY_SPACE (Wrapper->WrapId, Phy, PhyWrapperInitEarlyTableKV.Table[Index].Reg),
                Pcie
                );
      Value &= (~PhyWrapperInitEarlyTableKV.Table[Index].Mask);
      Value |= PhyWrapperInitEarlyTableKV.Table[Index].Data;
      PcieRegisterWrite (
        Wrapper,
        PHY_SPACE (Wrapper->WrapId, Phy, PhyWrapperInitEarlyTableKV.Table[Index].Reg),
        Value,
        FALSE,
        Pcie
        );
    }
    IDS_OPTION_HOOK (IDS_GNB_PCIE_PHYLANE_CONFIG, Wrapper, (AMD_CONFIG_PARAMS *)Pcie->StdHeader);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePhyLaneInitInitCallbackKV Exit\n");
  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Init core registers.
 *
 *
 * @param[in]  Wrapper         Pointer to wrapper configuration descriptor
 * @param[in]  Pcie            Pointer to global PCIe configuration
 */
VOID
STATIC
PcieEarlyCoreInitKV (
  IN       PCIe_WRAPPER_CONFIG           *Wrapper,
  IN       PCIe_PLATFORM_CONFIG          *Pcie
  )
{
  UINT8   CoreId;
  UINTN   Index;

  if (PcieLibIsPcieWrapper (Wrapper)) {
    IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyCoreInitKV Enter\n");
    for (CoreId = Wrapper->StartPcieCoreId; CoreId <= Wrapper->EndPcieCoreId; CoreId++) {
      for (Index = 0; Index < CoreInitTableKV.Length; Index++) {
        UINT32 Value;
        Value = PcieRegisterRead (
                  Wrapper,
                  CORE_SPACE (CoreId, CoreInitTableKV.Table[Index].Reg),
                  Pcie
                  );
        Value &= (~CoreInitTableKV.Table[Index].Mask);
        Value |= CoreInitTableKV.Table[Index].Data;
        PcieRegisterWrite (
          Wrapper,
          CORE_SPACE (CoreId, CoreInitTableKV.Table[Index].Reg),
          Value,
          FALSE,
          Pcie
          );
      }
    }
    IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyCoreInitKV Exit\n");
  }
}


/*----------------------------------------------------------------------------------------*/
/**
 * Per wrapper Pcie Init prior training.
 *
 *
 * @param[in]  Wrapper         Pointer to wrapper configuration descriptor
 * @param[in]  Buffer          Pointer buffer
 * @param[in]  Pcie            Pointer to global PCIe configuration
 */
AGESA_STATUS
STATIC
PcieEarlyInitCallbackKV (
  IN       PCIe_WRAPPER_CONFIG           *Wrapper,
  IN OUT   VOID                          *Buffer,
  IN       PCIe_PLATFORM_CONFIG          *Pcie
  )
{
  AGESA_STATUS  Status;
  BOOLEAN       CoreConfigChanged;
  BOOLEAN       PllConfigChanged;
  PCIe_ENGINE_CONFIG  *EnginesList;
  UINT8         CoreLane;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyInitCallbackKV Enter\n");
  CoreConfigChanged = FALSE;
  PllConfigChanged = FALSE;
  // Set default equalization presets (P8)
  EnginesList = PcieConfigGetChildEngine (Wrapper);
  if (Wrapper->WrapId == GFX_WRAP_ID) {
    while (EnginesList != NULL) {
      if ((EnginesList->Type.Port.StartCoreLane != UNUSED_LANE_ID) && (EnginesList->Type.Port.EndCoreLane != UNUSED_LANE_ID)) {
        // Default to no eq
        PcieRegisterRMW (
          Wrapper,
          WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_0801_ADDRESS + (EnginesList->Type.Port.PortId * 0x100)),
          D0F0xE4_WRAP_0801_StrapBifLcBypassEqReqPhase_MASK,
          (1 << D0F0xE4_WRAP_0801_StrapBifLcBypassEqReqPhase_OFFSET),
          TRUE,
          Pcie
          );
        for (CoreLane = EnginesList->Type.Port.StartCoreLane; CoreLane <= EnginesList->Type.Port.EndCoreLane; CoreLane ++) {
          PcieRegisterRMW (
            Wrapper,
            WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_0050_ADDRESS + CoreLane),
            D0F0xE4_WRAP_0050_StrapBifPcieLaneEqCntlDsPortTxPreset_MASK |
            D0F0xE4_WRAP_0050_StrapBifPcieLaneEqCntlUsPortTxPreset_MASK |
            D0F0xE4_WRAP_0050_StrapBifPcieLaneEqCntlUsPortRxPresetHint_MASK |
            D0F0xE4_WRAP_0050_StrapBifPcieLaneEqCntlDsPortRxPresetHint_MASK ,
            (8 << D0F0xE4_WRAP_0050_StrapBifPcieLaneEqCntlDsPortTxPreset_OFFSET) |
            (2 << D0F0xE4_WRAP_0050_StrapBifPcieLaneEqCntlUsPortTxPreset_OFFSET) |
            (2 << D0F0xE4_WRAP_0050_StrapBifPcieLaneEqCntlUsPortRxPresetHint_OFFSET) |
            (2 << D0F0xE4_WRAP_0050_StrapBifPcieLaneEqCntlDsPortRxPresetHint_OFFSET) ,
            TRUE,
            Pcie
            );
        }
      }

      EnginesList = PcieLibGetNextDescriptor (EnginesList);
    }
  }
  // Disable PCIe Debug Bus Clock
  PcieRegisterRMW (
    Wrapper,
    WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_8011_ADDRESS),
    D0F0xE4_WRAP_8011_Bitfield_23_23_MASK,
    0 << D0F0xE4_WRAP_8011_Bitfield_23_23_OFFSET,
    TRUE,
    Pcie
    );
  IDS_OPTION_HOOK (IDS_GNB_EQUAL_PRESET, Wrapper, (AMD_CONFIG_PARAMS *)Pcie->StdHeader);
  IDS_OPTION_HOOK (IDS_BEFORE_RECONFIGURATION, Pcie, (AMD_CONFIG_PARAMS *)Pcie->StdHeader);
  PcieTopologyPrepareForReconfig (Wrapper, Pcie);
  Status = PcieTopologySetCoreConfig (Wrapper, &CoreConfigChanged, Pcie);
  ASSERT (Status == AGESA_SUCCESS);
  PcieTopologyApplyLaneMuxV5 (Wrapper, Pcie);
  PciePifSetRxDetectPowerMode (Wrapper, Pcie);
  PciePifSetLs2ExitTime (Wrapper, Pcie);
  PciePifApplyGanging  (Wrapper, Pcie);
  PciePhyApplyGangingKV (Wrapper, Pcie);
  PcieTopologySelectMasterPll (Wrapper, &PllConfigChanged, Pcie);
  PcieTopologyExecuteReconfigV5 (Wrapper, Pcie);
  PcieTopologyCleanUpReconfig (Wrapper, Pcie);
  PcieTopologySetLinkReversalV4 (Wrapper, Pcie);
  PciePifPllConfigureKV (Wrapper, Pcie);
  PciePwrPowerDownDdiPllsKV (Wrapper, Pcie);
  PcieTopologyLaneControlV5 (
    DisableLanes,
    PcieUtilGetWrapperLaneBitMap (LANE_TYPE_CORE_ALL, LANE_TYPE_PCIE_CORE_ALLOC, Wrapper),
    Wrapper,
    Pcie
    );
  PcieSetDdiOwnPhyV4 (Wrapper, Pcie);
  PciePollPifForCompeletion (Wrapper, Pcie);
  PciePhyAvertClockPickersKV (Wrapper, Pcie);
  PcieEarlyCoreInitKV (Wrapper, Pcie);
  PcieSetSsidV4 (UserOptions.CfgGnbPcieSSID, Wrapper, Pcie);
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyInitCallbackKV Exit [%x]\n", Status);
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Pcie Init
 *
 *
 *
 * @param[in]  Pcie                Pointer to global PCIe configuration
 * @retval     AGESA_SUCCESS       Topology successfully mapped
 * @retval     AGESA_ERROR         Topology can not be mapped
 */

AGESA_STATUS
STATIC
PcieEarlyInitKV (
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  AGESA_STATUS        Status;
  AGESA_STATUS        AgesaStatus;
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyInitKV Enter\n");
  AgesaStatus = AGESA_SUCCESS;
  Status = PcieConfigRunProcForAllDescriptors (DESCRIPTOR_SILICON, 0, DESCRIPTOR_TERMINATE_TOPOLOGY, PciePortMapInitCallbackKV, NULL, Pcie);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);
  Status = PcieConfigRunProcForAllWrappers (DESCRIPTOR_ALL_WRAPPERS, PciePhyLaneInitInitCallbackKV, NULL, Pcie);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);
  Status = PcieConfigRunProcForAllWrappers (DESCRIPTOR_ALL_WRAPPERS, PcieEarlyInitCallbackKV, NULL, Pcie);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyInitKV Exit [%x]\n", AgesaStatus);
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
 * @param[in, out]  Buffer          Not used
 * @param[in]       Pcie            Pointer to global PCIe configuration
 *
 */

VOID
STATIC
PcieEarlyPortInitCallbackKV (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIe_WRAPPER_CONFIG   *Wrapper;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyPortInitCallbackKV Enter\n");
  ASSERT (Engine->EngineData.EngineType == PciePortEngine);
  PciePortProgramRegisterTable (PortInitEarlyTableKV.Table, PortInitEarlyTableKV.Length, Engine, FALSE, Pcie);
  PcieSetLinkSpeedCapV4 (PcieGen1, Engine, Pcie);
  PcieSetLinkWidthCap (Engine, Pcie);
  PcieCompletionTimeout (Engine, Pcie);
  PcieLinkSetSlotCap (Engine, Pcie);
  PcieLinkInitHotplug (Engine, Pcie);
  if (Engine->Type.Port.PortData.PortPresent == PortDisabled ||
      (Engine->Type.Port.PortData.EndpointStatus == EndpointNotPresent &&
       Engine->Type.Port.PortData.LinkHotplug != HotplugEnhanced &&
       Engine->Type.Port.PortData.LinkHotplug != HotplugServer)) {
    ASSERT (!PcieConfigIsSbPcieEngine (Engine));
    //
    //  Pass endpoint status in scratch
    //
    PciePortRegisterRMW (
      Engine,
      DxFxxE4_x01_ADDRESS,
      0x1,
      0x1,
      FALSE,
      Pcie
      );
    PcieTrainingSetPortStateV2 (Engine, LinkStateDeviceNotPresent, FALSE, Pcie);
  }

  Wrapper = PcieConfigGetParentWrapper (Engine);
  if (Wrapper->WrapId == GFX_WRAP_ID) {
    PciePortRegisterRMW (
      Engine,
      DxFxxE4_x02_ADDRESS,
      DxFxxE4_x02_Bitfield_15_15_MASK,
      (1 << DxFxxE4_x02_Bitfield_15_15_OFFSET),
      FALSE,
      Pcie
    );
  }

  if (PcieConfigIsSbPcieEngine (Engine)) {
    PcieTrainingSetPortStateV2 (Engine, LinkStateTrainingSuccess, FALSE, Pcie);
  }
  if (Engine->Type.Port.PortData.MiscControls.LinkComplianceMode == 0x1) {
    PcieTrainingSetPortStateV2 (Engine, LinkStateTrainingCompleted, FALSE, Pcie);
  }

  IDS_OPTION_HOOK (IDS_GNB_GEN1_LOOPBACK, Engine, (AMD_CONFIG_PARAMS *)Pcie->StdHeader);
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyPortInitCallbackKV Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Master procedure to init various features on all active ports
 *
 *
 *
 *
 * @param[in]   Pcie            Pointer to global PCIe configuration
 * @retval      AGESA_STATUS
 *
 */

AGESA_STATUS
STATIC
PcieEarlyPortInitKV (
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  AGESA_STATUS  Status;
  Status = AGESA_SUCCESS;

  // Leave all device in Presence Detect Presence state for distributed training will be completed at PciePortPostEarlyInit
  if (Pcie->TrainingAlgorithm == PcieTrainingDistributed) {
    Pcie->TrainingExitState = LinkStateResetExit;
  }

  PcieConfigRunProcForAllEngines (
    DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
    PcieEarlyPortInitCallbackKV,
    NULL,
    Pcie
    );

  IDS_OPTION_CALLOUT (IDS_CALLOUT_GNB_PCIE_EARLY_PORT_CONFIG, Pcie, (AMD_CONFIG_PARAMS *)Pcie->StdHeader);

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * PCIe Early Post Init
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
PcieEarlyInterfaceKV (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS          Status;
  AGESA_STATUS          AgesaStatus;
  PCIe_PLATFORM_CONFIG  *Pcie;
  AgesaStatus = AGESA_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyInterfaceKV Enter\n");
  Status = PcieLocateConfigurationData (StdHeader, &Pcie);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);
  if (Status == AGESA_SUCCESS) {
    PciePortsVisibilityControlV5 (UnhidePorts, Pcie);

    Status = PcieEarlyInitKV (Pcie);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    ASSERT (Status == AGESA_SUCCESS);

    Status = PcieEarlyPortInitKV (Pcie);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    ASSERT (Status == AGESA_SUCCESS);

    Status = PcieTrainingV2 (Pcie);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    ASSERT (Status == AGESA_SUCCESS);

    IDS_OPTION_CALLOUT (IDS_CALLOUT_GNB_PCIE_PHY_CONFIG, Pcie, StdHeader);
    PciePortsVisibilityControlV5 (HidePorts, Pcie);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyInterfaceKV Exit [0x%x]\n", AgesaStatus);
  return  AgesaStatus;
}

