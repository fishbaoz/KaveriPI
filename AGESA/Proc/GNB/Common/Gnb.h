/* $NoKeywords:$ */
/**
 * @file
 *
 * Misc common definition
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
#ifndef _GNB_H_
#define _GNB_H_

#include  "Ids.h"

#pragma pack (push, 1)

#define GNB_DEADLOOP() \
{ \
  VOLATILE BOOLEAN k; \
  k = TRUE; \
  while (k) { \
  } \
}
#ifdef IDSOPT_TRACING_ENABLED
  #if (IDSOPT_TRACING_ENABLED == TRUE)
    #define GNB_TRACE_ENABLE
  #endif
#endif


#ifndef GNB_DEBUG_CODE
  #ifdef GNB_TRACE_ENABLE
    #define  GNB_DEBUG_CODE(Code) Code
  #else
    #define  GNB_DEBUG_CODE(Code)
  #endif
#endif

#ifndef MIN
  #define MIN(x, y) (((x) > (y))? (y):(x))
#endif

#ifndef MAX
  #define MAX(x, y) (((x) > (y))? (x):(y))
#endif

#define OFF 0

#define PVOID UINT64

#define STRING_TO_UINT32(a, b, c, d) ((UINT32) ((d << 24) | (c << 16) | (b << 8) | a))

#define GnbLibGetHeader(x)  ((AMD_CONFIG_PARAMS*) (x)->StdHeader)

#define AGESA_STATUS_UPDATE(Current, Aggregated) \
if (Current > Aggregated) { \
  Aggregated = Current; \
}

#ifndef offsetof
  #define offsetof(s, m) (UINTN)&(((s *)0)->m)
#endif


//Table properties

#define TABLE_PROPERTY_DEFAULT                     0x00000000ul
#define TABLE_PROPERTY_IGFX_DISABLED               0x00000001ul
#define TABLE_PROPERTY_IOMMU_DISABLED              0x00000002ul
#define TABLE_PROPERTY_LCLK_DEEP_SLEEP             0x00000004ul
#define TABLE_PROPERTY_ORB_CLK_GATING              0x00000008ul
#define TABLE_PROPERTY_IOC_LCLK_CLOCK_GATING       0x00000010ul
#define TABLE_PROPERTY_IOC_SCLK_CLOCK_GATING       0x00000020ul
#define TABLE_PROPERTY_IOMMU_L1_CLOCK_GATING       0x00000040ul
#define TABLE_PROPERTY_IOMMU_L2_CLOCK_GATING       0x00000080ul
#define TABLE_PROPERTY_BAPM                        0x00000100ul
#define TABLE_PROPERTY_SECONDARY_GNB               0x00000200ul
#define TABLE_PROPERTY_NMI_SYNCFLOOD               0x00000400ul
#define TABLE_PROPERTY_NBDPM                       0x00000800ul
#define TABLE_PROPERTY_LOADLINE_ENABLE             0x00001000ul
#define TABLE_PROPERTY_SMU_SCLK_CLOCK_GATING       0x00002000ul
#define TABLE_PROPERTY_CHUB_CLOCK_GATING           0x00004000ul
#define TABLE_PROPERTY_HYBRID_BOOST                0x00008000ul
#define TABLE_PROPERTY_LHTC                        0x00010000ul
#define TABLE_PROPERTY_SVI2                        0x00020000ul
#define TABLE_PROPERTY_TDC                         0x00040000ul
#define TABLE_PROPERTY_PKG_POWER_TRACKING          0x00080000ul
#define TABLE_PROPERTY_DGFX_DISABLED               0x00100000ul
#define TABLE_PROPERTY_ACP_CLOCK_GATING_DISABLED   0x00200000ul
#define TABLE_PROPERTY_ACP_POWER_GATING_ENABLED    0x00400000ul
#define TABLE_PROPERTY_ACP_POWER_GATING_DISABLED   0x00800000ul
#define TABLE_PROPERTY_IOMMU_L2_DEBUG_3_31         0x01000000ul
#define TABLE_PROPERTY_AXG_DISABLE                 0x02000000ul

//Register access flags Flags
#define GNB_REG_ACC_FLAG_S3SAVE                    0x00000001ul

// Gnb PCIe Master PLL
#define GNB_PCIE_MASTERPLL_A   0xA
#define GNB_PCIE_MASTERPLL_B   0xB
#define GNB_PCIE_MASTERPLL_C   0xC
#define GNB_PCIE_MASTERPLL_D   0xD

/// LCLK DPM enable control
typedef enum {
  LclkDpmDisabled,          ///<LCLK DPM disabled
  LclkDpmRcActivity,        ///<LCLK DPM enabled and use Root Complex Activity monitor method
} LCLK_DPM_MODE;


/// Power gate data setting (do not change this structure definition)
typedef struct {
  UINT16  MothPsoPwrup;         ///< Mother Timer Powerup
  UINT16  MothPsoPwrdn;         ///< Mother Timer Powerdown
  UINT16  DaugPsoPwrup;         ///< Daughter Timer Powerup
  UINT16  DaugPsoPwrdn;         ///< Daughter Timer Powerdown
  UINT16  ResetTimer;           ///< Reset Timer
  UINT16  IsoTimer;             ///< Isolation Timer
} POWER_GATE_DATA;


/// Topology information
typedef struct {
  BOOLEAN   PhantomFunction;    ///< PCIe topology have device with phantom function
  BOOLEAN   PcieToPciexBridge;  ///< PCIe topology have device with Pcieto Pcix bridge
} GNB_TOPOLOGY_INFO;


/// GNB installable services
typedef enum {
  GnbPcieFamConfigService,                ///< PCIe config service
  GnbPcieFamInitService,                  ///< PCIe Init service
  GnbPcieFamDebugService,                 ///< PCIe Debug service
  GnbRegisterAccessService,               ///< GNB register access service
  GnbIommuService,                        ///< GNB IOMMU config service
  GnbUraService,                          ///< GNB Ura service
  GfxFamService,                          ///< GFX Family service
  GnbPcieMaxPayloadService,               ///< GNB MaxPayload service
  GnbFamTsService,                        ///< GNB TimeStamp service
  GnbFamAlibService,                      ///< GNB Alib service
  GnbFamBuildOptionPointer                ///< GNB Build Options service
} GNB_SERVICE_ID;

/// GNB service entry
typedef struct _GNB_SERVICE {
  GNB_SERVICE_ID        ServiceId;        ///< Service ID
  UINT64                Family;           ///< CPU family
  VOID                  *ServiceProtocol; ///< Service protocol
  struct  _GNB_SERVICE  *NextService;     ///< Pointer to next service
} GNB_SERVICE;

/// GNB SMU Firmware Pointers
typedef struct _GNB_FW_PTRS {
  VOID                  *FirmwareType1;   ///< Pointer to Type 1 SMU Firmware
  VOID                  *FirmwareType2;   ///< Poitner to Type 2 SMU Firmware
} GNB_FW_PTRS;

///
/// ACP Engine Structure
///
typedef struct {
  OUT UINT64 AcpEngineBase;           ///< AcpEngineBase[63:0] = Addr[63:0]
  OUT UINT32 AcpEngineSize;           ///< AcpEngineSize[31:0] = Addr[31:0]
} ACP_ENGINE_T;

#define GNB_STRINGIZE(x)                  #x
#define GNB_SERVICE_DEFINITIONS(x)        GNB_STRINGIZE (Services/x/x.h)
#define GNB_MODULE_DEFINITIONS(x)         GNB_STRINGIZE (Modules/x/x.h)
#define GNB_MODULE_INSTALL(x)             GNB_STRINGIZE (Modules/x/x##Install.h)
#pragma pack (pop)

#endif
