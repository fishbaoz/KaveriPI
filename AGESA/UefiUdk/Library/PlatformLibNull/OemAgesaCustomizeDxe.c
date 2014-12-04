/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD OEM Agesa Customization Before calling Agesa Entry Points
 *
 * This file is placed in the user's platform directory and contains the
 * build option selections desired for that platform.
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  UEFI
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
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
 * ***************************************************************************
 */
//UdhChange #include "Tiano.h"
#include "PiDxe.h"
#include "AGESA.h"
#include "OemAgesaCustomizeDxe.h"

/*---------------------------------------------------------------------------------------*/
/**
 *  OemCustomizeInitEnv
 *
 *  Description:
 *    This is the stub function will call the host environment through the binary block
 *    interface (call-out port) to provide a user hook opportunity
 *
 *  Parameters:
 *    @param[in]      *BootServices
 *    @param[in]      *InitEnv
 *
 *    @retval         VOID
 *
 **/
/*---------------------------------------------------------------------------------------*/
VOID
OemCustomizeInitEnv (
  IN  EFI_BOOT_SERVICES *BootServices,
  IN  AMD_ENV_PARAMS    *InitEnv
  )
{
  InitEnv->FchInterface.SataClass = SataNativeIde;
  InitEnv->FchInterface.IdeEnable = FALSE;
  InitEnv->FchInterface.SataIdeMode = TRUE;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  OemCustomizeInitMid
 *
 *  Description:
 *    This is the stub function will call the host environment through the binary block
 *    interface (call-out port) to provide a user hook opportunity
 *
 *  Parameters:
 *    @param[in]      *BootServices
 *    @param[in]      *InitMid
 *
 *    @retval         VOID
 *
 **/
/*---------------------------------------------------------------------------------------*/
VOID
OemCustomizeInitMid (
  IN  EFI_BOOT_SERVICES *BootServices,
  IN  AMD_MID_PARAMS    *InitMid
  )
{
}

/*---------------------------------------------------------------------------------------*/
/**
 *  OemCustomizeInitLate
 *
 *  Description:
 *    This is the stub function will call the host environment through the binary block
 *    interface (call-out port) to provide a user hook opportunity
 *
 *  Parameters:
 *    @param[in]      *BootServices
 *    @param[in]      *InitLate
 *
 *    @retval         VOID
 *
 **/
/*---------------------------------------------------------------------------------------*/
VOID
OemCustomizeInitLate (
  IN  EFI_BOOT_SERVICES *BootServices,
  IN  AMD_LATE_PARAMS    *InitLate
  )
{
}

/*---------------------------------------------------------------------------------------*/
/**
 *  OemCustomizeS3Save
 *
 *  Description:
 *    This is the stub function will call the host environment through the binary block
 *    interface (call-out port) to provide a user hook opportunity
 *
 *  Parameters:
 *    @param[in]      *BootServices
 *    @param[in]      *S3Save
 *
 *    @retval         VOID
 *
 **/
/*---------------------------------------------------------------------------------------*/
VOID
OemCustomizeS3Save (
  IN  EFI_BOOT_SERVICES   *BootServices,
  IN  AMD_S3SAVE_PARAMS   *S3Save
  )
{
}

/*---------------------------------------------------------------------------------------*/
/**
 *  OemHookAfterInitEnv
 *
 *  Description:
 *    This is the stub function which will be called after Agesa AmdInitEnv
 *    enrty point
 *
 *  Parameters:
 *    @param[in]      *BootServices
 *    @param[in]      *InitEnv
 *
 *    @retval         VOID
 *
 **/
/*---------------------------------------------------------------------------------------*/
VOID
OemHookAfterInitEnv (
  IN  EFI_BOOT_SERVICES   *BootServices,
  IN  AMD_ENV_PARAMS      *InitEnv
  )
{
}

/*---------------------------------------------------------------------------------------*/
/**
 *  OemHookAfterInitMid
 *
 *  Description:
 *    This is the stub function which will be called after Agesa AmdInitMid
 *    enrty point
 *
 *  Parameters:
 *    @param[in]      *BootServices
 *    @param[in]      *InitMid
 *
 *    @retval         VOID
 *
 **/
/*---------------------------------------------------------------------------------------*/
VOID
OemHookAfterInitMid (
  IN  EFI_BOOT_SERVICES   *BootServices,
  IN  AMD_MID_PARAMS      *InitMid
  )
{
}

/*---------------------------------------------------------------------------------------*/
/**
 *  OemHookAfterInitLate
 *
 *  Description:
 *    This is the stub function which will be called after Agesa AmdInitLate
 *    enrty point
 *
 *  Parameters:
 *    @param[in]      *BootServices
 *    @param[in]      *InitLate
 *
 *    @retval         VOID
 *
 **/
/*---------------------------------------------------------------------------------------*/
VOID
OemHookAfterInitLate (
  IN  EFI_BOOT_SERVICES   *BootServices,
  IN  AMD_LATE_PARAMS      *InitLate
  )
{
}