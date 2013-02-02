#ifndef __INTERFACE_UDPSOCKETSENDER_H__
#define __INTERFACE_UDPSOCKETSENDER_H__

//int GetHostByName(char *host_name, char*& host_ip);

class CUDPSocketSender
{
      
public:

//    Constructors and Destructures
      CUDPSocketSender();
      virtual ~CUDPSocketSender();


//    Thread Fundamental
      int               Init(char* domain, int port);
      int               Term();
      void              Reset();
	  int               Send(const char* message);
	  int				SendBinary(const char* message, int size);
	  int				Receive(char* message, int size);
	  int				ReceiveWithTimeout(char* message, int size, int timeoutMS);


      
private:
      char*             m_host_ip;
      int               m_port;
      int               m_initialized;
      SOCKET            m_udpSocket;
	  sockaddr_in       m_receiver;
      sockaddr_in       m_senderAddr;

};



#endif //__INTERFACE_UDPSOCKETSENDER_H__
