/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM Library for IO Access
 *
 * Contains interface to the AMD CPM library
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  Library
 * @e \$Revision: 281158 $   @e \$Date: 2013-12-17 20:36:04 -0600 (Tue, 17 Dec 2013) $
 *
 */
/*****************************************************************************
 *
 * Copyright 2012 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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
 ******************************************************************************
 */

#include <AmdCpmBase.h>
#include <AmdCpmBaseIo.h>
#include CPM_PPI_DEFINITION (AmdCpmTablePpi)

/*---------------------------------------------------------------------------------------*/
/**
 * Read an 8-bit MMIO register
 *
 *
 * @param[in] Address       The MMIO register address
 *
 * @retval    Register value
 *
 */
UINT8
EFIAPI
CpmMmioRead8 (
  IN       UINTN   Address
  )
{
  UINT8   Value;

  Value = *(volatile UINT8*)Address;
  return Value;
}


/*---------------------------------------------------------------------------------------*/
/**
 * Write an 8-bit MMIO register
 *
 *
 * @param[in] Address       The MMIO register address
 * @param[in] Value         The value to write to the MMIO register
 *
 */
UINT8
EFIAPI
CpmMmioWrite8 (
  IN       UINTN   Address,
  IN       UINT8   Value
  )
{
  return *(volatile UINT8*)Address = Value;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Read a 16-bit MMIO register
 *
 *
 * @param[in] Address       The MMIO register address
 *
 * @retval    Register value
 *
 */
UINT16
EFIAPI
CpmMmioRead16 (
  IN       UINTN   Address
  )
{
  UINT16  Value;

  Value = *(volatile UINT16*)Address;
  return Value;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write a 16-bit MMIO register
 *
 *
 * @param[in] Address       The MMIO register to read
 * @param[in] Value         The value to write to the MMIO register
 *
 */
UINT16
EFIAPI
CpmMmioWrite16 (
  IN       UINTN   Address,
  IN       UINT16  Value
  )
{
  return *(volatile UINT16*)Address = Value;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Read a 32-bit MMIO register
 *
 *
 * @param[in] Address       The MMIO register address
 *
 * @retval    Register value
 *
 */
UINT32
EFIAPI
CpmMmioRead32 (
  IN       UINTN   Address
  )
{
  UINT32  Value;

  Value = *(volatile UINT32*)Address;
  return Value;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write a 32-bit MMIO register
 *
 *
 * @param[in] Address       The MMIO register address
 * @param[in] Value         The value to write to the MMIO register
 *
 */
UINT32
EFIAPI
CpmMmioWrite32 (
  IN       UINTN   Address,
  IN       UINT32  Value
  )
{
  return *(volatile UINT32*)Address = Value;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Read a 64-bit MMIO register
 *
 *
 * @param[in] Address       The MMIO register address
 *
 * @retval    Register value
 *
 */
UINT64
EFIAPI
CpmMmioRead64 (
  IN       UINTN   Address
  )
{
  UINT64  Value;

  Value = *(volatile UINT64*)Address;
  return Value;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write a 64-bit MMIO register
 *
 *
 * @param[in] Address       The MMIO register address
 * @param[in] Value         The value to write to the MMIO register
 *
 */
UINT64
EFIAPI
CpmMmioWrite64 (
  IN       UINTN   Address,
  IN       UINT64  Value
  )
{
  return *(volatile UINT64*)Address = Value;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Reads an 8-bit MMIO register, performs a bitwise inclusive OR, and writes the
 * result back to the 8-bit MMIO register.
 *
 * @param[in] Address       The MMIO register address
 * @param[in] OrData        The value to OR with the read value from the MMIO register.
 *
 */
UINT8
EFIAPI
CpmMmioOr8 (
  IN       UINTN   Address,
  IN       UINT8   OrData
  )
{
  return CpmMmioWrite8 (Address, CpmMmioRead8 (Address) | OrData);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Reads an 8-bit MMIO register, performs a bitwise AND, and writes the result back
 * to the 16-bit MMIO register.
 *
 * @param[in] Address       The MMIO register address
 * @param[in] AndData       The value to AND with the read value from the MMIO register.
 *
 */
UINT8
EFIAPI
CpmMmioAnd8 (
  IN       UINTN   Address,
  IN       UINT8   AndData
  )
{
  return CpmMmioWrite8 (Address, CpmMmioRead8 (Address) & AndData);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Reads a 16-bit MMIO register, performs a bitwise AND followed by a bitwise
 * inclusive OR, and writes the result back to the 16-bit MMIO register.
 *
 * @param[in] Address       The MMIO register address
 * @param[in] AndData       The value to AND with the read value from the MMIO register.
 * @param[in] OrData        The value to OR with the read value from the MMIO register.
 *
 */
UINT8
EFIAPI
CpmMmioAndThenOr8 (
  IN       UINTN   Address,
  IN       UINT8   AndData,
  IN       UINT8   OrData
  )
{
    return CpmMmioWrite8 (Address, (CpmMmioRead8 (Address) & AndData) | OrData);
}


/*---------------------------------------------------------------------------------------*/
/**
 * Reads a 16-bit MMIO register, performs a bitwise inclusive OR, and writes the
 * result back to the 16-bit MMIO register.
 *
 * @param[in] Address       The MMIO register address
 * @param[in] OrData        The value to OR with the read value from the MMIO register.
 *
 */
UINT16
EFIAPI
CpmMmioOr16 (
  IN       UINTN       Address,
  IN       UINT16      OrData
  )
{
  return CpmMmioWrite16 (Address, CpmMmioRead16 (Address) | OrData);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Reads a 16-bit MMIO register, performs a bitwise AND, and writes the result back
 * to the 16-bit MMIO register.
 *
 * @param[in] Address       The MMIO register address
 * @param[in] AndData       The value to AND with the read value from the MMIO register.
 *
 */
UINT16
EFIAPI
CpmMmioAnd16 (
  IN       UINTN   Address,
  IN       UINT16  AndData
  )
{
  return CpmMmioWrite16 (Address, CpmMmioRead16 (Address) & AndData);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Reads a 16-bit MMIO register, performs a bitwise AND followed by a bitwise
 * inclusive OR, and writes the result back to the 16-bit MMIO register.
 *
 * @param[in] Address       The MMIO register address
 * @param[in] AndData       The value to AND with the read value from the MMIO register.
 * @param[in] OrData        The value to OR with the read value from the MMIO register.
 *
 */
UINT16
EFIAPI
CpmMmioAndThenOr16 (
  IN       UINTN   Address,
  IN       UINT16  AndData,
  IN       UINT16  OrData
  )
{
  return CpmMmioWrite16 (Address, (CpmMmioRead16 (Address) & AndData) | OrData);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Reads a 32-bit MMIO register, performs a bitwise inclusive OR, and writes the
 * result back to the 32-bit MMIO register.
 *
 * @param[in] Address       The MMIO register address
 * @param[in] OrData        The value to OR with the read value from the MMIO register.
 *
 */
UINT32
EFIAPI
CpmMmioOr32 (
  IN       UINTN   Address,
  IN       UINT32  OrData
  )
{
  return CpmMmioWrite32 (Address, CpmMmioRead32 (Address) | OrData);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Reads a 32-bit MMIO register, performs a bitwise AND, and writes the result back
 * to the 32-bit MMIO register.
 *
 * @param[in] Address       The MMIO register address
 * @param[in] AndData       The value to AND with the read value from the MMIO register.
 *
 */
UINT32
EFIAPI
CpmMmioAnd32 (
  IN       UINTN   Address,
  IN       UINT32  AndData
  )
{
  return CpmMmioWrite32 (Address, CpmMmioRead32 (Address) & AndData);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Reads a 32-bit MMIO register, performs a bitwise AND followed by a bitwise
 * inclusive OR, and writes the result back to the 32-bit MMIO register.
 *
 * @param[in] Address       The MMIO register address
 * @param[in] AndData       The value to AND with the read value from the MMIO register.
 * @param[in] OrData        The value to OR with the read value from the MMIO register.
 *
 */
UINT32
EFIAPI
CpmMmioAndThenOr32 (
  IN       UINTN   Address,
  IN       UINT32  AndData,
  IN       UINT32  OrData
  )
{
  return CpmMmioWrite32 (Address, (CpmMmioRead32 (Address) & AndData) | OrData);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Reads a 64-bit MMIO register, performs a bitwise inclusive OR, and writes the
 * result back to the 64-bit MMIO register.
 *
 * @param[in] Address       The MMIO register address
 * @param[in] OrData        The value to OR with the read value from the MMIO register.
 *
 */
UINT64
EFIAPI
CpmMmioOr64 (
  IN       UINTN   Address,
  IN       UINT64  OrData
  )
{
  return CpmMmioWrite64 (Address, CpmMmioRead64 (Address) | OrData);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Reads a 64-bit MMIO register, performs a bitwise AND, and writes the result back
 * to the 64-bit MMIO register.
 *
 * @param[in] Address       The MMIO register address
 * @param[in] AndData        The value to AND with the read value from the MMIO register.
 *
 */
UINT64
EFIAPI
CpmMmioAnd64 (
  IN       UINTN   Address,
  IN       UINT64  AndData
  )
{
  return CpmMmioWrite64 (Address, CpmMmioRead64 (Address) & AndData);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Reads a 64-bit MMIO register, performs a bitwise AND followed by a bitwise
 * inclusive OR, and writes the result back to the 64-bit MMIO register.
 *
 * @param[in] Address       The MMIO register address
 * @param[in] AndData       The value to AND with the read value from the MMIO register.
 * @param[in] OrData        The value to OR with the read value from the MMIO register.
 *
 */
UINT64
EFIAPI
CpmMmioAndThenOr64 (
  IN       UINTN   Address,
  IN       UINT64  AndData,
  IN       UINT64   OrData
  )
{
  return CpmMmioWrite64 (Address, (CpmMmioRead64 (Address) & AndData) | OrData);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Output a post code
 *
 * @param[in] Postcode  The post code to write
 *
 */
VOID
EFIAPI
CpmPostCode (
  IN       UINT32  Postcode
  )
{
  CpmIoWrite32 (0x80, Postcode);
};

/*---------------------------------------------------------------------------------------*/
/**
 * Read an 8-bit PCI register
 *
 *
 * @param[in] This      The pointer of CPM Table Ppi or Protocol
 * @param[in] Bus       The bus number of PCI device
 * @param[in] Device    The device number of PCI device
 * @param[in] Function  The function number of PCI device
 * @param[in] Offset    The offset of PCI register
 *
 * @retval    PCI register value
 *
 */
UINT8
EFIAPI
CpmPciRead8 (
  IN       VOID    *This,
  IN       UINT8   Bus,
  IN       UINT8   Device,
  IN       UINT8   Function,
  IN       UINT16  Offset
  )
{
  AMD_CPM_TABLE_PPI   *AmdCpmTablePpi;
  UINT32              Address;
  AmdCpmTablePpi = This;
  Address = AmdCpmTablePpi->MainTablePtr->PcieMemIoBaseAddr + (Bus << 20) + (Device << 15) + (Function << 12) + Offset;
  return AmdCpmTablePpi->CommonFunction.MmioRead8 (Address);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Read a 16-bit PCI register
 *
 *
 * @param[in] This      The pointer of CPM Table Ppi or Protocol
 * @param[in] Bus       The bus number of PCI device
 * @param[in] Device    The device number of PCI device
 * @param[in] Function  The function number of PCI device
 * @param[in] Offset    The offset of PCI register
 *
 * @retval    PCI register value
 *
 */
UINT16
EFIAPI
CpmPciRead16 (
  IN       VOID    *This,
  IN       UINT8   Bus,
  IN       UINT8   Device,
  IN       UINT8   Function,
  IN       UINT16  Offset
  )
{
  AMD_CPM_TABLE_PPI   *AmdCpmTablePpi;
  UINT32              Address;
  AmdCpmTablePpi = This;
  Address = AmdCpmTablePpi->MainTablePtr->PcieMemIoBaseAddr + (Bus << 20) + (Device << 15) + (Function << 12) + Offset;
  return AmdCpmTablePpi->CommonFunction.MmioRead16 (Address);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Read a 32-bit PCI register
 *
 *
 * @param[in] This      The pointer of CPM Table Ppi or Protocol
 * @param[in] Bus       The bus number of PCI device
 * @param[in] Device    The device number of PCI device
 * @param[in] Function  The function number of PCI device
 * @param[in] Offset    The offset of PCI register
 *
 * @retval    PCI register value
 *
 */
UINT32
EFIAPI
CpmPciRead32 (
  IN      VOID    *This,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function,
  IN      UINT16  Offset
  )
{
  AMD_CPM_TABLE_PPI   *AmdCpmTablePpi;
  UINT32              Address;
  AmdCpmTablePpi = This;
  Address = AmdCpmTablePpi->MainTablePtr->PcieMemIoBaseAddr + (Bus << 20) + (Device << 15) + (Function << 12) + Offset;
  return AmdCpmTablePpi->CommonFunction.MmioRead32 (Address);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write an 8-bit PCI register
 *
 *
 * @param[in] This      The pointer of CPM Table Ppi or Protocol
 * @param[in] Bus       The bus number of PCI device
 * @param[in] Device    The device number of PCI device
 * @param[in] Function  The function number of PCI device
 * @param[in] Offset    The offset of PCI register
 * @param[in] Data      The value to write
 *
 */
VOID
EFIAPI
CpmPciWrite8 (
  IN       VOID    *This,
  IN       UINT8   Bus,
  IN       UINT8   Device,
  IN       UINT8   Function,
  IN       UINT16  Offset,
  IN       UINT8   Data
  )
{
  AMD_CPM_TABLE_PPI   *AmdCpmTablePpi;
  UINT32              Address;
  AmdCpmTablePpi = This;
  Address = AmdCpmTablePpi->MainTablePtr->PcieMemIoBaseAddr + (Bus << 20) + (Device << 15) + (Function << 12) + Offset;
  AmdCpmTablePpi->CommonFunction.MmioWrite8 (Address, Data);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write a 16-bit PCI register
 *
 *
 * @param[in] This      The pointer of CPM Table Ppi or Protocol
 * @param[in] Bus       The bus number of PCI device
 * @param[in] Device    The device number of PCI device
 * @param[in] Function  The function number of PCI device
 * @param[in] Offset    The offset of PCI register
 * @param[in] Data      The value to write
 *
 */
VOID
EFIAPI
CpmPciWrite16 (
  IN       VOID    *This,
  IN       UINT8   Bus,
  IN       UINT8   Device,
  IN       UINT8   Function,
  IN       UINT16  Offset,
  IN       UINT16  Data
  )
{
  AMD_CPM_TABLE_PPI   *AmdCpmTablePpi;
  UINT32              Address;
  AmdCpmTablePpi = This;
  Address = AmdCpmTablePpi->MainTablePtr->PcieMemIoBaseAddr + (Bus << 20) + (Device << 15) + (Function << 12) + Offset;
  AmdCpmTablePpi->CommonFunction.MmioWrite16 (Address, Data);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write a 32-bit PCI register
 *
 *
 * @param[in] This      The pointer of CPM Table Ppi or Protocol
 * @param[in] Bus       The bus number of PCI device
 * @param[in] Device    The device number of PCI device
 * @param[in] Function  The function number of PCI device
 * @param[in] Offset    The offset of PCI register
 * @param[in] Data      The value to write
 *
 */
VOID
EFIAPI
CpmPciWrite32 (
  IN       VOID    *This,
  IN       UINT8   Bus,
  IN       UINT8   Device,
  IN       UINT8   Function,
  IN       UINT16  Offset,
  IN       UINT32  Data
  )
{
  AMD_CPM_TABLE_PPI   *AmdCpmTablePpi;
  UINT32              Address;
  AmdCpmTablePpi = This;
  Address = AmdCpmTablePpi->MainTablePtr->PcieMemIoBaseAddr + (Bus << 20) + (Device << 15) + (Function << 12) + Offset;
  AmdCpmTablePpi->CommonFunction.MmioWrite32 (Address, Data);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Reads an 8-bit PCI register, performs a bitwise AND and writes the result back
 * to the 8-bit PCI register.
 *
 * @param[in] This      The pointer of CPM Table Ppi or Protocol
 * @param[in] Bus       The bus number of PCI device
 * @param[in] Device    The device number of PCI device
 * @param[in] Function  The function number of PCI device
 * @param[in] Offset    The offset of PCI register
 * @param[in] AndData   The value to AND with the read value from the PCI register.
 *
 */
VOID
EFIAPI
CpmPciAnd8 (
  IN       VOID    *This,
  IN       UINT8   Bus,
  IN       UINT8   Device,
  IN       UINT8   Function,
  IN       UINT16  Offset,
  IN       UINT8   AndData
  )
{
  AMD_CPM_TABLE_PPI   *AmdCpmTablePpi;
  UINT32              Address;
  AmdCpmTablePpi = This;
  Address = AmdCpmTablePpi->MainTablePtr->PcieMemIoBaseAddr + (Bus << 20) + (Device << 15) + (Function << 12) + Offset;
  AmdCpmTablePpi->CommonFunction.MmioAnd8 (Address, AndData);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Reads a 16-bit PCI register, performs a bitwise AND and writes the result back
 * to the 16-bit PCI register.
 *
 * @param[in] This      The pointer of CPM Table Ppi or Protocol
 * @param[in] Bus       The bus number of PCI device
 * @param[in] Device    The device number of PCI device
 * @param[in] Function  The function number of PCI device
 * @param[in] Offset    The offset of PCI register
 * @param[in] AndData   The value to AND with the read value from the PCI register.
 *
 */
VOID
EFIAPI
CpmPciAnd16 (
  IN       VOID    *This,
  IN       UINT8   Bus,
  IN       UINT8   Device,
  IN       UINT8   Function,
  IN       UINT16  Offset,
  IN       UINT16  AndData
  )
{
  AMD_CPM_TABLE_PPI   *AmdCpmTablePpi;
  UINT32              Address;
  AmdCpmTablePpi = This;
  Address = AmdCpmTablePpi->MainTablePtr->PcieMemIoBaseAddr + (Bus << 20) + (Device << 15) + (Function << 12) + Offset;
  AmdCpmTablePpi->CommonFunction.MmioAnd16 (Address, AndData);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Reads a 32-bit PCI register, performs a bitwise AND and writes the result back
 * to the 32-bit PCI register.
 *
 * @param[in] This      The pointer of CPM Table Ppi or Protocol
 * @param[in] Bus       The bus number of PCI device
 * @param[in] Device    The device number of PCI device
 * @param[in] Function  The function number of PCI device
 * @param[in] Offset    The offset of PCI register
 * @param[in] AndData   The value to AND with the read value from the PCI register.
 *
 */
VOID
EFIAPI
CpmPciAnd32 (
  IN       VOID    *This,
  IN       UINT8   Bus,
  IN       UINT8   Device,
  IN       UINT8   Function,
  IN       UINT16  Offset,
  IN       UINT32  AndData
  )
{
  AMD_CPM_TABLE_PPI   *AmdCpmTablePpi;
  UINT32              Address;
  AmdCpmTablePpi = This;
  Address = AmdCpmTablePpi->MainTablePtr->PcieMemIoBaseAddr + (Bus << 20) + (Device << 15) + (Function << 12) + Offset;
  AmdCpmTablePpi->CommonFunction.MmioAnd32 (Address, AndData);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Reads an 8-bit PCI register, performs a bitwise OR and writes the result back
 * to the 8-bit PCI register.
 *
 * @param[in] This      The pointer of CPM Table Ppi or Protocol
 * @param[in] Bus       The bus number of PCI device
 * @param[in] Device    The device number of PCI device
 * @param[in] Function  The function number of PCI device
 * @param[in] Offset    The offset of PCI register
 * @param[in] OrData    The value to OR with the read value from the PCI register.
 *
 */
VOID
EFIAPI
CpmPciOr8 (
  IN       VOID    *This,
  IN       UINT8   Bus,
  IN       UINT8   Device,
  IN       UINT8   Function,
  IN       UINT16  Offset,
  IN       UINT8   OrData
  )
{
  AMD_CPM_TABLE_PPI   *AmdCpmTablePpi;
  UINT32              Address;
  AmdCpmTablePpi = This;
  Address = AmdCpmTablePpi->MainTablePtr->PcieMemIoBaseAddr + (Bus << 20) + (Device << 15) + (Function << 12) + Offset;
  AmdCpmTablePpi->CommonFunction.MmioOr8 (Address, OrData);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Reads a 16-bit PCI register, performs a bitwise OR and writes the result back
 * to the 16-bit PCI register.
 *
 * @param[in] This      The pointer of CPM Table Ppi or Protocol
 * @param[in] Bus       The bus number of PCI device
 * @param[in] Device    The device number of PCI device
 * @param[in] Function  The function number of PCI device
 * @param[in] Offset    The offset of PCI register
 * @param[in] OrData    The value to OR with the read value from the PCI register.
 *
 */
VOID
EFIAPI
CpmPciOr16 (
  IN       VOID    *This,
  IN       UINT8   Bus,
  IN       UINT8   Device,
  IN       UINT8   Function,
  IN       UINT16  Offset,
  IN       UINT16  OrData
  )
{
  AMD_CPM_TABLE_PPI   *AmdCpmTablePpi;
  UINT32              Address;
  AmdCpmTablePpi = This;
  Address = AmdCpmTablePpi->MainTablePtr->PcieMemIoBaseAddr + (Bus << 20) + (Device << 15) + (Function << 12) + Offset;
  AmdCpmTablePpi->CommonFunction.MmioOr16 (Address, OrData);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Reads a 32-bit PCI register, performs a bitwise OR and writes the result back
 * to the 32-bit PCI register.
 *
 * @param[in] This      The pointer of CPM Table Ppi or Protocol
 * @param[in] Bus       The bus number of PCI device
 * @param[in] Device    The device number of PCI device
 * @param[in] Function  The function number of PCI device
 * @param[in] Offset    The offset of PCI register
 * @param[in] OrData    The value to OR with the read value from the PCI register.
 *
 */
VOID
EFIAPI
CpmPciOr32 (
  IN       VOID    *This,
  IN       UINT8   Bus,
  IN       UINT8   Device,
  IN       UINT8   Function,
  IN       UINT16  Offset,
  IN       UINT32  OrData
  )
{
  AMD_CPM_TABLE_PPI   *AmdCpmTablePpi;
  UINT32              Address;
  AmdCpmTablePpi = This;
  Address = AmdCpmTablePpi->MainTablePtr->PcieMemIoBaseAddr + (Bus << 20) + (Device << 15) + (Function << 12) + Offset;
  AmdCpmTablePpi->CommonFunction.MmioOr32 (Address, OrData);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Reads an 8-bit PCI register, performs a bitwise AND followed by a bitwise
 * inclusive OR, and writes the result back to the 8-bit PCI register.
 *
 * @param[in] This      The pointer of CPM Table Ppi or Protocol
 * @param[in] Bus       The bus number of PCI device
 * @param[in] Device    The device number of PCI device
 * @param[in] Function  The function number of PCI device
 * @param[in] Offset    The offset of PCI register
 * @param[in] AndData   The value to AND with the read value from the PCI register.
 * @param[in] OrData    The value to OR with the read value from the PCI register.
 *
 */
VOID
EFIAPI
CpmPciAndThenOr8 (
  IN       VOID    *This,
  IN       UINT8   Bus,
  IN       UINT8   Device,
  IN       UINT8   Function,
  IN       UINT16  Offset,
  IN       UINT8   AndData,
  IN       UINT8   OrData
  )
{
  AMD_CPM_TABLE_PPI   *AmdCpmTablePpi;
  UINT32              Address;
  AmdCpmTablePpi = This;
  Address = AmdCpmTablePpi->MainTablePtr->PcieMemIoBaseAddr + (Bus << 20) + (Device << 15) + (Function << 12) + Offset;
  AmdCpmTablePpi->CommonFunction.MmioAndThenOr8 (Address, AndData, OrData);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Reads a 16-bit PCI register, performs a bitwise AND followed by a bitwise
 * inclusive OR, and writes the result back to the 16-bit PCI register.
 *
 * @param[in] This      The pointer of CPM Table Ppi or Protocol
 * @param[in] Bus       The bus number of PCI device
 * @param[in] Device    The device number of PCI device
 * @param[in] Function  The function number of PCI device
 * @param[in] Offset    The offset of PCI register
 * @param[in] AndData   The value to AND with the read value from the PCI register.
 * @param[in] OrData    The value to OR with the read value from the PCI register.
 *
 */
VOID
EFIAPI
CpmPciAndThenOr16 (
  IN       VOID    *This,
  IN       UINT8   Bus,
  IN       UINT8   Device,
  IN       UINT8   Function,
  IN       UINT16  Offset,
  IN       UINT16  AndData,
  IN       UINT16  OrData
  )
{
  AMD_CPM_TABLE_PPI   *AmdCpmTablePpi;
  UINT32              Address;
  AmdCpmTablePpi = This;
  Address = AmdCpmTablePpi->MainTablePtr->PcieMemIoBaseAddr + (Bus << 20) + (Device << 15) + (Function << 12) + Offset;
  AmdCpmTablePpi->CommonFunction.MmioAndThenOr16 (Address, AndData, OrData);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Reads a 32-bit PCI register, performs a bitwise AND followed by a bitwise
 * inclusive OR, and writes the result back to the 32-bit PCI register.
 *
 * @param[in] This      The pointer of CPM Table Ppi or Protocol
 * @param[in] Bus       The bus number of PCI device
 * @param[in] Device    The device number of PCI device
 * @param[in] Function  The function number of PCI device
 * @param[in] Offset    The offset of PCI register
 * @param[in] AndData   The value to AND with the read value from the PCI register.
 * @param[in] OrData    The value to OR with the read value from the PCI register.
 *
 */
VOID
EFIAPI
CpmPciAndThenOr32 (
  IN       VOID    *This,
  IN       UINT8   Bus,
  IN       UINT8   Device,
  IN       UINT8   Function,
  IN       UINT16  Offset,
  IN       UINT32  AndData,
  IN       UINT32  OrData
  )
{
  AMD_CPM_TABLE_PPI   *AmdCpmTablePpi;
  UINT32              Address;
  AmdCpmTablePpi = This;
  Address = AmdCpmTablePpi->MainTablePtr->PcieMemIoBaseAddr + (Bus << 20) + (Device << 15) + (Function << 12) + Offset;
  AmdCpmTablePpi->CommonFunction.MmioAndThenOr32 (Address, AndData, OrData);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Check Whether there is a PCIE device under a bridge
 *
 * @param[in] This      The pointer of CPM Table Ppi or Protocol
 * @param[in] Device    The device number of PCIe bridge
 * @param[in] Function  The function number of PCIe bridge
 *
 * @retval    1         There is PCI device under the bridge
 * @retval    2         There is PCI device under the bridge and Clock power managment support is not enabled
 * @retval    3         There is PCI device under the bridge and no Clock power managment capability support
 * @retval    0         There is no PCI device under the bridge
 *
 */
UINT8
EFIAPI
CpmCheckPcieDevice (
  IN       VOID                        *This,
  IN       UINT8                       Device,
  IN       UINT8                       Function
  )
{
  AMD_CPM_TABLE_PPI   *AmdCpmTablePpi = This;
  UINT32              Address;
  UINT32              BridgeAddress;
  UINT32              Value;
  UINT8               Bus;
  UINT8               BusTemp;
  UINT8               Status;
  UINT32              Offset;
  UINT8               Value8;
  BridgeAddress = AmdCpmTablePpi->MainTablePtr->PcieMemIoBaseAddr + (Device << 15) + (Function << 12) + 0x19;
  Bus = AmdCpmTablePpi->CommonFunction.MmioRead8 (BridgeAddress);
  BusTemp = Bus;
  Status = 0;
  Address = 0;
  if (Bus) {
    if (Bus != 0xFF) {
      Address = AmdCpmTablePpi->MainTablePtr->PcieMemIoBaseAddr + (Bus << 20);
      Value = AmdCpmTablePpi->CommonFunction.MmioRead32 (Address);
    } else {
      Value = 0xFFFFFFFF;
    }
  } else {
    BusTemp = 0x1F;
    AmdCpmTablePpi->CommonFunction.MmioWrite8 (BridgeAddress, BusTemp);
    Address = AmdCpmTablePpi->MainTablePtr->PcieMemIoBaseAddr + (BusTemp << 20);
    Value = AmdCpmTablePpi->CommonFunction.MmioRead32 (Address);
  }

  if (Value != 0xFFFFFFFF && Value != 0) {
    Status = 3;
    Offset = AmdCpmTablePpi->CommonFunction.MmioRead8 (Address + 0x34);
    while (Offset != 0 && Offset != 0xFF) {
      Value8 = AmdCpmTablePpi->CommonFunction.MmioRead8 (Address + Offset);
      if (Value8 != 0x10) {
        Offset = AmdCpmTablePpi->CommonFunction.MmioRead8 (Address + Offset + 0x01);
      } else {
        Value = AmdCpmTablePpi->CommonFunction.MmioRead32 (Address + Offset + 0x0C);
        if (Value & BIT18) {
          Value = AmdCpmTablePpi->CommonFunction.MmioRead32 (Address + Offset + 0x10);
          if (Value & BIT8) {
            Status = 1;
          } else {
            Status = 2;
          }
        }
        break;
      }
    }
  }
  if (Bus != 0xFF) {
    AmdCpmTablePpi->CommonFunction.MmioWrite8 (BridgeAddress, Bus);
  }
  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Detect the device
 *
 * @param[in] This      The pointer of CPM Table Ppi or Protocol
 * @param[in] DeviceId  The device Id
 * @param[out] Status   The status of detection bits
 *
 * @retval    TRUE      Device is found
 * @retval    FALSE     Device is not found
 *
 */
BOOLEAN
EFIAPI
CpmDetectDevice (
  IN       VOID                        *This,
  IN       UINT8                       DeviceId,
     OUT   UINT8                       *Status
  )
{
  AMD_CPM_GPIO_DEVICE_DETECTION_TABLE *DetectionTablePtr;
  AMD_CPM_TABLE_PPI       *CpmTablePpiPtr;
  UINT8                   Index;
  UINT8                   Detected;
  UINT8                   GpioDeviceId;
  UINT8                   GpioStatus;
  BOOLEAN                 Pin1Detected;
  BOOLEAN                 Pin2Detected;
  BOOLEAN                 Pin3Detected;

  CpmTablePpiPtr = This;
  Detected = 0;
  GpioStatus = 0;

  DetectionTablePtr   = CpmTablePpiPtr->CommonFunction.GetTablePtr (CpmTablePpiPtr, CPM_SIGNATURE_GPIO_DEVICE_DETECTION);

  if (DetectionTablePtr) {
    for (Index = 0; Index < AMD_GPIO_DEVICE_DETECT_SIZE; Index ++) {
      GpioDeviceId = DetectionTablePtr->DeviceDetectionList[Index].DeviceId;
      if (GpioDeviceId == 0xFF || GpioDeviceId == 0x00) {
        break;
      }
      if (GpioDeviceId == DeviceId ) {
        if (DetectionTablePtr->DeviceDetectionList[Index].Type == CPM_TYPE_GPIO_1) {
          Pin1Detected = (CpmTablePpiPtr->CommonFunction.GetGpio (CpmTablePpiPtr, DetectionTablePtr->DeviceDetectionList[Index].PinNum1) ==
                          DetectionTablePtr->DeviceDetectionList[Index].Value1);
          Detected = Pin1Detected;
          GpioStatus = Pin1Detected ? 1 : 0;
          break;
        } else if (DetectionTablePtr->DeviceDetectionList[Index].Type == CPM_TYPE_GPIO_2) {
          Pin1Detected = (CpmTablePpiPtr->CommonFunction.GetGpio (CpmTablePpiPtr, DetectionTablePtr->DeviceDetectionList[Index].PinNum1) ==
                          DetectionTablePtr->DeviceDetectionList[Index].Value1);
          Pin2Detected = (CpmTablePpiPtr->CommonFunction.GetGpio (CpmTablePpiPtr, DetectionTablePtr->DeviceDetectionList[Index].PinNum2) ==
                          DetectionTablePtr->DeviceDetectionList[Index].Value2);
          Detected = Pin1Detected && Pin2Detected;
          GpioStatus = (Pin1Detected ? 1 : 0) | (Pin2Detected ? 2 : 0);
          break;
        } else if (DetectionTablePtr->DeviceDetectionList[Index].Type == CPM_TYPE_GPIO_3) {
          Pin1Detected = (CpmTablePpiPtr->CommonFunction.GetGpio (CpmTablePpiPtr, DetectionTablePtr->DeviceDetectionList[Index].PinNum1) ==
                          DetectionTablePtr->DeviceDetectionList[Index].Value1);
          Pin2Detected = (CpmTablePpiPtr->CommonFunction.GetGpio (CpmTablePpiPtr, DetectionTablePtr->DeviceDetectionList[Index].PinNum2) ==
                          DetectionTablePtr->DeviceDetectionList[Index].Value2);
          Pin3Detected = (CpmTablePpiPtr->CommonFunction.GetGpio (CpmTablePpiPtr, DetectionTablePtr->DeviceDetectionList[Index].PinNum3) ==
                          DetectionTablePtr->DeviceDetectionList[Index].Value3);
          Detected = Pin1Detected && Pin2Detected && Pin3Detected;
          GpioStatus = (Pin1Detected ? 1 : 0) | (Pin2Detected ? 2 : 0) | (Pin3Detected ? 4 : 0);
          break;
        } else if (DetectionTablePtr->DeviceDetectionList[Index].Type == CPM_TYPE_GPIO_2_OR) {
          Pin1Detected = (CpmTablePpiPtr->CommonFunction.GetGpio (CpmTablePpiPtr, DetectionTablePtr->DeviceDetectionList[Index].PinNum1) ==
                          DetectionTablePtr->DeviceDetectionList[Index].Value1);
          Pin2Detected = (CpmTablePpiPtr->CommonFunction.GetGpio (CpmTablePpiPtr, DetectionTablePtr->DeviceDetectionList[Index].PinNum2) ==
                          DetectionTablePtr->DeviceDetectionList[Index].Value2);
          Detected = Pin1Detected || Pin2Detected;
          GpioStatus = (Pin1Detected ? 1 : 0) | (Pin2Detected ? 2 : 0);
          break;
        } else if (DetectionTablePtr->DeviceDetectionList[Index].Type == CPM_TYPE_GPIO_3_OR) {
          Pin1Detected = (CpmTablePpiPtr->CommonFunction.GetGpio (CpmTablePpiPtr, DetectionTablePtr->DeviceDetectionList[Index].PinNum1) ==
                          DetectionTablePtr->DeviceDetectionList[Index].Value1);
          Pin2Detected = (CpmTablePpiPtr->CommonFunction.GetGpio (CpmTablePpiPtr, DetectionTablePtr->DeviceDetectionList[Index].PinNum2) ==
                          DetectionTablePtr->DeviceDetectionList[Index].Value2);
          Pin3Detected = (CpmTablePpiPtr->CommonFunction.GetGpio (CpmTablePpiPtr, DetectionTablePtr->DeviceDetectionList[Index].PinNum3) ==
                          DetectionTablePtr->DeviceDetectionList[Index].Value3);
          Detected = Pin1Detected || Pin2Detected || Pin3Detected;
          GpioStatus = (Pin1Detected ? 1 : 0) | (Pin2Detected ? 2 : 0) | (Pin3Detected ? 4 : 0);
          break;
        }
      }
    }
  }
  if (Status) {
    *Status = GpioStatus;
  }
  return Detected;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Power On/Off the device
 *
 * @param[in] This      The pointer of CPM Table Ppi or Protocol
 * @param[in] DeviceId  The device Id
 * @param[in] State     0: Power Off. 1: Power On
 *
 */
VOID
EFIAPI
CpmPowerOnDevice (
  IN       VOID                        *This,
  IN       UINT8                       DeviceId,
  IN       UINT8                       State
  )
{
  AMD_CPM_GPIO_DEVICE_POWER_TABLE     *PowerTablePtr;
  UINT8                               Index;
  UINT8                               GpioDeviceId;

  PowerTablePtr       = ((AMD_CPM_TABLE_PPI*)This)->CommonFunction.GetTablePtr (This, CPM_SIGNATURE_GPIO_DEVICE_POWER);

  for (Index = 0; Index < AMD_GPIO_DEVICE_POWER_SIZE; Index ++) {
    GpioDeviceId = PowerTablePtr->DevicePowerList[Index].DeviceId;
    if (GpioDeviceId == 0xFF || GpioDeviceId == 0x00) {
      break;
    }
    if (PowerTablePtr->DevicePowerList[Index].DeviceId == DeviceId && PowerTablePtr->DevicePowerList[Index].Mode == State) {
      switch (PowerTablePtr->DevicePowerList[Index].Type) {
      case 0:
        ((AMD_CPM_TABLE_PPI*)This)->CommonFunction.SetGpio (
                            This,
                            PowerTablePtr->DevicePowerList[Index].Config.SetGpio.Pin,
                            PowerTablePtr->DevicePowerList[Index].Config.SetGpio.Value
                            );
        break;
      case 1:
        while (((AMD_CPM_TABLE_PPI*)This)->CommonFunction.GetGpio (This, PowerTablePtr->DevicePowerList[Index].Config.WaitGpio.Pin) !=
               PowerTablePtr->DevicePowerList[Index].Config.WaitGpio.Value) {
        }
        break;
      case 2:
        ((AMD_CPM_TABLE_PPI*)This)->CommonFunction.Stall (This, PowerTablePtr->DevicePowerList[Index].Config.Stall);
        break;
      }
    }
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get the config setting of the device
 *
 * @param[in] This      The pointer of CPM Table Ppi or Protocol
 * @param[in] DeviceId  The device Id
 *
 * @retval    0         Device off
 * @retval    1         Device on
 */
UINT8
EFIAPI
CpmGetDeviceConfig (
  IN       VOID                        *This,
  IN       UINT8                       DeviceId
  )
{
  AMD_CPM_GPIO_DEVICE_CONFIG_TABLE    *ConfigTablePtr;
  UINT8                               Index;
  UINT8                               GpioDeviceId;
  UINT8                               Status;

  ConfigTablePtr = ((AMD_CPM_TABLE_PPI*)This)->CommonFunction.GetTablePtr (This, CPM_SIGNATURE_GPIO_DEVICE_CONFIG);
  Status = 1;

  for (Index = 0; Index < AMD_GPIO_DEVICE_SIZE; Index ++) {
    GpioDeviceId = ConfigTablePtr->DeviceList[Index].DeviceId;
    if (GpioDeviceId == 0xFF || GpioDeviceId == 0x00) {
      break;
    }
    if (ConfigTablePtr->DeviceList[Index].DeviceId == DeviceId) {
      Status = ConfigTablePtr->DeviceList[Index].Config.Setting.Enable;
      break;
    }
  }
  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Wait KBC Input Buffer Empty
 *
 * @param[in] BaseAddr  Base address of KBC or EC register
 *
 * @retval        EFI_SUCCESS    Input buffer has been empty successfully
 * @retval        EFI_ERROR      Empty input buffer failed (see error for more details)
 *
 */
EFI_STATUS
EFIAPI
CpmKbcWaitInputBufferEmpty (
  IN       UINT8  BaseAddr
  )
{
  EFI_STATUS      Status;
  UINT32          TimeOut;
  UINT8           Data8;

  Status = EFI_DEVICE_ERROR;

  for (TimeOut = 0; TimeOut < 65536; TimeOut++) {
    Data8 = CpmIoRead8 (BaseAddr + 0x04);
    if (!(Data8 & BIT1)) {
      Status = EFI_SUCCESS;
      break;
    }
    Data8 = CpmIoRead8 (0x64);      // Flush it for AC/DC issue
    Data8 = CpmIoRead8 (0x80);      // Dummy read for delay
    Data8 = CpmIoRead8 (0x80);
    Data8 = CpmIoRead8 (0x80);
    Data8 = CpmIoRead8 (0x80);
  }
  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Wait KBC Output Buffer Full
 *
 * @param[in] BaseAddr  Base address of KBC or EC register
 *
 * @retval        EFI_SUCCESS    Output Buffer has been fulled successfully
 * @retval        EFI_ERROR      Full Output Buffer failed (see error for more details)
 */
EFI_STATUS
EFIAPI
CpmKbcWaitOutputBufferFull (
  IN       UINT8  BaseAddr
  )
{
  EFI_STATUS      Status;
  UINT32          TimeOut;
  UINT8           Data8;

  Status = EFI_DEVICE_ERROR;

  for (TimeOut = 0; TimeOut < 65536; TimeOut++) {
    Data8 = CpmIoRead8 (BaseAddr + 0x04);
    if (Data8 & BIT0) {
      Status = EFI_SUCCESS;
      break;
    }
    Data8 = CpmIoRead8 (0x80);      // Dummy read for delay
    Data8 = CpmIoRead8 (0x80);
    Data8 = CpmIoRead8 (0x80);
    Data8 = CpmIoRead8 (0x80);
  }
  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Wait KBC Output Buffer Empty
 *
 * @param[in] BaseAddr  Base address of KBC or EC register
 *
 * @retval        EFI_SUCCESS    Output Buffer has been Empty successfully
 * @retval        EFI_ERROR      Empty Output Buffer failed (see error for more details)
 */
EFI_STATUS
EFIAPI
CpmKbcWaitOutputBufferEmpty (
  IN       UINT8  BaseAddr
  )
{
  EFI_STATUS      Status;
  UINT32          TimeOut;
  UINT8           Data8;

  Status = EFI_DEVICE_ERROR;

  for (TimeOut = 0; TimeOut < 65536; TimeOut++) {
    Data8 = CpmIoRead8 (BaseAddr + 0x04);
    if (Data8 & BIT0) {
      Data8 = CpmIoRead8 (0x60);
      Data8 = CpmIoRead8 (0x80);      // Dummy read for delay
      Data8 = CpmIoRead8 (0x80);
      Data8 = CpmIoRead8 (0x80);
      Data8 = CpmIoRead8 (0x80);
    } else {
      Status = EFI_SUCCESS;
      break;
    }
  }
  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Read KBC register
 *
 * @param[in] BaseAddr  Base address of KBC or EC register
 * @param[in] Command   Command to send to KBC controller
 * @param[in] Data      Data to send to KBC controller
 * @param[in] Value      Value to read from KBC controller
 *
 * @retval        EFI_SUCCESS    Read KBC data successfully
 * @retval        EFI_ERROR      Read KBC data failed (see error for more details)
 */
EFI_STATUS
EFIAPI
CpmKbcRead (
  IN       UINT8                       BaseAddr,
  IN       UINT8                       Command,
  IN       UINT8                       *Data,
     OUT   UINT8                       *Value
  )
{
  EFI_STATUS  Status;

  Status = CpmKbcWaitInputBufferEmpty (BaseAddr);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  CpmIoWrite8 (BaseAddr + 0x04, Command);

  if (Data) {
    Status = CpmKbcWaitInputBufferEmpty (BaseAddr);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    CpmIoWrite8 (BaseAddr, *Data);
  }

  if (Value) {
    Status = CpmKbcWaitOutputBufferFull (BaseAddr);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    *Value = CpmIoRead8 (BaseAddr);
  } else {
    Status = EFI_INVALID_PARAMETER;
  }
  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write KBC register
 *
 * @param[in] BaseAddr  Base address of KBC or EC register
 * @param[in] Command   Command to send to KBC controller
 * @param[in] Data      Data to send to KBC controller
 *
 * @retval        EFI_SUCCESS    Write KBC controller successfully
 * @retval        EFI_ERROR      Write KBC controller failed (see error for more details)
 */
EFI_STATUS
EFIAPI
CpmKbcWrite (
  IN       UINT8                       BaseAddr,
  IN       UINT8                       Command,
  IN       UINT8                       *Data
  )
{
  EFI_STATUS  Status;

  Status = CpmKbcWaitInputBufferEmpty (BaseAddr);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  CpmIoWrite8 (BaseAddr + 0x04, Command);

  if (Data) {
    Status = CpmKbcWaitInputBufferEmpty (BaseAddr);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    CpmIoWrite8 (BaseAddr, *Data);
    Status = CpmKbcWaitInputBufferEmpty (BaseAddr);
  }

  return Status;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Register CPM base functions
 *
 * @param[in] This      The pointer of CPM Table Ppi or Protocol
 *
 */
VOID
EFIAPI
CpmRegisterBaseIo (
  IN       VOID                        *This
  )
{
  AMD_CPM_COMMON_FUNCTION             *CommonFunctionPtr;

  CommonFunctionPtr = &((AMD_CPM_TABLE_PPI*)This)->CommonFunction;

  CommonFunctionPtr->IoRead8          = CpmIoRead8;
  CommonFunctionPtr->IoRead16         = CpmIoRead16;
  CommonFunctionPtr->IoRead32         = CpmIoRead32;
  CommonFunctionPtr->IoWrite8         = CpmIoWrite8;
  CommonFunctionPtr->IoWrite16        = CpmIoWrite16;
  CommonFunctionPtr->IoWrite32        = CpmIoWrite32;
  CommonFunctionPtr->MmioRead8        = CpmMmioRead8;
  CommonFunctionPtr->MmioRead16       = CpmMmioRead16;
  CommonFunctionPtr->MmioRead32       = CpmMmioRead32;
  CommonFunctionPtr->MmioWrite8       = CpmMmioWrite8;
  CommonFunctionPtr->MmioWrite16      = CpmMmioWrite16;
  CommonFunctionPtr->MmioWrite32      = CpmMmioWrite32;
  CommonFunctionPtr->MmioAnd8         = CpmMmioAnd8;
  CommonFunctionPtr->MmioAnd16        = CpmMmioAnd16;
  CommonFunctionPtr->MmioAnd32        = CpmMmioAnd32;
  CommonFunctionPtr->MmioOr8          = CpmMmioOr8;
  CommonFunctionPtr->MmioOr16         = CpmMmioOr16;
  CommonFunctionPtr->MmioOr32         = CpmMmioOr32;
  CommonFunctionPtr->MmioAndThenOr8   = CpmMmioAndThenOr8;
  CommonFunctionPtr->MmioAndThenOr16  = CpmMmioAndThenOr16;
  CommonFunctionPtr->MmioAndThenOr32  = CpmMmioAndThenOr32;
  CommonFunctionPtr->MsrRead          = CpmMsrRead;
  CommonFunctionPtr->MsrWrite         = CpmMsrWrite;
  CommonFunctionPtr->PciRead8         = CpmPciRead8;
  CommonFunctionPtr->PciRead16        = CpmPciRead16;
  CommonFunctionPtr->PciRead32        = CpmPciRead32;
  CommonFunctionPtr->PciWrite8        = CpmPciWrite8;
  CommonFunctionPtr->PciWrite16       = CpmPciWrite16;
  CommonFunctionPtr->PciWrite32       = CpmPciWrite32;
  CommonFunctionPtr->PciAnd8          = CpmPciAnd8;
  CommonFunctionPtr->PciAnd16         = CpmPciAnd16;
  CommonFunctionPtr->PciAnd32         = CpmPciAnd32;
  CommonFunctionPtr->PciOr8           = CpmPciOr8;
  CommonFunctionPtr->PciOr16          = CpmPciOr16;
  CommonFunctionPtr->PciOr32          = CpmPciOr32;
  CommonFunctionPtr->PciAndThenOr8    = CpmPciAndThenOr8;
  CommonFunctionPtr->PciAndThenOr16   = CpmPciAndThenOr16;
  CommonFunctionPtr->PciAndThenOr32   = CpmPciAndThenOr32;
  CommonFunctionPtr->ReadTsc          = CpmReadTsc;
  CommonFunctionPtr->CpuidRead        = CpmCpuidRead;
  CommonFunctionPtr->PostCode         = CpmPostCode;
  CommonFunctionPtr->CheckPcieDevice  = CpmCheckPcieDevice;
  CommonFunctionPtr->DetectDevice     = CpmDetectDevice;
  CommonFunctionPtr->PowerOnDevice    = CpmPowerOnDevice;
  CommonFunctionPtr->GetDeviceConfig  = CpmGetDeviceConfig;
  CommonFunctionPtr->KbcRead          = CpmKbcRead;
  CommonFunctionPtr->KbcWrite         = CpmKbcWrite;
  return;
}
