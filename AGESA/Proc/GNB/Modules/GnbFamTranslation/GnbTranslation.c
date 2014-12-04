/* $NoKeywords:$ */
/**
 * @file
 *
 * Family specific function translation
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
#include  "GnbPcieConfig.h"
#include  "GnbFamServices.h"
#include  "GnbGfxFamServices.h"
#include  "GnbCommonLib.h"
#include  "GnbGfx.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBFAMTRANSLATION_GNBTRANSLATION_FILECODE
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
 * Check if IOMMU unit present and enabled
 *
 *
 *
 *
 * @param[in]  GnbHandle       Gnb handle
 * @param[in]  StdHeader       Standard configuration header
 *
 */
BOOLEAN
GnbFmCheckIommuPresent (
  IN       GNB_HANDLE                 *GnbHandle,
  IN       AMD_CONFIG_PARAMS          *StdHeader
  )
{
  AGESA_STATUS              Status;
  GNB_FAM_IOMMU_SERVICES    *GnbIommuConfigService;
  Status = GnbLibLocateService (GnbIommuService, GnbGetSocketId (GnbHandle), &GnbIommuConfigService, StdHeader);
  ASSERT (Status == AGESA_SUCCESS);
  if (Status == AGESA_SUCCESS) {
    return GnbIommuConfigService->GnbFmCheckIommuPresent (GnbHandle, StdHeader);
  }
  return FALSE;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Create IVRS entry
 *
 *
 * @param[in]  GnbHandle       Gnb handle
 * @param[in]  Type            Entry type
 * @param[in]  Ivrs            IVRS table pointer
 * @param[in]  StdHeader       Standard configuration header
 *
 */

AGESA_STATUS
GnbFmCreateIvrsEntry (
  IN       GNB_HANDLE                 *GnbHandle,
  IN       IVRS_BLOCK_TYPE            Type,
  IN       VOID                       *Ivrs,
  IN       AMD_CONFIG_PARAMS          *StdHeader
  )
{
  AGESA_STATUS              Status;
  GNB_FAM_IOMMU_SERVICES    *GnbIommuConfigService;
  Status = GnbLibLocateService (GnbIommuService, GnbGetSocketId (GnbHandle), &GnbIommuConfigService, StdHeader);
  ASSERT (Status == AGESA_SUCCESS);
  if (Status == AGESA_SUCCESS) {
    return GnbIommuConfigService->GnbFmCreateIvrsEntry (GnbHandle, Type, Ivrs, StdHeader);
  }
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Map graphics engine to display path
 *
 *
 *
 *
 * @param[in]   Engine            Engine configuration info
 * @param[out]  DisplayPathList   Display path list
 * @param[in]   Gfx               Pointer to global GFX configuration
 *
 */
AGESA_STATUS
GfxFmMapEngineToDisplayPath (
  IN       PCIe_ENGINE_CONFIG          *Engine,
     OUT   EXT_DISPLAY_PATH            *DisplayPathList,
  IN       GFX_PLATFORM_CONFIG         *Gfx
  )
{
  AGESA_STATUS              Status;
  GFX_FAM_SERVICES          *GfxFamilyService;
  GNB_HANDLE                *GnbHandle;
  GnbHandle = GnbGetHandle (GnbLibGetHeader (Gfx));
  Status = GnbLibLocateService (GfxFamService, GnbGetSocketId (GnbHandle), &GfxFamilyService, GnbLibGetHeader (Gfx));
  ASSERT (Status == AGESA_SUCCESS);
  if (Status == AGESA_SUCCESS) {
    return GfxFamilyService->GfxMapEngineToDisplayPath (Engine, DisplayPathList, Gfx);
  }
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Calculate COF for DFS out of Main PLL
 *
 *
 *
 * @param[in] Did         Did
 * @param[in] StdHeader   Standard Configuration Header
 * @retval                COF in 10khz
 */

UINT32
GfxFmCalculateClock (
  IN       UINT8                      Did,
  IN       AMD_CONFIG_PARAMS          *StdHeader
  )
{
  AGESA_STATUS              Status;
  GFX_FAM_SERVICES          *GfxFamilyService;
  GNB_HANDLE                *GnbHandle;
  GnbHandle = GnbGetHandle (StdHeader);
  Status = GnbLibLocateService (GfxFamService, GnbGetSocketId (GnbHandle), &GfxFamilyService, StdHeader);
  ASSERT (Status == AGESA_SUCCESS);
  if (Status == AGESA_SUCCESS) {
    return GfxFamilyService->GfxCalculateClock (Did, StdHeader);
  }
  return 200*100;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Disable GFX controller
 *
 *
 *
 * @param[in] StdHeader  Standard configuration header
 */

VOID
GfxFmDisableController (
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  AGESA_STATUS              Status;
  GFX_FAM_SERVICES          *GfxFamilyService;
  GNB_HANDLE                *GnbHandle;
  GnbHandle = GnbGetHandle (StdHeader);
  Status = GnbLibLocateService (GfxFamService, GnbGetSocketId (GnbHandle), &GfxFamilyService, StdHeader);
  ASSERT (Status == AGESA_SUCCESS);
  if (Status == AGESA_SUCCESS) {
    GfxFamilyService->GfxDisableController (StdHeader);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Set idle voltage mode for GFX
 *
 *
 * @param[in] Gfx             Pointer to global GFX configuration
 */

BOOLEAN
GfxFmIsVbiosPosted (
  IN      GFX_PLATFORM_CONFIG   *Gfx
  )
{
  AGESA_STATUS              Status;
  GFX_FAM_SERVICES          *GfxFamilyService;
  GNB_HANDLE                *GnbHandle;
  GnbHandle = GnbGetHandle (GnbLibGetHeader (Gfx));
  Status = GnbLibLocateService (GfxFamService, GnbGetSocketId (GnbHandle), &GfxFamilyService, GnbLibGetHeader (Gfx));
  ASSERT (Status == AGESA_SUCCESS);
  if (Status == AGESA_SUCCESS) {
    return GfxFamilyService->GfxIsVbiosPosted (Gfx);
  }
  return TRUE;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get base SSDT table
 *
 *
 *
 * @param[in]     StdHeader       Standard Configuration Header
 * @retval        pointer to SSTD table
 */
VOID *
GnbFmAlibGetBaseTable (
  IN       AMD_CONFIG_PARAMS             *StdHeader
  )
{
  AGESA_STATUS              Status;
  GNB_FAM_ALIB_SERVICES     *GnbFamilyAlibService;
  GNB_HANDLE                *GnbHandle;
  VOID                      *AlibSsdtTable;

  AlibSsdtTable = NULL;
  GnbHandle = GnbGetHandle (StdHeader);
  Status = GnbLibLocateService (GnbFamAlibService, GnbGetSocketId (GnbHandle), &GnbFamilyAlibService, StdHeader);
  ASSERT (Status == AGESA_SUCCESS);
  if (Status == AGESA_SUCCESS) {
    AlibSsdtTable = GnbFamilyAlibService->GnbFmAlibGetBaseTable (StdHeader);
  }

  return AlibSsdtTable;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get GnbBuildOptionsCommon Structure
 *
 *
 *
 * @param[in]     StdHeader       Standard Configuration Header
 * @retval        pointer to SSTD table
 */
VOID *
GnbFmGnbBuildOptions (
  IN       AMD_CONFIG_PARAMS             *StdHeader
  )
{
  AGESA_STATUS                    Status;
  GNB_FAM_BUILDOPTIONS_POINTERS   *GnbBuildOptionsPointer;
  GNB_HANDLE                      *GnbHandle;
  VOID                            *GnbBuildOptionsPtr;

  GnbBuildOptionsPtr = NULL;
  GnbHandle = GnbGetHandle (StdHeader);
  Status = GnbLibLocateService (GnbFamBuildOptionPointer, GnbGetSocketId (GnbHandle), &GnbBuildOptionsPointer, StdHeader);
  ASSERT (Status == AGESA_SUCCESS);
  if (Status == AGESA_SUCCESS) {
    GnbBuildOptionsPtr = (VOID*) (GnbBuildOptionsPointer->GnbFmBuildOptionsPtr);
  }

  return GnbBuildOptionsPtr;
}


