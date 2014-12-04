/* $NoKeywords:$ */
/**
 * @file
 *
 * GNB env init interface
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 290085 $   @e \$Date: 2014-04-18 13:53:57 -0500 (Fri, 18 Apr 2014) $
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
#include  "OptionGnb.h"
#include  "GnbLibFeatures.h"
#include  "GnbFamServices.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_GNBINITATENV_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
extern  OPTION_GNB_CONFIGURATION  GnbEnvFeatureTable[];
extern  BUILD_OPT_CFG UserOptions;
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
 * @param[in] GnbEnvConfigPtr     Pointer to gnb env configuration params.
 * @param[in] EnvParamsPtr        Pointer to env configuration params.
 */
VOID
GnbInitDataStructAtEnvDef (
  IN OUT   GNB_ENV_CONFIGURATION   *GnbEnvConfigPtr,
  IN       AMD_ENV_PARAMS          *EnvParamsPtr
  )
{
  GNB_BUILD_OPTIONS_COMMON  *GnbCommonOptions;

  GnbCommonOptions = (GNB_BUILD_OPTIONS_COMMON*) GnbFmGnbBuildOptions (&EnvParamsPtr->StdHeader);
  GnbEnvConfigPtr->PcieRefClkSpreadSpectrum = GnbCommonOptions->CfgPcieRefclkSpreadSpectrum;

  GnbEnvConfigPtr->Gnb3dStereoPinIndex = UserOptions.CfgGnb3dStereoPinIndex;
  GnbEnvConfigPtr->IommuSupport = UserOptions.CfgIommuSupport;
  GnbEnvConfigPtr->GpuFrequencyLimit = UserOptions.CfgGpuFrequencyLimit;
  GnbEnvConfigPtr->LvdsSpreadSpectrum = UserOptions.CfgLvdsSpreadSpectrum;
  GnbEnvConfigPtr->LvdsSpreadSpectrumRate = UserOptions.CfgLvdsSpreadSpectrumRate;
  GnbEnvConfigPtr->LvdsPowerOnSeqDigonToDe = UserOptions.CfgLvdsPowerOnSeqDigonToDe;
  GnbEnvConfigPtr->LvdsPowerOnSeqDeToVaryBl = UserOptions.CfgLvdsPowerOnSeqDeToVaryBl;
  GnbEnvConfigPtr->LvdsPowerOnSeqDeToDigon = UserOptions.CfgLvdsPowerOnSeqDeToDigon;
  GnbEnvConfigPtr->LvdsPowerOnSeqVaryBlToDe = UserOptions.CfgLvdsPowerOnSeqVaryBlToDe;
  GnbEnvConfigPtr->LvdsPowerOnSeqOnToOffDelay = UserOptions.CfgLvdsPowerOnSeqOnToOffDelay;
  GnbEnvConfigPtr->LvdsPowerOnSeqVaryBlToBlon = UserOptions.CfgLvdsPowerOnSeqVaryBlToBlon;
  GnbEnvConfigPtr->LvdsPowerOnSeqBlonToVaryBl = UserOptions.CfgLvdsPowerOnSeqBlonToVaryBl;
  GnbEnvConfigPtr->LvdsMaxPixelClockFreq = UserOptions.CfgLvdsMaxPixelClockFreq;
  GnbEnvConfigPtr->LcdBitDepthControlValue = UserOptions.CfgLcdBitDepthControlValue;
  GnbEnvConfigPtr->Lvds24bbpPanelMode = UserOptions.CfgLvds24bbpPanelMode;
  GnbEnvConfigPtr->LvdsMiscControl.Value = 0;
  GnbEnvConfigPtr->LvdsMiscControl.Value = UserOptions.CfgLvdsMiscControl.Value;
  if (UserOptions.CfgPcieRefClkSpreadSpectrum != 0) {
    GnbEnvConfigPtr->PcieRefClkSpreadSpectrum = UserOptions.CfgPcieRefClkSpreadSpectrum;
  }
  GnbEnvConfigPtr->GnbRemoteDisplaySupport = UserOptions.CfgGnbRemoteDisplaySupport;
  GnbEnvConfigPtr->LvdsMiscVoltAdjustment = UserOptions.CfgLvdsMiscVoltAdjustment;
  GnbEnvConfigPtr->DisplayMiscControl.Value = UserOptions.CfgDisplayMiscControl.Value;
  GnbEnvConfigPtr->DpFixedVoltSwingType = UserOptions.CfgDpFixedVoltSwingType;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Init GNB at Env
 *
 *
 *
 * @param[in] EnvParamsPtr        Pointer to env configuration params.
 * @retval                        Initialization status.
 */

AGESA_STATUS
GnbInitAtEnv (
  IN       AMD_ENV_PARAMS          *EnvParamsPtr
  )
{
  AGESA_STATUS  Status;
  Status = GnbLibDispatchFeatures (&GnbEnvFeatureTable[0], &EnvParamsPtr->StdHeader);
  return  Status;
}