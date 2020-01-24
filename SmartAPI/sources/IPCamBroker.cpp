#include "IPCamBroker.h"
#include "Logger.h"
#include "CamDetect.h"
#include "Timestamp.h"
#include "PlaylistGen.h"
#include "Retriever.h"
#include "Downloader.h"
#include "Configuration.h"
#include "Base64.h"
#include <curl/curl.h>

IPCamBroker* broker = nullptr;

void listProcessorThreadFunc();
void fileProcessorThreadFunc();

BrokerSignalHandler::BrokerSignalHandler()
{

}

void BrokerSignalHandler::suspend()
{
	writeLog("SUSPEND SIGNAL", LOG_CRITICAL);
}

void BrokerSignalHandler::resume()
{
	writeLog("RESUME SIGNAL", LOG_CRITICAL);
}

void BrokerSignalHandler::shutdown()
{
	writeLog("SHUTDOWN SIGNAL", LOG_CRITICAL);
	exit(0);
}

void BrokerSignalHandler::alarm()
{
	writeLog("ALARM SIGNAL", LOG_CRITICAL);
}

void BrokerSignalHandler::reset()
{
	writeLog("RESET SIGNAL", LOG_CRITICAL);
}

void BrokerSignalHandler::childExit()
{
	writeLog("CHILD PROCESS EXIT SIGNAL", LOG_CRITICAL);
}

void BrokerSignalHandler::userdefined1()
{
	writeLog("USER DEFINED 1 SIGNAL", LOG_CRITICAL);
}

void BrokerSignalHandler::userdefined2()
{
	writeLog("USER DEFINED 2 SIGNAL", LOG_CRITICAL);
}

IPCamBroker::IPCamBroker(std::string appname)
{
	curl_global_init(CURL_GLOBAL_ALL);
	_AppName = appname;
	Logger::GetInstance()->setModuleName(_AppName.c_str());
	Logger::GetInstance()->startLogging();	
	broker = this;
}

IPCamBroker::~IPCamBroker()
{
	curl_global_cleanup();
}

bool IPCamBroker::initialize(std::string port)
{
	_Port = port;

	if (!NetworkHelper::initializeNetworkHelper())
	{
		writeLog("Could not initialize WINSOCK", LOG_CRITICAL);
		return false;
	}
	else
	{
		writeLog("Initialized network", LOG_INFO);
	}

	_SigHdlr.registerCallbackClient(&_AppSignals);
	_SigHdlr.registerSignalHandlers();

	std::string parent_dir, current_dir;
	dircurrentdirectory(current_dir);
	dirgetparentdirectory(current_dir, parent_dir);

	_PhysicalFolder = parent_dir + "/recordings/";
	_PlaylistFolder = parent_dir + "/playlists/";
	_VirtualFolder = "http://localhost:" + _Port + "/recordings/";

	std::string dbdir = parent_dir + "/db/";
	_DatabaseFileName = dbdir + "IPCamBroker.db";

	std::string confdir = parent_dir + "/etc/";
	std::string conffilename = confdir + "IPCamBroker.ini";

	Configuration conf;

	if (!conf.loadConfiguration(conffilename))
	{
		writeLog("Error while loading configuration", LOG_CRITICAL);
		return false;
	}

	writeLog("Loaded config file", LOG_INFO);

	_VirtualFolder = conf.getValue("Device", "VirtualFolder", "http://localhost/recordings/");
	_Port = conf.getValue("Device", "Device", "9090");
	_SiteId = conf.getValue("Device", "SiteId", "MySite");
	_DeviceUser = conf.getValue("Device", "Username", "admin");
	_DevicePass = conf.getValue("Device", "Password", "admin");

	_CloudHost = conf.getValue("Cloud", "Host", "127.0.0.1");
	_CloudPort = conf.getValue("Cloud", "Port", "7070");
	_CloudUser = conf.getValue("Cloud", "Username", "admin");
	_CloudPass = conf.getValue("Cloud", "Password", "admin");

	std::string errmsg;

	if (!dirisdirectory(_PlaylistFolder))
	{
		dircreatedirectory(_PlaylistFolder);
	}

	if (!dirisdirectory(_PhysicalFolder))
	{
		dircreatedirectory(_PhysicalFolder);
	}

	if (!dirisdirectory(dbdir))
	{
		dircreatedirectory(dbdir);
		DbLocal::createDatabase(_DatabaseFileName);
	}
	else
	{
		DbLocal db;

		if (!db.openDatabase(_DatabaseFileName, errmsg))
		{
			remove(_DatabaseFileName.c_str());
			DbLocal::createDatabase(_DatabaseFileName);
		}
		db.closeDatabase();
	}

	_Local.openDatabase(_DatabaseFileName, errmsg);
	
	std::list<Site> sitelist;
	_Local.getSites(sitelist);

	if (sitelist.size() <= 0)
	{
		writeLog("Creating default site", LOG_INFO);
		std::vector<std::string> datarow;
		datarow.push_back(_SiteId);
		datarow.push_back("127.0.0.1");
		datarow.push_back("8080");
		datarow.push_back("NameFirst");
		datarow.push_back("NameLast");
		datarow.push_back("9999999999");
		datarow.push_back("siteowner@site.com");
		datarow.push_back("19800101");
		datarow.push_back("20381231");
		_SiteId = datarow[0];
		//Add site
		_Local.createSite(datarow);
	}
	
	return true;
}

void IPCamBroker::run()
{
	writeLog("Staring the list handling thread", LOG_INFO);
	std::thread listprocthread(listProcessorThreadFunc);
	listprocthread.detach();

	writeLog("Staring the file handling thread", LOG_INFO);
	std::thread fileprocthread(fileProcessorThreadFunc);
	fileprocthread.detach();

	writeLog("Starting run", LOG_INFO);

	_Listener.setEventHandler(this);

	RunState st = _Listener.run(atoi(_Port.c_str()));
	
	char buffer[32] = { 0 };

	sprintf(buffer, "Listener exit code: %d", (int)st);

	writeLog(buffer, LOG_INFO);

	NetworkHelper::deInitializeNetworkHelper();
	writeLog("Completed run", LOG_INFO);

	return;
}

int IPCamBroker::eventGet(HttpPacket *messagePtr, std::string &content, std::string &ctype)
{
	Authority auth = authorityLevel(messagePtr);
	if (auth == UnAuthorized)
	{
		return 401;
	}

	ctype = "csv";
	std::vector<std::string> params;
	strsplit(messagePtr->getURL(), params, '/', true);

	std::string objectid = "";

	if (params.size() > 2)
	{
		objectid = params[2];
	}

	if (params.size() == 1 && params[0] == "")
	{
		return _Local.getSites(content);
	}
	else
	{
		if (params[1] == "site")
		{
			writeLog("GET Site", LOG_INFO);

			if (_Local.getSites(content))
			{
				return 200;
			}
			else
			{
				return 404;
			}
		}

		if (params[1] == "camera")
		{
			writeLog("GET Camera", LOG_INFO);
			if (_Local.getCameras(content, objectid))
			{
				return 200;
			}
			else
			{
				return 404;
			}
		}

		if (params[1] == "recording")
		{
			writeLog("GET Recording", LOG_INFO);
			if (_Local.getRecording(content, objectid))
			{
				return 200;
			}
			else
			{
				return 404;
			}
		}
	}

	return 400;
}

int IPCamBroker::eventPost(HttpPacket *messagePtr, std::string &content, std::string &ctype)
{
	Authority auth = authorityLevel(messagePtr);
	if (auth == UnAuthorized)
	{
		return 401;
	}

	ctype = "csv";
	std::vector<std::string> params;
	strsplit(messagePtr->getURL(), params, '/', true);

	std::vector<std::string> payload;
	strsplit(messagePtr->getContent(), payload, '\n', true);

	std::vector<std::string> datarow;
	strreplace(payload[1], '\r', '\0');
	strsplit(payload[1], datarow, ',', true);

	std::string objectid = "";

	if (params.size() > 2)
	{
		objectid = params[2];
	}

	if (params[1] == "camera")
	{
		writeLog("POST Camera", LOG_INFO);
		content = messagePtr->getContent();
		if (_Local.createCamera(datarow))
		{
			std::string str = content;
			removeHeader(str);
			_NotificationChannel.notify(_CloudHost, _CloudPort, _CloudUser, _CloudPass, "CAMA", str);
			return 200;
		}
		else
		{
			return 404;
		}
	}

	if (params[1] == "snapshot")
	{
		writeLog("SNAPSHOT Camera", LOG_INFO);

		return 400;
	}

	if (params[1] == "ping")
	{
		writeLog("PING Camera", LOG_INFO);

		CamDetect dec;

		bool ret = dec.detect(datarow[0], datarow[1], datarow[2], datarow[3]);

		if (ret)
		{
			content = messagePtr->getContent();
			return 200;
		}

		return 404;
	}

	if (params[1] == "detect")
	{
		writeLog("DETECT and GET Camera", LOG_INFO);

		CamDetect dec;

		bool ret = dec.detect(datarow[0], datarow[1], datarow[2], datarow[3]);

		if (ret)
		{
			Timestamp ts;

			std::string ts_str = ts.getDateString("yyyyMMddhhmmss");

			std::string header = "camera_id, camera_name, site_id, ip_address, onvif_port, sd_port, user_id, password, is_active, last_accessed, last_file_timestamp, last_dir_timestamp, download_required, slot_1, slot_2, slot_3, slot_4\n";

			std::string camdata = "";
			camdata += ts_str + ",";
			camdata += ts_str + ",";
			camdata += _SiteId + ",";
			camdata += datarow[0] + ",";
			camdata += datarow[1] + ",";
			camdata += "8888,";
			camdata += datarow[2] + ",";
			camdata += datarow[3] + ",";
			camdata += "Y,";
			camdata += ts_str + ",";
			camdata += "198001010000000,19800101,Y,000000:055959,060000:115959,120000:175959,180000:235959";
			
			std::vector<std::string> camdatarow;
			strsplit(camdata, camdatarow, ',', true);

			if (_Local.createCamera(camdatarow))
			{
				content = header + camdata + "\n";
				_NotificationChannel.notify(_CloudHost, _CloudPort, _CloudUser, _CloudPass, "SITEU", camdata);
				return 200;
			}

			return 404;
		}
		else
		{
			return 404;
		}
	}

	if (params[1] == "recording" || params[1] == "site")
	{
		return 405;
	}

	return 400;
}

int IPCamBroker::eventPut(HttpPacket* messagePtr, std::string& content, std::string& ctype)
{
	Authority auth = authorityLevel(messagePtr);
	if (auth == UnAuthorized)
	{
		return 401;
	}

	ctype = "csv";
	std::vector<std::string> params;
	strsplit(messagePtr->getURL(), params, '/', true);

	std::vector<std::string> payload;
	strsplit(messagePtr->getContent(), payload, '\n', true);

	std::vector<std::string> datarow;
	strsplit(payload[1], datarow, ',', true);

	std::string objectid = "";

	if (params.size() > 2)
	{
		objectid = params[2];
	}

	if (params[1] == "site")
	{
		writeLog("PUT Site", LOG_INFO);
		content = messagePtr->getContent();
		if (_Local.updateSite(datarow, datarow[0]))
		{
			std::string str = content;
			removeHeader(str);
			_NotificationChannel.notify(_CloudHost, _CloudPort, _CloudUser, _CloudPass, "SITEU", str);
			return 200;
		}
		else
		{
			return 404;
		}
	}

	if (params[1] == "camera")
	{
		writeLog("PUT Camera", LOG_INFO);
		content = messagePtr->getContent();
		if (_Local.updateCamera(datarow, datarow[0]))
		{
			std::string str = content;
			removeHeader(str);
			_NotificationChannel.notify(_CloudHost, _CloudPort, _CloudUser, _CloudPass, "CAMU", str);
			return 200;
		}
		else
		{
			return 404;
		}
	}

	if (params[1] == "recording")
	{
		writeLog("PUT Recording", LOG_INFO);
		content = messagePtr->getContent();
		if (_Local.updateRecording(datarow, datarow[0]))
		{
			std::string str = content;
			removeHeader(str);
			_NotificationChannel.notify(_CloudHost, _CloudPort, _CloudUser, _CloudPass, "RECU", str);
			return 200;
		}
		else
		{
			return 404;
		}
	}

	return 400;
}

int IPCamBroker::eventDelete(HttpPacket *messagePtr, std::string &content, std::string &ctype)
{
	Authority auth = authorityLevel(messagePtr);
	if (auth == UnAuthorized)
	{
		return 401;
	}

	ctype = "csv";
	std::vector<std::string> params;
	strsplit(messagePtr->getURL(), params, '/', true);

	std::string objectid = "";

	if (params.size() > 2)
	{
		objectid = params[2];
	}

	if (params[1] == "camera")
	{
		writeLog("DELETE Camera", LOG_INFO);

		std::string cam;
		_Local.getCamera(cam, objectid);

		if (_Local.deleteCamera(objectid))
		{
			content = cam;
			removeHeader(cam);
			_NotificationChannel.notify(_CloudHost, _CloudPort, _CloudUser, _CloudPass, "CAMD", cam);
			return 200;
		}
		else
		{
			return 404;
		}
	}

	if (params[1] == "recording" || params[1] == "site")
	{
		return 403;
	}

	return 404;
}

std::string IPCamBroker::virtualDirectory()
{
	return _VirtualFolder;
}

std::string IPCamBroker::physicalDirectory()
{
	return _PhysicalFolder;
}

std::string IPCamBroker::playlistDirectory()
{
	return _PlaylistFolder;
}

DbLocal* IPCamBroker::database()
{
	return &_Local;
}

bool IPCamBroker::canDownload(Camera* camPtr)
{
	char str_start[10] = { 0 };
	char str_end[10] = { 0 };

	Timestamp ts;

	std::string tm_str = ts.getDateString("hhmmss");
	unsigned long currtime = atol(tm_str.c_str());

	memcpy(str_start, &camPtr->slot_1.c_str()[0], 6);
	memcpy(str_end, &camPtr->slot_1.c_str()[7], 6);

	if (currtime >= atol(str_start) && currtime <= atol(str_end))
	{
		return true;
	}

	memcpy(str_start, &camPtr->slot_2.c_str()[0], 6);
	memcpy(str_end, &camPtr->slot_2.c_str()[7], 6);

	if (currtime >= atol(str_start) && currtime <= atol(str_end))
	{
		return true;
	}

	memcpy(str_start, &camPtr->slot_3.c_str()[0], 6);
	memcpy(str_end, &camPtr->slot_3.c_str()[7], 6);

	if (currtime >= atol(str_start) && currtime <= atol(str_end))
	{
		return true;
	}

	memcpy(str_start, &camPtr->slot_4.c_str()[0], 6);
	memcpy(str_end, &camPtr->slot_4.c_str()[7], 6);

	if (currtime >= atol(str_start) && currtime <= atol(str_end))
	{
		return true;
	}

	return false;
}

Authority IPCamBroker::authorityLevel(HttpPacket* messagePtr)
{
	std::string auth_header = messagePtr->getHeader("Authorization");
	strreplace(auth_header, "Basic ", "");
	Base64 b64;
	unsigned long olen = 0;
	std::string decoded_auth_header = (char*)b64.decodeBase64(auth_header.c_str(), auth_header.length(), olen);
	decoded_auth_header[olen] = '\0';

	std::string device_tok = _DeviceUser + ":" + _DevicePass;
	std::string cloud_tok = _CloudUser + ":" + _CloudPass;

	if (decoded_auth_header == device_tok)
	{
		return DeviceUser;
	}
	else
	{
		if (decoded_auth_header == cloud_tok)
		{
			return CloudUser;
		}
	}

	return UnAuthorized;
}

void listProcessorThreadFunc()
{
	std::string errstr;

	while (true)
	{
		procsleep(30);

		//Get the camera list - always load each time
		std::list<Camera> camlist;

		std::string site_id = "";

		broker->database()->getCameras(camlist, site_id);

		writeLog("Fetched camera list", LOG_INFO);

		std::list<Camera>::iterator it = camlist.begin();

		while (it != camlist.end())
		{
			procsleep(5);

			PlaylistGen stg((*it));

			writeLog((*it).camera_name + " => processing", LOG_INFO);

			//Get the playlist
			if (stg.downloadFilelist())
			{
				writeLog((*it).camera_name + " => recording list downloaded", LOG_INFO);

				//Write out the playlist
				std::string pdir = broker->playlistDirectory();
				std::string ddir = broker->physicalDirectory();
				std::string vdir = broker->virtualDirectory();

				if (stg.writeOutPlaylist(pdir, ddir, vdir, broker->database()))
				{
					writeLog((*it).camera_name + " => generated playlist", LOG_INFO);

					//Update the camera's last file timestamp in the database
					stg.updateLastAccessTime(broker->database());

					writeLog((*it).camera_name + " => updated timestamps", LOG_INFO);
				}
			}

			++it;
		}
	}
}

void fileProcessorThreadFunc()
{
	std::string errstr;

	while (true)
	{
		procsleep(30);

		std::list<Site> sitelist;

		broker->database()->getSites(sitelist);

		if (sitelist.size() < 1)
		{
			writeLog("No site information available", LOG_ERROR);
		}

		Site site;

		for (auto s : sitelist)
		{
			site = s;
			break;
		}

		Timestamp ts;
		std::string dt_str = ts.getDateString("yyyyMMdd");
		unsigned long currdate = atol(dt_str.c_str());

		if (currdate >= atol(site.recording_start.c_str()) && currdate <= atol(site.recording_end.c_str()))
		{
			std::list<Camera> cams;
			std::string site_id = "";
			broker->database()->getCameras(cams, site_id);

			std::map<string, Camera> lookup;

			for (auto c : cams)
			{
				lookup[c.camera_id] = c;
			}

			std::list<Recording> dwnlist;

			std::string cam_id = "";

			broker->database()->getRecording(dwnlist, cam_id);

			writeLog("Fetched file list", LOG_INFO);

			std::list<Recording>::iterator it = dwnlist.begin();

			while (it != dwnlist.end())
			{
				procsleep(5);

				Camera curcam = lookup[(*it).camera_id];

				if ((*it).request_status == "Q")
				{
					if (broker->canDownload(&curcam))
					{
						Downloader ret;

						writeLog((*it).camera_file_uri + " downloading", LOG_INFO);

						char stat[2] = { 0 };

						std::string recdir;
						recdir += broker->physicalDirectory();
						recdir += "/";
						recdir += curcam.camera_name;

						stat[0] = ret.retrieve((*it), recdir);

						broker->database()->updateRecordingStatus((*it).request_id, stat);
					}
				}

				++it;
			}
		}
	}
}

void IPCamBroker::removeHeader(std::string& str)
{
	std::vector<std::string> lines;

	strsplit(str, lines, '\n', true);

	if (lines.size() > 1)
	{
		str = lines[1];
	}
}