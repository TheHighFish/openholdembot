#ifndef SYNC_SERVER_LINK_H
#define SYNC_SERVER_LINK_H


class CUDPSocketSender;

enum EMutexState 
{ 
	EMUTEX_STATE_NO_REQUEST,
	EMUTEX_STATE_PENDING,
	EMUTEX_STATE_GRANTED
};

const char* const g_mutexStates[] = 
{
	"EMUTEX_STATE_NO_REQUEST",
	"EMUTEX_STATE_PENDING",
	"EMUTEX_STATE_GRANTED"
};

#define DEAFULT_BOT_LEASE_TIME 3500

#include "InputServerProto.h"

class CSyncServerLink
{
public:
	// public functions
	CSyncServerLink(BOOL bInitiallyOwn = FALSE, LPCTSTR lpszName = NULL, LPSECURITY_ATTRIBUTES lpsaAttribute = NULL);
	virtual ~CSyncServerLink();



	virtual void				Clear();
	virtual int					Init(int port);
	virtual int					Term();
	virtual int					Lock(int dwMiliseconds, int leaseTime = DEAFULT_BOT_LEASE_TIME);
	virtual int					LockOnNoRequest(int dwMiliseconds, int leaseTime);
	virtual int					LockOnPendingRequest(int dwMiliseconds, int leaseTime);
	virtual void				SendLockRequest(int leaseTime);
	virtual void				SendReleaseNotification();
	virtual short				ReceiveResponse(int dwMiliseconds);
	virtual void				Unlock();
	virtual void				SetMutexState(const char* const caller, EMutexState newState);

private:

	CUDPSocketSender*				m_SyncServerUDPLink;//TripleSeven: 24/12/2012: Supporting SyncServer
	EMutexState						m_mutexState;
	CGeneralInputServerProtoMsg		m_lastMsgReceived;



};


#endif //SYNC_SERVER_LINK_H