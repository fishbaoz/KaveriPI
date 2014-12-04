/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH DXE lib
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: FCH
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
****************************************************************************
*/
#include <Library/FchDxeLib.h>
#define FILECODE UEFI_DXE_FCHDXELIB_FCHDXELIB_FILECODE
// Global Pointer
EFI_SYSTEM_TABLE                  *gST;
EFI_RUNTIME_SERVICES              *gRT;
EFI_BOOT_SERVICES                 *gBS;
EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *mPciRootBridgeIo = NULL;


/*----------------------------------------------------------------------------------------*/
/**
 * Init variable.- Called via drivers constructor
 *
 * @param[in]     ImageHandle Image handle
 * @param[in]     SystemTable Sytem info table
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
LibFchDxeInitialization (
  IN EFI_HANDLE        ImageHandle,
  IN       EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS  Status;

  if (mPciRootBridgeIo != NULL) {
    return EFI_ALREADY_STARTED;
  }

  gST = SystemTable;
  gBS = SystemTable->BootServices;
  gRT = SystemTable->RuntimeServices;
  ASSERT (gBS != NULL);
  ASSERT (gRT != NULL);

  Status = gBS->LocateProtocol (
                  &gEfiPciRootBridgeIoProtocolGuid,
                  NULL,
                  &mPciRootBridgeIo
                  );
  ASSERT_EFI_ERROR (Status);
  return Status;
}


//
//  Source address format (UINT32):
//     [15:0]  - Register number
//     [18:16] - Function number
//     [23:19] - Device number
//     [31:24] - Bus number
//
//  Target address format (UINT64):
//     [7:0]   - Register number
//     [15:8]  - Function number
//     [23:16] - Device number
//     [31:24] - Bus number
//     [63:32] - If non-zero, used as (extended) register number
//
typedef union {
  struct {
    UINT32 LoDword;
    UINT32 HiDword;
  } As32bit;
  UINT64 AsQword;
} QWORD;


UINT64
ConvertPciAddress (
  IN  UINT32 Address
  )
{
  QWORD EfiPciAddress = {0};

  if ((Address & 0xffff) > 0xff) {
    EfiPciAddress.As32bit.HiDword = (Address & 0xffff);
  } else {
    EfiPciAddress.As32bit.LoDword = (Address & 0xff);
  }
  EfiPciAddress.AsQword |= (((Address & 0x70000) >> 8) | ((Address & 0xf80000) >> 3) | ((Address & 0xff000000))) ;
  return EfiPciAddress.AsQword;
}


/*----------------------------------------------------------------------------------------*/
/**
 * I/O Read Access
 *
 * @param[in]       EfiWidth     Access width
 * @param[in]       Address      IO address
 * @param[out]      Value        Pointer to data buffer
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchIoRead (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT16                                   Address,
     OUT   VOID                                     *Value
  )
{
  mPciRootBridgeIo->Io.Read (
                         mPciRootBridgeIo,
                         EfiWidth,
                         (UINT64) Address,
                         1,
                         Value
                         );
}


/*----------------------------------------------------------------------------------------*/
/**
 * I/O Write Access
 *
 * @param[in]       EfiWidth     Access width
 * @param[in]       Address      IO address
 * @param[in]       Value        Pointer to data buffer
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchIoWrite (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT16                                   Address,
     OUT   VOID                                     *Value
  )
{
  mPciRootBridgeIo->Io.Write (
                         mPciRootBridgeIo,
                         EfiWidth,
                         (UINT64) Address,
                         1,
                         Value
                         );
}


/*----------------------------------------------------------------------------------------*/
/**
 * Memory Read Access
 *
 * @param[in]       EfiWidth     Access width
 * @param[in]       Address      Memory address
 * @param[out]      Value        Pointer to data buffer
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchMemRead (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT32                                   Address,
     OUT   VOID                                     *Value
  )
{
  mPciRootBridgeIo->Mem.Read (
                          mPciRootBridgeIo,
                          EfiWidth,
                          (UINT64) Address,
                          1,
                          Value
                          );
}


/*----------------------------------------------------------------------------------------*/
/**
 * Memory Write Access
 *
 * @param[in]       EfiWidth     Access width
 * @param[in]       Address      Memory address
 * @param[out]      Value        Pointer to data buffer
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchMemWrite (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT32                                   Address,
  IN       VOID                                     *Value
  )
{
  mPciRootBridgeIo->Mem.Write (
                          mPciRootBridgeIo,
                          EfiWidth,
                          (UINT64) Address,
                          1,
                          Value
                          );
}


UINT32
ModifyDataByWidth (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN OUT   VOID                                     *Data,
  IN       VOID                                     *AndMask,
  IN       VOID                                     *OrMask
  )
{
  UINT32   TempValue;

  switch (EfiWidth) {
  case EfiPciWidthUint8:
    TempValue = ((UINT32)*(UINT8 *)Data & (UINT32)*(UINT8 *) AndMask) | (UINT32)*(UINT8 *) OrMask;
    break;
  case EfiPciWidthUint16:
    TempValue = ((UINT32)*(UINT16 *)Data & (UINT32)*(UINT16 *) AndMask) | (UINT32)*(UINT16 *) OrMask;
    break;
  case EfiPciWidthUint32:
    TempValue = (*(UINT32 *)Data & *(UINT32 *) AndMask) | *(UINT32 *) OrMask;
    break;
  default:
    ASSERT (FALSE);
    TempValue = 0;
  }
  return TempValue;
}


/*----------------------------------------------------------------------------------------*/
/**
 * I/O Read-Modify-Write Access
 *
 * @param[in]       EfiWidth     Access width
 * @param[in]       Address      IO address
 * @param[in]       *AndMask     Pointer to And Mask
 * @param[in]       *OrMask      Pointer to Or Mask
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchIoRw (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT16                                   Address,
  IN       VOID                                     *AndMask,
  IN       VOID                                     *OrMask
  )
{
  UINT32     FchDxeLibData32;

  mPciRootBridgeIo->Io.Read (
                         mPciRootBridgeIo,
                         EfiWidth,
                         (UINT64) Address,
                         1,
                         &FchDxeLibData32
                         );

  FchDxeLibData32 = ModifyDataByWidth (EfiWidth, &FchDxeLibData32, AndMask, OrMask);
  mPciRootBridgeIo->Io.Write (
                         mPciRootBridgeIo,
                         EfiWidth,
                         (UINT64) Address,
                         1,
                         &FchDxeLibData32
                         );
}


/*----------------------------------------------------------------------------------------*/
/**
 * Memory Read-Modify-Write Access
 *
 * @param[in]       EfiWidth     Access width
 * @param[in]       Address      IO address
 * @param[in]       *AndMask     Pointer to And Mask
 * @param[in]       *OrMask      Pointer to Or Mask
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchMemRw (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT32                                   Address,
  IN       VOID                                     *AndMask,
  IN       VOID                                     *OrMask
  )
{
  UINT32     FchDxeLibData32;

  mPciRootBridgeIo->Mem.Read (
                          mPciRootBridgeIo,
                          EfiWidth,
                          (UINT64) Address,
                          1,
                          &FchDxeLibData32
                          );

  FchDxeLibData32 = ModifyDataByWidth (EfiWidth, &FchDxeLibData32, AndMask, OrMask);
  mPciRootBridgeIo->Mem.Write (
                          mPciRootBridgeIo,
                          EfiWidth,
                          (UINT64) Address,
                          1,
                          &FchDxeLibData32
                          );
}


/*----------------------------------------------------------------------------------------*/
/**
 * Indirect I/O Read Access
 *
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchIndirectIoRead (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT16                                   IoBase,
  IN       UINT8                                    IndexAddress,
     OUT   VOID                                     *Value
  )
{
  UINT8    i;
  UINT8    ByteCount;

  ByteCount = (UINT8) EfiWidth + 1;
  for (i = 0; i < ByteCount; i++, IndexAddress++) {
    LibFchIoWrite (EfiPciWidthUint8, IoBase, &IndexAddress);
    LibFchIoRead (EfiPciWidthUint8, IoBase + 1, (UINT8 *)Value + i);
  }
}


/*----------------------------------------------------------------------------------------*/
/**
 * Indirect I/O Write Access
 *
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchIndirectIoWrite (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT16                                   IoBase,
  IN       UINT8                                    IndexAddress,
  IN       VOID                                     *Value
  )
{
  UINT8    i;
  UINT8    ByteCount;

  ByteCount = (UINT8) EfiWidth + 1;
  for (i = 0; i < ByteCount; i++, IndexAddress++) {
    LibFchIoWrite (EfiPciWidthUint8, IoBase, &IndexAddress);
    LibFchIoWrite (EfiPciWidthUint8, IoBase + 1, (UINT8 *)Value + i);
  }
}


VOID
LibFchGetAcpiMmioBase (
  OUT   UINT32         *AcpiMmioBase
  )
{
  UINT32    Value16;

  LibFchIndirectIoRead (EfiPciWidthUint16, FCH_IOMAP_REGCD6, FCH_PMIOA_REG24 + 2, &Value16);
  *AcpiMmioBase = Value16 << 16;
}


VOID
LibFchGetAcpiPmBase (
  OUT   UINT16         *AcpiPmBase
  )
{
  LibFchIndirectIoRead (EfiPciWidthUint16, FCH_IOMAP_REGCD6, FCH_PMIOA_REG60, AcpiPmBase);
}



/*----------------------------------------------------------------------------------------*/
/**
 * PCI Read Access
 *
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchPciRead (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT32                                   Address,
  IN       VOID                                     *Value
  )
{
  UINT64                 PciAddress;

  PciAddress = ConvertPciAddress (Address);
  mPciRootBridgeIo->Pci.Read (
                          mPciRootBridgeIo,
                          EfiWidth,
                          PciAddress,
                          1,
                          Value
                          );
}


/*----------------------------------------------------------------------------------------*/
/**
 * PCI Write Access
 *
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchPciWrite (
  IN       EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_WIDTH    EfiWidth,
  IN       UINT32                                   Address,
  IN       VOID                                     *Value
  )
{
  UINT64                 PciAddress;

  PciAddress = ConvertPciAddress (Address);
  mPciRootBridgeIo->Pci.Write (
                          mPciRootBridgeIo,
                          EfiWidth,
                          PciAddress,
                          1,
                          Value
                          );
}





