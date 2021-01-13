//
// Created by mukeshp on 31-Jul-20.
//

#ifndef ON_BOARD_SENSOR_INTERFACE_H
#define ON_BOARD_SENSOR_INTERFACE_H

#include "iniparser.h"

#define ON_BOARD_SENSOR_CNFG_FILE_PATH      "."
#define ON_BOARD_SENSOR_CNFG_FILE           "OnBoardSensor.ini"

#define GET_ENABLE_DISABLE_STR(_arg_)   (( (_arg_) == 1) ? "ENABLE" : "DISABLE" )
#define GET_INTERRUPT_STR(_arg_)        (( (_arg_) == 1) ? "INT1" : "INT2" )

typedef struct OnBoardSensorCnfg
{
    /* Accelerometer sensor configurations */
    char mIsAccelerometerEnable;
    char mIsOrientationEnable;
    char mIsFreeFallEnable;
    char mInterruptPinSel;

    /* Pressure sensor configurations */
    char mIsPressureSensorEnable;
    char mIsPressureSensorTemperatureEnable;
    char mPressureChangeDelta;
    char mTemperatureChangeDelta;

    /* Humidity sensor configurations */
    char mIsHumiditySensorEnable;
    char mIsHumiditySensorTemperatureEnable;

    OnBoardSensorCnfg()
    {
        this->mIsAccelerometerEnable = 1;
        this->mIsOrientationEnable = 1;
        this->mIsFreeFallEnable = 1;
        this->mInterruptPinSel = 1;

        this->mIsPressureSensorEnable = 1;
        this->mIsPressureSensorTemperatureEnable = 0;

        this->mIsHumiditySensorEnable = 1;
        this->mIsHumiditySensorTemperatureEnable = 1;
    }
}OnBoardSensorCnfg_t;

int OnBoardSensor_ConfigurationRead(void);
int OnBoardSensor_ConfigurationWrite(const char *pTag, const char *pValue);

#endif //ON_BOARD_SENSOR_INTERFACE_H
