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
  0,                                                                                                            // SocketId
  {                                                                                                             // PCIe_PORT_DESCRIPTOR
    {                                                                                                           // Lanes 8:23, PCI Device Number 2
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PciePortEngine, 16, 31),
      PCIE_PORT_DATA_INITIALIZER_V2 (PortEnabled, ChannelTypeExt6db, 2, 1, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_MXM, 0)
    },

    {                                                                                                           // Lanes 16:19, PCI Device Number 3
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, 24, 31),
      PCIE_PORT_DATA_INITIALIZER_V2 (PortDisabled, ChannelTypeExt6db, 2, 2, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_MXM, 0)
    },

    {                                                                                                           // Lanes 4, PCI Device Number 4
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PciePortEngine, 8, 8),
      PCIE_PORT_DATA_INITIALIZER_V2 (PortEnabled, ChannelTypeExt6db, 3, 1, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_LAN, 0)
    },
    {                                                                                                           // Lanes 5, PCI Device Number 5
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PciePortEngine, 9, 9),
      PCIE_PORT_DATA_INITIALIZER_V2 (PortEnabled, ChannelTypeExt6db, 3, 2, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_MPCIE1, 0)
    },
    {
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PciePortEngine, 10, 10),                                                    // Lanes 6, PCI Device Number 6
      PCIE_PORT_DATA_INITIALIZER_V2 (PortEnabled, ChannelTypeExt6db, 3, 3, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_MPCIE2, 0)
    },
    {                                                                                                           // Lanes 7, PCI Device Number 7
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PciePortEngine, 11, 11),
      PCIE_PORT_DATA_INITIALIZER_V2 (PortEnabled, ChannelTypeExt6db, 3, 4, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_EXPRESSCARD, 0)
    },
    {                                                                                                           // Lanes 7, PCI Device Number 7
      DESCRIPTOR_TERMINATE_LIST, //Descriptor flags  !!!IMPORTANT!!! Terminate last element of array
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, 12, 15),
      PCIE_PORT_DATA_INITIALIZER_V2 (PortEnabled, ChannelTypeExt6db, 3, 5, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_EXPRESSCARD, 0)
    }
  },
  {                                                                                                               // PCIe_DDI_DESCRIPTOR
    {                                                                                                           // Port 0. Mini DDI slot
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieDdiEngine, 4, 7),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDP, Aux1, Hdp1)
    },
    {                                                                                                           // Port 1, DMC slot
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieDdiEngine, 32, 35),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDP, Aux2, Hdp2)                                                   // ConnectorTypeDP
    },
    {                                                                                                           // Port 2, Mini DDI slot
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieDdiEngine, 36, 39),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDP, Aux3, Hdp3)
    },
    {                                                                                                           // Port 3
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, 12, 15),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDP, Aux6, Hdp6)
    },
    {                                                                                                           // Via MXM slot, Lane[8,11] unused for DDI
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, 24, 31),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDP, Aux4, Hdp4)
    },
    {
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, 24, 31),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDP, Aux5, Hdp5)
    },
    {
      DESCRIPTOR_TERMINATE_LIST,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, 16, 23),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDP, Aux6, Hdp6)
    }
  },
};

AMD_CPM_PCIE_TOPOLOGY_TABLE gCpmPcieTopologyTable2 = {
  {CPM_SIGNATURE_PCIE_TOPOLOGY, sizeof (gCpmPcieTopologyTable2) / sizeof(UINT8), 0, 0, 0x02, 0x0F},             // Header
  0,                                                                                                            // SocketId
  {                                                                                                             // PCIe_PORT_DESCRIPTOR
    {                                                                                                           // Lanes 8:23, PCI Device Number 2
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PciePortEngine, 8, 23),
      PCIE_PORT_DATA_INITIALIZER (PortEnabled, ChannelTypeExt6db, 2, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_MXM)
    },

    {                                                                                                           // Lanes 16:19, PCI Device Number 3
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, 16, 19),
      PCIE_PORT_DATA_INITIALIZER (PortDisabled, ChannelTypeExt6db, 3, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_MXM)
    },

    {                                                                                                           // Lanes 4, PCI Device Number 4
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PciePortEngine, 4, 4),
      PCIE_PORT_DATA_INITIALIZER (PortEnabled, ChannelTypeExt6db, 4, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_LAN)
    },
    {                                                                                                           // Lanes 5, PCI Device Number 5
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PciePortEngine, 5, 5),
      PCIE_PORT_DATA_INITIALIZER (PortEnabled, ChannelTypeExt6db, 5, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_MPCIE1)
    },
    {
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PciePortEngine, 6, 6),                                                    // Lanes 6, PCI Device Number 6
      PCIE_PORT_DATA_INITIALIZER (PortEnabled, ChannelTypeExt6db, 6, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_MPCIE2)
    },
    {                                                                                                           // Lanes 7, PCI Device Number 7
      DESCRIPTOR_TERMINATE_LIST, //Descriptor flags  !!!IMPORTANT!!! Terminate last element of array
      PCIE_ENGINE_DATA_INITIALIZER (PciePortEngine, 7, 7),
      PCIE_PORT_DATA_INITIALIZER (PortEnabled, ChannelTypeExt6db, 7, HotplugDisabled, PcieGenMaxSupported, PcieGenMaxSupported, AspmL0sL1, DEVICE_ID_EXPRESSCARD)
    }
  },
  {                                                                                                             // PCIe_DDI_DESCRIPTOR
    {                                                                                                           // Port 0. Mini DDI slot
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieDdiEngine, 24, 27),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDP, Aux1, Hdp1)
    },
    {                                                                                                           // Port 1, DMC slot
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieDdiEngine, 28, 31),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDP, Aux2, Hdp2)                                                   // ConnectorTypeDP
    },
    {                                                                                                           // Port 2, Mini DDI slot
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieDdiEngine, 32, 35),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDP, Aux3, Hdp3)
    },
    {                                                                                                           // Port 3 (Reserved)
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, 20, 23),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDP, Aux6, Hdp6)
    },
    {                                                                                                           // Via MXM slot, Lane[8,11] unused for DDI
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, 8, 15),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDP, Aux4, Hdp4)
    },
    {
      0,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, 16, 23),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDP, Aux5, Hdp5)
    },
    {
      DESCRIPTOR_TERMINATE_LIST,
      PCIE_ENGINE_DATA_INITIALIZER (PcieUnusedEngine, 16, 23),
      PCIE_DDI_DATA_INITIALIZER (ConnectorTypeDP, Aux6, Hdp6)
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