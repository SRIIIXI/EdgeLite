cmake_minimum_required(VERSION 3.13)
project(OnBoardSensor)

set(CMAKE_CXX_STANDARD 14)

set(ONBOARD_SENSOR_BASE_DIR ${PROJECT_SOURCE_DIR})

include_directories(${ONBOARD_SENSOR_BASE_DIR})
include_directories(${ONBOARD_SENSOR_BASE_DIR}/i2c)
include_directories(${ONBOARD_SENSOR_BASE_DIR}/accelerometer)
include_directories(${ONBOARD_SENSOR_BASE_DIR}/pressure)
include_directories(${ONBOARD_SENSOR_BASE_DIR}/humidity)
include_directories(${ONBOARD_SENSOR_BASE_DIR}/Cnfg)
include_directories(${ONBOARD_SENSOR_BASE_DIR}/../ThirdParty/iniparser/include)

# link modbus library path
link_directories(${ONBOARD_SENSOR_BASE_DIR}/../ThirdParty/iniparser/lib)

# Static libraries
set(ONBOARD_SENSOR_INTERFACE_LINK_LIBS
        libiniparser.a
        )

# PROJECT sources
file(GLOB ONBOARD_SENSOR_SERVICE_SOURCES
        "${ONBOARD_SENSOR_BASE_DIR}/*.cpp"
        "${ONBOARD_SENSOR_BASE_DIR}/i2c/*.cpp"
        "${ONBOARD_SENSOR_BASE_DIR}/accelerometer/*.cpp"
        "${ONBOARD_SENSOR_BASE_DIR}/pressure/*.cpp"
        "${ONBOARD_SENSOR_BASE_DIR}/humidity/*.cpp"
        "${ONBOARD_SENSOR_BASE_DIR}/Cnfg/*.cpp"
        )

add_executable(OnBoardSensor ${ONBOARD_SENSOR_SERVICE_SOURCES})

target_link_libraries(OnBoardSensor PUBLIC -pthread ${ONBOARD_SENSOR_INTERFACE_LINK_LIBS})
