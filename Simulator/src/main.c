#include "Simulator.h"

int main(int argc, char* argv[])
{
    if(!simulator_initialize())
    {
        return -1;
    }

    simulator_start();

    simulator_stop();

    return 0;
}
