// Temporary file

#ifndef  _REQUESTS
#define _REQUESTS

#include "Types.h"
#include "Header.h"
#include "Body.h"

typedef struct Payload
{
	Header PayloadHeader;
	Payload PayloadType;
	Body PayloadBody;
}Payload;

#endif // ! _NS_REQUESTS