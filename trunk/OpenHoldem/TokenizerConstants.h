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

#ifndef INC_TOKENIZERCONSTANTS_H
#define INC_TOKENIZERCONSTANTS_H

enum {
  // End of file
  kTokenEndOfFile = 0,
  kTokenEndOfFunction,
  // Operators
  kTokenOperatorPlus,               // -
  kTokenOperatorMinus,              // +
  kTokenOperatorMultiplication,     // *
  kTokenOperatorDivision,           // /
  kTokenOperatorUnaryMinus,         // -
  kTokenOperatorModulo,             // % (OpenHoldem)
  kTokenOperatorExponentiation,     // **
  kTokenOperatorLog,                // ln
  kTokenOperatorEquality,           // == (OpenHoldem) = (OpenPPL)
  kTokenOperatorApproximatellyEqual,// ~~ 
  kTokenOperatorSmaller,            // <
  kTokenOperatorSmallerOrEqual,     // <= 
  kTokenOperatorGreater,            // >
  kTokenOperatorGreaterOrEqual,     // >=
  kTokenOperatorNotEqual,           // !=
  kTokenOperatorLogicalAnd,         // &&
  kTokenOperatorLogicalOr,	        // ||
  kTokenOperatorLogicalNot,         // !
  kTokenOperatorLogicalXOr,         // ^^
  kTokenOperatorBinaryAnd,          // &
  kTokenOperatorBinaryOr,           // |
  kTokenOperatorBinaryNot,          // ~
  kTokenOperatorBinaryXOr,          // ^
  kTokenOperatorBitShiftLeft,       // <<
  kTokenOperatorBitShiftRight,      // >>
  kTokenOperatorBitCount,           // `	
  kTokenOperatorPercentage,         // % (OpenPPL)
  kTokenOperatorConditionalIf,      // ?
  kTokenOperatorConditionalElse,    // :
  kTokenOperatorConditionalWhen,    // WHEN
  // Brackets
  kTokenBracketOpen_1,              // (				
  kTokenBracketOpen_2,              // [
  kTokenBracketOpen_3,              // {
  kTokenBracketClose_1,             // )
  kTokenBracketClose_2,             // ]
  kTokenBracketClose_3,             // }
  // Numbers, Identifier, Cards
  kTokenIdentifier,
  kTokenNumber,
  kTokenCards,
  // ## as start and end of list and function-headers
  kTokenDoubleShebang,              // ##
  // OpenPPL actions
  kTokenActionBeep,
  kTokenActionFold,
  kTokenActionCheck,
  kTokenActionCall,
  kTokenActionRaise,
  kTokenActionRaiseHalfPot,
  kTokenActionRaisePot,
  kTokenActionRaiseMax,
  kTokenActionReturn,
  kTokenActionSitOut,
  kTokenActionLeave,
  kTokenActionClose,
  // OpenPPL keyword FORCE
  kTokenKeywordForce,
  // Shanky-style delay (unsupported)
  kTokenUnsupportedDelay,
  // Special action-constants for node-types
  // Not really tokens, but handled here for consistency
  kTokenActionRaiseByBigBlinds,
  kTokenActionRaiseByPercentagedPotsize,
  kTokenActionUserVariableToBeSet,
  // Always leave that at the very end
  kNumberOfTokens,
};

inline bool TokenIsBracketOpen(int token) {
  return ((token == kTokenBracketOpen_1)
    || (token == kTokenBracketOpen_2)
	|| (token == kTokenBracketOpen_3));
}

bool TokenIsUnary(int token);

bool TokenIsBinary(int token);

inline bool TokenIsTernary(int token) {
  return ((token == kTokenOperatorConditionalIf)
    || (token == kTokenOperatorConditionalWhen));
}

inline bool TokenIsElementaryAction(int token) {
  return ((token >= kTokenActionBeep)
    && (token <= kTokenActionClose));
}

inline bool TokenIsBracketClose(int token) {
  return ((token == kTokenBracketClose_1)
    || (token == kTokenBracketClose_2)
	|| (token == kTokenBracketClose_3));
}

inline bool TokenIsOpenPPLAction(int token) {
  return ((token >= kTokenActionBeep)
    && (token <= kTokenActionClose));
}

int GetOperatorPriority(int token);

// Right assopciativ: only **, ln and conditional
inline int IsOperatorRightAssociativ(int token) {
  return ((token == kTokenOperatorExponentiation)
    || (token == kTokenOperatorLog)
    || (token == kTokenOperatorConditionalIf));
}

// For debugging output
CString TokenString(int token);

#endif INC_TOKENIZERCONSTANTS_H