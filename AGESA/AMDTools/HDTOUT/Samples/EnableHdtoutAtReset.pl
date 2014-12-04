# Version 1.4.0
# Date: 04/24/10

### DEFAULT SETTINGS #############################
my $emulator_num = 0;
my $ip_addr = "";
my $default_password = "hdtout_pwd";
my $default_username = "hdtout_usr";
##################################################

use constant HDT_PATH => 'C:\Progra~1\AMD\HDT';
use constant HDT_PATH64 => 'C:\Progra~1\AMD\HDT';

use lib HDT_PATH;
use lib HDT_PATH64;
use hdtPerl;
use strict;
use Time::HiRes qw(usleep);

main();

sub main {
    my $val_junk = 0;
    my $val_dr2 = 0;
    my $status = 0;
    my $timeout = 100;

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
    }

    hdtPerl::enter_debug();
    hdtPerl::enable_assert_dbreq_on_cpu_reset(0);
    hdtPerl::set_target_node(0) or die;
    hdtPerl::set_logc_core(0) or die;
    $status = hdtPerl::read_srb_register ("DR2", \$val_junk, \$val_dr2);
    while ($status == 0) {
        hdtPerl::exit_debug(0);
        poll_dbrdy();
        $status = hdtPerl::read_srb_register ("DR2", \$val_junk, \$val_dr2);
        $timeout--;
        if($timeout==0){
            die "ERROR: Registers are inaccessible!\n";
        }
    }
    if($val_dr2 != 0x99CC){
        # If HDTOUT has not been enabled, enable it
        hdtPerl::enable_assert_dbreq_on_cpu_reset(1);
        warm_reset();
        print ("Trigger Reset\n");
        poll_dbrdy();
        print ("Stop at First Reset\n");
        hdtPerl::execute_hdt_misc_command(5,0);
        usleep(100000);
        $val_junk = 0;
        $val_dr2 = 0x99CC;
        hdtPerl::write_srb_register ("DR2", \$val_junk, \$val_dr2) or die;
        hdtPerl::execute_hdt_misc_command(5,0);
        usleep(100000);
        print ("Enable HDTOUT\n");

        # Check if another reset exist
        my $val_edx = 0;
        do {
            hdtPerl::exit_debug(0);
            poll_dbrdy();
            hdtPerl::read_srb_register ("DR2", \$val_junk, \$val_dr2) or die;
            if($val_dr2!=0x99cc){
                print ("Stop at another reset\n");
                hdtPerl::execute_hdt_misc_command(5,0);
                usleep(100000);
                $val_junk = 0;
                $val_dr2 = 0x99CC;
                hdtPerl::write_srb_register ("DR2", \$val_junk, \$val_dr2) or die;
                hdtPerl::execute_hdt_misc_command(5,0);
                usleep(100000);
                print ("Re-enable HDTOUT\n");
            }elsif(hdtPerl::read_gpr_register ("rDX", \$val_junk, \$val_edx)==0){
                $val_edx = 0;
            }
        } while (($val_edx & 0xFFFF) != 0x99CC);

        print ("HDTOUT is on.\n");
    } else {
        print ("HDTOUT has been enabled.\n");
    }

    print ("\n*** Please run your script ***\n");
    hdtPerl::enable_assert_dbreq_on_cpu_reset(0);
}

sub poll_dbrdy {
    my $dbrdy_value = 0;
    do {
        usleep 1000;
        hdtPerl::dbrdy_status(0, \$dbrdy_value);
    } while ($dbrdy_value == 0);
    usleep 1000;

    hdtPerl::enter_debug();
}

sub warm_reset {
    hdtPerl::write_io(0,0xCF9,8,6);
    hdtPerl::exit_debug(0);
}
