/*
BSD 2-Clause License
Copyright (c) 2017, Subrato Roy (subratoroy@hotmail.com)
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "MessageBroker.h"
#include <CoreLib/Logger.h>
#include <CoreLib/StringEx.h>
#include <CoreLib/Responder.h>
#include <CoreLib/SignalHandler.h>
#include "MessageBus.h"

#include <memory.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <memory.h>
#include <stdlib.h>

#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define INVALID_SOCKET (-1)
#define SOCKET int

#define SOCKET_ERROR	 (-1)
#define LPSOCKADDR sockaddr*
#define MAX_RESPONDERS 65535

#ifndef ERESTART
#define ERESTART 99
#endif

static SOCKET listener_socket = INVALID_SOCKET;
static int listener_port = -1;
static void* logger = 0;
static socklen_t addrlen;
static pthread_mutex_t socket_lock;

typedef struct responder_thread_params
{
    pthread_t thread;
    void* responder;
}responder_thread_params;

typedef struct client_node
{
    char node_name[32];
    void* responder;
}client_node;

static void* client_node_array[MAX_RESPONDERS];

static void* responder_run(void* responder_thread_params);
static void on_signal_received(SignalType type);
static bool payload_handle_protocol(payload* message, void* vptr_responder);
static bool payload_receive(payload* message, void* vptr_responder);
static bool payload_send(payload* message, void* vptr_responder);
static bool payload_broadcast_registration(const char* node_name);
static bool payload_broadcast_deregistration(const char* node_name);
static bool payload_send_nodelist(client_node* node);

bool broker_initialize(char* appname, int port)
{
    listener_port = port;

    logger = logger_allocate(10, NULL);

    if(logger == NULL)
    {
        return false;
    }

    signals_register_callback(on_signal_received);
    signals_initialize_handlers();

    pthread_mutex_init(&socket_lock, NULL);

    return true;
}

bool broker_run()
{
    listener_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in bindAddr;

    memset((void*)&bindAddr, 0, sizeof(struct sockaddr_in));

    bindAddr.sin_family = AF_INET;
    bindAddr.sin_port = htons(listener_port);
    bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(listener_socket, (struct sockaddr*)&bindAddr, sizeof(bindAddr)) == SOCKET_ERROR)
    {
        return BindFailed;
    }

    WriteInformation(logger, "Socket bind complete");

    if(listen(listener_socket,5) == SOCKET_ERROR)
    {
        return ListenFailed;
    }

    WriteInformation(logger, "Socket in listening mode");

    while(true)
    {
        struct sockaddr remotehostaddr;
        memset((void*)&remotehostaddr, 0, sizeof(remotehostaddr));
        addrlen = sizeof(remotehostaddr);

        SOCKET client_sock = accept(listener_socket,&remotehostaddr,&addrlen);
        if(client_sock != INVALID_SOCKET)
        {
            struct responder_thread_params* params = (responder_thread_params*)calloc(1, sizeof (struct responder_thread_params));

            if (params)
            {
                params->responder = responder_assign_socket(params->responder, client_sock);

                pthread_attr_t pthread_attr;
                memset(&pthread_attr, 0, sizeof(pthread_attr_t));
                // default threading attributes
                pthread_attr_init(&pthread_attr);
                // allow a thread to exit cleanly without a join
                pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_DETACHED);
                if (pthread_create(&params->thread, &pthread_attr, responder_run, params) != 0)
                {
                    responder_close_socket(params->responder);
                    free(params->responder);
                    free(params);
                    continue;
                }
            }
            else
            {
                shutdown(client_sock, 2);
                close(client_sock);
            }
        }
        else
        {
            if ((errno != ECHILD) && (errno != ERESTART) && (errno != EINTR))
            {
                return SignalReceived;
            }
        }
    }
}

void broker_stop()
{
    pthread_mutex_lock(&socket_lock);

    for(size_t index = 0; index < MAX_RESPONDERS; index++)
    {
        client_node* ptr = client_node_array[index];

        if(ptr != NULL)
        {
            shutdown(index, 0);
            close(index);
            free(ptr->responder);
            free(ptr);
        }
    }

    pthread_mutex_unlock(&socket_lock);

    shutdown(listener_socket, 2);
    close(listener_socket);
    pthread_mutex_destroy(&socket_lock);
    logger_release(logger);
}

void* responder_run(void* responder_thread_params)
{
    pthread_detach(pthread_self());

    struct responder_thread_params* params = (struct responder_thread_params*)responder_thread_params;

    if(params == NULL)
    {
        pthread_exit(NULL);
        return NULL;
    }

    if(params->responder == NULL)
    {
        pthread_cancel(params->thread);
        free(responder_thread_params);
        pthread_exit(NULL);                
        return NULL;
    }

    client_node* disconnected_node = NULL;
    SOCKET current_socket = responder_get_socket(params->responder);
    bool ret = true;

    while(true)
    {
        payload client_payload = {0};

        if(payload_receive(&client_payload, params->responder))
        {
            ret = payload_handle_protocol(&client_payload, params->responder);

            if (client_payload.data_size > 0 && client_payload.data != NULL)
            {
                free(client_payload.data);
            }

            if (!ret)
            {
                break;
            }
        }
        else
        {
            pthread_mutex_lock(&socket_lock);
            char node_name[32] = { 0 };

            disconnected_node = client_node_array[current_socket];

            strcpy(node_name, ((client_node*)client_node_array[current_socket])->node_name);

            responder_close_socket(params->responder);
            client_node_array[current_socket] = NULL;

            payload_broadcast_deregistration(node_name);

            pthread_mutex_unlock(&socket_lock);

            break;
        }
    }

    pthread_cancel(params->thread);
    free(disconnected_node);
    free(params->responder);
    free(params);
    pthread_exit(NULL);
}

bool payload_handle_protocol(payload* message, void* vptr_responder)
{
    struct responder* responder_ptr = (struct responder*)vptr_responder;

    if(!responder_ptr)
    {
        return false;
    }

    int sender_socket = responder_get_socket(responder_ptr);

    /*
    // Event => Registration
    if(message->payload_type == Event && message->payload_sub_type == Register)
    {
        pthread_mutex_lock(&socket_lock);

        // Make sure that the registration request is from a new node
        client_node* new_node = client_node_array[sender_socket];

        if(new_node == NULL)
        {
            new_node = (struct client_node*)calloc(1, sizeof (struct client_node));

            if (new_node)
            {
                strcpy(new_node->node_name, message->sender);
                new_node->responder = responder_ptr;
                client_node_array[sender_socket] = new_node;

                // Send Node Online event to all other nodes
                payload_broadcast_registration(message->sender);

                // Send node list payload for the new node
                payload_send_nodelist(new_node);
            }
        }

        pthread_mutex_unlock(&socket_lock);

        return true;
    }

    // Event => Deregistration
    if(message->payload_type == Event && message->payload_sub_type == DeRegister)
    {
        pthread_mutex_lock(&socket_lock);

        client_node* old_node = client_node_array[sender_socket];

        // If we have the node in our array
        if(old_node != NULL)
        {
            payload_broadcast_deregistration(old_node->node_name);

            // Now remove the old node from the array
            //responder_close_socket(old_node->responder);
            //client_node_array[sender_socket] = NULL;
        }

        pthread_mutex_unlock(&socket_lock);
                
        return false;
    }

    //Loopback
    if(message->payload_type == Data && message->payload_sub_type == LoopBack)
    {
        payload_send(message, responder_ptr);
        return true;
    }
    */

    // All other payload types that carry trailing data buffers
    if(message->payload_type == Data || message->payload_type == Request || message->payload_type == Response || message->payload_type ==  Event)
    {
        pthread_mutex_lock(&socket_lock);

        for(size_t index = 0; index < MAX_RESPONDERS; index++)
        {
            client_node* ptr = client_node_array[index];

            if(ptr != NULL)
            {
                if(strcmp(ptr->node_name, message->receipient) == 0)
                {
                    payload_send(message, ptr->responder);
                    break;
                }
            }
        }

        pthread_mutex_unlock(&socket_lock);

        return true;
    }

    return true;
}

bool payload_broadcast_registration(const char* node_name)
{
    char buffer[129] = {0};
    sprintf(buffer, "Node %s REGISTRATION", node_name);
    WriteInformation(logger, buffer);

    for (size_t index = 0; index < MAX_RESPONDERS; index++)
    {
        if (client_node_array[index] == NULL)
        {
            continue;
        }

        client_node* ptr = client_node_array[index];

        if (strcmp(ptr->node_name, node_name) == 0)
        {
            continue;
        }

        struct payload node_online_message = { 0 };
        node_online_message.payload_type = Event;
        node_online_message.payload_data_type = Text;
        node_online_message.payload_id = 0;
        strcpy(node_online_message.receipient, ptr->node_name);
        strcpy(node_online_message.sender, "MessageBus");
        node_online_message.data_size = strlen(node_name);

        node_online_message.data = calloc(1, node_online_message.data_size + (size_t)1);
        if (node_online_message.data)
        {
            strcpy(node_online_message.data, node_name);
            payload_send(&node_online_message, ptr->responder);
            free(node_online_message.data);
            return true;
        }
    }

    return false;
}

bool payload_broadcast_deregistration(const char* node_name)
{
    char buffer[129] = {0};
    sprintf(buffer, "Node %s DEREGISTRATION", node_name);
    WriteInformation(logger, buffer);

    // Send Node Offline event to all other nodes
    for (size_t index = 0; index < MAX_RESPONDERS; index++)
    {
        if (client_node_array[index] == NULL)
        {
            continue;
        }

        client_node* ptr = client_node_array[index];

        if (strcmp(ptr->node_name, node_name) == 0)
        {
            continue;
        }

        struct payload node_offline_message;
        node_offline_message.payload_type = Event;
        node_offline_message.payload_data_type = Text;
        node_offline_message.payload_id = 0;
        strcpy(node_offline_message.receipient, ptr->node_name);
        strcpy(node_offline_message.sender, "MessageBus");
        node_offline_message.data_size = strlen(node_name);
        node_offline_message.data = calloc(1, node_offline_message.data_size + (size_t)1);

        if (node_offline_message.data)
        {
            strcpy(node_offline_message.data, node_name);
            payload_send(&node_offline_message, ptr->responder);
            free(node_offline_message.data);
            return true;
        }
    }

    return false;
}

bool payload_send_nodelist(client_node* node)
{
    struct payload node_list_message = { 0 };
    node_list_message.payload_type = Event;
    node_list_message.payload_data_type = Text;
    node_list_message.payload_id = 0;
    strcpy(node_list_message.receipient, node->node_name);
    strcpy(node_list_message.sender, "MessageBus");

    for (size_t index = 0; index < MAX_RESPONDERS; index++)
    {
        if (client_node_array[index] == NULL)
        {
            continue;
        }

        client_node* ptr = client_node_array[index];

        if (strcmp(ptr->node_name, node->node_name) == 0)
        {
            continue;
        }

        if (node_list_message.data == NULL)
        {
            size_t len = strlen(ptr->node_name);
            node_list_message.data = calloc(1, len + 2);

            if (node_list_message.data)
            {
                strcpy(node_list_message.data, ptr->node_name);
                node_list_message.data[len] = ',';
            }
        }
        else
        {
            size_t len = strlen(ptr->node_name) + strlen(node_list_message.data);

            char* temp_buffer = calloc(1, len + (size_t)2);
            if (temp_buffer)
            {
                strcpy(temp_buffer, (char*)node_list_message.data);
                strcat(temp_buffer, ptr->node_name);
                temp_buffer[len] = ',';

                free(node_list_message.data);
                node_list_message.data = temp_buffer;
            }
        }
    }

    //Out of loop

    if (node_list_message.data)
    {
        node_list_message.data[strlen(node_list_message.data) - 1] = 0;
        node_list_message.data_size = strlen(node_list_message.data);

        // Now send Node List event to the newly added node
        payload_send(&node_list_message, node->responder);
        free(node_list_message.data);
        return true;
    }
    
    return false;
}

bool payload_receive(payload* message, void* vptr_responder)
{
    if(!message)
    {
        return false;
    }

    struct responder* responder_ptr = (struct responder*)vptr_responder;

    if(!responder_ptr)
    {
        return  false;
    }

    if(!responder_is_connected(responder_ptr))
    {
        return false;
    }

    if(!responder_receive_buffer(responder_ptr, (char*)&message, sizeof (struct payload) - sizeof(char*), false))
    {
        return false;
    }

    if(message->data_size > 0)
    {
        if(!responder_receive_buffer(responder_ptr, &message->data, message->data_size, true))
        {
            return false;
        }
    }

    return true;
}

bool payload_send(payload* message, void* vptr_responder)
{
    if(!message)
    {
        return false;
    }

    struct responder* responder_ptr = (struct responder*)vptr_responder;

    if(!responder_ptr)
    {
        return  false;
    }

    if(!responder_is_connected(responder_ptr))
    {
        return false;
    }

    if(!responder_send_buffer(responder_ptr, (const char*)message, sizeof (struct payload) - sizeof (char*)))
    {
        return false;
    }

    if(message->data_size > 0 && message->data != NULL)
    {
        if(!responder_send_buffer(responder_ptr, message->data, message->data_size))
        {
            return false;
        }
    }

    return true;
}

void on_signal_received(SignalType stype)
{
    switch(stype)
    {
        case Suspend:
        {
            WriteLog(logger, "SUSPEND SIGNAL", LOG_CRITICAL);
            break;
        }
        case Resume:
        {
            WriteLog(logger, "RESUME SIGNAL", LOG_CRITICAL);
            break;
        }
        case Shutdown:
        {
            WriteLog(logger, "SHUTDOWN SIGNAL", LOG_CRITICAL);
            printf("\n\n====> SHUTDOWN SIGNAL <====\n\n");
            broker_stop();
            logger_release(logger);
            exit(0);
        }
        case Alarm:
        {
            WriteLog(logger, "ALARM SIGNAL", LOG_CRITICAL);
            break;
        }
        case Reset:
        {
            WriteLog(logger, "RESET SIGNAL", LOG_CRITICAL);
            break;
        }
        case ChildExit:
        {
            WriteLog(logger, "CHILD PROCESS EXIT SIGNAL", LOG_CRITICAL);
            break;
        }
        case Userdefined1:
        {
            WriteLog(logger, "USER DEFINED 1 SIGNAL", LOG_CRITICAL);
            break;
        }
        case Userdefined2:
        {
            WriteLog(logger, "USER DEFINED 2 SIGNAL", LOG_CRITICAL);
            break;
        }
        default:
        {
            WriteLog(logger, "UNKNOWN SIGNAL", LOG_CRITICAL);
            break;
        }
    }
}

