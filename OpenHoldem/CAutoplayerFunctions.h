#ifndef INC_CAUTOPLAYER_FUNCTIONS
#define INC_CAUTOPLAYER_FUNCTIONS

extern class CAutoplayerFunctions
{
public:
	CAutoplayerFunctions();
public:
	void Reset();
#define ENT CSLock lock(m_critsec);
	void	set_f$alli(const double d)		{ ENT _f$alli = d;}
	void	set_f$betsize(const double d)	{ ENT _f$betsize = d;}
	void	set_f$rais(const double d)		{ ENT _f$rais = d;}
	void	set_f$call(const double d)		{ ENT _f$call = d;}
	void	set_f$sitin(const double d)		{ ENT _f$sitin = d;}
	void	set_f$sitout(const double d)	{ ENT _f$sitout = d;}
	void	set_f$leave(const double d)		{ ENT _f$leave = d;}
	void	set_f$close(const double d)		{ ENT _f$close = d;}
	void	set_f$prefold(const double d)	{ ENT _f$prefold = d;}
	void	set_f$rebuy(const double d)		{ ENT _f$rebuy = d;}
	void	set_f$delay(const double d)		{ ENT _f$delay = d;}
	void	set_f$chat(const double d)		{ ENT _f$chat = d;}
#undef ENT
public:
	double	f$alli()	{ return _f$alli; }
	double	f$betsize()	{ return _f$betsize; }
	double	f$rais()	{ return _f$rais; }
	double	f$call()	{ return _f$call; }
	double	f$sitin()	{ return _f$sitin; }
	double	f$sitout()	{ return _f$sitout; }
	double	f$leave()	{ return _f$leave; }
	double	f$close()	{ return _f$close; }
	double	f$prefold()	{ return _f$prefold; }
	double	f$rebuy()	{ return _f$rebuy; }
	double	f$delay()	{ return _f$delay; }
	double	f$chat()	{ return _f$chat; }
private:
	double		_f$alli,    _f$betsize, _f$rais,  _f$call; 
	double		_f$sitin,   _f$sitout,  _f$leave, _f$close; 
	double		_f$prefold, _f$rebuy,   _f$chat,  _f$delay;
private:
	CCritSec	m_critsec;
} *p_autoplayer_functions;

#endif // INC_CAUTOPLAYER_FUNCTIONS