/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH PEIM
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH PEIM
 * @e \$Revision: 292136 $   @e \$Date: 2014-05-07 14:10:43 -0500 (Wed, 07 May 2014) $
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

#include "Tiano.h"
#include <Pei.h>

// Consumed PPI
#include EFI_PPI_CONSUMER (CpuIo)
#include EFI_PPI_DEFINITION (Stall)

// Produced PPI
#include EFI_PPI_PRODUCER (Smbus)

#include "Smbus.h"
#define FILECODE UEFI_PEI_SMBUS_SMBUS_FILECODE

EFI_PEIM_ENTRY_POINT (AmdSmbusPeiInit);

/*********************************************************************************
 * Name: FchSmbusIoRead8
 *
 * Description
 *   Use the CPU_IO_PPI to do an 8 bits I/O port read
 *
 * Input
 *   Private : a pointer to the private SMBUS data structure
 *   Address : the address to read from
 *
 * Output
 *   UINT8 : the read value
 *
 *********************************************************************************/
UINT8
FchSmbusIoRead8 (
  IN       AMD_SMBUS_PPI_PRIVATE  *Private,
  IN       UINTN                  Address
  )
{
  EFI_PEI_SERVICES    **PeiServices;
  PEI_CPU_IO_PPI      *CpuIo;

  // Get CPU IO PPI from PEI service pointer
  PeiServices = Private->PeiServices;
  CpuIo = (*PeiServices)->CpuIo;

  // Perform the read
  return (CpuIo->IoRead8 (
                   PeiServices,
                   CpuIo,
                   Private->BaseAddress + Address
                   ));
}


/*********************************************************************************
 * Name: FchSmbusIoWrite8
 *
 * Description
 *   Use the CPU_IO_PPI to do an 8 bits I/O port write
 *
 * Input
 *   Private : a pointer to the private SMBUS data structure
 *   Address : the address to read from
 *   Data    : the data to write
 *
 * Output
 *
 *********************************************************************************/
VOID
FchSmbusIoWrite8 (
  IN       AMD_SMBUS_PPI_PRIVATE  *Private,
  IN       UINTN                  Address,
  IN       UINT8                  Data
  )
{
  EFI_PEI_SERVICES    **PeiServices;
  PEI_CPU_IO_PPI      *CpuIo;

  // Get CPU IO PPI from the PEI service pointer
  PeiServices = Private->PeiServices;
  CpuIo = (*PeiServices)->CpuIo;

  // Perform the write
  CpuIo->IoWrite8 (
           PeiServices,
           CpuIo,
           Private->BaseAddress + Address,
           Data
           );
}


UINT16
ReadPmio16 (
  IN       EFI_PEI_SERVICES    **PeiServices,
  IN       PEI_CPU_IO_PPI      *CpuIo,
  IN       UINT8               PmioIndex
  )
{
  UINT16    Value16;

  CpuIo->IoWrite8 (PeiServices, CpuIo, FCH_IOMAP_REGCD6, PmioIndex + 1);
  Value16 = (UINT16) (CpuIo->IoRead8 (PeiServices, CpuIo, FCH_IOMAP_REGCD7) << 8);
  CpuIo->IoWrite8 (PeiServices, CpuIo, FCH_IOMAP_REGCD6, PmioIndex);
  return (Value16 + CpuIo->IoRead8 (PeiServices, CpuIo, FCH_IOMAP_REGCD7));
}


VOID
WritePmio16 (
  IN       EFI_PEI_SERVICES    **PeiServices,
  IN       PEI_CPU_IO_PPI      *CpuIo,
  IN       UINT8               PmioIndex,
  IN       UINT16              RegValue
  )
{
  CpuIo->IoWrite8 (PeiServices, CpuIo, FCH_IOMAP_REGCD6, PmioIndex + 1);
  CpuIo->IoWrite8 (PeiServices, CpuIo, FCH_IOMAP_REGCD7, (UINT8) (RegValue >> 8));
  CpuIo->IoWrite8 (PeiServices, CpuIo, FCH_IOMAP_REGCD6, PmioIndex);
  CpuIo->IoWrite8 (PeiServices, CpuIo, FCH_IOMAP_REGCD7, (UINT8) (RegValue & 0xFF));
}


/*********************************************************************************
 * Name: SmBusExecute
 *
 * Description
 *   Execute the SMBUS command by calling the worker functions
 *
 * Input
 *   PeiServices  : pointer to the PEI service table (SmBusExecute only)
 *   This         : pointer to the SMBUS (2) PPI instance
 *   SlaveAddress :
 *   Command      :
 *   PecCheck     :
 *   Length       :
 *   Buffer       :
 *
 * Output
 *
 *********************************************************************************/
EFI_STATUS
SmBusExecute (
  IN       EFI_PEI_SERVICES          **PeiServices,
  IN       PEI_SMBUS_PPI             *This,
  IN       EFI_SMBUS_DEVICE_ADDRESS  SlaveAddress,
  IN       EFI_SMBUS_DEVICE_COMMAND  Command,
  IN       EFI_SMBUS_OPERATION       Operation,
  IN       BOOLEAN                   PecCheck,
  IN OUT   UINTN                     *Length,
  IN OUT   VOID                      *Buffer
  )
{
  AMD_SMBUS_PPI_PRIVATE  *PeiPrivate;
  AMD_SMBUS_PPI_PRIVATE  Private;
  EFI_STATUS             Status;
  PEI_CPU_IO_PPI         *CpuIo;
  UINT16                 LoopCount;
  UINT8                  retryCount;
  UINT8                  bValue;
  BOOLEAN                IsBusy;
  BOOLEAN                BufferTooSmall;
  UINT8                  SmbusOperation;
  UINT8                  SlvAddrReg;
  UINT8                  HostCmdReg;
  UINT8                  *CallBuffer;
  UINT8                  Index;
  UINT8                  StsReg;
  UINT8                  BlockCount;
  UINT32                 DwLoopCount;

  StsReg      = 0;
  BlockCount  = 0;
  CallBuffer  = Buffer;
  Status      = EFI_SUCCESS;

  // Get the PEI service pointer from the private data structure
  PeiPrivate = (AMD_SMBUS_PPI_PRIVATE*) (This);

  // Create the private structure used by the library
  Private.PeiServices = (VOID*) (PeiServices);
  Private.IoRead8     = FchSmbusIoRead8;
  Private.IoWrite8    = FchSmbusIoWrite8;

  CpuIo = (*(Private.PeiServices))->CpuIo;
  Private.BaseAddress = ReadPmio16 (PeiServices, CpuIo, PeiPrivate->RegisterIndex) & ~(BIT2 + BIT1 + BIT0);

  //
  // Main I/O sequence starts here
  //
  if (! ((Operation == EfiSmbusQuickRead) || (Operation == EfiSmbusQuickWrite))) {
    if ((Length == NULL) || (Buffer == NULL)) {
      return EFI_INVALID_PARAMETER;
    }
  }


  // Keep polling until host gets SMBUS control
  LoopCount = 100;
  do {
    bValue = Private.IoRead8 (&Private, SMB_IOREG08);
    bValue |= BIT4;       // Set HostSemaphore
    Private.IoWrite8 (&Private, SMB_IOREG08, bValue);
    bValue = CpuIo->IoRead8 (
                      PeiServices,
                      CpuIo,
                      0xed
                      ); // 0xed IO Delay
    bValue = CpuIo->IoRead8 (
                      PeiServices,
                      CpuIo,
                      0xed
                      ); // 0xed IO Delay

    bValue = Private.IoRead8 (&Private, SMB_IOREG08);
    LoopCount--;
    if (LoopCount == 0) {
      return EFI_TIMEOUT;
    }
  } while ((bValue & BIT4) == 0);     // Loop until semaphore has established


  // Clear host status and wait until host is idle
  IsBusy = TRUE;
  retryCount = 3;
  while (retryCount--) {
    LoopCount = 100;
    do {
      Private.IoWrite8 (&Private, SMB_IOREG00, SMB_ALL_HOST_STATUS); // Clear all status bits
      bValue = CpuIo->IoRead8 (
                        PeiServices,
                        CpuIo,
                        0xed
                        );
      bValue = CpuIo->IoRead8 (
                        PeiServices,
                        CpuIo,
                        0xed
                        );
      bValue = Private.IoRead8 (&Private, SMB_IOREG00);
      if ( ! ( bValue & BIT0) ) {
        IsBusy = FALSE;         // Bit0 = 0, controller is idle
        break;
      }
    } while (LoopCount--);
    if (!IsBusy) {
      break;
    }
    Private.IoWrite8 (&Private, SMB_IOREG02, BIT1);     // Set [1] = Kill current host transaction
  }
  if (IsBusy) {
    return EFI_TIMEOUT;
  }

  // Clear slave status and wait until slave is idle
  IsBusy = TRUE;
  retryCount = 3;
  while (retryCount--) {
    LoopCount = 100;
    do {
      Private.IoWrite8 (&Private, SMB_IOREG01, 0x3f);     // Clear SMB_ALL_SLAVE_STATUS
      bValue = CpuIo->IoRead8 (
                        PeiServices,
                        CpuIo,
                        0xed
                        );
      bValue = CpuIo->IoRead8 (
                        PeiServices,
                        CpuIo,
                        0xed
                        );
      bValue = Private.IoRead8 (&Private, SMB_IOREG01);
      if ( ! ( bValue & BIT0) ) {
        IsBusy = FALSE;         // Bit0 = 0, slave is idle
        break;
      }
    } while (LoopCount--);
    if (!IsBusy) {
      break;
    }
    Private.IoWrite8 (&Private, SMB_IOREG01, BIT1);     // Set BIT1 = SlaveInit
  }
  if (IsBusy) {
    return EFI_TIMEOUT;
  }

  // Initialize variables
  SmbusOperation  = SMB_CMD_QUICK;
  SlvAddrReg      = (UINT8) ((SlaveAddress.SmbusDeviceAddress << 1) | SMBUS_READ_ENABLE);
  HostCmdReg      = (UINT8) Command;

  // Process Operation
  switch (Operation) {
  case EfiSmbusQuickWrite:
    SlvAddrReg &= ~SMBUS_READ_ENABLE;
    break;

  case EfiSmbusQuickRead:
    if (PecCheck) {
      Status = EFI_UNSUPPORTED;
    }
    break;

  case EfiSmbusSendByte:
    HostCmdReg = CallBuffer[0];
    SlvAddrReg &= ~SMBUS_READ_ENABLE;
    break;

  case EfiSmbusReceiveByte:
    SmbusOperation = SMB_CMD_BYTE;
    if (*Length < 1) {
      Status = EFI_BUFFER_TOO_SMALL;
    }
    *Length = 1;
    break;

  case EfiSmbusWriteByte:
    SlvAddrReg &= ~SMBUS_READ_ENABLE;

  case EfiSmbusReadByte:
    SmbusOperation = SMB_CMD_BYTE_DATA;
    if (*Length < 1) {
      Status = EFI_BUFFER_TOO_SMALL;
    }
    *Length = 1;
    break;

  case EfiSmbusReadWord:
    SmbusOperation = SMB_CMD_WORD_DATA;
    if (*Length < 2) {
      Status = EFI_BUFFER_TOO_SMALL;
    }
    *Length = 2;
    break;

  case EfiSmbusWriteWord:
    SmbusOperation = SMB_CMD_WORD_DATA;
    SlvAddrReg &= ~SMBUS_READ_ENABLE;
    if (*Length < 2) {
      Status = EFI_BUFFER_TOO_SMALL;
    }
    *Length = 2;
    break;

  case EfiSmbusWriteBlock:
    SlvAddrReg &= ~SMBUS_READ_ENABLE;
    BlockCount = (UINT8) (*Length);
    SmbusOperation = SMB_CMD_BLOCK;
    break;

  case EfiSmbusReadBlock:
    SmbusOperation = SMB_CMD_BLOCK;
    if ((*Length < 1) || (*Length > 72)) {
      Status = EFI_INVALID_PARAMETER;
      break;
    }
    break;

  case EfiSmbusBWBRProcessCall:
    Status = EFI_UNSUPPORTED;
    break;

  default:
    Status = EFI_INVALID_PARAMETER;
    break;
  }

  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // If the operation results in a Smbus collision with another master on the bus, it attempts
  // the requested transaction again for at least TRY_TIMES attempts. We assume the value of
  // TRY_TIMES is 3.
  //
  for (retryCount = 0; retryCount < TRY_TIMES; retryCount++) {
    // Clear all status Bits
    Private.IoWrite8 (&Private, SMB_IOREG00, SMB_ALL_HOST_STATUS);

    // Set SMBus slave address for the device to send/receive from
    Private.IoWrite8 (&Private, SMB_IOREG04, SlvAddrReg);

    // Set Command register
    Private.IoWrite8 (&Private, SMB_IOREG03, HostCmdReg);

    // Select SMBus protocol
    Private.IoWrite8 (&Private, SMB_IOREG02, (UINT8)SmbusOperation);

    //
    // Do a read from Reg02 to reset the counter if it's going to be a
    // block read/write operation
    //
    StsReg = Private.IoRead8 (&Private, SMB_IOREG02);


    if (SlvAddrReg & SMBUS_READ_ENABLE) {
      if (SmbusOperation == SMB_CMD_BLOCK) {
        Private.IoWrite8 (&Private, SMB_IOREG05, *(UINT8*)Length);
      }
    } else {
      if (SmbusOperation == SMB_CMD_BLOCK) {
        Private.IoWrite8 (&Private, SMB_IOREG05, *(UINT8*)Length);
        for (Index = 0; Index < *Length; Index++) {
          Private.IoWrite8 (&Private, SMB_IOREG07, CallBuffer[Index]);
        }
      } else if (SmbusOperation == SMB_CMD_WORD_DATA) {
        Private.IoWrite8 (&Private, SMB_IOREG06, CallBuffer[1]);
        Private.IoWrite8 (&Private, SMB_IOREG05, CallBuffer[0]);
      } else {
        Private.IoWrite8 (&Private, SMB_IOREG05, CallBuffer[0]);
      }
    }

    // Set Control Register (Initiate Operation, Interrupt disabled)
    Private.IoWrite8 (&Private, SMB_IOREG02, \
                        (UINT8) (SmbusOperation + BIT6 + ((PecCheck) ? (StsReg | 0xA0) : 0)));  // [6] = Start

    // Wait for controller IO to finish work
      // Wait till not busy
      StsReg = 0x01; //pre set BUSY
      DwLoopCount = 1000;
      do {
        bValue = CpuIo->IoRead8 (
                          PeiServices,
                          CpuIo,
                          0xed
                          );
        bValue = CpuIo->IoRead8 (
                          PeiServices,
                          CpuIo,
                          0xed
                          );
        StsReg = Private.IoRead8 (&Private, SMB_IOREG00);
        if ( ! ( StsReg & BIT0) ) {
          break;   // Bit0 = 0, Host is not busy
        }
      } while (DwLoopCount--);


    StsReg = Private.IoRead8 (&Private, SMB_IOREG00);
    if ( ( StsReg & (BIT0)) ) { //Still BUSY/COLLISON/ERROR
      Private.IoWrite8 (&Private, SMB_IOREG02, BIT7);     // Set [7] = Reset current host transaction
      Private.IoWrite8 (&Private, SMB_IOREG00, SMB_ALL_HOST_STATUS);
      continue;
    }

    if ((StsReg & BIT2) && (StsReg & BIT0)) {
      Status = EFI_DEVICE_ERROR;
      FCH_DEADLOOP ();
      break;
    }

    if (StsReg & BIT2) {
      Status = EFI_DEVICE_ERROR;
      break;
    }
    if (StsReg & BIT3) {
      // Clear bus error for another try
      Private.IoWrite8 (&Private, SMB_IOREG00, BIT3);
      continue;
    }


    //
    // Successful completion
    //
    switch (Operation) {
    case EfiSmbusReadWord:
    case EfiSmbusProcessCall:
      // Read a byte from Host data 1
      CallBuffer[1] = Private.IoRead8 (&Private, SMB_IOREG06);
      CallBuffer[0] = Private.IoRead8 (&Private, SMB_IOREG05);
      break;

    case EfiSmbusReceiveByte:
    case EfiSmbusReadByte:
      // Read a byte from Host data 0
      CallBuffer[0] = Private.IoRead8 (&Private, SMB_IOREG05);
      break;

    case EfiSmbusReadBlock:
      BufferTooSmall = FALSE;
      BlockCount = Private.IoRead8 (&Private, SMB_IOREG02);
      BlockCount = Private.IoRead8 (&Private, SMB_IOREG05);
      if (*Length < BlockCount) {
        BufferTooSmall = TRUE;
      } else {
        for (Index = 0; Index < BlockCount; Index++) {
          // Read the byte
          CallBuffer[Index] = Private.IoRead8 (&Private, SMB_IOREG07);
        }
      }
      *Length = BlockCount;
      if (BufferTooSmall) {
        Status = EFI_BUFFER_TOO_SMALL;
      }
      break;

    default:
      break;
    }

    if ((StsReg & BIT3) && (Status != EFI_BUFFER_TOO_SMALL)) {
      // Clear 'Bus Error' bit of 'Status Register'
      Status = EFI_DEVICE_ERROR;
      Private.IoWrite8 (&Private, SMB_IOREG00, BIT3);
      continue;
    } else {
      break;
    }
  }

  // Clear host controller status
  Private.IoWrite8 (&Private, SMB_IOREG00, SMB_ALL_HOST_STATUS);


  // Release SMBUS ownership
  bValue = Private.IoRead8 (&Private, SMB_IOREG08);
  bValue |= BIT5;        // Clear Host Semaphore
  Private.IoWrite8 (&Private, SMB_IOREG08, bValue);

  return Status;
}


/********************************************************************************
 * Name: SmBusArpDevice
 *
 * Description
 *   Perform ARP resolution. function currently not supported
 *
 * Input
 *
 * Output
 *   EFI_UNSUPPORTED : unsupported function
 *
 *********************************************************************************/
EFI_STATUS
SmBusArpDevice (
  IN       EFI_PEI_SERVICES              **PeiServices,
  IN       PEI_SMBUS_PPI                 *This,
  IN       BOOLEAN                       ArpAll,
  IN       EFI_SMBUS_UDID                *SmbusUdid, OPTIONAL
  IN OUT   EFI_SMBUS_DEVICE_ADDRESS      *SlaveAddress OPTIONAL
  )
{
  // currently not supported
  return (EFI_UNSUPPORTED);
}


/*********************************************************************************
 * Name: SmBusGetArpMap
 *
 * Description
 *   Get the ARP map. function currently not supported
 *
 * Input
 *
 * Output
 *   EFI_UNSUPPORTED : unsupported function
 *
 *********************************************************************************/
EFI_STATUS
SmBusGetArpMap (
  IN       EFI_PEI_SERVICES          **PeiServices,
  IN       PEI_SMBUS_PPI             *This,
  IN OUT   UINTN                     *Length,
  IN OUT   EFI_SMBUS_DEVICE_MAP      **SmbusDeviceMap
  )
{
  // currently not supported
  return (EFI_UNSUPPORTED);
}


/*********************************************************************************
 * Name: SmBusNotify
 *
 * Description
 *   Register a notify function. function currently not supported
 *
 * Input
 *
 * Output
 *   EFI_UNSUPPORTED : unsupported function
 *
 *********************************************************************************/
EFI_STATUS
SmBusNotify (
  IN       EFI_PEI_SERVICES               **PeiServices,
  IN       PEI_SMBUS_PPI                  *This,
  IN       EFI_SMBUS_DEVICE_ADDRESS       SlaveAddress,
  IN       UINTN                          Data,
  IN       PEI_SMBUS_NOTIFY_FUNCTION      NotifyFunction
  )
{
  // currently not supported
  return (EFI_UNSUPPORTED);
}




/*********************************************************************************
 * Name: AmdSmbusPeiInit
 *
 * Description
 *   Entry point of the AMD SMBUS PEIM
 *   Initialize the SMBUS controller and publish the PPIs (both version 1 and 2
 *   are supported)
 *
 * Input
 *   FfsHeader   : pointer to the firmware file system header
 *   PeiServices : pointer to the PEI service table
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 *********************************************************************************/
EFI_STATUS
AmdSmbusPeiInit (
  IN       EFI_FFS_FILE_HEADER  *FfsHeader,
  IN       EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                Status;
  AMD_PEI_FCH_SMBUS_PRIVATE *SmbusPrivate;
  EFI_PEI_PPI_DESCRIPTOR    *PpiListSmBus;
  PEI_CPU_IO_PPI            *CpuIo;
  UINT16                    Value16;

  CpuIo = (*PeiServices)->CpuIo;

  //
  // Create the PPIs
  //
  Status = (*PeiServices)->AllocatePool (
                             PeiServices,
                             sizeof (AMD_PEI_FCH_SMBUS_PRIVATE),
                             &SmbusPrivate
                             );

  ASSERT_PEI_ERROR (PeiServices, Status);

  //
  // Initialize the structure
  //
  SmbusPrivate->SmbusPpi0.SmbusPpi.Execute   = SmBusExecute;
  SmbusPrivate->SmbusPpi0.SmbusPpi.ArpDevice = SmBusArpDevice;
  SmbusPrivate->SmbusPpi0.SmbusPpi.GetArpMap = SmBusGetArpMap;
  SmbusPrivate->SmbusPpi0.SmbusPpi.Notify    = SmBusNotify;

  Value16 = ReadPmio16 (PeiServices, CpuIo, FCH_PMIOA_REG24);
  WritePmio16 (PeiServices, CpuIo, FCH_PMIOA_REG24, Value16 | BIT0);
  Value16 = ReadPmio16 (PeiServices, CpuIo, FCH_PMIOA_REG2C);
  PEI_ASSERT (PeiServices, Value16);
  WritePmio16 (PeiServices, CpuIo, FCH_PMIOA_REG2C, Value16 | BIT0);
  SmbusPrivate->SmbusPpi0.BaseAddress        = Value16 & ~(BIT2 + BIT1 + BIT0);
  SmbusPrivate->SmbusPpi0.RegisterIndex      = FCH_PMIOA_REG2C;

  SmbusPrivate->SmbusPpi1.SmbusPpi.Execute   = SmBusExecute;
  SmbusPrivate->SmbusPpi1.SmbusPpi.ArpDevice = SmBusArpDevice;
  SmbusPrivate->SmbusPpi1.SmbusPpi.GetArpMap = SmBusGetArpMap;
  SmbusPrivate->SmbusPpi1.SmbusPpi.Notify    = SmBusNotify;

  Value16 = ReadPmio16 (PeiServices, CpuIo, FCH_PMIOA_REG28);
  PEI_ASSERT (PeiServices, Value16);
  WritePmio16 (PeiServices, CpuIo, FCH_PMIOA_REG28, Value16 | (BIT0 + BIT2));
  SmbusPrivate->SmbusPpi1.BaseAddress        = Value16 & ~(BIT2 + BIT1 + BIT0);
  SmbusPrivate->SmbusPpi1.RegisterIndex      = FCH_PMIOA_REG28;

  //
  // Install the PPIs
  //
  Status = (*PeiServices)->AllocatePool (
                             PeiServices,
                             (sizeof (EFI_PEI_PPI_DESCRIPTOR) * 2),
                             &PpiListSmBus
                             );

  ASSERT_PEI_ERROR (PeiServices, Status);

  //
  // Create the PPI descriptor
  //
  PpiListSmBus[0].Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI);
  PpiListSmBus[0].Guid  = &gPeiSmbusPpiGuid;
  PpiListSmBus[0].Ppi   = &(SmbusPrivate->SmbusPpi0.SmbusPpi);

  PpiListSmBus[1].Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  PpiListSmBus[1].Guid  = &gPeiSmbusPpiGuid;
  PpiListSmBus[1].Ppi   = &(SmbusPrivate->SmbusPpi1.SmbusPpi);

  // The SMBUS host controllers should have been enabled earlier so nothing need to do here

  //
  // Publish the PPI
  //
  Status = (*PeiServices)->InstallPpi (
                             PeiServices,
                             PpiListSmBus
                             );
  return (Status);
}


