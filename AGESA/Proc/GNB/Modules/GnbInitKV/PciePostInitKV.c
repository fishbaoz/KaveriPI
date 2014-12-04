/* $NoKeywords:$ */
/**
 * @file
 *
 * PCIe post initialization.
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 284222 $   @e \$Date: 2014-02-06 09:33:07 -0600 (Thu, 06 Feb 2014) $
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
#include  "GnbPcieFamServices.h"
#include  "GnbPcieConfig.h"
#include  "GnbLibPci.h"
#include  "GnbLibPciAcc.h"
#include  "GnbPcieTrainingV2.h"
#include  "GnbPcieInitLibV1.h"
#include  "GnbPcieInitLibV4.h"
#include  "GnbPcieInitLibV5.h"
#include  "PcieComplexDataKV.h"
#include  "PcieLibKV.h"
#include  "GnbRegistersKV.h"
#include  "GnbRegisterAccKV.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITKV_PCIEPOSTINITKV_FILECODE
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


/*----------------------------------------------------------------------------------------*/
/**
 * Return the PCIe vendor id of the installed device
 *
 *
 *
 *
 * @param[in,out] VendorId        Pointer to vendor id
 * @param[in]     BridgeAddress   Desired bridge address
 * @param[in]     StdHeader       Standard configuration header
 *
 */

VOID
GetCardInfo (
  IN OUT   UINT32    *VendorId,
  IN       UINT32    BridgeAddress,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32    SaveBusConfiguration;
  UINT32    Value;
  PCI_ADDR  Bridge;
  PCI_ADDR  Card;

  Bridge.AddressValue = BridgeAddress;
  Card.AddressValue = MAKE_SBDFO (0, 5, 0, 0, 0);

  IDS_HDT_CONSOLE (GFX_MISC, "  Evaluate device [%02d:%02d:%02d]\n",
      Bridge.Address.Bus, Bridge.Address.Device, Bridge.Address.Function
      );

  GnbLibPciRead (Bridge.AddressValue | 0x18, AccessWidth32, &SaveBusConfiguration, StdHeader);
  Value = (((0xFF << 8) | 5) << 8) | Bridge.Address.Bus;
  GnbLibPciWrite (Bridge.AddressValue | 0x18, AccessWidth32, &Value, StdHeader);

  GnbLibPciRead (Card.AddressValue | 0x00, AccessWidth32, VendorId, StdHeader);
  IDS_HDT_CONSOLE (GNB_TRACE, "  Found DeviceId: %08x\n", *VendorId);

  GnbLibPciWrite (Bridge.AddressValue | 0x18, AccessWidth32, &SaveBusConfiguration, StdHeader);

}

/*----------------------------------------------------------------------------------------*/
/**
 * Pre-retrain workarounds
 *
 *
 *
 *
 * @param[in] VendorId        Scan configuration data
 * @param[in] Engine          Pointer to engine config
 * @param[in] Pcie            Pointer to PCIe config
 *
 */

VOID
STATIC
PciePreRetrainWorkaroundsKV (
  IN      UINT32                VendorId,
  IN      PCIe_ENGINE_CONFIG    *Engine,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{

  IDS_HDT_CONSOLE (GNB_TRACE, "PciePreRetrainWorkaroundsKV Enter\n");

  IDS_HDT_CONSOLE (GNB_TRACE, "  VendorId = %08x\n", VendorId);

  if (VendorId == 0x100315B3) {

    PciePortRegisterRMW (
      Engine,
      DxFxxE4_xB6_ADDRESS,
      DxFxxE4_xB6_LcUscEqNotReqd_MASK,
      (1 << DxFxxE4_xB6_LcUscEqNotReqd_OFFSET),
      TRUE,
      Pcie
      );

  }

  IDS_HDT_CONSOLE (GNB_TRACE, "PciePreRetrainWorkaroundsKV Exit\n");

}

/*----------------------------------------------------------------------------------------*/
/**
 * PHY lane RxAdaptMode Init
 *
 *
 *
 *
 * @param[in]  Wrapper             Pointer to wrapper config descriptor
 * @param[in]  Engine              Pointer to engine config descriptor
 * @param[in]  Pcie                Pointer to global PCIe configuration
 *
 */

VOID
STATIC
PciePhyLaneRxAdaptModeKV (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_ENGINE_CONFIG    *Engine,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT16                  Phy;
  UINT16                  PhyLaneIndex;
  UINT16                  Lane;
  UINT16                  StartLane;
  UINT16                  EndLane;
  D0F0xE4_PHY_4019_STRUCT D0F0xE4_PHY_4019;

  IDS_HDT_CONSOLE (GNB_TRACE, "PciePhyLaneInitRxAdaptModeKV Enter\n");
  // Get RxAdaptMode value from PortData
  D0F0xE4_PHY_4019.Value = 0;
  if (Engine->Type.Port.PortData.RxAdaptMode.InitOffsetCancellation == ADAPT_IOC_ENABLED) {
    D0F0xE4_PHY_4019.Field.RxAdaptMode_0_ = 1;
  }
  D0F0xE4_PHY_4019.Field.RxAdaptMode_3_1_ = Engine->Type.Port.PortData.RxAdaptMode.DFEControl;
  D0F0xE4_PHY_4019.Field.RxAdaptMode_5_4_ = Engine->Type.Port.PortData.RxAdaptMode.LEQControl;
  if (Engine->Type.Port.PortData.RxAdaptMode.DynamicOffsetCalibration == ADAPT_DOC_ENABLED) {
    D0F0xE4_PHY_4019.Field.RxAdaptMode_6_ = 1;
  }
  if (Engine->Type.Port.PortData.RxAdaptMode.FOMCalculation == ADAPT_FOMC_ENABLED) {
    D0F0xE4_PHY_4019.Field.RxAdaptMode_7_ = 1;
  }
  if (Engine->Type.Port.PortData.RxAdaptMode.PIOffsetCalibration == ADAPT_PIOC_ENABLED) {
    D0F0xE4_PHY_4019.Field.RxAdaptMode_8_ = 1;
  }
  // Auto mode can use Device ID and Vendor ID to select "AUTO" setting here
  if (Engine->EngineData.StartLane < Engine->EngineData.EndLane) {
    StartLane = Engine->EngineData.StartLane - Wrapper->StartPhyLane;
    EndLane = Engine->EngineData.EndLane - Wrapper->StartPhyLane;
  } else {
    StartLane = Engine->EngineData.EndLane - Wrapper->StartPhyLane;
    EndLane = Engine->EngineData.StartLane - Wrapper->StartPhyLane;
  }
  for (Lane = StartLane; Lane <= EndLane; ++Lane) {
    Phy = Lane / MAX_NUM_LANE_PER_PHY;
    PhyLaneIndex = Lane - Phy * MAX_NUM_LANE_PER_PHY;
    PcieRegisterRMW (
      Wrapper,
      PHY_SPACE (Wrapper->WrapId, Phy, D0F0xE4_PHY_4019_ADDRESS + PhyLaneIndex * 0x80),
      D0F0xE4_PHY_4019_RxAdaptMode_0__MASK |
      D0F0xE4_PHY_4019_RxAdaptMode_3_1__MASK |
      D0F0xE4_PHY_4019_RxAdaptMode_5_4__MASK |
      D0F0xE4_PHY_4019_RxAdaptMode_6__MASK |
      D0F0xE4_PHY_4019_RxAdaptMode_7__MASK |
      D0F0xE4_PHY_4019_RxAdaptMode_8__MASK,
      D0F0xE4_PHY_4019.Value,
      FALSE,
      Pcie
      );
    if (D0F0xE4_PHY_4019.Field.RxAdaptMode_3_1_ != 0) {
      PcieRegisterRMW (
        Wrapper,
        PHY_SPACE (Wrapper->WrapId, Phy, D0F0xE4_PHY_4021_ADDRESS + PhyLaneIndex * 0x80),
        D0F0xE4_PHY_4021_AdaptPiOffsetBypEn_MASK,
        1 << D0F0xE4_PHY_4021_AdaptPiOffsetBypEn_OFFSET,
        FALSE,
        Pcie
        );
    }
    PcieRegisterRMW (
      Wrapper,
      PHY_SPACE (Wrapper->WrapId, Phy, D0F0xE4_PHY_401D_ADDRESS + PhyLaneIndex * 0x80),
      D0F0xE4_PHY_401D_Gen3OffCorrMethod_MASK,
      1 << D0F0xE4_PHY_401D_Gen3OffCorrMethod_OFFSET,
      FALSE,
      Pcie
      );

  }
}


/*----------------------------------------------------------------------------------------*/
/**
 * Callback to init various features on all ports
 *
 *
 *
 *
 * @param[in]       Engine          Pointer to engine config descriptor
 * @param[in]       Pcie            Pointer to global PCIe configuration
 *
 */

VOID
STATIC
PciePostPortGen3SettingsKV (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIe_WRAPPER_CONFIG     *Wrapper;
  UINT32                  VendorId;

  IDS_HDT_CONSOLE (GNB_TRACE, "PciePostPortGen3SettingsKV Enter\n");

  VendorId = 0;
  Wrapper = PcieConfigGetParentWrapper (Engine);
  if (Wrapper->WrapId == 0) {

    GetCardInfo (
        &VendorId,
        Engine->Type.Port.Address.AddressValue,
        GnbLibGetHeader (Pcie));

    // RxAdaptMode only applies to GFX wrapper
    PciePhyLaneRxAdaptModeKV (Wrapper, Engine, Pcie);
    PciePreRetrainWorkaroundsKV (VendorId, Engine, Pcie);
  }

  IDS_OPTION_HOOK (IDS_GNB_GEN3_SETTINGS_OVERRIDE, Engine, (AMD_CONFIG_PARAMS *)Pcie->StdHeader);
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePostPortGen3SettingsKV Exit\n");
}


/*----------------------------------------------------------------------------------------*/
/**
 * Callback to init settings for Gen3 ports
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
PciePostPortInitAfterTrainingCallbackKV (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIE_LINK_SPEED_CAP LinkSpeedCapability;
  ASSERT (Engine->EngineData.EngineType == PciePortEngine);

  LinkSpeedCapability = PcieFmGetLinkSpeedCap (PCIE_PORT_GEN_CAP_MAX, Engine);
  if (PcieConfigCheckPortStatus (Engine, INIT_STATUS_PCIE_TRAINING_SUCCESS) && (LinkSpeedCapability > PcieGen1) && !PcieConfigIsSbPcieEngine (Engine)) {
    PciePostPortGen3SettingsKV (Engine, Pcie);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback to init various features on all ports
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
PciePostPortInitCallbackKV (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIE_LINK_SPEED_CAP LinkSpeedCapability;
  ASSERT (Engine->EngineData.EngineType == PciePortEngine);
  if (Engine->Type.Port.PortData.MiscControls.LinkSafeMode == PcieGen1) {
    PcieLinkSafeMode (Engine, Pcie);
  }
  LinkSpeedCapability = PcieFmGetLinkSpeedCap (PCIE_PORT_GEN_CAP_BOOT, Engine);
  PcieSetLinkSpeedCapV4 (LinkSpeedCapability, Engine, Pcie);
  if (PcieConfigCheckPortStatus (Engine, INIT_STATUS_PCIE_TRAINING_SUCCESS) && (LinkSpeedCapability > PcieGen1) && !PcieConfigIsSbPcieEngine (Engine)) {
    PcieTrainingSetPortStateV2 (Engine, LinkStateRetrain, FALSE, Pcie);
    PcieConfigUpdatePortStatus (Engine, 0, INIT_STATUS_PCIE_TRAINING_SUCCESS);
  }
  if (Engine->Type.Port.PortData.MiscControls.LinkComplianceMode == 0x1) {
    PcieForceCompliance (Engine, Pcie);
    PcieTrainingSetPortStateV2 (Engine, LinkStateResetExit, FALSE, Pcie);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback to init various features on all ports
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
PciePostS3PortInitCallbackKV (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIE_LINK_SPEED_CAP       LinkSpeedCapability;
  PCIE_LINK_TRAINING_STATE  State;

  ASSERT (Engine->EngineData.EngineType == PciePortEngine);

  LinkSpeedCapability = PcieFmGetLinkSpeedCap (PCIE_PORT_GEN_CAP_BOOT, Engine);
  PcieSetLinkSpeedCapV4 (LinkSpeedCapability, Engine, Pcie);

  if (Engine->Type.Port.PortData.MiscControls.LinkSafeMode == PcieGen1) {
    PcieLinkSafeMode (Engine, Pcie);
  }

  if (!PcieConfigIsSbPcieEngine (Engine)) {
    //
    // General Port
    //
    State = LinkStateDeviceNotPresent;
    if (Engine->Type.Port.PortData.LinkHotplug == HotplugDisabled || Engine->Type.Port.PortData.LinkHotplug == HotplugInboard) {
      //
      // Non hotplug device: we only check status from previous boot
      //
      if (PcieConfigCheckPortStatus (Engine, INIT_STATUS_PCIE_TRAINING_SUCCESS)) {
        State = LinkStateResetExit;
      }
    } else {
      UINT32  PcieScratch;
      //
      //  Get endpoint staus from scratch
      //
      PcieScratch = PciePortRegisterRead (Engine, DxFxxE4_x01_ADDRESS, Pcie);
      //
      // Hotplug device: we check ep status if reported
      //
      if ((PcieScratch & 0x1) == 0) {
        State = LinkStateResetExit;
      }
    }
    //
    // For compliance we always leave link in enabled state
    //
    if (Engine->Type.Port.PortData.MiscControls.LinkComplianceMode) {
      State = LinkStateResetExit;
    }
    PcieConfigUpdatePortStatus (Engine, 0, INIT_STATUS_PCIE_TRAINING_SUCCESS);
  } else {
    //
    // SB port
    //
    State = LinkStateTrainingSuccess;
  }
  PcieTrainingSetPortStateV2 (Engine, State, FALSE, Pcie);
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
PciePostEarlyPortInitKV (
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  AGESA_STATUS  Status;
  Status = AGESA_SUCCESS;
  // Distributed Training started at PciePortInit complete it now to get access to PCIe devices
  if (Pcie->TrainingAlgorithm == PcieTrainingDistributed) {
    Pcie->TrainingExitState = LinkStateTrainingCompleted;
  }
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback to power up panel on DDI ports with eDP connectors *
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
DdiPostPortInitCallbackKV (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  GMMx6464_STRUCT         GMMx6464;
  GMMx5C1C_STRUCT         GMMx5C1C;

  IDS_HDT_CONSOLE (GNB_TRACE, "DdiPostPortInitCallbackKV Enter\n");
  if ((Engine->Type.Ddi.DdiData.ConnectorType == ConnectorTypeEDP) ||
      (Engine->Type.Ddi.DdiData.ConnectorType == ConnectorTypeDpToLvds) ||
      (Engine->Type.Ddi.DdiData.ConnectorType == ConnectorTypeEDPToLvds) ||
      (Engine->Type.Ddi.DdiData.ConnectorType == ConnectorTypeEDPToLvdsSwInit)) {

    IDS_HDT_CONSOLE (GNB_TRACE, "Found eDP/LVDS Connector\n");

    GnbRegisterReadKV (GnbGetHandle (GnbLibGetHeader (Pcie)), GMMx6464_TYPE, GMMx6464_ADDRESS, &GMMx6464.Value, 0, GnbLibGetHeader (Pcie));
    GnbRegisterReadKV (GnbGetHandle (GnbLibGetHeader (Pcie)), GMMx5C1C_TYPE, GMMx5C1C_ADDRESS, &GMMx5C1C.Value, 0, GnbLibGetHeader (Pcie));
    GMMx6464.Field.LVTMA_PWRSEQ_EN = 1;
    GMMx6464.Field.LVTMA_PWRSEQ_TARGET_STATE = 1;
    GMMx6464.Field.LVTMA_BLON_OVRD = 1;
    GMMx5C1C.Field.PLL_REFCLK_SEL = 1;
    GnbRegisterWriteKV (GnbGetHandle (GnbLibGetHeader (Pcie)), GMMx5C1C_TYPE, GMMx5C1C_ADDRESS, &GMMx5C1C.Value, 0, GnbLibGetHeader (Pcie));
    GnbRegisterWriteKV (GnbGetHandle (GnbLibGetHeader (Pcie)), GMMx6464_TYPE, GMMx6464_ADDRESS, &GMMx6464.Value, 0, GnbLibGetHeader (Pcie));
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "DdiPostPortInitCallbackKV Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Procedure to init various features on all active ports after training
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
PciePostEarlyPortInitAfterTrainingKV (
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  AGESA_STATUS  Status;
  Status = AGESA_SUCCESS;
  PcieConfigRunProcForAllEngines (
    DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
    PciePostPortInitAfterTrainingCallbackKV,
    NULL,
    Pcie
    );
  return Status;
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
PciePostPortInitKV (
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  AGESA_STATUS  Status;
  Status = AGESA_SUCCESS;
  PcieConfigRunProcForAllEngines (
    DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
    PciePostPortInitCallbackKV,
    NULL,
    Pcie
    );

  PcieConfigRunProcForAllEngines (
    DESCRIPTOR_ALLOCATED | DESCRIPTOR_DDI_ENGINE,
    DdiPostPortInitCallbackKV,
    NULL,
    Pcie
    );

  return Status;
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
PciePostS3PortInitKV (
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  AGESA_STATUS  Status;
  Status = AGESA_SUCCESS;
  PcieConfigRunProcForAllEngines (
    DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
    PciePostS3PortInitCallbackKV,
    NULL,
    Pcie
    );

  PcieConfigRunProcForAllEngines (
    DESCRIPTOR_ALLOCATED | DESCRIPTOR_DDI_ENGINE,
    DdiPostPortInitCallbackKV,
    NULL,
    Pcie
    );

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
PciePostInitKV (
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIE_LINK_SPEED_CAP  GlobalSpeedCap;

  GlobalSpeedCap = PcieUtilGlobalGenCapability (
                     PCIE_PORT_GEN_CAP_BOOT | PCIE_GLOBAL_GEN_CAP_TRAINED_PORTS | PCIE_GLOBAL_GEN_CAP_HOTPLUG_PORTS,
                     Pcie
                     );


  PcieSetVoltageKV (GlobalSpeedCap, Pcie);
  return  AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * PCIe Post Init
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */
AGESA_STATUS
PciePostEarlyInterfaceKV (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS          AgesaStatus;
  AGESA_STATUS          Status;
  PCIe_PLATFORM_CONFIG  *Pcie;
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePostEarlyInterfaceKV Enter\n");
  AgesaStatus = AGESA_SUCCESS;
  Status = PcieLocateConfigurationData (StdHeader, &Pcie);
  IDS_OPTION_HOOK (IDS_BEFORE_GPP_TRAINING, Pcie, StdHeader);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);
  if (Status == AGESA_SUCCESS) {
    PciePortsVisibilityControlV5 (UnhidePorts, Pcie);

    Status = PciePostEarlyPortInitKV (Pcie);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    ASSERT (Status == AGESA_SUCCESS);

    Status = PcieTrainingV2 (Pcie);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    ASSERT (Status == AGESA_SUCCESS);

    Status = PciePostEarlyPortInitAfterTrainingKV (Pcie);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    ASSERT (Status == AGESA_SUCCESS);

    PciePortsVisibilityControlV5 (HidePorts, Pcie);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePostEarlyInterfaceKV Exit [0x%x]\n", AgesaStatus);
  return  AgesaStatus;
}

/*----------------------------------------------------------------------------------------*/
/**
 * PCIe Post Init
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */
AGESA_STATUS
PciePostInterfaceKV (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS          AgesaStatus;
  AGESA_STATUS          Status;
  PCIe_PLATFORM_CONFIG  *Pcie;
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePostInterfaceKV Enter\n");
  AgesaStatus = AGESA_SUCCESS;
  Status = PcieLocateConfigurationData (StdHeader, &Pcie);
  IDS_OPTION_HOOK (IDS_BEFORE_GEN2_INIT, Pcie, StdHeader);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);
  if (Status == AGESA_SUCCESS) {
    PciePortsVisibilityControlV5 (UnhidePorts, Pcie);

    Status = PciePostInitKV (Pcie);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    ASSERT (Status == AGESA_SUCCESS);

    Status = PciePostPortInitKV (Pcie);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    ASSERT (Status == AGESA_SUCCESS);

    Status = PcieTrainingV2 (Pcie);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    ASSERT (Status == AGESA_SUCCESS);

    PciePortsVisibilityControlV5 (HidePorts, Pcie);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePostInterfaceKV Exit [0x%x]\n", AgesaStatus);
  return  AgesaStatus;
}


/*----------------------------------------------------------------------------------------*/
/**
 * PCIe Post Init
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */
AGESA_STATUS
PciePostS3InterfaceKV (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS          AgesaStatus;
  AGESA_STATUS          Status;
  PCIe_PLATFORM_CONFIG  *Pcie;
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePostS3InterfaceKV Enter\n");
  AgesaStatus = AGESA_SUCCESS;
  Status = PcieLocateConfigurationData (StdHeader, &Pcie);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);
  if (Status == AGESA_SUCCESS) {
    PciePortsVisibilityControlV5 (UnhidePorts, Pcie);

    Status = PciePostInitKV (Pcie);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    ASSERT (Status == AGESA_SUCCESS);

    if (Pcie->TrainingAlgorithm == PcieTrainingDistributed) {
      Status = PciePostS3PortInitKV (Pcie);
    } else {
      Status = PciePostPortInitKV (Pcie);
    }
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    ASSERT (Status == AGESA_SUCCESS);

    Status = PcieTrainingV2 (Pcie);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    ASSERT (Status == AGESA_SUCCESS);

    PciePortsVisibilityControlV5 (HidePorts, Pcie);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePostS3InterfaceKV Exit [0x%x]\n", AgesaStatus);
  return  AgesaStatus;
}

/*----------------------------------------------------------------------------------------*/
/**
 * PCIe S3 restore
 *
 *
 *
 * @param[in]  StdHeader      Standard configuration header
 * @param[in]  ContextLength  Context Length (not used)
 * @param[in]  Context        Context pointer (not used)
 */
VOID
PcieLateRestoreInitKVS3Script (
  IN      AMD_CONFIG_PARAMS               *StdHeader,
  IN      UINT16                          ContextLength,
  IN      VOID*                           Context
  )
{
  PciePostS3InterfaceKV (StdHeader);
}
