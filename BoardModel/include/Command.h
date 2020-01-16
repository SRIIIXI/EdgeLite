#ifndef _COMMAND
#define _COMMAND

#include "Types.h"
#include "Header.h"

typedef union CommandBody
{
	struct RegisterDevice
	{
		unsigned long long Timestamp;
		float Latitude;
		float Longitude;
	}	
	
	struct ConfigureDeviceDownlink
	{
		NSInterfaces Interfaces;
		NSPortforwarding PortForwarding;
	}

	struct ConfigureDeviceUplink
	{
		char CommandText[16];
	};

	struct RebootDevice
	{
		char CommandText[16];
	}

	struct VPNDevice
	{
		char CommandText[16];
	};

	struct AppManagementBoard
	{
		char TargetApplication[33];
		char CommandText[16];
	};

}CommandBody;

typedef struct Command
{
	Header Header;
	PayloadType Type;
	CommandType CommandType;
	CommandBody Command;
}Command;

#endif