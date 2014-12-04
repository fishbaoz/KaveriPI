/* $NoKeywords:$ */
/**
 * @file
 *
 * GFX env post initialization.
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
#include  "GnbGfx.h"
#include  "GnbTable.h"
#include  "GnbPcieConfig.h"
#include  "GnbCommonLib.h"
#include  "GnbGfxInitLibV1.h"
#include  "GnbGfxConfig.h"
#include  "GnbGfxFamServices.h"
#include  "GfxLibKV.h"
#include  "GnbRegistersKV.h"
#include  "GnbRegisterAccKV.h"
#include  "GnbHandleLib.h"
#include  "cpuFamilyTranslation.h"
#include  "OptionGnb.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITKV_GFXENVINITKV_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

extern GNB_TABLE ROMDATA      GfxEnvInitTableKV[];
extern GNB_BUILD_OPTIONS_KV   GnbBuildOptionsKV;

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
 * Initialize GFX straps.
 *
 *
 * @param[in] Gfx             Pointer to global GFX configuration
 * @retval    AGESA_STATUS
 */

AGESA_STATUS
GfxEnvInitKV (
  IN      GFX_PLATFORM_CONFIG   *Gfx
  )
{
  D0F0xD4_x010914E1_STRUCT    D0F0xD4_x10914E1;
  D0F0xD4_x010914E2_STRUCT    D0F0xD4_x10914E2;
  D0F0xD4_x01091507_STRUCT    D0F0xD4_x1091507;
  D0F0x64_x1D_STRUCT          D0F0x64_x1D;
  UINT32                      D0F0xD4_x010914C3;
  GNB_HANDLE                 *GnbHandle;

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxEnvInitKV Enter\n");

  GnbHandle = GnbGetHandle (GnbLibGetHeader (Gfx));

  GnbRegisterReadKV (GnbHandle, D0F0xD4_x010914E1_TYPE, D0F0xD4_x010914E1_ADDRESS, &D0F0xD4_x10914E1.Value, 0, GnbLibGetHeader (Gfx));
  GnbRegisterReadKV (GnbHandle, D0F0xD4_x010914E2_TYPE, D0F0xD4_x010914E2_ADDRESS, &D0F0xD4_x10914E2.Value, 0, GnbLibGetHeader (Gfx));
  GnbRegisterReadKV (GnbHandle, D0F0xD4_x01091507_TYPE, D0F0xD4_x01091507_ADDRESS, &D0F0xD4_x1091507.Value, 0, GnbLibGetHeader (Gfx));
  GnbRegisterReadKV (GnbHandle, D0F0xD4_x010914C3_TYPE, D0F0xD4_x010914C3_ADDRESS, &D0F0xD4_x010914C3, 0, GnbLibGetHeader (Gfx));

  GnbLibPciIndirectRead (
    GNB_SBDFO | D0F0x60_ADDRESS,
    D0F0x64_x1D_ADDRESS | IOC_WRITE_ENABLE,
    AccessWidth32,
    &D0F0x64_x1D.Value,
    GnbLibGetHeader (Gfx)
    );

  D0F0x64_x1D.Field.VgaEn = 0x1;
  D0F0x64_x1D.Field.Vga16En = 0x1;

  D0F0xD4_x10914E2.Field.StrapBifAzLegacyDeviceTypeDis = 0x0;
  D0F0xD4_x10914E2.Field.StrapBifF0LegacyDeviceTypeDis = 0x0;
  D0F0xD4_x1091507.Field.StrapBifAudioEnPin = Gfx->GnbHdAudio;
  D0F0xD4_x10914E2.Field.StrapBifAudioEn = Gfx->GnbHdAudio;

  D0F0xD4_x10914E1.Field.StrapBifRegApSize = 0x2;

  if (Gfx->UmaInfo.UmaSize > 128 * 0x100000) {
    D0F0xD4_x10914E1.Field.StrapBifMemApSize = 0x1;
  } else if (Gfx->UmaInfo.UmaSize > 64 * 0x100000) {
    D0F0xD4_x10914E1.Field.StrapBifMemApSize = 0x0;
  } else {
    D0F0xD4_x10914E1.Field.StrapBifMemApSize = 0x2;
  }

  GnbLibPciIndirectWrite (
    GNB_SBDFO | D0F0x60_ADDRESS,
    D0F0x64_x1D_ADDRESS | IOC_WRITE_ENABLE,
    AccessS3SaveWidth32,
    &D0F0x64_x1D.Value,
    GnbLibGetHeader (Gfx)
    );

  D0F0xD4_x010914C3 |= BIT0;
  GnbRegisterWriteKV (GnbHandle, D0F0xD4_x010914E1_TYPE, D0F0xD4_x010914E1_ADDRESS, &D0F0xD4_x10914E1.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  GnbRegisterWriteKV (GnbHandle, D0F0xD4_x010914E2_TYPE, D0F0xD4_x010914E2_ADDRESS, &D0F0xD4_x10914E2.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  GnbRegisterWriteKV (GnbHandle, D0F0xD4_x01091507_TYPE, D0F0xD4_x01091507_ADDRESS, &D0F0xD4_x1091507.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  GnbRegisterWriteKV (GnbHandle, D0F0xD4_x010914C3_TYPE, D0F0xD4_x010914C3_ADDRESS, &D0F0xD4_x010914C3, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxEnvInitKV Exit\n");
  return  AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Init GFX at Env Post.
 *
 *
 *
 * @param[in] StdHeader     Standard configuration header
 * @retval    AGESA_STATUS
  */


AGESA_STATUS
GfxEnvInterfaceKV (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS          Status;
  AGESA_STATUS          AgesaStatus;
  GFX_PLATFORM_CONFIG   *Gfx;
  GNB_HANDLE            *GnbHandle;
  UINT32                Property;

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxEnvInterfaceKV Enter\n");
  AgesaStatus = AGESA_SUCCESS;
  Property = TABLE_PROPERTY_DEFAULT;
  Property |= GnbBuildOptionsKV.CfgChubClockGating ? TABLE_PROPERTY_CHUB_CLOCK_GATING : 0;
  Property |= !GnbBuildOptionsKV.CfgAcpClockGating ? TABLE_PROPERTY_ACP_CLOCK_GATING_DISABLED : 0;

  Status = GfxLocateConfigData (StdHeader, &Gfx);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);
  if (Status == AGESA_SUCCESS) {
    if (Gfx->UmaInfo.UmaMode != UMA_NONE) {
      Status = GfxEnvInitKV (Gfx);
      AGESA_STATUS_UPDATE (Status, AgesaStatus);
      ASSERT (Status == AGESA_SUCCESS);
    } else {
      GfxFmDisableController (StdHeader);
      Property |= TABLE_PROPERTY_IGFX_DISABLED;
    }
  } else {
    GfxFmDisableController (StdHeader);
    Property |= TABLE_PROPERTY_IGFX_DISABLED;
  }

  IDS_OPTION_HOOK (IDS_GNB_PROPERTY, &Property, StdHeader);

  GnbHandle = GnbGetHandle (StdHeader);
  ASSERT (GnbHandle != NULL);
  Status = GnbProcessTable (
             GnbHandle,
             GfxEnvInitTableKV,
             Property,
             GNB_TABLE_FLAGS_FORCE_S3_SAVE,
             StdHeader
             );
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxEnvInterfaceKV Exit [0x%x]\n", AgesaStatus);
  return  Status;
}
