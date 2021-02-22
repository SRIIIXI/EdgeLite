#include "ThingsBoardService.h"
#include "ThingsBoardAdapter.h"

#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <ctype.h>
#include <ifaddrs.h>
#include <netpacket/packet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <time.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>

#include <RTUCore/MessageBus.h>
#include <RTUCore/Logger.h>
#include <RTUCore/SignalHandler.h>
#include <RTUCore/StringEx.h>
#include <RTUCore/Configuration.h>
#include <RTUCore/Directory.h>
#include <RTUCore/Buffer.h>
#include <RTUCore/Environment.h>
#include <RTUCore/Dictionary.h>
#include <RTUCore/Queue.h>

static void on_network_event(const char* node_name, PayloadType ptype, DataType dtype, const char* messagebuffer, long buffersize, long payload_id);
static void on_signal_received(SignalType type);
static void on_downlink(const char *messageptr, long len);
static void upload(const char* messagebuffer, char payload_type);

static logger_t* logger = NULL;
static message_bus_t* message_bus = NULL;
static configuration_t* config = NULL;
static char version[16] = {0};
static bool enabled = true;
static char server[33] = {0};
static long port = 1883;
static char uri[65] = {0};

static dictionary_t* connection_map = NULL;
static bool continue_loop = true;

bool service_initialize()
{
    logger = logger_allocate_default();

    if(logger == NULL)
    {
        printf("Could not open log file %s\n", logger_filename(logger));
        return false;
    }

    signals_register_callback(on_signal_received);
    signals_initialize_handlers();

    connection_map = dictionary_allocate();

    config = configuration_allocate_default();

    WriteLog(logger, configuration_filename(config), LOG_ERROR);

    if(config == NULL)
    {
        WriteLog(logger, "Could not load configuration", LOG_ERROR);
        logger_release(logger);
        return false;
    }

    WriteInformation(logger, "Found configuration");

    strcpy(version, configuration_get_value_as_string(config, "default", "version"));
    strcpy(server, configuration_get_value_as_string(config, "default", "server"));
    port = (int)configuration_get_value_as_integer(config, "default", "port");
    enabled = configuration_get_value_as_boolean(config, "default", "enabled");
    strcpy(uri, configuration_get_value_as_string(config, "default", "uri"));

    WriteInformation(logger, "Read all configuration keys");

    if(!thingsboard_initialize(logger))
    {
        WriteLog(logger, "Could not load MQTT library", LOG_ERROR);
        logger_release(logger);
        return false;
    }

    char** client_ids = configuration_get_all_keys(config, "authentication");

    for(int kindex = 0; client_ids[kindex] != 0; kindex++)
    {
        const char* key_str = configuration_get_value_as_string(config, "authentication", client_ids[kindex]);

        long tpos = strindexofchar(key_str, ':');

        if(tpos > 0)
        {
            char userid[33] = {0};
            char passwd[33] = {0};
            strncpy(userid, &key_str[0], tpos);
            strncpy(passwd, &key_str[tpos+1], 32);
            thingsboard_adapter_t* adapter = thingsboard_connect(logger, client_ids[kindex], server, port, userid, passwd, on_downlink);

            if(adapter != NULL)
            {
                dictionary_set_reference(connection_map, client_ids[kindex], strlen(client_ids[kindex]), adapter);
            }
        }
    }

    strfreelist(client_ids);

    return true;
}

bool service_start()
{
    if(!enabled)
    {
        WriteInformation(logger, "ThingsBoard Service is not enabled");
        return  false;
    }

    message_bus = message_bus_initialize(on_network_event);
    if(!message_bus)
    {
        WriteLog(logger, "Could not intialize IPC", LOG_ERROR);
        return false;
    }

    WriteInformation(logger, "Initialized message bus client");

    if(!message_bus_open(message_bus))
    {
        WriteLog(logger, "Could not open IPC", LOG_ERROR);
        return false;
    }

    WriteInformation(logger, "Connected to message bus");

    while(continue_loop)
    {
        sleep(10);
    }

    return true;
}

bool service_restart()
{
    return false;
}

bool service_stop()
{
    char** connections = dictionary_get_all_keys(connection_map);

    for(int sindex = 0; connections[sindex] != 0; sindex++)
    {
        char* client_id = NULL;
        client_id = connections[sindex];

        thingsboard_adapter_t* adapter = dictionary_get_value(connection_map, client_id, strlen(client_id));

        thingsboard_close(adapter);
    }

    strfreelist(connections);
    thingsboard_release();
    dictionary_free(connection_map);

    WriteInformation(logger, "Closed and released protocol adapter");
    message_bus_close(message_bus);
    WriteInformation(logger, "Closed connection to message bus");
    message_bus_release(message_bus);
    WriteInformation(logger, "Released message bus");
    logger_release(logger);

    return false;
}

void on_network_event(const char* node_name, PayloadType ptype, DataType dtype, const char* messagebuffer, long buffersize, long payload_id)
{
    upload(messagebuffer, ptype);
}

void on_downlink(const char *messageptr, long len)
{
    return;
}

void upload(const char *messagebuffer, char payload_type)
{
    char device_id[65] = {0};

    //Normalize the payload and extract the device ID

    char* buffer = (char*)calloc(1, strlen(messagebuffer)+4);
    strcat(buffer, "{");
    strcat(buffer, messagebuffer);
    strcat(buffer, "}");
    strrepsubstrall(buffer, "timestamp", "ts");

    const long pos = strindexofsubstr(buffer, "peripheral_id");

    if(pos > -1)
    {
        char *temp = (char*)calloc(1, 65);
        for(long ctr = pos, idx=0; buffer[ctr] != ',' && buffer[ctr] != '}'; ctr++, idx++)
        {
            temp[idx] = buffer[ctr];
        }

        char *key, *value;
        key = (char*)calloc(1, 33);
        value = (char*)calloc(1, 33);
        strsplitkeyvaluechar(temp, ':', &key, &value);
        memset(temp, 0, 65);
        strcpy(temp, value);
        free(key);
        free(value);
        strrepcharall(temp, '"', ' ');
        strcpy(device_id, stralltrim(temp));
        free(temp);
    }

    //String handling complete

    thingsboard_adapter_t* adapter = NULL;

    adapter = (thingsboard_adapter_t*)dictionary_get_value(connection_map, device_id, strlen(device_id));

    if(adapter == NULL)
    {
        const char* token = configuration_get_value_as_string(config, "authentication", device_id);

        if(token != NULL)
        {
            long tpos = strindexofchar(token, ':');

            if(tpos > 0)
            {
                char userid[33] = {0};
                char passwd[33] = {0};
                strncpy(userid, &token[0], tpos);
                strncpy(passwd, &token[tpos+1], 32);

                adapter = thingsboard_connect(logger, device_id, server, port, userid, passwd, on_downlink);

                if(adapter == NULL)
                {
                    return;
                }

                dictionary_set_value(connection_map, device_id, strlen(device_id), adapter, sizeof (thingsboard_adapter_t*));
            }
        }
    }

    WriteInformation(logger, buffer);

    if(payload_type == Data)
    {
        thingsboard_send_data(adapter, buffer, uri);
    }

    if(payload_type == Event)
    {
        thingsboard_send_event(adapter, buffer, uri);
    }

    free(buffer);
}

void on_signal_received(SignalType stype)
{
    switch(stype)
    {
        case Suspend:
        {
            WriteLog(logger, "SUSPEND SIGNAL", LOG_WARNING);
            break;
        }
        case Resume:
        {
            WriteLog(logger, "RESUME SIGNAL", LOG_WARNING);
            break;
        }
        case Shutdown:
        {
            WriteLog(logger, "SHUTDOWN SIGNAL", LOG_CRITICAL);
            continue_loop = false;
        }
        case Alarm:
        {
            WriteLog(logger, "ALARM SIGNAL", LOG_WARNING);
            break;
        }
        case Reset:
        {
            WriteLog(logger, "RESET SIGNAL", LOG_WARNING);
            break;
        }
        case ChildExit:
        {
            WriteLog(logger, "CHILD PROCESS EXIT SIGNAL", LOG_WARNING);
            break;
        }
        case Userdefined1:
        {
            WriteLog(logger, "USER DEFINED 1 SIGNAL", LOG_WARNING);
            break;
        }
        case Userdefined2:
        {
            WriteLog(logger, "USER DEFINED 2 SIGNAL", LOG_WARNING);
            break;
        }
        case WindowResized:
        {
            WriteLog(logger, "WINDOW SIZED CHANGED SIGNAL", LOG_WARNING);
            break;
        }
        default:
        {
            WriteLog(logger, "UNKNOWN SIGNAL", LOG_WARNING);
            break;
        }
    }
}
