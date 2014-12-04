/* $NoKeywords:$ */
/**
 * @file
 *
 * Usb3PortProtectWA Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AMD Hudson-2 UEFI Driver
 * @e sub-project   Usb3PortProtectWA Driver
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
#define FILECODE UEFI_SMM_FCHSMMCOMPLEMENT_USB3PORTPROTECTWA_USB3PORTPROTECTWA_FILECODE
#include <Library/FchSmmLib.h>
#include "Usb3PortProtectWA.h"

EFI_SMM_CPU_IO2_PROTOCOL             *mSmmIo;

FCH_INIT_PROTOCOL                    gAmdFchInitInSmm;
BOOLEAN                              mPortU3Flag[4];
BOOLEAN                              mXhci0Enable;
BOOLEAN                              mXhci1Enable;
UINT32                               *mUserDefineXhciRomAddr;

EFI_STATUS
EFIAPI
Usb3PortProtectCallback (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SX_REGISTER_CONTEXT *DispatchContext,
  IN OUT   VOID                              *CommBuffer OPTIONAL,
  IN OUT   UINTN                             *CommBufferSize  OPTIONAL
  )
{
  UINT8                     dByte;
  UINT8                     controller;
  UINT8                     port;
  UINT32                    Or32;
  UINT32                    Mask32;
  UINT32                    dDword;
  UINT32                    dBar0;
  PCI_ADDR                  PciAddress;
  AMD_CONFIG_PARAMS         *SmmIo;
  EFI_STATUS                Status;

  Status = EFI_SUCCESS;
  SmmIo = (AMD_CONFIG_PARAMS*) &gSmst->SmmIo;

  for (controller = 0; controller < 2; controller ++) {
    PciAddress.AddressValue = ((((FCH_XHCI_DEV << 3) + controller) << 12) + FCH_XHCI_REG10);
    LibAmdPciRead (AccessWidth32, PciAddress, &dBar0, SmmIo);
    if (dBar0 != -1) {
      dByte = 0x04;
      dDword = 0x04;
      PciAddress.AddressValue = ((((FCH_XHCI_DEV << 3) + controller) << 12) + FCH_XHCI_REG48);
      LibAmdPciWrite (AccessWidth32, PciAddress, &dDword, SmmIo);
      Or32 = 0;
      Mask32 = BIT8;
      PciAddress.AddressValue = ((((FCH_XHCI_DEV << 3) + controller) << 12) + FCH_XHCI_REG4C);
      LibAmdPciRMW (AccessWidth32, PciAddress, &Or32, &Mask32, SmmIo);
      for (port = 0; port < 2; port ++) {
        mPortU3Flag[(controller << 1) + port] = FALSE;
        dDword = 0x420 + port * 0x10;
        PciAddress.AddressValue = ((((FCH_XHCI_DEV << 3) + controller) << 12) + FCH_XHCI_REG40);
        LibAmdPciWrite (AccessWidth32, PciAddress, &dDword, SmmIo);
        PciAddress.AddressValue = ((((FCH_XHCI_DEV << 3) + controller) << 12) + FCH_XHCI_REG44);
        LibAmdPciRead (AccessWidth32, PciAddress, &dDword, SmmIo);
        if ((dDword & (0xf << 5)) == (0x3 << 5)) {
          mPortU3Flag[(controller << 1) + port] = TRUE;
        }
        if ((dDword & 0x01) == 0) {
          dDword &= ~ BIT9;
          PciAddress.AddressValue = ((((FCH_XHCI_DEV << 3) + controller) << 12) + FCH_XHCI_REG44);
          LibAmdPciWrite (AccessWidth32, PciAddress, &dDword, SmmIo);
        }
      }
      Or32 = ( BIT8 + BIT9 );
      Mask32 = BIT8 ;
      PciAddress.AddressValue = ((((FCH_XHCI_DEV << 3) + controller) << 12) + FCH_XHCI_REG4C);
      LibAmdPciRMW (AccessWidth32, PciAddress, &Or32, &Mask32, SmmIo);
    }
  }
  return Status;
}

EFI_STATUS
EFIAPI
Usb3RestorePortProtectCallback (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SW_REGISTER_CONTEXT *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT   UINTN                             *SizeOfSwContext
  )
{
  UINT8                  controller;
  UINT8                  port;
  UINT8                  dByte;
  UINT32                 dDword;
  UINT32                 dBar0;
  UINT32                 Or32;
  UINT32                 Mask32;
  PCI_ADDR               PciAddress;
  AMD_CONFIG_PARAMS      *SmmIo;
  EFI_STATUS             Status;

  Status = EFI_SUCCESS;
  SmmIo = (AMD_CONFIG_PARAMS*) &gSmst->SmmIo;

  for (controller = 0; controller < 2; controller ++) {
    PciAddress.AddressValue = ((((FCH_XHCI_DEV << 3) + controller) << 12) + FCH_XHCI_REG10);
    LibAmdPciRead (AccessWidth32, PciAddress, &dBar0, SmmIo);
    if (dBar0 != -1) {
      dByte = 0x04;
      dDword = 0x04;
      PciAddress.AddressValue = ((((FCH_XHCI_DEV << 3) + controller) << 12) + FCH_XHCI_REG48);
      LibAmdPciWrite (AccessWidth32, PciAddress, &dDword, SmmIo);

      Or32 = 0;
      Mask32 = BIT8;
      PciAddress.AddressValue = ((((FCH_XHCI_DEV << 3) + controller) << 12) + FCH_XHCI_REG4C);
      LibAmdPciRMW (AccessWidth32, PciAddress, &Or32, &Mask32, SmmIo);

      for (port = 0; port < 2; port ++) {
        dDword = 0x420 + port * 0x10;
        PciAddress.AddressValue = ((((FCH_XHCI_DEV << 3) + controller) << 12) + FCH_XHCI_REG40);
        LibAmdPciWrite (AccessWidth32, PciAddress, &dDword, SmmIo);
        PciAddress.AddressValue = ((((FCH_XHCI_DEV << 3) + controller) << 12) + FCH_XHCI_REG44);
        LibAmdPciRead (AccessWidth32, PciAddress, &dDword, SmmIo);
        if (mPortU3Flag[(controller << 1) + port]) {
          if (((dDword & (0xf << 5)) == (0x5 << 5))) {
            dDword |= BIT31;
            PciAddress.AddressValue = ((((FCH_XHCI_DEV << 3) + controller) << 12) + FCH_XHCI_REG44);
            LibAmdPciWrite (AccessWidth32, PciAddress, &dDword, SmmIo);
            FchStall (500, SmmIo);
            PciAddress.AddressValue = ((((FCH_XHCI_DEV << 3) + controller) << 12) + FCH_XHCI_REG44);
            LibAmdPciRead (AccessWidth32, PciAddress, &dDword, SmmIo);
            if (dDword & BIT4) {
              break;
            }
          }
          if ((dDword & (0xf << 5)) == (0x7 << 5)) {
            dDword |= BIT31;
            PciAddress.AddressValue = ((((FCH_XHCI_DEV << 3) + controller) << 12) + FCH_XHCI_REG44);
            LibAmdPciWrite (AccessWidth32, PciAddress, &dDword, SmmIo);
            FchStall (500, SmmIo);
          }
        }
        if ((dDword & BIT9) == 0) {
          dDword |= BIT9;
          PciAddress.AddressValue = ((((FCH_XHCI_DEV << 3) + controller) << 12) + FCH_XHCI_REG44);
          LibAmdPciWrite (AccessWidth32, PciAddress, &dDword, SmmIo);
        }
      }
    }
  }
  return Status;
}

EFI_STATUS
EFIAPI
Usb3PmeClearCallback (
  IN       EFI_HANDLE                        DispatchHandle,
  IN       CONST FCH_SMM_SX_REGISTER_CONTEXT *DispatchContext,
  IN OUT   VOID                              *CommBuffer OPTIONAL,
  IN OUT   UINTN                             *CommBufferSize  OPTIONAL
  )
{
  UINT8                  controller;
  UINT8                  dByte;
  UINT32                 dDword;
  UINT32                 dDwordReg54;
  UINT64                 MemAddress;
  PCI_ADDR               PciAddress;
  AMD_CONFIG_PARAMS      *SmmIo;
  EFI_STATUS             Status;

  Status = EFI_SUCCESS;
  SmmIo = (AMD_CONFIG_PARAMS*) &gSmst->SmmIo;

  for (controller = 0; controller < 2; controller ++) {
    PciAddress.AddressValue = ((((FCH_XHCI_DEV << 3) + controller) << 12) + FCH_XHCI_REG00);
    LibAmdPciRead (AccessWidth32, PciAddress, &dDword, SmmIo);
    if (dDword != 0xffffffff) {
      PciAddress.AddressValue = ((((FCH_XHCI_DEV << 3) + controller) << 12) + FCH_XHCI_REG54);
      LibAmdPciRead (AccessWidth32, PciAddress, &dDwordReg54, SmmIo);
      if (dDwordReg54 & BIT15) {
        MemAddress = (UINT64) (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_xHC0Pme + controller);
        LibAmdMemRead (AccessWidth8, MemAddress, &dByte, SmmIo);
        MemAddress = (UINT64) (ACPI_MMIO_BASE + SMI_BASE + FCH_SMI_REG00);
        LibAmdMemRead (AccessWidth32, MemAddress, &dDword, SmmIo);
        if ((dDword & (1 << dByte)) == 0 ) {
          dDwordReg54 |= BIT15;
          PciAddress.AddressValue = ((((FCH_XHCI_DEV << 3) + controller) << 12) + FCH_XHCI_REG54);
          LibAmdPciWrite (AccessWidth32, PciAddress, &dDwordReg54, SmmIo);
        }
      }
    }
  }

  return Status;
}

EFI_STATUS
Usb3PortProtectRegister ( VOID )
{
  EFI_STATUS                             Status;
  FCH_SMM_SW_DISPATCH2_PROTOCOL          *AmdSwDispatch;
  FCH_SMM_SW_REGISTER_CONTEXT            SwRegisterContext;
  EFI_HANDLE                             SwHandle;
  FCH_SMM_SX_DISPATCH2_PROTOCOL          *AmdSxDispatch;
  FCH_SMM_SX_REGISTER_CONTEXT            SxRegisterContext;
  EFI_HANDLE                             SxHandle;
  AMD_CONFIG_PARAMS                      *SmmIo;

  SmmIo = (AMD_CONFIG_PARAMS*) &gSmst->SmmIo;

  if ( ReadFchChipsetRevision ( SmmIo ) < FCH_BOLTON ) {
    //
    //  Locate SMM SW dispatch protocol
    //
    Status = gSmst->SmmLocateProtocol (
                    &gFchSmmSwDispatch2ProtocolGuid,
                    NULL,
                    &AmdSwDispatch
                    );
    ASSERT_EFI_ERROR (Status);

    SwRegisterContext.AmdSwValue  = FCHOEM_AFTER_PCI_RESTORE_SWSMI;
    SwRegisterContext.Order       = 0x80;
    Status = AmdSwDispatch->Register (
                              AmdSwDispatch,
                              Usb3RestorePortProtectCallback,
                              &SwRegisterContext,
                              &SwHandle
                              );
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }
  //
  //  Locate SMM Sx dispatch protocol
  //
  Status = gSmst->SmmLocateProtocol (
                    &gFchSmmSxDispatch2ProtocolGuid,
                    NULL,
                    &AmdSxDispatch
                    );
  ASSERT_EFI_ERROR (Status);

  if ( ReadFchChipsetRevision ( SmmIo ) < FCH_BOLTON ) {
    SxRegisterContext.Type  = SxS3;
    SxRegisterContext.Phase = SxEntry;
    SxRegisterContext.Order = 1;

    Status = AmdSxDispatch->Register (
                               AmdSxDispatch,
                               Usb3PortProtectCallback,
                               &SxRegisterContext,
                               &SxHandle
                               );
  }

  SxRegisterContext.Type  = SxS3;
  SxRegisterContext.Phase = SxEntry;
  SxRegisterContext.Order = 1;

  Status = AmdSxDispatch->Register (
                             AmdSxDispatch,
                             Usb3PmeClearCallback,
                             &SxRegisterContext,
                             &SxHandle
                             );

  SxRegisterContext.Type  = SxS4;
  SxRegisterContext.Phase = SxEntry;
  SxRegisterContext.Order = 1;

  Status = AmdSxDispatch->Register (
                             AmdSxDispatch,
                             Usb3PmeClearCallback,
                             &SxRegisterContext,
                             &SxHandle
                             );

  return Status;
}

/*********************************************************************************
 * Name: Usb3PortProtectWAEntry
 *
 * Description
 *   Entry point of the Usb3PortProtect driver
 *   Register Smm callbacks for xHCI port protect Work-around
 *
 * Input
 *   ImageHandle : EFI Image Handle for the DXE driver
 *   SystemTable : pointer to the EFI system table
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 *********************************************************************************/
EFI_STATUS
Usb3PortProtectWAEntry (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  )

{
  FCH_INIT_PROTOCOL        *AmdFchInit;
  EFI_STATUS               Status;

  mSmmIo = &gSmst->SmmIo;
  if (mSmmIo == NULL) {
    return EFI_LOAD_ERROR;
  }

  mXhci0Enable = FALSE;
  mXhci1Enable = FALSE;
  mUserDefineXhciRomAddr = NULL;

  Status = gBS->LocateProtocol (
                  &gFchInitProtocolGuid,
                  NULL,
                  &AmdFchInit
                  );
  if (!EFI_ERROR (Status)) {
    mXhci0Enable = AmdFchInit->FchPolicy.Usb.Xhci0Enable;
    mXhci1Enable = AmdFchInit->FchPolicy.Usb.Xhci1Enable;
    mUserDefineXhciRomAddr = (UINT32 *) AmdFchInit->FchPolicy.Usb.UserDefineXhciRomAddr;
  } else {
    return EFI_ABORTED;
  }

  return Usb3PortProtectRegister ();
}
