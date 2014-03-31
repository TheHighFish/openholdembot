# Expects a handrank.txt file without
# any empty lines or comments.
# Generates a large table for the lyx-docu.

use strict;
use warnings;

# print header of the table
print '\begin_layout Standard', "\n";
print '\begin_inset Tabular', "\n";
print '<lyxtabular version="3" rows="169" columns="10">', "\n";
print '<features>', "\n";
print '<column alignment="center" valignment="top" width="0">', "\n";
print '<column alignment="center" valignment="top" width="0">', "\n";
print '<column alignment="center" valignment="top" width="0">', "\n";
print '<column alignment="center" valignment="top" width="0">', "\n";
print '<column alignment="center" valignment="top" width="0">', "\n";
print '<column alignment="center" valignment="top" width="0">', "\n";
print '<column alignment="center" valignment="top" width="0">', "\n";
print '<column alignment="center" valignment="top" width="0">', "\n";
print '<column alignment="center" valignment="top" width="0">', "\n";
print '<column alignment="center" valignment="top" width="0">', "\n";

my $line_number = 0;
# for all lines in the file...
while (<>)
{
	# next row in the table
	print '<row>', "\n";
	# row number to the first cell...
	$line_number++;
	print '<cell alignment="center" valignment="top" topline="true" bottomline="true" leftline="true" usebox="none">', "\n";
	print '\begin_inset Text', "\n";
	print '\begin_layout Plain Layout', "\n";
	print $line_number, "\n"; 
	print '\end_layout', "\n";
	print '\end_inset', "\n";
	print '</cell>', "\n";
	# for all 9 opponents...
	my $input = $_;	
	for (my $i=0; $i<9; $i++)
	{
		# extract data set
		my $length = 12;	
		my $start = 6 + $i * $length;
		my $next_data = substr($input, $start, $length);
		# split data set...
		my $hand = substr($next_data, 0, 5);
		my $better_hands = substr($next_data, 6, 4);		
		#  create next cell of the table
		print '<cell alignment="center" valignment="top" topline="true" bottomline="true" leftline="true" usebox="none">', "\n";
		print '\begin_inset Text', "\n";
		print '\begin_layout Plain Layout', "\n";
		print "$hand $better_hands\n"; 
		print '\end_layout', "\n";
		print '\end_inset', "\n";
		print '</cell>', "\n";
	}
	# end of row
	print '</row>', "\n";
}
# end of table
print '</lyxtabular>', "\n";
print '\end_inset', "\n";
print '\end_layout', "\n";