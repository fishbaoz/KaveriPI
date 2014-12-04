/* $NoKeywords:$ */
/**
 * @file
 *
 * mnidendimmtn.c
 *
 * TN northbridge constructor for dimm identification translator.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/NB/TN)
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
#include "mm.h"
#include "mn.h"
#include "OptionMemory.h"
#include "mntn.h"
#include "Ids.h"
#include "cpuRegisters.h"
#include "cpuFamRegisters.h"
#include "cpuFamilyTranslation.h"
#include "mfidendimm.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)


#define FILECODE PROC_MEM_NB_TN_MNIDENDIMMTN_FILECODE

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
MemNIdentifyDimmConstructorTN (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   MEM_DATA_STRUCT *MemPtr,
  IN       UINT8 NodeID
  )
{
  //
  // Determine if this is the expected NB Type
  //
  GetLogicalIdOfSocket (MemPtr->DiesPerSystem[NodeID].SocketId, &(MemPtr->DiesPerSystem[NodeID].LogicalCpuid), &(MemPtr->StdHeader));
  if (!MemNIsIdSupportedTN (&(MemPtr->DiesPerSystem[NodeID].LogicalCpuid))) {
    return FALSE;
  }

  NBPtr->NodeCount = 1;
  NBPtr->DctCount = MAX_DCTS_PER_NODE_TN;
  NBPtr->CsRegMsk = 0x7FF8FFE0;
  NBPtr->MemPtr = MemPtr;
  NBPtr->MCTPtr = &(MemPtr->DiesPerSystem[NodeID]);
  NBPtr->PciAddr.AddressValue = MemPtr->DiesPerSystem[NodeID].PciAddr.AddressValue;
  NBPtr->Node = ((UINT8) NBPtr->PciAddr.Address.Device) - 24;
  NBPtr->Ganged = FALSE;
  MemNInitNBRegTableTN (NBPtr, NBPtr->NBRegTable);
  NBPtr->MemNCmnGetSetFieldNb = MemNCmnGetSetFieldTN;
  NBPtr->GetBitField = MemNGetBitFieldNb;
  NBPtr->SetBitField = MemNSetBitFieldNb;
  NBPtr->GetSocketRelativeChannel = MemNGetSocketRelativeChannelNb;
  NBPtr->FamilySpecificHook[DCTSelectSwitch] = MemNDctCfgSelectUnb;
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
MemFTransSysAddrToCsTN (
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
  UINT64 CSMask;
  UINT64 InputAddr;
  UINT64 ChannelOffset;
  MEM_NB_BLOCK *NBPtr;
  MEM_DATA_STRUCT *MemPtr;
  UINT8 Die;

  UINT64 SysAddr;
  UINT8 *NodeID;
  UINT8 *ChannelSelect;
  UINT8 *ChipSelect;

  //
  // Determine if this is the expected NB Type
  //
  MemPtr = mmPtr->MemPtr;
  GetLogicalIdOfSocket (MemPtr->DiesPerSystem[BSP_DIE].SocketId, &(MemPtr->DiesPerSystem[BSP_DIE].LogicalCpuid), &(MemPtr->StdHeader));
  if (!MemNIsIdSupportedTN (&(MemPtr->DiesPerSystem[BSP_DIE].LogicalCpuid))) {
    return AGESA_ERROR;
  }

  SysAddr = AmdDimmIdentify->MemoryAddress;
  NodeID = &(AmdDimmIdentify->SocketId);
  ChannelSelect = &(AmdDimmIdentify->MemChannelId);
  ChipSelect = &(AmdDimmIdentify->ChipSelect);
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
        CSBase = ((UINT64) temp & NBPtr->CsRegMsk) << 8;

        // Obtain the CS Mask
        CSMask = ((UINT64) MemFGetPCI (NBPtr, *NodeID, DctNum, BFCSMask0Reg + (cs >> 1)) & NBPtr->CsRegMsk) << 8;

        // Adjust the Channel Addr for easy comparison
        InputAddr = ((ChannelAddr >> 8) & NBPtr->CsRegMsk) << 8;

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