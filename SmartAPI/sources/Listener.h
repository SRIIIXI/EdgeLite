#ifndef	_LISTENER
#define	_LISTENER

#include "HttpHandler.h"

typedef enum RunState
{
    Running =0,
    NormalExit = 1,
    BindFailed = -1,
    ListenFailed = -2,
	SignalReceived = 3
}RunState;

class Listener
{
public:
	Listener();
	virtual ~Listener();
    RunState run(int port);
	void stop();
    void setEventHandler(HTTPCallback *clbPtr);

private:
    SOCKET _ListenerSocket;
	HTTPCallback*   _CallbackPtr;
};

#endif
