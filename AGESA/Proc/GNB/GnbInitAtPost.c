/* $NoKeywords:$ */
/**
 * @file
 *
 * GNB POST init interface
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
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

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include  "AGESA.h"
#include  "Ids.h"
#include  "Gnb.h"
#include  "OptionGnb.h"
#include  "GnbLibFeatures.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_GNBINITATPOST_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
extern  OPTION_GNB_CONFIGURATION  GnbPostFeatureTable[];
extern  OPTION_GNB_CONFIGURATION  GnbPostAfterDramFeatureTable[];
extern  BUILD_OPT_CFG             UserOptions;

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */
/*----------------------------------------------------------------------------------------*/
/**
 * Default constructor of GNB configuration at Env
 *
 *
 *
 * @param[in] GnbPostConfigPtr     Pointer to GNB POST configuration params.
 * @param[in] PostParamsPtr        Pointer to POST configuration params.
 */
VOID
GnbInitDataStructAtPostDef (
  IN OUT   GNB_POST_CONFIGURATION  *GnbPostConfigPtr,
  IN       AMD_POST_PARAMS         *PostParamsPtr
  )
{
  GnbPostConfigPtr->IgpuEnableDisablePolicy = UserOptions.CfgIgpuEnableDisablePolicy;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Init GNB at Post
 *
 *
 *
 * @param[in]     PostParamsPtr Pointer to post configuration parameters
 * @retval                      Initialization status.
 */

AGESA_STATUS
GnbInitAtPost (
  IN OUT   AMD_POST_PARAMS         *PostParamsPtr
  )
{
  AGESA_STATUS          Status;
  Status = GnbLibDispatchFeatures (&GnbPostFeatureTable[0], &PostParamsPtr->StdHeader);
  return  Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Init GNB at Post after DRAM init
 *
 *
 *
 * @param[in]     PostParamsPtr Pointer to post configuration parameters
 * @retval                      Initialization status.
 */

AGESA_STATUS
GnbInitAtPostAfterDram (
  IN OUT   AMD_POST_PARAMS         *PostParamsPtr
  )
{
  AGESA_STATUS          Status;
  Status = GnbLibDispatchFeatures (&GnbPostAfterDramFeatureTable[0], &PostParamsPtr->StdHeader);
  return  Status;
}
