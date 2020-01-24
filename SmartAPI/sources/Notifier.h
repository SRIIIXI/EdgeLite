#ifndef _NOTIFIER
#define _NOTIFIER

#include <curl/curl.h>
#include <string>

using namespace std;

class Notifier
{
public:
	Notifier();
    ~Notifier();
    bool notify(std::string server, std::string port, std::string user, std::string pass, std::string type, std::string payload);
private:
    std::string httpbody;
    CURL *curl;
};

#endif
