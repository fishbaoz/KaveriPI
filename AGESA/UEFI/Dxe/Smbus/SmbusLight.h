/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH SMBUS DXE Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH SMBUS DXE Driver
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

#ifndef _AMD_SMBUS_LIGHT_H_
#define _AMD_SMBUS_LIGHT_H_

#include "Tiano.h"
#include "EfiDriverLib.h "
#include "EfiHobLib.h"
#include "LinkedList.h"
#include "AMD.h"
#include "Fch.h"

#include EFI_ARCH_PROTOCOL_DEFINITION (Metronome)
#include EFI_PROTOCOL_DEFINITION (PciIo)
#include EFI_PROTOCOL_DEFINITION (PciRootBridgeIo)

#include EFI_PROTOCOL_PRODUCER (Smbus)

#define R_COMMAND                                           0x04 // PCI Command Register, 16bit
#define HSTS_ALL                                            0xFF // R/WC
#define B_HST_EN                                            0x01 // RW
#define B_SMB_SMI_EN                                        0x02 // RW
#define B_I2C_EN                                            0x04 // RW
#define AMDSMBUS_DEV_FUNC                                   0x140000ul
#define R_BASE_ADDRESS                                      0x10 // PCI BAR for SMBus I/O
#define R_BASE_ADDRESS1                                     0x90 // PCI BAR1 for SMbus I/O
#define B_IOSE                                              0x01 // RW
#define R_HOST_CONFIGURATION                                0xD2 // SMBus Host Configuration Register
// TickPeriod in 100ns so convert to us
#define STALL_TIME                                          1000000 // 1, 000, 000 microseconds = 1 second
#define HSTS                                                0x00 // Host Status Register R/W
#define INTR                                                0x02 // R/WC
#define BYTE_DONE_STS                                       0x80 // R/WC
#define DERR                                                0x04 // R/WC
#define BERR                                                0x08 // R/WC
// Declare a local instance structure for this driver
#define HBSY                                                0x01        // RO
#define FAIL                                                0x10        // R/WC
#define IUS                                                 0x40        // R/WC
#define START                                               0x40        // WO
#define SMB_CMD_QUICK                                       0x00 << 2
#define SMB_CMD_BYTE                                        0x01 << 2
#define SMB_CMD_BYTE_DATA                                   0x02 << 2
#define SMB_CMD_WORD_DATA                                   0x03 << 2
#define SMB_CMD_BLOCK                                       0x05 << 2
#define HCTL                                                0x02  // Host Control Register R/W
#define HCMD                                                0x03  // Host Command Register R/W
#define TSA                                                 0x04  // Transmit Slave Address Register R/W
#define HD0                                                 0x05  // Data 0 Register R/W
#define HD1                                                 0x06  // Data 1 Register R/W
#define HBD                                                 0x07  // Host Block Data Register R/W
#define SSTS                                                0x10  // Slave Status Register R/WC
#define HOST_NOTIFY_STS                                     0x01  // R/WC
#define NDA                                                 0x14  // Notify Device Address Register RO
#define NDLB                                                0x16  // Notify Data Low Byte Register RO
#define NDHB                                                0x17  // Notify Data High Byte Register RO
#define READ_ENABLE                                         0x01
#define NUM_RESERVED                                        21
#define TRY_TIMES                                           3
#define NOTIFYTIME                                          10000000
//   reserved by the SMBus spec.
#define ADDRESS_ARP                                         0xC2 >> 1
#define DATA_PREPARE_TO_ARP                                 0x01
#define DATA_RESET_DEVICE                                   0x02
#define DATA_GET_UDID_GENERAL                               0x03
#define DATA_ASSIGN_ADDRESS                                 0x04
#define SMBUS_R_SSTS                                        0x01    // Slave Status Register R/W
#define SMBUS_B_SSTS_ALL                                    0xFD    // R/WC
#define SMBUS_B_SBSY                                        0x01    // RO
#define SMBUS_B_SINIT                                       0x02    // R/WC
#define SMBUS_B_KILL                                        0x02    // R/WC

#define MAX_SMBUS_DEVICES                                   107 // Max number of SMBus devices

EFI_FORWARD_DECLARATION (SMBUS_INSTANCE);

typedef
UINT8
(EFIAPI *SMBUS_IO_READ) (
  IN SMBUS_INSTANCE                                         *Private,
  IN UINT8                                                  Offset
  );

typedef
VOID
(EFIAPI *SMBUS_IO_WRITE) (
  IN SMBUS_INSTANCE                                         *Private,
  IN UINT8                                                  Offset,
  IN UINT8                                                  Data
  );

typedef
BOOLEAN
(EFIAPI *SMBUS_IO_DONE) (
  IN SMBUS_INSTANCE                                         *Private,
  IN UINT8                                                  *StsReg
  );

///
/// SMBUS Instance
///
typedef struct _SMBUS_INSTANCE {
  UINTN                                     Signature;                      ///< Signature
  EFI_HANDLE                                Handle;                         ///< Handle
  UINT32                                    SmbusIoBase;                    ///< SMBUS IO base
  SMBUS_IO_READ                             SmbusIoRead;                    ///< SMBUS IO read
  SMBUS_IO_WRITE                            SmbusIoWrite;                   ///< SMBUS IO write
  SMBUS_IO_DONE                             IoDone;                         ///< IO Done flag
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL           *PciRootBridgeIo;               ///< Pointer to PciRootBridgeIo
  UINT32                                    StallTime;                      ///< Stall time
  //
  // Published interface
  //
  EFI_SMBUS_HC_PROTOCOL                     SmbusController;                ///< SMBUS controller protocol
  UINT8                                     DeviceMapEntries;               ///< Device map entries
  EFI_SMBUS_DEVICE_MAP                      DeviceMap [MAX_SMBUS_DEVICES];  ///< Device map array
  EFI_LIST                                  NotifyFunctionList;             ///< Notify function list
  EFI_EVENT                                 NotificationEvent;              ///< Notification event
} SMBUS_INSTANCE;


///
/// SMBUS Notify Function Structure
///
typedef struct _SMBUS_NOTIFY_FUNCTION_LIST_NODE {
  UINT32                                    Signature;         ///< Signature
  EFI_LIST_NODE                             Link;              ///< Link
  EFI_SMBUS_DEVICE_ADDRESS                  SlaveAddress;      ///< Slave address
  UINTN                                     Data;              ///< Data
  EFI_SMBUS_NOTIFY_FUNCTION                 NotifyFunction;    ///< Notify function
} SMBUS_NOTIFY_FUNCTION_LIST_NODE;

#define SMBUS_INSTANCE_FROM_THIS(a) \
  CR (a, \
      SMBUS_INSTANCE, \
      SmbusController, \
      SMBUS_INSTANCE_SIGNATURE \
      )

#define SMBUS_NOTIFY_FUNCTION_LIST_NODE_FROM_LINK(_node) \
  CR (_node, \
      SMBUS_NOTIFY_FUNCTION_LIST_NODE, \
      Link, \
      SMBUS_INSTANCE_SIGNATURE \
      )

#define SMBUS_INSTANCE_SIGNATURE  EFI_SIGNATURE_32 ('p', 's', 'm', 'b')

VOID
SetDataAndRegister (
  IN SMBUS_INSTANCE             *Smbus
  );

UINT8
SmbusIoRead (
  IN SMBUS_INSTANCE                                         *Private,
  IN UINT8                                                  Offset
  );

VOID
SmbusIoWrite (
  IN SMBUS_INSTANCE                                         *Private,
  IN UINT8                                                  Offset,
  IN UINT8                                                  Data
  );

BOOLEAN
IoDone (
  IN SMBUS_INSTANCE                                         *Private,
  IN UINT8                                                  *StsReg
  );

EFI_STATUS
SmbusExecute (
  IN EFI_SMBUS_HC_PROTOCOL                                  *This,
  IN EFI_SMBUS_DEVICE_ADDRESS                               SlaveAddress,
  IN EFI_SMBUS_DEVICE_COMMAND                               Command,
  IN EFI_SMBUS_OPERATION                                    Operation,
  IN BOOLEAN                                                PecCheck,
  IN OUT UINTN                                              *Length,
  IN OUT VOID                                               *Buffer
  );

EFI_STATUS
ProcessOperation (
  IN EFI_SMBUS_OPERATION                                    Operation,
  IN BOOLEAN                                                PecCheck,
  IN OUT UINTN                                              *Length,
  IN UINT8                                                  Len,
  OUT UINT8                                                 *SlaveAddressReg,
  OUT UINT8                                                 *HostCommandReg,
  OUT UINTN                                                 *SmbusCommand
  );

EFI_STATUS
SuccessfulCompletion (
  IN EFI_SMBUS_OPERATION                                    Operation,
  IN SMBUS_INSTANCE                                         *Smbus,
  IN UINT8                                                  *Buf,
  IN OUT UINTN                                              *Length
  );

EFI_STATUS
SmbusArpDevice (
  IN EFI_SMBUS_HC_PROTOCOL                                  *This,
  IN BOOLEAN                                                ArpAll,
  IN EFI_SMBUS_UDID                                         *SmbusUdid,
  IN OUT EFI_SMBUS_DEVICE_ADDRESS                           *SlaveAddress
  );

EFI_STATUS
GetUdidData (
  IN SMBUS_INSTANCE                                         *Smbus,
  IN OUT EFI_SMBUS_DEVICE_MAP                               *DeviceMap
  );

EFI_STATUS
GetNextDeviceAddress (
  IN SMBUS_INSTANCE                                         *Smbus,
  IN EFI_SMBUS_DEVICE_ADDRESS                               *DeviceAddress
  );

BOOLEAN
CheckDeviceAddress (
  IN SMBUS_INSTANCE                                         *Smbus,
  IN EFI_SMBUS_DEVICE_ADDRESS                               DeviceAddress
  );

EFI_STATUS
AssignAddress (
  IN SMBUS_INSTANCE                                         *Smbus,
  IN OUT EFI_SMBUS_DEVICE_MAP                               *DeviceMap
  );

EFI_STATUS
SmbusGetArpMap (
  IN EFI_SMBUS_HC_PROTOCOL                                  *This,
  IN OUT UINTN                                              *Length,
  IN OUT EFI_SMBUS_DEVICE_MAP                               **SmbusDeviceMap
  );

EFI_STATUS
SmbusNotify (
  IN EFI_SMBUS_HC_PROTOCOL                                  *This,
  IN EFI_SMBUS_DEVICE_ADDRESS                               SlaveAddress,
  IN UINTN                                                  Data,
  IN EFI_SMBUS_NOTIFY_FUNCTION                              NotifyFunction
  );

VOID
SmbusNotifyFunction (
  IN EFI_EVENT                                              Event,
  IN VOID                                                   *Context
  );
EFI_STATUS
CheckSlaveStatus (
  IN SMBUS_INSTANCE                                         *Smbus
  );

#endif
