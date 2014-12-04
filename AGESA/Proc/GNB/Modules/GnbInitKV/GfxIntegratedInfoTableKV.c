/* $NoKeywords:$ */
/**
 * @file
 *
 * Integrated table info init
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
#include  "heapManager.h"
#include  "GeneralServices.h"
#include  "Gnb.h"
#include  "GnbFuseTable.h"
#include  "GnbPcie.h"
#include  "GnbGfx.h"
#include  "GnbSbLib.h"
#include  "GnbCommonLib.h"
#include  "GnbPcieConfig.h"
#include  "GnbGfxConfig.h"
#include  "GfxPwrPlayTable.h"
#include  "GnbGfxInitLibV1.h"
#include  "GnbGfxFamServices.h"
#include  "GnbRegistersKV.h"
#include  "GnbRegisterAccKV.h"
#include  "GnbNbInitLibV1.h"
#include  "GfxConfigLib.h"
#include  "GfxIntegratedInfoTable.h"
#include  "GfxLibKV.h"
#include  "GfxLibV3.h"
#include  "OptionGnb.h"
#include  "cpuFamilyTranslation.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITKV_GFXINTEGRATEDINFOTABLEKV_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

#define GFX_REFCLK                        100     // (in MHz) Reference clock is 100 MHz
#define GFX_NCLK_MIN                      700     // (in MHz) Minimum value for NCLK is 700 MHz

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */
CONST UINT8  DdiLaneConfigArrayKV [][4] = {
  {39, 32, 1, 0},
  {32, 39, 0, 1},
  {32, 35, 0, 0},
  {35, 32, 0, 0},
  {39, 36, 1, 1},
  {36, 39, 1, 1},
  {7 , 4 , 2, 2},
  {4 , 7 , 2, 2},
  {23, 16, 3, 3},
  {16, 23, 3, 3},
  {23, 20, 3, 3},
  {20, 23, 3, 3},
  {27, 24, 4, 4},
  {24, 27, 4, 4},
  {24, 31, 4, 5},
  {31, 24, 5, 4},
  {31, 28, 5, 5},
  {28, 31, 5, 5},
  {15, 8 , 6, 6},
  {8 , 15, 6, 6},
  {15, 12, 6, 6},
  {12, 15, 6, 6}
};

/*----------------------------------------------------------------------------------------*/
/**
 * Initialize display path for given engine
 *
 *
 *
 * @param[in]   Engine            Engine configuration info
 * @param[out]  DisplayPathList   Display path list
 * @param[in]   Gfx               Pointer to global GFX configuration
 */

AGESA_STATUS
GfxMapEngineToDisplayPathKV (
  IN       PCIe_ENGINE_CONFIG          *Engine,
     OUT   EXT_DISPLAY_PATH            *DisplayPathList,
  IN       GFX_PLATFORM_CONFIG         *Gfx
  )
{
  AGESA_STATUS      Status;
  UINT8             PrimaryDisplayPathId;
  UINT8             SecondaryDisplayPathId;
  UINTN             DisplayPathIndex;
  PrimaryDisplayPathId = 0xff;
  SecondaryDisplayPathId = 0xff;
  for (DisplayPathIndex = 0; DisplayPathIndex <  (sizeof (DdiLaneConfigArrayKV) / 4); DisplayPathIndex++) {
    if (DdiLaneConfigArrayKV[DisplayPathIndex][0] == Engine->EngineData.StartLane &&
        DdiLaneConfigArrayKV[DisplayPathIndex][1] == Engine->EngineData.EndLane) {
      PrimaryDisplayPathId = DdiLaneConfigArrayKV[DisplayPathIndex][2];
      SecondaryDisplayPathId = DdiLaneConfigArrayKV[DisplayPathIndex][3];
      break;
    }
  }
  if (PrimaryDisplayPathId != 0xff) {
    IDS_HDT_CONSOLE (GFX_MISC, "  Allocate Display Connector at Primary sPath[%d]\n", PrimaryDisplayPathId);
    Engine->InitStatus |= INIT_STATUS_DDI_ACTIVE;
    GfxIntegratedCopyDisplayInfo (
      Engine,
      &DisplayPathList[PrimaryDisplayPathId],
      (PrimaryDisplayPathId != SecondaryDisplayPathId) ? &DisplayPathList[SecondaryDisplayPathId] : NULL,
      Gfx
      );
    Status = AGESA_SUCCESS;
  } else {
    IDS_HDT_CONSOLE (GFX_MISC, "  Error!!! Map DDI lanes %d - %d to display path failed\n",
      Engine->EngineData.StartLane,
      Engine->EngineData.EndLane
      );
    PutEventLog (
      AGESA_ERROR,
      GNB_EVENT_INVALID_DDI_LINK_CONFIGURATION,
      Engine->EngineData.StartLane,
      Engine->EngineData.EndLane,
      0,
      0,
      GnbLibGetHeader (Gfx)
      );
    Status = AGESA_ERROR;
  }
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 *Init KV Nb p-State MemclkFreq
 *
 *
 * @param[in] IntegratedInfoTable Integrated info table pointer
 * @param[in] PpFuseArray         Fuse array pointer
 * @param[in] Gfx                 Gfx configuration info
 */

VOID
GfxFillNbPstateMemclkFreqKV (
  IN OUT   ATOM_INTEGRATED_SYSTEM_INFO_V1_8 *IntegratedInfoTable,
  IN       PP_FUSE_ARRAY_V2                 *PpFuseArray,
  IN       GFX_PLATFORM_CONFIG              *Gfx
  )
{
  UINT8                           i;
  UINT8                           Channel;
  ULONG                           memps0_freq;
  ULONG                           memps1_freq;

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxFillNbPstateMemclkFreqKV Enter\n");
  Channel = 0;
  if (((Gfx->UmaInfo.UmaAttributes & UMA_ATTRIBUTE_ON_DCT1) != 0) || ((Gfx->UmaInfo.UmaAttributes & UMA_ATTRIBUTE_ON_DCT3) != 0)) {
    Channel = 3;
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "  Channel =  = %d\n", Channel);

  memps0_freq = 100 * GfxLibExtractDramFrequencyV3 ((UINT8) PpFuseArray->MemClkFreq[Channel], GnbLibGetHeader (Gfx));
  memps1_freq = 100 * GfxLibExtractDramFrequencyV3 ((UINT8) PpFuseArray->M1MemClkFreq[Channel], GnbLibGetHeader (Gfx));

  for (i = 0; i < 4; i++) {
    if (PpFuseArray->NbPstateEn[i] == 1) {
      IntegratedInfoTable->ulNbpStateMemclkFreq[i] = (PpFuseArray->MemPstate[i] == 0) ? memps0_freq : memps1_freq;
    }
    IDS_HDT_CONSOLE (GNB_TRACE, "  Mclk[%d] = %08x\n", i, IntegratedInfoTable->ulNbpStateMemclkFreq[i]);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 *Calculate ulGMCRestoreResetTime
 *
 *
 * @param[in] IntegratedInfoTable Integrated info table pointer
 * @param[in] Gfx                 Gfx configuration info
 * @param[in] PpFuseArray         Fuse array pointer
 * @retval    AGESA_STATUS
 */
AGESA_STATUS
GfxCalculateRestoreResetTimeKV (
  IN       ATOM_INTEGRATED_SYSTEM_INFO_V1_8 *IntegratedInfoTable,
  IN       GFX_PLATFORM_CONFIG              *Gfx,
  IN       PP_FUSE_ARRAY_V2                 *PpFuseArray
  )
{
  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Init TDP Configuration
 *
 *
 * @param[in] IntegratedInfoTable Integrated info table pointer
 * @param[in] Gfx                 Gfx configuration info
 */

VOID
GfxIntInfoTableInitTDPConfigKV (
  IN       ATOM_INTEGRATED_SYSTEM_INFO_V1_8 *IntegratedInfoTable,
  IN       GFX_PLATFORM_CONFIG              *Gfx
  )
{
  UINT32            TargetTDP;
  UINT32            SocketTDP;
  D18F5xE8_STRUCT   D18F5xE8;
  AMD_CONFIG_PARAMS *StdHeader;
  GNB_HANDLE        *GnbHandle;

  // Target TDP
  StdHeader = GnbLibGetHeader (Gfx);
  GnbHandle = GnbGetHandle (StdHeader);

  GnbRegisterReadKV (GnbHandle, TYPE_D18F4, D18F4x1B8_ADDRESS, &SocketTDP, 0, StdHeader);
  SocketTDP &= 0xFFFF;
  GnbRegisterReadKV (GnbHandle, TYPE_D18F5, D18F5xE8_ADDRESS, &D18F5xE8.Value, 0, StdHeader);
  SocketTDP *= D18F5xE8.Field.Tdp2Watt;
  // SocketTDP units are watts with 10 decimal bits
  // Convert to 10's of mW
  SocketTDP = (SocketTDP * 100) >> 10;

  TargetTDP = (UINT32) (((AMD_MID_PARAMS *) StdHeader)->PlatformConfig.PowerCeiling / 100);
  IDS_OPTION_HOOK (IDS_GNB_TARGET_TDP, &TargetTDP, StdHeader);
  // Convert to 10's of mW
  TargetTDP *= 10;
  if (TargetTDP == 0) {
    TargetTDP = SocketTDP;
  }
  IntegratedInfoTable->asTdpConfig.TDP_config.uTDP_Value = SocketTDP;
  IntegratedInfoTable->asTdpConfig.TDP_config.uCTDP_Value = TargetTDP;

  if (TargetTDP > SocketTDP) {
    IntegratedInfoTable->asTdpConfig.TDP_config.uCTDP_Enable = 2;
  } else if (TargetTDP < SocketTDP) {
    IntegratedInfoTable->asTdpConfig.TDP_config.uCTDP_Enable = 1;
  } else {
    IntegratedInfoTable->asTdpConfig.TDP_config.uCTDP_Enable = 0;
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "Target TDP: %d\n", IntegratedInfoTable->asTdpConfig.TDP_config.uCTDP_Value);
  IDS_HDT_CONSOLE (GNB_TRACE, "Socket TDP: %d\n", IntegratedInfoTable->asTdpConfig.TDP_config.uTDP_Value);
  IDS_HDT_CONSOLE (GNB_TRACE, "uCTDP_Enable: %d\n", IntegratedInfoTable->asTdpConfig.TDP_config.uCTDP_Enable);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Build integrated info table
 *
 *
 *
 * @param[in] Gfx             Gfx configuration info
 * @retval    AGESA_STATUS
 */
AGESA_STATUS
STATIC
GfxIntInfoTableInitKV (
  IN      GFX_PLATFORM_CONFIG     *Gfx
  )
{
  AGESA_STATUS                    Status;
  AGESA_STATUS                    AgesaStatus;
  ATOM_FUSION_SYSTEM_INFO_V3      SystemInfoTableV3;
  PP_FUSE_ARRAY_V2                *PpFuseArray;
  ATOM_PPLIB_POWERPLAYTABLE4      *PpTable;
  ACP_ENGINE_T                    *AcpEnginePtr;
  D18F5x170_STRUCT                D18F5x170;
  GNB_HANDLE                      *GnbHandle;
  CPU_LOGICAL_ID                  LogicalId;

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxIntegratedInfoTableInitKV Enter\n");

  AgesaStatus = AGESA_SUCCESS;
  GnbHandle = GnbGetHandle (GnbLibGetHeader (Gfx));
  PpFuseArray = GnbLocateHeapBuffer (AMD_PP_FUSE_TABLE_HANDLE, GnbLibGetHeader (Gfx));

  if (PpFuseArray != NULL) {
    LibAmdMemFill (&SystemInfoTableV3, 0x00, sizeof (ATOM_FUSION_SYSTEM_INFO_V3), GnbLibGetHeader (Gfx));

    // Use common initialization first
    Status = GfxIntInfoTableInitV3 (Gfx, &SystemInfoTableV3, PpFuseArray);
    // Complete table with KV-specific fields

    // Fill in Nb P-state MemclkFreq Data
    GfxFillNbPstateMemclkFreqKV (&SystemInfoTableV3.sIntegratedSysInfo, PpFuseArray, Gfx);

    // Build PP table
    PpTable = (ATOM_PPLIB_POWERPLAYTABLE4*) &SystemInfoTableV3.ulPowerplayTable;
    Status = GfxPwrPlayBuildTable (PpTable,  Gfx);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);

    IDS_HDT_CONSOLE (GNB_TRACE, "KV Pplay done\n");

    // Assign usFormatID to represent Kaveri
    PpTable->usFormatID = 0x13;

    // Family specific data update
    // Determine ulGMCRestoreResetTime
    Status = GfxCalculateRestoreResetTimeKV (&SystemInfoTableV3.sIntegratedSysInfo, Gfx, PpFuseArray);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);

    //GfxFmIntegratedInfoTableInit (&SystemInfoV1Table.sIntegratedSysInfo, Gfx);
    SystemInfoTableV3.sIntegratedSysInfo.ulDDR_DLL_PowerUpTime = 4940;
    SystemInfoTableV3.sIntegratedSysInfo.ulDDR_PLL_PowerUpTime = 2000;

    SystemInfoTableV3.sIntegratedSysInfo.ulGPUCapInfo = GPUCAPINFO_TMDS_HDMI_USE_CASCADE_PLL_MODE | GPUCAPINFO_DP_USE_SINGLE_PLL_MODE;

    GetLogicalIdOfSocket (GnbGetSocketId (GnbHandle), &LogicalId, GnbLibGetHeader (Gfx));
    if ((LogicalId.Revision & AMD_F15_KV_A0) != 0) {
      SystemInfoTableV3.sIntegratedSysInfo.ulGPUCapInfo &= ~GPUCAPINFO_DFS_BYPASS_ENABLE;
    } else {
      SystemInfoTableV3.sIntegratedSysInfo.ulGPUCapInfo |= GPUCAPINFO_DFS_BYPASS_ENABLE;
    }

    // Populate usGPUReservedSysMemSize, usGPUReservedSysMemBaseAddrLo and usGPUReservedSysMemBaseAddrHi
    AcpEnginePtr = (ACP_ENGINE_T *) GnbLocateHeapBuffer (AMD_GNB_ACP_ENGINE_HANDLE, GnbLibGetHeader (Gfx));
    if (AcpEnginePtr != NULL) {
      SystemInfoTableV3.sIntegratedSysInfo.usGPUReservedSysMemSize = (USHORT) AcpEnginePtr->AcpEngineSize;
      SystemInfoTableV3.sIntegratedSysInfo.ulGPUReservedSysMemBaseAddrLo = (ULONG) (AcpEnginePtr->AcpEngineBase & 0xFFFFFFFF);
      SystemInfoTableV3.sIntegratedSysInfo.ulGPUReservedSysMemBaseAddrHi = (ULONG) (AcpEnginePtr->AcpEngineBase >> 32);
    }

    // Check if NbPstate enable
    GnbRegisterReadKV (GnbHandle, TYPE_D18F5, D18F5x170_ADDRESS, &D18F5x170.Value, 0, GnbLibGetHeader (Gfx));
    if ((D18F5x170.Field.SwNbPstateLoDis != 1) && (D18F5x170.Field.NbPstateMaxVal != 0)) {
      // If NbPstate enable, then enable NBDPM for driver
      IDS_HDT_CONSOLE (GNB_TRACE, "  Set NBDPM\n");
      SystemInfoTableV3.sIntegratedSysInfo.ulSystemConfig |= BIT3;
      IDS_HDT_CONSOLE (GNB_TRACE, "ulSystemConfig : %x\n", SystemInfoTableV3.sIntegratedSysInfo.ulSystemConfig);
    }

    GfxIntInfoTableInitTDPConfigKV (&SystemInfoTableV3.sIntegratedSysInfo, Gfx);

    IDS_OPTION_CALLOUT (IDS_CALLOUT_GNB_INTEGRATED_TABLE_CONFIG, &SystemInfoTableV3.sIntegratedSysInfo, GnbLibGetHeader (Gfx));
    IDS_HDT_CONSOLE (GNB_TRACE, "ulGPUCapInfo = 0x%x\n", SystemInfoTableV3.sIntegratedSysInfo.ulGPUCapInfo);
    //Copy integrated info table to Frame Buffer. (Do not use LibAmdMemCopy, routine not guaranteed access to above 4G memory in 32 bit mode.)
    GfxIntInfoTablePostToFbV3 (&SystemInfoTableV3, Gfx);
  } else {
    IDS_HDT_CONSOLE (GNB_TRACE, "  ERROR! Cannot locate fuse table\n");
    Status = AGESA_ERROR;
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxIntegratedInfoTableInitKV Exit [0x%x]\n", Status);
  return  AgesaStatus;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Build integrated info table
 *  GMC FB access requred
 *
 *
 * @param[in]   StdHeader     Standard configuration header
 * @retval      AGESA_STATUS
 */
AGESA_STATUS
GfxIntInfoTableInterfaceKV (
  IN      AMD_CONFIG_PARAMS       *StdHeader
  )
{
  AGESA_STATUS                    AgesaStatus;
  AGESA_STATUS                    Status;
  GFX_PLATFORM_CONFIG             *Gfx;
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxIntegratedInfoTableInterfaceKV Enter\n");
  AgesaStatus = AGESA_SUCCESS;
  if (GfxLibIsControllerPresent (StdHeader)) {
    Status = GfxLocateConfigData (StdHeader, &Gfx);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    if (Status != AGESA_FATAL) {
      Status = GfxIntInfoTableInitKV (Gfx);
      AGESA_STATUS_UPDATE (Status, AgesaStatus);
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxIntegratedInfoTableInterfaceKV Exit[0x%x]\n", AgesaStatus);
  return AgesaStatus;
}
