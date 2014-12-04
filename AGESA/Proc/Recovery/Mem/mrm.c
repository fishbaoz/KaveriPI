/* $NoKeywords:$ */
/**
 * @file
 *
 * mrm.c
 *
 * Main configuration for Recovery mode
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Proc/Recovery/Mem)
 * @e \$Revision: 281181 $ @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 **/
/*****************************************************************************
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
#include "Ids.h"
#include "amdlib.h"
#include "GeneralServices.h"
#include "heapManager.h"
#include "cpuServices.h"
#include "OptionMemoryRecovery.h"
#include "mm.h"
#include "mn.h"
#include "mt.h"
#include "Filecode.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)

#define FILECODE PROC_RECOVERY_MEM_MRM_FILECODE
/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */
#define MAX_DIES_PER_SOCKET     2

/*----------------------------------------------------------------------------
 *                           TYPEDEFS AND STRUCTURES
 *
 *----------------------------------------------------------------------------
 */
extern MEM_REC_NB_CONSTRUCTOR* MemRecNBInstalled[];
extern MEM_REC_TECH_CONSTRUCTOR* MemRecTechInstalled[];

/*----------------------------------------------------------------------------
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

VOID
STATIC
MemRecSPDDataProcess (
  IN OUT   MEM_DATA_STRUCT *MemPtr
  );

/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
/* -----------------------------------------------------------------------------*/
/**
 *
 *      This function is the Recovery memory configuration function for HY DDR3
 *
 *      Requirements:
 *
 *      Run-Time Requirements:
 *      1. Complete Hypertransport Bus Configuration
 *      2. AmdMemInitDataStructDef must be run to set default values
 *      3. MSR bit to allow access to high PCI regs set on all nodes
 *      4. BSP in Big Real Mode
 *      5. Stack available
 *      6. MCG_CTL=-1, MC4_EN=0 for all CPUs
 *      7. MCi_STS from shutdown/warm reset recorded (if desired) prior to entry
 *      8. All var MTRRs reset to zero
 *      9. State of NB_CFG.DisDatMsk set properly on all CPUs
 *
 *     @param[in,out]   *MemPtr   - Pointer to the MEM_DATA_STRUCT
 *
 *     @return          AGESA_STATUS
 *                          - AGESA_ALERT
 *                          - AGESA_FATAL
 *                          - AGESA_SUCCESS
 *                          - AGESA_WARNING
 */

AGESA_STATUS
AmdMemRecovery (
  IN OUT   MEM_DATA_STRUCT *MemPtr
  )
{
  UINT8 Socket;
  UINT8 Module;
  UINT8 i;
  AGESA_STATUS AgesaStatus;
  PCI_ADDR Address;
  MEM_NB_BLOCK NBBlock;
  MEM_TECH_BLOCK TechBlock;
  LOCATE_HEAP_PTR  SocketWithMem;
  ALLOCATE_HEAP_PARAMS AllocHeapParams;


  //
  // Read SPD data
  //
  MemRecSPDDataProcess (MemPtr);

  //
  // Get the socket id from heap.
  //
  SocketWithMem.BufferHandle = AMD_REC_MEM_SOCKET_HANDLE;
  if (HeapLocateBuffer (&SocketWithMem, &MemPtr->StdHeader) == AGESA_SUCCESS) {
    Socket = *(UINT8 *) SocketWithMem.BufferPtr;
  } else {
    ASSERT(FALSE);  // Socket handle not found
    return AGESA_FATAL;
  }

  //
  // Allocate buffer for memory init structures
  //
  AllocHeapParams.RequestedBufferSize = MAX_DIES_PER_SOCKET * sizeof (DIE_STRUCT);
  AllocHeapParams.BufferHandle = GENERATE_MEM_HANDLE (ALLOC_DIE_STRUCT_HANDLE, 0, 0, 0);
  AllocHeapParams.Persist = HEAP_LOCAL_CACHE;
  if (HeapAllocateBuffer (&AllocHeapParams, &MemPtr->StdHeader) != AGESA_SUCCESS) {
    ASSERT(FALSE); // Heap allocation failed to allocate Die struct
    return AGESA_FATAL;
  }
  MemPtr->DiesPerSystem = (DIE_STRUCT *)AllocHeapParams.BufferPtr;

  //
  // Discover populated CPUs
  //
  for (Module = 0; Module < MAX_DIES_PER_SOCKET; Module++) {
    if (GetPciAddress ((VOID *)MemPtr, Socket, Module, &Address, &AgesaStatus)) {
      MemPtr->DiesPerSystem[Module].SocketId = Socket;
      MemPtr->DiesPerSystem[Module].DieId = Module;
      MemPtr->DiesPerSystem[Module].PciAddr.AddressValue = Address.AddressValue;
    }
  }

  i = 0;
  while (MemRecNBInstalled[i] != NULL) {
    if (MemRecNBInstalled[i] (&NBBlock, MemPtr, 0) == TRUE) {
      break;
    }
    i++;
  };
  if (MemRecNBInstalled[i] == NULL) {
    ASSERT(FALSE);    // No NB installed
    return AGESA_FATAL;
  }
  MemRecTechInstalled[0] (&TechBlock, &NBBlock);
  NBBlock.TechPtr = &TechBlock;

  return NBBlock.InitRecovery (&NBBlock);
}

/*----------------------------------------------------------------------------
 *                              LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
/* -----------------------------------------------------------------------------*/
/**
 *
 *  This function fills a default SPD buffer with SPD values for all DIMMs installed in the system
 *
 *     @param[in,out]   *MemPtr   - Pointer to the MEM_DATA_STRUCT
 *
 */

VOID
STATIC
MemRecSPDDataProcess (
  IN OUT   MEM_DATA_STRUCT *MemPtr
  )
{
  BOOLEAN FindSocketWithMem;
  UINT8 Channel;
  UINT8 Dimm;
  UINT8 MaxSockets;
  UINT8 *SocketWithMem;
  UINT8 Socket;
  AGESA_STATUS AgesaStatus;
  SPD_DEF_STRUCT *DimmSPDPtr;
  ALLOCATE_HEAP_PARAMS AllocHeapParams;
  AGESA_READ_SPD_PARAMS SpdParam;
  ASSERT (MemPtr != NULL);
  FindSocketWithMem = FALSE;
  //
  // Allocate heap to save socket number with memory on it.
  //
  AllocHeapParams.RequestedBufferSize = sizeof (UINT8);
  AllocHeapParams.BufferHandle = AMD_REC_MEM_SOCKET_HANDLE;
  AllocHeapParams.Persist = HEAP_LOCAL_CACHE;
  if (HeapAllocateBuffer (&AllocHeapParams, &MemPtr->StdHeader) == AGESA_SUCCESS) {
    SocketWithMem = (UINT8 *) AllocHeapParams.BufferPtr;
    *SocketWithMem = 0;

    //
    // Allocate heap for the table
    //
    MaxSockets = (UINT8) GetPlatformNumberOfSockets ();

    AllocHeapParams.RequestedBufferSize = (MaxSockets * MAX_CHANNELS_PER_SOCKET * MAX_DIMMS_PER_CHANNEL * sizeof (SPD_DEF_STRUCT));
    AllocHeapParams.BufferHandle = AMD_MEM_SPD_HANDLE;
    AllocHeapParams.Persist = HEAP_LOCAL_CACHE;
    if (HeapAllocateBuffer (&AllocHeapParams, &MemPtr->StdHeader) == AGESA_SUCCESS) {
      MemPtr->SpdDataStructure = (SPD_DEF_STRUCT *) AllocHeapParams.BufferPtr;
      //
      // Initialize SpdParam Structure
      //
      LibAmdMemCopy ((VOID *)&SpdParam, (VOID *)MemPtr, (UINTN)sizeof (SpdParam.StdHeader), &MemPtr->StdHeader);
      //
      // Populate SPDDataBuffer
      //

      SpdParam.MemData = MemPtr;
      for (Socket = 0; Socket < MaxSockets; Socket ++) {
        SpdParam.SocketId = Socket;
        for (Channel = 0; Channel < MAX_CHANNELS_PER_SOCKET; Channel++) {
          SpdParam.MemChannelId = Channel;
          for (Dimm = 0; Dimm < MAX_DIMMS_PER_CHANNEL; Dimm++) {
            SpdParam.DimmId = Dimm;
            DimmSPDPtr = &(MemPtr->SpdDataStructure[(Socket * MAX_CHANNELS_PER_SOCKET + Channel) * MAX_DIMMS_PER_CHANNEL + Dimm]);
            SpdParam.Buffer = DimmSPDPtr->Data;
            AgesaStatus = AgesaReadSpdRecovery (0, &SpdParam);
            if (AgesaStatus == AGESA_SUCCESS) {
              DimmSPDPtr->DimmPresent = TRUE;
              IDS_HDT_CONSOLE (MEM_FLOW, "SPD Socket %d Channel %d Dimm %d: %08x\n", Socket, Channel, Dimm, SpdParam.Buffer);
              if (!FindSocketWithMem) {
                FindSocketWithMem = TRUE;
              }
            } else {
              DimmSPDPtr->DimmPresent = FALSE;
            }
          }
        }
        if (FindSocketWithMem) {
          *SocketWithMem = Socket;
          break;
        }
      }
    }
  }
}
