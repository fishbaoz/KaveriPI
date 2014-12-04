/* $NoKeywords:$ */
/**
 * @file
 *
 * Amd Smm access library
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   SMM Access DXE Driver
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

#include "AmdSmmAccess2.h"

EFI_HANDLE  SmmAccessHandle = NULL;

EFI_SMRAM_DESCRIPTOR mSmramMap;

STATIC EFI_SMM_ACCESS2_PROTOCOL  mSmmAccess2 = {
  AmdSmmOpen,
  AmdSmmClose,
  AmdSmmLock,
  AmdSmmGetCapability,
  FALSE,
  FALSE,
};

//FMO Move all these MSR functions to seperate library functions

EFI_STATUS
AmdSmmOpen (
  IN EFI_SMM_ACCESS2_PROTOCOL  *This
  )
{
  UINT64 Value;

  if (mSmmAccess2.LockState) {
    return  EFI_ACCESS_DENIED;
  }

  Value = AsmReadMsr64 (0xc0010113) & ~2; //TValid
  AsmWriteMsr64 (0xc0010113, Value);

  mSmramMap.RegionState = EFI_SMRAM_OPEN;
  mSmmAccess2.OpenState = TRUE;
  return EFI_SUCCESS;
}

EFI_STATUS
AmdSmmClose (
  IN EFI_SMM_ACCESS2_PROTOCOL  *This
  )
{

  UINT64 Value;

  if (mSmmAccess2.LockState) {
    return  EFI_ACCESS_DENIED;
  }

  Value = AsmReadMsr64 (0xc0010113) | 2;
  AsmWriteMsr64 (0xc0010113, Value);

  mSmramMap.RegionState = EFI_SMRAM_CLOSED;
  mSmmAccess2.OpenState = FALSE;
  return EFI_SUCCESS;
}

EFI_STATUS
AmdSmmLock (
  IN EFI_SMM_ACCESS2_PROTOCOL  *This
  )
{
  UINT64 Value;

  Value = AsmReadMsr64 (0xc0010115) | 1;
  AsmWriteMsr64 (0xc0010115, Value);
  mSmramMap.RegionState = EFI_SMRAM_LOCKED;
  mSmmAccess2.LockState = TRUE;
  return EFI_SUCCESS;
}


EFI_STATUS
AmdSmmGetCapability
(
  IN CONST EFI_SMM_ACCESS2_PROTOCOL  *This,
  IN OUT UINTN                       *SmramMapSize,
  IN OUT EFI_SMRAM_DESCRIPTOR        *SmramMap
  )
{
  if (*SmramMapSize < sizeof (mSmramMap)) {
    *SmramMapSize = sizeof (mSmramMap);
    return EFI_BUFFER_TOO_SMALL;
  }

  CopyMem (SmramMap, &mSmramMap, sizeof (mSmramMap));
  *SmramMapSize = sizeof (mSmramMap);
  return EFI_SUCCESS;
}


EFI_STATUS
InitializeTsegBase (
  IN UINT64 TsegBase,
  IN UINT64 TsegSize
  )
{

  UINT64  TSmmMask;

  //FMO use equates

  AsmWriteMsr64(0xC0010112, TsegBase);    // TSEG base

  TSmmMask = AsmReadMsr64 (0xC0010113);
  TSmmMask &= 0x1FFFF;
  TSmmMask |= (~(TsegSize - 1)) & 0xFFFFFFFFFFFE0000;
  TSmmMask |= 0x03;
  AsmWriteMsr64 (0xC0010113, TSmmMask);

  return EFI_SUCCESS;
}

EFI_STATUS
LocateTsegBaseSize (
  OUT UINT64  *TsegSize,
  OUT UINT64  *TsegBase
  )
{
  AMD_MEMORY_INFO_HOB             *AmdInfoHob = NULL;
  UINTN               i;

  AmdInfoHob = (AMD_MEMORY_INFO_HOB *) ((UINT8 *)GetFirstGuidHob (&gAmdMemoryInfoHobGuid) + sizeof (EFI_HOB_GUID_TYPE));

  *TsegSize = (UINT64)PcdGet32 (PcdTsegSize);
  if (!*TsegSize || (AmdInfoHob == NULL)) {
    return EFI_UNSUPPORTED;
  }

  for (i = 0; i < AmdInfoHob->NumberOfDescriptor; i++) {
    if (AmdInfoHob->Ranges[i].Base == 0) {
      *TsegBase =  AmdInfoHob->Ranges[i].Size - *TsegSize;
      break;
    }
  }
  return EFI_SUCCESS;
}
/*----------------------------------------------------------------------------------------*/
/**
 * Name: AmdInstallSmmControl
 *
 * This DXE driver produces the SMM Control Protocol
 *
 * @param[in]    ImageHandle     Pointer to the image handle
 * @param[in]    SystemTable     Pointer to the EFI system table
 *
 * @retval       EFI_SUCCESS     Driver initialized successfully
 * @retval       EFI_ERROR       Driver initialization failed
 *
 */
/*----------------------------------------------------------------------------------------*/
EFI_STATUS
InstallSmmAccess2 (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT64    TsegSize;
  UINT64    TsegBase;

  if (EFI_ERROR (LocateTsegBaseSize ( &TsegSize,  &TsegBase))) {
    return EFI_UNSUPPORTED;
  }

  InitializeTsegBase (TsegBase, TsegSize);

  mSmramMap.PhysicalStart =     TsegBase;
  mSmramMap.CpuStart      =     TsegBase;
  mSmramMap.PhysicalSize  =     TsegSize;
  mSmramMap.RegionState   =     EFI_SMRAM_CLOSED;

  //
  // Finally install the protocol
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &SmmAccessHandle,
                  &gEfiSmmAccess2ProtocolGuid,
                  &mSmmAccess2,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return (Status);
}

