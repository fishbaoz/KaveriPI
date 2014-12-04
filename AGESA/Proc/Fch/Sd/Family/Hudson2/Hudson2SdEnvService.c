/* $NoKeywords:$ */
/**
 * @file
 *
 * Config Hudson2 SD
 *
 * Init SD Controller.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: FCH
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
****************************************************************************
*/
#include "FchPlatform.h"
#include "Filecode.h"
#define FILECODE PROC_FCH_SD_FAMILY_HUDSON2_HUDSON2SDENVSERVICE_FILECODE

/**
 * FchInitEnvSdProgram - Config SD controller before PCI
 * emulation
 *
 *
 *
 * @param[in] FchDataPtr Fch configuration structure pointer.
 *
 */
VOID
FchInitEnvSdProgram (
  IN  VOID     *FchDataPtr
  )
{
  UINT32                 SdData32;
  FCH_DATA_BLOCK         *LocalCfgPtr;
  AMD_CONFIG_PARAMS      *StdHeader;
  UINT8                  SdClockControl;

  LocalCfgPtr = (FCH_DATA_BLOCK *) FchDataPtr;
  StdHeader = LocalCfgPtr->StdHeader;

  SdClockControl = (UINT8) (LocalCfgPtr->Sd.SdClockControl);
  if (( SdClockControl != 4 ) && ( SdClockControl != 6 ) && ( SdClockControl != 7 ))  {
    SdClockControl = 4;
  }
  SdClockControl = SdClockControl << 1;
  //
  // SD Configuration
  //
  if ( LocalCfgPtr->Sd.SdConfig != SdDisable) {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE +  FCH_PMIOA_REGD3, AccessWidth8, 0xBF, 0x40);
    RwMem (ACPI_MMIO_BASE + MISC_BASE +  FCH_MISC_REG41, AccessWidth8, 0xF1, 0x40 | SdClockControl );
    RwMem (ACPI_MMIO_BASE + MISC_BASE +  FCH_MISC_REG42, AccessWidth8, 0xFE, 0x00);
    RwMem (ACPI_MMIO_BASE + PMIO_BASE +  FCH_PMIOA_REGE7, AccessWidth8, 0x00, 0x12);

    ReadPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA4, AccessWidth32, &SdData32, StdHeader);
    SdData32 |= BIT31 + BIT24 + BIT18 + BIT16;     ///ADMA

    if ( LocalCfgPtr->Sd.SdConfig == SdDma) {
      SdData32 &=  ~(BIT16 + BIT24);               ///DMA
    } else if ( LocalCfgPtr->Sd.SdConfig == SdPio) {
      SdData32 &=  ~(BIT16 + BIT18 + BIT24);       ///PIO
    }

    SdData32 &= ~(BIT17 + BIT23);                  ///clear bitwidth
    SdData32 |= (LocalCfgPtr->Sd.SdSpeed << 17) + (LocalCfgPtr->Sd.SdBitWidth << 23);
    RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGA4, AccessWidth32, 0, SdData32, StdHeader);

    // SD: Some SD cards cannot be detected in HIGH speed mode
    RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGB0, AccessWidth32, (UINT32) (~ (0x03 << 10)), (UINT32) (0x03 << 10), StdHeader);
    if (LocalCfgPtr->Sd.SdSsid != NULL ) {
      RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REG2C, AccessWidth32, 0, LocalCfgPtr->Sd.SdSsid, StdHeader);
    }
    RwPci ((SD_BUS_DEV_FUN << 16) + SD_PCI_REGAC, AccessWidth32, ~(UINT32)BIT1, 0, StdHeader);
  } else {
    RwMem (ACPI_MMIO_BASE + PMIO_BASE +  FCH_PMIOA_REGD3, AccessWidth8, 0xBF, 0x00);
  }
}
