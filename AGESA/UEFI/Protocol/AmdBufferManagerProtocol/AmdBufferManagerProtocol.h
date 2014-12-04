/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Protocol Declaration for Heap Buffer Management
 *
 * Contains declaration for AMD_BUFFER_MANAGER_PROTOCOL
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  UEFI
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
 ****************************************************************************
 * AMD Generic Encapsulated Software Architecture
 *
 * Description: AmdBufferManagerProtocol.h - Protocol and GUID declarations.
 *
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
#ifndef _AMD_BUFFER_MANAGER_PROTOCOL_H_
#define _AMD_BUFFER_MANAGER_PROTOCOL_H_

#include "Amd.h"
#include "AGESA.h"

//
// GUID definition
//
#define AMD_BUFFER_MANAGER_PROTOCOL_GUID \
  { 0x3724cf01, 0x00c2, 0x17e9, 0x22, 0xb3, 0x0e, 0xa8, 0xaa, 0x89, 0x72, 0x00 }

extern EFI_GUID gAmdBufferManagerProtocolGuid;

//
/// Buffer Manager for Linked List for AGESA HOB Data
typedef struct _AGESA_DXE_BUFFER_MANAGER {
  UINTN                                  BufferLength;               ///< Length of Buffer
  UINTN                                  BufferHandle;               ///< Handle of Buffer
  VOID                                   *BufferPtr;                 ///< Buffer LOcation Ptr
  struct    _AGESA_DXE_BUFFER_MANAGER    *NextAgesaBufferManagerPtr; ///< Next Buffer LOcation Ptr
} AGESA_DXE_BUFFER_MANAGER;


/// Function prototype
typedef EFI_STATUS (EFIAPI *AMD_BUFFER_CALLOUT) (
  IN       UINT32                           Fcn1Data,    ///< Table Type'
  IN       UINTN                            Fcn2Data,    ///< Table Type'
  IN OUT   VOID                             *ConfigPtr   ///< Buffer Length
);

///
/// AMD Buffer Manager Protocol
///
typedef struct _AMD_BUFFER_MANAGER_PROTOCOL {
  AMD_CONFIG_PARAMS             StdHeader;            ///< AMD Standard Header
  AGESA_DXE_BUFFER_MANAGER      *DxeBufferManager;    ///< Buffer Manager Pointer
  AMD_BUFFER_CALLOUT            AmdBufferCallout;     ///< Buffer manager callout function
} AMD_BUFFER_MANAGER_PROTOCOL;


#endif // _AMD_BUFFER_MANAGER_PROTOCOL_H_
