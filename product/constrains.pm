 #!/usr/bin/perl
package constrains;
use strict;
use warnings;

sub apply_constrains {
    # Read the input line - DO NOT TOUCH IT !
    my ($r) = @_;

    ###############################################################
    #                                                             #
    # PUT ALL YOUR CONSTRAINS HERE BASED ON THE FOLLOWING MODEL   #
    #                                                             #
    #  if($r->[0] eq "??" && $r->[1] eq "??" && $r->[2] ne "??"){ #
    #	  return 1;                                               #
    #  };                                                         #
    #                                                             #
    # Always return 1 for NON VALID nple.                         #
    # Always return 0 for VALID nple.                             #
    # $r->[0] means field A                                       #
    # $r->[1] means field B                                       #
    # $r->[2] means field C and so on...                          #
    ###############################################################
	# Example
	if($r->[0] eq "A1" && $r->[1] eq "B3"){ 
    	  return 1;                                               
    }; 



    ################### END OF USER CONSTRAINS' CODE ##############

    # DO NOT TOUCH IT !
    return 0;

}
1;
