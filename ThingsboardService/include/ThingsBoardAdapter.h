#ifndef THINGSBOARD_ADAPTER
#define THINGSBOARD_ADAPTER

#include <RTUCore/Logger.h>

typedef struct thingsboard_adapter_t  thingsboard_adapter_t;
typedef void(*downlink_callback)(const char* messageptr, long len);

//static downlink_callback downlink = NULL;
//static logger_t* logger = NULL;

//bool mqtt_initialize(logger_t* loggerptr);
//bool mqtt_connect(const char* str_server, int num_port, const char* deviceid, downlink_callback dlink);
//bool mqtt_send_data(const char* str, const char* uri);
//bool mqtt_send_response(const char* str, const char* uri);
//bool mqtt_send_request(const char* str, const char* uri);
//bool mqtt_send_event(const char* str, const char* uri);
//bool mqtt_close();
//bool mqtt_release();

bool thingsboard_initialize(logger_t* loggerptr);
thingsboard_adapter_t* thingsboard_connect(logger_t* loggerptr, const char* clientid, const char* str_server, int num_port, const char* userid, const char* passwd, downlink_callback dlink);
bool thingsboard_send_data(thingsboard_adapter_t* thingsboard_instance, const char* str, const char* uri);
bool thingsboard_send_response(thingsboard_adapter_t* thingsboard_instance, const char* str, const char* uri);
bool thingsboard_send_request(thingsboard_adapter_t* thingsboard_instance, const char* str, const char* uri);
bool thingsboard_send_event(thingsboard_adapter_t* thingsboard_instance, const char* str, const char* uri);
bool thingsboard_close(thingsboard_adapter_t* thingsboard_instance);
bool thingsboard_release();

#endif

