/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH SMM LIBRARY
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AMD FCH UEFI Drivers
 * @e sub-project   FCH SMM LIBRARY
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 */
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
 ******************************************************************************
 */
#define FILECODE UEFI_SMM_FCHSMMCOMPLEMENT_USB3PORTPROTECTWA_LIBAMDSMMLIB_FILECODE

#include "Usb3PortProtectWA.h"

#include "FchPlatform.h"

/*---------------------------------------------------------------------------------------*/
/**
 * Read memory/MMIO
 *
 *
 * @param[in] AccessWidth   Access width
 * @param[in] MemAddress    Memory address
 * @param[in] Value         Pointer to data
 * @param[in] StdHeader     Standard configuration header
 *
 */
VOID
LibAmdMemRead (
  IN       ACCESS_WIDTH       AccessWidth,
  IN       UINT64             MemAddress,
     OUT   VOID               *Value,
  IN OUT   AMD_CONFIG_PARAMS  *StdHeader
  )
{
  EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo;
  EFI_SMM_IO_WIDTH            SmmWidth;

  SmmIo = (EFI_SMM_CPU_IO2_PROTOCOL *) StdHeader;

  switch (AccessWidth) {
  case AccessWidth8:
    SmmWidth = SMM_IO_UINT8;
    break;
  case AccessWidth16:
    SmmWidth = SMM_IO_UINT16;
    break;
  case AccessWidth32:
    SmmWidth = SMM_IO_UINT32;
    break;
  default:
    ASSERT_EFI_ERROR (FALSE);
    return;
  }
  SmmIo->Mem.Read (SmmIo, SmmWidth, (UINT64) MemAddress, 1, Value);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write memory/MMIO
 *
 *
 * @param[in] AccessWidth   Access width
 * @param[in] MemAddress    Memory address
 * @param[in] Value         Pointer to data
 * @param[in] StdHeader     Standard configuration header
 *
 */
VOID
LibAmdMemWrite (
  IN       ACCESS_WIDTH       AccessWidth,
  IN       UINT64             MemAddress,
  IN       VOID               *Value,
  IN OUT   AMD_CONFIG_PARAMS  *StdHeader
  )
{
  EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo;
  EFI_SMM_IO_WIDTH            SmmWidth;

  SmmIo = (EFI_SMM_CPU_IO2_PROTOCOL *) StdHeader;

  switch (AccessWidth) {
  case AccessWidth8:
    SmmWidth = SMM_IO_UINT8;
    break;
  case AccessWidth16:
    SmmWidth = SMM_IO_UINT16;
    break;
  case AccessWidth32:
    SmmWidth = SMM_IO_UINT32;
    break;
  default:
    ASSERT_EFI_ERROR (FALSE);
    return;
  }
  SmmIo->Mem.Write (SmmIo, SmmWidth, (UINT64) MemAddress, 1, Value);
}


/*---------------------------------------------------------------------------------------*/
/**
 * Read IO port
 *
 *
 * @param[in] AccessWidth   Access width
 * @param[in] IoAddress     IO port address
 * @param[in] Value         Pointer to save data
 * @param[in] StdHeader     Standard configuration header
 *
 */
VOID
LibAmdIoRead (
  IN       ACCESS_WIDTH       AccessWidth,
  IN       UINT16             IoAddress,
     OUT   VOID               *Value,
  IN OUT   AMD_CONFIG_PARAMS  *StdHeader
  )
{
  EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo;
  EFI_SMM_IO_WIDTH            SmmWidth;


  SmmIo = (EFI_SMM_CPU_IO2_PROTOCOL *) StdHeader;

  switch (AccessWidth) {
  case AccessWidth8:
    SmmWidth = SMM_IO_UINT8;
    break;
  case AccessWidth16:
    SmmWidth = SMM_IO_UINT16;
    break;
  case AccessWidth32:
    SmmWidth = SMM_IO_UINT32;
    break;
  default:
    ASSERT_EFI_ERROR (FALSE);
    return;
  }
  SmmIo->Io.Read (SmmIo, SmmWidth, (UINT64) IoAddress, 1, Value);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Write IO port
 *
 *
 * @param[in] AccessWidth   Access width
 * @param[in] IoAddress     IO port address
 * @param[in] Value         Pointer to data
 * @param[in] StdHeader     Standard configuration header
 *
 */
VOID
LibAmdIoWrite (
  IN       ACCESS_WIDTH        AccessWidth,
  IN       UINT16              IoAddress,
  IN       VOID                *Value,
  IN OUT   AMD_CONFIG_PARAMS   *StdHeader
  )
{
  EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo;

  EFI_SMM_IO_WIDTH            SmmWidth;


  SmmIo = (EFI_SMM_CPU_IO2_PROTOCOL *) StdHeader;

  switch (AccessWidth) {
  case AccessWidth8:
    SmmWidth = SMM_IO_UINT8;
    break;
  case AccessWidth16:
    SmmWidth = SMM_IO_UINT16;
    break;
  case AccessWidth32:
    SmmWidth = SMM_IO_UINT32;
    break;
  default:
    ASSERT_EFI_ERROR (FALSE);
    return;
  }
  SmmIo->Io.Write (SmmIo, SmmWidth, (UINT64) IoAddress, 1, Value);
}


/*---------------------------------------------------------------------------------------*/
/**
 * Read PCI config space
 *
 *
 * @param[in] AccessWidth   Access width
 * @param[in] PciAddress    Pci address
 * @param[in] Value         Pointer to data
 * @param[in] StdHeader     Standard configuration header
 *
 */
VOID
LibAmdPciRead (
  IN       ACCESS_WIDTH       AccessWidth,
  IN       PCI_ADDR           PciAddress,
     OUT   VOID               *Value,
  IN OUT   AMD_CONFIG_PARAMS  *StdHeader
  )
{
  UINT32                      Address32;

  Address32 = BIT31 + (UINT32) ((PciAddress.Address.Bus << 16)         + \
                                (PciAddress.Address.Device << 11)      + \
                                (PciAddress.Address.Function << 8)     + \
                                (PciAddress.Address.Register & 0xFF));
  LibAmdIoWrite (AccessWidth32, CFG_ADDR_PORT, &Address32, StdHeader);
  LibAmdIoRead (AccessWidth, CFG_DATA_PORT, Value, StdHeader);
}


/*---------------------------------------------------------------------------------------*/
/**
 * Write PCI config space
 *
 *
 * @param[in] AccessWidth   Access width
 * @param[in] PciAddress    Pci address
 * @param[in] Value         Pointer to data
 * @param[in] StdHeader     Standard configuration header
 *
 */
VOID
LibAmdPciWrite (
  IN       ACCESS_WIDTH       AccessWidth,
  IN       PCI_ADDR           PciAddress,
  IN       VOID               *Value,
  IN OUT   AMD_CONFIG_PARAMS  *StdHeader
  )
{
  UINT32                      Address32;

  Address32 = BIT31 + (UINT32) ((PciAddress.Address.Bus << 16)         + \
                                (PciAddress.Address.Device << 11)      + \
                                (PciAddress.Address.Function << 8)     + \
                                (PciAddress.Address.Register & 0xFF));
  LibAmdIoWrite (AccessWidth32, CFG_ADDR_PORT, &Address32, StdHeader);
  LibAmdIoWrite (AccessWidth, CFG_DATA_PORT, Value, StdHeader);
}


/*---------------------------------------------------------------------------------------*/
/**
 * PCI read modify write
 *
 *
 * @param[in] AccessWidth   Access width
 * @param[in] PciAddress    Pci address
 * @param[in] Data          SetBits, to bitwise-OR with read value
 * @param[in] DataMask      ClearBits, to negate and bitwise-AND with read value
 * @param[in] StdHeader     Standard configuration header
 *
 */
VOID
LibAmdPciRMW (
  IN       ACCESS_WIDTH       AccessWidth,
  IN       PCI_ADDR           PciAddress,
  IN       VOID               *Data,
  IN       VOID               *DataMask,
  IN OUT   AMD_CONFIG_PARAMS  *StdHeader
  )
{
  UINT32        Temp32;

  LibAmdPciRead (AccessWidth, PciAddress, &Temp32, StdHeader);
  Temp32 &= ~(*(UINT32*) DataMask);
  Temp32 |= *(UINT32*) Data;
  LibAmdPciWrite (AccessWidth, PciAddress, &Temp32, StdHeader);
}

/*---------------------------------------------------------------------------------------*/
/**
 *  ASSERT backend function for PEI phase
 *
 **/
BOOLEAN
IdsAssert (
  IN       UINT32      FileCode
  )
{
  CpuDeadLoop ();
  return TRUE;
}
