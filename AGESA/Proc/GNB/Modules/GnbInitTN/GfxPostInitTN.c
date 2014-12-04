/* $NoKeywords:$ */
/**
 * @file
 *
 * Family-specific graphics initialization at AMD_INIT_POST
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
#include  "amdlib.h"
#include  "Gnb.h"
#include  "GnbPcie.h"
#include  "GnbGfx.h"
#include  "GnbGfxConfig.h"
#include  "GnbFuseTable.h"
#include  "GnbGfxInitLibV1.h"
#include  "GnbRegistersTN.h"
#include  "PcieConfigData.h"
#include  "PcieConfigLib.h"
#include  "PcieComplexDataTN.h"
#include  "GnbPcieInitLibV1.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITTN_GFXPOSTINITTN_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


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
 * Callback to init various features on all ports
 *
 *
 *
 *
 * @param[in]       Engine          Pointer to engine config descriptor
 * @param[in, out]  Buffer          Not used
 * @param[in]       Pcie            Pointer to global PCIe configuration
 *
 */

VOID
STATIC
PcieGfxPifControlCallbackTN (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  GFX_CARD_CARD_INFO        *GfxDiscreteCardInfo;
  PCIe_WRAPPER_CONFIG       *Wrapper;
  UINT32                    AmdGfxCardBitmap;
  UINT8                     Pif;
  D0F0xE4_PIF_0011_STRUCT   D0F0xE4_PIF_0011;

  ASSERT (Engine->EngineData.EngineType == PciePortEngine);

  GfxDiscreteCardInfo = (GFX_CARD_CARD_INFO*) Buffer;
  Wrapper = PcieConfigGetParentWrapper (Engine);

  if (Wrapper->WrapId == GFX_WRAP_ID) {
    if (((Engine->Type.Port.StartCoreLane == 0) && (Engine->Type.Port.EndCoreLane == 15)) || ((Engine->Type.Port.StartCoreLane == 15) && (Engine->Type.Port.EndCoreLane == 0))) {
      AmdGfxCardBitmap = GfxDiscreteCardInfo->AmdPcieGfxCardBitmap;
      AmdGfxCardBitmap &= 1 << Engine->Type.Port.PortData.DeviceNumber;
      if (AmdGfxCardBitmap != 0) {
        for (Pif = 0; Pif < 2; Pif++) {
          D0F0xE4_PIF_0011.Value = PcieRegisterRead (
                                     Wrapper,
                                     PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0011_ADDRESS),
                                     Pcie
                                     );
          D0F0xE4_PIF_0011.Field.X8Lane70 = 0x1;
          D0F0xE4_PIF_0011.Field.MultiPif = 0x0;
          PcieRegisterWrite (
              Wrapper,
              PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0011_ADDRESS),
              D0F0xE4_PIF_0011.Value,
              FALSE,
              Pcie
              );
        }
      }
    }
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * GFX PIF Control
 *
 *
 *
 * @param[in]  StdHeader            Standard configuration header
 * @param[in]  GfxDiscreteCardInfo  Enumerated discrete graphics information structure
 * @retval     AGESA_STATUS
 */
VOID
PcieGfxPifControlTN (
  IN      AMD_CONFIG_PARAMS               *StdHeader,
  IN      GFX_CARD_CARD_INFO              GfxDiscreteCardInfo
  )
{
  AGESA_STATUS          AgesaStatus;
  AGESA_STATUS          Status;
  PCIe_PLATFORM_CONFIG  *Pcie;
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieGfxPifControlTN Enter\n");
  AgesaStatus = AGESA_SUCCESS;
  Status = PcieLocateConfigurationData (StdHeader, &Pcie);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);
  if (Status == AGESA_SUCCESS) {
    PcieConfigRunProcForAllEngines (
      DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
      PcieGfxPifControlCallbackTN,
      (VOID*) &GfxDiscreteCardInfo,
      Pcie
      );
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieGfxPifControlTN Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Init GFX at Post.
 *
 *
 *
 * @param[in]     StdHeader     Standard configuration header
 * @retval        AGESA_STATUS
 */


AGESA_STATUS
GfxPostInterfaceTN (
  IN       AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AMD_POST_PARAMS       *PostParamsPtr;
  GFX_CARD_CARD_INFO    GfxDiscreteCardInfo;
  AGESA_STATUS          Status;
  GFX_PLATFORM_CONFIG   *Gfx;
  PostParamsPtr = (AMD_POST_PARAMS *)StdHeader;
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxPostInterfaceTN Enter\n");
  Status = GfxLocateConfigData (StdHeader, &Gfx);
  ASSERT (Status == AGESA_SUCCESS);
  if (Status == AGESA_SUCCESS) {
    if (GfxLibIsControllerPresent (StdHeader)) {
      if (PostParamsPtr->MemConfig.UmaMode != UMA_NONE) {
        LibAmdMemFill (&GfxDiscreteCardInfo, 0x0, sizeof (GfxDiscreteCardInfo), StdHeader);
        GfxGetDiscreteCardInfo (&GfxDiscreteCardInfo, StdHeader);
        if (GfxDiscreteCardInfo.AmdPcieGfxCardBitmap != 0) {
          PcieGfxPifControlTN (StdHeader, GfxDiscreteCardInfo);
        }
        if (((GfxDiscreteCardInfo.PciGfxCardBitmap != 0) ||
          (GfxDiscreteCardInfo.AmdPcieGfxCardBitmap != GfxDiscreteCardInfo.PcieGfxCardBitmap)) ||
          ((PostParamsPtr->GnbPostConfig.IgpuEnableDisablePolicy == IGPU_DISABLE_ANY_PCIE)  &&
          ((GfxDiscreteCardInfo.PciGfxCardBitmap != 0) || (GfxDiscreteCardInfo.PcieGfxCardBitmap != 0)))) {
          PostParamsPtr->MemConfig.UmaMode = UMA_NONE;
          IDS_HDT_CONSOLE (GFX_MISC, "  GfxDisabled due dGPU policy\n");
        }
      }
    } else {
      PostParamsPtr->MemConfig.UmaMode = UMA_NONE;
      Gfx->GfxFusedOff = TRUE;
    }
  } else {
    PostParamsPtr->MemConfig.UmaMode = UMA_NONE;
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxPostInterfaceTN Exit [0x%x]\n", Status);
  return  Status;
}
