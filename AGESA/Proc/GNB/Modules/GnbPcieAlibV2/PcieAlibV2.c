/* $NoKeywords:$ */
/**
 * @file
 *
 * PCIe ALIB
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
#include  "heapManager.h"
#include  "cpuLateInit.h"
#include  "cpuRegisters.h"
#include  "Gnb.h"
#include  "GnbFamServices.h"
#include  "GnbPcie.h"
#include  "GnbPcieFamServices.h"
#include  "GnbCommonLib.h"
#include  "GnbPcieConfig.h"
#include  "GnbPcieInitLibV1.h"
#include  "GnbNbInitLibV1.h"
#include  "OptionGnb.h"
#include  "PcieAlibV2.h"
#include  "GnbFuseTable.h"
#include  "GnbLibFeatures.h"
#include  "Filecode.h"

#define FILECODE PROC_GNB_MODULES_GNBPCIEALIBV2_PCIEALIBV2_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

extern  OPTION_GNB_CONFIGURATION_V2 AlibDispatchTableV2 [];
extern  GNB_BUILD_OPTIONS ROMDATA GnbBuildOptions;


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */


AGESA_STATUS
PcieAlibUpdateGnbData (
  IN OUT   VOID                  *SsdtBuffer,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  );

VOID
STATIC
PcieAlibUpdatePciePortDataCallback (
  IN       PCIe_ENGINE_CONFIG     *Engine,
  IN OUT   VOID                   *Buffer,
  IN       PCIe_PLATFORM_CONFIG   *Pcie
  );


AGESA_STATUS
PcieAlibBuildAcpiTableV2 (
  IN       AMD_CONFIG_PARAMS      *StdHeader,
     OUT   VOID                   **AlibSsdtPtr
  );


/*----------------------------------------------------------------------------------------*/
/**
 * Create ACPI ALIB SSDT table
 *
 *
 *
 * @param[in] StdHeader           Standard configuration header
 * @retval    AGESA_STATUS
 */

AGESA_STATUS
PcieAlibV2Feature (
  IN       AMD_CONFIG_PARAMS    *StdHeader
  )
{
  AMD_LATE_PARAMS *LateParamsPtr;
  LateParamsPtr = (AMD_LATE_PARAMS*) StdHeader;
  return PcieAlibBuildAcpiTableV2 (StdHeader, &LateParamsPtr->AcpiAlib);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Build ALIB ACPI table
 *
 *
 *
 * @param[in]     StdHeader       Standard Configuration Header
 * @param[in,out] AlibSsdtPtr     Pointer to pointer to ALIB SSDT table
 * @retval        AGESA_SUCCESS
 * @retval        AGESA_ERROR
 */

AGESA_STATUS
PcieAlibBuildAcpiTableV2 (
  IN       AMD_CONFIG_PARAMS    *StdHeader,
     OUT   VOID                 **AlibSsdtPtr
  )
{
  AGESA_STATUS            Status;
  AGESA_STATUS            AgesaStatus;
  VOID                    *AlibSsdtBuffer;
  VOID                    *AlibSsdtTable;
  UINTN                   AlibSsdtlength;
  UINT32                  AmlObjName;
  VOID                    *AmlObjPtr;


  IDS_HDT_CONSOLE (GNB_TRACE, "PcieAlibBuildAcpiTableV2 Enter\n");
  AgesaStatus = AGESA_SUCCESS;
  AlibSsdtTable = GnbFmAlibGetBaseTable (StdHeader);
  AlibSsdtlength = ((ACPI_TABLE_HEADER*) AlibSsdtTable)->TableLength;
  if (*AlibSsdtPtr == NULL) {
    AlibSsdtBuffer = GnbAllocateHeapBuffer (
                       AMD_ACPI_ALIB_BUFFER_HANDLE,
                       AlibSsdtlength,
                       StdHeader
                       );
    ASSERT (AlibSsdtBuffer != NULL);
    if (AlibSsdtBuffer == NULL) {
      return  AGESA_ERROR;
    }
    *AlibSsdtPtr = AlibSsdtBuffer;
  } else {
    AlibSsdtBuffer = *AlibSsdtPtr;
  }
  // Check length of port data
  ASSERT (sizeof (_ALIB_PORT_DATA) <= 20);
  // Check length of global data
  ASSERT (sizeof (_ALIB_GLOBAL_DATA) <= 32);
  // Copy template to buffer
  LibAmdMemCopy (AlibSsdtBuffer, AlibSsdtTable, AlibSsdtlength, StdHeader);
  // Update table OEM fields.
  LibAmdMemCopy (
    (VOID *) &((ACPI_TABLE_HEADER*) AlibSsdtBuffer)->OemId,
    (VOID *) &GnbBuildOptions.OemIdString,
    sizeof (GnbBuildOptions.OemIdString),
    StdHeader);
  LibAmdMemCopy (
    (VOID *) &((ACPI_TABLE_HEADER*) AlibSsdtBuffer)->OemTableId,
    (VOID *) &GnbBuildOptions.OemTableIdString,
    sizeof (GnbBuildOptions.OemTableIdString),
    StdHeader);
  //
  // Update register base base
  //
  PcieAlibUpdateGnbData (AlibSsdtBuffer, StdHeader);
  //
  // Update transfer block
  //
  AmlObjName = STRING_TO_UINT32 ('A', 'D', 'A', 'T');
  AmlObjPtr = GnbLibFind (AlibSsdtBuffer, AlibSsdtlength, (UINT8*) &AmlObjName, sizeof (AmlObjName));
  if (AmlObjPtr != NULL) {
    AmlObjPtr = (UINT8 *) AmlObjPtr + 10;
  }
  // Dispatch function from table
  Status = GnbLibDispatchFeaturesV2 (&AlibDispatchTableV2[0], AmlObjPtr, StdHeader);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);
  if (AgesaStatus != AGESA_SUCCESS) {
    //Shrink table length to size of the header
    ((ACPI_TABLE_HEADER*) AlibSsdtBuffer)->TableLength = sizeof (ACPI_TABLE_HEADER);
  }
  ChecksumAcpiTable ((ACPI_TABLE_HEADER*) AlibSsdtBuffer, StdHeader);
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieAlibBuildAcpiTableV2 Exit [0x%x]\n", AgesaStatus);
  return AgesaStatus;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Update MMIO info
 *
 *
 *
 *
 * @param[in] SsdtBuffer          Pointer to SSDT table
 * @param[in] StdHeader           Standard configuration header
 */

AGESA_STATUS
PcieAlibUpdateGnbData (
  IN OUT   VOID                  *SsdtBuffer,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  UINT64        LocalMsrRegister;
  UINT32        AmlObjName;
  VOID          *AmlObjPtr;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieAlibUpdateGnbData Enter\n");
  //
  //  Locate Base address variable
  //
  AmlObjName = STRING_TO_UINT32 ('A', 'G', 'R', 'B');
  AmlObjPtr = GnbLibFind (
                SsdtBuffer,
                ((ACPI_TABLE_HEADER*) SsdtBuffer)->TableLength,
                (UINT8*) &AmlObjName,
                sizeof (AmlObjName)
                );
  // ASSERT (AmlObjPtr != NULL);
  if (AmlObjPtr == NULL) {
    return AGESA_ERROR;
  }
  //
  //  Update PCIe MMIO base
  //
  LibAmdMsrRead (MSR_MMIO_Cfg_Base, &LocalMsrRegister, StdHeader);
  if ((LocalMsrRegister & BIT0) != 0 && (LocalMsrRegister & 0xFFFFFFFF00000000) == 0) {
    *(UINT32*)((UINT8*) AmlObjPtr + 5) = (UINT32) (LocalMsrRegister & 0xFFFFF00000);
  } else {
    ASSERT (FALSE);
    return AGESA_ERROR;
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieAlibUpdateGnbData Exit\n");
  return AGESA_SUCCESS;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Update MMIO info
 *
 *
 *
 *
 * @param[in] DataBuffer          Pointer to data buffer
 * @param[in] StdHeader           Standard configuration header
 */

AGESA_STATUS
PcieAlibUpdateVoltageData (
  IN OUT   VOID                  *DataBuffer,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  PP_FUSE_ARRAY_V2   *PpFuseArray;
  AGESA_STATUS    Status;
  ALIB_DATA       *AlibData;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieAlibUpdateVoltageData Enter\n");
  Status = AGESA_SUCCESS;
  ASSERT (DataBuffer != NULL);
  AlibData = (ALIB_DATA *) DataBuffer;
  //
  //  Locate Fuse table
  //
  PpFuseArray = GnbLocateHeapBuffer (AMD_PP_FUSE_TABLE_HANDLE, StdHeader);
  ASSERT (PpFuseArray != NULL);
  if (PpFuseArray != NULL) {
    //
    //  Update GEN1 Vid
    //
    AlibData->Data.Data.PcieVidGen1 = PpFuseArray->VddNbVid[0];
    //
    //  Update GEN2 Vid
    //
    AlibData->Data.Data.PcieVidGen2 = PpFuseArray->VddNbVid[PpFuseArray->PcieGen2Vid];
    //
    //  Update GEN3 Vid
    //
    AlibData->Data.Data.PcieVidGen3 = PpFuseArray->VddNbVid[PpFuseArray->PcieGen3Vid];
    //
    //  Update DPM Mask
    //
    AlibData->Data.Data.DpmMask = (PpFuseArray->PerfFlag + 1);
    IDS_HDT_CONSOLE (GNB_TRACE, "  DpmMask = %02x\n", AlibData->Data.Data.DpmMask);
    //
    //  Update Boost data
    //
    AlibData->Data.Data.NumBoostStates = (PpFuseArray->NumBoostStates);
    IDS_HDT_CONSOLE (GNB_TRACE, "  NumBoost = %02x\n", AlibData->Data.Data.NumBoostStates);
  } else {
    Status = AGESA_ERROR;
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieAlibUpdateVoltageData Exit\n");
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Update PCIe info
 *
 *
 *
 *
 * @param[in] DataBuffer          Ponter to data buffer
 * @param[in] StdHeader           Standard configuration header
 */

AGESA_STATUS
PcieAlibUpdatePcieData (
  IN OUT   VOID                  *DataBuffer,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  PCIe_PLATFORM_CONFIG       *Pcie;
  AMD_LATE_PARAMS            *LateParamsPtr;
  AGESA_STATUS               Status;
  BOOLEAN                    DcTdpEnable;
  GNB_BUILD_OPTIONS_COMMON   *GnbCommonOptions;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieAlibUpdatePcieData Enter\n");
  ASSERT (DataBuffer != NULL);
  Status = AGESA_SUCCESS;

  GnbCommonOptions = GnbLocateHeapBuffer (AMD_GNB_BUILD_OPTIONS_HANDLE, StdHeader);
  if (GnbCommonOptions != NULL) {
    DcTdpEnable = GnbCommonOptions->CfgDcTdpEnable;
  } else {
    GnbCommonOptions = (GNB_BUILD_OPTIONS_COMMON*) GnbFmGnbBuildOptions (StdHeader);
    DcTdpEnable = GnbCommonOptions->CfgDcTdpEnable;
  }
  //
  //  Locate PCIe platform config
  //
  if (PcieLocateConfigurationData (StdHeader, &Pcie) == AGESA_SUCCESS) {
    //
    // Update policy data
    //
    ((ALIB_DATA *) DataBuffer)->Data.Data.PsppPolicy = Pcie->PsppPolicy;
    //
    // Update data for each port
    //
    PcieConfigRunProcForAllEngines (
      DESCRIPTOR_PCIE_ENGINE,
      PcieAlibUpdatePciePortDataCallback,
      DataBuffer,
      Pcie
      );
  } else {
    ASSERT (FALSE);
    Status = AGESA_FATAL;
  }

  LateParamsPtr = (AMD_LATE_PARAMS *) StdHeader;
  ((ALIB_DATA *) DataBuffer)->Data.Data.DockedTdpHeadroom =
      LateParamsPtr->GnbLateConfiguration.DockedTdpHeadroom;
  IDS_HDT_CONSOLE (GNB_TRACE, "  DockedTdpHeadroom = %02x\n",
      LateParamsPtr->GnbLateConfiguration.DockedTdpHeadroom);

  IDS_HDT_CONSOLE (GNB_TRACE, "  DcTdpEnable = %x\n", DcTdpEnable);
  ((ALIB_DATA *) DataBuffer)->Data.Data.DcTdpEnable = DcTdpEnable;
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieAlibUpdatePcieData Exit\n");
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback to update PCIe port data
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
PcieAlibUpdatePciePortDataCallback (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  _ALIB_PORT_DATA *PortData;

  PortData = &((ALIB_DATA *) Buffer)->PortData[Engine->Type.Port.PcieBridgeId].PortData;

  if (PcieConfigIsEngineAllocated (Engine) && (Engine->Type.Port.PortData.LinkHotplug != HotplugDisabled || PcieConfigCheckPortStatus (Engine, INIT_STATUS_PCIE_TRAINING_SUCCESS))) {
    //
    // Various speed capability
    //
    PortData->PciePortMaxSpeed = (UINT8) PcieFmGetLinkSpeedCap (PCIE_PORT_GEN_CAP_MAX, Engine);
    PortData->PciePortCurSpeed = (UINT8) PcieFmGetLinkSpeedCap (PCIE_PORT_GEN_CAP_BOOT, Engine);
    PortData->PciePortDcSpeed = PcieGen1;
    PortData->PciePortAcSpeed = PortData->PciePortMaxSpeed;
    if (Pcie->PsppPolicy == PsppBalanceLow) {
      PortData->PciePortAcSpeed = PcieGen1;
    }
    if (PcieConfigIsSbPcieEngine (Engine)) {
      PortData->PcieSbPort = 0x1;
      PortData->PciePortAcSpeed = PortData->PciePortMaxSpeed;
    }
    if (Engine->Type.Port.PortData.MiscControls.LinkSafeMode != 0) {
      PortData->PcieLinkSafeMode = 0x1;
      PortData->PcieLocalOverrideSpeed = Engine->Type.Port.PortData.MiscControls.LinkSafeMode;
    }
    //
    // various port capability
    //
    PortData->StartPhyLane = (UINT8) Engine->EngineData.StartLane;
    PortData->EndPhyLane = (UINT8) Engine->EngineData.EndLane;
    PortData->StartCoreLane = (UINT8) Engine->Type.Port.StartCoreLane;
    PortData->EndCoreLane = (UINT8) Engine->Type.Port.EndCoreLane;
    PortData->PortId = Engine->Type.Port.PortId;
    PortData->LinkHotplug = Engine->Type.Port.PortData.LinkHotplug;
    PortData->PciDev = (UINT8) Engine->Type.Port.Address.Address.Device;
    PortData->PciFun = (UINT8) Engine->Type.Port.Address.Address.Function;
  } else {
    PortData->PciePortMaxSpeed = PcieGen1;
    PortData->PciePortCurSpeed = PcieGen1;
    PortData->PciePortDcSpeed  = PcieGen1;
    PortData->PciePortAcSpeed  = PcieGen1;
    PortData->PcieLocalOverrideSpeed = PcieGen1;
  }
}

