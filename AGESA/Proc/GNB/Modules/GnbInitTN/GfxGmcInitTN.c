/* $NoKeywords:$ */
/**
 * @file
 *
 * Graphics Memory Controller Initialization.
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
#include  "Gnb.h"
#include  "GnbGfx.h"
#include  "GnbCommonLib.h"
#include  "GnbTable.h"
#include  "GnbPcieConfig.h"
#include  "GnbRegisterAccTN.h"
#include  "cpuFamilyTranslation.h"
#include  "GnbRegistersTN.h"
#include  "GfxLibTN.h"
#include  "GfxGmcInitTN.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITTN_GFXGMCINITTN_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

extern GNB_TABLE ROMDATA GfxGmcClockGatingDisableTN [];
extern GNB_TABLE ROMDATA GfxGmcInitTableTN [];
extern GNB_TABLE ROMDATA GfxGmcClockGatingEnableTN [];


#define GNB_GFX_DRAM_CH_0_PRESENT 1
#define GNB_GFX_DRAM_CH_1_PRESENT 2

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

DCT_REGISTER_ENTRY DctRegisterTable [] = {
  {
    TYPE_D18F2_dct0,
    D18F2x94_dct0_ADDRESS,
    (UINT16) offsetof (DCT_CHANNEL_INFO, D18F2x94_dct0)
  },
  {
    TYPE_D18F2_dct1,
    D18F2x94_dct1_ADDRESS,
    (UINT16) offsetof (DCT_CHANNEL_INFO, D18F2x94_dct1)
  },
  {
    TYPE_D18F2_dct0,
    D18F2x2E0_dct0_ADDRESS,
    (UINT16) offsetof (DCT_CHANNEL_INFO, D18F2x2E0_dct0)
  },
  {
    TYPE_D18F2_dct1,
    D18F2x2E0_dct1_ADDRESS,
    (UINT16) offsetof (DCT_CHANNEL_INFO, D18F2x2E0_dct1)
  },
  {
    TYPE_D18F2_dct0_mp0,
    D18F2x200_dct0_mp0_ADDRESS,
    (UINT16) offsetof (DCT_CHANNEL_INFO, D18F2x200_dct0_mp0)
  },
  {
    TYPE_D18F2_dct0_mp1,
    D18F2x200_dct0_mp1_ADDRESS,
    (UINT16) offsetof (DCT_CHANNEL_INFO, D18F2x200_dct0_mp1)
  },
  {
    TYPE_D18F2_dct1_mp0,
    D18F2x200_dct1_mp0_ADDRESS,
    (UINT16) offsetof (DCT_CHANNEL_INFO, D18F2x200_dct1_mp0)
  },
  {
    TYPE_D18F2_dct1_mp1,
    D18F2x200_dct1_mp1_ADDRESS,
    (UINT16) offsetof (DCT_CHANNEL_INFO, D18F2x200_dct1_mp1)
  },
  {
    TYPE_D18F2_dct0_mp0,
    D18F2x204_dct0_mp0_ADDRESS,
    (UINT16) offsetof (DCT_CHANNEL_INFO, D18F2x204_dct0_mp0)
  },
  {
    TYPE_D18F2_dct0_mp1,
    D18F2x204_dct0_mp1_ADDRESS,
    (UINT16) offsetof (DCT_CHANNEL_INFO, D18F2x204_dct0_mp1)
  },
  {
    TYPE_D18F2_dct1_mp0,
    D18F2x204_dct1_mp0_ADDRESS,
    (UINT16) offsetof (DCT_CHANNEL_INFO, D18F2x204_dct1_mp0)
  },
  {
    TYPE_D18F2_dct1_mp1,
    D18F2x204_dct1_mp1_ADDRESS,
    (UINT16) offsetof (DCT_CHANNEL_INFO, D18F2x204_dct1_mp1)
  },
  {
    TYPE_D18F2_dct0_mp0,
    D18F2x22C_dct0_mp0_ADDRESS,
    (UINT16) offsetof (DCT_CHANNEL_INFO, D18F2x22C_dct0_mp0)
  },
  {
    TYPE_D18F2_dct0_mp1,
    D18F2x22C_dct0_mp1_ADDRESS,
    (UINT16) offsetof (DCT_CHANNEL_INFO, D18F2x22C_dct0_mp1)
  },
  {
    TYPE_D18F2_dct1_mp0,
    D18F2x22C_dct1_mp0_ADDRESS,
    (UINT16) offsetof (DCT_CHANNEL_INFO, D18F2x22C_dct1_mp0)
  },
  {
    TYPE_D18F2_dct1_mp1,
    D18F2x22C_dct1_mp1_ADDRESS,
    (UINT16) offsetof (DCT_CHANNEL_INFO, D18F2x22C_dct1_mp1)
  },
  {
    TYPE_D18F2_dct0_mp0,
    D18F2x21C_dct0_mp0_ADDRESS,
    (UINT16) offsetof (DCT_CHANNEL_INFO, D18F2x21C_dct0_mp0)
  },
  {
    TYPE_D18F2_dct0_mp1,
    D18F2x21C_dct0_mp1_ADDRESS,
    (UINT16) offsetof (DCT_CHANNEL_INFO, D18F2x21C_dct0_mp1)
  },
  {
    TYPE_D18F2_dct1_mp0,
    D18F2x21C_dct1_mp0_ADDRESS,
    (UINT16) offsetof (DCT_CHANNEL_INFO, D18F2x21C_dct1_mp0)
  },
  {
    TYPE_D18F2_dct1_mp1,
    D18F2x21C_dct1_mp1_ADDRESS,
    (UINT16) offsetof (DCT_CHANNEL_INFO, D18F2x21C_dct1_mp1)
  },
  {
    TYPE_D18F2_dct0_mp0,
    D18F2x20C_dct0_mp0_ADDRESS,
    (UINT16) offsetof (DCT_CHANNEL_INFO, D18F2x20C_dct0_mp0)
  },
  {
    TYPE_D18F2_dct0_mp1,
    D18F2x20C_dct0_mp1_ADDRESS,
    (UINT16) offsetof (DCT_CHANNEL_INFO, D18F2x20C_dct0_mp1)
  },
  {
    TYPE_D18F2_dct1_mp0,
    D18F2x20C_dct1_mp0_ADDRESS,
    (UINT16) offsetof (DCT_CHANNEL_INFO, D18F2x20C_dct1_mp0)
  },
  {
    TYPE_D18F2_dct1_mp1,
    D18F2x20C_dct1_mp1_ADDRESS,
    (UINT16) offsetof (DCT_CHANNEL_INFO, D18F2x20C_dct1_mp1)
  }
};

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 * Initialize Fb location
 *
 *
 *
 * @param[in]   Gfx               Pointer to global GFX configuration
 *
 */
VOID
GfxGmcInitializeFbLocationTN (
  IN      GFX_PLATFORM_CONFIG   *Gfx
  )
{
  GMMx2024_STRUCT   GMMx2024;
  GMMx2068_STRUCT   GMMx2068;
  GMMx2C04_STRUCT   GMMx2C04;
  GMMx5428_STRUCT   GMMx5428;
  UINT64            FBBase;
  UINT64            FBTop;
  FBBase = 0x0F00000000;
  FBTop = FBBase + Gfx->UmaInfo.UmaSize - 1;
  GMMx2024.Value = 0;
  GMMx2C04.Value = 0;
  GMMx2024.Field.FB_BASE = (UINT16) (FBBase >> 24);
  GMMx2024.Field.FB_TOP = (UINT16) (FBTop >> 24);
  GMMx2068.Field.FB_OFFSET = (UINT32) (Gfx->UmaInfo.UmaBase >> 22);
  GMMx2C04.Field.NONSURF_BASE = (UINT32) (FBBase >> 8);
  GMMx5428.Field.CONFIG_MEMSIZE = Gfx->UmaInfo.UmaSize >> 20;
  GnbRegisterWriteTN (GMMx2024_TYPE, GMMx2024_ADDRESS, &GMMx2024.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  GnbRegisterWriteTN (GMMx2068_TYPE, GMMx2068_ADDRESS, &GMMx2068.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  GnbRegisterWriteTN (GMMx2C04_TYPE, GMMx2C04_ADDRESS, &GMMx2C04.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  GnbRegisterWriteTN (GMMx5428_TYPE, GMMx5428_ADDRESS, &GMMx5428.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get Sequencer model info
 *
 *
 * @param[out] DctChannelInfo  Various DCT/GMM info
 * @param[in]  Gfx             Pointer to global GFX configuration
 */

VOID
GfxGmcDctMemoryChannelInfoTN (
     OUT   DCT_CHANNEL_INFO              *DctChannelInfo,
  IN       GFX_PLATFORM_CONFIG           *Gfx
  )
{

  UINT32       Index;
  UINT32       Value;

  for (Index = 0; Index < (sizeof (DctRegisterTable) / sizeof (DCT_REGISTER_ENTRY)); Index++) {
    GnbRegisterReadTN (
      DctRegisterTable[Index].RegisterSpaceType,
      DctRegisterTable[Index].Address,
      &Value,
      0,
      GnbLibGetHeader (Gfx)
    );
    *(UINT32 *)((UINT8 *) DctChannelInfo + DctRegisterTable[Index].DctChannelInfoTableOffset) = Value;
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Initialize sequencer model
 *
 *
 *
 * @param[in]   Gfx               Pointer to global GFX configuration
 *
 */
VOID
GfxGmcInitializeSequencerTN (
  IN      GFX_PLATFORM_CONFIG   *Gfx
  )
{

  UINT32                    memps0_freq;
  UINT32                    memps1_freq;
  UINT32                    scale_mp0;
  UINT32                    scale_mp1;
  UINT8                     DramChannelPresent;
  GMMx2774_STRUCT           GMMx2774;
  GMMx2778_STRUCT           GMMx2778;
  GMMx27F0_STRUCT           GMMx27F0;
  GMMx27FC_STRUCT           GMMx27FC;
  GMMx2808_STRUCT           GMMx2808;
  DCT_CHANNEL_INFO          DctChannel;
  D18F5x170_STRUCT          D18F5x170;
  GMMx2004_STRUCT           GMMx2004;
  GMMx2214_STRUCT           GMMx2214;

  GfxGmcDctMemoryChannelInfoTN (&DctChannel, Gfx);

  DramChannelPresent = 0;
  if (!DctChannel.D18F2x94_dct1.Field.DisDramInterface) {
    DramChannelPresent |= GNB_GFX_DRAM_CH_1_PRESENT;
  }

  if (!DctChannel.D18F2x94_dct0.Field.DisDramInterface) {
    //if (channel 0 present)
    //memps0_freq = extract frequency from DRAM Configuration High <D18F2x094_dct[0]>[4:0] encoding
    //memps1_freq = extract frequency from Memory P-state Control Status <D18F2x2E0_dct[0]>[28:24] encoding
    DramChannelPresent |= GNB_GFX_DRAM_CH_0_PRESENT;
    memps0_freq = GfxLibExtractDramFrequency ((UINT8) DctChannel.D18F2x94_dct0.Field.MemClkFreq, GnbLibGetHeader (Gfx));
    memps1_freq = GfxLibExtractDramFrequency ((UINT8) DctChannel.D18F2x2E0_dct0.Field.M1MemClkFreq, GnbLibGetHeader (Gfx));
  } else {
    //memps0_freq = extract frequency from DRAM Configuration High <D18F2x094_dct[1]>[4:0] encoding
    //memps1_freq = extract frequency from Memory P-state Control Status <D18F2x2E0_dct[1]>[28:24] encoding
    memps0_freq = GfxLibExtractDramFrequency ((UINT8) DctChannel.D18F2x94_dct1.Field.MemClkFreq, GnbLibGetHeader (Gfx));
    memps1_freq = GfxLibExtractDramFrequency ((UINT8) DctChannel.D18F2x2E0_dct1.Field.M1MemClkFreq, GnbLibGetHeader (Gfx));
  }

  GnbRegisterReadTN (D18F5x170_TYPE, D18F5x170_ADDRESS, &D18F5x170.Value, 0, GnbLibGetHeader (Gfx));
  if (D18F5x170.Field.MemPstateDis == 1) {
    memps1_freq = memps0_freq;
  }

  //scale_mp0 = sclk_max_freq / memps0_freq
  //scale_mp1 = sclk_max_freq / memps1_freq
  //Multiply it by 100 to avoid dealing with floating point values
  scale_mp0 = (GfxLibGetMaxSclk (GnbLibGetHeader (Gfx)) * 100) / memps0_freq;
  scale_mp1 = (GfxLibGetMaxSclk (GnbLibGetHeader (Gfx)) * 100) / memps1_freq;

  GnbRegisterReadTN (GMMx2774_TYPE, GMMx2774_ADDRESS, &GMMx2774.Value, 0, GnbLibGetHeader (Gfx));
  GnbRegisterReadTN (GMMx2778_TYPE, GMMx2778_ADDRESS, &GMMx2778.Value, 0, GnbLibGetHeader (Gfx));
  GnbRegisterReadTN (GMMx27F0_TYPE, GMMx27F0_ADDRESS, &GMMx27F0.Value, 0, GnbLibGetHeader (Gfx));
  GnbRegisterReadTN (GMMx27FC_TYPE, GMMx27FC_ADDRESS, &GMMx27FC.Value, 0, GnbLibGetHeader (Gfx));

  if (((DramChannelPresent & GNB_GFX_DRAM_CH_0_PRESENT) != 0) && ((DramChannelPresent & GNB_GFX_DRAM_CH_1_PRESENT) != 0)) {
    GMMx2774.Field.ACTRD = (MIN (DctChannel.D18F2x200_dct0_mp0.Field.Trcd, DctChannel.D18F2x200_dct1_mp0.Field.Trcd) * scale_mp0) / 100;
    GMMx2774.Field.ACTWR = GMMx2774.Field.ACTRD;
    GMMx2774.Field.RASMACTRD = (MIN ((DctChannel.D18F2x204_dct0_mp0.Field.Trc - DctChannel.D18F2x200_dct0_mp0.Field.Trcd),
                                     (DctChannel.D18F2x204_dct1_mp0.Field.Trc - DctChannel.D18F2x200_dct1_mp0.Field.Trcd)) * scale_mp0) / 100;
    GMMx2774.Field.RASMACTWR = GMMx2774.Field.RASMACTRD;

    GMMx2778.Field.RAS2RAS = (MIN (DctChannel.D18F2x204_dct0_mp0.Field.Trc, DctChannel.D18F2x204_dct1_mp0.Field.Trc) * scale_mp0) / 100;
    GMMx2778.Field.RP = (MIN (DctChannel.D18F2x200_dct0_mp0.Field.Trp, DctChannel.D18F2x200_dct1_mp0.Field.Trp) * scale_mp0) / 100;
    GMMx2778.Field.WRPLUSRP = (MIN ((DctChannel.D18F2x22C_dct0_mp0.Field.Twr + DctChannel.D18F2x200_dct0_mp0.Field.Trp),
                                    (DctChannel.D18F2x22C_dct1_mp0.Field.Twr + DctChannel.D18F2x200_dct1_mp0.Field.Trp)) * scale_mp0) / 100;
    GMMx2778.Field.BUS_TURN = ((MIN ((DctChannel.D18F2x20C_dct0_mp0.Field.Tcwl + 4 + DctChannel.D18F2x20C_dct0_mp0.Field.Twtr + DctChannel.D18F2x21C_dct0_mp0.Field.TrwtTO),
                                     (DctChannel.D18F2x20C_dct1_mp0.Field.Tcwl + 4 + DctChannel.D18F2x20C_dct1_mp0.Field.Twtr + DctChannel.D18F2x21C_dct1_mp0.Field.TrwtTO)) / 2) * scale_mp0) / 100;

    GMMx27F0.Field.ACTRD = (MIN (DctChannel.D18F2x200_dct0_mp1.Field.Trcd, DctChannel.D18F2x200_dct1_mp1.Field.Trcd) * scale_mp1) / 100;
    GMMx27F0.Field.ACTWR = GMMx27F0.Field.ACTRD;
    GMMx27F0.Field.RASMACTRD = (MIN ((DctChannel.D18F2x204_dct0_mp1.Field.Trc - DctChannel.D18F2x200_dct0_mp1.Field.Trcd),
                                     (DctChannel.D18F2x204_dct1_mp1.Field.Trc - DctChannel.D18F2x200_dct1_mp1.Field.Trcd)) * scale_mp1) / 100;
    GMMx27F0.Field.RASMACTWR = GMMx27F0.Field.RASMACTRD;

    GMMx27FC.Field.RAS2RAS = (MIN (DctChannel.D18F2x204_dct0_mp1.Field.Trc, DctChannel.D18F2x204_dct1_mp1.Field.Trc) * scale_mp1) / 100;
    GMMx27FC.Field.RP = (MIN (DctChannel.D18F2x200_dct0_mp1.Field.Trp, DctChannel.D18F2x200_dct1_mp1.Field.Trp) * scale_mp1) / 100;
    GMMx27FC.Field.WRPLUSRP = (MIN ((DctChannel.D18F2x22C_dct0_mp1.Field.Twr + DctChannel.D18F2x200_dct0_mp1.Field.Trp),
                                    (DctChannel.D18F2x22C_dct1_mp1.Field.Twr + DctChannel.D18F2x200_dct1_mp1.Field.Trp)) * scale_mp1) / 100;
    GMMx27FC.Field.BUS_TURN = ((MIN ((DctChannel.D18F2x20C_dct0_mp1.Field.Tcwl + 4 + DctChannel.D18F2x20C_dct0_mp1.Field.Twtr + DctChannel.D18F2x21C_dct0_mp1.Field.TrwtTO),
                                     (DctChannel.D18F2x20C_dct1_mp1.Field.Tcwl + 4 + DctChannel.D18F2x20C_dct1_mp1.Field.Twtr + DctChannel.D18F2x21C_dct1_mp1.Field.TrwtTO)) / 2) * scale_mp1) / 100;

  } else if ((DramChannelPresent & GNB_GFX_DRAM_CH_0_PRESENT) != 0) {
    GMMx2774.Field.ACTRD = (DctChannel.D18F2x200_dct0_mp0.Field.Trcd * scale_mp0) / 100;
    GMMx2774.Field.ACTWR = GMMx2774.Field.ACTRD;
    GMMx2774.Field.RASMACTRD = ((DctChannel.D18F2x204_dct0_mp0.Field.Trc - DctChannel.D18F2x200_dct0_mp0.Field.Trcd) * scale_mp0) / 100;
    GMMx2774.Field.RASMACTWR = GMMx2774.Field.RASMACTRD;

    GMMx2778.Field.RAS2RAS = (DctChannel.D18F2x204_dct0_mp0.Field.Trc * scale_mp0) / 100;
    GMMx2778.Field.RP = (DctChannel.D18F2x200_dct0_mp0.Field.Trp * scale_mp0) / 100;
    GMMx2778.Field.WRPLUSRP = ((DctChannel.D18F2x22C_dct0_mp0.Field.Twr + DctChannel.D18F2x200_dct0_mp0.Field.Trp) * scale_mp0) / 100;
    GMMx2778.Field.BUS_TURN = (((DctChannel.D18F2x20C_dct0_mp0.Field.Tcwl + 4 + DctChannel.D18F2x20C_dct0_mp0.Field.Twtr + DctChannel.D18F2x21C_dct0_mp0.Field.TrwtTO) / 2) * scale_mp0) / 100;

    GMMx27F0.Field.ACTRD = (DctChannel.D18F2x200_dct0_mp1.Field.Trcd * scale_mp1) / 100;
    GMMx27F0.Field.ACTWR = GMMx27F0.Field.ACTRD;
    GMMx27F0.Field.RASMACTRD = ((DctChannel.D18F2x204_dct0_mp1.Field.Trc - DctChannel.D18F2x200_dct0_mp1.Field.Trcd) * scale_mp1) / 100;
    GMMx27F0.Field.RASMACTWR = GMMx27F0.Field.RASMACTRD;

    GMMx27FC.Field.RAS2RAS = (DctChannel.D18F2x204_dct0_mp1.Field.Trc * scale_mp1) / 100;
    GMMx27FC.Field.RP = (DctChannel.D18F2x200_dct0_mp1.Field.Trp * scale_mp1) / 100;
    GMMx27FC.Field.WRPLUSRP = ((DctChannel.D18F2x22C_dct0_mp1.Field.Twr + DctChannel.D18F2x200_dct0_mp1.Field.Trp) * scale_mp1) / 100;
    GMMx27FC.Field.BUS_TURN = (((DctChannel.D18F2x20C_dct0_mp1.Field.Tcwl + 4 + DctChannel.D18F2x20C_dct0_mp1.Field.Twtr + DctChannel.D18F2x21C_dct0_mp1.Field.TrwtTO) / 2) * scale_mp1) / 100;

  } else {
    GMMx2774.Field.ACTRD = (DctChannel.D18F2x200_dct1_mp0.Field.Trcd * scale_mp0) / 100;
    GMMx2774.Field.ACTWR = GMMx2774.Field.ACTRD;
    GMMx2774.Field.RASMACTRD = ((DctChannel.D18F2x204_dct1_mp0.Field.Trc - DctChannel.D18F2x200_dct1_mp0.Field.Trcd) * scale_mp0) / 100;
    GMMx2774.Field.RASMACTWR = GMMx2774.Field.RASMACTRD;

    GMMx2778.Field.RAS2RAS = (DctChannel.D18F2x204_dct1_mp0.Field.Trc * scale_mp0) / 100;
    GMMx2778.Field.RP = (DctChannel.D18F2x200_dct1_mp0.Field.Trp * scale_mp0) / 100;
    GMMx2778.Field.WRPLUSRP = ((DctChannel.D18F2x22C_dct1_mp0.Field.Twr + DctChannel.D18F2x200_dct1_mp0.Field.Trp) * scale_mp0) / 100;
    GMMx2778.Field.BUS_TURN = (((DctChannel.D18F2x20C_dct1_mp0.Field.Tcwl + 4 + DctChannel.D18F2x20C_dct1_mp0.Field.Twtr + DctChannel.D18F2x21C_dct1_mp0.Field.TrwtTO) / 2) * scale_mp0) / 100;

    GMMx27F0.Field.ACTRD = (DctChannel.D18F2x200_dct1_mp1.Field.Trcd * scale_mp1) / 100;
    GMMx27F0.Field.ACTWR = GMMx27F0.Field.ACTRD;
    GMMx27F0.Field.RASMACTRD = ((DctChannel.D18F2x204_dct1_mp1.Field.Trc - DctChannel.D18F2x200_dct1_mp1.Field.Trcd) * scale_mp1) / 100;
    GMMx27F0.Field.RASMACTWR = GMMx27F0.Field.RASMACTRD;

    GMMx27FC.Field.RAS2RAS = (DctChannel.D18F2x204_dct1_mp1.Field.Trc * scale_mp1) / 100;
    GMMx27FC.Field.RP = (DctChannel.D18F2x200_dct1_mp1.Field.Trp * scale_mp1) / 100;
    GMMx27FC.Field.WRPLUSRP = ((DctChannel.D18F2x22C_dct1_mp1.Field.Twr + DctChannel.D18F2x200_dct1_mp1.Field.Trp) * scale_mp1) / 100;
    GMMx27FC.Field.BUS_TURN = (((DctChannel.D18F2x20C_dct1_mp1.Field.Tcwl + 4 + DctChannel.D18F2x20C_dct1_mp1.Field.Twtr + DctChannel.D18F2x21C_dct1_mp1.Field.TrwtTO) / 2) * scale_mp1) / 100;
  }

  GnbRegisterWriteTN (GMMx2774_TYPE, GMMx2774_ADDRESS, &GMMx2774.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  GnbRegisterWriteTN (GMMx2778_TYPE, GMMx2778_ADDRESS, &GMMx2778.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  GnbRegisterWriteTN (GMMx27F0_TYPE, GMMx27F0_ADDRESS, &GMMx27F0.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  GnbRegisterWriteTN (GMMx27FC_TYPE, GMMx27FC_ADDRESS, &GMMx27FC.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  GMMx2808.Field.STATE0 = GfxLibGetNumberOfSclkPerDramBurst (scale_mp0, GnbLibGetHeader (Gfx));
  GMMx2808.Field.STATE1 = GfxLibGetNumberOfSclkPerDramBurst (scale_mp1, GnbLibGetHeader (Gfx));
  GnbRegisterWriteTN (GMMx2808_TYPE, GMMx2808_ADDRESS, &GMMx2808.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));


  //MC Performance settings base on memory channel configuration
  //If 1 channel
  GMMx2004.Value = 0x210;
  GMMx2214.Value = 0x3;
  if (((DramChannelPresent & GNB_GFX_DRAM_CH_0_PRESENT) != 0) && ((DramChannelPresent & GNB_GFX_DRAM_CH_1_PRESENT) != 0)) {
    //If 2 channels
    GMMx2004.Value = 0x1210;
    GMMx2214.Value = 0xC3;
  }
  GnbRegisterWriteTN (GMMx2004_TYPE, GMMx2004_ADDRESS, &GMMx2004.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  GnbRegisterWriteTN (GMMx2214_TYPE, GMMx2214_ADDRESS, &GMMx2214.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
}

/*----------------------------------------------------------------------------------------*/
/**
 * Enable Stutter Mode with/without power-gating
 *
 *
 *
 * @param[in]   Gfx               Pointer to global GFX configuration
 *
 */
VOID
GfxGmcEnableStutterModePowerGatingTN (
  IN      GFX_PLATFORM_CONFIG   *Gfx
  )
{
  GMMx28D4_STRUCT   GMMx28D4;
  GMMx28D8_STRUCT   GMMx28D8;
  CPU_LOGICAL_ID    LogicalId;
  GNB_HANDLE        *GnbHandle;
  D18F2x408_dct0_STRUCT  D18F2x408;

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxGmcEnableStutterModePowerGatingTN Enter\n");
  GnbHandle = GnbGetHandle (GnbLibGetHeader (Gfx));
  ASSERT (GnbHandle != NULL);
  GetLogicalIdOfSocket (GnbGetSocketId (GnbHandle), &LogicalId, GnbLibGetHeader (Gfx));
  if ((LogicalId.Revision & AMD_F15_TN_A0) == AMD_F15_TN_A0) {
    IDS_HDT_CONSOLE (GNB_TRACE, "CPU Rev = %x, force GmcPowerGatingStutterOnly\n", LogicalId.Revision);
    Gfx->GmcPowerGating = GmcPowerGatingStutterOnly;
  }

  //Easily hard lock in DC mode at high temperature
  //We want to enable GMC PG -> 20mW power saving and disable HalfNclk gating -> negligible power saving
  GnbRegisterReadTN (TYPE_D18F2_dct0, D18F2x408_dct0_ADDRESS, &D18F2x408.Value, 0, GnbLibGetHeader (Gfx));
  D18F2x408.Field.DisHalfNclkPwrGate = 1;
  GnbRegisterWriteTN (TYPE_D18F2_dct0, D18F2x408_dct0_ADDRESS, &D18F2x408.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));

  GnbRegisterReadTN (TYPE_D18F2_dct1, D18F2x408_dct1_ADDRESS, &D18F2x408.Value, 0, GnbLibGetHeader (Gfx));
  D18F2x408.Field.DisHalfNclkPwrGate = 1;
  GnbRegisterWriteTN (TYPE_D18F2_dct1, D18F2x408_dct1_ADDRESS, &D18F2x408.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));

  GnbRegisterReadTN (GMMx28D8_TYPE, GMMx28D8_ADDRESS, &GMMx28D8.Value, 0, GnbLibGetHeader (Gfx));
  if (Gfx->GmcPowerGating == GmcPowerGatingWithStutter) {
    GnbRegisterReadTN (GMMx28D4_TYPE, GMMx28D4_ADDRESS, &GMMx28D4.Value, 0, GnbLibGetHeader (Gfx));
    GMMx28D4.Field.RENG_EXECUTE_ON_PWR_UP = 1;
    GMMx28D8.Field.RENG_EXECUTE_ON_REG_UPDATE = 1;
    GnbRegisterWriteTN (GMMx28D4_TYPE, GMMx28D4_ADDRESS, &GMMx28D4.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  } else {
    //GmcPowerGatingStutterOnly
    GMMx28D8.Field.STCTRL_STUTTER_EN = 1;
    GMMx28D8.Field.STCTRL_DISABLE_GMC_OFFLINE = 1;
  }
  //System does not go into stutter mode anymore after running DX11
  //Protection faults were preventing non_display_idle from asserting
  GMMx28D8.Field.STCTRL_IGNORE_PROTECTION_FAULT = 1;
  GnbRegisterWriteTN (GMMx28D8_TYPE, GMMx28D8_ADDRESS, &GMMx28D8.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxGmcEnableStutterModePowerGatingTN Exit\n");
}
/*----------------------------------------------------------------------------------------*/
/**
 * Secure Garlic Access
 *
 *
 *
 * @param[in] Gfx         Pointer to global GFX configuration
 */

VOID
GfxGmcSecureGarlicAccessTN (
  IN      GFX_PLATFORM_CONFIG   *Gfx
  )
{
  GMMx2868_STRUCT   GMMx2868;
  GMMx286C_STRUCT   GMMx286C;
  GMMx287C_STRUCT   GMMx287C;

  GMMx2868.Value = (UINT32) (Gfx->UmaInfo.UmaBase >> 20);
  GnbRegisterWriteTN (GMMx2868_TYPE, GMMx2868_ADDRESS, &GMMx2868.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  GMMx286C.Value = (UINT32) (((Gfx->UmaInfo.UmaBase + Gfx->UmaInfo.UmaSize) >> 20) - 1);
  GnbRegisterWriteTN (GMMx286C_TYPE, GMMx286C_ADDRESS, &GMMx286C.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  // Areag FB - 32K reserved by VBIOS for SBIOS to use
  GMMx287C.Value = (UINT32) ((Gfx->UmaInfo.UmaBase + Gfx->UmaInfo.UmaSize - 32 * 1024) >> 12);
  GnbRegisterWriteTN (GMMx287C_TYPE, GMMx287C_ADDRESS, &GMMx287C.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));

}

/*----------------------------------------------------------------------------------------*/
/**
 * Initialize C6 aperture location
 *
 *
 *
 * @param[in]   Gfx               Pointer to global GFX configuration
 *
 */
VOID
GfxGmcInitializeC6LocationTN (
  IN      GFX_PLATFORM_CONFIG   *Gfx
  )
{
  D18F2x118_STRUCT    D18F2x118;
  D18F1x44_STRUCT     D18F1x44;
  GMMx2870_STRUCT     GMMx2870;
  GMMx2874_STRUCT     GMMx2874;

  // From D18F1x[144:140,44:40] DRAM Base/Limit,
  // {DramBase[47:24], 00_0000h} <= address[47:0] <= {DramLimit[47:24], FF_FFFFh}.
  GnbRegisterReadTN (D18F1x44_TYPE, D18F1x44_ADDRESS, &D18F1x44.Value, 0, GnbLibGetHeader (Gfx));
  //
  // base 39:20, base = Dram Limit + 1
  // ex: system 256 MB on Node 0, D18F1x44.Field.DramLimit_39_24_ = 0xE (240MB -1)
  // Node DRAM     D18F1x[144:140,44:40]    CC6DRAMRange         D18F4x128   D18F1x120 D18F1x124
  //    0 256MB    0MB ~ 240 MB - 1        240 MB ~ 256 MB - 1     0           0 MB,    256 MB - 1
  //

  // base 39:20
  GMMx2870.Value = ((D18F1x44.Field.DramLimit_39_24_ + 1) << 4);
  // top 39:20
  GMMx2874.Value = (((D18F1x44.Field.DramLimit_39_24_ + 1) << 24) + (16 * 0x100000) - 1) >> 20;

  // Check C6 enable,  D18F2x118[CC6SaveEn]
  GnbRegisterReadTN (D18F2x118_TYPE, D18F2x118_ADDRESS, &D18F2x118.Value, 0, GnbLibGetHeader (Gfx));

  if (D18F2x118.Field.CC6SaveEn) {

    GnbRegisterWriteTN (GMMx2874_TYPE, GMMx2874_ADDRESS, &GMMx2874.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
    GnbRegisterWriteTN (GMMx2870_TYPE, GMMx2870_ADDRESS, &GMMx2870.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Initialize GMC
 *
 *
 *
 * @param[in]   Gfx               Pointer to global GFX configuration
 *
 */

AGESA_STATUS
GfxGmcInitTN (
  IN      GFX_PLATFORM_CONFIG   *Gfx
  )
{
  GMMx28D8_STRUCT   GMMx28D8;
  GMMx206C_STRUCT   GMMx206C;
  GNB_HANDLE        *GnbHandle;

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxGmcInitTN Enter\n");
//2.1 Disable clock-gating
  GnbHandle = GnbGetHandle (GnbLibGetHeader (Gfx));
  ASSERT (GnbHandle != NULL);
  GnbProcessTable (
    GnbHandle,
    GfxGmcClockGatingDisableTN,
    0,
    GNB_TABLE_FLAGS_FORCE_S3_SAVE,
    GnbLibGetHeader (Gfx)
    );
//2.3 Sequencer model programming
  GfxGmcInitializeSequencerTN (Gfx);
//2.5 Frame buffer location
  GfxGmcInitializeFbLocationTN (Gfx);
//2.9.1 GARLIC request disable
  GfxGmcSecureGarlicAccessTN (Gfx);
//2.9.2 Securing GARLIC access
  GfxGmcInitializeC6LocationTN (Gfx);
//2.2 System memory address translation
//2.4 RENG init
//2.6 Performance tuning
//2.7 Miscellaneous programming
//2.11 Remove blackout
  GnbProcessTable (
    GnbHandle,
    GfxGmcInitTableTN,
    0,
    GNB_TABLE_FLAGS_FORCE_S3_SAVE,
    GnbLibGetHeader (Gfx)
    );
//4. Power management
//4.1 Enabling clock-gating
  if (Gfx->GmcClockGating) {
    GnbProcessTable (
      GnbHandle,
      GfxGmcClockGatingEnableTN,
      0,
      GNB_TABLE_FLAGS_FORCE_S3_SAVE,
      GnbLibGetHeader (Gfx)
      );
  }
//5. Steering all requests to ONION
  if (Gfx->UmaSteering == Onion) {
    GMMx206C.Value = 0x2;
    GnbRegisterWriteTN (GMMx206C_TYPE, GMMx206C_ADDRESS, &GMMx206C.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  }
  IDS_OPTION_CALLOUT (IDS_CALLOUT_GNB_GMM_REGISTER_OVERRIDE, Gfx, GnbLibGetHeader (Gfx));
//2.9.3 Locking critical registers
  if (Gfx->GmcLockRegisters) {
    GnbRegisterReadTN (GMMx28D8_TYPE, GMMx28D8_ADDRESS, &GMMx28D8.Value, 0, GnbLibGetHeader (Gfx));
    GMMx28D8.Field.CRITICAL_REGS_LOCK = 1;
    GnbRegisterWriteTN (GMMx28D8_TYPE, GMMx28D8_ADDRESS, &GMMx28D8.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Gfx));
  }
  if (Gfx->GmcPowerGating != GmcPowerGatingDisabled) {
    //4.2 Enabling stutter mode with or without power-gating
    GfxGmcEnableStutterModePowerGatingTN (Gfx);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxGmcInitTN Exit\n");
  return AGESA_SUCCESS;
}
