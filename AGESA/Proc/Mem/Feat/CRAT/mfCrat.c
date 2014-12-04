/* $NoKeywords:$ */
/**
 * @file
 *
 * mfCrat.c
 *
 * Feature CRAT table support
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/Main)
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
#include "Ids.h"
#include "heapManager.h"
#include "cpuServices.h"
#include "mm.h"
#include "mn.h"
#include "mu.h"
#include "mt.h"
#include "mfCrat.h"
#include "GeneralServices.h"
#include "Filecode.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)

#define FILECODE PROC_MEM_FEAT_CRAT_MFCRAT_FILECODE
/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */
#define FOURGB 0x010000ul

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

CRAT_MEMORY_AFFINITY_INFO_ENTRY
STATIC
*MakeMemAffinityInfoEntry (
  IN UINT8                            Domain,
  IN UINT64                           Base,
  IN UINT64                           Size,
  IN CRAT_MEMORY_AFFINITY_INFO_ENTRY  *BufferLocPtr
);

UINT32
STATIC
MemFCratApproximateSize (
  IN OUT   MEM_NB_BLOCK        *NBPtr
  );

UINT64
STATIC
GetDctInterleavedMemSize (
  IN OUT   MEM_NB_BLOCK        *NBPtr
  );

UINT64
STATIC
GetBankInterleavedAddress (
  IN OUT   MEM_NB_BLOCK        *NBPtr,
  IN       UINT64              DctBaseAddress,
  IN       UINT8               InterleavingPosition
  );

UINT64
STATIC
GetChannelInterleavedAddress (
  IN OUT   MEM_NB_BLOCK        *NBPtr,
  IN       UINT64              NodeBaseAddress,
  IN       UINT8               InterleavingPosition
  );

UINT64
STATIC
GetBaseAddrOfNonInterleavedRegion (
  IN OUT   MEM_NB_BLOCK        *NBPtr,
  IN       UINT8               DctLimit
  );
/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

BOOLEAN
MemFCratSupport (
  IN OUT   MEM_MAIN_DATA_BLOCK *MemMainPtr
  );

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *  This function gets CRAT memory affinity info and stores the info into heap
 *
 *     @param[in,out]   *MemMainPtr   - Pointer to the MEM_MAIN_DATA_BLOCK
 *
 */
BOOLEAN
MemFCratSupport (
  IN OUT   MEM_MAIN_DATA_BLOCK *MemMainPtr
  )
{
  UINT8    Node;
  UINT8    NodeCount;
  UINT8    Dct;
  UINT8    Cs;
  UINT64   NodeMemBase;
  UINT64   DctMemBase;
  UINT64   DctInterleavedMemSize;
  UINT64   MultipleDctMemBase;
  UINT64   MultipleDctMemSize;
  UINT64   CsSize;
  UINT8    Domain;
  UINT8    NumOfMemAffinityInfoEntries;
  CRAT_MEMORY_AFFINITY_INFO_HEADER *MemAffinityInfoHeaderPtr;
  CRAT_MEMORY_AFFINITY_INFO_ENTRY *MemAffinityInfoEntryPtr;
  ALLOCATE_HEAP_PARAMS AllocHeapParams;
  BOOLEAN  DctInterleaveEnabled;
  BOOLEAN  MultipleDctCombining;

  MEM_NB_BLOCK    *NBPtr;
  MEM_DATA_STRUCT *MemPtr;

  NBPtr     = MemMainPtr->NBPtr;
  MemPtr    = MemMainPtr->MemPtr;

  // Allocate heap for CRAT memory affinity info entry
  AllocHeapParams.RequestedBufferSize = MemFCratApproximateSize (NBPtr);
  AllocHeapParams.BufferHandle = AMD_MEM_CRAT_INFO_BUFFER_HANDLE;
  AllocHeapParams.Persist = HEAP_SYSTEM_MEM;
  if (AGESA_SUCCESS != HeapAllocateBuffer (&AllocHeapParams, &MemPtr->StdHeader)) {
    // Could not allocate heap for CRAT memory affinity info
    PutEventLog (AGESA_CRITICAL, MEM_ERROR_HEAP_ALLOCATE_FOR_CRAT_MEM_AFFINITY, NBPtr->Node, 0, 0, 0, &MemPtr->StdHeader);
    SetMemError (AGESA_CRITICAL, NBPtr->MCTPtr);
    ASSERT (FALSE);
    return FALSE;
  }

  MemAffinityInfoHeaderPtr = (CRAT_MEMORY_AFFINITY_INFO_HEADER *) ((UINT8 *) (AllocHeapParams.BufferPtr));
  MemAffinityInfoHeaderPtr ++;
  MemAffinityInfoEntryPtr  = (CRAT_MEMORY_AFFINITY_INFO_ENTRY *) MemAffinityInfoHeaderPtr;
  MemAffinityInfoHeaderPtr --;

  NumOfMemAffinityInfoEntries = 0;
  NodeCount = NBPtr->NodeCount;
  NodeMemBase = 0;

  for (Node = 0; Node < NodeCount; Node++) {
    NBPtr = &MemMainPtr->NBPtr[Node];
    Domain = Node;
    DctInterleaveEnabled = (NBPtr->GetBitField (NBPtr, BFDctSelIntLvEn) == 0) ? FALSE : TRUE;

    if (DctInterleaveEnabled) {
      //
      // With channel interleaving enabled, one memory affinity entry at least must be created for each channel
      //
      DctInterleavedMemSize = GetDctInterleavedMemSize (NBPtr);

      for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
        MemNSwitchDCTNb (NBPtr, Dct);
        if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
          if (NBPtr->DCTPtr->BkIntDis == FALSE && NBPtr->RefPtr->EnableBankIntlv == TRUE) {
            //
            // With channel interleaving and bank interleaving both enabled,
            // one memory affinity entry must be created for each interleaved region
            //
            ASSERT (NBPtr->DCTPtr->EnabledChipSels > 0);
            CsSize = (NBPtr->DCTPtr->Timings.DctMemSize << 16) / NBPtr->DCTPtr->EnabledChipSels;
            for (Cs = 0; Cs < NBPtr->DCTPtr->EnabledChipSels; Cs ++) {
              MemAffinityInfoEntryPtr = MakeMemAffinityInfoEntry (
                                          Domain,
                                          GetBankInterleavedAddress (NBPtr, GetChannelInterleavedAddress (NBPtr, NodeMemBase, Dct), Cs),
                                          CsSize,
                                          MemAffinityInfoEntryPtr
                                        );
              NumOfMemAffinityInfoEntries ++;
            }
          } else {
            //
            // With bank interleaving disabled on an entirely channel interleaved DCT,
            // a single memory affinity entry is created for the whole DCT
            //
            MemAffinityInfoEntryPtr = MakeMemAffinityInfoEntry (
                                        Domain,
                                        GetChannelInterleavedAddress (NBPtr, NodeMemBase, Dct),
                                        DctInterleavedMemSize,
                                        MemAffinityInfoEntryPtr
                                       );
            NumOfMemAffinityInfoEntries ++;

            if ((NBPtr->DCTPtr->Timings.DctMemSize << 16) > DctInterleavedMemSize) {
              //
              // With bank interleaving disabled on a partially channel interleaved DCT, memory affinity entries
              // are required to be created respectively for the interleaved and non-interleaved regions
              //
              MemAffinityInfoEntryPtr = MakeMemAffinityInfoEntry (
                                          Domain,
                                          NodeMemBase + GetBaseAddrOfNonInterleavedRegion (NBPtr, Dct),
                                          (NBPtr->DCTPtr->Timings.DctMemSize << 16) - DctInterleavedMemSize,
                                          MemAffinityInfoEntryPtr
                                         );
              NumOfMemAffinityInfoEntries ++;
            }
          }
        }
      }
    } else {
      //
      // With channel interleaving disabled, multiple channels could be combined into less memory affinity entries
      //
      DctMemBase = NodeMemBase;
      MultipleDctCombining = FALSE;
      MultipleDctMemBase = DctMemBase;
      MultipleDctMemSize = 0;
      for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
        MemNSwitchDCTNb (NBPtr, Dct);

        if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
          if (NBPtr->DCTPtr->BkIntDis == FALSE && NBPtr->RefPtr->EnableBankIntlv == TRUE) {
            if (MultipleDctCombining == TRUE) {
              //
              // When running into a bank interleaved channel that cannot be combined to a single memory affinity
              // entry, combine the previous adjacent channels first.
              //
              MemAffinityInfoEntryPtr = MakeMemAffinityInfoEntry (
                                          Domain,
                                          MultipleDctMemBase,
                                          MultipleDctMemSize,
                                          MemAffinityInfoEntryPtr
                                        );
              NumOfMemAffinityInfoEntries ++;
              MultipleDctCombining = FALSE;
            }
            //
            // With channel interleaving disabled and bank interleaving enabled,
            // one memory affinity entry must be created for each interleaved region
            //
            ASSERT (NBPtr->DCTPtr->EnabledChipSels > 0);
            CsSize = (NBPtr->DCTPtr->Timings.DctMemSize << 16) / NBPtr->DCTPtr->EnabledChipSels;
            for (Cs = 0; Cs < NBPtr->DCTPtr->EnabledChipSels; Cs ++) {
              MemAffinityInfoEntryPtr = MakeMemAffinityInfoEntry (
                                          Domain,
                                          GetBankInterleavedAddress (NBPtr, DctMemBase, Cs),
                                          CsSize,
                                          MemAffinityInfoEntryPtr
                                        );
              NumOfMemAffinityInfoEntries ++;
            }
          } else {
            //
            // With channel interleaving and bank interleaving both disabled,
            // multiple adjacent channels are combined into a single memory affinity entry
            //
            if (MultipleDctCombining == FALSE) {
              MultipleDctMemBase = DctMemBase;
              MultipleDctMemSize = 0;
              MultipleDctCombining = TRUE;
            }
            MultipleDctMemSize += (NBPtr->DCTPtr->Timings.DctMemSize << 16);
          }
          DctMemBase += (NBPtr->DCTPtr->Timings.DctMemSize << 16);
        }
      }
      if (MultipleDctCombining == TRUE) {
        //
        // Combine and create the last adjacent non-bank-interleaved channels into a single memory affinity entry
        //
        MemAffinityInfoEntryPtr = MakeMemAffinityInfoEntry (
                                    Domain,
                                    MultipleDctMemBase,
                                    MultipleDctMemSize,
                                    MemAffinityInfoEntryPtr
                                  );
        NumOfMemAffinityInfoEntries ++;
        MultipleDctCombining = FALSE;
      }
    }
    NodeMemBase += (NBPtr->MCTPtr->NodeMemSize << 16);
  }

  MemAffinityInfoHeaderPtr->NumOfMemAffinityInfoEntries = NumOfMemAffinityInfoEntries;
  MemAffinityInfoHeaderPtr->MemoryWidth = NBPtr->MemNGetMemoryWidth (NBPtr);

  return TRUE;
}

/*---------------------------------------------------------------------------------------
 *                          L O C A L    F U N C T I O N S
 *---------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 *
 * This function will add Memory entry.
 *
 *  Parameters:
 *    @param[in]  Domain                Proximity Domain
 *    @param[in]  Base                  Memory Base
 *    @param[in]  Size                  Memory Size
 *    @param[in]  BufferLocPtr          Point to the address of buffer
 *
 *    @retval       CRAT_MEMORY_AFFINITY_INFO_ENTRY * (new buffer location ptr)
 */
CRAT_MEMORY_AFFINITY_INFO_ENTRY
STATIC
*MakeMemAffinityInfoEntry (
  IN  UINT8                             Domain,
  IN  UINT64                            Base,
  IN  UINT64                            Size,
  IN  CRAT_MEMORY_AFFINITY_INFO_ENTRY   *BufferLocPtr
  )
{
  BufferLocPtr->Domain = Domain;
  BufferLocPtr->BaseAddressLow = (UINT32) Base;
  BufferLocPtr->BaseAddressHigh = (UINT32) (Base >> 32);
  BufferLocPtr->LengthLow = (UINT32) Size;
  BufferLocPtr->LengthHigh = (UINT32) (Size >> 32);
  BufferLocPtr ++;

  return BufferLocPtr;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function obtains the heap size required to hold the CRAT memory affinity
 *   entry size
 *
 *     @param[in,out]   *NBPtr        - Pointer to the MEM_NB_BLOCK
 *
 */
UINT32
STATIC
MemFCratApproximateSize (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT16    MaxNumOfMemAffinityInfoEntries;

  // At worst case each DCT can be working in an 8-way bank interleaving mode
  // while channel interleaving is disabled
  MaxNumOfMemAffinityInfoEntries = NBPtr->NodeCount * NBPtr->DctCount * 8;

  return MaxNumOfMemAffinityInfoEntries * sizeof (CRAT_MEMORY_AFFINITY_INFO_ENTRY) +
                                          sizeof (CRAT_MEMORY_AFFINITY_INFO_HEADER);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function obtains the interleaved memory size contributed on the
 *   current dct
 *
 *     @param[in,out]   *NBPtr        - Pointer to the MEM_NB_BLOCK
 *
 */
UINT64
STATIC
GetDctInterleavedMemSize (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8  Dct;
  UINT64 DctInterleavedMemSize;

  DctInterleavedMemSize = NBPtr->MCTPtr->NodeMemSize;
  // Find minimum memory size among the interleaved DCTs
  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);
    if (DctInterleavedMemSize > NBPtr->DCTPtr->Timings.DctMemSize) {
      DctInterleavedMemSize = NBPtr->DCTPtr->Timings.DctMemSize;
    }
  }

  return DctInterleavedMemSize << 16;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function obtains the bank interleaved address of an interleavd region
 *
 *     @param[in,out]   *NBPtr               - Pointer to the MEM_NB_BLOCK
 *     @param[in    ]   DctBaseAddress       - Base address of the current DCT
 *     @param[in    ]   InterleavingPosition - Position of the interleaved memory
 *
 */
UINT64
STATIC
GetBankInterleavedAddress (
  IN OUT   MEM_NB_BLOCK        *NBPtr,
  IN       UINT64              DctBaseAddress,
  IN       UINT8               InterleavingPosition
  )
{
  UINT8 i;
  UINT8 j;

  ASSERT (InterleavingPosition < 8);

  NBPtr->TechPtr->GetCSIntLvAddr (NBPtr->DCTPtr->BankAddrMap, &i, &j);
  DctBaseAddress |= (((UINT64)InterleavingPosition) << (i + 8));

  return DctBaseAddress;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function obtains the channel interleaved address of the current DCT
 *
 *     @param[in,out]   *NBPtr               - Pointer to the MEM_NB_BLOCK
 *     @param[in    ]   NodeBaseAddress      - Base address of the current node
 *     @param[in    ]   InterleavingPosition - Position of the interleaved memory
 *
 */
UINT64
STATIC
GetChannelInterleavedAddress (
  IN OUT   MEM_NB_BLOCK        *NBPtr,
  IN       UINT64              NodeBaseAddress,
  IN       UINT8               InterleavingPosition
  )
{
  CONST DCTSELBIT_ENTRY DctSelIntLvMode[] = {
    {0x0, 6},
    {0x1, 12},
    {0x2, 6},
    {0x3, 9},
    {0x4, 8},
    {0x5, 9},
  };
  UINT8 i;

  for (i = 0; i < sizeof (DctSelIntLvMode) / sizeof (DCTSELBIT_ENTRY); i++) {
    if (DctSelIntLvMode[i].DctSelIntLvAddr == NBPtr->DefDctSelIntLvAddr) {
      NodeBaseAddress |= (((UINT64)InterleavingPosition) << DctSelIntLvMode[i].DctSelFunctionBit);
      break;
    }
  }

  ASSERT (i < (sizeof (DctSelIntLvMode) / sizeof (DCTSELBIT_ENTRY)));

  return NodeBaseAddress;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function obtains the base address of a non-interleaved region
 *   on the current DCT
 *
 *     @param[in,out]   *NBPtr               - Pointer to the MEM_NB_BLOCK
 *     @param[in    ]   DctLimit             - Target DCT number
 *
 */
UINT64
STATIC
GetBaseAddrOfNonInterleavedRegion (
  IN OUT   MEM_NB_BLOCK        *NBPtr,
  IN       UINT8               DctLimit
  )
{
  UINT8 Dct;
  UINT8 DctSaved;
  UINT64 DctInterleavedMemSize;
  UINT64 BaseAddrOfNonInterleavedRegion;

  ASSERT (DctLimit < NBPtr->DctCount);

  DctSaved = NBPtr->Dct;
  BaseAddrOfNonInterleavedRegion = 0;
  DctInterleavedMemSize = GetDctInterleavedMemSize (NBPtr);

  //
  // The address space is prioritized for the interleaved regions
  //
  for (Dct = 0; Dct < NBPtr->DctCount; Dct ++) {
    MemNSwitchDCTNb (NBPtr, Dct);
    BaseAddrOfNonInterleavedRegion += DctInterleavedMemSize;
  }
  //
  // Add previous non-interleaved regions
  //
  for (Dct = 0; Dct < DctLimit; Dct ++) {
    MemNSwitchDCTNb (NBPtr, Dct);
    BaseAddrOfNonInterleavedRegion += (NBPtr->DCTPtr->Timings.DctMemSize << 16) - DctInterleavedMemSize;
  }
  // Switch back to the original DCT context
  MemNSwitchDCTNb (NBPtr, DctSaved);

  return BaseAddrOfNonInterleavedRegion;
}
