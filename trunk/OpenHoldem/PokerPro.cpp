#include "StdAfx.h"

#include "PokerPro.h"

#include "OpenHoldem.h"

#include "CScraper.h"
#include "CSymbols.h"
#include "CIteratorThread.h"
#include "CAutoplayer.h"
#include "CGlobal.h"

#include "registry.h"
#include "grammar.h"
#include "DialogPpro.h"

class PokerPro	ppro;

PokerPro::PokerPro(void) 
{
    __SEH_SET_EXCEPTION_HANDLER

    __SEH_HEADER

	Registry  reg;

    reg.read_reg();
    hostname = reg.hostname;
    port = reg.port;
    username = reg.username;
    password = reg.password;
    handhistory = reg.handhistory;
    chips = reg.chips;
    autoseat = reg.autoseat;

    // init
    memset(&data, 0, sizeof(ppdata));
    data.m_userchair = -1;
    autoplayer_can_act = false;

    ::WSAStartup( MAKEWORD(2,2), &m_wsadata );
    m_socket = INVALID_SOCKET;

    __SEH_LOGFATAL("PokerPro::Constructor : \n");
}


PokerPro::~PokerPro(void) 
{
    __SEH_HEADER

    ::WSACleanup();

    __SEH_LOGFATAL("PokerPro::Destructor : \n");
}

int PokerPro::connect(const char *host, const char * port) 
{
    __SEH_HEADER

    if (m_socket != INVALID_SOCKET) 
	{
        return -1;
    }

    int iresult;

    addrinfo*	pai;

    if (resolve(host, port, &pai) == 0) 
	{
        m_socket = INVALID_SOCKET;
        return -2;
    }

    if ((m_socket = ::socket(pai->ai_family, pai->ai_socktype, pai->ai_protocol)) != INVALID_SOCKET) 
	{
        iresult = ::connect(m_socket, pai->ai_addr, pai->ai_addrlen);
    }

    if (pai) 
	{
        ::freeaddrinfo(pai);
    }

    if (m_socket == INVALID_SOCKET) 
	{
        return -3;
    }

    if (iresult == SOCKET_ERROR) 
	{
        m_socket = INVALID_SOCKET;
        return -4;
    }

    nolinger(m_socket);

    return 1;

    __SEH_LOGFATAL("PokerPro::connect : \n");
}

int PokerPro::disconnect(void) 
{
    __SEH_HEADER

    ::shutdown(m_socket,SD_BOTH);
    ::closesocket( m_socket );
    m_socket = INVALID_SOCKET;
    data.m_isauthenticated = false;
    return 1;

    __SEH_LOGFATAL("PokerPro::disconnect : \n");
}

int PokerPro::send_login(const char* username, const char* password) 
{
    __SEH_HEADER

    PokerMessage pm;
    pm.m_event = 'LOGI';
    pm.m_chair = -1;
    sassign(pm.m_name, username, sizeof(pm.m_name));
    sassign(pm.m_pass, password, sizeof(pm.m_pass));
    return send_message((char*)(&pm), sizeof(PokerMessage));

    __SEH_LOGFATAL("PokerPro::send_login : \n");
}

int PokerPro::send_version(void) 
{
    __SEH_HEADER

    PokerMessage pm;
    pm.m_event = 'VERS';
    pm.m_version = PROTOCOL_VERSION;
    return send_message((char*)(&pm), sizeof(PokerMessage));

    __SEH_LOGFATAL("PokerPro::send_version : \n");
}

int PokerPro::send_goto(int location) 
{
    __SEH_HEADER

    PokerMessage pm;
    pm.m_event		= 'GOTO';
    pm.m_location	= location;
    return send_message((char*)(&pm), sizeof(PokerMessage));

    __SEH_LOGFATAL("PokerPro::send_goto : \n");
}

int PokerPro::send_sit(int chair) 
{
    __SEH_HEADER

    PokerMessage pm;
    pm.m_event = 'PSIT';
    pm.m_chair = (chair == -1) ? data.m_userchair : chair;
    return send_message((char*)(&pm), sizeof(PokerMessage));

    __SEH_LOGFATAL("PokerPro::send_sit : \n");
}

int PokerPro::send_stand(int chair) 
{
    __SEH_HEADER

    PokerMessage pm;
    pm.m_event = 'PUNS';
    pm.m_chair = (chair == -1) ? data.m_userchair : chair;
    return send_message((char*)(&pm), sizeof(PokerMessage));

    __SEH_LOGFATAL("PokerPro::send_stand : \n");
}

int PokerPro::send_chips(double chips) 
{
    __SEH_HEADER

    CHAT_MESSAGE	m;
    char			msg[100];
    sprintf(msg, "/chips %.2f", chips);

    memset(&m,0,sizeof(m));

    m.m_tag='CHAT';

    if (data.m_userchair >= 0)
        sassign(m.m_name, data.m_pinf[data.m_userchair].m_name, sizeof(m.m_name));
    m.m_name[sizeof(m.m_name)-1]=0;

    sassign(m.m_text, msg, sizeof(m.m_text));
    m.m_text[sizeof(m.m_text)-1]=0;

    int nbytes = (int) strlen(m.m_text);

    nbytes += sizeof(m.m_tag);
    nbytes += sizeof(m.m_name);
    if (nbytes < sizeof(CHAT_MESSAGE))
        nbytes++;
    if (nbytes < sizeof(PokerMessage))
        nbytes = sizeof(PokerMessage);

    int result = send_message( (char*)(&m), nbytes );
    return result;

    __SEH_LOGFATAL("PokerPro::send_chips : \n");
}

int PokerPro::send_deposit(double chips) 
{
    __SEH_HEADER

    CHAT_MESSAGE	m;
    char			msg[100];
    sprintf(msg, "/bank deposit %.2f", chips);

    memset(&m,0,sizeof(m));

    m.m_tag='CHAT';

    if (data.m_userchair >= 0)
        sassign(m.m_name, data.m_pinf[data.m_userchair].m_name, sizeof(m.m_name));
    m.m_name[sizeof(m.m_name)-1]=0;

    sassign(m.m_text, msg, sizeof(m.m_text));
    m.m_text[sizeof(m.m_text)-1]=0;

    int nbytes = (int) strlen(m.m_text);

    nbytes += sizeof(m.m_tag);
    nbytes += sizeof(m.m_name);
    if (nbytes < sizeof(CHAT_MESSAGE))
        nbytes++;
    if (nbytes < sizeof(PokerMessage))
        nbytes = sizeof(PokerMessage);

    int result = send_message( (char*)(&m), nbytes );
    return result;

    __SEH_LOGFATAL("PokerPro::send_deposit : \n");
}

int PokerPro::send_sitin(int chair)
{
    __SEH_HEADER

    PokerMessage pm;
    pm.m_event = 'PACT';
    pm.m_chair = (chair == -1) ? data.m_userchair : chair;
    return send_message((char*)(&pm), sizeof(PokerMessage));

    __SEH_LOGFATAL("PokerPro::send_sitin : \n");
}

int PokerPro::send_sitout(int chair) 
{
    __SEH_HEADER

    PokerMessage pm;
    pm.m_event = 'PINA';
    pm.m_chair = (chair == -1) ? data.m_userchair : chair;
    return send_message((char*)(&pm), sizeof(PokerMessage));

    __SEH_LOGFATAL("PokerPro::send_sitout : \n");
}

int PokerPro::send_action(int action, int amount) 
{
    __SEH_HEADER

    PokerMessage pm;
    pm.m_event = action;
    pm.m_chair = data.m_userchair;
    pm.m_amount = amount;
    return send_message((char*)(&pm), sizeof(PokerMessage));

    __SEH_LOGFATAL("PokerPro::send_action : \n");
}

int PokerPro::resolve (const char* host, const char* port, addrinfo** ppai) 
{
    __SEH_HEADER

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

    __SEH_LOGFATAL("PokerPro::resolve : \n");
}


void PokerPro::nolinger (SOCKET s) 
{
    __SEH_HEADER

    linger  _linger;
    _linger.l_onoff         = 0;    //0=dontlinger  1=linger
    _linger.l_linger        = 0;    //nseconds to linger

    setsockopt
    (
        s,
        SOL_SOCKET,
        SO_LINGER,
        (char *)&_linger,
        sizeof(_linger)
    );

    __SEH_LOGFATAL("PokerPro::nolinger : \n");
}

int PokerPro::get_random_vacant_chair(void) 
{
    __SEH_HEADER

    int rc = -1;

    if (data.m_location > 0)	
	{
        int vacant[10];
        int nvacant=0;
        for (int i=0; i<10; i++) 
		{
            if (data.m_pinf[i].m_isSeated&0x1) 
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

    __SEH_LOGFATAL("PokerPro::get_random_vacant_chair : \n");
}

int PokerPro::send_message(const char* pbytes, int nbytes) 
{
    __SEH_HEADER

    int result;

    if (nbytes > ppsock__msgsizmin)	
	{
        char msg[ppsock__msgsizmin];
        long* plong = ((long*)(&msg[0]));

        plong[0] = 'yasm';
        plong[1] = nbytes;

        result = sendbuffer(msg, sizeof(msg));
    }

    result = sendbuffer(pbytes, nbytes);

    return result;

    __SEH_LOGFATAL("PokerPro::send_message : \n");
}

int PokerPro::recv_message(const char** ppbytes, int* pnbytes) 
{
    __SEH_HEADER

    int nbytes = ppsock__msgsizmin;

    m_nmsg += 1;
    m_nmsg %= ppsock__nmsgmax;
    char* pbytes = &(m_data[m_nmsg][0]);

    int result = recvbuffer(pbytes, nbytes);
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

        result = recvbuffer(pbytes, nbytes);

        if (result == 0) 
		{
            return 0;
        }
    }

    *ppbytes = pbytes;
    *pnbytes = nbytes;

    return 1;

    __SEH_LOGFATAL("PokerPro::recv_message : \n");
}


int PokerPro::sendbuffer(const char* pbytes, int nbytes) 
{
    __SEH_HEADER

    int nbytes_sent=0;

    while (nbytes_sent < nbytes) 
	{
        int iresult = send(pbytes+nbytes_sent, nbytes-nbytes_sent);

        if (iresult == SOCKET_ERROR) 
		{
            return SOCKET_ERROR;
        }

        nbytes_sent += iresult;
    }
    return nbytes_sent;

    __SEH_LOGFATAL("PokerPro::sendbuffer : \n");
}

int PokerPro::recvbuffer (char* pbytes, int nbytes) 
{
    __SEH_HEADER

    int nbytesrecv = 0;

    while (nbytesrecv < nbytes) 
	{
        int nrecv = recv(pbytes+nbytesrecv, nbytes-nbytesrecv);

        if (nrecv < 1) 
		{
            return 0;    //closed or error
        }

        nbytesrecv += nrecv;
    }

    return nbytesrecv;

    __SEH_LOGFATAL("PokerPro::recvbuffer : \n");
}

int PokerPro::send(const char* bytes, int nbytes) 
{
    __SEH_HEADER

    return ::send( m_socket, bytes, nbytes, 0 );

    __SEH_LOGFATAL("PokerPro::send : \n");
}

int PokerPro::recv(char* bytes, int nbytes) 
{
    __SEH_HEADER

    return ::recv( m_socket, bytes, nbytes, 0 );

    __SEH_LOGFATAL("PokerPro::recv : \n");
}

void PokerPro::handle_message(const char* pbytes, int nbytes) 
{
    PokerMessage* pm;

    __SEH_HEADER

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

    default		:
        HandleEventUnknown		(pbytes);
        break;
    }

    __SEH_LOGFATAL("PokerPro::handle_message : m_event>%d\n", pm->m_event);
}


void PokerPro::HandleEventUnknown(const char* pbytes) 
{
    __SEH_HEADER

    char stag[8];
    stag[0] = pbytes[3];
    stag[1] = pbytes[2];
    stag[2] = pbytes[1];
    stag[3] = pbytes[0];
    stag[4] = 0;

    CString s;
    s.Format("Unknown message received (%s)...\n", stag);
    publish(&s, PUBLISH_ERR);

    __SEH_LOGFATAL("PokerPro::HandleEventUnknown : \n");
}


void PokerPro::HandleEventUserChair(PokerMessage* pm) 
{
    __SEH_HEADER

    data.m_userchair = pm->m_chair;

    __SEH_LOGFATAL("PokerPro::HandleEventUserChair : \n");
}

void PokerPro::HandleEventPlayerPlaying(PokerMessage* pm) 
{
    __SEH_HEADER

    data.m_pinf[pm->m_chair&0x1f].m_isPlaying = 1;

    __SEH_LOGFATAL("PokerPro::HandleEventPlayerPlaying : \n");
}

void PokerPro::HandleEventPlayerNotPlaying(PokerMessage* pm) 
{
    __SEH_HEADER

    data.m_pinf[pm->m_chair&0x1f].m_isPlaying = 0;

    __SEH_LOGFATAL("PokerPro::HandleEventPlayerNotPlaying : \n");
}

void PokerPro::HandleEventPlayerActive(PokerMessage* pm) 
{
    __SEH_HEADER

    data.m_pinf[pm->m_chair&0x1f].m_isActive = 1;

    if (pm->m_chair == data.m_userchair) 
	{
        CString s;
        s.Format("Sitting in.\n");
        publish(&s, 0);
    }

    __SEH_LOGFATAL("PokerPro::HandleEventPlayerActive : \n");
}

void PokerPro::HandleEventPlayerInactive(PokerMessage* pm) 
{
    __SEH_HEADER

    data.m_pinf[pm->m_chair&0x1f].m_isActive = 0;

    if (pm->m_chair == data.m_userchair) 
	{
        CString s;
        s.Format("Sitting out.\n");
        publish(&s, 0);
    }

    __SEH_LOGFATAL("PokerPro::HandleEventPlayerInactive : \n");
}

void PokerPro::HandleEventPlayerSit(PokerMessage* pm) 
{
    __SEH_HEADER

    data.m_pinf[pm->m_chair&0x1f].m_isSeated = 1;
    CString s;
    s.Format("Chair %d joining table.\n", pm->m_chair&0x1f);
    publish(&s, 0);

    __SEH_LOGFATAL("PokerPro::HandleEventPlayerSit : \n");
}

void PokerPro::HandleEventPlayerUnSit(PokerMessage* pm) {

    __SEH_HEADER

    data.m_pinf[pm->m_chair&0x1f].m_isSeated = 0;
    if (pm->m_chair == data.m_userchair) 
	{
        data.m_userchair = -1;
    }
    CString s;
    s.Format("Chair %d leaving table.\n", pm->m_chair&0x1f);
    publish(&s, 0);

    __SEH_LOGFATAL("PokerPro::HandleEventPlayerUnSit : \n");
}

void PokerPro::HandleEventSiteName(PokerMessage* pm) 
{
    __SEH_HEADER

    sassign(data.m_site_name, pm->m_text, sizeof(data.m_site_name));
    CString s;
    s.Format("%s\n", pm->m_text);
    publish(&s, 0);

    __SEH_LOGFATAL("PokerPro::HandleEventSiteName : \n");
}

void PokerPro::HandleEventTableInfo(PokerMessage* pm) 
{
    __SEH_HEADER

    TINF* ptinf = (TINF*)pm;
    data.m_tinf = *ptinf;

    __SEH_LOGFATAL("PokerPro::HandleEventTableInfo : \n");
}

void PokerPro::HandleEventGameInfo(PokerMessage* pm) 
{
    __SEH_HEADER

    GINF* pginf = (GINF*)pm;
    if (pginf->m_tid<=0 || pginf->m_tid>=MAX_TABLES) 
	{
        return;
    }
    data.m_ginf[pginf->m_tid] = *pginf;

    char sbs[32];
    char bbs[32];
    char BBs[32];
    convert_money( sbs, pginf->m_sblind );
    convert_money( bbs, pginf->m_minbet );
    convert_money( BBs, pginf->m_maxbet );

    char tL[2] = "*";
    if ((pginf->m_tmode&0x1) && pginf->m_tlevel) 
	{
        tL[0] = 'A' + pginf->m_tlevel - 1;
    }

    CString s;
    s.Format("%2d %s %s %s %-15s %2d/%2d %5s %5s %5s\n",
             pginf->m_tid,
             pginf->m_moneyid==0 ? "-" : "$",
             ((pginf->m_lim&0x7)==0 ? "N" : (pginf->m_lim&0x7)==1 ? "P" : "L"),
             (pginf->m_tmode&0x1) ? tL : "-",
             pginf->m_name,
             pginf->m_nSeated, pginf->m_nChairs,
             sbs, bbs, BBs);
    publish(&s, 0);

    __SEH_LOGFATAL("PokerPro::HandleEventGameInfo : \n");
}

void PokerPro::HandleEventGoto(PokerMessage* pm) 
{
    __SEH_HEADER

    data.m_location = pm->m_location;
    CString s;
    s.Format("Joined table %d.\n", pm->m_location);
    publish(&s, 0);

    __SEH_LOGFATAL("PokerPro::HandleEventGoto : \n");
}

void PokerPro::HandleEventVersion(PokerMessage* pm) 
{
    __SEH_HEADER

    __SEH_LOGFATAL("PokerPro::HandleEventVersion : \n");
}

void PokerPro::HandleEventPlayerLogout(PokerMessage* pm) 
{
    __SEH_HEADER

    data.m_isauthenticated = false;
    CString s;
    s.Format("Logout complete.\n");
    publish(&s, 0);

    __SEH_LOGFATAL("PokerPro::HandleEventPlayerLogout : \n");
}

void PokerPro::HandleEventPlayerLogin(PokerMessage* pm) 
{
    __SEH_HEADER

    data.m_isauthenticated = true;
    CString s;
    s.Format("Login complete.\n");
    publish(&s, 0);

    __SEH_LOGFATAL("PokerPro::HandleEventPlayerLogin : \n");
}

void PokerPro::HandleEventPlayerLoginReject(PokerMessage* pm) 
{
    __SEH_HEADER

    data.m_isauthenticated = false;
    CString s;
    s.Format("Login rejected.\n");
    publish(&s, 0);

    __SEH_LOGFATAL("PokerPro::HandleEventPlayerLoginReject : \n");
}

void PokerPro::HandleEventPlayerInfo(PokerMessage* pm) 
{
    __SEH_HEADER

    PINF* ppinf = (PINF*)pm;
    data.m_pinf[ppinf->m_chair&0x1f] = *ppinf;

    char sbal[32];
    char sbet[32];
    convert_money(sbal, ppinf->m_balance);
    convert_money(sbet, ppinf->m_betAmount);

    CString s;
    s.Format("%d %15s %s %s\n", ppinf->m_chair&0x1f, ppinf->m_name, sbal, sbet);
    publish(&s, 0);

    __SEH_LOGFATAL("PokerPro::HandleEventPlayerInfo : \n");
}

void PokerPro::HandleEventWPDS(PokerMessage* pm) 
{
    __SEH_HEADER

    PokerMessage PM;
    PM.m_event = pm->m_event;			//'WPDS'
    PM.m_lparam[1] = pm->m_lparam[1];	//hand number

    // generate 52 random numbers (0-51)
    for (int i=0; i<=51; i++) {
        unsigned char r = (unsigned char)(rand() % 52);
        PM.m_cparam[8+i] = r;
    }
    send_message((char*)(&PM), sizeof(PokerMessage));

    __SEH_LOGFATAL("PokerPro::HandleEventWPDS : \n");
}

void PokerPro::HandleEventWaitPlayer(PokerMessage* pm) 
{
    __SEH_HEADER

    int tag = pm->m_event;
    int action = 0;
    int amount = 0;
    CString s;

    if ((data.m_tinf.m_activePlayer&0xf) != data.m_userchair) 
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
        autoplayer_can_act = true;
        break;
    }

    send_action(action, amount);

    __SEH_LOGFATAL("PokerPro::HandleEventWaitPlayer : \n");
}

void PokerPro::HandleEventPlayerFold(PokerMessage* pm) 
{
    __SEH_HEADER

    CString s;

    data.m_pinf[pm->m_chair&0x1f].m_card[0] = 0;
    data.m_pinf[pm->m_chair&0x1f].m_card[1] = 0;

    s.Format("Chair %d (%s) folds.\n", pm->m_chair&0x1f, data.m_pinf[pm->m_chair&0x1f].m_name);
    publish(&s, 0);

    s.Format("%s FOLD\n", data.m_pinf[pm->m_chair&0x1f].m_name);
    writehh(&s);

    __SEH_LOGFATAL("PokerPro::HandleEventPlayerFold : \n");
}

void PokerPro::HandleEventPlayerMuck(PokerMessage* pm) 
{
    __SEH_HEADER

    CString s;

    data.m_pinf[pm->m_chair&0x1f].m_card[0] = 0;
    data.m_pinf[pm->m_chair&0x1f].m_card[1] = 0;
    s.Format("Chair %d (%s) mucks.\n", pm->m_chair&0x1f, data.m_pinf[pm->m_chair&0x1f].m_name);
    publish(&s, 0);

    s.Format("%s MUCK\n", data.m_pinf[pm->m_chair&0x1f].m_name);
    writehh(&s);

    __SEH_LOGFATAL("PokerPro::HandleEventPlayerMuck : \n");
}

void PokerPro::HandleEventPlayerRefund(PokerMessage* pm) 
{
    __SEH_HEADER

    CString s;

    char sbet[32];
    convert_money(sbet, pm->m_amount);
    s.Format("Chair %d (%s) refund %s.\n", pm->m_chair&0x1f, data.m_pinf[pm->m_chair&0x1f].m_name, sbet);
    publish(&s, 0);

    s.Format("%s RFND %s\n", data.m_pinf[pm->m_chair&0x1f].m_name, sbet);
    writehh(&s);

    __SEH_LOGFATAL("PokerPro::HandleEventPlayerRefund : \n");
}

void PokerPro::HandleEventPlayerBet(PokerMessage* pm) 
{
    __SEH_HEADER

    CString s, hh;

    int tag = pm->m_event;
    int chair = pm->m_chair&0x1f;
    int after = pm->m_amount; // amount after spend
    int balance = data.m_pinf[chair].m_balance; // balance after spend
    int before = data.m_pinf[chair].m_betAmount;
    int spend = after - before;
    int currbet = get_current_bet();
    int call = currbet - before;
    int rais = spend - call;

    if (rais > data.m_prev_rais) 
	{
        data.m_prev_rais = rais;
    }

    const char* action = get_action(chair, spend);
    const char* name = data.m_pinf[chair].m_name;

    data.m_pinf[chair].m_betAmount = after;

    char samt[32];
    convert_money(samt, spend);

    if (spend==0) {
        s.Format("Chair %d (%s) %s.\n", chair, name, action);
        hh.Format("%s %s\n", name, action);
    }
    else if (data.m_pinf[chair].m_balance) 
	{
        s.Format("Chair %d (%s) %s %s.\n", chair, name, action, samt);
        hh.Format("%s %s %s\n", name, action, samt);
    }
    else {
        s.Format("Chair %d (%s) %s %s all in.\n", chair, name, action, samt);
        hh.Format("%s %s %s ALLIN\n", name, action, samt );
    }

    publish(&s, 0);
    writehh(&hh);

    __SEH_LOGFATAL("PokerPro::HandleEventPlayerBet : \n");
}

void PokerPro::HandleEventPlayerName(PokerMessage* pm) 
{
    __SEH_HEADER

    sassign(data.m_pinf[pm->m_chair&0x1f].m_name, pm->m_name, sizeof(data.m_pinf[pm->m_chair&0x1f].m_name));

    __SEH_LOGFATAL("PokerPro::HandleEventPlayerName : \n");
}

void PokerPro::HandleEventPlayerBalance(PokerMessage* pm) 
{
    __SEH_HEADER

    data.m_pinf[pm->m_chair&0x1f].m_balance = pm->m_amount;

    if (pm->m_chair == data.m_userchair) 
	{
        char sbal[32];
        convert_money(sbal, pm->m_amount);
        CString s;
        s.Format("%s's balance is now %s\n", data.m_pinf[pm->m_chair&0x1f].m_name, sbal);
        publish(&s, 0);
    }

    __SEH_LOGFATAL("PokerPro::HandleEventPlayerBalance : \n");
}

void PokerPro::HandleEventPlayerWin(PokerMessage* pm) 
{
    __SEH_HEADER

    CString s;

    int chairbits = pm->m_chair;
    int npot = pm->m_lparam[2];

    int nwin=0;
    for (int chair=0; chair<=9; chair++) 
	{
        if (chairbits&(1<<chair)) 
		{
            nwin++;
        }
    }
    int win = data.m_pot[npot]/nwin;
    data.m_pot[npot] = 0;

    char swin[32];
    convert_money(swin, win);

    for (int i=0; i<=9; i++) 
	{
        if (chairbits & (1<<i))	
		{
            data.m_pinf[i].m_betAmount = win;
            s.Format("%s wins %s.\n", data.m_pinf[i].m_name, swin);
            publish(&s, 0);

            s.Format("%s WIN %s\n", data.m_pinf[i].m_name, swin);
            writehh(&s);
        }
    }

    __SEH_LOGFATAL("PokerPro::HandleEventPlayerWin : \n");
}

void PokerPro::HandleEventNextDealer(PokerMessage* pm) 
{
    __SEH_HEADER

    CString s;

    data.m_tinf.m_activeDealer = pm->m_chair&0x1f;

    char sbs[32];
    char bbs[32];
    char BBs[32];
    convert_money(sbs, data.m_ginf[data.m_tinf.m_tid].m_sblind);
    convert_money(bbs, data.m_ginf[data.m_tinf.m_tid].m_minbet);
    convert_money(BBs, data.m_ginf[data.m_tinf.m_tid].m_maxbet);
    s.Format("Table# %d %s %s %s %s %s %s %s\n",
             data.m_tinf.m_tid,
             data.m_tinf.m_name,
             data.m_ginf[data.m_tinf.m_tid].m_moneyid ? "RealMoney" : "PlayMoney",
             sbs, bbs, BBs,
             (data.m_tinf.m_lim&0x7)==2 ? "Limit" : (data.m_tinf.m_lim&0x7)==1 ? "PotLimit" : "NoLimit",
             "Holdem");
    writehh(&s);

    int dc = data.m_tinf.m_activeDealer&0xf;
    for (int i=0; i<10; i++) {
        int c = (dc+1+i) % 10;
        if ((data.m_pinf[c].m_isSeated&0x1) && (data.m_pinf[c].m_isActive&0x1) && data.m_pinf[c].m_balance > 0) {
            char sbal[32];
            convert_money(sbal, data.m_pinf[c].m_balance);

            s.Format("%s Chair%d %s %s\n", data.m_pinf[c].m_name, c, sbal, dc==c ? "BUTTON" : "");
            writehh(&s);
        }
    }

    __SEH_LOGFATAL("PokerPro::HandleEventNextDealer : \n");
}

void PokerPro::HandleEventNextPlayer(PokerMessage* pm) 
{
    __SEH_HEADER

    data.m_tinf.m_activePlayer = pm->m_chair&0x1f;

    __SEH_LOGFATAL("PokerPro::HandleEventNextPlayer : \n");
}

void PokerPro::HandleEventNextRound(PokerMessage* pm) 
{
    __SEH_HEADER

    for (int i=0; i<=9; i++) {
        data.m_pinf[i].m_betAmount=0;
    }
    data.m_prev_rais = 0;

    __SEH_LOGFATAL("PokerPro::HandleEventNextRound : \n");
}

void PokerPro::HandleEventNextHand(PokerMessage* pm) 
{
    __SEH_HEADER

    CString s;

    reset_hand();
    data.m_handnumber = pm->m_lparam[1];
    time_t gametime = pm->m_lparam[2];
    tm* t = gmtime(&gametime);
    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", t);
    s.Format("\nHand# %d %s GMT %s\n", data.m_handnumber, buf, data.m_site_name);
    publish(&s, 0);

    s.Format("\nHand# %d %s GMT %s\n", data.m_handnumber, buf, data.m_site_name);
    writehh(&s);

    __SEH_LOGFATAL("PokerPro::HandleEventNextHand : \n");
}

void PokerPro::HandleEventNextGame(PokerMessage* pm) 
{
    __SEH_HEADER

    __SEH_LOGFATAL("PokerPro::HandleEventNextGame : \n");
}

void PokerPro::HandleEventNextTable(PokerMessage* pm) 
{
    __SEH_HEADER


    __SEH_LOGFATAL("PokerPro::HandleEventNextTable : \n");
}

void PokerPro::HandleEventSetPot(PokerMessage* pm) 
{
    __SEH_HEADER

    data.m_pot[pm->m_pot] = pm->m_amount;

    __SEH_LOGFATAL("PokerPro::HandleEventSetPot : \n");
}

void PokerPro::HandleEventTotalPot(PokerMessage* pm) 
{
    __SEH_HEADER

    CString s;

    char spot[32];
    convert_money(spot, pm->m_amount);

    s.Format("DEALER POT %s\n", spot);
    writehh(&s);

    __SEH_LOGFATAL("PokerPro::HandleEventTotalPot : \n");
}

void PokerPro::HandleEventStakes(PokerMessage* pm) 
{
    __SEH_HEADER

    data.m_ginf[data.m_tinf.m_tid].m_sblind = pm->m_lparam[1];
    data.m_ginf[data.m_tinf.m_tid].m_minbet = pm->m_lparam[2];
    data.m_ginf[data.m_tinf.m_tid].m_maxbet = pm->m_lparam[3];

    __SEH_LOGFATAL("PokerPro::HandleEventStakes : \n");
}

void PokerPro::HandleEventEndHand(PokerMessage* pm) 
{
    __SEH_HEADER

    reset_hand();

    __SEH_LOGFATAL("PokerPro::HandleEventEndHand : \n");
}

void PokerPro::HandleEventEndTable(PokerMessage* pm) 
{
    __SEH_HEADER


    __SEH_LOGFATAL("PokerPro::HandleEventEndTable : \n");
}

void PokerPro::HandleEventDeal(PokerMessage* pm) 
{
    __SEH_HEADER

    CString s;

    for (int i=0; i<10; i++) {
        if (pm->m_lparam[1] & (1<<i)) {
            data.m_pinf[i].m_card[0] = (unsigned char)0xff;
            data.m_pinf[i].m_card[1] = (unsigned char)0xff;
        }
    }
    s.Format("Dealing...\n");
    publish(&s, 0);

    s.Format("DEALER DEAL\n");
    writehh(&s);

    __SEH_LOGFATAL("PokerPro::HandleEventDeal : \n");
}

void PokerPro::HandleEventPlayerChat(PokerMessage* pm) 
{
    __SEH_HEADER

    CHAT_MESSAGE *pc = (CHAT_MESSAGE*)pm;
    CString s;
    s.Format("%s \"%s\"\n", pc->m_name, pc->m_text);
    publish(&s, 0);

    __SEH_LOGFATAL("PokerPro::HandleEventPlayerChat : \n");
}

void PokerPro::HandleEventPokerValue(PokerMessage* pm) 
{
    __SEH_HEADER


    CString s;

    int isflush = (pm->m_amount & 0x90000000);

    int r4 = (pm->m_amount >> 16) & 15;
    int r3 = (pm->m_amount >> 12) & 15;
    int r2 = (pm->m_amount >>  8) & 15;
    int r1 = (pm->m_amount >>  4) & 15;
    int r0 = (pm->m_amount >>  0) & 15;

    char sval[8];
    sval[0] = rank2ascii(r4);
    sval[1] = rank2ascii(r3);
    sval[2] = rank2ascii(r2);
    sval[3] = rank2ascii(r1);
    sval[4] = rank2ascii(r0);
    sval[5] = isflush ? 's' : 'o';
    sval[6] = 0;
    sval[7] = 0;

    s.Format("%s POKER %s\n", data.m_pinf[pm->m_chair&0x1f].m_name, sval);
    writehh(&s);

    __SEH_LOGFATAL("PokerPro::HandleEventPokerValue : \n");
}

void PokerPro::HandleEventRevealCard(PokerMessage* pm) 
{
    __SEH_HEADER

    CString s;

    int ccard = (pm->m_lparam[2] >> 0) & 0xff;
    int ncard = (pm->m_lparam[2] >> 8) & 0xff;
    data.m_pinf[pm->m_chair&0x1f].m_card[ncard] = ccard;

    if (ncard == 1) 
	{
        char a[4];
        char b[4];
        card2ascii(a, data.m_pinf[pm->m_chair&0x1f].m_card[0]);
        card2ascii(b, data.m_pinf[pm->m_chair&0x1f].m_card[1]);

        int round = get_round();
        const char* what = (round <= 1) ? "HAND" : "SHOW";

        s.Format("%s %s %s %s\n", data.m_pinf[pm->m_chair&0x1f].m_name, what, a, b);
        writehh(&s);
    }

    __SEH_LOGFATAL("PokerPro::HandleEventRevealCard : \n");
}

void PokerPro::HandleEventFlop(PokerMessage* pm) 
{
    __SEH_HEADER

    int ccard = (pm->m_lparam[2] >> 0) & 0xff;
    int ncard = (pm->m_lparam[2] >> 8) & 0xff;

    ncard--;

    data.m_tinf.m_card[ncard] = ccard;
    CString s, hh;

    if (ncard < 2) 
	{
        return;
    }
    if (ncard == 2) 
	{
        char a[4], b[4], c[4];
        card2ascii(a, data.m_tinf.m_card[0]);
        card2ascii(b, data.m_tinf.m_card[1]);
        card2ascii(c, data.m_tinf.m_card[2]);

        s.Format("Dealer flop %s %s %s\n", a, b, c);
        hh.Format("DEALER FLOP %s %s %s\n", a, b, c);
    }

    if (ncard == 3) 
	{
        char d[4];
        card2ascii(d, data.m_tinf.m_card[3]);
        s.Format("Dealer turn %s\n", d);
        hh.Format("DEALER TURN %s\n", d);
    }

    if (ncard == 4) 
	{
        char e[4];
        card2ascii(e, data.m_tinf.m_card[4]);
        s.Format("Dealer river %s\n", e);
        hh.Format("DEALER RIVER %s\n", e);
    }

    publish(&s, 0);
    writehh(&hh);

    __SEH_LOGFATAL("PokerPro::HandleEventFlop : \n");
}

void PokerPro::HandleEventPlayerList(PokerMessage* pm) 
{
    __SEH_HEADER

    int seatedbits	= pm->m_lparam[1];
    int activebits	= pm->m_lparam[2];
    int playingbits	= pm->m_lparam[3];

    for (int i=0; i<10; i++) 
	{
        data.m_pinf[i].m_isSeated = (seatedbits & (1<<i)) ? 1 : 0;
        data.m_pinf[i].m_isActive = (activebits & (1<<i)) ? 1 : 0;
        data.m_pinf[i].m_isPlaying = (playingbits & (1<<i)) ? 1 : 0;
    }

    __SEH_LOGFATAL("PokerPro::HandleEventPlayerList : \n");
}

void PokerPro::HandleAccount(PokerMessage* pm) 
{
    __SEH_HEADER

    CString s;

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
    publish(&s, 0);

    s = s.MakeLower();
    writehh(&s);

    __SEH_LOGFATAL("PokerPro::HandleAccount : \n");
}

void PokerPro::HandleEventStarted(PokerMessage* pm) 
{
    __SEH_HEADER

    __SEH_LOGFATAL("PokerPro::HandleEventStarted : \n");
}

void PokerPro::HandleEventStopped(PokerMessage* pm) 
{
    __SEH_HEADER

    __SEH_LOGFATAL("PokerPro::HandleEventStopped : \n");
}

void PokerPro::HandleEventTournament(PokerMessage* pm) 
{
    __SEH_HEADER

    CString s;

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
        s.Format("%s PLACED %d%s", data.m_pinf[chair].m_name, place, get_cardinal_suffix(place));
    }
    break;
    }
    publish(&s, 0);

    __SEH_LOGFATAL("PokerPro::HandleEventTournament : \n");
}

void PokerPro::HandleEventTableLock(PokerMessage* pm) 
{
    __SEH_HEADER

    //const char* msg = pm->m_lparam[1] ? "locked" : "unlocked";

    __SEH_LOGFATAL("PokerPro::HandleEventTableLock : \n");
}

void PokerPro::HandleEventPing(PokerMessage* pm) 
{
    __SEH_HEADER

    pm->m_event = 'PONG';
    send_message((char *) pm, sizeof(PokerMessage) );

    __SEH_LOGFATAL("PokerPro::HandleEventPing : \n");
}

char* PokerPro::sassign(char* dst, const char* src, int dstsiz) 
{
    __SEH_HEADER

    strncpy( dst, src, dstsiz );
    dst[dstsiz-1]=0;
    return dst;

    __SEH_LOGFATAL("PokerPro::sassign : \n");
}

char* PokerPro::convert_money(char* text, int amount) 
{
    __SEH_HEADER

    int units = amount/100;
    int cents = amount%100;

    if (amount < 0)	
	{
        sprintf(text,"%d.%02d", units, cents);
    }
    if (amount < 0)	
	{
        sprintf(text,"0");
    }
    else if (amount < 100) 
	{
        sprintf(text,".%02d", cents);
    }
    else if (cents) 
	{
        sprintf(text,"%d.%02d", units, cents);
    }
    else 
	{
        sprintf(text,"%d", units);
    }

    return text;

    __SEH_LOGFATAL("PokerPro::convert_money : \n");
}

void PokerPro::reset_hand(void) 
{
    __SEH_HEADER

    int i;

    for (i=0; i<=4; i++) 
	{
        data.m_tinf.m_card[i]=0;
    }
    for (i=0; i<=9; i++) 
	{
        data.m_pinf[i].m_card[0]=0;
        data.m_pinf[i].m_card[1]=0;
        data.m_pinf[i].m_betAmount=0;
    }
    memset(data.m_pot,0,sizeof(data.m_pot));
    data.m_prev_rais = 0;

    __SEH_LOGFATAL("PokerPro::reset_hand : \n");
}

int PokerPro::get_current_bet(void) 
{
    __SEH_HEADER

    int bet=0;
    for (int i=0; i<10; i++) 
	{
        int _bet = data.m_pinf[i].m_betAmount;
        if (_bet > bet) 
		{
            bet = _bet;
        }
    }
    return bet;

    __SEH_LOGFATAL("PokerPro::get_current_bet : \n");
}

int PokerPro::get_betx(void) 
{
    __SEH_HEADER

    return (get_round() >= 3) ? data.m_tinf.m_betMax : data.m_tinf.m_betMin;

    __SEH_LOGFATAL("PokerPro::get_betx : \n");
}

int PokerPro::get_round(void) 
{
    __SEH_HEADER

    int round=0;
    int ncc=get_ncc();
    int npd=get_npd();

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

    __SEH_LOGFATAL("PokerPro::get_round : \n");
}

int PokerPro::get_ncc(void) 
{
    __SEH_HEADER

    int ncc=0;
    for (int i=0; i<5; i++) 
	{
        if (data.m_tinf.m_card[i]) 
		{
            ncc++;
        }
    }
    return ncc;

    __SEH_LOGFATAL("PokerPro::get_ncc : \n");
}

int PokerPro::get_npd(void) 
{
    __SEH_HEADER

    int npd=0;
    for (int i=0; i<=9; i++) 
	{
        if (data.m_pinf[i].m_card[0] == 0 && data.m_pinf[i].m_card[1] == 0) 
		{
            continue;
        }
        npd++;
    }
    return npd;

    __SEH_LOGFATAL("PokerPro::get_npd : \n");
}

void PokerPro::DoScrape(void) 
{
	// No critical section for CScraper is required in this function, as the only place this function is called is from the
	// heartbeat thread, and the heartbeat thread wraps the call to this function in a critical section itself

    __SEH_HEADER

    int			i, j;
    int			suit, rank;
    char		money[50];

    // Common cards
    for (i=0; i<=4; i++) 
	{
        p_scraper->set_card_common(i, CARD_NOCARD);

		if ((data.m_tinf.m_card[i]&0xff) == WH_CARDBACK)
		{
            p_scraper->set_card_common(i, CARD_BACK);
		}
        else if ((data.m_tinf.m_card[i]&0xff) == WH_NOCARD) 
		{
            p_scraper->set_card_common(i, CARD_NOCARD);
        }
        else 
		{
            if (((data.m_tinf.m_card[i]>>0)&0x0f) == WH_SUIT_CLUBS) 
                suit = StdDeck_Suit_CLUBS;

            else if (((data.m_tinf.m_card[i]>>0)&0x0f) == WH_SUIT_DIAMONDS) 
                suit = StdDeck_Suit_DIAMONDS;

            else if (((data.m_tinf.m_card[i]>>0)&0x0f) == WH_SUIT_HEARTS) 
                suit = StdDeck_Suit_HEARTS;

            else if (((data.m_tinf.m_card[i]>>0)&0x0f) == WH_SUIT_SPADES) 
                suit = StdDeck_Suit_SPADES;

            rank = ((data.m_tinf.m_card[i]>>4)&0x0f)-2;

            p_scraper->set_card_common(i, StdDeck_MAKE_CARD(rank, suit));
        }
    }

    // Player information
    for (i=0; i<=9; i++) 
	{

        // Player cards
        for (j=0; j<=1; j++) 
		{
            p_scraper->set_card_player(i, j, CARD_NOCARD);

            if (data.m_pinf[i].m_isPlaying&0x1) 
			{
                if ((data.m_pinf[i].m_card[j]&0xff) == WH_CARDBACK) 
				{
                    p_scraper->set_card_player(i, j, CARD_BACK);
                }
                else if ((data.m_pinf[i].m_card[j]&0xff) == WH_NOCARD) 
				{
                    p_scraper->set_card_player(i, j, CARD_NOCARD);
                }
                else 
				{
                    if (((data.m_pinf[i].m_card[j]>>0)&0x0f) == WH_SUIT_CLUBS)
                        suit = StdDeck_Suit_CLUBS;

                    else if (((data.m_pinf[i].m_card[j]>>0)&0x0f) == WH_SUIT_DIAMONDS)
                        suit = StdDeck_Suit_DIAMONDS;

                    else if (((data.m_pinf[i].m_card[j]>>0)&0x0f) == WH_SUIT_HEARTS)
                        suit = StdDeck_Suit_HEARTS;

                    else if (((data.m_pinf[i].m_card[j]>>0)&0x0f) == WH_SUIT_SPADES)
                        suit = StdDeck_Suit_SPADES;

                    rank = ((data.m_pinf[i].m_card[j]>>4)&0x0f)-2;

                    p_scraper->set_card_player(i, j, StdDeck_MAKE_CARD(rank, suit));
                }
            }
        }

        p_scraper->set_seated(i, (data.m_pinf[i].m_isSeated&0x1) ? "true" : "false");
        p_scraper->set_active(i, (data.m_pinf[i].m_isActive&0x1) ? "true" : "false");

        p_scraper->set_dealer(i, false);
        p_scraper->set_player_name(i, data.m_pinf[i].m_name);
        p_scraper->set_name_good_scrape(i, true);
        convert_money(money, data.m_pinf[i].m_balance);
        p_scraper->set_player_balance(i, atof(money));
        p_scraper->set_balance_good_scrape(i, true);
        convert_money(money, data.m_pinf[i].m_betAmount);
        p_scraper->set_player_bet(i, atof(money));
        p_scraper->set_sitting_out(i, !(data.m_pinf[i].m_isPlaying&0x1));

        // Clear some things if no one is at this chair
        if (!p_scraper->IsStringSeated(p_scraper->seated(i)) & !p_scraper->IsStringActive(p_scraper->active(i))) 
		{
            p_scraper->set_player_name(i, "");
            p_scraper->set_name_good_scrape(i, false);
            p_scraper->set_player_balance(i, 0.);
            p_scraper->set_balance_good_scrape(i, false);
        }

        // Pots
        convert_money(money, data.m_pot[i]);
        p_scraper->set_pot(i, atof(money));
    }

    p_scraper->set_dealer(data.m_tinf.m_activeDealer&0xf, true);

	// Limit info
	SLimitInfo LI;
	LI.handnumber = data.m_handnumber;
	LI.found_handnumber = true;

	convert_money(money, data.m_ginf[data.m_tinf.m_tid].m_sblind);
	LI.sblind = atof(money);
	LI.found_sblind = true;
    convert_money(money, data.m_ginf[data.m_tinf.m_tid].m_minbet);
	LI.bblind = atof(money);
    LI.found_bblind = true;
    convert_money(money, data.m_ginf[data.m_tinf.m_tid].m_maxbet);
    LI.bbet = atof(money);
	LI.found_bbet = true;

    convert_money(money, 0/*data.m_ginf[data.m_tinf.m_tid].*/);  // don't know where to get ante info from
    LI.ante = atof(money);
    LI.found_ante = true;

    LI.limit = (data.m_ginf[data.m_tinf.m_tid].m_lim&0x7)==0 ? LIMIT_NL :
			   (data.m_ginf[data.m_tinf.m_tid].m_lim&0x7)==1 ? LIMIT_PL : 
			   LIMIT_FL;
    LI.found_limit = true;

    LI.istournament = (data.m_ginf[data.m_tinf.m_tid].m_tmode&0x1) && data.m_ginf[data.m_tinf.m_tid].m_tlevel;

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
    if (autoplayer_can_act) 
	{
        p_scraper->set_button_state(3, p_scraper->s_limit_info()->limit==LIMIT_NL ? "true" : "false");
        p_scraper->set_button_state(2, 
			data.m_pinf[data.m_userchair].m_balance > get_current_bet() - data.m_pinf[data.m_userchair].m_betAmount &&
			((p_scraper->s_limit_info()->limit==LIMIT_NL || p_scraper->s_limit_info()->limit==LIMIT_PL) ||
			  get_current_bet() < (data.m_tinf.m_maxNBets&0xf)*get_betx()) ? "true" : "false");
        p_scraper->set_button_state(1, get_current_bet() - data.m_pinf[data.m_userchair].m_betAmount > 0 ? "true" : "false");
        p_scraper->set_button_state(4, get_current_bet() - data.m_pinf[data.m_userchair].m_betAmount == 0 ? "true" : "false");
        p_scraper->set_button_state(0, p_scraper->button_state(4)=="false" ? "true" : "false");
    }
    p_scraper->set_button_state(5, (data.m_pinf[data.m_userchair].m_isActive) ? "false" : "true");
    p_scraper->set_button_state(6, (data.m_pinf[data.m_userchair].m_isActive) ? "true" : "false");
    p_scraper->set_button_state(7, "true");

    __SEH_LOGFATAL("PokerPro::DoScrape : \n");
}

// Copied from CAutoplayer.cpp to be used for f$delay (4-4-2008) Spektre
int PokerPro::count_same_scrapes(void) 
{
    __SEH_HEADER

    int						i;
    static unsigned int		card_common_last[5] = {0};
    static unsigned int		card_player_last[10][2] = {0};
    static bool				dealer_last[10] = {0};
    static double			playerbalance_last[10] = {0};
    static double			playerbet_last[10] = {0};
    static double			myturnbitslast = 0;
    bool					same_scrape;
    static int				num_same_scrapes=0;

	int sym_myturnbits = (int) p_symbols->sym()->myturnbits;

	// These items need to be the same to count as a identical frame:
	// - up and down cards
	// - button position
	// - playerbets
	// - playerbalances
	// - button states
	same_scrape = true;
	for (i=0; i<=4; i++)
	{
		if (p_scraper->card_common(i) != card_common_last[i])  same_scrape = false;
	}

	for (i=0; i<=9; i++)
	{
		if (p_scraper->card_player(i, 0) != card_player_last[i][0])  same_scrape = false;
		if (p_scraper->card_player(i, 1) != card_player_last[i][1])  same_scrape = false;
		if (p_scraper->dealer(i) != dealer_last[i])  same_scrape = false;
		if (p_scraper->player_balance(i) != playerbalance_last[i])  same_scrape = false;
		if (p_scraper->player_bet(i) != playerbet_last[i])  same_scrape = false;
	}

	if (sym_myturnbits != myturnbitslast)  same_scrape = false;

	if (same_scrape)
	{
		num_same_scrapes++;
	}
	else
	{
		for (i=0; i<=4; i++)
		{
			card_common_last[i] = p_scraper->card_common(i);
		}
		for (i=0; i<=9; i++)
		{
			card_player_last[i][0] = p_scraper->card_player(i, 0);
			card_player_last[i][1] = p_scraper->card_player(i, 1);
			dealer_last[i] = p_scraper->dealer(i);
			playerbalance_last[i] = p_scraper->player_balance(i);
			playerbet_last[i] = p_scraper->player_bet(i);
		}
		myturnbitslast = sym_myturnbits;
		num_same_scrapes = 0;
	}

	return num_same_scrapes;

    __SEH_LOGFATAL("PokerPro::count_same_scrapes :\n");
}


void PokerPro::DoAutoplayer(void) 
{
    __SEH_HEADER

    int				x, delay;

	int sym_myturnbits = (int) p_symbols->sym()->myturnbits;
	int sym_br = (int) p_symbols->sym()->br;

	// Calculate f$play, f$prefold, f$delay and f$chat for use below
	p_symbols->CalcSecondaryFormulas();

	////////////////////////////////////////////////////////////////////////////////
	// f$play
	if (p_symbols->f$play()==-2) 
	{
		send_stand(data.m_userchair);	   // leave table
	}
	else if (p_symbols->f$play()==-1) 
	{ 
	}										// no action
																
	else if (p_symbols->f$play()==0 && p_scraper->GetButtonState(6)) 
	{
		send_sitout(data.m_userchair);    // sit out
	}

	else if (p_symbols->f$play()==1 && p_scraper->GetButtonState(5)) 
	{
		send_sitin(data.m_userchair);    // sit in
	}

	// Get count of stable frames for use a little bit further down
	x = count_same_scrapes();

	// If iterator thread is still iterating, then return
	if (_iter_vars.iterator_thread_running) 
		return;

	// if we have no visible buttons, then return
	if (!sym_myturnbits) 
		return;

	// If we don't have enough stable frames, or have not waited f$delay milliseconds, then return (added Spektre 2008-04-03)
	delay = p_symbols->f$delay() / p_global->preferences.scrape_delay;    // scale f$delay to a number of scrapes

	if (x < (int) p_global->preferences.frame_delay + delay) 
		return;

	// Now that we got through all of the above, we are ready to evaluate the primary formulas
	// and take the appropriate action
	p_symbols->CalcPrimaryFormulas(true);

	// take action
	if (p_symbols->f$alli() && p_scraper->GetButtonState(3) && autoplayer_can_act) 
	{
		Sleep(500);
		send_action('ALLI', 0);
		autoplayer_can_act = false;
		p_global->set_replay_recorded_this_turn(false);
		Sleep(500);
		p_autoplayer->set_prevaction(PREVACT_ALLI);
	}
	else if (p_symbols->f$swag() && p_scraper->GetButtonState(2) && autoplayer_can_act) 
	{
		Sleep(500);
		send_action('SBET', (int) (p_symbols->f$swag()*100));
		autoplayer_can_act = false;
		p_global->set_replay_recorded_this_turn(false);
		Sleep(500);
		p_autoplayer->set_didswag(4, 1);
		p_autoplayer->set_didswag(sym_br-1, 1);
		p_autoplayer->set_prevaction(PREVACT_SWAG);
	}
	else if (p_symbols->f$rais() && p_scraper->GetButtonState(2) && autoplayer_can_act) 
	{
		Sleep(500);
		send_action('RAIS', 0);
		autoplayer_can_act = false;
		p_global->set_replay_recorded_this_turn(false);
		Sleep(500);
		p_autoplayer->set_didrais(4, 1);
		p_autoplayer->set_didrais(sym_br-1, 1);
		p_autoplayer->set_prevaction(PREVACT_RAIS);
	}
	else if (p_symbols->f$call() && p_scraper->GetButtonState(1) && autoplayer_can_act) 
	{
		Sleep(500);
		send_action('CALL', 0);
		autoplayer_can_act = false;
		p_global->set_replay_recorded_this_turn(false);
		Sleep(500);
		p_autoplayer->set_didcall(4, 1);
		p_autoplayer->set_didcall(sym_br-1, 1);
		p_autoplayer->set_prevaction(PREVACT_CALL);
	}
	else if (p_scraper->GetButtonState(4) && autoplayer_can_act) 
	{
		Sleep(500);
		send_action('CHEC', 0);
		autoplayer_can_act = false;
		p_global->set_replay_recorded_this_turn(false);
		Sleep(500);
		p_autoplayer->set_didchec(4, 1);
		p_autoplayer->set_didchec(sym_br-1, 1);
		p_autoplayer->set_prevaction(PREVACT_CHEC);
	}
	else if (autoplayer_can_act) 
	{
		Sleep(500);
		send_action('FOLD', 0);
		p_global->set_replay_recorded_this_turn(false);
		autoplayer_can_act = false;
		Sleep(500);
		p_autoplayer->set_prevaction(PREVACT_FOLD);
	}

	p_symbols->UpdateAutoplayerInfo();

	__SEH_LOGFATAL("PokerPro::DoAutoplayer : \n");
}

void PokerPro::publish(CString *text, int flags) 
{
    __SEH_HEADER


    // limit size of message box to 100 lines
    int i, count = 1;
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
        MessageBox(NULL, *text, "PPRO: ERROR", MB_OK | MB_TOPMOST);
    }

    __SEH_LOGFATAL("PokerPro::publish : \n");
}

const char* PokerPro::get_action(int chair, int spend) 
{
    __SEH_HEADER

    int current = data.m_pinf[chair].m_betAmount;
    int round = get_round();
    int call = get_current_bet();
    int playerpot = get_player_pot();

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

    __SEH_LOGFATAL("PokerPro::get_action : \n");
}

int PokerPro::get_player_pot(void) 
{
    __SEH_HEADER

    int sum=0;
    for (int i=0; i<=9; i++) 
	{
        sum += data.m_pinf[i].m_betAmount;
    }

    return sum;

    __SEH_LOGFATAL("PokerPro::get_player_pot : \n");
}

char* PokerPro::card2ascii(char* text, unsigned char cc) 
{
    __SEH_HEADER

    const char* srank = " 123456789TJQKA|";
    const char* ssuit = " cdhs----------|";

    int rank = (cc >> 4) & 15;
    int suit = (cc >> 0) & 15;

    text[0] = srank[rank];
    text[1] = ssuit[suit];
    text[2] = 0;

    return text;

    __SEH_LOGFATAL("PokerPro::card2ascii : \n");
}

char PokerPro::rank2ascii(int rank) 
{
    __SEH_HEADER

    const char* srank = " 123456789TJQKA|";
    char ascii = srank[rank];
    return ascii;

    __SEH_LOGFATAL("PokerPro::rank2ascii : \n");
}

void PokerPro::writehh(CString *s) 
{
    __SEH_HEADER

    CString fn, fd;
    FILE *hh_fp;

    if (handhistory) 
	{
        fn.Format("%s\\ppro\\%s_%lu.log", _startup_path, data.m_site_name, p_global->session_id());
        hh_fp = fopen(fn.GetString(), "a");
        if (hh_fp==NULL) 
		{
            fd.Format("%s\\ppro", _startup_path);
            CreateDirectory(fd.GetString(), NULL);
            hh_fp = fopen(fn.GetString(), "a");
        }

        if (hh_fp!=NULL) 
		{
            fprintf(hh_fp, "%s", s->GetString());
            fflush(hh_fp);
            fclose(hh_fp);
        }
    }

    __SEH_LOGFATAL("PokerPro::writehh : \n");
}

const char* PokerPro::get_cardinal_suffix(int n) 
{
    __SEH_HEADER

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

    __SEH_LOGFATAL("PokerPro::get_cardinal_suffix : \n");
}