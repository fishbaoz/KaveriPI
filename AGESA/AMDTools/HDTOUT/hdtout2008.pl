# Version 2.3.0
# Date: 09/01/12
my $MajorVersion=0x2;
my $MinorVersion=0x30;

# # # # # # # # # # # # # # # # # # # # #
# Control flags                         #
# # # # # # # # # # # # # # # # # # # # #
my $dft_emulator_num = 0;      # Default Possum Index
my $interactive_mode_en = 0;   # Will ask for Possum Index if 1
my $force_reset_to_enable = 0; # When set, the script will reset system to enable HDTOUT without asking

my $dft_buffer_size = 4096;    # The smaller the buffer, the slower it runs

my $bsp_only = 1;              # 0: HDTOUT will be streamed out from all enabled cores
                               # 1: Only enable HDTOUT on BSP

my $dft_ip_addr = "";
my $default_password = "hdtout_pwd";
my $default_username = "hdtout_usr";

my $show_pci_reg_set = 0;      # Stream out PCI register set during memory init
my $show_pci_reg_get = 0;      # Stream out PCI register get during memory init

my $dump_spd_data = 1;         # Retrieve SPD data

my $exit_dbg_after_reset = 0;  # For Wombat, set to 1, if your chipset needs exit_dbg after writing CF9
my $reboot_test = 0;           # When enabled, reboot whenever the single string in HdtoutSelectTable is hit
my $use_cold_reset = 0;        # Note: some chipsets do not support this type of cold reset
my $use_relay_to_reset = 0;    # MUST have relay cables connected

# # # # #  Use the following parameters to select what to be streamed out # # # # #
my $HdtoutDefault = 1;              # 0: Skip HDTOUT from the beginning
                                    # 1: Enable HDTOUT from the beginning
my $HdtoutSelectTableEnable = 1;    # 0: Disable HdtoutSelectTable
                                    # 1: Enable HdtoutSelectTable
my $HdtoutSelectTable = [
    # Below are some examples of how this table is used:
    # - Only Stream out WL sequence of Node 1, Dct 0
    #     [["Start write leveling","!Node 1","!Dct 0"],["End write leveling","!Node 1","!Dct 0"]]
    # - Stop (or reset) at the end of DRAM training
    #     [["AmdInitPost: End"]]
];
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

use constant HDT_PATH => 'C:\Progra~1\AMD\HDT';
use constant HDT_PATH64 => 'C:\Progra~1\AMD\HDT';

use lib HDT_PATH;
use lib HDT_PATH64;
use strict;
use Time::HiRes qw(usleep);
use Getopt::Long; #Library to parse and interpret arguments
my $cmd;
my $MyResponse;
my $IsSimNow = 0;


# ############### PLEASE uncomment this block to use HDTOUT with SimNow ###########
# $IsSimNow = 1;
# use Win32::OLE;
# use Win32::OLE::Variant;
# $cmd = Win32::OLE->new('SimNow.Command') or die "Cannot open SimNow.Command\n";
# $MyResponse = Variant(VT_BSTR | VT_BYREF, ""); # response from SimNow


use hdtPerl;    # PLEASE comment out this line when working with SimNow

my $help;
my $logfile;
my $ip_addr;
my $password;
my $username;
my $emulator_num;
my $buffer_size;
my $FilterLow;
my $FilterHigh;

my $MaxSockets = 1;
my $MaxCores = {};
my $ActiveCoresCount = 0;
my $ActiveCoresList = {};
my $CurrentActiveCore;
my $HdtoutBP = {};
my $CoreScan = 0;
my $running = 1;
my $reboot_count = 0;
my $reset_issued = 0;
my $WorkingPath;
my $silicon_name="";
my $FileCodeHash = {};

$| = 1;

main();

sub main {
    ParseInput();
    open(OUTPUTFILE, ">$logfile") or die "can't open $logfile\n";

    printf "\n\nHDTOUT2008 Ver %x.%02x\n",$MajorVersion,$MinorVersion;
    printf OUTPUTFILE "\n\nHDTOUT2008 Ver %x.%02x\n",$MajorVersion,$MinorVersion;
    print "\"-h\" for help with usage\n\n";
    connect_to_target();

    # Stop script when Ctrl+C is pressed
    $SIG{INT} = sub { interrupt(); };

    # Handle crashes
    $SIG{BREAK} = sub { interrupt(); };
    $SIG{__DIE__} = sub { interrupt(); print ("\nNOTE: This script may not be able to enable HDTOUT on-the-fly on this system.\n      Please enable HDTOUT in BIOS and run this script again.\n");};

    allocate_new_core("0-0");   #BSP

    while($running) {
        poll_until_break();
        handle_reset();

        if ($CoreScan == 0) {
            get_number_of_cores();
            $CoreScan = 1;
        }

        for(my $Socket=0; $Socket<$MaxSockets; $Socket++) {
            for(my $Core=0; $Core<$MaxCores->{$Socket}; $Core++){
                attach_to($Socket, $Core);

                if($running == 1){
                    my $HdtoutHeader = 0;
                    my $val_junk = 0;
                    my $val_edx = 0;
                    read_register ("rDX", \$val_junk, \$val_edx);
                    if (($val_edx & 0xFFFF) == 0x99CC) {
                        allocate_new_core("$Socket-$Core");
                        $CurrentActiveCore = $ActiveCoresList->{"$Socket-$Core"};

                        $val_edx >>= 16;
                        if ($val_edx == 0x10BF) {
                            print ("\nHDTOUT init ($Socket-$Core)\n\n");
                            my $val_eax = 0;
                            read_register ("rAX", \$val_junk, \$val_eax);
                            $HdtoutHeader = $val_eax;
                            my $Version = 0;
                            $Version = read_mem_number($val_eax + 4, 2);
                            if ($Version > 0x0230) {
                                my $Major = $Version >> 8;
                                my $Minor = $Version & 0xFF;
                                print("\n\n\n\n* WARNING: This script may not work properly with this BIOS.\n");
                                printf("           Please update to version %X.%02X or later!", $Major, $Minor);
                                print("\n\nDo you want to continue anyway (Y/N)? ");
                                my $ToContinue = <STDIN>;
                                if($ToContinue =~ /^N/i){
                                    return;
                                }
                            }
                            if($HdtoutDefault==0){
                                write_mem_number($val_eax + 10,1,0);
                            }

                            # Set buffer size
                            write_mem_number($val_eax + 6,2,$buffer_size & 0xFFFF);

                            # Set EnableMask
                            write_mem_number($val_eax + 19,1, ($show_pci_reg_get << 1) | $show_pci_reg_set);

                            # Set Filter
                            if (($FilterHigh != 0) || ($FilterLow != 0)) {
                              write_mem_number($val_eax + 23, 4, $FilterLow);
                              write_mem_number($val_eax + 27, 4, $FilterHigh);
                            }

                            # Set BspOnly
                            write_mem_number ($val_eax + 31, 1, $bsp_only);

                            # Re-enable breakpoints after reset
                            if($HdtoutSelectTableEnable){
                                if($HdtoutHeader==0){
                                    my $val_dr3 = 0;
                                    read_register ("DR3", \$val_junk, \$val_dr3);
                                    $HdtoutHeader = $val_dr3;
                                }
                                enable_bp($HdtoutHeader);
                                $HdtoutBP->{"$Socket-$Core"} = 0;
                            }
                        } elsif ($val_edx == 0xA0BF) {
                          my $val_junk = 0;
                          my $val_eax = 0;
                          read_register ("rAX", \$val_junk, \$val_eax);
                          my $FileCode = $val_eax >> 16;
                          my $LineNum = $val_eax & 0xFFFF;
                          print_assert ($FileCode, $LineNum);
                        } elsif ($val_edx == 0xE0BF) {
                            if($ActiveCoresList->{"$Socket-$Core"}->{"CORE_ID"} == 0){
                                $Socket=$MaxSockets;
                                $Core=$MaxCores->{$Socket};
                                $running = 0;
                            }
                        } elsif ($val_edx == 0xC0BF) {
                            my $val_eax = 0;
                            my $val_ebx = 0;
                            read_register ("rAX", \$val_junk, \$val_eax);
                            read_register ("rBX", \$val_junk, \$val_ebx);
                            print_buffer($val_eax, $val_ebx);
                        } elsif ($val_edx == 0x1EBF) {
                            my $val_eax = 0;
                            my $val_ebx = 0;
                            read_register ("rAX", \$val_junk, \$val_eax);
                            read_register ("rBX", \$val_junk, \$val_ebx);
                            my $EventLevel = 0;
                            $EventLevel = $val_eax;
                            my $EventData = 0;
                            $EventData = $val_ebx;
                            if ($EventLevel == 0x1) {
                              printf ("[Warning] Heap Allocation Fail HeapSize: 0x%x\n", $EventData);
                            } elsif ($EventLevel == 0x2) {
                              printf ("[Warning] Can't Allocate Heap\n\tUse Non-buffer Mode With Default Filter define by IDS_DEBUG_PRINT_MASK\n",);
                            } else {
                              print "Undefined Event Level [$EventLevel]\n";
                            }
                        } elsif ($val_edx == 0xB0BF) {
                            print_recovery();
                            print_all_APs();
                            if ($reboot_test) {
                                if ($force_reset_to_enable) {
                                    hdtPerl::enable_assert_dbreq_on_cpu_reset(1);
                                    $val_junk = 0;
                                    my $val_dr2 = 0;
                                    hdtPerl::write_srb_register ("DR2", \$val_junk, \$val_dr2) or die;
                                    reset_system ();
                                    poll_until_break();
                                    handle_reset();
                                }
                                $reset_issued = 1;
                                $reboot_count++;
                                print "\n----------------------------------------------------------------------<Reset $reboot_count>\n";
                                print OUTPUTFILE "\n----------------------------------------------------------------------<Reset $reboot_count>\n";
                            } else {
                                $val_edx = 0xC0BF99CC;
                                hdtPerl::write_gpr_register ("rDX", \$val_junk, \$val_edx) or die;
                                single_step (3);
                                print "\nStop as requested!!!\n";
                                close(OUTPUTFILE);
                                return;
                            }
                        } elsif ($val_edx = 0xD0BF) {
                          TimeAnalyse();
                        }
                        if($HdtoutSelectTableEnable){
                            if(!defined $HdtoutBP->{"$Socket-$Core"}){
                                if($HdtoutHeader==0){
                                    my $val_dr3 = 0;
                                    read_register ("DR3", \$val_junk, \$val_dr3);
                                    $HdtoutHeader = $val_dr3;
                                }
                                enable_bp($HdtoutHeader);
                                $HdtoutBP->{"$Socket-$Core"} = 0;
                            }
                        }
                    }
                }else{
                    # Script has been stopped manually, print the rest of the buffer out
                    print_recovery();
                    print_all_APs();
                    close(OUTPUTFILE);
                    return;
                }
            }
        }

        if ($reset_issued) {
            $reset_issued = 0;
        } else {
            let_go();
        }
    }

    print_all_APs();
    print("Finish !!!\n");

    close(OUTPUTFILE);
}

sub allocate_new_core {
    if(!exists($ActiveCoresList->{$_[0]})){
        $ActiveCoresList->{$_[0]}->{"CORE_ID"} = $ActiveCoresCount;
        for(my $i=0; $i<10; $i++){
            $ActiveCoresList->{$_[0]}->{"COL_$i"} = "";
        }
        $ActiveCoresList->{$_[0]}->{"CURCOL"} = 0;
        $ActiveCoresCount++;
    }
}
sub print_assert {
  my $FileCode = sprintf "%x",$_[0];
  my $LineNum = $_[1];
  #Check the Filecode Hash
  if (%$FileCodeHash) {
    printf "\n[ASSERT] File:%s Line:%x\n",$FileCodeHash->{$FileCode}, $LineNum;
    printf OUTPUTFILE "\n[ASSERT] File:%s Line:%x\n",$FileCodeHash->{$FileCode}, $LineNum;
  } else {
    printf ("\n[ASSERT] Filecode:%s Line:%x\n", uc $FileCode, $LineNum);
    printf OUTPUTFILE ("\n[ASSERT] Filecode:%s Line:%x\n", uc $FileCode, $LineNum);
  }
  print "Press Ctrl+C for exit\n";
}

sub find_BF {
    my $BF = $_[0];
    my $WpInValid = 1;
    #validate the working path
    if ($WorkingPath ne undef) {
        if (-e $WorkingPath."/Agesa.h") {
            if (open (FILECODE, "<".$WorkingPath."/Proc/Mem/mm.h")) {
                $WpInValid = 0;
                my $state = 0;
                my $count = 0;
                while (<FILECODE>) {
                    if ($state == 0) {
                        if (/^\s*BFDevVendorIDReg,\s*\/\/\/\<.*DevVendorIDReg$/) {
                            $state = 1;
                        }
                    } elsif ($state == 1) {
                        if (/^\s*(BF\w+),/) {
                            $count++;
                            if ($count == hex($BF)) {
                                return $1;
                            }
                        } elsif (/^\s*\} BIT_FIELD_NAME;/) {
                            return $BF;
                        }
                    }
                }
                close FILECODE;
            }
        }
    }
    return $BF;
}

sub print_buffer {
    my $hdtout_array = $_[0];
    my $array_length = $_[1];
    if($array_length > 0){
        my $MemBuffer = read_mem_init($hdtout_array, $array_length);

        my $CurCol = $CurrentActiveCore->{"CURCOL"};
        for(my $i=0; $i<$array_length; $i++){
            my $Char = chr(read_mem_byte($MemBuffer,$i));
            # Special format
            if($Char eq "%"){
                $Char = chr(read_mem_byte($MemBuffer,++$i));
                # Vertical to horizontal display
                if($Char eq "v"){
                    $Char = chr(read_mem_byte($MemBuffer,++$i));
                    if($Char eq "h"){
                      $CurCol = read_mem_byte($MemBuffer,++$i) - ord("0");
                      $CurrentActiveCore->{"COL_0"} .= $CurrentActiveCore->{"COL_$CurCol"};
                      $CurrentActiveCore->{"COL_$CurCol"} = "";
                      $CurCol = 0;
                      if($i<($array_length-1)){
                        $Char = chr(read_mem_byte($MemBuffer,++$i));
                      }else{
                        $Char = "";
                      }
                    }elsif(($Char ge "0") && ($Char le "9")){
                      $CurCol = ord($Char) - ord("0");
                      if($i<($array_length-1)){
                        $Char = chr(read_mem_byte($MemBuffer,++$i));
                      }else{
                        $Char = "";
                      }
                    }else{
                      $Char = "%v".$Char;
                    }
                }else{
                    $Char = "%".$Char;
                }
            }

            $CurrentActiveCore->{"CURCOL"} = $CurCol;
            $CurrentActiveCore->{"COL_$CurCol"} .= $Char;
        }
        read_mem_exit($MemBuffer);

        # Insert SPD dump
        if ($dump_spd_data) {
            my @Fragments = split (/SPD Soc/,$CurrentActiveCore->{"COL_0"});

            $CurrentActiveCore->{"COL_0"} = "";
            foreach my $Piece (@Fragments) {
                if ($Piece =~ /^ket\s\d\sChannel\s\d\sDimm\s\d\:\s([\w]+)/) {
                    my $SPDdump = "";

                    $MemBuffer = read_mem_init(hex($1), 256);

                    for(my $i=0; $i<256; $i++){
                        if (($i % 16)==0) {
                            $SPDdump .= "\n";
                        }
                        $SPDdump .= sprintf ("%02X ",read_mem_byte($MemBuffer,$i));
                    }
                    read_mem_exit($MemBuffer);

                    $Piece = "SPD Soc$Piece";
                    my $x = substr($Piece,39,1,"$SPDdump\n\n");
                }
                $CurrentActiveCore->{"COL_0"} .= $Piece;
            }
        }

        # Insert undefined BF name
        if ($FilterLow & 0x10) {
            my @Fragments = split (/BF enum/,$CurrentActiveCore->{"COL_0"});
            $CurrentActiveCore->{"COL_0"} = "";
            my $PrevLen = 0;
            foreach my $Piece (@Fragments) {
                if ($Piece =~ /^:\s*([\w]+)/) {
                    my $Indent = "";
                    for (my $i = $PrevLen; $i < 20; $i++) {
                        $Indent .= " ";
                    }
                    my $BFstr = find_BF($1);
                    if ($BFstr =~ /^BF/) {
                        $Piece = "$Indent".pack("A30",$BFstr)." enum$Piece";
                    } else {
                        $Piece = "${Indent}BF enum$Piece";
                    }
                }
                $PrevLen = length ($Piece) - rindex ($Piece, '[');
                $CurrentActiveCore->{"COL_0"} .= $Piece;
            }
        }

        disp(0);
    }
}

sub disp {
    my $ruler = "\n\t\t\t 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F";

    if($CurrentActiveCore->{"CORE_ID"}==$_[0]){
        my $Str = $CurrentActiveCore->{"COL_0"};
        $CurrentActiveCore->{"COL_0"} = "";

        $Str =~ s/(Stage: .)/\1$ruler/g;
        $Str =~ s/\t/  /g;                 # Use 2-spaces for tab
        $Str =~ s/~/                                                                                                             /g;
#        $Str =~ s/\$/                                                                                                          (R)/g;

        print ($Str);
        print OUTPUTFILE ($Str);
    }
}

sub print_all_APs {
    for(my $Socket=0; $Socket<$MaxSockets; $Socket++) {
        for(my $Core=0; $Core<$MaxCores->{$Socket}; $Core++){
            if(exists($ActiveCoresList->{"$Socket-$Core"})){
                $CurrentActiveCore = $ActiveCoresList->{"$Socket-$Core"};
                if($CurrentActiveCore->{"CORE_ID"} != 0){
                    $CurrentActiveCore->{"COL_0"} = "AP[$Socket-$Core] ------------------------------------------------------\n".$CurrentActiveCore->{"COL_0"};
                    disp($CurrentActiveCore->{"CORE_ID"});
                }
            }
        }
    }
    if(!$IsSimNow){
        hdtPerl::enable_assert_dbreq_on_cpu_reset(0);
    }
}

sub print_recovery {
    for(my $Socket=0; $Socket<$MaxSockets; $Socket++) {
        for(my $Core=0; $Core<$MaxCores->{$Socket}; $Core++){
            if(exists($ActiveCoresList->{"$Socket-$Core"})){
                attach_to($Socket,$Core);
                $CurrentActiveCore = $ActiveCoresList->{"$Socket-$Core"};
                my $val_junk = 0;
                my $val_dr3 = 0;
                read_register ("DR3", \$val_junk, \$val_dr3);
                if($val_dr3 != 0){
                    my $Data = $val_dr3 + 512;
                    my $DataIndex = 0;
                    $DataIndex = read_mem_number($val_dr3 + 8, 2);
                    print_buffer($Data, $DataIndex);
                }
            }
        }
    }
}

sub enable_bp {
    my $addr = $_[0];

    if(scalar(@$HdtoutSelectTable) == 0){
        return;   # empty table
    }

    # Find number of breakpoints
    my $numbpunit = 0;
    foreach my $bpgroup(@$HdtoutSelectTable){
        if(scalar(@$bpgroup) == 0){
            die "\n\nERROR: Invalid Hdtout selection table!!!\n";
        }elsif(scalar(@$bpgroup) > 2){
            die "\n\nERROR: Invalid Hdtout selection table!!!\n";
        }
        foreach my $bpunit(@$bpgroup){
            foreach my $bp(@$bpunit){
                $numbpunit++;
            }
        }
    }
    write_mem_number($addr + 11,1,$numbpunit & 0xFF);

    # Setup breakpoints
    my $StrIndex = 56 + $numbpunit * 2 + 3;
    my $BpIndex = 56;
    foreach my $bpgroup(@$HdtoutSelectTable){
        for(my $j=0; $j<scalar(@$bpgroup); $j++){
            my $bpunit = $bpgroup->[$j];
            for(my $i=1; $i<=scalar(@$bpunit); $i++){
                my $bp = $bpunit->[$i-1];
                #only 1 bpunit in bpgroup set to Halt
                #else 1st one print on,2nd one print off
                my $Flag = (scalar(@$bpgroup)==1)?4:($j==0)?8:16;
                # ! prefix means status string, set BpFlag to Status String
                if ($bp =~ /^!(.*)/){
                    $Flag += 2;
                    $bp = $1;
                }
                # more than one bpunit in bpgroup set And flag
                if ($i < scalar(@$bpunit)){
                    $Flag += 1;
                }
                write_mem_number($addr + $BpIndex,1,$Flag);
                write_mem_number($addr + $BpIndex + 1,1,$StrIndex - 56);

                my @asciistr = unpack("C*",$bp);
                foreach(@asciistr){
                    write_mem_number($addr + $StrIndex,1,$_);
                    $StrIndex++;
                }
                write_mem_number($addr + $StrIndex,1,0);
                $StrIndex++;
                $BpIndex += 2;
            }
        }
    }
}

############ PORTING FUNCTIONS ###############################################

sub read_mem_number {
    my $Address = $_[0];
    my $Width = $_[1];
    my $TempBuffer = read_mem_init($Address, $Width);
    my $TempNumber = 0;
    for(my $i=$Width-1; $i>=0; $i--){
        $TempNumber = ($TempNumber * 256) + read_mem_byte($TempBuffer, $i);
    }
    read_mem_exit($TempBuffer);
    return $TempNumber;
}

sub write_mem_number {
    my $Address = $_[0];
    my $Width = $_[1];
    my $Value = $_[2];

    if($IsSimNow){
        my $hex_addr = sprintf("%s %X %X",($Width==4)?'d':(($Width==2)?'w':'b'), $Address, $Value);
        to_SimNow("debug.ExecCmd e$hex_addr", __LINE__, __FILE__);
    }else{
        my $Temp = hdtPerl::newba($Width);
        for (my $k = 0; $k < $Width; $k++) {
            hdtPerl::setba($Temp,$k,$Value & 0xFF);
            $Value = $Value >> 8;
        }
        hdtPerl::write_mem_buffer(0, $Address, $Width, 0, 6, 0, $Temp);
        read_mem_exit($Temp);
    }
}

sub read_mem_string {
    my $Address = $_[0];
    my $Width = $_[1];
    my $TempBuffer = read_mem_init($Address, $Width);
    my $string = "";
    for(my $i = 0; $i < $Width; $i++){
        $string .= chr (read_mem_byte($TempBuffer, $i));
    }
    read_mem_exit($TempBuffer);
    return $string;
}

sub read_mem_init($$) {
    my $addr = $_[0];
    my $length = $_[1];
    my $MemBuffer;

    if($IsSimNow){
        for(my $i=0; $i<$length; $i+=128){
            my $hex_addr = sprintf("%X", $addr + $i);
            to_SimNow("debug.ExecCmd db $hex_addr", __LINE__, __FILE__);
            to_SimNow("debug.MemDump", __LINE__, __FILE__);
            my $str = from_SimNow();
            my @TmpArr = split(/\n/,$str);
            foreach my $Tmp (@TmpArr){
                push(@{$MemBuffer},split(/ /,substr($Tmp,12,49)));
            }
        }
    }else{
        $MemBuffer = hdtPerl::newba($length);
        hdtPerl::read_mem_buffer(0, $addr, $length, 0, 6, 0, $MemBuffer);
    }
    return $MemBuffer;
}

sub read_mem_byte($$) {
    my $handle = $_[0];
    my $byte = $_[1];

    if($IsSimNow){
        return hex($handle->[$byte]);
    }else{
        return hdtPerl::getba($handle,$byte);
    }
}

sub read_mem_handle_number {
    my $handle = $_[0];
    my $byte = $_[1];
    my $Width = $_[2];
    my $TempNumber = 0;
    for(my $i=$Width-1; $i>=0; $i--){
        $TempNumber = ($TempNumber * 256) + read_mem_byte($handle, $byte + $i);
    }
    return $TempNumber;
}

sub read_mem_exit($){
    if($IsSimNow){
        $_[0] = [];
    }else{
        hdtPerl::delba($_[0]);
    }
}

sub connect_to_target {
    if($IsSimNow){
        enable_debugger();
        to_SimNow("debug.ExecCmd bc", __LINE__, __FILE__);
        to_SimNow("debug.ExecCmd bi 99cc", __LINE__, __FILE__);
        let_go();
    }else{
        my $version = "          ";
        hdtPerl::get_hdt_version ($version, 10) or die;
        $version =~ /([\d\.]*)/;
        $version = $1;
        print ("HDT v.$version\n\n");
        print OUTPUTFILE ("HDT v.$version\n\n");

        if($interactive_mode_en){
            hdtPerl::list_targets();
            print("Which Possum Index would you like to use? ");
            chomp($emulator_num = <STDIN>);
        }
        if ($ip_addr ne "") {
            print ("Connecting to HDT device $ip_addr...\n");
            if (hdtPerl::select_emulator ("protocol=yaap, ip=$ip_addr, username=$default_username, password=$default_password") == 0) {
                print("\nFail to connect to $ip_addr with default username and password!\n");
                print("  Enter username:");
                chomp($default_username = <STDIN>);
                print("  Enter password:");
                chomp($default_password = <STDIN>);
                print ("Try to connect one more time...\n");
                hdtPerl::select_emulator ("protocol=yaap, ip=$ip_addr, username=$default_username, password=$default_password") or die "\nFAIL TO CONNECT!!!\n";
            }
            print ("Connected to HDT device $ip_addr.\n");
        }else{
            print ("Connecting to HDT device $emulator_num...\n");
            hdtPerl::select_emulator_by_index($emulator_num) or die "\nFAIL TO CONNECT!!!\n";;
            print ("Connected to HDT device $emulator_num.\n");
            $exit_dbg_after_reset = 1;
        }

        my $val_junk = 0;
        my $val_dr2 = 0;
        my $status = 0;
        my $timeout = 100;
        hdtPerl::enter_debug();
        hdtPerl::enable_assert_dbreq_on_cpu_reset(0);
        hdtPerl::set_target_node(0) or die;
        hdtPerl::set_logc_core(0) or die;

        if (($version - 7.9) >= 0) {
            $status, $silicon_name = hdtPerl::get_silicon_name();
            print ("Target: $silicon_name\n");
            print OUTPUTFILE ("Target: $silicon_name\n");
        }

        # Check if HDTOUT is enabled
        $status = hdtPerl::read_srb_register ("DR2", \$val_junk, \$val_dr2);
        while ($status == 0) {
            hdtPerl::exit_debug(0);
            poll_until_break();
            $status = hdtPerl::read_srb_register ("DR2", \$val_junk, \$val_dr2);
            $timeout--;
            if($timeout==0){
                die "ERROR: Registers are inaccessible!\n";
            }
        }

        if($val_dr2 != 0x99CC){
            my $Answer = "N";
            if(!$force_reset_to_enable){
                print ("\nHDTOUT is not enabled. Do you want to RESET and enable it? (Y/N)");
                $Answer = <STDIN>;
            } else {
                $Answer = "Y";
            }
            if($Answer =~ /^Y/i){
                $force_reset_to_enable = 1;
                hdtPerl::enable_assert_dbreq_on_cpu_reset(1);   # trap on reset

                reset_system ();
                poll_until_break();
                handle_reset();
            }
        }
    }
}

sub poll_until_break {
    if($IsSimNow){
        while(SimNow_is_running()) {
          if ($running == 1) {
            usleep 3000;
          } else {
            $cmd->Exec("Shell.Stop", $MyResponse);
          }
        }
        enable_debugger();
    }else{
        my $dbrdy_value = 0;
        do {
            usleep 1000;
            hdtPerl::dbrdy_status(0, \$dbrdy_value);
        } while (($dbrdy_value == 0) && ($running == 1));
        usleep 1000;

        hdtPerl::enter_debug();
    }
}

sub handle_reset {
    if($IsSimNow){
    }elsif ($force_reset_to_enable) {
        # Re-enable HDTOUT if reset occurs
        my $val_junk = 0;
        my $val_dr2 = 0;
        my $val_eip = 0;
        my $val_edx = 0;

        hdtPerl::set_target_node(0) or die;
        hdtPerl::set_logc_core(0) or die;

        hdtPerl::read_srb_register ("DR2", \$val_junk, \$val_dr2) or die;

        if($val_dr2!=0x99cc){
            # print "IntChk single-step\n";
            hdtPerl::execute_hdt_misc_command(5,0);
            usleep(100000);
        }

        hdtPerl::read_srb_register ("DR2", \$val_junk, \$val_dr2) or die;
        if($val_dr2 != 0x99CC){
            print ("--- Reset Occurs -------------\n");
            print OUTPUTFILE ("\n--- Reset Occurs -------------\n\n");
            $val_junk = 0;
            $val_dr2 = 0x99CC;
            hdtPerl::write_srb_register ("DR2", \$val_junk, \$val_dr2) or die;
            # print "IntChk single-step\n";
            hdtPerl::execute_hdt_misc_command(5,0);
            usleep(100000);
            $HdtoutBP = {};
        }
    }
}

sub get_number_of_cores {
    if($IsSimNow || $bsp_only){
        $MaxSockets = 1;
        $MaxCores->{0} = 1;#get_CPU_count();  #No parallel training support for SimNow at the moment
    }else{
        my $NumCores = 0;
        hdtPerl::get_number_nodes(\$MaxSockets) or die;
        print ("\nThere are $MaxSockets sockets:\n");
        for(my $Socket=0; $Socket<$MaxSockets; $Socket++) {
            hdtPerl::set_target_node($Socket) or die;
            hdtPerl::num_logc_cores(\$NumCores) or die;
            print ("   Socket $Socket has $NumCores cores.\n");
            $MaxCores->{$Socket} = $NumCores;
        }
    }
}

sub attach_to($$) {
    if($IsSimNow){
        my $CPUNum = $_[1];
        to_SimNow("debug.Attach $CPUNum", __LINE__, __FILE__);
    }else{
        hdtPerl::set_target_node($_[0]) or die;
        usleep 1000;
        hdtPerl::set_logc_core($_[1]) or die;
        usleep 1000;
    }
}

sub let_go {
    if($IsSimNow){
        to_SimNow("debug.Attach 0", __LINE__, __FILE__);
        $cmd->Exec("Shell.Go", $MyResponse);
    }else{
        hdtPerl::ss(0);
        hdtPerl::exit_debug(0);
    }
}

sub single_step {
    my $count = $_[0];
    if($IsSimNow){
    }else{
        for (my $i=0; $i<$count; $i++) {
            hdtPerl::ss(0);
        }
    }
}

sub read_register($$$) {
    my $reg = $_[0];
    my $valhi = $_[1];
    my $vallo = $_[2];
    if($IsSimNow){
        to_SimNow("debug.ExecCmd r $reg", __LINE__, __FILE__);
        my $result = get_SimNow_return();
        if($result !~ /^\s*[0-9a-f].*/i){
           die "Fail to read $reg";
        }
        $$valhi = hex(substr($result,0,8));
        $$vallo = hex(substr($result,8,8));
   }else{
        if($reg =~ /^DR./){
            hdtPerl::read_srb_register ($reg, $valhi, $vallo) or die "Fail to read $reg register";
        }else{
            hdtPerl::read_gpr_register ($reg, $valhi, $vallo) or die "Fail to read $reg register";
        }
    }
}

############ SIMNOW LIBRARY ###############################################
sub to_SimNow($$$){
    ( my $command, my $line ) = @_;

    while(SimNow_is_running()) { usleep 1000; }
    if(!$cmd->Exec($command, $MyResponse)){
        die "SimNow command: \<$command\> failed at line $_[1] in $_[2].\n";
        return 0;
    }
  return 1;
}

sub from_SimNow(){
    return $MyResponse;
}

sub get_SimNow_return(){
    if($MyResponse =~ /.*\=\s*(\w*)/){
        return $1;
    }else{
        return $MyResponse;
    }
}

sub SimNow_is_running(){
    $cmd->Exec("Shell.Running", $MyResponse);
    return ($MyResponse eq "Yes");
}

sub enable_debugger(){
    do{
        $cmd->Exec("Debug.Enable", $MyResponse);
        usleep 1000;
        $cmd->Exec("Debug.EnableStatus", $MyResponse);
    }while(get_SimNow_return() ne "Enabled");
    usleep 1000;
}

sub get_CPU_count(){
    my $count = 0;
    to_SimNow("shell.CreatedDevices", __LINE__, __FILE__);
    my @lines = split(/\n/, from_SimNow());
    foreach my $l (@lines) {
        if($l =~ /^cpu\:/i){
            $count++;
        }
    }
    return $count;
}

sub reset_system {
    if($IsSimNow){
    }else{
        if ($use_cold_reset) {
            if ($use_relay_to_reset) {
                hdtPerl::assert_cold_reset();
                sleep 1;
                hdtPerl::deassert_cold_reset();
            } else {
                hdtPerl::write_io(0,0xCF9,8,0xE);   # trigger cold reset
            }
            if ($exit_dbg_after_reset) {
                hdtPerl::exit_debug(0);
            }
            sleep 1;
        } else {
            if ($use_relay_to_reset) {
                hdtPerl::assert_warm_reset();
                sleep 1;
                hdtPerl::deassert_warm_reset();
            } else {
                hdtPerl::write_io(0,0xCF9,8,6);     # trigger warm reset
            }
            if ($exit_dbg_after_reset) {
                hdtPerl::exit_debug(0);
            }
        }
    }
}

############ Command line interface ###############################################
sub ParseFilter {
  my @filters = @_;
  my %FilterBitMap = (
    "MEM_SETREG" => 0,
    "MEM_GETREG" => 1,
    "MEM_FLOW" => 2,
    "MEM_STATUS" => 3,
    "MEM_UNDEF_BF" => 4,


    "CPU_TRACE" =>  10,

    "GNB_TRACE" => 20,
    "GNB_PCIE_MISC" => 21,
    "GNB_PORTREG" =>  22,
    "GNB_HOSTREG" =>  23,
    "GNB_NB_MISC" =>  25,
    "GNB_GFX_MISC" => 27,
    "GNB_SMU" =>  28,


    "TOPO_TRACE" => 30,

    "FCH_TRACE" =>  40,

    "MAIN_FLOW" =>  50,
    "EVENT_LOG" =>  51,
    "PEFORMANCE" => 52,

    "IDS_TRACE" =>  53,
    "BVM_TRACE" =>  54,

    "S3" => 57,

    "LIB_PCI_RD" =>  58,
    "LIB_PCI_WR" =>  59,

    "TEST_POINT" =>  60,

    "PLAT_RSV1" =>  61,
    "PLAT_RSV2" =>  62,
    "PLAT_RSV3" =>  63,
  );
  my @ParsedFilter;
  foreach my $Prefix(@filters) {
      if ((grep {/^($Prefix)/i}keys %FilterBitMap) == ()) {
        printf "Can't found filter with \"%s\" prefix, ignored\n",$Prefix;
      } else {
        push @ParsedFilter,grep {/^($Prefix)/i}keys %FilterBitMap;
      }
  }
  #generate the final bitsmap
  printf  "filters: \n";
  foreach (@ParsedFilter){
    print "\t".$_."\n";
    my $f = uc $_;
    if ($FilterBitMap{$f}>=32) {
      $FilterHigh |= 1 << ($FilterBitMap{$f} - 32);
    } else {
      $FilterLow |= 1 << ($FilterBitMap{$f});
    }
  }
}
sub ParseYaap {
  my @yaap = @_;
  my %YaapHash;
  foreach (@yaap) {
    if(/^\s*(.+)=(.+)\s*$/) {
      $YaapHash{uc $1}=$2;
    }
  }
  foreach (keys %YaapHash) {
    if ((uc $_ ne "IP") and
        (uc $_ ne "USERNAME") and
        (uc $_ ne "PASSWORD")){
        printf "Unrecognized parameter %s in YAAP group\n\n",$_;
        $help = 1;
    }
  }
  $ip_addr = $YaapHash{"IP"};
  $password = $YaapHash{"PASSWORD"};
  $username = $YaapHash{"USERNAME"};

}
sub Initdefault {
  if ($logfile eq '') {
    my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime;
    $logfile = sprintf("C:\\HDTOUT\\hdtout_%d_%d_%d_%02d_%02d_%02d.txt",$mon+1,$mday,$year+1900,$hour,$min,$sec);
    mkdir ("C://HDTOUT");
  }

  if ($ip_addr eq '') {
    $ip_addr = $dft_ip_addr;
  }
  if ($password eq '') {
    $password =$default_password;
  }
  if ($username eq '') {
    $username =$default_username;
  }

  if ($emulator_num eq '') {
    $emulator_num = $dft_emulator_num;
  }

  if ($buffer_size eq '') {
    $buffer_size = $dft_buffer_size;
  }

# if (@filters == ()) {
#   @filters = @dft_filters;
# }
}
sub ParseInput {
  my @yaap;
  my @filters;
  GetOptions(
            'help'              => \$help,
            'logfile=s'         => \$logfile,
            'yaap=s{0,3}'             => \@yaap,
            'possum=s'          => \$emulator_num,
            'filter=s{0,64}'    => \@filters,
            'buffersize=s'      => \$buffer_size,
            'spd!'    => \$dump_spd_data,
            'wp=s'  => \$WorkingPath,
            'reset'             => \$force_reset_to_enable,
        ) or die "Incorrect usage! Try --help for help with usage\n";
  $logfile =~ s/\\/\//g;
  $WorkingPath =~ s/\\/\//g;
  ParseFilter(@filters);
  ParseYaap(@yaap);

#Init default value
  Initdefault();

#Init FileCode & File_Path Hash, if $WorkingPath provide.
  if ($WorkingPath ne undef) {
    InitFileCodeHash ($WorkingPath);
  }
  printf  "    logfile: %s \n",$logfile;
  printf  "    ip_addr: %s \n",$ip_addr;
  printf  "    password: %s \n", $password;
  printf  "    username: %s \n", $username;
  printf  "    possum: %s \n",$emulator_num;
  printf  "    buffer_size: %s \n",$buffer_size;
  printf  "    spd dump: %d \n",$dump_spd_data;
  printf  "    AGESA source: %s\n", $WorkingPath;

    if ($help) {
      ShowHelp();
    }

}
sub ShowHelp {
    print   "\n\nUSAGE: perl hdtout2008.pl  [-logfile FILE] [-possum number] [-buffersize size]\n";
    print   "  [-YAAP [ip=ipaddress] [username=name] [password=password]]\n";
    print   "  [-filter Filters] [-spd] [-wp]\n";
    print   "  Tips:All parameter is case-insensitive\n";
    print   "       Use first letter instead of whole word like -Y is short of -Yaap\n\n";
    print   "  [-Logfile FILE] Specify the logfile name include path\n";
    print   "    default is C:\\HDTOUT\\YMDT.txt \n";
    print   "    -Logfile C:\\Mylog\\pass.txt save the logfile to C:\\Mylog\\pass.txt\n";
    print   "  [-YAAP [ip=ipaddress] [username=name] [password=password]] Define YAAP config\n";
    print   "    ip=192.168.0.1 define the ip address as 192.168.0.1\n";
    print   "    username=root define the username as \"root\"\n";
    print   "    password=111111 define the password as \"111111\"\n";
    print   "  [-Possum number] Define emulator number\n";
    print   "    -Possum 1 connect to HDT using emulator number 1\n";
    print   "  [-Buffersize size] Define size of print buffer, by default will be 4096\n";
    print   "    define it as 0 will disable buffer mode to get realtime debug information\n";
    print   "    -Buffersize 1024 Define the buffer size as 1024\n";
    print   "  [-Spd] Switch of dump spd data, \"nospd\" for turn off it\n";
    print   "  [-reset] When specified, the script resets system to enable HDTOUT without asking\n";
    print   "  [-Wp] Optional, Specify the path to AGESA source code. When Assert occurs, \n";
    print   "    the script will get more detail information rather only Filecode\n";
    print   "  [-Filter Filters] Define the filter of the output string\n";
    print   "    Filters MAIN_FLOW    S3            EVENT_LOG    PEFORMANCE  \n";
    print   "            GNB_TRACE    GNB_PCIE_MISC GNB_PORTREG  GNB_HOSTREG \n";
    print   "            GNB_NB_MISC  GNB_GFX_MISC  GNB_SMU\n";
    print   "            MEM_FLOW     MEM_GETREG    MEM_SETREG   MEM_STATUS  MEM_UNDEF_BF\n";
    print   "            CPU_TRACE    TOPO_TRACE      FCH_TRACE    IDS_TRACE   BVM_TRACE\n";
    print   "            LIB_PCI_RD   LIB_PCI_WR    TEST_POINT\n";
    print   "            PLAT_RSV1    PLAT_RSV2     PLAT_RSV3\n";
    print   "    -Filter MAIN_FLOW GNB_PCIE \n";
    print   "            Ignore other output except Main flow & GNB PCIE part\n";
    print   "    Tips: Wildcard is accepted, use few character instead of whole word group\n";
    print   "    M is Wildcard of MEM_FLOW MEM_GETREG MEM_SETREG MEM_STATUS MAIN_FLOW\n";
    print   "    gnb is Wildcard of All GNB filters,GNB_PCIE GNB_PCIE_MISC GNB_PORTREG ...\n";
    print   "\n    Use Ctrl+C or Ctrl+Break to stop the script if it does not response.\n\n";
    print   "Guide to use HDTOUT for S3 resume, when HDTOUT isn't enable by build or Setup.\n";
    print   " Step 1: Open HDT Application Enable \"HDT on CPU reset\"\n";
    print   " Step 2: Power On target, make sure it been trap on reset state\n";
    print   " Step 3: Run This script, when pop \"HDTOUT is not enabled.\"\n";
    print   "         Select \"N\" to let it enable without reset\n";

    exit 0;
}

sub InitFileCodeHash {
  (my $_WorkingPath) = @_;
  #Check Validity of AGESA path
  if (-e $_WorkingPath."/Agesa.h") {
    open (FILECODE, "<".$WorkingPath."/Include/FileCode.h") or print "Can't find Filecode.h\n";
    my $FileCode;
    while (<FILECODE>) {
      if (/^#define\s+(.+?)\s+\(0x(.+)\)\s*$/) {
        my $FilePath = $1;
          $FileCode = hex $2;
        $FilePath =~ s/(.+?)_FILECODE$/$1.c/;
        $FilePath=~s/_/\//g;
        $FileCodeHash->{$FileCode} = $FilePath;
      }
    }
    close FILECODE;
  } else {
      print "Warnng: Working Path is Invalid\n";
  }
}
sub TimeAnalyse {
    print "Time Analyse Function\n";
    my $val_eax = 0;
    my $val_junk = 0;
    my $version = 0;
    my $CurrentIndex;
    my $MemoryBlockHandle;
    my $TscInMhz;
    my $DescriptionValid = 0;
    my $HeaderSize;
    my $PerfUnitSize;
    read_register ("rAX", \$val_junk, \$val_eax);
    my $signature = read_mem_string ($val_eax, 4);
    my $DescriptionMapping = {
      #Version 0.1.0.0
      0x00010000 =>
      {
        0x100 => "BeginProcAmdInitEarly",
        0x101 => "EndProcAmdInitEarly",
        0x102 => "BeginAmdHtInitialize",
        0x103 => "EndAmdHtInitialize",
        0x104 => "BeginGnbInitAtEarlier",
        0x105 => "EndGnbInitAtEarlier",
        0x106 => "BeginAmdCpuEarly",
        0x107 => "EndAmdCpuEarly",
        0x108 => "BeginGnbInitAtEarly",
        0x109 => "EndGnbInitAtEarly",
        0x10A => "BeginProcAmdInitEnv",
        0x10B => "EndProcAmdInitEnv",
        0x10C => "BeginInitEnv",
        0x10D => "EndInitEnv",
        0x10E => "BeginGnbInitAtEnv",
        0x10F => "EndGnbInitAtEnv",
        0x110 => "BeginProcAmdInitLate",
        0x111 => "EndProcAmdInitLate",
        0x112 => "BeginCreatSystemTable",
        0x113 => "EndCreatSystemTable",
        0x114 => "BeginDispatchCpuFeaturesLate",
        0x115 => "EndDispatchCpuFeaturesLate",
        0x116 => "BeginAmdCpuLate",
        0x117 => "EndAmdCpuLate",
        0x118 => "BeginGnbInitAtLate",
        0x119 => "EndGnbInitAtLate",
        0x11A => "BeginProcAmdInitMid",
        0x11B => "EndProcAmdInitMid",
        0x11C => "BeginDispatchCpuFeaturesMid",
        0x11D => "EndDispatchCpuFeaturesMid",
        0x11E => "BeginInitMid",
        0x11F => "EndInitMid",
        0x120 => "BeginGnbInitAtMid",
        0x121 => "EndGnbInitAtMid",
        0x122 => "BeginProcAmdInitPost",
        0x123 => "EndProcAmdInitPost",
        0x124 => "BeginGnbInitAtPost",
        0x125 => "EndGnbInitAtPost",
        0x126 => "BeginAmdMemAuto",
        0x127 => "EndAmdMemAuto",
        0x128 => "BeginAmdCpuPost",
        0x129 => "EndAmdCpuPost",
        0x12A => "BeginGnbInitAtPostAfterDram",
        0x12B => "EndGnbInitAtPostAfterDram",
        0x12C => "BeginProcAmdInitReset",
        0x12D => "EndProcAmdInitReset",
        0x12E => "BeginInitReset",
        0x12F => "EndInitReset",
        0x130 => "BeginHtInitReset",
        0x131 => "EndHtInitReset",
        0x132 => "BeginProcAmdInitResume",
        0x133 => "EndProcAmdInitResume",
        0x134 => "BeginAmdMemS3Resume",
        0x135 => "EndAmdMemS3Resume",
        0x136 => "BeginDispatchCpuFeaturesS3Resume",
        0x137 => "EndDispatchCpuFeaturesS3Resume",
        0x138 => "BeginSetCoresTscFreqSel",
        0x139 => "EndSetCoresTscFreqSel",
        0x13A => "BeginMemFMctMemClr_Init",
        0x13B => "EndnMemFMctMemClr_Init",
        0x13C => "BeginMemBeforeMemDataInit",
        0x13D => "EndMemBeforeMemDataInit",
        0x13E => "BeginProcAmdMemAuto",
        0x13F => "EndProcAmdMemAuto",
        0x140 => "BeginMemMFlowC32",
        0x141 => "EndMemMFlowC32",
        0x142 => "BeginMemInitializeMCT",
        0x143 => "EndMemInitializeMCT",
        0x144 => "BeginMemSystemMemoryMapping",
        0x145 => "EndMemSystemMemoryMapping",
        0x146 => "BeginMemDramTraining",
        0x147 => "EndMemDramTraining",
        0x148 => "BeginMemOtherTiming",
        0x149 => "EndMemOtherTiming",
        0x14A => "BeginMemUMAMemTyping",
        0x14B => "EndMemUMAMemTyping",
        0x14C => "BeginMemMemClr",
        0x14D => "EndMemMemClr",
        0x14E => "BeginMemMFlowTN",
        0x14F => "EndMemMFlowTN",
        0x150 => "BeginAgesaHookBeforeDramInit",
        0x151 => "EndAgesaHookBeforeDramInit",
        0x152 => "BeginProcMemDramTraining",
        0x153 => "EndProcMemDramTraining",
      },
    };

    #if the signature is "TIME" it is the old formate header
    if ($signature eq "TIME") {
      $HeaderSize = 12;
      $PerfUnitSize = 12;
      $CurrentIndex = read_mem_number ($val_eax + 4, 4);
      $MemoryBlockHandle = read_mem_init ($val_eax, $HeaderSize + $CurrentIndex * $PerfUnitSize);
      $TscInMhz = read_mem_handle_number ($MemoryBlockHandle, 8, 4);
    } elsif ($signature eq "PERF") {
      #"PERF" is new structure with Version field
      $version = read_mem_number ($val_eax + 4, 4);
      #Version 0.1.0.0
      if ($version <= 0x00010000) {
        $HeaderSize = 16;
        $PerfUnitSize = 20;
        $CurrentIndex = read_mem_number ($val_eax + 8, 4);
        $MemoryBlockHandle = read_mem_init ($val_eax, $HeaderSize + $CurrentIndex * $PerfUnitSize);
        $TscInMhz = read_mem_handle_number ($MemoryBlockHandle, 12, 4);
        $DescriptionValid = 1;
      }
    }

    printf "Tsc In Mhz: %d Index %d\n", $TscInMhz, $CurrentIndex;
    my $lastTimeInMs = 0;
    #Display the Performance information
    for (my $i = 0; $i < $CurrentIndex; $i++) {
      my $TimeInMs;
      #With out description ID
      if ($DescriptionValid == 0) {
        my $FileCode = read_mem_handle_number ($MemoryBlockHandle, $HeaderSize + ($i * $PerfUnitSize) + 2, 2);
        my $Line = read_mem_handle_number ($MemoryBlockHandle, $HeaderSize + ($i * $PerfUnitSize) + 0, 2);
        $TimeInMs = read_mem_handle_number ($MemoryBlockHandle, $HeaderSize + ($i * $PerfUnitSize) + 4, 8)/$TscInMhz/1000;
        my $delta = sprintf "D[+%05.4fms]", $TimeInMs - $lastTimeInMs;
        if ($FileCodeHash->{$FileCode} ne undef) {
            printf "%-40s Line: %04x                  [%05.3f ms] %s\n", $FileCodeHash->{$FileCode}, $Line, $TimeInMs, $delta;
            printf OUTPUTFILE "%-40s Line: %04x                  [%05.3f ms] %s\n", $FileCodeHash->{$FileCode}, $Line, $TimeInMs, $delta;
        } else {
            printf "FileCode %x Line: %x [%5.3f ms] %s\n", $FileCode, $Line, $TimeInMs, $delta;
            printf OUTPUTFILE "FileCode %x Line: %x [%5.3f ms] %s\n", $FileCode, $Line, $TimeInMs, $delta;
        }
      } else {
        #Version small & equal than v0.1.0.0
        if ($version <= 0x00010000) {
          my $FileCode = read_mem_handle_number ($MemoryBlockHandle, $HeaderSize + ($i * $PerfUnitSize) + 2, 2);
          my $Line = read_mem_handle_number ($MemoryBlockHandle, $HeaderSize + ($i * $PerfUnitSize) + 0, 2);
          my $DescriptionId = read_mem_handle_number ($MemoryBlockHandle, $HeaderSize + ($i * $PerfUnitSize) + 4, 8);
          $TimeInMs = read_mem_handle_number ($MemoryBlockHandle, $HeaderSize + ($i * $PerfUnitSize) + 12, 8)/$TscInMhz/1000;
          my $V0100 = 0x00010000;
          my $Description = $DescriptionMapping->{$V0100}->{$DescriptionId};
          my $delta = sprintf "D[+%05.4fms]", $TimeInMs - $lastTimeInMs;
          if ($FileCodeHash->{$FileCode} ne undef) {
              printf "%s:\n  %-48s Line: %04x [%05.3f ms] %s\n",$Description, $FileCodeHash->{$FileCode}, $Line, $TimeInMs, $delta;
              printf OUTPUTFILE "%s:\n  %-48s Line: %04x [%05.3f ms] %s\n",$Description, $FileCodeHash->{$FileCode}, $Line, $TimeInMs, $delta;
          } else {
              printf "%-35s: FileCode %04x Line: %04x [%5.3f ms] %s\n", $Description, $FileCode, $Line, $TimeInMs, $delta;
              printf OUTPUTFILE "%-35s: FileCode %04x Line: %04x [%5.3f ms] %s\n", $Description, $FileCode, $Line, $TimeInMs, $delta;
          }
        }
      }
      $lastTimeInMs = $TimeInMs;
    }
    $running = 0;
}

sub interrupt {
    $running = 0;
    if(!$IsSimNow) {
        hdtPerl::enable_assert_dbreq_on_cpu_reset (0);
    }
}
