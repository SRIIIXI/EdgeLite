cmake_minimum_required(VERSION 3.0)

project(IPCamBroker)

set(IPCamBroker_VERSION_MAJOR 1)
set(IPCamBroker_VERSION_MINOR 0)

if(WIN32)
include_directories(D:/oss/usr/include/ D:/oss/usr/include/curl/)
link_directories(D:/oss/usr/lib)
else()
include_directories(./include/ /include/ /usr/include/ /usr/local/include/)
link_directories(./lib/ /lib /usr/lib/ /usr/local/lib/ /usr/lib/x86_64-linux-gnu/ /usr/lib/arm-linux-gnu/)
endif()

if(WIN32)
link_libraries(ws2_32 libcurl)
else()
link_libraries(rt pthread curl dl)
endif()

set(PROJECT_SOURCE_DIR ./sources)

set(SOURCE
   ${SOURCE}
   ${PROJECT_SOURCE_DIR}/Base64.cpp
   ${PROJECT_SOURCE_DIR}/Configuration.cpp
   ${PROJECT_SOURCE_DIR}/Directory.cpp
   ${PROJECT_SOURCE_DIR}/Logger.cpp
   ${PROJECT_SOURCE_DIR}/SignalHandler.cpp
   ${PROJECT_SOURCE_DIR}/StringEx.cpp
   ${PROJECT_SOURCE_DIR}/Timestamp.cpp
   ${PROJECT_SOURCE_DIR}/HttpPacket.cpp
   ${PROJECT_SOURCE_DIR}/HttpHandler.cpp
   ${PROJECT_SOURCE_DIR}/Listener.cpp
   ${PROJECT_SOURCE_DIR}/NetworkHelper.cpp
   ${PROJECT_SOURCE_DIR}/Responder.cpp
   ${PROJECT_SOURCE_DIR}/DbLocal.cpp
   ${PROJECT_SOURCE_DIR}/SQLTemplates.cpp
   ${PROJECT_SOURCE_DIR}/IPCamBroker.cpp
   ${PROJECT_SOURCE_DIR}/sqlite3.c
   ${PROJECT_SOURCE_DIR}/CamDetect.cpp
   ${PROJECT_SOURCE_DIR}/Retriever.cpp
   ${PROJECT_SOURCE_DIR}/Notifier.cpp
   ${PROJECT_SOURCE_DIR}/Downloader.cpp
   ${PROJECT_SOURCE_DIR}/PlaylistGen.cpp
   ${PROJECT_SOURCE_DIR}/main.cpp
)

set(HEADERS
   ${HEADERS}
   ${PROJECT_SOURCE_DIR}/IPCamStructs.h
   ${PROJECT_SOURCE_DIR}/Base64.h
   ${PROJECT_SOURCE_DIR}/Configuration.h
   ${PROJECT_SOURCE_DIR}/Directory.h
   ${PROJECT_SOURCE_DIR}/Logger.h
   ${PROJECT_SOURCE_DIR}/OSDefinitions.h
   ${PROJECT_SOURCE_DIR}/SignalHandler.h
   ${PROJECT_SOURCE_DIR}/StringEx.h
   ${PROJECT_SOURCE_DIR}/Timestamp.h
   ${PROJECT_SOURCE_DIR}/HttpPacket.h
   ${PROJECT_SOURCE_DIR}/HttpHandler.h
   ${PROJECT_SOURCE_DIR}/Listener.h
   ${PROJECT_SOURCE_DIR}/NetworkHelper.h
   ${PROJECT_SOURCE_DIR}/Responder.h
   ${PROJECT_SOURCE_DIR}/DbLocal.h
   ${PROJECT_SOURCE_DIR}/IPCamBroker.h
   ${PROJECT_SOURCE_DIR}/SQLTemplates.h
   ${PROJECT_SOURCE_DIR}/sqlite3.h
   ${PROJECT_SOURCE_DIR}/sqlite3ext.h
   ${PROJECT_SOURCE_DIR}/Retriever.h
   ${PROJECT_SOURCE_DIR}/Notifier.h
   ${PROJECT_SOURCE_DIR}/Downloader.cpp
   ${PROJECT_SOURCE_DIR}/PlaylistGen.h
   ${PROJECT_SOURCE_DIR}/CamDetect.h
)

add_executable(IPCamBroker ${SOURCE})

