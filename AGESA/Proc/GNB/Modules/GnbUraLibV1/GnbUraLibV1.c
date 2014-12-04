/* $NoKeywords:$ */
/**
 * @file
 *
 * Service procedure to access PCI config space registers
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
#include  "GnbLib.h"
#include  "GnbPcieConfig.h"
#include  "GnbCommonLib.h"
#include  "GnbUraServices.h"
#include  "Filecode.h"

#define FILECODE PROC_GNB_MODULES_GNBURALIBV1_GNBURALIBV1_FILECODE

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

UINT8 RegisterDomainMap[] = {
  0x0,          ///< Stub
  0x0,          ///< Stub
  0x64,         ///< TYPE_D0F0x64
  0x98,         ///< TYPE_D0F0x98
  0xB8,         ///< TYPE_D0F0xB8
  0xFD        ///< End
};

/*----------------------------------------------------------------------------------------*/
/**
 * Convert URA Token to register address and field location
 *
 * @param[in]        UraToken            Register of bit field identifier
 * @param[in]        UraTable            Pointer to register token table
 * @param[in, out]   TokenInfo           Pointer to URA_TOKEN_INFO structure
 *
 * @retval     UraToken   Token is defined
 * @retval     _UNUSED    Token is undefined
 *
 */
URA_TOKEN
STATIC
UraTranslateToken (
  IN       URA_TOKEN         UraToken,
  IN       URA_ENTRY         *UraTable,
  IN OUT   URA_TOKEN_INFO    *TokenInfo
  )
{
  URA_TOKEN_STRUCT  Token;
  URA_FIELD_16B_ENTRY *Field16;
  URA_REGISTER_32B_ENTRY  *Reg32;
  URA_REGISTER_64B_ENTRY  *Reg64;

  Token.Encode = UraToken;
  Reg32 = NULL;
  Reg64 = NULL;
  Field16 = NULL;

  switch (Token.Parser.Type) {
  //
  // Parse 16 bit field encoding
  //
  case URA_TYPE_FIELD_16:
    Field16 = (URA_FIELD_16B_ENTRY *) &(UraTable[Token.Parser.Index]);
    TokenInfo->BfWidth = (UINT8) Field16->BfWidth;
    TokenInfo->BfOffset = (UINT8) Field16->BfOffset;
    while (Field16->Offset == 0) {
      Token.Parser.Index -= 15;
      Field16 = (URA_FIELD_16B_ENTRY *) &UraTable[Token.Parser.Index];
    }

    ASSERT (Field16->Offset != 0);

    if (Token.Parser.ParentType == URA_TOKEN_PARENT_TYPE_32) {
      Token.Parser.Index -= (Field16->Offset - 1) * 2 + 2;
      Reg32 = (URA_REGISTER_32B_ENTRY *) &UraTable[Token.Parser.Index];
    } else if (Token.Parser.ParentType == URA_TOKEN_PARENT_TYPE_64) {
      Token.Parser.Index -= Field16->Offset + 3;
      Reg64 = (URA_REGISTER_64B_ENTRY *) &(UraTable[Token.Parser.Index]);
    } else {
      ASSERT (FALSE);
      return _UNUSED;
    }

    TokenInfo->WholeRegAccess = FALSE;
    break;

  //
  // Parse 32 bit register encoding
  //
  case URA_TYPE_REGISTER_32:
    Reg32 = (URA_REGISTER_32B_ENTRY *) &UraTable[Token.Parser.Index];
    if (Reg32->Address == _UNUSED) {
      return _UNUSED;
    }
    TokenInfo->WholeRegAccess = TRUE;
    break;

  //
  // Parse 64 bit register encoding
  //
  case URA_TYPE_REGISTER_64:
    Reg64 = (URA_REGISTER_64B_ENTRY *) &(UraTable[Token.Parser.Index]);
    if (Reg64->DomainAddress == _UNUSED) {
      return _UNUSED;
    }
    TokenInfo->WholeRegAccess = TRUE;
    break;

  default:
    ASSERT (FALSE);
  }

  // Get register address
  if ((Token.Parser.ParentType == URA_TOKEN_PARENT_TYPE_32) && (Reg32 != NULL)) {
    TokenInfo->RegAddress = Reg32->Address;
    TokenInfo->RegDomainType = 0;
  } else if ((Token.Parser.ParentType == URA_TOKEN_PARENT_TYPE_64) && (Reg64 != NULL)) {
    TokenInfo->RegAddress = (UINT32)Reg64->DomainAddress;
    TokenInfo->RegDomainType = (UINT8)Reg64->DomainType;
  } else {
    ASSERT (FALSE);
    return _UNUSED;
  }

  TokenInfo->MethodType = Token.Parser.Selector;

  // Patch DomainType field
  if (TokenInfo->MethodType == TYPE_GNB_INDIRECT_ACCESS) {
    TokenInfo->RegDomainType = RegisterDomainMap[TokenInfo->RegDomainType];
  }

  TokenInfo->Flags = Token.Parser.S3Save;
  TokenInfo->StreamSet = Token.Parser.StreamSet;

  return Token.Encode;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Gnb Unified Register Access method
 *
 *
 * @param[in]       Device       Standard configuration header
 * @param[in]       UraToken     Context length
 * @param[in, out]  Value        Pointer to Context
 */
VOID
GnbUraGet (
  IN       DEV_OBJECT    *Device,
  IN       URA_TOKEN     UraToken,
  IN OUT   VOID          *Value
  )
{
  AGESA_STATUS              Status;
  GNB_URA_SERVICE           *GnbUraProtocol;
  URA_TOKEN_INFO            TokenInfo;
  URA_ENTRY                 *UraTable;
  UINT32                    UraTableAddress;

  UraTable = NULL;
  Status = GnbLibLocateService (GnbUraService, GnbGetSocketId (Device->GnbHandle), &GnbUraProtocol, Device->StdHeader);
  ASSERT (Status == AGESA_SUCCESS);

  GnbUraProtocol->GnbUraLocateRegTbl (Device, &UraTableAddress);
  UraTable = (URA_ENTRY *) ((UINTN)UraTableAddress);
  UraToken = UraTranslateToken (UraToken, UraTable, &TokenInfo);
  if (UraToken == _UNUSED) {
    return;
  }

  GnbUraProtocol->GnbUraGet (Device, &TokenInfo, Value);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Gnb Unified Register Access method
 *
 *
 * @param[in]  Device       Standard configuration header
 * @param[in]  UraToken     Ura token
 * @param[in]  Value        Pointer to Context
 */
VOID
GnbUraSet (
  IN       DEV_OBJECT          *Device,
  IN       URA_TOKEN           UraToken,
  IN       VOID                *Value
  )
{
  AGESA_STATUS              Status;
  GNB_URA_SERVICE           *GnbUraProtocol;
  URA_TOKEN_INFO            TokenInfo;
  URA_ENTRY                 *UraTable;
  UINT32                    UraTableAddress;

  Status = GnbLibLocateService (GnbUraService, GnbGetSocketId (Device->GnbHandle), &GnbUraProtocol, Device->StdHeader);
  ASSERT (Status == AGESA_SUCCESS);

  GnbUraProtocol->GnbUraLocateRegTbl (Device, &UraTableAddress);
  UraTable = (URA_ENTRY *) ((UINTN)UraTableAddress);
  UraToken = UraTranslateToken (UraToken, UraTable, &TokenInfo);
  if (UraToken == _UNUSED) {
    return;
  }

  GnbUraProtocol->GnbUraSet (Device, &TokenInfo, Value);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Gnb Unified Register Access method
 *
 *
 * @param[in]       Device             Standard configuration header
 * @param[in, out]  UraTokenRegister   Register token
 * @param[in]       UraTuple           Pointer to Context
 * @param[in, out]  CombinedCount      Token count
 */
VOID
GnbUraCombinedGet (
  IN       DEV_OBJECT        *Device,
  IN       URA_TOKEN         UraTokenRegister,
  IN OUT   URA_TUPLE         *UraTuple,
  IN       UINT32            CombinedCount
  )
{
  AGESA_STATUS            Status;
  GNB_URA_SERVICE         *GnbUraProtocol;
  URA_TOKEN_INFO          TokenInfo;
  URA_ENTRY               *UraTable;
  UINT32                  UraTableAddress;
  UINT32                  RegValue;
  UINT32                  TempValue;
  UINT32                  FieldMask;
  URA_TOKEN               UraToken;
  UINT32                  Index;

  Status = GnbLibLocateService (GnbUraService, GnbGetSocketId (Device->GnbHandle), &GnbUraProtocol, Device->StdHeader);
  ASSERT (Status == AGESA_SUCCESS);

  GnbUraProtocol->GnbUraLocateRegTbl (Device, &UraTableAddress);
  UraTable = (URA_ENTRY *) ((UINTN)UraTableAddress);
  UraTokenRegister = UraTranslateToken (UraTokenRegister, UraTable, &TokenInfo);
  if (UraTokenRegister == _UNUSED) {
    return;
  }

  GnbUraProtocol->GnbUraGet (Device, &TokenInfo, &RegValue);

  Index = 0;
  do {
    UraToken = UraTuple[Index].Token;
    UraToken = UraTranslateToken (UraToken, UraTable, &TokenInfo);
    if (UraToken == _UNUSED) {
      return;
    }

    TempValue = RegValue;
    TempValue = TempValue >> TokenInfo.BfOffset;
    FieldMask = (((UINT32)1 << TokenInfo.BfWidth) - 1);
    UraTuple[Index].Value = TempValue & FieldMask;
    Index++;
  } while (Index < CombinedCount);

}

/*----------------------------------------------------------------------------------------*/
/**
 * Gnb Unified Register Access method
 *
 *
 * @param[in]       Device             Standard configuration header
 * @param[in, out]  UraTokenRegister   Register token
 * @param[in]       UraTuple           Pointer to Context
 * @param[in, out]  CombinedCount      Token count
 */
VOID
GnbUraCombinedSet (
  IN       DEV_OBJECT     *Device,
  IN       URA_TOKEN      UraTokenRegister,
  IN OUT   URA_TUPLE      *UraTuple,
  IN       UINT32         CombinedCount
  )
{
  AGESA_STATUS            Status;
  GNB_URA_SERVICE         *GnbUraProtocol;
  URA_TOKEN_INFO          TokenInfo;
  URA_ENTRY               *UraTable;
  UINT32                  UraTableAddress;
  UINT32                  RegValue;
  UINT32                  TempValue;
  UINT32                  FieldMask;
  URA_TOKEN               UraToken;
  UINT32                  Index;

  Status = GnbLibLocateService (GnbUraService, GnbGetSocketId (Device->GnbHandle), &GnbUraProtocol, Device->StdHeader);
  ASSERT (Status == AGESA_SUCCESS);

  GnbUraProtocol->GnbUraLocateRegTbl (Device, &UraTableAddress);
  UraTable = (URA_ENTRY *) ((UINTN)UraTableAddress);
  UraTokenRegister = UraTranslateToken (UraTokenRegister, UraTable, &TokenInfo);
  if (UraTokenRegister == _UNUSED) {
    return;
  }

  if (TokenInfo.StreamSet == 1) {
    GnbUraProtocol->GnbUraStreamSet (Device, &TokenInfo, UraTuple, CombinedCount);
  } else {
    GnbUraProtocol->GnbUraGet (Device, &TokenInfo, &RegValue);

    Index = 0;
    do {
      UraToken = UraTuple[Index].Token;
      UraToken = UraTranslateToken (UraToken, UraTable, &TokenInfo);
      if (UraToken == _UNUSED) {
        return;
      }
      FieldMask = (((UINT32)1 << TokenInfo.BfWidth) - 1);
      TempValue = UraTuple[Index].Value & FieldMask;
      RegValue &= ~(FieldMask << TokenInfo.BfOffset);
      RegValue |= TempValue << TokenInfo.BfOffset;
      Index++;
    } while (Index < CombinedCount);

    UraTokenRegister = UraTranslateToken (UraTokenRegister, UraTable, &TokenInfo);
    GnbUraProtocol->GnbUraSet (Device, &TokenInfo, &RegValue);
  }
}
