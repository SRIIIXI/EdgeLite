#pragma once

#include "OSDefinitions.h"
#include "StringEx.h"
#include "Directory.h"
#include "HttpPacket.h"
#include "Responder.h"
#include <map>
#include <string>

using namespace std;

#ifndef	_HTTPHANDLER
#define	_HTTPHANDLER

class HTTPCallback
{
public:
    HTTPCallback()
	{

	}
	virtual int eventGet(HttpPacket *messagePtr, std::string &content, std::string &ctype) = 0;
	virtual int eventPut(HttpPacket *messagePtr, std::string &content, std::string &ctype)=0;
    virtual int eventPost(HttpPacket *messagePtr, std::string &content, std::string &ctype)=0;
    virtual int eventDelete(HttpPacket *messagePtr, std::string &content, std::string &ctype)=0;
	void setServerAddress(const char* serveraddr);
	std::string	serverAddress;
};

class HTTPHandler
{
public:

	HTTPHandler();
    virtual ~HTTPHandler();

    bool createResponder(SOCKET sk);
    void setEventHandler(HTTPCallback *clbPtr);
	void setServerAddress(const char* serveraddr);
    bool invokeHandler();

protected:
    void handleProtocol(HttpPacket* message, Responder *sourceDevice);
    void handleProtocol(HttpPacket* message);

    bool receiveHttpPacket(HttpPacket &message);
    bool sendHttpPacket(HttpPacket &message);

    void HandleHead(HttpPacket *messagePtr, Responder *sourceDevice);
    void HandleGet(HttpPacket *messagePtr, Responder *sourceDevice);
    void HandlePut(HttpPacket *messagePtr, Responder *sourceDevice);
    void HandlePost(HttpPacket *messagePtr, Responder *sourceDevice);
    void HandleDelete(HttpPacket *messagePtr, Responder *sourceDevice);
    void HandleTrace(HttpPacket *messagePtr, Responder *sourceDevice);
    void HandleOptions(HttpPacket *messagePtr, Responder *sourceDevice);
    void HandleConnect(HttpPacket *messagePtr, Responder *sourceDevice);

    bool alive;
private:
    Responder		_ResponderInstance;
    HttpPacket 	_HttpMessage;
    HTTPCallback*   _CallbackPtr;
	std::string	_ServerAddress;
	std::map<int, std::string> returncodes;
};

#endif
