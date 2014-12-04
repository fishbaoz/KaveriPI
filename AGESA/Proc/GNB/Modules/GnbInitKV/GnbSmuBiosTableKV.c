/* $NoKeywords:$ */
/**
 * @file
 *
 * SMU Bios Table
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
#include  "amdlib.h"
#include  "heapManager.h"
#include  "Ids.h"
#include  "Gnb.h"
#include  "GnbPcieConfig.h"
#include  "GnbCommonLib.h"
#include  "GnbRegisterAccKV.h"
#include  "GnbRegistersKV.h"
#include  "GnbSmuInitLibV7.h"
#include  "GnbSmuBiosTableKV.h"
#include  "mm.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITKV_GNBSMUBIOSTABLEKV_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
extern BUILD_OPT_CFG UserOptions;

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
 * Init SMU Bios Table
 *
 * @param[in]  StdHeader  Standard configuration header
 */

VOID
GnbInitSmuBiosTableKV (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  GNB_HANDLE              *GnbHandle;
  SMU7_BIOSTABLE          *BiosTable;
  UINT32                  *Ptr;
  D0F0xBC_x20054_STRUCT   D0F0xBC_x20054;
  UINT32                  Addr;
  UINT32                  Value;
  UINT8                   Index;

  GnbHandle = GnbGetHandle (StdHeader);

  BiosTable = (SMU7_BIOSTABLE *) GnbAllocateHeapBuffer (AMD_GNB_SMU_TABLE_HANDLE, sizeof (SMU7_BIOSTABLE), StdHeader);
  ASSERT (BiosTable != NULL);

  if (BiosTable != NULL) {

    GnbRegisterReadKV (GnbHandle, TYPE_D0F0xBC, D0F0xBC_x20054_ADDRESS,
                        &D0F0xBC_x20054.Value, 0, StdHeader);
    Addr = D0F0xBC_x20054.Value;
    ASSERT (Addr != NULL);

    IDS_HDT_CONSOLE (GNB_TRACE, "Read BiosTable = %08x with %08x bytes\n",
        BiosTable, sizeof (SMU7_BIOSTABLE));
    IDS_HDT_CONSOLE (GNB_TRACE, "  Start addr: %08x\n", Addr);

    Ptr = (UINT32 *) BiosTable;
    for (Index = 0; Index < (sizeof (SMU7_BIOSTABLE) / 4); Index++) {
      GnbRegisterReadKV (GnbHandle, TYPE_D0F0xBC, Addr, &Value,
          0, StdHeader);
      *(Ptr + Index) = Value;
      IDS_HDT_CONSOLE (GNB_TRACE, "  Addr: %08x Value: %08x Dest: %08x\n",
          Addr, Value, Ptr + Index);
      Addr += 4;
    }

  }

}

/*----------------------------------------------------------------------------------------*/
/**
 * SMU BIOS table
 *
 *
 * @param[in]  StdHeader       Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
GnbWriteSmuBiosTableKV (
  IN       AMD_CONFIG_PARAMS        *StdHeader
  )
{
  SMU7_BIOSTABLE          *BiosTable;
  GNB_HANDLE              *GnbHandle;
  D0F0xBC_x20054_STRUCT   D0F0xBC_x20054;
  UINT32                  Addr;
  UINT32                  Index;
  UINT32                  *Ptr;

  GnbHandle = GnbGetHandle (StdHeader);

  GnbRegisterReadKV (GnbHandle, TYPE_D0F0xBC, D0F0xBC_x20054_ADDRESS,
                      &D0F0xBC_x20054.Value, 0, StdHeader);
  Addr = D0F0xBC_x20054.Value;
  ASSERT (Addr != NULL);

  BiosTable = (SMU7_BIOSTABLE *) GnbLocateHeapBuffer (AMD_GNB_SMU_TABLE_HANDLE, StdHeader);
  ASSERT (BiosTable != NULL);

  IDS_HDT_CONSOLE (GNB_TRACE, "Write BiosTable = %08x with %08x bytes\n",
      BiosTable, sizeof (SMU7_BIOSTABLE));
  IDS_HDT_CONSOLE (GNB_TRACE, "  BiosTable[0] =   %02X\n", *((UINT8 *) BiosTable));

  if (BiosTable != NULL) {

    Ptr = (UINT32 *) BiosTable;
    for (Index = 0; Index < (sizeof (SMU7_BIOSTABLE) / 4); Index++) {
      GnbRegisterWriteKV (GnbHandle, TYPE_D0F0xBC, Addr, Ptr + Index,
          GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
      IDS_HDT_CONSOLE (GNB_TRACE, "  Addr: %08x Value: %08x Dest: %08x\n",
          Addr, *(Ptr + Index), Ptr + Index);
      Addr += 4;
    }

  }

  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Get memory information for BIOS table
 *
 * @param[in]  StdHeader  Standard configuration header
 */

VOID
GnbSmuBiosTableGetDimmInfoKV (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  SMU7_BIOSTABLE          *BiosTable;
  GNB_HANDLE              *GnbHandle;
  DIE_STRUCT              *MCTPtr;
  MEM_DATA_STRUCT         *MemDataPtr;
  UINT32                  VddIox100;
  UINT32                  Vddrx100;
  UINT32                  TargetTDP;
  UINT8                   offset;
  UINT16                  DimmValid;
  D18F2x2E0_dct0_STRUCT   D18F2x2E0_dct0;
  D18F2x2E0_dct3_STRUCT   D18F2x2E0_dct3;

  IDS_HDT_CONSOLE (GNB_TRACE, "Get Dimm information for BIOS table for SMU\n");

  GnbHandle = GnbGetHandle (StdHeader);
  BiosTable = (SMU7_BIOSTABLE *) GnbLocateHeapBuffer (AMD_GNB_SMU_TABLE_HANDLE, StdHeader);
  ASSERT (BiosTable != NULL);

  MCTPtr = (DIE_STRUCT *) GnbLocateHeapBuffer (GENERATE_MEM_HANDLE (ALLOC_DIE_STRUCT_HANDLE, 0, 0, 0), StdHeader);
  ASSERT (MCTPtr != NULL);

  MemDataPtr = (MEM_DATA_STRUCT *) GnbLocateHeapBuffer (AMD_MEM_DATA_HANDLE, StdHeader);
  ASSERT (MemDataPtr != NULL);

  DimmValid = (UINT16) (MCTPtr->DimmValid & 0xFFFF);
  DimmValid |= (UINT16) (MCTPtr->DimmValid >> 16);

  IDS_HDT_CONSOLE (GNB_TRACE, "Valid Dimm Mask: %08x\n", MCTPtr->DimmValid);
  IDS_HDT_CONSOLE (GNB_TRACE, "Local Dimm Mask: %04x\n", DimmValid);
  BiosTable->Dimms = 0;

  // Get number of dimms
  for (offset = 0; offset < 16; offset ++) {
    if ((DimmValid & ((UINT32)1 << offset)) != 0) {
      BiosTable->Dimms ++;
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "Number of Dimms: %d\n", BiosTable->Dimms);

  // Get memory frequency
  GnbRegisterReadKV (GnbHandle, D18F2x2E0_dct0_TYPE, D18F2x2E0_dct0_ADDRESS, &D18F2x2E0_dct0.Value, 0, StdHeader);
  GnbRegisterReadKV (GnbHandle, D18F2x2E0_dct3_TYPE, D18F2x2E0_dct3_ADDRESS, &D18F2x2E0_dct3.Value, 0, StdHeader);
  BiosTable->MemorySpeedHigh = MCTPtr->DctData[0].Timings.Speed & 0xFFFF;
  if ((D18F2x2E0_dct0.Field.M1MemClkFreq != 0) && (D18F2x2E0_dct3.Field.M1MemClkFreq != 0)) {
    BiosTable->MemorySpeedLow = BiosTable->MemorySpeedHigh;
  } else {
    BiosTable->MemorySpeedLow = DDR667_FREQUENCY;
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "Memory Speed Low: %dMHz\n", BiosTable->MemorySpeedLow);
  IDS_HDT_CONSOLE (GNB_TRACE, "Memory Speed High: %dMHz\n", BiosTable->MemorySpeedHigh);

  // Get VDDIO
  // Take the voltage in volt and multiply it by 2^15
  VddIox100 = 150;
  switch (MemDataPtr->ParameterListPtr->DDR3Voltage) {
  case VOLT1_5:
    VddIox100 = 150;
    break;
  case VOLT1_35:
    VddIox100 = 135;
    break;
  case VOLT1_25:
    VddIox100 = 125;
    break;
  default:
    ASSERT (FALSE);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "VDDIO: %d ", VddIox100);
  VddIox100 = VddIox100 << 15;
  // Need to round to nearest
  BiosTable->Vddio = (UINT16) (VddIox100 / 100);
  if ((VddIox100 % 100) >= 50) {
    BiosTable->Vddio ++;
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "%d\n", BiosTable->Vddio);

  // Get VDDR
  // Format is the same as VDDIO
  Vddrx100 = 105;
  if ((MemDataPtr->ParameterListPtr->VddpVddrVoltage.IsValid && (MemDataPtr->ParameterListPtr->VddpVddrVoltage.Voltage == VOLT0_95)) ||
      (!MemDataPtr->ParameterListPtr->VddpVddrVoltage.IsValid && (UserOptions.CfgMemoryPhyVoltage == VOLT0_95))) {
    Vddrx100 = 95;
  }
  ASSERT (UserOptions.CfgMemoryPhyVoltage < MAX_VDDR);
  IDS_HDT_CONSOLE (GNB_TRACE, "VDDR: %d ", Vddrx100);
  Vddrx100 <<= 15;
  BiosTable->Vddr = (UINT16) (Vddrx100 / 100);
  if ((Vddrx100 % 100) >= 50) {
    BiosTable->Vddr ++;
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "%d\n", BiosTable->Vddr);

  // Target TDP
  TargetTDP = (UINT32) (((AMD_POST_PARAMS *) StdHeader)->PlatformConfig.PowerCeiling / 100);
  IDS_OPTION_HOOK (IDS_GNB_TARGET_TDP, &TargetTDP, StdHeader);

  if (TargetTDP != 0) {
    BiosTable->TargetTdp = (UINT16) (TargetTDP * 256 / 10);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "Target TDP: %04x\n", BiosTable->TargetTdp);
}
