#ifndef _WEB_RETRIEVER
#define _WEB_RETRIEVER

#include <curl/curl.h>
#include <string>

using namespace std;

class WebRetriever
{
public:
	WebRetriever();
    ~WebRetriever();
    bool retrieve(std::string url, std::string user, std::string pass);
	bool retrieve(std::string url);
	std::string &getHttpBody();
private:
    std::string httpbody;
    CURL *curl;
};

#endif
