/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH WHEA DXE Driver
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

#ifndef _AMD_FCH_WHEA_SMM_H_
#define _AMD_FCH_WHEA_SMM_H_

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/FchInitProtocol.h>
#include <Protocol\FchSmmSwDispatch2.h>
#include <Protocol\FchSmmMiscDispatch.h>
#include "EinjSmm.h"
#include "AmdFchWheaSmmPlatform.h"
#include "HestSmm.h"
#include "FchPlatform.h"

extern EFI_GUID gEfiAmdFchWheaDataProtocolGuid;

#define PCIE_ADDRESS(base, bus, dev, func, reg) \
(UINT32) ( (UINT32)base + (((UINT32)bus) << 20) + (((UINT32)dev) << 15) + (((UINT32)func) << 12) + ((UINT32)reg) )

///
/// AMD FCH Private EINJ System Context structure
///
typedef struct {
  UINT16                      CmdReg;                      ///< PCIe_Cfg 0x04 Cmd Register
  UINT16                      BridgeCntlReg;               ///< PCIe_Cfg 0x3E Bridge Control Register
  UINT16                      DeviceStatusReg;             ///< PCIe_Cfg 0x60 Device Status Register
  UINT16                      RootCntlReg;                 ///< PCIe_Cfg 0x74 Root Control Register
  UINT32                      PcieUncorrErrMaskReg;        ///< PCIe_Cfg 0x158 PCIe Uncorrectable Error Mask Register
  UINT32                      PcieUncorrErrSeverityReg;    ///< PCIe_Cfg 0x15C PCIe Uncorrectable Error Severity Register
  UINT32                      PcieCorrErrMaskReg;          ///< PCIe_Cfg 0x164 PCIe Correctable Error Mask Register
  UINT32                      PcieAdvErrCapCntlReg;        ///< PCIe_Cfg 0x168 PCIe Advanced Error Capabilities and Control Register
  UINT32                      PcieRootErrCmdReg;           ///< PCIe_Cfg 0x17C PCIe Root Error Command Register
} AMD_FCH_WHEA_EINJ_CONTEXT;

///
/// AMD FCH Private EINJ data structure
///
typedef struct {
  BOOLEAN                     Valid;                       ///< Indicate if data valid
  BOOLEAN                     PlatformEinjValid;           ///< Indicate if Platform Einj supported
  UINT64                      *PlatformEinjBeginOpAddress; ///< Address in Platform Einj entry Begin Injection Operation
  UINT64                      PlatformEinjBeginOpValue;    ///< Value in Platform Einj entry Begin Injection Operation
  UINT64                      *PlatformEinjSetTypeAddress; ///< Address in Platform Einj entry Set Error Type
  UINT64                      *PlatformEinjCmdStatus;      ///< Address in Platform Einj entry Set Error Type
  AMD_FCH_WHEA_EINJ_CONTEXT   SystemContext[4];            ///< System Context
  GENERIC_ERROR_STATUS_BLOCK  *AmdHwErrBlk;                ///< Generic Error Status Block
  UINT32                      PcieBaseAddress;             ///< Pcie Base Address
} AMD_FCH_WHEA_EINJ_BUFFER;


EFI_STATUS
AmdFchWheaSmmInitEntry (
  IN EFI_HANDLE                          ImageHandle,
  IN EFI_SYSTEM_TABLE                    *SystemTable
  );

EFI_STATUS
EFIAPI
AmdSmiEinjTriggerActionCallBack (
  IN       EFI_HANDLE                        DispatchHandle,
  IN CONST FCH_SMM_SW_REGISTER_CONTEXT       *DispatchContext,
  IN OUT   FCH_SMM_SW_CONTEXT                *SwContext,
  IN OUT   UINTN                             *SizeOfSwContext
  );

BOOLEAN
AmdSmiEinjValidate (
  IN OUT   VOID
  );

EFI_STATUS
EFIAPI
AmdSmiEinjSystemContextSave (
  IN OUT   VOID
  );

EFI_STATUS
EFIAPI
AmdSmiEinjErrorInjectionSetup (
  IN OUT   VOID
  );

EFI_STATUS
EFIAPI
AmdSmiEinjClrStatus (
  IN OUT   VOID
  );

EFI_STATUS
EFIAPI
AmdSmiEinjErrorInjection (
  IN OUT   VOID
  );

EFI_STATUS
EFIAPI
AmdSmiEinjChkErr (
  IN OUT   VOID
  );

EFI_STATUS
EFIAPI
AmdSmiEinjErrInjDis (
  IN OUT   VOID
  );

EFI_STATUS
EFIAPI
AmdSmiEinjSystemContextRestore (
  IN OUT   VOID
  );

EFI_STATUS
EFIAPI
AmdFchWheaElogGpp (
  IN UINT8     PortId
  );

EFI_STATUS
EFIAPI
AmdMiscFchWheaHwSmiCallback (
  IN  EFI_HANDLE                         DispatchHandle,
  IN  FCH_SMM_MISC_REGISTER_CONTEXT      *DispatchContext
  );

extern AMD_FCH_WHEA_EINJ_BUFFER          *mEinjData;
#endif
