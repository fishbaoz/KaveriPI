/* $NoKeywords:$ */
/**
 * @file
 *
 * HEST
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH WHEA DXE Driver
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

#ifndef _HEST_H_
#define _HEST_H_

#include "FchPlatform.h"

#pragma pack(1)

#define MAX_HEST_SIZE               0x8000
#define MAX_ERROR_BLOCK_SIZE        0x1000

#define ERROR_SEV_RECOVERABLE       0x00
#define ERROR_SEV_FATAL             0x01
#define ERROR_SEV_CORRECTED         0x02
#define ERROR_SEV_NONE              0x03

#define PCIE_ERROR_GUID \
  { 0xd995e954, 0xbbc1, 0x430f, 0xad, 0x91, 0xb4, 0x4d, 0xcb, 0x3c, 0x6f, 0x35 }

///
/// PCIE Error Section DEVICE_ID
///
typedef struct {
  UINT16                     VendorId;                               ///< Vendor ID
  UINT16                     DeviceId;                               ///< Device ID
  UINT8                      ClassCode[3];                           ///< Class Code
  UINT8                      Function;                               ///< Function
  UINT8                      Device;                                 ///< Device
  UINT16                     Segment;                                ///< Segment
  UINT8                      PrimaryBus;                             ///< Primary Bus
  UINT8                      SecondaryBus;                           ///< Secondary Bus
  UINT16                     Slot;                                   ///< Slot
  UINT8                      Reserved;                               ///< Reserved
} DEVICE_ID;

///
/// PCIE Error Section CAP_STRUCTURE
///
typedef struct {
  UINT32                     CapabilityData[15];                     ///< CapabilityData
} CAP_STRUCTURE;

///
/// PCIE Error Section AER_INFO
///
typedef struct {
  UINT32                     AerInfoData[24];                        ///< AerInfoData
} AER_INFO;

///
/// Generic Error Status Block
///
typedef struct {
  UINT32                     BlockStatus;                            ///< Block Status
  UINT32                     RawDataOffset;                          ///< Raw Data Offset
  UINT32                     RawDataLength;                          ///< Raw Data Length
  UINT32                     DataLength;                             ///< Error Data Length
  UINT32                     ErrorSeverity;                          ///< Error Severity
} GENERIC_ERROR_STATUS_BLOCK;

///
/// Generic Error Data Entry
///
typedef struct {
  EFI_GUID                   SectionType;                            ///< Section Type
  UINT32                     Severity;                               ///< Severity
  UINT16                     Revision;                               ///< Revision
  UINT8                      SecValidMask;                           ///< Section Valid Mask
  UINT8                      SectionFlags;                           ///< Section Flags
  UINT32                     DataLength;                             ///< Error Data Length
  EFI_GUID                   FruId;                                  ///< FRU Id
  CHAR8                      FruString[20];                          ///< FRU Text
} GENERIC_ERROR_DATA_ENTRY;

///
/// PCIE Error Section
///
typedef struct {
  UINT8                      Validation;                             ///< Validation Bits
  UINT8                      ResevedValidation[7];                   ///< Validation Bits
  UINT32                     PortType;                               ///< Port Type
  UINT32                     Revision;                               ///< Revision
  UINT32                     CommandStatus;                          ///< Command Status
  UINT32                     Reserved;                               ///< Reserved
  DEVICE_ID                  DeviceId;                               ///< Device Id
  UINT8                      SerialNum[8];                           ///< Serial Num
  UINT32                     BridgeCtrlStatus;                       ///< Bridge Control Status
  CAP_STRUCTURE              CapabilityStructure;                    ///< Capability Structure
  AER_INFO                   AerInfo;                                ///< AER Info
} PCIE_ERROR_SECTION;

///
/// HW Error notification structure
///
typedef struct {
  UINT8                      Type;                                   ///< Type
  UINT8                      Length;                                 ///< Length
  UINT16                     ConfigWriteEnbale;                      ///< Configuration Write Enbale
  UINT32                     PollInterval;                           ///< Poll Interval
  UINT32                     Vector;                                 ///< Vector
  UINT32                     PollingThreshold;                       ///< Polling Threshold
  UINT32                     PollingThresholdInterval;               ///< Polling Threshold Interval
  UINT32                     ErrorThreshold;                         ///< Error Threshold
  UINT32                     ErrorThresholdInterval;                 ///< Error Threshold Interval
} HW_NOTIFY_METHOD;

///
/// Error Source Structure
///
typedef struct {
  UINT16                     Type;                                   ///< Type
  UINT16                     SourceId;                               ///< SourceId
  UINT16                     Reserved;                               ///< Reserved
  UINT8                      Flags;                                  ///< Flags
  UINT8                      Enabled;                                ///< Enabled
  UINT32                     NumberofRecordsToPreAllocate;           ///< NumberofRecordsToPreAllocate
  UINT32                     MaxSectionsPerRecord;                   ///< MaxSectionsPerRecord
  UINT32                     Bus;                                    ///< Bus
  UINT16                     Device;                                 ///< Device
  UINT16                     Function;                               ///< Function
  UINT16                     DeviceControl;                          ///< DeviceControl
  UINT16                     Reserved2;                              ///< Reserved2
  UINT64                     UncorrectableErrorMask;                 ///< UncorrectableErrorMask
  UINT64                     UncorrectableErrorSeverity;             ///< UncorrectableErrorSeverity
  UINT64                     CorrectableErrorMask;                   ///< CorrectableErrorMask
  UINT64                     AdvancedErrorCapabilitiesAndControl;    ///< AdvancedErrorCapabilitiesAndControl
  UINT64                     RootErrorCommand;                       ///< RootErrorCommand
} ERROR_SOURCE_STRUCTURE_AER;

///
/// ERROR_SOURCE_STRUCTURE_GENERIC_HW_ERROR
///
typedef struct {
  UINT16                     Type;                                   ///< Type
  UINT16                     SourceId;                               ///< SourceId
  UINT16                     Reserved;                               ///< Reserved
  UINT8                      Flags;                                  ///< Flags
  UINT8                      Enabled;                                ///< Enabled
  UINT32                     NumberofRecordsToPreAllocate;           ///< NumberofRecordsToPreAllocate
  UINT32                     MaxSectionsPerRecord;                   ///< MaxSectionsPerRecord
  UINT32                     MaxRawDataLength;                       ///< MaxRawDataLength
  GENERIC_ADDRESS_STRUCTURE  ErrorStatusAddress;                     ///< ErrorStatusAddress
  HW_NOTIFY_METHOD           NotifyMethod;                           ///< NotifyMethod
  UINT32                     ErrorStatusSize;                        ///< ErrorStatusSize
} ERROR_SOURCE_STRUCTURE_GENERIC_HW_ERROR;

///
/// Hardware Error Source Table Header
///
typedef struct {
  UINT32                     Signature;                              ///< Signature
  UINT32                     Length;                                 ///< Length
  UINT8                      Revision;                               ///< Revision
  UINT8                      Checksum;                               ///< Checksum
  UINT8                      OemId[6];                               ///< OemId[6]
  UINT8                      OemTableId[8];                          ///< OemTableId[8]
  UINT8                      OemRevision[4];                         ///< OemRevision[4]
  UINT8                      CreatorId[4];                           ///< CreatorId[4]
  UINT8                      CreatorRevision[4];                     ///< CreatorRevision[4]
  UINT32                     ErrorSouceCount;                        ///< ErrorSouceCount
} HEST_HEADER;

#pragma pack()

#endif
