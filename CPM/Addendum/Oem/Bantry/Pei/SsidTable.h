/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM OEM API, and related functions.
 *
 * Contains the table definition for SSID.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  OEM
 * @e \$Revision: 281158 $   @e \$Date: 2013-12-17 20:36:04 -0600 (Tue, 17 Dec 2013) $
 *
/*****************************************************************************
 *
 * Copyright 2012 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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
#ifndef _SSID_TABLE_H_
#define _SSID_TABLE_H_
#include "AmdCpmOemInitPeim.h"

//
// CPM Specific Ssid Table
//
AMD_CPM_SPECIFIC_SSID_TABLE gCpmSpecificSsidTable = {
  {CPM_SIGNATURE_SPECIFIC_SSID, sizeof (gCpmSpecificSsidTable) / sizeof (UINT8), 0, 0, 0, 0x06},
  {
    {   0x1002,   0x6610    },      // Oland XT
    {   0x1002,   0x6611    },      // Oland PRO
    {   0x1002,   0x6613    },      // Oland PRO
    {   0x1002,   0x6771    },      // Caicos
    {   0x1002,   0x6778    },      // Caicos
    {   0x1002,   0x6779    },      // Caicos
    {   0x1002,   0x6822    },      // VenusMCM
    {   0x1002,   0x682A    },      // VenusMCM
    {   0x1002,   0x6900    },      // Iceland
    {   0x1002,   0x6901    },      // Iceland

    {   0x1002,   0x1304    },      // Kaveri
    {   0x1002,   0x1305    },
    {   0x1002,   0x1306    },
    {   0x1002,   0x1307    },
    {   0x1002,   0x1308    },
    {   0x1002,   0x1309    },
    {   0x1002,   0x130A    },
    {   0x1002,   0x130B    },
    {   0x1002,   0x130C    },
    {   0x1002,   0x130D    },
    {   0x1002,   0x130E    },
    {   0x1002,   0x130F    },
    {   0x1002,   0x1310    },
    {   0x1002,   0x1311    },
    {   0x1002,   0x1312    },
    {   0x1002,   0x1313    },
    {   0x1002,   0x1315    },
    {   0x1002,   0x1316    },
    {   0x1002,   0x1317    },
    {   0x1002,   0x1318    },
    {   0x1002,   0x1319    },
    {   0x1002,   0x131A    },
    {   0x1002,   0x131B    },
    {   0x1002,   0x131C    },
    {   0x1002,   0x131D    },
    {   0x1002,   0x131E    },
    {   0x1002,   0x131F    },

    {   0x1002,   0x9900    },      // Trinity
    {   0x1002,   0x9901    },
    {   0x1002,   0x9902    },
    {   0x1002,   0x9903    },
    {   0x1002,   0x9904    },
    {   0x1002,   0x9905    },
    {   0x1002,   0x9906    },
    {   0x1002,   0x9907    },
    {   0x1002,   0x9908    },
    {   0x1002,   0x9909    },
    {   0x1002,   0x990A    },
    {   0x1002,   0x990B    },
    {   0x1002,   0x990C    },
    {   0x1002,   0x990D    },
    {   0x1002,   0x990D    },
    {   0x1002,   0x990E    },
    {   0x1002,   0x990F    },
    {   0x1002,   0x9910    },
    {   0x1002,   0x9911    },
    {   0x1002,   0x9912    },
    {   0x1002,   0x9913    },
    {   0x1002,   0x9914    },
    {   0x1002,   0x9915    },
    {   0x1002,   0x9916    },
    {   0x1002,   0x9917    },
    {   0x1002,   0x9918    },
    {   0x1002,   0x9919    },
    {   0x1002,   0x991A    },
    {   0x1002,   0x991B    },
    {   0x1002,   0x991C    },
    {   0x1002,   0x991D    },
    {   0x1002,   0x991E    },
    {   0x1002,   0x991F    },
    {   0x1002,   0x9911    },
    {   0x1002,   0x9911    },
    {   0x1002,   0x9911    },
    {   0x1002,   0x9911    },
    {   0x1002,   0x9911    },
    {   0x1002,   0x9911    },
    {   0x1002,   0x9990    },
    {   0x1002,   0x9991    },
    {   0x1002,   0x9992    },
    {   0x1002,   0x9993    },
    {   0x1002,   0x9994    },
    {   0x1002,   0x9995    },
    {   0x1002,   0x9996    },
    {   0x1002,   0x9997    },
    {   0x1002,   0x9998    },
    {   0x1002,   0x9999    },
    {   0x1002,   0x999A    },
    {   0x1002,   0x999B    },
    {   0x1002,   0x999C    },
    {   0x1002,   0x999D    },
    {   0x1002,   0x999E    },
    {   0x1002,   0x999F    },
    {   0x1002,   0x99A0    },
    {   0x1002,   0x99A1    },
    {   0x1002,   0x99A2    },
    {   0x1002,   0x99A3    },
    {   0x1002,   0x99A4    },
    {   0x1002,   0x99A5    },
    {   0x1002,   0x99A6    },
    {   0x1002,   0x99A7    },
    {   0x1002,   0x99A8    },
    {   0x1002,   0x99A9    },
    {   0x1002,   0x99AA    },
    {   0x1002,   0x99AB    },
    {   0x1002,   0x99AC    },
    {   0x1002,   0x99AD    },
    {   0x1002,   0x99AE    },
    {   0x1002,   0x99AF    },

    {   0xFFFF,   0xFFFF    },      //End of Table
  }
};
#endif