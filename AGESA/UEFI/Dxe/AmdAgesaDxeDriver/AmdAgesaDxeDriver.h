/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPU Initialization
 *
 * Contains CPU code to perform AMD InitMid and AmdInitLate initialization under DXE
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  UEFI
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
 ****************************************************************************
 * AMD Generic Encapsulated Software Architecture
 *
 * Description: AmdAgesaDxeDriver.h - Agesa Dxe Driver Initialization .
 *
 ******************************************************************************
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
#ifndef _AMD_AGESA_DXE_DRIVER_H_
#define _AMD_AGESA_DXE_DRIVER_H_

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */

// AGESA includes
#include "Tiano.h"
#include "EfiDriverLib.h"
#include "PeiHob.h"
#include "EfiHobLib.h"
#include "AGESA.h"
#include "heapManager.h"
#include "mmioMapManager.h"
#include "Ids.h"
#include "OemS3Save.h"
#include "cpuFeatures.h"
//
// Produced Protocols
//
#include EFI_PROTOCOL_PRODUCER (AmdAgesaMmioProtocol)
#include EFI_PROTOCOL_PRODUCER (AmdAgesaDxeProtocol)
#include EFI_PROTOCOL_PRODUCER (AmdBufferManagerProtocol)
#include EFI_PROTOCOL_PRODUCER (AmdInitMidProtocol)
#include EFI_PROTOCOL_PRODUCER (AmdInitLateProtocol)
#include EFI_PROTOCOL_PRODUCER (AmdAgesaMiscServicesProtocol)

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define AGESA_DXE_PRIVATE_DATA_SIGNATURE   EFI_SIGNATURE_32('A', 'C', 'D', 'X')

#define AGESA_DXE_PRIVATE_DATA_INSTANCE_FROM_THIS(a) \
  CR (a, AGESA_DXE_PRIVATE_DATA, CpuInterface, AGESA_DXE_PRIVATE_DATA_SIGNATURE)

EFI_GUID  PciIoProtocolInstallEventGuid = {0xbf9d5465, 0x4fc3, 0x4021, 0x92, 0x5, 0xfa, 0x5d, 0x3f, 0xe2, 0xa1, 0x95};
EFI_GUID  AmdInitMidProtocolInstallEventGuid = {0xbf9d5465, 0x4fc3, 0x4021, 0x92, 0x5, 0xfa, 0x5d, 0x3f, 0xe2, 0xa5, 0x95};
/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */
/// private data and access defines
typedef struct _AMD_CPU_DXE_PRIVATE {
  UINTN                             Signature;          ///< Signature
  AMD_MID_PARAMS                    *MidParamsPtr;      ///< Mid Params
  AMD_LATE_PARAMS                   *LateParamsPtr;     ///< Late Params
  AGESA_DXE_BUFFER_MANAGER          *BufferManagerPtr;  ///< Buffer Manager Pointer
  HEAP_MANAGER                      *HeapManagerPtr;    ///< Heap Manager
  AMD_AGESA_MMIO_PROTOCOL           MmioMapManager;     ///< MMIO map manager
  AMD_AGESA_DXE_PROTOCOL            CpuInterface;       ///< CpuInterface Data
  AMD_AGESA_MISC_SERVICES_PROTOCOL  AgesaMiscServices;  ///< AgesaMiscServices
} AGESA_DXE_PRIVATE_DATA;


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */
EFI_STATUS
EFIAPI
AgesaDxeDriverEntryPoint (
  IN       EFI_HANDLE         ImageHandle,
  IN       EFI_SYSTEM_TABLE   *SystemTable
  );
#endif // _AMD_AGESA_DXE_DRIVER_H_
