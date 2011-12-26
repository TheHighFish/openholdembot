################################################################################
#
# bracketify.pl
#
# A heuristic tool to add bracktes to (Open)PPL profiles
#
# Released as part of the OpenHoldem package under GPL v3.
# http://www.gnu.org/copyleft/gpl.html
#
# Forum:    http://www.maxinmontreal.com/forums/index.php 
# Download: http://code.google.com/p/openholdembot/
#
################################################################################
#
# Prerequisite: Perl installed
#   * http://www.activestate.com/activeperl/downloads
#
# Usage: 
#   * at the command prompt type:
#     "perl bracketify.pl < INPUTFILE.TXT > OUTPUTFILE.TXT"
#   * for testing purposes type only
#     "perl bracketify.pl"
#     and enter some PPL-code manually, Ctrl + C to quit.
#
################################################################################
#
# Language elements that need extra brackets
#   * When conditions
#   * Hand expressions
#   * Board expressions
#   * Not expressions with equality operators
#
################################################################################
#
# Things to know (for programmers, not for end-users)
#   * http://www.troubleshooters.com/codecorn/littperl/perlreg.htm
#
################################################################################

# While there is input: process it line by line
while (<>)
{
	# First replace tabs by 4 spaces to make life more easy
	#
	# Substitution is case-insensitive ("i" at the end), 
	# but we care about beautyful, readable output and "fix" the cases.
	s/\t/    /;
	# Then add a bracket after when-conditions.
	# Be careful: we alwaxs need an extra-bracket, even if one exists,
	# because we always add a closing bracket later.
	s/when /WHEN \(/i;
	s/when\(/WHEN \(\(/i;
	# But the funny thing is the right end of the expression
	# As the end is hard to detect with stupid rehular expressions we...
	# * add brackets to the left of any action
	# * add brackets to the left of any user-defined variable to be set
	# * add brackets to the end of a line, if there is no action
	s/ betmax /\) BETMAX /i;
	s/ raisemax /\) RAISEMAX /i;
	s/ allin /\) ALLIN /i;
	s/ betpot /\) BETPOT /i;
	s/ raisepot /\) RAISEPOT /i;
	s/ bethalfpot /\) BETHALFPOT /i;
	s/ raisehalfpot /\) RAISEHALFPOT /i;
	s/ bet /\) BET /i;
	s/ raise /\) RAISE /i;
	s/ call /\) CALL /i;
	s/ check /\) CHECK /i;
	s/ fold /\) FOLD /i;
	s/ sitout /\) SITOUT /i;
	# Second case: user defined variable to be set

	# Third case case: there is no action at the end,
	# i.e. a keyword "WHEN", but no "FORCE".
	# Then we add a bracket to the right end of the line
	# and hope, that it is no multi-line condition
	if ((m/^WHEN / || m/ WHEN /) && !(m/ FORCE / || m/ FORCE$/))
	{
		s/$/\)/i;	
	}
	# Add opening brackets to hand-expressions
	# There can be four cases, depending on spaces, brackets and equality operator
	# directly on the left and right.
	s/ hand / \(Hand /i;
	s/ hand=/\(Hand =/i;
	s/\(hand /\(Hand /i;
	s/\(hand=/\(Hand =/i;
	# Add closing brackets to hand expressions
	# This is probably a bit more difficult, 
	# because hand expressions can be spelled in different ways:
	#   * Hand = A K
	#   * Hand = AK
	#   * Hand = A K Suited
	#   * Hand = AK Suietd
	#   * Hand = Azuited
	# Maybe the most easy way is:
	#   * add brackets after the ranks (here "A K")
	#   * add brackets after "SUITED"
	#   * remove duplicate bracket before "SUITED"
	
	# Add opening brackets to board-expressions
	
	# Add closing brackets to board-expressions
	
	# Add opening brackets to not-expressions
	
	# Add closing brackets to not-expressions
	
	# Finally beautify some keywords
	# Standalone words only. with spaces or brackets on both sides (except "FORCE").
	s/ and / AND /i;
	s/\)and /\)AND /i;
	s/ and\(/ AND\(/i;
	s/\)and\(/\)AND\(/i;	
	
	# To do: other keywords like or, not, xor, ...
	
	s/ force/ FORCE/i;	
	# And finally write the processed line to standard output
	print;
}
