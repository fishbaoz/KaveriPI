/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPU APIC related utility functions and structures
 *
 * Contains code that provides mechanism to invoke and control APIC communication.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
 ******************************************************************************
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

#ifndef _CPU_APIC_UTILITIES_H_
#define _CPU_APIC_UTILITIES_H_


/*---------------------------------------------------------------------------------------
 *          M I X E D   (Definitions And Macros / Typedefs, Structures, Enums)
 *---------------------------------------------------------------------------------------
 */


/*---------------------------------------------------------------------------------------
 *                 D E F I N I T I O N S     A N D     M A C R O S
 *---------------------------------------------------------------------------------------
 */
#define APIC_CTRL_DWORD 0xF
#define APIC_CTRL_REG (APIC_CTRL_DWORD << 4)
#define APIC_CTRL_MASK 0xFF
#define APIC_CTRL_SHIFT 0

#define APIC_DATA_DWORD 0x38
#define APIC_DATA_REG (APIC_DATA_DWORD << 4)

#define APIC_REMOTE_READ_REG 0xC0
#define APIC_CMD_LO_REG 0x300
#define APIC_CMD_HI_REG 0x310

// APIC_CMD_LO_REG bits
#define CMD_REG_DELIVERY_STATUS 0x1000
#define CMD_REG_TO_READ 0x300
#define CMD_REG_REMOTE_RD_STS_MSK 0x30000ul
#define CMD_REG_REMOTE_DELIVERY_PENDING 0x10000ul
#define CMD_REG_REMOTE_DELIVERY_DONE 0x20000ul
#define CMD_REG_TO_NMI 0x400

// ExeFlags bits
#define WAIT_FOR_CORE     0x00000001ul
#define TASK_HAS_OUTPUT   0x00000002ul
#define RETURN_PARAMS     0x00000004ul
#define END_AT_HLT        0x00000008ul
#define PASS_EARLY_PARAMS 0x00000010ul

// Control Byte Values
// bit 7 indicates the type of message
// 1 - control message
// 0 - launch + APIC ID = message to go
//
#define CORE_UNAVAILABLE            0xFF
#define CORE_IDLE                   0xFE
#define CORE_IDLE_HLT               0xFD
#define CORE_ACTIVE                 0xFC
#define CORE_NEEDS_PTR              0xFB
#define CORE_NEEDS_DATA_SIZE        0xFA
#define CORE_STS_DATA_READY_1       0xF9
#define CORE_STS_DATA_READY_0       0xF8
#define CORE_DATA_FLAGS_READY       0xF7
#define CORE_DATA_FLAGS_ACKNOWLEDGE 0xF6
#define CORE_DATA_PTR_READY         0xF5

// Macro used to determine the number of dwords to transmit to the AP as input
#define SIZE_IN_DWORDS(sInput) ((UINT32) (((sizeof (sInput)) + 3) >> 2))

// IDT table
#define IDT_DESC_PRESENT     0x80

#define IDT_DESC_TYPE_LDT    0x02
#define IDT_DESC_TYPE_CALL16 0x04
#define IDT_DESC_TYPE_TASK   0x05
#define IDT_DESC_TYPE_INT16  0x06
#define IDT_DESC_TYPE_TRAP16 0x07
#define IDT_DESC_TYPE_CALL32 0x0C
#define IDT_DESC_TYPE_INT32  0x0E
#define IDT_DESC_TYPE_TRAP32 0x0F
/*---------------------------------------------------------------------------------------
 *               T Y P E D E F S,   S T R U C T U R E S,    E N U M S
 *---------------------------------------------------------------------------------------
 */
typedef VOID (*PF_AP_TASK) (AMD_CONFIG_PARAMS *StdHeader);
typedef VOID (*PF_AP_TASK_I) (VOID *, AMD_CONFIG_PARAMS *StdHeader);
typedef VOID (*PF_AP_TASK_C) (AMD_CONFIG_PARAMS *StdHeader, AMD_CPU_EARLY_PARAMS *);
typedef VOID (*PF_AP_TASK_IC) (VOID *, AMD_CONFIG_PARAMS *StdHeader, AMD_CPU_EARLY_PARAMS *);
typedef UINT32 (*PF_AP_TASK_O) (AMD_CONFIG_PARAMS *StdHeader);
typedef UINT32 (*PF_AP_TASK_IO) (VOID *, AMD_CONFIG_PARAMS *StdHeader);
typedef UINT32 (*PF_AP_TASK_OC) (AMD_CONFIG_PARAMS *StdHeader, AMD_CPU_EARLY_PARAMS *);
typedef UINT32 (*PF_AP_TASK_IOC) (VOID *, AMD_CONFIG_PARAMS *StdHeader, AMD_CPU_EARLY_PARAMS *);

/// Function pointer union representing the eight different
/// types of functions that an AP can be asked to perform.
typedef union {
  PF_AP_TASK     PfApTask;      ///< AMD_CONFIG_PARAMS *  input with no output
  PF_AP_TASK_I   PfApTaskI;     ///< VOID * + AMD_CONFIG_PARAMS *  input with no output
  PF_AP_TASK_C   PfApTaskC;     ///< AMD_CONFIG_PARAMS * + AMD_CPU_EARLY_PARAMS *  input with no output
  PF_AP_TASK_IC  PfApTaskIC;    ///< VOID * + AMD_CONFIG_PARAMS * + AMD_CPU_EARLY_PARAMS *  input with no output
  PF_AP_TASK_O   PfApTaskO;     ///< AMD_CONFIG_PARAMS * input with UINT32 output
  PF_AP_TASK_IO  PfApTaskIO;    ///< VOID * + AMD_CONFIG_PARAMS * input with UINT32 output
  PF_AP_TASK_OC  PfApTaskOC;    ///< AMD_CONFIG_PARAMS * + AMD_CPU_EARLY_PARAMS * input with UINT32 output
  PF_AP_TASK_IOC PfApTaskIOC;   ///< VOID * + AMD_CONFIG_PARAMS * + AMD_CPU_EARLY_PARAMS *  input with UINT32 output
} AP_FUNCTION_PTR;

/// Input structure for ApUtilTransmitBuffer and ApUtilReceiveBuffer
/// containing information about the data transfer from one core
/// to another.
typedef struct {
  IN OUT   UINT16 DataSizeInDwords;   ///< Size of the data to be transferred rounded up to the nearest dword
  IN OUT   VOID   *DataPtr;           ///< Pointer to the data
  IN       UINT32 DataTransferFlags;  ///< Flags dictating certain aspects of the data transfer
} AP_DATA_TRANSFER;

/// Input structure for ApUtilRunCodeOnSocketCore.
typedef struct _AP_TASK {
  AP_FUNCTION_PTR  FuncAddress;   ///< Pointer to the function that the AP will run
  AP_DATA_TRANSFER DataTransfer;  ///< Data transfer struct for optionally passing data that the AP should use as input to the function
  UINT32           ExeFlags;      ///< Flags dictating certain aspects of the AP tasking sequence
} AP_TASK;

/// Input structure for ApUtilWaitForCoreStatus.
typedef struct {
  IN       UINT8  *Status;            ///< Pointer to the 1st element of an array of values to wait for
  IN       UINT8  NumberOfElements;   ///< Number of elements in the array
  IN       UINT32 RetryCount;         ///< Number of remote read cycles to complete before quitting
  IN       UINT32 WaitForStatusFlags; ///< Flags dictating certain aspects of ApUtilWaitForCoreStatus
} AP_WAIT_FOR_STATUS;

/// Interrupt Descriptor Table entry
typedef struct {
  UINT16 OffsetLo;     ///< Lower 16 bits of the interrupt handler routine's offset
  UINT16 Selector;     ///< Interrupt handler routine's selector
  UINT8  Rsvd;         ///< Reserved
  UINT8  Flags;        ///< Interrupt flags
  UINT16 OffsetHi;     ///< Upper 16 bits of the interrupt handler routine's offset
  UINT32 Offset64;     ///< High order 32 bits of the handler's offset needed when in 64 bit mode
  UINT32 Rsvd64;       ///< Reserved
} IDT_DESCRIPTOR;

/// Structure needed to load the IDTR using the lidt instruction
typedef struct {
  UINT16 Limit;        ///< Interrupt Descriptor Table size
  UINT64 Base;         ///< Interrupt Descriptor Table base address
} IDT_BASE_LIMIT;

#define WAIT_STATUS_EQUALITY 0x00000001ul
#define WAIT_INFINITELY 0

// Data Transfer Flags
#define DATA_IN_MEMORY 0x00000001ul


/*---------------------------------------------------------------------------------------
 *                        F U N C T I O N    P R O T O T Y P E
 *---------------------------------------------------------------------------------------
 */
//                   These are   P U B L I C   functions, used by AGESA
UINT8
ApUtilReadRemoteControlByte (
  IN       UINT32 TargetApicId,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

VOID
ApUtilWriteControlByte (
  IN       UINT8 Value,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

UINT32
ApUtilReadRemoteDataDword (
  IN       UINT32 TargetApicId,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

VOID
ApUtilWriteDataDword (
  IN       UINT32 Value,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

UINT32
ApUtilRunCodeOnSocketCore (
  IN       UINT8 Socket,
  IN       UINT8 Core,
  IN       AP_TASK *TaskPtr,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

UINT8
ApUtilWaitForCoreStatus (
  IN       UINT32 TargetApicId,
  IN       AP_WAIT_FOR_STATUS *WaitParamsPtr,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

VOID
ApEntry (
  IN       AMD_CONFIG_PARAMS *StdHeader,
  IN       AMD_CPU_EARLY_PARAMS *CpuEarlyParams
  );

UINT32
ApUtilTaskOnExecutingCore (
  IN       AP_TASK *TaskPtr,
  IN       AMD_CONFIG_PARAMS *StdHeader,
  IN       VOID    *ConfigParams
  );

VOID
ApUtilTransmitBuffer (
  IN     UINT8   Socket,
  IN     UINT8   Core,
  IN     AP_DATA_TRANSFER *BufferInfo,
  IN     AMD_CONFIG_PARAMS *StdHeader
  );

AGESA_STATUS
ApUtilReceiveBuffer (
  IN       UINT8   Socket,
  IN       UINT8   Core,
  IN OUT   AP_DATA_TRANSFER  *BufferInfo,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

VOID
GetLocalApicIdForCore (
  IN       UINT32 TargetSocket,
  IN       UINT32 TargetCore,
     OUT   UINT32 *LocalApicId,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

VOID
ApUtilRunCodeOnAllLocalCoresAtEarly (
  IN       AP_TASK *TaskPtr,
  IN       AMD_CONFIG_PARAMS *StdHeader,
  IN       AMD_CPU_EARLY_PARAMS *CpuEarlyParamsPtr
  );

VOID
RelinquishControlOfAllAPs (
  IN       AMD_CONFIG_PARAMS   *StdHeader
  );

VOID
GetCsSelector (
  IN       UINT16 *Selector,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

VOID
SetIdtr (
  IN       IDT_BASE_LIMIT *IdtInfo,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

VOID
GetIdtr (
  IN       IDT_BASE_LIMIT *IdtInfo,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

#endif  /* _CPU_APIC_UTILITIES_H_ */
