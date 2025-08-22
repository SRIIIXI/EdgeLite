/*

Copyright (c) 2020, CIMCON Automation
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, is allowed only with prior permission from CIMCON Automation

*/

#include "MessageBroker.h"
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int listening_port = 49151;

    if(argc > 1)
    {
        listening_port = atoi(argv[1]);
    }

    if(!broker_initialize(argv[0], listening_port))
    {
        return -1;
    }

    broker_run();

	return 0;
}
