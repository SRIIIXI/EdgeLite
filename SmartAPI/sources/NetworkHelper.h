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
#ifndef	_NETWORK_HELPER
#define	_NETWORK_HELPER

#include "OSDefinitions.h"


// Protocol packet differentiation
const long REQUEST = 10;
const long RESPONSE = 11;

class NetworkHelper
{
public:
	NetworkHelper();
    static bool initializeNetworkHelper();
    static void deInitializeNetworkHelper();
	virtual ~NetworkHelper();
    static bool getEndpoint(const char* hostname, int port, sockaddr_in *socketAddress);
    static bool isIPAddress(char* str);
    static bool isIP6Address(char* str);
    static bool isIP4Address(char* str);
    static void getLocalHostName(char *hostname);
    static void getLocalIPAddress(const SOCKET newServerfd, char *ipaddress);
private:
};

#endif

