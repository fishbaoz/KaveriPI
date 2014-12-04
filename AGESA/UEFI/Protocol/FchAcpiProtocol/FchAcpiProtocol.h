/* $NoKeywords:$ */
/**
 * @file
 *
 * FCH DXE
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project   FCH DXE
 * @e \$Revision: 281181 $   @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*****************************************************************************
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

#ifndef _FCH_ACPI_PROTOCOL_H_
#define _FCH_ACPI_PROTOCOL_H_
//
// GUID definition
//
#define FCH_ACPI_PROTOCOL_GUID \
        { 0x2c71a209, 0x41af, 0x42d4, 0x89, 0x6d, 0xc3, 0xe, 0xfb, 0x3f, 0x11, 0x5e }
#define FCH_ACPI_TABLE_INSTALL_GUID \
        { 0x89fef8c7, 0xcc96, 0x4d20, 0xa4, 0x62, 0xcd, 0x11, 0x92, 0xf7, 0x3, 0x98 }


extern EFI_GUID gFchAcpiProtocolGuid;
extern EFI_GUID gFchAcpiTableInstallGuid;

struct _FCH_ACPI_PROTOCOL;
typedef struct _FCH_ACPI_PROTOCOL FCH_ACPI_PROTOCOL;

///
/// FCH_ACPI_INTERNAL_NAME
///
typedef struct {
  UINT32                                         AcpiAddress;                  ///< AcpiAddress
  UINT32                                         AcpiName;                     ///< AcpiName
} FCH_ACPI_INTERNAL_NAME;

///
/// FCH_ACPI_FIELD_NAME
///
typedef struct _FCH_ACPI_FIELD_NAME {
  UINT8                                          FieldLength;                  ///< FieldLength
  UINT32                                         Offset;                       ///< Offset
  UINT32                                         AcpiName;                     ///< AcpiName
} FCH_ACPI_FIELD_NAME;

typedef EFI_STATUS (EFIAPI *FCH_ACPI_ADD_DATA) (
  IN       FCH_ACPI_PROTOCOL                     *This,
  IN       UINT8                                 Length,
  IN       VOID                                  *Data,
  IN       UINT32                                AcpiName,
     OUT   UINT32                                *DataOffset
);

typedef EFI_STATUS (EFIAPI *FCH_ACPI_ENABLE_GPIO) (
  IN       FCH_ACPI_PROTOCOL                     *This,
  IN       BOOLEAN                               Enable
);

typedef EFI_STATUS (EFIAPI *FCH_ACPI_ENABLE_ASF) (
  IN       FCH_ACPI_PROTOCOL                     *This,
  IN       BOOLEAN                               Enable
);

typedef EFI_STATUS (EFIAPI *FCH_ACPI_SYNC_GPE_RESOURCE_DATA) (
  IN       FCH_ACPI_PROTOCOL                     *This
);

typedef EFI_STATUS (EFIAPI *FCH_ACPI_GET_AVL_GPE) (
  IN       FCH_ACPI_PROTOCOL                     *This,
     OUT   UINT8                                 *FchGpePin
);

typedef EFI_STATUS (EFIAPI *FCH_ACPI_GET_GPE_NUMBER) (
  IN       FCH_ACPI_PROTOCOL                     *This,
  IN       UINT8                                 *GpeMapOffset,
     OUT   UINT8                                 *GpeNumber
);

typedef EFI_STATUS (EFIAPI *FCH_ACPI_SET_GPE_NUMBER) (
  IN       FCH_ACPI_PROTOCOL                     *This,
  IN       UINT8                            *GpeMapOffset,
  IN       UINT8                            *GpeNumber
);
typedef EFI_STATUS (EFIAPI *FCH_ACPI_FIND_NEXT_SIGNATURE) (
  IN OUT   UINT8                                 **Aml,
  IN       UINT32                                Size,
  IN       UINT32                                Signature
);

typedef EFI_STATUS (EFIAPI *FCH_ACPI_GET_PATHNAME) (
  IN       FCH_ACPI_PROTOCOL                     *This,
  IN       UINT32                                IntDevAddress,
  IN OUT   UINT32                                *PathName
);

typedef EFI_STATUS (EFIAPI *FCH_ACPI_UPDATE_NAME) (
  IN       VOID                                  *AcpiTable,
  IN       VOID                                  *OldName,
  IN       VOID                                  *NewName,
  IN       UINT8                                 NameLength
);

typedef VOID (EFIAPI *FCH_ACPI_GET_PACKAGE_LENGTH) (
  IN       UINT8                                 *AmlPointer,
     OUT   UINT8                                 *ByteCount,
     OUT   UINT32                                *PkgLength
);

///
/// FCH DATA INTERFACE
///
typedef struct _FCH_ACPI_DATA_INTERFACE {
  UINT32                                         DataLength;            ///< Length of FCH Data block
  UINT32                                         AcpiInfoLength;        ///< Length of ACPI info block
  VOID                                           *FchData;              ///< FCH Data
  VOID                                           *AcpiFieldNames;       ///< FCH DATA ASL Field info data
} FCH_ACPI_DATA_INTERFACE;

///
/// FCH ACPI Tool
///
typedef struct _FCH_ACPI_TOOL {
  FCH_ACPI_FIND_NEXT_SIGNATURE                   FindNextSig;           ///< FindNextSig
  FCH_ACPI_UPDATE_NAME                           UpdateAcpiName;        ///< UpdateAcpiName
  FCH_ACPI_GET_PACKAGE_LENGTH                    GetPkgLength;          ///< GetPkgLength
} FCH_ACPI_TOOL;

///
/// FCH ACPI Protocol
///
typedef struct _FCH_ACPI_PROTOCOL {
  UINTN                                          Revision;                    ///< Revision
  FCH_ACPI_DATA_INTERFACE                        FchAcpiData;                 ///< FchAcpiData
  FCH_ACPI_TOOL                                  FchAcpiTool;                 ///< FchAcpiTool
  FCH_ACPI_GET_PATHNAME                          FchAcpiGetIntDevPathName;    ///< FchAcpiGetIntDevPathName
  FCH_ACPI_ADD_DATA                              FchAcpiAddDataField;         ///< FchAcpiAddDataField
  FCH_ACPI_ENABLE_GPIO                           FchAcpiEnableGpio;           ///< FchAcpiEnableGpio
  FCH_ACPI_ENABLE_ASF                            FchAcpiEnableAsf;            ///< FchAcpiEnableAsf
  FCH_ACPI_SYNC_GPE_RESOURCE_DATA                FchSyncGpeResourceData;      ///< FchSyncGpeResourceData
  FCH_ACPI_GET_AVL_GPE                           FchGetAvlGpe;                ///< FchGetAvlGpe
  FCH_ACPI_GET_GPE_NUMBER                        FchGetGpeNumber;             ///< FchGetGpeNumber
  FCH_ACPI_SET_GPE_NUMBER                        FchSetGpeNumber;             ///< FchSetGpeNumber
} FCH_ACPI_PROTOCOL;

// current Protocol revision
#define FCH_ACPI_PROTOCOL_REV                    0x01
#define AMD_FCH_INTERNAM_DEVICE_NAMEPATH_LENGTH  0x0C

#define FCH_ACPI_FIELD_INFO_END                  0xFF

#define FCH_ACPI_FIELD_LENGTH_BYTE               0x01
#define FCH_ACPI_FIELD_LENGTH_WORD               0x02
#define FCH_ACPI_FIELD_LENGTH_DWORD              0x04

#define FCH_ACPI_DEVICE_SMBUS                    0xFFFF0000ul
#define FCH_ACPI_DEVICE_ASF_SMBUS                0xFFFF0001ul
#define FCH_ACPI_DEVICE_GPIO                     0xFFFF0002ul
#define FCH_ACPI_DEVICE_GPE                      0xFFFF0003ul
#define FCH_ACPI_DEVICE_PWRBTN                   0xFFFF0004ul

#endif // _FCH_INIT_PROTOCOL_H_
