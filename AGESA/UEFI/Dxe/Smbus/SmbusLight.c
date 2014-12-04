/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH DXE
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH DXE
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

#include "SmbusLight.h"

#define SMB_IOREG08            0x8
#define FILECODE UEFI_DXE_SMBUS_SMBUSLIGHT_FILECODE

STATIC SMBUS_INSTANCE               *mSmbus;
STATIC EFI_HANDLE                   mDriverImageHandle;


EFI_DRIVER_ENTRY_POINT (InstallAmdSmbusLightEntryPoint)

/********************************************************************************
 * Name: InstallAmdSmbusLightEntryPoint
 *
 * Description
 *   Smbus Entrypoint
 *
 * Input
 *
 * Output
 *   EFI_UNSUPPORTED : unsupported function
 *
 *********************************************************************************/
EFI_STATUS
InstallAmdSmbusLightEntryPoint (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{
  EFI_STATUS                    Status;
  EFI_METRONOME_ARCH_PROTOCOL   *Metronome;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL   *IoDev;
  UINT32                            SmbusIoBase;

  Status = EFI_SUCCESS;
  EfiInitializeDriverLib (ImageHandle, SystemTable);
  mDriverImageHandle = ImageHandle;

  // Allocate two contiguous instances for SMBus and ASF
  Status = gBS->AllocatePool (EfiBootServicesData, (sizeof (SMBUS_INSTANCE)) * 2, &mSmbus);
  if (EFI_ERROR (Status)) {
    return Status;
  } else {
    //clear instances content
    gBS->SetMem (mSmbus, (sizeof (SMBUS_INSTANCE)) * 2, 0);
  }

  Status = gBS->LocateProtocol (&gEfiPciRootBridgeIoProtocolGuid, NULL, &mSmbus->PciRootBridgeIo);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->LocateProtocol (&gEfiPciRootBridgeIoProtocolGuid, NULL, &(mSmbus + 1)->PciRootBridgeIo);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->LocateProtocol (&gEfiMetronomeArchProtocolGuid, NULL, &Metronome);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->LocateProtocol (&gEfiPciRootBridgeIoProtocolGuid, NULL, &IoDev);
  if (EFI_ERROR (Status)) {
    return Status;
  }


  //read SMBus base address from FCH_PMIOA_REG2C
  Status = IoDev->Mem.Read (IoDev,
                           EfiPciWidthUint16,
                           (UINT64) (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG2C),
                           (UINTN) 1,
                           &SmbusIoBase);

  SmbusIoBase = SmbusIoBase & ((UINT32) 0xFE0);   //SmBus0 Base address [15:5]

  // Init data - SMBus
  mSmbus->Signature                 = SMBUS_INSTANCE_SIGNATURE;
  mSmbus->SmbusIoBase               = SmbusIoBase;
  mSmbus->StallTime                 = Metronome->TickPeriod / 10;
  mSmbus->IoDone                    = IoDone;
  mSmbus->SmbusIoRead               = SmbusIoRead;
  mSmbus->SmbusIoWrite              = SmbusIoWrite;
  mSmbus->SmbusController.Execute   = SmbusExecute;
  mSmbus->SmbusController.ArpDevice = SmbusArpDevice;
  mSmbus->SmbusController.GetArpMap = SmbusGetArpMap;
  mSmbus->SmbusController.Notify    = SmbusNotify;

  //read ASF base address from FCH_PMIOA_REG28
  Status = IoDev->Mem.Read (IoDev,
                           EfiPciWidthUint16,
                           (UINT64) (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REG28),
                           (UINTN) 1,
                           &SmbusIoBase);

  SmbusIoBase = SmbusIoBase & ((UINT32) 0xFE0); //Asf IO Base address [15:5]

  // Init data - ASF
  (mSmbus + 1)->Signature                 = SMBUS_INSTANCE_SIGNATURE;
  (mSmbus + 1)->SmbusIoBase               = SmbusIoBase;
  (mSmbus + 1)->StallTime                 = Metronome->TickPeriod / 10;
  (mSmbus + 1)->IoDone                    = IoDone;
  (mSmbus + 1)->SmbusIoRead               = SmbusIoRead;
  (mSmbus + 1)->SmbusIoWrite              = SmbusIoWrite;
  (mSmbus + 1)->SmbusController.Execute   = SmbusExecute;
  (mSmbus + 1)->SmbusController.ArpDevice = SmbusArpDevice;
  (mSmbus + 1)->SmbusController.GetArpMap = SmbusGetArpMap;
  (mSmbus + 1)->SmbusController.Notify    = SmbusNotify;

  SetDataAndRegister (mSmbus);
  InitializeListHead (&mSmbus->NotifyFunctionList);
  SetDataAndRegister (mSmbus + 1);
  InitializeListHead (&(mSmbus + 1)->NotifyFunctionList);

  // Install Protocol
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mSmbus->Handle,
                  &gEfiSmbusProtocolGuid,
                  &mSmbus->SmbusController,
                  NULL
                  );
  return Status;
}

/********************************************************************************
 * Name: SetDataAndRegister
 *
 * Description
 *   Init BaseAddress
 *
 * Input
 *   Smbus : a pointer to the SMBUS data structure
 *
 * Output
 *
 *
 *********************************************************************************/
VOID
SetDataAndRegister (
  IN SMBUS_INSTANCE             *Smbus
  )
{
  SmbusIoWrite (Smbus, HSTS, HSTS_ALL);
}


/*********************************************************************************
 * Name: SmbusIoRead
 *
 * Description
 *   Use the PCI_ROOT_Bridge_IO to do an 8 bits I/O port read
 *
 * Input
 *   Private : a pointer to the SMBUS data structure
 *   Offset  : the address to read from
 *
 * Output
 *   UINT8   : the read value
 *
 *********************************************************************************/
UINT8
SmbusIoRead (
  IN SMBUS_INSTANCE     *Private,
  IN UINT8                      Offset
  )
{
  UINT8                                                         FchSmbusData8;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL       *IoDev;

  IoDev = Private->PciRootBridgeIo;
  IoDev->Io.Read (IoDev, EfiPciWidthUint8, Private->SmbusIoBase + Offset, 1, &FchSmbusData8);

  return FchSmbusData8;
}

/*********************************************************************************
 * Name: SmbusIoRead
 *
 * Description
 *   Use the PCI_ROOT_Bridge_IO to do an 8 bits I/O port write
 *
 * Input
 *   Private : a pointer to the SMBUS data structure
 *   Offset  : the address to read from
 *   Data    : the write value
 *
 * Output
 *
 *********************************************************************************/
VOID
SmbusIoWrite (
  IN SMBUS_INSTANCE     *Private,
  IN UINT8                      Offset,
  IN UINT8                      Data
  )
{
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL               *IoDev;

  IoDev = Private->PciRootBridgeIo;
  IoDev->Io.Write (IoDev, EfiPciWidthUint8, Private->SmbusIoBase + Offset, 1, &Data);
}

/*********************************************************************************
 * Name: IoDone
 *
 * Description
 *   Check SMBUS status register
 *
 * Input
 *   Private : a pointer to the SMBUS data structure
 *   StsReg  : Test Bit
 *
 * Output
 *   BOOLEAN : Timeout or succesfully
 *
 *********************************************************************************/
BOOLEAN
IoDone (
  IN SMBUS_INSTANCE             *Private,
  IN UINT8                      *StsReg
  )
{
  BOOLEAN                       Ret;
  UINTN                         StallTries;
  UINTN                         StallIndex;

  Ret = FALSE;

  for (StallTries = STALL_TIME / Private->StallTime, StallIndex = 0; StallIndex < StallTries; ++StallIndex) {
    *StsReg = Private->SmbusIoRead (Private, HSTS);
    if (*StsReg & (INTR | BYTE_DONE_STS | DERR | BERR)) {
      Ret = TRUE;
      break;
    }
    gBS->Stall (Private->StallTime);
  }

  return Ret;
}

/*********************************************************************************
 * Name: SmBusExecute
 *
 * Description
 *   Execute the SMBUS command by calling the worker functions
 *
 * Input
 *   This         :
 *   SlaveAddress :
 *   Command      :
 *   Operation    :
 *   PecCheck     :
 *   Length       :
 *   Buffer       :
 *
 * Output
 *
 *********************************************************************************/
EFI_STATUS
SmbusExecute (
  IN       EFI_SMBUS_HC_PROTOCOL              *This,
  IN       EFI_SMBUS_DEVICE_ADDRESS           SlaveAddress,
  IN       EFI_SMBUS_DEVICE_COMMAND           Command,
  IN       EFI_SMBUS_OPERATION                Operation,
  IN       BOOLEAN                            PecCheck,
  IN OUT   UINTN                              *Length,
  IN OUT   VOID                               *Buffer
  )
{
  EFI_STATUS                                  Status;
  SMBUS_INSTANCE                              *Smbus;
  UINT8                                       StsReg,
                                              SlaveAddressReg,
                                              HostCommandReg,
                                              *Buf;
  UINTN                                       Count,
                                              Index,
                                              SmbusCommand;
  UINT32                                      LoopCount;
  UINT8                                       dbSmbusTempVar;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL             *IoDev;
  UINT8                                       bValue;
  BOOLEAN                                     IsBusy;
  UINT8                                       retryCount;

  Status = EFI_SUCCESS;
  Buf = (UINT8 *)Buffer;
  Smbus = SMBUS_INSTANCE_FROM_THIS (This);
  SetDataAndRegister (Smbus);

  IoDev = Smbus->PciRootBridgeIo;

  //
  // Host get SMBUS ownership
  //
  LoopCount = 500;
  while (LoopCount) {
    IoDev->Io.Read (IoDev, EfiPciWidthUint8, Smbus->SmbusIoBase + SMB_IOREG08, 1, &dbSmbusTempVar);
    if (dbSmbusTempVar & BIT6) {
      LoopCount--;
    } else {
      break;
    }
  }

  dbSmbusTempVar |= BIT4;
  IoDev->Io.Write (IoDev, EfiPciWidthUint8, Smbus->SmbusIoBase + SMB_IOREG08, 1, &dbSmbusTempVar);

  if (Operation != EfiSmbusQuickRead && Operation != EfiSmbusQuickWrite && (Length == NULL ||  Buffer == NULL)) {
    Status = EFI_INVALID_PARAMETER;
  }
  IsBusy = TRUE;
  retryCount = 3;
  while (retryCount--) {
    LoopCount = 100;
    do {
      dbSmbusTempVar = 0x1f;
      IoDev->Io.Write (IoDev, EfiPciWidthUint8, Smbus->SmbusIoBase , 1, &dbSmbusTempVar);
      IoDev->Io.Read (IoDev, EfiPciWidthUint8, 0xed, 1, &bValue);
      IoDev->Io.Read (IoDev, EfiPciWidthUint8, 0xed, 1, &bValue);
      IoDev->Io.Read (IoDev, EfiPciWidthUint8, Smbus->SmbusIoBase, 1, &dbSmbusTempVar);
      if ( ! ( dbSmbusTempVar & BIT0) ) {
        IsBusy = FALSE;         // Bit0 = 0, controller is idle
        break;
      }
    } while (LoopCount--);
    if (!IsBusy) {
      break;
    }
    dbSmbusTempVar = BIT1;
    IoDev->Io.Write (IoDev, EfiPciWidthUint8, Smbus->SmbusIoBase + 2, 1, &dbSmbusTempVar);
  }

  if (!EFI_ERROR (Status)) {
    StsReg = Smbus->SmbusIoRead (Smbus, HSTS);
    if ((StsReg & IUS) != 0) {
      Status = EFI_TIMEOUT;
    } else if (StsReg & HBSY) {
      Smbus->SmbusIoWrite (Smbus, HSTS, HSTS_ALL);
      Status = EFI_TIMEOUT;
    } else {
      Status = CheckSlaveStatus (Smbus);
    }

    if (!EFI_ERROR (Status)) {
      for (Count = 0; Count < TRY_TIMES; ++Count) {
        SmbusCommand = SMB_CMD_QUICK;
        SlaveAddressReg = (UINT8) ((SlaveAddress.SmbusDeviceAddress << 1) | READ_ENABLE);
        HostCommandReg = (UINT8)Command;
        Status = ProcessOperation (
                   Operation,
                   PecCheck,
                   Length,
                   *Buf,
                   &SlaveAddressReg,
                   &HostCommandReg,
                   &SmbusCommand
                   );

        if (EFI_ERROR (Status)) {
          break;
        }

        Smbus->SmbusIoWrite (Smbus, HSTS, (HBSY + INTR + DERR + BERR + FAIL));
        Smbus->SmbusIoWrite (Smbus, TSA, SlaveAddressReg);
        Smbus->SmbusIoWrite (Smbus, HCMD, HostCommandReg);
        Smbus->SmbusIoWrite (Smbus, HCTL, (UINT8) (SmbusCommand));
        //
        // Do a read from Reg02 to reset
        // the counter if it going to be a
        // block read/write operation
        //
        StsReg = Smbus->SmbusIoRead (Smbus, HCTL);
        if ((SlaveAddressReg & READ_ENABLE) != 0) {
          if (SmbusCommand == SMB_CMD_BLOCK) {
            Smbus->SmbusIoWrite (Smbus, HD0, (UINT8)*Length);
          }
        } else {
          if (SmbusCommand == SMB_CMD_BLOCK) {
            Smbus->SmbusIoWrite (Smbus, HD0, (UINT8)*Length);
            for (Index = 0; Index < *Length; Index++) {
              Smbus->SmbusIoWrite (Smbus, HBD, Buf [Index]);
            }
          } else if (SmbusCommand == SMB_CMD_WORD_DATA) {
            Smbus->SmbusIoWrite (Smbus, HD1, Buf [1]);
            Smbus->SmbusIoWrite (Smbus, HD0, Buf [0]);
          } else {
            Smbus->SmbusIoWrite (Smbus, HD0, Buf [0]);
          }
        }

        Smbus->SmbusIoWrite (Smbus, HCTL, (UINT8) (SmbusCommand + START + ((PecCheck) ? (StsReg | 0xA0) : 0)));

        if (! (Smbus->IoDone (Smbus, &StsReg))) {
          Status = EFI_TIMEOUT;
          break;
        } else if (StsReg & DERR) {
          Status = EFI_DEVICE_ERROR;
          break;
        } else if (StsReg & BERR) {
          Status = EFI_DEVICE_ERROR;
          Smbus->SmbusIoWrite (Smbus, HSTS, BERR);
          continue;
        }

        Status = SuccessfulCompletion (Operation, Smbus, Buf, Length);

        if ((StsReg & BERR) && (Status != EFI_BUFFER_TOO_SMALL)) {
          Status = EFI_DEVICE_ERROR;
          Smbus->SmbusIoWrite (Smbus, HSTS, BERR);
          continue;
        } else {
          break;
        }
      } // end for

      Smbus->SmbusIoWrite (Smbus, HSTS, (HBSY + INTR + DERR + BERR + FAIL));
    } //end if (!EFI_ERROR (Status))
  } //end if (!EFI_ERROR (Status))

  //
  // release SMBUS ownership
  //
  IoDev->Io.Read (IoDev, EfiPciWidthUint8, Smbus->SmbusIoBase + SMB_IOREG08, 1, &dbSmbusTempVar);
  dbSmbusTempVar |= BIT5;
  IoDev->Io.Write (IoDev, EfiPciWidthUint8, Smbus->SmbusIoBase + SMB_IOREG08, 1, &dbSmbusTempVar);
  return Status;
}

/*********************************************************************************
 * Name: ProcessOperation
 *
 * Description
 *   Setting flags for access SMBUS operation
 *
 * Input
 *   Operation       :
 *   PecCheck        :
 *   Length          :
 *   Len             :
 *   SlaveAddressReg :
 *   HostCommandReg  :
 *   SmbusCommand    :
 *
 * Output
 *   EFI_STATUS : input is avaliable or invalid
 *
 *********************************************************************************/
EFI_STATUS
ProcessOperation (
  IN       EFI_SMBUS_OPERATION        Operation,
  IN       BOOLEAN                    PecCheck,
  IN OUT   UINTN                      *Length,
  IN       UINT8                      Len,
     OUT   UINT8                      *SlaveAddressReg,
     OUT   UINT8                      *HostCommandReg,
     OUT   UINTN                      *SmbusCommand
  )
{
  EFI_STATUS                          Status;

  Status = EFI_SUCCESS;
  switch (Operation) {
  case EfiSmbusQuickWrite:
    *SlaveAddressReg &= ~READ_ENABLE;

  case EfiSmbusQuickRead:
    if (PecCheck) {
      Status = EFI_UNSUPPORTED;
    }
    break;

  case EfiSmbusSendByte:
    *HostCommandReg = Len;
    *SlaveAddressReg &= ~READ_ENABLE;

  case EfiSmbusReceiveByte:
    *SmbusCommand = SMB_CMD_BYTE;
    if (*Length < 1) {
      Status = EFI_BUFFER_TOO_SMALL;
    }
    *Length = 1;
    break;

  case EfiSmbusWriteByte:
    *SlaveAddressReg &= ~READ_ENABLE;

  case EfiSmbusReadByte:
    *SmbusCommand = SMB_CMD_BYTE_DATA;
    if (*Length < 1) {
      Status = EFI_BUFFER_TOO_SMALL;
    }
    *Length = 1;
    break;

  case EfiSmbusReadWord:
    *SmbusCommand = SMB_CMD_WORD_DATA;
    if (*Length < 2) {
      Status = EFI_BUFFER_TOO_SMALL;
    }
    *Length = 2;
    break;

  case EfiSmbusWriteWord:
    *SmbusCommand = SMB_CMD_WORD_DATA;
    *SlaveAddressReg &= ~READ_ENABLE;
    if (*Length < 2) {
      Status = EFI_BUFFER_TOO_SMALL;
    }
    *Length = 2;
    break;

  case EfiSmbusWriteBlock:
    *SlaveAddressReg &= ~READ_ENABLE;
    *SmbusCommand = SMB_CMD_BLOCK;
    break;

  case EfiSmbusReadBlock:
    *SmbusCommand = SMB_CMD_BLOCK;
    if ((*Length < 1) || (*Length > 72)) {
      Status = EFI_INVALID_PARAMETER;
    }
    break;

  case EfiSmbusBWBRProcessCall:
    Status = EFI_UNSUPPORTED;
    break;

  default:
    Status = EFI_INVALID_PARAMETER;
    break;
  }
  return Status;
}

/*********************************************************************************
 * Name: SuccessfulCompletion
 *
 * Description
 *
 *
 * Input
 *   Operation    :
 *   Smbus        :
 *   Buf          :
 *   Length       :
 *
 * Output
 *   EFI_STATUS
 *
 *********************************************************************************/
EFI_STATUS
SuccessfulCompletion (
  IN       EFI_SMBUS_OPERATION        Operation,
  IN       SMBUS_INSTANCE             *Smbus,
  IN       UINT8                      *Buf,
  IN OUT   UINTN                      *Length
  )
{
  EFI_STATUS                          Status;
  UINTN                               Index,
                                      BlockCount;

  Status = EFI_SUCCESS;

  switch (Operation) {
  case EfiSmbusReadWord:
  case EfiSmbusProcessCall:
    Buf [1] = Smbus->SmbusIoRead (Smbus, HD1);

  case EfiSmbusReadByte:
    Buf [0] = Smbus->SmbusIoRead (Smbus, HD0);
    break;

  case EfiSmbusReadBlock:
    BlockCount = Smbus->SmbusIoRead (Smbus, HD0);
    if (*Length < BlockCount) {
      BlockCount = *Length;
    }
    for (Index = 0; Index < BlockCount; ++Index) {
      Buf [Index] = Smbus->SmbusIoRead (Smbus, HBD);
    }
    *Length = BlockCount;
    break;

  default:
    break;
  }
  return Status;
}

/*********************************************************************************
 * Name: SmbusArpDevice
 *
 * Description
 *   Perform ARP resolution.
 *
 * Input
 *   This          :
 *   ArpAll        :
 *   SmbusUdid     :
 *   SlaveAddress  :
 *
 * Output
 *   EFI_STATUS
 *
 *********************************************************************************/
EFI_STATUS
SmbusArpDevice (
  IN       EFI_SMBUS_HC_PROTOCOL       *This,
  IN       BOOLEAN                     ArpAll,
  IN       EFI_SMBUS_UDID              *SmbusUdid,
  IN OUT   EFI_SMBUS_DEVICE_ADDRESS    *SlaveAddress
  )
{
  EFI_STATUS                           Status;
  EFI_SMBUS_DEVICE_ADDRESS             DeviceAddress;
  EFI_SMBUS_DEVICE_MAP                 *DeviceMap;
  SMBUS_INSTANCE                       *Smbus;
  UINTN                                Length;
  UINT8                                FchSmbusData8;

  Status = EFI_SUCCESS;
  Smbus = SMBUS_INSTANCE_FROM_THIS (This);
  SetDataAndRegister (Smbus);

  if (ArpAll) {
    FchSmbusData8 = DATA_PREPARE_TO_ARP;
    Length = 1;
    DeviceAddress.SmbusDeviceAddress = ADDRESS_ARP;
    Status = SmbusExecute (This, DeviceAddress, 0, EfiSmbusSendByte, TRUE, &Length, &FchSmbusData8);

    if (!EFI_ERROR (Status)) {
      do {
        DeviceMap = &Smbus->DeviceMap [Smbus->DeviceMapEntries];
        if (EFI_ERROR (GetUdidData (Smbus, DeviceMap))) {
          break;
        }

        if (DeviceMap->SmbusDeviceAddress.SmbusDeviceAddress == (0xFF >> 1)) {
          if (EFI_ERROR (GetNextDeviceAddress (Smbus, &DeviceMap->SmbusDeviceAddress))) {
            Status = EFI_OUT_OF_RESOURCES;
            break;
          }
        } else if (((DeviceMap->SmbusDeviceUdid.DeviceCapabilities) & 0xC0) != 0) {
          if (!CheckDeviceAddress (Smbus, DeviceMap->SmbusDeviceAddress)) {
            if (EFI_ERROR (GetNextDeviceAddress (Smbus, &DeviceMap->SmbusDeviceAddress))) {
              Status = EFI_OUT_OF_RESOURCES;
              break;
            }
          }
        }

        Status = AssignAddress (Smbus, DeviceMap);
        if (EFI_ERROR (Status)) {
          if (Status != EFI_DEVICE_ERROR) {
            break;
          }
        } else {
          ++Smbus->DeviceMapEntries;
        }
      } while (Smbus->DeviceMapEntries < MAX_SMBUS_DEVICES);
    } else {
      if (Status == EFI_DEVICE_ERROR) {
        Status = EFI_SUCCESS;
      }
    }
  } else {
    if (SmbusUdid != NULL && SlaveAddress != NULL) {
      if (Smbus->DeviceMapEntries >= MAX_SMBUS_DEVICES) {
        Status = EFI_OUT_OF_RESOURCES;
      }

      if (!EFI_ERROR (Status)) {
        DeviceMap = &Smbus->DeviceMap [Smbus->DeviceMapEntries];
        if (EFI_ERROR (GetNextDeviceAddress (Smbus, &DeviceMap->SmbusDeviceAddress))) {
          Status = EFI_OUT_OF_RESOURCES;
        }
        if (!EFI_ERROR (Status)) {
          DeviceMap->SmbusDeviceUdid = *SmbusUdid;
          Status = AssignAddress (Smbus, DeviceMap);

          if (!EFI_ERROR (Status)) {
            ++Smbus->DeviceMapEntries;
            SlaveAddress->SmbusDeviceAddress = DeviceMap->SmbusDeviceAddress.SmbusDeviceAddress;
          }
        }
      }
    } else {
      Status = EFI_INVALID_PARAMETER;
    }
  }
  return Status;
}

/*********************************************************************************
 * Name: GetUdidData
 *
 * Description
 *
 *
 * Input
 *   Smbus          :
 *   DeviceMap      :
 *
 * Output
 *   EFI_STATUS
 *
 *********************************************************************************/
EFI_STATUS
GetUdidData (
  IN       SMBUS_INSTANCE          *Smbus,
  IN OUT   EFI_SMBUS_DEVICE_MAP    *DeviceMap
  )
{
  EFI_SMBUS_DEVICE_ADDRESS         DeviceAddress;
  EFI_STATUS                       Status;
  UINTN                            Length;
  EFI_SMBUS_DEVICE_MAP             Temp;

  DeviceAddress.SmbusDeviceAddress = ADDRESS_ARP;
  Length = sizeof (EFI_SMBUS_DEVICE_MAP);

  Status = SmbusExecute (
             &Smbus->SmbusController,
             DeviceAddress,
             DATA_RESET_DEVICE,
             EfiSmbusReadBlock,
             TRUE,
             &Length,
             &Temp
             );

  if (!EFI_ERROR (Status)) {
    if (Length != sizeof (EFI_SMBUS_DEVICE_MAP)) {
      Status = EFI_DEVICE_ERROR;
    } else {
      *DeviceMap = Temp;
    }
  }
  return Status;
}

/*********************************************************************************
 * Name: GetNextDeviceAddress
 *
 * Description
 *
 *
 * Input
 *   Smbus          :
 *   DeviceAddress  :
 *
 * Output
 *   EFI_STATUS
 *
 *********************************************************************************/
EFI_STATUS
GetNextDeviceAddress (
  IN SMBUS_INSTANCE            *Smbus,
  IN EFI_SMBUS_DEVICE_ADDRESS  *DeviceAddress
  )
{
  EFI_STATUS                    Status;

  Status = EFI_OUT_OF_RESOURCES;

  for (DeviceAddress->SmbusDeviceAddress = 0x03; DeviceAddress->SmbusDeviceAddress < 0x7F; ++DeviceAddress->SmbusDeviceAddress) {
    if (CheckDeviceAddress (Smbus, *DeviceAddress)) {
      Status = EFI_SUCCESS;
      break;
    }
  }
  return Status;
}

/*********************************************************************************
 * Name: CheckDeviceAddress
 *
 * Description
 *
 *
 * Input
 *   Smbus          :
 *   DeviceAddress  :
 *
 * Output
 *   BOOLEAN
 *
 *********************************************************************************/
BOOLEAN
CheckDeviceAddress (
  IN SMBUS_INSTANCE                                         *Smbus,
  IN EFI_SMBUS_DEVICE_ADDRESS                               DeviceAddress
  )
{
  BOOLEAN                                                   Ret;
  UINT8                                                     Index;

  Ret = TRUE;
  for (Index = 0; Index < Smbus->DeviceMapEntries; ++Index) {
    if (DeviceAddress.SmbusDeviceAddress == Smbus->DeviceMap [Index].SmbusDeviceAddress.SmbusDeviceAddress) {
      Ret = FALSE;
      break;
    }
  }
  return Ret;
}

/*********************************************************************************
 * Name: AssignAddress
 *
 * Description
 *
 *
 * Input
 *   Smbus          :
 *   DeviceMap      :
 *
 * Output
 *   EFI_STATUS
 *
 *********************************************************************************/
EFI_STATUS
AssignAddress (
  IN       SMBUS_INSTANCE          *Smbus,
  IN OUT   EFI_SMBUS_DEVICE_MAP    *DeviceMap
  )
{
  EFI_SMBUS_DEVICE_ADDRESS         DeviceAddress;
  EFI_STATUS                       Status;
  UINTN                            Length;
  EFI_SMBUS_DEVICE_MAP             Temp;

  Temp = *DeviceMap;
  DeviceAddress.SmbusDeviceAddress = ADDRESS_ARP;
  Length = sizeof (EFI_SMBUS_DEVICE_MAP);
  Status = SmbusExecute (
             &Smbus->SmbusController,
             DeviceAddress,
             DATA_RESET_DEVICE,
             EfiSmbusWriteBlock,
             TRUE,
             &Length,
             &Temp
             );

  return Status;
}

/*********************************************************************************
 * Name: SmbusGetArpMap
 *
 * Description
 *
 *
 * Input
 *   This           :
 *   Length         :
 *   SmbusDeviceMap :
 *
 * Output
 *   EFI_STATUS
 *
 *********************************************************************************/
EFI_STATUS
SmbusGetArpMap (
  IN       EFI_SMBUS_HC_PROTOCOL    *This,
  IN OUT   UINTN                    *Length,
  IN OUT   EFI_SMBUS_DEVICE_MAP     **SmbusDeviceMap
  )
{
  SMBUS_INSTANCE                    *Smbus;

  Smbus = SMBUS_INSTANCE_FROM_THIS (This);
  *Length = Smbus->DeviceMapEntries;
  *SmbusDeviceMap = Smbus->DeviceMap;

  return EFI_SUCCESS;
}

/*********************************************************************************
 * Name: SmbusNotify
 *
 * Description
 *   Register a notify function
 *
 * Input
 *   This           :
 *   SlaveAddress   :
 *   Data           :
 *   NotifyFunction :
 *
 * Output
 *   EFI_STATUS
 *
 *********************************************************************************/
EFI_STATUS
SmbusNotify (
  IN EFI_SMBUS_HC_PROTOCOL            *This,
  IN EFI_SMBUS_DEVICE_ADDRESS         SlaveAddress,
  IN UINTN                            Data,
  IN EFI_SMBUS_NOTIFY_FUNCTION        NotifyFunction
  )
{
  EFI_STATUS                          Status;
  SMBUS_INSTANCE                      *Smbus;
  SMBUS_NOTIFY_FUNCTION_LIST_NODE     *Node;

  Status = EFI_UNSUPPORTED;
  Smbus = SMBUS_INSTANCE_FROM_THIS (This);

  if (NotifyFunction == NULL) {
    if (Smbus->NotificationEvent == NULL) {
      if (!EFI_ERROR (
        gBS->CreateEvent (
               EFI_EVENT_TIMER | EFI_EVENT_NOTIFY_SIGNAL,
               EFI_TPL_CALLBACK,
               SmbusNotifyFunction,
               NULL,
               &Smbus->NotificationEvent
               ))) {
        if (!EFI_ERROR (
          gBS->SetTimer (Smbus->NotificationEvent, TimerPeriodic, NOTIFYTIME)
          )) {
          gBS->AllocatePool (EfiBootServicesData, sizeof (SMBUS_NOTIFY_FUNCTION_LIST_NODE), &Node);
          gBS->SetMem (Node, sizeof (SMBUS_NOTIFY_FUNCTION_LIST_NODE), 0);
          Node->Signature = SMBUS_INSTANCE_SIGNATURE;
          Node->SlaveAddress.SmbusDeviceAddress = SlaveAddress.SmbusDeviceAddress;
          Node->Data = Data;
          Node->NotifyFunction = NotifyFunction;
          InsertTailList (&Smbus->NotifyFunctionList, &Node->Link);
        }
      }
    }
  } else {
    Status = EFI_INVALID_PARAMETER;
  }
  return Status;
}

/*********************************************************************************
 * Name: SmbusNotifyFunction
 *
 * Description
 *   Register a notify function
 *
 * Input
 *   Event     :
 *   Context   :
 *
 * Output
 *
 *
 *********************************************************************************/
VOID
SmbusNotifyFunction (
  IN EFI_EVENT                        Event,
  IN VOID                             *Context
  )
{
  EFI_LIST_NODE                       *Link;
  EFI_SMBUS_DEVICE_ADDRESS            DeviceAddress;
  SMBUS_NOTIFY_FUNCTION_LIST_NODE     *Node;
  UINT8                               FchSmbusData8;
  UINTN                               DataN;

  SetDataAndRegister (mSmbus);
  FchSmbusData8 = mSmbus->SmbusIoRead (mSmbus, SSTS);
  if ((FchSmbusData8 & HOST_NOTIFY_STS) != 0) {
    DeviceAddress.SmbusDeviceAddress = (mSmbus->SmbusIoRead (mSmbus, NDA)) >> 1;

    for (
    Link = GetFirstNode (&mSmbus->NotifyFunctionList);
    !IsNull (&mSmbus->NotifyFunctionList, Link);
    Link = GetNextNode (&mSmbus->NotifyFunctionList, &Node->Link)
    ) {
      Node = SMBUS_NOTIFY_FUNCTION_LIST_NODE_FROM_LINK (Link);
      if (Node->SlaveAddress.SmbusDeviceAddress == DeviceAddress.SmbusDeviceAddress) {
        DataN = (mSmbus->SmbusIoRead (mSmbus, NDHB) << 8)
          + (mSmbus->SmbusIoRead (mSmbus, NDLB));
        if ((UINT16)Node->Data == (UINT16)DataN) {
          Node->NotifyFunction (DeviceAddress, DataN);
        }
      }
    }
    mSmbus->SmbusIoWrite (mSmbus, SSTS, HOST_NOTIFY_STS);
  }
}

/*********************************************************************************
 * Name: CheckSlaveStatus
 *
 * Description
 *
 *
 * Input
 *   Smbus     :
 *
 * Output
 *   EFI_STATUS
 *
 *********************************************************************************/
EFI_STATUS
CheckSlaveStatus (
  IN SMBUS_INSTANCE       *Smbus
  )
{
  UINT32        LoopCount;
  UINT32        LoopCount1;
  UINT8         dbSmbusTempVar;
  BOOLEAN   ControllerBusy;

  ControllerBusy = TRUE;
  LoopCount1 = 3;

  while (LoopCount1 && ControllerBusy) {
    LoopCount = 100;

    while (LoopCount && ControllerBusy) {
      Smbus->SmbusIoWrite (Smbus, SMBUS_R_SSTS, SMBUS_B_SSTS_ALL); // out index 00h, 0ffh to clear status.
      gBS->Stall (2);
      dbSmbusTempVar = Smbus->SmbusIoRead (Smbus, SMBUS_R_SSTS);

      if ( dbSmbusTempVar & SMBUS_B_SBSY ) {
        // Bit0 = 1, controller is still busy!
        LoopCount--;
      } else {
        ControllerBusy = FALSE;
      }
    } // end of while (loopCount)

    if (ControllerBusy) {
      Smbus->SmbusIoWrite (Smbus, SMBUS_R_SSTS, SMBUS_B_SINIT);    // slave init
      LoopCount1--;
    }
  }

  if (ControllerBusy) {
    return EFI_TIMEOUT;
  } else {
    return EFI_SUCCESS;
  }
}

