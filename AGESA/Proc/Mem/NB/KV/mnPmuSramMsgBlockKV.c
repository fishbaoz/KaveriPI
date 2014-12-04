/* $NoKeywords:$ */
/**
 * @file
 *
 * mnPmuSramMsgBlockKV.c
 *
 * Common Northbridge  functions for KV
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/NB/KV)
 * @e \$Revision: 281181 $ @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 **/
/*****************************************************************************
*
* Copyright 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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
#include "Ids.h"
#include "mm.h"
#include "mn.h"
#include "mt.h"
#include "mnreg.h"
#include "mnpmu.h"
#include "merrhdl.h"
#include "mport.h"
#include "mnPmuSramMsgBlockKV.h"
#include "heapManager.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_MEM_NB_KV_MNPMUSRAMMSGBLOCKKV_FILECODE
/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                           TYPEDEFS AND STRUCTURES
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *    Load the PMU SRAM Message Block to DMEM.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return  TRUE - Load the PMU SRAM Message Block successfully.
 *     @return  FALSE - Fail to load the PMU SRAM Message Block.
 */

BOOLEAN
MemNWritePmuSramMsgBlockKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT16 i;
  UINT16 *ImagePtr;
  LOCATE_HEAP_PTR LocHeap;

  LocHeap.BufferHandle = AMD_MEM_PMU_SRAM_MSG_BLOCK_HANDLE;

  if (HeapLocateBuffer (&LocHeap, &(NBPtr->MemPtr->StdHeader)) != AGESA_SUCCESS) {
    return FALSE; // Could not locate heap for PMU SRAM Message BLock.
  }

  ImagePtr = (UINT16 *) LocHeap.BufferPtr;

  MemNSetBitFieldNb (NBPtr, BFPmuReset, 0);

  // Write the word to D18F2x[B,0]9C_x0005_[27FF:0800]_dct[3:0] (using the broadcast port for all phys at
  // D18F2xB98_dct[MaxDctMstr:0], and using the autoincrement feature).
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tStart writing PMU SRAM Message Block...\n");

  MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, PMU_FIRMWARE_SRAM_START | DCT_ACCESS_WRITE | DCT_OFFSET_AUTO_INC_EN);

  for (i = 0; i < sizeof (PMU_SRAM_MSG_BLOCK_KV) / sizeof (ImagePtr[0]); i++) {
    MemNSetBitFieldNb (NBPtr, RegDctAddlData, ImagePtr[i]);
    IDS_HDT_CONSOLE (MEM_SETREG, "~F2_9C_%x = %04x\n", PMU_FIRMWARE_SRAM_START + i, ImagePtr[i]);
  }

  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tEnd writing PMU SRAM Message Block!\n");
  MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0);

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *    Initialize the PMU SRAM Message Block buffer.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return  TRUE - initialize the PMU SRAM Message buffer successfully.
 *     @return  FALSE - Fail to initialize the PMU SRAM Message buffer.
 */

BOOLEAN
MemNInitPmuSramMsgBlockKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  LOCATE_HEAP_PTR LocHeap;
  ALLOCATE_HEAP_PARAMS AllocHeapParams;
  PMU_SRAM_MSG_BLOCK_KV *PmuSramMsgBlockPtr;

  LocHeap.BufferHandle = AMD_MEM_PMU_SRAM_MSG_BLOCK_HANDLE;
  if (HeapLocateBuffer (&LocHeap, &(NBPtr->MemPtr->StdHeader)) == AGESA_SUCCESS) {
    PmuSramMsgBlockPtr = (PMU_SRAM_MSG_BLOCK_KV *) LocHeap.BufferPtr;
  } else {
    // Allocate temporary buffer for PMU SRAM Message Block
    AllocHeapParams.RequestedBufferSize = sizeof (PMU_SRAM_MSG_BLOCK_KV);
    AllocHeapParams.BufferHandle = AMD_MEM_PMU_SRAM_MSG_BLOCK_HANDLE;
    AllocHeapParams.Persist = HEAP_LOCAL_CACHE;

    if (HeapAllocateBuffer (&AllocHeapParams, &(NBPtr->MemPtr->StdHeader)) != AGESA_SUCCESS) {
      return FALSE; // Could not allocate heap for PMU SRAM Message BLock.
    }

    PmuSramMsgBlockPtr = (PMU_SRAM_MSG_BLOCK_KV *) AllocHeapParams.BufferPtr;
  }

  LibAmdMemFill ((VOID *)PmuSramMsgBlockPtr, 0, (UINTN)sizeof (PMU_SRAM_MSG_BLOCK_KV), &(NBPtr->MemPtr->StdHeader));

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *    Free the PMU SRAM Message Block buffer.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return  TRUE - Free PMU SRAM Message buffer successfully.
 *     @return  FALSE - Fail to free PMU SRAM Message buffer.
 */

BOOLEAN
MemNPostPmuSramMsgBlockKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  // De-allocate the PMU SRAM Message Block buffer.
  return (BOOLEAN) (HeapDeallocateBuffer (AMD_MEM_PMU_SRAM_MSG_BLOCK_HANDLE, &(NBPtr->MemPtr->StdHeader)) == AGESA_SUCCESS);
}
