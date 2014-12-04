/* $NoKeywords:$ */
/**
 * @file
 *
 * mfidendimm.c
 *
 * Translate physical system address to dimm identification.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/Feat)
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
#include "amdlib.h"
#include "mm.h"
#include "mn.h"
#include "Ids.h"
#include "cpuRegisters.h"
#include "cpuFamRegisters.h"
#include "cpuFamilyTranslation.h"
#include "OptionMemory.h"
#include "heapManager.h"
#include "mfidendimm.h"
#include "GeneralServices.h"
#include "Filecode.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)

#define FILECODE PROC_MEM_FEAT_IDENDIMM_MFIDENDIMM_FILECODE
extern MEM_NB_SUPPORT memNBInstalled[];

/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */
#define MAX_DCTS_PER_DIE        2   ///< Max DCTs per die
#define MAX_CHLS_PER_DCT        1   ///< Max Channels per DCT

/*----------------------------------------------------------------------------
 *                           TYPEDEFS AND STRUCTURES
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
/*-----------------------------------------------------------------------------*/
/**
*
*   This function identifies the dimm on which the given memory address locates.
*
*   @param[in, out]   *AmdDimmIdentify - Pointer to the parameter structure AMD_IDENTIFY_DIMM
*
*   @retval           AGESA_SUCCESS - Successfully translate physical system address
*                                     to dimm identification.
*                     AGESA_BOUNDS_CHK - Targeted address is out of bound.
*
*/

AGESA_STATUS
AmdIdentifyDimm (
  IN OUT   AMD_IDENTIFY_DIMM *AmdDimmIdentify
  )
{
  UINT8 i;
  AGESA_STATUS RetVal;
  MEM_MAIN_DATA_BLOCK mmData;             // Main Data block
  MEM_NB_BLOCK *NBPtr;
  MEM_DATA_STRUCT MemData;
  LOCATE_HEAP_PTR LocHeap;
  ALLOCATE_HEAP_PARAMS AllocHeapParams;
  UINT8 Node;
  UINT8 Dct;
  UINT8 Die;
  UINT8 DieCount;

  LibAmdMemCopy (&(MemData.StdHeader), &(AmdDimmIdentify->StdHeader), sizeof (AMD_CONFIG_PARAMS), &(AmdDimmIdentify->StdHeader));
  mmData.MemPtr = &MemData;
  RetVal = MemSocketScan (&mmData);
  if (RetVal == AGESA_FATAL) {
    return RetVal;
  }
  DieCount = mmData.DieCount;

  // Search for AMD_MEM_AUTO_HANDLE on the heap first.
  // Only apply for space on the heap if cannot find AMD_MEM_AUTO_HANDLE on the heap.
  LocHeap.BufferHandle = AMD_MEM_AUTO_HANDLE;
  if (HeapLocateBuffer (&LocHeap, &AmdDimmIdentify->StdHeader) == AGESA_SUCCESS) {
    // NB block has already been constructed by main block.
    // No need to construct it here.
    NBPtr = (MEM_NB_BLOCK *)LocHeap.BufferPtr;
    mmData.NBPtr = NBPtr;
  } else {
    AllocHeapParams.RequestedBufferSize = (DieCount * (sizeof (MEM_NB_BLOCK)));
    AllocHeapParams.BufferHandle = AMD_MEM_AUTO_HANDLE;
    AllocHeapParams.Persist = HEAP_SYSTEM_MEM;
    if (HeapAllocateBuffer (&AllocHeapParams, &AmdDimmIdentify->StdHeader) != AGESA_SUCCESS) {
      PutEventLog (AGESA_FATAL, MEM_ERROR_HEAP_ALLOCATE_FOR_IDENTIFY_DIMM_MEM_NB_BLOCK, 0, 0, 0, 0, &AmdDimmIdentify->StdHeader);
      ASSERT(FALSE); // Could not allocate heap space for NB block for Identify DIMM
      return AGESA_FATAL;
    }
    NBPtr = (MEM_NB_BLOCK *)AllocHeapParams.BufferPtr;
    mmData.NBPtr = NBPtr;
    // Construct each die.
    for (Die = 0; Die < DieCount; Die ++) {
      i = 0;
      while (memNBInstalled[i].MemIdentifyDimmConstruct != 0) {
        if (memNBInstalled[i].MemIdentifyDimmConstruct (&NBPtr[Die], &MemData, Die)) {
          break;
        }
        i++;
      };
      if (memNBInstalled[i].MemIdentifyDimmConstruct == 0) {
        PutEventLog (AGESA_FATAL, MEM_ERROR_NO_CONSTRUCTOR_FOR_IDENTIFY_DIMM, Die, 0, 0, 0, &AmdDimmIdentify->StdHeader);
        ASSERT(FALSE); // No Identify DIMM constructor found
        return AGESA_FATAL;
      }
    }
  }

  i = 0;
  while (memNBInstalled[i].MemIdentifyDimmConstruct != 0) {
    if ((RetVal = memNBInstalled[i].MemTransSysAddrToCs (AmdDimmIdentify, &mmData)) == AGESA_SUCCESS) {
      // Translate Node, DCT and Chip select number to Socket, Channel and Dimm number.
      Node = AmdDimmIdentify->SocketId;
      Dct = AmdDimmIdentify->MemChannelId;
      AmdDimmIdentify->SocketId = MemData.DiesPerSystem[Node].SocketId;
      AmdDimmIdentify->MemChannelId = NBPtr[Node].GetSocketRelativeChannel (&NBPtr[Node], Dct, 0);
      AmdDimmIdentify->DimmId = AmdDimmIdentify->ChipSelect / 2;
      AmdDimmIdentify->ChipSelect %= 2;
      break;
    }
    i++;
  };

  return RetVal;
}


/*----------------------------------------------------------------------------
 *                              LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

/*-----------------------------------------------------------------------------*/
/**
*
*   This function is the interface to call the PCI register access function
*   defined in NB block.
*
*   @param[in]   *NBPtr - Pointer to the parameter structure MEM_NB_BLOCK
*   @param[in]   NodeID - Node ID number of the target Northbridge
*   @param[in]   DctNum - DCT number if applicable, otherwise, put 0
*   @param[in]   BitFieldName - targeted bitfield
*
*   @retval      UINT32 - 32 bits PCI register value
*
*/
UINT32
MemFGetPCI (
  IN   MEM_NB_BLOCK *NBPtr,
  IN   UINT8 NodeID,
  IN   UINT8 DctNum,
  IN   BIT_FIELD_NAME BitFieldName
  )
{
  MEM_NB_BLOCK *LocalNBPtr;
  UINT8 Die;

  // Find NBBlock that associates with node NodeID
  for (Die = 0; (Die < MAX_NODES_SUPPORTED) && (NBPtr[Die].Node != NodeID); Die ++);
  ASSERT (Die < MAX_NODES_SUPPORTED);

  // Get the northbridge pointer for the targeted node.
  LocalNBPtr = &NBPtr[Die];
  LocalNBPtr->FamilySpecificHook[DCTSelectSwitch] (LocalNBPtr, &DctNum);
  LocalNBPtr->Dct = DctNum;
  // The caller of this function will take care of the ganged/unganged situation.
  // So Ganged is set to be false here, and do PCI read on the DCT specified by DctNum.
  return LocalNBPtr->GetBitField (LocalNBPtr, BitFieldName);
}

/*-----------------------------------------------------------------------------*/
/**
*
*   This function returns an even parity bit (making the total # of 1's even)
*   {0, 1} = number of set bits in argument is {even, odd}.
*
*   @param[in]   address - the address on which the parity bit will be calculated
*
*   @retval      UINT8 - parity bit
*
*/

UINT8
MemFUnaryXOR (
  IN   UINT32 address
  )
{
  UINT8 parity;
  UINT8 index;
  parity = 0;
  for (index = 0; index < 32; ++ index) {
    parity = (UINT8) (parity ^ (address & 0x1));
    address = address >> 1;
  }
  return parity;
}
