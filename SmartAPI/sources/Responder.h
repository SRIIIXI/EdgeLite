//Copyright (C) 2006 Siemens Information Systems Limited,
//All Rights Reserved (R)
//Project       : Generic TCP/IP Networking Library
//Module        : 
//File name     : ClientSocket.h
//Description   : Cross platform TCP socket class with user defined callback facility
//Compatibility : Win32, POSIX - BSD, Winsock Network functions
//Change History:
//Number  |Date          |Author			|Remark
//-------------------------------------------------------------------------------
//|1      |15.06.2006    |Subrato Roy	    |Created

#ifndef	_RESPONDER
#define	_RESPONDER

#include "OSDefinitions.h"
#include "NetworkHelper.h"
#include "StringEx.h"

class Responder
{
public:
	Responder();
	Responder(const Responder& other);
	Responder& operator=( const Responder& other);

	virtual ~Responder();
	bool createSocket(const char* servername, int serverport);
	bool createSocket(SOCKET inSocket);
	bool connectSocket(int &returncode);
	bool closeSocket();
	bool isConnected();

	bool sendBuffer(const char* data, int &len);
	bool sendBuffer(const std::string &str);

    bool receiveBuffer(char* ioBuffer,int len);
    bool receiveString(std::string &ioStr, const char* delimeter);

	SOCKET getSocket();

    int pendingPreFetchedBufferSize();
private:
    bool				_Connected;
    SOCKET				_Socket;
    sockaddr_in			_ServerAddress;
	std::string			_ServerName;
    int					_ServerPort;
    int					_PreFetchedBufferSize;
    unsigned char*		_PreFetchedBuffer;
};

typedef Responder Client;

#endif

