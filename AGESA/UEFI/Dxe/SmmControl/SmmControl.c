/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH DXE Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   SMM Control DXE Driver
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

#include "SmmControl.h"
#define FILECODE UEFI_DXE_SMMCONTROL_SMMCONTROL_FILECODE


UINT32                           mAcpiMmioBase;
UINT16                           mAcpiPmBase;
UINT8                            mAcpiSmiCmd;
EFI_PHYSICAL_ADDRESS             mAcpiMmioBase64;
STATIC EFI_SMM_CONTROL_PROTOCOL  mSmmControl;

EFI_DRIVER_ENTRY_POINT (AmdInstallSmmControl);


EFI_STATUS
SmmControlDxePreInit (
  IN       EFI_HANDLE              ImageHandle
  )
{
  UINT16              SmmControlData16;
  UINT16              SmmControlMask16;
  UINT32              SmmControlData32;
  UINT8               SmmControlData8;
  UINT8               i;
  EFI_HANDLE          mDriverImageHandle;
  EFI_STATUS          Status;
  UINT64              Length;
  UINT64              Attributes;
  EFI_GCD_MEMORY_SPACE_DESCRIPTOR GcdMemorySpaceDescriptor;

  //
  // Enable ACPI MMIO space
  //
  LibFchIndirectIoRead (EfiPciWidthUint8, FCH_IOMAP_REGCD6, FCH_PMIOA_REG24, &SmmControlData8);
  SmmControlData8 |= BIT0;
  LibFchIndirectIoWrite (EfiPciWidthUint8, FCH_IOMAP_REGCD6, FCH_PMIOA_REG24, &SmmControlData8);

  //
  // Get ACPI MMIO base and AcpiPm1EvtBlk address
  //
  LibFchGetAcpiMmioBase (&mAcpiMmioBase);
  LibFchGetAcpiPmBase (&mAcpiPmBase);
  LibFchIndirectIoRead (EfiPciWidthUint8, FCH_IOMAP_REGCD6, FCH_PMIOA_REG6A, &mAcpiSmiCmd);

  if ((0 == mAcpiMmioBase) || (0 == mAcpiPmBase)) {
    return EFI_LOAD_ERROR;
  }

  mAcpiMmioBase64 = (EFI_PHYSICAL_ADDRESS) mAcpiMmioBase;
  Length = 0x1000;
  mDriverImageHandle = ImageHandle;

  Status = gDS->AllocateMemorySpace (
                  EfiGcdAllocateAddress,
                  EfiGcdMemoryTypeMemoryMappedIo,
                  12,
                  Length,
                  &mAcpiMmioBase64,
                  mDriverImageHandle,
                  NULL
                  );

  Status = gDS->GetMemorySpaceDescriptor (mAcpiMmioBase, &GcdMemorySpaceDescriptor);
  ASSERT_EFI_ERROR (Status);

  Attributes = GcdMemorySpaceDescriptor.Attributes | EFI_MEMORY_RUNTIME | EFI_MEMORY_UC;

  Status = gDS->SetMemorySpaceAttributes (
                  mAcpiMmioBase,
                  Length,
                  Attributes
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Clean up all SMI status and enable bits
  //
  // Clear all SmiControl registers
  SmmControlData32 = 0;
  for (i = FCH_SMI_REGA0; i <= FCH_SMI_REGC4; i += 4) {
    LibFchMemWrite (EfiPciWidthUint32, mAcpiMmioBase + SMI_BASE + i, &SmmControlData32);
  }

  // Clear all SmiStatus registers (SmiStatus0-4)
  SmmControlData32 = 0xFFFFFFFF;
  LibFchMemWrite (EfiPciWidthUint32, mAcpiMmioBase + SMI_BASE + FCH_SMI_REG80, &SmmControlData32);
  LibFchMemWrite (EfiPciWidthUint32, mAcpiMmioBase + SMI_BASE + FCH_SMI_REG84, &SmmControlData32);
  LibFchMemWrite (EfiPciWidthUint32, mAcpiMmioBase + SMI_BASE + FCH_SMI_REG88, &SmmControlData32);
  LibFchMemWrite (EfiPciWidthUint32, mAcpiMmioBase + SMI_BASE + FCH_SMI_REG8C, &SmmControlData32);
  LibFchMemWrite (EfiPciWidthUint32, mAcpiMmioBase + SMI_BASE + FCH_SMI_REG90, &SmmControlData32);


  //
  // If SCI is not enabled, clean up all ACPI PM status/enable registers
  //
  LibFchIoRead (EfiPciWidthUint16, mAcpiPmBase + R_FCH_ACPI_PM_CONTROL, &SmmControlData16);
  if (! (SmmControlData16 & BIT0)) {
    // Clear WAKE_EN, RTC_EN, SLPBTN_EN, PWRBTN_EN, GBL_EN and TMR_EN
    SmmControlData16 = 0;
    SmmControlMask16 = ~(BIT15 + BIT10 + BIT9 + BIT8 + BIT5 + BIT0);
    LibFchIoRw (EfiPciWidthUint16, mAcpiPmBase + R_FCH_ACPI_PM1_ENABLE, &SmmControlMask16, &SmmControlData16);

    // Clear WAKE_STS, RTC_STS, SLPBTN_STS, PWRBTN_STS, GBL_STS and TMR_STS
    SmmControlData16 = BIT15 + BIT10 + BIT9 + BIT8 + BIT5 + BIT0;
    LibFchIoWrite (EfiPciWidthUint16, mAcpiPmBase + R_FCH_ACPI_PM1_STATUS, &SmmControlData16);

    // Clear SLP_TYPx
    SmmControlData16 = 0;
    SmmControlMask16 = ~(BIT12 + BIT11 + BIT10);
    LibFchIoRw (EfiPciWidthUint16, mAcpiPmBase + R_FCH_ACPI_PM_CONTROL, &SmmControlMask16, &SmmControlData16);

    // Clear GPE0 Enable Register
    SmmControlData32 = 0;
    LibFchIoWrite (EfiPciWidthUint32, mAcpiPmBase + R_FCH_ACPI_EVENT_ENABLE, &SmmControlData32);

    // Clear GPE0 Status Register
    SmmControlData32 = 0xFFFFFFFF;
    LibFchIoWrite (EfiPciWidthUint32, mAcpiPmBase + R_FCH_ACPI_EVENT_STATUS, &SmmControlData32);
  }
  return EFI_SUCCESS;
}


EFI_STATUS
ClearSmi (
  VOID
  )
{
  UINT32              SmmControlData32;

  //
  // Clear SmiCmdPort Status Bit
  //
  SmmControlData32 = BIT11;
  MemWrite32 (mAcpiMmioBase64 + SMI_BASE + FCH_SMI_REG88, SmmControlData32);


  //
  // Set the EOS Bit
  //
  SmmControlData32 = MemRead32 (mAcpiMmioBase64 + SMI_BASE + FCH_SMI_REG98);
  SmmControlData32 |= BIT28;
  MemWrite32 (mAcpiMmioBase64 + SMI_BASE + FCH_SMI_REG98, SmmControlData32);

  return EFI_SUCCESS;
}




// Invoke SMI activation from either preboot or runtime environment
EFI_STATUS
AmdTrigger (
  IN       EFI_SMM_CONTROL_PROTOCOL   *This,
  IN OUT   INT8                       *ArgumentBuffer OPTIONAL,
  IN OUT   UINTN                      *ArgumentBufferSize OPTIONAL,
  IN       BOOLEAN                    Periodic OPTIONAL,
  IN       UINTN                      ActivationInterval OPTIONAL
  )
{
  UINT8                bIndex;
  UINT8                bData;
  UINT32               SmmControlData32;
  UINT8                bIrqMask;
  EFI_STATUS           Status;

  if (Periodic || ((NULL != ArgumentBuffer) && (NULL == ArgumentBufferSize))) {
    return EFI_INVALID_PARAMETER;
  }
  if (NULL == ArgumentBuffer) {
    bIndex = 0xff;
    bData = 0xff;
  } else {
    bIndex = ArgumentBuffer[0];
    bData = ArgumentBuffer[1];
  }

  //
  // Clear any pending CmdPort SMI
  //
  Status = ClearSmi ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Enable CmdPort SMI
  //
  SmmControlData32 = MemRead32 (mAcpiMmioBase64 + SMI_BASE + FCH_SMI_REGB2);
  SmmControlData32 &= ~(BIT7 + BIT6);
  SmmControlData32 |= BIT6;
  MemWrite32 (mAcpiMmioBase64 + SMI_BASE + FCH_SMI_REGB2, SmmControlData32);

  //
  // Clear SmiEnB to enable SMI function
  //
  SmmControlData32 = MemRead32 (mAcpiMmioBase64 + SMI_BASE + FCH_SMI_REG98);
  SmmControlData32 &= ~BIT31;
  MemWrite32 (mAcpiMmioBase64 + SMI_BASE + FCH_SMI_REG98, SmmControlData32);

  // Temporal WA-Ensure IRQ0 is masked off when entering software SMI
  bIrqMask = IoRead8 (FCH_8259_MASK_REG_MASTER);
  if (!(bIrqMask & BIT0)) {
    IoWrite8 (FCH_8259_MASK_REG_MASTER, 0xff);
  }
  //-WA

  //
  // Issue command port SMI
  //
  if (*ArgumentBufferSize == 2) {
    IoWrite16 (mAcpiSmiCmd, (bData << 8) + bIndex);
  } else {
    IoWrite8 (mAcpiSmiCmd, bIndex);
  }

  // WA-Restore IRQ0 mask if needed
  if (!(bIrqMask & BIT0)) {
    IoWrite8 (FCH_8259_MASK_REG_MASTER, bIrqMask);
  }
  //-WA

  return EFI_SUCCESS;
}


// Clear any system state that was created in response to the Trigger call
EFI_STATUS
AmdClear (
  IN      EFI_SMM_CONTROL_PROTOCOL   *This,
  IN      BOOLEAN                    Periodic OPTIONAL
  )
{
  if (Periodic) {
    return EFI_INVALID_PARAMETER;
  }

  return ClearSmi ();
}


// Provides information on the source register used to generate the SMI
EFI_STATUS
AmdGetRegisterInfo (
  IN       EFI_SMM_CONTROL_PROTOCOL   *This,
  IN OUT   EFI_SMM_CONTROL_REGISTER   *SmiRegister
  )
{
  if (NULL == SmiRegister) {
    return EFI_INVALID_PARAMETER;
  }

  SmiRegister->SmiTriggerRegister  = mAcpiSmiCmd;
  SmiRegister->SmiDataRegister     = mAcpiSmiCmd + 1;

  return EFI_SUCCESS;
}

//
// FUNCTION NAME.
//      VariableVirtualAddressChangeCallBack - Call back function for Virtual Address Change Event.
//
// FUNCTIONAL DESCRIPTION.
//      This function convert the virtual addreess to support runtime access.
//
// ENTRY PARAMETERS.
//      Event           - virtual Address Change Event.
//      Context         - virtual Address Change call back Context.
//
// EXIT PARAMETERS.
//      None.
//
// WARNINGS.
//      None.
//

VOID
EFIAPI
VariableVirtualAddressChangeCallBack (
  IN EFI_EVENT Event,
  IN VOID *Context
  )
{

  EfiConvertPointer (EFI_INTERNAL_POINTER, (VOID **) &mAcpiMmioBase64);
  EfiConvertPointer (EFI_INTERNAL_POINTER, (VOID *) &(mSmmControl.Trigger));
  EfiConvertPointer (EFI_INTERNAL_POINTER, (VOID *) &(mSmmControl.Clear));
  EfiConvertPointer (EFI_INTERNAL_POINTER, (VOID *) &(mSmmControl.GetRegisterInfo));
  return;
} // VariableVirtualAddressChangeCallBack

/*----------------------------------------------------------------------------------------*/
/**
 * Name: AmdInstallSmmControl
 *
 * This DXE driver produces the SMM Control Protocol
 *
 * @param[in]    ImageHandle     Pointer to the image handle
 * @param[in]    SystemTable     Pointer to the EFI system table
 *
 * @retval       EFI_SUCCESS     Driver initialized successfully
 * @retval       EFI_ERROR       Driver initialization failed
 *
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
AmdInstallSmmControl (
  IN       EFI_HANDLE              ImageHandle,
  IN       EFI_SYSTEM_TABLE        *SystemTable
  )
{
  EFI_STATUS                       Status;

  //
  // Initialize EFI library
  //
  Status = LibFchDxeInitialization (SystemTable);
  ASSERT_EFI_ERROR (Status);

  Status = EfiInitializeRuntimeDriverLib (ImageHandle, SystemTable, VariableVirtualAddressChangeCallBack);
  ASSERT_EFI_ERROR (Status);

  Status = SmmControlDxePreInit (ImageHandle);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  mSmmControl.Trigger              = AmdTrigger;
  mSmmControl.Clear                = AmdClear;
  mSmmControl.GetRegisterInfo      = AmdGetRegisterInfo;
  mSmmControl.MinimumTriggerPeriod = 0;


  //
  // Finally install the protocol
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gEfiSmmControlProtocolGuid,
                  &mSmmControl,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return (Status);
}

