/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM OEM API, and related functions.
 *
 * Contains the table definition for Pcie.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  OEM
 * @e \$Revision: 281158 $   @e \$Date: 2013-12-17 20:36:04 -0600 (Tue, 17 Dec 2013) $
 *
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

#ifndef _GNB_PCIE_TOPOLOGY_TABLE_H_
#define _GNB_PCIE_TOPOLOGY_TABLE_H_
#include "AmdCpmOemInitPeim.h"

//
// CPM PCIe Topology Table
//
AMD_CPM_PCIE_TOPOLOGY_TABLE gCpmPcieTopologyTable = {
  {CPM_SIGNATURE_PCIE_TOPOLOGY, sizeof (gCpmPcieTopologyTable) / sizeof(UINT8), 0, 0, 0x01, 0x0F},              // Header
  0,  // SocketId
  {   // PCIe_PORT_DESCRIPTOR
    { // Lanes 8:23, PCI Device Number 2
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PciePortEngine, GFX0_LANE_START_KV, GFX0_LANE_END_KV),
      PCIE_PORT_DATA_INITIALIZER_V2 (PortEnabled, ChannelTypeExt6db, APU_GFX0_DEV_KV, APU_GFX0_FUNC_KV, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_GFX0, 0)
    },

    { // Lanes 16:19, PCI Device Number 3
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, GFX1_LANE_START_KV, GFX1_LANE_END_KV),
      PCIE_PORT_DATA_INITIALIZER_V2 (PortDisabled, ChannelTypeExt6db, APU_GFX1_DEV_KV, APU_GFX1_FUNC_KV, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_GFX1, 0)
    },

    { // Lanes 4, PCI Device Number 4
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PciePortEngine, P_GPP0_LANE_START_KV, P_GPP3_LANE_END_KV),
      PCIE_PORT_DATA_INITIALIZER_V2 (PortEnabled, ChannelTypeExt6db, APU_GPP0_DEV_KV, APU_GPP0_FUNC_KV, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_APU_GPP, 0)
    },
    { // Lanes 5, PCI Device Number 5
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, P_GPP1_LANE_START_KV, P_GPP1_LANE_END_KV),
      PCIE_PORT_DATA_INITIALIZER_V2 (PortEnabled, ChannelTypeExt6db, APU_GPP1_DEV_KV, APU_GPP1_FUNC_KV, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_APU_GPP, 0)
    },
    { // Lanes 6, PCI Device Number 6
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, P_GPP2_LANE_START_KV, P_GPP2_LANE_END_KV),
      PCIE_PORT_DATA_INITIALIZER_V2 (PortEnabled, ChannelTypeExt6db, APU_GPP2_DEV_KV, APU_GPP2_FUNC_KV, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_APU_GPP, 0)
    },
    { // Lanes 7, PCI Device Number 7
      DESCRIPTOR_TERMINATE_LIST, //Descriptor flags  !!!IMPORTANT!!! Terminate last element of array
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, P_GPP3_LANE_START_KV, P_GPP3_LANE_END_KV),
      PCIE_PORT_DATA_INITIALIZER_V2 (PortEnabled, ChannelTypeExt6db, APU_GPP3_DEV_KV, APU_GPP3_FUNC_KV, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_APU_GPP, 0)
    }
  },
  {   // PCIe_DDI_DESCRIPTOR
    { // DP_0: FCH Nutmeg VGA
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieDdiEngine, DP0_LANE_START_KV, DP0_LANE_END_KV),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeNutmegDpToVga, Aux1, Hdp1)
    },
    { // DP_1: HDMI
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieDdiEngine, DP1_LANE_START_KV, DP1_LANE_END_KV),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeHDMI, Aux2, Hdp2)
    },
    { // DP_2: HDMI/DP, GPIO 191 input 1:DP 0:HDMI
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieDdiEngine, DP2_LANE_START_KV, DP2_LANE_END_KV),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDP, Aux3, Hdp3)
    },
    { // DP_3: keep information of DL_DVI
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, DP_DLDVI_LANE_START_KV, DP_DLDVI_LANE_END_KV),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDualLinkDVI, Aux2, Hdp2)
    },
    { // DP_4 (GFX[15:12]<=>PCIe[31:28]): HDMI/DP, GPIO 197 input 1:DP 0:HDMI
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, DDI0_LANE_START_KV, DDI0_LANE_END_KV),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeHDMI, Aux4, Hdp4)
    },
    { // DP_5 (GFX[11:8]<=>PCIe[27:24]): HDMI/DP, GPIO 198 input 1:DP 0:HDMI
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, DDI1_LANE_START_KV, DDI1_LANE_END_KV),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeHDMI, Aux5, Hdp5)
    },
    { // DP_6 (GFX[7:4]<=>PCIe[23:20]): HDMI/DP, GPIO 192 input 1:DP 0:HDMI
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, DDI2_LANE_START_KV, DDI2_LANE_END_KV),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeHDMI, Aux6, Hdp6)
    },
    { // DP_7 (GFX[3:0]<=>PCIe[19:16]): N/A
      DESCRIPTOR_TERMINATE_LIST,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, DDI3_LANE_START_KV, DDI3_LANE_END_KV ),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeHDMI, MaxAux, MaxHdp)
    }
  },
};

AMD_CPM_PCIE_TOPOLOGY_TABLE gCpmPcieTopologyTable2 = {
  {CPM_SIGNATURE_PCIE_TOPOLOGY, sizeof (gCpmPcieTopologyTable2) / sizeof(UINT8), 0, 0, 0x02, 0x0F},             // Header
  0,  // SocketId
  {   // PCIe_PORT_DESCRIPTOR
    { // Lanes 8:23, PCI Device Number 2
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PciePortEngine, GFX0_LANE_START_TN, GFX0_LANE_END_TN),
      PCIE_PORT_DATA_INITIALIZER (PortEnabled, ChannelTypeExt6db, APU_GFX0_DEV_TN, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_GFX0)
    },
    { // Lanes 16:19, PCI Device Number 3
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, GFX1_LANE_START_TN, GFX1_LANE_END_TN),
      PCIE_PORT_DATA_INITIALIZER (PortDisabled, ChannelTypeExt6db, APU_GFX1_DEV_TN, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_GFX1)
    },
    { // Lanes 4, PCI Device Number 4
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PciePortEngine, P_GPP0_LANE_START_TN, P_GPP3_LANE_END_TN),
      PCIE_PORT_DATA_INITIALIZER (PortEnabled, ChannelTypeExt6db, APU_GPP0_DEV_TN, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_APU_GPP)
    },
    { // Lanes 5, PCI Device Number 5
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, P_GPP1_LANE_START_TN, P_GPP1_LANE_END_TN),
      PCIE_PORT_DATA_INITIALIZER (PortEnabled, ChannelTypeExt6db, APU_GPP1_DEV_TN, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_APU_GPP)
    },
    { // Lanes 6, PCI Device Number 6
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, P_GPP2_LANE_START_TN, P_GPP2_LANE_END_TN),
      PCIE_PORT_DATA_INITIALIZER (PortEnabled, ChannelTypeExt6db, APU_GPP2_DEV_TN, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_APU_GPP)
    },
    { // Lanes 7, PCI Device Number 7
      DESCRIPTOR_TERMINATE_LIST, //Descriptor flags  !!!IMPORTANT!!! Terminate last element of array
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, P_GPP3_LANE_START_TN, P_GPP3_LANE_END_TN),
      PCIE_PORT_DATA_INITIALIZER (PortEnabled, ChannelTypeExt6db, APU_GPP3_DEV_TN, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_APU_GPP)
    }
  },
  {   // PCIe_DDI_DESCRIPTOR
    { // DP_0: FCH Nutmeg VGA
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieDdiEngine, DP0_LANE_START_TN, DP0_LANE_END_TN),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeNutmegDpToVga, Aux1, Hdp1)
    },
    { // DP_1: HDMI
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieDdiEngine, DP1_LANE_START_TN, DP1_LANE_END_TN),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeHDMI, Aux2, Hdp2)
    },
    { // DP_2: HDMI/DP
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieDdiEngine, DP2_LANE_START_TN, DP2_LANE_END_TN),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDP, Aux3, Hdp3)
    },
    { // DP_3: keep information of DL_DVI
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, DP_DLDVI_LANE_START_TN, DP_DLDVI_LANE_END_TN),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDualLinkDVI, Aux2, Hdp2)
    },
    { // DP_4 (GFX[15:12]<=>PCIe[23:20]): HDMI/DP, GPIO 197 input 1:DP 0:HDMI
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, DDI0_LANE_START_TN, DDI0_LANE_END_TN),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeHDMI, Aux4, Hdp4)
    },
    { // DP_5 (GFX[11:8]<=>PCIe[19:16]): HDMI/DP, GPIO 198 input 1:DP 0:HDMI
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, DDI1_LANE_START_TN, DDI1_LANE_END_TN),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeHDMI, Aux5, Hdp5)
    },
    { // DP_6 (GFX[7:4]<=>PCIe[15:12]): HDMI/DP, GPIO 192 input 1:DP 0:HDMI
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, DDI2_LANE_START_TN, DDI2_LANE_END_TN),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeHDMI, Aux6, Hdp6)
    },
    { // DP_7 (GFX[3:0]<=>PCIe[11:8]): N/A
      DESCRIPTOR_TERMINATE_LIST,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, DDI3_LANE_START_TN, DDI3_LANE_END_TN),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeHDMI, MaxAux, MaxHdp)
    }
  },
};

AMD_CPM_PCIE_TOPOLOGY_TABLE gCpmPcieTopologyTable3 = {
  {CPM_SIGNATURE_PCIE_TOPOLOGY, sizeof (gCpmPcieTopologyTable) / sizeof(UINT8), 0, 0, 0x04, 0x0F},              // Header
  0,  // SocketId
  {   // PCIe_PORT_DESCRIPTOR
    { // Lanes 8:23, PCI Device Number 2
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PciePortEngine, GFX0_LANE_START_KV, GFX0_LANE_END_KV),
      PCIE_PORT_DATA_INITIALIZER_V2 (PortEnabled, ChannelTypeExt6db, APU_GFX0_DEV_KV, APU_GFX0_FUNC_KV, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_GFX0, 0)
    },

    { // Lanes 16:19, PCI Device Number 3
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, GFX1_LANE_START_KV, GFX1_LANE_END_KV),
      PCIE_PORT_DATA_INITIALIZER_V2 (PortDisabled, ChannelTypeExt6db, APU_GFX1_DEV_KV, APU_GFX1_FUNC_KV, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_GFX1, 0)
    },

    { // Lanes 4, PCI Device Number 4
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PciePortEngine, P_GPP0_LANE_START_KV, P_GPP3_LANE_END_KV),
      PCIE_PORT_DATA_INITIALIZER_V2 (PortEnabled, ChannelTypeExt6db, APU_GPP0_DEV_KV, APU_GPP0_FUNC_KV, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_APU_GPP, 0)
    },
    { // Lanes 5, PCI Device Number 5
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, P_GPP1_LANE_START_KV, P_GPP1_LANE_END_KV),
      PCIE_PORT_DATA_INITIALIZER_V2 (PortEnabled, ChannelTypeExt6db, APU_GPP1_DEV_KV, APU_GPP1_FUNC_KV, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_APU_GPP, 0)
    },
    { // Lanes 6, PCI Device Number 6
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, P_GPP2_LANE_START_KV, P_GPP2_LANE_END_KV),
      PCIE_PORT_DATA_INITIALIZER_V2 (PortEnabled, ChannelTypeExt6db, APU_GPP2_DEV_KV, APU_GPP2_FUNC_KV, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_APU_GPP, 0)
    },
    { // Lanes 7, PCI Device Number 7
      DESCRIPTOR_TERMINATE_LIST, //Descriptor flags  !!!IMPORTANT!!! Terminate last element of array
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, P_GPP3_LANE_START_KV, P_GPP3_LANE_END_KV),
      PCIE_PORT_DATA_INITIALIZER_V2 (PortEnabled, ChannelTypeExt6db, APU_GPP3_DEV_KV, APU_GPP3_FUNC_KV, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_APU_GPP, 0)
    }
  },
  {   // PCIe_DDI_DESCRIPTOR
    { // DP_0: FCH Nutmeg VGA
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieDdiEngine, DP0_LANE_START_KV, DP0_LANE_END_KV),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeNutmegDpToVga, Aux1, Hdp1)
    },
    { // DP_1: HDMI
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieDdiEngine, DP1_LANE_START_KV, DP1_LANE_END_KV),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeHDMI, Aux2, Hdp2)
    },
    { // DP_2: HDMI/DP, GPIO 191 input 1:DP 0:HDMI
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieDdiEngine, DP2_LANE_START_KV, DP2_LANE_END_KV),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDP, Aux3, Hdp3)
    },
    { // DP_3: keep information of DL_DVI
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, DP_DLDVI_LANE_START_KV, DP_DLDVI_LANE_END_KV),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDualLinkDVI, Aux2, Hdp2)
    },
    { // DP_4 (GFX[15:12]<=>PCIe[31:28]): HDMI/DP, GPIO 197 input 1:DP 0:HDMI
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, DDI0_LANE_START_KV, DDI0_LANE_END_KV),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeHDMI, Aux4, Hdp4)
    },
    { // DP_5 (GFX[11:8]<=>PCIe[27:24]): HDMI/DP, GPIO 198 input 1:DP 0:HDMI
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, DDI1_LANE_START_KV, DDI1_LANE_END_KV),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeHDMI, Aux5, Hdp5)
    },
    { // DP_6 (GFX[7:4]<=>PCIe[23:20]): HDMI/DP, GPIO 192 input 1:DP 0:HDMI
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, DDI2_LANE_START_KV, DDI2_LANE_END_KV),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeHDMI, Aux6, Hdp6)
    },
    { // DP_7 (GFX[3:0]<=>PCIe[19:16]): N/A
      DESCRIPTOR_TERMINATE_LIST,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, DDI3_LANE_START_KV, DDI3_LANE_END_KV ),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeHDMI, MaxAux, MaxHdp)
    }
  },
};


//
// CPM PCIe Topology Override Table
//
AMD_CPM_PCIE_TOPOLOGY_OVERRIDE_TABLE    gCpmPcieTopologyOverride = {
  {CPM_SIGNATURE_PCIE_TOPOLOGY_OVERRIDE, sizeof (gCpmPcieTopologyOverride) / sizeof (UINT8), 0, 0, 0, 0x0F},
  {
    0xFF,
  },
};
#endif