/* $NoKeywords:$ */
/**
 * @file
 *
 * PCIe topology initialization service procedures.
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
#include  "Gnb.h"
#include  "GnbPcie.h"
#include  "GnbPcieFamServices.h"
#include  "GnbCommonLib.h"
#include  "GnbPcieConfig.h"
#include  "GnbPcieInitLibV1.h"
#include  "GnbPcieInitLibV5.h"
#include  "GnbRegistersCommonV2.h"
#include  "GnbSmuInitLibV7.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBPCIEINITLIBV5_PCIEWRAPPERSERVICESV5_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */


UINT8 LaneMuxSelectorArrayV5[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

/*----------------------------------------------------------------------------------------*/
/**
 * Locate mux array index
 *
 *
 *
 * @param[in, out]  LaneMuxSelectorArrayPtr     Pointer to mux selector array
 * @param[in]       LaneMuxValue                The value that match to array
 * @retval          Index                       Index successfully mapped
 */
UINT8
PcieTopologyLocateMuxIndexV5 (
  IN OUT   UINT8                *LaneMuxSelectorArrayPtr,
  IN       UINT8                LaneMuxValue
  )
{
  UINT8  Index;
  for (Index = 0; Index < sizeof (LaneMuxSelectorArrayV5); Index++ ) {
    if (LaneMuxSelectorArrayPtr [Index] ==  LaneMuxValue) {
      return Index;
    }
  }
  return 0;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Apply lane mux
 *
 *
 *
 * @param[in]  Wrapper             Pointer to wrapper config descriptor
 * @param[in]  Pcie                Pointer to global PCIe configuration
 */

VOID
PcieTopologyApplyLaneMuxV5 (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIe_ENGINE_CONFIG  *EngineList;
  UINT32              Index;
  UINT8               RxLaneMuxSelectorArray [sizeof (LaneMuxSelectorArrayV5)];
  UINT8               TxLaneMuxSelectorArray [sizeof (LaneMuxSelectorArrayV5)];

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieTopologyApplyLaneMuxV5 Enter\n");
  if (PcieLibIsPcieWrapper (Wrapper)) {
    LibAmdMemCopy (
      &TxLaneMuxSelectorArray[0],
      &LaneMuxSelectorArrayV5[0],
      sizeof (LaneMuxSelectorArrayV5),
      GnbLibGetHeader (Pcie)
      );
    LibAmdMemCopy (
      &RxLaneMuxSelectorArray[0],
      &LaneMuxSelectorArrayV5[0],
      sizeof (LaneMuxSelectorArrayV5),
      GnbLibGetHeader (Pcie)
      );
    EngineList = PcieConfigGetChildEngine (Wrapper);
    while (EngineList != NULL) {
      if (PcieLibIsPcieEngine (EngineList) && PcieLibIsEngineAllocated (EngineList)) {
        UINT32  CoreLaneBitmap;
        UINT32  PifLaneBitmap;
        UINT8   CurrentCoreLane;
        UINT8   CurrentPifLane;

        CoreLaneBitmap = PcieUtilGetEngineLaneBitMap (LANE_TYPE_PCIE_CORE_ALLOC, 0, EngineList);
        PifLaneBitmap = PcieUtilGetEngineLaneBitMap (LANE_TYPE_PCIE_PHY_NATIVE, 0, EngineList);

        while (CoreLaneBitmap != 0) {
          CurrentCoreLane = LibAmdBitScanForward (CoreLaneBitmap);
          CurrentPifLane = LibAmdBitScanForward (PifLaneBitmap);

          if (TxLaneMuxSelectorArray[CurrentPifLane] != CurrentCoreLane) {
            TxLaneMuxSelectorArray[PcieTopologyLocateMuxIndexV5 (TxLaneMuxSelectorArray, CurrentCoreLane)] = TxLaneMuxSelectorArray[CurrentPifLane];
            TxLaneMuxSelectorArray[CurrentPifLane] = CurrentCoreLane;
          }

          if (RxLaneMuxSelectorArray[CurrentCoreLane] != CurrentPifLane) {
            RxLaneMuxSelectorArray[PcieTopologyLocateMuxIndexV5 (RxLaneMuxSelectorArray, CurrentPifLane)] = RxLaneMuxSelectorArray[CurrentCoreLane];
            RxLaneMuxSelectorArray[CurrentCoreLane] = CurrentPifLane;
          }

          CoreLaneBitmap &= (~ (1 << CurrentCoreLane));
          PifLaneBitmap &= (~ (1 << CurrentPifLane));
        }
      }
      EngineList = PcieLibGetNextDescriptor (EngineList);
    }
    for (Index = 0; Index < 4; ++Index) {
      PcieRegisterWrite (
        Wrapper,
        WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_8021_ADDRESS + Index),
        ((UINT32 *) TxLaneMuxSelectorArray) [Index],
        FALSE,
        Pcie
        );
      PcieRegisterWrite (
        Wrapper,
        WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_8025_ADDRESS + Index),
        ((UINT32 *) RxLaneMuxSelectorArray) [Index],
        FALSE,
        Pcie
        );
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieTopologyApplyLaneMuxV5 Exit\n");
}


/*----------------------------------------------------------------------------------------*/
/**
 * Check if lane topology supports Gen3
 *
 *  Check if more that one link mapped to single PIF slice
 *
 * @param[in]  Wrapper             Pointer to wrapper config descriptor
 */

BOOLEAN
PcieTopologyIsGen3SupportedV5 (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper
  )
{
  UINT8                 LaneNibbleArray [4];
  UINT32                LaneBitmap;
  UINT8                 Nibble;
  UINT8                 NibbleBitmap;
  PCIe_ENGINE_CONFIG    *Engine;


  LibAmdMemFill (&LaneNibbleArray[0], 0x00, sizeof (LaneNibbleArray), PcieConfigGetStdHeader (Wrapper));
  Engine = PcieConfigGetChildEngine (Wrapper);
  while (Engine != NULL) {
    LaneBitmap = PcieUtilGetEngineLaneBitMap (LANE_TYPE_PCIE_PHY_NATIVE, 0, Engine);
    for (Nibble = 0; Nibble < 4; Nibble++) {
      NibbleBitmap = (0xF << (Nibble * 4));
      if ((LaneBitmap & NibbleBitmap) != 0) {
        if (++LaneNibbleArray [Nibble] > 1) {
          return FALSE;
        }
      }
    }
    Engine = PcieLibGetNextDescriptor (Engine);
  }
  return TRUE;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Execute/clean up reconfiguration
 *
 *
 * @param[in]  Wrapper             Pointer to wrapper config descriptor
 * @param[in]  Pcie                Pointer to global PCIe configuration
 */
VOID
PcieTopologyExecuteReconfigV5 (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  D0F0xE4_WRAP_8062_STRUCT  D0F0xE4_WRAP_8062;
  PCIe_SILICON_CONFIG       *Silicon;
  DEV_OBJECT                DevObject;

  if (PcieLibIsPcieWrapper (Wrapper)) {
    IDS_HDT_CONSOLE (GNB_TRACE, "PcieTopologyExecuteReconfigV5 Enter\n");

    D0F0xE4_WRAP_8062.Value = PcieRegisterRead (
                                Wrapper,
                                WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_8062_ADDRESS),
                                Pcie
                                );
    D0F0xE4_WRAP_8062.Field.ReconfigureEn = 0x1;
    D0F0xE4_WRAP_8062.Field.ResetPeriod = 0x0;
    PcieRegisterWrite (
      Wrapper,
      WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_8062_ADDRESS),
      D0F0xE4_WRAP_8062.Value,
      FALSE,
      Pcie
      );

    Silicon = PcieConfigGetParentSilicon (Wrapper);
    DevObject.StdHeader = GnbLibGetHeader (Pcie);
    DevObject.GnbHandle = GnbGetHandle (GnbLibGetHeader (Pcie));
    DevObject.DevPciAddress.AddressValue = Silicon->Address.AddressValue;
    GnbSmuServiceRequestV7 (
      &DevObject,
      SMC_MSG_RECONFIGURE,
      Wrapper->WrapId,
      0
      );

    D0F0xE4_WRAP_8062.Field.ConfigXferMode = 0x1;
    D0F0xE4_WRAP_8062.Field.ReconfigureEn = 0x0;
    PcieRegisterWrite (
      Wrapper,
      WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_8062_ADDRESS),
      D0F0xE4_WRAP_8062.Value,
      FALSE,
      Pcie
      );

    IDS_HDT_CONSOLE (GNB_TRACE, "PcieTopologyExecuteReconfigV5 Exit\n");
  }
}
