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
			}Utilization;

			struct Temperature
			{
				float Maximum;
				float Minimum;
				float SMA;
			}Temperature;
		}CPU;

		struct Memory
		{
			struct Physical
			{
				float Maximum;
				float Minimum;
				float SMA;
				float Total;
			}Physical;

			struct Virtual
			{
				float Maximum;
				float Minimum;
				float SMA;
				float Total;
			}Virtual;
		}Memory;
	};

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
		}Sent;

		struct Received
		{
			unsigned long Bytes;
			unsigned long Packets;
			unsigned long Dropped;
			unsigned long Error;
		}Received;
	};

	struct FileSystem
	{
		float FreeSpacePerCentage;
		float FreeSpaceMegaBytes;
	}FileSystem;

	struct DeviceAppStatus
	{
		char ApplicationId[33];
		unsigned long long Timestamp;
		ProcessStatus Status;
	}DeviceAppStatus;

	struct AppList
	{
		Application ListOfApps[10];
	}AppList;

	struct AppData
	{
		unsigned long long Timestamp;
		Application ActiveApplication;
		SensorValue SensorValues[64];
		Peripheral Peripheral[32];
	}AppData;

}TelemetryBody;

typedef struct Telemetry
{
	Header Header;
	Payload PayloadType;
	TelemetryType CommandType;
	TelemetryBody Body;
}Telemetry;

#endif