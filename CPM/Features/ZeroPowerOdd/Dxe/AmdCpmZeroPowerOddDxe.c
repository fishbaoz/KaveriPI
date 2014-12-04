/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM Zero Power Odd Initialization
 *
 * Contains CPM code to perform Zero Power Odd initialization under DXE
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  ZeroPowerOdd
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

EFI_GUID ZERO_POWER_ODD_ACPI_DEVICE_GUID = {
  0xb7951665, 0x0280, 0x487a, 0x83, 0xb6, 0x36, 0x27, 0x1d, 0x4c, 0x3a, 0x90
};

VOID
EFIAPI
InvokeAmdZeroPowerOddInitLate (
  IN      EFI_EVENT       Event,
  IN      VOID            *Context
  );

BOOLEAN
EFIAPI
CpmZeroPowerOddCallBack (
  IN      AMD_CPM_TABLE_PROTOCOL  *This,
  IN      VOID                    *AmlObjPtr,
  IN      VOID                    *Context
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM Zero Power Oddd DXE driver
 *
 * This function stores the setting of Zero Power Odd in NV Data and loads, updates
 * and installs Zero Power Odd SSDT Table.
 *
 * @param[in]     ImageHandle    Pointer to the firmware file system header
 * @param[in]     SystemTable    Pointer to System table
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */
EFI_STATUS
EFIAPI
AmdCpmZeroPowerOddDxeEntryPoint (
  IN      EFI_HANDLE              ImageHandle,
  IN      EFI_SYSTEM_TABLE        *SystemTable
  )
{
  EFI_STATUS                      Status;
  EFI_EVENT                       ReadyToBootEvent;
  //
  // Initialize Global Variable
  //
  CpmInitializeDriverLib (ImageHandle, SystemTable);

  Status = gBS->CreateEventEx (
                  CPM_EVENT_NOTIFY_SIGNAL,
                  CPM_TPL_CALLBACK,
                  InvokeAmdZeroPowerOddInitLate,
                  NULL,
                  &gEfiEventReadyToBootGuid,
                  &ReadyToBootEvent
                  );

  return Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * The function to load AMD CPM Zero Power Odd SSDT table.
 *
 * This function gets called each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Event          EFI_EVENT
 * @param[in]     Context        The Parameter Buffer
 *
 */
VOID
EFIAPI
InvokeAmdZeroPowerOddInitLate (
  IN      EFI_EVENT               Event,
  IN      VOID                    *Context
  )
{
  STATIC BOOLEAN                  InitlateInvoked = FALSE;
  AMD_CPM_TABLE_PROTOCOL          *CpmTableProtocolPtr;
  AMD_CPM_ZERO_POWER_ODD_TABLE    *CpmZeroPowerOddTablePtr;
  AMD_CPM_GPIO_DEVICE_POWER_TABLE *CpmPowerTablePtr;
  AMD_CPM_MAIN_TABLE              *CpmMainTablePtr;
  EFI_STATUS                      Status;
  UINT16                          Buffer[5];
  UINT8                           SciMap1;
  UINT8                           SciMap2;

  RECORD_TIME_DXE (BeginAmdCpmZeroPowerOddDxeDriver);

  if (!InitlateInvoked) {

    Status = gBS->LocateProtocol (&gAmdCpmTableProtocolGuid, NULL, &CpmTableProtocolPtr);
    if (EFI_ERROR (Status)) {
      return;
    }

    CpmTableProtocolPtr->CommonFunction.PostCode (0xD00A);

    CpmMainTablePtr         = CpmTableProtocolPtr->MainTablePtr;
    CpmZeroPowerOddTablePtr = (AMD_CPM_ZERO_POWER_ODD_TABLE *)CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_ZERO_POWER_ODD);
    if (CpmZeroPowerOddTablePtr) {
      CpmPowerTablePtr        = (AMD_CPM_GPIO_DEVICE_POWER_TABLE*)CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_GPIO_DEVICE_POWER);
      if (CpmMainTablePtr->ZeroPowerOddEn & BIT0) {
        if (!(CpmZeroPowerOddTablePtr->SataModeSupportMask & CpmTableProtocolPtr->CommonFunction.GetSataMode (CpmTableProtocolPtr))) {
          CpmMainTablePtr->ZeroPowerOddEn &= 0xF6;
        }
      } else {
        CpmMainTablePtr->ZeroPowerOddEn &= 0xF6;
      }

      SciMap1 = CpmTableProtocolPtr->CommonFunction.GetSciMap (CpmTableProtocolPtr, CpmZeroPowerOddTablePtr->EventPin1);
      SciMap2 = CpmTableProtocolPtr->CommonFunction.GetSciMap (CpmTableProtocolPtr, CpmZeroPowerOddTablePtr->EventPin2);

      if (CpmMainTablePtr->ZeroPowerOddEn & BIT0) {
        if ((SciMap1 & 0xF) < 0xA) {
          Buffer[0] = 0x3000 + (((SciMap1) & 0xF) << 8);
        } else {
          Buffer[0] = 0x4000 + ((((SciMap1) & 0xF) - 0x9) << 8);
        }
        if (((SciMap1 >> 4) & 0xF) < 0xA) {
          Buffer[0] |= 0x30 + ((SciMap1 >> 4) & 0xF);
        } else {
          Buffer[0] |= 0x40 + (((SciMap1 >> 4) & 0xF) - 0x9);
        }
      } else {
        Buffer[0] = 0;
      }
      if (CpmMainTablePtr->ZeroPowerOddEn & BIT1) {
        if ((SciMap2 & 0xF) < 0xA) {
          Buffer[1] = 0x3000 + (((SciMap2) & 0xF) << 8);
        } else {
          Buffer[1] = 0x4000 + ((((SciMap2) & 0xF) - 0x9) << 8);
        }
        if (((SciMap2 >> 4) & 0xF) < 0xA) {
          Buffer[1] |= 0x30 + ((SciMap2 >> 4) & 0xF);
        } else {
          Buffer[1] |= 0x40 + (((SciMap2 >> 4) & 0xF) - 0x9);
        }
      } else {
        Buffer[1] = 0;
      }
      Buffer[2] = (UINT16) CpmZeroPowerOddTablePtr->SataPortId;
      Buffer[3] = SciMap1;
      if (CpmMainTablePtr->ZeroPowerOddEn & BIT3) {
        Buffer[4] = 1;
      } else {
        Buffer[4] = 0;
      }

      CpmTableProtocolPtr->CommonFunction.AddSsdtTable (  CpmTableProtocolPtr,
                                                          &ZERO_POWER_ODD_ACPI_DEVICE_GUID,
                                                          NULL,
                                                          CpmZeroPowerOddCallBack,
                                                          &Buffer[0]
                                                       );
    }
    CpmTableProtocolPtr->CommonFunction.PostCode (0xD10A);
  }

  InitlateInvoked = TRUE;

  RECORD_TIME_DXE (EndAmdCpmZeroPowerOddDxeDriver);

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback function to override Zero Power Odd SSDT Table
 *
 * This function is used to update GPE numbers and SATA port number.
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
CpmZeroPowerOddCallBack (
  IN      AMD_CPM_TABLE_PROTOCOL  *This,
  IN      VOID                    *AmlObjPtr,
  IN      VOID                    *Context
  )
{
  UINT16  *BufferPtr;
  BufferPtr = (UINT16*)Context;

  switch (*((UINT32*)AmlObjPtr)) {
  case (CPM_SIGNATURE_32 ('X', 'L', '0', '6')):
    if (*BufferPtr) {
      *(UINT8*)AmlObjPtr = '_';
      *(UINT16 *) ((UINT8*)AmlObjPtr + 2) = *BufferPtr;
    }
    break;
  case (CPM_SIGNATURE_32 ('X', 'L', '1', '0')):
    if (*(BufferPtr + 1)) {
      *(UINT8*)AmlObjPtr = '_';
      *(UINT16 *) ((UINT8*)AmlObjPtr + 2) = *(BufferPtr + 1);
    }
    break;
  case (CPM_SIGNATURE_32 ('Z', 'P', 'R', 'W')):
    if (*(BufferPtr + 3)) {
      if (*((UINT32*)AmlObjPtr + 1) == 0x0A020612) {
        *((UINT8*)AmlObjPtr + 8) = *(UINT8*)(BufferPtr + 3);
      }
    }
    break;
  case (CPM_SIGNATURE_32 ('X', 'P', 'R', 'W')):
    if (*(BufferPtr + 4)) {
      *(UINT8*)AmlObjPtr = '_';
    }
    break;
  case (CPM_SIGNATURE_32 ('X', 'E', 'J', '0')):
    if (*(BufferPtr + 1)) {
      *(UINT8*)AmlObjPtr = '_';
    }
    break;
  case (CPM_SIGNATURE_32 ('P', 'R', 'T', '1')):
    *((UINT8 *)AmlObjPtr + 3) = '0' + *(UINT8*)(BufferPtr + 2);
    break;
  }

  return FALSE;
}
