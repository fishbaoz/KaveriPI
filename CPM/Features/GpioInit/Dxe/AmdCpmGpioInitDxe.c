/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM GPIO Initialization
 *
 * Contains CPM code to perform GPIO Init under DXE
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  GpioInit
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

VOID
EFIAPI
AmdCpmGpioInitMid (
  IN      VOID             *Context
  );

VOID
EFIAPI
AmdCpmGpioInitMidNotifyEvent (
  IN      EFI_EVENT        Event,
  IN      VOID             *Context
  );

VOID
EFIAPI
AmdCpmGpioInitLate (
  IN      EFI_EVENT        Event,
  IN      VOID             *Context
  );

VOID
CpmPcieClockInit (
  IN      AMD_CPM_TABLE_PROTOCOL  *CpmTableProtocolPtr
  );

VOID
CpmGeventInit (
  IN      AMD_CPM_TABLE_PROTOCOL  *CpmTableProtocolPtr
  );

VOID
CpmGeventSmiControlInit (
  IN      AMD_CPM_TABLE_PROTOCOL  *CpmTableProtocolPtr
  );


/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM GPIO Init DXE driver
 *
 * This function sets GEVENT pins and internal PCIe clock.
 *
 * @param[in]     ImageHandle    Pointer to the firmware file system header
 * @param[in]     SystemTable    Pointer to System table
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */

EFI_STATUS
EFIAPI
AmdCpmGpioInitDxeEntryPoint (
  IN      EFI_HANDLE                      ImageHandle,
  IN      EFI_SYSTEM_TABLE                *SystemTable
  )
{
  EFI_STATUS                              Status;
  EFI_EVENT                               ReadyToBootEvent;
  EFI_EVENT                               AfterInitEnvEvent;
  AMD_CPM_TABLE_PROTOCOL                  *CpmTableProtocolPtr;
  VOID                                    *mAfterInitEnvEvent;
  AMD_BUFFER_MANAGER_PROTOCOL             *BufferMgr;

  //
  // Initialize Global Variable
  //
  CpmInitializeDriverLib (ImageHandle, SystemTable);

  Status = gBS->LocateProtocol (  &gAmdCpmTableProtocolGuid,
                                  NULL,
                                  &CpmTableProtocolPtr
                                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  CpmTableProtocolPtr->CommonFunction.PostCode (0xD005);

  //
  // First test gAmdBufferManagerProtocolGuid is installed or not.
  //
  Status = gBS->LocateProtocol (
                  &gAmdBufferManagerProtocolGuid,
                  NULL,
                  &BufferMgr
                  );
  if (EFI_ERROR (Status)) {
    //
    // If gAmdBufferManagerProtocolGuid still not be installed, RegisterProtocolNotify
    //
    Status = gBS->CreateEvent (
                    CPM_EVENT_NOTIFY_SIGNAL,
                    CPM_TPL_NOTIFY,
                    AmdCpmGpioInitMidNotifyEvent,
                    CpmTableProtocolPtr,
                    &AfterInitEnvEvent
                    );

    if (EFI_ERROR (Status)) {
      return Status;
    }

    Status = gBS->RegisterProtocolNotify (
                    &gAmdBufferManagerProtocolGuid,
                    AfterInitEnvEvent,
                    &mAfterInitEnvEvent
                    );

    if (EFI_ERROR (Status)) {
      return Status;
    }
  } else {
    //
    // Already have gAmdBufferManagerProtocolGuid, invoke AmdCpmGpioInitMid()
    //
    AmdCpmGpioInitMid (CpmTableProtocolPtr);
  }

  Status = gBS->CreateEventEx (
                  CPM_EVENT_NOTIFY_SIGNAL,
                  CPM_TPL_NOTIFY,
                  AmdCpmGpioInitLate,
                  (VOID*)CpmTableProtocolPtr,
                  &gEfiEventReadyToBootGuid,
                  &ReadyToBootEvent
                  );

  CpmTableProtocolPtr->CommonFunction.PostCode (0xD105);


  return EFI_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Init internal PCIe clock.
 *
 * This function gets called each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Event          EFI_EVENT
 * @param[in]     Context        The Parameter Buffer
 *
 */

VOID
EFIAPI
AmdCpmGpioInitLate (
  IN      EFI_EVENT        Event,
  IN      VOID             *Context
  )
{
  STATIC BOOLEAN  AmdCpmGpioInitLateInvoked = FALSE;

  RECORD_TIME_DXE (BeginAmdCpmGpioInitDxeDriver);

  if (!AmdCpmGpioInitLateInvoked) {
    CpmGeventSmiControlInit (Context);
    CpmPcieClockInit (Context);
  }

  AmdCpmGpioInitLateInvoked = TRUE;
  RECORD_TIME_DXE (EndAmdCpmGpioInitDxeDriver);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Init GEVENT pins
 *
 * This function gets called each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Context        The Parameter Buffer
 *
 */

VOID
EFIAPI
AmdCpmGpioInitMid (
  IN      VOID             *Context
  )
{
  STATIC BOOLEAN  AmdCpmGpioInitMidInvoked = FALSE;

  RECORD_TIME_DXE (BeginAmdCpmGpioInitDxeDriver);

  if (!AmdCpmGpioInitMidInvoked) {
    CpmGeventInit (Context);
  }

  AmdCpmGpioInitMidInvoked = TRUE;
  RECORD_TIME_DXE (EndAmdCpmGpioInitDxeDriver);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Init GEVENT pins
 *
 * This function gets called each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Event          EFI_EVENT
 * @param[in]     Context        The Parameter Buffer
 *
 */

VOID
EFIAPI
AmdCpmGpioInitMidNotifyEvent (
  IN      EFI_EVENT        Event,
  IN      VOID             *Context
  )
{
  AmdCpmGpioInitMid (Context);
}

VOID
AddOneItemInClkList (
  IN      AMD_CPM_EXT_CLKGEN_TABLE    *ExtClkGenTablePtr,
  IN      AMD_CPM_EXT_CLKGEN_ITEM     *ListPtr,
  IN      UINT8                       *ListNum,
  IN      UINT8                       ClkNum,
  IN      UINT8                       ClkReq
  )
{
  AMD_CPM_EXT_CLKGEN_ITEM     *CurrentItemPtr;
  UINT8   Index;
  UINT8   Index2;
  if (ClkNum == 0xFE) {
    return;
  }
  for (Index = 0; Index < AMD_EXT_CLKGEN_SIZE; Index++) {
    if (ExtClkGenTablePtr->Item[Index].Function == 0xFF) {
      break;
    }
    if (ExtClkGenTablePtr->Item[Index].Function == ClkNum + 0x80 && ClkReq == 0) {
      CurrentItemPtr = &ExtClkGenTablePtr->Item[Index];
    } else if (ExtClkGenTablePtr->Item[Index].Function == ClkReq + 0x8F && ClkReq != 0xFF && ClkReq != 0) {
      CurrentItemPtr = &ExtClkGenTablePtr->Item[Index];
    } else {
      continue;
    }
    for (Index2 = 0; Index2 < (*ListNum); Index2++) {
      if (ListPtr[Index2].Offset == CurrentItemPtr->Offset) {
        ListPtr[Index2].AndMask &= CurrentItemPtr->AndMask;
        ListPtr[Index2].OrMask |= CurrentItemPtr->OrMask;
        break;
      }
    }
    if (Index2 == (*ListNum)) {
      ListPtr[Index2].Function = 0;
      ListPtr[Index2].Offset = CurrentItemPtr->Offset;
      ListPtr[Index2].AndMask = CurrentItemPtr->AndMask;
      ListPtr[Index2].OrMask = CurrentItemPtr->OrMask;
      (*ListNum) ++;
    }
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * The function to initialize internal PCIe clock.
 *
 * @param[in]     CpmTableProtocolPtr Pointer to AMD CPM Table Protocol
 *
 */
VOID
CpmPcieClockInit (
  IN      AMD_CPM_TABLE_PROTOCOL      *CpmTableProtocolPtr
  )
{
  AMD_CPM_PCIE_CLOCK_TABLE            *CpmPcieClockTablePtr;
  AMD_CPM_PCIE_CLOCK_ITEM             *TablePtr;
  UINT8                               SlotStatus;
  BOOLEAN                             ClockType;
  AMD_CPM_EXT_CLKGEN_TABLE            *ExtClkGenTablePtr;
  AMD_CPM_EXT_CLKGEN_ITEM             ClkGen[AMD_PCIE_CLOCK_SIZE];
  AMD_CPM_SAVE_CONTEXT_TABLE          *SaveContextTablePtr;
  UINT8                               Index;
  UINT8                               Data;
  UINT8                               i;
  UINT32                              PcieSlotStatus;

  ExtClkGenTablePtr = NULL;
  Index = 0;
  PcieSlotStatus = 0;
  CpmTableProtocolPtr->CommonFunction.PostCode (0xD205);
  CpmPcieClockTablePtr  = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_PCIE_CLOCK);
  SaveContextTablePtr   = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_SAVE_CONTEXT);
  ClockType             = (BOOLEAN) CpmTableProtocolPtr->CommonFunction.GetStrap (CpmTableProtocolPtr) & BIT1;
  if (!ClockType) {
    ExtClkGenTablePtr   = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_EXT_CLKGEN);
    if (!ExtClkGenTablePtr) {
      return;
    }
  }
  if (CpmPcieClockTablePtr && CpmTableProtocolPtr->MainTablePtr->UnusedGppClkOffEn) {
    TablePtr = &CpmPcieClockTablePtr->Item[0];
    i = 0;
    while (TablePtr->ClkId != 0xFF) {
      SlotStatus = 1;
      if (TablePtr->SlotCheck & BIT1) {
        SlotStatus = CpmTableProtocolPtr->CommonFunction.DetectDevice (CpmTableProtocolPtr, TablePtr->DeviceId, NULL);
      }
      if (SlotStatus && (TablePtr->SlotCheck & BIT0)) {
        SlotStatus = CpmTableProtocolPtr->CommonFunction.CheckPcieDevice (CpmTableProtocolPtr, TablePtr->Device, TablePtr->Function);
        if (SlotStatus) {
          if ((SlotStatus != 1) && (TablePtr->SlotCheck & BIT2)) {
            TablePtr->ClkReq = 0xFF;
            TablePtr->ClkReqExt = 0xFF;
          } else {
            SlotStatus = 1;
          }
        }
        PcieSlotStatus |= SlotStatus << (2 * i);
      }
      if (SlotStatus) {
        if (ClockType) {
          CpmTableProtocolPtr->CommonFunction.SetClkReq (CpmTableProtocolPtr, TablePtr->ClkId, TablePtr->ClkReq);
        } else {
          AddOneItemInClkList (ExtClkGenTablePtr, &ClkGen[0], &Index, TablePtr->ClkIdExt, TablePtr->ClkReqExt);
        }
      } else {
        if (ClockType) {
          CpmTableProtocolPtr->CommonFunction.SetClkReq (CpmTableProtocolPtr, TablePtr->ClkId, 0);
        } else {
          AddOneItemInClkList (ExtClkGenTablePtr, &ClkGen[0], &Index, TablePtr->ClkIdExt, 0);
        }
      }
      TablePtr++;
      i++;
    }

    if (SaveContextTablePtr) {
      if (CpmTableProtocolPtr->CommonFunction.GetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, BootMode)) == CPM_BOOT_MODE_S0) {
        CpmTableProtocolPtr->CommonFunction.SetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieClockSlotStatus) + 0x00, (UINT8) (PcieSlotStatus));
        CpmTableProtocolPtr->CommonFunction.SetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieClockSlotStatus) + 0x01, (UINT8) (PcieSlotStatus >> 8));
        CpmTableProtocolPtr->CommonFunction.SetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieClockSlotStatus) + 0x02, (UINT8) (PcieSlotStatus >> 16));
        CpmTableProtocolPtr->CommonFunction.SetSaveContext (CpmTableProtocolPtr, 0x80 + OFFSET_OF (AMD_CPM_PREDEFINED_SAVE_CONTEXT, PcieClockSlotStatus) + 0x03, (UINT8) (PcieSlotStatus >> 23));
      }
    }
    if (!ClockType) {
      i = 0;
      while (Index > 0) {
        if (CpmTableProtocolPtr->CommonFunction.ReadSmbusBlock) {
          CpmTableProtocolPtr->CommonFunction.ReadSmbusBlock (
                                                CpmTableProtocolPtr,
                                                ExtClkGenTablePtr->SmbusSelect,
                                                ExtClkGenTablePtr->SmbusAddress,
                                                ClkGen[i].Offset,
                                                1,
                                                &Data
                                                );
          Data = (Data & ClkGen[i].AndMask) | ClkGen[i].OrMask;
          CpmTableProtocolPtr->CommonFunction.WriteSmbusBlock (
                                                CpmTableProtocolPtr,
                                                ExtClkGenTablePtr->SmbusSelect,
                                                ExtClkGenTablePtr->SmbusAddress,
                                                ClkGen[i].Offset,
                                                1,
                                                &Data
                                                );
        }
        Index --;
        i ++;
      }
    }
  }

  CpmTableProtocolPtr->CommonFunction.PostCode (0xD305);

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * The function to initialize GEVENT pins.
 *
 * @param[in]     CpmTableProtocolPtr Pointer to AMD CPM Table Protocol
 *
 */
VOID
CpmGeventInit (
  IN      AMD_CPM_TABLE_PROTOCOL  *CpmTableProtocolPtr
  )
{
  AMD_CPM_GEVENT_INIT_TABLE       *GeventTablePtr;
  AMD_CPM_GEVENT_ITEM             *TablePtr;
  AMD_CPM_EXPRESS_CARD_TABLE      *ExpressCardTablePtr;
  AMD_CPM_ZERO_POWER_ODD_TABLE    *ZeroPowerOddTablePtr;

  GeventTablePtr   = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_GEVENT_INIT);
  if (GeventTablePtr) {
    TablePtr = &GeventTablePtr->GeventList[0];
    while (TablePtr->Pin != 0xFF) {
      CpmTableProtocolPtr->CommonFunction.SetGevent (CpmTableProtocolPtr, TablePtr->Pin, TablePtr->Setting.Raw);
      TablePtr++;
    }
  }
  ExpressCardTablePtr = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_PCIE_EXPRESS_CARD);
  if (ExpressCardTablePtr) {
    CpmTableProtocolPtr->CommonFunction.SetGeventSciTrig (
                                                CpmTableProtocolPtr,
                                                ExpressCardTablePtr->EventPin,
                                                CpmTableProtocolPtr->CommonFunction.GetGevent (CpmTableProtocolPtr, ExpressCardTablePtr->EventPin) ? 0 : 1
                                                );
  }
  if (CpmTableProtocolPtr->MainTablePtr->ZeroPowerOddEn & (BIT0 | BIT1)) {
    ZeroPowerOddTablePtr = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_ZERO_POWER_ODD);
    if (ZeroPowerOddTablePtr) {
      if (CpmTableProtocolPtr->MainTablePtr->ZeroPowerOddEn & BIT0) {
        CpmTableProtocolPtr->CommonFunction.SetGeventSciTrig (
                                                CpmTableProtocolPtr,
                                                ZeroPowerOddTablePtr->EventPin1,
                                                CpmTableProtocolPtr->CommonFunction.GetGevent (CpmTableProtocolPtr, ZeroPowerOddTablePtr->EventPin1) ? 0 : 1
                                                );
      }
      if (CpmTableProtocolPtr->MainTablePtr->ZeroPowerOddEn & BIT1) {
        CpmTableProtocolPtr->CommonFunction.SetGeventSciTrig (
                                                CpmTableProtocolPtr,
                                                ZeroPowerOddTablePtr->EventPin2,
                                                CpmTableProtocolPtr->CommonFunction.GetGevent (CpmTableProtocolPtr, ZeroPowerOddTablePtr->EventPin2) ? 0 : 1
                                                );
      }
    }
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * The function to initialize SMI Control for GEVENT pins.
 *
 * @param[in]     CpmTableProtocolPtr Pointer to AMD CPM Table Protocol
 *
 */
VOID
CpmGeventSmiControlInit (
  IN      AMD_CPM_TABLE_PROTOCOL  *CpmTableProtocolPtr
  )
{
  AMD_CPM_GEVENT_INIT_TABLE       *GeventTablePtr;
  AMD_CPM_GEVENT_ITEM             *TablePtr;

  GeventTablePtr   = CpmTableProtocolPtr->CommonFunction.GetTablePtr (CpmTableProtocolPtr, CPM_SIGNATURE_GEVENT_INIT);
  if (GeventTablePtr) {
    TablePtr = &GeventTablePtr->GeventList[0];
    while (TablePtr->Pin != 0xFF) {
      if (TablePtr->Pin < 0x40) {
        CpmTableProtocolPtr->CommonFunction.SetSmiControl (CpmTableProtocolPtr, (UINT8) TablePtr->Pin, TablePtr->Setting.Raw);
      }
      TablePtr++;
    }
  }
}
