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
# Known problems:
#   * does not translate multi-line-expressions, because we assume, 
#     that lines without an action are open-ended when-conditions
#     and add a terminating bracket at the "end".    
#
################################################################################
#
# Things to know (for programmers, not for end-users)
#   * http://www.troubleshooters.com/codecorn/littperl/perlreg.htm
#   * take a good editor with syntax high-lighting like NotePad++
#
################################################################################

use strict;
use warnings;
use diagnostics;

my $current_line_of_code = 0;

print "//\n";
print "// Cleaned up and brackets added by \"bracketify.pl\"\n";
print "//\n";

# While there is input: process it line by line
while (<>)
{
	$current_line_of_code++;
	# Print comments (full line only) as is, without any processing
	if (m/^\/\//)
	{
		print;
		next;
	}
	# First replace tabs by 4 spaces to make life more easy
	#
	# Substitution is case-insensitive ("i" at the end), 
	# but we care about beautyful, readable output and "fix" the cases.
	s/\t/    /g;
	# Also remove spaces at the beginning and at the end.
	s/^[ ]*//;
	s/[ ]*$//;
	# Ignore Option-settings
	if (m/MaxSessionHands =/ig
		|| m/MaxSessionTime = /ig
		|| m/MaxStackSize =/ig
		|| m/BathroomBreakAfter =/ig
		|| m/BathroomBreakTime =/ig
		|| m/SitOutWhenOpponentsAreLessThan =/ig
		|| m/MakePotSizedPreFlopRaisesWhen =/ig
		|| m/FoldToPre-FlopRaisesForAQAJsKQ =/ig
		|| m/FoldPost-FlopBelowTopPairToAnyBetOrRaise =/ig
		|| m/FoldToPost-FlopRaisesWithUnpairedBoardsFor =/ig
		|| m/FoldToPost-FlopRaisesWithPairedBoardsFor =/ig
		|| m/NeverBetTurnOrRiverWithHandBelow =/ig
		|| m/NeverBetFlopWithHandBelowMiddlePairWhen =/ig
		|| m/NeverBetFlopWithHandBelowMiddlePairStackSize =/ig
		|| m/NeverCallPairedBoardsWithHandBelow =/ig
		|| m/NeverCallFlushPossibleBoardsWithHandBelow =/ig
		|| m/FoldToPre-FlopRaiseWith55Or66When =/ig
		|| m/FoldToPre-FlopRaiseWith77Or88When =/ig
		|| m/FoldToPre-FlopRaiseWith99OrTTWhen =/ig
		|| m/FoldToPre-FlopRaiseWithJJOrQQWhen =/ig
		|| m/FoldToPre-FlopRaiseWithAKWhen =/ig
		|| m/FoldToPre-FlopRaiseWithAQsWhen =/ig
		|| m/FoldToPre-FlopRaiseWithAKsWhen =/ig
		|| m/FoldToPre-FlopRaiseWithKQsWhen =/ig
		|| m/SuperTightPlay =/ig
		|| m/SemiSuperTightPlay =/ig
		|| m/NoLimp =/ig
		|| m/AggressivePreFlop =/ig
		|| m/AggressivePostFlop =/ig
		|| m/AggressivePreFlopStackSize =/ig
		|| m/AggressivePostFlopStackSize =/ig
		|| m/MiningForSets =/ig
		|| m/PostFlopinBlindsOverride =/ig 
		|| m/PreFlopReRaiseOverride =/ig
		|| m/MinBetOverride =/ig
		|| m/FoldOrGoAllInWhenOpponentsAfterFoldsLessThan =/ig
		|| m/FoldOrGoAllInWhenOpponentsAfterFoldsLessThanStackSize =/ig
		|| m/ApplyFoldOrAllInAfter =/ig
		|| m/FoldOrGoAllInWhenOpponentsAtTableLessThan =/ig
		|| m/FoldOrGoAllInWhenOpponentsAtTableLessThanStackSize =/ig
		|| m/AlwaysCallWhenOpponentsRaiseIsLessThan =/ig
		|| m/AlwaysCallWhenStackSizeIsLessThanTotalInvested =/ig
		|| m/Verbose =/ig
		|| m/AutoPlay =/ig
		|| m/UserDefinedGameType =/ig
		|| m/UseOcrAtFullTilt =/ig
		|| m/AutoRebuyAtUB =/ig
		|| m/AlwaysAskForTableName =/ig
		|| m/EnableFoldInTurn =/ig 
		|| m/DefaultBetSizeFlop =/ig
		|| m/DefaultBetSizeTurn =/ig
		|| m/DefaultBetSizeRiver =/ig
		|| m/MoveMouseBeforeClickingAtFT =/ig
		|| m/WindowLeft =/ig
		|| m/WindowRight =/ig
		|| m/WindowBottom =/ig
		|| m/WindowTop =/ig)
	{
		# No processing, only print a warning, then skip it.
		print STDERR "\n";
		print STDERR "WARNING: option settings not supported -- ignored!\n";
		# Print current line of code
		print STDERR "Line: $current_line_of_code\n";
		print STDERR "Code: $_\n";
		next;
	}
	# Then try to detect multi-line-conditions,
	# i.e. lines with AND or OR at the beginning or at the end,
	# also lines with any bracket at the beginning or an opening bracket at the end.
	# We can't handle them, so terminate with error in that case.
	if ((m/^AND /i) or (m/^AND\(/i) or (m/^OR /i) or (m/^OR\(/i)
		or (m/ AND$/i) or (m/\)AND$/i) or (m/ OR$/i) or (m/\)OR$/i)
		or (m/^\)/i) or (m/^\(/i) or (m/\($/i))
	{
		# On error print to STDERR because STDOUT gets redirected to a file
		print STDERR "\n";
		print STDERR "ERROR: multi-line-condition detected.\n";
		print STDERR "OpenPPL could handle that, but bracketify.pl works line by line.\n";
		print STDERR "Please make it a one-liner-expression.\n";
		print STDERR "Line: $current_line_of_code\n";
		# Print current line of code
		print STDERR "Code: $_\n";
		exit
	}
	# Then add a bracket after when-conditions.
	# Be careful: we alwaxs need an extra-bracket, even if one exists,
	# because we always add a closing bracket later.
	s/when[ ]+/WHEN \(/i;
	s/when\(/WHEN \(\(/i;
	# But the funny thing is the right end of the expression
	# As the end is hard to detect with stupid regular expressions we...
	# * add brackets to the left of any action
	# * add brackets to the end of a line, if there is no action
	# * add brackets before user-defined variable to be set
	# * add brackets before any memory-store-command (me_st_)
	# Fist case: brackets before an action
	# We have to use more than the action keyword here,
	# otherwise code like "BotsLastAction = Raise" would be bracketed, too.	
	# Additionally we also prepare the line-info here (more details later)
	s/[ ]+betmax[ ]+force/\) LineInfo_Act BETMAX FORCE/i;
	s/[ ]+raisemax[ ]+force/\) LineInfo_Act RAISEMAX FORCE/i;
	s/[ ]+allin[ ]+force/\) LineInfo_Act ALLIN FORCE/i;
	s/[ ]+betpot[ ]+force/\) LineInfo_Act BETPOT FORCE/i;
	s/[ ]+raisepot[ ]+force/\) LineInfo_Act RAISEPOT FORCE/i;
	s/[ ]+bethalfpot[ ]+force/\) LineInfo_Act BETHALFPOT FORCE/i;
	s/[ ]+raisehalfpot[ ]+force/\) LineInfo_Act RAISEHALFPOT FORCE/i;
	s/[ ]+betmin[ ]+force/\) LineInfo_Act BETMIN FORCE/i;
	s/[ ]+raisemin[ ]+force/\) LineInfo_Act RAISEMIN FORCE/i;
	s/[ ]+bet[ ]+force/\) LineInfo_Act BET FORCE/i;
	s/[ ]+raise[ ]+force/\) LineInfo_Act RAISE FORCE/i;
	s/[ ]+call[ ]+force/\) LineInfo_Act CALL FORCE/i;
	s/[ ]+check[ ]+force/\) LineInfo_Act CHECK FORCE/i;
	s/[ ]+fold[ ]+force/\) LineInfo_Act FOLD FORCE/i;
	s/[ ]+sitout[ ]+force/\) LineInfo_Act SITOUT FORCE/i;
	# Slightly different line-info for return-statements, ending on "_Ret"
	s/[ ]+return[ ]/\) LineInfo_Ret RETURN /i;
	# Still first case: brackets before an action
	# Now for Bet/Raise X% FORCE or Bet/Raise X Force,
	# where Bet/Raise will be followed by a number.
	# For OpenPPL arbitrary expressions are possible here, but..
	# * arbitrary expressions would be too complicated for stupid regular expressions
	# * this tool is meant for Standard-PPL to be translated, where only numbers are possible
	# so we check for constant numbers only.
	# But first remove superfluous spaces (so that we don't miss anything later)...
	s/[ ]+bet[ ]+/ BET /i;
	s/[ ]+raise[ ]+/ RAISE /i;
	# ...and then add brackets
	s/[ ]+bet[ ]0/\) LineInfo_Act BET 0/i;	
	s/[ ]+bet[ ]1/\) LineInfo_Act BET 1/i;
	s/[ ]+bet[ ]2/\) LineInfo_Act BET 2/i;
	s/[ ]+bet[ ]3/\) LineInfo_Act BET 3/i;
	s/[ ]+bet[ ]4/\) LineInfo_Act BET 4/i;
	s/[ ]+bet[ ]5/\) LineInfo_Act BET 5/i;
	s/[ ]+bet[ ]6/\) LineInfo_Act BET 6/i;
	s/[ ]+bet[ ]7/\) LineInfo_Act BET 7/i;
	s/[ ]+bet[ ]8/\) LineInfo_Act BET 8/i;
	s/[ ]+bet[ ]9/\) LineInfo_Act BET 9/i;
	s/[ ]+raise[ ]0/\) LineInfo_Act RAISE 0/i;
	s/[ ]+raise[ ]1/\) LineInfo_Act RAISE 1/i;
	s/[ ]+raise[ ]2/\) LineInfo_Act RAISE 2/i;
	s/[ ]+raise[ ]3/\) LineInfo_Act RAISE 3/i;
	s/[ ]+raise[ ]4/\) LineInfo_Act RAISE 4/i;
	s/[ ]+raise[ ]5/\) LineInfo_Act RAISE 5/i;
	s/[ ]+raise[ ]6/\) LineInfo_Act RAISE 6/i;
	s/[ ]+raise[ ]7/\) LineInfo_Act RAISE 7/i;
	s/[ ]+raise[ ]8/\) LineInfo_Act RAISE 8/i;
	s/[ ]+raise[ ]9/\) LineInfo_Act RAISE 9/i;
	# Now caring about expressions,
	# which need to be enclodes in brackets
	s/[ ]+bet[ ]\(/\) LineInfo_Act BET \(/i;	
	s/[ ]+raise[ ]\(/\) LineInfo_Act RAISE \(/i;
	# Second case case: there is no action at the end,
	# i.e. a keyword "WHEN", but no "FORCE" and no user-variable.
	# Then we add a bracket to the right end of the line
	# and hope, that it is no multi-line condition
	if ((m/^WHEN /i || m/ WHEN /i) 
		&& !(m/ FORCE$/i)                   # Action
		&& !(m/(user[A-Za-z0-9_]*)$/i)      # User-variable
		&& !(m/me_st[A-Za-z0-9_\$]*$/i))  # Memory-store-command
	{
		s/$/\)/i;	
	}
	# Third case: user defined variable to be set,
	# where the line ends with a user-variable.
	# But first we remove superfluos spaces at the end to make things more robust...
	s/[ ]*$//;
	#... and then take care about user-variables.
	# There  are 3 sub-cases...
	if ((m/and[ ]+user[A-Za-z0-9_]*$/i) || (m/or[ ]+user[A-Za-z0-9_]*$/i)
		|| (m/not[ ]+user[A-Za-z0-9_]*$/i))
	{
		# Sub-case 1) user-variables after an operator like AND / OR / NOT.
		# In this case the variable is part of an open-ended-when-condition
		# and we have to add a bracket after it.
		s/((user[A-Za-z0-9_]*)$)/$1\)/i;
	}
	elsif (m/when[ ]*\([ ]*user[A-Za-z0-9_]*$/i)
	{	
		# Sub-case 2)Open-ended when-condition, with a single user-variable
		# Be careful, there already is an opening bracket after when.
		# In this case we also have to set the closing bracket.
		s/((user[A-Za-z0-9_]*)$)/$1\)/i;
	}
	else
	{
		# Sub-case 3) user-variable to be set (without operator)
		# In this case we have to add a closing bracket before the variable
		# The line-info for this case contains the postfix "_Var".
		s/((user[A-Za-z0-9_]*)$)/\) LineInfo_Var $1/i;
	}
	# 4th main case: OpenHoldems memory-story-command "me_st_"
	# As it should only be used at the very end after a condition
	# we can set very easily a closing bracket before it.
	# Postfix for the line-info is here "Mem".
	s/me_st_/) LineInfo_Mem me_st_/i;
	# Add opening and closing brackets to hand expressions
	# This requires a bit more complex regular expressions, 
	# because hand expressions can be spelled in different ways:
	#   * Hand = A K
	#   * Hand = AK
	#   * Hand = A K Suited
	#   * Hand = AK Suietd
	#   * Hand = Azuited
	# Note: suits like AhKs are not supported by OpenHoldem.
	# This was the old way to randomize actions with Shanky-PPL,
	# before "Random" got introduced.  
	# But we translate it anyway to avoid errors here.
	# OpenPPL will throw an error-message later.
	# Optional "uited" only, because the "s" got already caught.	
	s/([ (]+hand[ ]*=[ ]*[2-9TJQKAcdhs ]+(uited)?)/\($1\)/ig;	
	# Add opening and closing brackets to board-expressions
	# Similar to hand expressions
	s/([ (]+board[ ]*=[ ]*[2-9TJQKAcdhs ]+(uited)?)/\($1\)/ig;
	# One problem of the expressions above:
	# They might catch the "A" of "AND" of a hand- or board-expression.
	# Also "CA" of "CALL", "C" of "CHECK" or "A" of "ALLIN".
	# So we fix this.
	s/a\)nd/\) AND/ig;	
	s/ca\)ll/\) CALL/ig;
	s/c\)heck/\) CHECK/ig;
	s/a\)llin/\) ALLIN/ig;
	# Add opening and closing brackets to not-expressions with equality-operator,
	# also with smaller / larger comparisons.	
	s/([ (]+not[ ]+)([a-zA-Z0-9_]+[ ]*[=<>]+[ ]*[a-zA-Z0-9_]+)/$1 \($2\)/ig;
	# halh-made general solution
	#s/([ (]+(not)*(and)*(or)*[ ]+)([a-zA-Z0-9_]+[ ]*[=<>]+[ ]*[a-zA-Z0-9_]+)[ ]+(not)*(and)*(or)*[ ()]+/$1 \($2 $3\)/ig;
	# Finally beautify some keywords
	# Standalone words only. with spaces or brackets on both sides (except "FORCE").
	# 1) AND
	s/[ ]+and[ ]+/ AND /ig;
	s/\)and[ ]+/\) AND /ig;
	s/[ ]+and\(/ AND \(/ig;
	s/\)and\(/\) AND \(/ig;	
	# 2) OR
	s/[ ]+or[ ]+/ OR /ig;
	s/\)or[ ]+/\) OR /ig;
	s/[ ]+or\(/ OR \(/ig;
	s/\)or\(/\) OR \(/ig;	
	# 3) XOR
	s/[ ]+xor[ ]+/ XOR /ig;
	s/\)xor[ ]+/\) XOR /ig;
	s/[ ]+xor\(/ XOR \(/ig;
	s/\)xor\(/\) XOR \(/ig;	
	# 4) NOT, 
	# Be careful, contrary to the other operators 
	# there can be no closing bracket before NOT, but an opening one.
	s/[ ]+not[ ]+/ NOT /ig;
	s/\(not[ ]+/\(NOT /ig;
	s/[ ]+not\(/ NOT \(/gi;
	s/\(not\(/\(NOT \(/ig;
	# 5)  FORCE
	s/[ ]+force/ FORCE/ig;	
	# 6) CUSTOM
	s/^custom/CUSTOM/i;
	# 7) Betting rounds
	s/^preflop/PREFLOP/i;
	s/^flop/FLOP/i;
	s/^turn/TURN/i;
	s/^river/RIVER/i;
	# 7) Hand and Board
	s/\(hand/\(Hand/ig;
	s/\(board/\(Board/ig;
	# 8) Position symbols like InButton -> In Button
	s/button/Button/ig;
	s/smallblind/SmallBlind/ig;
	s/bigblind/BigBlind/ig;
	s/cutoff/CutOff/ig;
	s/EarlyPosition1/EarlyPosition1/ig; 
	s/EarlyPosition2/EarlyPosition2/ig;
	s/EarlyPosition3/EarlyPosition3/ig;
	s/MiddlePosition1/MiddlePosition1/ig;
	s/MiddlePosition2/MiddlePosition2/ig;
	s/MiddlePosition3/MiddlePosition3/ig;
	s/TheBlinds/TheBlinds/ig;
	s/UTG/UTG/ig;
	# Some beautifications...
	# Remove superflous spaces before opening brackets (but keep one)...
	s/[ ]+\(/ \(/g;
	# ... and after opening brackets...
	s/\([ ]+/\(/g;
	# ... and before closing brackets....
	s/[ ]+\)/\)/g;
	# ... and finally after closing brackets (but keep one).
	s/\)[ ]+/\) /g;
	# Remove multiple spaces before and after operators
	s/[ ]+\+[ ]+/ \+ /g;
	s/[ ]+\-[ ]+/ \- /g;
	s/[ ]+\*[ ]+/ \* /g;
	s/[ ]+\/[ ]+/ \/ /g;
	s/[ ]+\=[ ]+/ \= /g;
	s/[ ]+\>[ ]+/ \> /g;
	s/[ ]+\<[ ]+/ \< /g;
	s/[ ]+\>\=[ ]+/ \>\= /g;
	s/[ ]+\<\=[ ]+/ \<\= /g;
	# Proper indentation
	if ((m/^WHEN /i || m/ WHEN /i) 
		&& !(m/ FORCE$/i)                 # Action
		&& !(m/(user[A-Za-z0-9_]*)$/i)    # User-variable
		&& !(m/me_st[A-Za-z0-9_\$]$/i))   # Memory-store-command
	{
		# Open-ended when-condition
		# No indentation
		s/^([ ]*when)/WHEN/ig;		
	}
	else
	{
		# When-condition with action or with user-variable or memory-store-command
		# Indentation of 4 spaces
		s/^([ ]*when)/    WHEN/ig;	
	}
	# Make sure, there is at least one space between "WHEN" and the first bracket
	s/when\(/WHEN \(/ig;
	# Easy to read cases for all built-in symbols.
	# E.g. "nobettingonturn" should be spelled "NoBettingOnTurn", etc.
	# 1) BettingAction symbols
	s/Bets/Bets/ig;
	s/BotCalledBeforeFlop/BotCalledBeforeFlop/ig;
	s/BotIsLastRaiser/BotIsLastRaiser/ig;
	s/BotRaisedBeforeFlop/BotRaisedBeforeFlop/ig;
	s/BotRaisedOnFlop/BotRaisedOnFlop/ig;
	s/BotRaisedOnTurn/BotRaisedOnTurn/ig;
	s/BotsActionsOnFlop/BotsActionsOnFlop/ig;
	s/BotsActionsOnThisRound/BotsActionsOnThisRound/ig;
	s/BotsActionsOnThisRoundIncludingChecks/BotsActionsOnThisRoundIncludingChecks/ig;
	s/BotsActionsPreflop/BotsActionsPreflop/ig;
	s/BotsLastAction/BotsLastAction/ig;
	s/BotsLastPreflopAction/BotsLastPreflopAction/ig;
	s/CalledOnFlop/CalledOnFlop/ig;
	s/CalledOnTurn/CalledOnTurn/ig;
	s/Calls/Calls/ig;
	s/CallsSinceLastRaise/CallsSinceLastRaise/ig;
	s/Checks/Checks/ig;
	s/Folds/Folds/ig;
	s/NoBettingOnFlop/NoBettingOnFlop/ig;
	s/NoBettingOnTurn/NoBettingOnTurn/ig;
	s/NumberOfRaisesBeforeFlop/NumberOfRaisesBeforeFlop/ig;
	s/NumberOfRaisesOnFlop/NumberOfRaisesOnFlop/ig;
	s/NumberOfRaisesOnTurn/NumberOfRaisesOnTurn/ig;
	s/OpponentCalledOnFlop/OpponentCalledOnFlop/ig;
	s/OpponentCalledOnTurn/OpponentCalledOnTurn/ig;
	s/OpponentIsAllin/OpponentIsAllin/ig;
	s/Raises/Raises/ig;
	s/RaisesBeforeFlop/RaisesBeforeFlop/ig;
	s/RaisesOnFlop/RaisesOnFlop/ig;
	s/RaisesOnTurn/RaisesOnTurn/ig;
	s/RaisesSinceLastPlay/RaisesSinceLastPlay/ig;
	# 2) Betsizes and stacksizes
	s/AmountToCall/AmountToCall/ig;
	s/BetSize/BetSize/ig;
	s/BigBlindSize/BigBlindSize/ig;
	s/MaxOpponentStackSize/MaxOpponentStackSize/ig;
	s/MaxStillToActStackSize/MaxStillToActStackSize/ig;
	s/MinOpponentStackSize/MinOpponentStackSize/ig;
	s/MinStillToActStackSize/MinStillToActStackSize/ig;
	s/PotSize/PotSize/ig;
	s/StackSize/StackSize/ig;
	s/StartingStackSize/StartingStackSize/ig;
	s/TotalInvested/TotalInvested/ig;
	s/TotalInvestedThisRound/TotalInvestedThisRound/ig;
	# 3) Board symbols
	s/2ndTopFlopCardPairedOnRiver/2ndTopFlopCardPairedOnRiver/ig;
	s/2ndTopFlopCardPairedOnTurn/2ndTopFlopCardPairedOnTurn/ig;
	s/AcePresentOnFlop/AcePresentOnFlop/ig;
	s/FlushOnBoard/FlushOnBoard/ig;
	s/FlushPossible/FlushPossible/ig;
	s/FlushPossibleOnFlop/FlushPossibleOnFlop/ig;
	s/FlushPossibleOnTurn/FlushPossibleOnTurn/ig;
	s/FourCardsToWheelOnBoard/FourCardsToWheelOnBoard/ig;
	s/FourOf1SuiteOnTurn/FourOf1SuiteOnTurn/ig;
	s/FullHouseOnBoard/FullHouseOnBoard/ig;
	s/KingPresentOnFlop/KingPresentOnFlop/ig;
	s/LowCardsOnBoard/LowCardsOnBoard/ig;
	s/MoreThanOneStraightPossibleOnFlop/MoreThanOneStraightPossibleOnFlop/ig;
	s/MoreThanOneStraightPossibleOnTurn/MoreThanOneStraightPossibleOnTurn/ig;
	s/OneCardFlushPossible/OneCardFlushPossible/ig;
	s/OneCardStraightFlushPossible/OneCardStraightFlushPossible/ig;
	s/OneCardStraightPossible/OneCardStraightPossible/ig;
	s/OneCardStraightPossibleOnTurn/OneCardStraightPossibleOnTurn/ig;
	s/Only1OneCardStraightPossible/Only1OneCardStraightPossible/ig;
	s/OnlyOneStraightPossible/OnlyOneStraightPossible/ig;
	s/OvercardsOnBoard/OvercardsOnBoard/ig;
	s/PairOnBoard/PairOnBoard/ig;
	s/PairOnFlop/PairOnFlop/ig;
	s/PairOnTurn/PairOnTurn/ig;
	s/QuadsOnBoard/QuadsOnBoard/ig;
	s/QueenPresentOnFlop/QueenPresentOnFlop/ig;
	s/RankOfPairOnBoard/RankOfPairOnBoard/ig;
	s/RankOfSpareCardWhenTwoPairOnBoard/RankOfSpareCardWhenTwoPairOnBoard/ig;	
	s/RiverCardisOvercardToBoard/RiverCardisOvercardToBoard/ig;
	s/RunnerRunnerFlushPossibleAtRiver/RunnerRunnerFlushPossibleAtRiver/ig;
	s/SecondTopflopCardPairedOnRiver/SecondTopflopCardPairedOnRiver/ig;
	s/SecondTopflopCardPairedOnTurn/SecondTopflopCardPairedOnTurn/ig;
	s/StraightFlushOnBoard/StraightFlushOnBoard/ig;
	s/StraightFlushPossible/StraightFlushPossible/ig;
	s/StraightFlushPossibleByOthers/StraightFlushPossibleByOthers/ig;
	s/StraightOnBoard/StraightOnBoard/ig;
	s/StraightPossible/StraightPossible/ig;
	s/StraightPossibleOnFlop/StraightPossibleOnFlop/ig;
	s/StraightPossibleOnTurn/StraightPossibleOnTurn/ig;
	s/SuitsOnBoard/SuitsOnBoard/ig;
	s/SuitsOnFlop/SuitsOnFlop/ig;
	s/ThreeCardStraightOnBoard/ThreeCardStraightOnBoard/ig;
	s/TopFlopCardPairedonRiver/TopFlopCardPairedonRiver/ig;
	s/TopFlopCardPairedonTurn/TopFlopCardPairedonTurn/ig;
	s/TripsOnBoard/TripsOnBoard/ig;
	s/TripsOnBoardOnTurn/TripsOnBoardOnTurn/ig;
	s/TurnCardisOvercardToBoard/TurnCardisOvercardToBoard/ig;
	s/TurnCardPaired/TurnCardPaired/ig;
	s/TwoOfOneSuitPresentOnFlop/TwoOfOneSuitPresentOnFlop/ig;
	s/TwoPairOnBoard/TwoPairOnBoard/ig;
	s/UncoordinatedFlop/UncoordinatedFlop/ig;
	s/WheelPossible/WheelPossible/ig;
	# 4) Handstrength symbols
	s/HadOverpairOnFlop/HadOverpairOnFlop/ig;
	s/HadPairOnFlop/HadPairOnFlop/ig;
	s/HadPairOnTurn/HadPairOnTurn/ig;
	s/HadTopPairOnFlop/HadTopPairOnFlop/ig;
	s/HadTopPairOnTurn/HadTopPairOnTurn/ig;
	s/HadTwoPairOnFlop/HadTwoPairOnFlop/ig;
	s/Have2ndBestKicker/Have2ndBestKicker/ig;
	s/Have2ndBestKickerOrBetter/Have2ndBestKickerOrBetter/ig;
	s/Have2ndBestOverpairOrBetter/Have2ndBestOverpairOrBetter/ig;
	s/Have2ndNutFlush/Have2ndNutFlush/ig;
	s/Have2ndNutFlushDraw/Have2ndNutFlushDraw/ig;
	s/Have2ndNutStraight/Have2ndNutStraight/ig;
	s/Have2ndOverPair/Have2ndOverPair/ig;
	s/Have2ndTopPair/Have2ndTopPair/ig;
	s/Have2ndTopSet/Have2ndTopSet/ig;
	s/Have3rdBestKicker/Have3rdBestKicker/ig;
	s/Have3rdBestKickerOrBetter/Have3rdBestKickerOrBetter/ig;
	s/Have3rdBestOverpairOrBetter/Have3rdBestOverpairOrBetter/ig;
	s/Have3rdNutFlush/Have3rdNutFlush/ig;
	s/Have3rdNutFlushDraw/Have3rdNutFlushDraw/ig;
	s/Have3rdOverpair/Have3rdOverpair/ig;
	s/Have3rdTopPair/Have3rdTopPair/ig;
	s/Have3rdTopSet/Have3rdTopSet/ig;
	s/Have4thNutFlush/Have4thNutFlush/ig;
	s/Have4thNutFlushDraw/Have4thNutFlushDraw/ig;
	s/Have4thOverpair/Have4thOverpair/ig;
	s/Have4thTopPair/Have4thTopPair/ig;
	s/Have4thTopSet/Have4thTopSet/ig;
	s/Have5thNutFlush/Have5thNutFlush/ig;
	s/Have5thNutFlushDraw/Have5thNutFlushDraw/ig;
	s/Have5thOverpair/Have5thOverpair/ig;
	s/HaveBackdoor2ndNutFlushDraw/HaveBackdoor2ndNutFlushDraw/ig;
	s/HaveBackdoor3rdNutFlushDraw/HaveBackdoor3rdNutFlushDraw/ig;
	s/HaveBackdoorFlushDraw/HaveBackdoorFlushDraw/ig;
	s/HaveBackdoorNutFlushDraw/HaveBackdoorNutFlushDraw/ig;
	s/HaveBackdoorStraightDraw/HaveBackdoorStraightDraw/ig;
	s/HaveBestKicker/HaveBestKicker/ig;
	s/HaveBestKickerOrBetter/HaveBestKickerOrBetter/ig;
	s/HaveBestOverpairOrBetter/HaveBestOverpairOrBetter/ig;
	s/HaveBottomPair/HaveBottomPair/ig;
	s/HaveBottomSet/HaveBottomSet/ig;
	s/HaveBottomTrips/HaveBottomTrips/ig;
	s/HaveBottomTwoPair/HaveBottomTwoPair/ig;
	s/HaveDoubleGutshotDraw/HaveDoubleGutshotDraw/ig;
	s/HaveFlush/HaveFlush/ig;
	s/HaveFlushDraw/HaveFlushDraw/ig;
	s/HaveFullHouse/HaveFullHouse/ig;
	s/HaveInsideNutStraightDraw/HaveInsideNutStraightDraw/ig;
	s/HaveInsideStraightDraw/HaveInsideStraightDraw/ig;
	s/HaveNothing/HaveNothing/ig;
	s/HaveNutFlush/HaveNutFlush/ig;
	s/HaveNutFlushCard/HaveNutFlushCard/ig;
	s/HaveNutFlushDraw/HaveNutFlushDraw/ig;
	s/HaveNuts/HaveNuts/ig;
	s/HaveNutStraight/HaveNutStraight/ig;
	s/HaveNutStraightDraw/HaveNutStraightDraw/ig;
	s/HaveNutStraightFlush/HaveNutStraightFlush/ig;
	s/HaveOpenEndedStraightDraw/HaveOpenEndedStraightDraw/ig;
	s/HaveOverPair/HaveOverPair/ig;
	s/HavePair/HavePair/ig;
	s/HaveQuads/HaveQuads/ig;
	s/HaveRunnerRunnerFlushAtRiver/HaveRunnerRunnerFlushAtRiver/ig;
	s/HaveSet/HaveSet/ig;
	s/HaveStraight/HaveStraight/ig;
	s/HaveStraightDraw/HaveStraightDraw/ig;
	s/HaveStraightFlush/HaveStraightFlush/ig;
	s/HaveTopNonBoardPairedPair/HaveTopNonBoardPairedPair/ig;
	s/HaveTopPair/HaveTopPair/ig;
	s/HaveTopSet/HaveTopSet/ig;
	s/HaveTopTrips/HaveTopTrips/ig;
	s/HaveTopTwoPair/HaveTopTwoPair/ig;
	s/HaveTrips/HaveTrips/ig;
	s/HaveTwoPair/HaveTwoPair/ig;
	s/HaveUnderPair/HaveUnderPair/ig;
	s/HaveUnderStraight/HaveUnderStraight/ig;
	s/KingPresentOnFlop/KingPresentOnFlop/ig;
	s/NutFullHouseOrFourOfAKind/NutFullHouseOrFourOfAKind/ig;
	s/Overcards/Overcards/ig;
	s/PairInHand/PairInHand/ig;
	s/SuitsInHand/SuitsInHand/ig;
	# 5) Other symbols
	s/IsFinalTable/IsFinalTable/ig;
	s/Others/Others/ig;
	s/Random/Random/ig;
	# 6) Player symbols
	s/Opponents/Opponents/ig;
	s/OpponentsAtTable/OpponentsAtTable/ig;
	s/OpponentsLeft/OpponentsLeft/ig;
	s/OpponentsOnFlop/OpponentsOnFlop/ig;
	s/OpponentsOnRiver/OpponentsOnRiver/ig;
	s/OpponentsOnTurn/OpponentsOnTurn/ig;
	s/OpponentsWithHigherStack/OpponentsWithHigherStack/ig;
	s/OpponentsWithLowerStack/OpponentsWithLowerStack/ig;
	s/StillToAct/StillToAct/ig;
	# 7) PokerTracker Symbols
	s/PT_LastRaiserPFR/PT_LastRaiserPFR/ig;
	s/PT_LastRaiserVPIP/PT_LastRaiserVPIP/ig;
	s/PT_LastRaiser_AttemptToStealBlinds/PT_LastRaiser_AttemptToStealBlinds/ig;
	s/PT_LastRaiser_Hands/PT_LastRaiser_Hands/ig;
	s/PT_BigBlind_FoldBBToSteal/PT_BigBlind_FoldBBToSteal/ig;
	s/PT_SmallBlind_FoldSBToSteal/PT_SmallBlind_FoldSBToSteal/ig;
	s/PT_SmallBlind_VPIP/PT_SmallBlind_VPIP/ig;
	# 8) Position symbols
	s/FirstCallerPosition/FirstCallerPosition/ig;
	s/FirstRaiserPosition/FirstRaiserPosition/ig;
	s/LastCallerPosition/LastCallerPosition/ig;	
	s/LastRaiserPosition/LastRaiserPosition/ig;
	s/Position/Position/ig;
	s/First/First/ig;
	s/Middle/Middle/ig;
	s/Last/Last/ig;
	# Technical symbols
	# Not needed for standard PPL, and most probably not for openPPL either
	#
	# Check for unsafe code	and warn the user
	if ((m/Betsize =/i) or (m/TotalInvested =/i) or (m/AmountToCall =/i)
		or (m/StartingStacksize =/i) or (m/MinOpponentStacksize =/i)
		or (m/MaxOpponentStacksize =/i) or (m/Stacksize =/i)
		or (m/PotSize =/i))
	{
		print STDERR "\n";
		print STDERR "WARNING: Unsafe Stacksize operation!\n";
		print STDERR "\"StackSize = X\" and similar expressions\n";
		print STDERR "will cause troubles, because OpenHoldem uses real numbers.\n";
		print STDERR "You really should fix it; better use <= or >= instead.\n\n";
		# Print current line of code
		print STDERR "Line: $current_line_of_code\n";
		print STDERR "Code: $_\n";
		# No way to automatically fix it...
		exit
	}
	if (m/NutFullHouseOrFourOfAKind = 0/i)
	{
		s/NutFullHouseOrFourOfAKind = 0/NutFullHouseOrFourOfAKind = f\$OpenPPL_HINT_We_Dont_Have_A_FullHouse_At_All/ig;
		# "NutFullHouseOrFourOfAKind = 0" is unsafe.
		# OpenPPL uses consistently higher numbers for worse hands
		# and 999 if we don't have a full house at all.
		print STDERR "\n";
		print STDERR "WARNING: Unsafe NutFullHouseOrFourOfAKind-code auto-corrected!\n";
		print STDERR "\"NutFullHouseOrFourOfAKind = 0\" is unsafe.\n";
		print STDERR "OpenPPL uses consistently higher numbers for worse hands\n";
		print STDERR "and 999 if we don't have a full house at all.\n";
		print STDERR "Line: $current_line_of_code\n";
		print STDERR "Code: $_\n";
	}
	# Finally care about line-numbers
	if (m/LineInfo/i)
	{
		# We need another set of brackets, so one opening bracket after "When "
		s/^WHEN /WHEN \(/i;
		s/ WHEN / WHEN \(/i;
		# Then we have 4 place-holders:
		# * LineInfo_Act for actions
		# * LineInfo_Ret for return statements
		# * LineInfo_Var for user-defined variables to be set
		# * LineInfo_Mem for OpenHoldems memory-store-command.
		# Each of them needs a closing bracket
		s/LineInfo_Act/LineInfo_Act\)/i;
		s/LineInfo_Ret/LineInfo_Ret\)/i;
		s/LineInfo_Var/LineInfo_Var\)/i;
		s/LineInfo_Mem/LineInfo_Mem\)/i;
		# We keep the postfix, but replace "LineInfo" by something like "log$line_xyz"
		# so a message like "line_3147_Act" will appear in the log-file.
		# The log$-expression always evaluates to 1 (true),
		# so the result of new combined condition will be the same as the old one.
		s/LineInfo/  AND  log\$line_$current_line_of_code/i; 
	}
	# And finally write the processed line to standard output
	print;
}
