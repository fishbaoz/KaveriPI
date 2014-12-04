/* $NoKeywords:$ */
/**
 * @file
 *
 * mfmchi.c
 *
 * Feature Multi-channel interleaving support (more than 2 channels)
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/Feat/Chintlv)
 * @e \$Revision: 284159 $ @e \$Date: 2014-02-04 16:49:35 -0600 (Tue, 04 Feb 2014) $
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
#include "mm.h"
#include "mn.h"
#include "mu.h"
#include "Ids.h"
#include "GeneralServices.h"
#include "Filecode.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)

#define FILECODE PROC_MEM_FEAT_CHINTLV_MFMCHI_FILECODE
/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */

#define _4GB_ (0x10000ul >> 10)
#define _16MB_RJ16  0x0100

#define DCT_ADDR_VAL      0
#define LGCY_MMIO_HOLE_EN 1
#define DCT_OFFSET_EN     3
#define DCT_SEL           4

#define FIX_DCT(Dct)  ((UINT8) (((NBPtr->IsSupported[MasterDct1Relocate]) && ((Dct) == 1)) ? 3 : (Dct)))

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

/* -----------------------------------------------------------------------------*/
/**
 *
 *  Applies DIMM channel interleaving if enabled. Called once per Node.
 *  This function is dedicated for the memory controller that has DRAM Controller Base/Limit registers
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          TRUE -  This feature is enabled.
 *     @return          FALSE - This feature is not enabled.
 */

BOOLEAN
MemFInterleaveMultiChannels (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8  Dct;
  UINT8  Range;
  UINT8  MaxRangesUsed;
  UINT8  WeReMask;
  UINT8  DctIntLvMap;
  UINT8  DctIntLvEn;
  UINT8  IntLvDctCnt;
  UINT8  DctOffsetEn;
  UINT8  DctSelIntLvAddr;
  UINT32 IntLvMemSize;
  UINT32 BottomIo;
  UINT32 NodeMemSize;
  UINT32 NodeSysBase;
  UINT32 NodeSysLimit;
  UINT32 RangeMemSize;
  UINT32 DctSysBase;
  UINT32 DctSysLimit;
  UINT32 DctBaseReg;
  UINT32 DctLimitReg;
  UINT32 HoleOffset;
  MEM_PARAMETER_STRUCT *RefPtr;
  DIE_STRUCT *MCTPtr;

  RefPtr = NBPtr->RefPtr;
  MCTPtr = NBPtr->MCTPtr;

  //
  // Physical addresses in this function are right adjusted by 16 bits ([47:16])
  // They are BottomIO, HoleOffset, DctSelBaseAddr, NodeSysBase, NodeSysLimit.
  //

  // Exit if channel interleaving is not requested
  if (!(RefPtr->EnableChannelIntlv)) {
    return FALSE;
  }

  //
  // Scan through all DCTs to determine interleavable DCTs
  //
  IntLvDctCnt = 0;
  DctIntLvMap = 0;
  IntLvMemSize = 0x40000000;
  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);
    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      if (IntLvMemSize > NBPtr->DCTPtr->Timings.DctMemSize) {
        IntLvMemSize = NBPtr->DCTPtr->Timings.DctMemSize;
      }
      DctIntLvMap |= (UINT8) 1 << FIX_DCT (Dct);
      IntLvDctCnt++;
    }
  }

  //
  // Exit if number of channels is odd
  //
  if ((IntLvDctCnt < 2) || (!IS_POWER_OF_2 (IntLvDctCnt))) {
    PutEventLog (AGESA_WARNING, MEM_WARNING_CHANNEL_INTERLEAVING_NOT_ENABLED, NBPtr->Node, 0, 0, 0, &NBPtr->MemPtr->StdHeader);
    SetMemError (AGESA_WARNING, MCTPtr);
    return FALSE;
  }

  // Enforce bottom of IO to be 128MB aligned
  BottomIo = (RefPtr->BottomIo & 0xF8) << 8;

  NodeMemSize = 0;
  NodeSysBase = NBPtr->SharedPtr->CurrentNodeSysBase;
  NodeSysLimit = 0;
  DctSysBase = NBPtr->SharedPtr->CurrentNodeSysBase;
  DctSysLimit = 0;
  Dct = 0;
  DctOffsetEn = 0;
  RangeMemSize = 0;
  WeReMask = 0;
  MaxRangesUsed = 4;

  for (Range = 0; Range < MaxRangesUsed; Range++) {
    // When UMA is enabled, use the first range for interleaved region.
    // Otherwise, use the last range for interleaved region.
    RangeMemSize = IntLvMemSize * IntLvDctCnt;
    DctIntLvEn = DctIntLvMap;

    // For other cases, Use other ranges to map oversized DCTs
    if ((RefPtr->UmaMode == UMA_NONE) || (Range > 0)) {
      while (Dct < NBPtr->DctCount) {
        MemNSwitchDCTNb (NBPtr, Dct);
        if (IntLvMemSize < NBPtr->DCTPtr->Timings.DctMemSize) {
          RangeMemSize = NBPtr->DCTPtr->Timings.DctMemSize - IntLvMemSize;
          DctIntLvEn = 0;
          break;
        }
        Dct++;
      }
    }

    if (Dct >= NBPtr->DctCount) {
      if (RefPtr->UmaMode != UMA_NONE) {
        // When UMA is enabled, interleaved region has been mapped in range 0, exit loop
        break;
      } else {
        // When UMA is disabled, keep the loop running one more time to map interleaved region
        Dct = 0;
        DctOffsetEn = (Range == 0) ? 0 : 1;
        MaxRangesUsed = 0;
      }
    }

    NodeMemSize += RangeMemSize;
    DctSysLimit = DctSysBase + RangeMemSize - 1;

    DctBaseReg = (DctSysBase & 0xFFFFF800) | (FIX_DCT (Dct) << DCT_SEL) | (DctOffsetEn << DCT_OFFSET_EN) | (1 << DCT_ADDR_VAL);
    DctLimitReg = (DctSysLimit & 0xFFFFF800) | (DctIntLvEn);

    if ((DctSysBase < BottomIo) && (DctSysLimit >= BottomIo)) {
      // HW Dram Remap
      MCTPtr->Status[SbHWHole] = TRUE;
      RefPtr->GStatus[GsbHWHole] = TRUE;
      MCTPtr->NodeHoleBase = BottomIo;
      RefPtr->HoleBase = BottomIo;

      HoleOffset = (_4GB_RJ16 - BottomIo) + DctSysBase;

      NodeSysLimit = _4GB_RJ16 - BottomIo;

      DctLimitReg += _4GB_RJ16 - BottomIo;
      DctSysLimit += _4GB_RJ16 - BottomIo;

      MemNSetBitFieldNb (NBPtr, BFDramHoleBase, BottomIo >> 8);
      MemNSetBitFieldNb (NBPtr, BFDramHoleOffset, HoleOffset >> 7);
      MemNSetBitFieldNb (NBPtr, BFDramMemHoistValid, 1);

      DctBaseReg |= (1 << LGCY_MMIO_HOLE_EN);
    } else if (DctSysBase == BottomIo) {
      if (NodeSysBase == BottomIo) {
        // SW Node Hoist
        MCTPtr->Status[SbSWNodeHole] = TRUE;
        RefPtr->GStatus[GsbSpIntRemapHole] = TRUE;
        RefPtr->GStatus[GsbSoftHole] = TRUE;

        NodeSysBase = _4GB_RJ16;
      }

      RefPtr->HoleBase = BottomIo;
      NodeSysLimit += _4GB_RJ16 - BottomIo;

      DctBaseReg += _4GB_RJ16 - BottomIo;
      DctLimitReg += _4GB_RJ16 - BottomIo;
      DctSysLimit += _4GB_RJ16 - BottomIo;

      // Setup HW hoisting for the node if a MMIO hole is in its dram base/limit range
      if (!MCTPtr->Status[SbSWNodeHole]) {
        MCTPtr->Status[SbHWHole] = TRUE;
        RefPtr->GStatus[GsbHWHole] = TRUE;
        MCTPtr->NodeHoleBase = BottomIo;
        MemNSetBitFieldNb (NBPtr, BFDramHoleBase, BottomIo >> 8);
        MemNSetBitFieldNb (NBPtr, BFDramHoleOffset, 0);
        MemNSetBitFieldNb (NBPtr, BFDramMemHoistValid, 1);
      }
    } else {
      // No Remapping.  Normal Contiguous mapping
    }

    MemNSetBitFieldNb (NBPtr, BFDctBaseReg0 + Range, DctBaseReg);
    MemNSetBitFieldNb (NBPtr, BFDctLimitReg0 + Range, DctLimitReg);

    // Set DctHighAddrOffset for oversized DCTs
    if (DctIntLvEn == 0) {
      MemNSetBitFieldNb (NBPtr, BFDctHighAddressOffsetReg0 + FIX_DCT (Dct), (RefPtr->UmaMode != UMA_NONE) ? IntLvMemSize : RangeMemSize);
      Dct++;
    } else {
      // After mapping interleaved region, all ranges will need DctOffsetEn = 1
      DctOffsetEn = 1;
    }

    DctSysBase = (DctSysLimit + 1) & 0xFFFFFFF0;
  }

  // Set DCT address bit
  DctSelIntLvAddr = NBPtr->DefDctSelIntLvAddr;
  IDS_OPTION_HOOK (IDS_CHANNEL_INTERLEAVE, &DctSelIntLvAddr, &(NBPtr->MemPtr->StdHeader));
  MemNSetBitFieldNb (NBPtr, BFDctSelIntLvAddr, DctSelIntLvAddr);

  MemNSetBitFieldNb (NBPtr, BFBankSwapAddr8En, ((MemNGetBitFieldNb (NBPtr, BFBankSwap) != 0) && (DctSelIntLvAddr == 4)) ? 1 : 0);

  NodeSysLimit += NodeMemSize - 1;

  MCTPtr->NodeSysBase = NodeSysBase;
  MCTPtr->NodeSysLimit = NodeSysLimit;
  RefPtr->SysLimit = MCTPtr->NodeSysLimit;
  NBPtr->SharedPtr->TopNode = NBPtr->Node;

  NBPtr->SharedPtr->NodeMap[NBPtr->Node].IsValid = TRUE;
  NBPtr->SharedPtr->NodeMap[NBPtr->Node].SysBase = NodeSysBase;
  NBPtr->SharedPtr->NodeMap[NBPtr->Node].SysLimit = NodeSysLimit & 0xFFFFFF00;
  NBPtr->SharedPtr->CurrentNodeSysBase = (NodeSysLimit + 1) & 0xFFFFFFF0;

  // Set the Dram base and Dram limit here when single node
  if (NBPtr->NodeCount == 1) {
    WeReMask = 3;
    // Set the Dram base and set the WE and RE flags in the base.
    MemNSetBitFieldNb (NBPtr, BFDramBaseReg0, (NodeSysBase << 8) | WeReMask);
    MemNSetBitFieldNb (NBPtr, BFDramBaseHiReg0, NodeSysBase >> 24);
    // Set the Dram limit and set DstNode.
    MemNSetBitFieldNb (NBPtr, BFDramLimitReg0, ((NodeSysLimit << 8) & 0xFFFF0000));
    MemNSetBitFieldNb (NBPtr, BFDramLimitHiReg0, NodeSysLimit >> 24);
  }
  MemNSetBitFieldNb (NBPtr, BFDramBaseAddr, NodeSysBase >> (27 - 16));
  MemNSetBitFieldNb (NBPtr, BFDramLimitAddr, NodeSysLimit >> (27 - 16));

  return TRUE;
}
