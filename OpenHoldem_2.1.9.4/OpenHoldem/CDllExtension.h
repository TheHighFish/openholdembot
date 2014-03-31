#ifndef INC_CDLLEXTENSION_H
#define INC_CDLLEXTENSION_H

//#define OHUSER_API __declspec(dllimport)
typedef double (*process_message_t)(const char* message, const void* param);

struct SHoldemPlayer 
{
	char			m_name[16]		  ;	//player name if known
	double			m_balance		   ;	//player balance 
	double			m_currentbet		;	//player current bet
	unsigned char   m_cards[2]		  ;	//player cards

	unsigned char   m_name_known	: 1 ;	//0=no 1=yes
	unsigned char   m_balance_known : 1 ;	//0=no 1=yes
	unsigned char   m_fillerbits	: 6 ;	//filler bits
	unsigned char   m_fillerbyte		;	//filler bytes
};

struct SHoldemState 
{
	char			m_title[64]		 ;	//table title
	double			m_pot[10]		   ;	//total in each pot

	unsigned char   m_cards[5]		  ;	//common cards

	unsigned char   m_is_playing	: 1 ;	//0=sitting-out, 1=sitting-in
	unsigned char   m_is_posting	: 1 ;	//0=autopost-off, 1=autopost-on
	unsigned char   m_fillerbits	: 6 ;	//filler bits

	unsigned char   m_fillerbyte		;	//filler byte
	unsigned char   m_dealer_chair	  ;	//0-9

	SHoldemPlayer   m_player[10]		;	//player records
};


extern class CDllExtension 
{
public:
	// public functions
	CDllExtension();
	~CDllExtension();
	void PassStateToDll(const SHoldemState *pstate);
	CString GetDLLSpecifiedInFormula();
	void LoadDll(const char * path);
	void UnloadDll(void);
	const bool IsDllLoaded();

public:
	// public accessors
	const process_message_t	process_message() { return _process_message; }

private:
	// private variables - use public accessors and public mutators to address these
	HMODULE				_hmod_dll;

private:
	// private functions and variables - not available via accessors or mutators
	process_message_t	_process_message;

} *p_dll_extension;


double GetSymbolFromDll(const int chair, const char* name, bool& iserr);


#endif /* INC_CDLLEXTENSION_H */
