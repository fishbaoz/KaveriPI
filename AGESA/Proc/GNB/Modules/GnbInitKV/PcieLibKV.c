/* $NoKeywords:$ */
/**
 * @file
 *
 * KV specific PCIe services
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
#include  "Ids.h"
#include  "amdlib.h"
#include  "heapManager.h"
#include  "Gnb.h"
#include  "GnbPcie.h"
#include  "GnbSbLib.h"
#include  "GnbCommonLib.h"
#include  "GnbPcieInitLibV1.h"
#include  "GnbPcieInitLibV5.h"
#include  "GnbPcieConfig.h"
#include  "GnbPcieTrainingV2.h"
#include  "GnbNbInitLibV4.h"
#include  "GnbNbInitLibV1.h"
#include  "GnbNbInitLibV5.h"
#include  "PcieComplexDataKV.h"
#include  "PcieLibKV.h"
#include  "GnbRegistersKV.h"
#include  "GnbRegisterAccKV.h"
#include  "GnbFuseTable.h"
#include  "OptionGnb.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITKV_PCIELIBKV_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
extern GNB_BUILD_OPTIONS      GnbBuildOptions;


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
 * PLL powerdown
 *
 *
 * @param[in]  Wrapper     Pointer to Wrapper config descriptor
 * @param[in]  Pcie        Pointer to PICe configuration data area
 */

VOID
PciePifPllConfigureKV (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  D0F0xE4_PIF_0012_STRUCT D0F0xE4_PIF_0012;
  UINT8                   Pif;
  UINT8                   PowerGatingFlags;

  IDS_HDT_CONSOLE (GNB_TRACE, "PciePifPllConfigureKV Enter\n");
  D0F0xE4_PIF_0012.Value = PcieRegisterRead (
                             Wrapper,
                             PIF_SPACE (Wrapper->WrapId, 0, D0F0xE4_PIF_0012_ADDRESS),
                             Pcie
                             );

  D0F0xE4_PIF_0012.Field.PllRampUpTime = 0x0;

  if (Wrapper->Features.PllOffInL1 != 0) {
    D0F0xE4_PIF_0012.Field.TxPowerStateInTxs2 = PifPowerStateLS2;
    D0F0xE4_PIF_0012.Field.RxPowerStateInRxs2 = PifPowerStateLS2;
  } else {
    D0F0xE4_PIF_0012.Field.TxPowerStateInTxs2 = PifPowerStateL0;
    D0F0xE4_PIF_0012.Field.RxPowerStateInRxs2 = PifPowerStateL0;
  }

  PowerGatingFlags = GnbBuildOptions.CfgPciePowerGatingFlags;
  IDS_OPTION_HOOK (IDS_GNB_PCIE_POWER_GATING, &PowerGatingFlags, GnbLibGetHeader (Pcie));
  if ((Wrapper->Features.PowerOffUnusedPlls != 0) || ((PowerGatingFlags & PCIE_POWERGATING_SKIP_PHY) == 0)) {
    D0F0xE4_PIF_0012.Field.PllPowerStateInOff = PifPowerStateOff;
    D0F0xE4_PIF_0012.Field.PllPowerStateInTxs2 = PifPowerStateOff;
  } else {
    D0F0xE4_PIF_0012.Field.PllPowerStateInOff = PifPowerStateL0;
    D0F0xE4_PIF_0012.Field.PllPowerStateInTxs2 = PifPowerStateL0;
  }

  if (PcieConfigIsDdiWrapper (Wrapper) && !PcieConfigIsPcieWrapper (Wrapper)) {
    D0F0xE4_PIF_0012.Field.PllRampUpTime = 0x5;
  }

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
    // workaround for PCIe Link Down during Link Width Switching
    PcieRegisterRMW (
      Wrapper,
      PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0014_ADDRESS),
      D0F0xE4_PIF_0014_Service3Step4DelayPrgEn_MASK,
      1 << D0F0xE4_PIF_0014_Service3Step4DelayPrgEn_OFFSET,
      TRUE,
      Pcie
    );
    PcieRegisterRMW (
      Wrapper,
      PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_001D_ADDRESS),
      D0F0xE4_PIF_001D_PrgService3Step4Delay_MASK,
      0x2A30 << D0F0xE4_PIF_001D_PrgService3Step4Delay_OFFSET,
      TRUE,
      Pcie
    );
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "PciePifPllConfigureKV Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Power down DDI plls
 *
 *
 *
 * @param[in]  Wrapper         Pointer to wrapper config descriptor
 * @param[in]  Pcie            Pointer to global PCIe configuration
 */

VOID
PciePwrPowerDownDdiPllsKV (
  IN       PCIe_WRAPPER_CONFIG    *Wrapper,
  IN       PCIe_PLATFORM_CONFIG   *Pcie
  )
{
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePwrPowerDownDdiPllsKV Enter\n");
  if (PcieConfigIsDdiWrapper (Wrapper) && !PcieConfigIsPcieWrapper (Wrapper)) {
    PcieRegisterRMW (
      Wrapper,
      WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_802B_ADDRESS),
      D0F0xE4_WRAP_802B_LMTxPhyCmd0_MASK | D0F0xE4_WRAP_802B_LMTxPhyCmd1_MASK,
      (0x1 << D0F0xE4_WRAP_802B_LMTxPhyCmd0_OFFSET) | (0x1 << D0F0xE4_WRAP_802B_LMTxPhyCmd1_OFFSET),
      FALSE,
      Pcie
      );
    PciePollPifForCompeletion (Wrapper, Pcie);
    PcieRegisterRMW (
      Wrapper,
      WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_802B_ADDRESS),
      D0F0xE4_WRAP_802B_LMTxPhyCmd0_MASK | D0F0xE4_WRAP_802B_LMTxPhyCmd1_MASK,
      (0x7 << D0F0xE4_WRAP_802B_LMTxPhyCmd0_OFFSET) | (0x7 << D0F0xE4_WRAP_802B_LMTxPhyCmd1_OFFSET),
      FALSE,
      Pcie
      );
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePwrPowerDownDdiPllsKV Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Power down inactive lanes
 *
 *
 * @param[in]  Wrapper         Pointer to wrapper config descriptor
 * @param[in]  Pcie            Pointer to global PCIe configuration
 */

VOID
PciePowerDownPllInL1KV (
  IN       PCIe_WRAPPER_CONFIG    *Wrapper,
  IN       PCIe_PLATFORM_CONFIG   *Pcie
  )
{
  UINT32              LaneBitmapForPllOffInL1;
  UINT32              ActiveLaneBitmap;
  UINT8               PllPowerUpLatency;

  IDS_HDT_CONSOLE (GNB_TRACE, "PciePowerDownPllInL1KV Enter\n");
  ActiveLaneBitmap = PcieUtilGetWrapperLaneBitMap (LANE_TYPE_PCIE_PHY_NATIVE_ACTIVE | LANE_TYPE_PCIE_PHY_NATIVE_HOTPLUG, 0, Wrapper);
  if (ActiveLaneBitmap != 0) {
    PllPowerUpLatency = PciePifGetPllPowerUpLatencyKV (Wrapper, Pcie);
    LaneBitmapForPllOffInL1 = PcieLanesToPowerDownPllInL1 (PllPowerUpLatency, Wrapper, Pcie);
    if ((LaneBitmapForPllOffInL1 != 0) && ((ActiveLaneBitmap & LaneBitmapForPllOffInL1) == ActiveLaneBitmap)) {
      LaneBitmapForPllOffInL1 &= PcieUtilGetWrapperLaneBitMap (LANE_TYPE_PHY_NATIVE_ALL, 0, Wrapper);
      LaneBitmapForPllOffInL1 |= PcieUtilGetWrapperLaneBitMap (LANE_TYPE_PCIE_PHY_NATIVE_MASTER_PLL, 0, Wrapper);
      PciePifSetPllModeForL1 (LaneBitmapForPllOffInL1, Wrapper, Pcie);
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePowerDownPllInL1KV Exit\n");
}



/*----------------------------------------------------------------------------------------*/
/**
 * Request boot up voltage
 *
 *
 *
 * @param[in]  LinkCap             Global GEN capability
 * @param[in]  Pcie                Pointer to PCIe configuration data area
 */
VOID
PcieSetVoltageKV (
  IN      PCIE_LINK_SPEED_CAP   LinkCap,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT8                      TargetVid;
  UINT8                      MinVidIndex;
  UINT8                      VddNbVid[5];
  UINT8                      Index;
  PP_FUSE_ARRAY_V2           *PpFuseArray;
  UINT32                     Millivolt;
  D0F0xBC_xC0104004_STRUCT   D0F0xBC_xC0104004;
  D0F0xBC_xC0104007_STRUCT   D0F0xBC_xC0104007;
  D0F0xBC_xC0104008_STRUCT   D0F0xBC_xC0104008;
  D0F0xBC_xC0104098_STRUCT   D0F0xBC_xC0104098;
  D0F0xBC_xC0107067_STRUCT   D0F0xBC_xC0107067;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieSetVoltageKV Enter\n");

  Index = 0;
  PpFuseArray = GnbLocateHeapBuffer (AMD_PP_FUSE_TABLE_HANDLE, GnbLibGetHeader (Pcie));
  if (PpFuseArray == NULL) {
    GnbRegisterReadKV (GnbGetHandle (GnbLibGetHeader (Pcie)), D0F0xBC_xC0104004_TYPE, D0F0xBC_xC0104004_ADDRESS, &D0F0xBC_xC0104004, 0, GnbLibGetHeader (Pcie));
    GnbRegisterReadKV (GnbGetHandle (GnbLibGetHeader (Pcie)), D0F0xBC_xC0104007_TYPE, D0F0xBC_xC0104007_ADDRESS, &D0F0xBC_xC0104007, 0, GnbLibGetHeader (Pcie));
    GnbRegisterReadKV (GnbGetHandle (GnbLibGetHeader (Pcie)), D0F0xBC_xC0104008_TYPE, D0F0xBC_xC0104008_ADDRESS, &D0F0xBC_xC0104008, 0, GnbLibGetHeader (Pcie));
    GnbRegisterReadKV (GnbGetHandle (GnbLibGetHeader (Pcie)), D0F0xBC_xC0104098_TYPE, D0F0xBC_xC0104098_ADDRESS, &D0F0xBC_xC0104098, 0, GnbLibGetHeader (Pcie));
    GnbRegisterReadKV (GnbGetHandle (GnbLibGetHeader (Pcie)), D0F0xBC_xC0107067_TYPE, D0F0xBC_xC0107067_ADDRESS, &D0F0xBC_xC0107067, 0, GnbLibGetHeader (Pcie));
    VddNbVid[0] = (UINT8) D0F0xBC_xC0104004.Field.VddNbVid0;
    VddNbVid[1] = (UINT8) D0F0xBC_xC0104007.Field.VddNbVid1;
    VddNbVid[2] = (UINT8) D0F0xBC_xC0104008.Field.VddNbVid2;
    VddNbVid[3] = (UINT8) D0F0xBC_xC0104008.Field.VddNbVid3;
    VddNbVid[4] = (UINT8) D0F0xBC_xC0104098.Field.VddNbVid4;
    if (LinkCap <= PcieGen2) {
      Index = (UINT8) D0F0xBC_xC0107067.Field.PcieGen2Vid;
    } else if (LinkCap >= PcieGen3) {
      Index = (UINT8) D0F0xBC_xC0107067.Field.PcieGen3Vid;
    }
  } else {
    VddNbVid[0] = PpFuseArray->VddNbVid[0];
    VddNbVid[1] = PpFuseArray->VddNbVid[1];
    VddNbVid[2] = PpFuseArray->VddNbVid[2];
    VddNbVid[3] = PpFuseArray->VddNbVid[3];
    VddNbVid[4] = PpFuseArray->VddNbVid[4];
    if (LinkCap <= PcieGen2) {
      Index = PpFuseArray->PcieGen2Vid;
    } else if (LinkCap >= PcieGen3) {
      Index = PpFuseArray->PcieGen3Vid;
    }
  }

  if (LinkCap > PcieGen1) {

    ASSERT (VddNbVid[Index] != 0);
    TargetVid = VddNbVid[Index];

  } else {

    MinVidIndex = 0;
    for (Index = 0; Index < 5; Index++) {
      if (VddNbVid[Index] > VddNbVid[MinVidIndex]) {
        MinVidIndex = (UINT8) Index;
      }
    }
    ASSERT (VddNbVid[MinVidIndex] != 0);
    TargetVid = VddNbVid[MinVidIndex];

  }

  IDS_HDT_CONSOLE (PCIE_MISC, "  Set Voltage for Gen %d, Vid code %d\n", LinkCap, TargetVid);
  Millivolt = GnbTranslateVidCodeToMillivoltV5 (TargetVid, GnbLibGetHeader (Pcie)) * 4 / 100;
  GnbRegisterWriteKV (GnbGetHandle (GnbLibGetHeader (Pcie)), TYPE_SMU_MSG, SMC_MSG_VDDNB_REQUEST, &Millivolt, 0, GnbLibGetHeader (Pcie));
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieSetVoltageKV Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * PLL power up latency
 *
 *
 * @param[in]  Wrapper     Pointer to Wrapper config descriptor
 * @param[in]  Pcie        Pointer to PICe configuration data area
 * @retval                 Pll wake up latency in us
 */
UINT8
PciePifGetPllPowerUpLatencyKV (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  return 35;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Get max link speed capability supported by this port
 *
 *
 *
 * @param[in]  Flags              See Flags PCIE_PORT_GEN_CAP_BOOT / PCIE_PORT_GEN_CAP_MAX
 * @param[in]  Engine             Pointer to engine config descriptor
 * @retval     PcieGen1/PcieGen2  Max supported link gen capability
 */
PCIE_LINK_SPEED_CAP
PcieGetLinkSpeedCapKV (
  IN       UINT32                Flags,
  IN       PCIe_ENGINE_CONFIG    *Engine
  )
{
  PCIE_LINK_SPEED_CAP   LinkSpeedCapability;
  PCIe_WRAPPER_CONFIG   *Wrapper;
  PCIe_PLATFORM_CONFIG  *Pcie;
  D18F3x1FC_STRUCT      D18F3x1FC;

  Wrapper = PcieConfigGetParentWrapper (Engine);
  Pcie = PcieConfigGetPlatform (Wrapper);

  LinkSpeedCapability = PcieGen2;

  GnbRegisterReadKV (GnbGetHandle (GnbLibGetHeader (Pcie)), D18F3x1FC_TYPE, D18F3x1FC_ADDRESS, &D18F3x1FC.Value, 0, GnbLibGetHeader (Pcie));
  if (D18F3x1FC.Field.VddrLowVoltageSupport == 0) {
    if (Wrapper->WrapId == GFX_WRAP_ID) {
      if (PcieTopologyIsGen3SupportedV5 (Wrapper)) {
        LinkSpeedCapability = PcieGen3;
      }
    }
  }
  if (Engine->Type.Port.PortData.LinkSpeedCapability == PcieGenMaxSupported) {
    Engine->Type.Port.PortData.LinkSpeedCapability = (UINT8) LinkSpeedCapability;
  }
  if (Pcie->PsppPolicy == PsppPowerSaving) {
    LinkSpeedCapability = PcieGen1;
  }
  if (Engine->Type.Port.PortData.LinkSpeedCapability < LinkSpeedCapability) {
    LinkSpeedCapability = Engine->Type.Port.PortData.LinkSpeedCapability;
  }
  if ((Flags & PCIE_PORT_GEN_CAP_BOOT) != 0) {
    if ((Pcie->PsppPolicy == PsppBalanceLow || Engine->Type.Port.PortData.MiscControls.LinkSafeMode == PcieGen1) && !PcieConfigIsSbPcieEngine (Engine)) {
      LinkSpeedCapability = PcieGen1;
    }
  }
  return LinkSpeedCapability;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Get timestamp
 *
 *
 *
 * @param[in]  StdHeader             Standard header
 * @retval     TimeStamp             Count
 */

UINT32
GnbTimeStampKV (
  IN       AMD_CONFIG_PARAMS          *StdHeader
  )
{
  UINT32 TimeStamp;

  TimeStamp = 0;

  GnbLibPciIndirectRead (
    MAKE_SBDFO (0, 0, 0, 0, 0xE0),
    0x13280F0,
    AccessWidth32,
    &TimeStamp,
    StdHeader
    );

  return TimeStamp;

}

/*----------------------------------------------------------------------------------------*/
/**
 * UnitID Clumping
 *
 *
 * @param[in]  GnbHandle       GNB handle
 * @param[in]  StdHeader       Standard configuration header
 */

VOID
GnbClumpUnitIdKV (
  IN      GNB_HANDLE                *GnbHandle,
  IN      AMD_CONFIG_PARAMS         *StdHeader
  )
{

  D0F0xE4_WRAP_0080_STRUCT  D0F0xE4_WRAP_0080;
  D0F0x98_x3A_STRUCT        D0F0x98_x3A;

  D0F0x98_x3A.Value = 0xE << D0F0x98_x3A_GBIFClumpingEn_OFFSET;
  GnbLibPciIndirectRead (MAKE_SBDFO (0, 0, 0, 0, 0) | 0xE0, WRAP_SPACE (GFX_WRAP_ID, D0F0xE4_WRAP_0080_ADDRESS), AccessWidth32, &D0F0xE4_WRAP_0080.Value, StdHeader);
  if (D0F0xE4_WRAP_0080.Field.StrapBifLinkConfig == 0x5) {
    D0F0x98_x3A.Field.PGDClumpingEn = 0xA;
  }
  if (D0F0xE4_WRAP_0080.Field.StrapBifLinkConfig == 0x0) {
    D0F0x98_x3A.Field.PGDClumpingEn = 0xE;
  }
  // Set GNB
  GnbLibPciIndirectWrite (
    GnbHandle->Address.AddressValue | D0F0x94_ADDRESS,
    D0F0x98_x3A_ADDRESS,
    AccessS3SaveWidth32,
    &D0F0x98_x3A.Value,
    StdHeader
    );
  //Set UNB
  D0F0x98_x3A.Value |= BIT1;
  GnbLibPciWrite (
    MAKE_SBDFO (0, 0, 0x18, 0, D18F0x110_ADDRESS),
    AccessS3SaveWidth32,
    &D0F0x98_x3A.Value,
    StdHeader
    );
}

/*----------------------------------------------------------------------------------------*/
/**
 * Point "virtual" PLL clock picker away from PCIe
 *
 *
 *
 * @param[in]  Wrapper             Pointer to internal configuration data area
 * @param[in]  Pcie                Pointer to global PCIe configuration
 */
VOID
PciePhyAvertClockPickersKV (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT32  DdiLanes;
  UINT8   Nibble;
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePhyAvertClockPickersKV Enter\n");
  if (Wrapper->WrapId == DDI_WRAP_ID) {
    DdiLanes = 0xFF;
  } else if (Wrapper->WrapId == GPPSB_WRAP_ID) {
    DdiLanes = 0xF0;
  } else {
    DdiLanes = PcieUtilGetWrapperLaneBitMap (LANE_TYPE_DDI_PHY_NATIVE, 0, Wrapper);
  }
  for (Nibble = 0; Nibble < 4; Nibble++) {
    if (DdiLanes & (0xf << (Nibble * 4))) {
      PcieRegisterRMW (
        Wrapper,
        PHY_SPACE (Wrapper->WrapId, (Nibble >> 1), D0F0xE4_PHY_000B_ADDRESS + (Nibble & 0x1)),
        D0F0xE4_PHY_000B_MargPktSbiEn_MASK | D0F0xE4_PHY_000B_PcieModeSbiEn_MASK,
        (0x0 << D0F0xE4_PHY_000B_MargPktSbiEn_OFFSET) | (0x0 << D0F0xE4_PHY_000B_PcieModeSbiEn_OFFSET),
        FALSE,
        Pcie
        );
      if ((Nibble & 0x1) == 0) {
        PcieRegisterRMW (
          Wrapper,
          PHY_SPACE (Wrapper->WrapId, (Nibble >> 1), D0F0xE4_PHY_2012_ADDRESS),
          D0F0xE4_PHY_2012_PcieModeSub0_MASK | D0F0xE4_PHY_2012_PllSrcSub0_MASK,
          (0 << D0F0xE4_PHY_2012_PcieModeSub0_OFFSET) | (0 << D0F0xE4_PHY_2012_PllSrcSub0_OFFSET),
          FALSE,
          Pcie
          );
      } else {
        PcieRegisterRMW (
          Wrapper,
          PHY_SPACE (Wrapper->WrapId, (Nibble >> 1), D0F0xE4_PHY_2013_ADDRESS),
          D0F0xE4_PHY_2013_PcieModeSub1_MASK | D0F0xE4_PHY_2013_PllSrcSub1_MASK,
          (0 << D0F0xE4_PHY_2013_PcieModeSub1_OFFSET) | (1 << D0F0xE4_PHY_2013_PllSrcSub1_OFFSET),
          FALSE,
          Pcie
          );
      }
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePhyAvertClockPickersKV Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Power down unused lanes and plls
 *
 *
 * @param[in]  Wrapper         Pointer to wrapper config descriptor
 * @param[in]  Pcie            Pointer to global PCIe configuration
 */

VOID
PciePwrPowerDownUnusedLanesKV (
  IN       PCIe_WRAPPER_CONFIG    *Wrapper,
  IN       PCIe_PLATFORM_CONFIG   *Pcie
  )
{
  UINT32              UnusedLanes;
  UINT8                   PowerGatingFlags;
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePwrPowerDownUnusedLanesKV Enter\n");
  PowerGatingFlags = GnbBuildOptions.CfgPciePowerGatingFlags;
  IDS_OPTION_HOOK (IDS_GNB_PCIE_POWER_GATING, &PowerGatingFlags, GnbLibGetHeader (Pcie));
  if ((Wrapper->Features.PowerOffUnusedLanes != 0) || ((PowerGatingFlags & PCIE_POWERGATING_SKIP_PHY) == 0)) {
    UnusedLanes = PcieUtilGetWrapperLaneBitMap (LANE_TYPE_CORE_ALL, LANE_TYPE_PCIE_CORE_ALLOC_ACTIVE, Wrapper);
    PcieTopologyLaneControlV5 (
      DisableLanes,
      UnusedLanes,
      Wrapper,
      Pcie
      );
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePwrPowerDownUnusedLanesKV Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * PHY lane ganging
 *
 *
 *
 * @param[out] Wrapper             Pointer to internal configuration data area
 * @param[in]  Pcie                Pointer to global PCIe configuration
 */
VOID
PciePhyApplyGangingKV (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIe_ENGINE_CONFIG      *EngineList;
  UINT8                   PhyLinkWidth;
  UINT32                  RdptInitMode;
  UINT32                  IsOwnMstr;
  UINT32                  GangedModeEn;
  UINT8                   PhyLane;
  UINT32                  LaneBitmap;
  D0F0xE4_PHY_E006_STRUCT D0F0xE4_PHY_E006;

  IDS_HDT_CONSOLE (GNB_TRACE, "PciePhyApplyGangingV5 Enter\n");
  EngineList = PcieConfigGetChildEngine (Wrapper);
  while (EngineList != NULL) {
    if (PcieConfigIsEngineAllocated (EngineList)) {
      LaneBitmap = PcieUtilGetEngineLaneBitMap (LANE_TYPE_PCIE_PHY | LANE_TYPE_DDI_PHY, 0, EngineList);
      PhyLinkWidth = LibAmdBitScanReverse (LaneBitmap) - LibAmdBitScanForward (LaneBitmap) + 1;
      // Initialize values from a register read
      PhyLane = LibAmdBitScanForward (LaneBitmap);
      D0F0xE4_PHY_E006.Value = PcieRegisterRead (
        Wrapper,
        PHY_SPACE (Wrapper->WrapId, (PhyLane / MAX_NUM_LANE_PER_PHY), D0F0xE4_PHY_E006_ADDRESS + (PhyLane % MAX_NUM_LANE_PER_PHY) * 0x80),
        Pcie
        );
      RdptInitMode = D0F0xE4_PHY_E006.Field.RdptInitMode;
      IsOwnMstr = D0F0xE4_PHY_E006.Field.IsOwnMstr;
      GangedModeEn = D0F0xE4_PHY_E006.Field.GangedModeEn;
      if (PcieConfigIsPcieEngine (EngineList)) {
        RdptInitMode = 0;
        switch (PhyLinkWidth) {
        case 1:
          RdptInitMode = 0;
          break;
        case 2:
          RdptInitMode = 1;
          break;
        case 4:
          RdptInitMode = 2;
          break;
        case 8:
          RdptInitMode = 3;
          break;
        case 16:
          RdptInitMode = 4;
          break;
        default:
          ASSERT (FALSE);
        }
        IsOwnMstr = 1;
      } else {
        IsOwnMstr = 0;
        if (PhyLinkWidth == 8) {
          GangedModeEn = 1;
        } else {
          GangedModeEn = 0;
        }
      }
      for (PhyLane = LibAmdBitScanForward (LaneBitmap); PhyLane <= LibAmdBitScanReverse (LaneBitmap); ++PhyLane) {

        D0F0xE4_PHY_E006.Value = PcieRegisterRead (
          Wrapper,
          PHY_SPACE (Wrapper->WrapId, (PhyLane / MAX_NUM_LANE_PER_PHY), D0F0xE4_PHY_E006_ADDRESS + (PhyLane % MAX_NUM_LANE_PER_PHY) * 0x80),
          Pcie
          );
        D0F0xE4_PHY_E006.Field.RdptInitMode = RdptInitMode;
        D0F0xE4_PHY_E006.Field.IsOwnMstr = IsOwnMstr;
        D0F0xE4_PHY_E006.Field.GangedModeEn = GangedModeEn;
        PcieRegisterWrite (
          Wrapper,
          PHY_SPACE (Wrapper->WrapId, (PhyLane / MAX_NUM_LANE_PER_PHY), D0F0xE4_PHY_E006_ADDRESS + (PhyLane % MAX_NUM_LANE_PER_PHY) * 0x80),
          D0F0xE4_PHY_E006.Value,
          FALSE,
          Pcie
          );
      }
    }
    EngineList = PcieLibGetNextDescriptor (EngineList);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePhyApplyGangingV5 Exit\n");
}


