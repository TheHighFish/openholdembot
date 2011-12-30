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
#   * take a good editor with syntax high-lighting like NotePad++
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
	# Fist case: brackets before an action
	# We have to use more than the action keyword here,
	# otherwise code like "BotsLastAction = Raise) would be bracketed, too.	
	s/ betmax force/\) BETMAX FORCE/i;
	s/ raisemax force/\) RAISEMAX FORCE/i;
	s/ allin force/\) ALLIN FORCE/i;
	s/ betpot force/\) BETPOT FORCE/i;
	s/ raisepot force/\) RAISEPOT FORCE/i;
	s/ bethalfpot force/\) BETHALFPOT FORCE/i;
	s/ raisehalfpot force/\) RAISEHALFPOT FORCE/i;
	s/ betmin force/\) BETMIN FORCE/i;
	s/ raisemin force/\) RAISEMIN FORCE/i;
	s/ bet force/\) BET FORCE/i;
	s/ raise force/\) RAISE FORCE/i;
	s/ call force/\) CALL FORCE/i;
	s/ check force/\) CHECK FORCE/i;
	s/ fold force/\) FOLD FORCE/i;
	s/ sitout force/\) SITOUT FORCE/i;
	# Still first case: brackets before an action
	# Now for Bet/Raise X% FORCE or Bet/Raise X Force,
	# where Bet/Raise will be followed by a number.
	# For OpenPPL arbitrary expressions are possible here, but..
	# * arbitrary expressions would be too complicated for stupid regular expressions
	# * this tool is meant for Standard-PPL to be translated, where only numbers are possible
	# so we check for constant numbers only.
	# But first remove superfluous spaces (so that we don't miss anything later)...
	s/ bet    / BET /i;
	s/ bet   / BET /i;
	s/ bet  / BET /i;
	s/ raise    / RAISE /i;
	s/ raise   / RAISE /i;
	s/ raise  / RAISE /i;
	# ...and then add brackets
	s/ bet 0/\) BET 0/i;	
	s/ bet 1/\) BET 1/i;
	s/ bet 2/\) BET 2/i;
	s/ bet 3/\) BET 3/i;
	s/ bet 4/\) BET 4/i;
	s/ bet 5/\) BET 5/i;
	s/ bet 6/\) BET 6/i;
	s/ bet 7/\) BET 7/i;
	s/ bet 8/\) BET 8/i;
	s/ bet 9/\) BET 9/i;
	s/ raise 0/\) RAISE 0/i;
	s/ raise 1/\) RAISE 1/i;
	s/ raise 2/\) RAISE 2/i;
	s/ raise 3/\) RAISE 3/i;
	s/ raise 4/\) RAISE 4/i;
	s/ raise 5/\) RAISE 5/i;
	s/ raise 6/\) RAISE 6/i;
	s/ raise 7/\) RAISE 7/i;
	s/ raise 8/\) RAISE 8/i;
	s/ raise 9/\) RAISE 9/i;
	# Second case: user defined variable to be set,
	# where the line ends with a user-variable.
	# But first we remove superfluos spaces at the end to make things more robust...
	s/     $//;
	s/    $//;
	s/   $//;
	s/  $//;
	s/ $//;
	#... and then add a bracket before user-variables.
	s/((user[A-Za-z_]*)$)/\) $1/i;
	# Third case case: there is no action at the end,
	# i.e. a keyword "WHEN", but no "FORCE" and no user-variable.
	# Then we add a bracket to the right end of the line
	# and hope, that it is no multi-line condition
	if ((m/^WHEN /i || m/ WHEN /i) && !(m/ FORCE$/i) && !(m/(user[A-Za-z_]*)$/))
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
	# This requires a bit more complex regular expressions, 
	# because hand expressions can be spelled in different ways:
	#   * Hand = A K
	#   * Hand = AK
	#   * Hand = A K Suited
	#   * Hand = AK Suietd
	#   * Hand = Azuited
	s/([ (]+hand = [2-9TJQKA ]+(suited)?)/\($1\)/i;
	# Add opening brackets to board-expressions
	s/ board / \(Board /i;
	s/ board=/\(Board =/i;
	s/\(board /\(Board /i;
	s/\(board=/\(Board =/i;
	# Add closing brackets to board-expressions
	# Similar to hand expressions
	s/([ (]+board = [2-9TJQKA ]+(suited)?)/\($1\)/i;
	# Add opening and closing brackets to not-expressions with equality-operator
	s/([ (]+not[ ][a-zA-Z0-9_]+[ ]*=[ ]*[a-zA-Z0-9_]+)/\ ($1\)/i;	
	# Finally beautify some keywords
	# Standalone words only. with spaces or brackets on both sides (except "FORCE").
	# 1) AND
	s/ and / AND /i;
	s/\)and /\)AND /i;
	s/ and\(/ AND\(/i;
	s/\)and\(/\)AND\(/i;	
	# 2) OR
	s/ or / OR /i;
	s/\)or /\)OR /i;
	s/ or\(/ OR\(/i;
	s/\)or\(/\)OR\(/i;	
	# 3) XOR
	s/ xor / XOR /i;
	s/\)xor /\)XOR /i;
	s/ xor\(/ XOR\(/i;
	s/\)xor\(/\)XOR\(/i;	
	# 4) NOT, 
	# Be careful, contrary to the other operators 
	# there can be no closing bracket before NOT, but an opening one.
	s/ not / NOT /i;
	s/\(not /\(NOT /i;
	s/ not\(/ NOT\(/i;
	s/\(not\(/\(NOT\(/i;
	# 5)  FORCE
	s/ force/ FORCE/i;	
	# 6) CUSTOM
	s/^custom/CUSTOM/i;
	# 7) Betting rounds
	s/^preflop/PREFLOP/i;
	s/^flop/FLOP/i;
	s/^turn/TURN/i;
	s/^river/RIVER/i;
	# Some beautifications...
	# Remove superflous spaces before opening brackets (but keep one)...
	s/     \(/ \(/;
	s/    \(/ \(/;
	s/   \(/ \(/;
	s/  \(/ \(/;
	# ... and after opening brackets...
	s/\(     /\(/;
	s/\(    /\(/;
	s/\(   /\(/;
	s/\(  /\(/;
	s/\( /\(/;
	# ... and before closing brackets....
	s/     \)/\)/;
	s/    \)/\)/;
	s/   \)/\)/;
	s/  \)/\)/;
	s/ \)/\)/;
	# ... and finally after closing brackets (but keep one).
	s/\)     /\) /;
	s/\)    /\) /;
	s/\)   /\) /;
	s/\)  /\) /;
	# Proper indentation
	if ((m/^WHEN /i || m/ WHEN /i) && !(m/ FORCE$/i) && !(m/(user[A-Za-z_]*)$/))
	{
		# Open-ended when-condition
		# No indentation
		s/^([ ]*when)/WHEN/i;		
	}
	else
	{
		# When-condition with action or with user-variable
		# Indentation of 4 spaces
		s/^([ ]*when)/    WHEN/i;	
	}
	# And finally write the processed line to standard output
	print;
}
