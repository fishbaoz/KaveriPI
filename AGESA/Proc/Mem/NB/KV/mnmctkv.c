/* $NoKeywords:$ */
/**
 * @file
 *
 * mnmctkv.c
 *
 * Northbridge KV MCT supporting functions
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/NB/KV)
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
#include "heapManager.h"
#include "Ids.h"
#include "mport.h"
#include "Gnb.h"
#include "mm.h"
#include "mn.h"
#include "mnreg.h"
#include "OptionMemory.h"
#include "mnkv.h"
#include "merrhdl.h"
#include "cpuFeatures.h"
#include "Filecode.h"
#include "mftds.h"
#include "mu.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)


#define FILECODE PROC_MEM_NB_KV_MNMCTKV_FILECODE
/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */
#define DCT_ADDR_VAL      0
#define LGCY_MMIO_HOLE_EN 1
#define DCT_SEL           4
#define ACP_ENGINE_SIZE_4MB_RJ16  0x0040

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
extern BUILD_OPT_CFG UserOptions;

/* -----------------------------------------------------------------------------*/
/**
 *
 *      This function programs DDR mode to DDR3 for all DCTs
 *
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNSetDdrModeD3KV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8   Dct;
  BOOLEAN SwapDct1Dct3;

  // In the case DDR3 with Graphics downcore disabled, channel B is controlled by DCT3
  if (MemNGetBitFieldNb (NBPtr, BFDctEn) != 3) {
    SwapDct1Dct3 = TRUE;
  } else {
    SwapDct1Dct3 = FALSE;
  }

  // According to BKDG 0.94, DCT 1 does not exist. So force SwapDct1Dct3, but keep the current code to prepare for future changes.
  SwapDct1Dct3 = TRUE;

  // Set DramType and disable unused DCTs
  for (Dct = 0; Dct < MAX_DCTS_PER_NODE_KV; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);

    MemNSetBitFieldNb (NBPtr, BFDramType, DRAM_TYPE_DDR3_KV);

    if ((Dct == 0) || ((Dct == 1) && !SwapDct1Dct3) || ((Dct == 3) && SwapDct1Dct3)) {
      MemNSetBitFieldNb (NBPtr, BFChannelEn, 1);
    } else {
      MemNSetBitFieldNb (NBPtr, BFDisDramInterface, 1);
    }
  }

  // After disable unused DCTs, change the number of DCTs to 2 for DDR3 mode
  NBPtr->DctCount = MAX_D3_DCTS_PER_NODE_KV;

  // In case of Graphics downcore, set flag to redirect all DCT 1 accesses to DCT 3 from this point forward
  NBPtr->IsSupported[MasterDct1Relocate] = SwapDct1Dct3;

  // Program bit fields before memory init
  MemNSetBitFieldNb (NBPtr, BFEnSplitMctDatBuffers, 1);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function switches memory pstate for KV
 *
 *     @param[in,out]   *NBPtr    - Pointer to the MEM_NB_BLOCK
 *     @param[in]       MemPstate - Mem Pstate
 *
 * ----------------------------------------------------------------------------
 */
VOID
MemNSwitchMemPstateKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 MemPstate
  )
{
  ASSERT (MemPstate < 2);   // Only support 2 memory pstates
  IDS_HDT_CONSOLE (MEM_FLOW, "MemPsSel = %d\n", MemPstate);
  MemNSetBitFieldNb (NBPtr, BFMemPsSel, MemPstate);
  NBPtr->MemPstate = MemPstate;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function create the HT memory map
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return  TRUE  - indicates that a FATAL error has not occurred
 *     @return  FALSE - indicates that a FATAL error has occurred
 *
 * ----------------------------------------------------------------------------
 */
BOOLEAN
MemNHtMemMapKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8  Dct;
  UINT8  DctIndex;
  UINT8  Range;
  UINT8  WeReMask;
  UINT32 BottomIo;
  UINT32 NodeMemSize;
  UINT32 NodeSysBase;
  UINT32 NodeSysLimit;
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

  // Enforce bottom of IO to be 128MB aligned
  BottomIo = (RefPtr->BottomIo & 0xF8) << 8;

  NodeMemSize = 0;
  NodeSysBase = 0;
  NodeSysLimit = 0;
  DctSysBase = 0;
  DctSysLimit = 0;
  Range = 0;
  HoleOffset = 0;
  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    DctIndex = NBPtr->DctCount - 1 - Dct;
    IDS_SKIP_HOOK (IDS_BEFORE_HT_MEM_MAP, NBPtr, &(NBPtr->MemPtr->StdHeader)) {
      DctIndex = NBPtr->DctCount - 1 - DctIndex;
    }
    MemNSwitchDCTNb (NBPtr, DctIndex);

    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      NodeMemSize += NBPtr->DCTPtr->Timings.DctMemSize;
      DctSysLimit = DctSysBase + NBPtr->DCTPtr->Timings.DctMemSize - 1;

      DctBaseReg = (DctSysBase & 0xFFFFF800) | (FIX_DCT (DctIndex) << DCT_SEL) | (1 << DCT_ADDR_VAL);
      DctLimitReg = (DctSysLimit & 0xFFFFF800);

      if ((DctSysBase <= BottomIo) && (DctSysLimit >= BottomIo)) {
        // HW Dram Remap
        MCTPtr->Status[SbHWHole] = TRUE;
        RefPtr->GStatus[GsbHWHole] = TRUE;
        MCTPtr->NodeHoleBase = BottomIo;
        RefPtr->HoleBase = BottomIo;

        if (DctSysBase != BottomIo) {
          HoleOffset = (_4GB_RJ16 - BottomIo) + DctSysBase;
          DctBaseReg |= (1 << LGCY_MMIO_HOLE_EN);
        } else {
          DctBaseReg += _4GB_RJ16 - BottomIo;
        }

        NodeSysLimit = _4GB_RJ16 - BottomIo;

        DctLimitReg += _4GB_RJ16 - BottomIo;
        DctSysLimit += _4GB_RJ16 - BottomIo;

        MemNSetBitFieldNb (NBPtr, BFDramHoleBase, BottomIo >> 8);
        MemNSetBitFieldNb (NBPtr, BFDramHoleOffset, HoleOffset >> 7);
        MemNSetBitFieldNb (NBPtr, BFDramMemHoistValid, 1);
        MemNSetBitFieldNb (NBPtr, BFDramHoleValid, 1);
      }

      MemNSetBitFieldNb (NBPtr, BFDctBaseReg0 + Range, DctBaseReg);
      MemNSetBitFieldNb (NBPtr, BFDctLimitReg0 + Range, DctLimitReg);
      Range++;

      DctSysBase = (DctSysLimit + 1) & 0xFFFFFFF0;
    }
  }

  NodeSysLimit += NodeMemSize - 1;

  MCTPtr->NodeSysBase = NodeSysBase;
  MCTPtr->NodeSysLimit = NodeSysLimit;
  RefPtr->SysLimit = MCTPtr->NodeSysLimit;

  WeReMask = 3;
  // Set the Dram base and set the WE and RE flags in the base.
  MemNSetBitFieldNb (NBPtr, BFDramBaseReg0, (NodeSysBase << 8) | WeReMask);
  MemNSetBitFieldNb (NBPtr, BFDramBaseHiReg0, NodeSysBase >> 24);
  // Set the Dram limit and set DstNode.
  MemNSetBitFieldNb (NBPtr, BFDramLimitReg0, ((NodeSysLimit << 8) & 0xFFFF0000));
  MemNSetBitFieldNb (NBPtr, BFDramLimitHiReg0, NodeSysLimit >> 24);

  MemNSetBitFieldNb (NBPtr, BFDramBaseAddr, NodeSysBase >> (27 - 16));
  MemNSetBitFieldNb (NBPtr, BFDramLimitAddr, NodeSysLimit >> (27 - 16));

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function does synchronous channel initialization
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNSyncChannelInitKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 Dct;

  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);

    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      MemNSetBitFieldNb (NBPtr, BFPtrInitReq, 1);
      MemNPollBitFieldNb (NBPtr, BFPtrInitReq, 0, PCI_ACCESS_TIMEOUT, FALSE);
    }
  }

  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);

    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      MemNSetBitFieldNb (NBPtr, BFExitSelfRef, 1);
      MemNPollBitFieldNb (NBPtr, BFExitSelfRef, 0, PCI_ACCESS_TIMEOUT, FALSE);
      MemUWait10ns ((7800 * 2) / 10, NBPtr->MemPtr);  // wait Tref * 2
      MemNSetBitFieldNb (NBPtr, BFChanVal, 1);
    }
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  This function obtains the memory frequency in the current context
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */
UINT16
MemNGetMemClkFreqInCurrentContextKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 Dct;
  UINT16 MemClkSpeed;
  CONST BIT_FIELD_NAME MemPstateBF[4] = {BFMemPstate0, BFMemPstate1, BFMemPstate2, BFMemPstate3};

  for (Dct = 0; Dct < MAX_DCTS_PER_NODE_KV; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);
    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      break;
    }
  }

  if (MemNGetBitFieldNb (NBPtr, MemPstateBF[MemNGetBitFieldNb (NBPtr, BFNbPsSel)]) == 0) {
    MemClkSpeed = NBPtr->DCTPtr->Timings.Speed;
  } else {
    MemClkSpeed = MemNGetMemClkFreqUnb (NBPtr, (UINT8) MemNGetBitFieldNb (NBPtr, BFM1MemClkFreq));
  }
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tMemclk Freq: %d\n", MemClkSpeed);

  return MemClkSpeed;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  This function calculates and programs NB P-state dependent registers for KV
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNProgramNbPstateDependentRegistersKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 Dct;

  NBPtr->GetMemClkFreqInCurrentContext (NBPtr);

  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);

    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      MemNSetBitFieldNb (NBPtr, BFDataTxFifoWrDly, 0);
    }
  }

  MemFInitTableDrive (NBPtr, MTAfterNbPstateChange);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function releases the NB P-state force for KV
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   OptParam - Optional parameter
 *
 *     @return    TRUE
 */
BOOLEAN
MemNReleaseNbPstateKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  )
{
  // 6. Restore the initial D18F5x170[SwNbPstateLoDis, NbPstateDisOnP0] values.
  MemNSetBitFieldNb (NBPtr, BFNbPstateCtlReg, (MemNGetBitFieldNb (NBPtr, BFNbPstateCtlReg) & 0xFFFF9FFF) | (NBPtr->NbPsCtlReg & 0x6000));
  // 7. Restore the initial D18F5x170[NbPstateThreshold, NbPstateHi] values.
  MemNSetBitFieldNb (NBPtr, BFNbPstateCtlReg, (MemNGetBitFieldNb (NBPtr, BFNbPstateCtlReg) & 0xFFFFF13F) | (NBPtr->NbPsCtlReg & 0x0EC0));
  // 8. Restore the initial D18F5x170[NbPstateLo] values.
  MemNSetBitFieldNb (NBPtr, BFNbPstateLo, (NBPtr->NbPsCtlReg >> 3) & 3);

  // Clear NbPsSel to 0
  MemNSetBitFieldNb (NBPtr, BFNbPsSel, 0);

  return TRUE;
}


/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *     Report the Uma size that is going to be allocated.
 *     Total system memory   UMASize
 *     >= 6G                 1024M
 *     >= 4G but < 6G         512M
 *     < 4G                   256M
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          Uma size [31:0] = Addr [47:16]
 */
UINT32
MemNGetUmaSizeKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT32 SysMemSize;
  UINT32 SizeOfUma;

  SysMemSize = NBPtr->RefPtr->SysLimit + 1;
  SysMemSize = (SysMemSize + 0x100) & 0xFFFFF000;   // Ignore 16MB allocated for C6 when finding UMA size

  if (SysMemSize > 0x10000) {
    //
    // If SysLimit is > 4GB then calculate the SysMemSize by Adjusting the BottomIo
    //
    SysMemSize = (SysMemSize - 0x10000) + (NBPtr->RefPtr->BottomIo << 8);
    if (SysMemSize >= 0x10000 && SysMemSize < 0x18000) {
      SizeOfUma = 512 << (20 - 16);       // If SystemMemSize is >= 4GB but < 6 GB
    } else {
      SizeOfUma = 1024 << (20 - 16);      // If SystemMemSize is >= 6 GB
    }
  } else {
    SizeOfUma = 256 << (20 - 16);         // If SystemMemSize is < 4 GB
  }

  return SizeOfUma;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function allocates memory storage for C6 and ACP Engine when
 *     it is requested to be enabled
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */
VOID
MemNAllocateC6AndAcpEngineStorageKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT32 SysLimit;
  UINT32 DramLimitReg;
  ALLOCATE_HEAP_PARAMS AllocHeapParams;
  ACP_ENGINE_T *AcpEnginePtr;
  UINT32 AcpEngineBaseRJ16;
  UINT32 AcpEngineSizeRJ16;
  BOOLEAN AcpEngineFeature;

  AcpEngineFeature = FALSE;
  AcpEngineSizeRJ16 = 0;

  if (NBPtr->MemPtr->PlatFormConfig->AcpSize != NO_ACP_SIZE) {
    AcpEngineFeature = TRUE;

    // AcpSize is Right Justify by 16.
    AcpEngineSizeRJ16 = NBPtr->MemPtr->PlatFormConfig->AcpSize;
  }

  // SysLimit is Right Justify by 16.
  SysLimit = NBPtr->RefPtr->SysLimit;

  // Both CC6 and ACP Engine are enabled.
  if (IsFeatureEnabled (C6Cstate, NBPtr->MemPtr->PlatFormConfig, &(NBPtr->MemPtr->StdHeader)) && AcpEngineFeature) {
    // Senity check on ACP Engine size.
    ASSERT (AcpEngineSizeRJ16 <= ACP_ENGINE_SIZE_4MB_RJ16);

    if (!NBPtr->SharedPtr->C6Enabled && !NBPtr->SharedPtr->AcpEngineEnabled) {
      // System memory available is reduced by 32MB.
      // CC6 is 16MB and ACP Engine is 2MB or 4MB.  Since we use one
      // MTRR, it need to set to 32MB because of the power of 2.
      // Pretty much memory wasted up to 14MB.
      SysLimit -= _32MB_RJ16;

      NBPtr->MCTPtr->NodeSysLimit = SysLimit;
      NBPtr->RefPtr->SysLimit = SysLimit;
      NBPtr->SharedPtr->AcpEngineEnabled = TRUE;
      NBPtr->SharedPtr->C6Enabled = TRUE;

      // Set TOPMEM and MTRRs (only need to be done once for BSC)
      MemNC6AndGnbAcpEngineAdjustMSRs (NBPtr);

      // Allocate heap for ACP Engine
      AllocHeapParams.RequestedBufferSize = sizeof (ACP_ENGINE_T);
      AllocHeapParams.BufferHandle = AMD_GNB_ACP_ENGINE_HANDLE;
      AllocHeapParams.Persist = HEAP_SYSTEM_MEM;

      if (AGESA_SUCCESS != HeapAllocateBuffer (&AllocHeapParams, &(NBPtr->MemPtr->StdHeader))) {
        ASSERT(FALSE); // Could not allocate heap for ACP Engine.
      }

      AcpEnginePtr = (ACP_ENGINE_T *) AllocHeapParams.BufferPtr;

      // ACP Engine sit above the CC6 and CC6 is 16MB storage.
      AcpEngineBaseRJ16 = SysLimit + 1 + _16MB_RJ16;
      AcpEnginePtr->AcpEngineBase = (UINT64) ((UINT64) AcpEngineBaseRJ16 << 16);
      AcpEnginePtr->AcpEngineSize = AcpEngineSizeRJ16 << 16;
    }

    // Set BFCC6SaveEn
    MemNSetBitFieldNb (NBPtr, BFCC6SaveEn, 1);
    // LockDramCfg will be set in FinalizeMCT
  // Only CC6 is enabled.
  } else if (IsFeatureEnabled (C6Cstate, NBPtr->MemPtr->PlatFormConfig, &(NBPtr->MemPtr->StdHeader)))  {
    // Calculate new SysLimit
    if (!NBPtr->SharedPtr->C6Enabled) {
      // System memory available is reduced by 16MB.
      SysLimit -= _16MB_RJ16;

      NBPtr->MCTPtr->NodeSysLimit = SysLimit;
      NBPtr->RefPtr->SysLimit = SysLimit;
      NBPtr->SharedPtr->C6Enabled = TRUE;

      // Set TOPMEM and MTRRs (only need to be done once for BSC)
      MemNC6AdjustMSRs (NBPtr);
    }

    // Set BFCC6SaveEn
    MemNSetBitFieldNb (NBPtr, BFCC6SaveEn, 1);
    // LockDramCfg will be set in FinalizeMCT
  // Only ACP Engine is enabled.
  } else if (AcpEngineFeature) {
    // Senity check on ACP Engine size.
    ASSERT (AcpEngineSizeRJ16 <= ACP_ENGINE_SIZE_4MB_RJ16);

    // Calculate new SysLimit
    if (!NBPtr->SharedPtr->AcpEngineEnabled) {
      // System memory available is reduced by 16MB.
      SysLimit -= _16MB_RJ16;

      NBPtr->MCTPtr->NodeSysLimit = SysLimit;
      NBPtr->RefPtr->SysLimit = SysLimit;
      NBPtr->SharedPtr->AcpEngineEnabled = TRUE;

      // Set TOPMEM and MTRRs (only need to be done once for BSC)
      MemNC6AdjustMSRs (NBPtr);

      // Allocate heap for ACP Engine
      AllocHeapParams.RequestedBufferSize = sizeof (ACP_ENGINE_T);
      AllocHeapParams.BufferHandle = AMD_GNB_ACP_ENGINE_HANDLE;
      AllocHeapParams.Persist = HEAP_SYSTEM_MEM;

      if (AGESA_SUCCESS != HeapAllocateBuffer (&AllocHeapParams, &(NBPtr->MemPtr->StdHeader))) {
        ASSERT(FALSE); // Could not allocate heap for ACP Engine.
      }

      AcpEnginePtr = (ACP_ENGINE_T *) AllocHeapParams.BufferPtr;
      // ACP Engine sit above TOM.
      AcpEngineBaseRJ16 = SysLimit + 1;
      AcpEnginePtr->AcpEngineBase = (UINT64) ((UINT64) AcpEngineBaseRJ16 << 16);
      AcpEnginePtr->AcpEngineSize = AcpEngineSizeRJ16 << 16;
    }
  }

  if (NBPtr->SharedPtr->C6Enabled || NBPtr->SharedPtr->AcpEngineEnabled) {
    // Set Dram Limit
    DramLimitReg = MemNGetBitFieldNb (NBPtr, BFDramLimitReg0) & 0x0000FFFF;
    MemNSetBitFieldNb (NBPtr, BFDramLimitReg0, ((SysLimit << 8) & 0xFFFF0000) | DramLimitReg);
    MemNSetBitFieldNb (NBPtr, BFDramLimitHiReg0, SysLimit >> 24);
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function fixes up UMA info
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   OptParam - Optional parameter
 *
 *     @return    TRUE
 */
BOOLEAN
MemNFixupUmaInfoKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  )
{
  UMA_INFO *UmaInfoPtr;
  UINT64   Limit;
  UINT8    IntlvEn;
  UINT8    DctSel;
  UINT8    Range;

  UmaInfoPtr = (UMA_INFO *) OptParam;

  if (NBPtr->DctCount == MAX_D3_DCTS_PER_NODE_KV) {
    UmaInfoPtr->MemType = DDR3_TECHNOLOGY;
  } else {
    UmaInfoPtr->MemType = GDDR5_TECHNOLOGY;
  }

  for (Range = 0; Range < 4; Range++) {
    Limit = MemNGetBitFieldNb (NBPtr, BFDctLimitReg0 + Range);
    IntlvEn = (UINT8) (Limit & 0xF);
    Limit = ((Limit | 0x7FF) << (27 - 11));

    if (UmaInfoPtr->UmaBase < Limit) {
      if (IntlvEn != 0) {
        // Interleave is enabled
        if (NBPtr->DctCount == MAX_D3_DCTS_PER_NODE_KV) {
          UmaInfoPtr->UmaAttributes = UMA_ATTRIBUTE_INTERLEAVE | UMA_ATTRIBUTE_ON_DCT0 | UMA_ATTRIBUTE_ON_DCT1;
          IDS_HDT_CONSOLE (MEM_FLOW, "UMA is in interleaved region of DCT0 and DCT1\n");
        } else {
          UmaInfoPtr->UmaAttributes = UMA_ATTRIBUTE_INTERLEAVE | UMA_ATTRIBUTE_ON_DCT0 | UMA_ATTRIBUTE_ON_DCT1 | UMA_ATTRIBUTE_ON_DCT2 | UMA_ATTRIBUTE_ON_DCT3;
          IDS_HDT_CONSOLE (MEM_FLOW, "UMA is in interleaved region of DCT0, DCT1, DCT2 and DCT3\n");
        }
      } else {
        // No interleave, select DCT that UMA is on
        DctSel = (UINT8) (MemNGetBitFieldNb (NBPtr, BFDctBaseReg0 + Range) >> 4) & 7;
        UmaInfoPtr->UmaAttributes = (UINT32) 1 << (3 - DctSel + 27);
        IDS_HDT_CONSOLE (MEM_FLOW, "UMA is in DCT%d region\n", DctSel);
      }
      break;
    }
  }

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function sets final values for specific registers.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          TRUE -  No fatal error occurs.
 *     @return          FALSE - Fatal error occurs.
 */

BOOLEAN
MemNFinalizeMctKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT16 Speed;
  UINT32 Value32;
  UINT8 DcqBwThrotWm1;
  UINT8 DcqBwThrotWm2;

  Speed = NBPtr->DCTPtr->Timings.Speed;

  // F2x118
  MemNSetBitFieldNb (NBPtr, RegMctCfgLo, (MemNGetBitFieldNb (NBPtr, RegMctCfgLo) & 0xFFFFFF) | 0x1C000000);

  // F2x11C
  MemNSetBitFieldNb (NBPtr, RegMctCfgHi, 0x0CE00EA9);

  // F2x1B0
  MemNSetBitFieldNb (NBPtr, RegExtMctCfgLo, (MemNGetBitFieldNb (NBPtr, RegExtMctCfgLo) & 0x003FEFC0) | 0x0FC01001);

  // F2x1B4
  if (Speed == DDR667_FREQUENCY) {
    DcqBwThrotWm1 = 3;
    DcqBwThrotWm2 = 4;
  } else if (Speed == DDR800_FREQUENCY) {
    DcqBwThrotWm1 = 3;
    DcqBwThrotWm2 = 5;
  } else if (Speed == DDR1066_FREQUENCY) {
    DcqBwThrotWm1 = 4;
    DcqBwThrotWm2 = 6;
  } else if (Speed == DDR1333_FREQUENCY) {
    DcqBwThrotWm1 = 5;
    DcqBwThrotWm2 = 8;
  } else if  (Speed == DDR1600_FREQUENCY) {
    DcqBwThrotWm1 = 6;
    DcqBwThrotWm2 = 9;
  } else if  (Speed == DDR1866_FREQUENCY) {
    DcqBwThrotWm1 = 7;
    DcqBwThrotWm2 = 10;
  } else {
    DcqBwThrotWm1 = 8;
    DcqBwThrotWm2 = 12;
  }

  Value32 = MemNGetBitFieldNb (NBPtr, RegExtMctCfgHi);
  Value32 &= 0xFFFFFC00;
  Value32 |= (((UINT32) DcqBwThrotWm2 << 5) | (UINT32) DcqBwThrotWm1);
  MemNSetBitFieldNb (NBPtr, RegExtMctCfgHi, Value32);

  // Set LockDramCfg
  if (IsFeatureEnabled (C6Cstate, NBPtr->MemPtr->PlatFormConfig, &(NBPtr->MemPtr->StdHeader))) {
    IDS_SKIP_HOOK (IDS_LOCK_DRAM_CFG, NBPtr, &NBPtr->MemPtr->StdHeader) {
      MemNSetBitFieldNb (NBPtr, BFLockDramCfg, 1);
    }
  }

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function force the ECC symbol size
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   *Size    - ECC symbol size
 *                               FALSE = x4
 *                               TRUE  = x8
 *
 *     @return    TRUE
 */

BOOLEAN
MemNForceEccSymbolSizeKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *Size
  )
{
  *(BOOLEAN *)Size = FALSE;

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *   Set DRAM Controller select channel interleave address bit.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNInitChannelIntlvAddressBitKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  // Set DctSelIntLvAddr = D18F2x114[DctSelIntLvAddr[2]], D18F2x110[DctSelIntLvAddr[1:0]] = 100b
  // regardless channel interleave is enable or not.
  // NBPtr->DefDctSelIntLvAddr = 4;
  NBPtr->SetBitField (NBPtr, BFDctSelIntLvAddr, NBPtr->DefDctSelIntLvAddr);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function disable memory hole mappings
 *
 *     @param[in,out]   *NBPtr             - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   *OptParam          - Optional parameter
 *
 *     @return    TRUE
 */

BOOLEAN
MemNDisableMemHoleMappingKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  )
{
  UINT8 Range;
  CONST BIT_FIELD_NAME LgcyMmioHoleEnBF[] = {BFLgcyMmioHoleEn0, BFLgcyMmioHoleEn1, BFLgcyMmioHoleEn2, BFLgcyMmioHoleEn3};
  CONST BIT_FIELD_NAME DctIntLvEnBF[] = {BFDctIntLvEn0, BFDctIntLvEn1, BFDctIntLvEn2, BFDctIntLvEn3};

  for (Range = 0; Range < sizeof (LgcyMmioHoleEnBF) / sizeof (BIT_FIELD_NAME); Range++) {
    NBPtr->LgcyMmioHoleEnMap &= ~(1 << Range);
    NBPtr->LgcyMmioHoleEnMap |= (NBPtr->GetBitField (NBPtr, LgcyMmioHoleEnBF[Range]) << Range);
    NBPtr->SetBitField (NBPtr, LgcyMmioHoleEnBF[Range], 0);
  }

  // Work around for UBTS-418496
  for (Range = 0; Range < sizeof (DctIntLvEnBF) / sizeof (BIT_FIELD_NAME); Range++) {
    NBPtr->DctIntLvEnMap &= ~(0xF << (Range * 4));
    NBPtr->DctIntLvEnMap |= (NBPtr->GetBitField (NBPtr, DctIntLvEnBF[Range]) << (Range * 4));
    NBPtr->SetBitField (NBPtr, DctIntLvEnBF[Range], 0);
  }

  // Save and disable PrefIoDis[13] and PrefCpuDis[12]
  NBPtr->MctPrefecherDisMap &= ~(0x3 << 12);
  NBPtr->MctPrefecherDisMap |= (NBPtr->GetBitField (NBPtr, BFPrefCpuDis) << 12);
  NBPtr->SetBitField (NBPtr, BFPrefCpuDis, 1);
  NBPtr->MctPrefecherDisMap |= (NBPtr->GetBitField (NBPtr, BFPrefIoDis) << 13);
  NBPtr->SetBitField (NBPtr, BFPrefIoDis, 1);

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function restore memory hole mappings
 *
 *     @param[in,out]   *NBPtr             - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   *OptParam          - Optional parameter
 *
 *     @return    TRUE
 */

BOOLEAN
MemNRestoreMemHoleMappingKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *OptParam
  )
{
  UINT8 Range;
  CONST BIT_FIELD_NAME LgcyMmioHoleEnBF[] = {BFLgcyMmioHoleEn0, BFLgcyMmioHoleEn1, BFLgcyMmioHoleEn2, BFLgcyMmioHoleEn3};
  CONST BIT_FIELD_NAME DctIntLvEnBF[] = {BFDctIntLvEn0, BFDctIntLvEn1, BFDctIntLvEn2, BFDctIntLvEn3};

  for (Range = 0; Range < sizeof (LgcyMmioHoleEnBF) / sizeof (BIT_FIELD_NAME); Range++) {
    NBPtr->SetBitField (NBPtr, LgcyMmioHoleEnBF[Range], (NBPtr->LgcyMmioHoleEnMap >> Range) & 1);
  }

  for (Range = 0; Range < sizeof (DctIntLvEnBF) / sizeof (BIT_FIELD_NAME); Range++) {
    NBPtr->SetBitField (NBPtr, DctIntLvEnBF[Range], (NBPtr->DctIntLvEnMap >> (Range * 4)) & 0xF);
  }

  NBPtr->SetBitField (NBPtr, BFPrefCpuDis, (NBPtr->MctPrefecherDisMap >> 12) & 0x1);
  NBPtr->SetBitField (NBPtr, BFPrefIoDis, (NBPtr->MctPrefecherDisMap >> 13) & 0x1);

  return TRUE;
}

/*----------------------------------------------------------------------------
 *                              LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
