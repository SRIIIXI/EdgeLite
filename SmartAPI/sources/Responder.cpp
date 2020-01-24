#include "Responder.h"
#include "StringEx.h"

#if defined(_WIN32) || defined(WIN32)
#define close(n) closesocket(n)
#define socklen_t int
#else
#define SOCKET int
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR	 (-1)
#define LPSOCKADDR sockaddr*
#endif

Responder::Responder()
{
    _Socket = 0;
    _Connected = false;
    memset((void*)&_ServerAddress,0,sizeof(sockaddr_in));
    _PreFetchedBufferSize = 0;
    _PreFetchedBuffer = NULL;
}

Responder::Responder(const Responder& other)
{
    memset((void*)&_ServerAddress,0,sizeof(sockaddr_in));

    _PreFetchedBufferSize = 0;

	if(_PreFetchedBuffer != NULL)
	{
		delete _PreFetchedBuffer;
		_PreFetchedBuffer = NULL;
	}
	
	_Socket = other._Socket;
	_Connected = other._Connected;
	memcpy((void*)&_ServerAddress, (void*)&other._ServerAddress, sizeof(sockaddr_in));

	if(other._PreFetchedBufferSize > 0)
	{
		_PreFetchedBuffer = new unsigned char[other._PreFetchedBufferSize];
		_PreFetchedBufferSize = other._PreFetchedBufferSize;
		memcpy((unsigned char*)&_PreFetchedBuffer, (unsigned char*)&other._PreFetchedBuffer, _PreFetchedBufferSize);
	}
}

Responder& Responder::operator=( const Responder& other)
{
	 memset((void*)&_ServerAddress,0,sizeof(sockaddr_in));

    _PreFetchedBufferSize = 0;

	if(_PreFetchedBuffer != NULL)
	{
		delete _PreFetchedBuffer;
		_PreFetchedBuffer = NULL;
	}
	
	_Socket = other._Socket;
	_Connected = other._Connected;
	memcpy((void*)&_ServerAddress, (void*)&other._ServerAddress, sizeof(sockaddr_in));

	if(other._PreFetchedBufferSize > 0)
	{
		_PreFetchedBuffer = new unsigned char[other._PreFetchedBufferSize];
		_PreFetchedBufferSize = other._PreFetchedBufferSize;
		memcpy((unsigned char*)&_PreFetchedBuffer, (unsigned char*)&other._PreFetchedBuffer, _PreFetchedBufferSize);
	}

	return *this;
}

Responder::~Responder()
{
	closeSocket();
	while(isConnected()==true)
	{
	}

    if(_PreFetchedBuffer != NULL)
    {
        delete _PreFetchedBuffer;
    }
}

bool Responder::createSocket(const char* servername, int serverport)
{

    _ServerName = servername;
    _ServerPort = serverport;

    _ServerAddress.sin_family = AF_INET;
    _ServerAddress.sin_port = htons(serverport);
    u_long nRemoteAddr;

    char ipbuffer[32]={0};
    strncpy(ipbuffer, servername, 31);

    bool ip = NetworkHelper::isIP4Address(ipbuffer);

    if(!ip)
    {
        nRemoteAddr = inet_addr(_ServerName.c_str());
        if (nRemoteAddr == INADDR_NONE)
        {
            hostent* pHE = gethostbyname(_ServerName.c_str());
            if (pHE == 0)
            {
                nRemoteAddr = INADDR_NONE;
                return false;
            }
            nRemoteAddr = *((u_long*)pHE->h_addr_list[0]);
            _ServerAddress.sin_addr.s_addr = nRemoteAddr;
        }
    }
    else
    {
        #ifdef WIN32
            nRemoteAddr = inet_addr(_ServerName.c_str());
            _ServerAddress.sin_addr.s_addr = nRemoteAddr;
        #else
                inet_pton (AF_INET, _ServerName.c_str(), &_ServerAddress.sin_addr);
        #endif
    }


    _Socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(_Socket == INVALID_SOCKET)
    {
        return false;
    }


    return true;
}

bool Responder::createSocket(SOCKET inSocket)
{
    _Socket = inSocket;
    _Connected = true;
	return true;
}


bool Responder::connectSocket(int &returncode)
{
    if(_Connected == true)
	{
		return true;
	}

	returncode = connect(_Socket,(sockaddr*)&_ServerAddress, sizeof(sockaddr_in));

    if(returncode == SOCKET_ERROR)
	{
		#if defined(_WIN32) || defined(WIN32)
		returncode = ::WSAGetLastError();
		#else
		returncode = errno;
		#endif        
		
		shutdown(_Socket,2);
        close(_Socket);
		_Connected = false;

		return false;
	}

	_Connected = true;
	return true;
}

bool Responder::closeSocket()
{
    if(_Connected)
	{
        shutdown(_Socket,0);
        close(_Socket);
	}	
    _Connected = false;
	return false;
}

bool Responder::receiveString(string &ioStr, const char *delimeter)
{
    char	buffer[1024];
	long	returnvalue;
	std::string	data;
    std::string  currentLine, nextLine;

	data.erase();

    if(_PreFetchedBufferSize > 0)
	{
        if(strstr((char*)_PreFetchedBuffer,delimeter)!=0)
		{
            strsplit((const char*)_PreFetchedBuffer,delimeter,currentLine,nextLine);

            ioStr = currentLine;
			currentLine.erase();

            delete _PreFetchedBuffer;
            _PreFetchedBuffer = NULL;
            _PreFetchedBufferSize = nextLine.length();

            if(_PreFetchedBufferSize > 0)
            {
                _PreFetchedBuffer = new unsigned char[_PreFetchedBufferSize+1];
                memset(_PreFetchedBuffer, 0, _PreFetchedBufferSize+1);
                memcpy(_PreFetchedBuffer,nextLine.c_str(),_PreFetchedBufferSize);
            }

			return true;
		}

        data = (char*)_PreFetchedBuffer;
        _PreFetchedBufferSize = 0;
        delete _PreFetchedBuffer;
        _PreFetchedBuffer = NULL;
	}

	while(true)
	{
        memset(&buffer[0],0,1024);
        returnvalue = recv(_Socket,&buffer[0],1024,0);

		// Error or link down
		if(returnvalue < 1)
		{
			int error = SOCK_ERROR;
            ioStr.clear();
			_Connected = false;
			return false;
		}

		data += buffer;

		if(strstr(data.c_str(),delimeter)!=0)
		{
            strsplit(data,delimeter,currentLine, nextLine);

            _PreFetchedBufferSize = nextLine.length();
            
            if(_PreFetchedBufferSize > 0)
            {
                _PreFetchedBuffer = new unsigned char[_PreFetchedBufferSize+1];
                memset(_PreFetchedBuffer, 0, _PreFetchedBufferSize+1);
                memcpy(_PreFetchedBuffer,nextLine.c_str(),_PreFetchedBufferSize);
            }

            ioStr = currentLine;

			data.erase();
			currentLine.erase();
			return true;
		}
	}
	return true;
}

bool Responder::receiveBuffer(char* ioBuffer,int len)
{
	char*	buffer = 0;
	long	bufferpos = 0;
	long	bytesread = 0;
	long	bytesleft = len;

    // If there are pre-fetched bytes left, we have to copy that first and relase memory

    if(_PreFetchedBufferSize > 0)
    {
        memcpy(ioBuffer, _PreFetchedBuffer, _PreFetchedBufferSize);
        bytesleft = len - _PreFetchedBufferSize;
        bufferpos = _PreFetchedBufferSize;
        _PreFetchedBufferSize = 0;
        delete _PreFetchedBuffer;
        _PreFetchedBuffer = NULL;

        if(bytesleft < 1)
        {
            return true;
        }
    }

	while(true)
	{
		buffer = new char[bytesleft+1];
        memset(buffer, 0, bytesleft+1);
        bytesread = recv(_Socket,buffer,bytesleft,0);

		// Error or link down
		if(bytesread < 1)
		{
			int error = SOCK_ERROR;
			delete buffer;
			ioBuffer = 0;
			len	= 0;
			_Connected = false;
			return false;
		}

		memcpy(ioBuffer+bufferpos,buffer,bytesread);
		delete buffer;

		bufferpos = bufferpos + bytesread;

		bytesleft = bytesleft - bytesread;

		if(bufferpos >= len)
		{
			return true;
		}
	}
}

int Responder::pendingPreFetchedBufferSize()
{
    return _PreFetchedBufferSize;
}


bool Responder::sendBuffer(const char* data, int &len)
{
	if(!_Connected)
	{
		return false;
	}

	long sentsize =0;

    sentsize = send(_Socket, data, len,0);
	if(sentsize==SOCKET_ERROR)
	{
		return false;
	}

	return true;
}

bool Responder::sendBuffer(const std::string &str)
{
	int len = str.length();
	bool ret = sendBuffer(str.c_str(), len);
	return ret;
}

bool Responder::isConnected()
{
    return _Connected;
}

SOCKET Responder::getSocket()
{
	return _Socket;
}
