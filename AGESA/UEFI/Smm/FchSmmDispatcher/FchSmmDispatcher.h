/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Hudson-2 SMI Dispatcher Driver Example
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  UEFI
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

#ifndef _FCH_SMM_DISPATCHER_H_
#define _FCH_SMM_DISPATCHER_H_

#include "Tiano.h"
#include "EfiDebug.h"
#include "EfiRegTableLib.h"
#include "EfiCombinationLib.h"
#include "EfiSmmDriverLib.h"

#include "FchSmmSxDispatcher.h"
#include "FchSmmSwDispatcher.h"
#include "FchSmmPwrBtnDispatcher.h"
#include "FchSmmIoTrapDispatcher.h"
#include "FchSmmPeriodicalDispatcher.h"
#include "FchSmmGpeDispatcher.h"
#include "FchSmmUsbDispatcher.h"
#include "FchSmmMiscDispatcher.h"

#include "FchPlatform.h"
#include "FchDxeCommon.h"

#define Slp_Type     BIT1
#define SmiCmdPort   BIT11
#define UsbSmi       BIT12
#define PwrBtn       BIT19
#define Eos          BIT28
#define IoTrapping0  BIT20
#define IoTrapping1  BIT21
#define IoTrapping2  BIT22
#define IoTrapping3  BIT23
#define MemTrapping0 BIT24
#define MemTrapping1 BIT25
#define MemTrapping2 BIT26
#define MemTrapping3 BIT27
#define ShortTimer   BIT14
#define LongTimer    BIT15
#define Gpe          0xfffffffful
#define Smbus0       BIT15
#define AllGevents   0xfffffffful
#define FchGppSerr0  BIT21
#define FchGppSerr1  BIT22
#define FchGppSerr2  BIT23
#define FchGppSerr3  BIT24

typedef EFI_STATUS (EFIAPI *AMD_SM_SMM_CHILD_DISPATCHER_HANDLER) (
  IN      EFI_HANDLE     SmmImageHandle,
  IN OUT  VOID           *CommunicationBuffer OPTIONAL,
  IN OUT  UINTN          *SourceSize OPTIONAL
  );


///
/// AMD FCH SMM Dispatcher Structure
///
typedef struct {
  UINT32                                SmiStatusReg;   ///< Status Register
  UINT32                                SmiStatusBit;   ///< Status Bit
} FCH_SMM_COMMUNICATION_BUFFER;

///
/// AMD FCH SMM Dispatcher Structure
///
typedef struct {
  UINT32                                StatusReg;      ///< Status Register
  UINT32                                SmiStatusBit;   ///< Status Bit
  AMD_SM_SMM_CHILD_DISPATCHER_HANDLER   SmiDispatcher;  ///< Dispatcher Address
} FCH_SMM_DISPATCHER_TABLE;

///
/// AMD FCH SMM Dispatcher Structure
///
typedef struct {
  UINT16             ioPort;                            ///< I/O port
  EFI_SMM_IO_WIDTH   ioWidth;                           ///< I/O width
  UINT32             ioValue;                           ///< I/O value
} SAVE_B2B_IO;

///
/// AMD Save/Restore Structure for SMM
///
typedef struct {
  UINT8              Bus;                               ///< PCI Bus
  UINT8              Dev;                               ///< PCI Device
  UINT8              Func;                              ///< PCI Function
  UINT16             Offset;                            ///< Offset registers
  EFI_SMM_IO_WIDTH   DataWidth;                         ///< Data width
  UINT32             DataValue;                         ///< Data value
} SAVE_PCI;

#endif
