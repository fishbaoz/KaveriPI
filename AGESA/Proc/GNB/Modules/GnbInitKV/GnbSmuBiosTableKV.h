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
#ifndef _GNBSMUBIOSTABLE_H_
#define _GNBSMUBIOSTABLE_H_

#pragma pack (push, 1)


/// SMU BIOS Table structures

/// SMU PCIe PG Table
typedef struct {
  UINT8         CompControlLanePSD;  ///< Ccl PSD
  UINT8         CompControlLanePPD;  ///< Ccl PPD
  UINT8         CompControlLanePGDL; ///< Ccl PGDL
  UINT8         CompControlLanePGDH; ///< Ccl PGDH

  UINT8         CompControlLaneDDI;  ///< Ccl DDI
  UINT8         PllControlLanePSD;   ///< Pcl PSD
  UINT8         PllControlLanePPD;   ///< Pcl PPD
  UINT8         PllControlLanePGDL;  ///< Pcl PGDL

  UINT8         PllControlLanePGDH;  ///< Pcl PGDH
  UINT8         PllControlLaneDDI;   ///< Pcl DDI
  UINT8         padding[2];          ///< resvd
} SMU7_PCIEPGTABLE;

/// SMU Avs Table
typedef struct {
  UINT8         AvsEnabledForPstates;       ///< AvsEnabledForPstates
  UINT8         AvsOverrideEnabled;         ///< AvsOverrideEnabled
  UINT8         AvsPsmTempCompensation;     ///< AvsPsmTempCompensation
  UINT8         AvsOverrideOffset;          ///< AvsOverrideOffset

  UINT8         PsmCu[2][4];                ///< PsmCu

  UINT8         AvsNbEnabledForPstates;     ///< AvsNbEnabledForPstates
  UINT8         AvsNbOverrideEnabled;       ///< AvsNbOverrideEnabled
  UINT8         AvsNbPsmTempCompensation;   ///< AvsNbPsmTempCompensation
  UINT8         AvsNbOverrideOffset;        ///< AvsNbOverrideOffset

  UINT16        Cu0TemperatureSensor;       ///< Cu0TemperatureSensor
  UINT16        Cu1TemperatureSensor;       ///< Cu1TemperatureSensor

  UINT16        GnbTemperatureSensor;       ///< GnbTemperatureSensor
  UINT16        UnbTemperatureSensor;       ///< UnbTemperatureSensor

  UINT8         SpmiTestCode;               ///< SpmiTestCode
  UINT8         SpmiTestData;               ///< SpmiTestData
  UINT8         Padding[2];                 ///< Padding

  UINT32        SpmiSmcChainAddr;           ///< SpmiSmcChainAddr

  UINT8         SpmiStatusOpDone;           ///< SpmiStatusOpDone
  UINT8         SpmiStatusOpFailed;         ///< SpmiStatusOpFailed
  UINT8         Padding1[2];                ///< Padding1

} SMU7_AVSTABLE;

/// SMU Port 80 Table
typedef struct {
  UINT32        MmioAddress;                ///< MmioAddress      //DEF=0xFED80E44;
  UINT32        MemoryBaseHi;               ///< MemoryBaseHi
  UINT32        MemoryBaseLo;               ///< MemoryBaseLo
  UINT16        MemoryBufferSize;           ///< MemoryBufferSize
  UINT16        MemoryPosition;             ///< MemoryPosition
  UINT16        PollingInterval;            ///< PollingInterval  //DEF=1
  UINT8         EnableCsrShadow;            ///< EnableCsrShadow
  UINT8         EnableDramShadow;           ///< EnableDramShadow
} SMU7_PORT80MONITORTABLE;

/// SMU Bios Table
typedef struct {
  UINT16        TdcVrmLimit_IddNb;              ///< 10mA units
  UINT16        TdcVrmLimit_Idd;                ///< 10mA units

  UINT16        IddNbspike_OCP;                 ///< max IDDNB current for SVI2 current calculations
  UINT16        Iddspike_OCP;                   ///< max IDD current for SVI2 current calculations

  UINT8         Dimms;                          ///< number of DIMMs connected
  UINT8         spare;                          ///< spare
  UINT8         NbPstateAllCpusIdle;            ///< nbpstate to use when AllCpusIdle
  UINT8         PstateAllCpusIdle;              ///< pstate to use when AllCpusIdle

  UINT16        Vddr;                           ///< voltages Q1.15
  UINT16        Vddio;                          ///< voltage Q1.15

  UINT16        MemorySpeedLow;                 ///< MemorySpeedLow (MHz)
  UINT16        MemorySpeedHigh;                ///< MemorySpeedHigh (MHz)

  UINT16        TargetTdp;                      ///< TargetTdp watts Q8.8

  UINT8         TcenAliveCoreTcenId;            ///< TcenAliveCoreTcenId
  UINT8         TcenAliveGnbTcenId;             ///< TcenAliveGnbTcenId

  UINT16        PkgPwrLimitAC;                  ///< Package Power Limit AC watts Q8.8
  UINT16        PkgPwrLimitDC;                  ///< Package Power Limit DC watts Q8.8

  UINT32        Reserved[7];                    ///< Resvd

  SMU7_PCIEPGTABLE        PCIePGTable;          ///< SMU PCIe PG Table
  SMU7_AVSTABLE           AvsTable;             ///< SMU Avs Table
  SMU7_PORT80MONITORTABLE Port80MonitorTable;   ///< SMU Port 80 Table

} SMU7_BIOSTABLE;

#pragma pack (pop)


VOID
GnbInitSmuBiosTableKV (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  );

AGESA_STATUS
GnbWriteSmuBiosTableKV (
  IN       AMD_CONFIG_PARAMS        *StdHeader
  );

VOID
GnbSmuBiosTableGetDimmInfoKV (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  );
#endif
