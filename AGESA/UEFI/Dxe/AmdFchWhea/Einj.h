/* $NoKeywords:$ */
/**
 * @file
 *
 * EINJ
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

#ifndef _EINJ_H_
#define _EINJ_H_

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
#include "FchPlatform.h"

#pragma pack(1)

#define MAX_EINJ_SIZE                           0x2000
//
// Error Injection Actions
//
#define BEGIN_INJECTION_OPERATION               0x00
#define GET_TRIGGER_ERROR_ACTION_TABLE          0x01
#define SET_ERROR_TYPE                          0x02
#define GET_ERROR_TYPE                          0x03
#define END_OPERATION                           0x04
#define EXECUTE_OPERATION                       0x05
#define CHECK_BUSY_STATUS                       0x06
#define GET_COMMAND_STATUS                      0x07
#define TRIGGER_ERROR                           0xFF

#define WHEA_EINJ_CMD_SUCCESS                   0x00
#define WHEA_EINJ_CMD_UNKNOWN_FAILURE           0x01
#define WHEA_EINJ_CMD_INVALID_ACCESS            0x02


//
// Error Type Definition for SET_ERROR_TYPE Injection action
//
#define PROCESSOR_CORRECTABLE                   BIT0
#define PROCESSOR_UNCORRECTABLE_NON_FATAL       BIT1
#define PROCESSOR_UNCORRECTABLE_FATAL           BIT2
#define MEMORY_CORRECTABLE                      BIT3
#define MEMORY_UNCORRECTABLE_NON_FATAL          BIT4
#define MEMORY_UNCORRECTABLE_FATAL              BIT5
#define PCI_EXPRESS_CORRECTABLE                 BIT6
#define PCI_EXPRESS_UNCORRECTABLE_NON_FATAL     BIT7
#define PCI_EXPRESS_UNCORRECTABLE_FATAL         BIT8
#define PLATFORM_CORRECTABLE                    BIT9
#define PLATFORM_UNCORRECTABLE_NON_FATAL        BIT10
#define PLATFORM_UNCORRECTABLE_FATAL            BIT11

//
// Injection Instructions
//
#define READ_REGISTER                           0x00
#define READ_REGISTER_VALUE                     0x01
#define WRITE_REGISTER                          0x02
#define WRITE_REGISTER_VALUE                    0x03
#define NOOP                                    0x04


//
// Generic Address Structure definitions
//
#define SYSTEM_MEMORY                           0x00
#define SYSTEM_IO                               0x01
#define PCI_CONFIGURATION_SPACE                 0x02
#define EMBEDDED_CONTROLLER                     0x03
#define SMBUS                                   0x04
#define UNDEFINED                               0x00
#define BYTE_ACCESS                             0x01
#define WORD_ACCESS                             0x02
#define DWORD_ACCESS                            0x03
#define QWORD_ACCESS                            0x04

///
/// Generic Address Structure
///
typedef struct {
  UINT8   AddressSpaceID;                  ///< Address space ID
  UINT8   RegisterBitWidth;                ///< Register bit width
  UINT8   RegisterBitOffset;               ///< Register bit offset
  UINT8   AccessSize;                      ///< Access size
  UINT64  Address;                         ///< Address
} GENERIC_ADDRESS_STRUCTURE;


///
/// ACPI Error Injection Standard Header
///
typedef struct {
  UINT32  Signature;                       ///< Signature
  UINT32  Length;                          ///< Length
  UINT8   Revision;                        ///< Revision
  UINT8   Checksum;                        ///< Checksum
  UINT8   OemId[6];                        ///< OEM ID
  UINT8   OemTableId[8];                   ///< OEM Table ID
  UINT8   OemRevision[4];                  ///< OEM revision
  UINT8   CreatorId[4];                    ///< Creator ID
  UINT8   CreatorRevision[4];              ///< Creator revision
} ACPI_EINJ_STANDARD_HEADER;

///
/// Injection Header
///
typedef struct {
  UINT32  InjectionHeaderSize;             ///< Injection header size
  UINT8   InjectionFlags;                  ///< Injection flags
  UINT8   Reserved[3];                     ///< Reserved
  UINT32  InjectionInstructionEntryCount;  ///< Injection instruction entry count
} EINJ_HEADER;

///
/// Injection Instruction Entry
///
typedef struct {
  UINT8   InjectionAction;                 ///< Injection action
  UINT8   Instruction;                     ///< Instruction
  UINT8   Flags;                           ///< Flags
  UINT8   Reserved;                        ///< Reserved
  GENERIC_ADDRESS_STRUCTURE RegisterRegion;///< Register region
  UINT64  Value;                           ///< Value
  UINT64  Mask;                            ///< Mask
} EINJ_INSTRUCTION_ENTRY;

///
/// Trigger Error Header
///
typedef struct {
  UINT32  TriggerErrorHeaderSize;          ///< Trigger error header size
  UINT32  Revision;                        ///< Revision
  UINT32  EntireTriggerErrorTableSize;     ///< Entire trigger error table size
  UINT32  TriggerErrorEntryCount;          ///< Trigger error entry count
} TRIGGER_ERROR_HEADER;

///
/// Trigger Error Instruction Entry
///
typedef struct {
  UINT8   InjectionAction;                 ///< Injection action
  UINT8   Instruction;                     ///< Instruction
  UINT8   Flags;                           ///< Flags
  UINT8   Reserved;                        ///< Reserved
  GENERIC_ADDRESS_STRUCTURE RegisterRegion;///< Register region
  UINT64  Value;                           ///< Value
  UINT64  Mask;                            ///< Mask
} TRIGGER_ERROR_INSTRUCTION_ENTRY;

#pragma pack()

#endif
