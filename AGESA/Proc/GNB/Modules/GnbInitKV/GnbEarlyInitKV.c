/* $NoKeywords:$ */
/**
 * @file
 *
 * GNB early post initialization.
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 283514 $   @e \$Date: 2014-01-25 20:05:27 -0600 (Sat, 25 Jan 2014) $
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
#include  "GnbSmuInitLibV7.h"
#include  "GnbSmuFirmwareKV.h"
#include  "GnbRegisterAccKV.h"
#include  "GnbRegistersKV.h"
#include  "cpuF15PowerMgmt.h"
#include  "GnbSmuScsKV.h"
#include  "GnbSmuBiosTableKV.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITKV_GNBEARLYINITKV_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

extern GNB_TABLE ROMDATA    GnbEarlierInitTableBeforeSmuKV [];
extern GNB_TABLE ROMDATA    GnbEarlyInitTableKV [];
extern GNB_BUILD_OPTIONS_KV GnbBuildOptionsKV;
extern BUILD_OPT_CFG        UserOptions;

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
 * Init TDC
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

VOID
GnbInitTdcKV (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{

}

/*----------------------------------------------------------------------------------------*/
/**
 * Init VPC PopDown Support
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 */

VOID
GnbInitPopDownKV (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{

}

/*----------------------------------------------------------------------------------------*/
/**
 * Set VID through SMU
 *
 *
 * @param[in] GnbHandle       GNB_HANDLE
 * @param[in] Vid             VID code
 * @param[in] StdHeader       Standard configuration header
 * @retval    OriginalVid     Original VID
 */

UINT8
GnbRequestVoltageKV (
  IN       GNB_HANDLE            *GnbHandle,
  IN       UINT8                 Vid,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  D0F0xBC_xC0200090_STRUCT  D0F0xBC_xC0200090;
  D0F0xBC_xC0200094_STRUCT  D0F0xBC_xC0200094;
  UINT8                     OriginalVid;

  GnbRegisterReadKV (GnbHandle,
                     D0F0xBC_xC0200090_TYPE,
                     D0F0xBC_xC0200090_ADDRESS,
                     &D0F0xBC_xC0200090, 0, StdHeader);

  GnbRegisterReadKV (GnbHandle,
                     D0F0xBC_xC0200094_TYPE,
                     D0F0xBC_xC0200094_ADDRESS,
                     &D0F0xBC_xC0200094, 0, StdHeader);

  OriginalVid = (UINT8) D0F0xBC_xC0200094.Field.SmuVoltageLvl;

  IDS_HDT_CONSOLE (GNB_TRACE, "  Start Vid: %02x\n", OriginalVid);
  IDS_HDT_CONSOLE (GNB_TRACE, "  Ask Vid:   %02x\n", Vid);

  D0F0xBC_xC0200090.Field.SmuVoltageEn = 1;
  D0F0xBC_xC0200090.Field.SmuVoltageLvl = Vid;
  GnbRegisterWriteKV (GnbHandle,
                      D0F0xBC_xC0200090_TYPE, D0F0xBC_xC0200090_ADDRESS,
                      &D0F0xBC_xC0200090, 0, StdHeader);
  do {
    GnbRegisterReadKV (GnbHandle,
                       D0F0xBC_xC0200094_TYPE,
                       D0F0xBC_xC0200094_ADDRESS,
                       &D0F0xBC_xC0200094, 0, StdHeader);
  } while (D0F0xBC_xC0200094.Field.SmuVoltageSts != 1);

  IDS_HDT_CONSOLE (GNB_TRACE, "  Got Vid:   %02x\n",
                   D0F0xBC_xC0200094.Field.SmuVoltageLvl);

  return OriginalVid;

}

/*----------------------------------------------------------------------------------------*/
/**
 * Set Sclk through SMU
 *
 *
 * @param[in] GnbHandle       GNB_HANDLE
 * @param[in] Did             DID code
 * @param[in] StdHeader       Standard configuration header
 * @retval    OriginalDid     Original DID
 */

UINT8
GnbRequestCgSclkKV (
  IN       GNB_HANDLE            *GnbHandle,
  IN       UINT8                 Did,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  D0F0xBC_xC050008C_STRUCT  D0F0xBC_xC050008C;
  D0F0xBC_xC0500090_STRUCT  D0F0xBC_xC0500090;
  UINT8                     OriginalDid;

  do {
    GnbRegisterReadKV (GnbHandle,
                       D0F0xBC_xC0500090_TYPE,
                       D0F0xBC_xC0500090_ADDRESS, &D0F0xBC_xC0500090, 0, StdHeader);
  } while (D0F0xBC_xC0500090.Field.SclkStatus == 0);

  GnbRegisterReadKV (GnbHandle,
                     D0F0xBC_xC050008C_TYPE,
                     D0F0xBC_xC050008C_ADDRESS,
                     &D0F0xBC_xC050008C, 0, StdHeader);

  OriginalDid = (UINT8) D0F0xBC_xC050008C.Field.SclkDivider;

  D0F0xBC_xC050008C.Field.SclkDivider = Did;
  GnbRegisterWriteKV (GnbHandle,
                      D0F0xBC_xC050008C_TYPE,
                      D0F0xBC_xC050008C_ADDRESS,
                      &D0F0xBC_xC050008C, 0, StdHeader);

  do {
    GnbRegisterReadKV (GnbHandle,
                       D0F0xBC_xC0500090_TYPE,
                       D0F0xBC_xC0500090_ADDRESS, &D0F0xBC_xC0500090, 0, StdHeader);
  } while (D0F0xBC_xC0500090.Field.SclkStatus == 0);

  IDS_HDT_CONSOLE (GNB_TRACE, "  Start Did: %02x\n", OriginalDid);
  IDS_HDT_CONSOLE (GNB_TRACE, "  Ask Did:   %02x\n", Did);
  IDS_HDT_CONSOLE (GNB_TRACE, "  Got Did:   %02x\n",
                   D0F0xBC_xC050008C.Field.SclkDivider);

  return OriginalDid;

}

/*----------------------------------------------------------------------------------------*/
/**
 * Request Max SCLK
 *
 *
 * @param[in]  GnbHandle  GNB_HANDLE
 * @param[in]  StdHeader  Standard configuration header
 * @retval    OriginalDid     Original DID
 */

UINT8
GnbRequestMaxSclkKV (
  IN      GNB_HANDLE                      *GnbHandle,
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  D0F0xBC_xC0107068_STRUCT D0F0xBC_xC0107068; // Sclk1
  D0F0xBC_xC010706C_STRUCT D0F0xBC_xC010706C; // Sclk3, Sclk4
  D0F0xBC_xC0104007_STRUCT D0F0xBC_xC0104007; // Vid1
  D0F0xBC_xC0104008_STRUCT D0F0xBC_xC0104008; // Vid3
  D0F0xBC_xC0104098_STRUCT D0F0xBC_xC0104098; // Vid4
  UINT8                    OriginalDid;
  UINT8                    NewVid;
  UINT8                    NewDid;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbRequestMaxSclkKV Enter\n");

  GnbRegisterReadKV (GnbHandle,
                     D0F0xBC_xC0107068_TYPE,
                     D0F0xBC_xC0107068_ADDRESS, &D0F0xBC_xC0107068, 0, StdHeader);
  GnbRegisterReadKV (GnbHandle,
                     D0F0xBC_xC010706C_TYPE,
                     D0F0xBC_xC010706C_ADDRESS, &D0F0xBC_xC010706C, 0, StdHeader);
  GnbRegisterReadKV (GnbHandle,
                     D0F0xBC_xC0104007_TYPE,
                     D0F0xBC_xC0104007_ADDRESS, &D0F0xBC_xC0104007, 0, StdHeader);
  GnbRegisterReadKV (GnbHandle,
                     D0F0xBC_xC0104008_TYPE,
                     D0F0xBC_xC0104008_ADDRESS, &D0F0xBC_xC0104008, 0, StdHeader);
  GnbRegisterReadKV (GnbHandle,
                     D0F0xBC_xC0104098_TYPE,
                     D0F0xBC_xC0104098_ADDRESS, &D0F0xBC_xC0104098, 0, StdHeader);

  if (D0F0xBC_xC010706C.Field.SClkDid4 != 0) {
    NewDid = (UINT8) D0F0xBC_xC010706C.Field.SClkDid4;
    NewVid = (UINT8) D0F0xBC_xC0104098.Field.VddNbVid4;
  } else {
    NewDid = (UINT8) D0F0xBC_xC0107068.Field.SClkDid3;
    NewVid = (UINT8) D0F0xBC_xC0104008.Field.VddNbVid3;
  }

  GnbRequestVoltageKV (GnbHandle,
                       (UINT8) D0F0xBC_xC0104007.Field.VddNbVid1, StdHeader);

  GnbRequestVoltageKV (GnbHandle, NewVid, StdHeader);
  OriginalDid = GnbRequestCgSclkKV (GnbHandle, NewDid, StdHeader);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbRequestMaxSclkKV Exit\n");

  return OriginalDid;

}

/*----------------------------------------------------------------------------------------*/
/**
 * Request VDDB Pmin
 *
 *
 *
 * @param[in]  GnbHandle  GNB_HANDLE
 * @param[in]  StdHeader  Standard configuration header
 */

VOID
GnbRequestVddNbPminKV (
  IN      GNB_HANDLE                      *GnbHandle,
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  D18F5x170_STRUCT        D18F5x170;
  D18F5x160_STRUCT        D18F5x160;
  UINT32                  Millivolt;
  UINT32                  NbVid;
  DEV_OBJECT              DevObject;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbRequestVddNbPminKV Enter\n");
  // 1) Read F5x170[NbPstateMaxVal] to find NB Pmin.
  GnbRegisterReadKV (GnbHandle, D18F5x170_TYPE, D18F5x170_ADDRESS, &D18F5x170, 0, StdHeader);
  // 2) Determine voltage associated with NB Pmin.  Read F5x(160+4*NbPstateMaxVal)[NbVid].  (For example, NBP0 is F5x160, NBP1 is F5x164, etc)
  GnbRegisterReadKV (GnbHandle, D18F5x160_TYPE, (D18F5x160_ADDRESS + (4 * D18F5x170.Field.NbPstateMaxVal)), &D18F5x160, 0, StdHeader);
  NbVid = (D18F5x160.Field.NbVid_7 << 7) | (D18F5x160.Field.NbVid_6_0);
  // 3) Make voltage request (via BIOSSMC_MSG_VDDNB_REQUEST) for voltage determined by Step 2.  Note that message requires encoding in voltage, not VID.
  //    Use the following equation to decode SVI2 VIDs:
  //      Voltage = 1.55 - (0.00625*VID)
  //    Furthermore, VDDNB requests to SMU should be encoded in 0.25mV steps.
  //    Therefore, voltage should be translated into mV and then multiplied by 4, to be encoded as 0.25mV steps.
  //    Or... ( 1.55 * 1000 * 4) - (.00625 * 1000 * 4) * VID) = (1550 * 4) - (25 * VID)
  Millivolt = (1550 * 4) - (25 * NbVid);

  IDS_HDT_CONSOLE (GNB_TRACE, "  Set Voltage for NbPstateMaxVal = 0x%x, Vid code 0x%x = %d mV\n", D18F5x170.Field.NbPstateMaxVal, NbVid, Millivolt);
  DevObject.StdHeader = StdHeader;
  DevObject.GnbHandle = GnbHandle;
  DevObject.DevPciAddress.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0);
  GnbSmuServiceRequestV7 (
    &DevObject,
    SMC_MSG_VDDNB_REQUEST,
    Millivolt,
    0
    );
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbRequestVddNbPminKV Exit\n");
  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Initialize GFX straps.
 *
 *
 * @param[in] StdHeader  Standard configuration header
 */

VOID
GfxStrapsEarlyInitKV (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AMD_EARLY_PARAMS            *EarlyParamsPtr;
  D0F0xD4_x013014AC_STRUCT    D0F0xD4_x13014AC;
  D0F0xD4_x013014B6_STRUCT    D0F0xD4_x13014B6;
  D0F0xD4_x013014BE_STRUCT    D0F0xD4_x13014BE;
  GNB_HANDLE                 *GnbHandle;

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxStrapsEarlyInitKV Enter\n");

  EarlyParamsPtr = (AMD_EARLY_PARAMS *) StdHeader;
  GnbHandle = GnbGetHandle (StdHeader);

  GnbRegisterReadKV (GnbHandle, D0F0xD4_x013014AC_TYPE, D0F0xD4_x013014AC_ADDRESS, &D0F0xD4_x13014AC.Value, 0, StdHeader);
  GnbRegisterReadKV (GnbHandle, D0F0xD4_x013014B6_TYPE, D0F0xD4_x013014B6_ADDRESS, &D0F0xD4_x13014B6.Value, 0, StdHeader);
  GnbRegisterReadKV (GnbHandle, D0F0xD4_x013014BE_TYPE, D0F0xD4_x013014BE_ADDRESS, &D0F0xD4_x13014BE.Value, 0, StdHeader);

  D0F0xD4_x13014B6.Field.StrapBifAzLegacyDeviceTypeDis = 0x0;
  D0F0xD4_x13014B6.Field.StrapBifF0LegacyDeviceTypeDis = 0x0;

  D0F0xD4_x13014AC.Field.StrapBifAudioEnPin = EarlyParamsPtr->PlatformConfig.GnbHdAudio;
  D0F0xD4_x13014B6.Field.StrapBifAudioEn = EarlyParamsPtr->PlatformConfig.GnbHdAudio;

  // Enable PCI Vendor Specific Capabilities
  D0F0xD4_x13014BE.Field.StrapBifSymalignHwDebug = 1;

  GnbRegisterWriteKV (GnbHandle, D0F0xD4_x013014AC_TYPE, D0F0xD4_x013014AC_ADDRESS, &D0F0xD4_x13014AC.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
  GnbRegisterWriteKV (GnbHandle, D0F0xD4_x013014B6_TYPE, D0F0xD4_x013014B6_ADDRESS, &D0F0xD4_x13014B6.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
  GnbRegisterWriteKV (GnbHandle, D0F0xD4_x013014BE_TYPE, D0F0xD4_x013014BE_ADDRESS, &D0F0xD4_x13014BE.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxStrapsEarlyInitKV Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Enable Gfx gBIF
 *
 *
 * @param[in] StdHeader  Standard configuration header
 * @retval    AGESA_STATUS
 */

AGESA_STATUS
GfxGBifEnableKV (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  D0F0xD4_x01301486_STRUCT    D0F0xD4_x1301486;
  D0F0xD4_x01091518_STRUCT    D0F0xD4_x1091518;
  D0F0xBC_xC0104000_STRUCT    D0F0xBC_xC0104000;
  GNB_HANDLE                  *GnbHandle;

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxGBifEnableKV Enter\n");
  GnbHandle = GnbGetHandle (StdHeader);

  GnbRegisterReadKV (GnbHandle, D0F0xBC_xC0104000_TYPE, D0F0xBC_xC0104000_ADDRESS, &D0F0xBC_xC0104000.Value, 0, StdHeader);
  if (D0F0xBC_xC0104000.Field.GPU_DIS != 1) {
    GfxStrapsEarlyInitKV (StdHeader);
  }

  GnbRegisterReadKV (GnbHandle, D0F0xD4_x01301486_TYPE, D0F0xD4_x01301486_ADDRESS, &D0F0xD4_x1301486.Value, 0, StdHeader);
  GnbRegisterReadKV (GnbHandle, D0F0xD4_x01091518_TYPE, D0F0xD4_x01091518_ADDRESS, &D0F0xD4_x1091518.Value, 0, StdHeader);
  D0F0xD4_x1301486.Field.LinkTrainEn = 0x1;
  D0F0xD4_x1091518.Field.LinkTrainEn = 0x1;
  GnbRegisterWriteKV (GnbHandle, D0F0xD4_x01091518_TYPE, D0F0xD4_x01091518_ADDRESS, &D0F0xD4_x1091518.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
  GnbRegisterWriteKV (GnbHandle, D0F0xD4_x01301486_TYPE, D0F0xD4_x01301486_ADDRESS, &D0F0xD4_x1301486.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxGBifEnableKV Exit\n");

  return  AGESA_SUCCESS;
}
/*----------------------------------------------------------------------------------------*/
/**
 * GNB init at early post
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
GnbEarlyInterfaceKV (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS     Status;
  AGESA_STATUS     AgesaStatus;
  GNB_HANDLE       *GnbHandle;
  UINT32           Property;
  AgesaStatus = AGESA_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbEarlyInterfaceKV Enter\n");
  GnbHandle = GnbGetHandle (StdHeader);
  Property = TABLE_PROPERTY_DEFAULT;
  Property |= UserOptions.CfgGnbSyncFloodPinAsNmi ? TABLE_PROPERTY_NMI_SYNCFLOOD : 0;
  IDS_OPTION_HOOK (IDS_GNB_PROPERTY, &Property, StdHeader);
  GnbInitSmuBiosTableKV (StdHeader);
  GnbInitTdcKV (StdHeader);
  GnbInitPopDownKV (StdHeader);
  Status = GnbProcessTable (
             GnbHandle,
             GnbEarlyInitTableKV,
             Property,
             0,
             StdHeader
             );
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  GnbRequestVddNbPminKV (GnbHandle, StdHeader);

  Status = GfxGBifEnableKV (StdHeader);
  ASSERT (Status == AGESA_SUCCESS);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbEarlyInterfaceKV Exit [0x%x]\n", AgesaStatus);
  return  AgesaStatus;
}


/*----------------------------------------------------------------------------------------*/
/**
 * GNB init at earlier post
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
GnbEarlierInterfaceKV (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS      Status;
  AGESA_STATUS      AgesaStatus;
  GNB_HANDLE        *GnbHandle;
  UINT8             OriginalDid;

  AgesaStatus = AGESA_SUCCESS;
  Status = AGESA_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbEarlierInterfaceKV Enter\n");
  GnbHandle = GnbGetHandle (StdHeader);

  OriginalDid = GnbRequestMaxSclkKV (GnbHandle, StdHeader);

  Status = GnbProcessTable (
             GnbHandle,
             GnbEarlierInitTableBeforeSmuKV,
             0,
             0,
             StdHeader
             );
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  Status = GnbSmuFirmwareLoadV7 (GnbHandle, (FIRMWARE_HEADER_V7*) &FirmwareKV[0], StdHeader);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  GnbRequestCgSclkKV (GnbHandle, OriginalDid, StdHeader);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbEarlierInterfaceKV Exit [0x%x]\n", Status);
  return  AgesaStatus;
}

/*----------------------------------------------------------------------------------------*/
/**
 * GNB Scs interface for KV
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */
AGESA_STATUS
GnbScsInterfaceKV (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS      Status;
  AGESA_STATUS      AgesaStatus;
  GNB_HANDLE        *GnbHandle;

  AgesaStatus = AGESA_SUCCESS;
  Status = AGESA_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbScsInterfaceKV Enter\n");
  GnbHandle = GnbGetHandle (StdHeader);

  Status = GnbSmuLoadScsDataV7 (GnbHandle, &GnbSmuScsKV[0], StdHeader);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbScsInterfaceKV Exit [0x%x]\n", Status);
  return  AgesaStatus;
}
