/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH PEI LIBRARY
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH PEI LIBRARY
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
#include "PiPei.h"
#include <Library\DebugLib.h>

#include "AMD.h"
#define FILECODE UEFI_PEI_FCHPEILIB_LIBAMDPEI_FILECODE


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
  EFI_PEI_SERVICES     **PeiServices;
  EFI_PEI_CPU_IO_PPI      *CpuIo;

  PeiServices = (EFI_PEI_SERVICES**) (StdHeader->AltImageBasePtr);
  CpuIo = (*PeiServices)->CpuIo;

  switch (AccessWidth) {
  case AccessWidth8:
    *(UINT8 *) Value = CpuIo->MemRead8 (PeiServices, CpuIo, MemAddress);
    break;
  case AccessWidth16:
    *(UINT16 *) Value = CpuIo->MemRead16 (PeiServices, CpuIo, MemAddress);
    break;
  case AccessWidth32:
    *(UINT32 *) Value = CpuIo->MemRead32 (PeiServices, CpuIo, MemAddress);
    break;
  default:
    ASSERT (FALSE);
  }
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
  EFI_PEI_SERVICES     **PeiServices;
  EFI_PEI_CPU_IO_PPI   *CpuIo;

  PeiServices = (EFI_PEI_SERVICES**) (StdHeader->AltImageBasePtr);
  CpuIo = (*PeiServices)->CpuIo;

  switch (AccessWidth) {
  case AccessWidth8:
    CpuIo->MemWrite8 (PeiServices, CpuIo, MemAddress, *(UINT8 *) Value);
    break;
  case AccessWidth16:
    CpuIo->MemWrite16 (PeiServices, CpuIo, MemAddress, *(UINT16 *) Value);
    break;
  case AccessWidth32:
    CpuIo->MemWrite32 (PeiServices, CpuIo, MemAddress, *(UINT32 *) Value);
    break;
  default:
    ASSERT (FALSE);
  }
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
  EFI_PEI_SERVICES     **PeiServices;
  EFI_PEI_CPU_IO_PPI      *CpuIo;

  PeiServices = (EFI_PEI_SERVICES**) (StdHeader->AltImageBasePtr);
  CpuIo = (*PeiServices)->CpuIo;

  switch (AccessWidth) {
  case AccessWidth8:
    *(UINT8 *) Value = CpuIo->IoRead8 (PeiServices, CpuIo, IoAddress);
    break;
  case AccessWidth16:
    *(UINT16 *) Value = CpuIo->IoRead16 (PeiServices, CpuIo, IoAddress);
    break;
  case AccessWidth32:
    *(UINT32 *) Value = CpuIo->IoRead32 (PeiServices, CpuIo, IoAddress);
    break;
  default:
    ASSERT (FALSE);
  }
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
  EFI_PEI_SERVICES     **PeiServices;
  EFI_PEI_CPU_IO_PPI      *CpuIo;

  PeiServices = (EFI_PEI_SERVICES**) (StdHeader->AltImageBasePtr);
  CpuIo = (*PeiServices)->CpuIo;

  switch (AccessWidth) {
  case AccessWidth8:
    CpuIo->IoWrite8 (PeiServices, CpuIo, IoAddress, *(UINT8 *) Value);
    break;
  case AccessWidth16:
    CpuIo->IoWrite16 (PeiServices, CpuIo, IoAddress, *(UINT16 *) Value);
    break;
  case AccessWidth32:
    CpuIo->IoWrite32 (PeiServices, CpuIo, IoAddress, *(UINT32 *) Value);
    break;
  default:
    ASSERT (FALSE);
  }
}


UINT64
ConvertPciAddress (
  IN       PCI_ADDR     AmdPci
  )
{
  UINT32   reg;

  reg = AmdPci.Address.Register;
  return (UINT64) ((AmdPci.Address.Bus << 24) + \
                   (AmdPci.Address.Device << 16) + \
                   (AmdPci.Address.Function << 8) + \
                   ((reg < 256)? reg : (UINT64) ((reg * 2) << 31)) \
                   );
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
  EFI_PEI_SERVICES       **PeiServices;
  EFI_PEI_PCI_CFG2_PPI   *PciCfg;
  EFI_PEI_PCI_CFG_PPI_WIDTH PciWidth;

  PeiServices = (EFI_PEI_SERVICES**) (StdHeader->AltImageBasePtr);
  PciCfg = (*PeiServices)->PciCfg;

  switch (AccessWidth) {
  case AccessWidth8:
    PciWidth = EfiPeiPciCfgWidthUint8 ;
    break;
  case AccessWidth16:
    PciWidth = EfiPeiPciCfgWidthUint16 ;
    break;
  case AccessWidth32:
    PciWidth = EfiPeiPciCfgWidthUint32 ;
    break;
  default:
    PciWidth = EfiPeiPciCfgWidthUint32 ;
    ASSERT (FALSE);
  }
  PciCfg->Read (
        PeiServices,
        PciCfg,
        PciWidth,
        ConvertPciAddress (PciAddress),
        Value
        );
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
  EFI_PEI_SERVICES       **PeiServices;
  EFI_PEI_PCI_CFG2_PPI   *PciCfg;
  EFI_PEI_PCI_CFG_PPI_WIDTH PciWidth;


  PeiServices = (EFI_PEI_SERVICES**) (StdHeader->AltImageBasePtr);
  PciCfg = (*PeiServices)->PciCfg;

  switch (AccessWidth) {
  case AccessWidth8:
    PciWidth = EfiPeiPciCfgWidthUint8;
    break;
  case AccessWidth16:
    PciWidth = EfiPeiPciCfgWidthUint16;
    break;
  case AccessWidth32:
    PciWidth = EfiPeiPciCfgWidthUint32;
    break;
  default:
    PciWidth = EfiPeiPciCfgWidthUint32;
    ASSERT ( FALSE);
  }
  PciCfg->Write (
            PeiServices,
            PciCfg,
            PciWidth,
            ConvertPciAddress (PciAddress),
            Value
            );
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
  EFI_PEI_SERVICES       **PeiServices;
  EFI_PEI_PCI_CFG2_PPI   *PciCfg;
  EFI_PEI_PCI_CFG_PPI_WIDTH  PciWidth;

  PeiServices = (EFI_PEI_SERVICES**) (StdHeader->AltImageBasePtr);
  PciCfg = (*PeiServices)->PciCfg;

  switch (AccessWidth) {
  case AccessWidth8:
    PciWidth = EfiPeiPciCfgWidthUint8;
    break;
  case AccessWidth16:
    PciWidth = EfiPeiPciCfgWidthUint16;
    break;
  case AccessWidth32:
    PciWidth = EfiPeiPciCfgWidthUint32;
    break;
  default:
    PciWidth = EfiPeiPciCfgWidthUint32;
    ASSERT ( FALSE);
  }

  PciCfg->Modify (
            PeiServices,
            PciCfg,
            PciWidth,
            ConvertPciAddress (PciAddress),
            Data,
            DataMask
            );
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
  //EFI_BREAKPOINT ();
  CpuDeadLoop ();
  return TRUE;
}

