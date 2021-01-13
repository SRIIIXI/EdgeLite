//
// Created by mukeshp on 01-Sep-20.
//

#ifndef ONBOARDSENSOR_SI7020_H
#define ONBOARDSENSOR_SI7020_H

#define DEVICE_SI7020        (0x80)//(0x40)

enum
{
    HUMIDITY_HOLD_MASTER_MODE       =   0xE5,
    HUMIDITY_NO_HOLD_MASTER_MODE    =   0xF5,
    TEMPERATURE_HOLD_MASTER_MODE    =   0xE3,
    TEMPERATURE_NO_HOLD_MASTER_MODE =   0xF3,
    TEMPERATURE_FROM_PREVIOUS_RH    =   0xE0,
    IC_RESET                        =   0xFE,
};

void* SI7020_Poll_Device(void *arg);
int SI7020_Read_Relative_Humidity(float *pRHumidity);
int SI7020_Read_Temperature(float *pTemperature);

#endif //ONBOARDSENSOR_SI7020_H
