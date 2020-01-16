cmake_minimum_required(VERSION 2.8.12)

project(BoardModel)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(./include/)

set(PROJECT_SOURCE_DIR ./src)
set(PROJECT_HEADER_DIR ./include)

set(SOURCE
${SOURCE}
${PROJECT_SOURCE_DIR}/main.c
)

set(HEADERS
${HEADERS}

${PROJECT_HEADER_DIR}/Types.h
${PROJECT_HEADER_DIR}/Alert.h
${PROJECT_HEADER_DIR}/Command.h
${PROJECT_HEADER_DIR}/AppKeyValue.h
${PROJECT_HEADER_DIR}/Query.h
${PROJECT_HEADER_DIR}/Requests.h
${PROJECT_HEADER_DIR}/Responses.h
${PROJECT_HEADER_DIR}/Telemetry.h
${PROJECT_HEADER_DIR}/Header.h
${PROJECT_HEADER_DIR}/Body.h
)

add_executable(${PROJECT_NAME}  ${SOURCE} ${HEADERS})

