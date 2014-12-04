#
# This file provides a basic example of how to use StringBP module
#

use constant HDT_PATH => 'C:\Progra~1\AMD\HDT';
use constant HDT_PATH64 => 'C:\Progra~1\AMD\HDT';

use lib HDT_PATH;
use lib HDT_PATH64;
use hdtPerl;
use strict;
use Time::HiRes qw(usleep);
use StringBP;


##########################################################################
#   BREAKPOINT LIST                                                      #
##########################################################################
#   A breakpoint is a string that HDTOUT stops when its current output string matches the BP string.
#   To match the previous HDTOUT strings, prefix the BP string with '!'.

my $DftBreakpointList = [
    ["MR1", "!Dct 1", "!Node 0"], # BP1: Break on sending MRS 1 of DCT 1, node 0
    ["RC2", "!Node 1"]            # BP2: Break on sending control word 2 for any Dcts of Node 1
];

my $BreakpointList = [];
##########################################################################

main();
sub main {
    #user can use /S to skip the below input request
    my $skip_input = 0;
    $skip_input = 1 if(uc($ARGV[0]) eq '/S');
    #get input from user
    if(!$skip_input) {
      print "Input number of breakpoint:\n";
      my $NumBp = <STDIN>;
      for (my $i = 0; $i < $NumBp; $i++) {
        printf "Please input breakpoint[%d],split with \"&&\",ex:\"BPstr0 && BPstr1 && BPstr2\"\n",$i;
        my @tmparray =();
        #split the input, and delete the space in the being & end
        foreach (split /&&/,<STDIN>) {
          chomp;
          s/^\s*(.*?)\s*$/$1/;
          push @tmparray,$_;
        }
        push @$BreakpointList,[@tmparray];
      }
    }

    #if no input, get from the $DftBreakpointList
    $BreakpointList = $DftBreakpointList if($skip_input == 1);
    # initialize HDT
    my $emulator_num = 0;
    hdtPerl::list_targets();
    print("Which Possum Index would you like to use? ");
    chomp($emulator_num = <STDIN>);
    print ("Connecting to HDT device $emulator_num...\n");
    hdtPerl::select_emulator_by_index($emulator_num);
    print ("Connected to HDT device $emulator_num.\n");

    hdtPerl::enter_debug();

    # Create breakpoint object
    my $BP = StringBP->new();

    # Pass breakpoint list to breakpoint object
    $BP->set_bp($BreakpointList);

    # Trap on breakpoint
    my $bpnum = $BP->trap();
    if($bpnum > 0){
        print "\n\nBreak on BP$bpnum: \"".$BP->read_bp_string()."\"!!!\n\n";
        print "Status:\n".$BP->read_status_string()."\n\n";
    }else{
        print "No breakpoints hit !!!";
    }
}
