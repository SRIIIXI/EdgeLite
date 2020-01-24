#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include "Notifier.h"
#include "Logger.h"

struct url_data
{
    size_t size;
    char* data;
};

size_t NotifierCallback(void *ptr, size_t size, size_t nmemb, struct url_data *data)
{
    size_t index = data->size;
    size_t n = (size * nmemb);
    char* tmp;

    data->size += (size * nmemb);

    tmp = (char*)realloc(data->data, data->size + 1); // +1 for '\0'

    if(tmp)
    {
        data->data = tmp;
    }
    else
    {
        if(data->data)
        {
            free(data->data);
        }

        return 0;
    }

    memcpy((data->data + index), ptr, n);
    data->data[data->size] = '\0';

    return size * nmemb;
}

Notifier::Notifier()
{
    curl = curl_easy_init();
}

Notifier::~Notifier()
{
    if (curl)
    {
        curl_easy_cleanup(curl);
    }
}

bool Notifier::notify(std::string server, std::string port, std::string user, std::string pass, std::string type, std::string payload)
{
	std::string url = "http://";
	url += server + ":";
	url += port + "/notification&";
	url += type + "=";
	url += payload;

    struct url_data data;
    data.size = 0;
    data.data = (char*)malloc(4096); // reasonable size initial buffer

    if(NULL == data.data)
    {
        return false;
    }

    data.data[0] = '\0';

    CURLcode res;

    if (curl)
    {
        curl_easy_reset(curl);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NotifierCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC);

        curl_easy_setopt(curl, CURLOPT_USERNAME, user.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, pass.c_str());

        res = curl_easy_perform(curl);

        if(res != CURLE_OK)
        {
			writeLog(curl_easy_strerror(res), LOG_ERROR);
			writeLog(url, LOG_ERROR);
			return false;
        }

        httpbody = data.data;
    }

    return true;
}

