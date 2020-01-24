#include "NetworkHelper.h"

NetworkHelper::NetworkHelper()
{
}

NetworkHelper::~NetworkHelper()
{
}

bool NetworkHelper::initializeNetworkHelper()
{
	#if defined(_WIN32) || defined(WIN32)
		WSACleanup();
		WSADATA WSData;
		long nRc = WSAStartup(0x0202,&WSData);
		if(nRc != 0)
		{
			return false;
		}
		if(WSData.wVersion != 0x0202)
		{
			WSACleanup();
			return false;
		}
	#endif
	return true;
}

void NetworkHelper::deInitializeNetworkHelper()
{
	#if defined(_WIN32) || defined(WIN32)
		WSACleanup();
	#endif
}

bool NetworkHelper::getEndpoint(const char* hostname, int port, sockaddr_in *socketAddress)
{

	socketAddress->sin_family = AF_INET;

	if(port > 0)
	{
		socketAddress->sin_port = htons(port);
	}
	else
	{
	}

	if(!isIPAddress((char*)hostname))
	{
		hostent* pHE = gethostbyname(hostname);
		if (pHE == 0) 
		{
			return false;
		}
		socketAddress->sin_addr.s_addr = *((u_long*)pHE->h_addr_list[0]);
	}
	else
	{
		socketAddress->sin_addr.s_addr = inet_addr(hostname);
	} 
	return true;
}

bool NetworkHelper::isIPAddress(char* str)
{
	//return (IsIP4Address(str) || IsIP6Address(str));
	return isIP4Address(str);
}

bool NetworkHelper::isIP6Address(char*)
{
	return false;
}

bool NetworkHelper::isIP4Address(char* str)
{
	int slen = strlen(str);

	// Check the string length, for the range ...
	// 0.0.0.0 and 255.255.255.255
	if(slen < 7 || slen > 15)
	{
		// Bail out
		return false;
	}

	int ctr;
	bool isdelimeter = false;
	char nibble[4];
	memset((char*)&nibble[0],0,4);
	int nbindex = 0;
	for(ctr = 0 ; str[ctr] != '\0' ; ctr++)
	{
		// Check for permitted characters
		if(str[ctr] != '.' && isdigit(str[ctr]) <= 0)
		{
			// Bail out
			return false;
		}

		// '.' Delimeter case
		if(str[ctr] == '.')
		{
			if(isdelimeter)
			{
				// The flag was set in last iteration
				// This means ".." type of expression was found
				// Bail out
				return false;
			}

			// We have read a complete nibble
			// The characters in the nibble must represent a permissible value
			int numval = atoi(nibble);
			if(numval < 0 || numval > 255)
			{
				return false;
			}

			// Set the flag and continue
			memset((char*)&nibble[0],0,4);
			nbindex = 0;
			isdelimeter = true;
			continue;
		}

		if(isdigit(str[ctr])> 0)
		{
			isdelimeter = false;
			nibble[nbindex] = str[ctr];
			nbindex++;
			continue;
		}

	}

	return true;
}

void NetworkHelper::getLocalHostName(char *hostname)
{
  char name[128]={0};
  int namelen=128;
  if(gethostname(&name[0],namelen)==0)
  {
      strcpy(hostname, name);
  }
}

void NetworkHelper::getLocalIPAddress(const SOCKET newServerfd, char *ipaddress)
{
	struct sockaddr_in localAddress;
	socklen_t addressLength = sizeof(localAddress);
	getsockname(newServerfd, (struct sockaddr*)&localAddress, &addressLength);
	strcpy(ipaddress, inet_ntoa( localAddress.sin_addr));
}

