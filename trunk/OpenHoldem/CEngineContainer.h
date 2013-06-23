#ifndef INC_CENGINECONTAINER_H
#define INC_CENGINECONTAINER_H

#include "CVirtualSymbolEngine.h"

const int k_max_number_of_symbol_engines = 27;

extern class CEngineContainer
{
public:
	CEngineContainer();
	~CEngineContainer();
public:
	void ResetOnConnection();
	void CallSymbolEnginesToUpdateSymbolsIfNecessary();
private:
	void CreateSpecialSymbolEngines();
	void CreateSymbolEngines();
	void DestroyAllSymbolEngines();
	void DestroyAllSpecialSymbolEngines();
private:
	void ResetOnHandreset();
	void ResetOnNewRound();
	void ResetOnMyTurn();
	void ResetOnHeartbeat();
private:
	CVirtualSymbolEngine *_symbol_engines[k_max_number_of_symbol_engines]; 
	int _number_of_symbol_engines_loaded;
} *p_engine_container;

#endif INC_CENGINECONTAINER_H