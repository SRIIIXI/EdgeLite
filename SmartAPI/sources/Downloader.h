#ifndef _DOWNLOADER
#define _DOWNLOADER

#include <curl/curl.h>
#include <string>
#include "IPCamStructs.h"

using namespace std;

class Downloader
{
public:
	Downloader();
    ~Downloader();
    char retrieve(const Recording &dnl, const std::string &recdir);
private:
    CURL *curl;
};

#endif
