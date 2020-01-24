// HTTPServer.cpp : Defines the entry point for the console application.
//

#include "Logger.h"
#include "IPCamBroker.h"
#include "CamDetect.h"

int main(int argc, char* argv[])
{

	IPCamBroker brokerservice(argv[0]);

	if (!brokerservice.initialize("8080"))
	{
		return -1;
	}

	brokerservice.run();

	return 0;
}
