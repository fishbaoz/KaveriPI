/* $NoKeywords:$ */
/**
 * @file
 *
 * Service procedure to initialize Integrated Info Table
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
#include  "GnbGfx.h"
#include  "GnbCommonLib.h"
#include  "GnbPcieInitLibV1.h"
#include  "GnbPcieConfig.h"
#include  "GnbGfxFamServices.h"
#include  "GnbRegistersCommon.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBGFXINITLIBV1_GFXENUMCONNECTORS_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */



/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


typedef struct {
  PCIE_CONNECTOR_TYPE ConnectorType;
  UINT8               DisplayDeviceEnum;
  UINT16              ConnectorEnum;
  UINT16              EncoderEnum;
  UINT8               ConnectorIndex;
} EXT_CONNECTOR_INFO;

typedef struct {
  UINT8               DisplayDeviceEnum;
  UINT8               DeviceIndex;
  UINT16              DeviceTag;
  UINT16              DeviceAcpiEnum;
} EXT_DISPLAY_DEVICE_INFO;

typedef struct {
  AGESA_STATUS        Status;
  UINT8               DisplayDeviceEnum;
  UINT8               RequestedPriorityIndex;
  UINT8               CurrentPriorityIndex;
  PCIe_ENGINE_CONFIG  *Engine;
} CONNECTOR_ENUM_INFO;


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

AGESA_STATUS
GfxIntegratedEnumConnectorsForDevice (
  IN       UINT8                            DisplayDeviceEnum,
     OUT   EXT_DISPLAY_PATH                 *DisplayPathList,
  IN OUT   PCIe_PLATFORM_CONFIG             *Pcie,
  IN       GFX_PLATFORM_CONFIG              *Gfx
  );

VOID
GfxIntegratedDebugDumpDisplayPath (
  IN       EXT_DISPLAY_PATH                 *DisplayPath,
  IN       GFX_PLATFORM_CONFIG              *Gfx
  );



EXT_CONNECTOR_INFO ConnectorInfoTable[] = {
  {
    ConnectorTypeDP,
    DEVICE_DFP,
    CONNECTOR_DISPLAYPORT_ENUM,
    ENCODER_NOT_PRESENT,
    0,
  },
  {
    ConnectorTypeEDP,
    DEVICE_LCD,
    CONNECTOR_eDP_ENUM,
    ENCODER_NOT_PRESENT,
    1
  },
  {
    ConnectorTypeSingleLinkDVI,
    DEVICE_DFP,
    CONNECTOR_SINGLE_LINK_DVI_D_ENUM,
    ENCODER_NOT_PRESENT,
    2
  },
  {
    ConnectorTypeDualLinkDVI,
    DEVICE_DFP,
    CONNECTOR_DUAL_LINK_DVI_D_ENUM,
    ENCODER_NOT_PRESENT,
    3
  },
  {
    ConnectorTypeHDMI,
    DEVICE_DFP,
    CONNECTOR_HDMI_TYPE_A_ENUM,
    ENCODER_NOT_PRESENT,
    4
  },
  {
    ConnectorTypeDpToVga,
    DEVICE_CRT,
    CONNECTOR_VGA_ENUM,
    ENCODER_DP2VGA_ENUM_ID1,
    5
  },
  {
    ConnectorTypeDpToLvds,
    DEVICE_LCD,
    CONNECTOR_LVDS_ENUM,
    ENCODER_DP2LVDS_ENUM_ID2,
    6
  },
  {
    ConnectorTypeNutmegDpToVga,
    DEVICE_CRT,
    CONNECTOR_VGA_ENUM,
    ENCODER_ALMOND_ENUM_ID1,
    5
  },
  {
    ConnectorTypeSingleLinkDviI,
    DEVICE_DFP,
    CONNECTOR_SINGLE_LINK_DVI_I_ENUM,
    ENCODER_NOT_PRESENT,
    5
  },
  {
    ConnectorTypeCrt,
    DEVICE_CRT,
    CONNECTOR_VGA_ENUM,
    ENCODER_NOT_PRESENT,
    5
  },
  {
    ConnectorTypeLvds,
    DEVICE_LCD,
    CONNECTOR_LVDS_ENUM,
    ENCODER_NOT_PRESENT,
    6
  },
  {
    ConnectorTypeEDPToLvds,
    DEVICE_LCD,
    CONNECTOR_eDP_ENUM,
    ENCODER_NOT_PRESENT,
    1
  },
  {
    ConnectorTypeEDPToLvdsSwInit,
    DEVICE_LCD,
    CONNECTOR_eDP_ENUM,
    ENCODER_NOT_PRESENT,
    1
  },
  {
    ConnectorTypeAutoDetect,
    DEVICE_LCD,
    CONNECTOR_LVDS_eDP_ENUM,
    ENCODER_DP2LVDS_ENUM_ID2,
    7
  },
};

UINT8 ConnectorNumerArray[] = {
//  DP    eDP   SDVI-D  DDVI-D  HDMI   VGA   LVDS   Auto (eDP, LVDS, DP-to-LVDS)
    6,    1,    6,      6,      6,     1,    1,     2
};
/*----------------------------------------------------------------------------------------*/
/**
 * Enumerate all display connectors for specific display device type.
 *
 *
 *
 * @param[in] ConnectorType   Connector type (see PCIe_DDI_DATA::ConnectorType).
 * @retval    Pointer to EXT_CONNECTOR_INFO
 * @retval    NULL if connector type unknown.
 */
EXT_CONNECTOR_INFO*
GfxIntegratedExtConnectorInfo (
  IN       UINT8                         ConnectorType
  )
{
  UINTN Index;
  for (Index = 0; Index < (sizeof (ConnectorInfoTable) / sizeof (EXT_CONNECTOR_INFO)); Index++) {
    if (ConnectorInfoTable[Index].ConnectorType == ConnectorType) {
      return &ConnectorInfoTable[Index];
    }
  }
  return NULL;
}

EXT_DISPLAY_DEVICE_INFO DisplayDeviceInfoTable[] = {
  {
    DEVICE_CRT,
    1,
    ATOM_DEVICE_CRT1_SUPPORT,
    0x100,
  },
  {
    DEVICE_LCD,
    1,
    ATOM_DEVICE_LCD1_SUPPORT,
    0x110,
  },
  {
    DEVICE_DFP,
    1,
    ATOM_DEVICE_DFP1_SUPPORT,
    0x210,
  },
  {
    DEVICE_DFP,
    2,
    ATOM_DEVICE_DFP2_SUPPORT,
    0x220,
  },
  {
    DEVICE_DFP,
    3,
    ATOM_DEVICE_DFP3_SUPPORT,
    0x230,
  },
  {
    DEVICE_DFP,
    4,
    ATOM_DEVICE_DFP4_SUPPORT,
    0x240,
  },
  {
    DEVICE_DFP,
    5,
    ATOM_DEVICE_DFP5_SUPPORT,
    0x250,
  },
  {
    DEVICE_DFP,
    6,
    ATOM_DEVICE_DFP6_SUPPORT,
    0x260,
  }
};
/*----------------------------------------------------------------------------------------*/
/**
 * Enumerate all display connectors for specific display device type.
 *
 *
 *
 * @param[in] DisplayDeviceEnum   Display device enum
 * @param[in] DisplayDeviceIndex  Display device index
 * @retval    Pointer to EXT_DISPLAY_DEVICE_INFO
 * @retval    NULL if can not get display device info
 */
EXT_DISPLAY_DEVICE_INFO*
GfxIntegratedExtDisplayDeviceInfo (
  IN       UINT8                         DisplayDeviceEnum,
  IN       UINT8                         DisplayDeviceIndex
  )
{
  UINT8 Index;
  UINT8 LastIndex;
  LastIndex = 0xff;
  for (Index = 0; Index < (sizeof (DisplayDeviceInfoTable) / sizeof (EXT_DISPLAY_DEVICE_INFO)); Index++) {
    if (DisplayDeviceInfoTable[Index].DisplayDeviceEnum == DisplayDeviceEnum) {
      LastIndex = Index;
      if (DisplayDeviceInfoTable[Index].DeviceIndex == DisplayDeviceIndex) {
        return &DisplayDeviceInfoTable[Index];
      }
    }
  }
  if (DisplayDeviceEnum == DEVICE_LCD && LastIndex != 0xff) {
    return &DisplayDeviceInfoTable[LastIndex];
  }
  return NULL;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Enumerate all display connectors
 *
 *
 *
 * @param[out]    DisplayPathList   Display path list
 * @param[in,out] Pcie              PCIe platform configuration info
 * @param[in]     Gfx               Gfx configuration info
 */
AGESA_STATUS
GfxIntegratedEnumerateAllConnectors (
     OUT   EXT_DISPLAY_PATH            *DisplayPathList,
  IN OUT   PCIe_PLATFORM_CONFIG        *Pcie,
  IN       GFX_PLATFORM_CONFIG         *Gfx
  )
{
  AGESA_STATUS  AgesaStatus;
  AGESA_STATUS  Status;
  AgesaStatus = AGESA_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxIntegratedEnumerateAllConnectors Enter\n");
  Status = GfxIntegratedEnumConnectorsForDevice (
             DEVICE_DFP,
             DisplayPathList,
             Pcie,
             Gfx
             );
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  Status = GfxIntegratedEnumConnectorsForDevice (
             DEVICE_CRT,
             DisplayPathList,
             Pcie,
             Gfx
             );
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  Status = GfxIntegratedEnumConnectorsForDevice (
             DEVICE_LCD,
             DisplayPathList,
             Pcie,
             Gfx
             );
  AGESA_STATUS_UPDATE (Status, AgesaStatus);
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxIntegratedEnumerateAllConnectors Exit [0x%x]\n", Status);
  return AgesaStatus;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Enumerate all display connectors for specific display device type.
 *
 *
 *
 * @param[in]     Engine  Engine configuration info
 * @param[in,out] Buffer  Buffer pointer
 * @param[in]     Pcie    PCIe configuration info
 */
VOID
STATIC
GfxIntegratedDdiInterfaceCallback (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  CONNECTOR_ENUM_INFO     *ConnectorEnumInfo;
  EXT_CONNECTOR_INFO      *ExtConnectorInfo;
  ConnectorEnumInfo = (CONNECTOR_ENUM_INFO*) Buffer;
  ExtConnectorInfo = GfxIntegratedExtConnectorInfo (Engine->Type.Ddi.DdiData.ConnectorType);
  if (ExtConnectorInfo == NULL) {
    AGESA_STATUS_UPDATE (AGESA_ERROR, ConnectorEnumInfo->Status);
    PcieConfigDisableEngine (Engine);
    return;
  }
  if (ExtConnectorInfo->DisplayDeviceEnum != ConnectorEnumInfo->DisplayDeviceEnum) {
    //Not device type we are looking for
    return;
  }
  if (Engine->Type.Ddi.DisplayPriorityIndex >=  ConnectorEnumInfo->RequestedPriorityIndex &&
      Engine->Type.Ddi.DisplayPriorityIndex <  ConnectorEnumInfo->CurrentPriorityIndex) {
    ConnectorEnumInfo->CurrentPriorityIndex = Engine->Type.Ddi.DisplayPriorityIndex;
    ConnectorEnumInfo->Engine = Engine;
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Enumerate all display connectors for specific display device type.
 *
 *
 *
 * @param[in]     DisplayDeviceEnum Display device list
 * @param[out]    DisplayPathList   Display path list
 * @param[in,out] Pcie              PCIe configuration info
 * @param[in]     Gfx               Gfx configuration info
 */
AGESA_STATUS
GfxIntegratedEnumConnectorsForDevice (
  IN       UINT8                       DisplayDeviceEnum,
     OUT   EXT_DISPLAY_PATH            *DisplayPathList,
  IN OUT   PCIe_PLATFORM_CONFIG        *Pcie,
  IN       GFX_PLATFORM_CONFIG         *Gfx
  )
{
  UINT8                   DisplayDeviceIndex;
  CONNECTOR_ENUM_INFO     ConnectorEnumInfo;
  EXT_CONNECTOR_INFO      *ExtConnectorInfo;
  EXT_DISPLAY_DEVICE_INFO *ExtDisplayDeviceInfo;
  AGESA_STATUS            Status;
  UINT8                   ConnectorIdArray[sizeof (ConnectorNumerArray)];
  ConnectorEnumInfo.Status = AGESA_SUCCESS;
  DisplayDeviceIndex = 1;
  ConnectorEnumInfo.RequestedPriorityIndex = 0;
  ConnectorEnumInfo.DisplayDeviceEnum = DisplayDeviceEnum;
  LibAmdMemFill (ConnectorIdArray, 0x00, sizeof (ConnectorIdArray), GnbLibGetHeader (Gfx));
  do {
    ConnectorEnumInfo.Engine = NULL;
    ConnectorEnumInfo.CurrentPriorityIndex = 0xff;
    PcieConfigRunProcForAllEngines (
      DESCRIPTOR_ALLOCATED | DESCRIPTOR_VIRTUAL | DESCRIPTOR_DDI_ENGINE,
      GfxIntegratedDdiInterfaceCallback,
      &ConnectorEnumInfo,
      Pcie
      );
    if (ConnectorEnumInfo.Engine == NULL) {
      break;                                  // No more connector support this
    }
    ConnectorEnumInfo.RequestedPriorityIndex = ConnectorEnumInfo.CurrentPriorityIndex + 1;
    ExtConnectorInfo = GfxIntegratedExtConnectorInfo (ConnectorEnumInfo.Engine->Type.Ddi.DdiData.ConnectorType);
    ASSERT (ExtConnectorInfo != NULL);
    ASSERT (ExtConnectorInfo->ConnectorIndex < sizeof (ConnectorIdArray));
    if (ConnectorIdArray[ExtConnectorInfo->ConnectorIndex] >=  ConnectorNumerArray[ExtConnectorInfo->ConnectorIndex]) {
      //Run out of supported connectors
      AGESA_STATUS_UPDATE (AGESA_ERROR, ConnectorEnumInfo.Status);
      PcieConfigDisableEngine (ConnectorEnumInfo.Engine);
      continue;
    }
    ConnectorEnumInfo.Engine->Type.Ddi.ConnectorId = ConnectorIdArray[ExtConnectorInfo->ConnectorIndex] + 1;
    ExtDisplayDeviceInfo = GfxIntegratedExtDisplayDeviceInfo (DisplayDeviceEnum, DisplayDeviceIndex);
    if (ExtDisplayDeviceInfo == NULL) {
      //Run out of supported display device types
      AGESA_STATUS_UPDATE (AGESA_ERROR, ConnectorEnumInfo.Status);
      Status = AGESA_ERROR;
      PcieConfigDisableEngine (ConnectorEnumInfo.Engine);
    }

    if ((Gfx->Gnb3dStereoPinIndex != 0) && (ConnectorEnumInfo.Engine->Type.Ddi.DdiData.HdpIndex == (Gfx->Gnb3dStereoPinIndex - 1))) {
      AGESA_STATUS_UPDATE (AGESA_ERROR, ConnectorEnumInfo.Status);
      Status = AGESA_ERROR;
      PcieConfigDisableEngine (ConnectorEnumInfo.Engine);
    }

    ConnectorEnumInfo.Engine->Type.Ddi.DisplayDeviceId = DisplayDeviceIndex;

    Status = GfxFmMapEngineToDisplayPath (ConnectorEnumInfo.Engine, DisplayPathList, Gfx);
    AGESA_STATUS_UPDATE (Status, ConnectorEnumInfo.Status);
    if (Status != AGESA_SUCCESS) {
      continue;
    }
    ConnectorIdArray[ExtConnectorInfo->ConnectorIndex]++;
    DisplayDeviceIndex++;
  } while (ConnectorEnumInfo.Engine != NULL);
  return ConnectorEnumInfo.Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Initialize display path for given engine
 *
 *
 *
 * @param[in]  Engine            Engine configuration info
 * @param[out] DisplayPath       Display path list
 * @param[out] SecondaryDisplayPath Secondary display path list
 * @param[in] Gfx                Gfx configuration info
 */

VOID
GfxIntegratedCopyDisplayInfo (
  IN       PCIe_ENGINE_CONFIG          *Engine,
     OUT   EXT_DISPLAY_PATH            *DisplayPath,
     OUT   EXT_DISPLAY_PATH            *SecondaryDisplayPath,
  IN       GFX_PLATFORM_CONFIG         *Gfx
  )
{
  EXT_CONNECTOR_INFO      *ExtConnectorInfo;
  EXT_DISPLAY_DEVICE_INFO *ExtDisplayDeviceInfo;
  ExtConnectorInfo = GfxIntegratedExtConnectorInfo (Engine->Type.Ddi.DdiData.ConnectorType);
  ExtDisplayDeviceInfo = GfxIntegratedExtDisplayDeviceInfo (ExtConnectorInfo->DisplayDeviceEnum, Engine->Type.Ddi.DisplayDeviceId);
  DisplayPath->usDeviceConnector = ExtConnectorInfo->ConnectorEnum | (Engine->Type.Ddi.ConnectorId << 8);
  DisplayPath->usDeviceTag = ExtDisplayDeviceInfo->DeviceTag;
  DisplayPath->usDeviceACPIEnum = ExtDisplayDeviceInfo->DeviceAcpiEnum;
  DisplayPath->ucExtAUXDDCLutIndex = Engine->Type.Ddi.DdiData.AuxIndex;
  DisplayPath->ucExtHPDPINLutIndex = Engine->Type.Ddi.DdiData.HdpIndex;
  DisplayPath->ucChPNInvert = Engine->Type.Ddi.DdiData.LanePnInversionMask;
  DisplayPath->usCaps = Engine->Type.Ddi.DdiData.Flags;
  DisplayPath->usExtEncoderObjId = ExtConnectorInfo->EncoderEnum;
  if (Engine->Type.Ddi.DdiData.Mapping[0].ChannelMappingValue == 0) {
    DisplayPath->ChannelMapping.ucChannelMapping = (Engine->EngineData.StartLane < Engine->EngineData.EndLane) ? 0xE4 : 0x1B;
  } else {
    DisplayPath->ChannelMapping.ucChannelMapping = Engine->Type.Ddi.DdiData.Mapping[0].ChannelMappingValue;
  }
  GNB_DEBUG_CODE (
    GfxIntegratedDebugDumpDisplayPath (DisplayPath, Gfx);
    );
  if (Engine->Type.Ddi.DdiData.ConnectorType == ConnectorTypeDualLinkDVI) {
    if (SecondaryDisplayPath != NULL) {
      SecondaryDisplayPath->usDeviceConnector = DisplayPath->usDeviceConnector;
    }
    GNB_DEBUG_CODE (
      GfxIntegratedDebugDumpDisplayPath (DisplayPath, Gfx);
    );

    if (Engine->Type.Ddi.DdiData.Mapping[1].ChannelMappingValue == 0) {
      DisplayPath->ChannelMapping.ucChannelMapping = (Engine->EngineData.StartLane < Engine->EngineData.EndLane) ? 0xE4 : 0x1B;
    } else {
      DisplayPath->ChannelMapping.ucChannelMapping = Engine->Type.Ddi.DdiData.Mapping[1].ChannelMappingValue;
    }
  }
}



/*----------------------------------------------------------------------------------------*/
/**
 * Dump  display path settings
 *
 *
 *
 * @param[in] DisplayPath            Display path
 * @param[in] Gfx                    Gfx configuration
 */

VOID
GfxIntegratedDebugDumpDisplayPath (
  IN       EXT_DISPLAY_PATH            *DisplayPath,
  IN       GFX_PLATFORM_CONFIG         *Gfx
  )
{
 IDS_HDT_CONSOLE (GFX_MISC, "     usDeviceConnector = 0x%x\n",
   DisplayPath->usDeviceConnector
   );
 IDS_HDT_CONSOLE (GFX_MISC, "     usDeviceTag = 0x%x\n",
   DisplayPath->usDeviceTag
   );
 IDS_HDT_CONSOLE (GFX_MISC, "     usDeviceACPIEnum = 0x%x\n",
   DisplayPath->usDeviceACPIEnum
   );
 IDS_HDT_CONSOLE (GFX_MISC, "     usExtEncoderObjId = 0x%x\n",
   DisplayPath->usExtEncoderObjId
   );
 IDS_HDT_CONSOLE (GFX_MISC, "     ucChannelMapping = 0x%x\n",
   DisplayPath->ChannelMapping.ucChannelMapping
   );
 IDS_HDT_CONSOLE (GFX_MISC, "     ucChPNInvert = 0x%x\n",
   DisplayPath->ucChPNInvert
   );
 IDS_HDT_CONSOLE (GFX_MISC, "     usCaps = 0x%x\n",
   DisplayPath->usCaps
   );
}
