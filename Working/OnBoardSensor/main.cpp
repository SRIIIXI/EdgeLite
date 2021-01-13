//
// Created by mukeshp on 29-Aug-20.
//
#include <stdio.h>
#include <pthread.h>
#include "mma8451q.h"
#include "mpl3150a2.h"
#include "si7020.h"
#include "OnBoardSensorCnfg.h"

extern OnBoardSensorCnfg_t gOnBoardSensorCnfg;

pthread_t gPressureSensor;
pthread_t gHumiditySensor;

int main(int argc, char **argv)
{
    OnBoardSensor_ConfigurationRead();

    if(gOnBoardSensorCnfg.mIsPressureSensorEnable == 1)
    {
        MPL3150A2_InterruptPinConfigure();
        pthread_create(&gPressureSensor, NULL, MPL3150A2_Poll_Device, NULL);
    }

    if(gOnBoardSensorCnfg.mIsHumiditySensorEnable == 1)
    {
        pthread_create(&gHumiditySensor, NULL, SI7020_Poll_Device, NULL);
    }

    if(gOnBoardSensorCnfg.mIsAccelerometerEnable == 1)
    {
        MMA8451Q_InterruptPinConfigure();

        if(gOnBoardSensorCnfg.mIsOrientationEnable == 1)
        {
            MMA8451Q_Init_Orientation();
        }

        if(gOnBoardSensorCnfg.mIsFreeFallEnable == 1)
        {
            MMA8451Q_Init_FreeFallDetection();
        }

        MMA8451Q_Poll_Device();
    }

    while(1)
    {
        sleep(10);
    }

    return 0;
}
