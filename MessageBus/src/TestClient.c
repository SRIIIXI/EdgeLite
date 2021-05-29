//main.c

#include "MessageBus.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

void network_event(const char* node_name, PayloadType ptype, MessageType mtype, DataType dtype, const char* messagebuffer, long buffersize, long payload_id);

int main(int argc, char* argv[])
{
    void* message_bus = NULL;

    if(!message_bus_initialize(&message_bus, network_event))
    {
        return -1;
    }

    if(!message_bus_open(message_bus))
    {
        return -1;
    }

    printf("I am %s. I am light and slim. I use a C based IPC\n", message_bus_localname(message_bus));

    char str[] = "Hello Peer! Greetings";
    long payload_id = 0;
    int snooze_time = 0;

    message_bus_send_loopback(message_bus);

    while(snooze_time < 15)
    {
        long node_index = -1;
    //Assume we have another node name "MyPeer"
        node_index = message_bus_has_node(message_bus, "MyPeer");
        if (node_index > -1)
        {
            payload_id++;
            char* node_full_name = message_bus_node_fullname(message_bus, node_index);
            message_bus_send(message_bus, node_full_name, Data, UserData, Text, str, strlen(str), &payload_id);
        }

        sleep(5);

        snooze_time += 5;
    }

    message_bus_close(message_bus);

    return 0;
}

void network_event(const char* node_name, PayloadType ptype, MessageType mtype, DataType dtype, const char* messagebuffer, long buffersize, long payload_id)
{
    printf("%s %c %c %c %ld %s %ld\n", node_name, ptype, mtype, dtype, buffersize, messagebuffer, payload_id);
}

/*
CMakeList.txt for the above
---------------------------
cmake_minimum_required(VERSION 3.5)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(Project ExampleClient)
project(${Project})

include_directories(/include/ /usr/include/ /usr/local/include/)
link_directories(/lib /usr/lib /usr/local/lib)
link_libraries(rt pthread dl messagebus)
add_executable(${Project} main.c)
---------------------------
*/
