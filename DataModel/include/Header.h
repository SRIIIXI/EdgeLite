#ifndef _HEADER
#define _HEADER

#include "Types.h"

typedef struct Header
{
	char DeviceID[33];
	char Sender[33];
	char Receipient[33];
	unsigned short Version;
	unsigned long long Timestamp;
	unsigned long long CorelationId;
	char Encoding[33];
	TransportType TransportType;
}Header;


#endif