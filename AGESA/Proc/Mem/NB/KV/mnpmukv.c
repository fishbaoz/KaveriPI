/* $NoKeywords:$ */
/**
 * @file
 *
 * mnpmukv.c
 *
 * Common Northbridge register related functions for KV
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
#include "Ids.h"
#include "mm.h"
#include "mn.h"
#include "mt.h"
#include "mtsdi3.h"
#include "mnreg.h"
#include "mnpmu.h"
#include "mnPmuFirmwareD3KV.h"
#include "Filecode.h"
#include "heapManager.h"
#include "mnPmuSramMsgBlockKV.h"
#include "OptionMemory.h"
#include "mnkv.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)


#define FILECODE PROC_MEM_NB_KV_MNPMUKV_FILECODE



/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */
#define PMU_SRAM_DMEM_SIZE_KV      0x1800  ///< PMU DMEM size is 6K Bytes
#define PMU_SRAM_IMEM_SIZE_KV      0x3800  ///< PMU IMEM size is 14K Bytes
#define PMU_TRAINS_ECC_LANES       0x80    ///< PMU trains ECC lanes
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

VOID
STATIC
MemNLoadPmuFirmwareImage0KV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
STATIC
MemNLoadPmuFirmwareImage1KV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );
/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function set the PMU sequence control for DRAM training
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNSetPmuSequenceControlKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  LOCATE_HEAP_PTR LocHeap;
  PMU_SRAM_MSG_BLOCK_KV *PmuSramMsgBlockPtr;

  LocHeap.BufferHandle = AMD_MEM_PMU_SRAM_MSG_BLOCK_HANDLE;

  if (HeapLocateBuffer (&LocHeap, &(NBPtr->MemPtr->StdHeader)) != AGESA_SUCCESS) {
    ASSERT(FALSE); // Could not locate heap for PMU SRAM Message Block buffer.
  }

  PmuSramMsgBlockPtr = (PMU_SRAM_MSG_BLOCK_KV *) LocHeap.BufferPtr;

  // Both images get program with same sequenceCtl, each image would ignore the ones that did not apply to it.
  if (NBPtr->RefPtr->PmuTrainMode == PMU_TRAIN_1D) {
    IDS_HDT_CONSOLE (MEM_FLOW, "\t\tPMU 1D Training\n");
    // [0] 1=DevInit [1] 1=WrLvl Training, [2] 1=RxEnDly Training, [3] 1=1D Rd-Dqs Training, [4] 1=1D Wr-Dq Training.
    PmuSramMsgBlockPtr->SequenceCtl = 0x1F;
  } else if (NBPtr->RefPtr->PmuTrainMode == PMU_TRAIN_1D_2D_READ) {
    IDS_HDT_CONSOLE (MEM_FLOW, "\t\tPMU 1D and 2D read Training\n");
    // [0] 1=DevInit [1] 1=WrLvl Training, [2] 1=RxEnDly Training, [3] 1=1D Rd-Dqs Training, [4] 1=1D Wr-Dq Training, [5] 1=2D Read Training.
    PmuSramMsgBlockPtr->SequenceCtl = 0x3F;
  } else if (NBPtr->RefPtr->PmuTrainMode == PMU_TRAIN_1D_2D) {
    IDS_HDT_CONSOLE (MEM_FLOW, "\t\tPMU 1D and 2D Training\n");
    // [0] 1=DevInit [1] 1=WrLvl Training, [2] 1=RxEnDly Training, [3] 1=1D Rd-Dqs Training, [4] 1=1D Wr-Dq Training, [5] 1=2D Read Training, [6] 1=2D Write Training.
    PmuSramMsgBlockPtr->SequenceCtl = 0x7F;
  } else {
    IDS_HDT_CONSOLE (MEM_FLOW, "\t\tPMU Train auto\n");
    if (PmuFirmwareD3Image1KV.SramSize != PMU_IMAGE_NOT_USEABLE) {
      // PMU 1D and 2D Training
      PmuSramMsgBlockPtr->SequenceCtl = 0x7F;
    } else {
      // PMU 1D Training
      PmuSramMsgBlockPtr->SequenceCtl = 0x1F;
    }
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function passes to PMU DRAM configuration over PMU SRAM for DDR3 DRAM init and training
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNPopulatePmuSramConfigD3KV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 *SpdBufferPtr;
  UINT8 AddrMirror;
  UINT8 i;
  UINT8 Dimm;
  UINT8 DramType;
  UINT8 ModuleType;
  UINT8  NbPs;
  LOCATE_HEAP_PTR LocHeap;
  PMU_SRAM_MSG_BLOCK_KV *PmuSramMsgBlockPtr;

  LocHeap.BufferHandle = AMD_MEM_PMU_SRAM_MSG_BLOCK_HANDLE;

  if (HeapLocateBuffer (&LocHeap, &(NBPtr->MemPtr->StdHeader)) != AGESA_SUCCESS) {
    ASSERT(FALSE); // Could not locate heap for PMU SRAM Message Block buffer.
  }

  PmuSramMsgBlockPtr = (PMU_SRAM_MSG_BLOCK_KV *) LocHeap.BufferPtr;

  // Starting from Revision 1, KV SMB supports MstateEn.
  PmuSramMsgBlockPtr->Revision = 1;
  PmuSramMsgBlockPtr->CpuId = MemNGetBitFieldNb (NBPtr, RegCpuid);
  PmuSramMsgBlockPtr->ChipSelect = (UINT8) NBPtr->DCTPtr->Timings.CsPresent;

  //F2xA8[PerRankTiming] = 0
  PmuSramMsgBlockPtr->PerRankTiming = 0;

  AddrMirror = 0;
  for (i = 0; i < 8; i++) {
    if (((i & 1) != 0) && ((NBPtr->DCTPtr->Timings.DimmMirrorPresent & (1 << (i >> 1))) != 0)) {
      AddrMirror |= 1 << i;
    }
  }

  PmuSramMsgBlockPtr->AddrMirror = AddrMirror;

  DramType = 0;
  ModuleType = 0;
  for (Dimm = 0; Dimm < 2; Dimm++) {
    if (NBPtr->TechPtr->GetDimmSpdBuffer (NBPtr->TechPtr, &SpdBufferPtr, Dimm)) {
      DramType = SpdBufferPtr[2];
      ModuleType = SpdBufferPtr[3];

      if (Dimm == 0) {
        PmuSramMsgBlockPtr->RawCard0 = SpdBufferPtr[62];
        PmuSramMsgBlockPtr->Dimm0Rows =  ((SpdBufferPtr[5] >> 3) & 0x7) + 12;
        PmuSramMsgBlockPtr->Dimm0Cols = (SpdBufferPtr[5] & 0x7) + 9;
        PmuSramMsgBlockPtr->Dimm0Banks = ((SpdBufferPtr[4] >> 4) & 0x7) + 3;
      } else {
        PmuSramMsgBlockPtr->RawCard1 = SpdBufferPtr[62];
        PmuSramMsgBlockPtr->Dimm1Rows =  ((SpdBufferPtr[5] >> 3) & 0x7) + 12;
        PmuSramMsgBlockPtr->Dimm1Cols = (SpdBufferPtr[5] & 0x7) + 9;
        PmuSramMsgBlockPtr->Dimm1Banks = ((SpdBufferPtr[4] >> 4) & 0x7) + 3;
      }
    }
  }

  PmuSramMsgBlockPtr->DramType = DramType;
  PmuSramMsgBlockPtr->ModuleType = ModuleType;

  // M1StateEn = 0000b when Memory P-states are disabled
  PmuSramMsgBlockPtr->M1StateEn = 0;

  // SMBx4B[M1stateEn[3:0]]
  // Bit 0 is for NbP0
  // Bit 1 is for NbP1
  // Bit 2 is for NbP2
  // Bit 3 is for NbP3
  // For each bit
  // 1=M1 is used for that NbPstate
  // 0=M0 is used for that NbPstate
  if (MemNGetBitFieldNb (NBPtr, BFMemPstateDis) == 0) {
    for (NbPs = 0; NbPs < 4; ++NbPs) {
      PmuSramMsgBlockPtr->M1StateEn |= (MemNGetBitFieldNb (NBPtr, BFMemPstate0 + NbPs) << NbPs);
    }
  }

  // Bit 7 of ModuleType of byte offset 9 in the SMB is the ECC train bit.
  if (NBPtr->RefPtr->EnableEccFeature && NBPtr->MCTPtr->Status[SbEccDimms]) {
    PmuSramMsgBlockPtr->ModuleType |= PMU_TRAINS_ECC_LANES;
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function passes to PMU DRAM timings over PMU SRAM for DDR3 DRAM init and training.
 *     Timings are MemPstate dependent.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNPopulatePmuSramTimingsD3KV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT16 Mr0;
  UINT16 Mr1;
  UINT16 Mr2;
  UINT8  *MiniMaxTrfc;
  UINT8  Trfc;
  UINT8  j;
  LOCATE_HEAP_PTR LocHeap;
  PMU_SRAM_MSG_BLOCK_KV *PmuSramMsgBlockPtr;
  DRAM_CAD_BUS_CONFIG_KV *DramCadBusConfigPtr;

  LocHeap.BufferHandle = AMD_MEM_PMU_SRAM_MSG_BLOCK_HANDLE;

  if (HeapLocateBuffer (&LocHeap, &(NBPtr->MemPtr->StdHeader)) != AGESA_SUCCESS) {
    ASSERT(FALSE); // Could not locate heap for PMU SRAM Message Block buffer.
  }

  PmuSramMsgBlockPtr = (PMU_SRAM_MSG_BLOCK_KV *) LocHeap.BufferPtr;

  LocHeap.BufferHandle = AMD_MEM_DRAM_CAD_BUS_CONFIG_HANDLE;

  if (HeapLocateBuffer (&LocHeap, &(NBPtr->MemPtr->StdHeader)) != AGESA_SUCCESS) {
    ASSERT(FALSE); // Could not locate heap for DRAM CAD Bus Configuration buffer.
  }

  DramCadBusConfigPtr = (DRAM_CAD_BUS_CONFIG_KV *) LocHeap.BufferPtr;

  if (NBPtr->MemPstate == 0) {
    PmuSramMsgBlockPtr->TRP_M0 = (UINT8) MemNGetBitFieldNb (NBPtr, BFTrp);
    PmuSramMsgBlockPtr->TMRD_M0 = (UINT8) MemNGetBitFieldNb (NBPtr, BFTmrd);
    PmuSramMsgBlockPtr->SlowAccessMode_M0 = (DramCadBusConfigPtr->SlowMode[NBPtr->MemPstate]) ? 1 : 0;
    PmuSramMsgBlockPtr->CkeSetup_M0 = (UINT8) (DramCadBusConfigPtr->DctAddrTmg[NBPtr->MemPstate] & 0xFF);
    PmuSramMsgBlockPtr->CsOdtSetup_M0 = (UINT8) ((DramCadBusConfigPtr->DctAddrTmg[NBPtr->MemPstate] >> 8) & 0xFF);
    PmuSramMsgBlockPtr->AddrCmdSetup_M0 = (UINT8) ((DramCadBusConfigPtr->DctAddrTmg[NBPtr->MemPstate] >> 16) & 0xFF);
  } else {
    PmuSramMsgBlockPtr->TRP_M1 = (UINT8) MemNGetBitFieldNb (NBPtr, BFTrp);
    PmuSramMsgBlockPtr->TMRD_M1 = (UINT8) MemNGetBitFieldNb (NBPtr, BFTmrd);
    PmuSramMsgBlockPtr->SlowAccessMode_M1 = (DramCadBusConfigPtr->SlowMode[NBPtr->MemPstate]) ? 1 : 0;
    PmuSramMsgBlockPtr->CkeSetup_M1 = (UINT8) (DramCadBusConfigPtr->DctAddrTmg[NBPtr->MemPstate] & 0xFF);
    PmuSramMsgBlockPtr->CsOdtSetup_M1 = (UINT8) ((DramCadBusConfigPtr->DctAddrTmg[NBPtr->MemPstate] >> 8) & 0xFF);
    PmuSramMsgBlockPtr->AddrCmdSetup_M1 = (UINT8) ((DramCadBusConfigPtr->DctAddrTmg[NBPtr->MemPstate] >> 16) & 0xFF);
  }

  MiniMaxTrfc = &NBPtr->DCTPtr->Timings.Trfc0;
  Trfc = 0;
  for (j = 0; j < 4; j++) {
    if (Trfc < MiniMaxTrfc[j]) {
      Trfc = MiniMaxTrfc[j];
    }
  }

  if (NBPtr->MemPstate == 0) {
    PmuSramMsgBlockPtr->TRFC_M0 = Trfc;
  } else {
    PmuSramMsgBlockPtr->TRFC_M1 = Trfc;
  }

  Mr0 = (UINT16) MemNGetBitFieldNb (NBPtr, BFMxMr0);
  Mr1 = (UINT16) MemNGetBitFieldNb (NBPtr, BFMxMr1);
  Mr2 = (UINT16) MemNGetBitFieldNb (NBPtr, BFMxMr2);

  // Program MrsAddress[8]=1 (DLL):DLL reset for PMU init.
  Mr0 |= 1 << 8;
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tMR0 %04x\n", Mr0);
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tMR1 %04x\n", Mr1);
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tMR2 %04x\n", Mr2);

  if (NBPtr->MemPstate == 0) {
    PmuSramMsgBlockPtr->MR0_M0 = Mr0;
    PmuSramMsgBlockPtr->MR1_M0 = Mr1;
    PmuSramMsgBlockPtr->MR2_M0 = Mr2;
  } else {
    PmuSramMsgBlockPtr->MR0_M1 = Mr0;
    PmuSramMsgBlockPtr->MR1_M1 = Mr1;
    PmuSramMsgBlockPtr->MR2_M1 = Mr2;
  }

}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function load appropriate PMU firmware image.
 *
 *
 *     @param[in,out]   *NBPtr     - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNLoadPmuFirmwareKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  if (NBPtr->PmuFirmwareImage == 0) {
    MemNLoadPmuFirmwareImage0KV (NBPtr);
  } else {
    MemNLoadPmuFirmwareImage1KV (NBPtr);
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *    Determine the number of PMU images will be use.
 *
 *
 *     @param[in,out]   *NBPtr     - Pointer to the MEM_NB_BLOCK
 *
 */

UINT8
MemNNumberOfPmuFirmwareImageKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 result;

  if ((PmuFirmwareD3Image1KV.SramSize == PMU_IMAGE_NOT_USEABLE) || (NBPtr->RefPtr->PmuTrainMode == PMU_TRAIN_1D)) {
    result = PMU_IMAGE_MIN_KV;
  } else {
    result = PMU_IMAGE_MAX_KV;
  }

  return result;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *      Disable PMU on all DCTs
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNDisablePmuKV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  // Program D18F2x9C_x0002_0099_dct[3:0][PmuReset,PmuStall] = 1,1.
  MemNSetBitFieldNb (NBPtr, BFPmuStall, 1);
  MemNSetBitFieldNb (NBPtr, BFPmuReset, 1);
  MemNSetBitFieldNb (NBPtr, BFPmuSramSd, 1);

  // Program D18F2x9C_x0002_0080[3:0][PmuClkDiv]]=7 (PMU CLK turned off).
  MemNSetBitFieldNb (NBPtr, PER_MEM_PS (0, BFPMUClkDiv), 7);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *    Load PMU image0 to PMU SRAM.
 *
 *
 *     @param[in,out]   *NBPtr     - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
STATIC
MemNLoadPmuFirmwareImage0KV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT16 i;
  UINT8 Dct;

  // Sanity check the size of the PMU SRAM
  ASSERT (PMU_SRAM_DMEM_SIZE_KV <= PMU_SRAM_DMEM_SIZE_MAX);
  ASSERT (PMU_SRAM_IMEM_SIZE_KV <= PMU_SRAM_IMEM_SIZE_MAX);

  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb  (NBPtr, Dct);
    MemNSetBitFieldNb (NBPtr, BFPmuReset, 0);
  }

  // Set DctCfgBcEn to broadcast to all enabled DCTs
  MemNSetBitFieldNb (NBPtr, BFDctCfgBcEn, 1);

  // Write the word to D18F2x9C_x0005_[BFF:0000]_dct[3:0] and using the autoincrement feature.
  IDS_HDT_CONSOLE (MEM_FLOW, "Loading PMU Data image0 to DMEM\n");
  MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, PMU_FIRMWARE_SRAM_START | DCT_ACCESS_WRITE | DCT_OFFSET_AUTO_INC_EN | PMU_SRAM_DMEM_OFFSET);

  for (i = 0; i < PMU_SRAM_DMEM_SIZE_KV / sizeof (PmuFirmwareD3Image0KV.SramDataImage[0]); i++) {
    MemNSetBitFieldNb (NBPtr, RegDctAddlData, PmuFirmwareD3Image0KV.SramDataImage[i]);
  }
  MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0);

  // Write the word to D18F2x9C_x0005_[5BFF:4000]_dct[3:0] and using the autoincrement feature.
  IDS_HDT_CONSOLE (MEM_FLOW, "Loading PMU Code image0 to IMEM\n");
  MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, PMU_FIRMWARE_SRAM_START | DCT_ACCESS_WRITE | DCT_OFFSET_AUTO_INC_EN | PMU_SRAM_IMEM_OFFSET);
  for (i = 0; i < PMU_SRAM_IMEM_SIZE_KV / sizeof (PmuFirmwareD3Image0KV.SramCodeImage[0]); i++) {
    MemNSetBitFieldNb (NBPtr, RegDctAddlData, PmuFirmwareD3Image0KV.SramCodeImage[i]);
  }

  MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0);

  MemNSetBitFieldNb (NBPtr, BFDctCfgBcEn, 0);
  IDS_HDT_CONSOLE (MEM_FLOW, "PMU firmware revision %x is loaded successfully!\n", MemNGetBitFieldNb (NBPtr, PmuFirmwareRev));
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *    Load PMU image1 to PMU SRAM.
 *
 *
 *     @param[in,out]   *NBPtr     - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
STATIC
MemNLoadPmuFirmwareImage1KV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT16 i;
  UINT8 Dct;

  // Sanity check the size of the PMU SRAM
  ASSERT (PMU_SRAM_DMEM_SIZE_KV <= PMU_SRAM_DMEM_SIZE_MAX);
  ASSERT (PMU_SRAM_IMEM_SIZE_KV <= PMU_SRAM_IMEM_SIZE_MAX);

  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb  (NBPtr, Dct);
    // Program D18F2x9C_x0002_0099_dct[3:0][PmuReset,PmuStall] = 1,1.
    // Program D18F2x9C_x0002_000E_dct[3:0][PhyDisable]=0. Tester_mode=0.
    MemNPmuResetNb (NBPtr);

    MemNSetBitFieldNb (NBPtr, BFPmuReset, 0);
  }

  // Set DctCfgBcEn to broadcast to all enabled DCTs
  MemNSetBitFieldNb (NBPtr, BFDctCfgBcEn, 1);

  // Write the word to D18F2x9C_x0005_[BFF:0000]_dct[3:0] and using the autoincrement feature.
  IDS_HDT_CONSOLE (MEM_FLOW, "Loading PMU Data image1 to DMEM\n");
  MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, PMU_FIRMWARE_SRAM_START | DCT_ACCESS_WRITE | DCT_OFFSET_AUTO_INC_EN | PMU_SRAM_DMEM_OFFSET);

  for (i = 0; i < PMU_SRAM_DMEM_SIZE_KV / sizeof (PmuFirmwareD3Image1KV.SramDataImage[0]); i++) {
    MemNSetBitFieldNb (NBPtr, RegDctAddlData, PmuFirmwareD3Image1KV.SramDataImage[i]);
  }
  MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0);

  // Write the word to D18F2x9C_x0005_[5BFF:4000]_dct[3:0] and using the autoincrement feature.
  IDS_HDT_CONSOLE (MEM_FLOW, "Loading PMU Code image1 to IMEM\n");
  MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, PMU_FIRMWARE_SRAM_START | DCT_ACCESS_WRITE | DCT_OFFSET_AUTO_INC_EN | PMU_SRAM_IMEM_OFFSET);
  for (i = 0; i < PMU_SRAM_IMEM_SIZE_KV / sizeof (PmuFirmwareD3Image1KV.SramCodeImage[0]); i++) {
    MemNSetBitFieldNb (NBPtr, RegDctAddlData, PmuFirmwareD3Image1KV.SramCodeImage[i]);
  }

  MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0);

  MemNSetBitFieldNb (NBPtr, BFDctCfgBcEn, 0);
  IDS_HDT_CONSOLE (MEM_FLOW, "PMU firmware revision %x is loaded successfully!\n", MemNGetBitFieldNb (NBPtr, PmuFirmwareRev));
}

