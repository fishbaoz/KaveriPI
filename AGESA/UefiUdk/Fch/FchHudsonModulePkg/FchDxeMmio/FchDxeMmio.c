/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH DXE AUX Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH MMIO Range programming Service
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
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>

#include <Library/AgesaUefiLib.h>

#include <Protocol\AmdAgesaMmioProtocol.h>
#include "mmioMapManager.h"
#include <Library/FchDxeLib.h>

#define FILECODE UEFI_DXE_FCHDXEMMIO_FCHDXEMMIO_FILECODE

/**
 *---------------------------------------------------------------------------------------
 *
 *  FchDxeMmioEntryPoint
 *
 *  Description:
 *     Entry point for the cpu driver.
 *
 *  Parameters:
 *    @param[in]     ImageHandle
 *    @param[in]     *SystemTable
 *
 *    @retval         AGESA_STATUS
 *
 *---------------------------------------------------------------------------------------
 **/

EFI_STATUS
EFIAPI
FchDxeMmioEntryPoint (
  IN       EFI_HANDLE                       ImageHandle,
  IN       EFI_SYSTEM_TABLE                 *SystemTable
  )
{

  EFI_STATUS                   Status;
  AMD_MMIO_ATTRIBUTE           Attribute;
  PCI_ADDR                     PciAddr;
  UINT32                       PciAddress;
  AMD_AGESA_MMIO_PROTOCOL      *pMmioProtocol;
  AMD_AGESA_MMIO_PROTOCOL      *StdHeader;
  UINT32                       MmioAddr;
  UINT32                       ChipBaseAddress;
  UINTN                        BaseAddress;
  UINTN                        Length;

  Status = gBS->LocateProtocol (&gAmdAgesaMmioProtocolGuid, NULL, &pMmioProtocol);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  StdHeader = ( AMD_AGESA_MMIO_PROTOCOL* ) &pMmioProtocol;

  // AGESA MMIO protocol exist
  LibFchGetAcpiMmioBase (&MmioAddr);
  MmioAddr &= 0xFFFFC000;
  Attribute.MmioReadableRange = 1;
  Attribute.MmioWritableRange = 1;
  Attribute.MmioPostedRange = 1;
  Attribute.MmioSecuredRange = 0;
  Attribute.OverrideExisting = 1;
  PciAddr.AddressValue = 0;
  PciAddr.Address.Register = 0;

  // Check ROM Range
  PciAddress = (UINT32) (LPC_BUS_DEV_FUN << 16) + FCH_LPC_REG6C;
  LibFchPciRead (EfiPciWidthUint32, PciAddress, &ChipBaseAddress);
  BaseAddress = (0xFFFF & ChipBaseAddress) << 16;
  Length = (( UINTN )ChipBaseAddress & 0xFFFF0000) - BaseAddress + (1 << 16) - 1;
  Status = pMmioProtocol->AmdMmioMapManager (
                                    StdHeader,
                                    BaseAddress,
                                    Length,
                                    PciAddr,
                                    Attribute
                                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Attribute.OverrideExisting = 0;
  // SPI
  PciAddress = (UINT32) (LPC_BUS_DEV_FUN << 16) + FCH_LPC_REGA0;
  LibFchPciRead (EfiPciWidthUint32, PciAddress, &ChipBaseAddress);
  if ( ChipBaseAddress & 0x2 ) {
    BaseAddress = ChipBaseAddress & 0xFFFF0000;
    Length = 0x20;
    Status = pMmioProtocol->AmdMmioMapManager (
                                      StdHeader,
                                      BaseAddress,
                                      Length,
                                      PciAddr,
                                      Attribute
                                      );
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }
  // TPM
  BaseAddress = 0xFED40000;
  Length = 0x5000;
  Status = pMmioProtocol->AmdMmioMapManager (
                                    StdHeader,
                                    BaseAddress,
                                    Length,
                                    PciAddr,
                                    Attribute
                                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  // HPET
  LibFchMemRead ( EfiPciWidthUint32, MmioAddr + PMIO_BASE + FCH_PMIOA_REG50, &ChipBaseAddress);
  if ( ChipBaseAddress & 0x1 ) {
    BaseAddress = ( UINTN ) ChipBaseAddress & 0xFFFFF800;
    Length = 0x200;
    Status = pMmioProtocol->AmdMmioMapManager (
                                      StdHeader,
                                      BaseAddress,
                                      Length,
                                      PciAddr,
                                      Attribute
                                      );
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }
  // BIOS RAM
  LibFchMemRead ( EfiPciWidthUint32, MmioAddr + PMIO_BASE + FCH_PMIOA_REG20, &ChipBaseAddress);
  if ( ChipBaseAddress & 0x1 ) {
    BaseAddress = ( UINTN ) ChipBaseAddress & 0xFFFFF800;
    Length = 0x100;
    Status = pMmioProtocol->AmdMmioMapManager (
                                      StdHeader,
                                      BaseAddress,
                                      Length,
                                      PciAddr,
                                      Attribute
                                      );
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }
  // WatchDog Timer
  LibFchMemRead ( EfiPciWidthUint32, MmioAddr + PMIO_BASE + FCH_PMIOA_REG48, &ChipBaseAddress);
  if ( ChipBaseAddress & 0x1 ) {
    BaseAddress = ( UINTN ) ChipBaseAddress & 0xFFFFF800;
    Length = 0x100;
    Status = pMmioProtocol->AmdMmioMapManager (
                                      StdHeader,
                                      BaseAddress,
                                      Length,
                                      PciAddr,
                                      Attribute
                                      );
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }
  // IoApic
  LibFchMemRead ( EfiPciWidthUint32, MmioAddr + PMIO_BASE + FCH_PMIOA_REG34, &ChipBaseAddress);
  if ( ChipBaseAddress & 0x1 ) {
    BaseAddress = ( UINTN ) ChipBaseAddress & 0xFFFFF800;
    Length = 0x100;
    Status = pMmioProtocol->AmdMmioMapManager (
                                      StdHeader,
                                      BaseAddress,
                                      Length,
                                      PciAddr,
                                      Attribute
                                      );
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }
  // Acpi Mmio
  LibFchMemRead ( EfiPciWidthUint32, MmioAddr + PMIO_BASE + FCH_PMIOA_REG24, &ChipBaseAddress);
  if ( ChipBaseAddress & 0x1 ) {
    BaseAddress = ( UINTN ) ChipBaseAddress & 0xFFFFF800;
    Length = 0x1600;
    Status = pMmioProtocol->AmdMmioMapManager (
                                      StdHeader,
                                      BaseAddress,
                                      Length,
                                      PciAddr,
                                      Attribute
                                      );
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }
  return Status;
}


