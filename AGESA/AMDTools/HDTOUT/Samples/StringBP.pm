# Version 1.3.0
# Date: 02/11/10

package StringBP;
use strict;

use constant HDT_PATH => 'C:\Progra~1\AMD\HDT';
use constant HDT_PATH64 => 'C:\Progra~1\AMD\HDT';
use lib HDT_PATH;
use lib HDT_PATH64;
use hdtPerl;

use Time::HiRes qw(usleep);

my $BufferSize = 4096;
my $MaxSockets = 1;
my $MaxCores = {};
my $BreakpointList = [];
my $show_pci_reg_set = 1;
my $show_pci_reg_get = 1;
$MaxCores->{0} = 1;

########################################################################
## String Breakpoint constructor                                      ##
########################################################################

sub new {
    my $class = shift;
    my $self  = {};
    bless($self, $class);
    return $self;
}

########################################################################
##                                                                    ##
## Set breakpoints                                                    ##
##                                                                    ##
## Input: A breakpoint list that has the following format             ##
## [                                                                  ##
##    # BP1: Break on sending MRS 1 of DCT 1, node 0                  ##
##    ["MR1", "!Dct 1", "!Node 0"],                                   ##
##    # BP2: Break on sending control word 2 for any Dcts of Node 1   ##
##    ["RC2", "!Node 1"]                                              ##
## ];                                                                 ##
########################################################################

sub set_bp($) {
    my $self = shift;
    $BreakpointList = $_[0];
#    my $NumCores = 0;
#    hdtPerl::get_number_nodes(\$MaxSockets) or die;
#    #print ("\nThere are $MaxSockets sockets:\n");
#    for(my $Socket=0; $Socket<$MaxSockets; $Socket++) {
#        hdtPerl::set_target_node($Socket) or die;
#        hdtPerl::num_logc_cores(\$NumCores) or die;
#        #print ("   Socket $Socket has $NumCores cores.\n");
#        $MaxCores->{$Socket} = $NumCores;
#    }
}

########################################################################
##                                                                    ##
## Trap on breakpoints                                                ##
##                                                                    ##
##    Return value = 0: No breakpoints hit                            ##
##    Return value = 1: On breakpoint hit                             ##
########################################################################

sub trap {
    my $self = shift;

    my $running = 1;

    # Stop script when Ctrl+C is pressed
    $SIG{INT} = sub { $running = 0; };

    # Trap on reset
    hdtPerl::enable_assert_dbreq_on_cpu_reset(1);

    while($running) {
        my $dbrdy_value = 0;
        do {
            usleep 1000;
            hdtPerl::dbrdy_status(0, \$dbrdy_value);
        } while (($dbrdy_value == 0) && ($running == 1));
        usleep 1000;
        hdtPerl::enter_debug();

        handle_reset();

        for(my $Socket=0; $Socket<$MaxSockets; $Socket++) {
            for(my $Core=0; $Core<$MaxCores->{$Socket}; $Core++){
                hdtPerl::set_target_node($Socket) or die;
                usleep 1000;
                hdtPerl::set_logc_core($Core) or die;
                usleep 1000;

                if($running != 0){
                    my $val_junk = 0;
                    my $val_edx = 0;

                    hdtPerl::read_gpr_register ("rDX", \$val_junk, \$val_edx) or next;

                    if (($val_edx & 0xFFFF) == 0x99CC) {
                        $val_edx >>= 16;
                        my $val_junk = 0;
                        my $val_eax = 0;
                        if ($val_edx == 0x10BF) {
                            hdtPerl::read_gpr_register ("rAX", \$val_junk, \$val_eax) or die;
                            # write_mem_number($val_eax + 6,2,$BufferSize & 0xFFFF);
                            # Set EnableMask
                            write_mem_number($val_eax + 19,1, ($show_pci_reg_get << 1) | $show_pci_reg_set);
                        } elsif ($val_edx == 0xC0BF) {
                            hdtPerl::read_srb_register ("DR3", \$val_junk, \$val_eax) or die;
                        } elsif ($val_edx == 0xE0BF) {
                            if (($Socket==0) && ($Core==0)){
                                hdtPerl::enable_assert_dbreq_on_cpu_reset(0);
                                return 0;
                            }
                        } elsif ($val_edx == 0xB0BF) {
                            $val_edx = 0xC0BF99CC;
                            hdtPerl::write_gpr_register ("rDX", \$val_junk, \$val_edx) or die;

                            my $val_ebx=0;
                            hdtPerl::read_gpr_register ("rBX", \$val_junk, \$val_ebx) or die;
                            my $val_ebx_lo=$val_ebx&0xFFFF;
                            hdtPerl::write_gpr_register ("rBX", \$val_junk, \$val_ebx_lo) or die;

                            $val_ebx = $val_ebx >> 16;
                            my $bpnum = 0;
                            foreach my $bpunit(@$BreakpointList){
                                $bpnum++;
                                foreach my $bp(@$bpunit){
                                    $val_ebx--;
                                }
                                if($val_ebx<=0) {last;}
                            }
                            hdtPerl::enable_assert_dbreq_on_cpu_reset(0);
                            return $bpnum;
                        }
                        setup_bp ($val_eax);
                    }
                }
            }
        }
        hdtPerl::ss(0);
        hdtPerl::exit_debug(0);
    }

    hdtPerl::enable_assert_dbreq_on_cpu_reset(0);
}

########################################################################
##                                                                    ##
## Read the last string that one of breakpoints hit                   ##
##                                                                    ##
########################################################################

sub read_bp_string {
    my $self = shift;
    my $val_junk=0;
    my $val_eax=0;
    my $val_ebx=0;
    hdtPerl::read_gpr_register ("rAX", \$val_junk, \$val_eax) or die;
    hdtPerl::read_gpr_register ("rBX", \$val_junk, \$val_ebx) or die;
    return get_buffer($val_eax,$val_ebx);
}

########################################################################
##                                                                    ##
## Read status string                                                 ##
##                                                                    ##
########################################################################

sub read_status_string {
    my $self = shift;
    my $val_junk=0;
    my $val_dr3=0;
    hdtPerl::read_srb_register ("DR3", \$val_junk, \$val_dr3) or die;
    return get_buffer($val_dr3+356,150);
}

##--------------------------------------------
## Private: setup breakpoints
##--------------------------------------------

sub setup_bp($) {
    my $addr = $_[0];

    # Disable HDTOUT streaming
    write_mem_number($addr + 10,1,0);

    # Find number of breakpoints
    my $numbpunit = 0;
    foreach my $bpunit(@$BreakpointList){
        foreach my $bp(@$bpunit){
            $numbpunit++;
        }
    }
    write_mem_number($addr + 11,1,$numbpunit & 0xFF);

    # Setup breakpoints
    my $StrIndex = 56 + $numbpunit * 2 + 3;
    my $BpIndex = 56;
    foreach my $bpunit(@$BreakpointList){
        for(my $i=1; $i<=scalar(@$bpunit); $i++){
            my $bp = $bpunit->[$i-1];
            my $Flag = 4;
            if ($bp =~ /^!(.*)/){
                $Flag += 2;
                $bp = $1;
            }
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

##--------------------------------------------
## Private: Get a string from heap
##--------------------------------------------
sub get_buffer {
    my $hdtout_array = $_[0];
    my $array_length = $_[1];
    my $MemBuffer = hdtPerl::newba($array_length);
    hdtPerl::read_mem_buffer(0, $hdtout_array, $array_length, 0, 6, 0, $MemBuffer);

    my $Str="";
    for(my $i=0; $i<$array_length; $i++){
        my $Char = chr(hdtPerl::getba($MemBuffer,$i));
        if(ord($Char)==0){
          last;
        }else{
          $Str .= $Char;
        }
    }
    hdtPerl::delba($MemBuffer);
    $Str =~ s/^\s*//;
    $Str =~ s/\s*\n*$//;
    return $Str;
}

sub write_mem_number {
    my $Address = $_[0];
    my $Width = $_[1];
    my $Value = $_[2];
    my $Temp = hdtPerl::newba($Width);
    for (my $k = 0; $k < $Width; $k++) {
        hdtPerl::setba($Temp,$k,$Value & 0xFF);
        $Value = $Value >> 8;
    }
    hdtPerl::write_mem_buffer(0, $Address, $Width, 0, 6, 0, $Temp);
    hdtPerl::delba($Temp);
}

sub handle_reset {
    # Re-enable HDTOUT if reset occurs
    my $val_junk = 0;
    my $val_dr2 = 0;
    my $val_eip = 0;
    my $val_edx = 0;

    hdtPerl::set_target_node(0) or die;
    hdtPerl::set_logc_core(0) or die;

    hdtPerl::read_srb_register ("DR2", \$val_junk, \$val_dr2) or die;

    if($val_dr2!=0x99cc){
        # print "IntChk single-step \n";
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
        hdtPerl::execute_hdt_misc_command(5,0);
        usleep(100000);
        hdtPerl::exit_debug(0);
        my $dbrdy_value = 0;
        do {
            usleep 1000;
            hdtPerl::dbrdy_status(0, \$dbrdy_value);
        } while ($dbrdy_value == 0);
        usleep 1000;
        hdtPerl::enter_debug();
    }
}

1;  # PACKAGE END
