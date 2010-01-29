# Small Perl-script, that does apply some fixes 
# to the auto-generated HTML and CSS files for the docu.
#
# Additionally it is able to configure the LyX-files
# for PDF-output or for HTML-outout.
# See the configuration-section

use strict;
use warnings;

use File::Find;

my $change_LyX_for_PDF_output = 0;
# Configuration
my $change_LyX_for_HTML_output = 0;


# main program
find(\&wanted, '.');
pause();

sub wanted 
{ 
	# Filename in lower cases
	print $_, "\n";
	my $filename = lc($_); 
	my $infile;
	my $outfile;
	# HTML-files (both .htm and .html)
	if (index($filename, '.htm') > 0)
	{
		open($infile, $filename);
		open($outfile, '>Temp.tmp');
		while (<$infile>)
		{
			# Remove crippled width and height of images
			s/width="\d+\.\d*pt"//g; 
			s/height="\d+\.\d*pt"//g;
			print $outfile $_;
			#print $_;
		}
		close($infile);
		close($outfile);
		unlink($filename);
		rename('Temp.tmp', $filename);
	}
	# Style-sheets
	elsif (index($filename, ".css") > 0)
	{
	}
	# Lyx-files
	elsif (index($filename, ".lyx") > 0)
	{
		open($infile, $filename);
		open($outfile, '>Temp.tmp');
		while (<$infile>)
		{
			# Default numbering
			#\secnumdepth 3
			#\tocdepth 3
			# Numbering completely disabled
			#\secnumdepth -2
			#\tocdepth -2
			s/\\secnumdepth.*/\\secnumdepth -2/g; 
			s/\\tocdepth.*/\\tocdepth -2/g;
			print $outfile $_;
			#print $_;
		}
		close($infile);
		close($outfile);
		unlink($filename);
		rename('Temp.tmp', $filename);
	}
}

