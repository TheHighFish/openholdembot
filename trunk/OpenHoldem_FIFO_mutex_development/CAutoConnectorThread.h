#ifndef INC_CAUTOCONNECTORTHREAD_H
#define INC_CAUTOCONNECTORTHREAD_H

extern class CAutoConnectorThread
{
public:
	CAutoConnectorThread();
	~CAutoConnectorThread();
public:
	// This function should be called, when everything else 
	// in the main application is initialized, but not earlier!
	void StartThread();
private:
	static UINT AutoConnectorThreadFunction(LPVOID pParam);
} *p_autoconnectorthread;

#endif // INC_CAUTOCONNECTORTHREAD_H