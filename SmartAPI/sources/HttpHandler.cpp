#include "HttpHandler.h"

HTTPHandler::HTTPHandler()
{
    alive = false;
    _CallbackPtr = NULL;
	returncodes[200] = "OK";
	returncodes[401] = "Unauthorized";
	returncodes[400] = "Bad Request";
	returncodes[404] = "Not Found";
	returncodes[403] = "Forbidden";
	returncodes[405] = "Method Not Allowed";
}

HTTPHandler::~HTTPHandler()
{
}

void HTTPHandler::setServerAddress(const char* serveraddr)
{
	_ServerAddress = serveraddr;
}

void HTTPHandler::setEventHandler(HTTPCallback *clbPtr)
{
    _CallbackPtr = clbPtr;
}

bool HTTPHandler::createResponder(SOCKET sk)
{
    if(_ResponderInstance.isConnected())
    {
        _ResponderInstance.closeSocket();
    }

    if(!_ResponderInstance.createSocket(sk))
    {
        return false;
    }

    alive = true;

    return true;
}

bool HTTPHandler::invokeHandler()
{

    if(receiveHttpPacket(_HttpMessage))
    {
        handleProtocol(&_HttpMessage, &_ResponderInstance);
    }

     _ResponderInstance.closeSocket();
    return true;
}

bool HTTPHandler::sendHttpPacket(HttpPacket &message)
{
    if(!_ResponderInstance.isConnected())
    {
        return false;
    }

	std::string data;
    message.serialize(data);
    int len = data.length();
    return _ResponderInstance.sendBuffer(data.c_str(), len);
}

bool HTTPHandler::receiveHttpPacket(HttpPacket &message)
{
    if(!_ResponderInstance.isConnected())
    {
        return false;
    }

	std::string header;
    if(_ResponderInstance.receiveString(header,(char*)"\r\n\r\n"))
    {
        message.setHeader(header.c_str());
        message.deSerialize();

        if(message.hasBody())
        {
            char *buffer = NULL;

            int len = message.getContentSize();

            buffer = new char[len+1];
            memset(buffer,0,len+1);

            if(_ResponderInstance.receiveBuffer(buffer,len))
            {
                message.attachBody(buffer);
                delete [] buffer;
                buffer = NULL;
            }
            else
            {
                return false;
            }
        }
    }
    else
    {
        return false;
    }

    return true;
}

void HTTPHandler::handleProtocol(HttpPacket* message)
{
    handleProtocol((HttpPacket*)message, &_ResponderInstance);
}

void HTTPHandler::handleProtocol(HttpPacket* message, Responder *sourceDevice)
{
    if(message->getMessageType() == REQUEST)
	{
        if(strcmp((const char*)message->getRequest(),"GET")==0)
		{
			HandleGet(message,sourceDevice);
		}

        if(strcmp((const char*)message->getRequest(),"PUT")==0)
		{
			HandlePut(message,sourceDevice);
		}

        if(strcmp((const char*)message->getRequest(),"HEAD")==0)
		{
			HandleHead(message,sourceDevice);
		}

        if(strcmp((const char*)message->getRequest(),"POST")==0)
		{
			HandlePost(message,sourceDevice);
		}

        if(strcmp((const char*)message->getRequest(),"DELETE")==0)
		{
			HandleDelete(message,sourceDevice);
		}

        if(strcmp((const char*)message->getRequest(),"TRACE")==0)
		{
			HandleTrace(message,sourceDevice);
		}

        if(strcmp((const char*)message->getRequest(),"OPTIONS")==0)
		{
			HandleOptions(message,sourceDevice);
		}

        if(strcmp((const char*)message->getRequest(),"CONNECT")==0)
		{
			HandleConnect(message,sourceDevice);
		}
	}
	else
	{
	}

    if(message->hasBody())
	{
	}
}

void HTTPHandler::HandleHead(HttpPacket *messagePtr, Responder *sourceDevice)
{
}

void HTTPHandler::HandleGet(HttpPacket *messagePtr, Responder *sourceDevice)
{
	std::string httpheader;
    char buffer[1024]={0};
	std::string bufferedContentStr = "";
	std::string ctype = "";

	std::string host;
    messagePtr->getFieldValue("Host", host);
    vector<std::string> server;
    strsplit(host, server, ':');

    int retval = 200;

	std::string contentFileName;

    if(_CallbackPtr)
    {
        _CallbackPtr->setServerAddress(server[0].c_str());
		retval = _CallbackPtr->eventGet(messagePtr, bufferedContentStr, ctype);

		if (retval == 200)
		{
			sprintf(buffer,"HTTP/1.1 200 OK\r\n%c",'\0');
			httpheader = buffer;

			sprintf(buffer,"Cache-Control: private\r\n%c",'\0');
			httpheader += buffer;

			sprintf(buffer,"Content-Type: text/%s\r\n%c", ctype.c_str(), '\0');
			httpheader += buffer;

			sprintf(buffer,"Server: HTTP Server Lite v1.0 %s\r\n%c", server[0].c_str(),'\0');
			httpheader += buffer;

			bufferedContentStr += "\r\n\r\n";
			sprintf(buffer, "Content-Length: %d\r\n\r\n%c", (int)bufferedContentStr.length(), '\0');
			httpheader += buffer;
			//Dynamic content can be clubbed with header string itself
			httpheader += bufferedContentStr;		
		}
		else
		{
			sprintf(buffer, "HTTP/1.1 %d %s\r\n%c", retval, returncodes[retval].c_str(), '\0');
			httpheader = buffer;

			sprintf(buffer, "Cache-Control: private\r\n%c", '\0');
			httpheader += buffer;

			sprintf(buffer, "Server: HTTP Server Lite v1.0 %s\r\n%c", server[0].c_str(), '\0');
			httpheader += buffer;

			sprintf(buffer, "Content-Length: 0\r\n\r\n%c", '\0');
			httpheader += buffer;
		}
    }

	int headerlen = httpheader.length();
    sourceDevice->sendBuffer(httpheader.c_str(), headerlen);
}

void HTTPHandler::HandlePut(HttpPacket *messagePtr, Responder *sourceDevice)
{
	std::string httpheader;
	char buffer[1024] = { 0 };
	std::string bufferedContentStr = "";
	std::string ctype = "";

	std::string host;
	messagePtr->getFieldValue("Host", host);
	vector<std::string> server;
	strsplit(host, server, ':');

	int retval = 200;

	std::string contentFileName;

	if (_CallbackPtr)
	{
		_CallbackPtr->setServerAddress(server[0].c_str());
		retval = _CallbackPtr->eventPut(messagePtr, bufferedContentStr, ctype);

		if (retval == 200)
		{
			sprintf(buffer, "HTTP/1.1 200 OK\r\n%c", '\0');
			httpheader = buffer;

			sprintf(buffer, "Cache-Control: private\r\n%c", '\0');
			httpheader += buffer;

			sprintf(buffer, "Content-Type: text/%s\r\n%c", ctype.c_str(), '\0');
			httpheader += buffer;

			sprintf(buffer, "Server: HTTP Server Lite v1.0 %s\r\n%c", server[0].c_str(), '\0');
			httpheader += buffer;

			bufferedContentStr += "\r\n\r\n";
			sprintf(buffer, "Content-Length: %d\r\n\r\n%c", (int)bufferedContentStr.length(), '\0');
			httpheader += buffer;
			//Dynamic content can be clubbed with header string itself
			httpheader += bufferedContentStr;
		}
		else
		{
			sprintf(buffer, "HTTP/1.1 %d %s\r\n%c", retval, returncodes[retval].c_str(), '\0');
			httpheader = buffer;

			sprintf(buffer, "Cache-Control: private\r\n%c", '\0');
			httpheader += buffer;

			sprintf(buffer, "Server: HTTP Server Lite v1.0 %s\r\n%c", server[0].c_str(), '\0');
			httpheader += buffer;

			sprintf(buffer, "Content-Length: 0\r\n\r\n%c", '\0');
			httpheader += buffer;
		}
	}

	int headerlen = httpheader.length();
	sourceDevice->sendBuffer(httpheader.c_str(), headerlen);
}

void HTTPHandler::HandlePost(HttpPacket *messagePtr, Responder *sourceDevice)
{
	std::string httpheader;
	char buffer[1024] = { 0 };
	std::string bufferedContentStr = "";
	std::string ctype = "";

	std::string host;
	messagePtr->getFieldValue("Host", host);
	vector<std::string> server;
	strsplit(host, server, ':');

	int retval = 200;

	std::string contentFileName;

	if (_CallbackPtr)
	{
		_CallbackPtr->setServerAddress(server[0].c_str());
		retval = _CallbackPtr->eventPost(messagePtr, bufferedContentStr, ctype);

		if (retval == 200)
		{
			sprintf(buffer, "HTTP/1.1 200 OK\r\n%c", '\0');
			httpheader = buffer;

			sprintf(buffer, "Cache-Control: private\r\n%c", '\0');
			httpheader += buffer;

			sprintf(buffer, "Content-Type: text/%s\r\n%c", ctype.c_str(), '\0');
			httpheader += buffer;

			sprintf(buffer, "Server: HTTP Server Lite v1.0 %s\r\n%c", server[0].c_str(), '\0');
			httpheader += buffer;

			bufferedContentStr += "\r\n\r\n";
			sprintf(buffer, "Content-Length: %d\r\n\r\n%c", (int)bufferedContentStr.length(), '\0');
			httpheader += buffer;
			//Dynamic content can be clubbed with header string itself
			httpheader += bufferedContentStr;
		}
		else
		{
			sprintf(buffer, "HTTP/1.1 %d %s\r\n%c", retval, returncodes[retval].c_str(), '\0');
			httpheader = buffer;

			sprintf(buffer, "Cache-Control: private\r\n%c", '\0');
			httpheader += buffer;

			sprintf(buffer, "Server: HTTP Server Lite v1.0 %s\r\n%c", server[0].c_str(), '\0');
			httpheader += buffer;

			sprintf(buffer, "Content-Length: 0\r\n\r\n%c", '\0');
			httpheader += buffer;
		}
	}

	int headerlen = httpheader.length();
	sourceDevice->sendBuffer(httpheader.c_str(), headerlen);
}

void HTTPHandler::HandleDelete(HttpPacket *messagePtr, Responder *sourceDevice)
{
	std::string httpheader;
	char buffer[1024] = { 0 };
	std::string bufferedContentStr = "";
	std::string ctype = "";

	std::string host;
	messagePtr->getFieldValue("Host", host);
	vector<std::string> server;
	strsplit(host, server, ':');

	int retval = 200;

	std::string contentFileName;

	if (_CallbackPtr)
	{
		_CallbackPtr->setServerAddress(server[0].c_str());
		retval = _CallbackPtr->eventDelete(messagePtr, bufferedContentStr, ctype);

		if (retval == 200)
		{
			sprintf(buffer, "HTTP/1.1 200 OK\r\n%c", '\0');
			httpheader = buffer;

			sprintf(buffer, "Cache-Control: private\r\n%c", '\0');
			httpheader += buffer;

			sprintf(buffer, "Content-Type: text/%s\r\n%c", ctype.c_str(), '\0');
			httpheader += buffer;

			sprintf(buffer, "Server: HTTP Server Lite v1.0 %s\r\n%c", server[0].c_str(), '\0');
			httpheader += buffer;

			bufferedContentStr += "\r\n\r\n";
			sprintf(buffer, "Content-Length: %d\r\n\r\n%c", (int)bufferedContentStr.length(), '\0');
			httpheader += buffer;
			//Dynamic content can be clubbed with header string itself
			httpheader += bufferedContentStr;
		}
		else
		{
			sprintf(buffer, "HTTP/1.1 %d %s\r\n%c", retval, returncodes[retval].c_str(), '\0');
			httpheader = buffer;

			sprintf(buffer, "Cache-Control: private\r\n%c", '\0');
			httpheader += buffer;

			sprintf(buffer, "Server: HTTP Server Lite v1.0 %s\r\n%c", server[0].c_str(), '\0');
			httpheader += buffer;

			sprintf(buffer, "Content-Length: 0\r\n\r\n%c", '\0');
			httpheader += buffer;
		}
	}

	int headerlen = httpheader.length();
	sourceDevice->sendBuffer(httpheader.c_str(), headerlen);
}

void HTTPHandler::HandleTrace(HttpPacket *messagePtr, Responder *sourceDevice)
{
}

void HTTPHandler::HandleOptions(HttpPacket *messagePtr, Responder *sourceDevice)
{
}

void HTTPHandler::HandleConnect(HttpPacket *messagePtr, Responder *sourceDevice)
{
}

void HTTPCallback::setServerAddress(const char* serveraddr)
{
	serverAddress = serveraddr;
}
