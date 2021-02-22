#include <stdio.h>
#include "ThingsBoardService.h"

int main(int argc, char* argv[])
{
    if(!service_initialize())
    {
        return -1;
    }

    service_start();

    service_stop();

    return 0;
}
