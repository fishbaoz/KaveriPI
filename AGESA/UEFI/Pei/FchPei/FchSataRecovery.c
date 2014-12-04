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
#include "FchSataRecovery.h"
#include "FchPlatform.h"
#define FILECODE UEFI_PEI_FCHPEI_FCHSATARECOVERY_FILECODE

SATA_PHY_SETTING SataPhyTable[] =
{
  //Gen3
  0x0030, 0x0057A607,
  0x0031, 0x0057A607,
  0x0032, 0x0057A407,
  0x0033, 0x0057A407,
  0x0034, 0x0057A607,
  0x0035, 0x0057A607,
  0x0036, 0x0057A403,
  0x0037, 0x0057A403,

  //Gen2
  0x0120, 0x00071302,

  //Gen1
  0x0110, 0x00174101
};

VOID
FchRecoveryProgramSataPhy (
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  SATA_PHY_SETTING         *PhyTablePtr;
  UINT16                   Index;

  PhyTablePtr = &SataPhyTable[0];

  for (Index = 0; Index < (sizeof (SataPhyTable) / sizeof (SATA_PHY_SETTING)); Index++) {
    RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG80, AccessWidth16, 0x00, PhyTablePtr->PhyCoreControlWord, StdHeader);
    RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG98, AccessWidth32, 0x00, PhyTablePtr->PhyFineTuneDword, StdHeader);
    ++PhyTablePtr;
  }


  RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG80, AccessWidth16, 0x00, 0x110, StdHeader);
  RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG9C, AccessWidth32, (UINT32) (~(0x7 << 4)), (UINT32) (0x2 << 4), StdHeader);
  RwPci ((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG80, AccessWidth16, 0x00, 0x10, StdHeader);
}

EFI_STATUS
FchSataOnRecovery (
  IN      FCH_PEI_PRIVATE      FchPrivate,
  IN      UINT32               SataBar0,
  IN      UINT32               SataBar5
  )
{
  UINT8                  ValueByte;
  AMD_CONFIG_PARAMS      StdHeader;

  StdHeader = FchPrivate.StdHdr;
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG40), AccessWidth8, 0xff, BIT0, &StdHeader);
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG44), AccessWidth8, 0xff, BIT0, &StdHeader);
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG44 + 2), AccessWidth8, 0, 0x20, &StdHeader);
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG40), AccessWidth8, 0xff, BIT4, &StdHeader);
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG48 + 3), AccessWidth8, 0xff, BIT7, &StdHeader);
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG4C + 3), AccessWidth8, 0x07, 0xF8, &StdHeader);
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG4C), AccessWidth32, (UINT32) (~ (0xF8 << 26)), (UINT32) (0xF8 << 26), &StdHeader);
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG48), AccessWidth32, (UINT32) (~ (0x01 << 11)), (UINT32) (0x01 << 11), &StdHeader);
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG84), AccessWidth32, (UINT32) (~ (0x01 << 31)), (UINT32) (0x00 << 31), &StdHeader);
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG08), AccessWidth32, 0, 0x01018F40, &StdHeader);
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG02), AccessWidth16, 0, FCH_SATA_DID, &StdHeader);
  ValueByte = 0x08;
  LibAmdIoWrite (AccessWidth8, FCH_IOMAP_REGC00, &ValueByte, &StdHeader);
  LibAmdIoRead (AccessWidth8, FCH_IOMAP_REGC01, &ValueByte, &StdHeader);

  //
  // SATA PHY Programming Sequence
  //
  FchRecoveryProgramSataPhy (&StdHeader);

  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG10), AccessWidth32, BIT0, SataBar0, &StdHeader);
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG14), AccessWidth32, BIT0, SataBar0 + 0x100, &StdHeader);
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG18), AccessWidth32, BIT0, SataBar0 + 0x200, &StdHeader);
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG1C), AccessWidth32, BIT0, SataBar0 + 0x300, &StdHeader);
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG20), AccessWidth32, BIT0, SataBar0 + 0x400, &StdHeader);
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG24), AccessWidth32, 0xff, SataBar5, &StdHeader);

  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG04), AccessWidth8, 0, BIT0 + BIT1, &StdHeader);
  RwPci (((SATA_BUS_DEV_FUN << 16) + FCH_SATA_REG40), AccessWidth8, ~BIT0, 0, &StdHeader);
  return EFI_SUCCESS;
}
