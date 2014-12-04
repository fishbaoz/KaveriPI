/* $NoKeywords:$ */
/**
 * @file
 *
 * Initialize PP/DPM fuse table.
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
#include  "Ids.h"
#include  "Gnb.h"
#include  "GnbUra.h"
#include  "GnbPcieConfig.h"
#include  "GnbCommonLib.h"
#include  "GnbUraToken.h"
#include  "GnbRegisterAccKV.h"
#include  "GnbRegistersKV.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITKV_GNBURAKV_FILECODE

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
 * Gnb Unified Register Access method
 *
 *
 * @param[in]       Device           Pointer to Device object
 * @param[in]       UraTokenInfo     Pointer to URA_TOKEN_INFO structure
 * @param[in, out]  Value            Pointer to Context
 */
VOID
GnbUraGetKV (
  IN       DEV_OBJECT      *Device,
  IN       URA_TOKEN_INFO  *UraTokenInfo,
  IN OUT   VOID            *Value
  )
{
  ACCESS_WIDTH      Width;
  UINT32            RegValue;
  UINT32            FieldMask;
  UINT32            TargetAddress;

  RegValue = 0;
  Width = (UraTokenInfo->Flags == GNB_URA_FLAG_S3SAVE) ? AccessS3SaveWidth32 : AccessWidth32;
  switch (UraTokenInfo->MethodType) {
  case TYPE_GNB_INDIRECT_ACCESS:
    TargetAddress = Device->DevPciAddress.AddressValue | UraTokenInfo->RegDomainType;
    GnbLibPciIndirectRead ( TargetAddress, UraTokenInfo->RegAddress, Width, &RegValue, Device->StdHeader);
    IDS_HDT_CONSOLE (NB_MISC, "  Ura GET:  RegDomainType = 0x%x  IndirectAddress = 0x%08x, Value = 0x%08x\n", UraTokenInfo->RegDomainType, UraTokenInfo->RegAddress, RegValue);
    break;

  case TYPE_GNB_PROTOCOL_ACCESS:
    TargetAddress = UraTokenInfo->RegDomainType;
    GnbRegisterReadKV (Device->GnbHandle, (UINT8)TargetAddress, UraTokenInfo->RegAddress, &RegValue, UraTokenInfo->Flags, Device->StdHeader);
    IDS_HDT_CONSOLE (NB_MISC, "  Ura GET:  RegDomainType = %d, Address = 0x%08x, Value = 0x%08x\n",  TargetAddress, UraTokenInfo->RegAddress, RegValue);
    break;

  default:
    ASSERT (FALSE);
    return;
    break;
  }

  if (UraTokenInfo->WholeRegAccess == TRUE) {
    *(UINT32 *)Value = RegValue;
  } else {
    RegValue = RegValue >> UraTokenInfo->BfOffset;
    FieldMask = (((UINT32)1 << UraTokenInfo->BfWidth) - 1);
    *(UINT32 *)Value = RegValue & FieldMask;
  }

}

/*----------------------------------------------------------------------------------------*/
/**
 * Gnb Unified Register Access method
 *
 *
 * @param[in]       Device           Pointer to Device object
 * @param[in]       UraTokenInfo     Pointer to URA_TOKEN_INFO structure
 * @param[in, out]  Value            Pointer to Context
 */
VOID
GnbUraSetKV (
  IN       DEV_OBJECT      *Device,
  IN       URA_TOKEN_INFO  *UraTokenInfo,
  IN OUT   VOID            *Value
  )
{
  ACCESS_WIDTH       Width;
  UINT32                   RegValue;
  UINT32                   TargetValue;
  UINT32                   FieldMask;
  UINT32                   TempValue;

  Width = (UraTokenInfo->Flags == GNB_URA_FLAG_S3SAVE) ? AccessS3SaveWidth32 : AccessWidth32;
  FieldMask = 0;
  TempValue = *(UINT32 *)Value;

  switch (UraTokenInfo->MethodType) {
  case TYPE_GNB_INDIRECT_ACCESS:
    if (UraTokenInfo->WholeRegAccess == TRUE) {
      TargetValue =  TempValue;
    } else {
      GnbLibPciIndirectRead ( Device->DevPciAddress.AddressValue | UraTokenInfo->RegDomainType, UraTokenInfo->RegAddress, Width, &RegValue, Device->StdHeader);
      FieldMask = (((UINT32)1 << UraTokenInfo->BfWidth) - 1);
      TargetValue = RegValue & (~(FieldMask << UraTokenInfo->BfOffset));
      TargetValue |= (TempValue & FieldMask) << UraTokenInfo->BfOffset;
    }
    GnbLibPciIndirectWrite ( Device->DevPciAddress.AddressValue | UraTokenInfo->RegDomainType, UraTokenInfo->RegAddress, Width, &TargetValue, Device->StdHeader);
    IDS_HDT_CONSOLE (NB_MISC, "  Ura SET:  RegDomainType = 0x%x  IndirectAddress = 0x%08x, Value = 0x%08x\n", UraTokenInfo->RegDomainType, UraTokenInfo->RegAddress, TargetValue);
    break;

  case TYPE_GNB_PROTOCOL_ACCESS:
    if (UraTokenInfo->WholeRegAccess == TRUE) {
      TargetValue =  TempValue;
    } else {
      GnbRegisterReadKV (Device->GnbHandle, UraTokenInfo->RegDomainType, UraTokenInfo->RegAddress, &RegValue, UraTokenInfo->Flags, Device->StdHeader);
      FieldMask = (((UINT32)1 << UraTokenInfo->BfWidth) - 1);
      TargetValue = RegValue & (~(FieldMask << UraTokenInfo->BfOffset));
      TargetValue |= (TempValue & FieldMask) << UraTokenInfo->BfOffset;
    }
    GnbRegisterWriteKV (Device->GnbHandle, UraTokenInfo->RegDomainType, UraTokenInfo->RegAddress, &TargetValue, UraTokenInfo->Flags, Device->StdHeader);
    IDS_HDT_CONSOLE (NB_MISC, "  Ura SET:  RegDomainType = %d, Address = 0x%08x, Value = 0x%08x\n",  UraTokenInfo->RegDomainType, UraTokenInfo->RegAddress, TargetValue);
    break;

  default:
    ASSERT (FALSE);
    break;
  }

}

/*----------------------------------------------------------------------------------------*/
/**
 * Gnb Unified Register Access method
 *
 *
 * @param[in]       Device             Pointer to device object
 * @param[in]       UraTokenInfo       Pointer to URA_TOKEN_INFO structure
 * @param[in, out]  UraTuple           Pointer to structure URA_TUPLE
 * @param[in]       CombinedCount      Token count
 */
VOID
GnbUraStreamSetKV (
  IN       DEV_OBJECT        *Device,
  IN       URA_TOKEN_INFO    *UraTokenInfo,
  IN OUT   URA_TUPLE         *UraTuple,
  IN       UINT32            CombinedCount
  )
{
  ACCESS_WIDTH      Width;
  UINT32            RegValue;
  UINT32            Index;
  UINT32            StreamSetAddress;
  UINT32            StepLength;
  UINT32            TargetAddress;

  Width = (UraTokenInfo->Flags == GNB_URA_FLAG_S3SAVE) ? AccessS3SaveWidth32 : AccessWidth32;
  StreamSetAddress = UraTokenInfo->RegAddress;
  StepLength = UraTuple->StepLength;
  for (Index = 0; Index < CombinedCount; Index++) {
    RegValue = *(((UINT32 *) ((UINTN)UraTuple->Value)) + Index);
    switch (UraTokenInfo->MethodType) {
    case TYPE_GNB_INDIRECT_ACCESS:
      TargetAddress = Device->DevPciAddress.AddressValue | UraTokenInfo->RegDomainType;
      //IDS_HDT_CONSOLE (NB_MISC, "0x%08x:0x%08x, \n", StreamSetAddress, RegValue);
      GnbLibPciIndirectWrite (TargetAddress, StreamSetAddress, Width, &RegValue, Device->StdHeader);
      break;

    case TYPE_GNB_PROTOCOL_ACCESS:
      TargetAddress = UraTokenInfo->RegDomainType;
      GnbRegisterWriteKV (Device->GnbHandle, (UINT8)TargetAddress, StreamSetAddress, &RegValue, UraTokenInfo->Flags, Device->StdHeader);
      break;

    default:
      ASSERT (FALSE);
      return;
    }
    StreamSetAddress += StepLength;
  }
}

