/**
 * @file
 *
 * AMD Integrated Debug Debug_library Routines
 *
 * Contains all functions related to HDTOUT
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  IDS
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 */
/*****************************************************************************
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

 /*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AGESA.h"
#include "Ids.h"
#include "IdsLib.h"
#include "amdlib.h"
#include "AMD.h"
#include "IdsDebugPrint.h"
#include "IdsDpRam.h"
#include "Filecode.h"
CODE_GROUP (G1_PEICC)
RDATA_GROUP (G1_PEICC)

#define FILECODE PROC_IDS_DEBUG_IDSDPRAM_FILECODE

/**
 *  Determine whether IDS console is enabled.
 *
 *
 *  @retval       TRUE    Alway return true
 *
 **/
BOOLEAN
AmdIdsDpRamSupport (
  VOID
  )
{

  return TRUE;
}


/**
 * Read Debug Print Memory
 *
 *
 * @param[in] AccessWidth   Access width
 * @param[in] MemAddress    Memory address
 * @param[in] Value         Pointer to data
 *
 */
VOID
STATIC
AmdIdsDpRamRead (
  IN       ACCESS_WIDTH AccessWidth,
  IN       UINT64 MemAddress,
     OUT   VOID *Value
  )
{
  LibAmdMemRead (AccessWidth, MemAddress, Value, NULL);
}

/**
 * Write Debug Print Memory
 *
 *
 * @param[in] AccessWidth   Access width
 * @param[in] MemAddress    Memory address
 * @param[in] Value         Pointer to data
 *
 */
VOID
STATIC
AmdIdsDpRamWrite (
  IN       ACCESS_WIDTH AccessWidth,
  IN       UINT64 MemAddress,
     OUT   VOID *Value
  )
{
  LibAmdMemWrite (AccessWidth, MemAddress, Value, NULL);
}

/**
 *  get debug print latest Index
 *
 *  @param[in,out] LatestIndex  - LatestIndex to be filled
 *
 *  @retval       AGESA_ERROR    Debug Ram region is read only
 *  @retval       AGESA_SUCCESS  Successfully get the Latest Index

 *
**/
AGESA_STATUS
STATIC
AmdIdsDpRamGetLatestIndex (
  IN OUT   UINT32 *LatestIndex
  )
{
  UINT32 i;
  UINT32 _LatestIndex;
  SIGNATURE DpRamSig;
  UINT8 DpBufferDftValue;

  ASSERT (LatestIndex != NULL);
  ASSERT (IDS_DPRAM_BASE != 0);
  AmdIdsDpRamRead (AccessWidth32, DEBUG_PRINT_RAM_SIG_ADDR, &DpRamSig);

  //Check Signature, if uninit means it is 1st time comes here
  if (DpRamSig != DEBUG_PRINT_RAM_SIG) {
    //Init Debug Print RAM Header
    DpRamSig = DEBUG_PRINT_RAM_SIG;
    AmdIdsDpRamWrite (AccessWidth32, DEBUG_PRINT_RAM_SIG_ADDR, &DpRamSig);
    //Check if read only memory
    DpRamSig = 0;
    AmdIdsDpRamRead (AccessWidth32, DEBUG_PRINT_RAM_SIG_ADDR, &DpRamSig);
    if (DpRamSig != DEBUG_PRINT_RAM_SIG) {
      return AGESA_ERROR;
    }

    //Init Latest Index with zero
    _LatestIndex = 0;
    AmdIdsDpRamWrite (AccessWidth32, DEBUG_PRINT_RAM_LATESTIDX_ADDR, &_LatestIndex);
    //Init Debug Print Buffer with defalut value
    ASSERT (IDS_DPRAM_SIZE != 0);
    DpBufferDftValue = DEBUG_PRINT_BUFFER_DFT_VALUE;
    for (i = 0; i < DEBUG_PRINT_BUFFER_SIZE; i++) {
      AmdIdsDpRamWrite (AccessWidth8, DEBUG_PRINT_BUFFER_START + i, &DpBufferDftValue);
    }
  }
  AmdIdsDpRamRead (AccessWidth32, DEBUG_PRINT_RAM_LATESTIDX_ADDR, LatestIndex);
  return AGESA_SUCCESS;
}

/**
 *  Get DpRam customize Filter
 *
 *  @param[in,out] Filter    Filter do be filled
 *
 *  @retval       FALSE    Alway return FALSE
 *
 **/
BOOLEAN
AmdIdsDpRamGetFilter (
  IN OUT   UINT64 *Filter
  )
{
  return FALSE;
}

/**
 *  Init local private data
 *
 *  @param[in] Flag    - filter flag
 *  @param[in] debugPrintPrivate  - Point to debugPrintPrivate
 *
**/
VOID
AmdIdsDpRamInitPrivateData (
  IN      UINT64 Flag,
  IN      IDS_DEBUG_PRINT_PRIVATE_DATA *debugPrintPrivate
  )
{

}
/**
 *  Get a record's size which end of '\0'
 *
 *  @param[in] RecordAddr  - The address of a record
 *
 *  @retval    RecordSize    Size of a record
**/
UINT32
STATIC
AmdIdsDpRamGetRecordSize (
  IN      UINT64 RecordAddr
  )
{
  UINT32 RecordSize;
  UINT8 BufValue8;
  RecordSize = 0;
  do {
    AmdIdsDpRamRead (AccessWidth8, RecordAddr++, &BufValue8);
    RecordSize++;
  } while (BufValue8 != DEBUG_STRING_END_FLAG);

  return RecordSize;
}
/**
 *  Clean up the Debug Print buffer, remove oldest record, do the relocation
 *
 *  @param[in] NewRecordLength  - The Length of new record
 *  @param[in,out] LatestIndex  - LatestIndex to be Updated
 *
**/
VOID
STATIC
AmdIdsDpRamCleanUp (
  IN       UINTN NewRecordLength,
  IN OUT   UINT32 *LatestIndex
  )
{
  UINT32 SizeRemoved;
  UINT32 RemainedSize;
  UINT32 RecordOffset;
  UINT32 RecordSize;
  UINT8  Value8;
  UINT32 i;

  SizeRemoved = 0;
  RecordOffset = 0;
  RemainedSize = DEBUG_PRINT_BUFFER_SIZE - *LatestIndex;
  //Calculate How many record need be removed
  while (RemainedSize < NewRecordLength) {
    RecordSize = AmdIdsDpRamGetRecordSize (DEBUG_PRINT_BUFFER_START + RecordOffset);
    RecordOffset += RecordSize;
    RemainedSize += RecordSize;
  }
  //Move forward (RecordOffset) byte data
  for (i = 0; i < (*LatestIndex - RecordOffset); i++) {
    AmdIdsDpRamRead (AccessWidth8, DEBUG_PRINT_BUFFER_START + RecordOffset + i, &Value8);
    AmdIdsDpRamWrite (AccessWidth8, DEBUG_PRINT_BUFFER_START + i, &Value8);
  }
  *LatestIndex -= RecordOffset;
  //Fill LatestIndex ~ End with default value
  for (i = 0; i < (DEBUG_PRINT_BUFFER_SIZE - *LatestIndex); i++) {
    Value8 = DEBUG_PRINT_BUFFER_DFT_VALUE;
    AmdIdsDpRamWrite (AccessWidth8, DEBUG_PRINT_BUFFER_START + *LatestIndex + i, &Value8);
  }
}

/**
 *  Print formated string
 *
 *  @param[in] Buffer  - Point to input buffer
 *  @param[in] BufferSize  - Buffer size
 *  @param[in] debugPrintPrivate  - Option
 *
**/
VOID
AmdIdsDpRamPrint (
  IN      CHAR8   *Buffer,
  IN      UINTN BufferSize,
  IN      IDS_DEBUG_PRINT_PRIVATE_DATA *debugPrintPrivate
  )
{
  UINT32 LatestIndex;
  UINT32 Counter;
  UINT64 DebugPrintBufferAddr;
  UINT32 _BufferSize;
  UINT32 StopLoggingWhenBufferFull;

  ASSERT (BufferSize <= DEBUG_PRINT_BUFFER_SIZE);
  //Get Latest Index
  if (AmdIdsDpRamGetLatestIndex (&LatestIndex) == AGESA_SUCCESS) {
    //Add the size for '\0'
    _BufferSize = (UINT32) BufferSize + 1;
    StopLoggingWhenBufferFull = IDS_DPRAM_STOP_LOGGING_WHEN_BUFFER_FULL;
    //Check if exceed the limit, if so shift the oldest data, and do the relocation
    if (((LatestIndex + _BufferSize) > DEBUG_PRINT_BUFFER_SIZE) &&
        !StopLoggingWhenBufferFull) {
      AmdIdsDpRamCleanUp (_BufferSize, &LatestIndex);
    }
    if ((LatestIndex + _BufferSize) <= DEBUG_PRINT_BUFFER_SIZE) {
      //Save the data to RAM, Update the Latest Index
      Counter = _BufferSize;
      DebugPrintBufferAddr = (UINT64) (DEBUG_PRINT_BUFFER_START + LatestIndex);
      while (Counter--) {
        AmdIdsDpRamWrite (AccessWidth8, DebugPrintBufferAddr++, Buffer++);
      }
      LatestIndex += _BufferSize;
      AmdIdsDpRamWrite (AccessWidth32, DEBUG_PRINT_RAM_LATESTIDX_ADDR, &LatestIndex);
    }
  }
}


CONST IDS_DEBUG_PRINT ROMDATA  IdsDebugPrintRamInstance =
{
  AmdIdsDpRamSupport,
  AmdIdsDpRamGetFilter,
  AmdIdsDpRamInitPrivateData,
  AmdIdsDpRamPrint
};



