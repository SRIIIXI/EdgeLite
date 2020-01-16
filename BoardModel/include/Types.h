#ifndef _TYPES
#define

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum TelemetryTypes
{
	System=0,
	Network=1,
	Filesystem=2,
	AppStatus=3,
	AppManager=100
}TelemetryTypes;

typedef enum Status
{
	Running=0,
	Stopped=1,
	Error=2,
	Idle=3,
};

typedef enum NetworkIfType
{
	Ethernet=0,
	WiFi=1,
};

typedef enum ResponseType
{
	Succes=0,
	Warning=1,
	Error=2
}ResponseType;

typedef enum TransportType
{
	IP_MQTT = 0,
	IP_HTTPS = 1,
	IP_CoAP = 2,
	RF_ZigBee = 3,
	RF_Bluetooth = 4,
	RF_WiFi = 5,
	Unknown = -1
}TransportType;

typedef enum Payload
{
	Alert = 0,
	Telemetry = 1,
	Command = 2,
	Query = 3,
	Respoe = 4,
	HostedAppKeyValue = 5,
	Unknown = -1
}Payload;

typedef enum Services
{
	BootstrapService = 0,
	TraportService = 1,
	MonitoringService = 2,
	ApplicationManager = 3,
	ResourceManager = 4,
	PeripheralManager = 5,
	Unknown = -1
}Services;

typedef enum Application
{
	AirQuality = 0,
	Camera = 1,
	VideoAnalytics = 2,
	TrafficAnalytics = 3,
	SoundAnalytics = 4,
	RoadSurface = 5,
	ElectricVehicleCharging = 6,
	DigitalSignage = 7,
	SmartLighting = 8,
	PublicSafety = 9,
	PublicWiFi = 10,
	WasteManagement = 11,
	Parking = 12,
	ElectricMetering = 13,
	Unknown = -1
}Applicatio;

typedef enum CommandType
{
	Registration=0,
	Reboot=1,
	ConfigurationUplink=2,
	ConfigurationDownlink = 3,
	VPN=4,
	AppManagement=5
}CommandType;

typedef enum WiFiSecurity
{
	WPA=0,
	WPA-PSK=1,
	WEP=2,
	None=100,
	Unknown=-1
}WiFiSecurity;

typedef enum IPAddressType
{
	Static=0,
	Dynamic=1
}IPAddressType;

typedef struct IfEthernet
{
	bool IsEnabled;
	char PreferredDServer[16];
	char AlternateDServer[16];
	char BroadcastAddress[16];
	char SubnetAddress[16];
	char GatewayAddress[16];
	char IpAddress[16];
	char NetworkAddress[16];
	IPAddressType AddressType;
}IfEthernet;

typedef struct IfWiFi
{
	bool IsEnabled;
	char SSID[65];
	char Password[65];
	WiFiSecurity Security;
}IfWiFi;

typedef struct IfGSM
{
	bool IsEnabled;
	char APN[33];
}IfGSM;

typedef struct Interfaces
{
	IfEthernet IFEThernet[2];
	IfGSM IFGSM[2];
	IfWiFi IFWiFi[2];
}Interfaces;

typedef struct Portforwarding
{
	char ForwardedInterface[33];
	char ForwardedIpAddress[16];
	uigned short	ForwardedPort;
	char IncomingInterface[33];
	uigned short	IncomingPort;
}Portforwarding;

typedef struct HostedApp
{
	char AppName[33];
	Status AppStatus;
}HostedApp;

typedef struct SensorValue
{
	char Attribute[17];
	char Value[17];
	char Unit[17];
}SensorValue;

typedef struct Peripheral
{
	char HardwareId[32];
	char DeviceName[32];
	char Manufacturer[32];
}Peripheral;

#endif