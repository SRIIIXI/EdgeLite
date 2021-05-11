#include "Simulator.h"
#include <string.h>
#include <RTUCore/File.h>
#include <pthread.h>

int main(int argc, char* argv[])
{
    if(argc == 3)
    {
        if(strcmp("-c", argv[1]) == 0)
        {
            if(file_is_exists(argv[2]))
            {
                if(!simulator_initialize(NULL))
                {
                    return -1;
                }
            }
        }
    }
    else
    {
        if(!simulator_initialize(NULL))
        {
            return -1;
        }
    }

    simulator_start();

    simulator_stop();

    return 0;
}
