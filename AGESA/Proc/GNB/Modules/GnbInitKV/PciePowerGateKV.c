/* $NoKeywords:$ */
/**
 * @file
 *
 * PCIe power gate initialization
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 285004 $   @e \$Date: 2014-02-17 12:47:33 -0600 (Mon, 17 Feb 2014) $
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
#include  "OptionGnb.h"
#include  "GnbPcieConfig.h"
#include  "GnbPcieFamServices.h"
#include  "GnbPcieInitLibV1.h"
#include  "GnbNbInitLibV4.h"
#include  "GnbSmuInitLibV7.h"
#include  "GnbRegistersKV.h"
#include  "GnbRegisterAccKV.h"
#include  "PcieComplexDataKV.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITKV_PCIEPOWERGATEKV_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

extern GNB_BUILD_OPTIONS      GnbBuildOptions;

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

typedef union {
  struct {                                                              ///<
    UINT32                                                       Rx:1 ; ///<
    UINT32                                                       Tx:1 ; ///<
    UINT32                                                     Core:1 ; ///<
    UINT32                                                  SkipPhy:1 ; ///<
    UINT32                                                 SkipCore:1 ; ///<
    UINT32                                             ApuPwrGating:1 ; ///<
    UINT32                                          ApuPwrGatingDis:1 ; ///<
    UINT32                                            Reserved_15_7:9 ; ///<
    UINT32                                              LowerLaneID:8 ; ///<
    UINT32                                              UpperLaneID:8 ; ///<
  } Field;                                                              ///<
  UINT32 Value;
} SMU_ARGUMENT_STRUCT;


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 * Power gate APU Phy lanes below Gen3
 *
 *
 *
 *
 * @param[in]       Wrapper         Pointer to wrapper configuration
 * @param[in, out]  Buffer          Not used
 * @param[in]       Pcie            Pointer to global PCIe configuration
 * @retval          AGESA_SUCCESS
 *
 */

AGESA_STATUS
STATIC
PciePowerGateApuPhyCallbackKV (
  IN       PCIe_WRAPPER_CONFIG   *Wrapper,
  IN OUT   VOID                  *Buffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT8                   Index;
  UINTN                   State;
  UINT32                  LaneBitmap;
  UINT16                  StartLane;
  UINT16                  EndLane;
  DEV_OBJECT              DevObject;
  GNB_HANDLE              *GnbHandle;
  PCIe_ENGINE_CONFIG      *Engine;
  PCIE_LINK_SPEED_CAP     MaxLinkSpeedCapability;
  SMU_ARGUMENT_STRUCT     SmuArgument;

  IDS_HDT_CONSOLE (GNB_TRACE, "PciePowerGateApuPhyCallbackKV Enter\n");

  GnbHandle = GnbGetHandle (GnbLibGetHeader (Pcie));
  SmuArgument.Value = 0;

  if (Wrapper->WrapId == GFX_WRAP_ID) {
    // First, handle unused lanes and ddi lanes
    LaneBitmap = PcieUtilGetWrapperLaneBitMap (
                   LANE_TYPE_PHY_NATIVE_ALL,
                   LANE_TYPE_PCIE_PHY_NATIVE_ALLOC_ACTIVE | LANE_TYPE_PCIE_PHY_NATIVE_HOTPLUG,
                   Wrapper
                   );
    if (LaneBitmap != 0) {
      State = 0;
      StartLane = 0;
      EndLane = 0;
      for (Index = 0; Index <= (LibAmdBitScanReverse (LaneBitmap) + 1); Index++) {
        if ((State == 0) && ((LaneBitmap & (1 << Index)) != 0)) {
          StartLane = Index;
          State = 1;
        } else if ((State == 1) && ((LaneBitmap & (1 << Index)) == 0)) {
          EndLane = Index - 1;
          State = 0;
          SmuArgument.Field.ApuPwrGating = 1;
          SmuArgument.Field.LowerLaneID = StartLane + Wrapper->StartPhyLane;
          SmuArgument.Field.UpperLaneID = EndLane + Wrapper->StartPhyLane;
          IDS_HDT_CONSOLE (
            PCIE_MISC,
            "  LowerLaneID - %02d UpperLaneID - %02d Exit\n",
            SmuArgument.Field.LowerLaneID,
            SmuArgument.Field.UpperLaneID
            );

          DevObject.StdHeader = GnbLibGetHeader (Pcie);
          DevObject.GnbHandle = GnbHandle;
          DevObject.DevPciAddress.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0);
          GnbSmuServiceRequestV7 (
            &DevObject,
            SMC_MSG_PHY_LN_OFF,
            SmuArgument.Value,
            GNB_REG_ACC_FLAG_S3SAVE
            );
        }
      }
    }

    // Second, find non-gen3 link speed engines
    Engine = PcieConfigGetChildEngine (Wrapper);
    while (Engine != NULL) {
      if (PcieLibIsPcieEngine (Engine)) {
        MaxLinkSpeedCapability = PcieFmGetLinkSpeedCap (PCIE_PORT_GEN_CAP_MAX, Engine);
        if (MaxLinkSpeedCapability < PcieGen3) {
          SmuArgument.Field.ApuPwrGating = 1;
          SmuArgument.Field.LowerLaneID = PcieLibGetLoPhyLane (Engine);
          SmuArgument.Field.UpperLaneID = PcieLibGetHiPhyLane (Engine);
          IDS_HDT_CONSOLE (
            PCIE_MISC,
            "  LowerLaneID - %02d UpperLaneID - %02d Exit\n",
            SmuArgument.Field.LowerLaneID,
            SmuArgument.Field.UpperLaneID
            );

          DevObject.StdHeader = GnbLibGetHeader (Pcie);
          DevObject.GnbHandle = GnbHandle;
          DevObject.DevPciAddress.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0);
          GnbSmuServiceRequestV7 (
            &DevObject,
            SMC_MSG_PHY_LN_OFF,
            SmuArgument.Value,
            GNB_REG_ACC_FLAG_S3SAVE
            );
        }
      }
      Engine = PcieLibGetNextDescriptor (Engine);
    }
    IDS_HDT_CONSOLE (GNB_TRACE, "PciePowerGateApuPhyCallbackKV Exit\n");
    return AGESA_SUCCESS;
  }
  // For KV, only GFX wrapper has chance to support Gen3.
  SmuArgument.Field.LowerLaneID = Wrapper->StartPhyLane;
  SmuArgument.Field.UpperLaneID = Wrapper->EndPhyLane;
  SmuArgument.Field.ApuPwrGating = 1;

  IDS_HDT_CONSOLE (
    PCIE_MISC,
    "  LowerLaneID - %02d UpperLaneID - %02d Exit\n",
    SmuArgument.Field.LowerLaneID,
    SmuArgument.Field.UpperLaneID
    );

  DevObject.StdHeader = GnbLibGetHeader (Pcie);
  DevObject.GnbHandle = GnbHandle;
  DevObject.DevPciAddress.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0);
  GnbSmuServiceRequestV7 (
    &DevObject,
    SMC_MSG_PHY_LN_OFF,
    SmuArgument.Value,
    GNB_REG_ACC_FLAG_S3SAVE
    );

  IDS_HDT_CONSOLE (GNB_TRACE, "PciePowerGateApuPhyCallbackKV Exit\n");
  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Power down unused lanes
 *
 *
 *
 *
 * @param[in]       Wrapper         Pointer to wrapper configuration
 * @param[in, out]  Buffer          Not used
 * @param[in]       Pcie            Pointer to global PCIe configuration
 * @retval          AGESA_SUCCESS
 *
 */

AGESA_STATUS
STATIC
PciePowerGatePowerDownUnusedLanesCallbackKV (
  IN       PCIe_WRAPPER_CONFIG   *Wrapper,
  IN OUT   VOID                  *Buffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT8                   Index;
  UINTN                   State;
  UINT32                  LaneBitmap;
  UINT32                  LaneBitmapAll;
  UINT32                  MasterPllLane;
  UINT16                  StartLane;
  UINT16                  EndLane;
  DEV_OBJECT              DevObject;
  GNB_HANDLE              *GnbHandle;
  SMU_ARGUMENT_STRUCT     SmuArgument;
  PCIe_ENGINE_CONFIG      *Engine;

  IDS_HDT_CONSOLE (GNB_TRACE, "PciePowerGatePowerDownUnusedLanesCallbackKV Enter\n");

  GnbHandle = GnbGetHandle (GnbLibGetHeader (Pcie));

  LaneBitmap = PcieUtilGetWrapperLaneBitMap (
                 LANE_TYPE_PHY_NATIVE_ALL,
                 LANE_TYPE_PCIE_PHY_NATIVE_ALLOC_ACTIVE | LANE_TYPE_DDI_PHY_NATIVE_ACTIVE | LANE_TYPE_PCIE_PHY_NATIVE_HOTPLUG,
                 Wrapper
                 );

  LaneBitmapAll = PcieUtilGetWrapperLaneBitMap (
                 LANE_TYPE_PHY_NATIVE_ALL,
                 0,
                 Wrapper
                 );

  if (LaneBitmap != 0) {
    if (LaneBitmapAll > 0x00FF) {
      // We should not turn off all 8 lanes if the MasterPll is driven from those lanes
      MasterPllLane = 0x1 << ((Wrapper->MasterPll - 0xA) * 4);
      if ((MasterPllLane & 0xFF) != 0) {
        // MasterPll is on lower lanes
        if (((LaneBitmap & 0xFF) == 0xFF) && ((LaneBitmap & 0xFF00) != 0xFF00)) {
          // Trying to powerdown all lower lanes but not all of upper lanes, so keep MasterPll lane enabled
          LaneBitmap &= ~MasterPllLane;
        }
      } else {
        // MasterPll is on upper lanes
        if (((LaneBitmap & 0xFF00) == 0xFF00) && ((LaneBitmap & 0xFF) != 0xFF)) {
          // Trying to powerdown all upper lanes but not all of lower lanes, so keep MasterPll lane enabled
          LaneBitmap &= ~MasterPllLane;
        }
      }
    }
    State = 0;
    StartLane = 0;
    EndLane = 0;
    for (Index = 0; Index <= (LibAmdBitScanReverse (LaneBitmap) + 1); Index++) {
      if ((State == 0) && ((LaneBitmap & (1 << Index)) != 0)) {
        StartLane = Index;
        State = 1;
      } else if ((State == 1) && ((LaneBitmap & (1 << Index)) == 0)) {
        EndLane = Index - 1;
        State = 0;
        SmuArgument.Value = *(UINT32*) Buffer;
        SmuArgument.Field.Tx = 1;
        SmuArgument.Field.Rx = 1;
        SmuArgument.Field.Core = 1;
        SmuArgument.Field.LowerLaneID = StartLane + Wrapper->StartPhyLane;
        SmuArgument.Field.UpperLaneID = EndLane + Wrapper->StartPhyLane;

        // Check for Gen3 capability
        Engine = PcieConfigGetChildEngine (Wrapper);
        if ((Engine->Type.Port.PortData.LinkHotplug != HotplugDisabled) &&
            (Wrapper->WrapId == GFX_WRAP_ID)) {
          IDS_HDT_CONSOLE (GNB_TRACE, "No Rx gating for Gen3\n");
          SmuArgument.Field.Rx = 0;
        }

        IDS_HDT_CONSOLE (
          PCIE_MISC,
          "  LowerLaneID - %02d UpperLaneID - %02d Tx - %d  Rx - %d Core - %d Exit\n",
          SmuArgument.Field.LowerLaneID,
          SmuArgument.Field.UpperLaneID,
          SmuArgument.Field.Tx,
          SmuArgument.Field.Rx,
          SmuArgument.Field.Core
          );

        DevObject.StdHeader = GnbLibGetHeader (Pcie);
        DevObject.GnbHandle = GnbHandle;
        DevObject.DevPciAddress.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0);
        GnbSmuServiceRequestV7 (
          &DevObject,
          SMC_MSG_PHY_LN_OFF,
          SmuArgument.Value,
          GNB_REG_ACC_FLAG_S3SAVE
          );

      }
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePowerGatePowerDownUnusedLanesCallbackKV Exit\n");
  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Power down unused lanes
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
PciePowerGatePowerDownLanesCallbackKV (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  SMU_ARGUMENT_STRUCT     SmuArgument;
  UINT32                  LaneBitmap;
  GNB_HANDLE              *GnbHandle;
  DEV_OBJECT              DevObject;
  PCIE_LINK_SPEED_CAP     LinkSpeedCapability;

  GnbHandle = GnbGetHandle (GnbLibGetHeader (Pcie));

  IDS_HDT_CONSOLE (GNB_TRACE, "PciePowerGatePowerDownLanesCallbackKV Enter\n");
  LaneBitmap = PcieUtilGetEngineLaneBitMap (LANE_TYPE_PCIE_PHY_NATIVE_HOTPLUG, LANE_TYPE_PCIE_PHY_NATIVE_ALLOC_ACTIVE, Engine);
  if (LaneBitmap != 0) {
    SmuArgument.Value = *(UINT32*) Buffer;
    SmuArgument.Field.Tx = 1;
    SmuArgument.Field.Rx = 1;
    SmuArgument.Field.Core = 0;
    SmuArgument.Field.UpperLaneID = LibAmdBitScanReverse (LaneBitmap) + PcieConfigGetParentWrapper (Engine)->StartPhyLane;
    SmuArgument.Field.LowerLaneID = LibAmdBitScanForward (LaneBitmap) + PcieConfigGetParentWrapper (Engine)->StartPhyLane;

    // Check for Gen3 capability
    if (PcieLibIsPcieEngine (Engine)) {
      LinkSpeedCapability = PcieFmGetLinkSpeedCap (PCIE_PORT_GEN_CAP_MAX, Engine);
      if (LinkSpeedCapability == PcieGen3) {
        IDS_HDT_CONSOLE (GNB_TRACE, "No Rx gating for Gen3\n");
        SmuArgument.Field.Rx = 0;
      }
    }

    IDS_HDT_CONSOLE (
      PCIE_MISC,
      "  PCIe Lanes LowerLaneID - %02d UpperLaneID - %02d Tx - %d  Rx - %d Core - %d Exit\n",
      SmuArgument.Field.LowerLaneID,
      SmuArgument.Field.UpperLaneID,
      SmuArgument.Field.Tx,
      SmuArgument.Field.Rx,
      SmuArgument.Field.Core
      );

    DevObject.StdHeader = GnbLibGetHeader (Pcie);
    DevObject.GnbHandle = GnbHandle;
    DevObject.DevPciAddress.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0);
    GnbSmuServiceRequestV7 (
      &DevObject,
      SMC_MSG_PHY_LN_OFF,
      SmuArgument.Value,
      GNB_REG_ACC_FLAG_S3SAVE
      );
  }
  LaneBitmap = PcieUtilGetEngineLaneBitMap (LANE_TYPE_DDI_PHY_NATIVE_ACTIVE, 0, Engine);
  if (LaneBitmap != 0) {
    SmuArgument.Field.Tx = 1;
    SmuArgument.Field.Rx = 1;
    SmuArgument.Field.Core = 1;
    SmuArgument.Field.UpperLaneID = LibAmdBitScanReverse (LaneBitmap) + PcieConfigGetParentWrapper (Engine)->StartPhyLane;
    SmuArgument.Field.LowerLaneID = LibAmdBitScanForward (LaneBitmap) + PcieConfigGetParentWrapper (Engine)->StartPhyLane;
    IDS_HDT_CONSOLE (
      PCIE_MISC,
      "  DDI Lanes LowerLaneID - %02d UpperLaneID - %02d Tx - %d  Rx - %d Core - %d Exit\n",
      SmuArgument.Field.LowerLaneID,
      SmuArgument.Field.UpperLaneID,
      SmuArgument.Field.Tx,
      SmuArgument.Field.Rx,
      SmuArgument.Field.Core
      );

    DevObject.StdHeader = GnbLibGetHeader (Pcie);
    DevObject.GnbHandle = GnbHandle;
    DevObject.DevPciAddress.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0);
    GnbSmuServiceRequestV7 (
      &DevObject,
      SMC_MSG_PHY_LN_OFF,
      SmuArgument.Value,
      GNB_REG_ACC_FLAG_S3SAVE
      );

  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePowerGatePowerDownLanesCallbackKV Exit\n");
}


/*----------------------------------------------------------------------------------------*/
/**
 * Pcie Power gate init
 *
 *   Late PCIe initialization
 *
 * @param[in]  Pcie                Pointer to global PCIe configuration
 * @retval     AGESA_SUCCESS       Topology successfully mapped
 */

AGESA_STATUS
PciePowerGateKV (
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT8                   PowerGatingFlags;
  SMU_ARGUMENT_STRUCT     SmuArgument;
  GNB_HANDLE              *GnbHandle;
  D0F0xBC_x3FFFC_STRUCT   D0F0xBC_x3FFFC;

  IDS_HDT_CONSOLE (GNB_TRACE, "PciePowerGateKV Enter\n");

  GnbHandle = GnbGetHandle (GnbLibGetHeader (Pcie));

  PowerGatingFlags = GnbBuildOptions.CfgPciePowerGatingFlags;

  IDS_OPTION_HOOK (IDS_GNB_PCIE_POWER_GATING, &PowerGatingFlags, GnbLibGetHeader (Pcie));
  IDS_HDT_CONSOLE (GNB_TRACE, "  PowerGatingFlags = %02x\n", PowerGatingFlags);

  // Update flags
  SmuArgument.Value = 0;
  SmuArgument.Field.SkipCore = 1;
  SmuArgument.Field.SkipPhy = 1;

  if ((PowerGatingFlags & PCIE_POWERGATING_SKIP_CORE) == 0) {
    SmuArgument.Field.SkipCore = 0;
  }
  if ((PowerGatingFlags & PCIE_POWERGATING_SKIP_PHY) == 0) {
    SmuArgument.Field.SkipPhy = 0;
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "  PowerGatingFlags = %02x\n", PowerGatingFlags);
  IDS_HDT_CONSOLE (GNB_TRACE, "  SmuArgument.Value = %08x\n", SmuArgument.Value);

  D0F0xBC_x3FFFC.Value = SmuArgument.Value;
  IDS_HDT_CONSOLE (GNB_TRACE, "  3FFFC.Value = %08x\n", D0F0xBC_x3FFFC.Value);
  GnbRegisterWriteKV (GnbHandle, D0F0xBC_x3FFFC_TYPE,  D0F0xBC_x3FFFC_ADDRESS,
    &D0F0xBC_x3FFFC.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Pcie));

  // Power gate APU pcie phy lanes
  PcieConfigRunProcForAllWrappers (
    DESCRIPTOR_PCIE_WRAPPER | DESCRIPTOR_DDI_WRAPPER,
    PciePowerGateApuPhyCallbackKV,
    NULL,
    Pcie
    );

  // Power down unused lanes
  PcieConfigRunProcForAllWrappers (
    DESCRIPTOR_PCIE_WRAPPER | DESCRIPTOR_DDI_WRAPPER,
    PciePowerGatePowerDownUnusedLanesCallbackKV,
    &SmuArgument.Value,
    Pcie
    );
  //Power down hotplug lanes
  PcieConfigRunProcForAllEngines (
    DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE | DESCRIPTOR_DDI_ENGINE,
    PciePowerGatePowerDownLanesCallbackKV,
    &SmuArgument.Value,
    Pcie
    );
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePowerGateKV Exit\n");
  return  AGESA_SUCCESS;
}
