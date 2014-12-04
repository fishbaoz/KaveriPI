/* $NoKeywords:$ */
/**
 * @file
 *
 * Topology Services implementation.
 *
 * Implements the Topology Services for the AGESA core components,
 * as well as user topology service interfaces.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Topology
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

/*
 *----------------------------------------------------------------------------
 *                                MODULES USED
 *
 *----------------------------------------------------------------------------
 */



#include "AGESA.h"
#include "AdvancedApi.h"
#include "amdlib.h"
#include "Ids.h"
#include "Topology.h"
#include "TopoFeat.h"
#include "TopoNb.h"
#include "heapManager.h"
#include "cpuServices.h"
#include "cpuFeatures.h"
#include "Filecode.h"
CODE_GROUP (G1_PEICC)
RDATA_GROUP (G1_PEICC)

#define FILECODE PROC_TOPOLOGY_TOPOMAIN_FILECODE
#define APIC_Base_BSP    8
#define APIC_Base        0x1b

BOOLEAN
STATIC
IsBootCore (
  IN       STATE_DATA    *State
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Get a new Socket Die to Node Map.
 *
 * Put the Socket Die Table in heap with a known handle.  Content will be generated as
 * each node is discovered.
 *
 * @param[in,out] State global state
 */
VOID
NewNodeAndSocketTables (
  IN OUT   STATE_DATA *State
  )
{
  UINT8 i;
  UINT8 j;
  ALLOCATE_HEAP_PARAMS AllocHeapParams;

  // Allocate heap for the table
  State->SocketDieToNodeMap = NULL;
  AllocHeapParams.RequestedBufferSize = (((MAX_SOCKETS) * (MAX_DIES)) * sizeof (SOCKET_DIE_TO_NODE_ITEM));
  AllocHeapParams.BufferHandle = SOCKET_DIE_MAP_HANDLE;
  AllocHeapParams.Persist = HEAP_SYSTEM_MEM;
  if (HeapAllocateBuffer (&AllocHeapParams, State->ConfigHandle) == AGESA_SUCCESS) {
    State->SocketDieToNodeMap = (SOCKET_DIE_TO_NODE_MAP)AllocHeapParams.BufferPtr;
    // Initialize shared data structures
    for (i = 0; i < MAX_SOCKETS; i++) {
      for (j = 0; j < MAX_DIES; j++) {
        (*State->SocketDieToNodeMap)[i][j].Node = TOPOLOGY_LIST_TERMINAL;
        (*State->SocketDieToNodeMap)[i][j].LowCore = TOPOLOGY_LIST_TERMINAL;
        (*State->SocketDieToNodeMap)[i][j].HighCore = TOPOLOGY_LIST_TERMINAL;
      }
    }
  }
  // Allocate heap for the table
  State->NodeToSocketDieMap = NULL;
  AllocHeapParams.RequestedBufferSize = (MAX_NODES * sizeof (NODE_TO_SOCKET_DIE_ITEM));
  AllocHeapParams.BufferHandle = NODE_ID_MAP_HANDLE;
  AllocHeapParams.Persist = HEAP_SYSTEM_MEM;
  if (HeapAllocateBuffer (&AllocHeapParams, State->ConfigHandle) == AGESA_SUCCESS) {
    State->NodeToSocketDieMap = (NODE_TO_SOCKET_DIE_MAP)AllocHeapParams.BufferPtr;
    // Initialize shared data structures
    for (i = 0; i < MAX_NODES; i++) {
      (*State->NodeToSocketDieMap)[i].Socket = TOPOLOGY_LIST_TERMINAL;
      (*State->NodeToSocketDieMap)[i].Die = TOPOLOGY_LIST_TERMINAL;
    }
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Update maps with the core range for each module.
 *
 * Cores are numbered relative to a Processor, but sometimes there is a need to know the
 * starting and ending core ids on a particular node.  This same info is also useful for
 * supporting the Core count on a node other than the one currently executing.
 *
 * For each Processor, get the core count of each node using the family specific PCI core count
 * interface. The order of cores in a processor, and whether it is special for the BSP is family
 * specific.  But whether the processor orders core ids by module or node, iterate in the right
 * order and use the counts to determine each start and end range.
 *
 * Update compute unit status for each node.
 *
 * @param[in]   State    number of Nodes discovered.
*/
VOID
STATIC
UpdateCoreRanges (
  IN       STATE_DATA    *State
    )
{
  UINT8 Node;
  UINT8 ProcessorCores;
  UINT8 ModuleCoreCount[MAX_DIES];
  UINT8 Socket;
  UINT8 Module;

  ASSERT (State->SocketDieToNodeMap != NULL);
  ASSERT (State->NodeToSocketDieMap != NULL);

  for (Socket = 0; Socket < MAX_SOCKETS; Socket++) {
    // Is a Processor present in Socket?
    if ((*State->SocketDieToNodeMap)[Socket][0].Node != TOPOLOGY_LIST_TERMINAL) {
      // Get all the Module core counts for this processor
      // Note that the core counts are 1 based counts.
      // Since Compute Unit info is not module ordering dependent, write it now.
      for (Module = 0; Module < MAX_DIES; Module++) {
        if ((*State->SocketDieToNodeMap)[Socket][Module].Node != TOPOLOGY_LIST_TERMINAL) {
          ModuleCoreCount[Module] = State->Nb->GetNumCoresOnNode ((*State->SocketDieToNodeMap)[Socket][Module].Node, State->Nb);
          (*State->SocketDieToNodeMap)[Socket][Module].EnabledComputeUnits =
            State->Nb->GetEnabledComputeUnits ((*State->SocketDieToNodeMap)[Socket][Module].Node, State->Nb);
          (*State->SocketDieToNodeMap)[Socket][Module].DualCoreComputeUnits =
            State->Nb->GetDualCoreComputeUnits ((*State->SocketDieToNodeMap)[Socket][Module].Node, State->Nb);
          (*State->SocketDieToNodeMap)[Socket][Module].TripleCoreComputeUnits =
            State->Nb->GetTripleCoreComputeUnits ((*State->SocketDieToNodeMap)[Socket][Module].Node, State->Nb);
          (*State->SocketDieToNodeMap)[Socket][Module].QuadCoreComputeUnits =
            State->Nb->GetQuadCoreComputeUnits ((*State->SocketDieToNodeMap)[Socket][Module].Node, State->Nb);
        } else {
          ModuleCoreCount[Module] = 0;
        }
      }
      // Determine the core ordering rule for this processor.
      if ((((*State->NodeToSocketDieMap)[0].Socket == Socket) && State->Nb->IsOrderBSPCoresByNode) ||
          (!State->Nb->IsOrderCoresByModule)) {
        // Order core ranges on this processor by Node Id.
        ProcessorCores = 0;
        for (Node = 0; Node < State->Nb->GetNodeCount (State->Nb); Node++) {
          // Is this node a module in this processor?
          if ((*State->NodeToSocketDieMap)[Node].Socket == Socket) {
            Module = (*State->NodeToSocketDieMap)[Node].Die;
            if (ModuleCoreCount[Module] != 0) {
              (*State->SocketDieToNodeMap)[Socket][Module].LowCore = ProcessorCores;
              (*State->SocketDieToNodeMap)[Socket][Module].HighCore = ProcessorCores + (ModuleCoreCount[Module] - 1);
              IDS_HDT_CONSOLE (
                TOPO_TRACE,
                (IsBootCore (State) ?
                 "Topology: Socket %d, Die %d, is Node %d, with Cores %d thru %d. Compute Unit status (0x%x,0x%x,0x%x,0x%x).\n" :
                 ""),
                Socket,
                Module,
                Node,
                (*State->SocketDieToNodeMap)[Socket][Module].LowCore,
                (*State->SocketDieToNodeMap)[Socket][Module].HighCore,
                (*State->SocketDieToNodeMap)[Socket][Module].EnabledComputeUnits,
                (*State->SocketDieToNodeMap)[Socket][Module].DualCoreComputeUnits,
                (*State->SocketDieToNodeMap)[Socket][Module].TripleCoreComputeUnits,
                (*State->SocketDieToNodeMap)[Socket][Module].QuadCoreComputeUnits
                );
              ProcessorCores = ProcessorCores + ModuleCoreCount[Module];
            }
          }
        }
      } else {
        // Order core ranges in this processor by Module Id.
        ProcessorCores = 0;
        for (Module = 0; Module < MAX_DIES; Module++) {
          if (ModuleCoreCount[Module] != 0) {
            (*State->SocketDieToNodeMap)[Socket][Module].LowCore = ProcessorCores;
            (*State->SocketDieToNodeMap)[Socket][Module].HighCore = ProcessorCores + (ModuleCoreCount[Module] - 1);
              IDS_HDT_CONSOLE (
                TOPO_TRACE,
                (IsBootCore (State) ?
                 "Topology: Socket %d, Die %d, is Node %d, with Cores %d thru %d. Compute Unit status (0x%x,0x%x,0x%x,0x%x).\n" :
                 ""),
                Socket,
                Module,
                (*State->SocketDieToNodeMap)[Socket][Module].Node,
                (*State->SocketDieToNodeMap)[Socket][Module].LowCore,
                (*State->SocketDieToNodeMap)[Socket][Module].HighCore,
                (*State->SocketDieToNodeMap)[Socket][Module].EnabledComputeUnits,
                (*State->SocketDieToNodeMap)[Socket][Module].DualCoreComputeUnits,
                (*State->SocketDieToNodeMap)[Socket][Module].TripleCoreComputeUnits,
                (*State->SocketDieToNodeMap)[Socket][Module].QuadCoreComputeUnits
                );
            ProcessorCores = ProcessorCores + ModuleCoreCount[Module];
          }
        }
      }
    }
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Complete the coherent init with any system level initialization.
 *
 * Find the total number of cores and update the number of Nodes and cores in all cpus.
 *
 * @param[in]   State    number of Nodes discovered.
*/
VOID
STATIC
FinalizeCoherentInit (
  IN       STATE_DATA    *State
    )
{
  UINT8 TotalCores;

  TotalCores = State->Nb->GetNumCoresOnNode (0, State->Nb);

  State->Nb->SetTotalNodesAndCores (0, 1, TotalCores, State->Nb);

}

/*----------------------------------------------------------------------------------------*/
/**
 * Initialize the coherent fabric.
 *
 * Create the topology map of the coherent fabric.
 *
 * @param[in,out]   State   global state
 */
VOID
STATIC
CoherentInit (
  IN OUT   STATE_DATA    *State
  )
{
  // Because Node 0, the BSP, is not discovered, initialize info about it specially here.
  // Set Socket Die Map for the BSP.
  // There is no option to not have socket - node maps, if they aren't allocated that is a fatal bug.
  ASSERT (State->SocketDieToNodeMap != NULL);
  ASSERT (State->NodeToSocketDieMap != NULL);

  (*State->SocketDieToNodeMap)[0][0].Node = 0;
  (*State->NodeToSocketDieMap)[0].Socket = 0;
  (*State->NodeToSocketDieMap)[0].Die = 0;

  // UpdateCoreRanges requires the other maps to be initialized, and the node count set.
  FinalizeCoherentInit (State);
  UpdateCoreRanges (State);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Initialize the Node and Socket maps for an AP Core.
 *
 * In each core's local heap, create a Node to Socket map and a Socket/Module to Node map.
 *
 * @param[in]    State    global state, input data
 *
 */
VOID
STATIC
InitApMaps (
  IN       STATE_DATA *State
  )
{
  // There is no option to not have socket - node maps, if they aren't allocated that is a fatal bug.
  ASSERT (State->SocketDieToNodeMap != NULL);
  ASSERT (State->NodeToSocketDieMap != NULL);

  (*State->SocketDieToNodeMap)[0][0].Node = 0;
  (*State->NodeToSocketDieMap)[0].Socket = 0;
  (*State->NodeToSocketDieMap)[0].Die = 0;

  // This requires the other maps to be initialized.
  UpdateCoreRanges (State);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Is the currently running core the BSC?
 *
 * Determine whether the init steps for BSC or AP core should be run.
 *
 * @param[in]    State    global state, input data
 *
 * @retval       TRUE    This is the boot core.
 * @retval       FALSE   This is not the boot core.
 */
BOOLEAN
STATIC
IsBootCore (
  IN       STATE_DATA    *State
  )
{
  UINT64 Value;

  LibAmdMsrRead (APIC_Base, &Value, State->ConfigHandle);

  return ((BOOLEAN) (((UINT32) (Value & 0xFFFFFFFF) & ((UINT32)1 << APIC_Base_BSP)) != 0));
}

/***************************************************************************
 ***                      Topology Initialize                            ***
 ***************************************************************************/

/*----------------------------------------------------------------------------------------*/
/**
 * Topology Services Initialization.
 *
 * Build the system topology data structures.  Initialize hardware values needed
 * for correct fabric operation, such as core count.
 *
 * @param[in]   StdHeader              Opaque handle to standard config header
 * @param[in]   PlatformConfiguration  The platform configuration options.
 *
 * @retval      AGESA_SUCCESS     Only information events logged.
 *
 */
AGESA_STATUS
AmdTopologyInitialize (
  IN       AMD_CONFIG_PARAMS      *StdHeader,
  IN       PLATFORM_CONFIGURATION *PlatformConfiguration
  )
{
  STATE_DATA State;
  NORTHBRIDGE Nb;

  State.ConfigHandle = StdHeader;
  State.PlatformConfiguration = PlatformConfiguration;

  // Initialize for status and event output
  State.MaxEventClass = AGESA_SUCCESS;

  // Allocate permanent heap structs that are interfaces to other AGESA services.
  NewNodeAndSocketTables (&State);

  if (IsBootCore (&State)) {
    AGESA_TESTPOINT (TpProcTopologyEntry, State.ConfigHandle);
    // Create the BSP's northbridge.
    NewNorthBridge (0, &State, &Nb);
    State.Nb = &Nb;

    CoherentInit (&State);
    AGESA_TESTPOINT (TpProcTopologyDone, State.ConfigHandle);
  } else {
    // Do the AP Topology Init, which produces Node and Socket Maps for the AP's use.
    NewNorthBridge (0, &State, &Nb);
    State.Nb = &Nb;
    InitApMaps (&State);
  }
  return State.MaxEventClass;
}
