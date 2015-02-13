//******************************************************************************
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose: 
//
//******************************************************************************

// Selftest-function
// Upper-case to make it an invisible "OpenPPL"-function
const char* const kSelftestName = "Selftest";

const char* const kSelftestFunction = 
  "// Testing open-ended-when-conditions first\n"
  "WHEN 0\n"
  "   // This should never get called\n"
  "   WHEN 1 RETURN 1 FORCE\n"
  "WHEN 0\n"
  "   // This should not get called either\n"
  "   WHEN 1 RETURN 10 FORCE\n"
  "// The next OEWC has to be evaluated\n"
  "WHEN 1\n"
  "   // Next: testing inequality and negationfirst\n" 
  "   // which will be used heavily soon.\n"
  "   WHEN 0 != 0 RETURN 20 FORCE\n"
  "   WHEN !1 RETURN 21 FORCE\n"
  "   // Now testing all operators\n"
  "   WHEN 3 + 2 != 5 RETURN 30 FORCE\n"
  "   WHEN 3 - 2 != 1 RETURN 31 FORCE\n"
  "   WHEN 3 * 2 != 6 RETURN 32 FORCE\n"
  "   WHEN 6 / 2 != 3 RETURN 33 FORCE\n"
  "   // Percentage operator\n"
  "   // OH-script-style modulo can't get tested in OpenPPL\n"
  "   WHEN 3 % 2 != 0.06 RETURN 40 FORCE\n"
  "   // Exponentiation\n"
  "   WHEN 3 ** 2 != 9 RETURN 50 FORCE\n"
  "   // Comparison\n"
  "   WHEN 3 == 2 RETURN 61 FORCE\n"
  "   WHEN 3 ~~ 2 RETURN 62 FORCE\n"
  "   WHEN !(2.0000001 ~~ 2) RETURN 63 FORCE\n"
  "   WHEN 3 < 2 RETURN 64 FORCE\n"
  "   WHEN 3 > 4 RETURN 65 FORCE\n"
  "   WHEN 3 <= 2 RETURN 66 FORCE\n"
  "   WHEN 3 >= 4 RETURN 67 FORCE\n"
  "   // OH-script logical operators\n"
  "   WHEN 0 && 0 != 0 RETURN 70 FORCE\n"
  "   WHEN 0 && 1 != 0 RETURN 71 FORCE\n"
  "   WHEN 1 && 0 != 0 RETURN 72 FORCE\n"
  "   WHEN 1 && 1 != 1 RETURN 73 FORCE\n"
  "   WHEN (0 || 0) != 0 RETURN 74 FORCE\n"
  "   WHEN (0 || 1) != 1 RETURN 75 FORCE\n"
  "   WHEN (1 || 0) != 1 RETURN 76 FORCE\n"
  "   WHEN (1 || 1) != 1 RETURN 77 FORCE\n"
  "   WHEN 0 ^^ 0 != 0 RETURN 78 FORCE\n"
  "   WHEN 0 ^^ 1 != 1 RETURN 79 FORCE\n"
  "   WHEN 1 ^^ 0 != 1 RETURN 80 FORCE\n"
  "   WHEN 1 ^^ 1 != 0 RETURN 81 FORCE\n"
  "   // OpenPPL logical operators\n"
  "   WHEN 0 AND 0 != 0 RETURN 90 FORCE\n"
  "   WHEN 0 AND 1 != 0 RETURN 91 FORCE\n"
  "   WHEN 1 AND 0 != 0 RETURN 92 FORCE\n"
  "   WHEN 1 AND 1 != 1 RETURN 93 FORCE\n"
  "   WHEN (0 OR 0) != 0 RETURN 94 FORCE\n"
  "   WHEN (0 OR 1) != 1 RETURN 95 FORCE\n"
  "   WHEN (1 OR 0) != 1 RETURN 96 FORCE\n"
  "   WHEN (1 OR 1) != 1 RETURN 97 FORCE\n"
  "   WHEN 0 XOR 0 != 0 RETURN 98 FORCE\n"
  "   WHEN 0 XOR 1 != 1 RETURN 99 FORCE\n"
  "   WHEN 1 XOR 0 != 1 RETURN 100 FORCE\n"
  "   WHEN 1 XOR 1 != 0 RETURN 101 FORCE\n"
  "   // OH-script bitwise operators\n"
  "   WHEN 0b101 & 0b100 != 0b100 RETURN 110 FORCE\n"
  "   WHEN (0b100 | 0b010) != 0b110 RETURN 111 FORCE\n"
  "   WHEN (0b1010 ^ 0b1100) != 0b0110 RETURN 112 FORCE\n"
  "   // OpenPPL bitwise operators\n"
  "   WHEN 0b101 BITAND 0b100 != 0b100 RETURN 120 FORCE\n"
  "   WHEN (0b100 BITOR 0b010) != 0b110 RETURN 121 FORCE\n"
  "   WHEN (0b1010 BITXOR 0b1100) != 0b0110 RETURN 122 FORCE\n"
  "   // Binary negation\n"
  "   WHEN ~(~0b101) != 0b101 RETURN 130 FORCE\n"
  "   WHEN BITNOT (BITNOT 0b101) != 0b101 RETURN 131 FORCE\n"
  "   // Bit-shifts\n"
  "   WHEN 1 << 2 != 4 RETURN 140 FORCE\n"
  "   WHEN 8 >> 2 != 2 RETURN 141 FORCE\n"
  "   // Priority of operators\n"
  "   WHEN 1 + 2 * 3 ** 4 != 163 RETURN 150 FORCE\n"
  "   WHEN 1 ** 2 * 3 + 4 != 7 RETURN 151 FORCE\n"
  "   // Brackets\n"
  "   WHEN (2 + 3) * (4 + 5) != 45 RETURN 160 FORCE\n"
  "   // Symbol lookup gets implicitly tested, evaluating \"Selftest\"\n"
  "   // therefore we do no longer evaluate \"WHEN NOT True RETURN 170 FORCE\"\n"
  "   // to get rid of the dependency for the OpenPPPL-library.\n"
  "   //\n"
  "   // Finally: here we should return\n"
  "   WHEN 1 RETURN 180 FORCE\n"
  "// But this should never get called\n"
  "WHEN 1\n"
  "   WHEN 1 RETURN 190 FORCE\n"
  "// And if no OEWC gets evaluated at all, the result will be 0\n";

const double kSelftestExpectedResult = 180.0;

class CSelftestParserEvaluator {
 public:
  CSelftestParserEvaluator();
  ~CSelftestParserEvaluator();
 public:
  void Test();
};

