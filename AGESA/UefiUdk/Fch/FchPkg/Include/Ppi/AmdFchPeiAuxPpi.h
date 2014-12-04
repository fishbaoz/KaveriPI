/* $NoKeywords:$ */
 /**
 * @file
 *
 * AMD FCH EARLY INIT API, and related functions.
 *
 * Contains code that defines the FCH EARLY INIT PPI.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  UEFI
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
 * ***************************************************************************
 */

#ifndef _AMD_FCH_PEI_AUX_PPI_H_
#define _AMD_FCH_PEI_AUX_PPI_H_

extern EFI_GUID gAmdFchPeiAuxPpiGuid;

typedef struct _AMD_FCH_PEI_AUX_PPI AMD_FCH_PEI_AUX_PPI;

///
/// GPIO_DEF - FCH GPIO Programming Data Block
///
typedef struct _GPIO_DEF {
  UINT8                 Gpio;             ///< Gpio       - GPIO Number to config
  UINT8                 GpioSelect;       ///< GpioSelect - GPIO Iomux selection
  UINT8                 GpioType;         ///< GpioType   - GPIO type to config
  UINT8                 GpioValue;        ///< GpioValue  - Value to program to GPIO
} GPIO_DEF;

///
/// USB_PORT_BITMAP - USB BitMap for each controller
///
typedef struct _USB_PORT_BITMAP {
  UINT8                 Usb1PortBM;       ///< Usb1PortBM - USB Controller1 Ports BITMAP
  UINT8                 Usb2PortBM;       ///< Usb2PortBM - USB Controller2 Ports BITMAP
  UINT8                 Usb3PortBM;       ///< Usb3PortBM - XHCI/USB Controller3 Ports BITMAP
  UINT8                 Usb4PortBM;       ///< Usb4PortBM - USB Controller4 Ports BITMAP
} USB_PORT_BITMAP;

///
/// FCH_OSC_OUT_CLOCK_SEL - Fch Osc Out Clock Selection Frequency
///
typedef enum _FCH_OSC_OUT_CLOCK_SEL {
  FCH_OSC_OUT_CLOCK_SEL_14MHz   = 0,      ///< FCH_OSC_OUT_CLOCK_SEL_14MHz - Fch OSC out set as 14Mhz
  FCH_OSC_OUT_CLOCK_SEL_25MHz   = 1,      ///< FCH_OSC_OUT_CLOCK_SEL_25MHz - Fch OSC out set as 25Mhz
  FCH_OSC_OUT_CLOCK_SEL_48MHz   = 2,      ///< FCH_OSC_OUT_CLOCK_SEL_48MHz - Fch OSC out set as 48Mhz
  FCH_OSC_OUT_CLOCK_SEL_50MHz   = 3,      ///< FCH_OSC_OUT_CLOCK_SEL_50MHz - Fch OSC out set as 50Mhz
  FCH_OSC_OUT_CLOCK_SEL_24MHz   = 4,      ///< FCH_OSC_OUT_CLOCK_SEL_24MHz - Fch OSC out set as 24Mhz
} FCH_OSC_OUT_CLOCK_SEL;

///
/// Protocal protypes
///
typedef
VOID
(EFIAPI *FP_FCH_GPIO_PR) (
  AMD_FCH_PEI_AUX_PPI   *This,
  UINT16                NumberOfGpio,
  UINT8                 *GpioPr
  );

typedef
UINT8
(EFIAPI *FP_FCH_PEI_AUX_GPIO_READ_PR) (
  AMD_FCH_PEI_AUX_PPI   *This,
  UINT8                 GpioNumber
  );

typedef
VOID
(EFIAPI *FP_FCH_PEI_AUX_BOOT_TIMER_PR) (
  AMD_FCH_PEI_AUX_PPI   *This,
  BOOLEAN               EnableBootTimer,
  BOOLEAN               StopBootTimer,
  BOOLEAN               SystemReadyForBoot
  );

typedef
VOID
(EFIAPI *FP_FCH_PEI_AUX_DIS_USB_PORT_PR) (
  AMD_FCH_PEI_AUX_PPI   *This,
  USB_PORT_BITMAP       *UsbBitMapPr
  );

typedef
VOID
(EFIAPI *FP_FCH_PEI_AUX_OSC_OUT_CLK_PR) (
  AMD_FCH_PEI_AUX_PPI   *This,
  BOOLEAN               OscOut1ClockSel,
  FCH_OSC_OUT_CLOCK_SEL OscOut1ClockFreq
  );

/// FCH INIT PPI definition
typedef struct _AMD_FCH_PEI_AUX_PPI {
  UINTN                          Revision;                                 ///< Revision number
  FP_FCH_GPIO_PR                 FpFchPeiAuxGpio;                          ///< Programming GPIO Service
  FP_FCH_PEI_AUX_GPIO_READ_PR    FpFchPeiAuxReadGpio;                      ///< Read GPIO Service
  FP_FCH_PEI_AUX_BOOT_TIMER_PR   FpFchPeiAuxBootTimer;                     ///< Boot Timer Function Service
  FP_FCH_PEI_AUX_DIS_USB_PORT_PR FpFchPeiAuxDisUsbPort;                    ///< USB Port Disable Service
  FP_FCH_PEI_AUX_OSC_OUT_CLK_PR  FpFchPeiAuxOscOutClk;                     ///< Osc Out Clock Selection
} AMD_FCH_PEI_AUX_PPI;

//
// current PPI revision
//
#define AMD_FCH_PEI_AUX_PPI_REV  0x00


#endif // _AMD_FCH_PEI_AUX_PPI_H_
