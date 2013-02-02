#ifndef __INPUT_SERVER_PROTO_H__
#define __INPUT_SERVER_PROTO_H__

#include <wtypes.h>
//#ifndef MouseButtonDefine
//#define MouseButtonDefine
//enum MouseButton { MouseLeft, MouseMiddle, MouseRight };
//#endif //MouseButtonDefine
enum EInputRequestAuthorizer {EINP_AUTHORIZER_BOT, EINP_AUTHORIZER_SCOUT};


#ifndef MAX_TCP_MESSAGE_LENGTH
#define MAX_TCP_MESSAGE_LENGTH 1000
#endif //MAX_TCP_MESSAGE_LENGTH

#define INPUT_SERVER_PROTO_MAX_MSG_HEADER		0xf00dfeed
#define INPUT_SERVER_PROTO_MAX_MSG_SIZE			512
#define INPUT_SERVER_PROTO_MAX_ROOMNAME_SIZE	140
#define INPUT_SERVER_PROTO_MAX_SENDSTRING_SIZE	10
#define INPUT_SERVER_PROTO_HEADER_SIZE			8

const unsigned char g_inputHeader[4] = {0xf0, 0x0d, 0xfe, 0xed};


#define CMDTYPE_ACK						0x0007
#define CMDTYPE_MUTEXLOCK				0x000b
#define CMDTYPE_MUTEXUNLOCK 			0x000c
#define CMDTYPE_REGISTER_FOR_MUTEX		0x000e
#define CMDTYPE_REGISTER_FOR_MUTEX_ACK	0x000f
#define CMDTYPE_MUTEX_GRANTED			0x0010






struct SInputServerProtoAckMsg
{
	char						m_msgHeader[4];
	short						m_cmdType;
	short						m_msgSize;
	BOOL						m_ack;
	EInputRequestAuthorizer		m_authorizer;
};

struct SInputServerProtoMutexLockMsg
{
	char			m_msgHeader[4];
	short			m_cmdType;
	short			m_msgSize;
	DWORD			m_dwTimeout; //Timeout for mutex grabbing, 0 = infinity.
	int				m_session_id;
	unsigned int	m_leaseTime;
	char			m_ACKAddress[24]; //ACK/NACK address Null terminated string, may be DNS or IP
	int				m_ACKPort; //ACK/NACK address 

};

struct SInputServerProtoMutexUnLockMsg
{
	char			m_msgHeader[4];
	short			m_cmdType;
	short			m_msgSize;
	int				m_session_id; //TripleSeven 24/12/2012: SyncServer changes
};


struct SInputServerProtoRegisterForMutexMsg
{
	char			m_msgHeader[4];
	short			m_cmdType;
	short			m_msgSize;
	int				m_session_id;
	unsigned int	m_leaseTime;
	SOCKADDR		m_senderAddr;
};

struct SInputServerProtoRegisterForMutexAckMsg
{
	char						m_msgHeader[4];
	short						m_cmdType;
	short						m_msgSize;
	unsigned int				m_waitingListSize;
};

struct SInputServerProtoMutexGrantedMsg
{
	char						m_msgHeader[4];
	short						m_cmdType;
	short						m_msgSize;
	DWORD						m_timeStamp;
};


class CGeneralInputServerProtoMsg
{
public:
	
	
	short										GetCmdType(){return m_protoMessage[4];}
	int											HeaderFinder(int size);
	void										EncodeMsg(unsigned short inputServerKey = CGeneralInputServerProtoMsg::m_inputServerProtoKey);
	void										DecodeMsg(unsigned short inputServerKey = CGeneralInputServerProtoMsg::m_inputServerProtoKey);
	
	
public:
	union
	{
		char									m_protoMessage[INPUT_SERVER_PROTO_MAX_MSG_SIZE + MAX_TCP_MESSAGE_LENGTH];
		SInputServerProtoAckMsg					m_ackMsg;
		SInputServerProtoMutexLockMsg			m_mutexLockMsg;
		SInputServerProtoMutexUnLockMsg			m_mutexUnlockMsg;
		SInputServerProtoRegisterForMutexMsg	m_registerForMutexMsg;
		SInputServerProtoRegisterForMutexAckMsg	m_registerForMutexAckMsg;
		SInputServerProtoMutexGrantedMsg		m_mutexGrantedMsg;
	};
	
	static unsigned short						m_inputServerProtoKey;
private:
	unsigned short								EncodeShort(unsigned short keyFromServer, unsigned short data);
	unsigned short								DecodeShort(unsigned short keyFromServer, unsigned short codedData);
	void										EncodeEvenDataBuffer(char* data, int size, unsigned short keyFromServer);
	void										DecodeEvenDataBuffer(char* data, int size, unsigned short keyFromServer);
	int											MessageSizeToEncode(short cmdType);

	

};

#endif //__INPUT_SERVER_PROTO_H__