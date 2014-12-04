/* $NoKeywords:$ */
/**
 * @file
 *
 * MEMORY SMM Library
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AMD Memory Smm Library
 * @e sub-project:  Library
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
*****************************************************************************
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
#include "Tiano.h"
#include "Amd.h"
#include "DramMapKv.h"
#include "MemSmmLib.h"

#define FILECODE UEFI_SMM_MEMSMMLIB_KV_DRAMMAPKV_FILECODE

#include EFI_PROTOCOL_CONSUMER (SmmBase)

extern EFI_SMM_SYSTEM_TABLE      *mSmst;
extern  UINT64                   mMmioBase;

UINT64
BankSwapAddr (
  IN      BOOLEAN BankSwap,
  IN      UINT64  addr,
  IN      BOOLEAN DctSelBankSwap
  );

UINT64
BankSwizzleToAddr (
  IN      BOOLEAN swizzle,
  IN      UINT64  addr,
  IN      UINT8   bank,
  IN      UINT8   bankbit,
  IN      UINT8   offset
  );

UINT8
AddrToBankSwizzle (
  IN      BOOLEAN swizzle,
  IN      UINT64  addr,
  IN      UINT8   bankbit,
  IN      UINT8   offset
  );

UINT64
CSInterleavingBitSwap (
  IN      UINT64  addr,
  IN      UINT64  CSMask,
  IN      UINT8   hiBit,
  IN      UINT8   lowBit
  );

VOID
NormalizedToBankAddrMap (
  IN       UINT64  ChannelAddr,
  IN       UINT64  CSMask,
  IN       UINT8   DimmAddrMap,
  IN       BOOLEAN BankSwap,
  IN       BOOLEAN DctSelBankSwap,
  IN       BOOLEAN BankSwizzleMode,
  IN OUT   UINT8   *Bank,
  IN OUT   UINT32  *Row,
  IN OUT   UINT16  *Col
  );

UINT32 UnaryXOR (
  IN       UINT32  arg
  );

UINT32 Popcnt (
  IN       UINT32  arg
  );

/*----------------------------------------------------------------------------------------*/
/**
 *  Set_PCI
 *  Description
 *      Sets the value at given Bus, Device, Function and Offset
 *
 * @param[in]       Bus           Bus
 * @param[in]       Device        Device (mostly 24)
 * @param[in]       Function      Function
 * @param[in]       Foffset       Offset
 * @param[out]      Value         Value
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
Set_PCI (
  IN      UINT32   Bus,
  IN      UINT32   Device,
  IN      UINT32   Function,
  IN      UINT32   Foffset,
  IN      UINT64   Value
  )
{
  UINT32  AddressValue;

  AddressValue = MAKE_SBDFO (0, Bus, Device, Function, Foffset);
  AddressValue =  (UINT32) mMmioBase | AddressValue;
  LibAgesaSmmMemWrite (&mSmst->SmmIo, 2, AddressValue, &Value);
}

/*----------------------------------------------------------------------------------------*/
/**
 *  Get_PCI
 *  Description
 *      Reads the value at given Bus, Device, Function and Offset
 *
 * @param[in]       Bus           Bus
 * @param[in]       Device        Device (mostly 24)
 * @param[in]       Function      Function
 * @param[in]       Foffset       Offset
 *
 */
/*----------------------------------------------------------------------------------------*/
UINT64
Get_PCI (
  IN      UINT32   Bus,
  IN      UINT32   Device,
  IN      UINT32   Function,
  IN      UINT32   Foffset
  )
{
  UINT32  AddressValue;
  UINT64  Value;

  AddressValue = MAKE_SBDFO (0, Bus, Device, Function, Foffset);
  AddressValue =  (UINT32) mMmioBase | AddressValue;
  LibAgesaSmmMemRead (&mSmst->SmmIo, 2, AddressValue, &Value);
  return Value;
}

/*----------------------------------------------------------------------------------------*/
/**
 *  PlatformTranslateSysAddrToCS
 *  Description
 *      This function converst the given physical address into DIMM specific info
 *
 * @param[in]       SysAddr         SysAddr
 * @param[out]      CSFound         CSFound)
 * @param[out]      NodeID          NodeID
 * @param[out]      ChannelSelect   ChannelSelect
 * @param[out]      ChipSelect      ChipSelect
 * @param[out]      Bank            Bank
 * @param[out]      Row             Row
 * @param[out]      Col             Col
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
PlatformTranslateSysAddrToCS (
  IN       UINT64      SysAddr,
     OUT   BOOLEAN     *CSFound,
     OUT   UINT8       *NodeID,
     OUT   UINT8       *ChannelSelect,
     OUT   UINT8       *ChipSelect,
     OUT   UINT8       *Bank,
     OUT   UINT32      *Row,
     OUT   UINT16      *Col
  )
{
  UINT32    node;
  UINT32    range;
  UINT32    cs;
  UINT32    dctrange;
  UINT32    foffset;
  UINT64    temp;
  BOOLEAN   DramEn;
  UINT64    DramBase;
  UINT64    DramLimit;
  UINT64    DramBaseSysAddr;
  UINT64    DramLimitSysAddr;
  UINT64    DramHoleBase;
  UINT64    DramHoleOffset;
  UINT64    DctBaseAddr;
  UINT64    DctLimitAddr;
  BOOLEAN   DramMemHoistValid;
  BOOLEAN   DctSelBankSwap;
  BOOLEAN   DctAddrVal;
  BOOLEAN   LgcyMmioHoleEn;
  UINT32    DctSel;
  UINT64    ChannelOffset;
  UINT64    ChannelAddr;
  UINT64    CSBase;
  UINT64    CSMask;
  BOOLEAN   CSEn;
  UINT64    InputAddr;
  BOOLEAN   BankSwizzleMode;
  BOOLEAN   BankSwap;
  UINT8     DimmAddrMap;
  UINT64    DctSelIntLvAddr;
  UINT64    DctHighAddrOffset;
  BOOLEAN   BankSwapAddr8En;
  BOOLEAN   DctIntLvEn;
  BOOLEAN   DctOffsetEn;
  UINT8     Select;
  UINT32    temp2;

  *CSFound = FALSE;
  DctIntLvEn = FALSE;
  LgcyMmioHoleEn = FALSE;
  DctOffsetEn = FALSE;
  DctSel = 0;
  DctBaseAddr = 0;
  // Loop to determine the dram range
  for (node = 0; node < NUM_NODES; ++node) {
    for (range = 0; range < NUM_NODES; ++range) {
      // Extract variables from Dram Base
      foffset = 0x40 + (range << 3);
      temp = Get_PCI (BUS_0, (DEVICE_24 + node), FUNCTION_1, foffset);
      DramEn = (temp & 0x3) > 0 ? TRUE : FALSE;
      if (!DramEn) {
        continue;
      }
      DramBase = (((UINT64)Get_PCI (BUS_0, (DEVICE_24 + node), FUNCTION_1, foffset + 0x100) & 0xFF) << 40) | ((temp & 0xFFFF0000) << 8);
      // Extract variables from Dram Limit
      temp = Get_PCI (BUS_0, (DEVICE_24 + node), FUNCTION_1, foffset + 4);
      *NodeID = (UINT8)(temp & 0x7);
      DramLimit = (((UINT64)Get_PCI (BUS_0, (DEVICE_24 + node), FUNCTION_1, foffset + 0x104) & 0xFF) << 40) | ((temp << 8) | 0xFFFFFF);

      // For compatiblity with DRAM C6 Storage, extract variables from the per-node DRAM Base/Limit System Address
      temp = Get_PCI (BUS_0, (DEVICE_24 + *NodeID), FUNCTION_1, 0x120);
      DramBaseSysAddr = (temp & 0x1FFFFF) << 27;
      // Extract variables from Dram Limit
      temp = Get_PCI (BUS_0, (DEVICE_24 + *NodeID), FUNCTION_1, 0x124);
      DramLimitSysAddr = ((temp & 0x1FFFFF) << 27) | 0x7FFFFFF;

      if ((DramEn && DramBase <= SysAddr && SysAddr <= DramLimitSysAddr)) {
        // Extract variables from D18F1xF0
        temp = Get_PCI (BUS_0, (DEVICE_24 + *NodeID), FUNCTION_1, 0xF0);
        DramHoleOffset = (temp & 0xFF80) << 16;
        DramMemHoistValid = (temp & 0x2) > 0 ? TRUE : FALSE;
        DramHoleBase = ((temp >> 24) & 0xFF) << 24;
        // Address belongs to this node based on DramBase/Limit,
        // but is in the memory hole so it doesn't map to DRAM
        if (DramMemHoistValid && DramHoleBase <= SysAddr && SysAddr < 0x100000000) {
          return;
          }
        //
        // Extract Variables from D18F2x110
        //
        temp = Get_PCI (BUS_0, (DEVICE_24 + *NodeID), FUNCTION_2, 0x110);
        DctSelIntLvAddr = (temp >> 6) & 0x3;
        BankSwapAddr8En = (((temp >> 2) & 0x1) > 0) ? TRUE : FALSE;

        // Extract variables from D18F2x114
        temp = Get_PCI (BUS_0, (DEVICE_24 + *NodeID), FUNCTION_2, 0x114);
        DctSelIntLvAddr |= ((( temp >> 9) & 0x1) << 2);
        DctSelBankSwap = (BankSwapAddr8En && (DctSelIntLvAddr == 4)) ? TRUE : FALSE;

        // Single Channel
        DctAddrVal = FALSE;
        for (dctrange = 0; dctrange < 4; ++dctrange) {
          // Extract variables from F2x2[1,0][8,0]
          temp = Get_PCI (BUS_0, (DEVICE_24 + *NodeID), FUNCTION_1, 0x200 + 8 * dctrange);
          DctAddrVal = (temp & 0x1) > 0 ? TRUE : FALSE;
          if (!DctAddrVal) {
            continue;
          }
          DctBaseAddr = (((temp >> 11) & 0x1FFF) << 27);
          DctSel = (UINT32) ((temp >> 4) & 0x7);
          LgcyMmioHoleEn = ((temp >> 1) & 0x1) > 0 ? TRUE : FALSE;
          DctOffsetEn = ((temp >> 3) & 0x1) > 0 ? TRUE : FALSE;
          // Extract variables from F2x2[1,0][C,4]
          temp = Get_PCI (BUS_0, (DEVICE_24 + *NodeID), FUNCTION_1, 0x204 + 8 * dctrange);
          DctLimitAddr = (((temp >> 11) & 0x1FFF) << 27);
          DctIntLvEn   = (temp & 0xF) > 0 ? TRUE : FALSE;

          if (DctAddrVal && DctBaseAddr <= SysAddr && SysAddr <= DctLimitAddr) {
            break;
          }
        }

        *ChannelSelect = 0;

        // Determine Channel Info
        if (DctIntLvEn == 0) {
          *ChannelSelect = (UINT8) DctSel;
        } else if (Popcnt (DctIntLvEn) == 2) {
          Select = 0;
          switch (DctSelIntLvAddr) {
            case 0x0:
              Select = (UINT8) ((SysAddr >> 6) & 0x1);
              break;
            case 0x2:
              temp2 = UnaryXOR ((UINT32) (SysAddr >> 16) & 0x1F);
              Select = (UINT8) (((SysAddr >> 6) & 0x1) ^ temp2);
              break;
            case 0x3:
              temp2 = UnaryXOR ((UINT32) (SysAddr >> 16) & 0x1F);
              Select = (UINT8) (((SysAddr >> 9) & 0x1) ^ temp2);
              break;
            case 0x4:
              Select = (UINT8) ((SysAddr >> 8) & 0x1);
              break;
            case 0x5:
              Select = (UINT8) ((SysAddr >> 9) & 0x1);
              break;
            default:
              break;
          }
          // Determine the selected DCT
          if (Select > 0) {
            //Upper DCT selected
            if (DctIntLvEn & 0x8) {
              *ChannelSelect = 3;
            } else if (DctIntLvEn & 0x4) {
              *ChannelSelect = 2;
            } else {
              *ChannelSelect = 1;
            }
          } else {
            //Lower DCT selected
            if (DctIntLvEn & 0x1) {
              *ChannelSelect = 0;
            } else if (DctIntLvEn & 0x2) {
              *ChannelSelect = 1;
            } else {
              *ChannelSelect = 2;
            }
          }
        } else if (Popcnt (DctIntLvEn) == 4) {
          Select = 0;
          switch (DctSelIntLvAddr) {
            case 0x3:
              temp2 = UnaryXOR ((UINT32) (SysAddr >> 16) & 0x1F);
              temp2 |= (temp2 << 1);
              Select =  (UINT8) (((SysAddr >> 9) & 0x3) ^ temp2);
              break;
            case 0x4:
              Select = (UINT8) ((SysAddr >> 8) & 0x3);
              break;
            case 0x5:
              Select = (UINT8) ((SysAddr >> 9) & 0x3);
              break;
            default:
              break;
          }
          *ChannelSelect = Select;
        } else {
          //assert(0);
        }

        ChannelOffset = 0;
        if (DramMemHoistValid && (SysAddr >= 0x100000000)) {
          ChannelOffset = DramHoleOffset;
        } else {
            ChannelOffset = DramBase;
        }
        if (LgcyMmioHoleEn && (SysAddr >= ((UINT64)1 << 32))) {
          ChannelOffset = DramHoleOffset;
        } else {
          ChannelOffset = DctBaseAddr;
        }
        // Remove hoisting offset and normalize to DRAM bus addresses
        ChannelAddr = SysAddr - ChannelOffset;

        //Remove channel interleave bit
        if (Popcnt (DctIntLvEn) == 2) {
          switch (DctSelIntLvAddr) {
            case 0x0:
              ChannelAddr = ((ChannelAddr >> 7) << 6) | (ChannelAddr & 0x3F);
              break;
            case 0x2:
              ChannelAddr = ((ChannelAddr >> 7) << 6) | (ChannelAddr & 0x3F);
              break;
            case 0x3:
              ChannelAddr = ((ChannelAddr >> 10) << 9) | (ChannelAddr & 0x1FF);
              break;
            case 0x4:
              ChannelAddr = ((ChannelAddr >> 9) << 8) | (ChannelAddr & 0xFF);
              break;
            case 0x5:
              ChannelAddr = ((ChannelAddr >> 10) << 9) | (ChannelAddr & 0x1FF);
              break;
            default:
              break;
          }
        } else if (Popcnt (DctIntLvEn) == 4) {
          switch (DctSelIntLvAddr) {
            case 0x3:
              ChannelAddr = ((ChannelAddr >> 11) << 9) | (ChannelAddr & 0x1FF);
              break;
            case 0x4:
              ChannelAddr = ((ChannelAddr >> 10) << 8) | (ChannelAddr & 0xFF);
              break;
            case 0x5:
              ChannelAddr = ((ChannelAddr >> 11) << 9) | (ChannelAddr & 0x1FF);
              break;
            default:
              break;
          }
        }
        if (DctOffsetEn) {
          // Extract variables from F2x2[4C:40]
          temp = Get_PCI (BUS_0, DEVICE_24 + *NodeID, FUNCTION_1, 0x240 + 4 * (*ChannelSelect));
          DctHighAddrOffset = ((temp >> 11) & 0xFFF) << 27;
          ChannelAddr += DctHighAddrOffset;
        }
        // End Channel Interleave bit
        // Set DctCfgSel = ChannelSelect
        Set_PCI (BUS_0, (DEVICE_24 + *NodeID), FUNCTION_1, 0x10C, *ChannelSelect);
        // Determine the Chip Select
        for (cs = 0; cs < NUM_CS_PER_CHANNEL; ++cs) {
          // Obtain the CS Base from D18F2x[1,0][4C:40]
          foffset = 0x40 + (cs << 2);
          temp = Get_PCI (BUS_0, (DEVICE_24 + *NodeID), FUNCTION_2, foffset);
          CSEn = (temp & 0x1) > 0 ? TRUE : FALSE;
          CSBase = (((temp >> 19) & 0xFFF) << 27) | (((temp >> 5) & 0x3FF) << 11);
          // Obtain the CS Mask from D18F2x[64:60]
          foffset = 0x60 + ((cs >> 1) << 2);
          temp = Get_PCI (BUS_0, (DEVICE_24 + *NodeID), FUNCTION_2, foffset);
          CSMask = (((temp >> 19) & 0xFFF) << 27) | (((temp >> 5) & 0x3FF) << 11);
          // Adjust the Channel Addr for easy comparison
          InputAddr = (((ChannelAddr >> 27) & 0xFFF) << 27) | (((ChannelAddr >> 11) & 0x3FF) << 11);

          if (CSEn && (InputAddr & ~CSMask) == (CSBase & ~CSMask)) {
            *CSFound = TRUE;
            *ChipSelect = (UINT8)cs;
            // Extract variables from D18F2x80
            DimmAddrMap = (UINT8) ((((UINT64)Get_PCI (BUS_0, (DEVICE_24 + *NodeID), FUNCTION_2, 0x80)) >> ((*ChipSelect >> 1) * 4)) & 0xF);
            // Extract variables from the CS Mask
            foffset = 0x60 + ((*ChipSelect >> 1) << 2);

            // Extract variables from D18F2x94
            BankSwizzleMode = ((Get_PCI (BUS_0, DEVICE_24 + *NodeID, FUNCTION_2, 0x94) >> 22) & 0x1) > 0 ? TRUE : FALSE;
            // Extract variables from D18F2xA8
            BankSwap = ((Get_PCI (BUS_0, DEVICE_24 + *NodeID, FUNCTION_2, 0xA8) >> 20) & 0x1) > 0 ? TRUE : FALSE;
            // Determine the Bank, Row, and Col
            NormalizedToBankAddrMap (ChannelAddr, CSMask, DimmAddrMap, BankSwap, DctSelBankSwap, BankSwizzleMode, Bank, Row, Col);
            // Sanity check outputs
            return;
          }
         }
       }
     }
   }

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 *  NormalizedToBankAddrMap
 *  Description
 *      This function converst the given physical address into DIMM specific info
 *
 * @param[in]       ChannelAddr         ChannelAddr
 * @param[in]       CSMask              CSMask)
 * @param[in]       DimmAddrMap         DimmAddrMap
 * @param[in]       BankSwap            BankSwap
 * @param[in]       DctSelBankSwap      DctSelBankSwap
 * @param[in]       BankSwizzleMode     BankSwizzleMode
 * @param[in, out]  *Bank               Bank
   @param[in, out]  *Row                Row
   @param[in, out]  *Col                Col
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
NormalizedToBankAddrMap (
  IN       UINT64  ChannelAddr,
  IN       UINT64  CSMask,
  IN       UINT8   DimmAddrMap,
  IN       BOOLEAN BankSwap,
  IN       BOOLEAN DctSelBankSwap,
  IN       BOOLEAN BankSwizzleMode,
  IN OUT   UINT8   *Bank,
  IN OUT   UINT32  *Row,
  IN OUT   UINT16  *Col
  )
{
  UINT64  Addr;

  Addr = 0;

  ChannelAddr = BankSwapAddr (BankSwap, ChannelAddr, DctSelBankSwap);

  // BankSwizzleMode is calculated on the normalized addr
  // Select Row/Col based on the CS interleave adjustment tables
  switch (DimmAddrMap) {
  case 0x1:
    *Bank = AddrToBankSwizzle (BankSwizzleMode, ChannelAddr, 13, 0);
    Addr = CSInterleavingBitSwap (ChannelAddr, CSMask, 28, !BankSwap ? 16 : (DctSelBankSwap ? 11 : 12));
    *Row = (UINT32)(((Addr >> 18) & 0x3FF) | (((Addr >> 16) & 0x3) << 10));
    *Col = (UINT16)((Addr >> 3) & 0x3FF);
    break;
  case 0x2:
    *Bank = AddrToBankSwizzle (BankSwizzleMode, ChannelAddr, 13, 0);
    Addr = CSInterleavingBitSwap (ChannelAddr, CSMask, 29, !BankSwap ? 16 : (DctSelBankSwap ? 11 : 12));
    *Row = (UINT32) (((Addr >> 18) & 0x7FF) | (((Addr >> 16) & 0x3) << 11));
    *Col = (UINT8)((Addr >> 3) & 0x3FF);
    break;
  case 0x5:
    *Bank = AddrToBankSwizzle (BankSwizzleMode, ChannelAddr, 13, 0);
    Addr = CSInterleavingBitSwap (ChannelAddr, CSMask, 30, !BankSwap ? 16 : (DctSelBankSwap ? 11 : 12));
    *Row = (UINT32) (((Addr >> 18) & 0xFFF) | (((Addr >> 16) & 0x3) << 12));
    *Col = (UINT8) ((Addr >> 3) & 0x3FF);
    break;
  case 0x7:
    *Bank = AddrToBankSwizzle (BankSwizzleMode, ChannelAddr, 13, 0);
    Addr = CSInterleavingBitSwap (ChannelAddr, CSMask, 31, !BankSwap ? 16 : (DctSelBankSwap ? 11 : 12));
    *Row = (UINT32) (((Addr >> 18) & 0x1FFF) | (((Addr >> 16) & 0x3) << 13));
    *Col = (UINT8) ((Addr >> 3) & 0x3FF);
    break;
  case 0xA:
    *Bank = AddrToBankSwizzle (BankSwizzleMode, ChannelAddr, 13, 0);
    Addr = CSInterleavingBitSwap (ChannelAddr, CSMask, 32, !BankSwap ? 16 : (DctSelBankSwap ? 11 : 12));
    *Row = (UINT32) (((Addr >> 18) & 0x3FFF) | (((Addr >> 16) & 0x3) << 14));
    *Col = (UINT8)((Addr >> 3) & 0x3FF);
    break;
  case 0xB:
    *Bank = AddrToBankSwizzle (BankSwizzleMode, ChannelAddr, 14, 0);
    Addr = CSInterleavingBitSwap (ChannelAddr, CSMask, 33, !BankSwap ? 17 : (DctSelBankSwap ? 11 : 12));
    *Row = (UINT32) (((Addr >> 18) & 0x7FFF) | (((Addr >> 17) & 0x1) << 15));
    *Col = (UINT8) (((Addr >> 3) & 0x3FF) | (((Addr >> 13) & 0x1) << 11));
    break;
  default:
    break;
    }
}

/*----------------------------------------------------------------------------------------*/
/**
 *  CSInterleavingBitSwap
 *  Description
 *      Function returns a normalized address after adjusting for possible bit swapping
 *      by CS Interleaving.
 *
 * @param[in]       addr                addr
 * @param[in]       CSMask              CSMask)
 * @param[in]       hiBit               hiBit
 * @param[in]       lowBit              lowBit
 *
 */
/*----------------------------------------------------------------------------------------*/

UINT64
CSInterleavingBitSwap
(
  IN       UINT64 addr,
  IN       UINT64 CSMask,
  IN       UINT8 hiBit,
  IN       UINT8 lowBit
  )
{
  UINT64    mask;
  // Bits are swapped if the lower CSMask bits are zeros.
  // hiBit, lowBit indicate the LSB at the start of the range of bits that
  // take part in the swap an depend on the DimmAddrMap.
  // Sanity check the CSMask
  mask = ( CSMask >> lowBit & 0x7) ^ 0x7;
  return ( mask ? (addr & ~(mask << hiBit) & ~(mask << lowBit)) |
    (((addr >> hiBit) & mask) << lowBit) |
    (((addr >> lowBit) & mask) << hiBit) : addr);
}

/*----------------------------------------------------------------------------------------*/
/**
 *  AddrToBankSwizzle
 *  Description
 *      Function returns a DRAM device bank address after adjusting for possible
 *      remapping by bank swizzling. See D18F2x94[BankSwizzleMode]
 *
 * @param[in]       swizzle     swizzle
 * @param[in]       addr        addr)
 * @param[in]       bankbit     bankbit
 * @param[in]       offset      offset
 *
 */
/*----------------------------------------------------------------------------------------*/
UINT8
AddrToBankSwizzle (
  IN      BOOLEAN swizzle,
  IN      UINT64  addr,
  IN      UINT8   bankbit,
  IN      UINT8   offset
  )
{
  // bankbit indicates the LSB at the start of the range of bits that
  // are mapped to the DRAM device bank address and depends on the DimmAddrMap.
  if (!swizzle) {
    return (UINT8) (((addr >> bankbit) & 0x7));
    } else {
      return (
        (UINT8) ((((addr >> bankbit) & 1) ^ ((addr >> (bankbit + 3 + offset)) & 1) ^
        ((addr >> (bankbit + 6 + offset)) & 1)) |
        ((((addr >> (bankbit + 1)) & 1) ^ ((addr >> (bankbit + 4 + offset)) & 1) ^
        ((addr >> (bankbit + 7 + offset)) & 1)) << 1) |
        ((((addr >> (bankbit + 2)) & 1) ^ ((addr >> (bankbit + 5 + offset)) & 1) ^
        ((addr >> (bankbit + 8 + offset)) & 1)) << 2)
        ));
    }
}

/*----------------------------------------------------------------------------------------*/
/**
 *  BankSwapAddr
 *  Description
 *      Function returns a normalized address after adjusting for possible bit swapping
 *      by CS Interleaving.
 *
 * @param[in]       BankSwap            BankSwap
 * @param[in]       addr                addr
 * @param[in]       DctSelBankSwap      DctSelBankSwap
 *
 */
/*----------------------------------------------------------------------------------------*/

UINT64
BankSwapAddr
(
  IN  BOOLEAN   BankSwap,
  IN  UINT64    addr,
  IN  BOOLEAN   DctSelBankSwap
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

/*----------------------------------------------------------------------------------------*/
/**
 *  BankSwizzleToAddr
 *  Description
 *      Function returns a normalized DRAM addresss after adjusting for possible
 *      remapping by bank swizzlling. See D18F2x[1,0]94[BankSwizzleMode]
 *
 * @param[in]       swizzle     swizzle
 * @param[in]       addr        addr
 * @param[in]       bank        bank
 * @param[in]       bankbit     bankbit
 * @param[in]       offset      offset
 *
 */
/*----------------------------------------------------------------------------------------*/
UINT64
BankSwizzleToAddr
(
  IN       BOOLEAN    swizzle,
  IN       UINT64     addr,
  IN       UINT8      bank,
  IN       UINT8      bankbit,
  IN       UINT8      offset
  )
{
  // Recreate the normalized address; Note: A == A ^ B ^ B
  if (!swizzle) {
    addr |= bank << bankbit;
    } else {
      addr |= (
        ((bank & 1) ^ ((addr >> (bankbit + 3 + offset)) & 1) ^ ((addr >> (bankbit + 6 + offset)) & 1)) |
        ((((bank >> 1) & 1) ^ ((addr >> (bankbit + 4 + offset)) & 1) ^ ((addr >> (bankbit + 7 + offset)) & 1)) << 1) |
        ((((bank >> 2) & 1) ^ ((addr >> (bankbit + 5 + offset)) & 1) ^ ((addr >> (bankbit + 8 + offset)) & 1)) << 2)
        ) << bankbit;
    }
  return addr;
}
/*----------------------------------------------------------------------------------------*/
/**
 *  UnaryXOR
 *  Description
 *      Function returns a parity bit based upon the number of bit set in the argument.
 *
 * @param[in]       arg     arg
 *
 */
/*----------------------------------------------------------------------------------------*/
UINT32 UnaryXOR (
  IN  UINT32  arg
  )
{
  UINTN  i;
  UINT32 temp;
  temp = 0;
  for (i = 0; i < 32; i++) {
    temp ^= (arg & 0x1);
    arg = arg >> 1;
  }
  return temp;
}
/*----------------------------------------------------------------------------------------*/
/**
 *  Popcnt
 *  Description
 *      Function returns a count based on the number of bits set in the argument
 *
 * @param[in]       arg     arg
 *
 */
/*----------------------------------------------------------------------------------------*/
// Function returns a count based on the number of bits set in the argument
UINT32 Popcnt (
  IN  UINT32  arg
  )
{
  UINTN i;
  UINT32 temp;
  temp = 0;
  for (i = 0; i < 32; i++) {
    temp += (arg & 0x1);
    arg = arg >> 1;
  }
  return temp;
}