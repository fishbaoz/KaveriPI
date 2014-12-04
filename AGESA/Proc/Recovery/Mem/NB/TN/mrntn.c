/* $NoKeywords:$ */
/**
 * @file
 *
 * mrntn.c
 *
 * Common Northbridge  functions for TN Recovery
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Proc/Recovery/Mem/NB/TN)
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
#include "mrport.h"
#include "cpuFamRegisters.h"
#include "cpuRegisters.h"
#include "cpuFamilyTranslation.h"
#include "mm.h"
#include "mn.h"
#include "mrntn.h"
#include "heapManager.h"
#include "AdvancedApi.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)
#define FILECODE PROC_RECOVERY_MEM_NB_TN_MRNTN_FILECODE
/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */

#define MAX_DCTS_PER_DIE 2
#define SPLIT_CHANNEL   0x20000000ul
#define CHANNEL_SELECT  0x10000000ul
#define MAX_DELAYS    9   /* 8 data bytes + 1 ECC byte */
#define MAX_DIMMS     4   /* 4 DIMMs per channel */

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
MemRecNInitNBRegTableTN (
  IN OUT   TSEFO *NBRegTable
  );

UINT32
STATIC
MemRecNCmnGetSetFieldTN (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 IsSet,
  IN       BIT_FIELD_NAME FieldName,
  IN       UINT32 Field
  );

UINT32
STATIC
MemRecNcmnGetSetTrainDlyTN (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 IsSet,
  IN       TRN_DLY_TYPE TrnDly,
  IN       DRBN DrbnVar,
  IN       UINT16 Field
  );

BOOLEAN
STATIC
MemRecNIsIdSupportedTN (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       CPU_LOGICAL_ID *LogicalIdPtr
  );


/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
STATIC CONST UINT32 RecModeDefRegArrayTN[] = {
  BFDramBankAddrReg, 0x00001111,
  BFTref,            0x00000002,
  BFDramTimingLoReg, 0x00000000,
  BFDramTiming0,     0x0F060606,
  BFDramTiming1,     0x04100415,
  BFDramTiming2,     0x05050505,
  BFDramTiming3,     0x00000405,
  BFDramTiming4,     0x000B0B0B,
  BFDramTiming5,     0x0B0B0B0B,
  BFDramTiming6,     0x00171600,
  BFDramTiming10,    0x00000006,
  BFDramNBP0,        0x32000006,
  BFDramConfigHiReg, 0x00180800,
  BFDramODTCtlReg,   0x00006061,
  BFPhyFence,        0x000056B5,
  BFSwNbPstateLoDis, 0x00000001,
  NULL
};

/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function initializes the northbridge block
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   *MemPtr  - Pointer to the MEM_DATA_STRUCT
 *     @param[in]       NodeID   - Node ID for this NB block
 *
 *     @return          TRUE  - This node is a TN and this NB block has been initialized
 *     @return          FALSE - This node is not a TN
 */

BOOLEAN
MemRecConstructNBBlockTN (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   MEM_DATA_STRUCT *MemPtr,
  IN       UINT8 NodeID
  )
{
  UINT8 i;
  UINT8 Dct;
  UINT8 Channel;
  DIE_STRUCT *MCTPtr;
  ALLOCATE_HEAP_PARAMS AllocHeapParams;

  //
  // Determine if this is the expected NB Type
  //
  GetLogicalIdOfSocket (MemPtr->DiesPerSystem[NodeID].SocketId, &(MemPtr->DiesPerSystem[NodeID].LogicalCpuid), &(MemPtr->StdHeader));
  if (!MemRecNIsIdSupportedTN (NBPtr, &(MemPtr->DiesPerSystem[NodeID].LogicalCpuid))) {
    return FALSE;
  }

  //
  // Allocate buffer for DCT_STRUCTs and CH_DEF_STRUCTs
  //
  MCTPtr = &MemPtr->DiesPerSystem[NodeID];
  AllocHeapParams.RequestedBufferSize = sizeof (DCT_STRUCT) + (
                                            MAX_CHANNELS_PER_DCT_TN * (
                                              sizeof (CH_DEF_STRUCT) + (
                                                MAX_DIMMS * MAX_DELAYS * NUMBER_OF_DELAY_TABLES
                                              )
                                            )
                                          ) + sizeof (MEM_PS_BLOCK) + (sizeof (TSEFO) * BFEndOfList);
  AllocHeapParams.BufferHandle = GENERATE_MEM_HANDLE (ALLOC_DCT_STRUCT_HANDLE, NodeID, 0, 0);
  AllocHeapParams.Persist = HEAP_LOCAL_CACHE;
  if (HeapAllocateBuffer (&AllocHeapParams, &MemPtr->StdHeader) != AGESA_SUCCESS) {
    return FALSE;
  }

  MCTPtr->DctCount = MAX_DCTS_PER_NODE_TN;
  MCTPtr->DctData = (DCT_STRUCT *) AllocHeapParams.BufferPtr;
  MemPtr->DiesPerSystem[NodeID + 1].DctCount = MAX_DCTS_PER_NODE_TN;
  MemPtr->DiesPerSystem[NodeID + 1].DctData = (DCT_STRUCT *) AllocHeapParams.BufferPtr;
  AllocHeapParams.BufferPtr += MAX_DCTS_PER_NODE_TN * sizeof (DCT_STRUCT);
  for (Dct = 0; Dct < MAX_DCTS_PER_NODE_TN; Dct++) {
    MCTPtr->DctData[Dct].Dct = Dct;
    MCTPtr->DctData[Dct].ChannelCount = MAX_CHANNELS_PER_DCT_TN;
    MCTPtr->DctData[Dct].ChData = (CH_DEF_STRUCT *) AllocHeapParams.BufferPtr;
    MCTPtr->DctData[Dct].ChData[0].ChannelID = (MCTPtr->DieId * 2) + Dct;
    AllocHeapParams.BufferPtr += MAX_CHANNELS_PER_DCT_TN * sizeof (CH_DEF_STRUCT);
    for (Channel = 0; Channel < MAX_CHANNELS_PER_DCT_TN; Channel++) {
      MCTPtr->DctData[Dct].ChData[Channel].RcvEnDlys = (UINT16 *) AllocHeapParams.BufferPtr;
      AllocHeapParams.BufferPtr += (MAX_DIMMS * MAX_DELAYS) * 2;
      MCTPtr->DctData[Dct].ChData[Channel].WrDqsDlys = AllocHeapParams.BufferPtr;
      MCTPtr->DctData[Dct].ChData[Channel].Dct = Dct;
      AllocHeapParams.BufferPtr += (MAX_DIMMS * MAX_DELAYS);
    }
  }
  NBPtr->PsPtr = (MEM_PS_BLOCK *) AllocHeapParams.BufferPtr;
  AllocHeapParams.BufferPtr += sizeof (MEM_PS_BLOCK);

  //
  //  Initialize NB block's variables
  //
  NBPtr->NBRegTable = (TSEFO *) AllocHeapParams.BufferPtr;
  NBPtr->MemPtr = MemPtr;
  NBPtr->RefPtr = MemPtr->ParameterListPtr;
  NBPtr->MCTPtr = MCTPtr;
  NBPtr->AllNodeMCTPtr = &MemPtr->DiesPerSystem[NodeID];
  NBPtr->SPDPtr = &MemPtr->SpdDataStructure[MemPtr->DiesPerSystem[NodeID].SocketId * MAX_CHANNELS_PER_SOCKET * MAX_DIMMS_PER_CHANNEL];
  NBPtr->AllNodeSPDPtr = &MemPtr->SpdDataStructure[MemPtr->DiesPerSystem[NodeID].SocketId * MAX_CHANNELS_PER_SOCKET * MAX_DIMMS_PER_CHANNEL];
  NBPtr->PciAddr.AddressValue = MCTPtr->PciAddr.AddressValue;

  NBPtr->DctCachePtr = NBPtr->DctCache;

  MemRecNInitNBRegTableTN (NBPtr->NBRegTable);
  NBPtr->Dct = 0;
  NBPtr->Channel = 0;
  NBPtr->VarMtrrHiMsk = MemRecGetVarMtrrHiMsk (&(MemPtr->DiesPerSystem[NodeID].LogicalCpuid), &(MemPtr->StdHeader));

  LibAmdMemFill (NBPtr->DctCache, 0, sizeof (NBPtr->DctCache), &NBPtr->MemPtr->StdHeader);
  LibAmdMemFill (NBPtr->IsSupported, FALSE, sizeof (NBPtr->IsSupported), &NBPtr->MemPtr->StdHeader);
  for (i = 0; i < NumberOfHooks; i++) {
    NBPtr->FamilySpecificHook[i] = (BOOLEAN (*) (MEM_NB_BLOCK *, VOID *)) MemRecDefTrue;
  }

  NBPtr->InitRecovery = MemRecNMemInitTN;

  NBPtr->RecModeDefRegArray = RecModeDefRegArrayTN;

  NBPtr->SwitchNodeRec = (VOID (*) (MEM_NB_BLOCK *, UINT8)) MemRecDefRet;
  NBPtr->SwitchDCT = MemRecNSwitchDctTN;
  NBPtr->SwitchChannel = MemRecNSwitchChannelTN;
  NBPtr->SetMaxLatency = MemRecNSetMaxLatencyTN;
  NBPtr->GetSysAddrRec = MemRecNGetMCTSysAddrNb;
  NBPtr->SendMrsCmd = MemRecNSendMrsCmdNb;
  NBPtr->sendZQCmd = MemRecNSendZQCmdNb;
  NBPtr->SetDramOdtRec = MemRecNSetDramOdtTN;

  NBPtr->GetBitField = MemRecNGetBitFieldNb;
  NBPtr->SetBitField = MemRecNSetBitFieldNb;
  NBPtr->GetTrainDly = MemRecNGetTrainDlyNb;
  NBPtr->SetTrainDly = MemRecNSetTrainDlyNb;

  NBPtr->MemRecNCmnGetSetFieldNb = MemRecNCmnGetSetFieldTN;
  NBPtr->MemRecNcmnGetSetTrainDlyNb = MemRecNcmnGetSetTrainDlyTN;
  NBPtr->MemRecNSwitchDctNb = MemRecNSwitchDctTN;
  NBPtr->TrainingFlow = MemNRecTrainingFlowUnb;
  NBPtr->MemRecNInitializeMctNb = MemRecNInitializeMctTN;
  NBPtr->MemRecNFinalizeMctNb = MemRecNFinalizeMctTN;
  NBPtr->ReadPattern = MemRecNContReadPatternUnb;
  NBPtr->WritePattern = MemRecNContWritePatternUnb;
  NBPtr->CompareTestPattern = MemRecNCompareTestPatternUnb;
  NBPtr->IsSupported[CheckDramTerm] = TRUE;
  NBPtr->IsSupported[CheckDramTermDyn] = TRUE;
  MemRecNSwitchDctTN (NBPtr, 0);

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function sets the current DCT to work on.
 *   Should be called before accessing a certain DCT
 *   All data structures will be updated to point to the current DCT
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]   Dct - ID of the target DCT
 *
 */

VOID
MemRecNSwitchDctTN (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 Dct
  )
{

  MemRecNSetBitFieldNb (NBPtr, BFDctCfgSel, Dct);

  NBPtr->Dct = Dct & 1;
  NBPtr->SPDPtr = &(NBPtr->AllNodeSPDPtr[(NBPtr->MCTPtr->NodeId * MAX_DCTS_PER_DIE + Dct) * MAX_DIMMS_PER_CHANNEL]);
  NBPtr->DCTPtr = &(NBPtr->MCTPtr->DctData[NBPtr->Dct]);

  MemRecNSwitchChannelTN (NBPtr, 0);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function sets the current channel to work on.
 *   Should be called before accessing a certain channel
 *   All data structures will be updated to point to the current channel
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]   Channel - ID of the target channel
 *
 */

VOID
MemRecNSwitchChannelTN (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 Channel
  )
{
  NBPtr->Channel = 0;
  NBPtr->ChannelPtr = &(NBPtr->DCTPtr->ChData[0]);
}

/*----------------------------------------------------------------------------
 *                              LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function gets or set DQS timing during training.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]   TrnDly - type of delay to be set
 *     @param[in]   DrbnVar - encoding of Dimm-Rank-Byte-Nibble to be accessed
 *                  (use either DIMM_BYTE_ACCESS(dimm,byte) or CS_NBBL_ACCESS(cs,nibble) to use this encoding
 *     @param[in]   Field - Value to be programmed
 *     @param[in]   IsSet - Indicates if the function will set or get
 *
 *     @return      value read, if the function is used as a "get"
 */

UINT32
STATIC
MemRecNcmnGetSetTrainDlyTN (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 IsSet,
  IN       TRN_DLY_TYPE TrnDly,
  IN       DRBN DrbnVar,
  IN       UINT16 Field
  )
{
  UINT16 index;
  UINT16 offset;
  UINT32 value;
  UINT32 address;
  UINT8 DIMM;
  UINT8 Byte;

  DIMM = DRBN_DIMM (DrbnVar);
  Byte = DRBN_BYTE (DrbnVar);

  switch (TrnDly) {
  case AccessRcvEnDly:
    index = 0x10;
    break;
  case AccessWrDqsDly:
    index = 0x30;
    break;
  case AccessWrDatDly:
    index = 0x01;
    break;
  case AccessRdDqsDly:
    index = 0x05;
    break;
  case AccessPhRecDly:
    index = 0x50;
    break;
  default:
    index = 0;
    IDS_ERROR_TRAP;
  }

  switch (TrnDly) {
  case AccessRcvEnDly:
  case AccessWrDqsDly:
    index += (DIMM * 3);
    if ((Byte & 0x04) != 0) {
      // if byte 4,5,6,7
      index += 0x10;
    }
    if ((Byte & 0x02) != 0) {
      // if byte 2,3,6,7
      index++;
    }
    if (Byte > 7) {
      index += 2;
    }
    offset = 16 * (Byte % 2);
    break;

  case AccessRdDqsDly:
  case AccessWrDatDly:
    index += (DIMM * 0x100);
    // break is not being used here because AccessRdDqsDly and AccessWrDatDly also need
    // to run AccessPhRecDly sequence.
  case AccessPhRecDly:
    index += (Byte / 4);
    offset = 8 * (Byte % 4);
    break;
  default:
    offset = 0;
    IDS_ERROR_TRAP;
  }

  address = index;
  MemRecNSetBitFieldNb (NBPtr, BFDctAddlOffsetReg, address);
  while (MemRecNGetBitFieldNb (NBPtr, BFDctAccessDone) == 0) {}
  value = MemRecNGetBitFieldNb (NBPtr, BFDctAddlDataReg);

  if (IsSet != 0) {
    if (TrnDly == AccessPhRecDly) {
      value = NBPtr->DctCachePtr->PhRecReg[index & 0x03];
    }

    value = ((UINT32)Field << offset) | (value & (~((UINT32)0xFF << offset)));
    MemRecNSetBitFieldNb (NBPtr, BFDctAddlDataReg, value);
    address |= DCT_ACCESS_WRITE;
    MemRecNSetBitFieldNb (NBPtr, BFDctAddlOffsetReg, address);
    while (MemRecNGetBitFieldNb (NBPtr, BFDctAccessDone) == 0) {}

    if (TrnDly == AccessPhRecDly) {
      NBPtr->DctCachePtr->PhRecReg[index & 0x03] = value;
    }
  } else {
    value = (value >> offset) & 0xFF;
  }

  return value;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function gets or sets a value to a bit field in a PCI register.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]   FieldName - Name of Field to be set
 *     @param[in]   Field - Value to be programmed
 *     @param[in]   IsSet - Indicates if the function will set or get
 *
 *     @return      value read, if the function is used as a "get"
 */

UINT32
STATIC
MemRecNCmnGetSetFieldTN (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 IsSet,
  IN       BIT_FIELD_NAME FieldName,
  IN       UINT32 Field
  )
{
  SBDFO Address;
  PCI_ADDR PciAddr;
  UINT8 Type;
  UINT32 Value;
  UINT32 Highbit;
  UINT32 Lowbit;
  UINT32 Mask;

  Value = 0;
  if (FieldName < BFEndOfList) {
    Address = NBPtr->NBRegTable[FieldName];
    if (Address != 0) {
      Lowbit = TSEFO_END (Address);
      Highbit = TSEFO_START (Address);
      Type = TSEFO_TYPE (Address);

      if ((Address >> 29) == ((DCT_PHY_ACCESS << 1) | 1)) {
        // Special DCT Phy access
        Address &= 0x0FFFFFFF;
        Lowbit = 0;
        Highbit = 16;
      } else {
        // Normal DCT Phy access
        Address = TSEFO_OFFSET (Address);
      }


      if (Type == NB_ACCESS) {
        // Address |= (((UINT32) (24 + 0)) << 15);
        PciAddr.AddressValue = Address;
        // Fix for MCM
        PciAddr.Address.Device = NBPtr->MemPtr->DiesPerSystem[NBPtr->Node].PciAddr.Address.Device;
        PciAddr.Address.Bus = NBPtr->MemPtr->DiesPerSystem[NBPtr->Node].PciAddr.Address.Bus;
        PciAddr.Address.Segment = NBPtr->MemPtr->DiesPerSystem[NBPtr->Node].PciAddr.Address.Segment;
        Address = PciAddr.AddressValue;
        LibAmdPciRead (AccessWidth32, PciAddr, &Value, &NBPtr->MemPtr->StdHeader);
      } else if (Type == DCT_PHY_ACCESS) {
        MemRecNSetBitFieldNb (NBPtr, BFDctAddlOffsetReg, Address);
        while (MemRecNGetBitFieldNb (NBPtr, BFDctAccessDone) == 0) {}

        Value = MemRecNGetBitFieldNb (NBPtr, BFDctAddlDataReg);
      } else {
        IDS_ERROR_TRAP;
      }

      if (IsSet != 0) {
        // A 1<<32 == 1<<0 due to x86 SHL instruction, so skip if that is the case
        if ((Highbit - Lowbit) != 31) {
          Mask = (((UINT32)1 << (Highbit - Lowbit + 1)) - 1);
        } else {
          Mask = (UINT32)0xFFFFFFFF;
        }
        Value &= ~(Mask << Lowbit);
        Value |= (Field & Mask) << Lowbit;

        if (Type == NB_ACCESS) {
          PciAddr.AddressValue = Address;
          LibAmdPciWrite (AccessWidth32, PciAddr , &Value, &NBPtr->MemPtr->StdHeader);
        } else if (Type == DCT_PHY_ACCESS) {
          MemRecNSetBitFieldNb (NBPtr, BFDctAddlDataReg, Value);
          Address |= DCT_ACCESS_WRITE;

          MemRecNSetBitFieldNb (NBPtr, BFDctAddlOffsetReg, Address);
          while (MemRecNGetBitFieldNb (NBPtr, BFDctAccessDone) == 0) {}
        } else {
          IDS_ERROR_TRAP;
        }
      } else {
        Value = Value >> Lowbit;  // Shift
        // A 1<<32 == 1<<0 due to x86 SHL instruction, so skip if that is the case
        if ((Highbit - Lowbit) != 31) {
          Value &= (((UINT32)1 << (Highbit - Lowbit + 1)) - 1);
        }
      }
    }
  } else {
    IDS_ERROR_TRAP;   // Invalid bit field index
  }
  return Value;
}


/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function initializes bit field translation table
 *
*     @param[in,out]   *NBRegTable   - Pointer to the NB Table *
 */

VOID
STATIC
MemRecNInitNBRegTableTN (
  IN OUT   TSEFO *NBRegTable
  )
{
  UINT16 i;
  for (i = 0; i <= BFEndOfList; i++) {
    NBRegTable[i] = 0;
  }

  // ---------------------------------------------------------------------------
  //
  // FUNCTION 1
  //
  // ---------------------------------------------------------------------------
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (1, 0x40), 31, 0, BFDramBaseReg0);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (1, 0x44), 31, 0, BFDramLimitReg0);

  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (1, 0x10C), 0,   0, BFDctCfgSel);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (1, 0x10C), 5,   4, BFNbPsSel);
  // ---------------------------------------------------------------------------
  //
  // FUNCTION 2
  //
  // ---------------------------------------------------------------------------
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x40), 31,  0, BFCSBaseAddr0Reg);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x44), 31,  0, BFCSBaseAddr1Reg);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x48), 31,  0, BFCSBaseAddr2Reg);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x4C), 31,  0, BFCSBaseAddr3Reg);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x50), 31,  0, BFCSBaseAddr4Reg);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x54), 31,  0, BFCSBaseAddr5Reg);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x58), 31,  0, BFCSBaseAddr6Reg);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x5C), 31,  0, BFCSBaseAddr7Reg);

  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x60), 31,  0, BFCSMask0Reg);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x64), 31,  0, BFCSMask1Reg);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x68), 31,  0, BFCSMask2Reg);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x6C), 31,  0, BFCSMask3Reg);

  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x7C), 31,  0, BFDramInitRegReg);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x80), 31,  0, BFDramBankAddrReg);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x88), 31,  0, BFDramTimingLoReg);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x94), 31,  0, BFDramConfigHiReg);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x98), 31,  0, BFDctAddlOffsetReg);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x9C), 31,  0, BFDctAddlDataReg);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x98), 31, 31, BFDctAccessDone);

  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x84),  9,  7, BFDramTerm);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x84), 11, 10, BFDramTermDyn);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x8C), 17, 16, BFTref);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x94),  7,  7, BFMemClkFreqVal);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x94),  4,  0, BFMemClkFreq);

  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x78), 18, 18, BFDqsRcvEnTrain);

  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x7C), 15,  0, BFMrsAddress);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x7C), 18, 16, BFMrsBank);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x7C), 22, 20, BFMrsChipSel);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x7C), 26, 26, BFSendMrsCmd);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x7C), 27, 27, BFDeassertMemRstX);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x7C), 28, 28, BFAssertCke);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x7C), 29, 29, BFSendZQCmd);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x7C), 30, 30, BFSendCtrlWord);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x7C), 31, 31, BFEnDramInit);

  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x7C),  7,  7, BFLevel);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x7C), 12, 12, BFMrsQoff);

  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x8C), 18, 18, BFDisAutoRefresh);

  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x90),  0,  0, BFInitDram);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x90), 15, 12, BFX4Dimm);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x90), 16, 16, BFUnBuffDimm);

  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x94),  8,  8, BFDdr3Mode);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x94),  9,  9, BFLegacyBiosMode);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x94), 11, 10, BFZqcsInterval);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x94), 14, 14, BFDisDramInterface);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x94), 21, 21, BFFreqChgInProg);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x94), 27, 24, BFDcqBypassMax);

  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0xA8),  5,  5, BFSubMemclkRegDly);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0xA8),  6,  6, BFOdtSwizzle);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0xA8), 15, 8, BFCtrlWordCS);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0xA8), 17, 16, BFMemPhyPllPdMode);

  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x110),  8, 8, BFDramEnabled);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x110), 31, 0, BFDctSelBaseAddrReg);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x114), 31, 0, BFDctSelBaseOffsetReg);

  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x11C), 31,  0, BFMctCfgHiReg);

  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x200),  4,   0, BFTcl);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x200), 31,  0, BFDramTiming0);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x204), 31,  0, BFDramTiming1);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x208), 31,  0, BFDramTiming2);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x20C), 31,  0, BFDramTiming3);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x210), 31,  0, BFDramNBP0);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x210),  3,   0, BFRdPtrInit);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x210), 18, 16, BFDataTxFifoWrDly);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x210), 31, 22, BFMaxLatency);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x214), 31,  0, BFDramTiming4);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x218), 31,  0, BFDramTiming5);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x21C), 31,  0, BFDramTiming6);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x22C), 31,  0, BFDramTiming10);

  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x230), 31,  0, BFPhyRODTCSLow);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x234), 31,  0, BFPhyRODTCSHigh);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x238), 31,  0, BFPhyWODTCSLow);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x23C), 31,  0, BFPhyWODTCSHigh);

  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x250),  3,  2, BFCmdTestEnable);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x250),  7,  5, BFCmdType);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x250), 10, 10, BFTestStatus);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x250), 11, 11, BFSendCmd);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x250), 12, 12, BFCmdSendInProg);

  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x254), 26, 24, BFTgtChipSelectA );

  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x260), 20,  0, BFCmdCount);

  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x268), 17,  0, BFNibbleErrSts);

  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x270), 18,  0, BFDataPrbsSeed);

  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (2, 0x28C), 31,  0, BFDramCmd2Reg);

  // ---------------------------------------------------------------------------
  //
  // DCT PHY REGISTERS
  //
  // ---------------------------------------------------------------------------
  MAKE_TSEFO (NBRegTable, DCT_PHY_ACCESS, 0x00, 31,  0, BFODCControl);
  MAKE_TSEFO (NBRegTable, DCT_PHY_ACCESS, 0x00,  2,  0, BFCkeDrvStren);
  MAKE_TSEFO (NBRegTable, DCT_PHY_ACCESS, 0x00,  6,  4, BFCsOdtDrvStren);
  MAKE_TSEFO (NBRegTable, DCT_PHY_ACCESS, 0x00, 10,  8, BFAddrCmdDrvStren);
  MAKE_TSEFO (NBRegTable, DCT_PHY_ACCESS, 0x00, 14, 12, BFClkDrvStren);
  MAKE_TSEFO (NBRegTable, DCT_PHY_ACCESS, 0x00, 18, 16, BFDataDrvStren);
  MAKE_TSEFO (NBRegTable, DCT_PHY_ACCESS, 0x00, 22, 20, BFDqsDrvStren);

  MAKE_TSEFO (NBRegTable, DCT_PHY_ACCESS, 0x04, 31,  0, BFAddrTmgControl);

  MAKE_TSEFO (NBRegTable, DCT_PHY_ACCESS, 0x08,  0,  0, BFWrtLvTrEn);
  MAKE_TSEFO (NBRegTable, DCT_PHY_ACCESS, 0x08,  5,  4, BFTrDimmSel);
  MAKE_TSEFO (NBRegTable, DCT_PHY_ACCESS, 0x08, 11,  8, BFWrLvOdt);
  MAKE_TSEFO (NBRegTable, DCT_PHY_ACCESS, 0x08, 12, 12, BFWrLvOdtEn);
  MAKE_TSEFO (NBRegTable, DCT_PHY_ACCESS, 0x08, 30, 29, BFDisablePredriverCal);
  MAKE_TSEFO (NBRegTable, DCT_PHY_ACCESS, 0x0B, 31,  0, BFDramPhyStatusReg);

  MAKE_TSEFO (NBRegTable, DCT_PHY_ACCESS, 0x0C, 31, 16, BFPhyFence);

  MAKE_TSEFO (NBRegTable, DCT_PHY_ACCESS, 0x0D0F0F13, _NOT_USED_, _NOT_USED_, BFProcOdtAdv);
  MAKE_TSEFO (NBRegTable, DCT_PHY_ACCESS, 0x0D0FE013, _NOT_USED_, _NOT_USED_, BFPllRegWaitTime);
  MAKE_TSEFO (NBRegTable, DCT_PHY_ACCESS, 0x0D0FE006, _NOT_USED_, _NOT_USED_, BFPllLockTime);

  MAKE_TSEFO (NBRegTable, DCT_PHY_ACCESS, 0x0D0FE018,  _NOT_USED_,  _NOT_USED_, BFPhyPSMasterChannel);
  // ---------------------------------------------------------------------------
  //
  // FUNCTION 5
  //
  // ---------------------------------------------------------------------------
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (5, 0x160),  5,  1, BFNbFid);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (5, 0x160),  7,  7, BFNbDid);
  MAKE_TSEFO (NBRegTable, NB_ACCESS, _FN (5, 0x170), 14, 14, BFSwNbPstateLoDis);
}

/*-----------------------------------------------------------------------------*/
/**
 *     MemRecNIsIdSupportedTN
 *      This function matches the CPU_LOGICAL_ID with certain criteria to
 *      determine if it is supported by this NBBlock.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]       *LogicalIdPtr - Pointer to the CPU_LOGICAL_ID
 *
 *     @return          TRUE -  This node is a TN.
 *     @return          FALSE - This node is not a TN.
 *
 */
BOOLEAN
STATIC
MemRecNIsIdSupportedTN (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       CPU_LOGICAL_ID *LogicalIdPtr
  )
{
  if (((LogicalIdPtr->Family & AMD_FAMILY_15_TN) != 0)
      && ((LogicalIdPtr->Revision & AMD_F15_ALL) != 0)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

