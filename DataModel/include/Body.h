#ifndef _BODY
#define _BODY

#include "Types.h"
#include "Command.h"
#include "Query.h"
#include "AppKeyValue.h"
#include "Telemetry.h"

typedef union Body
{
	CommandType CommandPayload;
	AppKeyValue HAKVPayload;
	Query QueryPayload;
	Telemetry TelemetryPayload;
}Body;

#endif