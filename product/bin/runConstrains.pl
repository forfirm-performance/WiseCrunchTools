use strict;
use warnings;
use constrains('apply_constrains');

# Get input data from consolle
# Verify the number of inputs passed (must be 1, the file name)
# So, $#ARGV must be zero. Note: $#ARGV is the LAST INDEX of @ARGV, NOT the total number of items of @ARGV)
if($#ARGV!=1 ){
	print "\n";
	print "Wrong number of input arguments. Required: TestSet (valid file name), separator (as , or ; or...)!";
	print "\n";
  	exit(1);
}
	
# Get the filename.
my $TESTSET_FILE_NAME = $ARGV[0];

# Get the separator.
my $SEP = $ARGV[1];

# Verify that file does not exist
unless (-e $TESTSET_FILE_NAME) {
	print "\n";
	print "File " . $TESTSET_FILE_NAME . " does not exist!";
	print "\n";
	exit(1);
 } 

# Start apply constrains rules
open FILE, "+<", $TESTSET_FILE_NAME or die $! ;
while (my $line = <FILE>) {
	#Split row on ,
	my @r = split($SEP, $line);
	# Read single row values and apply filter (pass r by reference \@r. Look at http://stackoverflow.com/questions/15947997/perl-statisticspca-cant-use-string-0-as-an-array-ref-while-strict-ref)
	my $f=0;
	$f=&constrains::apply_constrains(\@r);
	# Print valid data to STDOUT and not valid nple to STDERR
	if($f==1){
		print STDERR $line;
	} else {
		print STDOUT $line;
	}

}
# Close TestSet file
close(FILE);

# Exit program
exit(0);
