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

#include "FchReset.h"
#include "FchPlatform.h"
#define FILECODE UEFI_PEI_FCHPEI_FCHRESET_FILECODE

// Consumed PPIs
#include EFI_PPI_CONSUMER (CpuIo)
#if defined (PI_SPECIFICATION_VERSION) && (PI_SPECIFICATION_VERSION >= 0x00010000)
  #include EFI_PPI_CONSUMER (PciCfg2)
#else
  #include EFI_PPI_CONSUMER (PciCfg)
#endif


/*********************************************************************************
 * Name: FchPeiReset
 *
 * Description
 *   Reset the system using the chipset cf9 port
 *
 * Input
 *   PeiServices : a pointer to the PEI service table pointer
 *
 * Output
 *   none if the reset is successful
 *
 *********************************************************************************/
EFI_STATUS
FchPeiReset (
  IN       EFI_PEI_SERVICES  **PeiServices
  )
{
#if defined (PI_SPECIFICATION_VERSION) && (PI_SPECIFICATION_VERSION >= 0x00010000)
  EFI_PEI_CPU_IO_PPI           *CpuIo;
  EFI_PEI_PCI_CFG2_PPI         *PciCfg;
#else
  PEI_CPU_IO_PPI               *CpuIo;
  PEI_PCI_CFG_PPI              *PciCfg;
#endif
  UINT32                       Pmcsr;
  EFI_BOOT_MODE                BootMode;

  // Get PCI CFG PPI from PEI service pointer
  PciCfg = (*PeiServices)->PciCfg;

  // Get the platform boot mode
  (*PeiServices)->GetBootMode (
                    PeiServices,
                    &BootMode
                    );

  PciCfg->Read (
            PeiServices,
            PciCfg,
            PeiPciCfgWidthUint32,
            (UINT64) ((FCH_XHCI_DEV << 16) | (FCH_XHCI_FUNC << 8) | FCH_XHCI_REG00),
            &Pmcsr
            );

  if ( ((Pmcsr == 0x78121022) || (Pmcsr == 0x78141022)) && (BootMode == BOOT_ON_S3_RESUME || BootMode == BOOT_ON_S4_RESUME) ) {
    PciCfg->Read (
              PeiServices,
              PciCfg,
              PeiPciCfgWidthUint32,
              (UINT64) ((FCH_XHCI_DEV << 16) | (FCH_XHCI_FUNC << 8) + FCH_XHCI_REG54),
              &Pmcsr
              );
    Pmcsr |= 0x3;
    PciCfg->Write (
              PeiServices,
              PciCfg,
              PeiPciCfgWidthUint32,
              (UINT64) ((FCH_XHCI_DEV << 16) | (FCH_XHCI_FUNC << 8) + FCH_XHCI_REG54),
              &Pmcsr
              );

    PciCfg->Read (
              PeiServices,
              PciCfg,
              PeiPciCfgWidthUint32,
              (UINT64) ((FCH_XHCI1_DEV << 16) | (FCH_XHCI1_FUNC << 8) + FCH_XHCI_REG54),
              &Pmcsr
              );
    Pmcsr |= 0x3;
    PciCfg->Write (
              PeiServices,
              PciCfg,
              PeiPciCfgWidthUint32,
              (UINT64) ((FCH_XHCI1_DEV << 16) | (FCH_XHCI1_FUNC << 8) + FCH_XHCI_REG54),
              &Pmcsr
              );
  }

  // Issue the reset
  CpuIo = (*PeiServices)->CpuIo;
  CpuIo->IoWrite8 (
            PeiServices,
            CpuIo,
            FCH_RESET_PORT,
            (COLD_RESET + RESET_CPU)
            );

  // Reset unsuccessful
  EFI_DEADLOOP ();
  return (EFI_DEVICE_ERROR);
}
