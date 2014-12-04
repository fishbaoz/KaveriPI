/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPM ACPI Thermal Fan SSDT Table
 *
 * Contains CPM code to provide ASL code support for ACPI Thermal Fan Feature.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      CPM
 * @e sub-project:  AcpiThermalFan
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
    "AcpiThermalFan.aml",       // Output file
    "SSDT",                     // Signature
    0x01,                       // DSDT Revision
    "AMD",                      // OEM ID
    "CPMTHERM",                 // OEM Table ID
    0x1                         // OEM Revision
    )
{
    #define CPM_ACPI_THERMAL_FAN_ASL

    #include "CmnDef.asi"
    #include "CmnExt.asi"

    // Thermal zone 0 parameters

    #define Zone0Crt        105
    #define Zone0Psv        98
    #define Zone0AC0        80
    #define Zone0AC1        50
    #define Zone0AC2        0
    #define Zone0AC3        0
    #define Zone0AL0        153     // 255 * 60% = 255
    #define Zone0AL1        102     // 255 * 40% = 102
    #define Zone0AL2        0
    #define Zone0AL3        0
    #define ThystAZ0        12      // hystersis for active cooling
    #define ThystP          5       // hystersis for passive cooling
    #define Plimit          1       // P state limit when Tpassive is reached

    External (\_PR.C000, DeviceObj)
    External (\_PR.C001, DeviceObj)
    External (\_PR.C002, DeviceObj)
    External (\_PR.C003, DeviceObj)
    External (\_PR.C004, DeviceObj)
    External (\_PR.C005, DeviceObj)
    External (\_PR.C006, DeviceObj)
    External (\_PR.C007, DeviceObj)
    External (\_PR.C000.PPCV, DeviceObj)
    External (\_PR.C001.PPCV, DeviceObj)
    External (\_PR.C002.PPCV, DeviceObj)
    External (\_PR.C003.PPCV, DeviceObj)
    External (\_PR.C004.PPCV, DeviceObj)
    External (\_PR.C005.PPCV, DeviceObj)
    External (\_PR.C006.PPCV, DeviceObj)
    External (\_PR.C007.PPCV, DeviceObj)

    Scope(\_TZ)
    {

        OperationRegion(STSI, SystemMemory, 0xF80C31E8, 0x08)
        Field(STSI,DwordAcc,NoLock,Preserve)
        {
            TSII, 8,                        // SB-TSI index register
            Offset(0x4),
            TSID, 8,                        // SB-TSI data register
        }
        IndexField(TSII, TSID, ByteAcc, NoLock, Preserve)
        {
            Offset(0x1),
            TS01, 8,                        // CPU Temperature high byte
            TS02, 8,                        // SB-TSI status
            TS03, 8,                        // SB-TSI configuration
            TS04, 8,                        //
            TS05, 8,                        //
            TS06, 8,                        //
            TS07, 8,                        //
            TS08, 8,                        //
            TS09, 8,                        //
            Offset(0x10),
            TS10, 8,                        //
            TS11, 8,                        //
            TS12, 8,                        //
            TS13, 8,                        //
            TS14, 8,                        //
            Offset(0x22),
            TS22, 8,                        //
            Offset(0x32),
            TS32, 8,                        //
            Offset(0xBF),
            TSBF, 8,                        //
            Offset(0xFE),
            TSFE, 8,                        //
            TSFF, 8,                        //
        }

        #define AL0Mask                 0x1
        #define AL1Mask                 0x2
        #define AL2Mask                 0x4
        #define AL3Mask                 0x8
        #define AL4Mask                 0x10
        #define AL5Mask                 0x20
        #define AL6Mask                 0x40
        #define AL7Mask                 0x80

        #define FanInputControlOffset   0x00
        #define FanControlOffset        0x01
        #define FanFreqOffset           0x02
        #define FanLowDutyOffset        0x03
        #define FanRegisterSize         0x10
        #define FanOn                   1
        #define FanOff                  0

        Name(CpmFanStatus, 0)           // Fan 0 status,    bit 0 = AL0
                                        //                      1 = AL1
                                        //                      :
                                        //                      7 = AL7
        Name(CpmFanTempCtrl0, Zone0AC0)
        Name(CpmFanTempCtrl1, Zone0AC1)
        Name(CpmFanTempCtrl2, Zone0AC2)
        Name(CpmFanTempCtrl3, Zone0AC3)
        Name(CpmFanSpeedCtrl0, Zone0AL0)  // _AL0
        Name(CpmFanSpeedCtrl1, Zone0AL1)  // _AL1
        Name(CpmFanSpeedCtrl2, Zone0AL2)  // _AL2
        Name(CpmFanSpeedCtrl3, Zone0AL3)  // _AL3
        Name(CpmHystersisForActiveCooling, ThystAZ0)
        Name(CpmHystersisForPassiveCooling, ThystP)

        Name(CpmCriticalTemperatureThreshold, Zone0Crt) // Zone 0 critical temperature threshold (_CRT) in degree
        Name(CpmPassiveCoolingThreshold, Zone0Psv)      // Zone 0 passive cooling threshold (_PSV)
        Name(CpmTc1, 1)
        Name(CpmTc2, 2)
        Name(CpmSamplePeriod, 150)
        Name(CpmAc0, Zone0AC0)    // Fan 0 _AC0 temp in degree
        Name(CpmAc1, Zone0AC1)    // Fan 0 _AC1 temp in degree
        Name(CpmAc2, Zone0AC2)    // _AC2
        Name(CpmAc3, Zone0AC3)    // _AC3

        Name(CpmCurrentTemperature, 0)
        Name(CpmTemperatureHighThreshold, 0)
        Name(CpmTemperatureLowThreshold, 0)
        Name(CpmPstateLimit, 0)


        // Input:   Arg0 = temperature sensor type
        // Output:  Temperature in 0.1 kelvin
        Method(CpmGetZoneTemperature, 1, Serialized)
        {
            If(LEqual(Arg0, 0)) {                                       // SB-TSI
                Or(\_TZ.TS03, 0x40, \_TZ.TS09)                          // Disable temperature update
                Store(\_TZ.TS01, Local1)                                // Get temperature high byte
                Store(\_TZ.TS10, Local2)                                // Get temperature low byte
                And(\_TZ.TS03, 0xBF, \_TZ.TS09)                         // Make sure temperature update is enabled
                Add(Multiply(256, Local1), Local2, Local3)              // Local3 = temperature (C) * 256
                Add(2732, Divide(Multiply(Local3, 10), 256), Local6)    // temperature * 10 / 256 + 2732
            }
            Return(Local6)                                              // Return converted temperature in unit of 0.1K
        }


        Method(CpmThermalTrapHandler, 0, Serialized)
        {
            Store(\_TZ.TS02, Local0)
            And(Local0, 0x18, Local0)               // Mask out SB-TSI TempHighAlert and TempLowAlert
            If(LNotEqual(Local0, 0)) {              // Take action only SB-TSI temp alert is triggered
                Or(\_TZ.TS03, 0x40, \_TZ.TS09)      // Disable temperature update, it will be enabled in _TMP

                Store(\_TZ.TS01, CpmCurrentTemperature) // Get CPU temperature high byte

                // Thermal zone 0 is designated for CPU fan
                CpmUpdateTemperatureThreshold ()
                Notify (\_TZ.THZ0,0x81)             // Notify OS that the thermal trip points are changed
            }
            Return(Local0)
        }

        // Input:   Arg0 = FANOUT #
        Method(CpmInitFanoutRegister, 1, Serialized)
        {
            Multiply (Arg0, FanRegisterSize, Local0)
            CpmWritePm2 ( Add (FanInputControlOffset, Local0), 0x06)   // Configure FAN0 as manual control
            CpmWritePm2 ( Add (FanControlOffset, Local0), 0x04)        // Select manual mode and fan polarity
            CpmWritePm2 ( Add (FanFreqOffset, Local0), 0x02)           // Set FeqDiv0 to 23.44KHz
            CpmWritePm2 ( Add (FanLowDutyOffset, Local0), 0xFF)        // Set initial fan 0 PWM duty cycle
        }

        // Input:   Arg0 = Temperature sensor
        Method(CpmInitTempInRegister, 1, Serialized)
        {
        }

        // Input:   Arg0 = Temperature sensor
        //          Arg1 = High temperature threshold in C
        //          Arg2 = Low temperature threshold in C

        Method(CpmSetTemperatureThreshold, 3, Serialized)
        {
            If(LEqual(Arg0, 0)) {                   // SB-TSI
                //Or(\_TZ.TS03, 0xC0, \_TZ.TS09)    // Disable alert and temperature update
                Store(0x2, \_TZ.TS32)               // Select 3 samples for alert threshold
                Store(\_TZ.TS02, Local0)
                Store(Arg1, \_TZ.TS07)              // Program SB-TSI high temperature threshold
                Store(0x0, \_TZ.TS13)
                Store(Arg2, \_TZ.TS08)              // Program SB-TSI low temperature threshold
                Store(0x0, \_TZ.TS14)
                //And(\_TZ.TS03, 0x3F, \_TZ.TS09)   // Disable alert and temperature update
            }
        }

        //  Arg0:   ALx mask
        //  Arg1:   ALx speed if non zero other 0 means ALx off
        Method(CpmSetFanSpeed, 2, Serialized)
        {
            If(LEqual(Arg1, FanOff))
            {
                And(CpmFanStatus, Not(Arg0), CpmFanStatus)  // Update fan status
            }
            Else
            {
                Or(CpmFanStatus, Arg0, CpmFanStatus)        // Update fan status
            }

            If(And(CpmFanStatus, AL0Mask)) {                // AL0 requested?
                Store(CpmFanSpeedCtrl0, Local0)             // Set AL0 fan speed
            }
            Elseif(And(CpmFanStatus, AL1Mask)) {            // AL1 request?
                Store(CpmFanSpeedCtrl1, Local0)             // Set AL1 fan speed
            }
            Elseif(And(CpmFanStatus, AL2Mask)) {            // AL2 request?
                Store(CpmFanSpeedCtrl2, Local0)             // Set AL2 fan speed
            }
            Elseif(And(CpmFanStatus, AL3Mask)) {            // AL3 request?
                Store(CpmFanSpeedCtrl3, Local0)             // Set AL3 fan speed
            }
            Else {
                Store(0, Local0)                            // Turn fan off (speed = 0) if no ALx state is on
            }
            Add (FanLowDutyOffset, Multiply (CpmReadTable(CpmAcpiThermalFanTable, CpmAcpiThermalFanFanNum), FanRegisterSize), Local1)
            CpmWritePm2 (Local1, Local0)                    // Set fan 0 PWM duty cycle
        }

        //  Arg0:   Fan number
        //  Return: 1 if fan on
        //          0 if fan off

        Method(CpmGetFanStatus, 1, Serialized)
        {
            Store(0, Local0)
            If(And(Arg0, CpmFanStatus)) {               // Fan Mask querried set?
                Store(1, Local0)
            }
            Return(Local0)
        }

        PowerResource(P0L0, 0, 0)                       // For AL0
        {
            Method(_STA) {Return(CpmGetFanStatus (AL0Mask))}
            Method(_ON)  {CpmSetFanSpeed (AL0Mask, FanOn)}
            Method(_OFF) {CpmSetFanSpeed (AL0Mask, FanOff)}
        }

        PowerResource(P0L1, 0, 0)                       // For AL1
        {
            Method(_STA) {Return(CpmGetFanStatus (AL1Mask))}
            Method(_ON)  {CpmSetFanSpeed (AL1Mask, FanOn)}
            Method(_OFF) {CpmSetFanSpeed (AL1Mask, FanOff)}
        }

        PowerResource(P0L2, 0, 0)                       // For AL2
        {
            Method(_STA) {Return(CpmGetFanStatus (AL2Mask))}
            Method(_ON)  {CpmSetFanSpeed (AL2Mask, FanOn)}
            Method(_OFF) {CpmSetFanSpeed (AL2Mask, FanOff)}
        }

        PowerResource(P0L3, 0, 0)                       // For AL3
        {
            Method(_STA) {Return(CpmGetFanStatus (AL3Mask))}
            Method(_ON)  {CpmSetFanSpeed (AL3Mask, FanOn)}
            Method(_OFF) {CpmSetFanSpeed (AL3Mask, FanOff)}
        }

        Device(Z0F0)                                    // Zone 0, Fan speed 0
        {
            Name (_HID, EISAID("PNP0C0B"))              // Fan device ID
            Name (_UID, 0)
            Name (_PR0,Package(){\_TZ.P0L0})
            Method(_INI, 0)
            {
                Store(CpmReadTable(CpmAcpiThermalFanTable, CpmAcpiThermalFanHysteresisInfo), CpmHystersisForActiveCooling)
                Store(CpmReadTable(CpmAcpiThermalFanTable, CpmAcpiThermalFanHysteresisInfoPsv), CpmHystersisForPassiveCooling)
                Store(CpmReadTable(CpmAcpiThermalFanTable, CpmAcpiThermalFanCpuAC0), CpmFanTempCtrl0)
                Store(CpmReadTable(CpmAcpiThermalFanTable, CpmAcpiThermalFanCpuAC1), CpmFanTempCtrl1)
                Store(CpmReadTable(CpmAcpiThermalFanTable, CpmAcpiThermalFanCpuAC2), CpmFanTempCtrl2)
                Store(CpmReadTable(CpmAcpiThermalFanTable, CpmAcpiThermalFanCpuAC3), CpmFanTempCtrl3)
                Store(Divide(Multiply(CpmReadTable(CpmAcpiThermalFanTable, CpmAcpiThermalFanCpuAL0), 255), 100), CpmFanSpeedCtrl0)
                Store(Divide(Multiply(CpmReadTable(CpmAcpiThermalFanTable, CpmAcpiThermalFanCpuAL1), 255), 100), CpmFanSpeedCtrl1)
                Store(Divide(Multiply(CpmReadTable(CpmAcpiThermalFanTable, CpmAcpiThermalFanCpuAL2), 255), 100), CpmFanSpeedCtrl2)
                Store(Divide(Multiply(CpmReadTable(CpmAcpiThermalFanTable, CpmAcpiThermalFanCpuAL3), 255), 100), CpmFanSpeedCtrl3)
                Store(CpmReadTable(CpmAcpiThermalFanTable, CpmAcpiThermalFanCpuCRT), CpmCriticalTemperatureThreshold)
                Store(CpmReadTable(CpmAcpiThermalFanTable, CpmAcpiThermalFanCpuPSV), CpmPassiveCoolingThreshold)
                Store(0, CpmPstateLimit)

                // Initialize fanout register
                CpmInitFanoutRegister (CpmReadTable(CpmAcpiThermalFanTable, CpmAcpiThermalFanFanNum))

                // Initialize TEMPIN register
                CpmInitTempInRegister (CpmReadTable(CpmAcpiThermalFanTable, CpmAcpiThermalFanThermalSensor))

                // Initialize HW temperature threshold
                CpmSetTemperatureThreshold (CpmReadTable(CpmAcpiThermalFanTable, CpmAcpiThermalFanThermalSensor), CpmAc3, 0)
            }
        }

        Device(Z0F1)                            // Zone 0, Fan speed 1
        {
            Name (_HID, EISAID("PNP0C0B"))      // Fan device ID
            Name (_UID, 1)
            Name (_PR0,Package(){\_TZ.P0L1})
        }

        Device(Z0F2)                            // Zone 0, Fan speed 2
        {
            Name (_HID, EISAID("PNP0C0B"))      // Fan device ID
            Name (_UID, 2)
            Name (_PR0,Package(){\_TZ.P0L2})
        }

        Device(Z0F3)                            // Zone 0, Fan speed 3
        {
            Name (_HID, EISAID("PNP0C0B"))      // Fan device ID
            Name (_UID, 3)
            Name (_PR0,Package(){\_TZ.P0L3})
        }


        Method(CpmUpdateTemperatureThreshold, 0, Serialized)
        {
            Store(Zone0Crt, CpmCriticalTemperatureThreshold)
            Store(Zone0Psv, CpmPassiveCoolingThreshold)
            Store(CpmFanTempCtrl0, CpmAc0)
            Store(CpmFanTempCtrl1, CpmAc1)
            Store(CpmFanTempCtrl2, CpmAc2)
            Store(CpmFanTempCtrl3, CpmAc3)

            Store(0, CpmPstateLimit)
            If(LGreaterEqual(CpmCurrentTemperature, CpmCriticalTemperatureThreshold)) {
                Store(128, CpmTemperatureHighThreshold)
                Store(CpmCriticalTemperatureThreshold, CpmTemperatureLowThreshold)
                Store(2, Local0)
                Store(Plimit, CpmPstateLimit)
            }
            Elseif(LGreaterEqual(CpmCurrentTemperature, CpmPassiveCoolingThreshold)) {
                Store(CpmCriticalTemperatureThreshold, CpmTemperatureHighThreshold)
                Store(CpmPassiveCoolingThreshold, CpmTemperatureLowThreshold)
                Store(2, Local0)
                Store(Plimit, CpmPstateLimit)
            }
            Elseif(LGreaterEqual(CpmCurrentTemperature, CpmAc0)) {
                Store(CpmPassiveCoolingThreshold, CpmTemperatureHighThreshold)
                Store(CpmAc0, CpmTemperatureLowThreshold)
                Store(1, Local0)
            }
            Elseif(LGreaterEqual(CpmCurrentTemperature, CpmAc1)) {
                Store(CpmAc0, CpmTemperatureHighThreshold)
                Store(CpmAc1, CpmTemperatureLowThreshold)
                Store(1, Local0)
            }
            Elseif(LGreaterEqual(CpmCurrentTemperature, CpmAc2)) {
                Store(CpmAc1, CpmTemperatureHighThreshold)
                Store(CpmAc2, CpmTemperatureLowThreshold)
                Store(1, Local0)
            }
            Elseif(LGreaterEqual(CpmCurrentTemperature, CpmAc3)) {
                Store(CpmAc2, CpmTemperatureHighThreshold)
                Store(CpmAc3, CpmTemperatureLowThreshold)
                Store(1, Local0)
            }
            Elseif(Lless(CpmCurrentTemperature, CpmAc3)) {
                Store(CpmAc3, CpmTemperatureHighThreshold)
                Store(0, CpmTemperatureLowThreshold)
                Store(0, Local0)
            }

            If(LEqual(Local0, 1)) {
                If(LGreaterEqual(CpmTemperatureLowThreshold, CpmHystersisForActiveCooling)) {
                    Subtract(CpmTemperatureLowThreshold, CpmHystersisForActiveCooling, Local1)
                    If(LEqual(CpmTemperatureLowThreshold, CpmAc0)) {
                        Store(Local1, CpmAc0)
                    }
                    Elseif(LEqual(CpmTemperatureLowThreshold, CpmAc1)) {
                        Store(Local1, CpmAc1)
                    }
                    Elseif(LEqual(CpmTemperatureLowThreshold, CpmAc2)) {
                        Store(Local1, CpmAc2)
                    }
                    Elseif(LEqual(CpmTemperatureLowThreshold, CpmAc3)) {
                        Store(Local1, CpmAc3)
                    }
                    Store(Local1, CpmTemperatureLowThreshold)
                }
            }
            Elseif(LEqual(Local0, 2)) {
                If(LGreaterEqual(CpmTemperatureLowThreshold, CpmPassiveCoolingThreshold)) {
                    Subtract(CpmTemperatureLowThreshold, CpmPassiveCoolingThreshold, CpmTemperatureLowThreshold)
                }
            }

            // Update _PPC value
            If (CondRefOf(\_PR.C000))
            {
                Store(CpmPstateLimit, \_PR.C000.PPCV)
            }
            If (CondRefOf(\_PR.C001))
            {
                Store(CpmPstateLimit, \_PR.C001.PPCV)
            }
            If (CondRefOf(\_PR.C002))
            {
                Store(CpmPstateLimit, \_PR.C002.PPCV)
            }
            If (CondRefOf(\_PR.C003))
            {
                Store(CpmPstateLimit, \_PR.C003.PPCV)
            }
            If (CondRefOf(\_PR.C004))
            {
                Store(CpmPstateLimit, \_PR.C004.PPCV)
            }
            If (CondRefOf(\_PR.C005))
            {
                Store(CpmPstateLimit, \_PR.C005.PPCV)
            }
            If (CondRefOf(\_PR.C006))
            {
                Store(CpmPstateLimit, \_PR.C006.PPCV)
            }
            If (CondRefOf(\_PR.C007))
            {
                Store(CpmPstateLimit, \_PR.C007.PPCV)
            }

            // Write threshold to hardware
            CpmSetTemperatureThreshold (CpmReadTable(CpmAcpiThermalFanTable, CpmAcpiThermalFanThermalSensor), CpmTemperatureHighThreshold, CpmTemperatureLowThreshold)
        }


        ThermalZone(THZ0)                           // Thermal Zone 0
        {

            // Return the temperature at which the OS must shut
            // down.

            Method(_CRT,0,Serialized)
            {
                Return(Add(2732, Multiply(CpmCriticalTemperatureThreshold, 10)))
            }

            // Notifies ASL Code the current cooling mode.
            //  0 - Active cooling
            //  1 - Passive cooling

            Method(_SCP,1,Serialized)
            {
                //Breakpoint
                // To-do, any initialization to do here?
            }

            // Return the Temperature to the OS.

            Method(_TMP,0,Serialized)
            {
                Return(CpmGetZoneTemperature(CpmReadTable(CpmAcpiThermalFanTable, CpmAcpiThermalFanThermalSensor)))
            }

            Method(_AC0) {
                Return(Add(2732,Multiply(CpmAc0,10)))
            }

            Method(_AC1) {
                Return(Add(2732,Multiply(CpmAc1,10)))
            }

            Method(_AC2) {
                Return(Add(2732,Multiply(CpmAc2,10)))
            }

            Method(_AC3) {
                Return(Add(2732,Multiply(CpmAc3,10)))
            }

            Method(_AL0) {Return(Package(){\_TZ.Z0F0})}

            Method(_AL1) {Return(Package(){\_TZ.Z0F1})}

            Method(_AL2) {Return(Package(){\_TZ.Z0F2})}

            Method(_AL3) {Return(Package(){\_TZ.Z0F3})}

            // Return the Processor(s) used for Passive Cooling.

            Method(_PSL,0,Serialized)
            {
                If (LNot(CondRefOf(\_PR.C001))) {
                    Return(Package(){\_PR.C000})
                }
                If (LNot(CondRefOf(\_PR.C002))) {
                    Return(Package(){\_PR.C000, \_PR.C001})
                }
                If (LNot(CondRefOf(\_PR.C003))) {
                    Return(Package(){\_PR.C000, \_PR.C001, \_PR.C002})
                }
                If (LNot(CondRefOf(\_PR.C004))) {
                    Return(Package(){\_PR.C000, \_PR.C001, \_PR.C002, \_PR.C003})
                }
                If (LNot(CondRefOf(\_PR.C005))) {
                    Return(Package(){\_PR.C000, \_PR.C001, \_PR.C002, \_PR.C003, \_PR.C004})
                }
                If (LNot(CondRefOf(\_PR.C006))) {
                    Return(Package(){\_PR.C000, \_PR.C001, \_PR.C002, \_PR.C003, \_PR.C004, \_PR.C005})
                }
                If (LNot(CondRefOf(\_PR.C007))) {
                    Return(Package(){\_PR.C000, \_PR.C001, \_PR.C002, \_PR.C003, \_PR.C004, \_PR.C005, \_PR.C006})
                }
                Return(Package(){\_PR.C000, \_PR.C001, \_PR.C002, \_PR.C003, \_PR.C004, \_PR.C005, \_PR.C006, \_PR.C007})
            }

            // Returns the temperature at which the OS initiates
            // CPU throttling.

            Method(_PSV,0,Serialized)
            {
                Return(Add(2732, Multiply(CpmPassiveCoolingThreshold, 10)))
            }

            // Returns TC1 value used in the passive cooling formula.

            Method(_TC1,0,Serialized)
            {
                Return(CpmTc1)
            }

            // Returns TC2 value used in the passive cooling formula.

            Method(_TC2,0,Serialized)
            {
                Return(CpmTc2)
            }

            // Returns the sampling period used in the passive
            // cooling formula.

            Method(_TSP,0,Serialized)
            {
                Return(CpmSamplePeriod)
            }

        }
    }

    Scope(\_GPE)                        // General-Purpose Event
    {
        Method(_L11)                    // CPU TALERT
        {
            \_TZ.CpmThermalTrapHandler ()
            Notify(\_TZ.THZ0, 0x80)     // Get OSPM to read temperature
        }
    }
}
