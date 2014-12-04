/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM OEM API, and related functions.
 *
 * Contains OEM code that defined CPM tables before CPM init.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  OEM
 * @e \$Revision: 281158 $   @e \$Date: 2013-12-17 20:36:04 -0600 (Tue, 17 Dec 2013) $
 *
 */
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

#include <AmdCpmPei.h>
#include "AmdCpmOemInitPeim.h"
#include "GnbPcieTopologyTable.h"


extern  AMD_CPM_PLATFORM_ID_TABLE             gCpmPlatformIdTable;
extern  AMD_CPM_PLATFORM_ID_CONVERT_TABLE     gCpmPlatformIdConvertTable;
extern  AMD_CPM_PRE_INIT_TABLE                gCpmPreInitTable;
extern  AMD_CPM_SAVE_CONTEXT_TABLE            gCpmSaveContextTable;
extern  AMD_CPM_GPIO_INIT_TABLE               gCpmGpioPreInitTable;
extern  AMD_CPM_GPIO_INIT_TABLE               gCpmGpioInitTable;
extern  AMD_CPM_GEVENT_INIT_TABLE             gCpmGeventInitTable;
extern  AMD_CPM_GPIO_MEM_VOLTAGE_TABLE        gCpmSetMemVoltage;
extern  AMD_CPM_GPIO_DEVICE_CONFIG_TABLE      gCpmGpioDeviceConfigTable;
extern  AMD_CPM_GPIO_DEVICE_DETECTION_TABLE   gCpmGpioDeviceDetectionTable;
extern  AMD_CPM_GPIO_DEVICE_RESET_TABLE       gCpmGpioDeviceResetTable;
extern  AMD_CPM_GPIO_DEVICE_POWER_TABLE       gCpmGpioDevicePowerTable;
extern  AMD_CPM_PCIE_CLOCK_TABLE              gCpmPcieClockTable;
extern  AMD_CPM_PCIE_CLOCK_TABLE              gCpmPcieClockTable2;
extern  AMD_CPM_EXT_CLKGEN_TABLE              gCpmExtClkGenTable;
extern  AMD_CPM_ACPI_THERMAL_FAN_TABLE        gCpmAcpiThermalFan;
extern  AMD_CPM_DEVICE_PATH_TABLE             gCpmDevicePathTable;
extern  AMD_CPM_SPECIFIC_SSID_TABLE           gCpmSpecificSsidTable;
extern  AMD_CPM_DISPLAY_FEATURE_TABLE         gCpmDisplayFeatureTable;
extern  AMD_CPM_ADAPTIVE_S4_TABLE             gCpmAdaptiveS4Table;

EFI_STATUS
EFIAPI
CpmTableOverride (
  IN       EFI_PEI_SERVICES       **PeiServices
  );

EFI_STATUS
EFIAPI
CpmOverrideTableNotifyCallback (
  IN       EFI_PEI_SERVICES             **PeiServices,
  IN       EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN       VOID                         *Ppi
  );

STATIC EFI_PEI_NOTIFY_DESCRIPTOR mCpmOemTableOverrideNotify = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmdCpmTablePpiGuid,
  CpmOverrideTableNotifyCallback
};

//
// CPM Main Table
//
AMD_CPM_MAIN_TABLE gCpmMainTable = {
  {CPM_SIGNATURE_MAIN_TABLE, sizeof (gCpmMainTable) / sizeof (UINT8), 0, 0, 0, 0x0000000F},
  "Bantry",                           // PlatformName
  0xFF,                               // BiosType
  0,                                  // CurrentPlatformId
  0,                                  // PcieMemIoBaseAddr
  0,                                  // AcpiMemIoBaseAddr
  NULL,                               // Reserved for Internal Used
  NULL,                               // Reserved for Internal Used
  NULL,                               // Reserved for Internal Used
  NULL,                               // Reserved for Internal Used
  NULL,                               // Reserved for Internal Used
  0x2400,                             // DisplayFeature: PowerXpressFixedMode
  0,                                  // ZeroPowerOdd
  0,                                  // AcpiThermalFan
  0,                                  // ExtClkGen
  0,                                  // UnusedGppClkOffEn
  0,                                  // AdaptiveS4En
  0,                                  // WirelessButtonEn
  0,                                  // Ec
  0,                                  // Reserved
  0,                                  // Reserved
  0,                                  // Reserved
  0                                   // Reserved
};

VOID *gCpmTableList[] = {
  &gCpmMainTable,

  &gCpmPlatformIdTable,
  &gCpmPlatformIdConvertTable,
  &gCpmPreInitTable,
  &gCpmSaveContextTable,

  &gCpmGpioPreInitTable,
  &gCpmGpioInitTable,
  &gCpmGeventInitTable,
  &gCpmGpioDeviceConfigTable,
  &gCpmGpioDevicePowerTable,
  &gCpmGpioDeviceDetectionTable,
  &gCpmGpioDeviceResetTable,
  &gCpmPcieClockTable,
  &gCpmPcieClockTable2,
  &gCpmSetMemVoltage,

  &gCpmPcieTopologyTable,
  &gCpmPcieTopologyTable2,
  &gCpmPcieTopologyOverride,

  &gCpmAcpiThermalFan,

  &gCpmDisplayFeatureTable,
  &gCpmDevicePathTable,
  &gCpmSpecificSsidTable,
  &gCpmAdaptiveS4Table,

  NULL
};

/*----------------------------------------------------------------------------------------*/
/**
 * Entry point of the AMD CPM OEM Init PEIM driver
 *
 * This function defines CPM OEM definition tables and installs AmdCpmOemTablePpi.
 * It also defines callback function to update these definition table on run time.
 *
 * @param[in]     FileHandle     Pointer to the firmware file system header
 * @param[in]     PeiServices    Pointer to Pei Services
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */
EFI_STATUS
EFIAPI
AmdCpmOemInitPeimEntryPoint (
  IN        CPM_PEI_FILE_HANDLE         FileHandle,
  IN        CPM_PEI_SERVICES            **PeiServices
  )
{
  EFI_STATUS              Status;
  AMD_CPM_OEM_TABLE_PPI   *AmdCpmOemTablePpi;
  EFI_PEI_PPI_DESCRIPTOR  *PpiListCpmOemTable;

  RECORD_TIME_PEI (BeginAmdCpmOemInitPeimDriver);

  Status = (*PeiServices)->AllocatePool (
                           PeiServices,
                           sizeof (AMD_CPM_OEM_TABLE_PPI),
                           &AmdCpmOemTablePpi
                           );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  AmdCpmOemTablePpi->Revision = AMD_CPM_OEM_REVISION;
  AmdCpmOemTablePpi->PlatformId = AMD_CPM_PLATFORM_ID_DEFAULT;
  AmdCpmOemTablePpi->TableList = &gCpmTableList[0];

  Status = (*PeiServices)->AllocatePool (
                             PeiServices,
                             sizeof (EFI_PEI_PPI_DESCRIPTOR),
                             &PpiListCpmOemTable
                             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  PpiListCpmOemTable->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  PpiListCpmOemTable->Guid = &gAmdCpmOemTablePpiGuid;
  PpiListCpmOemTable->Ppi = AmdCpmOemTablePpi;
  Status = (*PeiServices)->InstallPpi (
                              PeiServices,
                              PpiListCpmOemTable
                              );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = (**PeiServices).NotifyPpi (PeiServices, &mCpmOemTableOverrideNotify);

  RECORD_TIME_PEI (EndAmdCpmOemInitPeimDriver);

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback Function to Override CPM OEM Definition Tables
 *
 * @param[in]     PeiServices       The PEI core services table.
 * @param[in]     NotifyDescriptor  The descriptor for the notification event.
 * @param[in]     Ppi               Pointer to the PPI in question
 *
 * @retval        EFI_SUCCESS    Module initialized successfully
 * @retval        EFI_ERROR      Initialization failed (see error for more details)
 */
EFI_STATUS
EFIAPI
CpmOverrideTableNotifyCallback (
  IN       EFI_PEI_SERVICES             **PeiServices,
  IN       EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN       VOID                         *Ppi
  )
{
  EFI_STATUS  Status;
  RECORD_TIME_PEI (BeginAmdCpmOemTableOverride);
  Status = CpmTableOverride (PeiServices);
  RECORD_TIME_PEI (EndAmdCpmOemTableOverride);
  return Status;
}
