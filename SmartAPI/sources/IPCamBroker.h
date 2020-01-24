#ifndef _IPBUM_API_SERVICE
#define _IPBUM_API_SERVICE

#include "HttpHandler.h"
#include "DbLocal.h"
#include "Listener.h"
#include "SignalHandler.h"
#include "Notifier.h"

class BrokerSignalHandler : public SignalCallback
{
public:
	BrokerSignalHandler();
	void suspend();
	void resume();
	void shutdown();
	void alarm();
	void reset();
	void childExit();
	void userdefined1();
	void userdefined2();
};

typedef enum Authority
{
	UnAuthorized=0,
	DeviceUser=1,
	CloudUser=2
}Authority;

class IPCamBroker : public HTTPCallback
{
public:
	IPCamBroker(std::string appname);
	~IPCamBroker();

	bool initialize(std::string port = "80");
	void run();

	int eventGet(HttpPacket *messagePtr, std::string &content, std::string &ctype);
	int eventPut(HttpPacket *messagePtr, std::string &content, std::string &ctype);
	int eventPost(HttpPacket *messagePtr, std::string &content, std::string &ctype);
	int eventDelete(HttpPacket *messagePtr, std::string &content, std::string &ctype);

	std::string virtualDirectory();
	std::string physicalDirectory();
	std::string playlistDirectory();
	DbLocal* database();
	Authority authorityLevel(HttpPacket* messagePtr);
	bool canDownload(Camera* camPtr);
	void removeHeader(std::string& str);
private:

	DbLocal _Local;
	Listener _Listener;

	std::string _VirtualFolder;
	std::string _PhysicalFolder;
	std::string _PlaylistFolder;
	std::string _DatabaseFileName;
	std::string _Port;
	std::string _DeviceUser;
	std::string _DevicePass;

	std::string _CloudHost;
	std::string _CloudPort;
	std::string _CloudUser;
	std::string _CloudPass;

	std::string _AppName;
	std::string _SiteId;

	Notifier _NotificationChannel;

	SignalHandler   _SigHdlr;
	BrokerSignalHandler _AppSignals;
};

extern IPCamBroker* broker;

#endif