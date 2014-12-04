/* $NoKeywords:$ */
/**
 * @file
 *
 * Family-specific GNB initialization at AMD_INIT_MID
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
#include  "Gnb.h"
#include  "GnbPcie.h"
#include  "GnbFuseTable.h"
#include  "heapManager.h"
#include  "GnbCommonLib.h"
#include  "GnbPcieConfig.h"
#include  "GnbNbInitLibV1.h"
#include  "GnbNbInitLibV4.h"
#include  "GnbGfxInitLibV1.h"
#include  "GnbGfxConfig.h"
#include  "GnbTable.h"
#include  "GnbRegisterAccTN.h"
#include  "GnbRegistersTN.h"
#include  "OptionGnb.h"
#include  "GfxLibTN.h"
#include  "GnbFamServices.h"
#include  "GnbGfxFamServices.h"
#include  "GnbBapmCoeffCalcTN.h"
#include  "PcieComplexDataTN.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITTN_GNBMIDINITTN_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

extern GNB_BUILD_OPTIONS_TN   GnbBuildOptionsTN;
extern GNB_TABLE ROMDATA      GnbMidInitTableTN[];

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */
#define NUM_DPM_STATES 8

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 * Registers need to be set if no GFX PCIe ports are used
 *
 *
 *
 * @param[in]  Pcie                Pointer to PCIe_PLATFORM_CONFIG
 */

VOID
STATIC
GnbIommuMidInitCheckGfxPciePorts (
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIe_WRAPPER_CONFIG   *WrapperList;
  BOOLEAN               GfxPciePortUsed;
  D0F2xF4_x57_STRUCT    D0F2xF4_x57;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbIommuMidInitCheckGfxPciePorts Enter\n");
  GfxPciePortUsed = FALSE;

  WrapperList = PcieConfigGetChildWrapper (Pcie);
  ASSERT (WrapperList != NULL);
  if (WrapperList->WrapId == GFX_WRAP_ID) {
    PCIe_ENGINE_CONFIG   *EngineList;
    EngineList = PcieConfigGetChildEngine (WrapperList);
    while (EngineList != NULL) {
      if (PcieConfigIsPcieEngine (EngineList)) {
        IDS_HDT_CONSOLE (GNB_TRACE, "Checking Gfx ports device number %x\n", EngineList->Type.Port.NativeDevNumber);
        if (PcieConfigCheckPortStatus (EngineList, INIT_STATUS_PCIE_TRAINING_SUCCESS) ||
           ((EngineList->Type.Port.PortData.LinkHotplug != HotplugDisabled) && (EngineList->Type.Port.PortData.LinkHotplug != HotplugInboard))) {
          // GFX PCIe ports are being used
          GfxPciePortUsed = TRUE;
          IDS_HDT_CONSOLE (GNB_TRACE, "GFX PCIe ports are being used\n");
          break;
        }
      }
      EngineList = PcieLibGetNextDescriptor (EngineList);
    }
  }

  if (!GfxPciePortUsed) {
    //D0F2xF4_x57.Field.L1ImuPcieGfxDis needs to be set
    GnbRegisterReadTN (D0F2xF4_x57_TYPE, D0F2xF4_x57_ADDRESS, &D0F2xF4_x57.Value, 0, GnbLibGetHeader (Pcie));
    D0F2xF4_x57.Field.L1ImuPcieGfxDis = 1;
    GnbRegisterWriteTN (D0F2xF4_x57_TYPE, D0F2xF4_x57_ADDRESS, &D0F2xF4_x57.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Pcie));
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbIommuMidInitCheckGfxPciePorts Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback to for each PCIe port
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
GnbIommuMidInitOnPortCallback (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  GNB_TOPOLOGY_INFO           TopologyInfo;
  D0F2xFC_x07_L1_STRUCT       D0F2xFC_x07_L1;
  D0F2xFC_x0D_L1_STRUCT       D0F2xFC_x0D_L1;
  UINT8                       L1cfgSel;
  TopologyInfo.PhantomFunction = FALSE;
  TopologyInfo.PcieToPciexBridge = FALSE;
  if (Engine->Type.Port.PortData.LinkHotplug != HotplugDisabled) {
    TopologyInfo.PhantomFunction = TRUE;
    TopologyInfo.PcieToPciexBridge = TRUE;
  } else {
    if (PcieConfigIsSbPcieEngine (Engine)) {
      PCI_ADDR  StartSbPcieDev;
      PCI_ADDR  EndSbPcieDev;
      StartSbPcieDev.AddressValue = MAKE_SBDFO (0, 0, 0x15, 0, 0);
      EndSbPcieDev.AddressValue = MAKE_SBDFO (0, 0, 0x15, 7, 0);
      GnbGetTopologyInfoV4 (StartSbPcieDev, EndSbPcieDev, &TopologyInfo, GnbLibGetHeader (Pcie));
    } else {
      GnbGetTopologyInfoV4 (Engine->Type.Port.Address, Engine->Type.Port.Address, &TopologyInfo, GnbLibGetHeader (Pcie));
    }
  }
  L1cfgSel = (Engine->Type.Port.CoreId == 1) ? 1 : 0;
  GnbRegisterReadTN (
    D0F2xFC_x07_L1_TYPE,
    D0F2xFC_x07_L1_ADDRESS (L1cfgSel),
    &D0F2xFC_x07_L1.Value,
    0,
    GnbLibGetHeader (Pcie)
    );
  if (TopologyInfo.PhantomFunction) {
    D0F2xFC_x07_L1.Value &= ~BIT0;
  } else {
    D0F2xFC_x07_L1.Value |= BIT0;
  }
  GnbRegisterWriteTN (
    D0F2xFC_x07_L1_TYPE,
    D0F2xFC_x07_L1_ADDRESS (L1cfgSel),
    &D0F2xFC_x07_L1.Value,
    GNB_REG_ACC_FLAG_S3SAVE,
    GnbLibGetHeader (Pcie)
    );

  if (TopologyInfo.PcieToPciexBridge) {
    GnbRegisterReadTN (
      D0F2xFC_x0D_L1_TYPE,
      D0F2xFC_x0D_L1_ADDRESS (L1cfgSel),
      &D0F2xFC_x0D_L1.Value,
      0,
      GnbLibGetHeader (Pcie)
      );
    D0F2xFC_x0D_L1.Field.VOQPortBits = 0x7;
    GnbRegisterWriteTN (
      D0F2xFC_x0D_L1_TYPE,
      D0F2xFC_x0D_L1_ADDRESS (L1cfgSel),
      &D0F2xFC_x0D_L1.Value,
      GNB_REG_ACC_FLAG_S3SAVE,
      GnbLibGetHeader (Pcie)
      );
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Orb/Ioc Cgtt Override setting
 *
 *
 * @param[in]  Property   Property
 * @param[in]  StdHeader  Standard configuration header
 */

VOID
STATIC
GnbCgttOverrideTN (
  IN      UINT32                          Property,
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  UINT32                          CGINDx0_Value;
  UINT32                          CGINDx1_Value;
  GFX_PLATFORM_CONFIG             *Gfx;
  AGESA_STATUS                    Status;
  D0F0x64_x23_STRUCT              D0F0x64_x23;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbCgttOverrideTN Enter\n");

  CGINDx0_Value = 0xFFFFFFFF;
  //When orb clock gating is enabled in the BIOS clear CG_ORB_cgtt_lclk_override - bit 13
  CGINDx1_Value = 0xFFFFFFFF;
  if ((Property & TABLE_PROPERTY_ORB_CLK_GATING) == TABLE_PROPERTY_ORB_CLK_GATING) {
    CGINDx1_Value &= 0xFFFFDFFF;
  }
  //When ioc clock gating is enabled in the BIOS clear CG_IOC_cgtt_lclk_override - bit 15
  if ((Property & TABLE_PROPERTY_IOC_LCLK_CLOCK_GATING) == TABLE_PROPERTY_IOC_LCLK_CLOCK_GATING) {
    CGINDx1_Value &= 0xFFFF7FFF;
    if ((Property & TABLE_PROPERTY_IOMMU_DISABLED) != TABLE_PROPERTY_IOMMU_DISABLED) {
      //only IOMMU enabled and IOC clock gating enable
      GnbRegisterReadTN (D0F0x64_x23_TYPE, D0F0x64_x23_ADDRESS, &D0F0x64_x23.Value, 0, StdHeader);
      D0F0x64_x23.Field.SoftOverrideClk0 = 1;
      D0F0x64_x23.Field.SoftOverrideClk1 = 1;
      D0F0x64_x23.Field.SoftOverrideClk3 = 1;
      D0F0x64_x23.Field.SoftOverrideClk4 = 1;
      GnbRegisterWriteTN (D0F0x64_x23_TYPE, D0F0x64_x23_ADDRESS, &D0F0x64_x23.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
    }
  }
  //When smu sclk clock gating is enabled in the BIOS clear CG_IOC_cgtt_lclk_override - bit 18
  if ((Property & TABLE_PROPERTY_SMU_SCLK_CLOCK_GATING) == TABLE_PROPERTY_SMU_SCLK_CLOCK_GATING) {
    CGINDx1_Value &= 0xFFFBFFFF;
  }

  Status = GfxLocateConfigData (StdHeader, &Gfx);
  if (Status != AGESA_FATAL) {
    if (Gfx->GmcClockGating) {
      //In addition to above registers it is necessary to reset override bits for VMC, MCB, and MCD blocks
      // CGINDx0, clear bit 27, bit 28
      CGINDx0_Value &= 0xE7FFFFFF;
      GnbRegisterWriteTN (TYPE_CGIND, 0x0, &CGINDx0_Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
      // CGINDx1, clear bit 11
      CGINDx1_Value &= 0xFFFFF7FF;
    }

  }

  if (CGINDx1_Value != 0xFFFFFFFF) {
    GnbRegisterWriteTN (TYPE_CGIND, 0x1, &CGINDx1_Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbCgttOverrideTN Exit\n");

}

/*----------------------------------------------------------------------------------------*/
/**
 * IOMMU Mid Init
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
GnbIommuMidInit (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS          Status;
  PCIe_PLATFORM_CONFIG  *Pcie;
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbIommuMidInit Enter\n");
  Status = PcieLocateConfigurationData (StdHeader, &Pcie);
  if (Status == AGESA_SUCCESS) {
    PcieConfigRunProcForAllEngines (
      DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
      GnbIommuMidInitOnPortCallback,
      NULL,
      Pcie
      );
  }

  GnbIommuMidInitCheckGfxPciePorts (Pcie);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbIommuMidInit Exit [0x%x]\n", Status);
  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * LCLK DPM Initialization
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
GnbLclkDpmInitTN (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS              Status;
  PCIe_PLATFORM_CONFIG      *Pcie;
  PP_FUSE_ARRAY             *PpFuseArray;
  PCI_ADDR                  GnbPciAddress;
  UINT32                    Index;
  UINT8                     LclkDpmMode;
  D0F0xBC_x1F200_STRUCT     D0F0xBC_x1F200[NUM_DPM_STATES];
  D0F0xBC_x1F208_STRUCT     D0F0xBC_x1F208[NUM_DPM_STATES];
  D0F0xBC_x1F210_STRUCT     D0F0xBC_x1F210[NUM_DPM_STATES];
  D0F0xBC_x1F300_STRUCT     D0F0xBC_x1F300;
  D0F0xBC_x1F940_STRUCT     D0F0xBC_x1F940[NUM_DPM_STATES];
  DOUBLE                    PcieCacLut;
  D0F0xBC_x1F920_STRUCT     D0F0xBC_x1F920;
  D0F0xBC_x1FE00_STRUCT     D0F0xBC_x1FE00;
  D0F0xBC_x1F30C_STRUCT     D0F0xBC_x1F30C;
  D18F3x64_STRUCT           D18F3x64;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbLclkDpmInitTN Enter\n");
  Status = AGESA_SUCCESS;
  LclkDpmMode = GnbBuildOptionsTN.GnbCommonOptions.LclkDpmEn ? LclkDpmRcActivity : LclkDpmDisabled;
  IDS_OPTION_HOOK (IDS_GNB_LCLK_DPM_EN, &LclkDpmMode, StdHeader);
  if (LclkDpmMode == LclkDpmRcActivity) {
    PpFuseArray = GnbLocateHeapBuffer (AMD_PP_FUSE_TABLE_HANDLE, StdHeader);
    if (PpFuseArray != NULL) {
      Status = PcieLocateConfigurationData (StdHeader, &Pcie);
      if (Status == AGESA_SUCCESS) {
        GnbPciAddress.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0);
        //Clear DPM_EN bit in LCLK_DPM_CNTL register
        //Call BIOS service SMC_MSG_CONFIG_LCLK_DPM to disable LCLK DPM
        GnbRegisterReadTN (D0F0xBC_x1F300_TYPE, D0F0xBC_x1F300_ADDRESS, &D0F0xBC_x1F300.Value, 0, StdHeader);
        D0F0xBC_x1F300.Field.LclkDpmEn = 0x0;
        GnbRegisterWriteTN (D0F0xBC_x1F300_TYPE, D0F0xBC_x1F300_ADDRESS, &D0F0xBC_x1F300.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
        GnbSmuServiceRequestV4 (
          GnbPciAddress,
          SMC_MSG_CONFIG_LCLK_DPM,
          GNB_REG_ACC_FLAG_S3SAVE,
          StdHeader
          );

        //Initialize LCLK states
        LibAmdMemFill (D0F0xBC_x1F200, 0x00, sizeof (D0F0xBC_x1F200), StdHeader);
        LibAmdMemFill (D0F0xBC_x1F208, 0x00, sizeof (D0F0xBC_x1F208), StdHeader);
        LibAmdMemFill (D0F0xBC_x1F940, 0x00, sizeof (D0F0xBC_x1F208), StdHeader);

        D0F0xBC_x1F200[0].Field.LclkDivider = PpFuseArray->LclkDpmDid[0];
        D0F0xBC_x1F200[0].Field.VID = PpFuseArray->SclkVid[PpFuseArray->LclkDpmVid[0]];
        D0F0xBC_x1F200[0].Field.LowVoltageReqThreshold = 0xa;
        D0F0xBC_x1F210[0].Field.ActivityThreshold = 0xf;

        D0F0xBC_x1F200[5].Field.LclkDivider = PpFuseArray->LclkDpmDid[1];
        D0F0xBC_x1F200[5].Field.VID = PpFuseArray->SclkVid[PpFuseArray->LclkDpmVid[1]];
        D0F0xBC_x1F200[5].Field.LowVoltageReqThreshold = 0xa;
        D0F0xBC_x1F210[5].Field.ActivityThreshold = 0x32;
        D0F0xBC_x1F200[5].Field.StateValid = 0x1;

        D0F0xBC_x1F200[6].Field.LclkDivider = PpFuseArray->LclkDpmDid[2];
        D0F0xBC_x1F200[6].Field.VID = PpFuseArray->SclkVid[PpFuseArray->LclkDpmVid[2]];
        D0F0xBC_x1F200[6].Field.LowVoltageReqThreshold = 0xa;
        D0F0xBC_x1F210[6].Field.ActivityThreshold = 0x32;
        D0F0xBC_x1F200[6].Field.StateValid = 0x1;

        //5.7028E-04 * FDPM (in MHz) * 2^GPU_CAC_AVRG_CNTL.WEIGHT_PREC
        GnbRegisterReadTN (D0F0xBC_x1F920_TYPE, D0F0xBC_x1F920_ADDRESS, &D0F0xBC_x1F920.Value, 0, StdHeader);
        PcieCacLut = 0.0000057028 * (1 << D0F0xBC_x1F920.Field.WeightPrec);
        IDS_HDT_CONSOLE (GNB_TRACE, "LCLK DPM1 10khz %x (%d)\n", GfxFmCalculateClock (PpFuseArray->LclkDpmDid[1], StdHeader), GfxFmCalculateClock (PpFuseArray->LclkDpmDid[1], StdHeader));
        D0F0xBC_x1FE00.Field.Data = (UINT32) GnbFpLibDoubleToInt32 (PcieCacLut * GfxFmCalculateClock (PpFuseArray->LclkDpmDid[1], StdHeader));
        GnbRegisterWriteTN (D0F0xBC_x1FE00_TYPE, D0F0xBC_x1FE00_ADDRESS, &D0F0xBC_x1FE00.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
        //5.40239329E-04 * FDPM2 (in MHz) * 2^GPU_CAC_AVRG_CNTL.WEIGHT_PREC
        PcieCacLut = 0.00000540239329 * (1 << D0F0xBC_x1F920.Field.WeightPrec);
        D0F0xBC_x1F940[6].Field.PcieCacLut = (UINT32) GnbFpLibDoubleToInt32 (PcieCacLut * GfxFmCalculateClock (PpFuseArray->LclkDpmDid[2], StdHeader));
        IDS_HDT_CONSOLE (GNB_TRACE, "LCLK DPM2 10khz %x (%d)\n", GfxFmCalculateClock (PpFuseArray->LclkDpmDid[2], StdHeader), GfxFmCalculateClock (PpFuseArray->LclkDpmDid[2], StdHeader));

        for (Index = 0; Index < NUM_DPM_STATES; ++Index) {
          GnbRegisterWriteTN (
            D0F0xBC_x1F200_TYPE,
            D0F0xBC_x1F200_ADDRESS + Index * 0x20,
            &D0F0xBC_x1F200[Index].Value,
            GNB_REG_ACC_FLAG_S3SAVE,
            StdHeader
            );
          GnbRegisterWriteTN (
            D0F0xBC_x1F208_TYPE,
            D0F0xBC_x1F208_ADDRESS + Index * 0x20,
            &D0F0xBC_x1F208[Index].Value,
            GNB_REG_ACC_FLAG_S3SAVE,
            StdHeader
            );
          GnbRegisterWriteTN (
            D0F0xBC_x1F210_TYPE,
            D0F0xBC_x1F210_ADDRESS + Index * 0x20,
            &D0F0xBC_x1F210[Index].Value,
            GNB_REG_ACC_FLAG_S3SAVE,
            StdHeader
            );
          GnbRegisterWriteTN (
            D0F0xBC_x1F940_TYPE,
            D0F0xBC_x1F940_ADDRESS + Index * 4,
            &D0F0xBC_x1F940[Index].Value,
            GNB_REG_ACC_FLAG_S3SAVE,
            StdHeader
            );
        }
        //Enable LCLK DPM Voltage Scaling
        GnbRegisterReadTN (D0F0xBC_x1F300_TYPE, D0F0xBC_x1F300_ADDRESS, &D0F0xBC_x1F300.Value, 0, StdHeader);
        D0F0xBC_x1F300.Field.VoltageChgEn = 0x1;
        D0F0xBC_x1F300.Field.LclkDpmEn = 0x1;
        D0F0xBC_x1F300.Field.LclkDpmBootState = 0x5;
        GnbRegisterWriteTN (D0F0xBC_x1F300_TYPE, D0F0xBC_x1F300_ADDRESS, &D0F0xBC_x1F300.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);

        //Programming Lclk Thermal Throttling Threshold
        GnbRegisterReadTN (D18F3x64_TYPE, D18F3x64_ADDRESS, &D18F3x64.Value, 0, StdHeader);
        GnbRegisterReadTN (D0F0xBC_x1F30C_TYPE, D0F0xBC_x1F30C_ADDRESS, &D0F0xBC_x1F30C.Value, 0, StdHeader);
        D0F0xBC_x1F30C.Field.LowThreshold = (UINT16) (((D18F3x64.Field.HtcTmpLmt / 2 + 52) - 1 + 49) * 8);
        D0F0xBC_x1F30C.Field.HighThreshold = (UINT16) (((D18F3x64.Field.HtcTmpLmt / 2 + 52) + 49) * 8);
        GnbRegisterWriteTN (D0F0xBC_x1F30C_TYPE, D0F0xBC_x1F30C_ADDRESS, &D0F0xBC_x1F30C.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);

        GnbSmuServiceRequestV4 (
          GnbPciAddress,
          SMC_MSG_CONFIG_LCLK_DPM,
          GNB_REG_ACC_FLAG_S3SAVE,
          StdHeader
          );
      }
    } else {
      Status = AGESA_ERROR;
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbLclkDpmInitTN Exit [0x%x]\n", Status);
  return  Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * PCIe Mid Post Init
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
GnbMidInterfaceTN (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS      Status;
  UINT32            Property;
  AGESA_STATUS      AgesaStatus;
  GNB_HANDLE        *GnbHandle;
  UINT8             SclkDid;
  UINT32            MxcsrValue;
  UINT32            CR4Data;

  AgesaStatus = AGESA_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbMidInterfaceTN Enter\n");

  GnbHandle = GnbGetHandle (StdHeader);
  ASSERT (GnbHandle != NULL);

  Property = TABLE_PROPERTY_DEFAULT;
  Property |= GfxLibIsControllerPresent (StdHeader) ? 0 : TABLE_PROPERTY_IGFX_DISABLED;
  Property |= GnbBuildOptionsTN.GnbCommonOptions.LclkDeepSleepEn ? TABLE_PROPERTY_LCLK_DEEP_SLEEP : 0;
  Property |= GnbBuildOptionsTN.GnbCommonOptions.CfgOrbClockGatingEnable ? TABLE_PROPERTY_ORB_CLK_GATING : 0;
  Property |= GnbBuildOptionsTN.GnbCommonOptions.CfgIocLclkClockGatingEnable ? TABLE_PROPERTY_IOC_LCLK_CLOCK_GATING : 0;
  Property |= GnbFmCheckIommuPresent (GnbHandle, StdHeader) ? 0: TABLE_PROPERTY_IOMMU_DISABLED;
  Property |= GnbBuildOptionsTN.SmuSclkClockGatingEnable ? TABLE_PROPERTY_SMU_SCLK_CLOCK_GATING : 0;

  IDS_OPTION_HOOK (IDS_GNB_PROPERTY, &Property, StdHeader);

  if ((Property & TABLE_PROPERTY_IOMMU_DISABLED) == 0) {
    Status = GnbEnableIommuMmioV4 (GnbHandle, StdHeader);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    Status = GnbIommuMidInit (StdHeader);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
  }
  //
  // Set sclk to 100Mhz
  //
  SclkDid = GfxRequestSclkTNS3Save (
              GfxLibCalculateDidTN (98 * 100, StdHeader),
              StdHeader
              );

  Status = GnbProcessTable (
             GnbHandle,
             GnbMidInitTableTN,
             Property,
             GNB_TABLE_FLAGS_FORCE_S3_SAVE,
             StdHeader
             );
  AGESA_STATUS_UPDATE (Status, AgesaStatus);
  //
  // Restore Sclk
  //
  GfxRequestSclkTNS3Save (
    SclkDid,
    StdHeader
    );

  GnbCgttOverrideTN (Property, StdHeader);

  LibAmdReadCpuReg (4, &CR4Data);
  if ((CR4Data & 0x200) != 0) {
    LibAmdReadMxcsr (&MxcsrValue);
    IDS_HDT_CONSOLE (GNB_TRACE, "Read MxcsrValue = %x \n", MxcsrValue);
  }

  Status = GnbLclkDpmInitTN (StdHeader);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  if ((CR4Data & 0x200) != 0) {
    LibAmdWriteMxcsr (&MxcsrValue);
    IDS_HDT_CONSOLE (GNB_TRACE, "Write MxcsrValue = %x \n", MxcsrValue);
  } else {
    LibAmdFnclex ();
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbMidInterfaceTN Exit [0x%x]\n", AgesaStatus);
  return  AgesaStatus;
}
