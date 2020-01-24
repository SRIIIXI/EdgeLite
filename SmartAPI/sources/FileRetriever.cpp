#include <memory.h>
#include "FileRetriever.h"
#include "Directory.h"
#include "Logger.h"

typedef struct WebFile 
{
	std::string filename;
	FILE *stream;
	char res;
}WebFile;

static size_t CurlCallback(void *buffer, size_t size, size_t nmemb, void *stream)
{
	WebFile *out = (WebFile *)stream;

	size_t rc = fwrite(buffer, size, nmemb, out->stream);
	out->res = 'D';

	fflush(out->stream);

	return rc;
}

FileRetriever::FileRetriever()
{
	curl = curl_easy_init();
}

FileRetriever::~FileRetriever()
{
	if (curl)
	{
		curl_easy_cleanup(curl);
	}
}

char FileRetriever::retrieve(const Recording &dnl, const std::string &recdir)
{
	if (!dirisdirectory(recdir.c_str()))
	{
		dircreatedirectory(recdir.c_str());
	}

	std::string basefilename;

	dirgetname(dnl.camera_file_uri.c_str(), basefilename);

	WebFile callbackarg;
	callbackarg.stream = NULL;
 	callbackarg.filename = recdir + "/" + basefilename;
	callbackarg.res = 'E';

	callbackarg.stream = fopen(callbackarg.filename.c_str(), "wb");

	if (!callbackarg.stream)
	{
		callbackarg.res = 'E';
		return callbackarg.res;
	}

    CURLcode res;
	
    if (curl)
    {
        curl_easy_reset(curl);
        curl_easy_setopt(curl, CURLOPT_URL, dnl.camera_file_uri.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &callbackarg);

		curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC);

		//curl_easy_setopt(curl, CURLOPT_USERNAME, dnl.Username.c_str());
		//curl_easy_setopt(curl, CURLOPT_PASSWORD, dnl.Password.c_str());

        res = curl_easy_perform(curl);

		fclose(callbackarg.stream);

        if(res != CURLE_OK)
        {
			writeLog(curl_easy_strerror(res), LOG_ERROR);
			writeLog(dnl.camera_file_uri, LOG_ERROR);

			remove(callbackarg.filename.c_str());

			return callbackarg.res;
        }    		
    }

    return callbackarg.res;
}
