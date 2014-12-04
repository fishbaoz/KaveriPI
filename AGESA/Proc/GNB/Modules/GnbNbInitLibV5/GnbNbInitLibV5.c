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
#include  "heapManager.h"
#include  "Gnb.h"
#include  "GnbPcieConfig.h"
#include  "GnbCommonLib.h"
#include  "Filecode.h"
#include  "GnbRegistersCommonV2.h"
#include  "GnbFuseTable.h"
#include  "OptionGnb.h"
#include  "GnbFamServices.h"
#define FILECODE PROC_GNB_MODULES_GNBNBINITLIBV5_GNBNBINITLIBV5_FILECODE
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

/*----------------------------------------------------------------------------------------*/
/**
 * Init NB set top of memory
 *
 *
 *
 * @param[in] NbPciAddress    Gnb PCI address
 * @param[in] StdHeader       Standard Configuration Header
 */

AGESA_STATUS
GnbSetTomV5 (
  IN      PCI_ADDR            NbPciAddress,
  IN      AMD_CONFIG_PARAMS   *StdHeader
  )
{
  AGESA_STATUS  Status;
  UINT64        MsrData;
  UINT64        GnbTom2;
  UINT64        GnbTom3;
  UINT32        Value;

  Status = AGESA_SUCCESS;
  //Read memory size below 4G from MSR C001_001A
  LibAmdMsrRead (TOP_MEM, &MsrData, StdHeader);
  //Write to NB register 0x90
  Value = (UINT32)MsrData & 0xFF800000;     //Keep bits 31:23
  GnbLibPciRMW (
    NbPciAddress.AddressValue | D0F0x90_ADDRESS,
    AccessS3SaveWidth32,
    0x007FFFFF,
    Value,
    StdHeader
    );
  if (Value == 0) {
    Status = AGESA_WARNING;
  }

  GnbTom2 = 0;
  GnbTom3 = 0;
  LibAmdMsrRead (SYS_CFG, &MsrData, StdHeader);
  if ((MsrData & BIT21) != 0) {
    //If SYS_CFG(MtrrTom2En) then configure GNB TOM2 and TOM3
    //Read memory size above 4G from TOP_MEM2 (MSR C001_001D)
    LibAmdMsrRead (TOP_MEM2, &MsrData, StdHeader);
    if ((MsrData & (UINT64)0x0000FFFFFFC00000) > ((UINT64)0x0000010000000000)) {
      // If TOP_MEM2 is above 1TB, enable GNB TOM2 and TOM3
      // Set TOM2 for below 1TB limit
      GnbTom2 = 0x000000FD00000000;
      // TOM3 is INCLUSIVE, so set it to TOM - 1 using bits 47:22
      GnbTom3 = (MsrData - 1) & (UINT64)0x0000FFFFFFC00000;
    } else {
      // If TOP_MEM2 is below 1TB, set TOM2 using bits 39:22
      GnbTom2 = MsrData & (UINT64)0x000000FFFFC00000;       //Keep bits 39:22
      // If TOP_MEM2 is below 1TB, disable GNB TOM3
      GnbTom3 = 0;
    }
  }

  if (GnbTom2 != 0) {
    // Write memory size[39:32] to indirect register 1A[7:0]
    Value = (UINT32) ((GnbTom2 >> 32) & 0xFF);
    GnbLibPciIndirectRMW (
      NbPciAddress.AddressValue | D0F0x60_ADDRESS,
      D0F0x64_x1A_ADDRESS | IOC_WRITE_ENABLE,
      AccessS3SaveWidth32,
      0xFFFFFF00,
      Value,
      StdHeader
    );
    // Write memory size[31:23] to indirect register 19[31:23] and enable memory through bit 0
    Value = (UINT32)GnbTom2 & 0xFF800000;      //Keep bits 31:23
    Value |= BIT0;                            // Enable top of memory
    GnbLibPciIndirectRMW (
      NbPciAddress.AddressValue | D0F0x60_ADDRESS,
      D0F0x64_x19_ADDRESS | IOC_WRITE_ENABLE,
      AccessS3SaveWidth32,
      0x007FFFFF,
      Value,
      StdHeader
    );
  }

  if (GnbTom3 != 0) {
    // Above 1TB addressing TOM3 if MSR TOM is above 1TB
    // Write memory size[51:22] to indirect register 4E[29:0] and enable memory through bit 31
    Value = (UINT32) (GnbTom3 >> 22);
    Value |= BIT31;
    GnbLibPciIndirectRMW (
      NbPciAddress.AddressValue | D0F0x60_ADDRESS,
      D0F0x64_x4E_ADDRESS | IOC_WRITE_ENABLE,
      AccessS3SaveWidth32,
      0xBFFFFFFF,
      Value,
      StdHeader
    );
  }
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get the index of highest SCLK VID
 *
 * @param[in] StdHeader       Standard configuration header
 * @retval                    NBVDD VID index
 */
UINT8
GnbLocateHighestVidIndexV5 (
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT8             MaxVid;
  UINT8             MaxVidIndex;
  UINTN             Index;
  PP_FUSE_ARRAY_V2     *PpFuseArray;

  PpFuseArray = (PP_FUSE_ARRAY_V2 *) GnbLocateHeapBuffer (AMD_PP_FUSE_TABLE_HANDLE, StdHeader);
  ASSERT (PpFuseArray != NULL);
  if (PpFuseArray == NULL) {
    IDS_HDT_CONSOLE (GNB_TRACE, "  ERROR!!! Heap Location\n");
    return 0;
  }

  MaxVidIndex = 0;
  MaxVid = 0xff;
  for (Index = 0; Index < 5; Index++) {
    if (PpFuseArray->VddNbVid[Index] != 0 && PpFuseArray->VddNbVid[Index] < MaxVid) {
      MaxVid = PpFuseArray->VddNbVid[Index];
      MaxVidIndex = (UINT8) Index;
    }
  }
  ASSERT (PpFuseArray->VddNbVid[MaxVidIndex] != 0);
  return MaxVidIndex;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get the index of lowest SCLK VID
 *
 * @param[in] StdHeader       Standard configuration header
 * @retval                    NBVDD VID index
 */
UINT8
GnbLocateLowestVidIndexV5 (
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT8             MinVidIndex;
  UINTN             Index;
  PP_FUSE_ARRAY_V2     *PpFuseArray;

  PpFuseArray = (PP_FUSE_ARRAY_V2 *) GnbLocateHeapBuffer (AMD_PP_FUSE_TABLE_HANDLE, StdHeader);
  ASSERT (PpFuseArray != NULL);
  if (PpFuseArray == NULL) {
    IDS_HDT_CONSOLE (GNB_TRACE, "  ERROR!!! Heap Location\n");
    return 0;
  }

  MinVidIndex = 0;

  for (Index = 0; Index < 5; Index++) {
    if (PpFuseArray->VddNbVid[Index] > PpFuseArray->VddNbVid[MinVidIndex]) {
      MinVidIndex = (UINT8) Index;
    }
  }
  ASSERT (PpFuseArray->VddNbVid[MinVidIndex] != 0);
  return MinVidIndex;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get the highest SCLK VID (high voltage)
 *
 * @param[in] StdHeader       Standard configuration header
 * @retval                    NBVDD VID
 */
UINT8
GnbLocateHighestVidCodeV5 (
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT8                MaxVidIndex;
  PP_FUSE_ARRAY_V2     *PpFuseArray;

  PpFuseArray = (PP_FUSE_ARRAY_V2 *) GnbLocateHeapBuffer (AMD_PP_FUSE_TABLE_HANDLE, StdHeader);
  ASSERT (PpFuseArray != NULL);

  MaxVidIndex = GnbLocateHighestVidIndexV5 (StdHeader);
  ASSERT (PpFuseArray->VddNbVid[MaxVidIndex] != 0);
  return PpFuseArray->VddNbVid[MaxVidIndex];

}

/*----------------------------------------------------------------------------------------*/
/**
 * Get the lowest SCLK VID (low voltage)
 *
 * @param[in] StdHeader       Standard configuration header
 * @retval                    NBVDD VID
 */
UINT8
GnbLocateLowestVidCodeV5 (
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT8                MinVidIndex;
  PP_FUSE_ARRAY_V2     *PpFuseArray;

  PpFuseArray = (PP_FUSE_ARRAY_V2 *) GnbLocateHeapBuffer (AMD_PP_FUSE_TABLE_HANDLE, StdHeader);
  ASSERT (PpFuseArray != NULL);
  MinVidIndex = GnbLocateLowestVidIndexV5 (StdHeader);
  ASSERT (PpFuseArray->VddNbVid[MinVidIndex] != 0);
  return PpFuseArray->VddNbVid[MinVidIndex];
}

/*----------------------------------------------------------------------------------------*/
/**
 * Translate VID code to millivolt with two fraction bits
 *
 *
 * @param[in] Vid             VID code
 * @param[in] StdHeader       Standard configuration header
 * @retval    100 millivolt with two fraction bits
 */

UINT32
GnbTranslateVidCodeToMillivoltV5 (
  IN       UINT8                 Vid,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  UINT32       Millivolt;

  // According to SVI2 spec, Vid Code 0xF7 is 6.25mv. 6.25mv is one boundary value.
  if (Vid > 0xF7) {
    Vid = 0xF7;
    ASSERT (FALSE);
  }

  // Equation: VID code increase/decrease 1, there is one 6.25mv change.
  //           6.25mv could be described using integer 625 with two fraction bits.
  Millivolt = (0xF7 - Vid + 1) * 625;

  return Millivolt;
}

/*----------------------------------------------------------------------------------------*/
/**
 * NB Dynamic Wake
 * ORB_CNB_Wake signal is used to inform the CNB NCLK controller and GNB LCLK controller
 *   that ORB is (or will soon) push data into the synchronizer FIFO (i.e. wake is high).
 *
 * @param[in] NbPciAddress    Gnb PCI address
 * @param[in] StdHeader       Standard Configuration Header
 */

VOID
GnbOrbDynamicWakeV5 (
  IN      PCI_ADDR            NbPciAddress,
  IN      AMD_CONFIG_PARAMS   *StdHeader
  )
{

  D0F0x98_x2C_STRUCT D0F0x98_x2C;
  GNB_BUILD_OPTIONS_COMMON  *GnbCommonOptions;

  GnbLibPciIndirectRead (
    NbPciAddress.AddressValue | D0F0x94_ADDRESS,
    D0F0x98_x2C_ADDRESS,
    AccessWidth32,
    &D0F0x98_x2C.Value,
    StdHeader
    );

  // Enable Dynamic wake
  // Wake Hysteresis timer value.  Specifies the number of SMU pulses to count.
  GnbCommonOptions = (GNB_BUILD_OPTIONS_COMMON*) GnbFmGnbBuildOptions (StdHeader);
  if (GnbCommonOptions->CfgOrbDynWakeEnable) {
    D0F0x98_x2C.Field.DynWakeEn = 1;
  } else {
    D0F0x98_x2C.Field.DynWakeEn = 0;
  }
  D0F0x98_x2C.Field.WakeHysteresis = 0x19;

  IDS_OPTION_HOOK (IDS_GNB_ORBDYNAMIC_WAKE, &D0F0x98_x2C, StdHeader);

  GnbLibPciIndirectWrite (
    NbPciAddress.AddressValue | D0F0x94_ADDRESS,
    D0F0x98_x2C_ADDRESS,
    AccessS3SaveWidth32,
    &D0F0x98_x2C.Value,
    StdHeader
    );
}