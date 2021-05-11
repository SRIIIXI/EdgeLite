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

#include "Simulator.h"

#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <memory.h>
#include <malloc.h>
#include <time.h>
#include <math.h>

#include <RTUCore/MessageBus.h>
#include <RTUCore/Logger.h>
#include <RTUCore/SignalHandler.h>
#include <RTUCore/StringEx.h>
#include <RTUCore/StringList.h>
#include <RTUCore/Configuration.h>
#include <RTUCore/Directory.h>
#include <RTUCore/Dictionary.h>
#include <RTUCore/Buffer.h>

static void on_network_event(const char* node_name, PayloadType ptype, DataType dtype, const char* messagebuffer, long buffersize, long payload_id);
static void on_signal_received(SignalType type);
static bool read_parameter_configuration(void);

static bool get_random_boolean(void);
static float get_random_real(float lower, float upper);
static long long get_random_integer(long long lower, long long upper);
static void get_random_string(char* str);
static void get_random_timnestamp(char* str);

static message_bus_t* message_bus = NULL;
static logger_t* logger = NULL;
static configuration_t* config = NULL;
static string_list_t* destination_list = NULL;
static unsigned long parameter_count = 0;
static unsigned int sampling_rate = 0;
static SimulationParameter* simulation_parameters = NULL;
static char peripheral_id[65] = {0};
static char peripheral_name[129] = {0};
static bool continue_processing = true;

bool simulator_initialize(char const *confpath)
{
    logger = logger_allocate_default();

    if(logger == NULL)
    {
        return false;
    }

    signals_register_callback(on_signal_received);
    signals_initialize_handlers();

    if(confpath == NULL)
    {
        config = configuration_allocate_default();
    }
    else
    {
        config = configuration_allocate(confpath);
    }

    if(config == NULL)
    {
        WriteLog(logger, "Could not load configuration", LOG_ERROR);
        logger_release(logger);
        return false;
    }

    WriteInformation(logger, configuration_filename(config));

    const char* value = NULL;
    parameter_count = (unsigned long)configuration_get_value_as_integer(config, "default", "parameter_count");
    sampling_rate = (unsigned int)configuration_get_value_as_integer(config, "default", "sampling_rate");

    value = configuration_get_value_as_string(config, "default", "destination");
    if(value != NULL)
    {
        destination_list = str_list_allocate_from_string(destination_list, value, ",");
    }

    value = configuration_get_value_as_string(config, "default", "peripheral_id");
    if(value != NULL)
    {
        strcpy(peripheral_id, value);
    }

    value = configuration_get_value_as_string(config, "default", "peripheral_name");
    if(value != NULL)
    {
        strcpy(peripheral_name, value);
    }

    read_parameter_configuration();

    configuration_release(config);

    srand(time(0));

    continue_processing = true;

    return true;
}

bool read_parameter_configuration(void)
{
    simulation_parameters = (SimulationParameter*)calloc(1, sizeof (SimulationParameter)*parameter_count);

    unsigned long index = 0;
    for(index = 0; index < parameter_count; index++)
    {
        char section[16] = {0};
        sprintf(section, "parameter%lu", index+1);
        if(configuration_has_section(config, section))
        {
            WriteInformation(logger, section);
            simulation_parameters[index].UpperBound = (long long)configuration_get_value_as_integer(config, section, "upper");
            simulation_parameters[index].LowerBound  = (long long)configuration_get_value_as_integer(config, section, "lower");
            simulation_parameters[index].Type = (ParameterType)configuration_get_value_as_char(config, section, "data_type");

            if(simulation_parameters[index].Type != Integer &&
                    simulation_parameters[index].Type != Real &&
                    simulation_parameters[index].Type != Boolean &&
                    simulation_parameters[index].Type != Timestamp &&
                    simulation_parameters[index].Type != String)
            {
                simulation_parameters[index].Type = Unknown;
            }

            strncpy(simulation_parameters[index].ParameterName, configuration_get_value_as_string(config, section, "property_name"), 32);
            WriteInformation(logger, simulation_parameters[index].ParameterName);
        }
    }

    return true;
}

bool simulator_destroy()
{
    return true;
}

bool simulator_start()
{
    message_bus = message_bus_initialize(on_network_event);

    if(!message_bus)
    {
        WriteLog(logger, "Could not intialize IPC", LOG_ERROR);
        return false;
    }

    if(!message_bus_open(message_bus))
    {
        WriteLog(logger, "Could not open IPC", LOG_ERROR);
        return false;
    }

    while(continue_processing)
    {
        sleep(sampling_rate);

        buffer_t* payload = buffer_allocate_default();

        buffer_append_char(payload, '"');
        buffer_append_string(payload, "peripheral_id");
        buffer_append_char(payload, '"');
        buffer_append_char(payload, ':');
        buffer_append_char(payload, '"');
        buffer_append_string(payload, peripheral_id);
        buffer_append_char(payload, '"');

        buffer_append_char(payload, ',');

        buffer_append_char(payload, '"');
        buffer_append_string(payload, "peripheral_name");
        buffer_append_char(payload, '"');
        buffer_append_char(payload, ':');
        buffer_append_char(payload, '"');
        buffer_append_string(payload, peripheral_name);
        buffer_append_char(payload, '"');

        buffer_append_char(payload, ',');

        buffer_append_char(payload, '"');
        buffer_append_string(payload, "timestamp");
        buffer_append_char(payload, '"');
        buffer_append_char(payload, ':');
        buffer_append_unix_timestamp(payload);

        buffer_append_char(payload, ',');

        for(long index = 0; index < parameter_count; index++)
        {
            buffer_append_char(payload, '"');
            buffer_append_string(payload, simulation_parameters[index].ParameterName);
            buffer_append_char(payload, '"');
            buffer_append_char(payload, ':');

            switch (simulation_parameters[index].Type)
            {
                case Integer:
                {
                    long val = get_random_integer(simulation_parameters[index].LowerBound, simulation_parameters[index].UpperBound);
                    buffer_append_integer(payload, val);
                    break;
                }
                case Real:
                {
                    float val = get_random_real(simulation_parameters[index].LowerBound, simulation_parameters[index].UpperBound);
                    buffer_append_real(payload, val);
                    break;
                }
                case Boolean:
                {
                    bool val = get_random_boolean();
                    buffer_append_boolean(payload, val);
                    break;
                }
                case Timestamp:
                {
                    buffer_append_unix_timestamp(payload);
                    break;
                }
                case String:
                {
                    char str_temp[16] = {0};
                    get_random_string(str_temp);
                    buffer_append_string(payload, str_temp);
                    break;
                }
                case Unknown:
                default:
                {
                    break;
                }
            }

            buffer_append_char(payload, ',');
        }

        buffer_remove_end(payload, 1);

        char* destination = str_list_get_first(destination_list);

        while(destination)
        {           
            if(message_bus_send(message_bus, destination, Data, Text, (char*)buffer_get_data(payload), (long)buffer_get_size(payload)))
            {
                WriteInformation(logger, (char*)buffer_get_data(payload));
            }
            else
            {
                WriteLog(logger, "Send failure over IPC", LOG_ERROR);
            }


            destination = str_list_get_next(destination_list);
        }

        buffer_free(payload);
    }

    return true;
}

bool simulator_restart()
{
    return false;
}

bool simulator_stop()
{
    message_bus_close(message_bus);
    message_bus_release(message_bus);
    str_list_free(destination_list);
    logger_release(logger);
    free(simulation_parameters);
    return false;
}

bool get_random_boolean(void)
{
    return rand() % 2;
}

float get_random_real(float lower, float upper)
{
    return (float)get_random_integer((long long)lower, (long long)upper);
}

long long get_random_integer(long long lower, long long upper)
{
    int rand_num = (rand() % (upper - lower + 1)) + lower;
    return rand_num;
}

void get_random_string(char* str)
{
    return get_random_timnestamp(str);
}

void get_random_timnestamp(char* str)
{
    char buffer[15] = {0};
    time_t t ;
    struct tm *tmp ;
    time(&t);
    tmp = localtime(&t);

    sprintf(buffer, "%04d%02d%02d%02d%02d%02d",
             (tmp->tm_year+1900), (tmp->tm_mon+1), tmp->tm_mday,
             tmp->tm_hour, tmp->tm_min, tmp->tm_sec);


    strcpy(str, buffer);
    return;
}

void on_network_event(const char* node_name, PayloadType ptype, DataType dtype, const char* messagebuffer, long buffersize, long payload_id)
{  
    WriteInformation(logger,messagebuffer);
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
            continue_processing = false;
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
        case WindowResized:
        {
            WriteLog(logger, "WINDOW SIZE CHANGED SIGNAL", LOG_CRITICAL);
            break;
        }
        default:
        {
            WriteLog(logger, "UNKNOWN SIGNAL", LOG_CRITICAL);
            break;
        }
    }
}
