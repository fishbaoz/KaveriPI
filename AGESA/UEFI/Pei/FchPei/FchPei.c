/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH PEIM
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH PEIM
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*****************************************************************************
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
 ******************************************************************************
 */
#include "Tiano.h"
#include <Pei.h>

#include "FchPei.h"
#include "FchReset.h"
#include "FchStall.h"
#include "FchPeiLib.h"
#define FILECODE UEFI_PEI_FCHPEI_FCHPEI_FILECODE

// Consumed PPIs
#include EFI_PPI_CONSUMER (CpuIo)
// Produced PPIs
#include EFI_PPI_PRODUCER (Reset)
#include EFI_PPI_PRODUCER (Stall)
#include EFI_PPI_PRODUCER (AmdFchInitPpi)


//
// Module globals
//
STATIC PEI_RESET_PPI mResetPpi = {
  FchPeiReset
};

STATIC EFI_PEI_STALL_PPI mStallPpi = {
  FCH_STALL_RESOLUTION_USEC,
  FchPeiStall
};

STATIC EFI_PEI_PPI_DESCRIPTOR mPpiListReset = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiResetPpiGuid,
  &mResetPpi
};

STATIC EFI_PEI_PPI_DESCRIPTOR mPpiListStall = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiStallPpiGuid,
  &mStallPpi
};


EFI_PEIM_ENTRY_POINT (FchPeiInit);

/*********************************************************************************
 * Name: FchPeiInit
 *
 * Description:
 *               > Update boot mode
 *               > Install Reset PPI
 *               > Install SMBUS PPI
 *               > Install Stall PPI
 *
 * Input
 *   FfsHeader   : pointer to the firmware file system header
 *   PeiServices : pointer to the PEI service table
 *
 * Output
 *   EFI_SUCCESS : Module initialized successfully
 *   EFI_ERROR   : Initialization failed (see error for more details)
 *
 *********************************************************************************/
EFI_STATUS
EFIAPI
FchPeiInit (
  IN       EFI_FFS_FILE_HEADER  *FfsHeader,
  IN       EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                Status;
  PEI_RESET_PPI             *PeiReset;
  EFI_PEI_STALL_PPI         *PeiStall;

  FCH_PEI_PRIVATE           *FchPrivate;
  EFI_PEI_PPI_DESCRIPTOR    *PpiListFchInit;


  Status = (*PeiServices)->AllocatePool (
                             PeiServices,
                             sizeof (FCH_PEI_PRIVATE),
                             &FchPrivate
                             );
  ASSERT_PEI_ERROR (PeiServices, Status);

  FchPrivate->Signature                    = FCH_PEI_PRIVATE_DATA_SIGNATURE;
  FchPrivate->StdHdr.AltImageBasePtr       = (UINT32) PeiServices;
  FchPrivate->FchInitPpi.Revision          = AMD_FCH_INIT_PPI_REV;
  FchPrivate->FchInitPpi.FpFchXhciRecovery = FchInitXhciOnRecovery;
  FchPrivate->FchInitPpi.FpFchEhciRecovery = FchInitEhciOnRecovery;
  FchPrivate->FchInitPpi.FpFchSataRecovery = FchInitSataOnRecovery;
  FchPrivate->FchInitPpi.FpFchGppRecovery  = FchInitGppOnRecovery;

  //
  // Update the boot mode
  //
  Status = FchUpdateBootMode (PeiServices);
  ASSERT_PEI_ERROR (PeiServices, Status);


  //
  // publish other PPIs
  //
  // Reset PPI
  // check to see if an instance is already installed
  Status = (*PeiServices)->LocatePpi (
                             PeiServices,
                             &gPeiResetPpiGuid,
                             0,
                             NULL,
                             &PeiReset
                             );

  if (EFI_NOT_FOUND == Status) {
    // No instance currently installed, install our own
    Status = (*PeiServices)->InstallPpi (
                               PeiServices,
                               &mPpiListReset
                               );

    ASSERT_PEI_ERROR (PeiServices, Status);
  }

  // Stall PPI
  // check to see if an instance is already installed
  Status = (*PeiServices)->LocatePpi (
                             PeiServices,
                             &gPeiStallPpiGuid,
                             0,
                             NULL,
                             &PeiStall
                             );

  if (EFI_NOT_FOUND == Status) {
    // There is no instance currently installed, install our own
    Status = (*PeiServices)->InstallPpi (
                               PeiServices,
                               &mPpiListStall
                               );

    ASSERT_PEI_ERROR (PeiServices, Status);
  }

  // Allocate memory for the PPI descriptor
  Status = (*PeiServices)->AllocatePool (
                             PeiServices,
                             sizeof (EFI_PEI_PPI_DESCRIPTOR),
                             &PpiListFchInit
                             );
  ASSERT_PEI_ERROR (PeiServices, Status);

  PpiListFchInit->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  PpiListFchInit->Guid  = &gAmdFchInitPpiGuid;
  PpiListFchInit->Ppi   = &FchPrivate->FchInitPpi;

  Status = (*PeiServices)->InstallPpi (
                             PeiServices,
                             PpiListFchInit
                             );
  return Status;
}



/*********************************************************************************
 * Name: FchGetBootMode
 *
 * Description:
 *   This function determines whether the platform is resuming from an S state
 *   using the FCH ACPI registers
 *
 * Input:
 *   PeiServices : a pointer to the PEI service table pointer
 *   pFchPrivate  : pointer to the FCH PEI private data structure
 *
 * Output:
 *   EFI_BOOT_MODE : Boot mode from SB
 *
 *********************************************************************************/
EFI_BOOT_MODE
FchGetBootMode (
  IN       EFI_PEI_SERVICES    **PeiServices
  )
{
  PEI_CPU_IO_PPI      *CpuIo;
  UINT16              FchBootMode;
  UINTN               AcpiPm1Ctl;

  //
  // find the CpuIo protocol
  //
  CpuIo = (*PeiServices)->CpuIo;

  //
  // Check the FCH WAK_STS bit in the ACPI_PM1_CTL register
  //
  // get the address PM1_CTL register address
  AcpiPm1Ctl  = LibFchPmIoRead16 (PeiServices, FCH_PMIOA_REG62);

  // get the boot mode as seen by the south bridge
  FchBootMode = (CpuIo->IoRead16 (PeiServices, CpuIo, AcpiPm1Ctl) & ACPI_BM_MASK);

  // convert the boot mode to the EFI version
  if (ACPI_S3 == FchBootMode) {
    return (BOOT_ON_S3_RESUME);
  }
  if (ACPI_S4 == FchBootMode) {
    return (BOOT_ON_S4_RESUME);
  }
  if (ACPI_S5 == FchBootMode) {
    return (BOOT_ON_S5_RESUME);
  }
  // S0 or unsupported Sx mode
  return (BOOT_WITH_FULL_CONFIGURATION);
}


/*********************************************************************************
 * Name: FchUpdateBootMode
 *
 * Description:
 *   This function update the platform boot mode based on the information
 *   gathered from the south bridge.
 *   Note that we do not publish the BOOT_MODE PPI since the platform
 *   is responsible for deciding what the actual boot mode is.
 *
 * Input:
 *   PeiServices : a pointer to the PEI service table pointer
 *   pFchPrivate  : pointer to the FCH PEI private data structure
 *
 * Output:
 *
 *********************************************************************************/
EFI_STATUS
FchUpdateBootMode (
  IN       EFI_PEI_SERVICES    **PeiServices
  )
{
  EFI_BOOT_MODE  BootMode;
  EFI_BOOT_MODE  FchBootMode;

  // Get FCH Boot mode
  FchBootMode = FchGetBootMode (PeiServices);

  // Get the platform boot mode
  (*PeiServices)->GetBootMode (
                    PeiServices,
                    &BootMode
                    );

  // Update boot mode if we are more important than the platform
  if ((BOOT_IN_RECOVERY_MODE != BootMode) && (BOOT_ON_FLASH_UPDATE != BootMode) && (BOOT_WITH_FULL_CONFIGURATION != FchBootMode)) {
    // Set Sx boot mode
    (*PeiServices)->SetBootMode (
                      PeiServices,
                      FchBootMode
                      );
  }
  return (EFI_SUCCESS);
}


EFI_STATUS
EFIAPI
FchInitXhciOnRecovery (
  IN       AMD_FCH_INIT_PPI   *This,
  IN       UINT32             XhciRomAddress
  )
{
  FCH_PEI_PRIVATE           *FchPrivate;

  FchPrivate = FCH_PEI_PRIVATE_FROM_THIS (This);
  FchXhciOnRecovery (*FchPrivate, XhciRomAddress);
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
FchInitEhciOnRecovery (
  IN       AMD_FCH_INIT_PPI   *This,
  IN       UINT32             EhciTemporaryBarAddress
  )
{
  FCH_PEI_PRIVATE           *FchPrivate;

  FchPrivate = FCH_PEI_PRIVATE_FROM_THIS (This);
  FchEhciOnRecovery (*FchPrivate, EhciTemporaryBarAddress);
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
FchInitSataOnRecovery (
  IN       AMD_FCH_INIT_PPI   *This,
  IN       UINT32             SataBar0,
  IN       UINT32             SataBar5
  )
{
  FCH_PEI_PRIVATE           *FchPrivate;

  FchPrivate = FCH_PEI_PRIVATE_FROM_THIS (This);
  FchSataOnRecovery (*FchPrivate, SataBar0, SataBar5 );
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
FchInitGppOnRecovery (
  IN       AMD_FCH_INIT_PPI   *This,
  IN       FCH_GPP_R          *FchGpp
  )
{
  FCH_PEI_PRIVATE           *FchPrivate;

  FchPrivate = FCH_PEI_PRIVATE_FROM_THIS (This);
  FchGppOnRecovery (*FchPrivate, FchGpp);
  return EFI_SUCCESS;
}
