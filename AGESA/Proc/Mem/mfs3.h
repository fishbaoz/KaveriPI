/* $NoKeywords:$ */
/**
 * @file
 *
 * mfS3.h
 *
 * S3 resume memory related functions.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/Feat/S3)
 * @e \$Revision: 281181 $ @e \$Date: 2013-12-18 02:18:55 -0600 (Wed, 18 Dec 2013) $
 *
 **/
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
* ***************************************************************************
*
*/

#ifndef _MFS3_H_
#define _MFS3_H_

/*----------------------------------------------------------------------------
 *   Mixed (DEFINITIONS AND MACROS / TYPEDEFS, STRUCTURES, ENUMS)
 *
 *----------------------------------------------------------------------------
 */

/*-----------------------------------------------------------------------------
 *                         DEFINITIONS AND MACROS
 *
 *-----------------------------------------------------------------------------
 */
#define PRESELFREF 0
#define POSTSELFREF 1
#define DCT0 0
#define DCT1 1
#define DCT0_MASK 0x1
#define DCT1_MASK 0x2
#define DCT0_NBPSTATE_SUPPORT_MASK 0x4
#define DCT1_NBPSTATE_SUPPORT_MASK 0x8
#define DCT0_DDR3_MASK 0x10
#define DCT1_DDR3_MASK 0x20
#define NODE_WITHOUT_DIMM_MASK 0x80
#define DCT0_ANY_DIMM_MASK 0x55
#define DCT1_ANY_DIMM_MASK 0xAA
#define ANY_DIMM_MASK 0xFF

#define DCT_PHY_FLAG 0
#define DCT_EXTRA_FLAG 1
#define SET_S3_SPECIAL_OFFSET(AccessType, Dct, Offset) ((AccessType << 11) | (Dct << 10) | Offset)

#define RESTORE_TRAINING_MODE 1
#define CAPSULE_REBOOT_MODE 2
#define S3_RESUME_MODE 4
/*----------------------------------------------------------------------------
 *                         TYPEDEFS, STRUCTURES, ENUMS
 *
 *----------------------------------------------------------------------------
 */
/// struct for all the descriptor for pre exit self refresh and post exit self refresh
typedef struct _DESCRIPTOR_GROUP {
  _4BYTE_ALIGN PCI_DEVICE_DESCRIPTOR PCIDevice[2];  ///< PCI device descriptor
  _4BYTE_ALIGN CONDITIONAL_PCI_DEVICE_DESCRIPTOR CPCIDevice[2];  ///< Conditional PCI device descriptor
  _4BYTE_ALIGN MSR_DEVICE_DESCRIPTOR MSRDevice[2];  ///< MSR device descriptor
  _4BYTE_ALIGN CONDITIONAL_MSR_DEVICE_DESCRIPTOR CMSRDevice[2];  ///< Conditional MSR device descriptor
} DESCRIPTOR_GROUP;

/// Northbridge block to be used in S3 resume and save.
typedef struct _S3_MEM_NB_BLOCK {
  _4BYTE_ALIGN UINT8 MemS3SpecialCaseHeapSize; ///< Heap size for the special case register heap.
  struct _MEM_NB_BLOCK *NBPtr;    ///< Pointer to the north bridge block.
  _4BYTE_ALIGN VOID (*MemS3ExitSelfRefReg) (MEM_NB_BLOCK *NBPtr, AMD_CONFIG_PARAMS *StdHeaderPtr); ///< S3 Exit self refresh register
  _4BYTE_ALIGN VOID (*MemS3GetConPCIMask) (MEM_NB_BLOCK *NBPtr, DESCRIPTOR_GROUP *DescriptPtr); ///< Get conditional mask for PCI register setting
  _4BYTE_ALIGN VOID (*MemS3GetConMSRMask) (MEM_NB_BLOCK *NBPtr, DESCRIPTOR_GROUP *DescriptPtr); ///< Get conditional mask for MSR register setting
  _4BYTE_ALIGN UINT16 (*MemS3GetRegLstPtr) (MEM_NB_BLOCK *NBPtr, DESCRIPTOR_GROUP *DescriptPtr); ///< Get register list pointer for both PCI and MSR register
  _4BYTE_ALIGN BOOLEAN (*MemS3Resume) (struct _S3_MEM_NB_BLOCK *S3NBPtr, UINT8 NodeID);///< Exit Self Refresh
  _4BYTE_ALIGN VOID (*MemS3RestoreScrub) (MEM_NB_BLOCK *NBPtr, UINT8 NodeID);///< Restore scrubber base
  _4BYTE_ALIGN AGESA_STATUS (*MemS3GetDeviceRegLst) (UINT32 ReigsterLstID, VOID **RegisterHeader, MEM_NB_BLOCK *NBPtr); ///< Get register list for a device
  _4BYTE_ALIGN BOOLEAN (*MemS3PspDetect) (MEM_NB_BLOCK *NBPtr); ///< PSP detection
} S3_MEM_NB_BLOCK;

/// Header for heap space to store the special case register.
typedef struct _S3_SPECIAL_CASE_HEAP_HEADER {
  _1BYTE_ALIGN UINT8 Node;  ///< Node ID for the the header
  _1BYTE_ALIGN UINT8 Offset;  ///< Offset for the target node
} S3_SPECIAL_CASE_HEAP_HEADER;

/// Flag for hob data save and restore
typedef enum _MEM_HOB_DATA_TYPE {
  S3_UMA_SIZE,  ///< UMA size
  S3_UMA_BASE,  ///< UMA base
  S3_UMA_MODE,  ///< UMA mode
  S3_SUB_4G_CACHE_TOP,  ///< Sub 4G Cache Top
  S3_SYSLIMIT,  ///< System limit
  S3_UMA_ATTRIBUTE,  ///< UMA attribute
  S3_VDDIO  ///< VDDIO
} MEM_HOB_DATA_TYPE;

/// Header for heap space to store reduced memory internal data
typedef struct _REDUCED_MEM_BLOCK_HEAP_HEADER {
  _2BYTE_ALIGN UINT16 Version;                 ///< Version of header
  _1BYTE_ALIGN   UINT8  NumNodes;                ///< Number of reduced NB blocks in the list
} REDUCED_MEM_BLOCK_HEAP_HEADER;

/// Reduced NB Block to store data during memory context save/restore
typedef struct _REDUCED_NB_BLOCK {
  _4BYTE_ALIGN UINT32 NodeMemSize;             ///< Base[47:16], total DRAM size controlled by both DCT0 and DCT1 of this Node.
  _4BYTE_ALIGN UINT32 NodeSysBase;             ///< System base of this node
  _1BYTE_ALIGN UINT8  NumDcts;                 ///< Number of reduced DCT blocks in the list
} REDUCED_NB_BLOCK;

/// Reduced DCT Block to store data during memory context save/restore
typedef struct _REDUCED_DCT_BLOCK {
  _1BYTE_ALIGN UINT8  Dct;                     ///< Dct Number
  _4BYTE_ALIGN UINT32 DctMemSize;              ///< Base[47:16], total DRAM size controlled by this DCT.
  _1BYTE_ALIGN UINT8  EnabledChipSels;         ///< Number of enabled chip selects on current DCT
  _1BYTE_ALIGN UINT8  BankAddrMap;             ///< Bank Address Mapping
  _1BYTE_ALIGN BOOLEAN BkIntDis;               ///< Bank interleave requested but not enabled on current DCT
} REDUCED_DCT_BLOCK;

/*----------------------------------------------------------------------------
 *                           FUNCTIONS PROTOTYPE
 *
 *----------------------------------------------------------------------------
 */
AGESA_STATUS
AmdMemS3Resume (
  IN   AMD_CONFIG_PARAMS *StdHeader
  );

AGESA_STATUS
MemS3ResumeInitNB (
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

AGESA_STATUS
MemS3Deallocate (
  IN   AMD_CONFIG_PARAMS *StdHeader
  );

AGESA_STATUS
MemFS3GetPciDeviceRegisterList (
  IN       PCI_DEVICE_DESCRIPTOR     *Device,
     OUT   PCI_REGISTER_BLOCK_HEADER **RegisterHdr,
  IN       AMD_CONFIG_PARAMS         *StdHeader
  );

AGESA_STATUS
MemFS3GetCPciDeviceRegisterList (
  IN       CONDITIONAL_PCI_DEVICE_DESCRIPTOR *Device,
     OUT   CPCI_REGISTER_BLOCK_HEADER        **RegisterHdr,
  IN       AMD_CONFIG_PARAMS                 *StdHeader
  );

AGESA_STATUS
MemFS3GetMsrDeviceRegisterList (
  IN       MSR_DEVICE_DESCRIPTOR     *Device,
     OUT   MSR_REGISTER_BLOCK_HEADER **RegisterHdr,
  IN       AMD_CONFIG_PARAMS         *StdHeader
  );

AGESA_STATUS
MemFS3GetCMsrDeviceRegisterList (
  IN       CONDITIONAL_MSR_DEVICE_DESCRIPTOR     *Device,
     OUT   CMSR_REGISTER_BLOCK_HEADER            **RegisterHdr,
  IN       AMD_CONFIG_PARAMS                     *StdHeader
  );

AGESA_STATUS
MemFS3GetDeviceList (
  IN OUT   DEVICE_BLOCK_HEADER **DeviceBlockHdrPtr,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

VOID
MemFS3Wait10ns (
  IN       UINT32 Count,
  IN OUT   MEM_DATA_STRUCT *MemPtr
  );

BOOLEAN
MemNS3ResumeUNb (
  IN OUT   S3_MEM_NB_BLOCK *S3NBPtr,
  IN       UINT8 NodeID
  );

VOID
MemNS3GetBitFieldNb (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
MemNS3SetBitFieldNb (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
MemNS3RestoreScrubNb (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 Node
  );

AGESA_STATUS
MemS3InitNB (
  IN OUT   S3_MEM_NB_BLOCK **S3NBPtr,
  IN OUT   MEM_DATA_STRUCT **MemPtr,
  IN OUT   MEM_MAIN_DATA_BLOCK *mmData,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

VOID
MemNS3SetPreDriverCalUnb (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

BOOLEAN
MemNS3DctCfgSelectUnb (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       VOID *Dct
  );

VOID
MemNS3GetNBPStateDepRegUnb (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
MemNS3SetNBPStateDepRegUnb (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
MemNS3SaveNBRegisterUnb (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
MemNS3RestoreNBRegisterUnb (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
MemNS3SetMemClkFreqValUnb (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
MemNS3ChangeMemPStateContextNb (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
MemNS3ForceNBP0Unb (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
MemNS3ReleaseNBPSUnb (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
MemNSaveHobDataUnb (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

VOID
MemNRestoreHobDataUnb (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );

AGESA_STATUS
AmdMemPspDetect (
  IN   AMD_CONFIG_PARAMS *StdHeader
  );

BOOLEAN
MemS3PspNotDetected (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
MemNS3ConfigureDisDllShutdownSrKV (
  IN       ACCESS_WIDTH AccessWidth,
  IN       PCI_ADDR Address,
  IN OUT   VOID *Value,
  IN OUT   VOID *ConfigPtr
  );
#endif //_MFS3_H_
