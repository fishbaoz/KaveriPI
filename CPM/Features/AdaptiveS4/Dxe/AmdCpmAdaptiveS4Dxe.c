/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM Adaptive S4 Initialization
 *
 * Contains CPM code to perform Adaptive S4 initialization under DXE
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  AdaptiveS4
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

#include <AmdCpmDxe.h>


EFI_GUID ADAPTIVE_S4_ACPI_DEVICE_GUID = {
  0xa8080674, 0xe53c, 0x4ddf, 0x9f, 0xf8, 0xba, 0x21, 0xf7, 0xa6, 0x99, 0x92
};

VOID
EFIAPI
InvokeAmdAdaptiveS4InitMid (
  IN      EFI_EVENT               Event,
  IN      VOID                    *Context
  );

VOID
EFIAPI
InvokeAmdAdaptiveS4InitLate (
  IN      EFI_EVENT       Event,
  IN      VOID            *Context
  );


/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM Adaptive S4 DXE driver
 *
 * This function stores the setting of Adaptive S4 in NV Data and installs Adaptive
 * S4 SSDT Table.
 *
 * @param[in]     ImageHandle    Pointer to the firmware file system header
 * @param[in]     SystemTable    Pointer to System table
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */
EFI_STATUS
EFIAPI
AmdCpmAdaptiveS4DxeEntryPoint (
  IN      EFI_HANDLE              ImageHandle,
  IN      EFI_SYSTEM_TABLE        *SystemTable
  )
{
  EFI_STATUS                      Status;
  EFI_EVENT                       ReadyToBootEvent;
  EFI_EVENT                       AdaptiveS4DisplayOff;
  VOID                            *mAdaptiveS4DisplayOff;

  //
  // Initialize Global Variable
  //
  CpmInitializeDriverLib (ImageHandle, SystemTable);

  Status = gBS->CreateEvent (
                  CPM_EVENT_NOTIFY_SIGNAL,
                  CPM_TPL_NOTIFY,
                  InvokeAmdAdaptiveS4InitMid,
                  NULL,
                  &AdaptiveS4DisplayOff
                  );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->RegisterProtocolNotify (
                  &gAmdCpmAllPciIoProtocolsInstalledProtocolGuid,
                  AdaptiveS4DisplayOff,
                  &mAdaptiveS4DisplayOff
                  );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->CreateEventEx (
                  CPM_EVENT_NOTIFY_SIGNAL,
                  CPM_TPL_NOTIFY,
                  InvokeAmdAdaptiveS4InitLate,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &ReadyToBootEvent
                  );

  return Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * The function to turn off display in Adaptive S4.
 *
 * @param[in]     Event          EFI_EVENT
 * @param[in]     Context        The Parameter Buffer
 *
 */

VOID
EFIAPI
InvokeAmdAdaptiveS4InitMid (
  IN      EFI_EVENT               Event,
  IN      VOID                    *Context
  )
{
  STATIC BOOLEAN                  InitlateInvoked = FALSE;
  AMD_CPM_TABLE_PROTOCOL          *CpmTableProtocolPtr;
  AMD_CPM_ADAPTIVE_S4_TABLE       *CpmAdaptiveS4TablePtr;
  AMD_CPM_MAIN_TABLE              *CpmMainTablePtr;
  AMD_CPM_DEVICE_PATH_TABLE       *DevicePathTablePtr;
  AMD_CPM_SAVE_CONTEXT_TABLE      *SaveContextTablePtr;
  EFI_STATUS                      Status;
  UINT32                          Offset;
  UINT8                           BufferSignature0;
  UINT8                           BufferSignature1;
  UINT8                           BufferSignature2;
  UINT8                           BufferSignature3;
  UINT8                           AdativeS4Flag;
  UINT8                           Data;
  UINT8                           EcOffset;
  UINT16                          GfxVid;
  UINT8                           i;
  UINT8                           bus;
  UINT16                          BaseAddr;
  UINT8                           BootMode;

  AdativeS4Flag = 0;

  if (!InitlateInvoked) {

    Status = gBS->LocateProtocol (&gAmdCpmTableProtocolGuid, NULL, &CpmTableProtocolPtr);
    if (EFI_ERROR (Status)) {
      return;
    }

    CpmTableProtocolPtr->CommonFunction.PostCode (0xD20B);

    CpmMainTablePtr         = CpmTableProtocolPtr->MainTablePtr;
    CpmAdaptiveS4TablePtr   = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_ADAPTIVE_S4);
    DevicePathTablePtr      = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_DEVICE_PATH);
    SaveContextTablePtr     = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_SAVE_CONTEXT);
    if (SaveContextTablePtr && CpmAdaptiveS4TablePtr && CpmMainTablePtr->AdaptiveS4En) {
      BootMode = CpmTableProtocolPtr->CommonFunction.GetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, BootMode));
      if (BootMode == CPM_BOOT_MODE_S4) {
        if (CpmMainTablePtr->AdaptiveS4En == 2) {
          Offset = CpmMainTablePtr->AcpiMemIoBaseAddr + 0x100 * CpmAdaptiveS4TablePtr->BufferType + CpmAdaptiveS4TablePtr->BufferOffset;
          BufferSignature0 = CpmTableProtocolPtr->CommonFunction.MmioRead8 (Offset + 0x00);
          BufferSignature1 = CpmTableProtocolPtr->CommonFunction.MmioRead8 (Offset + 0x01);
          BufferSignature2 = CpmTableProtocolPtr->CommonFunction.MmioRead8 (Offset + 0x02);
          BufferSignature3 = CpmTableProtocolPtr->CommonFunction.MmioRead8 (Offset + 0x03);
          if (BufferSignature3 == 'A' && BufferSignature2 == 'S' && BufferSignature1 == '3' && BufferSignature0 == 'P') {
            AdativeS4Flag = 1;
          }
        } else if (CpmMainTablePtr->AdaptiveS4En == 1) {
          EcOffset = CpmAdaptiveS4TablePtr->EcRamOffset + 0x04;
          Status = CpmTableProtocolPtr->CommonFunction.KbcRead (0x62, 0x80, &EcOffset, &Data);
          if (EFI_ERROR (Status)) {
            return;
          }
          AdativeS4Flag = Data & BIT0;
        }
        if (AdativeS4Flag) {
          for (i = 0; i < AMD_DISPLAY_DEVICE_PATH_SIZE; i++) {
            if (!DevicePathTablePtr->Path[i].FeatureMask.Mask.Valid) {
              break;
            }
            if (DevicePathTablePtr->Path[i].Bridge.Device == 0) {
              bus = 0;
              GfxVid = CpmTableProtocolPtr->CommonFunction.PciRead16 (
                                  CpmTableProtocolPtr,
                                  bus,
                                  DevicePathTablePtr->Path[i].Device.Device,
                                  DevicePathTablePtr->Path[i].Device.Function,
                                  0x00    // PCI_VENDOR_ID_OFFSET
                                  );
            } else {
              bus = CpmTableProtocolPtr->CommonFunction.PciRead8 (
                                  CpmTableProtocolPtr,
                                  0,
                                  DevicePathTablePtr->Path[i].Bridge.Device,
                                  DevicePathTablePtr->Path[i].Bridge.Function,
                                  0x19    // PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET
                                  );
              GfxVid = CpmTableProtocolPtr->CommonFunction.PciRead16 (
                                  CpmTableProtocolPtr,
                                  bus,
                                  DevicePathTablePtr->Path[i].Device.Device,
                                  DevicePathTablePtr->Path[i].Device.Function,
                                  0x00    // PCI_VENDOR_ID_OFFSET
                                  );
            }
            if (GfxVid != 0xFFFF && GfxVid != 0x0000) {
              if (CpmTableProtocolPtr->CommonFunction.PciRead8 (
                                  CpmTableProtocolPtr,
                                  bus,
                                  DevicePathTablePtr->Path[i].Device.Device,
                                  DevicePathTablePtr->Path[i].Device.Function,
                                  0x0B    // R_BASE_CODE
                                  ) == 3) {
                BaseAddr = CpmTableProtocolPtr->CommonFunction.PciRead16 (
                                  CpmTableProtocolPtr,
                                  bus,
                                  DevicePathTablePtr->Path[i].Device.Device,
                                  DevicePathTablePtr->Path[i].Device.Function,
                                  0x20    // R_BASE_CODE
                                  ) & 0xFFF0;
                CpmTableProtocolPtr->CommonFunction.IoWrite32 (BaseAddr, 0x172C);
                CpmTableProtocolPtr->CommonFunction.IoWrite32 (BaseAddr + 4, BIT16 | CpmTableProtocolPtr->CommonFunction.IoRead32 (BaseAddr + 4));
              }
            }
          }
        }
      }
    }
    CpmTableProtocolPtr->CommonFunction.PostCode (0xD30B);
  }

  InitlateInvoked = TRUE;

  return;
}

BOOLEAN
EFIAPI
CpmAdaptiveS4SsdtCallBack (
  IN      AMD_CPM_TABLE_PROTOCOL  *This,
  IN      VOID                    *AmlObjPtr,
  IN      VOID                    *Context
  )
{
  UINT32  *BufferPtr;

  BufferPtr = (UINT32*)Context;

  switch ( *((UINT32*)AmlObjPtr)) {
  case (CPM_SIGNATURE_32 ('E', 'C', '0', '_')):
    if (*BufferPtr) {
      *(UINT32*) AmlObjPtr = *BufferPtr;
    }
    break;
  }
  return FALSE;
}

/*----------------------------------------------------------------------------------------*/
/**
 * The function to load AMD CPM Adaptive S4 SSDT table.
 *
 * This function gets called each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Event          EFI_EVENT
 * @param[in]     Context        The Parameter Buffer
 *
 */

VOID
EFIAPI
InvokeAmdAdaptiveS4InitLate (
  IN      EFI_EVENT               Event,
  IN      VOID                    *Context
  )
{
  STATIC BOOLEAN                  InitlateInvoked = FALSE;
  AMD_CPM_TABLE_PROTOCOL          *CpmTableProtocolPtr;
  AMD_CPM_ADAPTIVE_S4_TABLE       *CpmAdaptiveS4TablePtr;
  AMD_CPM_MAIN_TABLE              *CpmMainTablePtr;
  EFI_STATUS                      Status;
  UINT32                          Buffer;

  RECORD_TIME_DXE (BeginAmdCpmAdaptiveS4DxeDriver);

  if (!InitlateInvoked) {

    Status = gBS->LocateProtocol (&gAmdCpmTableProtocolGuid, NULL, &CpmTableProtocolPtr);
    if (EFI_ERROR (Status)) {
      return;
    }

    CpmTableProtocolPtr->CommonFunction.PostCode (0xD00B);

    CpmMainTablePtr         = CpmTableProtocolPtr->MainTablePtr;
    CpmAdaptiveS4TablePtr   = (AMD_CPM_ADAPTIVE_S4_TABLE *)CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_ADAPTIVE_S4);
    if (CpmAdaptiveS4TablePtr && CpmMainTablePtr->AdaptiveS4En) {
      Buffer    = CFG_AMD_CPM_ASL_EC_NAME;

      CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  CpmTableProtocolPtr,
                                                          &ADAPTIVE_S4_ACPI_DEVICE_GUID,
                                                          NULL,
                                                          CpmAdaptiveS4SsdtCallBack,
                                                          &Buffer
                                                       );
    }
    CpmTableProtocolPtr->CommonFunction.PostCode (0xD10B);
  }

  InitlateInvoked = TRUE;

  RECORD_TIME_DXE (EndAmdCpmAdaptiveS4DxeDriver);

  return;
}
