#ifndef TRANSPORT_SERVICE
#define TRANSPORT_SERVICE

#include <RTUCore/MessageBus.h>

bool service_initialize();
bool service_start();
bool service_restart();
bool service_stop();

#endif
