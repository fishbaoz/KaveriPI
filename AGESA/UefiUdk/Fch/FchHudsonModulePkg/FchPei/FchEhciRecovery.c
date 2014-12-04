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

#include <Library\DebugLib.h>

#include "FchPei.h"
#include "FchEhciRecovery.h"
#include "FchPlatform.h"
#define FILECODE UEFI_PEI_FCHPEI_FCHEHCIRECOVERY_FILECODE


/**
 * EnableEhciController - Assign resources and enable EHCI controllers
 *
 * @param[in]   Value            Controller PCI config address (bus# + device# + function#)
 * @param[in]   BarAddress       Base address for Ehci controller
 * @param[in]   StdHeader        AMD standard header config param
 */
VOID
EnableEhciController (
  IN  UINT32              Value,
  IN  UINT32              BarAddress,
  IN  AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32  Var;
  WritePci ((UINT32) Value + FCH_EHCI_REG10, AccessWidth32, &BarAddress, StdHeader);
  RwPci ((UINT32) Value + FCH_EHCI_REG04, AccessWidth8, 0, BIT1 + BIT2, StdHeader);
  ReadPci ((UINT32) Value + FCH_EHCI_REG10, AccessWidth32, &BarAddress, StdHeader);
  if ( (BarAddress != - 1) && (BarAddress != 0) ) {
    RwPci ((UINT32) Value + FCH_EHCI_REG04, AccessWidth8, 0, BIT1, StdHeader);
    Var = 0x00020F00;
    WriteMem (BarAddress + FCH_EHCI_BAR_REGC0, AccessWidth32, &Var);
    RwMem (BarAddress + FCH_EHCI_BAR_REGA4, AccessWidth32, 0xFF00FF00, 0x00400040);
    RwMem (BarAddress + FCH_EHCI_BAR_REGBC, AccessWidth32, ~( BIT12 + BIT14), BIT12 + BIT14);
    RwMem (BarAddress + FCH_EHCI_BAR_REGB0, AccessWidth32, ~BIT5, BIT5);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGF0, AccessWidth16, ~BIT12, BIT12);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGF0, AccessWidth32, ~BIT16, BIT16);
    RwMem (BarAddress + FCH_EHCI_BAR_REGB4, AccessWidth32, ~BIT12, BIT12);
    RwMem (BarAddress + FCH_EHCI_BAR_REGC4, AccessWidth32, (UINT32) (~ 0x00000f00), 0x00000200);
    RwMem (BarAddress + FCH_EHCI_BAR_REGC0, AccessWidth32, (UINT32) (~ 0x0000ff00), 0x00000f00);
    RwPci ((UINT32) Value + FCH_EHCI_REG50, AccessWidth32, ~ ((UINT32) (0x01 << 6)), (UINT32) (0x01 << 6), StdHeader);
    RwPci ((UINT32) Value + FCH_EHCI_REG50, AccessWidth32, ~ ((UINT32) (0x0F << 8)), (UINT32) (0x01 << 8), StdHeader);
    RwPci ((UINT32) Value + FCH_EHCI_REG50, AccessWidth32, ~ ((UINT32) (0x0F << 12)), (UINT32) (0x01 << 12), StdHeader);
    RwPci ((UINT32) Value + FCH_EHCI_REG50, AccessWidth32, ~ ((UINT32) (0x01 << 17)), (UINT32) (0x01 << 17), StdHeader);
    RwPci ((UINT32) Value + FCH_EHCI_REG50, AccessWidth32, ~ ((UINT32) (0x01 << 21)), (UINT32) (0x01 << 21), StdHeader);
    RwPci ((UINT32) Value + FCH_EHCI_REG50, AccessWidth32, ~ ((UINT32) (0x01 << 29)), (UINT32) (0x01 << 29), StdHeader);
    RwPci ((UINT32) Value + FCH_EHCI_REG50 + 2, AccessWidth16, (UINT16)0xFFFF, BIT16 + BIT10, StdHeader);
    RwPci ((UINT32) Value + FCH_EHCI_REG54, AccessWidth32, ~ (BIT0 + (3 << 5) + (7 << 7)), 0x0000026b, StdHeader);
    RwPci ((UINT32) Value + FCH_EHCI_REG54, AccessWidth32, ~BIT4, BIT4, StdHeader);
    RwPci ((UINT32) Value + FCH_EHCI_REG54, AccessWidth32, ~BIT11, BIT11, StdHeader);
    RwPci ((UINT32) Value + FCH_EHCI_REG54, AccessWidth16, (UINT16)0x5FFF, BIT13 + BIT15, StdHeader);
    RwPci ((UINT32) Value + FCH_EHCI_REG50 + 2, AccessWidth16, ~BIT3, BIT3, StdHeader);
    RwPci ((UINT32) Value + FCH_EHCI_REG54 + 2, AccessWidth16, (UINT16)0xFFFC, BIT0 + BIT1, StdHeader);
    RwPci ((UINT32) Value + FCH_EHCI_REG54 + 2, AccessWidth16, (UINT16)0xFFFB, BIT2, StdHeader);
    RwPci ((UINT32) Value + FCH_EHCI_REG54 + 2, AccessWidth16, (UINT16)0xFFF7, BIT3, StdHeader);
  }
}
/*********************************************************************************
 * Name: FchEhciOnRecovery
 *
 * Description
 *
 *
 * Input
 *   FchPrivate  : FCH PEI private data structure
 *   EhciTemporaryBarAddress    : temporary base address value of Ehci controllers
 *
 * Output
 *
 *
 *********************************************************************************/
EFI_STATUS
FchEhciOnRecovery (
  IN      FCH_PEI_PRIVATE      FchPrivate,
  IN      UINT32               EhciTemporaryBarAddress
  )
{
  AMD_CONFIG_PARAMS     StdHeader;

  StdHeader = FchPrivate.StdHdr;
  EnableEhciController (USB1_EHCI_BUS_DEV_FUN << 16, EhciTemporaryBarAddress, &StdHeader);
  EnableEhciController (USB2_EHCI_BUS_DEV_FUN << 16, EhciTemporaryBarAddress + 0x400, &StdHeader);
  EnableEhciController (USB3_EHCI_BUS_DEV_FUN << 16, EhciTemporaryBarAddress + 0x800, &StdHeader);
  return EFI_SUCCESS;
}


