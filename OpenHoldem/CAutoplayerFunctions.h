//******************************************************************************
//
// This file is part of the OpenHoldem project
//    Source code:           https://github.com/OpenHoldem/openholdembot/
//    Forums:                http://www.maxinmontreal.com/forums/index.php
//    Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//******************************************************************************
//
// Purpose:
//
//******************************************************************************

#ifndef INC_CAUTOPLAYER_FUNCTIONS
#define INC_CAUTOPLAYER_FUNCTIONS

#include "CSpaceOptimizedGlobalObject.h"

class CAutoplayerFunctions : public CSpaceOptimizedGlobalObject {
 public:
	CAutoplayerFunctions();
 public:
	void CalcPrimaryFormulas();
	void CalcSecondaryFormulas();
	double GetAutoplayerFunctionValue(const int function_code);
 private:
  void CalcPrimaryFormulasOHScript();
  void CalcPrimaryFormulasOpenPPL();
  void TranslateOpenPPLDecisionToAutoplayerFunctions(double decision);
  bool IsPercentagePotsizeBet(double decision);
  double BetSizeForPercentagedPotsizeBet(double decision);
 private:
  void CheckIfDecisionMatchesElementaryAction(int decision, int action);
  void CalculateSingleOpenPPLBackupAction(
    int potential_action, int potential_backup);
  void CalculateOpenPPLBackupActions();
 private:
  bool IsFoldAllinSituation();
 private:
	CCritSec	m_critsec;
};

extern CAutoplayerFunctions *p_autoplayer_functions;

#endif // INC_CAUTOPLAYER_FUNCTIONS