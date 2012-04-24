#!/usr/bin/perl
#
# Edgar Berdahl, Jan 21 2012
# Updated February 2012
#
# First argument is input filename for MDL file
# Second argument is output filename for DSP file
#
# For example: perl5.10.0 Synth-A-Modeler.plx examplelinks.mdl examplelinks.dsp
#
# ALL NUMBERS IN MDL FILES SHOULD BE FLOATING-POINT NUMBERS (NOT INTEGERS)

use v5.10;

use warnings;
$filename = $ARGV[0];      # e.g. "examplelinks.mdl";
$filenameout = $ARGV[1];   # e.g. "examplelinks.dsp";
my $indx = 0;              # index variable used over and over again






# First define the regular expressions that will be used for parsing the MDL file
my $mylabel = qr/([a-zA-Z\d]+)/;       # for a single label
my $mylabels = qr/([a-zA-Z,\d\s]+)/;   # for comma-separated labels with whitespaces

my $myval = qr/([-\+Ee\.\d]+)/;              # A "floating-point" number can contain the characters +, -, e, E, ., and any digit and in any order
my $myParams = qr/(\s*[-\+\.eE,\s\d]+\s*)/;  # To represent comma-represented lists of numbers  ... this was previously my $myParams = qr/([-\d\.eE,\s]+)?/;   

my $link = qr/(link|pluck|touch)/;
my $linkline = qr/\A\s*${link}\(\s*${myParams}\s*\)\s*,\s*${mylabel}\s*,\s*${mylabel}\s*,\s*${mylabel}\s*,\s*\(\s*${mylabels}\s*\)\s*;\s*$/;

my $vertex = qr/(mass|port|ground|resonator)/;
my $vertexline = qr/\A\s*${vertex}\(\s*${myParams}\s*\)\s*,\s*${mylabel}\s*,\s*\(\s*${mylabels}\s*\)\s*;\s*$/;

my $audiooutDetails = qr/(.+)/;
my $audiooutline = qr/\A\s*audioout\s*,\s*${mylabel}${audiooutDetails};\s*$/;
my $moreAudioOutSums = qr/(.*)/;
my $audiooutParseSum = qr/\A\s*,\s*${mylabel}\s*,\s*${myval}\s*${moreAudioOutSums}/;

my $rtadjustIndex = qr/([\d]+)/;
my $rtadjustline = qr/\A\s*rtadjust\s*,\s*${mylabel}\s*,\s*${mylabel}\s*:\s*${rtadjustIndex}\s*;\s*$/;






# Usage: $elementParams = replace_param( $elementParams, $n, $newParam );
#
# replace_param receives a comma-separated list of parameters (which can be either labels
# or numerical values), and it replaces the $nth item in the list with $newParam, which
# can either be a label or a numerical value.
my $myLabelOrValue = qr/([-\+\.a-zA-Z\d]+)/;
my $myFullLabelOrValue = qr/(\A\s*[-\+\.a-zA-Z\d]+\s*$)/;
my $myOptionalListOfLabelsOrValues = qr/([-\+\.,a-zA-Z\d]*)/;
my $separateParams = qr/${myLabelOrValue}\s*,\s*${myOptionalListOfLabelsOrValues}\s*/;
sub replace_param
{
    $listOfParams = $_[0];
    $indexToReplace = $_[1];
    $insertionValue = $_[2];
    #say "BOO" . $listOfParams . ":" . $indexToReplace . ":" . $insertionValue;

    my $inFrontOf = "";
    my $current = "";
    my $behind = $listOfParams;

    do {
	if ($inFrontOf eq "") { $inFrontOf = $current;
	} else { $inFrontOf = $inFrontOf . "," . $current; }

	if ($behind =~ qr/$myFullLabelOrValue/) {    # only one element left
	    #say "FULLLABELORVALUE: " . $1;
	    $current = $1;
	    $behind = "";

	} else {
	    if ($behind =~ qr/$separateParams/) {
		#say "SEPARATING: " . $1 . ", 2222: " . $2;
		$current = $1;
		$behind = $2;
	    } else {
		die;
	    }
	}
	
	$indexToReplace--;
    #say "INFRONTOF: " . $inFrontOf . "    BEHIND: " . $behind;
    } while ($indexToReplace > 0);


    if (($inFrontOf eq "") && ($behind eq "")) { return $insertionValue; } 
    if ($inFrontOf eq "") { return $insertionValue . "," . $behind; }
    if ($behind eq "") { return $inFrontOf . "," . $insertionValue; }
    return $inFrontOf . "," . $insertionValue . "," . $behind;
}







# Then read through the MDL file to get the number of link-like objects and mass-like elements
# and construct the vector of inputs and outputs to the main function
my $vertexNum = 0;
my $linkNum = 0;
my $portNum = 0;
my $inputs = "";
my $outputs = "";
my $audioOutputs = "";
my $audioOutNum = 0;
my $rtAdjustInputs = "";
my $rtAdjustNum = 0;
open $filehandle, '<', $filename
    or die "Cannot read '$filename': $!\n";
while (<$filehandle>)
{
    chomp;
    $theline = $_;
    
    if ($theline =~ qr/$linkline/) { 
	say "Read this link:  " . $1 . " " . $2 . " " . $3 . " " . $4 . " " . $5 . " " . $6;
	$linkNum++;
    }
    if ($theline =~ qr/$vertexline/) {
	say "Read this vertex:  " . $1 . " " . $2 . " " . $3 . " " . $4;
	$vertexNum++;
	if ($vertexNum > 1) {
	    $inputs = $inputs . ",";
	    $outputs = $outputs . ",";
	}
	if ($1 eq "port") {    # if it is a port in particular
	    $portNum++;
	    $inputs = $inputs . $3 . "p";
	    $outputs = $outputs . $3;
	} else {
	    $inputs = $inputs . $3 . "p";
	    $outputs = $outputs . $3;
	}
    }    
    if ($theline =~ qr/$audiooutline/) {
	$audioOutNum++;
	$audioOutputs = $audioOutputs . "," . $1;
#	say "Read AUDIOOUT: " . $theline;
    }
    if ($theline =~ qr/$rtadjustline/) {
	$rtAdjustNum++;
	$rtAdjustInputs = $rtAdjustInputs . "," . $1;
    }
}
$outputs = $outputs . $audioOutputs;
$inputs = $inputs . $rtAdjustInputs;

close $filehandle;
my $feedbackNum = $vertexNum - $portNum;
my $theline = "There are " . $vertexNum . " vertices and " . $linkNum . " link(s) and " . $portNum . " port(s) and " . $feedbackNum . " non-port mass-like objects.\n";
say $theline;









open my $fh_out, '>', $filenameout
    or die "Cannot write '$filenameout': $!\n";
say $fh_out "// This DSP file has been generated by the Synth-A-Modeler compiler.";
say $fh_out "import(\"physicalmodeling.lib\");\n";
my $functionDef = "bigBlock(" . $inputs . ") = (" . $outputs . ") with {";
say $fh_out $functionDef;









# We parse the link-like objects and format them such that they can be inserted into the output .DSP file
open $filehandle, '<', $filename
    or die "Cannot read '$filename': $!\n";
my $linkLine;
while (<$filehandle>)
{
    chomp;
    $theline = $_;
    
    if ($theline =~ qr/$linkline/) {    # if we find a line specifying a link
	my $o1 = $1;  my $o2 = $2;  my $o3 = $3;  my $o4 = $4;  my $o5 = $5;  my $o6 = $6;    # save captures

	# Change output line only if we find an rtadjust specifying to do so
	# Pass through the rtadjust's to see if any of the Faust object's parameters should be real-time adjustable
	open my $fh_inner, '<', $filename
	    or die "Cannot read '$filename': $!\n";
	while (<$fh_inner>) {
	    chomp;
	    $theline_inner = $_;
	    
	    if ($theline_inner =~ qr/$rtadjustline/) {
		my $i1 = $1;  my $i2 = $2;  my $i3 = $3;
		#say "Does " . $2 . " match " . $o6 . "?";
		#say "LINK RTadjust 1: " . $1 . ", 2: " . $2 . ", 3: " . $3;
                # Check if the rtadjust's label is a subset of the current list of labels
		if ($o6 =~ qr/(\s?$2\s?|\s?$2\s?,.*|.*,\s?$2\s?|.*,\s?$2\s?,.*)/) {
		    #say "YES!";
		    #$o2 = $o2 . "," . $i3 . "->" . $i1;
		    $o2 = replace_param( $o2, $i3, $i1 );
		    say "RETURN VALUE: " . $o2;
		}
	    }
	}
	close $fh_inner;

	# Set default output line
	$linkLine = "\t" . $o3 . " = (" . $o4 . "p - " . $o5 . "p) : " . $o1 . "(" . $o2 . ");";
	say $fh_out $linkLine;

	$numBasicLinks++;
    }
}
say $fh_out "";
close $filehandle;






# Then for each mass-like object
#  - provide a link between its index to xN, xNo (where N is an integer) and its label
open $filehandle, '<', $filename
    or die "Cannot read '$filename': $!\n";
my $vertexLine;
my $vertexInd = 0;
my $portInd = 0;
my $massLabel;
while (<$filehandle>)
{
    chomp;
    $theline = $_;

    if ($theline =~ qr/$vertexline/) {
#	print "Read a vertex: $theline\n";
	$typeOfMass = $1;
	$massName = $3;

	if ($1 eq "port") {    # if it is an external mass
	    $vertexLine = "\t" . $3 . " = (0.0";
	} else {                  # if it is a mass to be simulated 
	    $vertexLine = "\t" . $3 . " = (0.0";
	}

	open $fh_inner, '<', $filename
	    or die "Cannot read '$filename': $!\n";
	while (<$fh_inner>) {
	    chomp;
	    $theline_inner = $_;
	    
	    if ($theline_inner =~ qr/$linkline/) {
		if ($4 eq $massName) {
		    $vertexLine = $vertexLine . "-" . $3;
		}
		if ($5 eq $massName) {
		    $vertexLine = $vertexLine . "+" . $3;
		}
	    }
	}
	close $fh_inner;

	my $elementParams = $2;  # save captured Faust object parameters for element
	my $listOfLabels = $4;   # save list of labels for element

	# Change output line only if we find an rtadjust specifying to do so
	# Pass through the rtadjust's to see if any of the Faust object's parameters should be real-time adjustable
	open $fh_inner, '<', $filename
	    or die "Cannot read '$filename': $!\n";
	while (<$fh_inner>) {
	    chomp;
	    $theline_inner = $_;
	    
	    if ($theline_inner =~ qr/$rtadjustline/) {
		my $i1 = $1;  my $i2 = $2;  my $i3 = $3;   # Save rtadjust regex
		#say "Does " . $2 . " match " . $listOfLabels . "?";
		#say "MASS RTadjust 1: " . $1 . ", 2: " . $2 . ", 3: " . $3;
                # Check if the rtadjust's label is a subset of the current list of labels
		if ($listOfLabels =~ qr/(\s?$2\s?|\s?$2\s?,.*|.*,\s?$2\s?|.*,\s?$2\s?,.*)/) {
		    #say "YES!";
		    #$elementParams = $elementParams . "," . $i3 . "->" . $i1;
		    $elementParams = replace_param( $elementParams, $i3, $i1 );
		    #say "RETURN VALUE: " . $elementParams;
		}
	    }
	}
	close $fh_inner;

	if ($1 eq "port") {      # if it is an external port
	    $vertexLine = $vertexLine . ");";
	} else {                 # if it is a mass to be simulated 
	    $vertexLine = $vertexLine . ") : " . $typeOfMass . "(" . $elementParams . ");";
	}
	say $fh_out $vertexLine;
    }
}
close $filehandle;
say $fh_out "";





# Handle any additional specified audio outputs
open my $filehandle, '<', $filename
    or die "Cannot read '$filename': $!\n";
my $audiooutEqn;
my $audioParams;
while (<$filehandle>)
{
    chomp;
    $theline = $_;

    if ($theline =~ qr/$audiooutline/) {
	$audiooutEqn = "\t" . $1 . " = 0.0";
	say "Read AUDIOOUT: " . $theline;
	say "Read for: " . $1 . " and " . $2;
	$audioParams = $2;
	say "AUDIOPARAMS: " . $audioParams;
	while ($audioParams =~ qr/$audiooutParseSum/) {
	    say "We have 1: " . $1 . ", 2: " . $2 . ", 3: " . $3;
	    $audiooutEqn = $audiooutEqn . "+" . $1 . "*(" . $2 . ")";
	    $audioParams = $3;
	    #say "Now parsing: \"" . $audioParams . "\"";
	}
	$audiooutEqn = $audiooutEqn . ";";
	say $fh_out $audiooutEqn;
    }
}
close $filehandle;
say $fh_out "};\n";






# Finally we generate text for selecting the appropriate feedbacks
# and selecting the outputs (opposite of the feedbacks)
my $feedbackSelector = "";
my $outputsSelector = "";
for $indx (1 .. ($feedbackNum)) {
    $feedbackSelector = $feedbackSelector . "_";
    $outputsSelector = $outputsSelector . "!";
    if ($indx < $feedbackNum) {
	$feedbackSelector = $feedbackSelector . ",";
	$outputsSelector = $outputsSelector . ",";
    }
}
for $indx (1 .. ($portNum+$audioOutNum)) {
    $outputsSelector = $outputsSelector . ",_";
}

say $fh_out "process = (bigBlock)~(" . $feedbackSelector . "):(" . $outputsSelector . ");";
close $fh_out;
