#   Download page:         http://code.google.com/p/openholdembot/
#   Forums:                http://www.maxinmontreal.com/forums/index.php
#   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html

# Simple script that translates the OpenPPL-library to (nearly) C++
# for the new OpenPPL symbol-engines.
use strict;
my $outside_function   = 0;
my $start_of_function  = 1;
my $inside_function    = 2;
my $parsing_state      = $outside_function;
my $current_line;

while (<>) {
	chomp;
	if (m/^##/) {
		function_header();
		next;
	}
	# Any alpha-numerical character or (even empty) comments (slash)
	elsif (m/[a-zA-Z0-9\/]/) {
		# Remove spaces at the beginning
		s/^[ \t]*//;
		# Remove OpenPPL-Prefix
		s/f\$OpenPPL_//ig;
		# Replace [ by (
		s/\[/\(/g;
		# Replace ] by )
		s/\]/\)/g;
		# Print comments as is
		if (m/\/\//) {
			comment();
			next;
		}	
		# Append brackets to expected function calls
		s/([a-zA-z][a-zA-Z0-9_\$]*)/$1\(\)/ig;
		# Replace bit-count-operator by function-calls
		s/`/bit_count/g;
		if ($parsing_state == $start_of_function) {
			start_of_code();
			next;
		}
		elsif ($parsing_state == $inside_function) {
			code();
			next;		
		}
		else {
			# This must not happen
			# Comments and code handled
			print "ERROR!";
			exit;
		}
	}
	else {
		empty_space();
		next;
	}
}

sub function_header {		
	$parsing_state = $start_of_function;
	s/##//g;
	s/f\$/double Calculate/;
	print;
	print "()    // Please revisit the return type. int or bool might be more appropriate\n";
	print "{\n";
	print "    return (";
}

sub comment {
	if ($parsing_state == $inside_function)
	{
		# Indent
		print "\n";
		print "        ";
		print;
	}
	elsif ($parsing_state == $start_of_function) 
	{
		# Do not indent
		# No line break
		$parsing_state = $inside_function;
		print;
	}
	else {
		# Print as is
		print "\n";
		print;
	}
}

sub start_of_code {
	# No line-break directly after "return("
	$parsing_state = $inside_function;
	# Print content without line-break and indentation
	print;
}

sub code {
	# Print content with indentation
	print "\n";
	print "        ";
	print;
}

sub empty_space {
	# Empty line
	# End of function
	if (($parsing_state == $start_of_function) || ($parsing_state == $inside_function))
	{
		$parsing_state = $outside_function;
		# Finish return statement		
		print ");\n}\n\n";
		return;
	}
	# Another empty line or comment, just print as is
	print "\n";
	print;
}