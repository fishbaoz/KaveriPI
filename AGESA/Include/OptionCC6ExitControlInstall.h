/* $NoKeywords:$ */
/**
 * @file
 *
 * Install of option: CC6 Exit Control
 *
 * Contains AMD AGESA install macros and test conditions. Output is the
 * defaults tables reflecting the User's build options selection.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Options
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
 *
 ***************************************************************************/

#ifndef _OPTION_CC6_EXIT_CONTROL_INSTALL_H_
#define _OPTION_CC6_EXIT_CONTROL_INSTALL_H_

#include "cpuCC6ExitControl.h"

/*  This option is designed to be included into the platform solution install
 *  file. The platform solution install file will define the options status.
 *  Check to validate the definition
 */
#define OPTION_CC6_EXIT_CONTROL_FEAT
#define F15_KV_CC6_EXIT_CONTROL_SUPPORT

#if OPTION_CC6_EXIT_CONTROL == TRUE
  #if (AGESA_ENTRY_INIT_LATE == TRUE) || (AGESA_ENTRY_INIT_FINAL_RESTORE == TRUE)

    #ifdef OPTION_FAMILY15H
      #if OPTION_FAMILY15H == TRUE
        // Family 15h kV
        #if OPTION_FAMILY15H_KV == TRUE
          extern CONST CPU_FEATURE_DESCRIPTOR ROMDATA CpuFeatureCC6ExitControl;
          #undef OPTION_CC6_EXIT_CONTROL_FEAT
          #define OPTION_CC6_EXIT_CONTROL_FEAT &CpuFeatureCC6ExitControl,
          extern CONST CC6_EXIT_CONTROL_FAMILY_SERVICES ROMDATA F15KvCC6ExitControlSupport;
          #undef F15_KV_CC6_EXIT_CONTROL_SUPPORT
          #define F15_KV_CC6_EXIT_CONTROL_SUPPORT {AMD_FAMILY_15_KV, &F15KvCC6ExitControlSupport},
        #endif
      #endif
    #endif

  #endif
#endif

CONST CPU_SPECIFIC_SERVICES_XLAT ROMDATA CC6ExitControlFamilyServiceArray[] =
{
  F15_KV_CC6_EXIT_CONTROL_SUPPORT
  {0, NULL}
};

CONST CPU_FAMILY_SUPPORT_TABLE ROMDATA CC6ExitControlFamilyServiceTable =
{
  (sizeof (CC6ExitControlFamilyServiceArray) / sizeof (CPU_SPECIFIC_SERVICES_XLAT)),
  &CC6ExitControlFamilyServiceArray[0]
};

#endif  // _OPTION_CC6_EXIT_CONTROL_INSTALL_H_
