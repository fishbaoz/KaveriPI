/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD F15Kv Microcode patch.
 *
 * F15Kv Microcode Patch rev 06003106 for 6301 or equivalent.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x15/KV
 * @e \$Revision: 290541 $   @e \$Date: 2014-04-23 10:15:07 -0500 (Wed, 23 Apr 2014) $
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
 *
 ***************************************************************************/



/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AGESA.h"
#include "Ids.h"
#include "cpuRegisters.h"
#include "cpuEarlyInit.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

UCODE_VS_FLAG (06003106)

// Encrypt Patch code 06003106 for 6301 and equivalent

CONST UINT8 ROMDATA CpuF15KvId6301MicrocodePatch [IDS_PAD_4K] =
{
  0x14,
  0x20,
  0x18,
  0x04,
  0x06,
  0x31,
  0x00,
  0x06,
  0x02,
  0x80,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x01,
  0x63,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0xad,
  0xa8,
  0xd0,
  0x1b,
  0x1d,
  0x8f,
  0x03,
  0xef,
  0x5c,
  0xa6,
  0xd4,
  0x46,
  0x01,
  0xaa,
  0xcb,
  0x14,
  0xc7,
  0xfa,
  0x94,
  0x03,
  0x01,
  0x1f,
  0x29,
  0xab,
  0x8e,
  0x67,
  0x20,
  0xc5,
  0x67,
  0xa2,
  0x12,
  0x1f,
  0x3b,
  0xcc,
  0x3a,
  0x18,
  0x39,
  0xbb,
  0xc0,
  0xb2,
  0xbc,
  0x3d,
  0x13,
  0xd6,
  0x8e,
  0xdb,
  0xda,
  0x3a,
  0xb0,
  0x76,
  0x1c,
  0x6c,
  0x68,
  0xc5,
  0x09,
  0xc1,
  0x7c,
  0x54,
  0xad,
  0xcf,
  0x0e,
  0x25,
  0x96,
  0x68,
  0x8f,
  0xd4,
  0x75,
  0x9c,
  0xb1,
  0x8a,
  0x7b,
  0x25,
  0xc0,
  0xd9,
  0x37,
  0xe6,
  0x36,
  0x63,
  0x7a,
  0x62,
  0xd9,
  0x09,
  0x94,
  0x62,
  0x67,
  0x40,
  0x0f,
  0xff,
  0x4c,
  0xc6,
  0xae,
  0xcf,
  0x8c,
  0xe2,
  0xc7,
  0x74,
  0xcb,
  0x3e,
  0x68,
  0x2d,
  0x40,
  0x6e,
  0x9f,
  0xec,
  0x21,
  0x8f,
  0x54,
  0x21,
  0xfb,
  0x2b,
  0x60,
  0xd3,
  0x5b,
  0x2f,
  0x1e,
  0x79,
  0x6e,
  0xde,
  0x39,
  0x2f,
  0xe9,
  0xc0,
  0xda,
  0xec,
  0x1b,
  0xc5,
  0xf4,
  0x6e,
  0xe9,
  0xda,
  0x54,
  0xb7,
  0x0b,
  0x63,
  0x6b,
  0xae,
  0x23,
  0x2e,
  0x43,
  0x6d,
  0xd5,
  0x37,
  0xde,
  0x27,
  0x05,
  0x32,
  0xa1,
  0x2e,
  0x34,
  0xb2,
  0xde,
  0x18,
  0xe3,
  0x17,
  0x50,
  0x58,
  0x6c,
  0x67,
  0x88,
  0x91,
  0x17,
  0x34,
  0x8c,
  0x85,
  0x58,
  0xeb,
  0xc0,
  0x68,
  0xc8,
  0xb7,
  0xfd,
  0x04,
  0x8f,
  0xe1,
  0xd0,
  0x2a,
  0xaa,
  0xc7,
  0x3a,
  0xa8,
  0x0d,
  0x7f,
  0xa0,
  0xd5,
  0x78,
  0x7e,
  0x65,
  0x3d,
  0xff,
  0xfc,
  0x9b,
  0xfb,
  0x3f,
  0x21,
  0xe9,
  0xc2,
  0xbc,
  0x48,
  0xd1,
  0xac,
  0xce,
  0xc3,
  0x86,
  0x16,
  0x82,
  0x06,
  0xe4,
  0x76,
  0x57,
  0x3f,
  0xe1,
  0xa8,
  0x9c,
  0xf4,
  0x2c,
  0x21,
  0x0d,
  0x2e,
  0xe1,
  0x55,
  0x81,
  0xaf,
  0x3d,
  0xd6,
  0x88,
  0x41,
  0x0c,
  0x0b,
  0x54,
  0x8d,
  0xb4,
  0xd1,
  0xc8,
  0x0b,
  0x77,
  0x7b,
  0x3c,
  0xdc,
  0x13,
  0x21,
  0xb4,
  0xf1,
  0x43,
  0x04,
  0xcd,
  0x06,
  0xc8,
  0xe5,
  0x7b,
  0xbb,
  0x2b,
  0x42,
  0x5e,
  0x86,
  0x56,
  0xd9,
  0x88,
  0x1f,
  0xbb,
  0x7a,
  0xa3,
  0xaa,
  0xc4,
  0x1b,
  0xc3,
  0x8f,
  0x04,
  0x06,
  0x82,
  0x14,
  0x38,
  0x7b,
  0x5a,
  0x8e,
  0x4f,
  0x0c,
  0xc5,
  0x91,
  0xf4,
  0x31,
  0x5a,
  0x05,
  0x44,
  0x1e,
  0x48,
  0xce,
  0x39,
  0x9a,
  0xc2,
  0xbb,
  0xa1,
  0x21,
  0x67,
  0x08,
  0xfd,
  0xb9,
  0xe0,
  0x22,
  0x90,
  0xb3,
  0x7d,
  0xe5,
  0x46,
  0x4d,
  0x88,
  0xe5,
  0x8f,
  0xfc,
  0x48,
  0x9b,
  0x56,
  0x78,
  0x31,
  0x29,
  0xa0,
  0x65,
  0x61,
  0xc0,
  0x5c,
  0xdf,
  0xbd,
  0x98,
  0x04,
  0xb9,
  0x60,
  0x1e,
  0xbf,
  0xf3,
  0xe3,
  0x57,
  0x86,
  0x41,
  0x77,
  0xce,
  0x24,
  0x39,
  0xc6,
  0xf7,
  0x2a,
  0x19,
  0x96,
  0xe5,
  0xaa,
  0xf5,
  0x79,
  0x46,
  0x26,
  0x97,
  0x4d,
  0xca,
  0x97,
  0x8b,
  0x31,
  0x4d,
  0x19,
  0x59,
  0x1c,
  0x74,
  0x4d,
  0xb7,
  0x1f,
  0x83,
  0x17,
  0x51,
  0xca,
  0xa1,
  0x37,
  0xaf,
  0xf8,
  0x5d,
  0x95,
  0x7c,
  0xb0,
  0x92,
  0xec,
  0x56,
  0x67,
  0xcc,
  0xd4,
  0x52,
  0x35,
  0xae,
  0x90,
  0x65,
  0x31,
  0x46,
  0x25,
  0xea,
  0xcc,
  0x0d,
  0xdf,
  0xea,
  0x54,
  0x58,
  0xe3,
  0x56,
  0xd6,
  0xd8,
  0x2a,
  0xab,
  0xe8,
  0x0e,
  0x7a,
  0xeb,
  0x4b,
  0x8a,
  0x4d,
  0x09,
  0xa1,
  0xa7,
  0x6d,
  0x6e,
  0xf5,
  0xd9,
  0x53,
  0xea,
  0xd9,
  0xf3,
  0x13,
  0xd8,
  0x66,
  0x8b,
  0xd8,
  0xd3,
  0x23,
  0x3a,
  0x44,
  0x94,
  0x5b,
  0x0f,
  0x4b,
  0xe6,
  0x9b,
  0x93,
  0xd3,
  0x83,
  0xf1,
  0xf2,
  0xa3,
  0x13,
  0x6b,
  0xa7,
  0x3d,
  0xe6,
  0x31,
  0xe6,
  0x77,
  0x2c,
  0xaa,
  0x44,
  0xa1,
  0x62,
  0x33,
  0x10,
  0x08,
  0xca,
  0x7f,
  0xf7,
  0x43,
  0xba,
  0x11,
  0x63,
  0xbb,
  0x41,
  0xd0,
  0xce,
  0xee,
  0x35,
  0x40,
  0x09,
  0x3c,
  0xcb,
  0x57,
  0x9a,
  0x5c,
  0xa6,
  0x02,
  0x9c,
  0x92,
  0x9d,
  0x78,
  0x1d,
  0x4b,
  0xed,
  0x50,
  0xdb,
  0x7e,
  0x70,
  0x94,
  0x8f,
  0x4b,
  0xeb,
  0x13,
  0x7d,
  0xbb,
  0xe7,
  0x13,
  0xe8,
  0xb7,
  0x6c,
  0x4a,
  0x5c,
  0xc2,
  0x2b,
  0x6a,
  0x95,
  0xa0,
  0x50,
  0xdc,
  0xad,
  0xae,
  0xc7,
  0xbf,
  0xd2,
  0x53,
  0xb5,
  0x7f,
  0x1d,
  0x9c,
  0x20,
  0xb5,
  0x31,
  0xaa,
  0xae,
  0x98,
  0x18,
  0x2d,
  0xde,
  0x6f,
  0xd7,
  0x4d,
  0x81,
  0x63,
  0x4a,
  0x5b,
  0x38,
  0x43,
  0x79,
  0x0c,
  0x66,
  0x6b,
  0x80,
  0xe7,
  0x00,
  0x2f,
  0xc3,
  0x33,
  0xfd,
  0x69,
  0xd1,
  0x9e,
  0x2f,
  0xce,
  0x07,
  0x13,
  0xc1,
  0x37,
  0xb4,
  0x89,
  0x67,
  0x1a,
  0x43,
  0xd1,
  0xb9,
  0xe6,
  0x5a,
  0xd8,
  0x8e,
  0x33,
  0x6b,
  0x24,
  0x7a,
  0x26,
  0x2f,
  0xf5,
  0x4a,
  0xce,
  0xe5,
  0xe6,
  0x80,
  0x2e,
  0x89,
  0x4f,
  0xf3,
  0xe0,
  0xb5,
  0xd6,
  0x37,
  0x59,
  0x47,
  0x1e,
  0x9f,
  0x6a,
  0xab,
  0xc7,
  0x7c,
  0x15,
  0xdc,
  0x40,
  0x12,
  0x38,
  0xf2,
  0xce,
  0x22,
  0xa3,
  0xf2,
  0x02,
  0xa4,
  0x9d,
  0x33,
  0x6f,
  0xdd,
  0xa8,
  0x2d,
  0x94,
  0x6a,
  0xd5,
  0x7b,
  0x54,
  0x48,
  0x43,
  0xe0,
  0xb8,
  0xd6,
  0x7c,
  0x14,
  0xe1,
  0xb1,
  0xbe,
  0xf6,
  0x67,
  0xec,
  0xb2,
  0x2e,
  0x44,
  0x76,
  0x50,
  0x30,
  0xd0,
  0xcb,
  0xbe,
  0xa3,
  0x50,
  0x82,
  0x7c,
  0xc6,
  0x8e,
  0xb5,
  0x6e,
  0x34,
  0x7d,
  0x29,
  0x11,
  0xe3,
  0xfc,
  0x18,
  0xc9,
  0xce,
  0x40,
  0x5d,
  0xbd,
  0x01,
  0x0a,
  0xcf,
  0x96,
  0x74,
  0xe8,
  0x34,
  0x24,
  0xe9,
  0x9e,
  0x07,
  0x2d,
  0x3e,
  0x9b,
  0x95,
  0xad,
  0x1b,
  0xe5,
  0xf2,
  0x22,
  0x61,
  0x50,
  0xb1,
  0xcb,
  0xa8,
  0xfc,
  0x11,
  0x2e,
  0x77,
  0x4b,
  0x80,
  0x1b,
  0xd7,
  0x22,
  0x5f,
  0x9d,
  0x7d,
  0xd8,
  0xb8,
  0x3b,
  0xf7,
  0xd5,
  0xfc,
  0xb7,
  0x51,
  0x10,
  0x87,
  0xea,
  0x00,
  0xce,
  0xd5,
  0x23,
  0x85,
  0x80,
  0x85,
  0x41,
  0x70,
  0xae,
  0x2c,
  0xd5,
  0x83,
  0x3c,
  0x75,
  0xa0,
  0x11,
  0xd5,
  0x25,
  0xac,
  0x0d,
  0xfa,
  0x40,
  0x72,
  0x67,
  0xee,
  0x88,
  0x35,
  0x3f,
  0x57,
  0x7c,
  0xbb,
  0xeb,
  0xd2,
  0x35,
  0x79,
  0x10,
  0x69,
  0x07,
  0xcc,
  0x7b,
  0xcf,
  0x8e,
  0x1e,
  0x67,
  0x87,
  0x33,
  0x63,
  0x3d,
  0x4e,
  0xcc,
  0xd8,
  0xbc,
  0x18,
  0xb7,
  0x11,
  0xf5,
  0x96,
  0x7c,
  0x19,
  0x1d,
  0xe2,
  0xa7,
  0x18,
  0xa1,
  0x60,
  0x76,
  0xf9,
  0xdf,
  0x4e,
  0xbe,
  0x5d,
  0x70,
  0x07,
  0xad,
  0x7b,
  0x18,
  0xd8,
  0xc0,
  0x72,
  0x63,
  0x47,
  0x22,
  0xd9,
  0xfe,
  0xeb,
  0x14,
  0xce,
  0xad,
  0x40,
  0xc8,
  0x83,
  0x8a,
  0x1d,
  0xab,
  0xf2,
  0xa9,
  0xf2,
  0xf2,
  0x50,
  0xe8,
  0x85,
  0x7d,
  0xf5,
  0x07,
  0x8b,
  0x3b,
  0x72,
  0xa2,
  0xde,
  0x7b,
  0x34,
  0xb9,
  0x2a,
  0x2e,
  0xc0,
  0xdf,
  0xb5,
  0xc9,
  0x7d,
  0xa8,
  0x23,
  0xa6,
  0x82,
  0x0c,
  0x46,
  0x67,
  0x6f,
  0xb2,
  0xa0,
  0x45,
  0xc1,
  0x69,
  0xb2,
  0xf1,
  0xf8,
  0x94,
  0xd2,
  0x05,
  0x7f,
  0xb2,
  0x94,
  0x1f,
  0xf0,
  0xa7,
  0x9f,
  0x4b,
  0x44,
  0x4c,
  0x6b,
  0xbb,
  0xc9,
  0xdb,
  0x89,
  0x43,
  0x62,
  0xb1,
  0xb5,
  0xcf,
  0xe0,
  0xc2,
  0x65,
  0xf8,
  0xc7,
  0x4d,
  0x07,
  0xef,
  0x09,
  0xcc,
  0xeb,
  0x28,
  0x76,
  0xa0,
  0xfb,
  0x74,
  0xd5,
  0x17,
  0x13,
  0x9d,
  0xb0,
  0xc2,
  0x73,
  0x63,
  0x86,
  0xd5,
  0x44,
  0x69,
  0xf8,
  0x4f,
  0xbf,
  0xe3,
  0xf6,
  0xaf,
  0x6f,
  0x63,
  0xc9,
  0xdf,
  0x04,
  0x31,
  0x4b,
  0x9c,
  0x35,
  0x0d,
  0x0f,
  0x90,
  0xfa,
  0x28,
  0xf6,
  0x27,
  0xbe,
  0x77,
  0x69,
  0x85,
  0xb5,
  0xda,
  0x2a,
  0x2b,
  0x1b,
  0xeb,
  0xb4,
  0xe5,
  0xe4,
  0x18,
  0xec,
  0x8f,
  0x92,
  0xde,
  0xcd,
  0x71,
  0xc9,
  0xe3,
  0x6c,
  0x0b,
  0x46,
  0x7f,
  0x72,
  0x8b,
  0xdf,
  0xb2,
  0x5d,
  0x6f,
  0xb6,
  0x4b,
  0x42,
  0xc1,
  0xa6,
  0x1f,
  0xae,
  0x92,
  0x38,
  0x20,
  0xa3,
  0x0b,
  0x4a,
  0x11,
  0xe9,
  0xdf,
  0x6e,
  0xa4,
  0xff,
  0x4a,
  0x92,
  0x79,
  0x38,
  0x44,
  0x73,
  0x53,
  0x2b,
  0xc0,
  0x44,
  0x35,
  0x26,
  0x2b,
  0x32,
  0x3c,
  0x73,
  0xb7,
  0xcd,
  0x27,
  0xac,
  0xbd,
  0x7b,
  0x16,
  0x9a,
  0x55,
  0xaa,
  0xe4,
  0xaf,
  0xbe,
  0xac,
  0x61,
  0x41,
  0xa9,
  0x1a,
  0x4f,
  0xa2,
  0xe7,
  0x52,
  0xaf,
  0x8b,
  0x3f,
  0x22,
  0xbe,
  0x70,
  0xd4,
  0x74,
  0x7e,
  0xd7,
  0xbe,
  0x05,
  0x82,
  0xad,
  0x96,
  0x03,
  0x58,
  0xd0,
  0x28,
  0x93,
  0xa8,
  0xc2,
  0x99,
  0x87,
  0x1c,
  0x92,
  0x5e,
  0x52,
  0x94,
  0x17,
  0xdc,
  0x40,
  0x6d,
  0xae,
  0xb9,
  0xf4,
  0xd7,
  0x69,
  0x96,
  0xb2,
  0x4e,
  0xc4,
  0x2f,
  0x0a,
  0x3c,
  0xdc,
  0xfc,
  0xad,
  0x29,
  0x63,
  0x5a,
  0x78,
  0x93,
  0x5c,
  0xae,
  0x3f,
  0x65,
  0xb6,
  0x37,
  0x2a,
  0xec,
  0x54,
  0xff,
  0xfc,
  0x63,
  0x24,
  0x1f,
  0xec,
  0x55,
  0xbe,
  0xe7,
  0xfd,
  0x78,
  0x0c,
  0x40,
  0xd2,
  0xa1,
  0x1a,
  0x8d,
  0x72,
  0x3e,
  0x46,
  0x61,
  0x3a,
  0x90,
  0xc4,
  0x41,
  0xc5,
  0xeb,
  0xc7,
  0x17,
  0xd5,
  0xce,
  0x94,
  0x43,
  0x11,
  0x12,
  0xb7,
  0x69,
  0xd3,
  0xc4,
  0x52,
  0x44,
  0x64,
  0xf2,
  0x5e,
  0x3f,
  0xbf,
  0x2d,
  0x72,
  0xb9,
  0x1e,
  0x55,
  0xc8,
  0x73,
  0x07,
  0x11,
  0x8a,
  0x01,
  0xad,
  0xd8,
  0xa2,
  0xa5,
  0x35,
  0x31,
  0x27,
  0xfa,
  0x65,
  0x50,
  0x81,
  0xae,
  0xcb,
  0x12,
  0x10,
  0x6a,
  0x77,
  0x69,
  0xca,
  0xf2,
  0x25,
  0xc8,
  0xad,
  0xc3,
  0xf6,
  0x4e,
  0xce,
  0x54,
  0xe7,
  0x06,
  0x68,
  0xaf,
  0xc8,
  0xc3,
  0x24,
  0x0a,
  0x38,
  0xf4,
  0xd7,
  0xfc,
  0xa1,
  0x13,
  0xa7,
  0x06,
  0x15,
  0x9f,
  0xe4,
  0xf3,
  0x64,
  0x70,
  0xbe,
  0xe1,
  0xf0,
  0xd4,
  0x67,
  0xae,
  0xf4,
  0x3b,
  0x43,
  0xe7,
  0x5c,
  0x83,
  0x60,
  0xe0,
  0x5a,
  0x10,
  0xc6,
  0xbb,
  0x42,
  0x81,
  0xb5,
  0x44,
  0xe9,
  0x5e,
  0xb2,
  0x4a,
  0x46,
  0xb6,
  0x42,
  0x8d,
  0x3f,
  0x48,
  0x53,
  0x8c,
  0xda,
  0xdf,
  0x84,
  0xcf,
  0x45,
  0xa8,
  0x78,
  0x8f,
  0xd8,
  0xe0,
  0x66,
  0x86,
  0xcb,
  0x9a,
  0x3b,
  0x90,
  0xe6,
  0x12,
  0xcb,
  0x49,
  0x45,
  0x9e,
  0x9b,
  0x84,
  0x71,
  0xb5,
  0xbf,
  0xab,
  0x2b,
  0xe3,
  0xb2,
  0x3d,
  0xbb,
  0x3e,
  0x91,
  0x46,
  0xee,
  0x35,
  0x0c,
  0x08,
  0xb4,
  0xe3,
  0x5c,
  0x75,
  0x6a,
  0xbb,
  0x7b,
  0x92,
  0xf7,
  0xe2,
  0x0d,
  0x93,
  0xaa,
  0xc7,
  0xf4,
  0xc7,
  0x6b,
  0x58,
  0x5b,
  0xdd,
  0x76,
  0xd3,
  0xb2,
  0x66,
  0xe7,
  0x87,
  0x29,
  0xfe,
  0x99,
  0xd7,
  0x41,
  0x7b,
  0x58,
  0x06,
  0x4c,
  0x3a,
  0xeb,
  0x10,
  0x06,
  0x63,
  0xec,
  0x80,
  0x4f,
  0x71,
  0xc2,
  0x94,
  0xf1,
  0xa3,
  0x57,
  0x04,
  0xed,
  0xb2,
  0xa0,
  0xd5,
  0x46,
  0xdb,
  0x87,
  0x7c,
  0x1d,
  0xef,
  0xea,
  0x40,
  0x96,
  0x15,
  0x72,
  0x1c,
  0x6c,
  0xd2,
  0x8b,
  0xe8,
  0xb8,
  0xb0,
  0x64,
  0xd8,
  0x49,
  0x02,
  0xf0,
  0xd6,
  0xef,
  0x3a,
  0x6a,
  0x93,
  0x82,
  0x83,
  0xcd,
  0x45,
  0xc6,
  0xaa,
  0x02,
  0x88,
  0xeb,
  0x0a,
  0xe6,
  0xd2,
  0x71,
  0xbe,
  0x58,
  0xda,
  0x9d,
  0x7d,
  0xbf,
  0x85,
  0x5d,
  0x2e,
  0x75,
  0x8d,
  0x53,
  0xf5,
  0x8c,
  0x38,
  0xf2,
  0xc9,
  0x21,
  0x97,
  0xf7,
  0x10,
  0xb1,
  0xcc,
  0x2b,
  0x7a,
  0x6c,
  0x9c,
  0x12,
  0x3d,
  0xda,
  0xf9,
  0xb6,
  0xf6,
  0xc7,
  0x4d,
  0xa8,
  0x74,
  0x3c,
  0xb6,
  0xca,
  0x62,
  0xf7,
  0x15,
  0x14,
  0xea,
  0xa5,
  0x16,
  0x11,
  0x71,
  0xf1,
  0xf8,
  0xe2,
  0x85,
  0x9e,
  0x92,
  0xc1,
  0x97,
  0x2d,
  0x99,
  0xf7,
  0x9f,
  0x28,
  0x30,
  0x1d,
  0xae,
  0xab,
  0xde,
  0xd1,
  0xab,
  0xfd,
  0x89,
  0x07,
  0x49,
  0x55,
  0xe0,
  0xb7,
  0x9a,
  0xed,
  0x77,
  0x19,
  0x39,
  0x8b,
  0x15,
  0xb5,
  0x06,
  0xbd,
  0x8f,
  0xbc,
  0xae,
  0x3b,
  0xc7,
  0x2a,
  0xbd,
  0x8a,
  0xc7,
  0xce,
  0x77,
  0xfc,
  0x17,
  0xb9,
  0xf7,
  0x95,
  0x04,
  0xf1,
  0xb2,
  0x4c,
  0x10,
  0x37,
  0xf3,
  0x6d,
  0xcb,
  0x6c,
  0xfc,
  0x5e,
  0xc7,
  0xf7,
  0xe6,
  0xdf,
  0x0f,
  0xd3,
  0x8e,
  0x59,
  0xb7,
  0xec,
  0x1a,
  0x75,
  0x8b,
  0xce,
  0xf9,
  0x25,
  0x2d,
  0x02,
  0xcc,
  0x5f,
  0xa0,
  0xe4,
  0x86,
  0xf9,
  0x08,
  0xcb,
  0x89,
  0x2b,
  0x79,
  0x8c,
  0xbd,
  0x1e,
  0x8a,
  0xe6,
  0xde,
  0x86,
  0xb6,
  0xa5,
  0x06,
  0x07,
  0x8b,
  0xb4,
  0x3f,
  0x6e,
  0x67,
  0x2b,
  0xe9,
  0xfd,
  0xd8,
  0xf9,
  0x7f,
  0x65,
  0xeb,
  0x7e,
  0x5e,
  0x9d,
  0x26,
  0xed,
  0x1d,
  0xe4,
  0xad,
  0x12,
  0xde,
  0x80,
  0x67,
  0xc3,
  0xb1,
  0x00,
  0x90,
  0x1f,
  0xd2,
  0x47,
  0x15,
  0x63,
  0xda,
  0xa0,
  0x1b,
  0xc1,
  0x83,
  0x0a,
  0x39,
  0x9a,
  0x1d,
  0x2e,
  0x46,
  0xe5,
  0x43,
  0x2e,
  0x80,
  0x72,
  0x50,
  0xb1,
  0xee,
  0x67,
  0xd0,
  0xbc,
  0xeb,
  0xa4,
  0xbe,
  0x21,
  0x47,
  0x55,
  0x36,
  0xe9,
  0x9d,
  0x18,
  0x01,
  0xcf,
  0xd2,
  0x28,
  0x74,
  0xf5,
  0x62,
  0xf4,
  0x7b,
  0xc5,
  0xac,
  0xd4,
  0x9a,
  0xb8,
  0xe6,
  0x41,
  0x84,
  0x2e,
  0x2b,
  0xb7,
  0x59,
  0x66,
  0x37,
  0x38,
  0xb7,
  0xa5,
  0x44,
  0x99,
  0xd9,
  0x18,
  0xe7,
  0x5e,
  0x2d,
  0x09,
  0x17,
  0xdd,
  0x8a,
  0x1c,
  0xab,
  0x03,
  0x36,
  0x4b,
  0x13,
  0x09,
  0x85,
  0x36,
  0x31,
  0x20,
  0xe4,
  0x37,
  0xf2,
  0xfb,
  0x3f,
  0x04,
  0xf8,
  0xff,
  0xee,
  0x39,
  0xb9,
  0xc7,
  0x93,
  0xd4,
  0x99,
  0x14,
  0x01,
  0x67,
  0x33,
  0x5c,
  0xa0,
  0xb8,
  0x75,
  0x14,
  0x0b,
  0x24,
  0xf5,
  0x97,
  0x42,
  0x57,
  0x85,
  0x5d,
  0xf8,
  0x3f,
  0x62,
  0x83,
  0xd0,
  0xa1,
  0x9f,
  0x20,
  0x8d,
  0x58,
  0x51,
  0x29,
  0x31,
  0xe8,
  0x6e,
  0xc1,
  0x89,
  0x95,
  0xa3,
  0xdb,
  0xb0,
  0xd6,
  0x2c,
  0x05,
  0x9c,
  0x37,
  0x67,
  0xc3,
  0xf2,
  0xdf,
  0xbc,
  0xa5,
  0x09,
  0xa5,
  0x18,
  0x98,
  0x14,
  0x63,
  0x4a,
  0x2b,
  0x58,
  0xda,
  0xd0,
  0xcd,
  0x1d,
  0xce,
  0x1a,
  0xca,
  0x04,
  0xc7,
  0x99,
  0xfd,
  0xbd,
  0x9d,
  0x55,
  0x0f,
  0xab,
  0x71,
  0xea,
  0x50,
  0x59,
  0x0a,
  0xdc,
  0x5e,
  0x94,
  0x4e,
  0x0f,
  0x1a,
  0xde,
  0x1d,
  0xa0,
  0x8c,
  0x31,
  0xea,
  0x09,
  0x93,
  0x60,
  0xb9,
  0x78,
  0x08,
  0xa8,
  0x6c,
  0x04,
  0xb3,
  0x3b,
  0x24,
  0xbf,
  0x37,
  0xa3,
  0x72,
  0xed,
  0x30,
  0x1a,
  0x51,
  0xee,
  0x56,
  0xaa,
  0xac,
  0xa6,
  0xe0,
  0xad,
  0x00,
  0x3e,
  0x0b,
  0x6c,
  0x2d,
  0xc1,
  0xd2,
  0xc6,
  0xd3,
  0xb8,
  0x6d,
  0x45,
  0xba,
  0x37,
  0xd6,
  0x3e,
  0xec,
  0x8c,
  0x8e,
  0x07,
  0xb5,
  0xc3,
  0x00,
  0xdf,
  0x40,
  0xa9,
  0x77,
  0xbc,
  0xe7,
  0xd0,
  0xef,
  0x35,
  0x83,
  0x00,
  0x30,
  0xd7,
  0x53,
  0x02,
  0x8b,
  0x45,
  0xb8,
  0x7a,
  0x79,
  0x04,
  0xb8,
  0x09,
  0x67,
  0xa6,
  0xa6,
  0x53,
  0xbc,
  0x77,
  0x34,
  0xa1,
  0x25,
  0xb7,
  0xd0,
  0x75,
  0x86,
  0x7c,
  0x59,
  0x1d,
  0xd9,
  0xdd,
  0xc5,
  0xbe,
  0x0b,
  0x21,
  0x99,
  0x48,
  0x60,
  0x9d,
  0x5a,
  0x63,
  0x01,
  0xbc,
  0xa9,
  0x73,
  0x04,
  0x66,
  0xe2,
  0x51,
  0xd1,
  0x60,
  0x82,
  0x88,
  0x20,
  0xd0,
  0x86,
  0xce,
  0xca,
  0x8d,
  0xaa,
  0x25,
  0xb1,
  0x8a,
  0xff,
  0x12,
  0x4c,
  0x81,
  0xb3,
  0x83,
  0x0b,
  0xd0,
  0x0d,
  0xdd,
  0x84,
  0x7c,
  0x92,
  0x28,
  0x21,
  0x34,
  0x78,
  0xe4,
  0xfb,
  0x90,
  0x19,
  0x35,
  0xd9,
  0x16,
  0x80,
  0xe2,
  0xd5,
  0x07,
  0x04,
  0xd6,
  0x9e,
  0x85,
  0x93,
  0x63,
  0x36,
  0x96,
  0xd0,
  0xcd,
  0x9b,
  0x31,
  0xa3,
  0xfc,
  0xb5,
  0x8c,
  0xca,
  0xd1,
  0x05,
  0x30,
  0x9c,
  0x9f,
  0xbf,
  0x8a,
  0x83,
  0x40,
  0xf4,
  0x75,
  0xed,
  0x4c,
  0xa3,
  0x11,
  0x57,
  0xb5,
  0x0c,
  0xb8,
  0x40,
  0x38,
  0xae,
  0xdd,
  0x9f,
  0xb5,
  0x89,
  0x47,
  0xad,
  0xf8,
  0x23,
  0x97,
  0xbd,
  0x54,
  0x18,
  0x9a,
  0x3f,
  0x76,
  0x82,
  0x8b,
  0x8e,
  0x60,
  0x43,
  0x3c,
  0xba,
  0x78,
  0x92,
  0x2a,
  0xa0,
  0x51,
  0xa7,
  0xf3,
  0x73,
  0xf1,
  0x67,
  0x7b,
  0xd9,
  0x72,
  0x40,
  0x7b,
  0xb8,
  0xcd,
  0xaa,
  0x79,
  0x56,
  0x61,
  0xb8,
  0xbe,
  0xd7,
  0x54,
  0xd2,
  0x7a,
  0xb7,
  0x00,
  0xac,
  0x7b,
  0x3a,
  0x42,
  0x9b,
  0x36,
  0x13,
  0x6e,
  0x80,
  0x1b,
  0x0f,
  0xc7,
  0x47,
  0x71,
  0xee,
  0x02,
  0x32,
  0x82,
  0x25,
  0xcd,
  0x4f,
  0xe0,
  0x85,
  0xa1,
  0x9e,
  0xd0,
  0x5e,
  0x55,
  0x25,
  0x04,
  0x6d,
  0x29,
  0x00,
  0x00,
  0x73,
  0xed,
  0x04,
  0x30,
  0x72,
  0x0d,
  0xed,
  0x54,
  0xbb,
  0xdc,
  0x41,
  0xf9,
  0x0e,
  0xe8,
  0x2c,
  0x0a,
  0x96,
  0x30,
  0x57,
  0x08,
  0x2d,
  0xe8,
  0x72,
  0x79,
  0xe8,
  0xa9,
  0x67,
  0xc5,
  0x7a,
  0x2c,
  0x28,
  0xad,
  0x14,
  0x59,
  0x0d,
  0x7a,
  0x35,
  0xb5,
  0x5e,
  0xcb,
  0x14,
  0xb7,
  0xb2,
  0x9f,
  0xad,
  0x51,
  0x5a,
  0x67,
  0x44,
  0x0f,
  0x69,
  0x10,
  0x83,
  0x9e,
  0x7e,
  0xed,
  0x5a,
  0x9c,
  0x9b,
  0xd8,
  0xce,
  0xe4,
  0x6a,
  0x2d,
  0x59,
  0x34,
  0x55,
  0x66,
  0xe7,
  0x99,
  0x41,
  0x91,
  0x14,
  0x83,
  0x18,
  0x25,
  0x7e,
  0x32,
  0x70,
  0xff,
  0xcf,
  0x18,
  0x7c,
  0x64,
  0x89,
  0x73,
  0x5c,
  0x5d,
  0x74,
  0xaf,
  0x21,
  0x34,
  0x2b,
  0x27,
  0xc6,
  0xa8,
  0x69,
  0x45,
  0x61,
  0x99,
  0xd3,
  0x17,
  0xe6,
  0x1c,
  0xab,
  0x31,
  0x6d,
  0x0e,
  0xb6,
  0x76,
  0x3e,
  0xf8,
  0xfc,
  0x2d,
  0xfd,
  0x4c,
  0x62,
  0x82,
  0x86,
  0x00,
  0xa7,
  0xda,
  0x2a,
  0x25,
  0x92,
  0xab,
  0x0e,
  0x74,
  0x94,
  0x85,
  0x59,
  0x38,
  0x71,
  0x0f,
  0x3e,
  0xfa,
  0x42,
  0xe6,
  0xe7,
  0x01,
  0x73,
  0xa9,
  0x94,
  0xb2,
  0xfc,
  0x10,
  0xfa,
  0x03,
  0x3a,
  0xe8,
  0x43,
  0x1d,
  0x5a,
  0x38,
  0xfc,
  0x7a,
  0x90,
  0x98,
  0xd1,
  0xb0,
  0x95,
  0x59,
  0xaa,
  0x7e,
  0x4f,
  0x4f,
  0x18,
  0x77,
  0x5a,
  0x6f,
  0xc0,
  0xf1,
  0x0a,
  0xcd,
  0x38,
  0x88,
  0xc2,
  0x13,
  0x53,
  0xbe,
  0x8e,
  0xc7,
  0xe6,
  0x9c,
  0xa9,
  0x27,
  0x15,
  0x09,
  0x45,
  0x32,
  0x22,
  0x6f,
  0x1f,
  0xe8,
  0x8d,
  0xe7,
  0x04,
  0x7c,
  0x3c,
  0x59,
  0x03,
  0xe1,
  0x6a,
  0x2d,
  0x22,
  0x9d,
  0x1d,
  0x69,
  0x3f,
  0x34,
  0xe1,
  0x45,
  0x76,
  0xad,
  0x99,
  0x2d,
  0xa7,
  0x3a,
  0xce,
  0xda,
  0x0c,
  0x32,
  0x5f,
  0xa7,
  0x79,
  0x59,
  0x17,
  0x18,
  0xda,
  0x96,
  0x6f,
  0x49,
  0xa1,
  0xfa,
  0x1e,
  0x4b,
  0x8a,
  0x89,
  0xc1,
  0x64,
  0xbe,
  0x33,
  0x86,
  0x9d,
  0x55,
  0x67,
  0x8b,
  0x5e,
  0xc5,
  0x18,
  0xc5,
  0x72,
  0xc8,
  0x2d,
  0x8e,
  0xbf,
  0x9f,
  0xf0,
  0x3a,
  0x47,
  0xd6,
  0x3a,
  0x66,
  0x01,
  0xa2,
  0x73,
  0x46,
  0x8d,
  0x13,
  0x2b,
  0x52,
  0x1d,
  0x60,
  0x21,
  0xd6,
  0xea,
  0xa3,
  0xf5,
  0x85,
  0x89,
  0x6a,
  0xe1,
  0x61,
  0x6c,
  0x46,
  0x98,
  0x7b,
  0x45,
  0x66,
  0x68,
  0x21,
  0x65,
  0x9d,
  0xf9,
  0xfe,
  0x68,
  0xa5,
  0x8d,
  0x46,
  0xa9,
  0x38,
  0x3d,
  0x1f,
  0x43,
  0x24,
  0x49,
  0x39,
  0x3f,
  0x7e,
  0x39,
  0xcc,
  0x2a,
  0x6a,
  0x86,
  0xbc,
  0xb5,
  0x47,
  0xf7,
  0x3f,
  0x33,
  0x85,
  0xe4,
  0x5c,
  0x73,
  0xd7,
  0x0d,
  0x1c,
  0x34,
  0xde,
  0xee,
  0x4f,
  0x22,
  0x22,
  0xdb,
  0xae,
  0x3f,
  0xc5,
  0xa6,
  0x15,
  0x68,
  0x0c,
  0x66,
  0x87,
  0x4d,
  0x55,
  0x39,
  0x3d,
  0x06,
  0x29,
  0x6a,
  0x36,
  0x29,
  0x67,
  0x33,
  0x23,
  0x48,
  0x05,
  0x7f,
  0x2e,
  0x6e,
  0xef,
  0xb4,
  0x67,
  0x5e,
  0x02,
  0x53,
  0x20,
  0x2b,
  0x06,
  0xca,
  0xfb,
  0x0e,
  0xd9,
  0x1f,
  0xa9,
  0xfa,
  0x24,
  0x17,
  0x3d,
  0x30,
  0xfe,
  0xd9,
  0x5b,
  0x7f,
  0x57,
  0xe0,
  0x2e,
  0x3d,
  0x8a,
  0xfe,
  0xa0,
  0xb1,
  0x7e,
  0x39,
  0x85,
  0x83,
  0x4a,
  0x36,
  0xdc,
  0xa2,
  0x60,
  0xd6,
  0x07,
  0x44,
  0xae,
  0x29,
  0xd7,
  0xce,
  0x30,
  0x96,
  0xf4,
  0x38,
  0x9b,
  0x39,
  0x91,
  0x31,
  0xf4,
  0xa5,
  0xe2,
  0xbf,
  0xc0,
  0x45,
  0xde,
  0xb7,
  0x64,
  0x1e,
  0x6d,
  0x31,
  0xf9,
  0xe0,
  0x05,
  0xec,
  0x38,
  0xc3,
  0x6c,
  0xa5,
  0xd5,
  0xc5,
  0x9f,
  0x89,
  0xcc,
  0x5a,
  0xd3,
  0x5c,
  0x33,
  0x7c,
  0x72,
  0xb2,
  0xcc,
  0x6d,
  0x8f,
  0xab,
  0xce,
  0x2d,
  0xdf,
  0x3b,
  0xda,
  0x50,
  0x86,
  0x79,
  0x30,
  0x32,
  0x22,
  0xef,
  0x06,
  0xef,
  0x3d,
  0x76,
  0x2a,
  0xd7,
  0xf2,
  0xd9,
  0xc3,
  0x60,
  0xf4,
  0xb4,
  0x06,
  0xdd,
  0xc4,
  0x0b,
  0x7c,
  0x9b,
  0xd5,
  0x2c,
  0xa4,
  0xb7,
  0x37,
  0x49,
  0x6c,
  0xd8,
  0x73,
  0x89,
  0xd3,
  0x32,
  0xbc,
  0x4f,
  0x78,
  0x72,
  0x46,
  0xd5,
  0xd7,
  0x6a,
  0x22,
  0xbd,
  0xdc,
  0xb7,
  0x94,
  0xff,
  0xc9,
  0x54,
  0x8b,
  0x35,
  0x77,
  0x63,
  0xe2,
  0x13,
  0xcb,
  0x89,
  0x69,
  0x53,
  0x05,
  0x45,
  0xc7,
  0x33,
  0xf6,
  0xc7,
  0xb3,
  0x74,
  0x8b,
  0x05,
  0x9b,
  0x44,
  0x35,
  0xeb,
  0x87,
  0xbf,
  0xb6,
  0x2e,
  0xc1,
  0x51,
  0x87,
  0xd3,
  0x32,
  0x03,
  0x90,
  0x65,
  0x21,
  0x47,
  0x21,
  0x41,
  0x23,
  0x2c,
  0xbf,
  0xc3,
  0xb2,
  0x2d,
  0xd2,
  0xf2,
  0x64,
  0x9f,
  0xe1,
  0x74,
  0xd9,
  0x23,
  0xe5,
  0xfc,
};

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */