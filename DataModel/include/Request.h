// Temporary file

#ifndef  _REQUEST
#define _REQUEST

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