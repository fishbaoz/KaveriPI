
if ($#ARGV != 1) {
  print "Usage: SamuB2c.pl SamuPatch.bin XX\n";
  print "Where XX defines the label SamuPatchXX[]";
  exit 1;
}

$Program = $ARGV[1];
my $OutFile = "GnbSamuPatch".$Program.".h";
open my $INFILE,'<', $ARGV[0] or die "Could not open input file file!";
open my $OUTFILE,'>', $OutFile or die "Could not open output file!";
my $filetag = uc($OutFile);
$filetag =~ s/\./_/gxms;

print {$OUTFILE} <<"XXXXXX";
/**
 * \@file
 *
 * SAMU firmware
 *
 *
 *
 * \@xrefitem bom "File Content Label" "Release Content"
 * \@e project:     AGESA
 * \@e sub-project: GNB
 * \@e \\\$Revision:\$   \@e \\\$Date:\$
 *
 */
/*
*****************************************************************************
*
* Copyright 2008 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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
* ***************************************************************************
*
*/

XXXXXX

my $header_def = "#ifndef _".$filetag."_\n"
                 ."#define _".$filetag."_\n\n"
                 ;

print {$OUTFILE} $header_def;

print {$OUTFILE} "UINT8 SamuPatch", $Program,"[] = {\n";
#@in_file_data= <$INFILE>;

#my $firststring = join(" ", @in_file_data);
#print ("$firststring\n");
#while ($firststring =~ m/([A-Fa-f0-9]+)/g) {
#     $line = " 0x".$1.",\n";
#     print ("$line\n");
#     print {$OUTFILE} $line;
#}
binmode $INFILE;
@xx=<$INFILE>;

my $slen = tell($INFILE);
printf "ROM Size:%d KB \n",$slen/1024;

  #Local variable
  my $buff1;
#  my $buff2;
#  my $biospos;
#  my $Smulen;
#  my $Nonfound;
  seek($INFILE, 0, 0 );
  read($INFILE, $buff1, $slen);
  my @BioaInArray = unpack ("C[$slen]",$buff1);
#  print @BioaInArray
my $numOfByte = 0;
foreach my $byteData (@BioaInArray) {
  #printf ("0x%x\n", $byteData);
  $numOfByte = $numOfByte + 1;
  printf {$OUTFILE} "  0x%x,\n", $byteData;

}

#printf "ROM Size:%d KB \n",$slen/1024;

print {$OUTFILE} "};\n\n";

print {$OUTFILE} "UINT32 SamuPatch", $Program, "Header", "[] = {\n";
my $samuPatchSig = "SAMU";
print unpack("H*", $samuPatchSig);
printf {$OUTFILE} "  0x".unpack("H*", $samuPatchSig).",\n";

printf {$OUTFILE} "  0x%x\n", $numOfByte;
print {$OUTFILE} "};\n#endif\n";
close ($OUTFILE);
close ($INFILE);