/* $NoKeywords:$ */
/**
 * @file
 *
 * mnidendimmkv.c
 *
 * KV northbridge constructor for dimm identification translator.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/NB/HY)
 * @e \$Revision: 284205 $ @e \$Date: 2014-02-05 20:05:28 -0600 (Wed, 05 Feb 2014) $
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
#include "OptionMemory.h"       // need def for MEM_FEAT_BLOCK_NB
#include "mnkv.h"
#include "Ids.h"
#include "cpuRegisters.h"
#include "cpuFamRegisters.h"
#include "cpuFamilyTranslation.h"
#include "mfidendimm.h"
#include "Filecode.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)

#define FILECODE PROC_MEM_NB_KV_MNIDENDIMMKV_FILECODE

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
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
VOID
STATIC
NormalizedToBankAddrMapKV (
  IN       UINT64  ChannelAddr,
  IN       UINT64  CSMask,
  IN       UINT8   DimmAddrMap,
  IN       UINT8   RankDef,
  IN       BOOLEAN BankSwap,
  IN       BOOLEAN DctSelBankSwap,
  IN       BOOLEAN BankSwizzleMode,
      OUT  UINT8   *Bank,
      OUT  UINT32  *Row,
      OUT  UINT16  *Col
  );

UINT64
STATIC
BankSwapAddrKV (
  IN       BOOLEAN BankSwap,
  IN       UINT64 addr,
  IN       BOOLEAN DctSelBankSwap
  );

UINT8
STATIC
AddrToBankSwizzleKV (
  IN       BOOLEAN swizzle,
  IN       UINT64 addr,
  IN       UINT8 bankbit,
  IN       UINT8 offset
  );

UINT64
STATIC
CSInterleavingBitSwapKV (
  IN       UINT64 addr,
  IN       UINT64 CSMask,
  IN       UINT8 hiBit,
  IN       UINT8 lowBit
  );

/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function initializes the northbridge block for dimm identification translator
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   *MemPtr  - Pointer to the MEM_DATA_STRUCT
 *     @param[in,out]   NodeID   - ID of current node to construct
 *     @return          TRUE     - This is the correct constructor for the targeted node.
 *     @return          FALSE    - This isn't the correct constructor for the targeted node.
 */

BOOLEAN
MemNIdentifyDimmConstructorKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   MEM_DATA_STRUCT *MemPtr,
  IN       UINT8 NodeID
  )
{
  //
  // Determine if this is the expected NB Type
  //
  GetLogicalIdOfSocket (MemPtr->DiesPerSystem[NodeID].SocketId, &(MemPtr->DiesPerSystem[NodeID].LogicalCpuid), &(MemPtr->StdHeader));
  if (!MemNIsIdSupportedKV (&(MemPtr->DiesPerSystem[NodeID].LogicalCpuid))) {
    return FALSE;
  }

  NBPtr->NodeCount = MAX_NODES_SUPPORTED_KV;
  NBPtr->DctCount = MAX_DCTS_PER_NODE_KV;
  NBPtr->CsRegMsk = 0x7FF8FFE0;
  NBPtr->MemPtr = MemPtr;
  NBPtr->MCTPtr = &(MemPtr->DiesPerSystem[NodeID]);
  NBPtr->PciAddr.AddressValue = MemPtr->DiesPerSystem[NodeID].PciAddr.AddressValue;
  NBPtr->Node = ((UINT8) NBPtr->PciAddr.Address.Device) - 24;
  NBPtr->Ganged = FALSE;
  MemNInitNBRegTableKV (NBPtr, NBPtr->NBRegTable);
  NBPtr->MemNCmnGetSetFieldNb = MemNCmnGetSetFieldKV;
  NBPtr->SetBitField = MemNSetBitFieldNb;
  NBPtr->GetBitField = MemNGetBitFieldNb;
  NBPtr->GetSocketRelativeChannel = MemNGetSocketRelativeChannelNb;
  NBPtr->FamilySpecificHook[DCTSelectSwitch] = MemNDctCfgSelectKV;
  NBPtr->FamilySpecificHook[FixupSysAddr] = MemNDefaultFamilyHookNb;

  return TRUE;
}

/*-----------------------------------------------------------------------------*/
/**
*
*   This function translates the given physical system address to
*   a node, channel select, chip select, bank, row, and column address.
*
*   @param[in, out]   *AmdDimmIdentify - Pointer to the parameter structure AMD_IDENTIFY_DIMM
*   @param[in, out]   *mmPtr - Pointer to the MEM_MAIN_DATA_BLOCK
*
*   @retval           AGESA_SUCCESS - The chip select address is found
*   @retval           AGESA_BOUNDS_CHK - Targeted address is out of bound.
*
*/
AGESA_STATUS
MemFTransSysAddrToCsKV (
  IN OUT   AMD_IDENTIFY_DIMM *AmdDimmIdentify,
  IN       MEM_MAIN_DATA_BLOCK *mmPtr
  )
{
  BOOLEAN CSFound;
  BOOLEAN DctSelHiRngEn;
  BOOLEAN DctSelIntLvEn;
  BOOLEAN DctGangEn;
  BOOLEAN HiRangeSelected;
  BOOLEAN DramHoleValid;
  BOOLEAN CSEn;
  BOOLEAN SwapDone;
  BOOLEAN IntLvRgnSwapEn;
  UINT8 DctSelHi;
  UINT8 DramEn;
  UINT8 range;
  UINT8 IntlvEn;
  UINT8 IntlvSel;
  UINT8 ILog;
  UINT8 DctSelIntLvAddr;
  UINT8 DctNum;
  UINT8 cs;
  UINT8 BadDramCs;
  UINT8 spare;
  UINT8 IntLvRgnBaseAddr;
  UINT8 IntLvRgnLmtAddr;
  UINT8 IntLvRgnSize;
  UINT32 temp;
  UINT32 DramHoleOffset;
  UINT32 DramHoleBase;
  UINT64 DramBase;
  UINT64 DramLimit;
  UINT64 DramLimitSysAddr;
  UINT64 DctSelBaseAddr;
  UINT64 DctSelBaseOffset;
  UINT64 ChannelAddr;
  UINT64 CSBase;
  UINT64 CsMaskReg;
  UINT64 CSMask;
  UINT64 InputAddr;
  UINT64 ChannelOffset;
  UINT8 DimmAddrMap;
  UINT8 RankDef;
  BOOLEAN BankSwizzleMode;
  BOOLEAN BankSwap;
  BOOLEAN DctSelBankSwap;
  MEM_NB_BLOCK *NBPtr;
  MEM_DATA_STRUCT *MemPtr;
  UINT8 Die;

  UINT64 SysAddr;
  UINT8 *NodeID;
  UINT8 *ChannelSelect;
  UINT8 *ChipSelect;
  UINT8 *Bank;
  UINT32 *Row;
  UINT16 *Col;

  //
  // Determine if this is the expected NB Type
  //
  MemPtr = mmPtr->MemPtr;
  GetLogicalIdOfSocket (MemPtr->DiesPerSystem[BSP_DIE].SocketId, &(MemPtr->DiesPerSystem[BSP_DIE].LogicalCpuid), &(MemPtr->StdHeader));
  if (!MemNIsIdSupportedKV (&(MemPtr->DiesPerSystem[BSP_DIE].LogicalCpuid))) {
    return AGESA_ERROR;
  }

  SysAddr = AmdDimmIdentify->MemoryAddress;
  NodeID = &(AmdDimmIdentify->SocketId);
  ChannelSelect = &(AmdDimmIdentify->MemChannelId);
  ChipSelect = &(AmdDimmIdentify->ChipSelect);
  Bank = &(AmdDimmIdentify->Bank);
  Row = &(AmdDimmIdentify->Row);
  Col = &(AmdDimmIdentify->Column);
  CSFound = FALSE;
  ILog = 0;
  NBPtr = mmPtr->NBPtr;

  NBPtr->FamilySpecificHook[FixupSysAddr] (NBPtr, &SysAddr);

  // Loop to determine the dram range
  for (Die = 0; Die < mmPtr->DieCount; Die ++) {
    range = NBPtr[Die].Node;

    // DRAM Base
    temp = MemFGetPCI (NBPtr, 0, 0, BFDramBaseReg0 + range);
    DramEn = (UINT8) (temp & 0x3);
    IntlvEn = (UINT8) ((temp >> 8) & 0x7);

    DramBase = ((UINT64) (MemFGetPCI (NBPtr, 0, 0, BFDramBaseHiReg0 + range) & 0xFF) << 40) |
                 (((UINT64) temp & 0xFFFF0000) << 8);

    // DRAM Limit
    temp = MemFGetPCI (NBPtr, 0, 0, BFDramLimitReg0 + range);
    *NodeID = (UINT8) (temp & 0x7);
    IntlvSel = (UINT8) ((temp >> 8) & 0x7);
    DramLimit = ((UINT64) (MemFGetPCI (NBPtr, 0, 0, BFDramLimitHiReg0 + range) & 0xFF) << 40) |
                  (((UINT64) temp << 8) | 0xFFFFFF);
    DramLimitSysAddr = (((UINT64) MemFGetPCI (NBPtr, *NodeID, 0, BFDramLimitAddr)) << 27) | 0x7FFFFFF;
    ASSERT (DramLimit <= DramLimitSysAddr);

    if ((DramEn != 0) && (DramBase <= SysAddr) && (SysAddr <= DramLimitSysAddr) &&
        ((IntlvEn == 0) || (IntlvSel == ((SysAddr >> 12) & IntlvEn)))) {
      // Determine the number of bit positions consumed by Node Interleaving
      switch (IntlvEn) {

      case 0x0:
        ILog = 0;
        break;

      case 0x1:
        ILog = 1;
        break;

      case 0x3:
        ILog = 2;
        break;

      case 0x7:
        ILog = 3;
        break;

      default:
        IDS_ERROR_TRAP;
      }

      DramHoleOffset = MemFGetPCI (NBPtr, *NodeID, 0, BFDramHoleOffset) << 23;
      DramHoleValid = (BOOLEAN) MemFGetPCI (NBPtr, *NodeID, 0, (mmPtr->DieCount == 1) ? BFDramMemHoistValid : BFDramHoleValid);
      DramHoleBase = MemFGetPCI (NBPtr, *NodeID, 0, BFDramHoleBase) << 24;
      // Address belongs to this node based on DramBase/Limit,
      // but is in the memory hole so it doesn't map to DRAM
      if (DramHoleValid && (DramHoleBase <= SysAddr) && (SysAddr < 0x100000000)) {
        return AGESA_BOUNDS_CHK;
      }

      // F2x10C Swapped Interleaved Region
      IntLvRgnSwapEn = (BOOLEAN) MemFGetPCI (NBPtr, *NodeID, 0, BFIntLvRgnSwapEn);
      if (IntLvRgnSwapEn) {
        IntLvRgnBaseAddr = (UINT8) MemFGetPCI (NBPtr, *NodeID, 0, BFIntLvRgnBaseAddr);
        IntLvRgnLmtAddr = (UINT8) MemFGetPCI (NBPtr, *NodeID, 0, BFIntLvRgnLmtAddr);
        IntLvRgnSize = (UINT8) MemFGetPCI (NBPtr, *NodeID, 0, BFIntLvRgnSize);
        ASSERT (IntLvRgnSize == (IntLvRgnLmtAddr - IntLvRgnBaseAddr + 1));
        if (((SysAddr >> 34) == 0) &&
          ((((SysAddr >> 27) >= IntLvRgnBaseAddr) && ((SysAddr >> 27) <= IntLvRgnLmtAddr))
           || ((SysAddr >> 27) < IntLvRgnSize))) {
          SysAddr ^= (UINT64) IntLvRgnBaseAddr << 27;
        }
      }

      // Extract variables from F2x110 DRAM Controller Select Low Register
      DctSelHiRngEn = (BOOLEAN) MemFGetPCI (NBPtr, *NodeID, 0, BFDctSelHiRngEn);
      DctSelHi = (UINT8) MemFGetPCI (NBPtr, *NodeID, 0, BFDctSelHi);
      DctSelIntLvEn = (BOOLEAN) MemFGetPCI (NBPtr, *NodeID, 0, BFDctSelIntLvEn);
      DctGangEn = (BOOLEAN) MemFGetPCI (NBPtr, *NodeID, 0, BFDctGangEn);
      DctSelIntLvAddr = (UINT8) MemFGetPCI (NBPtr, *NodeID, 0, BFDctSelIntLvAddr);
      DctSelBaseAddr = (UINT64) MemFGetPCI (NBPtr, *NodeID, 0, BFDctSelBaseAddr) << 27;
      DctSelBaseOffset = (UINT64) MemFGetPCI (NBPtr, *NodeID, 0, BFDctSelBaseOffset) << 26;
      DctSelBankSwap = (BOOLEAN) MemFGetPCI (NBPtr, *NodeID, 0, BFDctSelBankSwap);

      // Determine if high DCT address range is being selected
      if (DctSelHiRngEn && !DctGangEn && (SysAddr >= DctSelBaseAddr)) {
        HiRangeSelected = TRUE;
      } else {
        HiRangeSelected = FALSE;
      }

      // Determine Channel
      if (DctGangEn) {
        *ChannelSelect = (UINT8) ((SysAddr >> 3) & 0x1);
      } else if (HiRangeSelected) {
        *ChannelSelect = DctSelHi;
      } else if (DctSelIntLvEn && (DctSelIntLvAddr == 0)) {
        *ChannelSelect = (UINT8) ((SysAddr >> 6) & 0x1);
      } else if (DctSelIntLvEn && (((DctSelIntLvAddr >> 1) & 0x1) != 0)) {
        temp = MemFUnaryXOR ((UINT32) ((SysAddr >> 16) & 0x1F));
        if ((DctSelIntLvAddr & 0x1) != 0) {
          *ChannelSelect = (UINT8) (((SysAddr >> 9) & 0x1) ^ temp);
        } else {
          *ChannelSelect = (UINT8) (((SysAddr >> 6) & 0x1) ^ temp);
        }
      } else if (DctSelIntLvEn) {
        *ChannelSelect = (UINT8) ((SysAddr >> (12 + ILog)) & 0x1);
      } else if (DctSelHiRngEn) {
        *ChannelSelect = ~DctSelHi & 0x1;
      } else {
        *ChannelSelect = 0;
      }
      ASSERT (*ChannelSelect < NBPtr[*NodeID].DctCount);

      // Determine base address offset
      if (HiRangeSelected) {
        if ((DctSelBaseAddr < DramHoleBase) && DramHoleValid && (SysAddr >= (UINT64) 0x100000000)) {
          ChannelOffset = (UINT64) DramHoleOffset;
        } else {
          ChannelOffset = DctSelBaseOffset;
        }
      } else {
        if (DramHoleValid && (SysAddr >= (UINT64) 0x100000000)) {
          ChannelOffset = (UINT64) DramHoleOffset;
        } else {
          ChannelOffset = DramBase;
        }
      }

      // Remove hoisting offset and normalize to DRAM bus addresses
      ChannelAddr = SysAddr - ChannelOffset;

      // Remove node interleaving
      if (IntlvEn != 0) {
        ChannelAddr = ((ChannelAddr >> (12 + ILog)) << 12) | (ChannelAddr & 0xFFF);
      }

      // Remove channel interleave
      if (DctSelIntLvEn && !HiRangeSelected && !DctGangEn) {
        if ((DctSelIntLvAddr & 1) != 1) {
          // A[6] Select or Hash 6
          ChannelAddr = ((ChannelAddr >> 7) << 6) | (ChannelAddr & 0x3F);
        } else if (DctSelIntLvAddr == 1) {
          // A[12]
          ChannelAddr = ((ChannelAddr >> 13) << 12) | (ChannelAddr & 0xFFF);
        } else {
          // Hash 9
          ChannelAddr = ((ChannelAddr >> 10) << 9) | (ChannelAddr & 0x1FF);
        }
      }

      // Determine the Chip Select
      for (cs = 0; cs < MAX_CS_PER_CHANNEL; ++ cs) {
        DctNum = DctGangEn ? 0 : *ChannelSelect;

        // Obtain the CS Base
        temp = MemFGetPCI (NBPtr, *NodeID, DctNum, BFCSBaseAddr0Reg + cs);
        CSEn = (BOOLEAN) (temp & 0x1);
        CSBase = (((temp >> 19) & 0xFFF) << 27) | (((temp >> 5) & 0x7FF) << 11);

        // Obtain the CS Mask
        CsMaskReg = (UINT64)MemFGetPCI (NBPtr, *NodeID, DctNum, BFCSMask0Reg + (cs >> 1)) & NBPtr->CsRegMsk;
        CSMask = (((CsMaskReg >> 19) & 0xFFF) << 27) | (((CsMaskReg >> 5) & 0x7FF) << 11);

        // Adjust the Channel Addr for easy comparison
        InputAddr = (((ChannelAddr >> 27) & 0xFFF) << 27) | (((ChannelAddr >> 11) & 0x7FF) << 11);

        if (CSEn && ((InputAddr & ~CSMask) == (CSBase & ~CSMask))) {
          CSFound = TRUE;

          *ChipSelect = cs;

          temp = MemFGetPCI (NBPtr, *NodeID, 0, BFOnLineSpareControl);
          SwapDone = (BOOLEAN) ((temp >> (1 + 2 * (*ChannelSelect))) & 0x1);
          BadDramCs = (UINT8) ((temp >> (4 + 4 * (*ChannelSelect))) & 0x7);
          if (SwapDone && (cs == BadDramCs)) {
            // Find the spare rank for the channel
            for (spare = 0; spare < MAX_CS_PER_CHANNEL; ++spare) {
              if ((MemFGetPCI (NBPtr, *NodeID, DctNum, BFCSBaseAddr0Reg + spare) & 0x2) != 0) {
                *ChipSelect = spare;
                break;
              }
            }
          }

          // Extract variables from D18F2x80
          DimmAddrMap = (UINT8) (MemFGetPCI (NBPtr, *NodeID, DctNum, BFDramBankAddrReg) >> ((*ChipSelect >> 1) * 4)) & 0xF;
          // Extract variables from the CS Mask
          RankDef = (UINT8)MemFGetPCI (NBPtr, *NodeID, DctNum, BFRankDef0 + (*ChipSelect >> 1));
          // Extract variables from D18F2x94
          BankSwizzleMode = MemFGetPCI (NBPtr, *NodeID, DctNum, BFBankSwizzleMode) > 0 ? TRUE : FALSE;
          // Extract variables from D18F2xA8
          BankSwap = MemFGetPCI (NBPtr, *NodeID, DctNum, BFBankSwap) > 0 ? TRUE : FALSE;

          // Determine the Bank, Row, and Col
          NormalizedToBankAddrMapKV (ChannelAddr, CSMask, DimmAddrMap, RankDef, BankSwap, DctSelBankSwap, BankSwizzleMode, Bank, Row, Col);

          // Sanity check outputs
          ASSERT (*ChipSelect < MAX_CS_PER_CHANNEL);

          break;
        }
      }
    }
    if (CSFound) {
      break;
    }
  }

  // last ditch sanity check
  ASSERT (!CSFound || ((*NodeID < mmPtr->DieCount) && (*ChannelSelect < NBPtr[*NodeID].DctCount) && (*ChipSelect < MAX_CS_PER_CHANNEL)));
  if (CSFound) {
    return AGESA_SUCCESS;
  } else {
    return AGESA_BOUNDS_CHK;
  }

}

/*-----------------------------------------------------------------------------*/
/**
*
*   This function returns a Bank, Row, and Col as seen on the DRAM Command/Addr bus
*
*   @param[in]    ChannelAddr - Memory address within the channel
*   @param[in]    CSMask - Chip select mask
*   @param[in]    DimmAddrMap - DIMM configuration information of address mapping
*   @param[in]    RankDef - Rank definition
*   @param[in]    BankSwap - Bank swap mode
*   @param[in]    DctSelBankSwap - DCT Select Bank swap mode
*   @param[in]    BankSwizzleMode - Bank swizzle mode
*   @param[out]   Bank - Pointer to the bank number
*   @param[out]   Row - Pointer to the row number
*   @param[out]   Col - Pointer to the column number
*
*/
VOID
STATIC
NormalizedToBankAddrMapKV (
  IN       UINT64  ChannelAddr,
  IN       UINT64  CSMask,
  IN       UINT8   DimmAddrMap,
  IN       UINT8   RankDef,
  IN       BOOLEAN BankSwap,
  IN       BOOLEAN DctSelBankSwap,
  IN       BOOLEAN BankSwizzleMode,
     OUT   UINT8   *Bank,
     OUT   UINT32  *Row,
     OUT   UINT16  *Col
  )
{
  UINT64 Addr;
  ChannelAddr = BankSwapAddrKV (BankSwap, ChannelAddr, DctSelBankSwap);

  // BankSwizzleMode is calculated on the normalized addr
  // Select Row/Col based on the CS interleave adjustment tables
  switch (DimmAddrMap) {
  case 0x1:
    *Bank = (UINT8)AddrToBankSwizzleKV (BankSwizzleMode, ChannelAddr, 13, 0);
    Addr = CSInterleavingBitSwapKV (ChannelAddr, CSMask, 28, !BankSwap ? 16 : (DctSelBankSwap ? 11 : 12));
    *Row = (UINT32) (((Addr >> 18) & 0x3FF) | (((Addr >> 16) & 0x3) << 10));
    *Col = (UINT16) ((Addr >> 3) & 0x3FF);
    break;
  case 0x2:
    *Bank = (UINT8)AddrToBankSwizzleKV (BankSwizzleMode, ChannelAddr, 13, 0);
    Addr = CSInterleavingBitSwapKV (ChannelAddr, CSMask, 29, !BankSwap ? 16 : (DctSelBankSwap ? 11 : 12));
    *Row = (UINT32) (((Addr >> 18) & 0x7FF) | (((Addr >> 16) & 0x3) << 11));
    *Col = (UINT16) ((Addr >> 3) & 0x3FF);
    break;
  case 0x5:
    *Bank = (UINT8)AddrToBankSwizzleKV (BankSwizzleMode, ChannelAddr, 13, 0);
    Addr = CSInterleavingBitSwapKV (ChannelAddr, CSMask, 30, !BankSwap ? 16 : (DctSelBankSwap ? 11 : 12));
    *Row = (UINT32) (((Addr >> 18) & 0xFFF) | (((Addr >> 16) & 0x3) << 12));
    *Col = (UINT16) ((Addr >> 3) & 0x3FF);
    break;
  case 0x7:
    *Bank = (UINT8)AddrToBankSwizzleKV (BankSwizzleMode, ChannelAddr, 13, 0);
    Addr = CSInterleavingBitSwapKV (ChannelAddr, CSMask, 31, !BankSwap ? 16 : (DctSelBankSwap ? 11 : 12));
    *Row = (UINT32) (((Addr >> 18) & 0x1FFF) | (((Addr >> 16) & 0x3) << 13));
    *Col = (UINT16) ((Addr >> 3) & 0x3FF);
    break;
  case 0xA:
    *Bank = (UINT8)AddrToBankSwizzleKV (BankSwizzleMode, ChannelAddr, 13, 0);
    Addr = CSInterleavingBitSwapKV (ChannelAddr, CSMask, 32, !BankSwap ? 16 : (DctSelBankSwap ? 11 : 12));
    *Row = (UINT32) (((Addr >> 18) & 0x3FFF) | (((Addr >> 16) & 0x3) << 14));
    *Col = (UINT16) ((Addr >> 3) & 0x3FF);
    break;
  case 0xB:
    *Bank = (UINT8)AddrToBankSwizzleKV (BankSwizzleMode, ChannelAddr, 14, 0);
    Addr = CSInterleavingBitSwapKV (ChannelAddr, CSMask, 33, !BankSwap ? 17 : (DctSelBankSwap ? 11 : 12));
    *Row = (UINT32) (((Addr >> 18) & 0x7FFF) | (((Addr >> 17) & 0x1) << 15));
    *Col = (UINT16) (((Addr >> 3) & 0x3FF) | (((Addr >> 13) & 0x1) << 11));
    break;
  default:
    ASSERT (FALSE);
  }
}

/*-----------------------------------------------------------------------------*/
/**
*
*   Function returns a normalized address after remapping by bank swaping. See D18F2xA8[BankSwap].
*
*   @param[in]    BankSwap - Bank swap mode
*   @param[in]    addr - memory address within channel
*   @param[in]    DctSelBankSwap - DCT select bank swap mode
*
*   @retval       Normalized address after remapping by bank swaping.
*
*/
UINT64
STATIC
BankSwapAddrKV (
  IN       BOOLEAN BankSwap,
  IN       UINT64 addr,
  IN       BOOLEAN DctSelBankSwap
  )
{
  if (BankSwap) {
    if (DctSelBankSwap) {
      // [10:8] swapped with [15:13]
      addr = ((((addr >> 8) & 0x7) << 13) | (((addr >> 13) & 0x7) << 8) | (addr & ~(0xE700)));
    } else {
      // [11:9] swapped with [15:13]
      addr = ((((addr >> 9) & 0x7) << 13) | (((addr >> 13) & 0x7) << 9) | (addr & ~(0xEE00)));
    }
  }
    return addr;
}

/*-----------------------------------------------------------------------------*/
/**
*
*   Function returns a DRAM device bank address after adjusting for possible
*   remapping by bank swizzling. See D18F2x94[BankSwizzleMode].
*
*   @param[in]    swizzle - Bank swizzle mode
*   @param[in]    addr - memory address within channel
*   @param[in]    bankbit - Bank bit to be swapped
*   @param[in]    offset - Offset to the bank bit to be swapped
*
*   @retval       DRAM device bank address after adjusting for possible remapping by bank swizzling
*
*/
UINT8
STATIC
AddrToBankSwizzleKV (
  IN       BOOLEAN swizzle,
  IN       UINT64 addr,
  IN       UINT8 bankbit,
  IN       UINT8 offset
  )
{
  // bankbit indicates the LSB at the start of the range of bits that
  // are mapped to the DRAM device bank address and depends on the DimmAddrMap.
  if (!swizzle) {
    return (UINT8) ((addr >> bankbit) & 0x7);
  } else {
    return (UINT8) (
        (((addr >> bankbit) & 1) ^ ((addr >> (bankbit + 3 + offset)) & 1) ^
        ((addr >> (bankbit + 6 + offset)) & 1)) |
        ((((addr >> (bankbit + 1)) & 1) ^ ((addr >> (bankbit + 4 + offset)) & 1) ^
        ((addr >> (bankbit + 7 + offset)) & 1)) << 1) |
        ((((addr >> (bankbit + 2)) & 1) ^ ((addr >> (bankbit + 5 + offset)) & 1) ^
        ((addr >> (bankbit + 8 + offset)) & 1)) << 2)
      );
  }
}

/*-----------------------------------------------------------------------------*/
/**
*
*   Function returns a normalized address after adjusting for possible bit swapping
*   by CS Interleaving.
*
*   @param[in]    addr - memory address within channel
*   @param[in]    CSMask - Chip select mask
*   @param[in]    hiBit - LSB at the start of the range of bits that take part in the swap
*   @param[in]    lowBit - LSB at the start of the range of bits that take part in the swap
*
*   @retval      Normalized address after adjusting for possible bit swapping by CS Interleaving
*
*/
UINT64
STATIC
CSInterleavingBitSwapKV (
  IN       UINT64 addr,
  IN       UINT64 CSMask,
  IN       UINT8 hiBit,
  IN       UINT8 lowBit
  )
{
  UINT64 mask;

  // Bits are swapped if the lower CSMask bits are zeros.
  // hiBit, lowBit indicate the LSB at the start of the range of bits that
  // take part in the swap an depend on the DimmAddrMap.
  // Sanity check the CSMask
  ASSERT (0x7 == ((CSMask >> lowBit & 0x7) | (CSMask >> hiBit & 0x7)));
  mask = (CSMask >> lowBit & 0x7) ^ 0x7;

  return (mask ? (addr & ~(mask << hiBit) & ~(mask << lowBit)) |
      (((addr >> hiBit) & mask) << lowBit) |
      (((addr >> lowBit) & mask) << hiBit) : addr);
}