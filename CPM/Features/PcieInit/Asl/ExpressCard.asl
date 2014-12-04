/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM Express Card SSDT Table
 *
 * Contains CPM code to provide ASL code support for Express Card.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  PcieInit
 * @e \$Revision: 281158 $   @e \$Date: 2013-12-17 20:36:04 -0600 (Tue, 17 Dec 2013) $
 *
 */
/*****************************************************************************
 *
 * Copyright 2012 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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

DefinitionBlock (
    "ExpressCard.aml",      // Output file
    "SSDT",                 // Signature
    0x01,                   // DSDT Revision
    "AMD",                  // OEM ID
    "CPMXPRES",             // OEM Table ID
    0x1                     // OEM Revision
    )
{
    #define CPM_PCIE_ASL

    #include "CmnDef.asi"
    #include "CmnExt.asi"

    Scope(\_GPE)                                    // General-Purpose Event
    {
        Method(CpmExpressCardEventMethod)
        {
            Store(CpmReadTable(CpmExpressCardTable, CpmExpressCardBridgeDevice), Local0)
            Store(CpmReadTable(CpmExpressCardTable, CpmExpressCardBridgeFunction), Local1)
            Store(CpmReadTable(CpmExpressCardTable, CpmExpressCardEventPin), Local2)
            Store(CpmReadTable(CpmExpressCardTable, CpmExpressCardDeviceId), Local3)

            If (CpmReadGevent (Local2))             // hot remove
            {
                CpmWriteGeventTriggerType (Local2, 0)
                CpmSetPostCode (0x0F04)
                CpmPcieHotplug(Local0, Local1, 0)
                CpmSetPostCode (0x0F05)
            }
            else                                    // hot insert
            {
                CpmWriteGeventTriggerType (Local2, 1)
                Store(CpmGetDeviceStatus(Local3), Local4)
                If (Local4)
                {
                    CpmSetPostCode (0x0204)
                    CpmPcieHotplug(Local0, Local1, 1)
                    CpmSetPostCode (0x0205)
                }
            }
            If (CondRefOf(\_SB.PCI0.CpmExpressCardBridge))
            {
                Notify(\_SB.PCI0.CpmExpressCardBridge, 0x00)
            }
        }

        Name(CpmWirelessButtonCurrentStatus, 0)     // Radio SW ON-OFF status

        Method(CpmWirelessButtonEventMethod)
        {
            Store(0, Local7)
            Store(CpmSaveContextTable, Local0)
            If (Local0)
            {
                Store(CpmReadTable(CpmSaveContextTable, CpmSaveContextBufferType), Local0)
                Store(CpmReadTable(CpmSaveContextTable, CpmSaveContextBufferOffset), Local1)
                Add(CpmAcpiMmioBaseAddr, Add(Multiply(Local0, 0x100), Local1), Local1)
                CpmWriteMem8(Local1, CpmWirelessButtonStatus, 0, 8, CpmWirelessButtonCurrentStatus)
            }
            Xor(CpmWirelessButtonCurrentStatus, 0x01, CpmWirelessButtonCurrentStatus)
            Store(CpmWirelessButtonTable, Local1)
            Store(CpmReadTable(CpmMainTable, CpmWirelessButtonEn), Local2)
            If(LAnd(Local1, Local2))
            {
                If(CpmWirelessButtonCurrentStatus)        // OFF, on boot up, at least one wireless device is on.
                {
                    CpmSetPostCode(0x4401)
                    // Power off Antenna
                    CpmSetDevicePower(CpmReadMem32(CpmWirelessButtonTable, CpmWirelessButtonDeviceIdRadio, 0, 8), 0)
                    CpmSetDevicePower(CpmReadMem32(CpmWirelessButtonTable, CpmWirelessButtonDeviceIdOther, 0, 8), 0)

                    If(LEqual(Local2, 2))                 // Power Control Enabled, Power Off devices
                    {
                        Store(CpmReadMem8(CpmWirelessButtonTable, CpmWirelessButtonBridge, 0, 5), Local0)
                        Store(CpmReadMem8(CpmWirelessButtonTable, CpmWirelessButtonBridge, 5, 3), Local1)
                        If(LOr(Local0, Local1))
                        {
                            CpmPcieHotplug(Local0, Local1, 0)
                            Or(Local7, 1, Local7)
                        }

                        Store(CpmReadMem8(CpmWirelessButtonTable, Add(CpmWirelessButtonBridge, 1), 0, 5), Local0)
                        Store(CpmReadMem8(CpmWirelessButtonTable, Add(CpmWirelessButtonBridge, 1), 5, 3), Local1)
                        If(LOr(Local0, Local1))
                        {
                            CpmPcieHotplug(Local0, Local1, 0)
                            Or(Local7, 2, Local7)
                        }

                        Store(CpmReadMem8(CpmWirelessButtonTable, Add(CpmWirelessButtonBridge, 2), 0, 5), Local0)
                        Store(CpmReadMem8(CpmWirelessButtonTable, Add(CpmWirelessButtonBridge, 2), 5, 3), Local1)
                        If(LOr(Local0, Local1))
                        {
                            CpmPcieHotplug(Local0, Local1, 0)
                            Or(Local7, 4, Local7)
                        }

                        Store(CpmReadMem8(CpmWirelessButtonTable, Add(CpmWirelessButtonBridge, 3), 0, 5), Local0)
                        Store(CpmReadMem8(CpmWirelessButtonTable, Add(CpmWirelessButtonBridge, 3), 5, 3), Local1)
                        If(LOr(Local0, Local1))
                        {
                            CpmPcieHotplug(Local0, Local1, 0)
                            Or(Local7, 8, Local7)
                        }

                        CpmSetDevicePower(CpmReadMem32(CpmWirelessButtonTable, CpmWirelessButtonDeviceIdPower, 0, 8), 0)

                        CpmSetPostCode(0x4402)
                    }
                } 
                Else                                    // ON
                {
                    CpmSetPostCode(0x4403)

                    If(LEqual(Local2, 2))               // Power Control Enabled, Power Off devices
                    {
                        CpmSetDevicePower(CpmReadMem32(CpmWirelessButtonTable, CpmWirelessButtonDeviceIdPower, 0, 8), 1)

                        Sleep(100)
                        Sleep(100)

                        Store(CpmReadMem8(CpmWirelessButtonTable, CpmWirelessButtonBridge, 0, 5), Local0)
                        Store(CpmReadMem8(CpmWirelessButtonTable, CpmWirelessButtonBridge, 5, 3), Local1)
                        If(LOr(Local0, Local1))
                        {
                            CpmPcieHotplug(Local0, Local1, 1)
                            Or(Local7, 1, Local7)
                        }

                        Store(CpmReadMem8(CpmWirelessButtonTable, Add(CpmWirelessButtonBridge, 1), 0, 5), Local0)
                        Store(CpmReadMem8(CpmWirelessButtonTable, Add(CpmWirelessButtonBridge, 1), 5, 3), Local1)
                        If(LOr(Local0, Local1))
                        {
                            CpmPcieHotplug(Local0, Local1, 1)
                            Or(Local7, 2, Local7)
                        }

                        Store(CpmReadMem8(CpmWirelessButtonTable, Add(CpmWirelessButtonBridge, 2), 0, 5), Local0)
                        Store(CpmReadMem8(CpmWirelessButtonTable, Add(CpmWirelessButtonBridge, 2), 5, 3), Local1)
                        If(LOr(Local0, Local1))
                        {
                            CpmPcieHotplug(Local0, Local1, 1)
                            Or(Local7, 4, Local7)
                        }

                        Store(CpmReadMem8(CpmWirelessButtonTable, Add(CpmWirelessButtonBridge, 3), 0, 5), Local0)
                        Store(CpmReadMem8(CpmWirelessButtonTable, Add(CpmWirelessButtonBridge, 3), 5, 3), Local1)
                        If(LOr(Local0, Local1))
                        {
                            CpmPcieHotplug(Local0, Local1, 1)
                            Or(Local7, 8, Local7)
                        }

                        CpmSetPostCode(0x4404)
                    }

                    // antenna on
                    CpmSetDevicePower(CpmReadMem32(CpmWirelessButtonTable, CpmWirelessButtonDeviceIdRadio, 0, 8), 1)
                    CpmSetDevicePower(CpmReadMem32(CpmWirelessButtonTable, CpmWirelessButtonDeviceIdOther, 0, 8), 1)
                }
                If (And(Local7, 1))
                {
                    If (CondRefOf(\_SB.PCI0.CpmWirelessBridge0))
                    {
                        Notify(\_SB.PCI0.CpmWirelessBridge0, 0x00)
                    }
                }
                If (And(Local7, 2))
                {
                    If (CondRefOf(\_SB.PCI0.CpmWirelessBridge1))
                    {
                        Notify(\_SB.PCI0.CpmWirelessBridge1, 0x00)
                    }
                }
                If (And(Local7, 4))
                {
                    If (CondRefOf(\_SB.PCI0.CpmWirelessBridge2))
                    {
                        Notify(\_SB.PCI0.CpmWirelessBridge2, 0x00)
                    }
                }
                If (And(Local7, 8))
                {
                    If (CondRefOf(\_SB.PCI0.CpmWirelessBridge3))
                    {
                        Notify(\_SB.PCI0.CpmWirelessBridge3, 0x00)
                    }
                }
            }
        }
    }
}
