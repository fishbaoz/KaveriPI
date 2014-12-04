/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH SMM Library
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AMD FCH UEFI Drivers
 * @e sub-project:  Library
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
*****************************************************************************
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

#include <Library/FchSmmLib.h>
#include "FchPlatform.h"

#define FILECODE UEFI_SMM_FCHSMMLIB_FCHSMMLIB_FILECODE

/*----------------------------------------------------------------------------------------*/
/**
 * Init variable.
 *
 * SystemTable Sytem info table
 */
/*----------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------*/
/**
 *  AgesaAllocateBuffer
 *
 * Description
 *    Call the host environment interface to provide a user hook opportunity.
 *
 *  @param[in]      FcnData
 *  @param[in,out]  AllocParams
 *
 * @return   The AGESA Status returned from the callout.
 *
 */
AGESA_STATUS
AgesaAllocateBuffer (
  IN       UINTN               FcnData,
  IN OUT   AGESA_BUFFER_PARAMS *AllocParams
  )
{
  return AGESA_SUCCESS;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  AgesaLocateBuffer
 *
 * Description
 *    Call the host environment interface to provide a user hook opportunity.
 *
 *  @param[in]      FcnData
 *  @param[in,out]  LocateParams
 *
 * @return   The AGESA Status returned from the callout.
 *
 */
AGESA_STATUS
AgesaLocateBuffer (
  IN       UINTN               FcnData,
  IN OUT   AGESA_BUFFER_PARAMS *LocateParams
  )
{
  return AGESA_SUCCESS;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  AgesaDeAllocateBuffer
 *
 * Description
 *    Call the host environment interface to provide a user hook opportunity.
 *
 *  @param[in]      FcnData
 *  @param[in,out]  DeallocParams
 *
 * @return   The AGESA Status returned from the callout.
 *
 */
AGESA_STATUS
AgesaDeallocateBuffer (
  IN       UINTN               FcnData,
  IN OUT   AGESA_BUFFER_PARAMS *DeallocParams
  )
{
  return AGESA_SUCCESS;
}


/*----------------------------------------------------------------------------------------*/
/**
 *  LibFchSmmMemRead
 *  Description
 *      SMM Memory Read Access
 *
 * @param[in]       SmmIo           Pointer to SMM CPU IO interface
 * @param[in]       AccessWidth     Access width
 * @param[in]       Address         Memory address
 * @param[out]      Value           Pointer to data buffer
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchSmmMemRead (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       EFI_SMM_IO_WIDTH            AccessWidth,
  IN       UINT64                      Address,
     OUT   VOID                        *Value
  )
{
  SmmIo->Mem.Read (
                   SmmIo,
                   AccessWidth,
                   Address,
                   1,
                   Value
                   );
}


/*----------------------------------------------------------------------------------------*/
/**
 *  LibFchSmmIoRead
 *  Description
 *      SMM I/O Read Access
 *
 * @param[in]       SmmIo           Pointer to SMM CPU IO interface
 * @param[in]       AccessWidth     Access width
 * @param[in]       Address         IO address
 * @param[out]      Value           Pointer to data buffer
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchSmmIoRead (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       EFI_SMM_IO_WIDTH            AccessWidth,
  IN       UINT16                      Address,
     OUT   VOID                        *Value
  )
{
  SmmIo->Io.Read (
                   SmmIo,
                   AccessWidth,
                   Address,
                   1,
                   Value
                   );
}


/*----------------------------------------------------------------------------------------*/
/**
 *  LibFchSmmIoWrite
 *  Description
 *      SMM I/O Write Access
 *
 * @param[in]       SmmIo           Pointer to SMM CPU IO interface
 * @param[in]       AccessWidth     Access width
 * @param[in]       Address         IO address
 * @param[out]      Value           Pointer to data buffer
 *
 */
/*----------------------------------------------------------------------------------------*/
VOID
LibFchSmmIoWrite (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       EFI_SMM_IO_WIDTH            AccessWidth,
  IN       UINT64                      Address,
  IN       VOID                        *Value
  )
{
  SmmIo->Io.Write (
              SmmIo,
              AccessWidth,
              Address,
              1,
              Value
              );
}


VOID
LibFchSmmPciRead (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       EFI_SMM_IO_WIDTH            AccessWidth,
  IN       UINT32                      PciAddress,
     OUT   VOID                        *Value
  )
{
  UINT32     Address32;

  Address32 = BIT31 + (UINT32) ((PciAddress >> 8) & 0xFFFFFF00) + (PciAddress & 0xFF);
  LibFchSmmIoWrite (SmmIo, SMM_IO_UINT32, CFG_ADDR_PORT, &Address32);
  LibFchSmmIoRead (SmmIo, AccessWidth, CFG_DATA_PORT, Value);
}


VOID
LibFchSmmPmioRead8 (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       UINT8                       Offset,
     OUT   VOID                        *Value
  )
{
  LibFchSmmIoWrite (SmmIo, SMM_IO_UINT8, FCH_IOMAP_REGCD6, &Offset);
  LibFchSmmIoRead (SmmIo, SMM_IO_UINT8, FCH_IOMAP_REGCD7, Value);
}


VOID
LibFchSmmPmioWrite8 (
  IN       EFI_SMM_CPU_IO2_PROTOCOL    *SmmIo,
  IN       UINT8                       Offset,
     OUT   VOID                        *Value
  )
{
  LibFchSmmIoWrite (SmmIo, SMM_IO_UINT8, FCH_IOMAP_REGCD6, &Offset);
  LibFchSmmIoWrite (SmmIo, SMM_IO_UINT8, FCH_IOMAP_REGCD7, Value);
}




