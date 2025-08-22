#include "ThingsBoardAdapter.h"
#include <stdio.h>
#include <mosquitto.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#pragma pack(1)
struct thingsboard_adapter_t
{
    char server[33];
    int port;
    char uri[33];
    bool run;
    char client_id[33];
    char user_id[33];
    char password[33];
    struct mosquitto *mqtt_context;
    pthread_t receiver_function;
    downlink_callback mqtt_callback;
    logger_t* logger;
};

static int keep_alive = 60;

static void on_connect(struct mosquitto *mosq, void *obj, int result);
static void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message);
static void* receiver_thread(void* ptr);
static bool mqtt_internal_send(struct mosquitto *mqtt_context, const char* topic, int data_len, const char* data);

static bool is_library_initialized = false;

bool thingsboard_initialize(logger_t* loggerptr)
{
    if(!is_library_initialized)
    {
        int rc = 0;

        rc = mosquitto_lib_init();
        if(rc != MOSQ_ERR_SUCCESS)
        {
            WriteLog(loggerptr, "Could not initialize MQTT stack", LOG_ERROR);
            return false;
        }
        else
        {
            is_library_initialized = true;
        }
    }

    return true;
}

thingsboard_adapter_t* thingsboard_connect(logger_t* loggerptr, const char* clientid, const char* str_server, int num_port, const char* userid, const char* passwd, downlink_callback dlink)
{
    thingsboard_adapter_t* thingsboard_instance = (thingsboard_adapter_t*)calloc(1, sizeof (thingsboard_adapter_t));

    if(thingsboard_instance == NULL)
    {
        return NULL;
    }

    thingsboard_instance->logger = loggerptr;

    int rc = 0;
    thingsboard_instance->run = true;
    thingsboard_instance->port = num_port;
    strcpy(thingsboard_instance->server, str_server);
    strcpy(thingsboard_instance->user_id, userid);
    strcpy(thingsboard_instance->password, passwd);
    thingsboard_instance->mqtt_callback = dlink;
    bool clean_session = true;
    strncpy(thingsboard_instance->client_id, clientid, 32);
    thingsboard_instance->client_id[32] =0;

    thingsboard_instance->mqtt_context = mosquitto_new(thingsboard_instance->client_id, clean_session, thingsboard_instance);

    if(!thingsboard_instance->mqtt_context)
    {
        WriteLog(thingsboard_instance->logger, "Could not create a MQTT client", LOG_ERROR);
        mosquitto_destroy(thingsboard_instance->mqtt_context);
        return NULL;
    }

    mosquitto_message_callback_set(thingsboard_instance->mqtt_context, on_message);
    mosquitto_connect_callback_set(thingsboard_instance->mqtt_context, on_connect);

    rc = mosquitto_username_pw_set(thingsboard_instance->mqtt_context, thingsboard_instance->user_id, thingsboard_instance->password);

    if(rc != MOSQ_ERR_SUCCESS)
    {
        WriteLog(thingsboard_instance->logger, "Could not set username and password", LOG_ERROR);
        mosquitto_destroy(thingsboard_instance->mqtt_context);
        return NULL;
    }

    rc = mosquitto_connect(thingsboard_instance->mqtt_context, thingsboard_instance->server, thingsboard_instance->port, keep_alive);

    if(rc != MOSQ_ERR_SUCCESS)
    {
        WriteLog(thingsboard_instance->logger, "Could not connect to MQTT broker", LOG_ERROR);
        mosquitto_destroy(thingsboard_instance->mqtt_context);
        return NULL;
    }

    WriteInformation(thingsboard_instance->logger, "Connected to MQTT broker");

    pthread_attr_t pthread_attr;
    memset(&pthread_attr, 0, sizeof(pthread_attr_t));
    // default threading attributes
    pthread_attr_init(&pthread_attr);
    // allow a thread to exit cleanly without a join
    pthread_attr_setdetachstate (&pthread_attr,PTHREAD_CREATE_DETACHED);
    if (pthread_create(&thingsboard_instance->receiver_function, &pthread_attr, receiver_thread, thingsboard_instance) != 0)
    {
        mosquitto_destroy(thingsboard_instance->mqtt_context);
        return NULL;
    }

    pthread_attr_destroy(&pthread_attr);

    sleep(10);

    return thingsboard_instance;
}

bool thingsboard_send_data(thingsboard_adapter_t* thingsboard_instance, const char* str, const char* uri)
{
    if(thingsboard_instance == NULL)
    {
        return false;
    }

    char publish_topic[65] = {0};
    sprintf(publish_topic, "%s/telemetry", uri);
    int data_len = (int)strlen(str);

    return mqtt_internal_send(thingsboard_instance->mqtt_context, publish_topic, data_len, str);
}

bool thingsboard_send_event(thingsboard_adapter_t* thingsboard_instance, const char* str, const char* uri)
{
    if(thingsboard_instance == NULL)
    {
        return false;
    }

    char publish_topic[65] = {0};
    sprintf(publish_topic, "%s/attributes", uri);
    int data_len = (int)strlen(str);

    return mqtt_internal_send(thingsboard_instance->mqtt_context, publish_topic, data_len, str);
}

bool thingsboard_send_response(thingsboard_adapter_t* thingsboard_instance, const char* str, const char* uri)
{
    if(thingsboard_instance == NULL)
    {
        return false;
    }

    return true;
}

bool thingsboard_send_request(thingsboard_adapter_t* thingsboard_instance, const char* str, const char* uri)
{
    if(thingsboard_instance == NULL)
    {
        return false;
    }

    return true;
}

bool thingsboard_close(thingsboard_adapter_t* thingsboard_instance)
{
    if(thingsboard_instance == NULL)
    {
        return false;
    }

    thingsboard_instance->run = false;
    sleep(4);

    int rc = mosquitto_disconnect(thingsboard_instance->mqtt_context);
    if(rc != MOSQ_ERR_SUCCESS)
    {
        WriteLog(thingsboard_instance->logger, "Could not disconnect from MQTT broker", LOG_ERROR);
        return false;
    }

    mosquitto_destroy(thingsboard_instance->mqtt_context);

    return true;
}

bool thingsboard_release()
{
    if(is_library_initialized)
    {
        mosquitto_lib_cleanup();
        is_library_initialized = false;
        return true;
    }

    return false;
}

bool mqtt_internal_send(struct mosquitto *mqtt_context, const char* topic, int data_len, const char* data)
{
    if(mosquitto_publish(mqtt_context, NULL, topic, data_len, data, 0, 0) != MOSQ_ERR_SUCCESS)
    {
        return false;
    }

    return true;
}

static void* receiver_thread(void *ptr)
{
    pthread_detach(pthread_self());

    thingsboard_adapter_t* tb_ptr = (thingsboard_adapter_t*)ptr;

    if(tb_ptr == NULL)
    {
        return NULL;
    }

    int rc = 0;

    while(tb_ptr->run)
    {
        rc = mosquitto_loop(tb_ptr->mqtt_context, -1, 1);

        if(tb_ptr->run && rc)
        {
            WriteLog(tb_ptr->logger, "MQTT connection error ... reconnecting in 10 seconds", LOG_ERROR);
            sleep(2);

            rc = mosquitto_reconnect(tb_ptr->mqtt_context);

            if(rc != MOSQ_ERR_SUCCESS)
            {
                WriteLog(tb_ptr->logger, "MQTT ould not reconnect", LOG_ERROR);
            }
        }
    }

    if(ptr)
    {
        return ptr;
    }
    else
    {
        return NULL;
    }
}

void on_connect(struct mosquitto *mosq, void *obj, int result)
{
    thingsboard_adapter_t* tb_ptr = (thingsboard_adapter_t*)obj;

    if(tb_ptr == NULL)
    {
        return;
    }

    WriteLog(tb_ptr->logger, "MQTT connect callback", LOG_INFO);
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
    thingsboard_adapter_t* tb_ptr = (thingsboard_adapter_t*)obj;

    if(tb_ptr == NULL)
    {
        return;
    }

    if((
       strstr(message->topic, "/request/") ||
       strstr(message->topic, "/response/") ||
       strstr(message->topic, "/data/")
     ))
    {
        if(tb_ptr->mqtt_callback)
        {
            tb_ptr->mqtt_callback((char*)message->payload, message->payloadlen);
        }
    }
    else
    {
        WriteLog(tb_ptr->logger, "Unsubscribed message received, ignored", LOG_WARNING);
    }
}



