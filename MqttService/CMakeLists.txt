cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(APP_VERSION_MAJOR 1)
set(APP_VERSION_MINOR 0)
set(APP_VERSION_PATCH 0)

set(Project ThingsBoardService)
project(${Project})

include_directories( ./include/ ../RTUCore/include /include/ /usr/include/ /usr/local/include/)
link_directories(/lib /usr/lib /usr/local/lib)
link_libraries(rt pthread dl rtucore mosquitto)

set(SOURCE
${SOURCE}
./src/ThingsBoardAdapter.c
./src/ThingsBoardService.c
./src/main.c
)

set(HEADERS
${HEADERS}
./include/ThingsBoardAdapter.h
./include/ThingsBoardService.h
)

set(CONFIG
${CONFIG}
./etc/service_transport.conf
)

add_executable(service_thingsboard  ${SOURCE} ${HEADERS})

install(TARGETS service_thingsboard DESTINATION /usr/bin)
install(FILES ${CONFIG} DESTINATION /etc)
set(CPACK_PACKAGE_FILE_NAME ${Project}.${CMAKE_SYSTEM_PROCESSOR}.${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}.${APP_VERSION_PATCH})
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Subrato Roy")
set(CPACK_PACKAGE_DESCRIPTION, "ThingsBoard Transport Service")

include(CPack)
