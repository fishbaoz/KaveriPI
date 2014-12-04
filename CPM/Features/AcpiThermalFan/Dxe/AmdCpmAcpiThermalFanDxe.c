/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM ACPI Thermal Fan Initialization
 *
 * Contains CPM code to perform ACPI Thermal Fan initialization under DXE
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  AcpiThermalFan
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


EFI_GUID  ACPI_THERMAL_FAN_ACPI_DEVICE_GUID = {
  0x6983b378, 0x9e8e, 0x4c3b, 0xac, 0xea, 0x23, 0x3c, 0x98, 0x09, 0xe7, 0xc1
};

VOID
EFIAPI
InvokeAmdThermalFanInitLate (
  IN        EFI_EVENT           Event,
  IN        VOID                *Context
  );



/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM ACPI Thermal Fan DXE driver
 *
 * This function stores Thermal Fan Policy and other hardware information in NV Data.
 * It also loads, updates and installs ACPI Thermal Fan SSDT Table.
 *
 * @param[in]     ImageHandle    Pointer to the firmware file system header
 * @param[in]     SystemTable    Pointer to System table
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */
EFI_STATUS
EFIAPI
AmdCpmAcpiThermalFanDxeEntryPoint (
  IN        EFI_HANDLE                      ImageHandle,
  IN        EFI_SYSTEM_TABLE                *SystemTable
  )
{
  EFI_STATUS                                Status;
  AMD_CPM_TABLE_PROTOCOL                    *TableProtocolPtr;
  AMD_CPM_ACPI_THERMAL_FAN_TABLE            *AcpiThermalFanTablePtr;
  EFI_EVENT                                 ReadyToBootEvent;

  //
  // Initialize Global Variable
  //
  CpmInitializeDriverLib (ImageHandle, SystemTable);

  RECORD_TIME_DXE (BeginAmdCpmAcpiThermalFanDxeDriver);

  Status = gBS->LocateProtocol (  &gAmdCpmTableProtocolGuid,
                                  NULL,
                                  &TableProtocolPtr
                                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  TableProtocolPtr->CommonFunction.PostCode (0xD000);

  if (TableProtocolPtr->CommonFunction.IsThermalSupport (TableProtocolPtr)) {

    AcpiThermalFanTablePtr = TableProtocolPtr->CommonFunction.GetTablePtr (TableProtocolPtr, CPM_SIGNATURE_ACPI_THERMAL_FAN);

    if (AcpiThermalFanTablePtr) {

      if (TableProtocolPtr->MainTablePtr->AcpiThermalFanEn && !(TableProtocolPtr->CommonFunction.GetStrap (TableProtocolPtr) & BIT0)) {
        Status = gBS->CreateEventEx (
                                CPM_EVENT_NOTIFY_SIGNAL,
                                CPM_TPL_NOTIFY,
                                InvokeAmdThermalFanInitLate,
                                TableProtocolPtr,
                                &gEfiEventReadyToBootGuid,
                                &ReadyToBootEvent
                                );
      }
    }
  }

  TableProtocolPtr->CommonFunction.PostCode (0xD100);

  RECORD_TIME_DXE (EndAmdCpmAcpiThermalFanDxeDriver);

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback function to override ACPI Thermal Fan SSDT Table
 *
 * This function is used to update the offset of SBI Address Register, GPE number
 * and device name under _PR.
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
CpmAcpiThermalFanSsdtCallBack (
  IN        VOID                    *This,
  IN        VOID                    *AmlObjPtr,
  IN        VOID                    *Context
  )
{
  UINT32                            *RegionAddress;
  UINT32                            *BufferPtr;
  UINT16                            *Operation16;
  UINT32                            Data1;
  UINT32                            Data2;
  AMD_CPM_TABLE_PROTOCOL            *TableProtocolPtr;

  BufferPtr = (UINT32*)Context;
  TableProtocolPtr = This;

  switch ( *((UINT32*)AmlObjPtr)) {
  case (CPM_SIGNATURE_32 ('S', 'T', 'S', 'I')):
    if (TableProtocolPtr->CommonFunction.IsAmlOpRegionObject (AmlObjPtr)) {
      RegionAddress       = (UINT32*) ((UINT8*) AmlObjPtr + 6);
      (*RegionAddress)    = *BufferPtr;
    }
    break;
  case (CPM_SIGNATURE_32 ('_', 'L', '1', '1')):
    Operation16     = (UINT16 *) ((UINT8*) AmlObjPtr + 2);
    (*Operation16)  = *(UINT16*) (BufferPtr + 1);
    break;
  case (CPM_SIGNATURE_32 ('C', '0', '0', '0')):
  case (CPM_SIGNATURE_32 ('C', '0', '0', '1')):
  case (CPM_SIGNATURE_32 ('C', '0', '0', '2')):
  case (CPM_SIGNATURE_32 ('C', '0', '0', '3')):
  case (CPM_SIGNATURE_32 ('C', '0', '0', '4')):
  case (CPM_SIGNATURE_32 ('C', '0', '0', '5')):
  case (CPM_SIGNATURE_32 ('C', '0', '0', '6')):
  case (CPM_SIGNATURE_32 ('C', '0', '0', '7')):
    if (*(BufferPtr + 2) != 0) {
      RegionAddress = (UINT32*) AmlObjPtr;
      Data1 = (*RegionAddress) & 0xFF000000;
      Data2 = *(BufferPtr + 2) & 0x00FFFFFF;
      (*RegionAddress)  = Data1 | Data2;
    }
    break;
  }
  return FALSE;
}

/*----------------------------------------------------------------------------------------*/
/**
 * The function to load AMD CPM ACPI Thermal Fan SSDT table.
 *
 * This function gets called each time the EFI_EVENT_SIGNAL_READY_TO_BOOT gets signaled.
 *
 * @param[in]     Event          EFI_EVENT
 * @param[in]     Context        The Parameter Buffer
 *
 */
VOID
EFIAPI
InvokeAmdThermalFanInitLate (
  IN        EFI_EVENT           Event,
  IN        VOID                *Context
  )
{
  STATIC BOOLEAN                      InitlateInvoked = FALSE;
  AMD_CPM_TABLE_PROTOCOL              *TableProtocolPtr = Context;
  AMD_CPM_ACPI_THERMAL_FAN_TABLE      *ThermalFanCtrlTablePtr;
  AMD_CPM_MAIN_TABLE                  *CpmMainTablePtr;
  UINT32                              Buffer[3];
  UINT8                               SciMap;

  RECORD_TIME_DXE (BeginAmdCpmLoadThermalFanSsdt);

  if (!InitlateInvoked) {
    TableProtocolPtr->CommonFunction.PostCode (0xD200);

    CpmMainTablePtr         = TableProtocolPtr->MainTablePtr;
    ThermalFanCtrlTablePtr  = (AMD_CPM_ACPI_THERMAL_FAN_TABLE *)TableProtocolPtr->CommonFunction.GetTablePtr (TableProtocolPtr, CPM_SIGNATURE_ACPI_THERMAL_FAN);
    SciMap                  = TableProtocolPtr->CommonFunction.GetSciMap (TableProtocolPtr, ThermalFanCtrlTablePtr->FanHwConfig.EventPin);

    Buffer[0] = TableProtocolPtr->CommonFunction.GetSbTsiAddr (TableProtocolPtr);

    if ((SciMap & 0xF) < 0xA) {
      Buffer[1] = 0x3000 + (((SciMap) & 0xF) << 8);
    } else {
      Buffer[1] = 0x4000 + ((((SciMap) & 0xF) - 0x9) << 8);
    }
    if (((SciMap >> 4) & 0xF) < 0xA) {
      Buffer[1] |= 0x30 + ((SciMap >> 4) & 0xF);
    } else {
      Buffer[1] |= 0x40 + (((SciMap >> 4) & 0xF) - 0x9);
    }
    Buffer[2] = CFG_AMD_CPM_ASL_CPU0;

    TableProtocolPtr->CommonFunction.AddSsdtTable ( TableProtocolPtr,
                                                    &ACPI_THERMAL_FAN_ACPI_DEVICE_GUID,
                                                    NULL,
                                                    CpmAcpiThermalFanSsdtCallBack,
                                                    &Buffer[0]
                                                    );

    TableProtocolPtr->CommonFunction.PostCode (0xD300);
  }
  InitlateInvoked = TRUE;

  RECORD_TIME_DXE (EndAmdCpmLoadThermalFanSsdt);

  return;
}
