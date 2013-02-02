#include "StdAfx.h"
#include <complex>

#include "OpenHoldem.h"
#include "UDPSocketSender.h"
#include "InputServerProto.h"
#include "CSessionCounter.h"
#include "CSyncServerLink.h"


void CSyncServerLink::Clear()
{
	m_mutexState = EMUTEX_STATE_NO_REQUEST;
}

CSyncServerLink::CSyncServerLink(BOOL bInitiallyOwn /* = FALSE */, LPCTSTR lpszName /* = NULL */, LPSECURITY_ATTRIBUTES lpsaAttribute /*= NULL*/)
{
	Clear();
	int port = atoi(lpszName);
	write_log(3, "CSyncServerLink C'tor: Using port[%d]\n", port);
	Init(port);
}


CSyncServerLink::~CSyncServerLink()
{
	Clear();
}


int CSyncServerLink::Lock(int tryLockTime, int leaseTime /* = DEAFULT_BOT_LEASE_TIME */)
{
	write_log(1, "CSyncServerLink::Lock: Function entered\n");
	if (EMUTEX_STATE_NO_REQUEST == m_mutexState)
	{
		return LockOnNoRequest(tryLockTime, leaseTime);
	}
	else if (EMUTEX_STATE_PENDING == m_mutexState)
	{
		return LockOnPendingRequest(tryLockTime, leaseTime);
	}
	else if (EMUTEX_STATE_GRANTED == m_mutexState)
	{
		write_log(1, "CSyncServerLink::Lock: WARNING, lock requested when already granted\n");
		return 1;
	}
	return 0; 
}


void CSyncServerLink::Unlock()
{
	write_log(1, "CSyncServerLink::Unlock: Function entered\n");
	SendReleaseNotification();
	SetMutexState("Unlock", EMUTEX_STATE_NO_REQUEST);
}


int CSyncServerLink::LockOnNoRequest(int tryLockTime, int leaseTime)
{
	short msgType;
	write_log(1, "CSyncServerLink::LockOnNoRequest: Function entered\n");
	SendLockRequest(leaseTime);
	msgType = ReceiveResponse(200); 
	if (CMDTYPE_REGISTER_FOR_MUTEX_ACK == msgType)
	{
		SetMutexState("LockOnNoRequest", EMUTEX_STATE_PENDING);
		return LockOnPendingRequest(tryLockTime, leaseTime);
	}
	else if (CMDTYPE_MUTEX_GRANTED == msgType)
	{
		write_log(1, "CSyncServerLink::LockOnNoRequest: WARNING: CMDTYPE_MUTEX_GRANTED Received unexpectedly\n");
		DWORD timePassed = GetTickCount() - m_lastMsgReceived.m_mutexGrantedMsg.m_timeStamp;
		if (timePassed < ((DWORD)leaseTime - 1500))
		{
			write_log(1, "CSyncServerLink::LockOnNoRequest: CMDTYPE_MUTEX_GRANTED received, timePassed[%d]\n", timePassed);
			SetMutexState("LockOnNoRequest", EMUTEX_STATE_GRANTED);
			return 1;
		}
		else
		{
			write_log(1, "CSyncServerLink::LockOnNoRequest: CMDTYPE_MUTEX_GRANTED received, but mutex was granted [%d] ms ago. requesting again.\n", timePassed);
			Unlock(); //Don't keep Lockserver waiting
			return Lock(tryLockTime);
		}
	}
	else if (0 == msgType)
	{
		write_log(1, "CSyncServerLink::LockOnNoRequest: Timed out\n");
	}
	else
	{
		write_log(1, "CSyncServerLink::LockOnNoRequest: unknown message received unexpectedly or error occurred\n");
	}
	return 0;
}

int CSyncServerLink::LockOnPendingRequest(int tryLockTime, int leaseTime)
{
	short msgType;
	write_log(1, "CSyncServerLink::LockOnPendingRequest: Function entered\n");
	msgType = ReceiveResponse(tryLockTime);
	if (CMDTYPE_MUTEX_GRANTED == msgType)
	{
		DWORD timePassed = GetTickCount() - m_lastMsgReceived.m_mutexGrantedMsg.m_timeStamp;
		if (timePassed < (DWORD)leaseTime - 1500)
		{
			write_log(1, "CSyncServerLink::LockOnPendingRequest: CMDTYPE_MUTEX_GRANTED received, timePassed[%d]\n", timePassed);
			SetMutexState("LockOnPendingRequest", EMUTEX_STATE_GRANTED);
			return 1;
		}
		else
		{
			write_log(1, "CSyncServerLink::LockOnPendingRequest: CMDTYPE_MUTEX_GRANTED received, but mutex was granted [%d] ms ago. requesting again.\n", timePassed);
			Unlock();
			return Lock(tryLockTime, leaseTime);
		}
	}
	else if (CMDTYPE_REGISTER_FOR_MUTEX_ACK == msgType)
	{
		write_log(1, "CSyncServerLink::LockOnPendingRequest: CMDTYPE_REGISTER_FOR_MUTEX_ACK received unexpectedly\n");
	}
	else if (0 == msgType)
	{
		write_log(1, "CSyncServerLink::LockOnPendingRequest: Timed out\n");
	}
	else
	{
		write_log(1, "CSyncServerLink::LockOnPendingRequest: unknown message received unexpectedly or error occurred\n");
	}
	return 0;
}


int CSyncServerLink::Init(int port)
{
	m_SyncServerUDPLink = new CUDPSocketSender();
	m_SyncServerUDPLink->Init("127.0.0.1", port);
	return 1;
}

int CSyncServerLink::Term()
{
	m_SyncServerUDPLink->Term();
	delete m_SyncServerUDPLink;
	m_SyncServerUDPLink = NULL;
	return 1;
}


void CSyncServerLink::SendLockRequest(int leaseTime)
{
	write_log(1, "CSyncServerLink::SendLockRequest: Function entered\n");
	CGeneralInputServerProtoMsg msg;

	memcpy(msg.m_registerForMutexMsg.m_msgHeader, g_inputHeader, 4);
	msg.m_registerForMutexMsg.m_cmdType		= CMDTYPE_REGISTER_FOR_MUTEX;
	msg.m_registerForMutexMsg.m_msgSize		= sizeof(SInputServerProtoRegisterForMutexMsg) - INPUT_SERVER_PROTO_HEADER_SIZE;
	msg.m_registerForMutexMsg.m_session_id	= p_sessioncounter->session_id();
	msg.m_registerForMutexMsg.m_leaseTime	= leaseTime;
	m_SyncServerUDPLink->SendBinary(msg.m_protoMessage, sizeof(SInputServerProtoRegisterForMutexMsg));
}

void CSyncServerLink::SendReleaseNotification()
{
	write_log(1, "CSyncServerLink::SendReleaseNotification: Function entered\n");
	CGeneralInputServerProtoMsg msg;

	memcpy(msg.m_mutexUnlockMsg.m_msgHeader, g_inputHeader, 4);
	msg.m_mutexUnlockMsg.m_cmdType			= CMDTYPE_MUTEXUNLOCK;
	msg.m_mutexUnlockMsg.m_msgSize			= sizeof(SInputServerProtoMutexUnLockMsg) - INPUT_SERVER_PROTO_HEADER_SIZE;
	msg.m_mutexUnlockMsg.m_session_id		= p_sessioncounter->session_id();
	m_SyncServerUDPLink->SendBinary(msg.m_protoMessage, sizeof(SInputServerProtoMutexUnLockMsg));
}

short CSyncServerLink::ReceiveResponse(int tryLockTime)
{
	write_log(1, "CSyncServerLink::ReceiveResponse: Function entered, dwMiliseconds[%d]\n", tryLockTime);
	int								bytesRecvd;
	short							cmdType;
	bytesRecvd = m_SyncServerUDPLink->ReceiveWithTimeout(m_lastMsgReceived.m_protoMessage, sizeof(CGeneralInputServerProtoMsg), tryLockTime);
	write_log(1, "CSyncServerLink::ReceiveResponse: [%d] bytes received\n", bytesRecvd);
	if (0 < bytesRecvd)
	{
		cmdType = m_lastMsgReceived.GetCmdType();
		if (CMDTYPE_REGISTER_FOR_MUTEX_ACK == cmdType)
		{
			write_log(1, "CSyncServerLink::ReceiveResponse: cmdType[%s], waitlist[%d]\n", "CMDTYPE_REGISTER_FOR_MUTEX_ACK", m_lastMsgReceived.m_registerForMutexAckMsg.m_waitingListSize);
			return CMDTYPE_REGISTER_FOR_MUTEX_ACK;
		}
		else if (CMDTYPE_MUTEX_GRANTED == cmdType)
		{
			write_log(1, "CSyncServerLink::ReceiveResponse: cmdType[%s]\n", "CMDTYPE_MUTEX_GRANTED");
			return CMDTYPE_MUTEX_GRANTED;
		}
		else
		{
			write_log(1, "CSyncServerLink::ReceiveResponse: ERROR: unknown cmdType[%d]\n", cmdType);
		}
		return -1;
	}
	else if (0 == bytesRecvd)
	{	
		write_log(1, "CSyncServerLink::ReceiveResponse: Timed out\n");
		return 0;
	}
	write_log(1, "CSyncServerLink::ReceiveResponse: ERROR: Socket error\n");
	return -1;
}

void CSyncServerLink::SetMutexState(const char* const caller, EMutexState newState)
{
	write_log(1, "CSyncServerLink::SetMutexState: Caller[%s]: Changing from state[%s] to [%s]\n", caller, g_mutexStates[m_mutexState], g_mutexStates[newState]);
	m_mutexState = newState;
}

