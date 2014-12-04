#
# This file provides a basic example of how to use FuncList module
#

use constant HDT_PATH => 'C:\Progra~1\AMD\HDT';
use constant HDT_PATH64 => 'C:\Progra~1\AMD\HDT';
use constant WRITE => 0;
use constant READ => 1;
use constant FLUSH => 2;

use lib HDT_PATH;
use lib HDT_PATH64;
use hdtPerl;
use strict;
use Time::HiRes qw(usleep);
use StringBP;

use FuncList;

main();
sub main {
    # initialize HDT
    my $emulator_num = 0;
    hdtPerl::list_targets();
    #print("Which Possum Index would you like to use? ");
    #chomp($emulator_num = <>);
    print ("Connecting to HDT device $emulator_num...\n");
    hdtPerl::select_emulator_by_index($emulator_num);
    print ("Connected to HDT device $emulator_num.\n");

    hdtPerl::enter_debug();

    # Stop after training complete
    my $BreakpointList = [
        ["End MaxRdLat training","!Node 0"]
    ];
    my $BP = StringBP->new();
    $BP->set_bp($BreakpointList);
    my $bpnum = $BP->trap();
    if($bpnum > 0){
        #print "\n\nBreak at BP$bpnum: \"".$BP->read_bp_string()."\"!!!\n\n";
        print "\nDone training.\n\n";
    }else{
        die;
    }

    # Create FuncList object
    my $FL = FuncList->new();

    # Get a list of supported functions, this step needs to be done before any function call
    my $func_list = $FL->get_list();
    if(scalar(@$func_list) > 0){
        print "\nFunction list found! Supported functions:\n";
        foreach my $func_name (@$func_list) {
            print "    $func_name\n";
        }

        my $PhyAddrHi = 0x00000000;
        my $PhyAddrLo = 0x0DB00000;
        my @CompressedPattern = (0x44, 0xA6, 0x38, 0x4F, 0x4B, 0x2E, 0xEF, 0xD5, 0x54);
        my @UncompressedPattern;
        my $BufferAddr = 0;
        my $ClCnt = 9;

        # Allocate buffer for training pattern
        print "\nAllocating buffer...\n";
        $BufferAddr = $FL->allocate_buffer($ClCnt);
        if($BufferAddr != 0){
            # Fill buffer with pattern
            print "\nFilling pattern to buffer...\n";
            my $Pattern = hdtPerl::newba($ClCnt*64);
            for (my $k = 0; $k < $ClCnt*64; $k++) {
                my $i = $k >> 6;
                my $Mask = 0x80 >> (($k >> 3) & 7);

                my $Value = 0;
                if ($i != 0) {
                  $Value = 1 << ($i - 1);
                }

                if ($CompressedPattern[$i] & $Mask) {
                  $Value = ~$Value;
                }
                $UncompressedPattern[$k] = $Value & 0xFF;
                hdtPerl::setba($Pattern,$k,$Value & 0xFF);
            }
            hdtPerl::write_mem_buffer(0, $BufferAddr, $ClCnt*64, 0, 6, 0, $Pattern);
            hdtPerl::delba($Pattern);

            # Set FS base
            $FL->set_phy_addr_hi($PhyAddrHi);

            # Call Write pattern function
            print "\nWriting pattern to DRAM...\n";
            $FL->call($func_list->[WRITE],$PhyAddrLo,$BufferAddr,$ClCnt);

            # Call Read pattern function
            print "\nReading pattern back from DRAM...\n";
            $FL->call($func_list->[READ],$BufferAddr,$PhyAddrLo,$ClCnt);

            # Compare read back data to pattern
            my $Pass = 0xFF;
            my $Buffer = hdtPerl::newba($ClCnt*64);
            hdtPerl::read_mem_buffer(0, $BufferAddr, $ClCnt*64, 0, 6, 0, $Buffer);
            for (my $k = 0; $k < $ClCnt*64; $k++) {
                if (hdtPerl::getba($Buffer,$k) != $UncompressedPattern[$k]) {
                  # if bytelane n fails, clear bit n
                  $Pass &= ~(1 << ($k & 0x07));
                }
            }
            hdtPerl::delba($Buffer);
            printf("\nCompare result bitmap: %02X\n",$Pass);

            # Call Flush pattern function
            print "\nFlushing pattern from cache...\n";
            $FL->call($func_list->[FLUSH],$PhyAddrLo,$ClCnt);

            # Restore FS base
            $FL->restore_phy_addr_hi();

            # Deallocate buffer after use
            $BufferAddr = $FL->deallocate_buffer();
        }else{
            print "\nERROR: Could not allocate buffer for training pattern!!!\n";
        }
    }else{
        print "\nERROR: No functions found!!!\n";
    }
}