##################################################
#
# Rules2Cpp
# =========
#
# PURPOSE: Translating rules for the OH-Validator
#   into usable CPP-code (preprocessor-macros).
#
# USAGE: At the command-line:
#   "perl rules2cpp <THE_RULES_FILE>"
#   This will print the generated code to stdout.
#
# AUTHOR: TheHighFish
#
# DATE: 2008.11.31
#
##################################################
#
# Rules look like this:
#
# BEGIN_TESTCASE
#     TEST_ID ("0074")
#     REASONING ("Range check for symbol nbetstorais")
#     PRECONDITION (true)
#     POSTCONDITION (nbetstorais >= 1)
#     SYMBOLS_POSSIBLY_AFFECTED ("nbetstorais")
# END_TESTCASE
#
# We only have to put each symbol into a gws-function-call,
#   and leave the rest as is.
#
##################################################
#
# Operate on all lines...
while (<>)
{
	# Fix 2 common misspellings
	s/TEST_ID/TESTCASE_ID/g;
	s/TASTCASE/TESTCASE/g;
	# Print to stdout without change
	# * lines, that shall stay as is
	# * lines, that already got converted (including "gws")
	# * preprocessor macros ("#define")
	# * empty lines
	# * comments
	if ((/^.*BEGIN_TESTCASE.*$/)
		|| (/^.*TEST_ID.*/)
		|| (/^.*REASONING.*$/)
		|| (/^.*SYMBOLS_POSSIBLY_AFFECTED.*$/)
		|| (/^.*END_TESTCASE.*$/)
		|| (/^.*gws.*$/)
		|| (/^#define.*$/)
		|| (/^[ ]*$/)
		|| (/^.*\/\/.*$/))
	{
		print $_;
	}
	else
	{
		# In the other lines place a gws-callaround each symnols,
		#   then print to stdout.
		s/[a-z\$][a-z_\$0-9]*/gws("$&")/g;
		# If we substituted true or false,
		#   substitute it back.
		s/gws\("true"\)/true/g;
		s/gws\("false"\)/false/g;
		print $_;
	}
}