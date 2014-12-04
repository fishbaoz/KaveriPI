/* $NoKeywords:$ */
/**
 * @file
 *
 * Family-specific GNB initialization at AMD_INIT_EARLY
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
#include  "OptionGnb.h"
#include  "GnbCommonLib.h"
#include  "GnbPcieConfig.h"
#include  "GnbTable.h"
#include  "GnbNbInitLibV4.h"
#include  "GnbRegisterAccTN.h"
#include  "GnbRegistersTN.h"
#include  "GfxLibTN.h"
#include  "GnbCacWeightsTN.h"
#include  "cpuFamilyTranslation.h"
#include  "GnbHandleLib.h"
#include  "GnbBapmCoeffCalcTN.h"
#include  "GnbInitTN.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITTN_GNBEARLYINITTN_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

extern GNB_TABLE ROMDATA      GnbEarlyInitTableTN [];
extern GNB_TABLE ROMDATA      GnbEarlierInitTableBeforeSmuTN [];
extern GNB_TABLE ROMDATA      GnbEarlierInitTableAfterSmuTN [];
extern GNB_BUILD_OPTIONS_TN   GnbBuildOptionsTN;
extern BUILD_OPT_CFG          UserOptions;
extern GNB_FW_PTRS            GnbFwPtrs;

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
 * Gnb TN Decrease all of the SMU VIDs by 4 (+25mV)

 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 */
VOID
GnbAdjustSmuVidBeforeSmuTN (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  D0F0xBC_xE0001008_STRUCT  D0F0xBC_xE0001008;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbAdjustSmuVidBeforeSmuTN Enter\n");

  GnbRegisterReadTN (D0F0xBC_xE0001008_TYPE, D0F0xBC_xE0001008_ADDRESS, &D0F0xBC_xE0001008, 0, StdHeader);
  D0F0xBC_xE0001008.Field.SClkVid3 -= 4;
  D0F0xBC_xE0001008.Field.SClkVid2 -= 4;
  D0F0xBC_xE0001008.Field.SClkVid1 -= 4;
  D0F0xBC_xE0001008.Field.SClkVid0 -= 4;
  GnbRegisterWriteTN (D0F0xBC_xE0001008_TYPE, D0F0xBC_xE0001008_ADDRESS, &D0F0xBC_xE0001008, 0, StdHeader);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbAdjustSmuVidBeforeSmuTN Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Gnb TN Decrease all of the SMU VIDs by 4 (+25mV)

 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 */
VOID
GnbAdjustSmuVidAfterSmuTN (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  D0F0xBC_x1F88C_STRUCT                   D0F0xBC_x1F88C;
  D0F0xBC_x1F8DC_STRUCT                   D0F0xBC_x1F8DC;
  D0F0xBC_x1F8E0_STRUCT                   D0F0xBC_x1F8E0;
  D0F0xBC_x1F8E4_STRUCT                   D0F0xBC_x1F8E4;
  D0F0xBC_x1F8E8_STRUCT                   D0F0xBC_x1F8E8;
  D0F0xBC_x1F400_STRUCT                   D0F0xBC_x1F400;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbAdjustSmuVidAfterSmuTN Enter\n");

  //Adjust SMU VIDs
  GnbRegisterReadTN (D0F0xBC_x1F88C_TYPE, D0F0xBC_x1F88C_ADDRESS, &D0F0xBC_x1F88C, 0, StdHeader);
  GnbRegisterReadTN (D0F0xBC_x1F8DC_TYPE, D0F0xBC_x1F8DC_ADDRESS, &D0F0xBC_x1F8DC, 0, StdHeader);
  GnbRegisterReadTN (D0F0xBC_x1F8E0_TYPE, D0F0xBC_x1F8E0_ADDRESS, &D0F0xBC_x1F8E0, 0, StdHeader);
  GnbRegisterReadTN (D0F0xBC_x1F8E4_TYPE, D0F0xBC_x1F8E4_ADDRESS, &D0F0xBC_x1F8E4, 0, StdHeader);
  GnbRegisterReadTN (D0F0xBC_x1F8E8_TYPE, D0F0xBC_x1F8E8_ADDRESS, &D0F0xBC_x1F8E8, 0, StdHeader);

  D0F0xBC_x1F88C.Field.NbVid_3 -= 4;
  D0F0xBC_x1F88C.Field.NbVid_2 -= 4;
  D0F0xBC_x1F88C.Field.NbVid_1 -= 4;
  D0F0xBC_x1F88C.Field.NbVid_0 -= 4;

  D0F0xBC_x1F8DC.Field.SClkVid3 -= 4;
  D0F0xBC_x1F8DC.Field.SClkVid2 -= 4;
  D0F0xBC_x1F8DC.Field.SClkVid1 -= 4;
  D0F0xBC_x1F8DC.Field.SClkVid0 -= 4;
  D0F0xBC_x1F8E0.Field.BapmSclkVid_2 -= 4;
  D0F0xBC_x1F8E0.Field.BapmSclkVid_1 -= 4;
  D0F0xBC_x1F8E0.Field.BapmSclkVid_0 -= 4;
  D0F0xBC_x1F8E4.Field.BapmNbVid_1 -= 4;
  D0F0xBC_x1F8E4.Field.BapmNbVid_0 -= 4;
  D0F0xBC_x1F8E4.Field.BapmSclkVid_3 -= 4;
  D0F0xBC_x1F8E8.Field.BapmNbVid_3 -= 4;
  D0F0xBC_x1F8E8.Field.BapmNbVid_2 -= 4;

  GnbRegisterWriteTN (D0F0xBC_x1F88C_TYPE, D0F0xBC_x1F88C_ADDRESS, &D0F0xBC_x1F88C, 0, StdHeader);
  GnbRegisterWriteTN (D0F0xBC_x1F8DC_TYPE, D0F0xBC_x1F8DC_ADDRESS, &D0F0xBC_x1F8DC, 0, StdHeader);
  GnbRegisterWriteTN (D0F0xBC_x1F8E0_TYPE, D0F0xBC_x1F8E0_ADDRESS, &D0F0xBC_x1F8E0, 0, StdHeader);
  GnbRegisterWriteTN (D0F0xBC_x1F8E4_TYPE, D0F0xBC_x1F8E4_ADDRESS, &D0F0xBC_x1F8E4, 0, StdHeader);
  GnbRegisterWriteTN (D0F0xBC_x1F8E8_TYPE, D0F0xBC_x1F8E8_ADDRESS, &D0F0xBC_x1F8E8, 0, StdHeader);

  //D0F0xBC_x1F400[SviLoadLineOffsetVddNB]=01b (-25mV)
  GnbRegisterReadTN (D0F0xBC_x1F400_TYPE, D0F0xBC_x1F400_ADDRESS, &D0F0xBC_x1F400, 0, StdHeader);
  D0F0xBC_x1F400.Field.SviLoadLineOffsetVddNB = 1;
  GnbRegisterWriteTN (D0F0xBC_x1F400_TYPE, D0F0xBC_x1F400_ADDRESS, &D0F0xBC_x1F400, 0, StdHeader);


  IDS_HDT_CONSOLE (GNB_TRACE, "GnbAdjustSmuVidAfterSmuTN Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Gnb SMU LHTC support
 *
 * Part of BAPM enablement.
 * When BAPM is disabled in battery mode firmware will enable LHTC.
 *
 * @param[in]  StdHeader  Standard configuration header
 */
VOID
GnbBapmLhtcInitTN (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  D0F0xBC_x1F638_STRUCT            D0F0xBC_x1F638;
  D0F0xBC_x1F428_STRUCT            D0F0xBC_x1F428;
  D0F0xBC_x1F86C_STRUCT            D0F0xBC_x1F86C;
  D0F0xBC_x1F628_STRUCT            D0F0xBC_x1F628;
  D0F0xBC_xE0104188_STRUCT         D0F0xBC_xE0104188;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbBapmLhtcInitTN Enter\n");

  GnbRegisterReadTN (D0F0xBC_x1F638_TYPE, D0F0xBC_x1F638_ADDRESS, &D0F0xBC_x1F638, 0, StdHeader);
  GnbRegisterReadTN (D0F0xBC_x1F428_TYPE, D0F0xBC_x1F428_ADDRESS, &D0F0xBC_x1F428, 0, StdHeader);
  GnbRegisterReadTN (D0F0xBC_xE0104188_TYPE, D0F0xBC_xE0104188_ADDRESS, &D0F0xBC_xE0104188, 0, StdHeader);

  //1. Set HTC period to 10 in PM_TIMERS_2 register
  //Still need to keep PM_CONFIG.Enable_HTC_Limit to 0
  D0F0xBC_x1F428.Field.EnableHtcLimit = 0;
  GnbRegisterWriteTN (D0F0xBC_x1F428_TYPE, D0F0xBC_x1F428_ADDRESS, &D0F0xBC_x1F428, 0, StdHeader);
  D0F0xBC_x1F638.Field.HtcPeriod = 10;
  GnbRegisterWriteTN (D0F0xBC_x1F638_TYPE, D0F0xBC_x1F638_ADDRESS, &D0F0xBC_x1F638, 0, StdHeader);

  //2. Read BapmLhtcCap fuse
  GnbRegisterReadTN (D0F0xBC_x1F86C_TYPE, D0F0xBC_x1F86C_ADDRESS, &D0F0xBC_x1F86C, 0, StdHeader);
  GnbRegisterReadTN (D0F0xBC_x1F628_TYPE, D0F0xBC_x1F628_ADDRESS, &D0F0xBC_x1F628, 0, StdHeader);
  if (D0F0xBC_x1F86C.Field.BapmLhtcCap == 0) {
    D0F0xBC_x1F628.Field.HtcActivePstateLimit = 0;
  } else {
    D0F0xBC_x1F628.Field.HtcActivePstateLimit = D0F0xBC_xE0104188.Field.LhtcPstateLimit;
  }
  GnbRegisterWriteTN (D0F0xBC_x1F628_TYPE, D0F0xBC_x1F628_ADDRESS, &D0F0xBC_x1F628, 0, StdHeader);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbBapmLhtcInitTN Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Measured temperature with BAPM
 *
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 */
VOID
GnbBapmMeasuredTempTN (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  D0F0xBC_x1F428_STRUCT            D0F0xBC_x1F428;
  D0F0xBC_xE0104188_STRUCT         D0F0xBC_xE0104188;
  D0F0xBC_x1F844_STRUCT            D0F0xBC_x1F844;
  D0F0xBC_x1F848_STRUCT            D0F0xBC_x1F848;
  D0F0xBC_x1F84C_STRUCT            D0F0xBC_x1F84C;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbBapmMeasuredTempTN Enter\n");

  GnbRegisterReadTN (D0F0xBC_xE0104188_TYPE, D0F0xBC_xE0104188_ADDRESS, &D0F0xBC_xE0104188, 0, StdHeader);

  //Measured temperature with BAPM
  GnbRegisterReadTN (D0F0xBC_x1F428_TYPE, D0F0xBC_x1F428_ADDRESS, &D0F0xBC_x1F428, 0, StdHeader);
  D0F0xBC_x1F428.Field.OverrideCalcTemp = 0;
  if (D0F0xBC_xE0104188.Field.BapmMeasuredTemp == 1) {
    D0F0xBC_x1F844.Value = 0x38B;
    GnbRegisterWriteTN (D0F0xBC_x1F844_TYPE, D0F0xBC_x1F844_ADDRESS, &D0F0xBC_x1F844, 0, StdHeader);
    D0F0xBC_x1F848.Value = 0x38D;
    GnbRegisterWriteTN (D0F0xBC_x1F848_TYPE, D0F0xBC_x1F848_ADDRESS, &D0F0xBC_x1F848, 0, StdHeader);
    D0F0xBC_x1F84C.Value = 0x389;
    GnbRegisterWriteTN (D0F0xBC_x1F84C_TYPE, D0F0xBC_x1F84C_ADDRESS, &D0F0xBC_x1F84C, 0, StdHeader);

    D0F0xBC_x1F428.Field.OverrideCalcTemp = 1;
  }
  GnbRegisterWriteTN (D0F0xBC_x1F428_TYPE, D0F0xBC_x1F428_ADDRESS, &D0F0xBC_x1F428, 0, StdHeader);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbBapmMeasuredTempTN Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Gnb SMU LHTC Enable
 *
 * Part of BAPM enablement.
 * When BAPM is disabled in battery mode firmware will enable LHTC.
 *
 * @param[in]  StdHeader  Standard configuration header
 */
VOID
GnbLhtcEnableTN (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  D0F0xBC_x1F428_STRUCT            D0F0xBC_x1F428;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbLhtcEnableTN Enter\n");

  GnbRegisterReadTN (D0F0xBC_x1F428_TYPE, D0F0xBC_x1F428_ADDRESS, &D0F0xBC_x1F428, 0, StdHeader);
  D0F0xBC_x1F428.Field.EnableHtcLimit = 1;
  GnbRegisterWriteTN (D0F0xBC_x1F428_TYPE, D0F0xBC_x1F428_ADDRESS, &D0F0xBC_x1F428, 0, StdHeader);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbLhtcEnableTN Exit\n");
}
/*----------------------------------------------------------------------------------------*/
/**
 * Gnb TN Update BAPMTI_TjOffset
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 */
VOID
GnbTjOffsetUpdateTN (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  D0F0xBC_x1F870_STRUCT             D0F0xBC_x1F870;

  CPU_LOGICAL_ID                    LogicalId;
  GNB_HANDLE                        *GnbHandle;
  D0F0xBC_xE0104040_STRUCT          D0F0xBC_xE0104040;
  D0F0xBC_x1F85C_STRUCT             D0F0xBC_x1F85C;
  D0F0xBC_xE010413C_STRUCT          D0F0xBC_xE010413C;
  UINT32                            TimerPeriod;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbTjOffsetUpdateTN Enter\n");


  TimerPeriod = D0F0xBC_x1F468_TimerPeriod_Value;
  GnbRegisterReadTN (D0F0xBC_x1F85C_TYPE, D0F0xBC_x1F85C_ADDRESS, &D0F0xBC_x1F85C, 0, StdHeader);
  GnbRegisterReadTN (D0F0xBC_xE010413C_TYPE, D0F0xBC_xE010413C_ADDRESS, &D0F0xBC_xE010413C, 0, StdHeader);
  // Determine desired AgingRate:
  // PM_FUSES4.TdpAgeRate * Fuse[BAPMTI_Ts] (encoded in us)
  // Re-encode TdpAgeRate with 1ms BAPM interval
  D0F0xBC_x1F85C.Field.TdpAgeRate = (D0F0xBC_x1F85C.Field.TdpAgeRate * D0F0xBC_xE010413C.Field.BAPMTI_Ts) / (TimerPeriod / 100);
  GnbRegisterWriteTN (D0F0xBC_x1F85C_TYPE, D0F0xBC_x1F85C_ADDRESS, &D0F0xBC_x1F85C, 0, StdHeader);

  GnbHandle = GnbGetHandle (StdHeader);
  ASSERT (GnbHandle != NULL);
  GetLogicalIdOfSocket (GnbGetSocketId (GnbHandle), &LogicalId, StdHeader);
  if ((LogicalId.Revision & AMD_F15_TN_A0) != AMD_F15_TN_A0) {
    IDS_HDT_CONSOLE (GNB_TRACE, "CPU Rev = %x, Skip GnbTjOffsetUpdateTN\n", LogicalId.Revision);
    return;
  }
  //update BAPMTI_TjOffset for revA0 parts
  GnbRegisterReadTN (D0F0xBC_xE0104040_TYPE, D0F0xBC_xE0104040_ADDRESS, &D0F0xBC_xE0104040, 0, StdHeader);
  GnbRegisterReadTN (D0F0xBC_x1F870_TYPE, D0F0xBC_x1F870_ADDRESS, &D0F0xBC_x1F870, 0, StdHeader);
  //9900h=FS1r2/FP2 Devastator
  //9903h=FS1r2/FP2 Devastator Lite
  //9990h=FS1r2/FP2 Scrapper
  //9901h=FM2 Devastator
  //9904h=FM2 Devastator Lite
  //9991h=FM2 Scrapper
  if ((D0F0xBC_xE0104040.Field.DeviceID == 0x9900) || (D0F0xBC_xE0104040.Field.DeviceID == 0x9903)) {
    D0F0xBC_x1F870.Field.BAPMTI_TjOffset_0 = 0x26;
    D0F0xBC_x1F870.Field.BAPMTI_TjOffset_1 = 0x26;
    D0F0xBC_x1F870.Field.BAPMTI_TjOffset_2 = 0x26;
  } else if (D0F0xBC_xE0104040.Field.DeviceID == 0x9990) {
    D0F0xBC_x1F870.Field.BAPMTI_TjOffset_0 = 0x2E;
    D0F0xBC_x1F870.Field.BAPMTI_TjOffset_1 = 0x2E;
    D0F0xBC_x1F870.Field.BAPMTI_TjOffset_2 = 0x2E;
  } else {
    IDS_HDT_CONSOLE (GNB_TRACE, "GnbTjOffsetUpdateTN Skip DID- %x\n", D0F0xBC_xE0104040.Field.DeviceID);
  }
  GnbRegisterWriteTN (D0F0xBC_x1F870_TYPE, D0F0xBC_x1F870_ADDRESS, &D0F0xBC_x1F870, 0, StdHeader);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbTjOffsetUpdateTN Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * GPU CAC enablement and weights programming
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 */

VOID
GnbCacEnablement (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  D0F0xBC_x1F464_STRUCT   D0F0xBC_x1F464;
  D0F0xBC_x1F160_STRUCT   D0F0xBC_x1F160;
  D0F0xBC_x1F920_STRUCT   D0F0xBC_x1F920;
  PCI_ADDR                PciAddress;
  UINT8                   Index;
  D0F0xBC_x1F91C_STRUCT   D0F0xBC_x1F91C;
  D18F5x160_STRUCT        D18F5x160;
  DOUBLE                  UnbCac;
  GMMx898_STRUCT          GMMx898;

  GnbRegisterReadTN (D0F0xBC_x1F920_TYPE, D0F0xBC_x1F920_ADDRESS, &D0F0xBC_x1F920, 0, StdHeader);
  D0F0xBC_x1F920.Field.Alpha = 0x29;
  GnbRegisterWriteTN (D0F0xBC_x1F920_TYPE, D0F0xBC_x1F920_ADDRESS, &D0F0xBC_x1F920, 0, StdHeader);

  //UNB_CAC_VALUE.UNB_CAC = 2.3734E-04 * FNBPS0 (in MHz) * 2^GPU_CAC_AVRG_CNTL.WEIGHT_PREC
  GnbRegisterReadTN (D18F5x160_TYPE, D18F5x160_ADDRESS, &D18F5x160.Value, 0, StdHeader);
  IDS_HDT_CONSOLE (GNB_TRACE, "NBP0 10khz %x (%d)\n", GfxLibGetNclkTN ((UINT8) D18F5x160.Field.NbFid, (UINT8) D18F5x160.Field.NbDid), GfxLibGetNclkTN ((UINT8) D18F5x160.Field.NbFid, (UINT8) D18F5x160.Field.NbDid));
  UnbCac = 0.00000204831536 * (1 << D0F0xBC_x1F920.Field.WeightPrec) * GfxLibGetNclkTN ((UINT8) D18F5x160.Field.NbFid, (UINT8) D18F5x160.Field.NbDid);
  D0F0xBC_x1F91C.Field.UnbCac = (UINT32) GnbFpLibDoubleToInt32 (UnbCac);
  IDS_HDT_CONSOLE (GNB_TRACE, "UnbCac %x (%d)\n", D0F0xBC_x1F91C.Field.UnbCac, D0F0xBC_x1F91C.Field.UnbCac);
  GnbRegisterWriteTN (D0F0xBC_x1F91C_TYPE, D0F0xBC_x1F91C_ADDRESS, &D0F0xBC_x1F91C.Value, 0, StdHeader);

  GnbRegisterReadTN (D0F0xBC_x1F160_TYPE, D0F0xBC_x1F160_ADDRESS, &D0F0xBC_x1F160, 0, StdHeader);
  D0F0xBC_x1F160.Field.TdpClampEn = 0x1;
  D0F0xBC_x1F160.Field.TdpClampHys = 0x4;
  D0F0xBC_x1F160.Field.TdpWaterfallCtl = 0x25;
  GnbRegisterWriteTN (D0F0xBC_x1F160_TYPE, D0F0xBC_x1F160_ADDRESS, &D0F0xBC_x1F160, 0, StdHeader);

  GnbRegisterReadTN (GMMx898_TYPE, GMMx898_ADDRESS, &GMMx898, 0, StdHeader);
  GMMx898.Field.Threshold = 0x31;
  GnbRegisterWriteTN (GMMx898_TYPE, GMMx898_ADDRESS, &GMMx898, 0, StdHeader);

  // Set CAC/TDP interval
  GnbRegisterReadTN (D0F0xBC_x1F464_TYPE, D0F0xBC_x1F464_ADDRESS, &D0F0xBC_x1F464, 0, StdHeader);
  D0F0xBC_x1F464.Field.TdpCntl = 1;
  GnbRegisterWriteTN (D0F0xBC_x1F464_TYPE, D0F0xBC_x1F464_ADDRESS, &D0F0xBC_x1F464, 0, StdHeader);

  // Program GPU CAC weights

  for (Index = 0; Index < (sizeof (CacWeightsTN) / sizeof (CacWeightsTN[0])); Index++) {
    GnbRegisterWriteTN (D0F0xBC_x1F9A0_TYPE, (D0F0xBC_x1F9A0_ADDRESS + (Index * 4)), &CacWeightsTN[Index], 0, StdHeader);
  }

  // Call BIOS service SMC_MSG_CONFIG_TDP_CNTL
  PciAddress.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0);
  GnbSmuServiceRequestV4 (
    PciAddress,
    SMC_MSG_CONFIG_TDP_CNTL,
    0,
    StdHeader
    );
}

/*----------------------------------------------------------------------------------------*/
/**
 * Decode power of CPU out of Watt
 *
 *
 *
 * @param[in] Encode      PwrCpu encode
 * @param[in] StdHeader   Standard Configuration Header
 * @retval                mWatt
 */
INT32
CpuPowerDecode (
  IN      UINT8                           Encode,
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  INT32                   Power;
  D0F0xBC_x1F850_STRUCT   D0F0xBC_x1F850;

  GnbRegisterReadTN (D0F0xBC_x1F850_TYPE, D0F0xBC_x1F850_ADDRESS, &D0F0xBC_x1F850, 0, StdHeader);

  //TdpWatt = TdpWattEncode / 1024
  //PwrCpu / TdpWatt = Encode
  //PwrCpu = Encode * TdpWattEncode / 1024

  Power = (INT32) ((Encode * D0F0xBC_x1F850.Field.Tdp2Watt *1000) / 1024);

  return Power;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Encode the offset of power of CPU
 *
 *
 *
 * @param[in] NewPower    New power of mWatt
 * @param[in] OrgPower    Original power of mWatt
 * @param[in] StdHeader   Standard Configuration Header
 * @retval                Encode
 */
UINT8
CpuPowerOffsetEncode (
  IN      INT32                           NewPower,
  IN      INT32                           OrgPower,
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  INT8                    PowerOffsetEncode;
  INT32                   PowerOffset;
  D0F0xBC_x1F850_STRUCT   D0F0xBC_x1F850;
  BOOLEAN                 Postive;

  GnbRegisterReadTN (D0F0xBC_x1F850_TYPE, D0F0xBC_x1F850_ADDRESS, &D0F0xBC_x1F850, 0, StdHeader);
  if (NewPower > OrgPower) {
    PowerOffset = NewPower - OrgPower;
    Postive = TRUE;
  } else {
    PowerOffset = OrgPower - NewPower;
    Postive = FALSE;
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "Cpu New Pwr %x (%d)\n", NewPower, NewPower);
  IDS_HDT_CONSOLE (GNB_TRACE, "Cpu org Pwr %x (%d)\n", OrgPower, OrgPower);
  IDS_HDT_CONSOLE (GNB_TRACE, "Tdp2Watt %x, (%d)\n", D0F0xBC_x1F850.Field.Tdp2Watt, D0F0xBC_x1F850.Field.Tdp2Watt);
  //Ceil of (mWatt *1024 / TdpWattEncode) / 1000 = Encode in watt
  PowerOffset = (((PowerOffset * 1024) / D0F0xBC_x1F850.Field.Tdp2Watt) + 500) / 1000;

  if (Postive) {
    PowerOffsetEncode = (INT8) PowerOffset;
  } else {
    PowerOffsetEncode = 0 - (INT8) PowerOffset;
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PowerOffsetEncode %x\n", PowerOffsetEncode);
  return (UINT8) PowerOffsetEncode;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Decode power of GPU out of Watt
 *
 *
 *
 * @param[in] Encode      PwrGpu encode
 * @param[in] StdHeader   Standard Configuration Header
 * @retval                mWatt
 */
INT16
GpuPowerDecode (
  IN      UINT16                          Encode,
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  INT16                   Power;

  Power = (INT16) Encode;

  //MaxPwrGpu[15:0], MidPwrGpu[15:0], and NomPwrGpu[15:0] are represented in mW units
  //So MaxPwrGpu of 21.7 is represented as 21700 (mW) -> 0101010011000100

  return Power;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Decode Max Tj
 *
 *
 *
 * @param[in] Encode      Tj encode
 * @retval                100 x Tj
 */
INT16
TjMaxDecode (
  IN      UINT8               Encode
  )
{
  INT16    TjMax;

  TjMax = (INT16) Encode;

  return (TjMax * 100);
}

/*----------------------------------------------------------------------------------------*/
/**
 * for BAPMTI_TjOffset decoding
 *
 *
 *
 * @param[in] Encode      Tj encode
 * @retval                100 x Tjoffset
 */
INT16
TjOffsetDecode (
  IN      UINT8               Encode
  )
{
  UINT16      Number;
  UINT8       Floating;
  BOOLEAN     Postive;
  UINT8       TjOffsetEncode;

  TjOffsetEncode = Encode;
  Postive = TRUE;

  if (Encode == 0) {
    return 0;
  }

  if ((TjOffsetEncode & 0x80) != 0) {
    Postive = FALSE;
    TjOffsetEncode = (UINT8) (~(Encode - 1));
  }

  Number = ((TjOffsetEncode >> 2) & 0x1F) * 100;

  Floating = (TjOffsetEncode & 0x3);
  if (Floating == 1) {
    Number += 25;
  } else if (Floating == 2) {
    Number += 50;
  } else if (Floating == 3) {
    Number += 75;
  } else {
  }

  if (Postive) {
    return  (INT16) Number;
  } else {
    return (INT16) (0 - Number);
  }

}

/*----------------------------------------------------------------------------------------*/
/**
 * Trinity SMU supports a software-writeable TjOffset (called swTjOffset) that can be programmed to
 * account for underspec thermal solutions.
 * There is a mechanism for customers to adjust TjOffset (via BAPM_PARAMETERS3.TjOffset)
 * for under-performing thermal solutions.
 * BIOS will adjust NomPow/MidPow/MaxPow based on this software-programmable TjOffset (called swTjOffset).
 * SMU firmware will add this value to Fuse[TjOffset] for all TE's during BAPM calculations.
 *
 * Tj stands for junction temperature of the processor. However, here is a general description of
 * our software-programmable TjOffset for BAPM (Birdirectional Application Power Management):
 * "swTjOffset is an adjustable offset for BAPM thermal calculations to account for changes in
 * junction temperature, TjOffset. For further details, see Thermal Guide."
 *
 * @param[in]  StdHeader  Standard configuration header
 */
VOID
GnbSoftwareTjOffsetTN (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  D0F0xBC_x1F860_STRUCT   D0F0xBC_x1F860;
  D0F0xBC_x1F864_STRUCT   D0F0xBC_x1F864;
  D0F0xBC_x1F868_STRUCT   D0F0xBC_x1F868;
  D0F0xBC_x1F870_STRUCT   D0F0xBC_x1F870;
  D0F0xBC_x1F898_STRUCT   D0F0xBC_x1F898;
  D0F0xBC_x1F8C0_STRUCT   D0F0xBC_x1F8C0;
  D0F0xBC_x1F850_STRUCT   D0F0xBC_x1F850;

  D0F0xBC_x1F6AC_STRUCT   D0F0xBC_x1F6AC;
  D0F0xBC_x1F6B0_STRUCT   D0F0xBC_x1F6B0;
  D0F0xBC_x1F6B4_STRUCT   D0F0xBC_x1F6B4;
  D0F0xBC_x1F6B8_STRUCT   D0F0xBC_x1F6B8;
  INT8                    SwTjOffset;
  INT16                   Delta_T_org;
  INT16                   Delta_T_new;
  INT32                   Cpu_New_Pwr;
  INT32                   Gpu_New_Pwr;

  SwTjOffset = (INT8) UserOptions.CfgGnbSwTjOffset;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSoftwareTjOffsetTN Enter\n");

  IDS_OPTION_HOOK (IDS_GNB_PMM_SWTJOFFSET, &SwTjOffset, StdHeader);
  if (SwTjOffset == 0) {
    return;
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "User Input Tj Offset %x\n", SwTjOffset);

  GnbRegisterReadTN (D0F0xBC_x1F850_TYPE, D0F0xBC_x1F850_ADDRESS, &D0F0xBC_x1F850, 0, StdHeader);
  ASSERT (D0F0xBC_x1F850.Field.Tdp2Watt != 0);
  if (D0F0xBC_x1F850.Field.Tdp2Watt == 0) {
    IDS_HDT_CONSOLE (GNB_TRACE, "Tdp2Watt is 0 - exit here\n", SwTjOffset);
    return;
  }
  GnbRegisterReadTN (D0F0xBC_x1F860_TYPE, D0F0xBC_x1F860_ADDRESS, &D0F0xBC_x1F860, 0, StdHeader);
  GnbRegisterReadTN (D0F0xBC_x1F864_TYPE, D0F0xBC_x1F864_ADDRESS, &D0F0xBC_x1F864, 0, StdHeader);
  GnbRegisterReadTN (D0F0xBC_x1F868_TYPE, D0F0xBC_x1F868_ADDRESS, &D0F0xBC_x1F868, 0, StdHeader);
  GnbRegisterReadTN (D0F0xBC_x1F870_TYPE, D0F0xBC_x1F870_ADDRESS, &D0F0xBC_x1F870, 0, StdHeader);
  GnbRegisterReadTN (D0F0xBC_x1F898_TYPE, D0F0xBC_x1F898_ADDRESS, &D0F0xBC_x1F898, 0, StdHeader);
  GnbRegisterReadTN (D0F0xBC_x1F8C0_TYPE, D0F0xBC_x1F8C0_ADDRESS, &D0F0xBC_x1F8C0, 0, StdHeader);

  //Tjoffset_new = Tjoffset_org + SwTjOffset
  //Delta_T_org = T_die - Tjoffset_org - 45

  //Delta_T_new = T_die - Tjoffset_new - 45
  //            = T_die - (Tjoffset_org + SwTjOffset) - 45
  //            = T_die - Tjoffset_org - SwTjOffset - 45

  //Pwr_new = Pwr_org * (Delta_T_new/Delta_T_org)
  //        = Pwr_org * (T_org - TjOffset) / T_org

  //Cpu0
  Delta_T_org = TjMaxDecode ((UINT8) D0F0xBC_x1F860.Field.BAPMTI_TjMax_0) - TjOffsetDecode ((UINT8) D0F0xBC_x1F870.Field.BAPMTI_TjOffset_0) - 4500;
  Delta_T_new = TjMaxDecode ((UINT8) D0F0xBC_x1F860.Field.BAPMTI_TjMax_0) - TjOffsetDecode ((UINT8) D0F0xBC_x1F870.Field.BAPMTI_TjOffset_0) - (SwTjOffset * 100) - 4500;
  IDS_HDT_CONSOLE (GNB_TRACE, "Cpu0 Delta T org %x (%d)\n", Delta_T_org, Delta_T_org);
  IDS_HDT_CONSOLE (GNB_TRACE, "Cpu0 Delta T New %x (%d)\n", Delta_T_new, Delta_T_new);
  Cpu_New_Pwr = (CpuPowerDecode ((UINT8) D0F0xBC_x1F868.Field.MaxPwrCpu_0, StdHeader) * Delta_T_new) / Delta_T_org;
  D0F0xBC_x1F6AC.Field.MaxPwrCpu_0 = CpuPowerOffsetEncode (Cpu_New_Pwr, CpuPowerDecode ((UINT8) D0F0xBC_x1F868.Field.MaxPwrCpu_0, StdHeader), StdHeader);
  Cpu_New_Pwr = (CpuPowerDecode ((UINT8) D0F0xBC_x1F868.Field.NomPwrCpu_0, StdHeader) * Delta_T_new) / Delta_T_org;
  D0F0xBC_x1F6AC.Field.NomPwrCpu_0 = CpuPowerOffsetEncode (Cpu_New_Pwr, CpuPowerDecode ((UINT8) D0F0xBC_x1F868.Field.NomPwrCpu_0, StdHeader), StdHeader);
  Cpu_New_Pwr = (CpuPowerDecode ((UINT8) D0F0xBC_x1F8C0.Field.MidPwrCpu_0, StdHeader) * Delta_T_new) / Delta_T_org;
  D0F0xBC_x1F6B8.Field.MidPwrCpu_0 = CpuPowerOffsetEncode (Cpu_New_Pwr, CpuPowerDecode ((UINT8) D0F0xBC_x1F8C0.Field.MidPwrCpu_0, StdHeader), StdHeader);

  //Cpu1
  Delta_T_org = TjMaxDecode ((UINT8) D0F0xBC_x1F860.Field.BAPMTI_TjMax_1) - TjOffsetDecode ((UINT8) D0F0xBC_x1F870.Field.BAPMTI_TjOffset_1) - 4500;
  Delta_T_new = TjMaxDecode ((UINT8) D0F0xBC_x1F860.Field.BAPMTI_TjMax_1) - TjOffsetDecode ((UINT8) D0F0xBC_x1F870.Field.BAPMTI_TjOffset_1) - (SwTjOffset * 100) - 4500;
  IDS_HDT_CONSOLE (GNB_TRACE, "Cpu1 Delta T org %x (%d)\n", Delta_T_org, Delta_T_org);
  IDS_HDT_CONSOLE (GNB_TRACE, "Cpu1 Delta T New %x (%d)\n", Delta_T_new, Delta_T_new);
  Cpu_New_Pwr = (CpuPowerDecode ((UINT8) D0F0xBC_x1F868.Field.MaxPwrCpu_1, StdHeader) * Delta_T_new) / Delta_T_org;
  D0F0xBC_x1F6AC.Field.MaxPwrCpu_1 = CpuPowerOffsetEncode (Cpu_New_Pwr, CpuPowerDecode ((UINT8) D0F0xBC_x1F868.Field.MaxPwrCpu_1, StdHeader), StdHeader);
  Cpu_New_Pwr = (CpuPowerDecode ((UINT8) D0F0xBC_x1F868.Field.NomPwrCpu_1, StdHeader) * Delta_T_new) / Delta_T_org;
  D0F0xBC_x1F6AC.Field.NomPwrCpu_1 = CpuPowerOffsetEncode (Cpu_New_Pwr, CpuPowerDecode ((UINT8) D0F0xBC_x1F868.Field.NomPwrCpu_1, StdHeader), StdHeader);
  Cpu_New_Pwr = (CpuPowerDecode ((UINT8) D0F0xBC_x1F8C0.Field.MidPwrCpu_1, StdHeader) * Delta_T_new) / Delta_T_org;
  D0F0xBC_x1F6B8.Field.MidPwrCpu_1 = CpuPowerOffsetEncode (Cpu_New_Pwr, CpuPowerDecode ((UINT8) D0F0xBC_x1F8C0.Field.MidPwrCpu_1, StdHeader), StdHeader);

  //GPU
  Delta_T_org = TjMaxDecode ((UINT8) D0F0xBC_x1F864.Field.BAPMTI_GpuTjMax) - TjOffsetDecode ((UINT8) D0F0xBC_x1F870.Field.BAPMTI_TjOffset_2) - 4500;
  Delta_T_new = TjMaxDecode ((UINT8) D0F0xBC_x1F864.Field.BAPMTI_GpuTjMax) - TjOffsetDecode ((UINT8) D0F0xBC_x1F870.Field.BAPMTI_TjOffset_2) - (SwTjOffset * 100) - 4500;
  IDS_HDT_CONSOLE (GNB_TRACE, "Gpu Delta T org %x (%d)\n", Delta_T_org, Delta_T_org);
  IDS_HDT_CONSOLE (GNB_TRACE, "Gpu Delta T New %x (%d)\n", Delta_T_new, Delta_T_new);
  Gpu_New_Pwr = (GpuPowerDecode ((UINT16) D0F0xBC_x1F898.Field.MaxPwrGpu, StdHeader) * Delta_T_new) / Delta_T_org;
  D0F0xBC_x1F6B0.Field.MaxPwrGpu = (UINT16) (Gpu_New_Pwr - GpuPowerDecode ((UINT16) D0F0xBC_x1F898.Field.MaxPwrGpu, StdHeader));
  Gpu_New_Pwr = (GpuPowerDecode ((UINT16) D0F0xBC_x1F898.Field.NomPwrGpu, StdHeader) * Delta_T_new) / Delta_T_org;
  D0F0xBC_x1F6B0.Field.NomPwrGpu = (UINT16) (Gpu_New_Pwr - GpuPowerDecode ((UINT16) D0F0xBC_x1F898.Field.NomPwrGpu, StdHeader));
  Gpu_New_Pwr = (GpuPowerDecode ((UINT16) D0F0xBC_x1F8C0.Field.MidPwrGpu, StdHeader) * Delta_T_new) / Delta_T_org;
  D0F0xBC_x1F6B8.Field.MidPwrGpu = (UINT16) (Gpu_New_Pwr - GpuPowerDecode ((UINT16) D0F0xBC_x1F8C0.Field.MidPwrGpu, StdHeader));

  //SwTjOffset
  D0F0xBC_x1F6B4.Field.TjOffset = SwTjOffset;

  GnbRegisterWriteTN (D0F0xBC_x1F6AC_TYPE, D0F0xBC_x1F6AC_ADDRESS, &D0F0xBC_x1F6AC, 0, StdHeader);
  GnbRegisterWriteTN (D0F0xBC_x1F6B0_TYPE, D0F0xBC_x1F6B0_ADDRESS, &D0F0xBC_x1F6B0, 0, StdHeader);
  GnbRegisterWriteTN (D0F0xBC_x1F6B4_TYPE, D0F0xBC_x1F6B4_ADDRESS, &D0F0xBC_x1F6B4, 0, StdHeader);
  GnbRegisterWriteTN (D0F0xBC_x1F6B8_TYPE, D0F0xBC_x1F6B8_ADDRESS, &D0F0xBC_x1F6B8, 0, StdHeader);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSoftwareTjOffsetTN Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Configurable TDP feature (here-on referred to as cTDP feature) provides flexibility to
 * AMD APUs, traditionally defined to be at fixed nominal TDPs, to fit well in platforms
 * that have thermal solutions designed for higher or lower than nominal TDP.
 *
 * @param[in]  StdHeader  Standard configuration header
 */
VOID
GnbConfigurableTdpTN (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  D0F0xBC_x1F758_STRUCT   D0F0xBC_x1F758;
  UINT16                  CfgTdpValue;
  GNB_HANDLE              *GnbHandle;
  CPU_LOGICAL_ID          LogicalId;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbConfigurableTdpTN Enter\n");

  GnbHandle = GnbGetHandle (StdHeader);
  ASSERT (GnbHandle != NULL);
  GetLogicalIdOfSocket (GnbGetSocketId (GnbHandle), &LogicalId, StdHeader);

  CfgTdpValue = (UINT16) (((AMD_EARLY_PARAMS *) StdHeader)->PlatformConfig.PowerCeiling * 256 / 1000);
  IDS_HDT_CONSOLE (GNB_TRACE, "User Input Target TDP %x\n", CfgTdpValue);

  // If TargetTDP is defined by build config or IDS and Richland APU, then set TargetTDP for SMU
  if ((CfgTdpValue != 0) && ((LogicalId.Revision & AMD_F15_RL_ALL) != 0)) {
    GnbRegisterReadTN (D0F0xBC_x1F758_TYPE, D0F0xBC_x1F758_ADDRESS, &D0F0xBC_x1F758, 0, StdHeader);
    D0F0xBC_x1F758.Field.TargetTdp = CfgTdpValue;
    GnbRegisterWriteTN (D0F0xBC_x1F758_TYPE, D0F0xBC_x1F758_ADDRESS, &D0F0xBC_x1F758, 0, StdHeader);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbConfigurableTdpTN Exit\n");
}
/*----------------------------------------------------------------------------------------*/
/**
 * Init TDC
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

VOID
GnbInitTdcTN (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AMD_EARLY_PARAMS        *EarlyParams;
  D0F0xBC_x1F62C_STRUCT   D0F0xBC_x1F62C;
  D0F0xBC_x1F840_STRUCT   D0F0xBC_x1F840;

  EarlyParams = (AMD_EARLY_PARAMS *) StdHeader;
  D0F0xBC_x1F62C.Field.Idd = EarlyParams->PlatformConfig.VrmProperties[CoreVrm].CurrentLimit / 10;
  D0F0xBC_x1F62C.Field.Iddnb = EarlyParams->PlatformConfig.VrmProperties[NbVrm].CurrentLimit / 10;
  GnbRegisterWriteTN (D0F0xBC_x1F62C_TYPE, D0F0xBC_x1F62C_ADDRESS, &D0F0xBC_x1F62C, 0, StdHeader);

  D0F0xBC_x1F840.Field.IddspikeOCP = EarlyParams->PlatformConfig.VrmProperties[CoreVrm].SviOcpLevel / 10;
  D0F0xBC_x1F840.Field.IddNbspikeOCP = EarlyParams->PlatformConfig.VrmProperties[NbVrm].SviOcpLevel / 10;
  GnbRegisterWriteTN (D0F0xBC_x1F840_TYPE, D0F0xBC_x1F840_ADDRESS, &D0F0xBC_x1F840, 0, StdHeader);
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
GnbEarlyInterfaceTN (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS                     Status;
  GNB_HANDLE                       *GnbHandle;
  UINT32                           Property;
  D0F0xBC_xE0104188_STRUCT         D0F0xBC_xE0104188;

  Status = AGESA_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbEarlyInterfaceTN Enter\n");
  Property = TABLE_PROPERTY_DEFAULT;

  //Check fuse to support BAPM disabled.
  GnbRegisterReadTN (D0F0xBC_xE0104188_TYPE, D0F0xBC_xE0104188_ADDRESS, &D0F0xBC_xE0104188, 0, StdHeader);
  if (D0F0xBC_xE0104188.Field.BapmDisable == 0) {
    Property |= GnbBuildOptionsTN.GnbCommonOptions.CfgBapmSupport ? TABLE_PROPERTY_BAPM : 0;

    //Check HybridBoost
    if ((Property & TABLE_PROPERTY_BAPM) == TABLE_PROPERTY_BAPM) {
      Property |= UserOptions.CfgHybridBoostEnable ? TABLE_PROPERTY_HYBRID_BOOST : 0;
      IDS_HDT_CONSOLE (GNB_TRACE, "HyBrid = %x\n", UserOptions.CfgHybridBoostEnable);
    }
  }

  IDS_OPTION_HOOK (IDS_GNB_PROPERTY, &Property, StdHeader);
  IDS_OPTION_CALLOUT (IDS_CALLOUT_GNB_BEFORE_EARLY_INIT, NULL, StdHeader);

  // SMU LHTC support init
  GnbBapmLhtcInitTN (StdHeader);

  if ((Property & TABLE_PROPERTY_BAPM) == TABLE_PROPERTY_BAPM) {
    GnbTjOffsetUpdateTN (StdHeader);
    GnbConfigurableTdpTN (StdHeader);
    GnbSoftwareTjOffsetTN (StdHeader);
    GnbBapmCalculateCoeffsTN (StdHeader);
    GnbCacEnablement (StdHeader);
    GnbBapmMeasuredTempTN (StdHeader);
  } else {
    //  If BAPM is disabled (either through fusing or CBS option), AGESA should enable LHTC algorithm.
    //  Right now, LHTC is only enabled in the "DisableBAPM()" firmware routine, so unless Driver specifically calls this message,
    //  LHTC will never be enabled if BAPM is disabled from the start.
    GnbLhtcEnableTN (StdHeader);
  }

  GnbInitTdcTN (StdHeader);
  GnbHandle = GnbGetHandle (StdHeader);
  ASSERT (GnbHandle != NULL);
  Status = GnbProcessTable (
             GnbHandle,
             GnbEarlyInitTableTN,
             Property,
             0,
             StdHeader
             );
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbEarlyInterfaceTN Exit [0x%x]\n", Status);
  return  Status;
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
GnbEarlierInterfaceTN (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS              Status;
  GNB_HANDLE                *GnbHandle;
  CPU_LOGICAL_ID            LogicalId;
  D0F0xBC_xE0107060_STRUCT  D0F0xBC_xE0107060;
  D0F0xBC_xE0001008_STRUCT  D0F0xBC_xE0001008;
  VOID                      *SmuFirmwareHeader;
  Status = AGESA_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbEarlierInterfaceTN Enter\n");

  GnbAdjustSmuVidBeforeSmuTN (StdHeader);

  GnbHandle = GnbGetHandle (StdHeader);
  ASSERT (GnbHandle != NULL);
  GnbRegisterReadTN (D0F0xBC_xE0107060_TYPE, D0F0xBC_xE0107060_ADDRESS, &D0F0xBC_xE0107060, 0, StdHeader);
  GnbRegisterReadTN (D0F0xBC_xE0001008_TYPE, D0F0xBC_xE0001008_ADDRESS, &D0F0xBC_xE0001008, 0, StdHeader);
  GfxRequestVoltageTN ((UINT8) D0F0xBC_xE0001008.Field.SClkVid1, StdHeader);
  GfxRequestSclkTN ((UINT8) D0F0xBC_xE0107060.Field.SClkDpmDid1, StdHeader);
  Status = GnbProcessTable (
             GnbHandle,
             GnbEarlierInitTableBeforeSmuTN,
             0,
             0,
             StdHeader
             );
  GetLogicalIdOfSocket (GnbGetSocketId (GnbHandle), &LogicalId, StdHeader);
  if ((LogicalId.Revision & AMD_F15_RL_ALL) != 0) {
    SmuFirmwareHeader = GnbFwPtrs.FirmwareType2;      // Load SMU firmware for Richland
  } else {
    SmuFirmwareHeader = GnbFwPtrs.FirmwareType1;      // Load SMU firmware for Trinity
  }
  ASSERT (SmuFirmwareHeader != NULL);
  if (SmuFirmwareHeader != NULL) {
    GnbSmuFirmwareLoadV4 (GnbHandle->Address, (FIRMWARE_HEADER_V4*) SmuFirmwareHeader, StdHeader);
  }
  Status = GnbProcessTable (
             GnbHandle,
             GnbEarlierInitTableAfterSmuTN,
             0,
             0,
             StdHeader
             );

  GnbAdjustSmuVidAfterSmuTN (StdHeader);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbEarlierInterfaceTN Exit [0x%x]\n", Status);
  return  Status;
}
