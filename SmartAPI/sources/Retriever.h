#ifndef _RETRIEVER
#define _RETRIEVER

#include <curl/curl.h>
#include <string>

using namespace std;

class Retriever
{
public:
	Retriever();
    ~Retriever();
    bool retrieve(std::string url, std::string user, std::string pass);
	bool retrieve(std::string url);
	std::string &getHttpBody();
private:
    std::string httpbody;
    CURL *curl;
};

#endif
