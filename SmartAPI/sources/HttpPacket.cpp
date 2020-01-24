#include "HttpPacket.h"
#include <algorithm>

HttpPacket::HttpPacket()
{
    _RequestBuffer.erase();
    _Content = NULL;
    _HasContent = false;
    _Request.erase();
    _URL.erase();
    _Protocol.erase();
    _Version.erase();
    _ResponseText.erase();
    _ResponseCode = -1;
    _MessageType = -1;
    _KeyList.clear();
    _ValueList.clear();
    _ContentSize = 0;
}

HttpPacket::HttpPacket(const char* buffer)
{
    _RequestBuffer.erase();
    _Content = NULL;
    _HasContent = false;
    _Request.erase();
    _URL.erase();
    _Protocol.erase();
    _Version.erase();
    _ResponseText.erase();
    _ResponseCode = -1;
    _MessageType = -1;
    _RequestBuffer = buffer;
    _KeyList.clear();
    _ValueList.clear();
    _ContentSize = 0;
}

void HttpPacket::reset()
{
}

HttpPacket::~HttpPacket()
{

    if(	_Content != NULL)
    {
        delete _Content;
        _Content = NULL;
    }

    _RequestBuffer.erase();
    _HasContent = false;
    _Request.erase();
    _URL.erase();
    _Protocol.erase();
    _Version.erase();
    _ResponseText.erase();
    _ResponseCode = -1;
    _MessageType = -1;
    _KeyList.clear();
    _ValueList.clear();
    _ContentSize = 0;
}

void HttpPacket::setHeader(const char* buffer)
{
	reset();
    _RequestBuffer = buffer;
}

void HttpPacket::attachBody(const char* buffer)
{
    if(	_Content != NULL)
    {
        delete _Content;
        _Content = NULL;
    }

	_ContentSize = strlen(buffer);

    _Content = new char[_ContentSize+1];
	memset(_Content, 0, _ContentSize+1);
    memcpy(_Content,buffer,_ContentSize);

	char temp[10] = { 0 };
	sprintf(temp, "%d", _ContentSize);

	addHeader("Content-Length", temp);
}

const char*	HttpPacket::getRequest()
{
    return _Request.c_str();
}

const char*	HttpPacket::getProtocol()
{
    return _Protocol.c_str();
}

const char*	HttpPacket::getURL()
{
    return _URL.c_str();
}

const char*	HttpPacket::getVersion()
{
    return _Version.c_str();
}

const char*	HttpPacket::getResponseText()
{
    return _ResponseText.c_str();
}

long HttpPacket::getResponseCode()
{
    return _ResponseCode;
}

long HttpPacket::getMessageType()
{
    return _MessageType;
}

const char*	HttpPacket::getContent()
{
    return _Content;
}

int HttpPacket::getContentSize()
{
    return _ContentSize;
}


void HttpPacket::getFieldValue(const char* fieldName, std::string &value)
{
    std::vector<std::string>::iterator iter = std::find(_KeyList.begin(), _KeyList.end(), fieldName);

    if(iter == _KeyList.end())
	{
		value = "";
	}
    else
    {
        int position = std::distance( _KeyList.begin(), iter) ;
        value = _ValueList.at(position);
    }
	return;
}


bool HttpPacket::deSerialize()
{
	std::string fieldValueParams;
	std::string field, value;

    getLine(_MessageLine);
    decodeMessageIdentificationLine(_MessageLine.c_str());

	while(true)
	{
		getLine(fieldValueParams);
		processLine(fieldValueParams.c_str(), field, value);
		if(field.length() < 1)
		{
			break;
		}

        _KeyList.push_back(field);
        _ValueList.push_back(value);

		if(strcmp(field.c_str(),"Content-Length") == 0 || strcmp(field.c_str(), "content-length") == 0)
		{
			if(atoi(value.c_str()) > 0)
			{
                _HasContent = true;
                _ContentSize = atoi(value.c_str());
			}
			else
			{
                _HasContent = false;
			}
			break;;
		}
	}
	return true;
}

bool HttpPacket::hasBody()
{
    return _HasContent;
}

void HttpPacket::getLine(std::string &line)
{
    if(_RequestBuffer.length()<1)
    {
        line.clear();
        return;
    }

	std::string next;

	std::string delim = "\r\n";

    int pos = strsplit(_RequestBuffer, delim, line, next);

    if(pos == -1)
    {
        line = _RequestBuffer;
        _RequestBuffer.clear();
        return;
    }

    _RequestBuffer = next;
}

void HttpPacket::processLine(const char *line, std::string &field, std::string &value)
{
    int delimeterpos = strcharacterpos(line, ':');

	field = "";
	value = "";
	int ctr = 0;
	for(ctr = 0; line[ctr] != 0; ctr++)
	{
        if(ctr < delimeterpos)
		{
			field += line[ctr];
		}

        if(ctr > delimeterpos+1)
		{
			value += line[ctr];
		}
        if(ctr == delimeterpos)
		{
			continue;
		}
	}
}

void HttpPacket::decodeMessageIdentificationLine(const char* requestLine)
{
    _Request.erase();
    _URL.erase();
    _Version.erase();
    _Protocol.erase();
    _ResponseCode = -1;
    _ResponseText.erase();
    _MessageType = REQUEST;

	int ws = 0;
	std::string token1, token2, token3;

	for(int index = 0; requestLine[index] != 0 ; index++)
	{
		if(requestLine[index] == ' ' || requestLine[index] == '\t')
		{
			ws++;
			continue;
		}
		if(ws > 2)
		{
			break;
		}

		if(ws == 0)
		{
			token1 += requestLine[index];
		}
		if(ws == 1)
		{
			token2 += requestLine[index];
		}
		if(ws == 2)
		{
			token3 += requestLine[index];
		}
	}

    if(strcharacterpos(token1.c_str(),'/') == -1)
	{
        _Request = token1;
        _URL = token2;
        strsplit(token3.c_str(),'/',_Protocol,_Version);
        _MessageType = REQUEST;
		return;
	}
	else
	{
        strsplit(token1.c_str(),'/',_Protocol,_Version);
        _ResponseCode = atoi(token2.c_str());
        _ResponseText = token3;
        _MessageType = RESPONSE;
		return;
	}
}

void HttpPacket::encodeMessageIdentificationLine()
{
	char tempBuffer[1024];
	memset(tempBuffer,0,1024);
    if(_MessageType == RESPONSE)
	{
        sprintf(tempBuffer,"%s/%s %d %s\r\n",_Protocol.c_str(),_Version.c_str(),_ResponseCode,_ResponseText.c_str());
	}
	else
	{
        sprintf(tempBuffer,"%s %s %s/%s\r\n",_Request.c_str(),_URL.c_str(),_Protocol.c_str(),_Version.c_str());
	}
    _MessageLine = tempBuffer;
}

void HttpPacket::setProtocolInformation(const char* request, const char* URL, const char* protocol, const char* version)
{
    _MessageType = REQUEST;
    _KeyList.clear();
    _ValueList.clear();
    _Protocol = protocol;
    _Version = version;
    _Request = request;
    _URL = URL;
}

void HttpPacket::setProtocolInformation(const char* protocol, const char* version, long responsecode, const char* responsetext)
{
    _MessageType = RESPONSE;
    _KeyList.clear();
    _ValueList.clear();
    _Protocol = protocol;
    _Version = version;
    _ResponseCode = responsecode;
    _ResponseText = responsetext;
}

void HttpPacket::addHeader(const char* field, const char* value)
{
	if (std::find(_KeyList.begin(), _KeyList.end(), field) != _KeyList.end())
	{
		return;
	}

    _KeyList.push_back(field);
    _ValueList.push_back(value);
}

std::string HttpPacket::getHeader(std::string headerName)
{
	int ctr = 0;

	for (auto s : _KeyList)
	{
		if (s == headerName)
		{
			return _ValueList[ctr];
		}
		ctr++;
	}
}


void HttpPacket::serialize(std::string &sipString)
{
	encodeMessageIdentificationLine();

    char buffer[8096]={0};
    char temp[1025]={0};

    strcpy(buffer,_MessageLine.c_str());

    int headercount = _KeyList.size();

    for(int index = 0; index < headercount; index++)
    {
        sprintf(temp,"%s: %s\r\n",((std::string)_KeyList[index]).c_str(),((std::string)_ValueList[index]).c_str());
        strcat(buffer,temp);
    }

    strcat(buffer,"\r\n\r\n");

	sipString = buffer;
}


