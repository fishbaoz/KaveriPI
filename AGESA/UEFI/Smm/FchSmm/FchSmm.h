/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH SMM Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH SMM Driver
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

#ifndef _FCH_SMM_DRIVER_H_
#define _FCH_SMM_DRIVER_H_

#include "Tiano.h"
#include "EfiDebug.h"
#include "EfiRegTableLib.h"
#include "EfiCombinationLib.h"
#include "EfiSmmDriverLib.h"

#include "FchPlatform.h"
#include "FchDxeCommon.h"

#include EFI_PROTOCOL_CONSUMER (PciRootBridgeIo)
#include EFI_PROTOCOL_DEFINITION (PciIo)
#include EFI_PROTOCOL_DEPENDENCY (SmmBase)
#include EFI_PROTOCOL_DEPENDENCY (LoadedImage)

#include EFI_PROTOCOL_CONSUMER (FchInitProtocol)
#include EFI_PROTOCOL_CONSUMER (FchSmmSwDispatch)
#include EFI_PROTOCOL_CONSUMER (FchSmmSxDispatch)
#include EFI_PROTOCOL_CONSUMER (FchSmmPeriodicalDispatch)
#include EFI_PROTOCOL_CONSUMER (FchSmmGpeDispatch)
#include EFI_PROTOCOL_CONSUMER (FchSmmPwrBtnDispatch)
#include EFI_PROTOCOL_CONSUMER (FchSmmMiscDispatch)
#include EFI_PROTOCOL_CONSUMER (FchSmmIoTrapDispatch)
#include EFI_PROTOCOL_CONSUMER (FchSmmUsbDispatch)

EFI_STATUS
EFIAPI
AmdSmiAcpiOnCallback (
  IN  EFI_HANDLE                       DispatchHandle,
  IN  FCH_SMM_SW_REGISTER_CONTEXT      *DispatchContext
  );

EFI_STATUS
EFIAPI
AmdSmiAcpiOnCallback2 (
  IN  EFI_HANDLE                       DispatchHandle,
  IN  FCH_SMM_SW_REGISTER_CONTEXT      *DispatchContext,
  IN  FCH_SMM_SW_CONTEXT               *EfiSmmSwContext,
  IN  UINTN                            *SizeOfEfiSmmSwContext
  );

EFI_STATUS
EFIAPI
AmdSmiAcpiOffCallback (
  IN  EFI_HANDLE                       DispatchHandle,
  IN  FCH_SMM_SW_REGISTER_CONTEXT      *DispatchContext
  );

#ifdef FCH_SPI_PROTECT_SMI
  EFI_STATUS
  EFIAPI
  AmdSmiSpiUnlockCallback (
    IN  EFI_HANDLE                       DispatchHandle,
    IN  FCH_SMM_SW_REGISTER_CONTEXT      *DispatchContext
  );

  EFI_STATUS
  EFIAPI
  AmdSmiSpiLockCallback (
    IN  EFI_HANDLE                       DispatchHandle,
    IN  FCH_SMM_SW_REGISTER_CONTEXT      *DispatchContext
  );
#endif

VOID
FchSmmAcpiOn (
  IN      FCH_DATA_BLOCK   *FchDataPtr
  );

VOID
FchInitS3EarlyRestore (
  IN      FCH_DATA_BLOCK     *FchDataPtr
  );

VOID
FchInitS3LateRestore (
  IN      FCH_DATA_BLOCK     *FchDataPtr
  );

EFI_STATUS
EFIAPI
AmdSmiBeforePciS3RestoreCallback (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  FCH_SMM_SW_REGISTER_CONTEXT   *DispatchContext
);

EFI_STATUS
EFIAPI
AmdSmiAfterPciS3RestoreCallback (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  FCH_SMM_SW_REGISTER_CONTEXT   *DispatchContext
);

EFI_STATUS
EFIAPI
AmdGpeSmiCallback (
  IN  EFI_HANDLE                        DispatchHandle,
  IN  FCH_SMM_GPE_REGISTER_CONTEXT      *DispatchContext
  );

EFI_STATUS
EFIAPI
AmdSmiPowerButtonCallback (
  IN  EFI_HANDLE                           DispatchHandle,
  IN  FCH_SMM_PWRBTN_REGISTER_CONTEXT      *DispatchContext
  );

EFI_STATUS
EFIAPI
AmdSmiPowerButtonCallbackTest (
  IN  EFI_HANDLE                           DispatchHandle,
  IN  FCH_SMM_PWRBTN_REGISTER_CONTEXT      *DispatchContext
  );

EFI_STATUS
EFIAPI
AmdSmiPowerButtonCallback2Test (
  IN  EFI_HANDLE                        DispatchHandle,
  IN  FCH_SMM_PWRBTN_REGISTER_CONTEXT   *DispatchContext,
  IN OUT   VOID                         *CommBuffer,
  IN OUT   UINTN                        *CommBufferSize
  );

EFI_STATUS
EFIAPI
AmdSmiS3SleepEntryCallback (
  IN  EFI_HANDLE                       DispatchHandle,
  IN  FCH_SMM_SX_REGISTER_CONTEXT      *DispatchContext
  );

EFI_STATUS
EFIAPI
AmdSmiS4SleepEntryCallback (
  IN  EFI_HANDLE                       DispatchHandle,
  IN  FCH_SMM_SX_REGISTER_CONTEXT      *DispatchContext
  );

EFI_STATUS
EFIAPI
AmdSmiS4SleepEntryCallback2 (
  IN  EFI_HANDLE                       DispatchHandle,
  IN  FCH_SMM_SX_REGISTER_CONTEXT      *DispatchContext,
  IN OUT   VOID                        *CommBuffer,
  IN OUT   UINTN                       *CommBufferSize
  );

EFI_STATUS
EFIAPI
AmdSmiS5SleepEntryCallback (
  IN  EFI_HANDLE                       DispatchHandle,
  IN  FCH_SMM_SX_REGISTER_CONTEXT      *DispatchContext
  );


EFI_STATUS
EFIAPI
AmdMiscFchGppHpSmiCallback (
  IN  EFI_HANDLE                        DispatchHandle,
  IN  FCH_SMM_MISC_REGISTER_CONTEXT     *DispatchContext
  );

#ifdef FCH_TIMER_TICK_INTERVAL_WA
  EFI_STATUS
  EFIAPI
  AmdMiscFchHpetIntervalCallback (
    IN  EFI_HANDLE                        DispatchHandle,
    IN  FCH_SMM_MISC_REGISTER_CONTEXT     *DispatchContext
  );

  EFI_STATUS
  EFIAPI
  AmdMiscFchIrq2TrapCallback (
    IN  EFI_HANDLE                        DispatchHandle,
    IN  FCH_SMM_MISC_REGISTER_CONTEXT     *DispatchContext
  );
#endif

EFI_STATUS
EFIAPI
AmdIoTrapSmiCallback (
  IN       EFI_HANDLE   DispatchHandle,
  IN       CONST VOID   *Context        OPTIONAL,
  IN OUT   VOID         *CommBuffer     OPTIONAL,
  IN OUT   UINTN        *CommBufferSize OPTIONAL
  );

EFI_STATUS
EFIAPI
FchUsbBeforeOCcallback (
  IN  EFI_HANDLE                        DispatchHandle,
  IN  FCH_SMM_USB_REGISTER_CONTEXT      *DispatchContext
  );

EFI_STATUS
FchSmmRegisterSwSmi (
  VOID
  );

EFI_STATUS
FchSmmRegisterPeriodicTimerSmi (
  VOID
  );

EFI_STATUS
FchSmmRegisterGpeSmi (
  VOID
  );

EFI_STATUS
FchSmmRegisterPowerButtonSmi (
  VOID
  );

EFI_STATUS
FchSmmRegisterSxSmi (
  VOID
  );

VOID
FchEnableSleepTransition (
  VOID
  );

EFI_STATUS
FchSmmRegisterMiscSmi (
  VOID
  );

EFI_STATUS
FchSmmRegisterIoTrapSmi (
  VOID
  );

EFI_STATUS
RegisterUsbPatchSmi (
  VOID
  );
#endif
