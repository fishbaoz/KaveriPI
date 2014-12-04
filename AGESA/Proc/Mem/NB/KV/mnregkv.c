/* $NoKeywords:$ */
/**
 * @file
 *
 * mnregkv.c
 *
 * Common Northbridge register related functions for KV
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/NB/KV)
 * @e \$Revision: 282148 $ @e \$Date: 2014-01-07 10:57:44 -0600 (Tue, 07 Jan 2014) $
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
#include "AdvancedApi.h"
#include "amdlib.h"
#include "Ids.h"
#include "OptionMemory.h"
#include "mm.h"
#include "mn.h"
#include "mnkv.h"
#include "mnreg.h"
#include "mnpmu.h"
#include "mport.h"
#include "merrhdl.h"
#include "heapManager.h"
#include "cpuRegisters.h"
#include "cpuFamRegisters.h"
#include "cpuFamilyTranslation.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)


#define FILECODE PROC_MEM_NB_KV_MNREGKV_FILECODE

/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */
#define BC_FIELDS   4

#define PMU_SMB_MEM_PS0_START   0x23
#define PMU_SMB_MEM_PS1_START   0x37

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

// Pad name, Pad instance, Chiplet number, Group number, Pad number
STATIC CONST DDR_CHIP_TO_PAD_ENTRY ROMDATA ChipToPadTableKV[] = {
  {MEMCKE0, 2, 0, 0, 3},
  {MEMCKE0, 3, 0, 0, 2},
  {MEMCKE0, 0, 0, 0, 1},
  {MEMCKE0, 1, 0, 0, 0},
  {MEMADD0, 14, 0, 2, 11},
  {MEMADD0, 15, 0, 2, 10},
  {MEMADD0, 12, 0, 2, 9},
  {MEMBANK0, 2, 0, 2, 8},
  {MEMADD0, 8, 0, 2, 7},
  {MEMADD0, 7, 0, 2, 6},
  {MEMADD0, 11, 0, 2, 5},
  {MEMADD0, 9, 0, 2, 4},
  {MEMCLK0_L, 4, 1, 0, 11},
  {MEMCLK0_H, 4, 1, 0, 10},
  {MEMCLK0_L, 2, 1, 0, 3},
  {MEMCLK0_H, 2, 1, 0, 2},
  {MEMCLK0_L, 0, 1, 0, 1},
  {MEMCLK0_H, 0, 1, 0, 0},
  {MEMADD0, 5, 1, 2, 9},
  {MEMADD0, 6, 1, 2, 8},
  {MEMADD0, 2, 1, 2, 7},
  {MEMADD0, 1, 1, 2, 6},
  {MEMADD0, 3, 1, 2, 5},
  {MEMADD0, 4, 1, 2, 4},
  {MEMCLK0_L, 3, 2, 0, 3},
  {MEMCLK0_H, 3, 2, 0, 2},
  {MEMCLK0_L, 1, 2, 0, 1},
  {MEMCLK0_H, 1, 2, 0, 0},
  {MEMADD0, 10, 2, 1, 7},
  {MEMBANK0, 0, 2, 1, 6},
  {MEMADD0, 0, 2, 1, 5},
  {MEMBANK0, 1, 2, 1, 4},
  {MEMCS0_L, 6, 2, 2, 11},
  {MEMCS0_L, 4, 2, 2, 10},
  {MEMCS0_L, 2, 2, 2, 9},
  {MEMCS0_L, 0, 2, 2, 8},
  {MEMADD0, 13, 3, 0, 3},
  {MEMCAS0_L, 0, 3, 0, 2},
  {MEMWE0_L, 0, 3, 0, 1},
  {MEMRAS0_L, 0, 3, 0, 0},
  {MEMODT0, 3, 3, 1, 7},
  {MEMODT0, 1, 3, 1, 6},
  {MEMODT0, 0, 3, 1, 5},
  {MEMODT0, 2, 3, 1, 4},
  {MEMCS0_L, 7, 3, 2, 11},
  {MEMCS0_L, 5, 3, 2, 10},
  {MEMCS0_L, 3, 3, 2, 9},
  {MEMCS0_L, 1, 3, 2, 8},
  {MEMDQ, 7, 0, 2, 7},
  {MEMDQ, 6, 0, 2, 6},
  {MEMDQ, 5, 0, 2, 5},
  {MEMDQ, 4, 0, 2, 4},
  {MEMDQ, 3, 0, 0, 3},
  {MEMDQ, 2, 0, 0, 2},
  {MEMDQ, 1, 0, 0, 1},
  {MEMDQ, 0, 0, 0, 0},
  {MEMDQS, 0, 0, 1, 10},
  {MEMDQS, 0, 0, 1, 9},
  {MEMDQS, 0, 0, 1, 8},
  {MEMDQ_L, 0, 0, 0, 0},
  {MEMDQ_H, 0, 0, 0, 1},
  {MEMDQS_H, 0, 0, 0, 2}
};

/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
/*-----------------------------------------------------------------------------*/
/**
 *      This function matches the CPU_LOGICAL_ID with certain criteria to
 *      determine if it is supported by this NBBlock.
 *
 *     @param[in]       *LogicalIdPtr - Pointer to the CPU_LOGICAL_ID
 *
 *     @return          TRUE -  This node is a KV.
 *     @return          FALSE - This node is not a KV.
 *
 */
BOOLEAN
MemNIsIdSupportedKV (
  IN       CPU_LOGICAL_ID *LogicalIdPtr
  )
{
  if (((LogicalIdPtr->Family & AMD_FAMILY_15_KV) != 0)
      && ((LogicalIdPtr->Revision & AMD_F15_ALL) != 0)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/*----------------------------------------------------------------------------
 *                              LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------*/

/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function gets or sets a value to a bit field in a PCI register.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]   FieldName - Bit Field to be programmed
 *     @param[in]   InputValue - Value to be programmed
 *     @param[in]   IsSet - Indicates if the function will set or get
 *
 *     @return      value read, if the function is used as a "get"
 */

UINT32
MemNCmnGetSetFieldKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 IsSet,
  IN       BIT_FIELD_NAME FieldName,
  IN       UINT32 InputValue
  )
{
  BF_LOCATION *BfLoc;
  PMU_SRAM_ADDR *PmuSramAddr;
  PCI_ADDR PciAddr;
  UINT32 Address;
  UINT32 SaveAddress;
  UINT32 HiBit;
  UINT32 LoBit;
  UINT32 Mask;
  BOOLEAN IsLinked;
  BOOLEAN WholeRegAccess;
  UINT32 RegIndex;
  UINT32 Value;
  UINT32 RegOffset;
  UINT32 Field;
  BF_NAME_ENC BfNameEnc;

  BOOLEAN HasMoreInstances;
  UINT8   i;
  UINT8   Limit;
  CONST UINT8   BroadcastBit[BC_FIELDS] =  {8, 12, 20, 24};
  CONST UINT32  BroadcastMask[BC_FIELDS] = {0x00000F00, 0x0000F000, 0x00F00000, 0x03000000};
  UINT8   BroadcastIndex[BC_FIELDS];
  UINT8   BroadcastBase[BC_FIELDS];
  UINT8   BroadcastLimit[BC_FIELDS];
  UINT8   PadTabIdx;
  BOOLEAN UseGroup;
  BOOLEAN UsePadTab;
  BOOLEAN UnrollNextField;

  Field = InputValue;
  Value = 0;
  BfNameEnc.FieldName = FieldName;
  FieldName = BfNameEnc.Parse.Index;
  PadTabIdx = GET_SIZE_OF (ChipToPadTableKV);
  UseGroup = FALSE;
  UsePadTab = FALSE;

  ASSERT ((FieldName < PmuSRAMRangeEnd) && (FieldName >= 0));

  if (NBPtr->NBRegTable[FieldName] != 0) {
    //======================================================================
    // Obtain register address and bit field location
    //======================================================================
    //
    if (FieldName < BFEndOfList) {
      BfLoc = (BF_LOCATION *) &(NBPtr->NBRegTable[FieldName]);
      RegIndex = BfLoc->RegIndex;
      HiBit = BfLoc->HiBit;
      LoBit = BfLoc->LoBit;
      IsLinked = (BOOLEAN) BfLoc->Linked;
      if ((HiBit - LoBit) < 31) {
        WholeRegAccess = FALSE;
      } else {
        WholeRegAccess = TRUE;
      }
      SaveAddress = Address = NBPtr->NBRegTable[RegIndex];
    } else if (FieldName < RegIdLimit) {
      RegIndex = FieldName;
      WholeRegAccess = TRUE;
      HiBit = 31;
      LoBit = 0;
      IsLinked = FALSE;
      SaveAddress = Address = NBPtr->NBRegTable[RegIndex];
    } else {
      // PMU SRAM fields
      PmuSramAddr = (PMU_SRAM_ADDR *) &(NBPtr->NBRegTable[FieldName]);
      RegIndex = RegSramMsgBlk;

      RegOffset = PmuSramAddr->RegOffset;
      if ((RegOffset >= PMU_SMB_MEM_PS0_START) && (RegOffset < PMU_SMB_MEM_PS1_START)) {
        RegOffset += NBPtr->MemPstate * (PMU_SMB_MEM_PS1_START - PMU_SMB_MEM_PS0_START);
      }
      SaveAddress = Address = NBPtr->NBRegTable[RegIndex] + ((RegOffset & 0xFFE) / 2);

      HiBit = PmuSramAddr->HiBit;
      LoBit = PmuSramAddr->LoBit;
      if ((RegOffset & 1) != 0) {
        HiBit += 8;
        LoBit += 8;
      }

      IsLinked = (BOOLEAN) PmuSramAddr->Linked;
      BfNameEnc.FieldName = FieldName | 0xFFFF0000;   // Disable SW broadcast
      if ((HiBit - LoBit) < 15) {
        WholeRegAccess = FALSE;
      } else {
        WholeRegAccess = TRUE;
        Field &= 0xFFFF;
      }
    }

    ASSERT ((HiBit < 32) && (LoBit <= HiBit));    // Only support 32 bit registers

    //======================================================================
    // Prepare broadcast matrix for Phy registers
    //======================================================================
    //
    if (RegIndex > DctPhyRegRangeStart) {
      IDS_HDT_CONSOLE (MEM_SETREG, "~                   [+] BF enum: %x\n", FieldName);
      for (i = 0; i < BC_FIELDS; i++) {
        Limit = (UINT8) ((Address & BroadcastMask[i]) >> BroadcastBit[i]);
        if (Limit == 0) {
          // This field has single instance, skip unrolling
          BroadcastBase[i] = BroadcastLimit[i] = 0;
        } else if (((BfNameEnc.FieldName & 0xFFFF0000) == 0xFFFF0000) || ((Address & BroadcastMask[i]) == BroadcastMask[i])) {
          // When SW broadcast is disabled or HW broadcast is specified, set base = limit
          BroadcastBase[i] = BroadcastLimit[i] = Limit;
        } else {
          // SW broadcast by default
          BroadcastBase[i] = 0;
          BroadcastLimit[i] = Limit;

          // Check override for each field separately
          switch (i) {
          case 0:
            // For Dimm, DLL, NBPs dependent registers:
            // When PER_DIMM or PER_NB_PS macro is used, select instance specified by macro
            if (BfNameEnc.Parse.DLNPlus1 != 0) {
              BroadcastBase[i] = BroadcastLimit[i] = (UINT8) BfNameEnc.Parse.DLNPlus1 - 1;
              ASSERT ((BfNameEnc.Parse.DLNPlus1 < (4 + 1)) || (BfNameEnc.Parse.DLNPlus1 == (8 + 1)));  // Only support up to 4 instances
            }
            break;
          case 1:
            // For ABYTE and DBYTE type, initialize ChipToPadTable index for translation
            if ((Address & 0x000E0000) == 0x00000000) {
              PadTabIdx = 0;
              UseGroup = (Limit <= 4);
              UsePadTab = TRUE;
            }
            break;
          case 2:
            if ((Address & 0x000F0000) == 0x00010000) {
              // For DBYTE type, chiplet controls byte lane:
              // When PER_DATA_BYTE macro is used, select instance specified by macro
              if (BfNameEnc.Parse.PadIdxPlus1 != 0) {
                BroadcastBase[i] = BroadcastLimit[i] = (UINT8) BfNameEnc.Parse.PadIdxPlus1 - 1;
                ASSERT ((BfNameEnc.Parse.PadIdxPlus1 < (10 + 1)));  // Only support up to 10 instances
              }
            }
            break;
          case 3:
            // For MemPstate dependent registers:
            if (BfNameEnc.Parse.MemPsPlus1 == 0) {
              // Access current MemPstate by default
              BroadcastBase[i] = BroadcastLimit[i] = (UINT8) NBPtr->MemPstate;
            } else if (BfNameEnc.Parse.MemPsPlus1 < 3) {
              // PER_MEM_PS macro is used, select MemPs specified by macro
              BroadcastBase[i] = BroadcastLimit[i] = (UINT8) BfNameEnc.Parse.MemPsPlus1 - 1;
            }
            break;
          }

          ASSERT (BroadcastBase[i] <= BroadcastLimit[i]);
          ASSERT (BroadcastLimit[i] <= Limit);

          // Use HW broadcast instead of SW broadcast when writing whole register
          if (WholeRegAccess && (BroadcastLimit[i] == Limit) && (BroadcastBase[i] == 0)) {
            BroadcastBase[i] = BroadcastLimit[i] = (UINT8) (BroadcastMask[i] >> BroadcastBit[i]);
          }
        }
        // Initialize Index at Base for below unrolling loop
        BroadcastIndex[i] = BroadcastBase[i];
      }
    }

    //======================================================================
    // Get/Set register/field
    //======================================================================
    //
    HasMoreInstances = FALSE;     // One instance by default
    do {
      if (RegIndex > DctPhyRegRangeStart) {
        // Unroll broadcast fields for partial write only
        UnrollNextField = TRUE;

        // Search ChipToPadTable
        while (PadTabIdx < GET_SIZE_OF (ChipToPadTableKV)) {
          if ((BfNameEnc.Parse.PadName == 0) || (BfNameEnc.Parse.PadName == ChipToPadTableKV[PadTabIdx].PadName)) {
            if (((Address & 0x000F0000) == 0x00010000) || (BfNameEnc.Parse.PadIdxPlus1 == 0) || (BfNameEnc.Parse.PadIdxPlus1 == (UINT8) (ChipToPadTableKV[PadTabIdx].Instance + 1))) {
              BroadcastIndex[1] = BroadcastLimit[1] = UseGroup ? ChipToPadTableKV[PadTabIdx].Group : ChipToPadTableKV[PadTabIdx].PadNum;
              if ((Address & 0x000F0000) == 0x00000000) {
                // Only use table chiplet for ABYTE. DBYTE chiplets are byte lanes.
                BroadcastIndex[2] = BroadcastLimit[2] = (UINT8) ChipToPadTableKV[PadTabIdx].Chiplet;
              }
              UnrollNextField = !UnrollNextField;
              PadTabIdx++;
              break;
            }
          }
          PadTabIdx++;
          if (PadTabIdx == GET_SIZE_OF (ChipToPadTableKV)) {
            // table index wraps around
            PadTabIdx = 0;
            ASSERT (UnrollNextField);   // No match in ChipToPadTable
            UnrollNextField = !UnrollNextField;
          }
        }

        // Unroll each broadcast field
        for (i = 0; i < BC_FIELDS; i++) {
          Address = (Address & (~BroadcastMask[i])) | ((UINT32) BroadcastIndex[i] << BroadcastBit[i]);
          if (UnrollNextField) {
            if (BroadcastIndex[i] < BroadcastLimit[i]) {
              BroadcastIndex[i]++;
              UnrollNextField = FALSE;
            } else {
              BroadcastIndex[i] = BroadcastBase[i];
            }
          }
        }

        HasMoreInstances = !UnrollNextField;
        ASSERT (PadTabIdx > 0);   // Unexpect PadTabIdx == 0
        if (!HasMoreInstances && UsePadTab) {
          break;    // Skip last iteration when ChipToPadTable is used
        }

        if (!IsSet) {
          // Use instance 0 when reading hardware broadcast registers.
          for (i = 0; i < BC_FIELDS; i++) {
            if ((Address & BroadcastMask[i]) == BroadcastMask[i]) {
              Address &= ~BroadcastMask[i];
            }
          }

          if (BfNameEnc.Parse.Direct == 1) {
            Address = MEM_PS_PER_INSTANCE (NBPtr->MemPstate, SaveAddress);
            HasMoreInstances = FALSE;     // One instance by default
          }

          MemNCmnGetSetFieldKV (NBPtr, 1, RegDctAddlOffset, Address);
          Value = MemNCmnGetSetFieldKV (NBPtr, 0, RegDctAddlData, 0);
          IDS_HDT_CONSOLE (MEM_GETREG, "~F2_9C_%x = %x\n", Address, Value);
          HasMoreInstances = FALSE;
        } else if (!WholeRegAccess) {
          MemNCmnGetSetFieldKV (NBPtr, 1, RegDctAddlOffset, Address);
          Value = MemNCmnGetSetFieldKV (NBPtr, 0, RegDctAddlData, 0);
          IDS_HDT_CONSOLE (MEM_GETREG, "~F2_9C_%x = %x\n", Address, Value);
        } else {
          Value = Field;
        }
      } else if (RegIndex > NbRegRangeStart) {
        ASSERT ((Address & 0xFFFF0000) == 0);       // NB_REG address only has 16 bits
        PciAddr.AddressValue = Address;
        PciAddr.Address.Device = NBPtr->PciAddr.Address.Device;
        PciAddr.Address.Bus = NBPtr->PciAddr.Address.Bus;
        PciAddr.Address.Segment = NBPtr->PciAddr.Address.Segment;
        Address = PciAddr.AddressValue;
        if (!(WholeRegAccess && IsSet)) {
          LibAmdPciRead (AccessWidth32, PciAddr, &Value, &NBPtr->MemPtr->StdHeader);
          if ((FieldName != RegDctAddlData) && (FieldName != RegDctAddlOffset)) {
            IDS_HDT_CONSOLE (MEM_GETREG, "~F%d_%03x = %x\n", (Address >> 12) & 0xF, Address & 0xFFF, Value);
          }
        } else {
          Value = Field;
        }
      } else {
        ASSERT (FALSE);
      }

      if (IsSet) {
        if (!WholeRegAccess) {
          Mask = (((UINT32)1 << (HiBit - LoBit + 1)) - 1);
          Value &= ~(Mask << LoBit);
          Value |= (Field & Mask) << LoBit;
        }

        if (RegIndex > DctPhyRegRangeStart) {
          if (BfNameEnc.Parse.Direct == 1) {
            Address = MEM_PS_PER_INSTANCE (NBPtr->MemPstate, SaveAddress);
            HasMoreInstances = FALSE;     // One instance by default
          }
          ASSERT (!NBPtr->IsSupported[ScrubberEn]);   // Phy CSR write is not allowed after scrubber is enabled
          MemNCmnGetSetFieldKV (NBPtr, 1, RegDctAddlData, Value);
          MemNCmnGetSetFieldKV (NBPtr, 1, RegDctAddlOffset, Address | DCT_ACCESS_WRITE);
          IDS_HDT_CONSOLE (MEM_SETREG, "~F2_9C_%x [%d:%d] = %x\n", Address, HiBit, LoBit, Field);
        } else if (RegIndex > NbRegRangeStart) {
          PciAddr.AddressValue = Address;
          LibAmdPciWrite (AccessWidth32, PciAddr, &Value, &NBPtr->MemPtr->StdHeader);
          if ((FieldName != RegDctAddlData) && (FieldName != RegDctAddlOffset)) {
            IDS_HDT_CONSOLE (MEM_SETREG, "~F%d_%03x [%d:%d] = %x", (Address >> 12) & 0xF, Address & 0xFFF, HiBit, LoBit, Field);
            IDS_HDT_CONSOLE (MEM_SETREG, ", BF enum: %x\n", FieldName);
          }
        } else {
          ASSERT (FALSE);
        }
        if (IsLinked) {
          MemNCmnGetSetFieldKV (NBPtr, 1, FieldName + 1, InputValue >> (HiBit - LoBit + 1));
        }
      } else if (!WholeRegAccess) {
        Value = Value >> LoBit;
        Value &= (((UINT32)1 << (HiBit - LoBit + 1)) - 1);
        if (IsLinked) {
          Value |= MemNCmnGetSetFieldKV (NBPtr, 0, FieldName + 1, 0) << (HiBit - LoBit + 1);
        }
      }
    } while (HasMoreInstances);
  } else {
    IDS_HDT_CONSOLE (MEM_UNDEF_BF, "\t\tUndefined BF enum: %x\n", FieldName);
  }
  return Value;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function is used by families that use a separate DctCfgSel bit to
 *     select the current DCT which will be accessed by function 2.
 *     NOTE:  This function must be called BEFORE the NBPtr->Dct variable is
 *     updated.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]       *Dct     - Pointer to ID of the target DCT
 *
 */

BOOLEAN
MemNDctCfgSelectKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       VOID *Dct
  )
{
  //
  // Sanity check the current DctCfgSel setting
  //
  ASSERT (FIX_DCT (NBPtr->Dct) == NBPtr->GetBitField (NBPtr, BFDctCfgSel));
  //
  // Set the DctCfgSel to new DCT
  // In the case DDR3 with Graphics downcore disabled, channel B is controlled by DCT3
  //
  MemNCmnGetSetFieldKV (NBPtr, 1, BFDctCfgSel, FIX_DCT (*(UINT8 *) Dct));

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
MemNSwitchDctKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 Dct
  )
{
  ASSERT (NBPtr->DctCount > Dct);
  //
  // Set the DctCfgSel to new DCT
  //
  NBPtr->FamilySpecificHook[DCTSelectSwitch] (NBPtr, &Dct);
  NBPtr->Dct = Dct;
  NBPtr->MCTPtr->Dct = NBPtr->Dct;
  NBPtr->DCTPtr = &(NBPtr->MCTPtr->DctData[NBPtr->Dct]);
  NBPtr->PsPtr = &(NBPtr->PSBlock[NBPtr->Dct]);
  NBPtr->DctCachePtr = &(NBPtr->DctCache[NBPtr->Dct]);

  MemNSwitchChannelNb (NBPtr, NBPtr->Channel);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function initializes bit field translation table
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]   NBRegTable[]  - Pointer to the bit field data structure
 *
 */

VOID
MemNInitNBRegTableKV (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   TSEFO NBRegTable[]
  )
{
  UINT16 i;
  ALLOCATE_HEAP_PARAMS AllocHeapParams;
  LOCATE_HEAP_PTR LocHeap;

  // ---------------------------------------------------------------------------
  // Allocate heap for NB register table
  // ---------------------------------------------------------------------------

  // If NBRegTable for this family exists, use it
  LocHeap.BufferHandle = GENERATE_MEM_HANDLE (ALLOC_NB_REG_TABLE, NbRegTabKV, 0, 0);
  if (HeapLocateBuffer (&LocHeap, &(NBPtr->MemPtr->StdHeader)) == AGESA_SUCCESS) {
    NBPtr->NBRegTable = (TSEFO *) LocHeap.BufferPtr;
    return;   // skip populating NBRegTable since it already exists.
  }

  // Allocate new buffer for NBRegTable if it has not been allocated
  AllocHeapParams.RequestedBufferSize = sizeof (TSEFO) * (PmuSRAMRangeEnd);
  AllocHeapParams.BufferHandle = GENERATE_MEM_HANDLE (ALLOC_NB_REG_TABLE, NbRegTabKV, 0, 0);
  AllocHeapParams.Persist = HEAP_SYSTEM_MEM;
  if (AGESA_SUCCESS != HeapAllocateBuffer (&AllocHeapParams, &(NBPtr->MemPtr->StdHeader))) {
    ASSERT (FALSE); // NB and Tech Block Heap allocate error
    return;   // Exit with error
  }
  NBPtr->NBRegTable = (TSEFO *)AllocHeapParams.BufferPtr;
  NBRegTable = NBPtr->NBRegTable;

  // Initialize table with 0s
  for (i = 0; i < BFEndOfList; i++) {
    NBRegTable[i] = 0;
  }

  // ---------------------------------------------------------------------------
  // FUNCTION 1
  // ---------------------------------------------------------------------------
  REG_DEF (NBRegTable, RegDramBase0, _FN (1, 0x40));
  _BF_DEF (NBRegTable, RegDramBase0, BFDramBaseReg0, 31, 0);
  REG_DEF (NBRegTable, RegDramLimit0, _FN (1, 0x44));
  _BF_DEF (NBRegTable, RegDramLimit0, BFDramLimitReg0, 31, 0);
  REG_DEF (NBRegTable, RegDramBaseHi0, _FN (1, 0x140));
  _BF_DEF (NBRegTable, RegDramBaseHi0, BFDramBaseHiReg0, 31, 0);
  REG_DEF (NBRegTable, RegDramLimitHi0, _FN (1, 0x144));
  _BF_DEF (NBRegTable, RegDramLimitHi0, BFDramLimitHiReg0, 31, 0);

  REG_DEF (NBRegTable, RegDramHoleAddr, _FN (1, 0xF0));
  _BF_DEF (NBRegTable, RegDramHoleAddr, BFDramHoleBase, 31, 24);
  _BF_DEF (NBRegTable, RegDramHoleAddr, BFDramHoleOffset, 15,  7);
  _BF_DEF (NBRegTable, RegDramHoleAddr, BFDramMemHoistValid,  1,  1);
  _BF_DEF (NBRegTable, RegDramHoleAddr, BFDramHoleValid,  0,  0);

  REG_DEF (NBRegTable, RegDctCfgSel, _FN (1, 0x10C));
  _BF_DEF (NBRegTable, RegDctCfgSel, BFDctCfgBcEn,  7,  7);
  _BF_DEF (NBRegTable, RegDctCfgSel, BFNbPsSel,  5,  4);
  _BF_DEF (NBRegTable, RegDctCfgSel, BFMemPsSel,  3,  3);
  _BF_DEF (NBRegTable, RegDctCfgSel, BFDctCfgSel,  2,  0);

  REG_DEF (NBRegTable, RegDramBaseSysAddr, _FN (1, 0x120));
  _BF_DEF (NBRegTable, RegDramBaseSysAddr, BFDramBaseAddr, 20,  0);

  REG_DEF (NBRegTable, RegDramLimitSysAddr, _FN (1, 0x124));
  _BF_DEF (NBRegTable, RegDramLimitSysAddr, BFDramLimitAddr, 20,  0);

  REG_DEF (NBRegTable, RegDramCtlrBase0, _FN (1, 0x200));
  _BF_DEF (NBRegTable, RegDramCtlrBase0, BFDctBaseReg0, 31, 0);
  _BF_DEF (NBRegTable, RegDramCtlrBase0, BFLgcyMmioHoleEn0, 1, 1);
  REG_DEF (NBRegTable, RegDramCtlrBase1, _FN (1, 0x208));
  _BF_DEF (NBRegTable, RegDramCtlrBase1, BFDctBaseReg1, 31, 0);
  _BF_DEF (NBRegTable, RegDramCtlrBase1, BFLgcyMmioHoleEn1, 1, 1);
  REG_DEF (NBRegTable, RegDramCtlrBase2, _FN (1, 0x210));
  _BF_DEF (NBRegTable, RegDramCtlrBase2, BFDctBaseReg2, 31, 0);
  _BF_DEF (NBRegTable, RegDramCtlrBase2, BFLgcyMmioHoleEn2, 1, 1);
  REG_DEF (NBRegTable, RegDramCtlrBase3, _FN (1, 0x218));
  _BF_DEF (NBRegTable, RegDramCtlrBase3, BFDctBaseReg3, 31, 0);
  _BF_DEF (NBRegTable, RegDramCtlrBase3, BFLgcyMmioHoleEn3, 1, 1);

  REG_DEF (NBRegTable, RegDramCtlrLimit0, _FN (1, 0x204));
  _BF_DEF (NBRegTable, RegDramCtlrLimit0, BFDctLimitReg0, 31, 0);
  _BF_DEF (NBRegTable, RegDramCtlrLimit0, BFDctIntLvEn0, 3, 0);
  REG_DEF (NBRegTable, RegDramCtlrLimit1, _FN (1, 0x20C));
  _BF_DEF (NBRegTable, RegDramCtlrLimit1, BFDctLimitReg1, 31, 0);
  _BF_DEF (NBRegTable, RegDramCtlrLimit1, BFDctIntLvEn1, 3, 0);
  REG_DEF (NBRegTable, RegDramCtlrLimit2, _FN (1, 0x214));
  _BF_DEF (NBRegTable, RegDramCtlrLimit2, BFDctLimitReg2, 31, 0);
  _BF_DEF (NBRegTable, RegDramCtlrLimit2, BFDctIntLvEn2, 3, 0);
  REG_DEF (NBRegTable, RegDramCtlrLimit3, _FN (1, 0x21C));
  _BF_DEF (NBRegTable, RegDramCtlrLimit3, BFDctLimitReg3, 31, 0);
  _BF_DEF (NBRegTable, RegDramCtlrLimit3, BFDctIntLvEn3, 3, 0);

  // Define old bit field names for UMA Allocation function
  _BF_DEF (NBRegTable, RegDramCtlrLimit0, BFDctSelIntLvEn, 3, 0);   // equivalent to F1x204[DctIntLvEn]
  _BF_DEF (NBRegTable, RegDramCtlrBase1, BFDctSelBaseAddr, 31, 11); // equivalent to F1x208[DctBaseAddr]
  _BF_DEF (NBRegTable, RegDramCtlrBase1, BFDctSelHi, 6, 4);         // equivalent to F1x208[DctSel]

  REG_DEF (NBRegTable, RegDctHiAddrOffset0, _FN (1, 0x240));
  _BF_DEF (NBRegTable, RegDctHiAddrOffset0, BFDctHighAddressOffsetReg0, 31, 0);
  REG_DEF (NBRegTable, RegDctHiAddrOffset1, _FN (1, 0x244));
  _BF_DEF (NBRegTable, RegDctHiAddrOffset1, BFDctHighAddressOffsetReg1, 31, 0);
  REG_DEF (NBRegTable, RegDctHiAddrOffset2, _FN (1, 0x248));
  _BF_DEF (NBRegTable, RegDctHiAddrOffset2, BFDctHighAddressOffsetReg2, 31, 0);
  REG_DEF (NBRegTable, RegDctHiAddrOffset3, _FN (1, 0x24C));
  _BF_DEF (NBRegTable, RegDctHiAddrOffset3, BFDctHighAddressOffsetReg3, 31, 0);

  // ---------------------------------------------------------------------------
  // FUNCTION 2
  // ---------------------------------------------------------------------------
  REG_DEF (NBRegTable, RegCSBaseAddr0, _FN (2, 0x40));
  _BF_DEF (NBRegTable, RegCSBaseAddr0, BFCSBaseAddr0Reg, 31, 0);
  REG_DEF (NBRegTable, RegCSBaseAddr1, _FN (2, 0x44));
  _BF_DEF (NBRegTable, RegCSBaseAddr1, BFCSBaseAddr1Reg, 31, 0);
  REG_DEF (NBRegTable, RegCSBaseAddr2, _FN (2, 0x48));
  _BF_DEF (NBRegTable, RegCSBaseAddr2, BFCSBaseAddr2Reg, 31, 0);
  REG_DEF (NBRegTable, RegCSBaseAddr3, _FN (2, 0x4C));
  _BF_DEF (NBRegTable, RegCSBaseAddr3, BFCSBaseAddr3Reg, 31, 0);

  REG_DEF (NBRegTable, RegCSMask0, _FN (2, 0x60));
  _BF_DEF (NBRegTable, RegCSMask0, BFCSMask0Reg, 31, 0);
  REG_DEF (NBRegTable, RegCSMask1, _FN (2, 0x64));
  _BF_DEF (NBRegTable, RegCSMask1, BFCSMask1Reg, 31, 0);

  REG_DEF (NBRegTable, RegDramCtl, _FN (2, 0x78));
  _BF_DEF (NBRegTable, RegDramCtl, BFDramType, 10, 8);
  _BF_DEF (NBRegTable, RegDramCtl, BFRxPtrInitReq, 6, 6);
  _BF_DEF (NBRegTable, RegDramCtl, BFPtrInitReq, 4, 4);
  _BF_DEF (NBRegTable, RegDramCtl, BFDccPgSsBusDis, 3, 3);
  _BF_DEF (NBRegTable, RegDramCtl, BFGsyncDis, 2, 2);
  _BF_DEF (NBRegTable, RegDramCtl, BFChanVal, 0, 0);

  REG_DEF (NBRegTable, RegDramInit, _FN (2, 0x7C));
  _BF_DEF (NBRegTable, RegDramInit, BFAssertCke, 28, 28);

  REG_DEF (NBRegTable, RegDramBankAddr, _FN (2, 0x80));
  _BF_DEF (NBRegTable, RegDramBankAddr, BFDramBankAddrReg, 7, 0);

  REG_DEF (NBRegTable, RegDramMRS, _FN (2, 0x84));
  _BF_DEF (NBRegTable, RegDramMRS, BFBurstCtrl,  1,  0);

  REG_DEF (NBRegTable, RegDramTmgLo, _FN (2, 0x88));
  _BF_DEF (NBRegTable, RegDramTmgLo, BFMemClkDis, 29, 24);

  REG_DEF (NBRegTable, RegDramTmgHi, _FN (2, 0x8C));
  _BF_DEF (NBRegTable, RegDramTmgHi, BFDisAutoRefresh, 18, 18);

  REG_DEF (NBRegTable, RegDramConfigLo, _FN (2, 0x90));
  _BF_DEF (NBRegTable, RegDramConfigLo, BFIdleCycLimit, 31, 28);
  _BF_DEF (NBRegTable, RegDramConfigLo, BFDisDllShutdownSR, 27, 27);
  _BF_DEF (NBRegTable, RegDramConfigLo, BFPendRefPaybackS3En, 25, 25);
  _BF_DEF (NBRegTable, RegDramConfigLo, BFStagRefEn, 24, 24);
  _BF_DEF (NBRegTable, RegDramConfigLo, BFForceAutoPchg, 23, 23);
  _BF_DEF (NBRegTable, RegDramConfigLo, BFDynPageCloseEn, 20, 20);
  _BF_DEF (NBRegTable, RegDramConfigLo, BFDimmEccEn, 19, 19);
  _BF_DEF (NBRegTable, RegDramConfigLo, BFPendRefPayback, 18, 18);
  _BF_DEF (NBRegTable, RegDramConfigLo, BFEnterSelfRef, 17, 17);
  _BF_DEF (NBRegTable, RegDramConfigLo, BFUnBuffDimm, 16, 16);
  _BF_DEF (NBRegTable, RegDramConfigLo, BFExitSelfRef,  1,  1);

  REG_DEF (NBRegTable, RegDramConfigHi, _FN (2, 0x94));
  _BF_DEF (NBRegTable, RegDramConfigHi, BFDphyMemPsSelEn, 31, 31);
  _BF_DEF (NBRegTable, RegDramConfigHi, BFDcqBypassMax, 28, 24);
  _BF_DEF (NBRegTable, RegDramConfigHi, BFBankSwizzleMode, 22, 22);
  _BF_DEF (NBRegTable, RegDramConfigHi, BFFreqChgInProg, 21, 21);
  _BF_DEF (NBRegTable, RegDramConfigHi, BFSlowAccessMode, 20, 20);
  _BF_DEF (NBRegTable, RegDramConfigHi, BFPowerDownMode, 16, 16);
  _BF_DEF (NBRegTable, RegDramConfigHi, BFPowerDownEn, 15, 15);
  _BF_DEF (NBRegTable, RegDramConfigHi, BFDisDramInterface, 14, 14);
  _BF_DEF (NBRegTable, RegDramConfigHi, BFZqcsInterval, 11, 10);
  _BF_DEF (NBRegTable, RegDramConfigHi, BFMemClkFreqVal, 7, 7);
  _BF_DEF (NBRegTable, RegDramConfigHi, BFMemClkFreq, 4, 0);

  REG_DEF (NBRegTable, RegDctAddlOffset, _FN (2, 0x98));
  REG_DEF (NBRegTable, RegDctAddlData, _FN (2, 0x9C));

  REG_DEF (NBRegTable, RegDctTempThrottle, _FN (2, 0xA4));
  _BF_DEF (NBRegTable, RegDctTempThrottle, BFCmdThrottleMode, 14, 12);
  _BF_DEF (NBRegTable, RegDctTempThrottle, BFBwCapEn,  11, 11);
  _BF_DEF (NBRegTable, RegDctTempThrottle, BFODTSEn,  8, 8);

  REG_DEF (NBRegTable, RegDramCtlrMisc2, _FN (2, 0xA8));
  _BF_DEF (NBRegTable, RegDramCtlrMisc2, BFPerRankTimingEn, 31, 31);
  _BF_DEF (NBRegTable, RegDramCtlrMisc2, BFRefChCmdMgtDis, 29, 29);
  _BF_DEF (NBRegTable, RegDramCtlrMisc2, BFFastSelfRefEntryDis, 28, 28);
  _BF_DEF (NBRegTable, RegDramCtlrMisc2, BFPrtlChPDEnhEn, 22, 22);
  _BF_DEF (NBRegTable, RegDramCtlrMisc2, BFAggrPDEn, 21, 21);
  _BF_DEF (NBRegTable, RegDramCtlrMisc2, BFBankSwap, 20, 20);
  _BF_DEF (NBRegTable, RegDramCtlrMisc2, BFPDPhyPSDis, 19, 19);
  _BF_DEF (NBRegTable, RegDramCtlrMisc2, BFMemPhyPllPdMode, 17, 16);

  REG_DEF (NBRegTable, RegTraceBufferCtlr, _FN (2, 0xC0));
  _BF_DEF (NBRegTable, RegTraceBufferCtlr, BFTraceModeEn,  0,  0);

  REG_DEF (NBRegTable, RegDramCtlrSelLo, _FN (2, 0x110));
  _BF_DEF (NBRegTable, RegDramCtlrSelLo, BFMemCleared, 10, 10);
  _BF_DEF (NBRegTable, RegDramCtlrSelLo, BFMemClrBusy,  9,  9);
  _BF_DEF (NBRegTable, RegDramCtlrSelLo, BFDramEnabled,  8,  8);
  _BF_DEF (NBRegTable, RegDramCtlrSelLo, BFDctSelIntLvAddr,  7,  6);
  _BF_DEF (NBRegTable, RegDramCtlrSelLo, BFMemClrInit,  3,  3);
  _BF_DEF (NBRegTable, RegDramCtlrSelLo, BFBankSwapAddr8En,  2,  2);

  REG_DEF (NBRegTable, RegDramCtlrSelHi, _FN (2, 0x114));
  _BF_DEF (NBRegTable, RegDramCtlrSelHi, BFDctSelIntLvAddrHi,  9,  9);

  REG_DEF (NBRegTable, RegMctCfgLo, _FN (2, 0x118));
  _BF_DEF (NBRegTable, RegMctCfgLo, BFLockDramCfg, 19, 19);
  _BF_DEF (NBRegTable, RegMctCfgLo, BFCC6SaveEn, 18, 18);

  REG_DEF (NBRegTable, RegMctCfgHi, _FN (2, 0x11C));
  _BF_DEF (NBRegTable, RegMctCfgHi, BFMctCfgHiReg, 31, 0);
  _BF_DEF (NBRegTable, RegMctCfgHi, BFPrefIoDis, 13, 13);
  _BF_DEF (NBRegTable, RegMctCfgHi, BFPrefCpuDis, 12, 12);

  REG_DEF (NBRegTable, RegExtMctCfgLo, _FN (2, 0x1B0));

  REG_DEF (NBRegTable, RegExtMctCfgHi, _FN (2, 0x1B4));
  _BF_DEF (NBRegTable, RegExtMctCfgHi, BFFlushWrOnS3StpGnt, 27, 27);
  _BF_DEF (NBRegTable, RegExtMctCfgHi, BFEnSplitMctDatBuffers, 26, 26);

  REG_DEF (NBRegTable, RegCkeToCsMap, _FN (2, 0x1BC));

  REG_DEF (NBRegTable, RegDramNbPstate, _FN (2, 0x210));
  _BF_DEF (NBRegTable, RegDramNbPstate, BFMaxLatency, 31, 22);
  _BF_DEF (NBRegTable, RegDramNbPstate, BFDataTxFifoWrDly, 18, 16);

  REG_DEF (NBRegTable, RegDramCtlrMisc3, _FN (2, 0x244));
  _BF_DEF (NBRegTable, RegDramCtlrMisc3, BFPrtlChPDDynDly, 3, 0);

  // ---------------------------------------------------------------------------
  // Reliable read/write
  // ---------------------------------------------------------------------------
  REG_DEF (NBRegTable, RegDramLpbkTrnCtl, _FN (2, 0x250));
  _BF_DEF (NBRegTable, RegDramLpbkTrnCtl, BFLfsrRollOver ,  13, 13);
  _BF_DEF (NBRegTable, RegDramLpbkTrnCtl, BFCmdSendInProg, 12,  12);
  _BF_DEF (NBRegTable, RegDramLpbkTrnCtl, BFSendCmd, 11,  11);
  _BF_DEF (NBRegTable, RegDramLpbkTrnCtl, BFTestStatus, 10,  10);
  _BF_DEF (NBRegTable, RegDramLpbkTrnCtl, BFCmdTgt,  9,   8);
  _BF_DEF (NBRegTable, RegDramLpbkTrnCtl, BFCmdType,  7,   5);
  _BF_DEF (NBRegTable, RegDramLpbkTrnCtl, BFStopOnErr,  4,   4);
  _BF_DEF (NBRegTable, RegDramLpbkTrnCtl, BFResetAllErr,  3,   3);
  _BF_DEF (NBRegTable, RegDramLpbkTrnCtl, BFCmdTestEnable,  2,   2);

  REG_DEF (NBRegTable, RegTargetABase, _FN (2, 0x254));
  _BF_DEF (NBRegTable, RegTargetABase, BFTgtChipSelectA,  26, 24);
  _BF_DEF (NBRegTable, RegTargetABase, BFTgtBankA,  23, 21);
  _BF_DEF (NBRegTable, RegTargetABase, BFTgtAddressA,   9,  0);

  REG_DEF (NBRegTable, RegDramCmd0, _FN (2, 0x25C));
  _BF_DEF (NBRegTable, RegDramCmd0, BFBubbleCnt2,  31, 22);
  _BF_DEF (NBRegTable, RegDramCmd0, BFBubbleCnt,  21, 12);
  _BF_DEF (NBRegTable, RegDramCmd0, BFCmdStreamLen,   7,  0);

  REG_DEF (NBRegTable, RegDramCmd1, _FN (2, 0x260));
  _BF_DEF (NBRegTable, RegDramCmd1, BFCmdCount,  20,  0);

  REG_DEF (NBRegTable, RegDramStatus1, _FN (2, 0x268));
  _BF_DEF (NBRegTable, RegDramStatus1, BFNibbleErrSts, 19,  0);

  REG_DEF (NBRegTable, RegDramPRBS, _FN (2, 0x270));
  _BF_DEF (NBRegTable, RegDramPRBS, BFDataPrbsSeed, 18,   0);

  REG_DEF (NBRegTable, RegDramDqMaskLo, _FN (2, 0x274));
  _BF_DEF (NBRegTable, RegDramDqMaskLo, BFDramDqMaskLow , 31,  0);

  REG_DEF (NBRegTable, RegDramDqMaskHi, _FN (2, 0x278));
  _BF_DEF (NBRegTable, RegDramDqMaskHi, BFDramDqMaskHigh, 31,  0);

  REG_DEF (NBRegTable, RegDramEccMask, _FN (2, 0x27C));
  _BF_DEF (NBRegTable, RegDramEccMask, BFDramEccMask, 7,  0);

  REG_DEF (NBRegTable, RegDramCmd2, _FN (2, 0x28C));
  _BF_DEF (NBRegTable, RegDramCmd2, BFSendActCmd, 31, 31);
  _BF_DEF (NBRegTable, RegDramCmd2, BFSendPchgCmd, 30, 30);
  _BF_DEF (NBRegTable, RegDramCmd2, BFCmdChipSelect, 29, 22);
  _BF_DEF (NBRegTable, RegDramCmd2, BFCmdBank, 21, 18);
  _BF_DEF (NBRegTable, RegDramCmd2, BFCmdAddress, 17,  0);

  REG_DEF (NBRegTable, RegDQErrLo, _FN (2, 0x294));
  _BF_DEF (NBRegTable, RegDQErrLo, BFDQErrLow, 31,  0);

  REG_DEF (NBRegTable, RegDQErrHi, _FN (2, 0x298));
  _BF_DEF (NBRegTable, RegDQErrHi, BFDQErrHigh, 31,  0);

  // ---------------------------------------------------------------------------
  // GMC
  // ---------------------------------------------------------------------------
  REG_DEF (NBRegTable, RegGmcToDctCtl0, _FN (2, 0x400));
  _BF_DEF (NBRegTable, RegGmcToDctCtl0, BFGmcTokenLimit, 11,  8);
  _BF_DEF (NBRegTable, RegGmcToDctCtl0, BFMctTokenLimit,  3,  0);

  REG_DEF (NBRegTable, RegGmcToDctCtl1, _FN (2, 0x404));
  _BF_DEF (NBRegTable, RegGmcToDctCtl1, BFUrgentTknDis, 31, 31);
  _BF_DEF (NBRegTable, RegGmcToDctCtl1, BFUrGmcMinTokens, 27, 24);
  _BF_DEF (NBRegTable, RegGmcToDctCtl1, BFUrGmcTokenLimit, 20,  16);
  _BF_DEF (NBRegTable, RegGmcToDctCtl1, BFUrMctMinTokens,  11,  8);
  _BF_DEF (NBRegTable, RegGmcToDctCtl1, BFUrMctTokenLimit,  4,  0);

  REG_DEF (NBRegTable, RegGmcToDctCtl2, _FN (2, 0x408));
  _BF_DEF (NBRegTable, RegGmcToDctCtl2, BFCpuElevPrioPeriod, 28, 24);

  // ---------------------------------------------------------------------------
  // FUNCTION 3
  // ---------------------------------------------------------------------------
  REG_DEF (NBRegTable, RegMcaNbCtl, _FN (3, 0x40));
  _BF_DEF (NBRegTable, RegMcaNbCtl, BFMcaNbCtlReg, 31,  0);

  REG_DEF (NBRegTable, RegMcaNbCfg, _FN (3, 0x44));
  _BF_DEF (NBRegTable, RegMcaNbCfg, BFSyncOnUcEccEn, 2,  2);
  _BF_DEF (NBRegTable, RegMcaNbCfg, BFDramEccEn, 22,  22);

  REG_DEF (NBRegTable, RegScrubRateCtl, _FN (3, 0x58));
  _BF_DEF (NBRegTable, RegScrubRateCtl, BFDramScrub, 4,  0);

  REG_DEF (NBRegTable, RegDramScrubAddrLo, _FN (3, 0x5c));
  _BF_DEF (NBRegTable, RegDramScrubAddrLo, BFScrubReDirEn, 0,  0);
  _BF_DEF (NBRegTable, RegDramScrubAddrLo, BFScrubAddrLoReg, 31,  0);

  REG_DEF (NBRegTable, RegDramScrubAddrHi, _FN (3, 0x60));
  _BF_DEF (NBRegTable, RegDramScrubAddrHi, BFScrubAddrHiReg, 31,  0);

  REG_DEF (NBRegTable, RegNbCfg1Lo, _FN (3, 0x88));
  _BF_DEF (NBRegTable, RegNbCfg1Lo, BFDisDramScrub, 27,  27);

  REG_DEF (NBRegTable, RegNbCap, _FN (3, 0xE8));
  _BF_DEF (NBRegTable, RegNbCap, BFEccCapable, 3,  3);

  REG_DEF (NBRegTable, RegCpuid, _FN (3, 0xFC));
  _BF_DEF (NBRegTable, RegCpuid, BFBaseModel, 7,  4);
  _BF_DEF (NBRegTable, RegCpuid, BFExtModel, 19, 16);

  REG_DEF (NBRegTable, RegExtMcaNbCfg, _FN (3, 0x180));
  _BF_DEF (NBRegTable, RegExtMcaNbCfg, BFEccSymbolSize, 25,  25);

  REG_DEF (NBRegTable, RegProductInformationRegister1, _FN (3, 0x1FC));
  _BF_DEF (NBRegTable, RegProductInformationRegister1, BFVddpVddrLowVoltSupp, 21,  21);

  REG_DEF (NBRegTable, RegDctAndFusePowerGateCtrl, _FN (3, 0x2B4));
  _BF_DEF (NBRegTable, RegDctAndFusePowerGateCtrl, BFDctClkGateEn, 11,  8);
  _BF_DEF (NBRegTable, RegDctAndFusePowerGateCtrl, BFDctPwrGateEn, 3,  0);

  // ---------------------------------------------------------------------------
  // FUNCTION 5
  // ---------------------------------------------------------------------------
  REG_DEF (NBRegTable, RegNbCap2, _FN (5, 0x84));
  _BF_DEF (NBRegTable, RegNbCap2, BFDdrMaxRate,  20, 16);
  _BF_DEF (NBRegTable, RegNbCap2, BFDctEn, 15, 12);

  REG_DEF (NBRegTable, RegNbPstate0, _FN (5, 0x160));
  _BF_DEF (NBRegTable, RegNbPstate0, BFMemPstate0, 18, 18);
  _BF_DEF (NBRegTable, RegNbPstate0, BFNbFid0, 6, 1);
  _BF_DEF (NBRegTable, RegNbPstate0, BFNbDid0, 7, 7);
  REG_DEF (NBRegTable, RegNbPstate1, _FN (5, 0x164));
  _BF_DEF (NBRegTable, RegNbPstate1, BFMemPstate1, 18, 18);
  _BF_DEF (NBRegTable, RegNbPstate1, BFNbFid1, 6, 1);
  _BF_DEF (NBRegTable, RegNbPstate1, BFNbDid1, 7, 7);
  REG_DEF (NBRegTable, RegNbPstate2, _FN (5, 0x168));
  _BF_DEF (NBRegTable, RegNbPstate2, BFMemPstate2, 18, 18);
  _BF_DEF (NBRegTable, RegNbPstate2, BFNbFid2, 6, 1);
  _BF_DEF (NBRegTable, RegNbPstate2, BFNbDid2, 7, 7);
  REG_DEF (NBRegTable, RegNbPstate3, _FN (5, 0x16C));
  _BF_DEF (NBRegTable, RegNbPstate3, BFMemPstate3, 18, 18);
  _BF_DEF (NBRegTable, RegNbPstate3, BFNbFid3, 6, 1);
  _BF_DEF (NBRegTable, RegNbPstate3, BFNbDid3, 7, 7);

  REG_DEF (NBRegTable, RegNbPstateCtl, _FN (5, 0x170));
  _BF_DEF (NBRegTable, RegNbPstateCtl, BFNbPstateCtlReg, 31, 0);
  _BF_DEF (NBRegTable, RegNbPstateCtl, BFMemPstateDis, 31, 31);
  _BF_DEF (NBRegTable, RegNbPstateCtl, BFSwNbPstateLoDis, 14, 14);
  _BF_DEF (NBRegTable, RegNbPstateCtl, BFNbPstateHi,  7,  6);
  _BF_DEF (NBRegTable, RegNbPstateCtl, BFNbPstateLo,  4,  3);
  _BF_DEF (NBRegTable, RegNbPstateCtl, BFNbPstateMaxVal,  1,  0);

  REG_DEF (NBRegTable, RegNbPstateStatus, _FN (5, 0x174));
  _BF_DEF (NBRegTable, RegNbPstateStatus, BFCurMemPstate, 24, 24);
  _BF_DEF (NBRegTable, RegNbPstateStatus, BFCurNbPstate, 20, 19);
  _BF_DEF (NBRegTable, RegNbPstateStatus, BFNbPstateDis,  0,  0);


  REG_DEF (NBRegTable, RegNbEccExclusionBaseLow, _FN (5, 0x240));
  _BF_DEF (NBRegTable, RegNbEccExclusionBaseLow, BFECCExclusionBaseLow, 31, 0);

  REG_DEF (NBRegTable, RegNbEccExclusionBaseHigh, _FN (5, 0x244));
  _BF_DEF (NBRegTable, RegNbEccExclusionBaseHigh, BFECCExclusionBaseHigh, 15, 0);

  REG_DEF (NBRegTable, RegNbEccExclusionLimitLow, _FN (5, 0x248));
  _BF_DEF (NBRegTable, RegNbEccExclusionLimitLow, BFECCExclusionLimitLow, 31, 0);

  REG_DEF (NBRegTable, RegNbEccExclusionLimitHigh, _FN (5, 0x24C));
  _BF_DEF (NBRegTable, RegNbEccExclusionLimitHigh, BFECCExclusionLimitHigh, 15, 0);

  // ---------------------------------------------------------------------------
  //
  // DCT PHY REGISTERS
  //
  // ---------------------------------------------------------------------------
  //
  // SW Broadcast:
  //    - DCT PHY registers can have multiple instances per register address.
  //    - BIOS must read-mod-write to all instances to change a bit field.
  //    - For convenience, SetBitField will do SW broadcast by default. The broadcast fields
  //      must be defined with the upper bound of the range as show in below example.
  //    - When SW broadcast is not desired, a particular macro must be used to specify which
  //      field to skip broadcast and pass in the new index of that field. See examples.
  //    - GetBitField always issues single read (no SW broadcast)
  //
  // For example:
  //    D18F2x9C_x0[3,1:0][F,7:0]1_[F,B:0]04A
  //                  |      |        |
  //                  |      |        |
  //                  |      |        +-----------+
  //                  |      +------------------+ |
  //                  +------------------------+| |
  //                                           || |
  //                                           vv v
  //    REG_DEF (NBRegTable, RegDqDqsRxCtl, 0x0171B04A);
  //    _BF_DEF (NBRegTable, RegDqDqsRxCtl, BFMajorMode, 6, 4);
  //
  //    GetBitField (RegDqDqsRxCtl);                   // no broadcast due to register read
  //    SetBitField (RegDqDqsRxCtl, 1);                // broadcast to all instances of that register, with read-mod-write
  //    SetBitField (BFMajorMode, 1);                  // broadcast to all instances of DqDqsRxCtl
  //    SetBitField (PER_MEM_PS (0, BFMajorMode), 1);  // partially broadcast to MP0 instances
  //    SetBitField (SW_BC_DIS (BFMajorMode), 1);      // single write to 0x0171B04A
  //
  REG_DEF (NBRegTable, RegPllMemPs0, 0x00020000);
  REG_DEF (NBRegTable, RegPllMemPs1, 0x00020001);

  REG_DEF (NBRegTable, RegPwrStateCmd, 0x0002000B);

  REG_DEF (NBRegTable, RegGlobalCtl, 0x0002000E);
  _BF_DEF (NBRegTable, RegGlobalCtl, BFPhyDisable,  1,  1);
  _BF_DEF (NBRegTable, RegGlobalCtl, BFG5Mode,  0,  0);

  REG_DEF (NBRegTable, RegMemResetCtl, 0x00020060);
  _BF_DEF (NBRegTable, RegMemResetCtl, BFMemResetL,  0,  0);

  REG_DEF (NBRegTable, RegCalRate, 0x00020088);
  _BF_DEF (NBRegTable, RegCalRate, BFCalRun,  4,  4);
  _BF_DEF (NBRegTable, RegCalRate, BFCalOnce, 5,  5);
  _BF_DEF (NBRegTable, RegCalRate, BFCalInitMode, 7,  7);

  REG_DEF (NBRegTable, RegCalBusy, 0x00020097);

  REG_DEF (NBRegTable, RegGlobalCtlSlaveAbyte, 0x0000000E);
  _BF_DEF (NBRegTable, RegGlobalCtlSlaveAbyte, BFG5ModeAbyte,  0,  0);

  REG_DEF (NBRegTable, RegGlobalCtlSlaveDbyte, 0x0001000E);
  _BF_DEF (NBRegTable, RegGlobalCtlSlaveDbyte, BFG5ModeDbyte,  0,  0);

  REG_DEF (NBRegTable, RegGlobalCtlSlaveAbit, 0x0009000E);
  _BF_DEF (NBRegTable, RegGlobalCtlSlaveAbit, BFG5ModeAbit,  0,  0);

  REG_DEF (NBRegTable, RegPmuClkDiv, 0x01020080);
  _BF_DEF (NBRegTable, RegPmuClkDiv, BFPMUClkDiv, 3, 0);

  REG_DEF (NBRegTable, RegDisCal, 0x00020087);
  _BF_DEF (NBRegTable, RegDisCal, BFDisAutoComp,  1,  1);
  _BF_DEF (NBRegTable, RegDisCal, BFDisPredriverCal,  0,  0);

  REG_DEF (NBRegTable, RegPllRegWaitTime, 0x00020093);
  REG_DEF (NBRegTable, RegPllLockTime, 0x00020089);

  REG_DEF (NBRegTable, RegCalMisc2, 0x00020098);
  _BF_DEF (NBRegTable, RegCalMisc2, BFCalG5D3, 13, 13);
  _BF_DEF (NBRegTable, RegCalMisc2, BFCalCmptrResTrim, 12, 11);

  REG_DEF (NBRegTable, RegMbProtocolShadow, 0x00020004);
  _BF_DEF (NBRegTable, RegMbProtocolShadow, BFUsRdy, 1, 1);
  _BF_DEF (NBRegTable, RegMbProtocolShadow, BFUs2Rdy, 2, 2);

  REG_DEF (NBRegTable, RegUsMbMsg, 0x00020032);
  _BF_DEF (NBRegTable, RegUsMbMsg, BFUsMessage, 15, 0);

  REG_DEF (NBRegTable, RegUsMbProtocol, 0x00020033);
  _BF_DEF (NBRegTable, RegUsMbProtocol, BFRdy, 0, 0);

  REG_DEF (NBRegTable, RegUsMb2Msg, 0x00020034);
  _BF_DEF (NBRegTable, RegUsMb2Msg, BFUs2Message, 15, 0);

  REG_DEF (NBRegTable, RegUsMb2Protocol, 0x00020035);
  _BF_DEF (NBRegTable, RegUsMb2Protocol, BFRdy2, 0, 0);

  REG_DEF (NBRegTable, RegPmuRst, 0x00020099);
  _BF_DEF (NBRegTable, RegPmuRst, BFPmuSramSd, 6, 6);
  _BF_DEF (NBRegTable, RegPmuRst, BFPmuReset, 3, 3);
  _BF_DEF (NBRegTable, RegPmuRst, BFPmuStall, 0, 0);

  REG_DEF (NBRegTable, RegSramMsgBlk, 0x00050000);

  REG_DEF (NBRegTable, RegPhyClkCtl, 0x00FF000D);
  _BF_DEF (NBRegTable, RegPhyClkCtl, BFPhyClkCtl, 0, 0);

  REG_DEF (NBRegTable, RegDqDqsRxCtl, 0x0081B04A);
  _BF_DEF (NBRegTable, RegDqDqsRxCtl, BFMajorMode, 6, 4);

  REG_DEF (NBRegTable, RegDqDqsRcvCntrl1, 0x00F1B04A);

  REG_DEF (NBRegTable, RegRdPtrInitVal, 0x00F0F82E);     // HW broadcast

  REG_DEF (NBRegTable, RegDataRdPtrInitVal, 0x00F1082E); // HW broadcast

  REG_DEF (NBRegTable, RegTxImpedance, 0x03FFF041);   // HW broadcast

  REG_DEF (NBRegTable, RegCadTxImpedance, 0x0130B041);   // CAD chip to pad translation

  REG_DEF (NBRegTable, RegRxCtl1, 0x00FFF04A);           // HW broadcast
  REG_DEF (NBRegTable, RegFAF04A, 0x00FAF04A);           // HW broadcast

  REG_DEF (NBRegTable, RegDataRdPtrOffset, 0x03F10028);  // HW broadcast

  REG_DEF (NBRegTable, RegDataTxImpedance, 0x0181B041);  // Data chip to pad translation

  REG_DEF (NBRegTable, RegDataRxImpedance, 0x0181B04D);  // Data chip to pad translation

  REG_DEF (NBRegTable, RegCadTxSlewRate, 0x0030B05F);    // CAD chip to pad translation

  REG_DEF (NBRegTable, RegDataTxSlewRate, 0x0081B05F);   // Data chip to pad translation

  REG_DEF (NBRegTable, RegAcsmOdtCtrl0, 0x00F400E0);
  REG_DEF (NBRegTable, RegAcsmOdtCtrl1, 0x00F400E1);
  REG_DEF (NBRegTable, RegAcsmOdtCtrl2, 0x00F400E2);
  REG_DEF (NBRegTable, RegAcsmOdtCtrl3, 0x00F400E3);
  REG_DEF (NBRegTable, RegAcsmOdtCtrl4, 0x00F400E4);
  REG_DEF (NBRegTable, RegAcsmOdtCtrl5, 0x00F400E5);
  REG_DEF (NBRegTable, RegAcsmOdtCtrl6, 0x00F400E6);
  REG_DEF (NBRegTable, RegAcsmOdtCtrl7, 0x00F400E7);

  REG_DEF (NBRegTable, RegVrefByteAbyte, 0x00F00015);
  REG_DEF (NBRegTable, RegVrefByteDbyte, 0x00F10015);
  REG_DEF (NBRegTable, RegVrefByteMaster, 0x00F20015);

  REG_DEF (NBRegTable, RegProcOdtTmg, 0x00F10016);      // HW broadcast

  // Registers trained by PMU
  REG_DEF (NBRegTable, RegDataRxDly, 0x01819380);
  REG_DEF (NBRegTable, RegDataTxDly, 0x01819381);
  REG_DEF (NBRegTable, RegDataTxEqHiImp, 0x0181B046);
  REG_DEF (NBRegTable, RegDataTxEqLoImp, 0x0181B047);
  REG_DEF (NBRegTable, RegDataTxEqBoostImp, 0x0181B048);

  REG_DEF (NBRegTable, RegRdPtrOffsetCad0Group0, 0x00000028);
  REG_DEF (NBRegTable, RegRdPtrOffsetCad0Group1, 0x00001028);
  REG_DEF (NBRegTable, RegRdPtrOffsetCad0Group2, 0x00002028);
  REG_DEF (NBRegTable, RegRdPtrOffsetCad0Group3, 0x00003028);
  REG_DEF (NBRegTable, RegRdPtrOffsetCad1Group0, 0x00100028);
  REG_DEF (NBRegTable, RegRdPtrOffsetCad1Group1, 0x00101028);
  REG_DEF (NBRegTable, RegRdPtrOffsetCad1Group2, 0x00102028);
  REG_DEF (NBRegTable, RegRdPtrOffsetCad1Group3, 0x00103028);
  REG_DEF (NBRegTable, RegRdPtrOffsetCad2Group0, 0x00200028);
  REG_DEF (NBRegTable, RegRdPtrOffsetCad2Group1, 0x00201028);
  REG_DEF (NBRegTable, RegRdPtrOffsetCad2Group2, 0x00202028);
  REG_DEF (NBRegTable, RegRdPtrOffsetCad2Group3, 0x00203028);
  REG_DEF (NBRegTable, RegRdPtrOffsetCad3Group0, 0x00300028);
  REG_DEF (NBRegTable, RegRdPtrOffsetCad3Group1, 0x00301028);
  REG_DEF (NBRegTable, RegRdPtrOffsetCad3Group2, 0x00302028);
  REG_DEF (NBRegTable, RegRdPtrOffsetCad3Group3, 0x00303028);

  REG_DEF (NBRegTable, RegDataRdPtrOffset0, 0x00010028);
  REG_DEF (NBRegTable, RegDataRdPtrOffset1, 0x00110028);
  REG_DEF (NBRegTable, RegDataRdPtrOffset2, 0x00210028);
  REG_DEF (NBRegTable, RegDataRdPtrOffset3, 0x00310028);
  REG_DEF (NBRegTable, RegDataRdPtrOffset4, 0x00410028);
  REG_DEF (NBRegTable, RegDataRdPtrOffset5, 0x00510028);
  REG_DEF (NBRegTable, RegDataRdPtrOffset6, 0x00610028);
  REG_DEF (NBRegTable, RegDataRdPtrOffset7, 0x00710028);
  REG_DEF (NBRegTable, RegDataRdPtrOffset8, 0x00810028);

  REG_DEF (NBRegTable, RegTxDlyCad0Group0, 0x00000081);
  REG_DEF (NBRegTable, RegTxDlyCad0Group1, 0x00001081);
  REG_DEF (NBRegTable, RegTxDlyCad0Group2, 0x00002081);
  REG_DEF (NBRegTable, RegTxDlyCad0Group3, 0x00003081);
  REG_DEF (NBRegTable, RegTxDlyCad1Group0, 0x00100081);
  REG_DEF (NBRegTable, RegTxDlyCad1Group1, 0x00101081);
  REG_DEF (NBRegTable, RegTxDlyCad1Group2, 0x00102081);
  REG_DEF (NBRegTable, RegTxDlyCad1Group3, 0x00103081);
  REG_DEF (NBRegTable, RegTxDlyCad2Group0, 0x00200081);
  REG_DEF (NBRegTable, RegTxDlyCad2Group1, 0x00201081);
  REG_DEF (NBRegTable, RegTxDlyCad2Group2, 0x00202081);
  REG_DEF (NBRegTable, RegTxDlyCad2Group3, 0x00203081);
  REG_DEF (NBRegTable, RegTxDlyCad3Group0, 0x00300081);
  REG_DEF (NBRegTable, RegTxDlyCad3Group1, 0x00301081);
  REG_DEF (NBRegTable, RegTxDlyCad3Group2, 0x00302081);
  REG_DEF (NBRegTable, RegTxDlyCad3Group3, 0x00303081);

  REG_DEF (NBRegTable, RegDllTuningCad0Group0, 0x0000006F);
  REG_DEF (NBRegTable, RegDllTuningCad0Group1, 0x0000016F);
  REG_DEF (NBRegTable, RegDllTuningCad0Group2, 0x0000026F);
  REG_DEF (NBRegTable, RegDllTuningCad1Group0, 0x0010006F);
  REG_DEF (NBRegTable, RegDllTuningCad1Group1, 0x0010016F);
  REG_DEF (NBRegTable, RegDllTuningCad1Group2, 0x0010026F);
  REG_DEF (NBRegTable, RegDllTuningCad2Group0, 0x0020006F);
  REG_DEF (NBRegTable, RegDllTuningCad2Group1, 0x0020016F);
  REG_DEF (NBRegTable, RegDllTuningCad2Group2, 0x0020026F);
  REG_DEF (NBRegTable, RegDllTuningCad3Group0, 0x0030006F);
  REG_DEF (NBRegTable, RegDllTuningCad3Group1, 0x0030016F);
  REG_DEF (NBRegTable, RegDllTuningCad3Group2, 0x0030026F);

  REG_DEF (NBRegTable, RegDataDllTuningCad0Group0, 0x0001006F);
  REG_DEF (NBRegTable, RegDataDllTuningCad0Group1, 0x0001016F);
  REG_DEF (NBRegTable, RegDataDllTuningCad0Group2, 0x0001026F);
  REG_DEF (NBRegTable, RegDataDllTuningCad1Group0, 0x0011006F);
  REG_DEF (NBRegTable, RegDataDllTuningCad1Group1, 0x0011016F);
  REG_DEF (NBRegTable, RegDataDllTuningCad1Group2, 0x0011026F);
  REG_DEF (NBRegTable, RegDataDllTuningCad2Group0, 0x0021006F);
  REG_DEF (NBRegTable, RegDataDllTuningCad2Group1, 0x0021016F);
  REG_DEF (NBRegTable, RegDataDllTuningCad2Group2, 0x0021026F);
  REG_DEF (NBRegTable, RegDataDllTuningCad3Group0, 0x0031006F);
  REG_DEF (NBRegTable, RegDataDllTuningCad3Group1, 0x0031016F);
  REG_DEF (NBRegTable, RegDataDllTuningCad3Group2, 0x0031026F);
  REG_DEF (NBRegTable, RegDataDllTuningCad4Group0, 0x0041006F);
  REG_DEF (NBRegTable, RegDataDllTuningCad4Group1, 0x0041016F);
  REG_DEF (NBRegTable, RegDataDllTuningCad4Group2, 0x0041026F);
  REG_DEF (NBRegTable, RegDataDllTuningCad5Group0, 0x0051006F);
  REG_DEF (NBRegTable, RegDataDllTuningCad5Group1, 0x0051016F);
  REG_DEF (NBRegTable, RegDataDllTuningCad5Group2, 0x0051026F);
  REG_DEF (NBRegTable, RegDataDllTuningCad6Group0, 0x0061006F);
  REG_DEF (NBRegTable, RegDataDllTuningCad6Group1, 0x0061016F);
  REG_DEF (NBRegTable, RegDataDllTuningCad6Group2, 0x0061026F);
  REG_DEF (NBRegTable, RegDataDllTuningCad7Group0, 0x0071006F);
  REG_DEF (NBRegTable, RegDataDllTuningCad7Group1, 0x0071016F);
  REG_DEF (NBRegTable, RegDataDllTuningCad7Group2, 0x0071026F);
  REG_DEF (NBRegTable, RegDataDllTuningCad8Group0, 0x0081006F);
  REG_DEF (NBRegTable, RegDataDllTuningCad8Group1, 0x0081016F);
  REG_DEF (NBRegTable, RegDataDllTuningCad8Group2, 0x0081026F);

  REG_DEF (NBRegTable, RegDataDqDqsRxCtl00, 0x0001004A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl01, 0x0001104A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl02, 0x0001204A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl03, 0x0001304A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl04, 0x0001404A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl05, 0x0001504A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl06, 0x0001604A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl07, 0x0001704A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl08, 0x0001804A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl09, 0x0001904A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl0a, 0x0001A04A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl0b, 0x0001B04A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl10, 0x0011004A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl11, 0x0011104A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl12, 0x0011204A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl13, 0x0011304A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl14, 0x0011404A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl15, 0x0011504A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl16, 0x0011604A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl17, 0x0011704A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl18, 0x0011804A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl19, 0x0011904A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl1a, 0x0011A04A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl1b, 0x0011B04A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl20, 0x0021004A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl21, 0x0021104A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl22, 0x0021204A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl23, 0x0021304A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl24, 0x0021404A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl25, 0x0021504A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl26, 0x0021604A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl27, 0x0021704A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl28, 0x0021804A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl29, 0x0021904A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl2a, 0x0021A04A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl2b, 0x0021B04A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl30, 0x0031004A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl31, 0x0031104A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl32, 0x0031204A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl33, 0x0031304A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl34, 0x0031404A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl35, 0x0031504A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl36, 0x0031604A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl37, 0x0031704A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl38, 0x0031804A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl39, 0x0031904A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl3a, 0x0031A04A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl3b, 0x0031B04A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl40, 0x0041004A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl41, 0x0041104A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl42, 0x0041204A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl43, 0x0041304A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl44, 0x0041404A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl45, 0x0041504A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl46, 0x0041604A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl47, 0x0041704A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl48, 0x0041804A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl49, 0x0041904A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl4a, 0x0041A04A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl4b, 0x0041B04A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl50, 0x0051004A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl51, 0x0051104A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl52, 0x0051204A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl53, 0x0051304A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl54, 0x0051404A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl55, 0x0051504A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl56, 0x0051604A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl57, 0x0051704A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl58, 0x0051804A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl59, 0x0051904A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl5a, 0x0051A04A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl5b, 0x0051B04A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl60, 0x0061004A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl61, 0x0061104A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl62, 0x0061204A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl63, 0x0061304A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl64, 0x0061404A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl65, 0x0061504A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl66, 0x0061604A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl67, 0x0061704A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl68, 0x0061804A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl69, 0x0061904A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl6a, 0x0061A04A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl6b, 0x0061B04A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl70, 0x0071004A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl71, 0x0071104A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl72, 0x0071204A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl73, 0x0071304A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl74, 0x0071404A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl75, 0x0071504A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl76, 0x0071604A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl77, 0x0071704A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl78, 0x0071804A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl79, 0x0071904A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl7a, 0x0071A04A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl7b, 0x0071B04A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl80, 0x0081004A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl81, 0x0081104A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl82, 0x0081204A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl83, 0x0081304A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl84, 0x0081404A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl85, 0x0081504A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl86, 0x0081604A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl87, 0x0081704A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl88, 0x0081804A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl89, 0x0081904A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl8a, 0x0081A04A);
  REG_DEF (NBRegTable, RegDataDqDqsRxCtl8b, 0x0081B04A);

  REG_DEF (NBRegTable, RegDataRxDly000, 0x00010080);
  REG_DEF (NBRegTable, RegDataRxDly001, 0x00010180);
  REG_DEF (NBRegTable, RegDataRxDly010, 0x00011080);
  REG_DEF (NBRegTable, RegDataRxDly011, 0x00011180);
  REG_DEF (NBRegTable, RegDataRxDly020, 0x00012080);
  REG_DEF (NBRegTable, RegDataRxDly021, 0x00012180);
  REG_DEF (NBRegTable, RegDataRxDly030, 0x00013080);
  REG_DEF (NBRegTable, RegDataRxDly031, 0x00013180);
  REG_DEF (NBRegTable, RegDataRxDly100, 0x00110080);
  REG_DEF (NBRegTable, RegDataRxDly101, 0x00110180);
  REG_DEF (NBRegTable, RegDataRxDly110, 0x00111080);
  REG_DEF (NBRegTable, RegDataRxDly111, 0x00111180);
  REG_DEF (NBRegTable, RegDataRxDly120, 0x00112080);
  REG_DEF (NBRegTable, RegDataRxDly121, 0x00112180);
  REG_DEF (NBRegTable, RegDataRxDly130, 0x00113080);
  REG_DEF (NBRegTable, RegDataRxDly131, 0x00113180);
  REG_DEF (NBRegTable, RegDataRxDly200, 0x00210080);
  REG_DEF (NBRegTable, RegDataRxDly201, 0x00210180);
  REG_DEF (NBRegTable, RegDataRxDly210, 0x00211080);
  REG_DEF (NBRegTable, RegDataRxDly211, 0x00211180);
  REG_DEF (NBRegTable, RegDataRxDly220, 0x00212080);
  REG_DEF (NBRegTable, RegDataRxDly221, 0x00212180);
  REG_DEF (NBRegTable, RegDataRxDly230, 0x00213080);
  REG_DEF (NBRegTable, RegDataRxDly231, 0x00213180);
  REG_DEF (NBRegTable, RegDataRxDly300, 0x00310080);
  REG_DEF (NBRegTable, RegDataRxDly301, 0x00310180);
  REG_DEF (NBRegTable, RegDataRxDly310, 0x00311080);
  REG_DEF (NBRegTable, RegDataRxDly311, 0x00311180);
  REG_DEF (NBRegTable, RegDataRxDly320, 0x00312080);
  REG_DEF (NBRegTable, RegDataRxDly321, 0x00312180);
  REG_DEF (NBRegTable, RegDataRxDly330, 0x00313080);
  REG_DEF (NBRegTable, RegDataRxDly331, 0x00313180);
  REG_DEF (NBRegTable, RegDataRxDly400, 0x00410080);
  REG_DEF (NBRegTable, RegDataRxDly401, 0x00410180);
  REG_DEF (NBRegTable, RegDataRxDly410, 0x00411080);
  REG_DEF (NBRegTable, RegDataRxDly411, 0x00411180);
  REG_DEF (NBRegTable, RegDataRxDly420, 0x00412080);
  REG_DEF (NBRegTable, RegDataRxDly421, 0x00412180);
  REG_DEF (NBRegTable, RegDataRxDly430, 0x00413080);
  REG_DEF (NBRegTable, RegDataRxDly431, 0x00413180);
  REG_DEF (NBRegTable, RegDataRxDly500, 0x00510080);
  REG_DEF (NBRegTable, RegDataRxDly501, 0x00510180);
  REG_DEF (NBRegTable, RegDataRxDly510, 0x00511080);
  REG_DEF (NBRegTable, RegDataRxDly511, 0x00511180);
  REG_DEF (NBRegTable, RegDataRxDly520, 0x00512080);
  REG_DEF (NBRegTable, RegDataRxDly521, 0x00512180);
  REG_DEF (NBRegTable, RegDataRxDly530, 0x00513080);
  REG_DEF (NBRegTable, RegDataRxDly531, 0x00513180);
  REG_DEF (NBRegTable, RegDataRxDly600, 0x00610080);
  REG_DEF (NBRegTable, RegDataRxDly601, 0x00610180);
  REG_DEF (NBRegTable, RegDataRxDly610, 0x00611080);
  REG_DEF (NBRegTable, RegDataRxDly611, 0x00611180);
  REG_DEF (NBRegTable, RegDataRxDly620, 0x00612080);
  REG_DEF (NBRegTable, RegDataRxDly621, 0x00612180);
  REG_DEF (NBRegTable, RegDataRxDly630, 0x00613080);
  REG_DEF (NBRegTable, RegDataRxDly631, 0x00613180);
  REG_DEF (NBRegTable, RegDataRxDly700, 0x00710080);
  REG_DEF (NBRegTable, RegDataRxDly701, 0x00710180);
  REG_DEF (NBRegTable, RegDataRxDly710, 0x00711080);
  REG_DEF (NBRegTable, RegDataRxDly711, 0x00711180);
  REG_DEF (NBRegTable, RegDataRxDly720, 0x00712080);
  REG_DEF (NBRegTable, RegDataRxDly721, 0x00712180);
  REG_DEF (NBRegTable, RegDataRxDly730, 0x00713080);
  REG_DEF (NBRegTable, RegDataRxDly731, 0x00713180);
  REG_DEF (NBRegTable, RegDataRxDly800, 0x00810080);
  REG_DEF (NBRegTable, RegDataRxDly801, 0x00810180);
  REG_DEF (NBRegTable, RegDataRxDly810, 0x00811080);
  REG_DEF (NBRegTable, RegDataRxDly811, 0x00811180);
  REG_DEF (NBRegTable, RegDataRxDly820, 0x00812080);
  REG_DEF (NBRegTable, RegDataRxDly821, 0x00812180);
  REG_DEF (NBRegTable, RegDataRxDly830, 0x00813080);
  REG_DEF (NBRegTable, RegDataRxDly831, 0x00813180);

  REG_DEF (NBRegTable, RegDataTxDly000, 0x00010081);
  REG_DEF (NBRegTable, RegDataTxDly001, 0x00010181);
  REG_DEF (NBRegTable, RegDataTxDly010, 0x00011081);
  REG_DEF (NBRegTable, RegDataTxDly011, 0x00011181);
  REG_DEF (NBRegTable, RegDataTxDly020, 0x00012081);
  REG_DEF (NBRegTable, RegDataTxDly021, 0x00012181);
  REG_DEF (NBRegTable, RegDataTxDly030, 0x00013081);
  REG_DEF (NBRegTable, RegDataTxDly031, 0x00013181);
  REG_DEF (NBRegTable, RegDataTxDly100, 0x00110081);
  REG_DEF (NBRegTable, RegDataTxDly101, 0x00110181);
  REG_DEF (NBRegTable, RegDataTxDly110, 0x00111081);
  REG_DEF (NBRegTable, RegDataTxDly111, 0x00111181);
  REG_DEF (NBRegTable, RegDataTxDly120, 0x00112081);
  REG_DEF (NBRegTable, RegDataTxDly121, 0x00112181);
  REG_DEF (NBRegTable, RegDataTxDly130, 0x00113081);
  REG_DEF (NBRegTable, RegDataTxDly131, 0x00113181);
  REG_DEF (NBRegTable, RegDataTxDly200, 0x00210081);
  REG_DEF (NBRegTable, RegDataTxDly201, 0x00210181);
  REG_DEF (NBRegTable, RegDataTxDly210, 0x00211081);
  REG_DEF (NBRegTable, RegDataTxDly211, 0x00211181);
  REG_DEF (NBRegTable, RegDataTxDly220, 0x00212081);
  REG_DEF (NBRegTable, RegDataTxDly221, 0x00212181);
  REG_DEF (NBRegTable, RegDataTxDly230, 0x00213081);
  REG_DEF (NBRegTable, RegDataTxDly231, 0x00213181);
  REG_DEF (NBRegTable, RegDataTxDly300, 0x00310081);
  REG_DEF (NBRegTable, RegDataTxDly301, 0x00310181);
  REG_DEF (NBRegTable, RegDataTxDly310, 0x00311081);
  REG_DEF (NBRegTable, RegDataTxDly311, 0x00311181);
  REG_DEF (NBRegTable, RegDataTxDly320, 0x00312081);
  REG_DEF (NBRegTable, RegDataTxDly321, 0x00312181);
  REG_DEF (NBRegTable, RegDataTxDly330, 0x00313081);
  REG_DEF (NBRegTable, RegDataTxDly331, 0x00313181);
  REG_DEF (NBRegTable, RegDataTxDly400, 0x00410081);
  REG_DEF (NBRegTable, RegDataTxDly401, 0x00410181);
  REG_DEF (NBRegTable, RegDataTxDly410, 0x00411081);
  REG_DEF (NBRegTable, RegDataTxDly411, 0x00411181);
  REG_DEF (NBRegTable, RegDataTxDly420, 0x00412081);
  REG_DEF (NBRegTable, RegDataTxDly421, 0x00412181);
  REG_DEF (NBRegTable, RegDataTxDly430, 0x00413081);
  REG_DEF (NBRegTable, RegDataTxDly431, 0x00413181);
  REG_DEF (NBRegTable, RegDataTxDly500, 0x00510081);
  REG_DEF (NBRegTable, RegDataTxDly501, 0x00510181);
  REG_DEF (NBRegTable, RegDataTxDly510, 0x00511081);
  REG_DEF (NBRegTable, RegDataTxDly511, 0x00511181);
  REG_DEF (NBRegTable, RegDataTxDly520, 0x00512081);
  REG_DEF (NBRegTable, RegDataTxDly521, 0x00512181);
  REG_DEF (NBRegTable, RegDataTxDly530, 0x00513081);
  REG_DEF (NBRegTable, RegDataTxDly531, 0x00513181);
  REG_DEF (NBRegTable, RegDataTxDly600, 0x00610081);
  REG_DEF (NBRegTable, RegDataTxDly601, 0x00610181);
  REG_DEF (NBRegTable, RegDataTxDly610, 0x00611081);
  REG_DEF (NBRegTable, RegDataTxDly611, 0x00611181);
  REG_DEF (NBRegTable, RegDataTxDly620, 0x00612081);
  REG_DEF (NBRegTable, RegDataTxDly621, 0x00612181);
  REG_DEF (NBRegTable, RegDataTxDly630, 0x00613081);
  REG_DEF (NBRegTable, RegDataTxDly631, 0x00613181);
  REG_DEF (NBRegTable, RegDataTxDly700, 0x00710081);
  REG_DEF (NBRegTable, RegDataTxDly701, 0x00710181);
  REG_DEF (NBRegTable, RegDataTxDly710, 0x00711081);
  REG_DEF (NBRegTable, RegDataTxDly711, 0x00711181);
  REG_DEF (NBRegTable, RegDataTxDly720, 0x00712081);
  REG_DEF (NBRegTable, RegDataTxDly721, 0x00712181);
  REG_DEF (NBRegTable, RegDataTxDly730, 0x00713081);
  REG_DEF (NBRegTable, RegDataTxDly731, 0x00713181);
  REG_DEF (NBRegTable, RegDataTxDly800, 0x00810081);
  REG_DEF (NBRegTable, RegDataTxDly801, 0x00810181);
  REG_DEF (NBRegTable, RegDataTxDly810, 0x00811081);
  REG_DEF (NBRegTable, RegDataTxDly811, 0x00811181);
  REG_DEF (NBRegTable, RegDataTxDly820, 0x00812081);
  REG_DEF (NBRegTable, RegDataTxDly821, 0x00812181);
  REG_DEF (NBRegTable, RegDataTxDly830, 0x00813081);
  REG_DEF (NBRegTable, RegDataTxDly831, 0x00813181);

  REG_DEF (NBRegTable, RegCadTxImpedance00, 0x00000041);
  REG_DEF (NBRegTable, RegCadTxImpedance01, 0x00001041);
  REG_DEF (NBRegTable, RegCadTxImpedance02, 0x00002041);
  REG_DEF (NBRegTable, RegCadTxImpedance03, 0x00003041);
  REG_DEF (NBRegTable, RegCadTxImpedance04, 0x00004041);
  REG_DEF (NBRegTable, RegCadTxImpedance05, 0x00005041);
  REG_DEF (NBRegTable, RegCadTxImpedance06, 0x00006041);
  REG_DEF (NBRegTable, RegCadTxImpedance07, 0x00007041);
  REG_DEF (NBRegTable, RegCadTxImpedance08, 0x00008041);
  REG_DEF (NBRegTable, RegCadTxImpedance09, 0x00009041);
  REG_DEF (NBRegTable, RegCadTxImpedance0a, 0x0000A041);
  REG_DEF (NBRegTable, RegCadTxImpedance0b, 0x0000B041);
  REG_DEF (NBRegTable, RegCadTxImpedance10, 0x00100041);
  REG_DEF (NBRegTable, RegCadTxImpedance11, 0x00101041);
  REG_DEF (NBRegTable, RegCadTxImpedance12, 0x00102041);
  REG_DEF (NBRegTable, RegCadTxImpedance13, 0x00103041);
  REG_DEF (NBRegTable, RegCadTxImpedance14, 0x00104041);
  REG_DEF (NBRegTable, RegCadTxImpedance15, 0x00105041);
  REG_DEF (NBRegTable, RegCadTxImpedance16, 0x00106041);
  REG_DEF (NBRegTable, RegCadTxImpedance17, 0x00107041);
  REG_DEF (NBRegTable, RegCadTxImpedance18, 0x00108041);
  REG_DEF (NBRegTable, RegCadTxImpedance19, 0x00109041);
  REG_DEF (NBRegTable, RegCadTxImpedance1a, 0x0010A041);
  REG_DEF (NBRegTable, RegCadTxImpedance1b, 0x0010B041);
  REG_DEF (NBRegTable, RegCadTxImpedance20, 0x00200041);
  REG_DEF (NBRegTable, RegCadTxImpedance21, 0x00201041);
  REG_DEF (NBRegTable, RegCadTxImpedance22, 0x00202041);
  REG_DEF (NBRegTable, RegCadTxImpedance23, 0x00203041);
  REG_DEF (NBRegTable, RegCadTxImpedance24, 0x00204041);
  REG_DEF (NBRegTable, RegCadTxImpedance25, 0x00205041);
  REG_DEF (NBRegTable, RegCadTxImpedance26, 0x00206041);
  REG_DEF (NBRegTable, RegCadTxImpedance27, 0x00207041);
  REG_DEF (NBRegTable, RegCadTxImpedance28, 0x00208041);
  REG_DEF (NBRegTable, RegCadTxImpedance29, 0x00209041);
  REG_DEF (NBRegTable, RegCadTxImpedance2a, 0x0020A041);
  REG_DEF (NBRegTable, RegCadTxImpedance2b, 0x0020B041);
  REG_DEF (NBRegTable, RegCadTxImpedance30, 0x00300041);
  REG_DEF (NBRegTable, RegCadTxImpedance31, 0x00301041);
  REG_DEF (NBRegTable, RegCadTxImpedance32, 0x00302041);
  REG_DEF (NBRegTable, RegCadTxImpedance33, 0x00303041);
  REG_DEF (NBRegTable, RegCadTxImpedance34, 0x00304041);
  REG_DEF (NBRegTable, RegCadTxImpedance35, 0x00305041);
  REG_DEF (NBRegTable, RegCadTxImpedance36, 0x00306041);
  REG_DEF (NBRegTable, RegCadTxImpedance37, 0x00307041);
  REG_DEF (NBRegTable, RegCadTxImpedance38, 0x00308041);
  REG_DEF (NBRegTable, RegCadTxImpedance39, 0x00309041);
  REG_DEF (NBRegTable, RegCadTxImpedance3a, 0x0030A041);
  REG_DEF (NBRegTable, RegCadTxImpedance3b, 0x0030B041);

  REG_DEF (NBRegTable, RegClkTreeTermCntrMstr, 0x000200A7);

  // DRAM PHY Power Savings
  REG_DEF (NBRegTable, RegCadDllLockMaintenance, 0x01300014);
  _BF_DEF (NBRegTable, RegCadDllLockMaintenance, BFCadMaxDurDllNoLock,  3,  0);
  _BF_DEF (NBRegTable, RegCadDllLockMaintenance, BFCadDllPumpPeriod, 7,  4);

  REG_DEF (NBRegTable, RegDataDllLockMaintenance, 0x01810014);
  _BF_DEF (NBRegTable, RegDataDllLockMaintenance, BFDataMaxDurDllNoLock,  3,  0);
  _BF_DEF (NBRegTable, RegDataDllLockMaintenance, BFDataDllPumpPeriod, 7,  4);

  REG_DEF (NBRegTable, RegDctPhyA78DllControl, 0x00F00078);  // HW broadcast
  REG_DEF (NBRegTable, RegDctPhyD78DllControl, 0x00F10F78);  // HW broadcast
  REG_DEF (NBRegTable, RegDctPhy0x01F1F045, 0x01F1F045);     // HW broadcast

  REG_DEF (NBRegTable, RegVrefInGlobal, 0x000200B2);
  _BF_DEF (NBRegTable, RegVrefInGlobal, BFGlobalVrefInSel, 2, 0);

  REG_DEF (NBRegTable, RegVrefOutGlobal0, 0x000200B3);
  _BF_DEF (NBRegTable, RegVrefOutGlobal0, BFGlobalVrefOut0Sel, 2, 0);

  REG_DEF (NBRegTable, RegVrefOutGlobal1, 0x000200B4);
  _BF_DEF (NBRegTable, RegVrefOutGlobal1, BFGlobalVrefOut1Sel, 2, 0);

  REG_DEF (NBRegTable, RegVrefOutGlobal2, 0x000200B5);
  _BF_DEF (NBRegTable, RegVrefOutGlobal2, BFGlobalVrefOut2Sel, 2, 0);

  REG_DEF (NBRegTable, RegVrefOutGlobal3, 0x000200B6);
  _BF_DEF (NBRegTable, RegVrefOutGlobal3, BFGlobalVrefOut3Sel, 2, 0);

  // Lane to CRC Map
  REG_DEF (NBRegTable, RegLaneToCrcMap0, 0x00070015);
  REG_DEF (NBRegTable, RegLaneToCrcMap1, 0x00070016);

  // VrefDnom
  REG_DEF (NBRegTable, RegDctPhy0x01F1F043, 0x01F1F043);     // HW broadcast

  // TxControlDq
  REG_DEF (NBRegTable, RegDctPhy0x00F0F04E, 0x00F0F04E);     // HW broadcast
  REG_DEF (NBRegTable, RegDctPhy0x00F1F04E, 0x00F1F04E);     // HW broadcast

  REG_DEF (NBRegTable, RegTxControlDq, 0x03F6F04E);         // HW broadcast
  REG_DEF (NBRegTable, RegCadByteDbgCtrl, 0x00F0001A);      // HW broadcast
  REG_DEF (NBRegTable, RegDataByteDbgCtrl, 0x00F1001A);     // HW broadcast
  REG_DEF (NBRegTable, RegMasterByteDbgCtrl, 0x0002001A);
  REG_DEF (NBRegTable, RegMasterD3EvntMerr, 0x0002005B);
  REG_DEF (NBRegTable, RegDataDqDqsRcvCntrl3, 0x00F1F051);  // HW broadcast
  REG_DEF (NBRegTable, RegDataRcvMajorMode, 0x01F1F043);    // HW broadcast

  // Unused DBYTE 9
  REG_DEF (NBRegTable, RegDctPhyD0091F04A, 0x0091F04A);  // HW broadcast
  REG_DEF (NBRegTable, RegDctPhyD00910F77, 0x00910F77);  // HW broadcast
  REG_DEF (NBRegTable, RegDctPhyD00910000, 0x00910000);
  REG_DEF (NBRegTable, RegDctPhyD0391F04D, 0x0391F04D);  // HW broadcast
  REG_DEF (NBRegTable, RegDctPhyD0391F041, 0x0391F041);  // HW broadcast

  // Unused DBYTE receivers
  REG_DEF (NBRegTable, RegDctPhyD00F1804A, 0x00F1804A);  // HW broadcast
  REG_DEF (NBRegTable, RegDctPhyD00F1904A, 0x00F1904A);  // HW broadcast
  REG_DEF (NBRegTable, RegDctPhyD00F1B04A, 0x00F1B04A);  // HW broadcast

  // ---------------------------------------------------------------------------
  // LINK BITFIELD
  // ---------------------------------------------------------------------------
  LINK_BF (NBRegTable, BFDctSelIntLvAddr, BFDctSelIntLvAddrHi);
  LINK_BF (NBRegTable, BFBaseModel, BFExtModel);

  IDS_OPTION_HOOK (IDS_INIT_MEM_REG_TABLE, NBPtr, &NBPtr->MemPtr->StdHeader);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function defines bit fields that only for DDR3
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNInitNBRegTableD3KV (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  TSEFO *NBRegTable;

  NBRegTable = NBPtr->NBRegTable;

  _BF_DEF (NBRegTable, RegDramCtl, BFAddrCmdTriEn, 17, 17);

  _BF_DEF (NBRegTable, RegDramMRS, BFPchgPDModeSel,  23,  23);

  _BF_DEF (NBRegTable, RegDramTmgHi, BFTref, 17, 16);

  REG_DEF (NBRegTable, RegDdr3DramTmg0, _FN (2, 0x200));
  _BF_DEF (NBRegTable, RegDdr3DramTmg0, BFTras, 29, 24);
  _BF_DEF (NBRegTable, RegDdr3DramTmg0, BFTrp, 20, 16);
  _BF_DEF (NBRegTable, RegDdr3DramTmg0, BFTrcd, 12, 8);
  _BF_DEF (NBRegTable, RegDdr3DramTmg0, BFTcl, 4, 0);

  REG_DEF (NBRegTable, RegDdr3DramTmg1, _FN (2, 0x204));
  _BF_DEF (NBRegTable, RegDdr3DramTmg1, BFTrtp, 27, 24);
  _BF_DEF (NBRegTable, RegDdr3DramTmg1, BFFourActWindow, 21, 16);
  _BF_DEF (NBRegTable, RegDdr3DramTmg1, BFTrrd, 11, 8);
  _BF_DEF (NBRegTable, RegDdr3DramTmg1, BFTrc, 5, 0);

  REG_DEF (NBRegTable, RegDdr3DramTmg2, _FN (2, 0x208));
  _BF_DEF (NBRegTable, RegDdr3DramTmg2, BFTrfc3, 26, 24);
  _BF_DEF (NBRegTable, RegDdr3DramTmg2, BFTrfc2, 18, 16);
  _BF_DEF (NBRegTable, RegDdr3DramTmg2, BFTrfc1, 10, 8);
  _BF_DEF (NBRegTable, RegDdr3DramTmg2, BFTrfc0, 2, 0);

  REG_DEF (NBRegTable, RegDdr3DramTmg3, _FN (2, 0x20C));
  _BF_DEF (NBRegTable, RegDdr3DramTmg3, BFWrDqDqsEarly, 17, 16);
  _BF_DEF (NBRegTable, RegDdr3DramTmg3, BFTwtr, 11, 8);
  _BF_DEF (NBRegTable, RegDdr3DramTmg3, BFTcwl, 4, 0);

  REG_DEF (NBRegTable, RegDdr3DramTmg4, _FN (2, 0x214));
  _BF_DEF (NBRegTable, RegDdr3DramTmg4, BFTwrwrSdSc, 19, 16);
  _BF_DEF (NBRegTable, RegDdr3DramTmg4, BFTwrwrSdDc, 11, 8);
  _BF_DEF (NBRegTable, RegDdr3DramTmg4, BFTwrwrDd, 3, 0);

  REG_DEF (NBRegTable, RegDdr3DramTmg5, _FN (2, 0x218));
  _BF_DEF (NBRegTable, RegDdr3DramTmg5, BFTrdrdBan, 31, 30);
  _BF_DEF (NBRegTable, RegDdr3DramTmg5, BFTrdrdSdSc, 27, 24);
  _BF_DEF (NBRegTable, RegDdr3DramTmg5, BFTrdrdSdDc, 19, 16);
  _BF_DEF (NBRegTable, RegDdr3DramTmg5, BFTwrrd, 11, 8);
  _BF_DEF (NBRegTable, RegDdr3DramTmg5, BFTrdrdDd, 3, 0);

  REG_DEF (NBRegTable, RegDdr3DramTmg6, _FN (2, 0x21C));
  _BF_DEF (NBRegTable, RegDdr3DramTmg6, BFTrwtWB, 20, 16);
  _BF_DEF (NBRegTable, RegDdr3DramTmg6, BFTrwtTO, 12, 8);

  REG_DEF (NBRegTable, RegDdr3DramTmg7, _FN (2, 0x220));
  _BF_DEF (NBRegTable, RegDdr3DramTmg7, BFTmod, 12, 8);
  _BF_DEF (NBRegTable, RegDdr3DramTmg7, BFTmrd, 3, 0);

  REG_DEF (NBRegTable, RegDdr3DramTmg8, _FN (2, 0x224));
  _BF_DEF (NBRegTable, RegDdr3DramTmg8, BFTzqcs, 10, 8);
  _BF_DEF (NBRegTable, RegDdr3DramTmg8, BFTzqoper, 3, 0);

  REG_DEF (NBRegTable, RegDdr3DramTmg9, _FN (2, 0x228));
  _BF_DEF (NBRegTable, RegDdr3DramTmg9, BFTstag3, 31, 24);
  _BF_DEF (NBRegTable, RegDdr3DramTmg9, BFTstag2, 23, 16);
  _BF_DEF (NBRegTable, RegDdr3DramTmg9, BFTstag1, 15, 8);
  _BF_DEF (NBRegTable, RegDdr3DramTmg9, BFTstag0, 7, 0);

  REG_DEF (NBRegTable, RegDdr3DramTmg10, _FN (2, 0x22C));
  _BF_DEF (NBRegTable, RegDdr3DramTmg10, BFTwrDDR3, 4, 0);

  REG_DEF (NBRegTable, RegPhyRODTCSLow , _FN (2, 0x230));
  REG_DEF (NBRegTable, RegPhyRODTCSHigh, _FN (2, 0x234));
  REG_DEF (NBRegTable, RegPhyWODTCSLow , _FN (2, 0x238));
  REG_DEF (NBRegTable, RegPhyWODTCSHigh, _FN (2, 0x23C));

  REG_DEF (NBRegTable, RegDdr3DramOdtCtl, _FN (2, 0x240));
  _BF_DEF (NBRegTable, RegDdr3DramOdtCtl, BFWrOdtOnDuration, 14, 12);
  _BF_DEF (NBRegTable, RegDdr3DramOdtCtl, BFWrOdtTrnOnDly, 10, 8);
  _BF_DEF (NBRegTable, RegDdr3DramOdtCtl, BFRdOdtOnDuration, 7, 4);
  _BF_DEF (NBRegTable, RegDdr3DramOdtCtl, BFRdOdtTrnOnDly, 3, 0);

  REG_DEF (NBRegTable, RegDdr3DramPwrMng0, _FN (2, 0x248));
  _BF_DEF (NBRegTable, RegDdr3DramPwrMng0, BFAggrPDDelay, 29, 24);
  _BF_DEF (NBRegTable, RegDdr3DramPwrMng0, BFPchgPDEnDelay, 21, 16);
  _BF_DEF (NBRegTable, RegDdr3DramPwrMng0, BFTxpdll, 12, 8);
  _BF_DEF (NBRegTable, RegDdr3DramPwrMng0, BFTxp, 3, 0);

  REG_DEF (NBRegTable, RegDdr3DramPwrMng1, _FN (2, 0x24C));
  _BF_DEF (NBRegTable, RegDdr3DramPwrMng1, BFTcksrx, 29, 24);
  _BF_DEF (NBRegTable, RegDdr3DramPwrMng1, BFTcksre, 21, 16);
  _BF_DEF (NBRegTable, RegDdr3DramPwrMng1, BFTckesr, 13, 8);
  _BF_DEF (NBRegTable, RegDdr3DramPwrMng1, BFTpd, 3, 0);

  REG_DEF (NBRegTable, RegMemPsCtlSts, _FN (2, 0x2E0));
  _BF_DEF (NBRegTable, RegMemPsCtlSts, BFFastMstateDis, 30, 30);
  _BF_DEF (NBRegTable, RegMemPsCtlSts, BFM1MemClkFreq, 28, 24);
  _BF_DEF (NBRegTable, RegMemPsCtlSts, BFMxMrsEn, 22, 20);

  REG_DEF (NBRegTable, RegMrsBuf0, _FN (2, 0x2E8));
  _BF_DEF (NBRegTable, RegMrsBuf0, BFMxMr0, 15, 0);
  _BF_DEF (NBRegTable, RegMrsBuf0, BFMxMr1, 31, 16);

  REG_DEF (NBRegTable, RegMrsBuf1, _FN (2, 0x2EC));
  _BF_DEF (NBRegTable, RegMrsBuf1, BFMxMr2, 15, 0);
  _BF_DEF (NBRegTable, RegDramCtl, BFChannelEn, 1, 1);

  REG_DEF (NBRegTable, RegDramCtlMisc3, _FN (2, 0x2F0));
  _BF_DEF (NBRegTable, RegDramCtlMisc3, BFEffArbDis, 0, 0);
  // ---------------------------------------------------------------------------
  // PHY
  // ---------------------------------------------------------------------------

  // ---------------------------------------------------------------------------
  // PMU SRAM FIELDS
  // ---------------------------------------------------------------------------

  PMU_SRAM (NBRegTable, PmuFirmwareRev    , 0x02, 15, 0);
  PMU_SRAM (NBRegTable, PmuCpuId          , 0x04, 15, 0);
  PMU_SRAM (NBRegTable, PmuCpuIdHi        , 0x06, 15, 0);

  PMU_SRAM (NBRegTable, PmuDramType       , 0x08, 7, 0);
  PMU_SRAM (NBRegTable, PmuModuleType     , 0x09, 7, 0);
  PMU_SRAM (NBRegTable, PmuRawCard0       , 0x0A, 7, 0);
  PMU_SRAM (NBRegTable, PmuRawCard1       , 0x0B, 7, 0);
  PMU_SRAM (NBRegTable, PmuChipSelect     , 0x0E, 7, 0);
  PMU_SRAM (NBRegTable, PmuAddrMirror     , 0x0F, 7, 0);

  PMU_SRAM (NBRegTable, PmuDimm0Cols      , 0x10, 7, 0);
  PMU_SRAM (NBRegTable, PmuDimm0Banks     , 0x11, 7, 0);
  PMU_SRAM (NBRegTable, PmuDimm0Rows      , 0x12, 7, 0);
  PMU_SRAM (NBRegTable, PmuDimm1Cols      , 0x13, 7, 0);
  PMU_SRAM (NBRegTable, PmuDimm1Banks     , 0x14, 7, 0);
  PMU_SRAM (NBRegTable, PmuDimm1Rows      , 0x15, 7, 0);

  PMU_SRAM (NBRegTable, PmuTestFail       , 0x1C, 7, 0);
  PMU_SRAM (NBRegTable, PmuPerRankTiming  , 0x1D, 7, 0);
  PMU_SRAM (NBRegTable, PmuCurrentTemp    , 0x1E, 15, 0);

  PMU_SRAM (NBRegTable, PmuSequenceCtl    , 0x20, 15, 0);

  PMU_SRAM (NBRegTable, PmuCkeSetup       , 0x23, 7, 0);
  PMU_SRAM (NBRegTable, PmuCsOdtSetup     , 0x24, 7, 0);
  PMU_SRAM (NBRegTable, PmuAddrCmdSetup   , 0x25, 7, 0);
  PMU_SRAM (NBRegTable, PmuSlowAccessMode , 0x26, 7, 0);
  PMU_SRAM (NBRegTable, PmutRP            , 0x27, 7, 0);
  PMU_SRAM (NBRegTable, PmutMRD           , 0x28, 7, 0);
  PMU_SRAM (NBRegTable, PmutRFC           , 0x29, 7, 0);
  PMU_SRAM (NBRegTable, PmuMR0            , 0x2A, 15, 0);
  PMU_SRAM (NBRegTable, PmuMR1            , 0x2C, 15, 0);
  PMU_SRAM (NBRegTable, PmuMR2            , 0x2E, 15, 0);
  PMU_SRAM (NBRegTable, PmuCD_R_W         , 0x30, 7, 0);
  PMU_SRAM (NBRegTable, PmuCD_R_R         , 0x31, 7, 0);
  PMU_SRAM (NBRegTable, PmuCD_W_W         , 0x32, 7, 0);
  PMU_SRAM (NBRegTable, PmuCD_W_R         , 0x33, 7, 0);
  PMU_SRAM (NBRegTable, PmuCD_R_R_SD      , 0x34, 7, 0);
  PMU_SRAM (NBRegTable, PmuCD_W_W_SD      , 0x35, 7, 0);
  PMU_SRAM (NBRegTable, PmuTrdrdban_Phy   , 0x36, 7, 0);

  LINK_BF (NBRegTable, PmuCpuId, PmuCpuIdHi);
}

