cmake_minimum_required(VERSION 2.8.12)

project(DataModel)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(./include/)

set(PROJECT_SOURCE_DIR ./src)
set(PROJECT_HEADER_DIR ./include)

set(SOURCE
${SOURCE}
${PROJECT_SOURCE_DIR}/DataModel.c
)

set(HEADERS
${HEADERS}

${PROJECT_HEADER_DIR}/Types.h
${PROJECT_HEADER_DIR}/Command.h
${PROJECT_HEADER_DIR}/AppKeyValue.h
${PROJECT_HEADER_DIR}/Query.h
${PROJECT_HEADER_DIR}/Request.h
${PROJECT_HEADER_DIR}/Response.h
${PROJECT_HEADER_DIR}/Telemetry.h
${PROJECT_HEADER_DIR}/Header.h
${PROJECT_HEADER_DIR}/Body.h
${PROJECT_HEADER_DIR}/DataModel.h
)

add_library(${PROJECT_NAME} SHARED ${SOURCE} ${HEADERS})

