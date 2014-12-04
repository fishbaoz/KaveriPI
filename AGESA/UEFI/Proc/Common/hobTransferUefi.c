/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Hob Transfer functions.
 *
 * Contains code that copy Heap to temp memory or main memory.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  UEFI/Proc/Common
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
 ******************************************************************************
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
 ******************************************************************************
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "Tiano.h"
#include "Pei.h"
#include "PeiLib.h"
#include "AGESA.h"
#include "amdlib.h"
#include "Ids.h"
#include "heapManager.h"
#include "GeneralServices.h"
#include "Filecode.h"

#include EFI_GUID_DEFINITION (AmdHeapHob)
#define FILECODE UEFI_PROC_COMMON_HOBTRANSFERUEFI_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          P U B L I C     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
#define GLOBAL_CPU_UEFI_HOB_LIST_TEMP_ADDR 0x200000ul ///< Fixed the row data at 2M memory address.

UINT64
HeapGetBaseAddressInTempMem (
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CopyHeapToTempRamAtPost
 *
 *     This function copies BSP heap content to RAM
 *
 *    @param[in,out]   StdHeader   - Pointer to AMD_CONFIG_PARAMS struct.
 *
 *    @retval          AGESA_STATUS
 *
 */
AGESA_STATUS
CopyHeapToTempRamAtPost (
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  EFI_PEI_SERVICES    **PeiServices;
  AGESA_STATUS        IgnoredStatus;
  EFI_STATUS          Status;
  UINT32              SizeOfHeapNode;
  UINT32              HeapBaseInCache;
  UINT8               *HeapBaseInTempMem;
  UINT8               *Source;
  UINT8               *Destination;
  UINT8               AlignTo16ByteInCache;
  UINT32              SizeOfNodeData;
  UINT32              TotalSize;
  UINT32              HeapInCacheOffset;
  HEAP_MANAGER        *HeapManagerInCache;
  BUFFER_NODE         *HeapInCache;
  HEAP_MANAGER        *HeapManagerInTempMem;
  BUFFER_NODE         *HeapInTempMem;
  BUFFER_NODE         *EndNodeInTempMem;

  //
  // Only BSP is expected to run this routine
  //
  if (IsBsp (StdHeader, &IgnoredStatus)) {
    if (StdHeader->HeapStatus != HEAP_LOCAL_CACHE) {
      //
      // System memory range is out of PEI, we don't have to copy any thing into HOB.
      //
      return AGESA_UNSUPPORTED;
    }

    PeiServices = (EFI_PEI_SERVICES **) StdHeader->ImageBasePtr;

    Status = EFI_SUCCESS;
    HeapInCache = NULL;
    HeapManagerInCache = NULL;
    SizeOfHeapNode = 0;

    TotalSize = sizeof (HEAP_MANAGER);
    SizeOfNodeData = 0;

    // Heap in cache variables
    HeapBaseInCache = (UINT32) StdHeader->HeapBasePtr;
    HeapManagerInCache = (HEAP_MANAGER *) HeapBaseInCache;
    HeapInCacheOffset = HeapManagerInCache->FirstActiveBufferOffset;
    HeapInCache = (BUFFER_NODE*) (HeapBaseInCache + HeapInCacheOffset);
    // Heap in temp buffer variables
    HeapBaseInTempMem = (UINT8 *) GLOBAL_CPU_UEFI_HOB_LIST_TEMP_ADDR;
    HeapManagerInTempMem = (HEAP_MANAGER *) HeapBaseInTempMem;
    HeapInTempMem = (BUFFER_NODE *) (HeapBaseInTempMem + TotalSize);
    EndNodeInTempMem = HeapInTempMem;

    //
    // Calculate the whole size of heap that need to transferred.
    //
    while (HeapInCacheOffset != AMD_HEAP_INVALID_HEAP_OFFSET) {
      //
      // Copy nodes that will persist in system memory
      //
      if (HeapInCache->Persist > HEAP_LOCAL_CACHE) {
        AlignTo16ByteInCache = HeapInCache->PadSize;

        SizeOfNodeData = HeapInCache->BufferSize - AlignTo16ByteInCache;
        SizeOfHeapNode = SizeOfNodeData + sizeof (BUFFER_NODE);

        TotalSize += SizeOfHeapNode;
        //
        // Copy heap in cache with 16-byte alignment to temp buffer compactly.
        //
        Source = (UINT8 *) HeapInCache + sizeof (BUFFER_NODE) + AlignTo16ByteInCache;
        Destination = (UINT8 *) HeapInTempMem + sizeof (BUFFER_NODE);
        LibAmdMemCopy  (HeapInTempMem, HeapInCache, sizeof (BUFFER_NODE), StdHeader);
        LibAmdMemCopy  (Destination, Source, SizeOfNodeData, StdHeader);
        //
        // Fix buffer size in temp memory which has no 16-byte alignment.
        // Next node offset is the total size used.
        //
        HeapInTempMem->BufferSize = SizeOfNodeData;
        HeapInTempMem->OffsetOfNextNode = TotalSize;
        HeapInTempMem->PadSize = 0;
        EndNodeInTempMem = HeapInTempMem;
        HeapInTempMem = (BUFFER_NODE *) (HeapBaseInTempMem + TotalSize);
      }
      //
      // Point to the next buffer node
      //
      HeapInCacheOffset = HeapInCache->OffsetOfNextNode;
      HeapInCache = (BUFFER_NODE *) (HeapBaseInCache + HeapInCacheOffset);
    }

    //
    // Finalize new heap manager in temp memory
    // No free space node is reserved for temp memory.
    //
    HeapManagerInTempMem->FirstFreeSpaceOffset = AMD_HEAP_INVALID_HEAP_OFFSET;
    if (TotalSize == sizeof (HEAP_MANAGER)) {
      HeapManagerInTempMem->UsedSize = sizeof (HEAP_MANAGER);
      HeapManagerInTempMem->FirstActiveBufferOffset = AMD_HEAP_INVALID_HEAP_OFFSET;
    } else {
      HeapManagerInTempMem->UsedSize = TotalSize;
      HeapManagerInTempMem->FirstActiveBufferOffset = sizeof (HEAP_MANAGER);
      //
      // Last Node has no next node.
      //
      EndNodeInTempMem->OffsetOfNextNode = AMD_HEAP_INVALID_HEAP_OFFSET;
    }
    HeapManagerInCache->Signature = HEAP_SIGNATURE_INVALID;
    HeapManagerInTempMem->Signature = HEAP_SIGNATURE_VALID;

    //
    // Start to copy data into UEFI HOB.
    //
    Status = PeiBuildHobGuidData (PeiServices, &gAmdHeapHobGuid, HeapBaseInTempMem, (UINTN) TotalSize);
    if (EFI_ERROR (Status)) {
      return AGESA_ERROR;
    }
    //
    // Update StdHeader with new heap status and base address
    //
    StdHeader->HeapStatus = HEAP_TEMP_MEM;
    StdHeader->HeapBasePtr = HeapGetBaseAddressInTempMem (StdHeader);
  }
  return AGESA_SUCCESS;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CopyHeapToMainRamAtPost
 *
 *    This function copies Temp Ram heap content to Main Ram
 *
 *    @param[in,out]   StdHeader   - Pointer to AMD_CONFIG_PARAMS struct.
 *
 *    @retval          AGESA_STATUS
 *
 */
AGESA_STATUS
CopyHeapToMainRamAtPost (
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  return AGESA_SUCCESS;
}
