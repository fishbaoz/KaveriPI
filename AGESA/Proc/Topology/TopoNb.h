/* $NoKeywords:$ */
/**
 * @file
 *
 * Topology NorthBridge header
 *
 * Defines the interface to the Topology NorthBridge module.
 *
 * It is expected that there will be multiple northbridge implementation files all
 * conforming to this common interface.
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

#ifndef _TOPO_NB_H_
#define _TOPO_NB_H_

/**
 * @page topoimplnb Topology Northbridge Implementation Guide
 *
 * The Topology Northbridge provides access to the Northbridge hardware, in a manner that
 * isolates calling code from knowledge about the hardware implementation or which
 * features are supported in the current build.  This is the mechanism in the Topology code for
 * supporting new Family or Model northbridges, as well as the means for supporting
 * multiple northbridges in a single build or mixed revision northbridge sets.
 *
 * @par Adding a Method to the Northbridge
 *
 * To add a new method to the Northbridge, follow these steps.
 * <ul>
 * <li> Create a typedef for the Method with the correct parameters and return type.
 *
 *   <ul>
 *   <li> Name the method typedef (F_METHOD_NAME)(), where METHOD_NAME is the same
 *     name as the method table item, but with "_"'s and UPPERCASE, rather than mixed case.
 *     @n <tt> typedef VOID (F_METHOD_NAME)(); </tt> @n
 *
 *   <li> Make a reference type for references to a method implementation:
 *     @n <tt> /// Reference to a Method              </tt>
 *     @n <tt> typedef F_METHOD_NAME *PF_METHOD_NAME </tt> @n
 *   </ul>
 *
 * <li> One of the parameters to @b all northbridge Methods is @b required to be a
 *   reference to its current northbridge object.  By convention, this is the
 *   last parameter.
 *
 * <li> Provide a standard doxygen function preamble for the Method typedef. Begin the
 *   detailed description by providing a reference to the method instances page by including
 *   the lines below:
 *   @code
 *   *
 *   * @TopoNbInstances
 *   *
 *   @endcode
 *   @note It is important to provide documentation for the method type, because the method may not
 *   have an implementation in any families supported by the current package. @n
 *
 * <li> Add to the NORTHBRIDGE struct an item for the Method:
 *   @n <tt>  PF_METHOD_NAME MethodName; ///< Method: description. </tt> @n
 * </ul>
 *
 * @par Implementing an Instance of a Northbridge method.
 *
 * To implement an instance of a method for a specific feature follow these steps.
 *
 * - In appropriate files, implement the method with the return type and parameters
 * matching the Method typedef.
 *   - If the Method implementation is common to all families, use the northbridge file
 *   for the function area, for example, add a new coherent initialization support method to the
 *   coherent northbridge file.
 *   - If the Method implementation is unique to each supported northbridge, use the
 *   family specific file for that function area (create it, if it doesn't already exist).
 *   The family specific files have the same name as the common one suffixed with "FamNN",
 *   or "FamNNRevX" if for a model or revision.
 *
 * - Name the function MethodName().  If Family specific, FamNNMethodName().
 *
 * - Create a doxygen function preamble for the method instance.  Begin the detailed description with
 *   an Implements command to reference the method type and add this instance to the Method Instances page.
 *   @code
 *   *
 *   *  @TopoNbMethod{::F_METHOD_NAME}.
 *   *
 *   @endcode
 *
 * - To access other northbridge routines or data as part of the method implementation,
 *   the function must use Nb->OtherMethod().  Do not directly access other northbridge
 *   routines, because in the table there may be overrides or this routine may be shared by
 *   multiple configurations.
 *
 * - Add the instance, or the correct family specific instance, to the NORTHBRIDGE instances
 *   used by the northbridge constructor.
 *
 * - If a northbridge does not need an instance of the method use one of the CommonReturns from
 *   CommonReturns.h with the same return type.
 *
 * @par Making common Northbridge Methods.
 *
 * In some cases, Northbridge methods can easily have a common implementation because the hardware
 * is very compatible or is even standard.  In other cases, where processor family northbridges
 * differ in their implementation, it may be possible to provide a single, common method
 * implementation.  This can be accomplished by adding Northbridge data members.
 *
 * For example, a bit position or bit field mask can be used to accommodate different bit placement or size.
 * Another example, a small table can be used to translate index values from a common set
 * to specific sets.
 *
 * The Northbridge Method Instance must use its NORTHBRIDGE reference parameter to access
 * private data members.
 *
 * @par Invoking Topology Northbridge Methods.
 *
 * Each unique northbridge is constructed based on matching the current northbridge.
 * @n @code
 *   NORTHBRIDGE Nb;
 *   // Create the BSP's northbridge.
 *   NewNorthBridge (0, State, &Nb);
 *   State->Nb = &Nb;
 * @endcode
 *
 * The following example shows how to invoke a Northbridge method.
 * @n @code
 *   State->Nb->MethodName (State->Nb);
 * @endcode
 *
 */

/*----------------------------------------------------------------------------
 *   Mixed (DEFINITIONS AND MACROS / TYPEDEFS, STRUCTURES, ENUMS)
 *
 *----------------------------------------------------------------------------
 */

/*-----------------------------------------------------------------------------
 *                         DEFINITIONS AND MACROS
 *
 *-----------------------------------------------------------------------------
 */

/** Use a macro to convert a Node number to a PCI device.  If some future port of
 * this code needs to, this can easily be replaced by the function declaration:
 * UINT8 makePCIDeviceFromNode(UINT8 Node);
 */
#define MakePciDeviceFromNode(Node)             \
  ((UINT8) (24 + (Node)))

/** Use a macro to convert a Node number to a PCI bus.  If some future port of
 * this code needs to, this can easily be replaced by the function declaration:
 * UINT8 MakePciBusFromNode(UINT8 Node);
 */
#define MakePciBusFromNode(Node)                \
  ((UINT8) (0))

/** Use a macro to convert a Node number to a PCI Segment.  If some future port of
 * this code needs to, this can easily be replaced by the function declaration:
 * UINT8 MakePciSegmentFromNode(UINT8 Node);
 */
#define MakePciSegmentFromNode(Node)            \
  ((UINT8) (0))

/*----------------------------------------------------------------------------
 *                         TYPEDEFS, STRUCTURES, ENUMS
 *
 *----------------------------------------------------------------------------
 */
/**
 * Get the enabled Compute Units.
 *
 * Processors which don't support compute units return zero.
 *
 * @TopoNbInstances
 *
 * @param[in]   Node     The node for which we want the enabled compute units.
 * @param[in]   Nb       Our Northbridge.
 *
 * @return      The  enabled compute units value.
 */
typedef UINT8 F_GET_ENABLED_COMPUTE_UNITS (
  IN       UINT8       Node,
  IN       NORTHBRIDGE *Nb
  );

/// Reference to a method.
typedef F_GET_ENABLED_COMPUTE_UNITS *PF_GET_ENABLED_COMPUTE_UNITS;

/**
 * Get the dual core Compute Units.
 *
 * Processors which don't support compute units return zero.
 *
 * @TopoNbInstances
 *
 * @param[in]   Node     The node for which we want the  dual core compute units.
 * @param[in]   Nb       Our Northbridge.
 *
 * @return      The dual core compute units value.
 */
typedef UINT8 F_GET_DUALCORE_COMPUTE_UNITS (
  IN       UINT8       Node,
  IN       NORTHBRIDGE *Nb
  );

/// Reference to a method.
typedef F_GET_DUALCORE_COMPUTE_UNITS *PF_GET_DUALCORE_COMPUTE_UNITS;

/**
 * Get the triple core Compute Units.
 *
 * Processors which don't support compute units or triple core compute units return zero.
 *
 * @TopoNbInstances
 *
 * @param[in]   Node     The node for which we want the triple core compute units.
 * @param[in]   Nb       Our Northbridge.
 *
 * @return      The triple core compute units value.
 */
typedef UINT8 F_GET_TRIPLECORE_COMPUTE_UNITS (
  IN       UINT8       Node,
  IN       NORTHBRIDGE *Nb
  );

/// Reference to a method.
typedef F_GET_TRIPLECORE_COMPUTE_UNITS *PF_GET_TRIPLECORE_COMPUTE_UNITS;

/**
 * Get the quad core Compute Units.
 *
 * Processors which don't support compute units or quad core compute units return zero.
 *
 * @TopoNbInstances
 *
 * @param[in]   Node     The node for which we want the quad core compute units.
 * @param[in]   Nb       Our Northbridge.
 *
 * @return      The quad core compute units value.
 */
typedef UINT8 F_GET_QUADCORE_COMPUTE_UNITS (
  IN       UINT8       Node,
  IN       NORTHBRIDGE *Nb
  );

/// Reference to a method.
typedef F_GET_QUADCORE_COMPUTE_UNITS *PF_GET_QUADCORE_COMPUTE_UNITS;

/**
 * Return the number of cores (1 based count) on Node.
 *
 * @TopoNbInstances
 *
 * @param[in]     Node   the Node that will be examined
 * @param[in]     Nb     this northbridge
 *
 * @return        the number of cores
 */
typedef UINT8 F_GET_NUM_CORES_ON_NODE (
  IN       UINT8       Node,
  IN       NORTHBRIDGE *Nb
  );
/// Reference to a method.
typedef F_GET_NUM_CORES_ON_NODE *PF_GET_NUM_CORES_ON_NODE;

/**
 * Write the total number of cores and Nodes to the Node
 *
 * @TopoNbInstances
 *
 * @param[in]     Node         the Node that will be examined
 * @param[in]     TotalNodes   the total number of Nodes
 * @param[in]     TotalCores   the total number of cores
 * @param[in]     Nb           this northbridge
 */
typedef VOID F_SET_TOTAL_NODES_AND_CORES (
  IN       UINT8       Node,
  IN       UINT8       TotalNodes,
  IN       UINT8       TotalCores,
  IN       NORTHBRIDGE *Nb
  );
/// Reference to a method.
typedef F_SET_TOTAL_NODES_AND_CORES *PF_SET_TOTAL_NODES_AND_CORES;

/**
 * Get the Count of Nodes in the system.
 *
 * @TopoNbInstances
 *
 * @param[in] Nb    This Northbridge.
 *
 * @return    The Count (1 based) of Nodes in the system.
 */
typedef UINT8 F_GET_NODE_COUNT (
  IN       NORTHBRIDGE *Nb
  );

/// Reference to a method.
typedef F_GET_NODE_COUNT *PF_GET_NODE_COUNT;

/**
 * Make a compatibility key.
 *
 * @TopoNbInstances
 *
 * @param[in] Node the Node
 * @param[in] Nb   this northbridge
 *
 * @return the key
 */
typedef UINT64 F_MAKE_KEY (
  IN       UINT8       Node,
  IN       NORTHBRIDGE *Nb
  );
/// Reference to a method.
typedef F_MAKE_KEY *PF_MAKE_KEY;

/**
 * The northbridge interface.
 *
 * Abstract the hardware implementation of the processor northbridge.  Feature code does
 * not need to be tailored to specific families.  Also, more than a single family (or
 * model in some cases) can be supported at once.  Multiple family support can be for
 * mixed revisions or for incompatible revisions where only one is used at a time.
 *
 */
struct _NORTHBRIDGE {               // See forward declaration in TopoFeat.h
  PF_GET_NUM_CORES_ON_NODE GetNumCoresOnNode;    /**< Method: Count cores */
  PF_SET_TOTAL_NODES_AND_CORES SetTotalNodesAndCores;    /**< Method: Set Node and Core counts */
  PF_GET_NODE_COUNT GetNodeCount;    /**< Method: Get the Count (1 based) of Nodes in the system. */
  PF_GET_ENABLED_COMPUTE_UNITS GetEnabledComputeUnits; /**< Method: Get the Enabled Compute Units */
  PF_GET_DUALCORE_COMPUTE_UNITS GetDualCoreComputeUnits; /**< Method: Get which Compute Units have two cores. */
  PF_GET_TRIPLECORE_COMPUTE_UNITS GetTripleCoreComputeUnits; /**< Method: Get which Compute Units have triple cores. */
  PF_GET_QUADCORE_COMPUTE_UNITS GetQuadCoreComputeUnits; /**< Method: Get which Compute Units have quad cores. */
  BOOLEAN IsOrderBSPCoresByNode; /**< Property: This processor orders Cores by Node id on the BSP, if TRUE. */
  BOOLEAN IsOrderCoresByModule;  /**< Property: Processors other than the BSP order Cores by Module, if TRUE. */
  UINT64 CompatibleKey;         /**< Property: Used for checking compatibility of northbridges in the system */
  PF_MAKE_KEY MakeKey;    /**< Method: make the compatibility key for this node */
  VOID *ConfigHandle;     /**< Property: Save the StdHeader for implementation use. */
};

/*----------------------------------------------------------------------------
 *                           FUNCTIONS PROTOTYPE
 *
 *----------------------------------------------------------------------------
 */
/**
 * Make a compatibility key.
 *
 */
UINT64
MakeKey (
  IN       UINT8 Node,
  IN       NORTHBRIDGE *Nb
  );

VOID
NewNorthBridge (
  IN       UINT8       Node,
  IN       STATE_DATA  *State,
     OUT   NORTHBRIDGE *Nb
  );

#endif  /* _TOPO_NB_H_ */
