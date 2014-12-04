/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Hudson-2 SMI Dispatcher Driver
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  UEFI
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 */
/******************************************************************************
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
****************************************************************************/
#include "FchSmmDispatcher.h"

FCH_SMM_DISPATCHER_TABLE   FchSmmDispatcherTable[] = {
  {
    FCH_SMI_REG88,
    UsbSmi,
    FchSmmUsbDispatchHandler
  },
  {
    FCH_SMI_REG88,
    Slp_Type,
    FchSmmSxDispatchHandler
  },
  {
    FCH_SMI_REG88,
    SmiCmdPort,
    FchSmmSwDispatchHandler
  },
  {
    FCH_SMI_REG84,
    PwrBtn,
    FchSmmPwrBtnDispatchHandler
  },
  {
    FCH_SMI_REG90,
    IoTrapping0 | IoTrapping1 | IoTrapping2 | IoTrapping3,
    FchSmmIoTrapDispatchHandler
  },
  {
    FCH_SMI_REG90,
    ShortTimer | LongTimer,
    FchSmmPeriodicalDispatchHandler
  },
  {
    FCH_SMI_REG10,
    Gpe,
    FchSmmGpeDispatchHandler
  },
  {
    FCH_SMI_REG84,
    Smbus0,
    FchSmmMiscDispatchHandler
  },
  {
    FCH_SMI_REG80,
    AllGevents,
    FchSmmMiscDispatchHandler
  },
#ifdef FCH_TIMER_TICK_INTERVAL_WA
  {
    FCH_SMI_REG90,
    BIT24,
    FchSmmMiscDispatchHandler
  },
  {
    FCH_SMI_REG8C,
    BIT2,
    FchSmmMiscDispatchHandler
  },
#endif
  {
    FCH_SMI_REG90,
    MemTrapping0 | MemTrapping1 | MemTrapping2 | MemTrapping3,
    FchSmmMiscDispatchHandler
  },
  {
    FCH_SMI_REG88,
    FchGppSerr0 | FchGppSerr1 | FchGppSerr2 | FchGppSerr3,
    FchSmmMiscDispatchHandler
  },
};
UINT8 NumOfDispatcherTableEntry = sizeof (FchSmmDispatcherTable) / sizeof (FCH_SMM_DISPATCHER_TABLE);

SAVE_B2B_IO       B2bIoList[] = {
  {CFG_ADDR_PORT, SMM_IO_UINT32, 0},
  {0x70,  SMM_IO_UINT8, 0},
  {0x72,  SMM_IO_UINT8, 0},
  {FCH_IOMAP_REGC00, SMM_IO_UINT8, 0},
  {FCH_IOMAP_REGC50, SMM_IO_UINT8, 0},
  {FCH_IOMAP_REGCD0, SMM_IO_UINT8, 0},
  {FCH_IOMAP_REGCD4, SMM_IO_UINT8, 0},
  {FCH_IOMAP_REGCD6, SMM_IO_UINT8, 0},
};
UINT8 NumOfB2bIoListEntry = sizeof (B2bIoList) / sizeof (SAVE_B2B_IO);

SAVE_PCI      SavePciList[] = {
  {0, 0, 0, 0x60, SMM_IO_UINT32, 0},
  {0, 0, 0, 0x94, SMM_IO_UINT32, 0},
  {0, 0, 0, 0xE8, SMM_IO_UINT32, 0},
  {0, 0, 0, 0xE0, SMM_IO_UINT32, 0},
};
UINT8 NumOfSavePciListEntry = sizeof (SavePciList) / sizeof (SAVE_PCI);
