#ifndef _FILE_RETRIEVER
#define _FILE_RETRIEVER

#include <curl/curl.h>
#include <string>
#include "IPCamStructs.h"

using namespace std;

class FileRetriever
{
public:
	FileRetriever();
    ~FileRetriever();
    char retrieve(const Recording &dnl, const std::string &recdir);
private:
    CURL *curl;
};

#endif
