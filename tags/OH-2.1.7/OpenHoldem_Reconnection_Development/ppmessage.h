/*
	filename	:	ppmessage.h

	author		:	ray bornert

	copyright	:	copyright © 2007-FEB-01
					all rights reserved

	license		:	private non-commercial use for:
					the poker professional platform
					the pokerbot world championship

	comments	:

	this is the official ppro server/client message structure
	all messages are exactly 64 bytes by default regardless of
	the actual number of relevant bytes used for information.

	the following c data types are used:
	int		- assumed to be 4 bytes
	short	- assumed to be 2 bytes
	char	- assumed to be 1 bytes

	if your compiler does not use these sizes then you will need
	to make the compiler specific adjustments to reference the
	correct 1 and 2 and 4 byte data types.

	you can use the following c printf statement to discover the
	facts about your specific c/c++ compiler:

	printf("sizeof(char)=%d sizeof(short)=%d sizeof(int)=%d\n",
		sizeof(char),sizeof(short),sizeof(int));

	some (but not all) of the field offsets have been named below.
	those that have not been named will usually be referenced in
	the source code as
	m_lparam[n]

	m_lparam[0] will always contain the pertinent message tag that
	allows the source code to understand how to process the contents.
	message tags are typically 4 byte literals as in 'FOLD'

	the server was developed on intel hardware and thus any and all
	byte ordering is standard intel little endian by default. if you
	are compiling a client on big endian hardware then you will need
	to supply your own transform code for both incoming and outgoing
	messages.

	in all cases you need to refer to the actual source code if you
	want to gain a full understanding of how to process each message.
*/


#ifndef _POKERMESSAGE_H_
#define _POKERMESSAGE_H_

#define PROTOCOL_VERSION	(0x00000002) // 2006-JAN-12
#define ppsock__msgsizmin		(64)
#define ppsock__msgsizmax		(256)
#define ppsock__nmsgmax			(256)

//optional line to prevent the compiler from changing sizes and offsets
//#pragma pack(1)

class PokerMessage
{

public:	//MEMBERS

union
{
	int		m_lparam[16];	//=64 bytes
	short	m_sparam[32];	//=64 bytes
	char	m_cparam[64];	//=64 bytes

	struct
	{
		int		m_fill0;			//INT 0
		int		m_fill1;			//INT 1

		union
		{
			char	m_text[52];			//chat text
			struct
			{
			char	m_name[16];			//either casino or player
			char	m_pass[16];			//either casino or player
			};
		};

		//INT 15
		union
		{
		int		m_cid;				//connection id user by server
		int		m_poid;				//player object id
		};
	};

	struct
	{
		union	//INT 0
		{
		int		m_event;			//from client to server
		int		m_state;			//from server to client
		};

		union	//INT 1
		{
		int		m_handle;			//future use
		int		m_chair;			//chair number 0-9
		int		m_pot;				//pot number 0-9
		int		m_chairBits;		//each bit 9-0 denotes a chair
		int		m_minBet;			//table stakes
		int		m_location;			//target server side location (usually a table) 0=lobby
		int		m_version;			//version number
		};

		union	//INT 2
		{
		int		m_amount;			//bet amount
		int		m_cardCode;			//binary encoding for a specific card
		int		m_nCardsPerPlayer;	//for the deal
		int		m_maxBet;			//table stakes
		int		m_balance;			//player balance
		int		m_currentBet;		//player current bet
		};

		union	//INT 4
		{
		int		m_nRounds;			//number of rounds per hand
		};
	};
};

public:	//METHODS
	
	PokerMessage(){}
	~PokerMessage(){}

};

struct GINF
{
	int m_tag;			//0
	int m_tid;			//1
	char m_name[16];	//2-5

	int m_sblind;		//6
	int m_minbet;		//7
	int m_maxbet;		//8

	char m_nChairs;
	char m_nSeated;
	char m_pokerid;
	char m_moneyid;		//9

	char m_lim;
	char m_tmode;
	char m_tlevel;
	char fillerc;		//10

	int m_filler[4];	//11-14

	int m_poid;			//15
};

struct TINF
{
	int m_tag;			//0
	int m_betSBlind;	//1
	int m_betMin;		//2
	int m_betMax;		//3
	int m_potAmount;	//4

						//5
	int m_activeDealer	: 4;
	int m_activePlayer	: 4;
	int m_maxNBets		: 4;
	int m_maxNRounds	: 4;
	int m_pause			: 1;
	int m_lim			: 3;
	int m_tmode			: 1;
	int m_locked		: 1;
	int m_fillerbits	:10;

	char m_nCards;		//6-7
	char m_card[7];

	char m_name[16];	//8-11
	int m_tid;			//12
	int m_filler[2];	//13-14

	int m_poid;			//15
};

struct PINF
{
	int m_tag;			//0
						//1

	int m_chair		:	5;
	int m_isSeated	:	1;
	int m_isActive	:	1;
	int m_isPlaying	:	1;
	int m_filler8	:	8;
	int m_filler16	:	16;

	int m_balance;		//2
	int m_betAmount;	//3
	int m_handValue;	//4

						//5-6
	char m_nCards;
	char m_card[7];

	char m_name[32];	//7-14

	int m_poid;		//15
};

struct CHAT_MESSAGE
{
	int m_tag;			//'CHAT'
	char m_name[16];	//login/speaker
	char m_text[236];	//actual text
};

//optional line to restore the compiler default pack
//#pragma pack(0)


#endif
