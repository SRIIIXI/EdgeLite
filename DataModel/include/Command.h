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
	};	
	
	struct ConfigureDeviceDownlink
	{
		NetworkInterface Interfaces;
		PortforwardingRule PortForwarding;
	};

	struct ConfigureDeviceUplink
	{
		char CommandText[16];
	};

	struct RebootDevice
	{
		char RebootCommand[16];
	};

	struct VPNDevice
	{
		char VPNCommand[16];
	};

	struct AppManagementBoard
	{
		char TargetApplication[33];
		char AppCommand[16];
	};

}CommandBody;

typedef struct CommandStruct
{
	Header Header;
	PayloadType Type;
	CommandType CommandType;
	CommandBody Command1;
}CommandStruct;

#endif