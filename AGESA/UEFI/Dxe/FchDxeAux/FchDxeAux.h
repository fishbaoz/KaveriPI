/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH DXE Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH DXE Driver
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

#ifndef _FCH_DXE_AUX_H_
#define _FCH_DXE_AUX_H_

#include "Tiano.h"
#include "EfiDriverLib.h"
#include "FchPlatform.h"
#include "FchDxeLib.h"

// Protocols
#include EFI_PROTOCOL_PRODUCER (FchDxeAuxInitProtocol)

//
// Module data structure
//
/// Private data and access defines
typedef struct _FCH_DXE_AUX_PRIVATE {
  UINTN                            Signature;          ///< Signature
  FCH_DXE_AUX_INIT_PROTOCOL        FchDxeAuxInit;      ///< FCH Dxe Aux
  AMD_CONFIG_PARAMS                StdHdr;             ///< AMD_CONFIG_PARAMS
} FCH_DXE_AUX_PRIVATE;

#define FCH_DXE_AUX_PRIVATE_DATA_SIGNATURE   EFI_SIGNATURE_32 ('S', 'B', 'i', 'A')

#define FCH_DXE_PRIVATE_FROM_THIS(a) \
 (FCH_DXE_AUX_PRIVATE*) (CR (a, FCH_DXE_AUX_PRIVATE, FchDxeAuxInit, FCH_DXE_AUX_PRIVATE_DATA_SIGNATURE))


//
// Functions Prototypes
//
EFI_STATUS
EFIAPI
FchDxeAuxInit (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  );

/**
 * @page fchauxgpio FCH AUX GPIO programming Services
 *
 * FCH AUX Specific Services provides access to supported
 * GPIO Programming service functions.
 *
 * For GPIO Programming Service, we provided two phase in UEFI BIOS.
 * <ul>
 * <li> In DXE, there is function "FpFchDxeAuxGpio" in protocol "FchDxeAuxInitProtocolGuid".
 * </ul>
 *
 * <ul>
 * <li> In PEI, There is function "FpFchPeiAuxGpio" in PPI "AmdFchPeiAuxPpiGuid"
 * </ul>
 *
 * @par Input Parameter
 *   NumberOfGpio - Number of GPIO to be program
 *   *GpioPr      - Pointer of GPIO table
 *
 * @par GPIO table struct
 *
 * <ul>
 * <li>GPIO_DEF
 *
 * </ul>
 * @par PEI Example
 *
 * <ul>
 * <li> In Platform BIOS,
 * </ul>
 *
 *  @code
 *
 * #include "Fch.h"
 *
 * #include EFI_PPI_CONSUMER (AmdFchPeiAuxPpi)
 *
 * static GPIO_DEF PlatformPeiAuxGpioDataBlock[] = {
 *  {FCH_GPIO_REG32, GPIO_32_SELECT, GPIO_32_TYPE, GPO_32_LEVEL + GPIO_32_STICKY + GPIO_32_PULLUP + GPIO_32_PULLDOWN},
 *  {FCH_GPIO_REG33, GPIO_33_SELECT, GPIO_33_TYPE, GPO_33_LEVEL + GPIO_33_STICKY + GPIO_33_PULLUP + GPIO_33_PULLDOWN},
 *  {FCH_GPIO_REG43, GPIO_43_SELECT, GPIO_43_TYPE, GPO_43_LEVEL + GPIO_43_STICKY + GPIO_43_PULLUP + GPIO_43_PULLDOWN},
 *  {FCH_GPIO_REG45, GPIO_45_SELECT, GPIO_45_TYPE, GPO_45_LEVEL + GPIO_45_STICKY + GPIO_45_PULLUP + GPIO_45_PULLDOWN},
 *  {FCH_GPIO_REG47, GPIO_47_SELECT, GPIO_47_TYPE, GPO_47_LEVEL + GPIO_47_STICKY + GPIO_47_PULLUP + GPIO_47_PULLDOWN}
 * };
 *
 *
 * static UINT16 PlatformGpioCount = sizeof (PlatformPeiAuxGpioDataBlock) / sizeof (GPIO_DEF);
 *
 * EFI_STATUS
 * SbPlatformSetting (
 *  IN  EFI_PEI_SERVICES        **PeiServices,
 *  IN  SYSTEM_CONFIGURATION    *SystemConfiguration,
 *  IN  BOOLEAN                 SetupVariableExist
 * )
 *{
 * EFI_PEI_PPI_DESCRIPTOR     *PeiPpiDescriptor;
 * AMD_FCH_PEI_AUX_PPI        FchPeiAuxPpi;
 * EFI_STATUS                 Status;
 *
 * EFI_GUID gAmdFchPeiAuxPpiGuid = AMD_FCH_PEI_AUX_PPI_GUID;
 *
 * //
 * // GPIO Setting
 * //
 *  Status = (**PeiServices).LocatePpi (
 *                                      PeiServices,
 *                                      &gAmdFchPeiAuxPpiGuid,
 *                                      0,
 *                                      &PeiPpiDescriptor,
 *                                      &FchPeiAuxPpi
 *                                      );
 *
 *  if (!EFI_ERROR (Status)) {
 *    FchPeiAuxPpi->FpFchPeiAuxGpio (FchPeiAuxPpi, PlatformGpioCount, (UINT8*) &PlatformPeiAuxGpioDataBlock);
 *  }
 *
 *  return Status;
 *
 * @endcode
 *
 */
VOID
FchDxeAuxGpio (
  IN       UINT16               NumberOfGpio,
  IN       UINT8                *GpioPr
  );

/**
 * @page auxgpioread FCH AUX Read GPIO Services
 *
 * FCH AUX Specific Services provides access to supported
 * Read GPIO service functions.
 *
 * For Read GPIO Service, we provided two phase in UEFI BIOS.
 * <ul>
 * <li> In DXE, there is function "FpFchDxeAuxGpioRead" in protocol "FchDxeAuxInitProtocolGuid".
 * </ul>
 *
 * <ul>
 * <li> In PEI, There is function "FpFchPeiAuxGpioRead" in PPI "AmdFchPeiAuxPpiGuid"
 * </ul>
 *
 * @par Input Parameter
 *   GpioNumber - which GPO that would read
 *
 * @par DXE Example
 *
 * <ul>
 * <li> In Platform BIOS,
 * </ul>
 *
 * @code
 *
 * #include EFI_PROTOCOL_CONSUMER  (FchDxeAuxInitProtocol)
 * FCH_DXE_AUX_INIT_PROTOCOL  *pFchDxeAuxInitProtocol;
 *
 * EFI_STATUS
 * DxePlatformEntryPoint (
 *  IN EFI_HANDLE         ImageHandle,
 *  IN EFI_SYSTEM_TABLE   *SystemTable
 * )
 * {
 * Status = gBS->LocateProtocol (&gFchDxeAuxInitProtocolGuid, NULL, &pFchDxeAuxInitProtocol);
 * if (!EFI_ERROR (Status)) {
 *   UINT8  GpioPort;
 *   GpioPort = FCH_GEVENT_REG12;
 *   GpioPort = pFchDxeAuxInitProtocol->FpFchDxeAuxGpioRead (GpioPort);
 *   }
 * }
 *
 * GPIO status description (Retuen value)
 *
 *   BIT0 - Owned By Ec
 *   BIT1 - Owned By Host
 *   BIT2 - Sticky bit
 *   BIT3 - Pull Up Disable
 *   BIT4 - Pull Down Enable
 *   BIT5 - Gpio Output Disable
 *   BIT6 - Output Value (BIT5 as 0)
 *   BIT7 - Pin Value (Read only)
 * @endcode
 *
 */
UINT8
FchDxeAuxGpioRead (
  IN       UINT8                GpioNumber
  );

/**
 * @page auxdisusbport FCH AUX USB Port Disable Services
 *
 * FCH AUX Specific Services provides access to supported
 * Disable USB Ports service functions.
 *
 * For Disable USB Ports Service, we provided two phase in UEFI BIOS.
 * <ul>
 * <li> In DXE, there is function "FpFchDxeAuxDisUsbPort" in protocol "FchDxeAuxInitProtocolGuid".
 * </ul>
 *
 * <ul>
 * <li> In PEI, There is function "FpFchPeiAuxDisUsbPort" in PPI "AmdFchPeiAuxPpiGuid"
 * </ul>
 *
 * @par Input Parameter
 *   *UsbBitMapPr  - Pointer of BitMap for each USB controller
 *
 * @par USB BitMap struct
 *
 * <ul>
 * <li> USB_PORT_BITMAP
 *
 * </ul>
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
 * static USB_PORT_BITMAP PlatformPeiAuxDisUsbPortBM[] = {
 *    {0x1F, 0x1F, 0x0F, 0x03}, // Disable All of USB ports
 *  };
 *
 * EFI_STATUS
 * SbPlatformSetting (
 *  IN  EFI_PEI_SERVICES        **PeiServices,
 *  IN  SYSTEM_CONFIGURATION    *SystemConfiguration,
 *  IN  BOOLEAN                 SetupVariableExist
 * )
 *{
 * EFI_PEI_PPI_DESCRIPTOR     *PeiPpiDescriptor;
 * AMD_FCH_PEI_AUX_PPI        FchPeiAuxPpi;
 * EFI_STATUS                 Status;
 *
 * EFI_GUID gAmdFchPeiAuxPpiGuid = AMD_FCH_PEI_AUX_PPI_GUID;
 *
 *  Status = (**PeiServices).LocatePpi (
 *                                      PeiServices,
 *                                      &gAmdFchPeiAuxPpiGuid,
 *                                      0,
 *                                      &PeiPpiDescriptor,
 *                                      &FchPeiAuxPpi
 *                                      );
 *
 *  if (!EFI_ERROR (Status)) {
 *    FchPeiAuxPpi->FpFchPeiAuxDisUsbPort ( FchPeiAuxPpi, PlatformPeiAuxDisUsbPortBM);
 *  }
 *
 *  return Status;
 *
 * @endcode
 *
 */
VOID
FchDxeDisUsbPort (
  IN       USB_PORT_BITMAP      *UsbBitMapPr
  );
/**
 * @page auxboottimer FCH AUX Boot Timer Function Services
 *
 * FCH AUX Specific Services provides access to supported
 * Boot Timer service functions.
 *
 * For Boot Timer Service, we provided two phase in UEFI BIOS.
 * <ul>
 * <li> In DXE, there is function "FpFchDxeAuxBootTimer" in protocol "FchDxeAuxInitProtocolGuid".
 * </ul>
 *
 * <ul>
 * <li> In PEI, There is function "FpFchPeiAuxBootTimer" in PPI "AmdFchPeiAuxPpiGuid"
 * </ul>
 *
 * @par Input Parameter
 *   BOOLEAN              EnableBootTimer,
 *   BOOLEAN              StopBootTimer,
 *.  BOOLEAN              SystemReadyForBoot
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
 * EFI_STATUS
 * SbPlatformSetting (
 *  IN  EFI_PEI_SERVICES        **PeiServices,
 *  IN  SYSTEM_CONFIGURATION    *SystemConfiguration,
 *  IN  BOOLEAN                 SetupVariableExist
 * )
 *{
 * EFI_PEI_PPI_DESCRIPTOR     *PeiPpiDescriptor;
 * AMD_FCH_PEI_AUX_PPI        FchPeiAuxPpi;
 * EFI_STATUS                 Status;
 * BOOLEAN                    EnableBootTimer;
 * BOOLEAN                    StopBootTimer;
 * BOOLEAN                    SystemReadyForBoot;
 *
 * EFI_GUID gAmdFchPeiAuxPpiGuid = AMD_FCH_PEI_AUX_PPI_GUID;
 *
 *  Status = (**PeiServices).LocatePpi (
 *                                      PeiServices,
 *                                      &gAmdFchPeiAuxPpiGuid,
 *                                      0,
 *                                      &PeiPpiDescriptor,
 *                                      &FchPeiAuxPpi
 *                                      );
 *
 *  if (!EFI_ERROR (Status)) {
 *    EnableBootTimer    = FALSE;
 *    StopBootTimer      = FALSE;
 *    SystemReadyForBoot = TRUE;
 *    FchPeiAuxPpi->FpFchPeiAuxBootTimer (  FchPeiAuxPpi, EnableBootTimer, StopBootTimer, SystemReadyForBoot);
 *  }
 *
 *  return Status;
 *
 * @endcode
 *
 */
VOID
FchDxeAuxBootTimer (
  IN       BOOLEAN              EnableBootTimer,
  IN       BOOLEAN              StopBootTimer,
  IN       BOOLEAN              SystemReadyForBoot
  );
/**
 * @page auxesataset FCH AUX E-Sata Port Setting Services
 *
 * FCH AUX Specific Services provides access to supported
 * E-Sata Port setting service functions.
 *
 * There is only Dxe phase in UEFI BIOS.
 * <ul>
 * <li> In DXE, there is function "FpFchDxeAuxEsataPort" in protocol "FchDxeAuxInitProtocolGuid".
 * </ul>
 *
 * @par Input Parameter
 *   EsataBitMapPr - which is Bits Map that Sata Port would like to set as E-Sata Port
 *
 * @par DXE Example
 *
 * <ul>
 * <li> In Platform BIOS,
 * </ul>
 *
 * @code
 *
 * #include EFI_PROTOCOL_CONSUMER  (FchDxeAuxInitProtocol)
 * FCH_DXE_AUX_INIT_PROTOCOL  *pFchDxeAuxInitProtocol;
 *
 * static ESATA_PORT_BITMAP PlatformDxeAuxEsataPortBM[] = {
 *   {0xC0},    // Port6 and Port7 set as E-Sata port
 * };
 *
 * EFI_STATUS
 * DxePlatformEntryPoint (
 *  IN EFI_HANDLE         ImageHandle,
 *  IN EFI_SYSTEM_TABLE   *SystemTable
 * )
 * {
 *   Status = gBS->LocateProtocol (&gFchDxeAuxInitProtocolGuid, NULL, &pFchDxeAuxInitProtocol);
 *   if (!EFI_ERROR (Status)) {
 *     pFchDxeAuxInitProtocol->FpFchDxeAuxEsataPort ( PlatformDxeAuxEsataPortBM );
 *   }
 * }
 *
 * @endcode
 *
 */
VOID
FchDxeEsataPortSet (
  IN       ESATA_PORT_BITMAP      *EsataBitMapPr
  );
#endif // _FCH_DXE_AUX_H_
