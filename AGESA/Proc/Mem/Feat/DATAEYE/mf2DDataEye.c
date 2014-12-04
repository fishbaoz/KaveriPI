/* $NoKeywords:$ */
/**
 * @file
 *
 * mf2DDataEye.c
 *
 * DQS 2 Dimensional Training Data Eye Storage and reporting
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/Tech)
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
#include "amdlib.h"
#include "AdvancedApi.h"
#include "GeneralServices.h"
#include "Ids.h"
#include "heapManager.h"
#include "mm.h"
#include "mn.h"
#include "mu.h"
#include "mt.h"
#include "mport.h"
#include "merrhdl.h"
#include "Filecode.h"
#include "OptionMemory.h"
#include "cpuServices.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)

#define FILECODE PROC_MEM_FEAT_DATAEYE_MF2DDATAEYE_FILECODE
/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */

#define MAX_DELAYS    9   /* 8 data bytes + 1 ECC byte */
#define MAX_DATAEYES_PER_CHANNEL   (MAX_CS_PER_CHANNEL / 2) ///< Maximum number of Data Eyes that will
                                                            ///<  be stored per channel.
//
// DATA EYE DEFINITIONS
//
#define NUMBER_OF_DATAEYE_BITMAPS_PER_CHANNEL 4 ///< Maximum number of Data Eye Bitmaps
#define DATAEYE_VREF_RANGE 31 ///< Number of VREF steps recorded in Data Eye Bitmap
#define DATAEYE_RD_DQS_DELAY_RANGE 32 ///< Number of read DQS delay steps recorded in Data Eye Bitmap

// RLE DEFINITIONS
#define RLE_XDIR 0
#define RLE_YDIR 1

/*----------------------------------------------------------------------------
 *                           TYPEDEFS AND STRUCTURES
 *
 *----------------------------------------------------------------------------
 */

/// RLE Data Byte Definition
typedef struct _DATAEYE_RLE_BYTE {
     OUT UINT8 RunCount:7;
     OUT UINT8 Bit:1;
} DATAEYE_RLE_BYTE;

/// RLE Data Eye Bitmap Format
typedef struct _DATAEYE_RLE_HEADER {
     OUT UINT8 Height;
     OUT UINT8 Width;
     OUT UINT8 Encoding;
} DATA_EYE_RLE_HEADER;

/// List of Data Eyes for a channel
typedef struct _DATAEYE_CH_HEADER {
     OUT UINT16 Size;
     OUT UINT16 ImageOffset[NUMBER_OF_DATAEYE_BITMAPS_PER_CHANNEL];
} DATAEYE_CH_HEADER;

typedef struct _DATAEYE_BUFFER {
     OUT UINT8             EyeCount;
     OUT BOOLEAN           Present[MAX_DATAEYES_PER_CHANNEL];
     OUT COMPOSITE_DATAEYE DataEye[MAX_DATAEYES_PER_CHANNEL];
} DATAEYE_BUFFER;

/*----------------------------------------------------------------------------
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
BOOLEAN
STATIC
MemF2DDataEyeSaveCompositeEyes (
  IN OUT   MEM_TECH_BLOCK *TechPtr,
  IN       VOID *Mem2DEntryData
  );

BOOLEAN
STATIC
MemF2DDataEyeCompressEyes (
  IN OUT   MEM_TECH_BLOCK *TechPtr,
  IN       VOID *OptParam
  );

INT8
STATIC
MemFGetEyeCenter (
  IN       MEM_2D_ENTRY* Data,
  IN       UINT8 Lane
  );

UINT8
STATIC
DataEyeEncodeRLE (
     OUT   UINT8 *Output,
  IN       UINT32 *Input
  );

UINT16
STATIC
DataEyeDecodeRLE (
     OUT UINT32 *Output,
  IN     UINT8 *Input
);

/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *   This function initializes the 2D Data Eye functions.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

BOOLEAN
MemF2DDataEyeInit (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MEM_PARAMETER_STRUCT *RefPtr;
  RefPtr = NBPtr->RefPtr;

  if (RefPtr->DataEyeEn) {
    ASSERT (NBPtr != NULL);
    ASSERT (NBPtr->TechPtr != NULL);
    NBPtr->TechPtr->TechnologySpecificHook[DataEyeSaveCompositeEyes] = MemF2DDataEyeSaveCompositeEyes;
    NBPtr->TechPtr->TechnologySpecificHook[DataEyeCompressEyes]      = MemF2DDataEyeCompressEyes;
    return TRUE;
  } else {
    return FALSE;
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function Creates a composite Data Eye from all byte lanes of 2D
 *     training Data and saves it into a temporary list of composite eyes
 *     for the current channel.
 *
 *     @param[in,out]   *TechPtr - Pointer to the MEM_TECH_BLOCK
 *     @param[in]       *Mem2DEntryData - Pointer to the 2D training data.
 *
 *     @return  TRUE - Function supported
 *     @return  FALSE - Function not supported
 */
BOOLEAN
STATIC
MemF2DDataEyeSaveCompositeEyes (
  IN OUT   MEM_TECH_BLOCK *TechPtr,
  IN       VOID *Mem2DEntryData
  )
{
  MEM_NB_BLOCK *NBPtr;
  MEM_DATA_STRUCT *MemPtr;
  DATAEYE_BUFFER *DataEyeBuffer;
  MEM_2D_ENTRY *Data;
  ALLOCATE_HEAP_PARAMS AllocHeapParams;
  LOCATE_HEAP_PTR LocHeap;
  UINT8 Index;
  UINT8 Lane;
  UINT8 Vref;
  INT8 Center;
  //
  // Initialize pointers
  //
  NBPtr = TechPtr->NBPtr;
  MemPtr = NBPtr->MemPtr;
  DataEyeBuffer = NULL;
  Data = (MEM_2D_ENTRY*) Mem2DEntryData;
  //
  // Locate Temporary Storage, if not Allocated, allocate and initialize it.
  //
  LocHeap.BufferHandle = AMD_MEM_DATAEYE_WORK_AREA_HANDLE;
  if (HeapLocateBuffer (&LocHeap, &(NBPtr->MemPtr->StdHeader)) == AGESA_SUCCESS) {
    DataEyeBuffer = (DATAEYE_BUFFER*) LocHeap.BufferPtr;
  } else {
    //
    // Allocate temporary buffer large enough to store composite data eyes for 1 Channel
    //
    AllocHeapParams.RequestedBufferSize = sizeof (DATAEYE_BUFFER);
    AllocHeapParams.BufferHandle = AMD_MEM_DATAEYE_WORK_AREA_HANDLE;
    AllocHeapParams.Persist = HEAP_LOCAL_CACHE;
    if (HeapAllocateBuffer (&AllocHeapParams, &MemPtr->StdHeader) == AGESA_SUCCESS) {
      DataEyeBuffer = (DATAEYE_BUFFER*)AllocHeapParams.BufferPtr;
      //
      // Initialize Header and Composite Eye Data Buffer
      //
      DataEyeBuffer->EyeCount = 0;
      for (Index = 0; Index < GET_SIZE_OF (DataEyeBuffer->Present); Index++) {
        DataEyeBuffer->Present[Index] = FALSE;
      }
      LibAmdMemFill ((VOID *)DataEyeBuffer->DataEye, 0xFF, (UINTN)sizeof (DataEyeBuffer->DataEye), &MemPtr->StdHeader);
    } else {
      //
      // Report Error
      //
      SetMemError (AGESA_FATAL, NBPtr->MCTPtr);
      PutEventLog (AGESA_FATAL, MEM_ERROR_HEAP_ALLOCATE_FOR_DATAEYE, NBPtr->Node, NBPtr->Dct, NULL, NULL, &MemPtr->StdHeader);
      return FALSE;
    }
  }
  //
  // Create the composite Data Eye in the next available buffer position
  //
  Index = (TechPtr->ChipSel >> 1);
  for (Lane = 0; Lane < MemT2DGetMaxLanes (TechPtr); Lane++) {
    //
    // For Each Lane, get the 1D trained center
    //
    Center = MemFGetEyeCenter (Data, Lane);
    //for (Vref = 0; Vref < NBPtr->TotalMaxVrefRange; Vref++) {
    for (Vref = 0; Vref < DATAEYE_VREF_RANGE; Vref++) {
      //
      // For Each value of Vref, combine the results by shifing the Pos & Neg
      // results into position, centering the eye data arount the trained center
      // cropping to 32 delay widths.
      //
      ASSERT (Index < MAX_DATAEYES_PER_CHANNEL);

      DataEyeBuffer->DataEye[Index][Vref] &= ((Data->Lane[Lane].Vref[Vref].PosRdDqsDly >> ((DATAEYE_RD_DQS_DELAY_RANGE / 2) - Center)) |
                                              (Data->Lane[Lane].Vref[Vref].NegRdDqsDly << ((DATAEYE_RD_DQS_DELAY_RANGE / 2) + Center)));
    } // Vref
  } // Lane

  DataEyeBuffer->EyeCount += (TechPtr->ChipSel & 1) ? 0 : 1;
  DataEyeBuffer->Present[Index] = TRUE;

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function takes the saved composite Data Eyes and compresses them into
 *       another data buffer, replaces the original buffer with the new one, then
 *       one. Then saves the new pointer in the Channel struct
 *
 *     @param[in,out]   *TechPtr - Pointer to the MEM_TECH_BLOCK
 *     @param[in,out]   *OptParam - Unused
 *
 *     @return  TRUE - Function supported
 *     @return  FALSE - Function not supported
 */
BOOLEAN
STATIC
MemF2DDataEyeCompressEyes (
  IN OUT   MEM_TECH_BLOCK *TechPtr,
  IN       VOID *OptParam
  )
{
  MEM_NB_BLOCK *NBPtr;
  MEM_DATA_STRUCT *MemPtr;
  DATAEYE_BUFFER *DataEyeBuffer;
  DATAEYE_BUFFER TempWorkspace;
  DATAEYE_CH_HEADER Header;
  BOOLEAN Status;
  ALLOCATE_HEAP_PARAMS AllocHeapParams;
  LOCATE_HEAP_PTR LocHeap;
  UINT8 *WorkspacePtr;
  UINT8 *CompressedEyes;
  UINT8 Index;
  UINT8 Size;
  UINT8 Offset;
  UINT8 CompressedEyesSize;
  //
  // Initialize pointers
  //
  NBPtr = TechPtr->NBPtr;
  MemPtr = NBPtr->MemPtr;
  DataEyeBuffer = NULL;
  Status = TRUE;
  WorkspacePtr = (UINT8*) &TempWorkspace.DataEye;
  //
  // Locate Temporary Storage, if not Allocated, then there is nothing to do
  //
  LocHeap.BufferHandle = AMD_MEM_DATAEYE_WORK_AREA_HANDLE;
  if (HeapLocateBuffer (&LocHeap, &(NBPtr->MemPtr->StdHeader)) == AGESA_SUCCESS) {
    DataEyeBuffer = (DATAEYE_BUFFER*) LocHeap.BufferPtr;
    //
    // Initialize Header
    //
    for ( Index = 0; Index < GET_SIZE_OF (Header.ImageOffset); Index++) {
      Header.ImageOffset[Index] = 0;
    }
    //
    // Compress Data Eyes into local workspace
    //
    Size = 0;
    Offset = sizeof (Header);
    for (Index = 0; Index < GET_SIZE_OF (DataEyeBuffer->DataEye); Index++) {
      if (DataEyeBuffer->Present[Index]) {
        Header.ImageOffset[Index] = Offset;
        Size = DataEyeEncodeRLE (WorkspacePtr, &(DataEyeBuffer->DataEye[Index][0]));
        Offset = Offset + Size;
        WorkspacePtr += Size;
      } else {
        Header.ImageOffset[Index] = 0;
      }
    }
    Header.Size = Offset;
    //
    // De-allocate the original buffer.
    //
    if (HeapDeallocateBuffer (AMD_MEM_DATAEYE_WORK_AREA_HANDLE, &MemPtr->StdHeader) != AGESA_SUCCESS) {
      SetMemError (AGESA_FATAL, NBPtr->MCTPtr);
      PutEventLog (AGESA_FATAL, MEM_ERROR_HEAP_DEALLOCATE_FOR_DATAEYE, NULL, NULL, NULL, NULL, &MemPtr->StdHeader);
      Status = FALSE;
    }
    //
    // Allocate Storage for final image and copy compressed Eyes into it.
    //
    AllocHeapParams.RequestedBufferSize = Offset;
    AllocHeapParams.BufferHandle = GENERATE_MEM_HANDLE (ALLOC_DATAEYES_HANDLE, NBPtr->Node, NBPtr->Dct, 0);
    AllocHeapParams.Persist = HEAP_LOCAL_CACHE;
    if (HeapAllocateBuffer (&AllocHeapParams, &MemPtr->StdHeader) == AGESA_SUCCESS) {
      CompressedEyes = (UINT8*) AllocHeapParams.BufferPtr;
      //
      // Copy Header and Compressed Data into new buffer
      //
      LibAmdMemCopy ((VOID *)CompressedEyes, (VOID *)&Header, (UINTN)sizeof (Header), &MemPtr->StdHeader);
      CompressedEyesSize = Offset - sizeof (Header);
      LibAmdMemCopy ((VOID *) (CompressedEyes + sizeof (Header)), (VOID *)&TempWorkspace.DataEye, CompressedEyesSize, &MemPtr->StdHeader);
      //
      // Save the pointer to the new buffer
      //
      if (TechPtr->Direction == DQS_READ_DIR) {
        //
        // Assign pointer according to Read or write training.
        //
        NBPtr->ChannelPtr->RdDataEyes = (VOID *) CompressedEyes;
      } else {
        NBPtr->ChannelPtr->WrDataEyes = (VOID *) CompressedEyes;
      }
    } else {
      //
      // Report Error and Set Status to false
      //
      SetMemError (AGESA_FATAL, NBPtr->MCTPtr);
      PutEventLog (AGESA_FATAL, MEM_ERROR_HEAP_ALLOCATE_FOR_DATAEYE, NBPtr->Node, NBPtr->Dct, NULL, NULL, &MemPtr->StdHeader);
      Status = FALSE;
    }
  }

  return Status;
}

/* -----------------------------------------------------------------------------*/
/**
*
*     Calculate data eye center at Nominal Vref position in 2D Data Eye.
*
*     @param[in]   *Data - Pointer to the 2D Data for the curent CS
*     @param[in]    Lane - current lane
*
*     @return       1D Data Eye Center at the Nominal Vref position.
*/
INT8
STATIC
MemFGetEyeCenter (
  IN       MEM_2D_ENTRY* Data,
  IN       UINT8 Lane
  )
{
  UINT8 i;
  INT8 Center;
  INT8 LeftEdge;
  INT8 RightEdge;
  BOOLEAN LeftEdgeFound;
  UINT8 Vref;

  LeftEdgeFound = FALSE;
  LeftEdge = -32;
  RightEdge = 31;
  Vref = Data->Vnom;
  //
  // Scan Negative Delay Range
  //
  for ( i = 0; i < DATAEYE_RD_DQS_DELAY_RANGE; i++) {
    if ((Data->Lane[Lane].Vref[Vref].NegRdDqsDly & (0x01 << ((DATAEYE_RD_DQS_DELAY_RANGE - 1) - i))) != 0) {
      if (!LeftEdgeFound) {
        LeftEdge = i - DATAEYE_RD_DQS_DELAY_RANGE;
        LeftEdgeFound = TRUE;
      }
      RightEdge = i - DATAEYE_RD_DQS_DELAY_RANGE;
    }
  }
  //
  // Scan Positive Delay Range
  //
  for ( i = 0; i < DATAEYE_RD_DQS_DELAY_RANGE; i++) {
    if ((Data->Lane[Lane].Vref[Vref].PosRdDqsDly & (0x01 << ((DATAEYE_RD_DQS_DELAY_RANGE - 1) - i))) != 0) {
      if (!LeftEdgeFound) {
        LeftEdge = i;
        LeftEdgeFound = TRUE;
      }
      RightEdge = i;
    }
  }
  //
  // Calculate Center
  //
  Center = ( LeftEdge + RightEdge + 1) / 2;

  return Center;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *    Compress a data Eye using RLE Compression
 *
 *     @param[in]  *Output - UINT8 Pointer to buffer where compressed data
                              eye will be stored
 *     @param[in]  *Input  - UINT32* Pointer to Data Eye Image to compress
 *
 *     @return UINT8 Size of compressed Data Eye in bytes.
 */
UINT8
STATIC
DataEyeEncodeRLE (
     OUT   UINT8 *Output,
  IN       UINT32 *Input
  )
{
  UINT8 TempBuffer[248];
  DATA_EYE_RLE_HEADER *OutHeader;
  UINT8 *OutData;
  UINT8 Column;
  UINT8 Row;
  UINT8 i;
  UINT8 RunCount;
  UINT8 RleSize;
  UINT8 CurData;
  UINT8 LastData;

  OutHeader = (DATA_EYE_RLE_HEADER*)Output;
  OutData = Output + sizeof (DATA_EYE_RLE_HEADER);
  //
  // First Compress in the X Direction
  //
  i = 0;
  RunCount = 1;
  LastData = 0xFF;
  for (Row = 0; Row < DATAEYE_VREF_RANGE; Row++) {
    for (Column = 0; Column < DATAEYE_RD_DQS_DELAY_RANGE; Column++ ) {
      CurData = (UINT8) ((Input[Row] >> Column) & 0x01);
      // If not the first Data bit
      if (LastData != 0xFF) {
        if ((CurData == LastData) && (RunCount < 0x7F)) {
          // Count repeat values
          RunCount++;
        } else {
          // Save current data and Run Count, advance pointer
          OutData[i++] = ((LastData << 7) | (RunCount & 0x7F));
          RunCount = 1;
        }
      }
      //
      // Save Current Data
      //
      LastData = CurData;
    } // Next Column
  } // Next Row
  // Save Last Data
  OutData[i++] = ((LastData << 7) | (RunCount & 0x7F));
  RleSize = i;
  //
  // Now try in the Y Direction
  //
  i = 0;
  RunCount = 1;
  LastData = 0xFF;
  for (Column = 0; Column < DATAEYE_RD_DQS_DELAY_RANGE; Column++ ) {
    for (Row = 0; Row < DATAEYE_VREF_RANGE; Row++) {
      CurData = (UINT8) ((Input[Row] >> Column) & 0x01);
      // If not the first Data bit
      if (LastData != 0xFF) {
        if ((CurData == LastData) && (RunCount < 0x7F)) {
          // Count repeat values
          RunCount++;
        } else {
          // Save current data and Run Count, advance pointer
          TempBuffer[i++] = ((LastData << 7) | (RunCount & 0x7F));
          RunCount = 1;
        }
      }
      //
      // Save Current Data
      //
      LastData = CurData;
    } // Next Column
  } // Next Row
  // Save Last Data
  TempBuffer[i++] = ((LastData << 7) | (RunCount & 0x7F));
  //
  // If Y Direction is smaller size, copy the YDIR Encoding to the output
  // buffer and update the header.
  //
  if ( i < RleSize) {
    RleSize = i;
    OutHeader->Encoding = RLE_YDIR;
    for (i = 0; i < RleSize; i++) {
      OutData[i] = TempBuffer[i];
    }
  } else {
    OutHeader->Encoding = RLE_XDIR;
  }
  OutHeader->Width = DATAEYE_RD_DQS_DELAY_RANGE;
  OutHeader->Height = DATAEYE_VREF_RANGE;
  return RleSize + sizeof (DATA_EYE_RLE_HEADER);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *    De-Compress a, RLE COmpressed Data Eye
 *
 *     @param[in]  *Output - UINT32* Pointer to buffer where the uncompressed data
 *                             eye will be stored
 *     @param[in]  *Input  - UINT8* Pointer to the compressed Data Eye Image
 *
 *     @return UINT8 Size of compressed Data Eye in bytes.
 */
UINT16
STATIC
DataEyeDecodeRLE (
     OUT   UINT32 *Output,
  IN       UINT8 *Input
  )
{
  DATA_EYE_RLE_HEADER *InputHeader;
  DATAEYE_RLE_BYTE *InputData;
  UINT16 OutputLength;
  UINT16 i;
  UINT16 Column;
  UINT16 Row;
  UINT8 ImageWidth;
  UINT8 ImageHeight;
  UINT8 RleDir;
  UINT8 RunCount;
  UINT8 Bit;
  UINT8 Pos;

  InputHeader = (DATA_EYE_RLE_HEADER*)Input;
  InputData = (DATAEYE_RLE_BYTE*) (Input + sizeof (DATA_EYE_RLE_HEADER));
  ImageWidth = InputHeader->Width;
  ImageHeight = InputHeader->Height;
  OutputLength = ImageWidth * ImageHeight;
  RleDir = (InputHeader->Encoding & 1);
  //
  // Zero out current output buffer
  //
  for (i = 0; i < (OutputLength / ((sizeof (*Output)) * 8)); i++) {
    Output[i] = 0;
  }
  Pos = 0;
  i = 0;
  Row = 0;
  Column = 0;
  while (i < OutputLength) {
    RunCount = InputData[Pos].RunCount;
    if ((i + RunCount) > OutputLength) {
      // This is an error condition
      break;
    }
    if (InputData[Pos].Bit == 1) {
      for (Bit = 0 ; Bit < RunCount; Bit++) {
        Output[Row] |= (1 << Column);
        if (RleDir == RLE_XDIR) {
          Column++;
          Row = Row + (Column / DATAEYE_RD_DQS_DELAY_RANGE);
          Column %= DATAEYE_RD_DQS_DELAY_RANGE;
        } else {
          Row++;
          Column = Column + (Row / DATAEYE_VREF_RANGE);
          Row %= DATAEYE_VREF_RANGE;
        }
      } // RunCount
    } else {
      if (RleDir == RLE_XDIR) {
        Column = Column + RunCount;
        Row = Row + (Column / DATAEYE_RD_DQS_DELAY_RANGE);
        Column %= DATAEYE_RD_DQS_DELAY_RANGE;
      } else {
        Row = Row + RunCount;
        Column = Column + (Row / DATAEYE_VREF_RANGE);
        Row %= DATAEYE_VREF_RANGE;
      }
    }
    i = i + RunCount;
    Pos++;
  }
  return i;
}


/* -----------------------------------------------------------------------------*/
/**
 *
 *      Get 2D training data eye.
 *
 *      @param[in, out]   *AmdGetDataEye - Pointer to AMD_GET_DATAEYE structure.
 *
 *      @return          AGESA_STATUS
 *                          - AGESA_SUCCESS
 *                          - AGESA_WARNING
 */
AGESA_STATUS
AmdGet2DDataEye (
  IN OUT   AMD_GET_DATAEYE *AmdGetDataEye
  )
{
  MEM_PARAMETER_STRUCT *RefPtr;
  MEM_DATA_STRUCT *MemPtr;
  UINT8 *CompressedEyesPtr;
  DATAEYE_CH_HEADER *Header;
  UINT8 SocketId;
  UINT8 ChannelId;
  UINT8 DimmId;
  UINT8 MaxChannels;
  UINT8 MaxDimms;

  SocketId = AmdGetDataEye->SocketId;
  ChannelId = AmdGetDataEye->MemChannelId;
  DimmId = AmdGetDataEye->DimmId;

  MemPtr = AmdGetDataEye->PostParamsPtr->MemConfig.MemData;
  RefPtr = MemPtr->ParameterListPtr;

  IDS_HDT_CONSOLE_INIT(&AmdGetDataEye->StdHeader);
  IDS_HDT_CONSOLE (MEM_FLOW, "\n\t AmdGet2DDataEye Socket %d Channel %d Dimm %d\n", SocketId, ChannelId, DimmId);

  if (!IsProcessorPresent (SocketId, &AmdGetDataEye->StdHeader)) {
    IDS_HDT_CONSOLE (MEM_FLOW, "\n\t Processor not populated on Socket %d\n", SocketId);
    return AGESA_WARNING;
  }

  MaxChannels = GetMaxChannelsPerSocket (RefPtr->PlatformMemoryConfiguration, SocketId, &AmdGetDataEye->StdHeader);
  if (ChannelId > MaxChannels) {
    IDS_HDT_CONSOLE (MEM_FLOW, "\n\t Channel %d is invalid\n", ChannelId);
    return AGESA_WARNING;
  }

  MaxDimms = GetMaxDimmsPerChannel (RefPtr->PlatformMemoryConfiguration, SocketId, ChannelId);
  if (DimmId > MaxDimms) {
    IDS_HDT_CONSOLE (MEM_FLOW, "\n\t Dimm %d is invalid\n", DimmId);
    return AGESA_WARNING;
  }

  if (!RefPtr->DataEyeEn) {
    IDS_HDT_CONSOLE (MEM_FLOW, "\n\t Data eye configuration is disabled!\n");
    return AGESA_WARNING;
  }

  if (AmdGetDataEye->DataEyeType == RD_DATAEYE) {
    CompressedEyesPtr = ((CH_DEF_STRUCT *)MemPtr->SocketList[SocketId].ChannelPtr[ChannelId])->RdDataEyes;
  } else {
    CompressedEyesPtr = ((CH_DEF_STRUCT *)MemPtr->SocketList[SocketId].ChannelPtr[ChannelId])->WrDataEyes;
  }

  Header = (DATAEYE_CH_HEADER *) CompressedEyesPtr;

  if ((CompressedEyesPtr != NULL) && (Header->ImageOffset[DimmId] != 0)) {
    CompressedEyesPtr += Header->ImageOffset[DimmId];
    DataEyeDecodeRLE ( (UINT32 *) AmdGetDataEye->DataEyeBuffer, CompressedEyesPtr);
  } else {
    IDS_HDT_CONSOLE (MEM_FLOW, "\n\t 2D %s Training Data for Socket %d channel %d Dimm %d does not exist\n", (AmdGetDataEye->DataEyeType == RD_DATAEYE) ? "Read" : "Write", SocketId, ChannelId, DimmId);
    return AGESA_WARNING;
  }

  return AGESA_SUCCESS;
}
