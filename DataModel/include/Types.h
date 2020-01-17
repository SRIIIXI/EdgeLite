#ifndef _TYPES
#define _TYPES

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum ServiceType
{
	CoreService=1,
	UserApplication=2
}ServiceType;

typedef enum TelemetryType
{
	System=0,
	Network=1,
	Filesystem=2,
	Sensor=3,
}TelemetryType;

typedef enum ProcessStatus
{
	Running=0,
	Stopped=1,
	Suspended=2,
	Idle=3,
}ProcessStatus;

typedef enum NetworkIfType
{
	Ethernet=0,
	WiFi=1,
	GSM=2
}NetworkIfType;

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
	RF_WiFi = 5
}TransportType;

typedef enum PayloadType
{
	Alert = 0,
	Notification = 1,
	Telemetry = 2,
	Command = 3,
	Query = 4,
	Response = 5
}PayloadType;

typedef enum Services
{
	BootstrapService = 0,
	TransportService = 1,
	MonitoringService = 2,
	ApplicationManager = 3,
	ResourceManager = 4,
	PeripheralManager = 5
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
	ElectricMetering = 13
}Application;

typedef enum CommandType
{
	Registration=0,
	Reboot=1,
	Configuration=2,
	VPN=3,
	ProcessManagement=5
}CommandType;

typedef enum WiFiSecurity
{
	WPA=0,
	PSK=1,
	WEP=2,
	None=100
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
	char IMEI[16];
	char IMSI[16];
	char MSISDN[16];
}IfGSM;

typedef struct NetworkInterface
{
	IfEthernet IFEThernet[2];
	IfGSM IFGSM[2];
	IfWiFi IFWiFi[2];
}NetworkInterface;

typedef struct PortforwardingRule
{
	char ForwardedInterface[33];
	char ForwardedIpAddress[16];
	unsigned short	ForwardedPort;
	char IncomingInterface[33];
	unsigned short	IncomingPort;
}PortforwardingRule;

typedef struct Service
{
	char AppName[33];
	ProcessStatus AppStatus;
	ServiceType AppType;
}Service;

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