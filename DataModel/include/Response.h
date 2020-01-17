#ifndef _RESPONSE
#define _RESPONSE

#include "Types.h"
#include "Header.h"

typedef struct GenericResponse
{
	Header PayloadHeader;
	Payload PayloadType;

	struct Response
	{
		ResponseType Type;
		short ResponseCode;
		char Message[65];
	}
}GenericResponse;

typedef struct RegistrationResponse
{
	Header PayloadHeader;
	Payload PayloadType;

	struct RegisterResponse
	{
		float Latitude;
		float Longitude;
	}
}RegistrationResponse;

typedef struct ConfigurationResponse
{
	Header PayloadHeader;
	Payload PayloadType;
	CommandType PayloadCommand;

	struct ConfigureNSResponse
	{
		Interfaces DeviceInterfaces;
		Portforwarding DevicePortForwarding;
	}
}ConfigurationResponse;

#endif