/* $NoKeywords:$ */
/**
 * @file
 *
 * GNB env post initialization.
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 289975 $   @e \$Date: 2014-04-17 10:05:38 -0500 (Thu, 17 Apr 2014) $
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
#include  "GnbCommonLib.h"
#include  "GnbTable.h"
#include  "GnbPcieConfig.h"
#include  "GnbNbInitLibV1.h"
#include  "GnbNbInitLibV4.h"
#include  "GnbNbInitLibV5.h"
#include  "GnbRegisterAccKV.h"
#include  "GnbRegistersKV.h"
#include  "GnbFuseTableKV.h"
#include  "PcieComplexDataKV.h"
#include  "PcieLibKV.h"
#include  "OptionGnb.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITKV_GNBENVINITKV_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
extern GNB_BUILD_OPTIONS_KV   GnbBuildOptionsKV;
extern GNB_TABLE ROMDATA      GnbEnvInitTableKV [];
extern GNB_TABLE ROMDATA      GnbIommuInitTableKV [];
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
 * Registers need to be set if no GFX PCIe ports are used
 *
 *
 *
 * @param[in]  Pcie                Pointer to PCIe_PLATFORM_CONFIG
 */

BOOLEAN
STATIC
GnbIommuEnvInitCheckGfxPciePorts (
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIe_WRAPPER_CONFIG   *WrapperList;
  BOOLEAN               GfxPciePortUsed;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbIommuMidInitCheckGfxPciePorts Enter\n");
  GfxPciePortUsed = FALSE;

  WrapperList = PcieConfigGetChildWrapper (Pcie);
  ASSERT (WrapperList != NULL);
  if (WrapperList->WrapId == GFX_WRAP_ID) {
    PCIe_ENGINE_CONFIG   *EngineList;
    EngineList = PcieConfigGetChildEngine (WrapperList);
    while (EngineList != NULL) {
      if (PcieConfigIsPcieEngine (EngineList)) {
        IDS_HDT_CONSOLE (GNB_TRACE, "Checking Gfx ports device number %x\n", EngineList->Type.Port.NativeDevNumber);
        if (PcieConfigCheckPortStatus (EngineList, INIT_STATUS_PCIE_TRAINING_SUCCESS) ||
           ((EngineList->Type.Port.PortData.LinkHotplug != HotplugDisabled) && (EngineList->Type.Port.PortData.LinkHotplug != HotplugInboard))) {
          // GFX PCIe ports are being used
          GfxPciePortUsed = TRUE;
          IDS_HDT_CONSOLE (GNB_TRACE, "GFX PCIe ports are being used\n");
          break;
        }
      }
      EngineList = PcieLibGetNextDescriptor (EngineList);
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbIommuMidInitCheckGfxPciePorts Exit\n");
  return GfxPciePortUsed;
}

/*----------------------------------------------------------------------------------------*/
/**
 * GNB init at env
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
GnbEnvInterfaceKV (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS          Status;
  AGESA_STATUS          AgesaStatus;
  AMD_ENV_PARAMS        *EnvParamsPtr;
  UINT32                Property;
  GNB_HANDLE            *GnbHandle;
  PCI_ADDR              GnbPciAddress;
  D18F5x170_STRUCT      D18F5x170;
  D0F0x7C_STRUCT        D0F0x7C;
  D18F5xE8_STRUCT       D18F5xE8;
  UINT32                SocketTDP;
  PCIe_PLATFORM_CONFIG  *Pcie;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbEnvInterfaceKV Enter\n");
  Property = TABLE_PROPERTY_DEFAULT;
  AgesaStatus = AGESA_SUCCESS;
  EnvParamsPtr = (AMD_ENV_PARAMS *) StdHeader;
  GnbLoadFuseTableKV (StdHeader);
  Property |= GnbBuildOptionsKV.GnbCommonOptions.CfgIommuL1ClockGatingEnable ? TABLE_PROPERTY_IOMMU_L1_CLOCK_GATING : 0;
  Property |= GnbBuildOptionsKV.GnbCommonOptions.CfgIommuL2ClockGatingEnable ? TABLE_PROPERTY_IOMMU_L2_CLOCK_GATING : 0;
  Property |= GnbBuildOptionsKV.CfgIgfxIommuv1Dis ? TABLE_PROPERTY_IOMMU_L2_DEBUG_3_31 : 0;
  if (!EnvParamsPtr->GnbEnvConfiguration.IommuSupport) {
    Property |= TABLE_PROPERTY_IOMMU_DISABLED;
  }

  GnbHandle = GnbGetHandle (StdHeader);
  GnbRegisterReadKV (GnbHandle, TYPE_D18F4, D18F4x1B8_ADDRESS, &SocketTDP, 0, StdHeader);
  SocketTDP &= 0xFFFF;
  GnbRegisterReadKV (GnbHandle, TYPE_D18F5, D18F5xE8_ADDRESS, &D18F5xE8.Value, 0, StdHeader);
  SocketTDP *= D18F5xE8.Field.Tdp2Watt;
  // SocketTDP units are watts with 10 decimal bits
  // Convert to 10's of mW
  SocketTDP = (SocketTDP * 100) >> 10;
  IDS_HDT_CONSOLE (GNB_TRACE, " SocketTDP = %d\n", SocketTDP);
  if ((SocketTDP < 2000) && (SocketTDP > 1600)) {
    GnbOverrideFuseTableKV (StdHeader);
  }

  GnbLibPciRead (MAKE_SBDFO (0, 0, 0, 0, D0F0x7C_ADDRESS), AccessWidth32, &D0F0x7C.Value, StdHeader);
  Property |= (D0F0x7C.Field.ForceIntGfxDisable != 0) ? TABLE_PROPERTY_IGFX_DISABLED : 0;

  Status = PcieLocateConfigurationData (StdHeader, &Pcie);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);
  if (Status == AGESA_SUCCESS) {
    Property |= GnbIommuEnvInitCheckGfxPciePorts (Pcie) ? 0 : TABLE_PROPERTY_DGFX_DISABLED;
  }

  GnbPciAddress = GnbGetHostPciAddress (GnbHandle);
  Status = GnbSetTomV5 (GnbPciAddress, StdHeader);
  GnbOrbDynamicWakeV5 (GnbPciAddress, StdHeader);
  GnbClumpUnitIdKV (GnbHandle, StdHeader);
  if (GnbIsGnbConnectedToSb (GnbHandle)) {
    GnbLpcDmaDeadlockPreventionV4 (GnbHandle, StdHeader);
  }
  if (GnbBuildOptionsKV.CfgNbdpmEnable) {
    Property &= ~TABLE_PROPERTY_NBDPM;
    GnbRegisterReadKV (
      GnbHandle,
      TYPE_D18F5,
      D18F5x170_ADDRESS,
      &D18F5x170.Value,
      0,
      StdHeader
    );
    // Check if NbPstate enable
    if ((D18F5x170.Field.SwNbPstateLoDis != 1) && (D18F5x170.Field.NbPstateMaxVal != 0)) {
      Property |= TABLE_PROPERTY_NBDPM;
    }
  }
  IDS_OPTION_HOOK (IDS_GNB_PROPERTY, &Property, StdHeader);

  Status = GnbProcessTable (
             GnbHandle,
             GnbEnvInitTableKV,
             Property,
             GNB_TABLE_FLAGS_FORCE_S3_SAVE,
             StdHeader
             );
  AGESA_STATUS_UPDATE (Status, AgesaStatus);
  Status = GnbProcessTable (
             GnbHandle,
             GnbIommuInitTableKV,
             Property,
             GNB_TABLE_FLAGS_FORCE_S3_SAVE,
             StdHeader
             );
  AGESA_STATUS_UPDATE (Status, AgesaStatus);
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbEnvInterfaceKV Exit [0x%x]\n", AgesaStatus);
  return  AgesaStatus;
}
