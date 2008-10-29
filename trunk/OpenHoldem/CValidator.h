#ifndef INC_VALIDATOR_H
#define INC_VALIDATOR_H

class CValidator
{
public:
	// public functions
	CValidator();
	~CValidator();
	void validate_GameState();

private:
	// private functions
	void validate_single_Rule();
	double gws(const char *the_Symbol);

private:
	// private data members, not (directly) accessible
	char	*_testcase_id;
	bool	_precondition;
	bool	_postcondition;
	char	*_reasoning;
	char	*_symbols_possibly_affected;
}; 

#endif
