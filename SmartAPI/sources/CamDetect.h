#ifndef _CAM_DETECT
#define _CAM_DETECT

#include <curl/curl.h>
#include <string>
#include "IPCamStructs.h"

using namespace std;

class CamDetect
{
public:
	CamDetect();
	~CamDetect();
	bool detect(std::string host, std::string port, std::string user, std::string pass);
	bool probe(std::string host, std::string port, std::string user, std::string pass);
private:
	std::string httpbody;
	CURL* curl;
};

#endif
