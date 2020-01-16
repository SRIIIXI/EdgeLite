#ifndef _TELEMETRY
#define _TELEMETRY

#include "Types.h"
#include "Header.h"

typedef union TelemetryBody
{
	struct System
	{
		struct CPU
		{
			struct Utilization
			{
				float Maximum;
				float Minimum;
				float SMA;
			}

			struct Temperature
			{
				float Maximum;
				float Minimum;
				float SMA;
			}
		}

		struct Memory
		{
			struct Physical
			{
				float Maximum;
				float Minimum;
				float SMA;
				float Total;
			}

			struct Virtual
			{
				float Maximum;
				float Minimum;
				float SMA;
				float Total;
			}
		}
	}

	struct Network
	{
		unsigned long long StatusTimestamp;
		NetworkIfType InterfaceType;
		char InterfaceName[32];
		unsigned short Status;

		struct Sent
		{
			unsigned long Bytes;
			unsigned long Packets;
			unsigned long Dropped;
			unsigned long Error;
		}

		struct Received
		{
			unsigned long Bytes;
			unsigned long Packets;
			unsigned long Dropped;
			unsigned long Error;
		}
	}

	struct FileSystem
	{
		float FreeSpacePerCentage;
		float FreeSpaceMegaBytes;
	}

	struct DeviceAppStatus
	{
		char ApplicationId[33];
		unsigned long long Timestamp;
		AppStatus Status;
	}

	struct AppList
	{
		Application AppList[10];
	}

	struct AppData
	{
		unsigned long long Timestamp;
		Application ActiveApplication;
		SensorValue SensorValues[64];
		Peripheral Peripheral[32];
	}
}TelemetryBody;

typedef struct Telemetry
{
	Header Header;
	Payload PayloadType;
	TelemetryTypes CommandType;
	TelemetryBody Body;
}Telemetry;

#endif