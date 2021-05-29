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

#ifndef LIB_MESSAGE_BUS_C
#define LIB_MESSAGE_BUS_C

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LIBRARY_EXPORT __attribute__((visibility("default")))
#define LIBRARY_ENTRY __attribute__((constructor))
#define LIBRARY_EXIT __attribute__((destructor))

// Shared libary load/unload handlers
extern  LIBRARY_ENTRY void library_load();
extern  LIBRARY_EXIT void library_unload();

#pragma push(1)
typedef struct payload
{
    char payload_type;
    char payload_data_type;
    char sender[32];
    char receipient[32];
    long payload_id;
    long data_size;
    char* data;
}payload;
#pragma pack(0)

// Structures and types
typedef enum PayloadType
{
    Data='D',
    Event='E',
    Request='Q',
    Response='R'
}PayloadType;

typedef enum DataType
{
    Text='T',
    Video='V',
    Image='I',
    Audio='A',
    Raw='R'
}DataType;

typedef void(*messabus_bus_callback)(const char* node_name, PayloadType ptype, DataType dtype, const char* messagebuffer, long buffersize, long payload_id);

extern LIBRARY_EXPORT bool message_bus_initialize(void** pptr, messabus_bus_callback callback);
extern LIBRARY_EXPORT bool message_bus_open(void* ptr);
extern LIBRARY_EXPORT bool message_bus_close(void* ptr);
extern LIBRARY_EXPORT bool message_bus_send(void* ptr, const char* node_name, PayloadType ptype, DataType dtype, const char* messagebuffer, long buffersize, long *payload_id);
extern LIBRARY_EXPORT bool message_bus_send_loopback(void* ptr);
extern LIBRARY_EXPORT long message_bus_has_node(void* ptr, const char* node_name);
extern LIBRARY_EXPORT char* message_bus_node_fullname(void* ptr, long node_index);
extern LIBRARY_EXPORT char* message_bus_localname(void* ptr);

#ifdef __cplusplus
}
#endif

#endif
