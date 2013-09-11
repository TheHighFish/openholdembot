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

#ifndef INC_CSYMBOLENGINEACTIVEDEALTPLAYING_H
#define INC_CSYMBOLENGINEACTIVEDEALTPLAYING_H

#include "CVirtualSymbolEngine.h"
#include "CSymbolEngineUserchair.h"
#include "NumericalFunctions.h"

extern class CSymbolEngineActiveDealtPlaying: public CVirtualSymbolEngine
{
public:
	CSymbolEngineActiveDealtPlaying();
	~CSymbolEngineActiveDealtPlaying();
public:
	// Mandatory reset-functions
	void InitOnStartup();
	void ResetOnConnection();
	void ResetOnHandreset();
	void ResetOnNewRound();
	void ResetOnMyTurn();
	void ResetOnHeartbeat();
public:
	// Public accessors
	int playersactivebits()		{ return _playersactivebits & playersseatedbits(); }
	int opponentsactivebits()	{ return (playersactivebits() & ~userchairbit());}
	int nplayersactive()		{ return bitcount(playersactivebits()); }
	int nopponentsactive()		{ return bitcount(opponentsactivebits()); }
public:
	int playersplayingbits()	{ return _playersplayingbits & playersseatedbits(); }
	int opponentsplayingbits()	{ return (playersplayingbits() & ~userchairbit());}
	int nplayersplaying()		{ return bitcount(playersplayingbits()); }
	int nopponentsplaying()		{ return bitcount(opponentsplayingbits()); }
public:
	int playersdealtbits()		{ return _playersdealtbits; }
	int opponentsdealtbits()	{ return (_playersdealtbits & ~userchairbit());}
	int nplayersdealt()			{ return bitcount(playersdealtbits()); }
	int nopponentsdealt()		{ return bitcount(opponentsdealtbits()); }
public:
	int playersseatedbits()		{ return _playersseatedbits; }
	int opponentsseatedbits()	{ return (_playersseatedbits & ~userchairbit());}
	int nplayersseated()		{ return bitcount(playersseatedbits()); }
	int nopponentsseated()		{ return bitcount(opponentsseatedbits()); }
public:
	// Especially useful for the case when we are only interested in opponents
	// and calculate that value from players, subtracting the userchair.
	int userchairbit()				{ return 1 << USER_CHAIR; }
private:
	void CalculateActiveBits();
	void CalculatePlayingBits();
	void CalculateDealtBits();
	void CalculateSeatedBits();
private:
	int _playersactivebits;
	int _playersplayingbits;
	int _playersdealtbits;
	int _playersseatedbits;
} *p_symbol_engine_active_dealt_playing;

#endif INC_CSYMBOLENGINEACTIVEDEALTPLAYING_H