cmake_minimum_required(VERSION 3.5)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(APP_VERSION_MAJOR 1)
set(APP_VERSION_MINOR 0)
set(APP_VERSION_PATCH 0)

set(Project Simulator)
project(${Project})

include_directories(./include/ /include/ /usr/include/ /usr/local/include/)
link_directories(/lib /usr/lib /usr/local/lib)
link_libraries(rt pthread dl rtucore)

set(SOURCE
${SOURCE}
./src/Simulator.c
./src/main.c
)

set(HEADERS
${HEADERS}
./include/Simulator.h
)

add_executable(service_simulator ${SOURCE} ${HEADERS})

install(TARGETS service_simulator DESTINATION local/bin)
set(CPACK_PACKAGE_FILE_NAME ${Project}.${CMAKE_SYSTEM_PROCESSOR}.${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}.${APP_VERSION_PATCH})

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Subrato Roy")
set(CPACK_PACKAGE_DESCRIPTION, "Peripheral Simulator")

include(CPack)

