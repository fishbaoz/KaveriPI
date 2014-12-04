/* $NoKeywords:$ */
/**
 * @file
 *
 * Family specific GFX library
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
#include  "amdlib.h"
#include  "Ids.h"
#include  "S3SaveState.h"
#include  "Gnb.h"
#include  "GnbPcieConfig.h"
#include  "GnbGfx.h"
#include  "GfxLibKV.h"
#include  "GnbCommonLib.h"
#include  "GnbRegisterAccKV.h"
#include  "GnbRegistersKV.h"
#include  "cpuLateInit.h"
#include  "heapManager.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITKV_GFXLIBKV_FILECODE
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
 * Disable GFX controller
 *
 *
 *
 * @param[in] StdHeader  Standard configuration header
 */

VOID
GfxDisableControllerKV (
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  GnbLibPciRMW (
    MAKE_SBDFO (0, 0, 0, 0,D0F0x7C_ADDRESS),
    AccessS3SaveWidth32,
    0xffffffff,
    1 << D0F0x7C_ForceIntGfxDisable_OFFSET,
    StdHeader
    );
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get system PLL COF
 *
 *
 *
 * @param[in] StdHeader  Standard configuration header
 * @retval    System PLL COF
 */
UINT32
GfxLibGetSystemPllCofKV (
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  D0F0xBC_xC0500000_STRUCT  D0F0xBC_xC0500000;
  GnbRegisterReadKV (GnbGetHandle (StdHeader), D0F0xBC_xC0500000_TYPE,  D0F0xBC_xC0500000_ADDRESS, &D0F0xBC_xC0500000.Value, 0, StdHeader);
  return 100 * (D0F0xBC_xC0500000.Field.MainPllOpFreqIdStartup + 0x10);
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
GfxCalculateClockKV (
  IN       UINT8                       Did,
  IN       AMD_CONFIG_PARAMS          *StdHeader
  )
{
  UINT32  Divider;
  UINT32  SystemPllCof;
  SystemPllCof = GfxLibGetSystemPllCofKV (StdHeader) * 100;
  if (Did >= 8 && Did <= 0x3F) {
    Divider = Did * 25;
  } else if (Did > 0x3F && Did <= 0x5F) {
    Divider = (Did - 64) * 50 + 1600;
  } else if (Did > 0x5F && Did <= 0x7E) {
    Divider = (Did - 96) * 100 + 3200;
  } else if (Did == 0x7f) {
    Divider = 128 * 100;
  } else {
    ASSERT (FALSE);
    return 200 * 100;
  }
  ASSERT (Divider != 0);
  return (((SystemPllCof * 100) + (Divider - 1)) / Divider);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Check if Video BIOS has posted or not
 *
 *
 * @param[in] Gfx             Pointer to global GFX configuration
 */

BOOLEAN
GfxIsVbiosPostedKV (
  IN      GFX_PLATFORM_CONFIG   *Gfx
  )
{
  GMMx1740_STRUCT GMMx1740;
  GnbRegisterReadKV (GnbGetHandle (GnbLibGetHeader (Gfx)), GMMx1740_TYPE, GMMx1740_ADDRESS, &GMMx1740.Value, 0, GnbLibGetHeader (Gfx));

  return ((GMMx1740.Value & ATOM_S7_ASIC_INIT_COMPLETEb1) != 0) ? TRUE : FALSE;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Set VID through CG client
 *
 *
 * @param[in] Vid             VID code
 * @param[in] StdHeader       Standard configuration header
 * @retval    AGESA_STATUS
 */

AGESA_STATUS
GfxRequestVoltageKV (
  IN       UINT8                 Vid,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{

  return AGESA_SUCCESS;
}

/*---------------------------------------------------------------------------------------*/
/**
 * This function will add HSA Processing Unit entry for Gnb.
 *
 *    @param[in, out] StdHeader            Standard Head Pointer
 *    @retval         AGESA_ERROR          Crat table entry is not created successfully
 *    @retval         AGESA_SUCCESS        Crat table entry is created successfully
 */
AGESA_STATUS
GfxMakeHSAProcUnitEntryKV (
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  CRAT_HSA_PROCESSING_UNIT   *HsaProcUnit;

  HsaProcUnit = (CRAT_HSA_PROCESSING_UNIT *) GnbAllocateHeapBuffer (AMD_GNB_CRAT_HSA_TABLE_HANDLE, sizeof (CRAT_HSA_PROCESSING_UNIT), StdHeader);
  if (HsaProcUnit == NULL) {
    return AGESA_ERROR;
  }
  HsaProcUnit->Length = 40;
  HsaProcUnit->Flags.Enabled = 1;
  HsaProcUnit->Flags.GpuPresent = 1;
  HsaProcUnit->ProcessorIdLow = 0x80000000;
  HsaProcUnit->NumSIMDCores = GPU__GC__NUM_SE * GPU__GC__NUM_SH_PER_SE * GPU__GC__NUM_CU_PER_SH;
  HsaProcUnit->MaxWavesSIMD = 40;
  HsaProcUnit->LDSSizeInKB = 64;
  HsaProcUnit->WaveFrontSize = 64;
  HsaProcUnit->NumBanks = GPU__GC__NUM_SE;
  HsaProcUnit->NumArrays = GPU__GC__NUM_SH_PER_SE;
  HsaProcUnit->NumCUPerArray = GPU__GC__NUM_SH_PER_SE * GPU__GC__NUM_CU_PER_SH;
  HsaProcUnit->NumSIMDPerCU = 4;
  HsaProcUnit->MaxSlotsScratchCU = 128;

  return AGESA_SUCCESS;
}