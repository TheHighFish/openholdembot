#include "StdAfx.h"

#include <string.h>
#include <time.h>

#include "CAutoplayer.h"
#include "CGrammar.h"
#include "CHeartbeatThread.h"
#include "CIteratorThread.h"
#include "CPokerPro.h"
#include "CPreferences.h"
#include "CScraper.h"
#include "CSessionCounter.h"
#include "CStableFramesCounter.h"
#include "CSymbols.h"
#include "DialogPPro.h"
#include "OpenHoldem.h"


PokerPro		*p_pokerpro = NULL;

PokerPro::PokerPro(void) 
{
	// init
	memset(&_ppdata, 0, sizeof(SPPData));
	_ppdata.m_userchair = -1;
	_autoplayer_can_act = false;

	::WSAStartup( MAKEWORD(2,2), &_wsadata );
	_socket = INVALID_SOCKET;
}


PokerPro::~PokerPro(void) 
{
	::WSACleanup();
}

int PokerPro::Connect(const char *host, const char * port) 
{
	if (_socket != INVALID_SOCKET) 
	{
		return -1;
	}

	int iresult = 0;

	addrinfo*	pai = NULL;

	if (Resolve(host, port, &pai) == 0) 
	{
		_socket = INVALID_SOCKET;
		return -2;
	}

	if ((_socket = ::socket(pai->ai_family, pai->ai_socktype, pai->ai_protocol)) != INVALID_SOCKET) 
	{
		iresult = ::connect(_socket, pai->ai_addr, pai->ai_addrlen);
	}

	if (pai) 
	{
		::freeaddrinfo(pai);
	}

	if (_socket == INVALID_SOCKET) 
	{
		return -3;
	}

	if (iresult == SOCKET_ERROR) 
	{
		_socket = INVALID_SOCKET;
		return -4;
	}

	NoLinger(_socket);

	return 1;
}

int PokerPro::Disconnect(void) 
{
	::shutdown(_socket,SD_BOTH);
	::closesocket( _socket );
	_socket = INVALID_SOCKET;
	_ppdata.m_isauthenticated = false;
	return 1;
}

const int PokerPro::SendLogin(const char* username, const char* password) 
{
	PokerMessage pm;
	pm.m_event = 'LOGI';
	pm.m_chair = -1;
	SAssign(pm.m_name, 16, username, sizeof(pm.m_name));
	SAssign(pm.m_pass, 16, password, sizeof(pm.m_pass));
	return SendPPMessage((char*)(&pm), sizeof(PokerMessage));
}

const int PokerPro::SendVersion(void) 
{
	PokerMessage pm;
	pm.m_event = 'VERS';
	pm.m_version = PROTOCOL_VERSION;
	return SendPPMessage((char*)(&pm), sizeof(PokerMessage));
}

const int PokerPro::SendGoto(const int location) 
{
	PokerMessage pm;
	pm.m_event		= 'GOTO';
	pm.m_location	= location;
	return SendPPMessage((char*)(&pm), sizeof(PokerMessage));
}

const int PokerPro::SendSit(const int chair) 
{
	PokerMessage pm;
	pm.m_event = 'PSIT';
	pm.m_chair = (chair == -1) ? _ppdata.m_userchair : chair;
	setHHFilename();
	return SendPPMessage((char*)(&pm), sizeof(PokerMessage));
}

const int PokerPro::SendStand(const int chair) 
{
	PokerMessage pm;
	pm.m_event = 'PUNS';
	pm.m_chair = (chair == -1) ? _ppdata.m_userchair : chair;
	return SendPPMessage((char*)(&pm), sizeof(PokerMessage));
}

const int PokerPro::SendChips(const double chips) 
{
	CHAT_MESSAGE	m;
	char			msg[100] = {0};
	sprintf_s(msg, 100, "/chips %.2f", chips);

	memset(&m,0,sizeof(m));

	m.m_tag='CHAT';

	if (_ppdata.m_userchair >= 0)
		SAssign(m.m_name, 16, _ppdata.m_pinf[_ppdata.m_userchair].m_name, sizeof(m.m_name));
	m.m_name[sizeof(m.m_name)-1]=0;

	SAssign(m.m_text, 236, msg, sizeof(m.m_text));
	m.m_text[sizeof(m.m_text)-1]=0;

	int nbytes = (int) strlen(m.m_text);

	nbytes += sizeof(m.m_tag);
	nbytes += sizeof(m.m_name);
	if (nbytes < sizeof(CHAT_MESSAGE))
		nbytes++;
	if (nbytes < sizeof(PokerMessage))
		nbytes = sizeof(PokerMessage);

	int result = SendPPMessage( (char*)(&m), nbytes );
	return result;
}

const int PokerPro::SendDeposit(const double chips) 
{
	CHAT_MESSAGE	m;
	char			msg[100] = {0};
	sprintf_s(msg, 100, "/bank deposit %.2f", chips);

	memset(&m,0,sizeof(m));

	m.m_tag='CHAT';

	if (_ppdata.m_userchair >= 0)
		SAssign(m.m_name, 16, _ppdata.m_pinf[_ppdata.m_userchair].m_name, sizeof(m.m_name));
	m.m_name[sizeof(m.m_name)-1]=0;

	SAssign(m.m_text, 236, msg, sizeof(m.m_text));
	m.m_text[sizeof(m.m_text)-1]=0;

	int nbytes = (int) strlen(m.m_text);

	nbytes += sizeof(m.m_tag);
	nbytes += sizeof(m.m_name);
	if (nbytes < sizeof(CHAT_MESSAGE))
		nbytes++;
	if (nbytes < sizeof(PokerMessage))
		nbytes = sizeof(PokerMessage);

	int result = SendPPMessage( (char*)(&m), nbytes );
	return result;
}

const int PokerPro::SendSitin(const int chair)
{
	PokerMessage pm;
	pm.m_event = 'PACT';
	pm.m_chair = (chair == -1) ? _ppdata.m_userchair : chair;
	return SendPPMessage((char*)(&pm), sizeof(PokerMessage));
}

const int PokerPro::SendSitout(const int chair) 
{
	PokerMessage pm;
	pm.m_event = 'PINA';
	pm.m_chair = (chair == -1) ? _ppdata.m_userchair : chair;
	return SendPPMessage((char*)(&pm), sizeof(PokerMessage));
}

const int PokerPro::SendAction(const int action, const int amount) 
{
	PokerMessage pm;
	pm.m_event = action;
	pm.m_chair = _ppdata.m_userchair;
	pm.m_amount = amount;
	return SendPPMessage((char*)(&pm), sizeof(PokerMessage));
}

int PokerPro::Resolve (const char* host, const char* port, addrinfo** ppai) 
{
	addrinfo	hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	addrinfo *pai = NULL;

	int iresult = ::getaddrinfo( host, port, &hints, &pai );

	if (iresult != 0) 
	{
		return false;
	}

	if (pai) 
	{
		*ppai = pai;
	}

	return true;
}


void PokerPro::NoLinger (SOCKET s) 
{
	linger  _linger;
	_linger.l_onoff		 = 0;	//0=dontlinger  1=linger
	_linger.l_linger		= 0;	//nseconds to linger

	setsockopt
	(
		s,
		SOL_SOCKET,
		SO_LINGER,
		(char *)&_linger,
		sizeof(_linger)
	);
}

const int PokerPro::GetRandomVacantChair(void) 
{
	int rc = -1;

	if (_ppdata.m_location > 0)	
	{
		int vacant[10];
		int nvacant=0;
		for (int i=0; i<10; i++) 
		{
			if (_ppdata.m_pinf[i].m_isSeated&0x1) 
			{
				continue;
			}
			vacant[nvacant++] = i;
		}
		if (nvacant) 
		{
			rc = vacant[rand()%nvacant];
		}
	}
	return rc;
}

int PokerPro::SendPPMessage(const char* pbytes, const int nbytes) 
{
	int result = 0;

	if (nbytes > ppsock__msgsizmin)	
	{
		char msg[ppsock__msgsizmin];
		long* plong = ((long*)(&msg[0]));

		plong[0] = 'yasm';
		plong[1] = nbytes;

		result = SendBuffer(msg, sizeof(msg));
	}

	result = SendBuffer(pbytes, nbytes);

	return result;
}

int PokerPro::ReceivePPMessage(const char** ppbytes, int* pnbytes) 
{
	int nbytes = ppsock__msgsizmin;

	_nmsg += 1;
	_nmsg %= ppsock__nmsgmax;
	char* pbytes = &(_data[_nmsg][0]);

	int result = ReceiveBuffer(pbytes, nbytes);
	if (result == 0) 
	{
		return 0;
	}

	long* plong = ((long*)pbytes);

	if (plong[0] == 'yasm') 
	{
		//this is a special yas message
		nbytes = plong[1];

		if (nbytes > ppsock__msgsizmax) 
		{
			return 0;
		}

		result = ReceiveBuffer(pbytes, nbytes);

		if (result == 0) 
		{
			return 0;
		}
	}

	*ppbytes = pbytes;
	*pnbytes = nbytes;

	return 1;
}


int PokerPro::SendBuffer(const char* pbytes, const int nbytes) 
{
	int nbytes_sent=0;

	while (nbytes_sent < nbytes) 
	{
		int iresult = Send(pbytes+nbytes_sent, nbytes-nbytes_sent);

		if (iresult == SOCKET_ERROR) 
		{
			return SOCKET_ERROR;
		}

		nbytes_sent += iresult;
	}
	return nbytes_sent;
}

int PokerPro::ReceiveBuffer (char* pbytes, const int nbytes) 
{
	int nbytesrecv = 0;

	while (nbytesrecv < nbytes) 
	{
		int nrecv = Receive(pbytes+nbytesrecv, nbytes-nbytesrecv);

		if (nrecv < 1) 
		{
			return 0;	//closed or error
		}

		nbytesrecv += nrecv;
	}

	return nbytesrecv;
}

int PokerPro::Send(const char* bytes, const int nbytes) 
{
	return ::send( _socket, bytes, nbytes, 0 );
}

int PokerPro::Receive(char* bytes, const int nbytes) 
{
	return ::recv( _socket, bytes, nbytes, 0 );
}

void PokerPro::HandlePPMessage(const char* pbytes, const int nbytes) 
{
	PokerMessage* pm;

	pm = (PokerMessage*)pbytes;

	switch (pm->m_event) 
	{

	case 'WPA'	:
	case 'WPSB'	:
	case 'WPBB'	:
	case 'WPB'	:
	case 'WPR'	:
		HandleEventWaitPlayer			(pm);
		break;

	case 'WPDS'	:
		HandleEventWPDS				(pm);
		break;

	case 'PBET'	:
	case 'BBLI'	:
	case 'SBLI'	:
	case 'ANTE'	:
		HandleEventPlayerBet			(pm);
		break;

	case 'FOLD'	:
		HandleEventPlayerFold			(pm);
		break;
	case 'MUCK'	:
		HandleEventPlayerMuck			(pm);
		break;
	case 'PREF'	:
		HandleEventPlayerRefund		(pm);
		break;
	case 'PBAL'	:
		HandleEventPlayerBalance		(pm);
		break;
	case 'PACT'	:
		HandleEventPlayerActive		(pm);
		break;
	case 'PINA'	:
		HandleEventPlayerInactive		(pm);
		break;
	case 'PUNS'	:
		HandleEventPlayerUnSit		(pm);
		break;
	case 'PSIT'	:
		HandleEventPlayerSit			(pm);
		break;
	case 'PINF'	:
		HandleEventPlayerInfo			(pm);
		break;
	case 'PWIN'	:
		HandleEventPlayerWin			(pm);
		break;
	case 'PNAM'	:
		HandleEventPlayerName			(pm);
		break;
	case 'PLIS'	:
		HandleEventPlayerList			(pm);
		break;
	case 'PNPL'	:
		HandleEventPlayerNotPlaying	(pm);
		break;
	case 'PPLA'	:
		HandleEventPlayerPlaying		(pm);
		break;

	case 'CHAT'	:
		HandleEventPlayerChat			(pm);
		break;

	case 'LIR0'	:
		HandleEventPlayerLoginReject	(pm);
		break;
	case 'LOGO'	:
		HandleEventPlayerLogout		(pm);
		break;
	case 'LOGI'	:
		HandleEventPlayerLogin		(pm);
		break;

	case 'NEXH'	:
		HandleEventNextHand			(pm);
		break;
	case 'NEXT'	:
		HandleEventNextTable			(pm);
		break;
	case 'NEXG'	:
		HandleEventNextGame			(pm);
		break;
	case 'NEXR'	:
		HandleEventNextRound			(pm);
		break;
	case 'NEXD'	:
		HandleEventNextDealer			(pm);
		break;
	case 'NEXP'	:
		HandleEventNextPlayer			(pm);
		break;

	case 'FLOP'	:
	case 'DCCU'	:
		HandleEventFlop				(pm);
		break;

	case 'RCRD'	:
		HandleEventRevealCard			(pm);
		break;
	case 'PVAL'	:
		HandleEventPokerValue			(pm);
		break;
	case 'DEAL'	:
		HandleEventDeal				(pm);
		break;
	case 'GINF'	:
		HandleEventGameInfo			(pm);
		break;
	case 'GOTO'	:
		HandleEventGoto				(pm);
		break;
	case 'SNAM'	:
		HandleEventSiteName			(pm);
		break;
	case 'STAK'	:
		HandleEventStakes				(pm);
		break;
	case 'SPOT'	:
		HandleEventSetPot				(pm);
		break;
	case 'TPOT'	:
		HandleEventTotalPot			(pm);
		break;
	case 'TINF'	:
		HandleEventTableInfo			(pm);
		break;
	case 'UCHA'	:
		HandleEventUserChair			(pm);
		break;
	case 'VERS'	:
		HandleEventVersion			(pm);
		break;

	case 'EEOG'	:
		HandleEventEndHand			(pm);
		break;
	case 'ENDT'	:
		HandleEventEndTable			(pm);
		break;

	case 'STAR'	:
		HandleEventStarted			(pm);
		break;
	case 'STOP'	:
		HandleEventStopped			(pm);
		break;

	case 'REGI'	:
	case 'RER0'	:
	case 'RER1'	:
	case 'RER2'	:
	case 'PWR0'	:
	case 'PASS'	:
	case 'PWR1'	:
		HandleAccount					(pm);
		break;

	case 'PING'	:
		HandleEventPing				(pm);
		break;

	case 'TMOD' :
	case '+TM'  :
	case '-TM'  :
	case 'TLOS' :
		HandleEventTournament			(pm);
		break;
		
	case 'TLCK' :
		MessageBox(0, "Message 'TLCK' received'\n."
			"'TLCK' is undocumented and thus not supported.\n"
			"If you want to simulate tournaments,\n" 
			"please use an older version of the PPro-server\n" 
			"according to the manual.\n",
			"PokerPro-Error", 0);
		break;

	default		:
		HandleEventUnknown		(pbytes);
		break;
	}
}


void PokerPro::HandleEventUnknown(const char* pbytes) 
{
	char stag[8] = {0};
	stag[0] = pbytes[3];
	stag[1] = pbytes[2];
	stag[2] = pbytes[1];
	stag[3] = pbytes[0];
	stag[4] = 0;

	CString s = "";
	s.Format("Unknown message received (%s)...\n", stag);
	Publish(&s, PUBLISH_ERR);
}


void PokerPro::HandleEventUserChair(const PokerMessage* pm) 
{
	_ppdata.m_userchair = pm->m_chair;
}

void PokerPro::HandleEventPlayerPlaying(const PokerMessage* pm) 
{
	_ppdata.m_pinf[pm->m_chair&0x1f].m_isPlaying = 1;
}

void PokerPro::HandleEventPlayerNotPlaying(const PokerMessage* pm) 
{
	_ppdata.m_pinf[pm->m_chair&0x1f].m_isPlaying = 0;
}

void PokerPro::HandleEventPlayerActive(const PokerMessage* pm) 
{
	_ppdata.m_pinf[pm->m_chair&0x1f].m_isActive = 1;

	if (pm->m_chair == _ppdata.m_userchair) 
	{
		CString s = "";
		s.Format("Sitting in.\n");
		Publish(&s, 0);
	}
}

void PokerPro::HandleEventPlayerInactive(const PokerMessage* pm) 
{
	_ppdata.m_pinf[pm->m_chair&0x1f].m_isActive = 0;

	if (pm->m_chair == _ppdata.m_userchair) 
	{
		CString s = "";
		s.Format("Sitting out.\n");
		Publish(&s, 0);
	}
}

void PokerPro::HandleEventPlayerSit(const PokerMessage* pm) 
{
	_ppdata.m_pinf[pm->m_chair&0x1f].m_isSeated = 1;
	CString s = "";
	s.Format("Chair %d joining table.\n", pm->m_chair&0x1f);
	Publish(&s, 0);
}

void PokerPro::HandleEventPlayerUnSit(const PokerMessage* pm) {

	_ppdata.m_pinf[pm->m_chair&0x1f].m_isSeated = 0;
	if (pm->m_chair == _ppdata.m_userchair) 
	{
		_ppdata.m_userchair = -1;
	}
	CString s = "";
	s.Format("Chair %d leaving table.\n", pm->m_chair&0x1f);
	Publish(&s, 0);
}

void PokerPro::HandleEventSiteName(const PokerMessage* pm) 
{
	SAssign(_ppdata.m_site_name, 32, pm->m_text, sizeof(_ppdata.m_site_name));
	CString s = "";
	s.Format("%s\n", pm->m_text);
	Publish(&s, 0);
}

void PokerPro::HandleEventTableInfo(const PokerMessage* pm) 
{
	TINF* ptinf = (TINF*)pm;
	_ppdata.m_tinf = *ptinf;
}

void PokerPro::HandleEventGameInfo(const PokerMessage* pm) 
{
	GINF* pginf = (GINF*)pm;
	if (pginf->m_tid<=0 || pginf->m_tid>=MAX_TABLES) 
	{
		return;
	}
	_ppdata.m_ginf[pginf->m_tid] = *pginf;

	char sbs[32];
	char bbs[32];
	char BBs[32];
	ConvertMoney( sbs, 32, pginf->m_sblind );
	ConvertMoney( bbs, 32, pginf->m_minbet );
	ConvertMoney( BBs, 32, pginf->m_maxbet );

	char tL[2] = "*";
	if ((pginf->m_tmode&0x1) && pginf->m_tlevel) 
	{
		tL[0] = 'A' + pginf->m_tlevel - 1;
	}

	CString s = "";
	s.Format("%2d %s %s %s %-15s %2d/%2d %5s %5s %5s\n",
			 pginf->m_tid,
			 pginf->m_moneyid==0 ? "-" : "$",
			 ((pginf->m_lim&0x7)==0 ? "N" : (pginf->m_lim&0x7)==1 ? "P" : "L"),
			 (pginf->m_tmode&0x1) ? tL : "-",
			 pginf->m_name,
			 pginf->m_nSeated, pginf->m_nChairs,
			 sbs, bbs, BBs);
	Publish(&s, 0);
}

void PokerPro::HandleEventGoto(const PokerMessage* pm) 
{
	_ppdata.m_location = pm->m_location;
	CString s = "";
	s.Format("Joined table %d.\n", pm->m_location);
	Publish(&s, 0);
}

void PokerPro::HandleEventVersion(const PokerMessage* pm) 
{
}

void PokerPro::HandleEventPlayerLogout(const PokerMessage* pm) 
{
	_ppdata.m_isauthenticated = false;
	CString s = "";
	s.Format("Logout complete.\n");
	Publish(&s, 0);
}

void PokerPro::HandleEventPlayerLogin(const PokerMessage* pm) 
{
	_ppdata.m_isauthenticated = true;
	CString s = "";
	s.Format("Login complete.\n");
	Publish(&s, 0);
}

void PokerPro::HandleEventPlayerLoginReject(const PokerMessage* pm) 
{
	_ppdata.m_isauthenticated = false;
	CString s = "";
	s.Format("Login rejected.\n");
	Publish(&s, 0);
}

void PokerPro::HandleEventPlayerInfo(const PokerMessage* pm) 
{
	PINF* ppinf = (PINF*)pm;
	_ppdata.m_pinf[ppinf->m_chair&0x1f] = *ppinf;

	char sbal[32];
	char sbet[32];
	ConvertMoney(sbal, 32, ppinf->m_balance);
	ConvertMoney(sbet, 32, ppinf->m_betAmount);

	CString s = "";
	s.Format("%d %15s %s %s\n", ppinf->m_chair&0x1f, ppinf->m_name, sbal, sbet);
	Publish(&s, 0);
}

void PokerPro::HandleEventWPDS(const PokerMessage* pm) 
{
	PokerMessage PM;
	PM.m_event = pm->m_event;			//'WPDS'
	PM.m_lparam[1] = pm->m_lparam[1];	//hand number

	// generate 52 random numbers (0-51)
	for (int i=0; i<=51; i++) {
		unsigned char r = (unsigned char)(rand() % 52);
		PM.m_cparam[8+i] = r;
	}
	SendPPMessage((char*)(&PM), sizeof(PokerMessage));
}

void PokerPro::HandleEventWaitPlayer(const PokerMessage* pm) 
{
	int tag = pm->m_event;
	int action = 0;
	int amount = 0;
	CString s = "";

	if ((_ppdata.m_tinf.m_activePlayer&0xf) != _ppdata.m_userchair) 
	{
		return;
	}

	switch (tag) 
	{
	case 'WPA'	:	// wait player small blind
		action = 'ANTE';
		break;
	case 'WPSB'	:	// wait player small blind
		action = 'SBLI';
		break;
	case 'WPBB'	:	// wait player big blind
		action = 'BBLI';
		break;
	case 'WPR'	:	// wait player reveal
		action = 'MUCK';
		//action = 'SHOW';
		break;
	case 'WPB'	:	// wait player bet
		//action = get_decision( amount );
		_autoplayer_can_act = true;
		break;
	}

	SendAction(action, amount);
}

void PokerPro::HandleEventPlayerFold(const PokerMessage* pm) 
{
	CString s = "";

	_ppdata.m_pinf[pm->m_chair&0x1f].m_card[0] = 0;
	_ppdata.m_pinf[pm->m_chair&0x1f].m_card[1] = 0;

	s.Format("Chair %d (%s) folds.\n", pm->m_chair&0x1f, _ppdata.m_pinf[pm->m_chair&0x1f].m_name);
	Publish(&s, 0);

	if (prefs.ppro_ft_hhlog_format()  == false) {

		s.Format("%s FOLD\n", _ppdata.m_pinf[pm->m_chair&0x1f].m_name);
		WriteHH(&s);

	}

	else {

		s.Format("%s folds\n", _ppdata.m_pinf[pm->m_chair&0x1f].m_name);
		writeFThh(&s);

	}

}

void PokerPro::HandleEventPlayerMuck(const PokerMessage* pm) 
{
	CString s = "";

	_ppdata.m_pinf[pm->m_chair&0x1f].m_card[0] = 0;
	_ppdata.m_pinf[pm->m_chair&0x1f].m_card[1] = 0;
	s.Format("Chair %d (%s) mucks.\n", pm->m_chair&0x1f, _ppdata.m_pinf[pm->m_chair&0x1f].m_name);
	Publish(&s, 0);

	if (prefs.ppro_ft_hhlog_format()  == false) {

		s.Format("%s MUCK\n", _ppdata.m_pinf[pm->m_chair&0x1f].m_name);
		WriteHH(&s);

	}

	else {

		s.Format("%s mucks\n", _ppdata.m_pinf[pm->m_chair&0x1f].m_name);
		writeFThh(&s);

	}

}

void PokerPro::HandleEventPlayerRefund(const PokerMessage* pm) 
{
	CString s = "";

	char sbet[32] = {0};
	ConvertMoney(sbet, 32, pm->m_amount);
	s.Format("Chair %d (%s) refund %s.\n", pm->m_chair&0x1f, _ppdata.m_pinf[pm->m_chair&0x1f].m_name, sbet);
	Publish(&s, 0);

	if (prefs.ppro_ft_hhlog_format()  == false) {

		s.Format("%s RFND %s\n", _ppdata.m_pinf[pm->m_chair&0x1f].m_name, sbet);
		WriteHH(&s);

	}

	else {

		s.Format("Uncalled bet of $%s returned to %s\n", sbet, _ppdata.m_pinf[pm->m_chair&0x1f].m_name);
		writeFThh(&s);

	}


}

void PokerPro::HandleEventPlayerBet(const PokerMessage* pm) 
{
	CString s = "", hh = "";

	int tag = pm->m_event;
	int chair = pm->m_chair&0x1f;
	int after = pm->m_amount; // amount after spend
	int balance = _ppdata.m_pinf[chair].m_balance; // balance after spend
	int before = _ppdata.m_pinf[chair].m_betAmount;
	int spend = after - before;
	int currbet = GetCurrentBet();
	int call = currbet - before;
	int rais = spend - call;

	if (rais > _ppdata.m_prev_rais) 
	{
		_ppdata.m_prev_rais = rais;
	}

	const char* action = GetAction(chair, spend);
	const char* name = _ppdata.m_pinf[chair].m_name;

	_ppdata.m_pinf[chair].m_betAmount = after;

	char samt[32];
	char betAmt[32];
	char sbAmt[32];
	char bbAmt[32];

	ConvertMoney(samt, 32, spend);
	ConvertMoney(betAmt, 32, after);
	ConvertMoney(sbAmt, 32, _ppdata.m_tinf.m_betSBlind);
	ConvertMoney(bbAmt, 32, _ppdata.m_tinf.m_betMin);

	if (spend==0) {
		s.Format("Chair %d (%s) %s.\n", chair, name, action);
		if (prefs.ppro_ft_hhlog_format()  == false) {
			hh.Format("%s %s\n", name, action);
		}
		else {

			if (!std::strcmp(action,"SBLIND")) {
				hh.Format("%s posts the small blind of $%s\n", name, sbAmt);
			}
			if (!std::strcmp(action,"BBLIND")) {
				hh.Format("%s posts the big blind of $%s\n", name, bbAmt);
			}
			if (!std::strcmp(action,"CHECK")) {
				hh.Format("%s checks\n", name);
			}
			if (!std::strcmp(action,"CALL")) {
				hh.Format("%s calls $%s\n", name, samt);
			}
			if (!std::strcmp(action,"BET")) {
				hh.Format("%s bets $%s\n", name, betAmt);
			}
			if (!std::strcmp(action,"RAISE")) {
				hh.Format("%s raises to $%s\n", name, betAmt);
			}
		}
	}
	else if (_ppdata.m_pinf[chair].m_balance) {
		s.Format("Chair %d (%s) %s %s.\n", chair, name, action, samt);
		if (prefs.ppro_ft_hhlog_format()  == false) {
			hh.Format("%s %s %s\n", name, action, samt);
		}
		else {
			if (!std::strcmp(action,"SBLIND")) {
				hh.Format("%s posts the small blind of $%s\n", name, sbAmt);
			}
			if (!std::strcmp(action,"BBLIND")) {
				hh.Format("%s posts the big blind of $%s\n", name, bbAmt);
			}
			if (!std::strcmp(action,"CHECK")) {
				hh.Format("%s checks\n", name);
			}
			if (!std::strcmp(action,"CALL")) {
				hh.Format("%s calls $%s\n", name, samt);
			}
			if (!std::strcmp(action,"BET")) {
				hh.Format("%s bets $%s\n", name, betAmt);
			}
			if (!std::strcmp(action,"RAISE")) {
				hh.Format("%s raises to $%s\n", name, betAmt);
			}
		}
	}
	else {
		s.Format("Chair %d (%s) %s %s all in.\n", chair, name, action, samt);
		if (prefs.ppro_ft_hhlog_format()  == false) {
			hh.Format("%s %s %s ALLIN\n", name, action, samt );
		}
		else {
			if (!std::strcmp(action,"CALL")) {
				hh.Format("%s calls $%s, and is all in\n", name, samt);
			}
			if (!std::strcmp(action,"BET")) {
				hh.Format("%s bets $%s, and is all in\n", name, betAmt);
			}
			if (!std::strcmp(action,"RAISE")) {
				hh.Format("%s raises to $%s, and is all in\n", name, betAmt);
			}
		}
	}

	Publish(&s, 0);
	if (prefs.ppro_ft_hhlog_format() == false) {
		WriteHH(&hh);
	}
	else {
		writeFThh(&hh);
	}

}

void PokerPro::HandleEventPlayerName(const PokerMessage* pm) 
{
	SAssign(_ppdata.m_pinf[pm->m_chair&0x1f].m_name, 32, pm->m_name, sizeof(_ppdata.m_pinf[pm->m_chair&0x1f].m_name));
}

void PokerPro::HandleEventPlayerBalance(const PokerMessage* pm) 
{
	_ppdata.m_pinf[pm->m_chair&0x1f].m_balance = pm->m_amount;

	if (pm->m_chair == _ppdata.m_userchair) 
	{
		char sbal[32] = {0};
		ConvertMoney(sbal, 32, pm->m_amount);
		CString s = "";
		s.Format("%s's balance is now %s\n", _ppdata.m_pinf[pm->m_chair&0x1f].m_name, sbal);
		Publish(&s, 0);
	}
}

void PokerPro::HandleEventPlayerWin(const PokerMessage* pm) 
{
	CString s;

	int chairbits = pm->m_chair;
	int npot = pm->m_lparam[2];

	int nwin=0;
	for (int chair=0; chair<=9; chair++) {
		if (chairbits&(1<<chair)) {
			nwin++;
		}
	}
	int win = _ppdata.m_pot[npot]/nwin;
	_ppdata.m_pot[npot] = 0;

	char swin[32];
	ConvertMoney(swin,32, win);

	for (int i=0; i<=9; i++) {
		if (chairbits & (1<<i))	{
			_ppdata.m_pinf[i].m_betAmount = win;
			s.Format("%s wins %s.\n", _ppdata.m_pinf[i].m_name, swin);
			Publish(&s, 0);

			if (prefs.ppro_ft_hhlog_format()  == true) {
				unsigned char count_of_pots;
				unsigned char real_pot_number[10];
				CString pot_name;


				count_of_pots = 0;

				for (int j=0; j<=9; j++) {
					/*
					Clarification of the Below:
					On the 1st iteration, the real pot number is 0, 
					on the subsequent iterations it stays the same if the pot
					is equal to the preceding pot, (ie it is split) and goes up one otherwise.
					*/
					if (j>0){
						if (_ppdata.m_pot[j]==_ppdata.m_pot[j-1]){
							real_pot_number[j] = real_pot_number[j-1];
						}
						else {
							count_of_pots++;
							real_pot_number[j] = real_pot_number[j-1] + 1;
						}
					}
					else {
						count_of_pots++;
						real_pot_number[j] = 0;
					}
					if (_ppdata.m_pot[j]==0){
						break;
					}

				}

				switch (real_pot_number[npot]){
					case 0:
						if (count_of_pots > 1){
							pot_name = "the main pot";
						}
						else {
							pot_name = "the pot";
						}
						break;
					case 1:
						if (count_of_pots > 2){
							pot_name = "side pot #1";
						}
						else {
							pot_name = "the side pot";
						}
						break;
					default:
						pot_name.Format("side pot #%l", npot);
						break;
				}
				if (_ppdata.m_pinf[i].m_handValue > 0 ){
					s.Format("%s wins %s ($%s) with %s\n", _ppdata.m_pinf[i].m_name, pot_name, swin, FT_string_PokerVal(_ppdata.m_pinf[i].m_handValue));
				}
				else {
					s.Format("%s wins %s ($%s)\n", _ppdata.m_pinf[i].m_name, pot_name, swin);
				}
				writeFThh(&s);
			}
		}
	}
}

void PokerPro::HandleEventNextDealer(const PokerMessage* pm) 
{
	CString s;

	_ppdata.m_tinf.m_activeDealer = pm->m_chair&0x1f;

	char sbs[32];
	char bbs[32];
	char BBs[32];
	ConvertMoney(sbs,32, _ppdata.m_ginf[_ppdata.m_tinf.m_tid].m_sblind);
	ConvertMoney(bbs,32, _ppdata.m_ginf[_ppdata.m_tinf.m_tid].m_minbet);
	ConvertMoney(BBs,32, _ppdata.m_ginf[_ppdata.m_tinf.m_tid].m_maxbet);

	if (prefs.ppro_ft_hhlog_format()  == false) {
		s.Format("Table# %d %s %s %s %s %s %s %s\n",
			_ppdata.m_tinf.m_tid,
			_ppdata.m_tinf.m_name,
			_ppdata.m_ginf[_ppdata.m_tinf.m_tid].m_moneyid ? "RealMoney" : "PlayMoney",
			sbs, bbs, BBs,
			(_ppdata.m_tinf.m_lim&0x7)==2 ? "Limit" : (_ppdata.m_tinf.m_lim&0x7)==1 ? "PotLimit" : "NoLimit",
			"Holdem");
		WriteHH(&s);
	}
	else {
		if(_ppdata.m_handnumber > 0){

			time_t rawtime;
			tm* TimeHolder;
			tm FNTime;

			time(&rawtime);
			TimeHolder = localtime(&rawtime);
			FNTime = TimeHolder[0];
						s.Format("\n\n\nFull Tilt Poker Game #%d: Table %s - $%s/$%s - %s Hold'em - %d:%d:%d ET - %d/%02d/%02d\n",
				_ppdata.m_handnumber,
				_ppdata.m_tinf.m_name,
				sbs, bbs,
				(_ppdata.m_tinf.m_lim&0x7)==2 ? "Limit" : (_ppdata.m_tinf.m_lim&0x7)==1 ? "Pot Limit" : "No Limit",
				FNTime.tm_hour,
				FNTime.tm_min,
				FNTime.tm_sec,
				FNTime.tm_year + 1900,
				FNTime.tm_mon + 1,
				FNTime.tm_mday);
			writeFThh(&s);
		}
	}

	int dc = _ppdata.m_tinf.m_activeDealer&0xf;
	for (int i=0; i<10; i++) {
		int c;
		if (prefs.ppro_ft_hhlog_format()  == false) {
			c = (dc+1+i) % 10;
		}
		else {
			c = i + 1;
		}
		if ((_ppdata.m_pinf[c].m_isSeated&0x1) && (_ppdata.m_pinf[c].m_isActive&0x1) && _ppdata.m_pinf[c].m_balance > 0) {
			char sbal[32];
			ConvertMoney(sbal,32, _ppdata.m_pinf[c].m_balance);

			if (prefs.ppro_ft_hhlog_format()  == false) {
				s.Format("%s Chair%d %s %s\n", _ppdata.m_pinf[c].m_name, c, sbal, dc==c ? "BUTTON" : "");
				WriteHH(&s);
			}
			else {
				if(_ppdata.m_handnumber > 0){
					s.Format("Seat %d: %s ($%s)\n",
						c,
						_ppdata.m_pinf[c].m_name,
						sbal
						);
					writeFThh(&s);
				}
			}
		}
	}

}

void PokerPro::HandleEventNextPlayer(const PokerMessage* pm) 
{
	_ppdata.m_tinf.m_activePlayer = pm->m_chair&0x1f;
}

void PokerPro::HandleEventNextRound(const PokerMessage* pm) 
{
	for (int i=0; i<=9; i++) {
		_ppdata.m_pinf[i].m_betAmount=0;
	}
	_ppdata.m_prev_rais = 0;
}

void PokerPro::HandleEventNextHand(const PokerMessage* pm) 
{
	CString s = "";

	ResetHand();
	_ppdata.m_handnumber = pm->m_lparam[1];
	time_t gametime = pm->m_lparam[2];
	tm t;
	gmtime_s(&t, &gametime);

	char buf[32] = {0};
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &t);
	s.Format("\nHand# %d %s GMT %s\n", _ppdata.m_handnumber, buf, _ppdata.m_site_name);
	Publish(&s, 0);

	if (prefs.ppro_ft_hhlog_format()  == false) {
		s.Format("\nHand# %d %s GMT %s\n",  _ppdata.m_handnumber, buf, _ppdata.m_site_name);
		WriteHH(&s);
	}

}

void PokerPro::HandleEventNextGame(const PokerMessage* pm) 
{
}

void PokerPro::HandleEventNextTable(const PokerMessage* pm) 
{
}

void PokerPro::HandleEventSetPot(const PokerMessage* pm) 
{
	_ppdata.m_pot[pm->m_pot] = pm->m_amount;
}

void PokerPro::HandleEventTotalPot(const PokerMessage* pm) 
{
	CString s = "";

	char spot[32] = {0};
	ConvertMoney(spot, 32, pm->m_amount);

	if (prefs.ppro_ft_hhlog_format()  == false) {
		s.Format("DEALER POT %s\n", spot);
		WriteHH(&s);
	}
}

void PokerPro::HandleEventStakes(const PokerMessage* pm) 
{
	_ppdata.m_ginf[_ppdata.m_tinf.m_tid].m_sblind = pm->m_lparam[1];
	_ppdata.m_ginf[_ppdata.m_tinf.m_tid].m_minbet = pm->m_lparam[2];
	_ppdata.m_ginf[_ppdata.m_tinf.m_tid].m_maxbet = pm->m_lparam[3];
}

void PokerPro::HandleEventEndHand(const PokerMessage* pm) 
{

	CString s, hh;
	int chair;
	int npot;
	int count_of_pots;
	int rake;
	char sTotalPot[32];
	char sPot[32];
	char sRake[32];
	char a[4], b[4], c[4], d[4], e[4];



	if (prefs.ppro_ft_hhlog_format() == true){
		s.Format("*** SUMMARY ***\n");
		writeFThh(&s);
		count_of_pots = 0;
		for (int j=0; j<=9; j++) {
			count_of_pots++;
			if (_ppdata.m_pot[j]==0){
				break;
			}
		}

		ConvertMoney(sTotalPot,50, _ppdata.m_total_pot);
		s.Format("Total pot %s",sTotalPot);
		writeFThh(&s);

		rake = _ppdata.m_total_pot;

		if (count_of_pots > 1) {

			npot = 0;

			while (npot < count_of_pots) {

				ConvertMoney(sPot,50,_ppdata.m_pot[npot]);
				rake = rake - _ppdata.m_pot[npot];

				switch (npot) {
					case 0:
						s.Format(" Main pot %s.",sPot);
						writeFThh(&s);
						break;
					case 1:
						if (count_of_pots > 2) {
							s.Format(" Side pot 1 %s.",sPot);
						}
						else {
							s.Format(" Side pot %s.",sPot);
						}
						writeFThh(&s);
						break;
					default:
						s.Format(" Side pot %d %s.",npot,sPot);
						writeFThh(&s);
						break;
				}

				npot++;
			}
		}

		ConvertMoney(sRake,50,rake);
		s.Format(" | Rake %s\n",sRake);
		writeFThh(&s);

		Card2Ascii(a,_ppdata.m_tinf.m_card[0]);
		Card2Ascii(b,_ppdata.m_tinf.m_card[1]);
		Card2Ascii(c,_ppdata.m_tinf.m_card[2]);
		Card2Ascii(d,_ppdata.m_tinf.m_card[3]);
		Card2Ascii(e,_ppdata.m_tinf.m_card[4]);

		if (!_ppdata.m_tinf.m_card[0]==0) {
			if (!_ppdata.m_tinf.m_card[3]==0) {
				if (!_ppdata.m_tinf.m_card[4]==0) {
					s.Format("Board: [%s %s %s %s %s]\n",a,b,c,d,e);
					writeFThh(&s);
				}
				else {
					s.Format("Board: [%s %s %s %s]\n",a,b,c,d);
					writeFThh(&s);
				}
			}
			else {
				s.Format("Board: [%s %s %s]\n",a,b,c);
				writeFThh(&s);
			}

		}

		for (int i=0; i<10; i++) {
			chair = i + 1;
			s.Format("Seat %d: This line is not read by Poker Tracker.\n",
				chair
				);
			writeFThh(&s);
		}

		s.Format("\n\n\n");
		writeFThh(&s);

	}


	ResetHand();
}

void PokerPro::HandleEventEndTable(const PokerMessage* pm) 
{
}

void PokerPro::HandleEventDeal(const PokerMessage* pm) 
{
	CString s = "";

	for (int i=0; i<10; i++) {
		if (pm->m_lparam[1] & (1<<i)) {
			_ppdata.m_pinf[i].m_card[0] = (unsigned char)0xff;
			_ppdata.m_pinf[i].m_card[1] = (unsigned char)0xff;
		}
	}
	s.Format("Dealing...\n");
	Publish(&s, 0);

	if (prefs.ppro_ft_hhlog_format()  == false) {
		s.Format("DEALER DEAL\n");
		WriteHH(&s);
	}
	else {
		s.Format("The button is in seat #%d\n", _ppdata.m_tinf.m_activeDealer&0xf);
		writeFThh(&s);
	}

}

void PokerPro::HandleEventPlayerChat(const PokerMessage* pm) 
{
	CHAT_MESSAGE *pc = (CHAT_MESSAGE*)pm;
	CString s = "";
	s.Format("%s \"%s\"\n", pc->m_name, pc->m_text);
	Publish(&s, 0);
}

void PokerPro::HandleEventPokerValue(const PokerMessage* pm) 
{
	CString s = "";

	 _ppdata.m_pinf[pm->m_chair&0x1f].m_handValue = pm->m_amount;

	int isflush = (pm->m_amount & 0x90000000);

	int r4 = (pm->m_amount >> 16) & 15;
	int r3 = (pm->m_amount >> 12) & 15;
	int r2 = (pm->m_amount >>  8) & 15;
	int r1 = (pm->m_amount >>  4) & 15;
	int r0 = (pm->m_amount >>  0) & 15;

	char sval[8] = {0};
	sval[0] = Rank2Ascii(r4);
	sval[1] = Rank2Ascii(r3);
	sval[2] = Rank2Ascii(r2);
	sval[3] = Rank2Ascii(r1);
	sval[4] = Rank2Ascii(r0);
	sval[5] = isflush ? 's' : 'o';
	sval[6] = 0;
	sval[7] = 0;

	if (prefs.ppro_ft_hhlog_format()  == false) {
		s.Format("%s POKER %s\n", _ppdata.m_pinf[pm->m_chair&0x1f].m_name, sval);
		WriteHH(&s);
	}
	else {
		char a[4];
		char b[4];
		Card2Ascii(a, _ppdata.m_pinf[pm->m_chair&0x1f].m_card[0]);
		Card2Ascii(b, _ppdata.m_pinf[pm->m_chair&0x1f].m_card[1]);

		s.Format("%s shows [%s %s] %s\n", _ppdata.m_pinf[pm->m_chair&0x1f].m_name,a,b,FT_string_PokerVal(pm->m_amount));
		writeFThh(&s);
	}

}

void PokerPro::HandleEventRevealCard(const PokerMessage* pm) 
{
	CString s;

	int ccard = (pm->m_lparam[2] >> 0) & 0xff;
	int ncard = (pm->m_lparam[2] >> 8) & 0xff;
	_ppdata.m_pinf[pm->m_chair&0x1f].m_card[ncard] = ccard;

	if (ncard == 1) {
		char a[4];
		char b[4];
		Card2Ascii(a, _ppdata.m_pinf[pm->m_chair&0x1f].m_card[0]);
		Card2Ascii(b, _ppdata.m_pinf[pm->m_chair&0x1f].m_card[1]);

		int round = GetRound();
		const char* what = (round <= 1) ? "HAND" : "SHOW";

		if (prefs.ppro_ft_hhlog_format()  == false) {
			s.Format("%s %s %s %s\n", _ppdata.m_pinf[pm->m_chair&0x1f].m_name, what, a, b);
			WriteHH(&s);
		}
		else {
			if (round <= 1) {
				s.Format("Dealt to %s [%s %s]\n", _ppdata.m_pinf[pm->m_chair&0x1f].m_name, a, b);

			}
			else {
				if (_ppdata.m_tinf.m_card[4] > 0){
					s.Format("%s shows [%s %s]\n", _ppdata.m_pinf[pm->m_chair&0x1f].m_name, a, b);
				}
				else {
					s.Format("",1);
				}
			}
			writeFThh(&s);
		}
	}

}

void PokerPro::HandleEventFlop(const PokerMessage* pm) 
{
	int ccard = (pm->m_lparam[2] >> 0) & 0xff;
	int ncard = (pm->m_lparam[2] >> 8) & 0xff;

	ncard--;

	_ppdata.m_tinf.m_card[ncard] = ccard;
	CString s, hh;

	if (ncard < 2) { return; }
	if (ncard == 2) {
		char a[4], b[4], c[4];
		Card2Ascii(a, _ppdata.m_tinf.m_card[0]);
		Card2Ascii(b, _ppdata.m_tinf.m_card[1]);
		Card2Ascii(c, _ppdata.m_tinf.m_card[2]);

		s.Format("Dealer flop %s %s %s\n", a, b, c);
		if (prefs.ppro_ft_hhlog_format()  == false) {
			hh.Format("DEALER FLOP %s %s %s\n", a, b, c);
		}
		else {
			hh.Format("*** FLOP *** [%s %s %s]\n", a, b, c);
		}
	}

	if (ncard == 3) {
		char a[4], b[4], c[4], d[4];
		Card2Ascii(a, _ppdata.m_tinf.m_card[0]);
		Card2Ascii(b, _ppdata.m_tinf.m_card[1]);
		Card2Ascii(c, _ppdata.m_tinf.m_card[2]);
		Card2Ascii(d, _ppdata.m_tinf.m_card[3]);
		s.Format("Dealer turn %s\n", d);
		if (prefs.ppro_ft_hhlog_format()  == false) {
			hh.Format("DEALER TURN %s\n", d);
		}
		else {
			hh.Format("*** TURN *** [%s %s %s] [%s]\n", a, b, c, d);
		}
	}

	if (ncard == 4) {
		char a[4], b[4], c[4], d[4], e[4];
		Card2Ascii(a, _ppdata.m_tinf.m_card[0]);
		Card2Ascii(b, _ppdata.m_tinf.m_card[1]);
		Card2Ascii(c, _ppdata.m_tinf.m_card[2]);
		Card2Ascii(d, _ppdata.m_tinf.m_card[3]);
		Card2Ascii(e, _ppdata.m_tinf.m_card[4]);
		s.Format("Dealer river %s\n", e);
		if (prefs.ppro_ft_hhlog_format()  == false) {
			hh.Format("DEALER RIVER %s\n", e);
		}
		else {
			hh.Format("*** RIVER *** [%s %s %s %s] [%s]\n", a, b, c, d, e);
		}
	}

	Publish(&s, 0);
	if (prefs.ppro_ft_hhlog_format()  == false) {
		WriteHH(&hh);
	}
	else {
		writeFThh(&hh);
	}

}

void PokerPro::HandleEventPlayerList(const PokerMessage* pm) 
{
	int seatedbits	= pm->m_lparam[1];
	int activebits	= pm->m_lparam[2];
	int playingbits	= pm->m_lparam[3];

	for (int i=0; i<10; i++) 
	{
		_ppdata.m_pinf[i].m_isSeated = (seatedbits & (1<<i)) ? 1 : 0;
		_ppdata.m_pinf[i].m_isActive = (activebits & (1<<i)) ? 1 : 0;
		_ppdata.m_pinf[i].m_isPlaying = (playingbits & (1<<i)) ? 1 : 0;
	}
}

void PokerPro::HandleAccount(const PokerMessage* pm) 
{
	CString s = "";

	switch (pm->m_event) 
	{
	case 'RER0'	:
		s = "Username invalid"			;
		break;
	case 'RER1'	:
		s = "Password invalid"			;
		break;
	case 'RER2'	:
		s = "Username exists"				;
		break;
	case 'PWR0'	:
		s = "Current password mismatch"	;
		break;
	case 'PWR1'	:
		s = "New password invalid"		;
		break;
	case 'REGI'	:
		s = "Registration accepted"		;
		break;
	case 'PASS'	:
		s = "New password accepted"		;
		break;
	case 'LIR0'	:
		s = "Login rejected"				;
		break;
	case 'LOGI'	:
		s = "Login accepted"				;
		break;
	}
	Publish(&s, 0);

	if (prefs.ppro_ft_hhlog_format()  == false) {
		s = s.MakeLower();
		WriteHH(&s);
	}
}

void PokerPro::HandleEventStarted(const PokerMessage* pm) 
{
}

void PokerPro::HandleEventStopped(const PokerMessage* pm) 
{
}

void PokerPro::HandleEventTournament(const PokerMessage* pm) 
{
	CString s = "";

	switch (pm->m_event) 
	{
	case 'TMOD':
		s.Format("tournament %s", pm->m_lparam[1] ? "mode on" : "mode off");
		break;
	case '+TM' :
		s.Format("tournament started");
		break;
	case '-TM' :
		s.Format("tournament stopped");
		break;
	case 'TLOS'	:
	{
		int chair = pm->m_lparam[1];
		int place = pm->m_lparam[2];
		s.Format("%s PLACED %d%s", _ppdata.m_pinf[chair].m_name, place, GetCardinalSuffix(place));
	}
	break;
	}
	Publish(&s, 0);
}

void PokerPro::HandleEventTableLock(const PokerMessage* pm) 
{
	//const char* msg = pm->m_lparam[1] ? "locked" : "unlocked";
}

void PokerPro::HandleEventPing(PokerMessage* pm) 
{
	pm->m_event = 'PONG';
	SendPPMessage((char *) pm, sizeof(PokerMessage) );
}

char* PokerPro::SAssign(char* dst, const int dst_size, const char* src, const int dstsiz) 
{
	strncpy_s( dst, dst_size, src, dstsiz );
	dst[dstsiz-1]=0;
	return dst;
}

char* PokerPro::ConvertMoney(char* text, const int dst_size, const int amount) 
{
	int units = amount/100;
	int cents = amount%100;

	if (amount < 0)	
	{
		sprintf_s(text, dst_size, "%d.%02d", units, cents);
	}
	if (amount < 0)	
	{
		sprintf_s(text, dst_size, "0");
	}
	else if (amount < 100) 
	{
		sprintf_s(text, dst_size, ".%02d", cents);
	}
	else if (cents) 
	{
		sprintf_s(text, dst_size, "%d.%02d", units, cents);
	}
	else 
	{
		sprintf_s(text, dst_size, "%d", units);
	}

	return text;
}

void PokerPro::ResetHand(void) 
{
	int i= 0;

	for (i=0; i<=4; i++) 
	{
		_ppdata.m_tinf.m_card[i]=0;
	}
	for (i=0; i<=9; i++) 
	{
		_ppdata.m_pinf[i].m_card[0]=0;
		_ppdata.m_pinf[i].m_card[1]=0;
		_ppdata.m_pinf[i].m_betAmount=0;
		_ppdata.m_pinf[i].m_handValue=0;
	}
	memset(_ppdata.m_pot,0,sizeof(_ppdata.m_pot));
	_ppdata.m_prev_rais = 0;
}

const bool PokerPro::IsConnected(void)
{
	return _socket!=INVALID_SOCKET;
}

const int PokerPro::GetCurrentBet(void) 
{
	int bet=0;
	for (int i=0; i<10; i++) 
	{
		int _bet = _ppdata.m_pinf[i].m_betAmount;
		if (_bet > bet) 
		{
			bet = _bet;
		}
	}
	return bet;
}

const int PokerPro::GetBetX(void) 
{
	return (GetRound() >= 3) ? _ppdata.m_tinf.m_betMax : _ppdata.m_tinf.m_betMin;
}

const int PokerPro::GetRound(void) 
{
	int round=0;
	int ncc=GetNcc();
	int npd=GetNpd();

	switch (ncc) 
	{
	case 0:
		round = npd ? 1 : 0;
		break;
	case 1:
	case 2:
		round=1;
		break;
	case 3:
		round=2;
		break;
	case 4:
		round=3;
		break;
	case 5:
		round=4;
		break;
	}
	return round;
}

const int PokerPro::GetNcc(void) 
{
	int ncc=0;
	for (int i=0; i<5; i++) 
	{
		if (_ppdata.m_tinf.m_card[i]) 
		{
			ncc++;
		}
	}
	return ncc;
}

const int PokerPro::GetNpd(void) 
{
	int npd=0;
	for (int i=0; i<=9; i++) 
	{
		if (_ppdata.m_pinf[i].m_card[0] == 0 && _ppdata.m_pinf[i].m_card[1] == 0) 
		{
			continue;
		}
		npd++;
	}
	return npd;
}

void PokerPro::DoScrape(void) 
{
	// No critical section for CScraper is required in this function, as the only place this function is called is from the
	// heartbeat thread, and the heartbeat thread wraps the call to this function in a critical section itself

	int				i=0, j=0;
	unsigned int	suit = StdDeck_Suit_CLUBS;
	unsigned int	rank = StdDeck_Rank_2;
	unsigned int	card = CARD_NOCARD;
	char			money[50] = "";

	// Common cards
	for (i=0; i<=4; i++) 
	{
		p_scraper->set_card_common(i, CARD_NOCARD);

		if ((_ppdata.m_tinf.m_card[i]&0xff) == WH_CARDBACK)
		{
			p_scraper->set_card_common(i, CARD_BACK);
		}

		else if ((_ppdata.m_tinf.m_card[i]&0xff) == WH_NOCARD) 
		{
			p_scraper->set_card_common(i, CARD_NOCARD);
		}

		else 
		{
			if (((_ppdata.m_tinf.m_card[i]>>0)&0x0f) == WH_SUIT_CLUBS) 
			{
				suit = StdDeck_Suit_CLUBS;
				rank = ((_ppdata.m_tinf.m_card[i]>>4)&0x0f)-2;
				card = StdDeck_MAKE_CARD(rank, suit);
			}

			else if (((_ppdata.m_tinf.m_card[i]>>0)&0x0f) == WH_SUIT_DIAMONDS) 
			{
				suit = StdDeck_Suit_DIAMONDS;
				rank = ((_ppdata.m_tinf.m_card[i]>>4)&0x0f)-2;
				card = StdDeck_MAKE_CARD(rank, suit);
			}

			else if (((_ppdata.m_tinf.m_card[i]>>0)&0x0f) == WH_SUIT_HEARTS) 
			{
				suit = StdDeck_Suit_HEARTS;
				rank = ((_ppdata.m_tinf.m_card[i]>>4)&0x0f)-2;
				card = StdDeck_MAKE_CARD(rank, suit);
			}

			else if (((_ppdata.m_tinf.m_card[i]>>0)&0x0f) == WH_SUIT_SPADES) 
			{
				suit = StdDeck_Suit_SPADES;
				rank = ((_ppdata.m_tinf.m_card[i]>>4)&0x0f)-2;
				card = StdDeck_MAKE_CARD(rank, suit);
			}

			else
			{
				card = CARD_NOCARD;
			}

			p_scraper->set_card_common(i, card);
		}
	}

	// Player information
	for (i=0; i<=9; i++) 
	{

		// Player cards
		for (j=0; j<=1; j++) 
		{
			p_scraper->set_card_player(i, j, CARD_NOCARD);

			if (_ppdata.m_pinf[i].m_isPlaying&0x1) 
			{
				if ((_ppdata.m_pinf[i].m_card[j]&0xff) == WH_CARDBACK) 
				{
					p_scraper->set_card_player(i, j, CARD_BACK);
				}

				else if ((_ppdata.m_pinf[i].m_card[j]&0xff) == WH_NOCARD) 
				{
					p_scraper->set_card_player(i, j, CARD_NOCARD);
				}

				else 
				{
					if (((_ppdata.m_pinf[i].m_card[j]>>0)&0x0f) == WH_SUIT_CLUBS)
					{
						suit = StdDeck_Suit_CLUBS;
						rank = ((_ppdata.m_pinf[i].m_card[j]>>4)&0x0f)-2;
						card = StdDeck_MAKE_CARD(rank, suit);
					}

					else if (((_ppdata.m_pinf[i].m_card[j]>>0)&0x0f) == WH_SUIT_DIAMONDS)
					{
						suit = StdDeck_Suit_DIAMONDS;
						rank = ((_ppdata.m_pinf[i].m_card[j]>>4)&0x0f)-2;
						card = StdDeck_MAKE_CARD(rank, suit);
					}

					else if (((_ppdata.m_pinf[i].m_card[j]>>0)&0x0f) == WH_SUIT_HEARTS)
					{
						suit = StdDeck_Suit_HEARTS;
						rank = ((_ppdata.m_pinf[i].m_card[j]>>4)&0x0f)-2;
						card = StdDeck_MAKE_CARD(rank, suit);
					}

					else if (((_ppdata.m_pinf[i].m_card[j]>>0)&0x0f) == WH_SUIT_SPADES)
					{
						suit = StdDeck_Suit_SPADES;
						rank = ((_ppdata.m_pinf[i].m_card[j]>>4)&0x0f)-2;
						card = StdDeck_MAKE_CARD(rank, suit);
					}

					else
					{
						card = CARD_NOCARD;
					}

					p_scraper->set_card_player(i, j, card);
				}
			}
		}

		p_scraper->set_seated(i, (_ppdata.m_pinf[i].m_isSeated&0x1) ? "true" : "false");
		p_scraper->set_active(i, (_ppdata.m_pinf[i].m_isActive&0x1) ? "true" : "false");

		p_scraper->set_dealer(i, false);
		p_scraper->set_player_name(i, _ppdata.m_pinf[i].m_name);
		p_scraper->set_name_good_scrape(i, true);
		ConvertMoney(money, 50, _ppdata.m_pinf[i].m_balance);
		p_scraper->set_player_balance(i, atof(money));
		p_scraper->set_balance_good_scrape(i, true);
		ConvertMoney(money, 50, _ppdata.m_pinf[i].m_betAmount);
		p_scraper->set_player_bet(i, atof(money));
		p_scraper->set_sitting_out(i, !(_ppdata.m_pinf[i].m_isPlaying&0x1));

		// Clear some things if no one is at this chair
		if (!p_scraper->IsStringSeated(p_scraper->seated(i)) & !p_scraper->IsStringActive(p_scraper->active(i))) 
		{
			p_scraper->set_player_name(i, "");
			p_scraper->set_name_good_scrape(i, false);
			p_scraper->set_player_balance(i, 0.);
			p_scraper->set_balance_good_scrape(i, false);
		}

		// Pots
		ConvertMoney(money, 50, _ppdata.m_pot[i]);
		p_scraper->set_pot(i, atof(money));
	}

	p_scraper->set_dealer(_ppdata.m_tinf.m_activeDealer&0xf, true);

	// Limit info
	SLimitInfo LI;
	// We have to convert the handnumber here,
	// as the PPro-server sents ints, but we use now a String for handnumbers
	LI.handnumber.Format("%d", _ppdata.m_handnumber); 
	LI.found_handnumber = true;

	ConvertMoney(money, 50, _ppdata.m_ginf[_ppdata.m_tinf.m_tid].m_sblind);
	LI.sblind = atof(money);
	LI.found_sblind = true;
	ConvertMoney(money, 50, _ppdata.m_ginf[_ppdata.m_tinf.m_tid].m_minbet);
	LI.bblind = atof(money);
	LI.found_bblind = true;
	ConvertMoney(money, 50, _ppdata.m_ginf[_ppdata.m_tinf.m_tid].m_maxbet);
	LI.bbet = atof(money);
	LI.found_bbet = true;

	ConvertMoney(money, 50, 0/*_ppdata.m_ginf[_ppdata.m_tinf.m_tid].*/);  // don't know where to get ante info from
	LI.ante = atof(money);
	LI.found_ante = true;

	LI.limit = (_ppdata.m_ginf[_ppdata.m_tinf.m_tid].m_lim&0x7)==0 ? k_gametype_NL :
			   (_ppdata.m_ginf[_ppdata.m_tinf.m_tid].m_lim&0x7)==1 ? k_gametype_PL : 
			   k_gametype_FL;
	LI.found_limit = true;

	LI.istournament = (_ppdata.m_ginf[_ppdata.m_tinf.m_tid].m_tmode&0x1) && _ppdata.m_ginf[_ppdata.m_tinf.m_tid].m_tlevel;

	LI.sb_bb = 0.;
	LI.found_sb_bb = false;
	LI.bb_BB = 0.;
	LI.found_bb_BB = false;

	p_scraper->SetLimitInfo(LI);

	// buttons
	p_scraper->set_button_label(0, "fold");
	p_scraper->set_button_label(1, "call");
	p_scraper->set_button_label(2, "raise");
	p_scraper->set_button_label(3, "allin");
	p_scraper->set_button_label(4, "check");
	p_scraper->set_button_label(5, "sitin");
	p_scraper->set_button_label(6, "sitout");
	p_scraper->set_button_label(7, "autopost");

	for (i=0; i<=4; i++) 
	{
		p_scraper->set_button_state(i, "false");
	}
	if (_autoplayer_can_act) 
	{
		p_scraper->set_button_state(3, p_scraper->s_limit_info()->limit==k_gametype_NL ? "true" : "false");
		p_scraper->set_button_state(2, 
			_ppdata.m_pinf[_ppdata.m_userchair].m_balance > GetCurrentBet() - _ppdata.m_pinf[_ppdata.m_userchair].m_betAmount &&
			((p_scraper->s_limit_info()->limit==k_gametype_NL || p_scraper->s_limit_info()->limit==k_gametype_PL) ||
			  GetCurrentBet() < (_ppdata.m_tinf.m_maxNBets&0xf)*GetBetX()) ? "true" : "false");
		p_scraper->set_button_state(1, GetCurrentBet() - _ppdata.m_pinf[_ppdata.m_userchair].m_betAmount > 0 ? "true" : "false");
		p_scraper->set_button_state(4, GetCurrentBet() - _ppdata.m_pinf[_ppdata.m_userchair].m_betAmount == 0 ? "true" : "false");
		p_scraper->set_button_state(0, p_scraper->button_state(4)=="false" ? "true" : "false");
	}
	p_scraper->set_button_state(5, (_ppdata.m_pinf[_ppdata.m_userchair].m_isActive) ? "false" : "true");
	p_scraper->set_button_state(6, (_ppdata.m_pinf[_ppdata.m_userchair].m_isActive) ? "true" : "false");
	p_scraper->set_button_state(7, "true");
}

void PokerPro::DoAutoplayer(void) 
{
	int sym_myturnbits = (int) p_symbols->sym()->myturnbits;
	int sym_br = (int) p_symbols->sym()->br;

	// Calculate f$play, f$prefold, f$rebuy, f$delay and f$chat for use below
	p_symbols->CalcSecondaryFormulas();

	////////////////////////////////////////////////////////////////////////////////
	// f$play
	if (p_symbols->f$play()==-2) 
	{
		SendStand(_ppdata.m_userchair);	   // leave table
	}
	else if (p_symbols->f$play()==-1) 
	{ 
	}										// no action
																
	else if (p_symbols->f$play()==0 && p_scraper->GetButtonState(6)) 
	{
		SendSitout(_ppdata.m_userchair);	// sit out
	}

	else if (p_symbols->f$play()==1 && p_scraper->GetButtonState(5)) 
	{
		SendSitin(_ppdata.m_userchair);	// sit in
	}

	// If iterator thread is still iterating, then return
	if (iter_vars.iterator_thread_running()) 
		return;

	// if we have no visible buttons, then return
	if (!sym_myturnbits) 
		return;

	// scale f$delay to a number of scrapes
	int additional_frames_to_wait = p_symbols->f$delay() / (prefs.scrape_delay() + 1);	

	// If we don't have enough stable frames, or have not waited f$delay milliseconds, then return.
	if (p_stableframescounter->UpdateNumberOfStableFrames() < (prefs.frame_delay() + additional_frames_to_wait))
		return;

	// Now that we got through all of the above, we are ready to evaluate the primary formulas
	// and take the appropriate action
	p_symbols->CalcPrimaryFormulas(true);

	// take action
	if (p_symbols->f$alli() && p_scraper->GetButtonState(3) && _autoplayer_can_act) 
	{
		Sleep(500);
		SendAction('ALLI', 0);
		_autoplayer_can_act = false;
		p_heartbeat_thread->set_replay_recorded_this_turn(false);
		Sleep(500);
		p_symbols->RecordPrevAction(k_action_allin);
	}
	else if (p_symbols->f$swag() && p_scraper->GetButtonState(2) && _autoplayer_can_act) 
	{
		Sleep(500);
		SendAction('SBET', (int) (p_symbols->f$swag()*100));
		_autoplayer_can_act = false;
		p_heartbeat_thread->set_replay_recorded_this_turn(false);
		Sleep(500);
		p_symbols->RecordPrevAction(k_action_swag);
	}
	else if (p_symbols->f$rais() && p_scraper->GetButtonState(2) && _autoplayer_can_act) 
	{
		Sleep(500);
		SendAction('RAIS', 0);
		_autoplayer_can_act = false;
		p_heartbeat_thread->set_replay_recorded_this_turn(false);
		Sleep(500);
		p_symbols->RecordPrevAction(k_action_raise);
	}
	else if (p_symbols->f$call() && p_scraper->GetButtonState(1) && _autoplayer_can_act) 
	{
		Sleep(500);
		SendAction('CALL', 0);
		_autoplayer_can_act = false;
		p_heartbeat_thread->set_replay_recorded_this_turn(false);
		Sleep(500);
		p_symbols->RecordPrevAction(k_action_call);
	}
	else if (p_scraper->GetButtonState(4) && _autoplayer_can_act) 
	{
		Sleep(500);
		SendAction('CHEC', 0);
		_autoplayer_can_act = false;
		p_heartbeat_thread->set_replay_recorded_this_turn(false);
		Sleep(500);
		p_symbols->RecordPrevAction(k_action_check);
	}
	else if (_autoplayer_can_act) 
	{
		Sleep(500);
		SendAction('FOLD', 0);
		p_heartbeat_thread->set_replay_recorded_this_turn(false);
		_autoplayer_can_act = false;
		Sleep(500);
		p_symbols->RecordPrevAction(k_action_fold);
	}
}

void PokerPro::Publish(CString *text, const int flags) 
{
	// limit size of message box to 100 lines
	int i = 0, count = 1;

	for (i = m_pproDlg->messages.GetLength(); i>=0; i--) 
	{
		if (m_pproDlg->messages.Mid(i, 1) == "\n") 
		{
			count++;
		}
		if (count>=99) 
		{
			m_pproDlg->messages = m_pproDlg->messages.Mid(i+1);
			continue;
		}
	}

	m_pproDlg->messages.Append(text->GetString());
	m_pproDlg->messages_changed = true;

	if (flags & PUBLISH_ERR) 
	{
		if (!prefs.disable_msgbox())
			MessageBox(NULL, *text, "PPRO: ERROR", MB_OK | MB_TOPMOST);
	}
}

const char* PokerPro::GetAction(const int chair, const int spend) 
{
	int current = _ppdata.m_pinf[chair].m_betAmount;
	int round = GetRound();
	int call = GetCurrentBet();
	int playerpot = GetPlayerPot();

	if (round <= 0 && playerpot <= 0) 
	{
		return "SBLIND";
	}
	if (round <= 0 && playerpot > 0) 
	{
		return "BBLIND";
	}
	if (round >= 1 && spend <= 0) 
	{
		return "CHECK";
	}
	if (round >= 1 && call > 0 && current+spend <= call) 
	{
		return "CALL";
	}
	if (round >= 1 && current+spend > call && playerpot <= 0 && call <= 0) 
	{
		return "BET";
	}
	if (round >= 1 && current+spend > call && playerpot > 0 && call > 0) 
	{
		return "RAISE";
	}
	return "SPEND";
}

const int PokerPro::GetPlayerPot(void) 
{
	int sum=0;
	for (int i=0; i<=9; i++) 
	{
		sum += _ppdata.m_pinf[i].m_betAmount;
	}

	return sum;
}

char* PokerPro::Card2Ascii(char* text, const unsigned char cc) 
{
	const char* srank = " 123456789TJQKA|";
	const char* ssuit = " cdhs----------|";

	int rank = (cc >> 4) & 15;
	int suit = (cc >> 0) & 15;

	text[0] = srank[rank];
	text[1] = ssuit[suit];
	text[2] = 0;

	return text;
}

char PokerPro::Rank2Ascii(const int rank) 
{
	const char* srank = " 123456789TJQKA|";
	char ascii = srank[rank];
	return ascii;
}

void PokerPro::WriteHH(const CString *s) 
{
	CString fn = "", fd = "";
	FILE *hh_fp = NULL;

	if (prefs.ppro_handhistory()) 
	{
		fn.Format("%s\\ppro\\%s_%lu.log", _startup_path, _ppdata.m_site_name, p_sessioncounter->session_id());
		fopen_s(&hh_fp, fn.GetString(), "a");
		if (hh_fp==NULL) 
		{
			fd.Format("%s\\ppro", _startup_path);
			CreateDirectory(fd.GetString(), NULL);
			fopen_s(&hh_fp, fn.GetString(), "a");
		}

		if (hh_fp!=NULL) 
		{
			fprintf(hh_fp, "%s", s->GetString());
			fflush(hh_fp);
			fclose(hh_fp);
		}
	}
}

void PokerPro::writeFThh(const CString *s) {

		CString fn, fd;
		FILE *hh_fp;

		if (prefs.ppro_handhistory()) {
			fn.Format("%s\\ppro\\FTHH\\%s\\%s", _startup_path,_ppdata.m_pinf[_ppdata.m_userchair].m_name,m_ftp_filename);
			fopen_s(&hh_fp, fn.GetString(), "a");
			if (hh_fp==NULL) {
				fd.Format("%s\\ppro\\FTHH\\%s", _startup_path,_ppdata.m_pinf[_ppdata.m_userchair].m_name);
				CreateDirectory(fd.GetString(), NULL);
				fopen_s(&hh_fp, fn.GetString(), "a");
				if (hh_fp==NULL) {
					fd.Format("%s\\ppro\\FTHH", _startup_path);
					CreateDirectory(fd.GetString(), NULL);
					fd.Format("%s\\ppro\\FTHH\\%s", _startup_path,_ppdata.m_pinf[_ppdata.m_userchair].m_name);
					CreateDirectory(fd.GetString(), NULL);
					fopen_s(&hh_fp, fn.GetString(), "a");

					if (hh_fp==NULL) {
						fd.Format("%s\\ppro", _startup_path);
						CreateDirectory(fd.GetString(), NULL);
						fd.Format("%s\\ppro\\FTHH", _startup_path);
						CreateDirectory(fd.GetString(), NULL);
						fd.Format("%s\\ppro\\FTHH\\%s", _startup_path,_ppdata.m_pinf[_ppdata.m_userchair].m_name);
						CreateDirectory(fd.GetString(), NULL);
						fopen_s(&hh_fp, fn.GetString(), "a");
					}

				}

			}

			if (hh_fp!=NULL) {
				fprintf(hh_fp, "%s", s->GetString());
				fflush(hh_fp);
				fclose(hh_fp);
			}
		}

}


const char* PokerPro::GetCardinalSuffix(const int n) 
{
	switch (n%10) 
	{
		case 1:
			return "st";
			break;
		case 2:
			return "nd";
			break;
		case 3:
			return "rd";
			break;
		default:
			return "th";
			break;
	}

	return "??";
}

const CString PokerPro::FT_string_PokerVal(int in_hand_value) {

		CString ReturnValue;
		unsigned char Card1;
		unsigned char Card2;
		unsigned char Card4;

		Card1 = (in_hand_value >> 16) & 15;
		Card2 = (in_hand_value >> 12) & 15;
		Card4 = (in_hand_value >> 4) & 15;

		switch (in_hand_value/0x1000000) {
			case 0x80:
				if (Card1 == 0xE) {
					ReturnValue = "a royal flush";
				}
				else {
					ReturnValue.Format("a straight flush, %s high", FT_FullCardName(Card1));
				}
				break;
			case 0x40:
				ReturnValue.Format("four of a kind, %ss", FT_FullCardName(Card1));
				break;
			case 0x20:
				ReturnValue.Format("a full house, %ss and %ss", FT_FullCardName(Card1), FT_FullCardName(Card4));
				break;
			case 0x10:
				ReturnValue.Format("a flush, %s high", FT_FullCardName(Card1));
				break;
			case 0x08:
				ReturnValue.Format("a straight, %s high", FT_FullCardName(Card1));
				break;
			case 0x04:
				ReturnValue.Format("three of a kind, %ss", FT_FullCardName(Card1));
				break;
			case 0x02:
				ReturnValue.Format("two pair, %ss and %ss", FT_FullCardName(Card1), FT_FullCardName(Card4));
				break;
			case 0x01:
				ReturnValue.Format("a pair, %ss", FT_FullCardName(Card1));
				break;
			case 0x00:
				ReturnValue.Format("%s %s high", FT_FullCardName(Card1), FT_FullCardName(Card2));
				break;
			default:
				ReturnValue = "unknown hand";
				break;
		}

		return ReturnValue;

}

const CString PokerPro::FT_FullCardName(unsigned char in_card_value) {

		CString ReturnValue;

		switch (in_card_value) {
			case 0xE:
				ReturnValue = "Ace";
				break;
			case 0xD:
				ReturnValue = "King";
				break;
			case 0xC:
				ReturnValue = "Queen";
				break;
			case 0xB:
				ReturnValue = "Jack";
				break;
			case 0xA:
				ReturnValue = "Ten";
				break;
			case 0x9:
				ReturnValue = "Nine";
				break;
			case 0x8:
				ReturnValue = "Eight";
				break;
			case 0x7:
				ReturnValue = "Seven";
				break;
			case 0x6:
				ReturnValue = "Six";
				break;
			case 0x5:
				ReturnValue = "Five";
				break;
			case 0x4:
				ReturnValue = "Four";
				break;
			case 0x3:
				ReturnValue = "Three";
				break;
			case 0x2:
				ReturnValue = "Two";
				break;
			case 0x1:
				ReturnValue = "Ace";
				break;
			default:
				ReturnValue = "an uncard";
				break;
		}

		return ReturnValue;

}

void PokerPro::setHHFilename(void) {

	time_t rawtime;
	tm* TimeHolder;
	tm FNTime;

	char sbs[32];
	char bbs[32];

	time(&rawtime);
	TimeHolder = localtime(&rawtime);
	FNTime = TimeHolder[0];

	//Spreading this atrocity to multiple lines for clarity.
	//As clear as anything using Format can be, at any rate...


	m_ftp_filename.Format(
		"FT%d%02d%02d %s - $%s-$%s - %s.txt",
		FNTime.tm_year + 1900,
		FNTime.tm_mon + 1,
		FNTime.tm_mday,
		_ppdata.m_tinf.m_name,
		ConvertMoney(sbs,32,_ppdata.m_tinf.m_betSBlind),
		ConvertMoney(bbs,32,_ppdata.m_tinf.m_betMin),
		((_ppdata.m_tinf.m_lim&0x7)==0 ? "No Limit Hold'em" :
		(_ppdata.m_tinf.m_lim&0x7)==1 ? "Pot Limit Hold'em" :
		"Limit Hold'em")  //Copied wholesale from our friends as I have no idea how to read m_lim otherwise.
		);

}
