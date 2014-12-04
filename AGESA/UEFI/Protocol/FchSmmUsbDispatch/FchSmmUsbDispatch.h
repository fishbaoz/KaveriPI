/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD FCH SMM USB Dispacther Protocol
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AMD FCH UEFI Drivers
 * @e sub-project:  Protocols
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 */
/******************************************************************************
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
****************************************************************************/
#ifndef _FCH_SMM_USB_DISPATCH_PROTOCOL_H_
#define _FCH_SMM_USB_DISPATCH_PROTOCOL_H_

#define FCH_SMM_USB_DISPATCH_PROTOCOL_GUID \
       {0x59053B0D, 0xEEB8, 0x4379, 0xB1, 0xC8, 0x14, 0x5F, 0x1B, 0xB, 0xE4, 0xB9}
extern EFI_GUID gFchSmmUsbDispatchProtocolGuid;

struct _FCH_SMM_USB_DISPATCH_PROTOCOL;
typedef struct _FCH_SMM_USB_DISPATCH_PROTOCOL FCH_SMM_USB_DISPATCH_PROTOCOL;

///
/// AMD FCH SMM USB Register Context
///
typedef enum {
  Legacy,              ///< Legacy support
  Wake                 ///< Wake up support
} FCH_SMM_USB_SMI_TYPE;

///
/// AMD FCH USB Legacy SMI Register Context
///
typedef struct {
  FCH_SMM_USB_SMI_TYPE   Type;              ///< Legacy Type of support
  EFI_DEVICE_PATH_PROTOCOL  *Device;        ///< USB controllers
  UINT8                     Order;          ///< Priority 0-Highest (reserved), 0xFF-Lowest (reserved)
} FCH_SMM_USB_REGISTER_CONTEXT;

typedef EFI_STATUS (EFIAPI *FCH_SMM_USB_HANDLER_ENTRY_POINT) (
  IN       EFI_HANDLE                         DispatchHandle,
  IN       FCH_SMM_USB_REGISTER_CONTEXT       *UsbRegisterContext
  );

typedef EFI_STATUS (EFIAPI *FCH_SMM_USB_DISPATCH_REGISTER) (
  IN CONST FCH_SMM_USB_DISPATCH_PROTOCOL         *This,
  IN       FCH_SMM_USB_HANDLER_ENTRY_POINT       CallBackFunction,
  IN OUT   FCH_SMM_USB_REGISTER_CONTEXT          *UsbRegisterContext,
  OUT      EFI_HANDLE                            *DispatchHandle
);

typedef EFI_STATUS (EFIAPI *FCH_SMM_USB_DISPATCH_UNREGISTER) (
  IN       CONST FCH_SMM_USB_DISPATCH_PROTOCOL   *This,
  IN       EFI_HANDLE                            DispatchHandle
);

struct  _FCH_SMM_USB_DISPATCH_PROTOCOL {
  FCH_SMM_USB_DISPATCH_REGISTER    Register;
  FCH_SMM_USB_DISPATCH_UNREGISTER  UnRegister;
};

//
// PI 1.2
//

#define FCH_SMM_USB_DISPATCH2_PROTOCOL_GUID \
       {0x29053B0D, 0x2EB8, 0x2379, 0xB1, 0xC8, 0x14, 0x5F, 0x1B, 0xB, 0xE4, 0xB9}
extern EFI_GUID gFchSmmUsbDispatch2ProtocolGuid;

struct _FCH_SMM_USB_DISPATCH2_PROTOCOL;
typedef struct _FCH_SMM_USB_DISPATCH2_PROTOCOL FCH_SMM_USB_DISPATCH2_PROTOCOL;

typedef EFI_STATUS (EFIAPI *FCH_SMM_USB_HANDLER2_ENTRY_POINT) (
  IN       EFI_HANDLE                         DispatchHandle,
  IN       FCH_SMM_USB_REGISTER_CONTEXT       *UsbRegisterContext,
  IN OUT   VOID                               *CommBuffer,
  IN OUT   UINTN                              *CommBufferSize
  );

typedef EFI_STATUS (EFIAPI *FCH_SMM_USB_DISPATCH2_REGISTER) (
  IN CONST FCH_SMM_USB_DISPATCH2_PROTOCOL        *This,
  IN       FCH_SMM_USB_HANDLER2_ENTRY_POINT      CallBack2Function,
  IN OUT   FCH_SMM_USB_REGISTER_CONTEXT          *UsbRegisterContext,
  OUT      EFI_HANDLE                            *DispatchHandle
);

typedef EFI_STATUS (EFIAPI *FCH_SMM_USB_DISPATCH2_UNREGISTER) (
  IN       CONST FCH_SMM_USB_DISPATCH2_PROTOCOL  *This,
  IN       EFI_HANDLE                            DispatchHandle
);

struct  _FCH_SMM_USB_DISPATCH2_PROTOCOL {
  FCH_SMM_USB_DISPATCH2_REGISTER    Register;
  FCH_SMM_USB_DISPATCH2_UNREGISTER  UnRegister;
};
#endif
