#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include "CamDetect.h"
#include "Logger.h"
#include "Responder.h"
#include "HttpPacket.h"

std::string ONVIFPacket = "<?xml version=\"1.0\" encoding=\"UTF-8\"?> <SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\"> <SOAP-ENV:Body> <tds:GetDeviceInformation/> </SOAP-ENV:Body> </SOAP-ENV:Envelope>";

struct url_data
{
	size_t size;
	char* data;
};

size_t ONVIFResponseCallback(void* ptr, size_t size, size_t nmemb, struct url_data* data)
{
	size_t index = data->size;
	size_t n = (size * nmemb);
	char* tmp;

	data->size += (size * nmemb);

	tmp = (char*)realloc(data->data, data->size + 1); // +1 for '\0'

	if (tmp)
	{
		data->data = tmp;
	}
	else
	{
		if (data->data)
		{
			free(data->data);
		}

		return 0;
	}

	memcpy((data->data + index), ptr, n);
	data->data[data->size] = '\0';

	return size * nmemb;
}

size_t ONVIFRequestCallback(void* ptr, size_t size, size_t nmemb, struct url_data* data)
{
	//struct upload_status* upload_ctx = (struct upload_status*)userp;
	const char* buffer;

	if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1)) 
	{
		return 0;
	}

	buffer = ONVIFPacket.c_str();

	if (buffer)
	{
		size_t len = ONVIFPacket.length();
		memcpy(ptr, buffer, len);
		return len;
	}

	return 0;
}

CamDetect::CamDetect()
{
	curl = curl_easy_init();
}

CamDetect::~CamDetect()
{
	if (curl)
	{
		curl_easy_cleanup(curl);
	}
}

bool CamDetect::detect(std::string host, std::string port, std::string user, std::string pass)
{
	stralltrim(host);
	stralltrim(port);
	stralltrim(user);
	stralltrim(pass);

	struct url_data data;
	data.size = 0;
	data.data = (char*)malloc(4096); // reasonable size initial buffer

	if (NULL == data.data)
	{
		return false;
	}

	data.data[0] = '\0';

	CURLcode res;

	if (curl)
	{
		string url = "http://" + host + ":" + port + "/";

		curl_easy_reset(curl);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_USERNAME, user.c_str());
		curl_easy_setopt(curl, CURLOPT_PASSWORD, pass.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_BASIC);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ONVIFResponseCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

		curl_easy_setopt(curl, CURLOPT_READFUNCTION, ONVIFRequestCallback);
		curl_easy_setopt(curl, CURLOPT_READDATA, &data);

		curl_easy_setopt(curl, CURLOPT_POST, 1L);

		res = curl_easy_perform(curl);

		if (res != CURLE_OK)
		{
			writeLog(curl_easy_strerror(res), LOG_ERROR);
			writeLog(url, LOG_ERROR);
			return false;
		}

		httpbody = data.data;

		if (strstr(ONVIFPacket.c_str(), "Envelope") == nullptr)
		{
			return false;
		}
	}

	return true;
}

bool CamDetect::probe(std::string host, std::string port, std::string user, std::string pass)
{
	Responder resp;

	if (resp.createSocket(host.c_str(), atoi(port.c_str())))
	{
		int retcode = 0;

		if (resp.connectSocket(retcode))
		{
			HttpPacket requestpacket;

			requestpacket.setProtocolInformation("POST", "/", "HTTP", "1.1");
			requestpacket.attachBody(ONVIFPacket.c_str());

			std::string outbuffer;

			requestpacket.serialize(outbuffer);

			resp.sendBuffer(outbuffer);
			resp.sendBuffer(ONVIFPacket);

			HttpPacket reponsepacket;

			std::string header;
			if (resp.receiveString(header, (char*)"\r\n\r\n"))
			{
				reponsepacket.setHeader(header.c_str());
				reponsepacket.deSerialize();

				if (reponsepacket.hasBody())
				{
					char* buffer = NULL;

					int len = reponsepacket.getContentSize();

					buffer = new char[len + 1];
					memset(buffer, 0, len + 1);

					if (resp.receiveBuffer(buffer, len))
					{
						reponsepacket.attachBody(buffer);
						delete buffer;
						buffer = nullptr;
					}
					else
					{
						return false;
					}
				}
			}


			return true;
		}
	}

	return false;
}

