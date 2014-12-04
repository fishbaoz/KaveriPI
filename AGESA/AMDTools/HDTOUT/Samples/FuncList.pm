# Version 1.0.1
# Date: 07/24/09

package FuncList;
use strict;

use constant HDT_PATH => 'C:\Progra~1\AMD\HDT';
use constant HDT_PATH64 => 'C:\Progra~1\AMD\HDT';
use lib HDT_PATH;
use lib HDT_PATH64;
use hdtPerl;

use Time::HiRes qw(usleep);

##############################################
## FuncList constructor                     ##
##############################################

sub new {
    my $class = shift;
    my $self  = {};
    $self->{FUNC_LIST}   = {};
    $self->{BUFFER_ADDR} = 0;
    $self->{OLD_ESP} = 0;
    $self->{FS_BASE} = undef;
    $self->{CR4} = undef;
    $self->{HWCR_HI} = undef;
    $self->{HWCR_LO} = undef;
    bless($self, $class);
    return $self;
}

##############################################
##                                          ##
## Get a list of supported functions        ##
##                                          ##
##      Requirement: HDT in debug mode      ##
##############################################

sub get_list {
    my $self = shift;
    my $func_list_addr = 0;
    my $func_name_list = [];

    my $val_junk = 0;
    my $val_dr3 = 0;
    hdtPerl::read_srb_register ("DR3", \$val_junk, \$val_dr3) or die;

    if($val_dr3 != 0){
        $func_list_addr = $val_dr3 + 12;
    } else {
        return $func_name_list;
    }

    $func_list_addr = read_mem_number($func_list_addr, 4);
    if($func_list_addr != 0){
        while(1){
            my $func_addr = 0;
            hdtPerl::read_memory32(0, $func_list_addr, 32, \$val_junk, \$func_addr);
            if(($func_addr >> 20) == 0xFFF){
                my $mem_buff = hdtPerl::newba(40);
                hdtPerl::read_mem_buffer(0, $func_list_addr + 4, 40, 0, -1, 0, $mem_buff);
                my $func_name = "";
                for(my $i=0; $i < 40; $i++){
                    my $Char = hdtPerl::getba($mem_buff,$i);
                    if($Char == 0){
                        last;
                    }
                    $func_name .= chr($Char);
                }
                hdtPerl::delba($mem_buff);
                $self->{FUNC_LIST}->{$func_name} = $func_addr;
                push(@$func_name_list, $func_name);
                $func_list_addr += 44;
            } else {
                last;
            }
        }
    }

    return $func_name_list;
}

##############################################
##                                          ##
## Allocate buffer on stack                 ##
## Only one buffer can be allocated         ##
##                                          ##
## IN: ClCnt - Number of cache lines        ##
## OUT: 0 - FAIL                            ##
##      !0 - BufferAddr                     ##
##                                          ##
##      Requirement: HDT in debug mode      ##
##############################################

sub allocate_buffer {
    my $self = shift;
    my $ClCnt = $_[0];

    if(($ClCnt != 0) && ($self->{BUFFER_ADDR} == 0)){
        my $BufferSize = 64 * $ClCnt;
        my $val_junk = 0;
        my $val_esp = 0;
        hdtPerl::read_gpr_register ("rSP", \$val_junk, \$val_esp);
        $self->{OLD_ESP} = $val_esp;
        $val_esp = ($val_esp - $BufferSize - 15) & 0xFFFFFFF0;
        if($val_esp > 0x40100){
            $self->{BUFFER_ADDR} = $val_esp;
            hdtPerl::write_gpr_register ("rSP", \$val_junk, \$val_esp);
            return $val_esp;
        }
    }
    return 0;
}

##############################################
##                                          ##
## Deallocate buffer                        ##
##                                          ##
## OUT: 0 - FAIL                            ##
##      1 - SUCCEED                         ##
##                                          ##
##      Requirement: HDT in debug mode      ##
##############################################

sub deallocate_buffer {
    my $self = shift;

    my $val_junk = 0;
    my $val_esp = 0;
    hdtPerl::read_gpr_register ("rSP", \$val_junk, \$val_esp);
    if($self->{BUFFER_ADDR} == $val_esp){
        $val_esp = $self->{OLD_ESP};
        hdtPerl::write_gpr_register ("rSP", \$val_junk, \$val_esp);
        $self->{BUFFER_ADDR} = 0;
        return 1;
    }
    return 0;
}

##############################################
##                                          ##
## Set physical address high                ##
##                                          ##
## OUT: 0 - FAIL                            ##
##      1 - SUCCEED                         ##
##                                          ##
##      Requirement: HDT in debug mode      ##
##############################################

sub set_phy_addr_hi {
    my $self = shift;
    my $phy_addr_hi = $_[0];

    # Set FS_BASE
    my $MSR_hi = 0;
    my $MSR_lo = 0;
    if(!defined $self->{FS_BASE}){
        hdtPerl::read_msr_by_addr(0, 0xC0000100, 64, \$MSR_hi, \$MSR_lo);
        $self->{FS_BASE} = $MSR_hi;
    }
    hdtPerl::write_msr_by_addr(0, 0xC0000100, 64, $phy_addr_hi, 0);

    # Enable SSE
    my $val_junk = 0;
    my $val_cr4 = 0;
    hdtPerl::read_srb_register ("CR4", \$val_junk, \$val_cr4);
    if(!defined $self->{CR4}){
        $self->{CR4} = $val_cr4;
    }
    $val_cr4 = $val_cr4 | (1 << 9);
    hdtPerl::write_srb_register ("CR4", \$val_junk, \$val_cr4);

    # Disable Wrap32
    if(!defined $self->{HWCR_LO}){
        hdtPerl::read_msr_by_addr(0, 0xC0010015, 64, \$MSR_hi, \$MSR_lo);
        $self->{HWCR_HI} = $MSR_hi;
        $self->{HWCR_LO} = $MSR_lo;
    }
    $MSR_lo |= 0x00020000;
    $MSR_lo &= 0xFFFF7FFF;
    hdtPerl::write_msr_by_addr(0, 0xC0010015, 64, $MSR_hi, $MSR_lo);

    return 1;
}

##############################################
##                                          ##
## Restore physical address high            ##
##                                          ##
## OUT: 0 - FAIL                            ##
##      1 - SUCCEED                         ##
##                                          ##
##      Requirement: HDT in debug mode      ##
##############################################

sub restore_phy_addr_hi {
    my $self = shift;
    my $phy_addr_hi = $_[0];

    if(defined $self->{FS_BASE}){
        my $MSR_hi = $self->{FS_BASE};
        hdtPerl::write_msr_by_addr(0, 0xC0000100, 64, $MSR_hi, 0);

        my $val_junk = 0;
        my $val_cr4 = $self->{CR4};
        hdtPerl::write_srb_register ("CR4", \$val_junk, \$val_cr4);

        $MSR_hi = $self->{HWCR_HI};
        my $MSR_lo =  $self->{HWCR_LO};
        hdtPerl::write_msr_by_addr(0, 0xC0010015, 64, $MSR_hi, $MSR_lo);
        return 1;
    }
    return 0;
}

##############################################
##                                          ##
## Call a function in function list         ##
##                                          ##
##      Requirement: HDT in debug mode      ##
##############################################

sub call {
    my $self = shift;
    my $func_name = $_[0];

    # Save stack pointer
    my $val_junk = 0;
    my $val_esp = 0;
    hdtPerl::read_gpr_register ("rSP", \$val_junk, \$val_esp);
    my $old_esp = $val_esp;

    # Process parameters
    if($func_name =~ /\w+\((.*)\)/){
        if($1 ne ""){
            # Function has parameters
            my @params = split(/,/,$1);
            for(my $i=(scalar(@params)-1); $i >= 0; $i--){
                my $ParamWidth = 0;
                if($params[$i] eq "ClCnt"){
                    $ParamWidth = 2;
                }elsif(($params[$i] eq "BufferAddr")||($params[$i] eq "PhyAddrLo")){
                    $ParamWidth = 4;
                }else{
                    die ("\nERROR: Unsupported parameter \'".$params[$i]."\' in function $func_name.\n      Your script may be outdated.");
                }

                # Push parameter on stack
                $val_esp -= $ParamWidth;
                write_mem_number($val_esp,$ParamWidth,$_[$i+1]);
            }
        }
    }

    # Setup return point
    my $val_eip = 0;
    hdtPerl::read_gpr_register ("rIP", \$val_junk, \$val_eip);
    $val_esp -= 4;
    write_mem_number($val_esp,4,$val_eip);

    # Setup breakpoint
    my $MSR_hi = 0;
    my $MSR_lo = 0;
    hdtPerl::read_msr_by_addr(0, 0xC001100A, 64, \$MSR_hi, \$MSR_lo);
    hdtPerl::write_msr_by_addr(0, 0xC001100A, 64, $MSR_hi, $MSR_lo | 1);
    my $val_dr0 = 0;
    hdtPerl::read_srb_register ("DR0", \$val_junk, \$val_dr0);
    hdtPerl::write_srb_register ("DR0", \$val_junk, \$val_eip);
    my $val_dr7 = 0;
    hdtPerl::read_srb_register ("DR7", \$val_junk, \$val_dr7);
    $val_dr7 = $val_dr7 | 2;
    hdtPerl::write_srb_register ("DR7", \$val_junk, \$val_dr7);

    # Jump to function
    hdtPerl::write_gpr_register ("rSP", \$val_junk, \$val_esp);
    $val_eip = $self->{FUNC_LIST}->{$func_name};
    hdtPerl::write_gpr_register ("rIP", \$val_junk, \$val_eip);

    # Execute and wait for return
    hdtPerl::exit_debug(0);
    my $dbrdy_value = 0;
    do {
        usleep 1000;
        hdtPerl::dbrdy_status(0, \$dbrdy_value);
    } while ($dbrdy_value == 0);
    usleep 1000;

    # Clean up, restore everything
    hdtPerl::write_msr_by_addr(0, 0xC001100A, 64, $MSR_hi, $MSR_lo);
    hdtPerl::write_srb_register ("DR0", \$val_junk, \$val_dr0);
    $val_dr7 = $val_dr7 & 0xFFFFFFFD;
    hdtPerl::write_srb_register ("DR7", \$val_junk, \$val_dr7);
    hdtPerl::write_gpr_register ("rSP", \$val_junk, \$old_esp);
}


#-------------------------------------------------
# Private function
#-------------------------------------------------
sub read_mem_number {
    my $Address = $_[0];
    my $Width = $_[1];
    my $TempBuffer = hdtPerl::newba($Width);
    hdtPerl::read_mem_buffer(0, $Address, $Width, 0, 6, 0, $TempBuffer);
    my $TempNumber = 0;
    for(my $i=$Width-1; $i>=0; $i--){
        $TempNumber = ($TempNumber<<8) + hdtPerl::getba($TempBuffer, $i);
    }
    hdtPerl::delba($TempBuffer);
    return $TempNumber;
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

1;  # PACKAGE END