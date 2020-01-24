#ifndef _ABNFMESSAGE
#define _ABNFMESSAGE

#include "OSDefinitions.h"
#include "StringEx.h"
#include "NetworkHelper.h"

typedef enum ContentType
{
	Text=0,
	Image=1,
	Audio=2,
	Video=3,
	Binary=4
}ContentType;

class HttpPacket
{
public:
	HttpPacket();
	HttpPacket(const char* buffer);
	virtual ~HttpPacket();

	// Incoming packet functions
    void setHeader(const char* buffer);
    bool deSerialize();

	// Outgoing packet functions
	// Request
    void setProtocolInformation(const char* request, const char* URL, const char* protocol, const char* version);
	// Response
    void setProtocolInformation(const char* protocol, const char* version, long responsecode, const char* responsetext);
    void addHeader(const char* field, const char* value);
    void serialize(std::string &sipString);

	// Common for transmission/reception
    void	attachBody(const char* buffer);

	// Reset all internal data - useful when we reuse the packet
	void reset();

    bool	hasBody();
    const char*	getRequest();
    const char*	getProtocol();
    const char*	getURL();
    const char*	getVersion();
    const char*	getResponseText();
    const char*	getContent();
    long	getResponseCode();
    long	getMessageType();
    void	getFieldValue(const char* fieldName, std::string &value);
    int getContentSize();

	std::string getHeader(std::string headerName);

private:
    void decodeMessageIdentificationLine(const char* messageLine);
    void encodeMessageIdentificationLine();
    void processLine(const char* line, std::string &field, std::string &value);
    void getLine(std::string &line);

    std::vector<std::string> _KeyList;
    std::vector<std::string> _ValueList;

    std::string	_RequestBuffer;
    char*       _Content;
    bool		_HasContent;
    std::string	_Request;
    std::string	_URL;
    std::string	_Protocol;
    std::string	_Version;
    std::string	_ResponseText;
    std::string _MessageLine;
    long		_ResponseCode;
    long		_MessageType;
    int         _ContentSize;
};

#endif
