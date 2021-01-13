//
// Created by mukeshp on 01-Sep-20.
//
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "I2c_Interface.h"
#include "si7020.h"
#include "OnBoardSensorCnfg.h"

extern OnBoardSensorCnfg_t gOnBoardSensorCnfg;

void* SI7020_Poll_Device(void *arg)
{
    float tRHumidity;
    float temperature;
    int tRetVal = 0;

    pthread_detach(pthread_self());

    while(1)
    {
        tRetVal = SI7020_Read_Relative_Humidity(&tRHumidity);
        if(tRetVal != 0 )
        {
            printf("Error in SI7020_Read_Relative_Humidity at %s\n", __func__ );
            sleep(5);
            continue;
        }

        if(gOnBoardSensorCnfg.mIsHumiditySensorTemperatureEnable)
        {
            tRetVal = SI7020_Read_Temperature(&temperature);
            if (tRetVal != 0)
            {
                printf("Error in SI7020_Read_Temperature at %s\n", __func__);
            }
        }

        sleep(5);
    }

    pthread_exit(NULL);
}

int SI7020_Read_Relative_Humidity(float *pRHumidity)
{
    int tRetVal = 0;
    unsigned char tData[2] = {0,};
    float tHumidity = 0;

    tRetVal = I2C_ReadRegister(DEVICE_SI7020, HUMIDITY_NO_HOLD_MASTER_MODE, (char*)&tData[0] , 2, 0);
    if(0 != tRetVal)
    {
        printf("Error to read HUMIDITY_NO_HOLD_MASTER_MODE register at %s\n", __func__ );
        return -1;
    }

    //printf("Humidity --> tData[0]:%d, tData[1]:%d\n", tData[0], tData[1]);
    tHumidity = (((tData[0] * 256 + tData[1]) * 125.0) / 65536.0) - 6;

    printf("SI7020: Relative Humidity : %.2f RH \n", tHumidity);

    (*pRHumidity) = tHumidity;

    return 0;
}

int SI7020_Read_Temperature(float *pTemperature)
{
    int tRetVal = 0;
    unsigned char tData[2] = {0,};
    float temperature = 0;

    tRetVal = I2C_ReadRegister(DEVICE_SI7020, TEMPERATURE_NO_HOLD_MASTER_MODE, (char*)&tData[0] , 2, 0);
    if(0 != tRetVal)
    {
        printf("Error to read TEMPERATURE_NO_HOLD_MASTER_MODE register at %s\n", __func__ );
        return -1;
    }

    //printf("Temperature --> tData[0]:%d, tData[1]:%d\n", tData[0], tData[1]);
    temperature = (((tData[0] * 256 + tData[1]) * 175.72) / 65536.0) - 46.85;
    //float fTemp = cTemp * 1.8 + 32;

    printf("SI7020: Temperature in Celsius : %.2f C \n", temperature);
    //printf("Temperature in Fahrenheit : %.2f F \n", fTemp);

    (*pTemperature) = temperature;

    return 0;
}

