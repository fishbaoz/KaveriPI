/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM Zero Power Odd SSDT Table
 *
 * Contains CPM code to provide ASL code support for Zero Power Odd.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  ZeroPowerOdd
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
    "ZeroPwrOdd.aml",                                               // Output file
    "SSDT",                                                         // Signature
    0x01,                                                           // DSDT Revision
    "AMD",                                                          // OEM ID
    "CPMZPODD",                                                     // OEM Table ID
    0x1                                                             // OEM Revision
    )
{
    #define CPM_ZERO_POWER_ODD

    #include "CmnDef.asi"
    #include "CmnExt.asi"

    External(\_SB.PCI0.SATA, DeviceObj)
    External(\_SB.PCI0.SATA._ADR, DeviceObj)

    External(STDV, MethodObj)
    External(STFN, MethodObj)

    Scope(\_SB.PCI0.SATA)                                           // ODD Zero Power support at SATA Port 1 scope on AHCI mode
    {
        // Status of ODD: 1: ON. 0: OFF
        Name(CpmOddStatus, 1)

        //
        // _DSM Device Specific Method
        //
        // Arg0:   UUID      Unique function identifier
        // Arg1:   Integer   Revision Level
        // Arg2:   Integer   Function Index (0 = Return Supported Functions)
        // Arg3:   Package   Parameters
        //
        Method(_DSM,4, Serialized)
        {
            //
            // If(LEqual(Arg0,ToUUID("3DDDFAA6-361B-4eb4-A424-8D10089D1653")))
            //
            // Note: ToUUID function is available only to ACPI 3.0 compliant ASL compilers.
            //  The ToUUID function is equivalent to an ASCII string to hex conversion in the
            //  following specified order:
            //
            //    For a given ASCII UUID string formatted as:
            //      "AABBCCDD-EEFF-GGHH-IIJJ-KKLLMMNNOOPP",
            //    where AA-PP represent ASCII strings of hexidecimal numbers.  ToUUID() of
            //    this string returns a buffer containing the integer conversion of aa-pp:
            //      Buffer(0x10){DD,CC,BB,AA,FF,EE,HH,GG,II,JJ,KK,LL,MM,NN,OO,PP}
            //
            //  Therefore, ToUUID("E4DB149B-FCFE-425b-A6D8-92357D78FC7F") equals
            //  Buffer(){0x9B,0x14,0xDB,0xE4,0xFE,0xFC,0x5B,0x42,0xD8,0xA6,0x7F,0xFC,0x78,0x7D,0x35,0x92}
            //
            If(LEqual(XCMP(Arg0,Buffer(){0x9B,0x14,0xDB,0xE4,0xFE,0xFC,0x5B,0x42,0xA6,0xD8,0x92,0x35,0x7D,0x78,0xFC,0x7F}), 1))
            {
                // Function 0: return supported functions
                //
                // Returns:
                //   Functions 1-2 are supported
                //
                If(LEqual(Arg2,0))
                {
                    Return(Buffer(){0x03})
                }

                //
                // Function 1: Query Link Power Interface
                //
                // Returns:
                //   A package of Integers, where each package element is the address
                //   (_ADR value) of child devices (ports) where link power controls may
                //   inhibit device enumeration.
                //
                ElseIf(LEqual(Arg2,1))
                {
                    Name (ADDR, Package(){0x0001FFFF})
                    Store (Add (Multiply (CpmReadTable(CpmZeroPowerOddTable, CpmZeroPowerOddSataPortId), 0x10000), 0x0000FFFF), Index(ADDR, 0))
                    Return(Package(){ADDR})
                }

                //
                // Function 2: Control Link Power
                //   Arg3:  Device address, Requested link power state (Package with two entries)
                //      Entry 1 - Device address (as described in the ACPI specification _ADR Object Encoding table). Note that "Ones" (ASL constant) applies setting to all possible devices.
                //      Entry 2 - Link power control value:
                //              0x00: allow link power to be removed
                //              0x01: apply power to link and child device(s) 
                //
                // Returns:
                //   0: Success
                //   1: Invalid device address
                //
                ElseIf (LEqual(Arg2,2)) {
                    Store(DeRefOf(Index(Arg3, 0)), Local0)
                    If (LOr(LEqual(Local0, Add (Multiply (CpmReadTable(CpmZeroPowerOddTable, CpmZeroPowerOddSataPortId), 0x10000), 0x0000FFFF)), LEqual(Local0, Ones)))
                    {
                        Store(DeRefOf(Index(Arg3, 1)), Local1)
                        If (LEqual(Local1, 0))
                        {
                            STPT(0)                                     // Disable PHY Port 1
                            Return(0)
                        } ElseIf (LEqual(Local1, 1))
                        {
                            If(CpmIsWin8())
                            {
                                \_SB.PCI0.SATA.ODD8._PS0()              // _PS0 implicitly enables PHY Port 1
                            }
                            ElseIf (CpmIsLinux())
                            {
                                \_SB.PCI0.SATA.ODDL._PS0()              // _PS0 implicitly enables PHY Port 1
                            }
                            Else
                            {
                                \_SB.PCI0.SATA.ODDZ._PS0()              // _PS0 implicitly enables PHY Port 1
                            }
                            Return(0)
                        }
                    }
                }
            }
            Return(1)
        }

        // XCMP - Since ACPI 1.0 compliant OS does not understand LEqual() of two buffers,
        //        this function does a UUID buffer compare of UUID1 to UUID2.
        //
        // Arg0:    Buffer   UUID1
        // Arg1:    Buffer   UUID2
        //
        // Output:  1 = Compare success
        //          0 = Compare Failure
        //
        Method(XCMP,2)
        {
            // Each buffer must be 16 bytes long or it's not a UUID
            If(LNotEqual(0x10, SizeOf(Arg0)))
            {
                Return(0)
            }
            If(LNotEqual(0x10, SizeOf(Arg1)))
            {
                Return(0)
            }

            // Start loop at beginning of buffer
            Store(0, Local0)

            // Check each byte in each buffer for a match
            While(LLess(Local0, 16))
            {
                if(LNotEqual(DerefOf(Index(Arg0,Local0)),DerefOf(Index(Arg1,Local0))))
                {
                    Return(0)
                }
                Increment(Local0)
            }

            Return(1)
        }


        Method(STDV, 0, Serialized)
        {
            Store ( _ADR, Local0 )
            Return ( ShiftRight ( Local0, 16 ) )
        }

        Method(STFN, 0, Serialized)
        {
            Store ( _ADR, Local0 )
            Return ( And ( Local0, 0xFFFF ) )
        }

        Method(PBAA, 0, Serialized)
        {
            Store (CpmReadPci32 (0, STDV(), STFN(), 0x24), Local0)
            If( LEqual(Local0, 0xFFFFFFFF) )
            {
                Store(0,Local0)
            }
            Return(Local0)
        }

        // Enable/Disable Sata Port
        // Arg0 - 0: Disable. 1: Enable
        Method(STPT, 1, Serialized)
        {
            CpmWritePci (0, STDV(), STFN(), 0x42, CpmReadTable(CpmZeroPowerOddTable, CpmZeroPowerOddSataPortId), 1, Xor(Arg0, 1))    // Enable PHY Port n
        }

        // Enable/Disable Sata Port Phy
        // Arg0 - 0: Disable. 1: Enable
        Method(STPP, 1, Serialized)
        {
            CpmWritePci (0, STDV(), STFN(), 0x42, CpmReadTable(CpmZeroPowerOddTable, CpmZeroPowerOddSataPortId), 1, Xor(Arg0, 1))    // Enable PHY Port n
            If (LEqual (CpmReadPci ( 0, STDV(), STFN(), 0x0A,0, 16), 0x106))
            {
                If( LNotEqual(PBAA(), 0) )
                {
                    CpmWritePci (0, STDV(), STFN(), 0x40, 0, 1, 1)          // Write Enable
                    CpmWriteMem32 (PBAA(), 0x0C, CpmReadTable(CpmZeroPowerOddTable, CpmZeroPowerOddSataPortId), 1, Arg0)             // Port n implemented
                    Store (CpmReadPci (0, STDV(), STFN(), 0x42, 0, 6), Local0)  // Port Status
                    If (LEqual(Local0, 0x3F))                           // at lease reserve one port enabled
                    {
                        CpmWriteMem32 (PBAA(), 0x0C, 0, 1, 1)           // Enable Port 0
                    } ElseIf (LAnd (CpmReadPci (0, STDV(), STFN(), 0x42, 0, 1), Xor(And(Local0, 0x3E), 0x3E))) {
                        CpmWriteMem32 (PBAA(), 0x0C, 0, 1, 0)           // Disable Port 0
                    }
                    Store (CpmReadMem32 (PBAA(), 0x0C, 0, 6), Local1)   // Port Implement
                    Store(0, Local2)
                    While(Local1)
                    {
                        If (And(Local1, 0x01))
                        {
                            Increment(Local2)
                        }
                        ShiftRight(Local1, 1, Local1)
                    }
                    CpmWriteMem32 (PBAA(), 0, 0, 5, Decrement(Local2))  // Update Port Number
                    CpmWritePci (0, STDV(), STFN(), 0x40, 0, 1, 0)      // Write Disable
                }
            }
        }

        // Get Port Interface power management
        Method(STPM, 0, Serialized)
        {
            Store (0, Local1)
            If( LNotEqual(PBAA(), 0) )
            {
                Store (Add (0x118, Multiply(0x80, CpmReadTable(CpmZeroPowerOddTable, CpmZeroPowerOddSataPortId))), Local0) // 0x118 + ZPPT * 0x80:[0]Port1 Start
                Store (CpmReadMem32 (PBAA(), Local0, 0, 1), Local0)     // Interface power management
            }
            Return(Local1)
        }

        // Port reset in AHCI mode
        Method(STPR, 0, Serialized)
        {
            If (LEqual (CpmReadPci ( 0, STDV(), STFN(), 0x0A,0, 16), 0x106))
            {
                If( LNotEqual(PBAA(), 0) )
                {
                    Store (Add (0x118, Multiply(0x80, CpmReadTable(CpmZeroPowerOddTable, CpmZeroPowerOddSataPortId))), Local0) // 0x118 + ZPPT * 0x80:[0]Port Start
                    CpmWriteMem32 (PBAA(), Local0, 0, 1, 0)                         // Clear Px.CMS.ST bit
                    Sleep(1)                                                        // wait for 1ms
                    Store (Add (0x12C, Multiply(0x80, CpmReadTable(CpmZeroPowerOddTable, CpmZeroPowerOddSataPortId))), Local0) // 0x12C + ZPPT * 0x80:[3:0]device detection Initialization
                    CpmWriteMem32 (PBAA(), Local0, 0, 4, 1)                         // Assert port reset
                    Sleep(2)                                                        // wait for 2ms
                    CpmWriteMem32 (PBAA(), Local0, 0, 4, 0)                         // De-assert port reset
                }
            }
        }

        Device(PRT1)
        {
            Name(_ADR, 0x00000001)                                  // Address for Port 1 (ODD)
                                                                    // FFFF in Lower WORD indicates
                                                                    // no Port Multiplier

            Method(_STA, 0)
            {
                If (And(CpmReadTable(CpmMainTable, CpmZeroPowerOddEn), 1))                      // ODD Zero Power Enabled
                {
                    Return(0x0)                                     // Disable ODD - ODDZ needto be enabled
                }
                Else
                {
                    Return(0xF)                                     // Enable ODD - ODDZ need to be disabled
                }
            }

            Device(ODD)
            {
                Method (_ADR)
                {
                    Return(CpmReadTable(CpmZeroPowerOddTable, CpmZeroPowerOddSataPortId))
                 }

                // Set Device Removable icon for MS hot-fix KB950186 Vista 
                Method(_RMV,0)
                {
                    Return(0x00)
                }
            } // end of ODD
        }

        Device(ODD8)
        {
            #include "OddDevice.asi"

            Method(_STA, 0)
            {
                If (CpmIsWin8())
                {
                    Return(ZSTA())
                }
                Else
                {
                    Return(0)
                }
            }

            Name(_S0W, 0x04)

            Method(_DSW, 3, Serialized)
            {
                If (Arg0) {
                    // enable device resume, this imply OS want the device to idle, instead of _DIS.
                    // enable powerdown-allow-flag.
                    Store(0x01, WDSW)
                } Else {
                    // disable powerdown-allow-flag.
                    Store(0x00, WDSW)
                }
            }

            Name(ZPRW,Package(){0x06, 0x3})                         // GPE pin 6, wake from S3

            Method(XPRW,0)
            {
                If (LNot(CpmIsWin8()))
                {
                    Store (CpmReadTable(CpmZeroPowerOddTable, CpmZeroPowerOddEventPin3), Index (ZPRW, 0x00))   // Dummy Event
                }
                Return(ZPRW)
            }
        }

        Device(ODDZ)
        {
            #include "OddDevice.asi"

            Method(_STA, 0)
            {
                If (LNot(LOr(CpmIsWin8(), CpmIsLinux())))
                {
                    Return(ZSTA())
                }
                Else
                {
                    Return(0)
                }
            }
        }

        Device(ODDL)
        {
            #include "OddDevice.asi"

            Method(_STA, 0)
            {
                If (CpmIsLinux())
                {
                    Return(ZSTA())
                }
                Else
                {
                    Return(0)
                }
            }

            Name(_S0W, 0x04)

            Method(_DSW, 3, Serialized)
            {
                If (Arg0) {
                    // enable device resume, this imply OS want the device to idle, instead of _DIS.
                    // enable powerdown-allow-flag.
                    Store(0x01, WDSW)
                } Else {
                    // disable powerdown-allow-flag.
                    Store(0x00, WDSW)
                }
            }
        }
    }

    Scope(\_GPE)                                                    // General-Purpose Event
    {
        Method(XL06,0x0,Notserialized)                              // ODD_DA#
        {
            If (LAnd(And(CpmReadTable(CpmMainTable, CpmZeroPowerOddEn), 1),LEqual(CpmReadGevent (CpmReadTable(CpmZeroPowerOddTable, CpmZeroPowerOddEventPin2)),0))) // Handle ODD_DA# only when Zero Power enabled
            {
                If(CpmReadGeventTriggerType (CpmReadTable(CpmZeroPowerOddTable, CpmZeroPowerOddEventPin1)))    // Did we get Rising Edge(Removal)?
                {
                    Sleep(20)                                       // Let GEVENT16# signal input debounce for 20ms
                    If(CpmReadGevent(CpmReadTable(CpmZeroPowerOddTable, CpmZeroPowerOddEventPin1)))            // If GEVENT16# input value is High then card is not present
                    {
                        If (And(CpmReadTable(CpmMainTable, CpmZeroPowerOddEn), 0x10))
                        {
                            \_SB.PCI0.SATA.STPR()                   // Apply a port reset in AHCI mode
                        }
                        If (CpmIsWin8())                            // Notify for different OS mode
                        {
                            Notify(\_SB.PCI0.SATA.ODD8, 0x02) 
                        } Elseif (CpmIsLinux()) 
                        {
                            Notify(\_SB.PCI0.SATA.ODDL, 0x02) 
                        } Else
                        {
                            Notify(\_SB.PCI0.SATA.ODDZ, 0x80) 
                        }
                        Sleep(500)
                        CpmSetPostCode (0xDF15)
                    }
                }
                Else                                                // Did we get Falling Edge(Insert)?
                {
                    Sleep(20)                                       // Let GEVENT16# signal input debounce for 20ms
                    If(LNot(CpmReadGevent(CpmReadTable(CpmZeroPowerOddTable, CpmZeroPowerOddEventPin1))))      // If GEVENT6# input value is Low then card is present
                    {
                        If (CpmIsWin8())                            // Notify for different OS mode
                        {
                            Notify(\_SB.PCI0.SATA.ODD8, 0x02) 
                        } Elseif (CpmIsLinux()) 
                        {
                            Notify(\_SB.PCI0.SATA.ODDL, 0x02) 
                        } Else
                        {
                            Notify(\_SB.PCI0.SATA.ODDZ, 0x80) 
                        }
                        Sleep(500)
                        CpmSetPostCode (0xDF14)
                    }
                }
            }
            CpmWriteGeventTriggerType (CpmReadTable(CpmZeroPowerOddTable, CpmZeroPowerOddEventPin1), Xor(CpmReadGevent(CpmReadTable(CpmZeroPowerOddTable, CpmZeroPowerOddEventPin1)), 1))  // Switch SB detection logic
        }

        //
        // Pumori uses GEVENT16# for ODD_PLUGIN#
        //
        Method(XL10)
        {
            Sleep(200)                                  // Let GEVENT16# signal input debounce for 200ms

            If (CpmReadGeventTriggerType(CpmReadTable(CpmZeroPowerOddTable, CpmZeroPowerOddEventPin2)))  // Gevent16# input level = High, hot remove
            {
                If (CpmReadGevent(CpmReadTable(CpmZeroPowerOddTable, CpmZeroPowerOddEventPin2)))         // Did we get Rising Edge(Removal)?
                {
                    CpmSetPostCode (0xDF17)
                    CpmSetDevicePower(CpmReadTable(CpmZeroPowerOddTable, CpmZeroPowerOddDeviceId), 0)
                    Store(0, \_SB.PCI0.SATA.CpmOddStatus)
                    Sleep(40)
                    \_SB.PCI0.SATA.STPP(0)                  // Disable Port
                    If(CpmIsWin8())
                    {
                        Store(0x3, \_SB.PCI0.SATA.ODD8.ODPS)
                    }
                    Elseif(CpmIsLinux())
                    {
                        Store(0x3, \_SB.PCI0.SATA.ODDL.ODPS)
                    }
                    Else
                    {
                        Store(0x3, \_SB.PCI0.SATA.ODDZ.ODPS)
                    }
                    CpmWriteGeventTriggerType (CpmReadTable(CpmZeroPowerOddTable, CpmZeroPowerOddEventPin2), Xor(CpmReadGevent(CpmReadTable(CpmZeroPowerOddTable, CpmZeroPowerOddEventPin2)), 1))  // Switch SB detection logic to raising edge
                }
            }
            Else
            {
                Sleep(20)                                       // Let GEVENT16# signal input debounce for 20ms
                If(LNot(CpmReadGevent(CpmReadTable(CpmZeroPowerOddTable, CpmZeroPowerOddEventPin2))))  // If GEVENT6# input value is Low then card is present
                {
                    CpmSetPostCode (0xDF16)
                    \_SB.PCI0.SATA.STPP(1)                  // Enable Port
                    Sleep(40)                               // Power On ODD
                    CpmSetDevicePower(CpmReadTable(CpmZeroPowerOddTable, CpmZeroPowerOddDeviceId), 1)
                    Store(1, \_SB.PCI0.SATA.CpmOddStatus)
                    Sleep(40)
//                  \_SB.PCI0.SATA.STPR()                   // Apply a port reset in AHCI mode
                    If(CpmIsWin8())
                    {
                        Store(0x0, \_SB.PCI0.SATA.ODD8.ODPS)
                    }
                    Elseif(CpmIsLinux)
                    {
                        Store(0x0, \_SB.PCI0.SATA.ODDL.ODPS)
                    }
                    Else
                    {
                        Store(0x0, \_SB.PCI0.SATA.ODDZ.ODPS)
                    }
                    CpmWriteGeventTriggerType (CpmReadTable(CpmZeroPowerOddTable, CpmZeroPowerOddEventPin2), Xor(CpmReadGevent(CpmReadTable(CpmZeroPowerOddTable, CpmZeroPowerOddEventPin2)), 1))  // Switch SB detection logic to raising edge
                }
            }
            Sleep(40)                                   // Sleep 40ms

            If (And(CpmReadTable(CpmMainTable, CpmZeroPowerOddEn), 1))
            {
                If(CpmIsWin8())
                {
                    Notify(\_SB.PCI0.SATA.ODD8, 0x1)
                }
                Elseif (CpmIsLinux()) 
                {
                    Notify(\_SB.PCI0.SATA.ODDL, 0x1) 
                }
                Else
                {
                    Notify(\_SB.PCI0.SATA.ODDZ, 0x1)
                    Notify(\_SB.PCI0.SATA.ODDZ, 0x81)
                }
            }
            Else
            {
                Notify(\_SB.PCI0.SATA.PRT1, 0x1)
                // for AMDAHCI's prior to Win8 driver, extra requirement for plug in
                if(LNOT(LOr(CpmIsWin8(), CpmIsLinux())))
                {
                    Notify(\_SB.PCI0.SATA.PRT1, 0x81)
                }
            }

        } // End Method(_L11)

    }
}
