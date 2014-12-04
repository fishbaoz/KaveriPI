/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM OEM tables, and callback function.
 *
 * Contains code that defines OEM tables and callback function to override
 * OEM table on run time.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  OEM
 * @e \$Revision: 284517 $   @e \$Date: 2014-02-11 15:57:17 -0600 (Tue, 11 Feb 2014) $
 *
 */
/*****************************************************************************
 *
 * Copyright 2012 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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
 ******************************************************************************
 */

#include <AmdCpmPei.h>
#include <AmdCpmOem.h>
#include "AmdCpmOemInitPeim.h"


//
// OEM CPM Table Definition
//
#include "GpioSettingsTable.h"
#include "SsidTable.h"
#include "GeventSettingsTable.h"
#include "MemoryVoltageSettings.h"
#include "GpioDevicesTable.h"
#include "PcieClocksTable.h"
#include "DisplayFeaturesTable.h"
#include "ThermalFanControlTable.h"
#include "AdaptiveS4.h"

#define PCIE_DDI_ENABLE_PIN             50   // GPIO50
#define PCIE_DDI_ENABLE_VALUE           GPIO_VALUE(GPIO_FUNCTION_2, GPIO_OUTPUT_HIGH, GPIO_PU_PD_DIS, GPIO_STICKY_DIS)
#define PCIE_DDI_ENABLE_VALUE_SMB       GPIO_VALUE(GPIO_FUNCTION_2, GPIO_OUTPUT_HIGH, GPIO_PU_EN, GPIO_STICKY_DIS)
//#define PCIE_DDI_ENABLE_VALUE_SMB       GPIO_VALUE(GPIO_FUNCTION_2, GPIO_INPUT, GPIO_PU_EN, GPIO_STICKY_EN)
#define PCIE_DDI_ENABLE_VALUE_DET       GPIO_VALUE(GPIO_FUNCTION_2, GPIO_INPUT, GPIO_PD_EN, GPIO_STICKY_EN)

//
// Platform Id Table: Get Board Id from GPIO pins
//
AMD_CPM_PLATFORM_ID_TABLE           gCpmPlatformIdTable = {
  {CPM_SIGNATURE_GET_PLATFORM_ID, sizeof (gCpmPlatformIdTable) / sizeof (UINT8), 0, 0, 0, 1},
  {
    14,                         // BOARD_ID0, ID[2:0]: 001-RevA, 010-RevB, ...
    15,                         // BOARD_ID1
    16,                         // BOARD_ID2
    17,                         // BOARD_ID3, 0: DAP, 1: non-DAP
    18,                         // BOARD_ID4, ID[5:4]: 00-SKU0, 01-SKU1
    19,                         // BOARD_ID5
    0xFF
  }
};

//
// Convert Table from Board Id to Platform Id
//
AMD_CPM_PLATFORM_ID_CONVERT_TABLE   gCpmPlatformIdConvertTable = {
  {CPM_SIGNATURE_GET_PLATFORM_ID_CONVERT, sizeof (gCpmPlatformIdConvertTable) / sizeof (UINT8), 0, 0, 0, 1},
  {
    {CPM_CPU_REVISION_ID_KV,  0x0000, 0x0000,    0x0000}, // Board Id -> Platform Id
    {CPM_CPU_REVISION_ID_TN,  0x0000, 0x0000,    0x0001}, // Board Id -> Platform Id
    {CPM_CPU_REVISION_ID_CZ,  0x0000, 0x0000,    0x0002}, // Board Id -> Platform Id
    0xFFFF,
  }
};

//
// Pre-Init Table
//
AMD_CPM_PRE_INIT_TABLE              gCpmPreInitTable = {
  {CPM_SIGNATURE_PRE_INIT, sizeof (gCpmPreInitTable) / sizeof (UINT8), 0, 0, 0, 0x01},
  {
    {   SB_MMIO_ACCESS,     0x03,   0xEA,   0xFE,   0x01    },          // PMIO2_RegEA[0]: PCIDisable = 1
    {   SB_MMIO_ACCESS,     0x03,   0x2E,   0xF9,   0x00    },          // PMIO2_Reg2E[2:1]: Smbus0Sel = 0
//  {   SB_MMIO_ACCESS,     0x03,   0x3C,   0x7F,   0x80    },          // PMIO2_Reg3C[7]: SmartVoltEnable = 1
    {   SB_MMIO_ACCESS,     0x03,   0xBE,   0xED,   0x12    },          // PMIO2_RegBE[1,4]: Enable KbRst
    {   SB_MMIO_ACCESS,     0x03,   0xC8,   0xBF,   0x40    },          // PMIO2_RegC8[8]: Enable TALERT# pin
    {   PCI_REG_ACCESS,     0xA3,   0x4A,   0xDF,   0x20    },          // LPC Reg4A[5]: Port enable for IO port 80h
    {   PCI_REG_ACCESS,     0xA3,   0x78,   0xF7,   0x00    },          // LPC Reg78[3]: Disable LDRQ1#
    {   EXT_PCI_REG_ACCESS, 0xC3,   0xE4,   0x8F,   0x00    },          // APU_MISC Reg1E4[6:4] = 0
    {   PCI_REG_ACCESS,     0xC3,   0xA4,   0x00,   0xEF    },          // APU_MISC RegA4 = 0xEF
    {   PCI_REG_ACCESS,     0xC3,   0xA5,   0x00,   0x0F    },          // APU_MISC RegA5 = 0x0F
    0xFF,
  }
};

//
// Save Context Table
//
AMD_CPM_SAVE_CONTEXT_TABLE           gCpmSaveContextTable = {
  {CPM_SIGNATURE_SAVE_CONTEXT, sizeof (gCpmSaveContextTable) / sizeof (UINT8), 0, 0, 0, 0x01},
  0x05,               // BufferType
  0x50,               // BufferOffset
  0x10,               // BufferSize
};

// OEM CPM Table definition end

//
// Convert Table between Register Value and DDI Card Id
//
UINT8   DdiCard[] = {
  DDI_CARD_TYPE_DP,       ConnectorTypeDP,
  DDI_CARD_TYPE_DVI,      ConnectorTypeSingleLinkDVI,
  DDI_CARD_TYPE_HDMI,     ConnectorTypeHDMI,
  DDI_CARD_TYPE_DL_DVI,   ConnectorTypeDualLinkDVI,
  DDI_CARD_TYPE_EDP,      ConnectorTypeEDP,
  DDI_CARD_TYPE_LVDS,     ConnectorTypeDpToLvds,
  DDI_CARD_TYPE_EDP_LVDS, ConnectorTypeEDPToLvdsSwInit,
  DDI_CARD_TYPE_AUTO,     ConnectorTypeDpToLvds,
  DDI_CARD_TYPE_UNKNOWN,  INVALID_CONNECTOR_TYPE
};

/*----------------------------------------------------------------------------------------*/
/**
 * ConfigureDdiTo4444
 *
 * Description:
 *   This function is used to configure PCIE 1x16 to 4x4 PCIe:
 *     DP_0: GFX[15:12] <-> Aux3, Hdp3, GPIO192 - 1:DP, 0:HDMI
 *     DP_1: GFX[11:8]  <-> Aux4, Hdp4, GPIO197 - 1:DP, 0:HDMI
 *     DP_2: GFX[7:4]   <-> Aux5, Hdp5, GPIO198 - 1:DP, 0:HDMI
 *     DP_3: GFX[3:0]   <-> Not available
 * Parameters:
 *   @param[in]     This                         Pointer to AMD CPM Table PPI
 *   @param[in]     PcieTopologyTablePtr         Pointer to PCIe Topology Table
 *   @param[in]     PcieTopologyOverrideTablePtr Pointer to PCIe Topology Override Table
 *   @param[in]     ItemPtr                      pointer to AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_ITEM
 *   @param[in]     DdiConnectorType             pointer to DDI display port connector type
 *   @param[in]     DdiPortPresent               pointer to DDI display port present
 *   @param[in]     CardType                     card type
 *   @param[in]     GfxLinkCfg                   GFX Link Confioguration
 *
 * @retval        VOID
 *
 */
VOID
ConfigureDdiTo4444 (
  IN       AMD_CPM_TABLE_PPI                     *This,
  IN       AMD_CPM_PCIE_TOPOLOGY_TABLE           *PcieTopologyTablePtr,
  IN       AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_TABLE  *PcieTopologyOverrideTablePtr,
  IN       AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_ITEM   *ItemPtr,
  IN       UINT8                                 *DdiConnectorType,
  IN       UINT8                                 *DdiPortPresent,
  IN       UINT8                                 CardType,
  IN       UINT8                                 GfxLinkCfg
  )
{
  UINT8           Index;
  UINT8           OverRideFlag;

  if ((CardType == CARD_B962) && (*DdiPortPresent == 0)) {
    ItemPtr->Flag.Raw    = VALID_OVERRIDE + LANE_OVERRIDE_ENABLE + ENABLE_OVERRIDE_ENABLE;
    ItemPtr->Offset      = PCIE_PORT_GFX0_OFFSET;
    ItemPtr->Enable      = PciePortEngine;
    ItemPtr->StartLane   = (UINT8) PcieTopologyTablePtr->Port[PCIE_PORT_GFX1_OFFSET].EngineData.StartLane;
    ItemPtr->EndLane     = (UINT8) PcieTopologyTablePtr->Port[PCIE_PORT_GFX1_OFFSET].EngineData.EndLane;
    ItemPtr++;

    ItemPtr->Flag.Raw    = VALID_OVERRIDE + LANE_OVERRIDE_ENABLE + PORT_PRESENT_OVERRIDE_ENABLE + ENABLE_OVERRIDE_ENABLE;
    ItemPtr->Offset      = PCIE_PORT_GFX1_OFFSET;
    ItemPtr->Enable      = PciePortEngine;
    ItemPtr->StartLane   = (UINT8) PcieTopologyTablePtr->Port[PCIE_PORT_GFX0_OFFSET].EngineData.StartLane;
    ItemPtr->EndLane     = (UINT8) PcieTopologyTablePtr->Port[PCIE_PORT_GFX1_OFFSET].EngineData.StartLane + 1;
    ItemPtr->PortPresent = PortEnabled;
    ItemPtr++;
  } else {
    *DdiPortPresent &= 0x07;
    DdiConnectorType[3] = INVALID_CONNECTOR_TYPE;

    ItemPtr->Flag.Raw   = VALID_OVERRIDE + ENABLE_OVERRIDE_ENABLE;
    ItemPtr->Offset     = PCIE_PORT_GFX0_OFFSET;
    ItemPtr->Enable     = PcieUnusedEngine;
    ItemPtr++;

    if (GfxLinkCfg == GFX_X16) {
      ItemPtr->Flag.Raw   = VALID_OVERRIDE + ENABLE_OVERRIDE_ENABLE;
      ItemPtr->Offset     = PCIE_PORT_GFX1_OFFSET;
      ItemPtr->Enable     = PcieUnusedEngine;
      ItemPtr++;
    } else {
      *DdiPortPresent &= 0x03;
      DdiConnectorType[2] = INVALID_CONNECTOR_TYPE;
    }

    for (Index = 0; Index < 4; Index++) {
      OverRideFlag = VALID_OVERRIDE + IS_DDI_DESCRIPTOR + ENABLE_OVERRIDE_ENABLE;
      ItemPtr->Offset   = PCIE_GFX_DDI_PORT0_OFFSET + Index;
      if (DdiConnectorType[Index] == INVALID_CONNECTOR_TYPE) {
        ItemPtr->Enable = PcieUnusedEngine;
      } else {
        OverRideFlag    |= DDI_TYPE_OVERRIDE_ENABLE;
        ItemPtr->Enable  = PcieDdiEngine;
        ItemPtr->DdiType = DdiConnectorType[Index];
      }
      ItemPtr->Flag.Raw = OverRideFlag;
      ItemPtr++;
    }
  }

  ItemPtr->Flag.Raw = 0xFF;
}

/*----------------------------------------------------------------------------------------*/
/**
 * ConfigureDdiTo88
 *
 * Description:
 *   This function is used to configure PCIE 1x16 to 2x8 PCIe:
 *     DP_0: GFX[15:12] <-> Aux3, Hdp3, GPIO192 - 1:DP, 0:HDMI
 *     DP_1: GFX[11:8]  <-> Aux4, Hdp4, GPIO197 - 1:DP, 0:HDMI
 *     DP_2: GFX[7:4]   <-> Aux5, Hdp5, GPIO198 - 1:DP, 0:HDMI
 *     DP_3: GFX[3:0]   <-> Not available
 * Parameters:
 *   @param[in]     This                         Pointer to AMD CPM Table PPI
 *   @param[in]     PcieTopologyTablePtr         Pointer to PCIe Topology Table
 *   @param[in]     PcieTopologyOverrideTablePtr Pointer to PCIe Topology Override Table
 *   @param[in]     ItemPtr                      pointer to AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_ITEM
 *   @param[in]     DdiConnectorType             pointer to DDI display port connector type
 *   @param[in]     DdiPortPresent               pointer to DDI display port present
 *   @param[in]     CardType                     card type
 *   @param[in]     GfxLinkCfg                   GFX Link Confioguration
 *
 * @retval        VOID
 *
 */
VOID
ConfigureDdiTo88 (
  IN       AMD_CPM_TABLE_PPI                     *This,
  IN       AMD_CPM_PCIE_TOPOLOGY_TABLE           *PcieTopologyTablePtr,
  IN       AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_TABLE  *PcieTopologyOverrideTablePtr,
  IN       AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_ITEM   *ItemPtr,
  IN       UINT8                                 *DdiConnectorType,
  IN       UINT8                                 *DdiPortPresent,
  IN       UINT8                                 CardType,
  IN       UINT8                                 GfxLinkCfg
  )
{

  UINT8           OverRideFlag;

  if ((CardType == CARD_B964) && (*DdiPortPresent == 0)) {
    ItemPtr->Flag.Raw    = VALID_OVERRIDE + LANE_OVERRIDE_ENABLE + ENABLE_OVERRIDE_ENABLE;
    ItemPtr->Offset      = PCIE_PORT_GFX0_OFFSET;
    ItemPtr->Enable      = PciePortEngine;
    ItemPtr->StartLane   = (UINT8) PcieTopologyTablePtr->Port[PCIE_PORT_GFX1_OFFSET].EngineData.StartLane;
    ItemPtr->EndLane     = (UINT8) PcieTopologyTablePtr->Port[PCIE_PORT_GFX1_OFFSET].EngineData.EndLane;
    ItemPtr++;

    ItemPtr->Flag.Raw    = VALID_OVERRIDE + LANE_OVERRIDE_ENABLE + PORT_PRESENT_OVERRIDE_ENABLE + ENABLE_OVERRIDE_ENABLE;
    ItemPtr->Offset      = PCIE_PORT_GFX1_OFFSET;
    ItemPtr->Enable      = PciePortEngine;
    ItemPtr->StartLane   = (UINT8) PcieTopologyTablePtr->Port[PCIE_PORT_GFX0_OFFSET].EngineData.StartLane;
    ItemPtr->EndLane     = (UINT8) PcieTopologyTablePtr->Port[PCIE_PORT_GFX1_OFFSET].EngineData.StartLane + 1;
    ItemPtr->PortPresent = PortEnabled;
    ItemPtr++;
  } else {
    *DdiPortPresent &= 0x05;
    DdiConnectorType[1] = INVALID_CONNECTOR_TYPE;
    DdiConnectorType[3] = INVALID_CONNECTOR_TYPE;

    ItemPtr->Flag.Raw   = VALID_OVERRIDE + ENABLE_OVERRIDE_ENABLE;
    ItemPtr->Offset     = PCIE_PORT_GFX0_OFFSET;
    ItemPtr->Enable     = PcieUnusedEngine;
    ItemPtr++;

    if (GfxLinkCfg == GFX_X16) {
      ItemPtr->Flag.Raw   = VALID_OVERRIDE + ENABLE_OVERRIDE_ENABLE;
      ItemPtr->Offset     = PCIE_PORT_GFX1_OFFSET;
      ItemPtr->Enable     = PcieUnusedEngine;
      ItemPtr++;
    } else {
      *DdiPortPresent &= 0x03;
      DdiConnectorType[2] = INVALID_CONNECTOR_TYPE;
    }

    // DP_0 + DP_1
    OverRideFlag = VALID_OVERRIDE + IS_DDI_DESCRIPTOR + ENABLE_OVERRIDE_ENABLE;
    ItemPtr->Offset   = PCIE_GFX_DDI_PORT0_OFFSET + 0;
    if (DdiConnectorType[0] == INVALID_CONNECTOR_TYPE) {
      ItemPtr->Enable = PcieUnusedEngine;
    } else {
      OverRideFlag    |= DDI_TYPE_OVERRIDE_ENABLE;
      ItemPtr->Enable  = PcieDdiEngine;
      ItemPtr->DdiType = DdiConnectorType[0];
      if (DdiConnectorType[0] == ConnectorTypeDualLinkDVI) {
        OverRideFlag    |= LANE_OVERRIDE_ENABLE;
        ItemPtr->StartLane  = (UINT8) PcieTopologyTablePtr->Ddi[PCIE_GFX_DDI_PORT0_OFFSET + 0].EngineData.StartLane;
        ItemPtr->EndLane    = (UINT8) PcieTopologyTablePtr->Ddi[PCIE_GFX_DDI_PORT0_OFFSET + 1].EngineData.EndLane;
      }
    }
    ItemPtr->Flag.Raw = OverRideFlag;
    ItemPtr ++;

    // DP_2 + DP_3
    OverRideFlag = VALID_OVERRIDE + IS_DDI_DESCRIPTOR + ENABLE_OVERRIDE_ENABLE;
    ItemPtr->Offset   = PCIE_GFX_DDI_PORT0_OFFSET + 2;
    if (DdiConnectorType[2] == INVALID_CONNECTOR_TYPE) {
      ItemPtr->Enable = PcieUnusedEngine;
    } else {
      OverRideFlag    |= DDI_TYPE_OVERRIDE_ENABLE;
      ItemPtr->Enable  = PcieDdiEngine;
      ItemPtr->DdiType = DdiConnectorType[2];
      if (DdiConnectorType[2] == ConnectorTypeDualLinkDVI) {
        OverRideFlag    |= LANE_OVERRIDE_ENABLE;
        ItemPtr->StartLane  = (UINT8) PcieTopologyTablePtr->Ddi[PCIE_GFX_DDI_PORT0_OFFSET + 2].EngineData.StartLane;
        ItemPtr->EndLane    = (UINT8) PcieTopologyTablePtr->Ddi[PCIE_GFX_DDI_PORT0_OFFSET + 3].EngineData.EndLane;
      }
    }
    ItemPtr->Flag.Raw = OverRideFlag;
    ItemPtr++;
  }

  ItemPtr->Flag.Raw = 0xFF;
}

/*----------------------------------------------------------------------------------------*/
/**
 * ConfigureDdiTo448
 *
 * Description:
 *   This function is used to configure PCIE 1x16 to upper x8 PCIe + 2x4 PCIe:
 *     DP_0: GFX[15:12] <-> Aux3, Hdp3, GPIO192 - 1:DP, 0:HDMI
 *     DP_1: GFX[11:8]  <-> Aux4, Hdp4, GPIO197 - 1:DP, 0:HDMI
 *     DP_2: GFX[7:4]   <-> Aux5, Hdp5, GPIO198 - 1:DP, 0:HDMI
 *     DP_3: GFX[3:0]   <-> Not available
 * Parameters:
 *   @param[in]     This                         Pointer to AMD CPM Table PPI
 *   @param[in]     PcieTopologyTablePtr         Pointer to PCIe Topology Table
 *   @param[in]     PcieTopologyOverrideTablePtr Pointer to PCIe Topology Override Table
 *   @param[in]     ItemPtr                      pointer to AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_ITEM
 *   @param[in]     DdiConnectorType             pointer to DDI display port connector type
 *   @param[in]     DdiPortPresent               pointer to DDI display port present
 *   @param[in]     CardType                     card type
 *   @param[in]     GfxLinkCfg                   GFX Link Confioguration
 *
 * @retval        VOID
 *
 */
VOID
ConfigureDdiTo448 (
  IN       AMD_CPM_TABLE_PPI                     *This,
  IN       AMD_CPM_PCIE_TOPOLOGY_TABLE           *PcieTopologyTablePtr,
  IN       AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_TABLE  *PcieTopologyOverrideTablePtr,
  IN       AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_ITEM   *ItemPtr,
  IN       UINT8                                 *DdiConnectorType,
  IN       UINT8                                 *DdiPortPresent,
  IN       UINT8                                 CardType,
  IN       UINT8                                 GfxLinkCfg
  )
{
  UINT8             Index;
  UINT8             OverRideFlag;

  if ((CardType == CARD_B966) && (*DdiPortPresent == 0)) {
    ItemPtr->Flag.Raw    = VALID_OVERRIDE + LANE_OVERRIDE_ENABLE + ENABLE_OVERRIDE_ENABLE;
    ItemPtr->Offset      = PCIE_PORT_GFX0_OFFSET;
    ItemPtr->Enable      = PciePortEngine;
    ItemPtr->StartLane   = (UINT8) PcieTopologyTablePtr->Port[PCIE_PORT_GFX1_OFFSET].EngineData.StartLane;
    ItemPtr->EndLane     = (UINT8) PcieTopologyTablePtr->Port[PCIE_PORT_GFX1_OFFSET].EngineData.EndLane;
    ItemPtr++;

    ItemPtr->Flag.Raw    = VALID_OVERRIDE + LANE_OVERRIDE_ENABLE + PORT_PRESENT_OVERRIDE_ENABLE + ENABLE_OVERRIDE_ENABLE;
    ItemPtr->Offset      = PCIE_PORT_GFX1_OFFSET;
    ItemPtr->Enable      = PciePortEngine;
    ItemPtr->StartLane   = (UINT8) PcieTopologyTablePtr->Port[PCIE_PORT_GFX0_OFFSET].EngineData.StartLane;
    ItemPtr->EndLane     = (UINT8) PcieTopologyTablePtr->Port[PCIE_PORT_GFX1_OFFSET].EngineData.StartLane + 1;
    ItemPtr->PortPresent = PortEnabled;
    ItemPtr++;
  } else {
    *DdiPortPresent &= 0x07;
    DdiConnectorType[3] = INVALID_CONNECTOR_TYPE;

    // To filter invalid configuration
    // Default set to DL-DVI:DDI:DDI
    if (*DdiPortPresent == 0x00) {
      *DdiPortPresent = 0x07;
    }

    ItemPtr->Flag.Raw   = VALID_OVERRIDE + ENABLE_OVERRIDE_ENABLE;
    ItemPtr->Offset     = PCIE_PORT_GFX0_OFFSET;
    ItemPtr->Enable     = PcieUnusedEngine;
    ItemPtr++;

    if (GfxLinkCfg == GFX_X16) {
      ItemPtr->Flag.Raw   = VALID_OVERRIDE + ENABLE_OVERRIDE_ENABLE;
      ItemPtr->Offset     = PCIE_PORT_GFX1_OFFSET;
      ItemPtr->Enable     = PcieUnusedEngine;
      ItemPtr++;
    } else {
      *DdiPortPresent &= 0x03;
      DdiConnectorType[2] = INVALID_CONNECTOR_TYPE;
    }

    OverRideFlag = VALID_OVERRIDE + IS_DDI_DESCRIPTOR + ENABLE_OVERRIDE_ENABLE;
    ItemPtr->Offset   = PCIE_GFX_DDI_PORT0_OFFSET + 2;
    if (DdiConnectorType[2] == INVALID_CONNECTOR_TYPE) {
      ItemPtr->Enable = PcieUnusedEngine;
    } else {
      OverRideFlag    |= DDI_TYPE_OVERRIDE_ENABLE;
      ItemPtr->Enable  = PcieDdiEngine;
      ItemPtr->DdiType = DdiConnectorType[2];
      if (DdiConnectorType[2] == ConnectorTypeDualLinkDVI) {
        OverRideFlag    |= LANE_OVERRIDE_ENABLE;
        ItemPtr->StartLane  = (UINT8) PcieTopologyTablePtr->Ddi[PCIE_GFX_DDI_PORT0_OFFSET + 2].EngineData.StartLane;
        ItemPtr->EndLane    = (UINT8) PcieTopologyTablePtr->Ddi[PCIE_GFX_DDI_PORT0_OFFSET + 3].EngineData.EndLane;
      }
    }
    ItemPtr->Flag.Raw = OverRideFlag;
    ItemPtr++;

    for (Index = 0; Index < 2; Index++) {
      OverRideFlag = VALID_OVERRIDE + IS_DDI_DESCRIPTOR + ENABLE_OVERRIDE_ENABLE;
      ItemPtr->Offset       = PCIE_GFX_DDI_PORT0_OFFSET + Index;
      if (DdiConnectorType[Index] == INVALID_CONNECTOR_TYPE) {
        ItemPtr->Enable     = PcieUnusedEngine;
      } else {
        OverRideFlag       |= DDI_TYPE_OVERRIDE_ENABLE;
        ItemPtr->Enable     = PcieDdiEngine;
        ItemPtr->DdiType    = DdiConnectorType[Index];
      }
      ItemPtr->Flag.Raw     = OverRideFlag;
      ItemPtr++;
    }
  }

  ItemPtr->Flag.Raw = 0xFF;
}

/*----------------------------------------------------------------------------------------*/
/**
 * ConfigureDdiTo844
 *
 * Description:
 *   This function is used to configure PCIE 1x16 to lower x8 PCIe + 2x4 PCIe:
 *     DP_0: GFX[15:12] <-> Aux3, Hdp3, GPIO192 - 1:DP, 0:HDMI
 *     DP_1: GFX[11:8]  <-> Aux4, Hdp4, GPIO197 - 1:DP, 0:HDMI
 *     DP_2: GFX[7:4]   <-> Aux5, Hdp5, GPIO198 - 1:DP, 0:HDMI
 *     DP_3: GFX[3:0]   <-> Not available
 * Parameters:
 *   @param[in]     This                         Pointer to AMD CPM Table PPI
 *   @param[in]     PcieTopologyTablePtr         Pointer to PCIe Topology Override Table
 *   @param[in]     PcieTopologyOverrideTablePtr Pointer to PCIe Topology Override Table
 *   @param[in]     ItemPtr                      pointer to AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_ITEM
 *   @param[in]     DdiConnectorType             pointer to DDI display port connector type
 *   @param[in]     DdiPortPresent               pointer to DDI display port present
 *   @param[in]     CardType                     card type
 *   @param[in]     GfxLinkCfg                   GFX Link Confioguration
 *
 * @retval        VOID
 *
 */
VOID
ConfigureDdiTo844 (
  IN       AMD_CPM_TABLE_PPI                     *This,
  IN       AMD_CPM_PCIE_TOPOLOGY_TABLE           *PcieTopologyTablePtr,
  IN       AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_TABLE  *PcieTopologyOverrideTablePtr,
  IN       AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_ITEM   *ItemPtr,
  IN       UINT8                                 *DdiConnectorType,
  IN       UINT8                                 *DdiPortPresent,
  IN       UINT8                                 CardType,
  IN       UINT8                                 GfxLinkCfg
  )
{
  UINT8             Index;
  UINT8             OverRideFlag;

  if ((CardType == CARD_B967) && (*DdiPortPresent == 0)) {
    ItemPtr->Flag.Raw    = VALID_OVERRIDE + LANE_OVERRIDE_ENABLE + ENABLE_OVERRIDE_ENABLE;
    ItemPtr->Offset      = PCIE_PORT_GFX0_OFFSET;
    ItemPtr->Enable      = PciePortEngine;
    ItemPtr->StartLane   = (UINT8) PcieTopologyTablePtr->Port[PCIE_PORT_GFX1_OFFSET].EngineData.StartLane;
    ItemPtr->EndLane     = (UINT8) PcieTopologyTablePtr->Port[PCIE_PORT_GFX1_OFFSET].EngineData.EndLane;
    ItemPtr++;

    ItemPtr->Flag.Raw    = VALID_OVERRIDE + LANE_OVERRIDE_ENABLE + PORT_PRESENT_OVERRIDE_ENABLE + ENABLE_OVERRIDE_ENABLE;
    ItemPtr->Offset      = PCIE_PORT_GFX1_OFFSET;
    ItemPtr->Enable      = PciePortEngine;
    ItemPtr->StartLane   = (UINT8) PcieTopologyTablePtr->Port[PCIE_PORT_GFX0_OFFSET].EngineData.StartLane;
    ItemPtr->EndLane     = (UINT8) PcieTopologyTablePtr->Port[PCIE_PORT_GFX1_OFFSET].EngineData.StartLane + 1;
    ItemPtr->PortPresent = PortEnabled;
    ItemPtr++;
  } else {
    *DdiPortPresent &= 0x0D;
    DdiConnectorType[1] = INVALID_CONNECTOR_TYPE;

    ItemPtr->Flag.Raw   = VALID_OVERRIDE + ENABLE_OVERRIDE_ENABLE;
    ItemPtr->Offset     = PCIE_PORT_GFX0_OFFSET;
    ItemPtr->Enable     = PcieUnusedEngine;
    ItemPtr++;

    if (GfxLinkCfg == GFX_X16) {
      ItemPtr->Flag.Raw   = VALID_OVERRIDE + ENABLE_OVERRIDE_ENABLE;
      ItemPtr->Offset     = PCIE_PORT_GFX1_OFFSET;
      ItemPtr->Enable     = PcieUnusedEngine;
      ItemPtr++;
    } else {
      *DdiPortPresent &= 0x0C;
      DdiConnectorType[0] = INVALID_CONNECTOR_TYPE;
    }

    // To filter invalid configuration, set default to x4 DDI & DL-DVI
    if ((*DdiPortPresent != 1) && (*DdiPortPresent != 4) && (*DdiPortPresent != 5)) {
      *DdiPortPresent &= 0x05;
      DdiPortPresent[3] = INVALID_CONNECTOR_TYPE;
    }

    OverRideFlag = VALID_OVERRIDE + IS_DDI_DESCRIPTOR + ENABLE_OVERRIDE_ENABLE;
    ItemPtr->Offset   = PCIE_GFX_DDI_PORT0_OFFSET + 0;
    if (DdiConnectorType[0] == INVALID_CONNECTOR_TYPE) {
      ItemPtr->Enable = PcieUnusedEngine;
    } else {
      OverRideFlag    |= DDI_TYPE_OVERRIDE_ENABLE;
      ItemPtr->Enable  = PcieDdiEngine;
      ItemPtr->DdiType = DdiConnectorType[0];
      if (DdiConnectorType[0] == ConnectorTypeDualLinkDVI) {
        OverRideFlag    |= LANE_OVERRIDE_ENABLE;
        ItemPtr->StartLane  = (UINT8) PcieTopologyTablePtr->Ddi[PCIE_GFX_DDI_PORT0_OFFSET + 0].EngineData.StartLane;
        ItemPtr->EndLane    = (UINT8) PcieTopologyTablePtr->Ddi[PCIE_GFX_DDI_PORT0_OFFSET + 1].EngineData.EndLane;
      }
    }
    ItemPtr->Flag.Raw = OverRideFlag;
    ItemPtr++;

    for (Index = 2; Index < 4; Index++) {
      OverRideFlag = VALID_OVERRIDE + IS_DDI_DESCRIPTOR + ENABLE_OVERRIDE_ENABLE;
      ItemPtr->Offset       = PCIE_GFX_DDI_PORT0_OFFSET + Index;
      if (DdiConnectorType[Index] == INVALID_CONNECTOR_TYPE) {
        ItemPtr->Enable     = PcieUnusedEngine;
      } else {
        OverRideFlag       |= DDI_TYPE_OVERRIDE_ENABLE;
        ItemPtr->Enable     = PcieDdiEngine;
        ItemPtr->DdiType    = DdiConnectorType[Index];
      }
      ItemPtr->Flag.Raw     = OverRideFlag;
      ItemPtr++;
    }
  }

  ItemPtr->Flag.Raw = 0xFF;
}

/*----------------------------------------------------------------------------------------*/
/**
 * ConfigureDdiToC527
 *
 * Description:
 *   This function is used to configure PCIE 1x16 to 2x8 PCIe:
 *     DP_3: GFX[15:12] <-> Aux3, Hdp3
 *     DP_2: GFX[11:8]  <-> Not available
 *     DP_1: GFX[7:4]   <-> Aux5, Hdp5
 *     DP_0: GFX[3:0]   <-> Not available
 * Parameters:
 *   @param[in]     This                         Pointer to AMD CPM Table PPI
 *   @param[in]     PcieTopologyTablePtr         Pointer to PCIe Topology Table
 *   @param[in]     PcieTopologyOverrideTablePtr Pointer to PCIe Topology Override Table
 *   @param[in]     ItemPtr                      pointer to AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_ITEM
 *   @param[in]     DdiConnectorType             pointer to DDI display port connector type
 *   @param[in]     DdiPortPresent               pointer to DDI display port present
 *   @param[in]     CardType                     card type
 *   @param[in]     GfxLinkCfg                   GFX Link Confioguration
 *
 * @retval        VOID
 *
 */
VOID
ConfigureDdiToC527 (
  IN       AMD_CPM_TABLE_PPI                     *This,
  IN       AMD_CPM_PCIE_TOPOLOGY_TABLE           *PcieTopologyTablePtr,
  IN       AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_TABLE  *PcieTopologyOverrideTablePtr,
  IN       AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_ITEM   *ItemPtr,
  IN       UINT8                                 *DdiConnectorType,
  IN       UINT8                                 *DdiPortPresent,
  IN       UINT8                                 CardType,
  IN       UINT8                                 GfxLinkCfg
  )
{
  UINT8           Index;
  UINT8           OverRideFlag;

//  *DdiPortPresent &= 0x5;
  DdiConnectorType[0] = DdiConnectorType[1];
  DdiConnectorType[1] = DdiConnectorType[3];
  DdiConnectorType[3] = DdiConnectorType[0];
  DdiConnectorType[0] = INVALID_CONNECTOR_TYPE;
  DdiConnectorType[2] = INVALID_CONNECTOR_TYPE;

  ItemPtr->Flag.Raw   = VALID_OVERRIDE + ENABLE_OVERRIDE_ENABLE;
  ItemPtr->Offset     = PCIE_PORT_GFX0_OFFSET;
  ItemPtr->Enable     = PcieUnusedEngine;
  ItemPtr++;

  if (GfxLinkCfg == GFX_X16) {
    ItemPtr->Flag.Raw   = VALID_OVERRIDE + ENABLE_OVERRIDE_ENABLE;
    ItemPtr->Offset     = PCIE_PORT_GFX1_OFFSET;
    ItemPtr->Enable     = PcieUnusedEngine;
    ItemPtr++;
  } else {
    *DdiPortPresent &= 0x03;
    DdiConnectorType[3] = INVALID_CONNECTOR_TYPE;
  }

  for (Index = 0; Index < 4; Index++) {
    OverRideFlag = VALID_OVERRIDE + IS_DDI_DESCRIPTOR + ENABLE_OVERRIDE_ENABLE;
    ItemPtr->Offset   = PCIE_GFX_DDI_PORT0_OFFSET + Index;
    if (DdiConnectorType[3 - Index] == INVALID_CONNECTOR_TYPE) {
      ItemPtr->Enable = PcieUnusedEngine;
    } else {
      OverRideFlag    |= DDI_TYPE_OVERRIDE_ENABLE;
      ItemPtr->Enable  = PcieDdiEngine;
      ItemPtr->DdiType = DdiConnectorType[3 - Index];
      if (DdiConnectorType[3 - Index] == ConnectorTypeDualLinkDVI) {
        OverRideFlag    |= LANE_OVERRIDE_ENABLE;
        ItemPtr->StartLane  = (UINT8) PcieTopologyTablePtr->Ddi[PCIE_GFX_DDI_PORT0_OFFSET + Index].EngineData.StartLane;
        ItemPtr->EndLane    = (UINT8) PcieTopologyTablePtr->Ddi[PCIE_GFX_DDI_PORT0_OFFSET + Index + 1].EngineData.EndLane;
      }
    }
    ItemPtr->Flag.Raw = OverRideFlag;
    ItemPtr++;
  }

  ItemPtr->Flag.Raw = 0xFF;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get DDI Card Id
 *
 * This function reads register value of SMBUS device on DDI card and converts it to
 * pre-defined DDI Card Id.
 *
 * @param[in]     This           Pointer to AMD CPM Table PPI
 * @param[in]     Smbus          Smbus Select. 0: Smbus0. 1: Smbus1.
 * @param[in]     Address        Smbus Address
 *
 * @retval        DDI Card Id
 *
 */
UINT8
GetDdiCardId (
  IN       AMD_CPM_TABLE_PPI   *This,
  IN       UINT8               Smbus,
  IN       UINT8               Address
  )
{
  EFI_STATUS      Status;
  UINT8           Data;
  UINT8           index;
  UINT8           Value;

  Status = This->CommonFunction.ReadSmbus (This, Smbus, Address, 0, 1, &Data);
  Value = INVALID_CONNECTOR_TYPE;
  if (!EFI_ERROR (Status)) {
    for (index = 0; index < sizeof (DdiCard); index += 2) {
      if (DdiCard[index] == Data) {
        Value = DdiCard[index + 1];
        break;
      }
    }
  }
  return Value;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Update GPIO Device Config Table
 *
 * @param[in]     GpioDeviceConfigTablePtr  Pointer to GPIO Device Config Table
 * @param[in]     DeviceId                  Device Id to update
 * @param[in]     Mode                      Value. 0: Disable. 1: Enable
 *
 */
VOID
SetDevice (
  IN       AMD_CPM_GPIO_DEVICE_CONFIG_TABLE     *GpioDeviceConfigTablePtr,
  IN       UINT8                                DeviceId,
  IN       UINT8                                Mode
  )
{
  UINT8   Index;
  UINT8   GpioDeviceId;

  for (Index = 0; Index < AMD_GPIO_DEVICE_SIZE; Index ++) {
    GpioDeviceId = GpioDeviceConfigTablePtr->DeviceList[Index].DeviceId;
    if (GpioDeviceId == 0xFF || GpioDeviceId == 0x00) {
      break;
    }
    if (GpioDeviceId == DeviceId) {
      GpioDeviceConfigTablePtr->DeviceList[Index].Config.Setting.Enable = Mode;
      break;
    }
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Detect PCIe Devices Automatically
 *
 * This function patches PCIe Topology Override Table by detecting PCIe devices
 * automatically.
 *
 * @param[in]     AmdCpmTablePpi                Pointer to AMD CPM Table PPI
 * @param[in]     PcieTopologyTablePtr          Pointer to PCIe Topology Table
 * @param[in]     PcieTopologyOverrideTablePtr  Pointer to PCIe Topology Override Table
 * @param[in]     GpioDeviceConfigTablePtr      Pointer to GPIO Device Config Table
 * @param[in]     OemSetupOption                pointer to CPM_OEM_SETUP_OPTION
 *
 */
VOID
DetectPcieDevices (
  IN       AMD_CPM_TABLE_PPI                       *AmdCpmTablePpi,
  IN       AMD_CPM_PCIE_TOPOLOGY_TABLE             *PcieTopologyTablePtr,
  IN       AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_TABLE    *PcieTopologyOverrideTablePtr,
  IN       AMD_CPM_GPIO_DEVICE_CONFIG_TABLE        *GpioDeviceConfigTablePtr,
  IN       CPM_OEM_SETUP_OPTION                    *OemSetupOption
  )
{
  AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_ITEM     *ItemPtr;
  UINT8                                   CardType;
  UINTN                                   Index;
  UINT8                                   DdiConnectorType [4];
  UINT8                                   DdiPortPresent;
  UINT8                                   Data8;
  UINT16                                  DdiEnValue;
  UINT8                                   SmbusSlaveAddress [4] =
                                          {
                                            GFX_DP0_I2C_ADDRESS >> 1,
                                            GFX_DP1_I2C_ADDRESS >> 1,
                                            GFX_DP2_I2C_ADDRESS >> 1,
                                            GFX_DP3_I2C_ADDRESS >> 1,
                                          };
//  UINT8                                   DpHdmiDungle [4] =
//                                          {
//                                            DEVICE_ID_DUNGLE_DP4,
//                                            DEVICE_ID_DUNGLE_DP5,
//                                            DEVICE_ID_DUNGLE_DP3,
//                                            DEVICE_ID_DUNGLE_DP3,
//                                          };
  UINT8                                   GfxLinkCfg;

  DdiEnValue = PCIE_DDI_ENABLE_VALUE_DET;
  GfxLinkCfg = OemSetupOption->GfxLinkCfg;
  if (PcieTopologyOverrideTablePtr) {
    ItemPtr = &PcieTopologyOverrideTablePtr->Item[0];
    CardType = CARD_UNKNOWN;
    AmdCpmTablePpi->CommonFunction.DetectDevice (AmdCpmTablePpi, DEVICE_ID_PCIE_CARD, &CardType);

    if (GfxLinkCfg == GFX_2X8) {
      ItemPtr->Flag.Raw    = VALID_OVERRIDE + LANE_OVERRIDE_ENABLE + ENABLE_OVERRIDE_ENABLE;
      ItemPtr->Offset      = PCIE_PORT_GFX0_OFFSET;
      ItemPtr->Enable      = PciePortEngine;
      ItemPtr->StartLane   = (UINT8) PcieTopologyTablePtr->Port[PCIE_PORT_GFX0_OFFSET].EngineData.StartLane;
      ItemPtr->EndLane     = (UINT8) PcieTopologyTablePtr->Port[PCIE_PORT_GFX1_OFFSET].EngineData.StartLane + 1;
      ItemPtr++;

      ItemPtr->Flag.Raw    = VALID_OVERRIDE + PORT_PRESENT_OVERRIDE_ENABLE + ENABLE_OVERRIDE_ENABLE;
      ItemPtr->Offset      = PCIE_PORT_GFX1_OFFSET;
      ItemPtr->Enable      = PciePortEngine;
      ItemPtr->PortPresent = PortEnabled;
      ItemPtr++;
    }

    if ((AmdCpmTablePpi->CommonFunction.DetectDevice (AmdCpmTablePpi, DEVICE_ID_DAP, NULL))) {
      // Bantry DAP DDI DP2 is DL DVI
      ItemPtr->Flag.Raw   = VALID_OVERRIDE + IS_DDI_DESCRIPTOR + ENABLE_OVERRIDE_ENABLE;
      ItemPtr->Offset     = PCIE_DDI_PORT1_OFFSET;
      ItemPtr->Enable     = PcieUnusedEngine;
      ItemPtr++;

      ItemPtr->Flag.Raw   = VALID_OVERRIDE + IS_DDI_DESCRIPTOR + LANE_OVERRIDE_ENABLE + DDI_TYPE_OVERRIDE_ENABLE + ENABLE_OVERRIDE_ENABLE;
      ItemPtr->Offset     = PCIE_DDI_PORT2_OFFSET;
      ItemPtr->Enable     = PcieDdiEngine;
//      ItemPtr->StartLane  = (UINT8) PcieTopologyTablePtr->Ddi[PCIE_DDI_PORT1_OFFSET].EngineData.StartLane;
//      ItemPtr->EndLane    = (UINT8) PcieTopologyTablePtr->Ddi[PCIE_DDI_PORT2_OFFSET].EngineData.EndLane;
      ItemPtr->StartLane  = (UINT8) PcieTopologyTablePtr->Ddi[PCIE_DDI_PORT3_OFFSET].EngineData.StartLane;
      ItemPtr->EndLane    = (UINT8) PcieTopologyTablePtr->Ddi[PCIE_DDI_PORT3_OFFSET].EngineData.EndLane;
      ItemPtr->DdiType    = ConnectorTypeDualLinkDVI;
      ItemPtr++;
    } else {
      // Bantry: DDI DP2 is DP/HDMI => GPIO191 input 1:DP 0:HDMI, default is DP
//      if (AmdCpmTablePpi->CommonFunction.DetectDevice (AmdCpmTablePpi, DEVICE_ID_DUNGLE_DP2, NULL)) {
//        ItemPtr->Flag.Raw   = VALID_OVERRIDE + IS_DDI_DESCRIPTOR + DDI_TYPE_OVERRIDE_ENABLE + ENABLE_OVERRIDE_ENABLE;
//        ItemPtr->Offset     = PCIE_DDI_PORT2_OFFSET;
//        ItemPtr->Enable     = PcieDdiEngine;
//        ItemPtr->DdiType    = ConnectorTypeHDMI;
//        ItemPtr++;
//      }
    }

    DdiPortPresent = 0;
    Data8 = 1;
//    if ((OemSetupOption->PcieDdiDetection == 1) && (CardType > CARD_B967)) {
    if (OemSetupOption->PcieDdiDetection == 1) {
      // PCIE card auto-detection is enabled
      for (Index = 0; Index < 4; Index ++) {
        DdiConnectorType[Index] = GetDdiCardId (AmdCpmTablePpi, GFX_PCIE_SMBUS_NO, SmbusSlaveAddress[Index]);
        if (DdiConnectorType[Index] != INVALID_CONNECTOR_TYPE) {
          DdiPortPresent |= Data8;
        }
        Data8 = Data8 << 1;
      }

      if (DdiPortPresent) {
        if (CardType > CARD_B967) {
          DdiEnValue = PCIE_DDI_ENABLE_VALUE_SMB & 0xFFFE;
        }
//        if (((DdiPortPresent == 0xA) || (DdiPortPresent == 0x2) || (DdiPortPresent == 0x8)) && (DdiConnectorType[1] != ConnectorTypeDualLinkDVI) && (DdiConnectorType[3] != ConnectorTypeDualLinkDVI))  {
        if (((DdiPortPresent == 0xA) || (DdiPortPresent == 0x2) || (DdiPortPresent == 0x8)))  {
          // assume it is a C527 card
          CardType = CARD_C527;
        } else {
//          for (Index = 0; Index < 4; Index ++) {
//            if (DdiConnectorType[Index] != INVALID_CONNECTOR_TYPE) {
//              // DDI DP[5:3]: HDMI/DP, GPIO input 1:DP 0:HDMI
//              if ((DdiConnectorType[Index] == ConnectorTypeDP) && (AmdCpmTablePpi->CommonFunction.DetectDevice (AmdCpmTablePpi, DpHdmiDungle[Index], NULL))) {
//                DdiConnectorType[Index] = ConnectorTypeHDMI;
//              }
//            }
//          }
        }
//      } else {
//        DdiEnValue = PCIE_DDI_ENABLE_VALUE_DET;
      }
    }

    if ((DdiPortPresent) || ((CardType >= CARD_B962) && (CardType <= CARD_B967))) {
      switch (CardType) {
      case CARD_B962: // x16 to 4x4 PCIe
        ConfigureDdiTo4444 (AmdCpmTablePpi, PcieTopologyTablePtr, PcieTopologyOverrideTablePtr, ItemPtr, DdiConnectorType, &DdiPortPresent, CardType, GfxLinkCfg);
        break;
      case CARD_B964: // x16 to 2x8 PCIe
        ConfigureDdiTo88 (AmdCpmTablePpi, PcieTopologyTablePtr, PcieTopologyOverrideTablePtr, ItemPtr, DdiConnectorType, &DdiPortPresent, CardType, GfxLinkCfg);
        break;
      case CARD_B966: // x16 to upper x8 PCIe + 2x4 PCIe
        ConfigureDdiTo448 (AmdCpmTablePpi, PcieTopologyTablePtr, PcieTopologyOverrideTablePtr, ItemPtr, DdiConnectorType, &DdiPortPresent, CardType, GfxLinkCfg);
        break;
      case CARD_B967: // x16 to lower x8 PCIe + 2x4 PCIe
        ConfigureDdiTo844 (AmdCpmTablePpi, PcieTopologyTablePtr, PcieTopologyOverrideTablePtr, ItemPtr, DdiConnectorType, &DdiPortPresent, CardType, GfxLinkCfg);
        break;
      case CARD_C527: // x16 to 2x8 PCIe
        ConfigureDdiToC527 (AmdCpmTablePpi, PcieTopologyTablePtr, PcieTopologyOverrideTablePtr, ItemPtr, DdiConnectorType, &DdiPortPresent, CardType, GfxLinkCfg);
        break;
      default:
        ConfigureDdiTo4444 (AmdCpmTablePpi, PcieTopologyTablePtr, PcieTopologyOverrideTablePtr, ItemPtr, DdiConnectorType, &DdiPortPresent, CardType, GfxLinkCfg);
        break;
      }
      SetDevice (GpioDeviceConfigTablePtr, DEVICE_ID_DDI, CPM_DEVICE_ON);
      AmdCpmTablePpi->CommonFunction.SetGpio (AmdCpmTablePpi, PCIE_DDI_ENABLE_PIN, DdiEnValue);
//      AmdCpmTablePpi->CommonFunction.SetGpio (AmdCpmTablePpi, PCIE_DDI_ENABLE_PIN, (PCIE_DDI_ENABLE_VALUE & 0xFFFE));
    } else {
      ItemPtr->Flag.Raw   = VALID_OVERRIDE + IS_DDI_DESCRIPTOR + ENABLE_OVERRIDE_ENABLE;
      ItemPtr->Offset     = PCIE_GFX_DDI_PORT0_OFFSET;
      ItemPtr->Enable     = PcieUnusedEngine;
      ItemPtr++;

      ItemPtr->Flag.Raw   = VALID_OVERRIDE + IS_DDI_DESCRIPTOR + ENABLE_OVERRIDE_ENABLE;
      ItemPtr->Offset     = PCIE_GFX_DDI_PORT1_OFFSET;
      ItemPtr->Enable     = PcieUnusedEngine;
      ItemPtr++;

      ItemPtr->Flag.Raw   = VALID_OVERRIDE + IS_DDI_DESCRIPTOR + ENABLE_OVERRIDE_ENABLE;
      ItemPtr->Offset     = PCIE_GFX_DDI_PORT2_OFFSET;
      ItemPtr->Enable     = PcieUnusedEngine;
      ItemPtr++;

      ItemPtr->Flag.Raw = 0xFF;
    }
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * CPM Override Function After AMD CPM Table PPI
 *
 * This function updates CPM OEM Tables according to setup options or the value to be detected
 * on run time after AMD CPM Table PPI is installed.
 *
 * @param[in]     PeiServices    Pointer to Pei Services
 *
 * @retval        EFI_SUCCESS    Function initialized successfully
 * @retval        EFI_ERROR      Function failed (see error for more details)
 *
 */
EFI_STATUS
EFIAPI
CpmTableOverride (
  IN       EFI_PEI_SERVICES       **PeiServices
  )
{
  EFI_STATUS                            Status;
  AMD_CPM_TABLE_PPI                     *AmdCpmTablePpi;
  AMD_CPM_DISPLAY_FEATURE_TABLE         *DisplayFeatureTablePtr;
  AMD_CPM_MAIN_TABLE                    *MainTablePtr;
  AMD_CPM_PCIE_CLOCK_TABLE              *PcieClockTablePtr;
  AMD_CPM_GPIO_DEVICE_CONFIG_TABLE      *GpioDeviceConfigTablePtr;
  AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_TABLE  *PcieTopologyOverrideTablePtr;
  AMD_CPM_PCIE_TOPOLOGY_TABLE           *PcieTopologyTablePtr;
  CPM_OEM_SETUP_OPTION                  OemSetupOption;
  UINT8                                 Index;

  Status = (*PeiServices)->LocatePpi (
                           PeiServices,
                           &gAmdCpmTablePpiGuid,
                           0,
                           NULL,
                           &AmdCpmTablePpi
                           );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  MainTablePtr                  = AmdCpmTablePpi->MainTablePtr;
  DisplayFeatureTablePtr        = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_DISPLAY_FEATURE);
  PcieClockTablePtr             = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_PCIE_CLOCK);
  PcieTopologyTablePtr          = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_PCIE_TOPOLOGY);
  GpioDeviceConfigTablePtr      = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_GPIO_DEVICE_CONFIG);
  PcieTopologyOverrideTablePtr  = AmdCpmTablePpi->CommonFunction.GetTablePtr (AmdCpmTablePpi, CPM_SIGNATURE_PCIE_TOPOLOGY_OVERRIDE);

  Status = CpmOemSetupOption (PeiServices, &OemSetupOption);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  MainTablePtr->DisplayFeature.Raw &= 0xFFFFFC00;

  switch (OemSetupOption.SpecialVgaFeature) {
  case 1: // HCF_dGPU
    MainTablePtr->DisplayFeature.Raw |= (1 << DFC_HYPER_CROSS_FIRE);
    break;
  case 2: // Surround View
    MainTablePtr->DisplayFeature.Raw |= (1 << DFC_SURROUND_VIEW);
    break;
  case 3: //PX
    MainTablePtr->DisplayFeature.Raw |= BIT2;
    break;
  }

#ifdef CPM_SKIP_SSID_OVERRIDE_CONTROL
  if (OemSetupOption.SkipSsidOverride) {
    DisplayFeatureTablePtr->FunctionDisableMask |= 1;
  }
#endif

  if ((OemSetupOption.PrimaryVideoAdaptor == DGPU_AS_PRIMARY)) {
    MainTablePtr->DisplayFeature.Raw |= (1 << DFC_DGPU_PRIMARY);
  }

  if (OemSetupOption.OnboardLanSwitch == FEATURE_DISABLE) {
    SetDevice (GpioDeviceConfigTablePtr, DEVICE_ID_LAN, CPM_DEVICE_OFF);
  }

  if (OemSetupOption.WirelessLanRfEnable == FEATURE_ENABLE) {
    SetDevice (GpioDeviceConfigTablePtr, DEVICE_ID_WLAN_RF, CPM_DEVICE_ON);
  } else {
    SetDevice (GpioDeviceConfigTablePtr, DEVICE_ID_WLAN_RF, CPM_DEVICE_OFF);
  }

  if (OemSetupOption.MiniPcieSlotEnable == FEATURE_DISABLE) {
    SetDevice (GpioDeviceConfigTablePtr, DEVICE_ID_WLAN_RF, CPM_DEVICE_OFF);
    SetDevice (GpioDeviceConfigTablePtr, DEVICE_ID_MPCIE, CPM_DEVICE_OFF);
  } else {
    SetDevice (GpioDeviceConfigTablePtr, DEVICE_ID_MPCIE, CPM_DEVICE_ON);
  }

  if (OemSetupOption.GfxLinkCfg == GFX_2X8) {
    SetDevice (GpioDeviceConfigTablePtr, DEVICE_ID_GFX_LINK_CFG, CPM_DEVICE_OFF);
  } else {
    SetDevice (GpioDeviceConfigTablePtr, DEVICE_ID_GFX_LINK_CFG, CPM_DEVICE_ON);
  }

  MainTablePtr->ZeroPowerOddEn = 0;

  MainTablePtr->UnusedGppClkOffEn = OemSetupOption.UnusedGppClkOff;

  MainTablePtr->AcpiThermalFanEn  = OemSetupOption.AcpiThermalFanEn ? 0 : 1;
  MainTablePtr->AdaptiveS4En      = OemSetupOption.AdaptiveS4En ? 2 : 0;

  DetectPcieDevices (AmdCpmTablePpi, PcieTopologyTablePtr, PcieTopologyOverrideTablePtr, GpioDeviceConfigTablePtr, &OemSetupOption);

  if (PcieClockTablePtr && OemSetupOption.UnusedGppClkOff && !OemSetupOption.ClockRequest) {
    Index = 0;
    while (PcieClockTablePtr->Item[Index].ClkId != 0xFF) {
      if (PcieClockTablePtr->Item[Index].ClkReq != CLK_DISABLE) {
        PcieClockTablePtr->Item[Index].ClkReq = CLK_ENABLE;
      }
      if (PcieClockTablePtr->Item[Index].ClkReqExt != CLK_DISABLE) {
        PcieClockTablePtr->Item[Index].ClkReqExt = CLK_ENABLE;
      }
      Index++;
    }
  }

  return Status;
}
