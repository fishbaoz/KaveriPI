/* $NoKeywords:$ */
/**
 * @file
 *
 * mruc.c
 *
 * Utility functions recovery
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Proc/Recovery/Mem)
 * @e \$Revision: 281181 $ @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 **/
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
* ***************************************************************************
*
*/

/*
 *----------------------------------------------------------------------------
 *                                MODULES USED
 *
 *----------------------------------------------------------------------------
 */


#include "AGESA.h"
#include "OptionMemory.h"
#include "PlatformMemoryConfiguration.h"
#include "amdlib.h"
#include "Ids.h"
#include "mrport.h"
#include "mru.h"
#include "Filecode.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)

#define FILECODE PROC_RECOVERY_MEM_MRUC_FILECODE
/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                           TYPEDEFS AND STRUCTURES
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

VOID
STATIC
MemRecUResetTargetWTIO (
  IN OUT   MEM_DATA_STRUCT *MemPtr
  );

VOID
STATIC
MemRecUSetTargetWTIO (
  IN       UINT32 Address,
  IN OUT   MEM_DATA_STRUCT *MemPtr
  );


/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
/* -----------------------------------------------------------------------------*/
/**
 *
 *      This function returns the (Index)th UINT8
 *   from an indicated test Pattern.
 *
 *     @param[in] Pattern - encoding of test Pattern type
 *     @param[in] Buffer[] - buffer to be filled
 *     @param[in] Size - Size of the bugger
 *     @param[in] *StdHeader - pointer to AMD_CONFIG_PARAMS
 *
 */

VOID
MemRecUFillTrainPattern (
  IN       TRAIN_PATTERN Pattern,
  IN       UINT8 Buffer[],
  IN       UINT16 Size,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  STATIC UINT8 PatternData[2] = {0x55, 0xAA};

  LibAmdMemFill (Buffer, PatternData[Pattern == TestPattern0 ? TestPattern1 : TestPattern0], Size, StdHeader);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *      This function flushes cache lines
 *
 *     @param[in] Address - System Address [39:8]
 *     @param[in,out] *MemPtr - Pointer to MEM_DATA_STRUCT
 *
 */

VOID
MemRecUProcIOClFlush (
  IN       UINT32 Address,
  IN OUT   MEM_DATA_STRUCT *MemPtr
  )
{
  MemRecUSetTargetWTIO (Address, MemPtr);
  MemRecUFlushPattern (Address);
  MemRecUResetTargetWTIO (MemPtr);
}

/*----------------------------------------------------------------------------
 *                              LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *      This function resets the target address space to Write Through IO by disabling IORRs
 */

VOID
STATIC
MemRecUResetTargetWTIO (
  IN OUT   MEM_DATA_STRUCT *MemPtr
  )
{
  S_UINT64 Smsr;
  Smsr.hi = 0;
  Smsr.lo = 0;
  LibAmdMsrWrite (IORR0_MASK, (UINT64 *)&Smsr, &MemPtr->StdHeader);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *       This function sets the target range to WT IO (using an IORR overlapping
 *       the already existing
 *      @param[in,out] *MemPtr - pointer to MEM_DATA_STRUCTURE
 *      @param[in] Address - System Address
 *
 */

VOID
STATIC
MemRecUSetTargetWTIO (
  IN       UINT32 Address,
  IN OUT   MEM_DATA_STRUCT *MemPtr
  )
{
  S_UINT64 SMsr;

  SMsr.lo = Address;
  SMsr.hi = 0;
  LibAmdMsrWrite (IORR0_BASE,(UINT64 *)&SMsr, &MemPtr->StdHeader);          // ;IORR0 Base
  SMsr.hi = 0xFFFF;
  SMsr.lo = 0xFC000800;
  LibAmdMsrWrite (IORR0_MASK, (UINT64 *)&SMsr, &MemPtr->StdHeader);          // ;64MB Mask
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  Waits specified number of 10ns cycles
 *      @param[in,out] MemPtr - pointer to MEM_DATA_STRUCTURE
 *      @param[in] Count - Number of 10ns cycles to wait
 *
 */

VOID
MemRecUWait10ns (
  IN       UINT32 Count,
  IN OUT   MEM_DATA_STRUCT *MemPtr
  )
{
  S_UINT64 SMsr;

  LibAmdMsrRead (TSC, (UINT64 *)&SMsr, &MemPtr->StdHeader);
  Count += SMsr.lo;
  while (SMsr.lo < Count) {
    LibAmdMsrRead (TSC, (UINT64 *)&SMsr, &MemPtr->StdHeader);
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *      Find the entry of platform specific overriding table.
 *
 *      @param[in] PlatformMemoryConfiguration - Platform config table
 *      @param[in] EntryType - Entry type
 *      @param[in] SocketID - Physical socket ID
 *      @param[in] ChannelID - Physical channel ID
 *      @param[in] DimmID - Physical Dimm ID
 *
 *      @return  NULL - entry could not be found.
 *      @return  Pointer - points to the entry's data.
 *
 * ----------------------------------------------------------------------------
 */
VOID *
MemRecFindPSOverrideEntry (
  IN       PSO_TABLE *PlatformMemoryConfiguration,
  IN       PSO_ENTRY EntryType,
  IN       UINT8 SocketID,
  IN       UINT8 ChannelID,
  IN       UINT8 DimmID
  )
{
  UINT8 *Buffer;

  Buffer = PlatformMemoryConfiguration;
  while (Buffer[0] != PSO_END) {
    if (Buffer[0] == EntryType) {
      if ((Buffer[2] & ((UINT8) 1 << SocketID)) != 0 ) {
        if ((Buffer[3] & ((UINT8) 1 << ChannelID)) != 0 ) {
          ASSERT ((Buffer[0] == PSO_MAX_DIMMS) ? (Buffer[5] <= MAX_DIMMS_PER_CHANNEL) : TRUE);
          ASSERT ((Buffer[0] == PSO_MAX_CHIPSELS) ? (Buffer[5] <= MAX_CS_PER_CHANNEL) : TRUE);
          ASSERT ((Buffer[0] == PSO_MAX_CHNLS) ? (Buffer[5] <= MAX_CHANNELS_PER_SOCKET) : TRUE);
          if ((Buffer[4] & ((UINT8) 1 << DimmID)) != 0 ) {
            return &Buffer[5];
          }
        }
      }
    }
    Buffer += Buffer[1] + 2;
  }
  return NULL;
}

