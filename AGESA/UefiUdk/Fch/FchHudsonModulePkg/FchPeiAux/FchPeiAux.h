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

#ifndef _FCH_PEI_AUX_H_
#define _FCH_PEI_AUX_H_

#include <Library\DebugLib.h>
#include <Library/AgesaUefiLib.h>

#include "FchPlatform.h"

#include <Ppi\AmdFchPeiAuxPpi.h>


/// Module data structure
typedef struct _FCH_PEI_PRIVATE {
  UINTN                    Signature;          ///< Signature
  AMD_FCH_PEI_AUX_PPI      FchPeiAuxPpi;       ///< FCH INIT PPI
  AMD_CONFIG_PARAMS        StdHdr;             ///< AMD_CONFIG_PARAMS
} FCH_PEI_AUX_PRIVATE;

#define FCH_PEI_AUX_PRIVATE_DATA_SIGNATURE   SIGNATURE_32 ('$', 'F', 'P', 'E')

#define FCH_PEI_PRIVATE_FROM_THIS(a) \
  (FCH_PEI_AUX_PRIVATE*) (CR ((a), FCH_PEI_AUX_PRIVATE, FchPeiAuxPpi, FCH_PEI_AUX_PRIVATE_DATA_SIGNATURE))

/// Functions Prototypes
EFI_STATUS
EFIAPI
FchPeiAux (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  );

VOID
FchPeiAuxGpio (
  IN       AMD_FCH_PEI_AUX_PPI  *This,
  IN       UINT16               NumberOfGpio,
  IN       UINT8                *GpioPr
  );

UINT8
FchPeiGpioRead (
  IN       AMD_FCH_PEI_AUX_PPI  *This,
  IN       UINT8                GpioNumber
  );

VOID
FchPeiBootTimer (
  IN       AMD_FCH_PEI_AUX_PPI  *This,
  IN       BOOLEAN              EnableBootTimer,
  IN       BOOLEAN              StopBootTimer,
  IN       BOOLEAN              SystemReadyForBoot
  );

VOID
FchPeiDisUsbPort (
  IN       AMD_FCH_PEI_AUX_PPI  *This,
  IN       USB_PORT_BITMAP      *UsbBitMapPr
  );

/**
 * @page auxoscoutsel FCH AUX OSC out clock selection Services
 *
 * FCH AUX Specific Services provides access to supported
 * OSC out clock selection service functions.
 *
 * For Boot Timer Service, we provided only PEI phase in UEFI BIOS.
 *
 * <ul>
 * <li> In PEI, There is function "FpFchPeiAuxOscOutClk" in PPI "AmdFchPeiAuxPpiGuid"
 * </ul>
 *
 * @par Input Parameter
 *        BOOLEAN               OscOut1ClockSel,
 *        FCH_OSC_OUT_CLOCK_SEL OscOut1ClockFreq,
 *
 * @par FCH OSC OUT clock selection items
 *
 *
 * <ul>
 * <li> FCH_OSC_OUT_CLOCK_SEL
 *
 * <ul>
 * <li>   FCH_OSC_OUT_CLOCK_SEL_14MHz   - Fch OSC out set as 14Mhz
 * <li>   FCH_OSC_OUT_CLOCK_SEL_25MHz   - Fch OSC out set as 25Mhz
 * <li>   FCH_OSC_OUT_CLOCK_SEL_48MHz   - Fch OSC out set as 48Mhz
 * <li>   FCH_OSC_OUT_CLOCK_SEL_50MHz   - Fch OSC out set as 50Mhz
 * <li>   FCH_OSC_OUT_CLOCK_SEL_24MHz   - Fch OSC out set as 24Mhz
 * </ul>
 * </ul>
 *
 * @par PEI Example
 * <ul>
 * <li> In Platform BIOS,
 * </ul>
 *
 * @code
 *
 * #include "Fch.h"
 *
 * #include EFI_PPI_CONSUMER (AmdFchPeiAuxPpi)
 *
 *
 * VOID
 * SetOscOutClock (
 *  IN EFI_PEI_SERVICES              **PeiServices
 *  )
 *{
 *  EFI_STATUS                        Status;
 *  AMD_FCH_PEI_AUX_PPI               *FchPeiAuxPpi;
 *  EFI_PEI_PPI_DESCRIPTOR            *PeiPpiDescriptor;
 *  BOOLEAN                           OscOut1ClockSel;
 *
 *  Status = (**PeiServices).LocatePpi (
 *                                       PeiServices,
 *                                       &gAmdFchPeiAuxPpiGuid,
 *                                       0,
 *                                       &PeiPpiDescriptor,
 *                                       &FchPeiAuxPpi
 *                                       );
 *  if (!EFI_ERROR (Status)) {
 *     OscOut1ClockSel    = TRUE;
 *     FchPeiAuxPpi->FpFchPeiAuxOscOutClk (  FchPeiAuxPpi, OscOut1ClockSel, FCH_OSC_OUT_CLOCK_SEL_14MHz);
 *  }
 *}
 *
 * @endcode
 *
 */
VOID
FchPeiOscOutClockSel (
  IN       AMD_FCH_PEI_AUX_PPI   *This,
  IN       BOOLEAN               OscOut1ClockSel,
  IN       FCH_OSC_OUT_CLOCK_SEL OscOut1ClockFreq
  );
#endif // _FCH_PEI_AUX_H_
