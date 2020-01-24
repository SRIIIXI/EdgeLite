#include "Listener.h"
#include "OSDefinitions.h"
#include "Responder.h"
#include "HttpHandler.h"
#include "HttpPacket.h"
#include "Responder.h"
#include "SignalHandler.h"

#if defined(_WIN32) || defined(WIN32)
#define close(n) closesocket(n)
#define socklen_t int
#else
#define SOCKET int
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR	 (-1)
#define LPSOCKADDR sockaddr*
#endif

socklen_t addrlen;
sockaddr localaddr;
struct sockaddr_in *sadd;
char localipaddress[32];

Listener::Listener()
{
    _CallbackPtr = NULL;
}

Listener::~Listener()
{
	stop();	
}

void Listener::setEventHandler(HTTPCallback *clbPtr)
{
    _CallbackPtr = clbPtr;
}

RunState Listener::run(int port)
{
    _ListenerSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    sockaddr_in bindAddr;

    memset((void*)&bindAddr, 0, sizeof(sockaddr_in));

    bindAddr.sin_family = AF_INET;
    bindAddr.sin_port = htons(port);
    bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(::bind(_ListenerSocket,(sockaddr*)&bindAddr,sizeof(bindAddr)) == SOCKET_ERROR)
    {
        return BindFailed;
    }

    if(::listen(_ListenerSocket,5)==SOCKET_ERROR)
    {
        return ListenFailed;
    }

    while(true)
    {
        sockaddr remotehostaddr;
        memset((void*)&remotehostaddr, 0, sizeof(remotehostaddr));
        addrlen = sizeof(remotehostaddr);

        SOCKET sock = accept(_ListenerSocket,&remotehostaddr,&addrlen);
        if(sock != INVALID_SOCKET)
        {
            HTTPHandler httphdlr;
            httphdlr.setServerAddress((const char*)&localipaddress[0]);

            if (_CallbackPtr)
            {
                httphdlr.setEventHandler(_CallbackPtr);
            }

            httphdlr.createResponder(sock);

            httphdlr.invokeHandler();
        }
        else
        {
            if ((errno != ECHILD) && (errno != ERESTART) && (errno != EINTR))
            {
				return SignalReceived;
            }
        }
    }

    return NormalExit;
}

void Listener::stop()
{
	close(_ListenerSocket);
}


