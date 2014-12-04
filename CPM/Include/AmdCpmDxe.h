/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM structures and definitions
 *
 * Contains AMD CPM Platform Interface
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  Include
 * @e \$Revision: 287560 $   @e \$Date: 2014-03-18 13:52:46 -0500 (Tue, 18 Mar 2014) $
 *
 */
/*****************************************************************************
 *
 * Copyright 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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

#ifndef _AMD_CPM_DXE_H_
#define _AMD_CPM_DXE_H_

#include <AmdCpmBase.h>
#include CPM_PROTOCOL_DEFINITION (AmdCpmTableProtocol)
#include CPM_PROTOCOL_DEFINITION (AmdCpmNvDataProtocol)
#include CPM_PROTOCOL_DEFINITION (AmdCpmBootTimeRecordProtocol)
#include CPM_PROTOCOL_DEFINITION (AmdBufferManagerProtocol)
#include CPM_PROTOCOL_DEFINITION (AmdCpmDisplayFeatureProtocol)
#include CPM_PROTOCOL_DEFINITION (AmdCpmAllPciIoProtocolsInstalled)
#include CPM_GUID_DEFINITION (AmdCpmTableHob)
#include CPM_GUID_DEFINITION (AmdCpmBootTimeRecordHob)

#ifndef AMD_CPM_EDKII
  #include <EfiDriverLib.h>
  #include <EfiHobLib.h>
  #include <AcpiCommon.h>
  #include <pci22.h>
  #include EFI_PROTOCOL_DEFINITION (CpmFirmwareVolume)
  #include EFI_PROTOCOL_DEFINITION (Smbus)
  #include EFI_PROTOCOL_DEFINITION (AcpiSupport)
  #include EFI_PROTOCOL_DEFINITION (LegacyBios)
  #include EFI_PROTOCOL_DEFINITION (LegacyRegion)
  #include EFI_PROTOCOL_DEFINITION (PciIo)
  #include EFI_GUID_DEFINITION (Acpi)
  #include EFI_GUID_DEFINITION (Hob)
#else
  #include <PiDxe.h>
  #include CPM_LIBRARY_DEFINITION (BaseLib)
  #include CPM_LIBRARY_DEFINITION (BaseMemoryLib)
  #include CPM_LIBRARY_DEFINITION (DebugLib)
  #include CPM_LIBRARY_DEFINITION (HobLib)
  #include CPM_LIBRARY_DEFINITION (PcdLib)
  #include CPM_LIBRARY_DEFINITION (UefiLib)
  #include CPM_LIBRARY_DEFINITION (UefiBootServicesTableLib)
  #include CPM_INDUSTRYSTANDARD_DEFINITION (Acpi)
  #include CPM_INDUSTRYSTANDARD_DEFINITION (Pci22)
  #include CPM_INDUSTRYSTANDARD_DEFINITION (SmBus)
  #include EFI_PROTOCOL_DEFINITION (CpmFirmwareVolume)
  #include EFI_PROTOCOL_DEFINITION (SmbusHc)
  #include EFI_PROTOCOL_DEFINITION (AcpiTable)
  #include EFI_PROTOCOL_DEFINITION (PciIo)
  #include EFI_GUID_DEFINITION (EventGroup)
  #include EFI_GUID_DEFINITION (HobList)
#endif

#endif
