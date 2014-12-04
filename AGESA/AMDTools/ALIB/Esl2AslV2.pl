# Version 0.1

my $input_file = 0;
$numArgs = $#ARGV + 1;
my @array = "X";
my @def_name_array;
my @def_val_array;
my @files_array;
my $debug_info = 0;
my $inArg;

for ($inArg = 0; $inArg < ($#ARGV + 1); $inArg++) {
   if ($ARGV[$inArg] eq "-debug") {
      $debug_info = 1;
   } else {
      push (@files_array, $ARGV[$inArg])
   }
}

for ($input_file = 0; $input_file < (($#files_array + 1)); $input_file++) {
   printf $files_array[$input_file];
   open my $INFILE,'<', $files_array[$input_file] or die "Could not open file!";
   my $outfilename = $files_array[$input_file];
   $outfilename =~ s/esl/asl/gxms;
   printf $outfilename;
   open my $OUTFILE, '>', $outfilename or die "Could not open file!";
   @raw_data= <$INFILE>;

   foreach $line (@raw_data)
   {
     my $i;
     my $comments = "";
     #remove debug info
     if ($line =~ /(,)(\s+)Debug/ ) {
        if ($debug_info == 0) {
           $line = "// ".$line;
        }
     }

     if (debug_info == 0) {
     }
     #remove comment
     if ($line =~/\/\//) {
        $line = $`;
        $comments = "//".$';
     }
     #save definition
#     if ($line =~ /(\#define)(\s+)([A-Za-z0-9_]+)(\s+)(([A-Za-z0-9_]+))/) {
#       push (@def_name_array, $3);
#       push (@def_val_array, $5);
#       $line = "// ".$line;
#       #printf $1."- ".$2."-".$3."-".$4."-".$5."\n";
#       #printf $line." \n";
#     } else {
#        #substitute definitions
#        for ($i = 0; $i < ($#def_name_array + 1); $i++) {
#          $line =~ s/$def_name_array[$i]/$def_val_array[$i]/;
#        }
#     }
     #substitute proc and var identificators
     while ($line =~ /(var|proc|bf)([A-Za-z0-9_]+)/) {
       my $pvar = $1.$2;
       if ($2 =~ /(Local[0-7])/) {
         $replacment = $1;
       } else {
         $i = 0;
         while (($pvar ne $array[$i]) && ($i < ($#array + 1))) {
            $i++;
         }
         if ($i == ($#array + 1)) {
            push (@array, $pvar);
         }
         $replacment = sprintf ("A%03d", $i);
       }
       $line =~ s/$pvar/$replacment/;
     }
     printf {$OUTFILE} $line.$comments;
   }
   close $INFILE;
   close $OUTFILE;
}