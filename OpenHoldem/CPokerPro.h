#ifndef INC_POKERPRO_H
#define INC_POKERPRO_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <wspiapi.h>	// kludge for Win2k/VS2003+, per post here: http://blogs.msdn.com/seshadripv/archive/2005/10/21/483408.aspx
#include "ppmessage.h"
#include "..\CCritSec\CCritSec.h"

#define MAX_TABLES	10
#define MAX_PLAYERS 10

#define PUBLISH_ERR		(1<<0)

struct SPPData 
{
	char	m_site_name[32];
	GINF	m_ginf[MAX_TABLES];
	TINF	m_tinf;
	PINF	m_pinf[MAX_PLAYERS];
	int		m_location;
	int		m_isauthenticated;
	int		m_userchair;
	int		m_handnumber;
	int		m_pot[10];
	int		m_total_pot;
	int		m_prev_rais;
	int		m_isconnected;
};

extern class PokerPro 
{
public:
	// public functions
	PokerPro(void);
	~PokerPro(void);
	int Connect(const char *host, const char * port);
	int Disconnect(void);
	const int SendLogin(const char* username, const char* password);
	const int SendVersion(void);
	const int SendGoto(const int location);
	const int SendSit(const int chair = -1);
	const int SendStand(const int chair = -1);
	const int SendChips(const double chips);
	const int SendDeposit(const double chips);
	const int SendSitin(const int chair);
	const int SendSitout(const int chair);
	const int SendAction(const int action, const int amount);
	void DoScrape(void);
	void DoAutoplayer(void);
	const int GetRandomVacantChair(void);
	int ReceivePPMessage(const char** ppbytes, int* pnbytes);
	void HandlePPMessage(const char* pbytes, const int nbytes);
	void ResetHand(void);
	const bool IsConnected(void);

public:
	// public accessors
	const SPPData * ppdata() { return &_ppdata; }
	SOCKET socket() { return _socket; }

public:
#define ENT CSLock lock(m_critsec);
	// public mutators
	void set_ppdata_clear() { ENT memset(&_ppdata, 0, sizeof(SPPData)); }
	void set_ppdata_reset_userchair() { ENT _ppdata.m_userchair = -1; }
	void set_ppdata_tinf_reset() { ENT memset(&_ppdata.m_tinf, 0, sizeof(TINF)); }
	void set_ppdata_tinf_tid_reset() { ENT _ppdata.m_tinf.m_tid = 0; }
#undef ENT

private:
	// private variables - use public accessors and public mutators to address these	
	SOCKET		_socket;
	int			_table;
	SPPData		_ppdata;
	CString m_ftp_filename;

private:
	// private functions and variables - not available via accessors or mutators	
	int Resolve (const char* host, const char* port, addrinfo** ppai);
	void NoLinger (SOCKET s);
	int SendPPMessage(const char* pbytes, const int nbytes);
	int SendBuffer(const char* pbytes, const int nbytes);
	int ReceiveBuffer (char* pbytes, const int nbytes);
	int Send(const char* bytes, const int nbytes);
	int Receive(char* bytes, const int nbytes);
	void HandleEventUnknown(const char* pbytes);
	void HandleEventUserChair			(const PokerMessage* pm);
	void HandleEventPlayerPlaying		(const PokerMessage* pm);
	void HandleEventPlayerNotPlaying	(const PokerMessage* pm);
	void HandleEventPlayerActive		(const PokerMessage* pm);
	void HandleEventPlayerInactive		(const PokerMessage* pm);
	void HandleEventPlayerSit			(const PokerMessage* pm);
	void HandleEventPlayerUnSit			(const PokerMessage* pm);
	void HandleEventSiteName			(const PokerMessage* pm);
	void HandleEventTableInfo			(const PokerMessage* pm);
	void HandleEventGameInfo			(const PokerMessage* pm);
	void HandleEventGoto				(const PokerMessage* pm);
	void HandleEventVersion				(const PokerMessage* pm);
	void HandleEventPlayerLogout		(const PokerMessage* pm);
	void HandleEventPlayerLogin			(const PokerMessage* pm);
	void HandleEventPlayerLoginReject	(const PokerMessage* pm);
	void HandleEventPlayerInfo			(const PokerMessage* pm);
	void HandleEventWaitPlayer			(const PokerMessage* pm);
	void HandleEventWPDS				(const PokerMessage* pm);
	void HandleEventPlayerFold			(const PokerMessage* pm);
	void HandleEventPlayerMuck			(const PokerMessage* pm);
	void HandleEventPlayerRefund		(const PokerMessage* pm);
	void HandleEventPlayerBet			(const PokerMessage* pm);
	void HandleEventPlayerName			(const PokerMessage* pm);
	void HandleEventPlayerBalance		(const PokerMessage* pm);
	void HandleEventPlayerWin			(const PokerMessage* pm);
	void HandleEventNextDealer			(const PokerMessage* pm);
	void HandleEventNextPlayer			(const PokerMessage* pm);
	void HandleEventNextRound			(const PokerMessage* pm);
	void HandleEventNextHand			(const PokerMessage* pm);
	void HandleEventNextGame			(const PokerMessage* pm);
	void HandleEventNextTable			(const PokerMessage* pm);
	void HandleEventSetPot				(const PokerMessage* pm);
	void HandleEventTotalPot			(const PokerMessage* pm);
	void HandleEventStakes				(const PokerMessage* pm);
	void HandleEventEndHand				(const PokerMessage* pm);
	void HandleEventEndTable			(const PokerMessage* pm);
	void HandleEventDeal				(const PokerMessage* pm);
	void HandleEventPlayerChat			(const PokerMessage* pm);
	void HandleEventPokerValue			(const PokerMessage* pm);
	void HandleEventRevealCard			(const PokerMessage* pm);
	void HandleEventFlop				(const PokerMessage* pm);
	void HandleEventPlayerList			(const PokerMessage* pm);
	void HandleAccount					(const PokerMessage* pm);
	void HandleEventStarted				(const PokerMessage* pm);
	void HandleEventStopped				(const PokerMessage* pm);
	void HandleEventTournament			(const PokerMessage* pm);
	void HandleEventTableLock			(const PokerMessage* pm);
	void HandleEventPing				(PokerMessage* pm);
	char* SAssign(char* dst, const int dst_size, const char* src, const int dstsiz);
	char* ConvertMoney(char* text, const int dst_size, const int amount);
	const int GetCurrentBet(void);
	const int GetBetX(void);
	const int GetRound(void);
	const int GetNcc(void);
	const int GetNpd(void);
	void Publish(CString *text, const int flags);
	const char* GetAction(const int chair, const int spend);
	const int GetPlayerPot(void);
	char* Card2Ascii(char* text, const unsigned char cc);
	char Rank2Ascii(const int rank);
	void WriteHH(const CString *s);
	void writeFThh(const CString *s);
	const char* GetCardinalSuffix(const int n);
	void setHHFilename(void);
	const CString FT_string_PokerVal(int in_hand_value);
	const CString FT_FullCardName(unsigned char in_card_value);
	const int CountSameScrapes(void);						// Added to handle delay  (4-3-2008)  Spektre 

	WSADATA		_wsadata;
	char		_data[ppsock__nmsgmax][ppsock__msgsizmax];
	int			_nmsg;
	bool		_autoplayer_can_act;

	CCritSec	m_critsec;

} *p_pokerpro;

#endif /* INC_POKERPRO_H */
