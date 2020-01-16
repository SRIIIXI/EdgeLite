#ifndef _BODY
#define _BODY

#include "Types.h"
#include "Alert.h"
#include "Command.h"
#include "Query.h"
#include "AppKeyValue.h"
#include "Telemetry.h"

typedef union Body
{
	Alert AlertPayload;
	Command CommandPayload;
	AppKeyValue HAKVPayload;
	Query QueryPayload;
	Telemetry TelemetryPayload;
}Body;

#endif