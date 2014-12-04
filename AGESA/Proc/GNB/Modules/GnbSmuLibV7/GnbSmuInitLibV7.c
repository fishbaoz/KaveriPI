/* $NoKeywords:$ */
/**
 * @file
 *
 * NB services
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
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
* ***************************************************************************
*
*/

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include  "AGESA.h"
#include  "Ids.h"
#include  "amdlib.h"
#include  "S3SaveState.h"
#include  "Gnb.h"
#include  "GnbPcieConfig.h"
#include  "GnbCommonLib.h"
#include  "GnbPcieInitLibV1.h"
#include  "GnbSmuInitLibV7.h"
#include  "heapManager.h"
#include  "GnbFamServices.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBSMULIBV7_GNBSMUINITLIBV7_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

#define SMC_RAM_START_ADDR 0x20000ul
#ifndef INVALID_SMU_MSG
  #define INVALID_SMU_MSG  0xFF
#endif

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 * SMU service request
 *
 *
 * @param[in]  DevObject       Pointer to Device object
 * @param[in]  RequestId       Request ID
 * @param[in]  RequestArgument Request Argument
 * @param[in]  AccessFlags     See GNB_ACCESS_FLAGS_* definitions
 */

VOID
GnbSmuServiceRequestV7 (
  IN       DEV_OBJECT               *DevObject,
  IN       UINT8                    RequestId,
  IN       UINT32                   RequestArgument,
  IN       UINT32                   AccessFlags
  )
{
  UINT32             BFIntDone;
  UINT32             BFIntToggle;
  UINT32             BFIntAck;
  UINT32             RxSmuIntArgument;
  URA_TUPLE          UraTuple[2];
  UINT32             IsMsgValid;

  IsMsgValid = FALSE;
  if (RequestId != INVALID_SMU_MSG) {
    IsMsgValid = TRUE;
  }

  if (IsMsgValid == TRUE) {
    IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuServiceRequestV7 Enter\n");
    IDS_HDT_CONSOLE (NB_MISC, "  Service Request %d\n", RequestId);
    IDS_HDT_CONSOLE (NB_MISC, "  Service Request Argument 0x%x\n", RequestArgument);

    if ((AccessFlags & GNB_REG_ACC_FLAG_S3SAVE) != 0) {
      SMU_MSG_CONTEXTV7 SmuMsgContext;
      SmuMsgContext.GnbPciAddress.AddressValue = DevObject->DevPciAddress.AddressValue;
      SmuMsgContext.RequestId = RequestId;
      SmuMsgContext.RequestArgument = RequestArgument;
      S3_SAVE_DISPATCH (DevObject->StdHeader, GnbSmuServiceRequestV7S3Script_ID, sizeof (SmuMsgContext), &SmuMsgContext);
    }

    do {
      GnbUraGet (DevObject, TBfxSmuIntDone, &BFIntDone);
    } while (BFIntDone == 0x0);

    RxSmuIntArgument = RequestArgument;
    GnbUraSet (DevObject, TRxSmuIntArgument, &RxSmuIntArgument);

    GnbUraGet (DevObject, TBfxSmuIntToggle, &BFIntToggle);
    UraTuple[0].Token = TBfxSmuIntToggle;
    UraTuple[0].Value = ~BFIntToggle;
    UraTuple[1].Token = TBfxSmuServiceIndex;
    UraTuple[1].Value = RequestId;
    GnbUraCombinedSet (DevObject, TRxSmuIntReq, &UraTuple[0], 2);

    do {
      GnbUraGet (DevObject, TBfxSmuIntAck, &BFIntAck);
    } while (BFIntAck == 0x0);

    do {
      GnbUraGet (DevObject, TBfxSmuIntDone, &BFIntDone);
    } while (BFIntDone == 0x0);

    IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuServiceRequestV7 Exit\n");
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * SMU service request for S3 script
 *
 *
 * @param[in]  StdHeader       Standard configuration header
 * @param[in]  ContextLength   Context length
 * @param[in]  Context         Pointer to Context
 */

VOID
GnbSmuServiceRequestV7S3Script (
  IN      AMD_CONFIG_PARAMS     *StdHeader,
  IN      UINT16                ContextLength,
  IN      VOID                  *Context
  )
{
  SMU_MSG_CONTEXTV7 *SmuMsgContext;
  DEV_OBJECT        DevObject;
  SmuMsgContext =  (SMU_MSG_CONTEXTV7 *) Context;

  DevObject.StdHeader = StdHeader;
  DevObject.DevPciAddress = SmuMsgContext->GnbPciAddress;
  DevObject.GnbHandle = GnbGetHandle (StdHeader);

  GnbSmuServiceRequestV7 (&DevObject, SmuMsgContext->RequestId, SmuMsgContext->RequestArgument, 0);
}

/*----------------------------------------------------------------------------------------*/
/**
 * SMU firmware download
 *
 *
 * @param[in]  GnbHandle       Pointer to GNB_HANDLE
 * @param[in]  Firmware        Pointer t0 firmware
 * @param[in]  StdHeader       Standard configuration header
 */

AGESA_STATUS
GnbSmuFirmwareLoadV7 (
  IN       GNB_HANDLE               *GnbHandle,
  IN       FIRMWARE_HEADER_V7       *Firmware,
  IN       AMD_CONFIG_PARAMS        *StdHeader
  )
{
  AGESA_STATUS                Status;
  UINT32                      BfxSmuProtectedMode;
  UINT32                      BfxSmuBootSeqDone;
  UINT32                      BfxSmuAuthDone;
  UINT32                      BfxSmuAuthPass;
  UINT32                      RxSmuAuthVector;
  UINT32                      BfxSmuRstReg;
  UINT32                      BfxSmuCkDisable;
  UINT32                      BfxSmuInterruptsEnabled;
  UINT32                      RxSmuRamStartAddr;
  GNB_REGISTER_SERVICE        *GnbRegisterAccessProtocol;
  PCI_ADDR                    GnbPciAddress;
  DEV_OBJECT                  DevObject;
  URA_TUPLE                   UraTuple;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuFirmwareLoadV7 Enter\n");
  IDS_HDT_CONSOLE (NB_MISC, "  Firmware version 0x%x\n", Firmware->Version);
  IDS_OPTION_HOOK (IDS_REPORT_SMU_FW_VERSION, &(Firmware->Version), StdHeader);

  GnbPciAddress = GnbGetHostPciAddress (GnbHandle);
  Status = GnbLibLocateService (GnbRegisterAccessService, GnbGetSocketId (GnbHandle), &GnbRegisterAccessProtocol, StdHeader);
  ASSERT (Status == AGESA_SUCCESS);

  DevObject.GnbHandle = GnbHandle;
  DevObject.StdHeader = StdHeader;
  DevObject.DevPciAddress.AddressValue = GnbPciAddress.AddressValue;

  IDS_HDT_CONSOLE (GNB_TRACE, "Step 2 & 10: make sure Rom firmware sequence is done\n");
  // Step 2, 10, make sure Rom firmware sequence is done
  do {
    GnbUraGet (&DevObject, TBfxSmuBootSeqDone, &BfxSmuBootSeqDone);
  } while (BfxSmuBootSeqDone == 0);

  IDS_HDT_CONSOLE (GNB_TRACE, "Step 1: check if firmware running in protected mode\n");
  // Step 1, check if firmware running in protected mode
  GnbUraGet (&DevObject, TBfxSmuProtectedMode, &BfxSmuProtectedMode);
  if (BfxSmuProtectedMode == 0) {
    // Step3, Clear firmware interrupt flags
    IDS_HDT_CONSOLE (GNB_TRACE, "Step 3: Clear firmware interrupt flags\n");
    //IDS_HDT_CONSOLE (GNB_TRACE, "Read register data 0x%x\n", FIRMWARE_FLAGS.Value);
    BfxSmuInterruptsEnabled = 0;
    GnbUraSet (&DevObject, TBfxSmuInterruptsEnabled, &BfxSmuInterruptsEnabled);
  }

  //Step 4, 11, Assert SMU reset
  IDS_HDT_CONSOLE (GNB_TRACE, "Step 4, 11: Assert SMU reset\n");
  BfxSmuRstReg = 1;
  GnbUraSet (&DevObject, TBfxSmuRstReg, &BfxSmuRstReg);

  // Step5, 12, Load firmware
  IDS_HDT_CONSOLE (GNB_TRACE, "Step5, 12, Load firmware\n");
  // 4 means byte length of next address during firmware download
  UraTuple.StepLength = 4;
  UraTuple.Value = (UINT32) ((UINTN) (Firmware));
  GnbUraCombinedSet (&DevObject, TRxSmuRamStartAddr | GNB_URA_STREAM_SET, &UraTuple, (Firmware->ImageSize >> 2));

  if (BfxSmuProtectedMode == 0) {
    IDS_HDT_CONSOLE (GNB_TRACE, "Step6, write jmp to RAM firmware\n");
    //Step 6, Write jmp to RAM firmware
    RxSmuRamStartAddr = 0xE0000000 + ((SMC_RAM_START_ADDR + Firmware->HeaderSize) >> 2);
    GnbUraSet (&DevObject, TRxSmuRomStartAddr, &RxSmuRamStartAddr);
  } else {
    //Step 13, Clear authentification done
    IDS_HDT_CONSOLE (GNB_TRACE, "Step 13, Clear authentification done\n");
    BfxSmuAuthDone = 0;
    GnbUraSet (&DevObject, TBfxSmuAuthDone, &BfxSmuAuthDone);
  }

  // Step 7, 14 Enable SMU clock
  IDS_HDT_CONSOLE (GNB_TRACE, "Step 7, 14 Enable SMU clock\n");
  BfxSmuCkDisable = 0;
  GnbUraSet (&DevObject, TBfxSmuCkDisable, &BfxSmuCkDisable);

  //Step 8, 15, Deassert SMU reset
  IDS_HDT_CONSOLE (GNB_TRACE, "Step 8, 15, Deassert SMU reset\n");
  BfxSmuRstReg = 0;
  GnbUraSet (&DevObject, TBfxSmuRstReg, &BfxSmuRstReg);

  if (BfxSmuProtectedMode == 1) {
    IDS_HDT_CONSOLE (NB_MISC, "  Protected mode: poll init authentication vector\n");
    // Step 16, Wait for rom firmware init authentication vector
    IDS_HDT_CONSOLE (GNB_TRACE, "Step 16, Wait for rom firmware init authentication vector\n");
    do {
      GnbUraGet (&DevObject, TRxSmuAuthVector, &RxSmuAuthVector);
    } while (RxSmuAuthVector != 0x400);
    // Call Authentication service
    GnbSmuServiceRequestV7 (&DevObject, 0, 0, 0);
    IDS_HDT_CONSOLE (NB_MISC, "  Protected mode: poll init authentication done\n");
    // Wait for authentication done
    do {
      GnbUraGet (&DevObject, TBfxSmuAuthDone, &BfxSmuAuthDone);
    } while (BfxSmuAuthDone == 0x0);

    //Step 17, Check Authentication results
    IDS_HDT_CONSOLE (GNB_TRACE, "Step 17, Check Authentication results\n");
    GnbUraGet (&DevObject, TBfxSmuAuthPass, &BfxSmuAuthPass);
    if (BfxSmuAuthPass == 0) {
      IDS_HDT_CONSOLE (NB_MISC, "  ERROR!!!Authentication fail!!!\n");
      ASSERT (FALSE);
      return AGESA_FATAL;
    }
    // Step 18, Clear firmware interrupt enable flag
    IDS_HDT_CONSOLE (GNB_TRACE, "Step 18, Clear firmware interrupt enable flag\n");
    BfxSmuInterruptsEnabled = 0;
    GnbUraSet (&DevObject, TBfxSmuInterruptsEnabled, &BfxSmuInterruptsEnabled);

    //Step 19, Assert SMU reset
    IDS_HDT_CONSOLE (GNB_TRACE, "Step 19, Assert SMU reset\n");
    BfxSmuRstReg = 1;
    GnbUraSet (&DevObject, TBfxSmuRstReg, &BfxSmuRstReg);

    //Step 20, Deassert SMU reset
    IDS_HDT_CONSOLE (GNB_TRACE, "Step 20, Deassert SMU reset\n");
    BfxSmuRstReg = 0;
    GnbUraSet (&DevObject, TBfxSmuRstReg, &BfxSmuRstReg);
  }

  //Step 9, 21 Wait firmware to initialize
  IDS_HDT_CONSOLE (GNB_TRACE, "Step 9, 21 Wait firmware to initialize\n");
  do {
    GnbUraGet (&DevObject, TBfxSmuInterruptsEnabled, &BfxSmuInterruptsEnabled);
  } while (BfxSmuInterruptsEnabled == 0);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuFirmwareLoadV7 Exit\n");
  return AGESA_SUCCESS;
}

