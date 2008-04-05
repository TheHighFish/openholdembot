#ifndef INC_POKERPRO_H
#define INC_POKERPRO_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <wspiapi.h>	// kludge for Win2k/VS2003+, per post here: http://blogs.msdn.com/seshadripv/archive/2005/10/21/483408.aspx
#include "ppmessage.h"

#define MAX_TABLES	10
#define MAX_PLAYERS 10

#define PUBLISH_ERR		(1<<0)

struct ppdata {
	char	m_site_name[32];
	GINF	m_ginf[MAX_TABLES];
	TINF	m_tinf;
	PINF	m_pinf[MAX_PLAYERS];
	int		m_location;
	int		m_isauthenticated;
	int		m_userchair;
	int		m_handnumber;
	int		m_pot[10];
	int		m_prev_rais;
	int		m_isconnected;
};

extern class PokerPro {
public:
	PokerPro(void);
	~PokerPro(void);

	int connect(const char *host, const char * port);
	int disconnect(void);
	int send_login(const char* username, const char* password);
	int send_version(void);
	int send_goto(int location);
	int send_sit(int chair = -1);
	int send_stand(int chair = -1);
	int send_chips(double chips);
	int send_deposit(double chips);
	int send_sitin(int chair);
	int send_sitout(int chair);
	int send_action(int action, int amount);
	void DoScrape(void);
	void DoAutoplayer(void);
	int get_random_vacant_chair(void);
	int recv_message(const char** ppbytes, int* pnbytes);
	void handle_message(const char* pbytes, int nbytes);
	void reset_hand(void);

	SOCKET		m_socket;
	CString		hostname, port, username, password;
	bool		handhistory;
	int			table, chips;
	bool		autoseat;
	ppdata		data;

private:
	int resolve (const char* host, const char* port, addrinfo** ppai);
	void nolinger (SOCKET s);
	int send_message(const char* pbytes, int nbytes);
	int sendbuffer(const char* pbytes, int nbytes);
	int recvbuffer (char* pbytes, int nbytes);
	int send(const char* bytes, int nbytes);
	int recv(char* bytes, int nbytes);

	void HandleEventUnknown(const char* pbytes);
	void HandleEventUserChair			(PokerMessage* pm);
	void HandleEventPlayerPlaying		(PokerMessage* pm);
	void HandleEventPlayerNotPlaying	(PokerMessage* pm);
	void HandleEventPlayerActive		(PokerMessage* pm);
	void HandleEventPlayerInactive		(PokerMessage* pm);
	void HandleEventPlayerSit			(PokerMessage* pm);
	void HandleEventPlayerUnSit			(PokerMessage* pm);
	void HandleEventSiteName			(PokerMessage* pm);
	void HandleEventTableInfo			(PokerMessage* pm);
	void HandleEventGameInfo			(PokerMessage* pm);
	void HandleEventGoto				(PokerMessage* pm);
	void HandleEventVersion				(PokerMessage* pm);
	void HandleEventPlayerLogout		(PokerMessage* pm);
	void HandleEventPlayerLogin			(PokerMessage* pm);
	void HandleEventPlayerLoginReject	(PokerMessage* pm);
	void HandleEventPlayerInfo			(PokerMessage* pm);
	void HandleEventWaitPlayer			(PokerMessage* pm);
	void HandleEventWPDS				(PokerMessage* pm);
	void HandleEventPlayerFold			(PokerMessage* pm);
	void HandleEventPlayerMuck			(PokerMessage* pm);
	void HandleEventPlayerRefund		(PokerMessage* pm);
	void HandleEventPlayerBet			(PokerMessage* pm);
	void HandleEventPlayerName			(PokerMessage* pm);
	void HandleEventPlayerBalance		(PokerMessage* pm);
	void HandleEventPlayerWin			(PokerMessage* pm);
	void HandleEventNextDealer			(PokerMessage* pm);
	void HandleEventNextPlayer			(PokerMessage* pm);
	void HandleEventNextRound			(PokerMessage* pm);
	void HandleEventNextHand			(PokerMessage* pm);
	void HandleEventNextGame			(PokerMessage* pm);
	void HandleEventNextTable			(PokerMessage* pm);
	void HandleEventSetPot				(PokerMessage* pm);
	void HandleEventTotalPot			(PokerMessage* pm);
	void HandleEventStakes				(PokerMessage* pm);
	void HandleEventEndHand				(PokerMessage* pm);
	void HandleEventEndTable			(PokerMessage* pm);
	void HandleEventDeal				(PokerMessage* pm);
	void HandleEventPlayerChat			(PokerMessage* pm);
	void HandleEventPokerValue			(PokerMessage* pm);
	void HandleEventRevealCard			(PokerMessage* pm);
	void HandleEventFlop				(PokerMessage* pm);
	void HandleEventPlayerList			(PokerMessage* pm);
	void HandleAccount					(PokerMessage* pm);
	void HandleEventStarted				(PokerMessage* pm);
	void HandleEventStopped				(PokerMessage* pm);
	void HandleEventTournament			(PokerMessage* pm);
	void HandleEventTableLock			(PokerMessage* pm);
	void HandleEventPing				(PokerMessage* pm);
	char* sassign(char* dst, const char* src, int dstsiz);
	char* convert_money(char* text, int amount);
	int get_current_bet(void);
	int get_betx(void);
	int get_round(void);
	int get_ncc(void);
	int get_npd(void);
	void publish(CString *text, int flags);
	const char* get_action(int chair, int spend);
	int get_player_pot(void);
	char* card2ascii(char* text, unsigned char cc);
	char rank2ascii(int rank);
	void writehh(CString *s);
	const char* get_cardinal_suffix(int n);
	int count_same_scrapes(void);                        // Added to handle delay  (4-3-2008)  Spektre 

	WSADATA		m_wsadata;
	char		m_data[ppsock__nmsgmax][ppsock__msgsizmax];
	int			m_nmsg;
	bool		autoplayer_can_act;

} ppro;


#endif /* INC_POKERPRO_H */