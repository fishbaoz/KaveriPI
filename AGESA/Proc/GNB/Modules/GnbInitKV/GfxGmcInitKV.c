/* $NoKeywords:$ */
/**
 * @file
 *
 * PCIe late post initialization.
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
#include  "Gnb.h"
#include  "GnbGfx.h"
#include  "GnbCommonLib.h"
#include  "GnbTable.h"
#include  "GnbPcieConfig.h"
#include  "GnbRegisterAccKV.h"
#include  "cpuFamilyTranslation.h"
#include  "GnbRegistersKV.h"
#include  "GfxLibKV.h"
#include  "GfxGmcInitKV.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITKV_GFXGMCINITKV_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

extern GNB_TABLE ROMDATA GfxGmcClockGatingDisableKV [];
extern GNB_TABLE ROMDATA GfxGmcInitTableKV [];
extern GNB_TABLE ROMDATA GfxGmcInitCh1Dct3KV [];
extern GNB_TABLE ROMDATA GfxGmcClockGatingEnableKV [];
extern GNB_TABLE ROMDATA GfxGmcTCClientSteering2DctKV [];
extern GNB_TABLE ROMDATA GfxGmcTCClientSteering1DctModeAKV [];
extern GNB_TABLE ROMDATA GfxGmcTCClientSteering1DctModeBKV [];

#define GNB_GFX_DRAM_CH_0_PRESENT 1
#define GNB_GFX_DRAM_CH_1_PRESENT 2

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
 * Initialize Fb location
 *
 *
 *
 * @param[in]   GnbHandle         Pointer to GNB_HANDLE
 * @param[in]   Gfx               Pointer to global GFX configuration
 *
 */
VOID
GfxGmcInitializeFbLocationKV (
  IN      GNB_HANDLE            *GnbHandle,
  IN      GFX_PLATFORM_CONFIG   *Gfx
  )
{
  GMMx2024_STRUCT   GMMx2024;
  GMMx2068_STRUCT   GMMx2068;
  GMMx2C04_STRUCT   GMMx2C04;
  GMMx5428_STRUCT   GMMx5428;
  UINT64            FBBase;
  UINT64            FBTop;

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxGmcInitializeFbLocationKV Enter\n");

  FBBase = 0x0F400000000;
  FBTop = FBBase + Gfx->UmaInfo.UmaSize - 1;
  GMMx2024.Value = 0;
  GMMx2C04.Value = 0;
  GMMx2024.Field.FB_BASE = (UINT16) (FBBase >> 24);
  GMMx2024.Field.FB_TOP = (UINT16) (FBTop >> 24);
  GMMx2068.Field.FB_OFFSET = (UINT32) (Gfx->UmaInfo.UmaBase >> 22);
  GMMx2C04.Field.NONSURF_BASE = (UINT32) (FBBase >> 8);
  GMMx5428.Field.CONFIG_MEMSIZE = Gfx->UmaInfo.UmaSize >> 20;
  GnbRegisterWriteKV (GnbHandle, GMMx2024_TYPE, GMMx2024_ADDRESS, &GMMx2024.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  GnbRegisterWriteKV (GnbHandle, GMMx2068_TYPE, GMMx2068_ADDRESS, &GMMx2068.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  GnbRegisterWriteKV (GnbHandle, GMMx2C04_TYPE, GMMx2C04_ADDRESS, &GMMx2C04.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  GnbRegisterWriteKV (GnbHandle, GMMx5428_TYPE, GMMx5428_ADDRESS, &GMMx5428.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
}

/*----------------------------------------------------------------------------------------*/
/**
 * Initialize sequencer model
 *
 *
 *
 * @param[in]   GnbHandle         Pointer to GNB_HANDLE
 * @param[in]   Gfx               Pointer to global GFX configuration
 */
UINT8
GfxGmcInitializeHubAndCitfSteeringKV (
  IN      GNB_HANDLE            *GnbHandle,
  IN      GFX_PLATFORM_CONFIG   *Gfx
  )
{
  UINT8                     DramChannelPresent;
  GMMx2004_STRUCT           GMMx2004;
  GMMx2008_STRUCT           GMMx2008;
  UINT32                    D18F2x60;
  UINT32                    D18F2x64;
  UINT32                    D18F2x68;
  UINT32                    D18F2x6C;
  D18F2x78_dct0_STRUCT      D18F2x78;

  // Read memory configuration info
  DramChannelPresent = 0;

  // When any of F2x[6C:60] is not zero, its channel is enabled
  GnbRegisterReadKV (GnbHandle, D18F2x60_dct0_TYPE, D18F2x60_dct0_ADDRESS, &D18F2x60, 0, GnbLibGetHeader (Gfx));
  GnbRegisterReadKV (GnbHandle, D18F2x64_dct0_TYPE, D18F2x64_dct0_ADDRESS, &D18F2x64, 0, GnbLibGetHeader (Gfx));
  GnbRegisterReadKV (GnbHandle, D18F2x68_dct0_TYPE, D18F2x68_dct0_ADDRESS, &D18F2x68, 0, GnbLibGetHeader (Gfx));
  GnbRegisterReadKV (GnbHandle, D18F2x6C_dct0_TYPE, D18F2x6C_dct0_ADDRESS, &D18F2x6C, 0, GnbLibGetHeader (Gfx));
  GnbRegisterReadKV (GnbHandle, D18F2x78_dct0_TYPE, D18F2x78_dct0_ADDRESS, &D18F2x78.Value, 0, GnbLibGetHeader (Gfx));
  if ((D18F2x60 != 0) || (D18F2x64 != 0) || (D18F2x68 != 0) || (D18F2x6C != 0)) {
    DramChannelPresent |= GNB_GFX_DRAM_CH_0_PRESENT;
  }
  // Check channel 3 - when any of F2x[6C:60] is not zero, its channel is enabled
  GnbRegisterReadKV (GnbHandle, D18F2x60_dct3_TYPE, D18F2x60_dct3_ADDRESS, &D18F2x60, 0, GnbLibGetHeader (Gfx));
  GnbRegisterReadKV (GnbHandle, D18F2x64_dct3_TYPE, D18F2x64_dct3_ADDRESS, &D18F2x64, 0, GnbLibGetHeader (Gfx));
  GnbRegisterReadKV (GnbHandle, D18F2x68_dct3_TYPE, D18F2x68_dct3_ADDRESS, &D18F2x68, 0, GnbLibGetHeader (Gfx));
  GnbRegisterReadKV (GnbHandle, D18F2x6C_dct3_TYPE, D18F2x6C_dct3_ADDRESS, &D18F2x6C, 0, GnbLibGetHeader (Gfx));
  GnbRegisterReadKV (GnbHandle, D18F2x78_dct3_TYPE, D18F2x78_dct3_ADDRESS, &D18F2x78.Value, 0, GnbLibGetHeader (Gfx));
  if ((D18F2x60 != 0) || (D18F2x64 != 0) || (D18F2x68 != 0) || (D18F2x6C != 0)) {
    DramChannelPresent |= GNB_GFX_DRAM_CH_1_PRESENT;
  }

  GnbRegisterReadKV (GnbHandle, GMMx2004_TYPE, GMMx2004_ADDRESS, &GMMx2004.Value, 0, GnbLibGetHeader (Gfx));
  GnbRegisterReadKV (GnbHandle, GMMx2008_TYPE, GMMx2008_ADDRESS, &GMMx2008.Value, 0, GnbLibGetHeader (Gfx));

  GMMx2004.Field.NOOFCHAN = 2;

  if (DramChannelPresent == (GNB_GFX_DRAM_CH_0_PRESENT | GNB_GFX_DRAM_CH_1_PRESENT)) {
    GMMx2008.Field.CHAN0 = 0;
    GMMx2008.Field.CHAN1 = 3;
    GMMx2008.Field.CHAN2 = 0;
    GMMx2008.Field.CHAN3 = 3;
  } else if (DramChannelPresent == GNB_GFX_DRAM_CH_0_PRESENT) {
    GMMx2008.Field.CHAN0 = 0;
    GMMx2008.Field.CHAN1 = 0;
    GMMx2008.Field.CHAN2 = 0;
    GMMx2008.Field.CHAN3 = 0;
  } else if (DramChannelPresent == GNB_GFX_DRAM_CH_1_PRESENT) {
    GMMx2008.Field.CHAN0 = 3;
    GMMx2008.Field.CHAN1 = 3;
    GMMx2008.Field.CHAN2 = 3;
    GMMx2008.Field.CHAN3 = 3;
  }

  GnbRegisterWriteKV (GnbHandle, GMMx2004_TYPE, GMMx2004_ADDRESS, &GMMx2004.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  GnbRegisterWriteKV (GnbHandle, GMMx2008_TYPE, GMMx2008_ADDRESS, &GMMx2008.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));

  return DramChannelPresent;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Enable Stutter Mode with/without power-gating
 *
 *
 *
 * @param[in]   GnbHandle         Pointer to GNB_HANDLE
 * @param[in]   Gfx               Pointer to global GFX configuration
 *
 */
VOID
GfxGmcEnableStutterModePowerGatingKV (
  IN      GNB_HANDLE            *GnbHandle,
  IN      GFX_PLATFORM_CONFIG   *Gfx
  )
{
  GMMx3508_STRUCT   GMMx3508;
  GMMx350C_STRUCT   GMMx350C;

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxGmcEnableStutterModePowerGatingKV Enter\n");

  GnbRegisterReadKV (GnbHandle, GMMx3508_TYPE, GMMx3508_ADDRESS, &GMMx3508.Value, 0, GnbLibGetHeader (Gfx));
  GnbRegisterReadKV (GnbHandle, GMMx350C_TYPE, GMMx350C_ADDRESS, &GMMx350C.Value, 0, GnbLibGetHeader (Gfx));
  if (Gfx->GmcPowerGating == GmcPowerGatingWithStutter) {
    // Enabling power gating
    GMMx3508.Field.RENG_EXECUTE_ON_PWR_UP = 1;
    GMMx350C.Field.RENG_EXECUTE_ON_REG_UPDATE = 1;
    GnbRegisterWriteKV (GnbHandle, GMMx350C_TYPE, GMMx350C_ADDRESS, &GMMx350C.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
    GnbRegisterWriteKV (GnbHandle, GMMx3508_TYPE, GMMx3508_ADDRESS, &GMMx3508.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  } else {
    // Disabling power gating
    GMMx3508.Field.RENG_EXECUTE_ON_PWR_UP = 0;
    GMMx350C.Field.RENG_EXECUTE_ON_REG_UPDATE = 0;
    GnbRegisterWriteKV (GnbHandle, GMMx3508_TYPE, GMMx3508_ADDRESS, &GMMx3508.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
    GnbRegisterWriteKV (GnbHandle, GMMx350C_TYPE, GMMx350C_ADDRESS, &GMMx350C.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
    if (Gfx->GmcPowerGating == GmcPowerGatingStutterOnly) {
      //GmcPowerGatingStutterOnly
      GMMx350C.Field.STCTRL_STUTTER_EN = 1;
      GnbRegisterWriteKV (GnbHandle, GMMx350C_TYPE, GMMx350C_ADDRESS, &GMMx350C.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxGmcEnableStutterModePowerGatingKV Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Secure Garlic Access
 *
 *
 *
 * @param[in]   GnbHandle         Pointer to GNB_HANDLE
 * @param[in] Gfx         Pointer to global GFX configuration
 */

VOID
GfxGmcSecureGarlicAccessKV (
  IN      GNB_HANDLE            *GnbHandle,
  IN      GFX_PLATFORM_CONFIG   *Gfx
  )
{
  UINT32   GMMx2868;
  UINT32   GMMx286C;
  UINT32   GMMx2878;

  GMMx2868 = (UINT32) (Gfx->UmaInfo.UmaBase >> 20);
  GnbRegisterWriteKV (GnbHandle, GMMx2868_TYPE, GMMx2868_ADDRESS, &GMMx2868, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  GMMx286C = (UINT32) (((Gfx->UmaInfo.UmaBase + Gfx->UmaInfo.UmaSize) >> 20) - 1);
  GnbRegisterWriteKV (GnbHandle, GMMx286C_TYPE, GMMx286C_ADDRESS, &GMMx286C, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  // Areag FB - 32K reserved by VBIOS for SBIOS to use
  GMMx2878 = (UINT32) ((Gfx->UmaInfo.UmaBase + Gfx->UmaInfo.UmaSize - 32 * 1024) >> 12);
  GnbRegisterWriteKV (GnbHandle, GMMx2878_TYPE, GMMx2878_ADDRESS, &GMMx2878, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
}

/*----------------------------------------------------------------------------------------*/
/**
 * Initialize C6 aperture location
 *
 *
 *
 * @param[in]   GnbHandle         Pointer to GNB_HANDLE
 * @param[in]   Gfx               Pointer to global GFX configuration
 *
 */
VOID
GfxGmcInitializeC6LocationKV (
  IN      GNB_HANDLE            *GnbHandle,
  IN      GFX_PLATFORM_CONFIG   *Gfx
  )
{
  D18F2x118_STRUCT    D18F2x118;
  D18F1x124_STRUCT    D18F1x124;
  UINT32              GMMx2870;
  UINT32              GMMx2874;

  // From D18F1x[124,120] DRAM Base/Limit,
  // {DramBase[47:27], 000_0000h} <= address[47:0] <= {DramLimit[47:27], 7FF_FFFFh}.
  GnbRegisterReadKV (GnbHandle, D18F1x124_TYPE, D18F1x124_ADDRESS, &D18F1x124.Value, 0, GnbLibGetHeader (Gfx));
  // base 47:20
  GMMx2870 = ((D18F1x124.Field.DramLimitAddr_47_27 + 1) << 7);
  // top 47:20
  GMMx2874 = (((D18F1x124.Field.DramLimitAddr_47_27 + 1) << 7) - 1) + ((16 * 0x100000) << 20);

  // Check C6 enable,  D18F2x118[CC6SaveEn]
  GnbRegisterReadKV (GnbHandle, D18F2x118_TYPE, D18F2x118_ADDRESS, &D18F2x118.Value, 0, GnbLibGetHeader (Gfx));
  if (D18F2x118.Field.CC6SaveEn) {
    GnbRegisterWriteKV (GnbHandle, GMMx2874_TYPE, GMMx2874_ADDRESS, &GMMx2874, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
    GnbRegisterWriteKV (GnbHandle, GMMx2870_TYPE, GMMx2870_ADDRESS, &GMMx2870, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Initialize GMC
 *
 *
 *
 * @param[in]   Gfx               Pointer to global GFX configuration
 *
 */

AGESA_STATUS
GfxGmcInitKV (
  IN      GFX_PLATFORM_CONFIG   *Gfx
  )
{
  UINT32            GMMx350C;
  UINT32            GMMx206C;
  D18F3x44_STRUCT   D18F3x44;
  GNB_HANDLE        *GnbHandle;
  UINT8             DramChannelPresent;

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxGmcInitKV Enter\n");
//2.1 Disable clock-gating
  GnbHandle = GnbGetHandle (GnbLibGetHeader (Gfx));
  ASSERT (GnbHandle != NULL);
  GnbProcessTable (
    GnbHandle,
    GfxGmcClockGatingDisableKV,
    0,
    GNB_TABLE_FLAGS_FORCE_S3_SAVE,
    GnbLibGetHeader (Gfx)
    );
//2.3 HUB & CITF channel steering
  DramChannelPresent = GfxGmcInitializeHubAndCitfSteeringKV (GnbHandle, Gfx);
//2.6 Frame buffer location
  GfxGmcInitializeFbLocationKV (GnbHandle, Gfx);
//2.10 Securing GARLIC access
//2.10.1 GARLIC request disable
  GfxGmcSecureGarlicAccessKV (GnbHandle, Gfx);
//2.10.2 C6 save aperture
  GfxGmcInitializeC6LocationKV (GnbHandle, Gfx);
//2.2 System memory address translation
//2.4 Sequencer model programming
//2.5 Power Gating - PGFSM and RENG init
//2.8 Performance tuning
//2.9-2.11 Garlic Programming
//2.12 Display latency
//2.13 Remove blackout
  GnbProcessTable (
    GnbHandle,
    GfxGmcInitCh1Dct3KV,
    0,
    GNB_TABLE_FLAGS_FORCE_S3_SAVE,
    GnbLibGetHeader (Gfx)
    );
  GnbProcessTable (
    GnbHandle,
    GfxGmcInitTableKV,
    0,
    GNB_TABLE_FLAGS_FORCE_S3_SAVE,
    GnbLibGetHeader (Gfx)
    );
//2.6 TC Client Steering
  if (DramChannelPresent == (GNB_GFX_DRAM_CH_0_PRESENT | GNB_GFX_DRAM_CH_1_PRESENT)) {
    GnbProcessTable (
      GnbHandle,
      GfxGmcTCClientSteering2DctKV,
      0,
      GNB_TABLE_FLAGS_FORCE_S3_SAVE,
      GnbLibGetHeader (Gfx)
      );
  } else if (DramChannelPresent == GNB_GFX_DRAM_CH_0_PRESENT) {
    GnbProcessTable (
      GnbHandle,
      GfxGmcTCClientSteering1DctModeAKV,
      0,
      GNB_TABLE_FLAGS_FORCE_S3_SAVE,
      GnbLibGetHeader (Gfx)
      );
  } else if (DramChannelPresent == GNB_GFX_DRAM_CH_1_PRESENT) {
    GnbProcessTable (
      GnbHandle,
      GfxGmcTCClientSteering1DctModeBKV,
      0,
      GNB_TABLE_FLAGS_FORCE_S3_SAVE,
      GnbLibGetHeader (Gfx)
      );
  }
//4. Power management
//4.1 Enabling clock-gating
  if (Gfx->GmcClockGating) {
    GnbProcessTable (
      GnbHandle,
      GfxGmcClockGatingEnableKV,
      0,
      GNB_TABLE_FLAGS_FORCE_S3_SAVE,
      GnbLibGetHeader (Gfx)
      );
  }

//5. Steering all requests to ONION
  switch (Gfx->UmaSteering) {
  case DefaultGarlic:
    GMMx206C = DefaultGarlic;
    GnbRegisterReadKV (GnbHandle, D18F3x44_TYPE, D18F3x44_ADDRESS, &D18F3x44.Value, 0, GnbLibGetHeader (Gfx));
    if (D18F3x44.Field.DramEccEn == 1) {
      // If ECC enabled, do not use DefaultGarlic
      GMMx206C = SystemTrafficOnion;
    }
    break;
  case SystemTrafficOnion:
    GMMx206C = SystemTrafficOnion;
    break;
  case Onion:
    GMMx206C = Onion;
    break;
  case Garlic:
    GMMx206C = Garlic;
    break;
  default:
    GMMx206C = DefaultGarlic;
    ASSERT (FALSE);
    break;
  }
  GnbRegisterWriteKV (GnbHandle, GMMx206C_TYPE, GMMx206C_ADDRESS, &GMMx206C, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));

  IDS_OPTION_CALLOUT (IDS_CALLOUT_GNB_GMM_REGISTER_OVERRIDE, Gfx, GnbLibGetHeader (Gfx));
  if (Gfx->GmcLockRegisters) {
    GnbRegisterReadKV (GnbHandle, GMMx350C_TYPE, GMMx350C_ADDRESS, &GMMx350C, 0, GnbLibGetHeader (Gfx));
    GMMx350C |= BIT27;
    GnbRegisterWriteKV (GnbHandle, GMMx350C_TYPE, GMMx350C_ADDRESS, &GMMx350C, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  }
//4.2 Enabling stutter mode with or without power-gating
  GfxGmcEnableStutterModePowerGatingKV (GnbHandle, Gfx);
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxGmcInitKV Exit\n");
  return AGESA_SUCCESS;
}
