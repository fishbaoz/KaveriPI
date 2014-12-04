/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM PCIE Initialization
 *
 * Contains CPM code to perform PCIE initialization under DXE
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  PcieInit
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

AMD_CPM_TABLE_PROTOCOL          *CpmTableProtocolPtr;

EFI_GUID EXPRESS_CARD_ACPI_DEVICE_GUID = {
  0x5f26ca84, 0x837d, 0x4c3e, 0x9d, 0x76, 0xb6, 0xa7, 0x88, 0xb7, 0x50, 0x57
};

VOID
EFIAPI
InvokeAmdExpressCardInitLate (
  IN      EFI_EVENT           Event,
  IN      VOID                *Context
  );


/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM PCIE Init DXE driver
 *
 * This function loads, overrides and installs Express Card SSDT table.
 *
 * @param[in]     ImageHandle    Pointer to the firmware file system header
 * @param[in]     SystemTable    Pointer to System table
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */

EFI_STATUS
EFIAPI
AmdCpmPcieInitDxeEntryPoint (
  IN      EFI_HANDLE                      ImageHandle,
  IN      EFI_SYSTEM_TABLE                *SystemTable
  )
{
  EFI_STATUS                              Status;
  EFI_EVENT                               ReadyToBootEvent;

  //
  // Initialize Global Variable
  //
  CpmInitializeDriverLib (ImageHandle, SystemTable);

  Status = gBS->CreateEventEx (
                              CPM_EVENT_NOTIFY_SIGNAL,
                              CPM_TPL_NOTIFY,
                              InvokeAmdExpressCardInitLate,
                              NULL,
                              &gEfiEventReadyToBootGuid,
                              &ReadyToBootEvent
                              );

  return Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Callback function to update Express Card SSDT table
 *
 * This function is used to update GPE number and PCIE Bridge Name
 *
 * @param[in]     This           Pointer to Protocol
 * @param[in]     AmlObjPtr      The AML Object Buffer
 * @param[in]     Context        The Parameter Buffer
 *
 * @retval        TRUE           SSDT Table has been updated completely
 * @retval        FALSE          SSDT Table has not been updated completely
 */

BOOLEAN
EFIAPI
CpmExpressCardSsdtCallBack (
  IN      AMD_CPM_TABLE_PROTOCOL  *This,
  IN      VOID                    *AmlObjPtr,
  IN      VOID                    *Context
  )
{
  UINT32  *BufferPtr;

  BufferPtr = (UINT32*)Context;

  switch ( *((UINT32*)AmlObjPtr)) {
  case (CPM_SIGNATURE_32 ('M', '1', '2', '9')):     // CpmExpressCardEventMethod: M129
    if (*(BufferPtr )) {
      *(UINT32*) AmlObjPtr = *(BufferPtr);
    }
    break;
  case (CPM_SIGNATURE_32 ('M', '1', '3', '0')):     // CpmExpressCardBridge: M130
    if (*(BufferPtr + 1)) {
      *(UINT32*) AmlObjPtr = *(BufferPtr + 1);
    }
    break;
  case (CPM_SIGNATURE_32 ('M', '1', '2', '1')):     // CpmWirelessButtonEventMethod: M121
    if (*(BufferPtr + 2)) {
      *(UINT32*) AmlObjPtr = *(BufferPtr + 2);
    }
    break;
  case (CPM_SIGNATURE_32 ('M', '1', '2', '3')):     // CpmWirelessBridge0: M123
    if (*(BufferPtr + 3)) {
      *(UINT32*) AmlObjPtr = *(BufferPtr + 3);
    }
    break;
  case (CPM_SIGNATURE_32 ('M', '1', '2', '4')):     // CpmWirelessBridge1: M124
    if (*(BufferPtr + 4)) {
      *(UINT32*) AmlObjPtr = *(BufferPtr + 4);
    }
    break;
  case (CPM_SIGNATURE_32 ('M', '1', '2', '5')):     // CpmWirelessBridge2: M125
    if (*(BufferPtr + 5)) {
      *(UINT32*) AmlObjPtr = *(BufferPtr + 5);
    }
    break;
  case (CPM_SIGNATURE_32 ('M', '1', '2', '6')):     // CpmWirelessBridge3: M126
    if (*(BufferPtr + 6)) {
      *(UINT32*) AmlObjPtr = *(BufferPtr + 6);
    }
    break;
  case (CPM_SIGNATURE_32 ('M', '1', '2', '7')):     // CpmWirelessButtonCurrentStatus: M127
    if (*(BufferPtr + 7)) {
      *(((UINT8*) AmlObjPtr) + 4) = 1;
      *(BufferPtr + 7) = 0;
    }
    break;
  }
  return FALSE;
}


/*----------------------------------------------------------------------------------------*/
/**
 * The function to load, override and install AMD CPM Express Card SSDT table.
 *
 * This function gets called each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Event          EFI_EVENT
 * @param[in]     Context        The Parameter Buffer
 *
 */

VOID
EFIAPI
InvokeAmdExpressCardInitLate (
  IN      EFI_EVENT               Event,
  IN      VOID                    *Context
  )
{
  AMD_CPM_NV_DATA_PROTOCOL        *CpmNvDataProtocolPtr;
  AMD_CPM_TABLE_PROTOCOL          *CpmTableProtocolPtr;
  AMD_CPM_SAVE_CONTEXT_TABLE      *SaveContextTablePtr;
  AMD_CPM_EXPRESS_CARD_TABLE      *ExpressCardTablePtr;
  AMD_CPM_PCIE_TOPOLOGY_TABLE     *PcieTopologyTablePtr;
  AMD_CPM_WIRELESS_BUTTON_TABLE   *WirelessButtonTablePtr;
  STATIC BOOLEAN                  InitlateInvoked = FALSE;
  EFI_STATUS                      Status;
  UINT32                          Buffer[8];
  UINT8                           SciMap;
  UINT8                           Index;
  UINT32                          PCIeStatus;
  UINT32                          DeviceVendoreId;
  UINT32                          IsFchBridge;

  RECORD_TIME_DXE (BeginAmdCpmPcieInitDxeDriver);

  if (!InitlateInvoked) {

    Status = gBS->LocateProtocol (  &gAmdCpmTableProtocolGuid,
                                    NULL,
                                    &CpmTableProtocolPtr
                                    );
    if (EFI_ERROR (Status)) {
      return;
    }

    CpmTableProtocolPtr->CommonFunction.PostCode (0xD006);

    Status = gBS->LocateProtocol (  &gAmdCpmNvDataProtocolGuid,
                                    NULL,
                                    &CpmNvDataProtocolPtr
                                    );
    if (EFI_ERROR (Status)) {
      return;
    }

    ExpressCardTablePtr   = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_PCIE_EXPRESS_CARD);
    SaveContextTablePtr   = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_SAVE_CONTEXT);
    PcieTopologyTablePtr  = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_PCIE_TOPOLOGY);
    WirelessButtonTablePtr  = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_WIRELESS_BUTTON);

    if (SaveContextTablePtr && PcieTopologyTablePtr) {
      PCIeStatus = 0;
      for ( Index = 0; Index < AMD_PCIE_PORT_DESCRIPTOR_SIZE; Index ++ ) {
        if ((PcieTopologyTablePtr->Port[Index].EngineData.EngineType == PciePortEngine) && (PcieTopologyTablePtr->Port[Index].Port.PortPresent)) {
          DeviceVendoreId =  CpmTableProtocolPtr->CommonFunction.PciRead32 (
                                CpmTableProtocolPtr,
                                0x00,
                                PcieTopologyTablePtr->Port[Index].Port.DeviceNumber,
                                PcieTopologyTablePtr->Port[Index].Port.FunctionNumber,
                                0x00
                                );
          if (DeviceVendoreId != 0 && DeviceVendoreId != 0xFFFFFFFF) {
            PCIeStatus |= 1 << Index;
          }
          if (CpmTableProtocolPtr->CommonFunction.IsUmi (CpmTableProtocolPtr, PcieTopologyTablePtr->Port[Index].Port.DeviceNumber, PcieTopologyTablePtr->Port[Index].Port.FunctionNumber)) {
            PCIeStatus |= 1 << Index;
          }
        }
        if (PcieTopologyTablePtr->Port[Index].Flags == DESCRIPTOR_TERMINATE_LIST) {
          break;
        }
      }

      CpmTableProtocolPtr->CommonFunction.SetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieDeviceStatus) + 0x00, (UINT8) (PCIeStatus));
      CpmTableProtocolPtr->CommonFunction.SetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieDeviceStatus) + 0x01, (UINT8) (PCIeStatus >> 8));
      CpmTableProtocolPtr->CommonFunction.SetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieDeviceStatus) + 0x02, (UINT8) (PCIeStatus >> 16));
      CpmTableProtocolPtr->CommonFunction.SetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieDeviceStatus) + 0x03, (UINT8) (PCIeStatus >> 24));
    }

    if (ExpressCardTablePtr) {
      SciMap = CpmTableProtocolPtr->CommonFunction.GetSciMap (CpmTableProtocolPtr, ExpressCardTablePtr->EventPin);

      Buffer[0] = 'L_';
      if ((SciMap & 0xF) < 0xA) {
        Buffer[0] |= (0x30 + ((SciMap) & 0xF)) << 24;
      } else {
        Buffer[0] |= (0x40 + (((SciMap) & 0xF) - 0x9)) << 24;
      }
      if (((SciMap >> 4) & 0xF) < 0xA) {
        Buffer[0] |= (0x30 + ((SciMap >> 4) & 0xF)) << 16;
      } else {
        Buffer[0] |= (0x40 + (((SciMap >> 4) & 0xF) - 0x9)) << 16;
      }
      IsFchBridge = CpmTableProtocolPtr->CommonFunction.IsFchDevice (
                                                          CpmTableProtocolPtr,
                                                          ExpressCardTablePtr->Device,
                                                          ExpressCardTablePtr->Function
                                                          );
      if (IsFchBridge) {
        Buffer[1] = CpmTableProtocolPtr->CommonFunction.GetFchPcieAslName (
                                                          CpmTableProtocolPtr,
                                                          ExpressCardTablePtr->Device,
                                                          ExpressCardTablePtr->Function
                                                          );
      } else {
        Buffer[1] = CpmTableProtocolPtr->CommonFunction.GetPcieAslName (
                                                          CpmTableProtocolPtr,
                                                          ExpressCardTablePtr->Device,
                                                          ExpressCardTablePtr->Function
                                                          );
      }
    } else {
      Buffer[0] = 0;
      Buffer[1] = 0;
    }

    if (WirelessButtonTablePtr && CpmTableProtocolPtr->MainTablePtr->WirelessButtonEn) {
      SciMap = CpmTableProtocolPtr->CommonFunction.GetSciMap (CpmTableProtocolPtr, WirelessButtonTablePtr->EventPin);
      Buffer[2] = 'L_';
      if ((SciMap & 0xF) < 0xA) {
        Buffer[2] |= (0x30 + ((SciMap) & 0xF)) << 24;
      } else {
        Buffer[2] |= (0x40 + (((SciMap) & 0xF) - 0x9)) << 24;
      }
      if (((SciMap >> 4) & 0xF) < 0xA) {
        Buffer[2] |= (0x30 + ((SciMap >> 4) & 0xF)) << 16;
      } else {
        Buffer[2] |= (0x40 + (((SciMap >> 4) & 0xF) - 0x9)) << 16;
      }
      for (Index = 0; Index < 4; Index++) {
        IsFchBridge = CpmTableProtocolPtr->CommonFunction.IsFchDevice (
                                                            CpmTableProtocolPtr,
                                                            WirelessButtonTablePtr->Bridge[Index].Device,
                                                            WirelessButtonTablePtr->Bridge[Index].Function
                                                            );
        if (IsFchBridge) {
          Buffer[3 + Index] = CpmTableProtocolPtr->CommonFunction.GetFchPcieAslName (
                                                            CpmTableProtocolPtr,
                                                            WirelessButtonTablePtr->Bridge[Index].Device,
                                                            WirelessButtonTablePtr->Bridge[Index].Function
                                                            );
        } else {
          Buffer[3 + Index] = CpmTableProtocolPtr->CommonFunction.GetPcieAslName (
                                                            CpmTableProtocolPtr,
                                                            WirelessButtonTablePtr->Bridge[Index].Device,
                                                            WirelessButtonTablePtr->Bridge[Index].Function
                                                            );
        }
      }
      if (SaveContextTablePtr) {
        if (CpmTableProtocolPtr->CommonFunction.GetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, WirelessButtonStatus)) == 0) {
          Buffer[7] = 1;
        } else {
          Buffer[7] = 0;
        }
      }
    } else {
      for (Index = 0; Index < 6; Index++) {
        Buffer[2 + Index] = 0;
      }
    }
    if (ExpressCardTablePtr || (WirelessButtonTablePtr && CpmTableProtocolPtr->MainTablePtr->WirelessButtonEn)) {
      CpmTableProtocolPtr->CommonFunction.AddSsdtTable (
                                                          CpmTableProtocolPtr,
                                                          &EXPRESS_CARD_ACPI_DEVICE_GUID,
                                                          NULL,
                                                          CpmExpressCardSsdtCallBack,
                                                          &Buffer[0]
                                                          );
    }
    CpmTableProtocolPtr->CommonFunction.PostCode (0xD106);
  }
  InitlateInvoked = TRUE;

  RECORD_TIME_DXE (EndAmdCpmPcieInitDxeDriver);

  return;
}
