//
// Created by mukeshp on 31-Aug-20.
//

#ifndef ONBOARDSENSOR_MPL3150A2_H
#define ONBOARDSENSOR_MPL3150A2_H

#define DEVICE_MPL3150A2        (0xC0)

#define MPL3150A2_INT_GPIO           158
#define MPL3150A2_INT_GPIO_STRING   "gpio158" //CN2_52

#define MPL3150A2_GET_FRACTION_PRESSURE(_arg_)  (((_arg_) == 1) ? 0.50 :\
                                                ((_arg_) == 2) ? 0.25 :\
                                                ((_arg_) == 3) ? 0.75 : 0.00)
enum
{
    SYSMOD_STANDBY  =   0,
    SYSMOD_ACTIVE   =   1
};

enum
{
    OUT_P_MSB   =   0x01,
    OUT_P_CSB   =   0x02,
    OUT_P_LSB   =   0x03,
    OUT_T_MSB   =   0x04,
    OUT_T_LSB   =   0x05,
    WHO_AM_I_MPL3150A2    =   0x0C,
    SYSMOD_MPL3150A2      =   0x11,
    INT_SOURCE_MPL3150A2  =   0x12,
    P_WND_LSB   =   0x19,
    P_WND_MSB   =   0x1A,
    T_WIN       =   0x1B,
    P_MIN_MSB   =   0x1C,
    P_MIN_CSB   =   0x1D,
    P_MIN_LSB   =   0x1E,
    T_MIN_MSB   =   0x1F,
    T_MIN_LSB   =   0x20,
    P_MAX_MSB   =   0x21,
    P_MAX_CSB   =   0x22,
    P_MAX_LSB   =   0x23,
    T_MAX_MSB   =   0x24,
    T_MAX_LSB   =   0x25,
    CTRL_REG1_MPL3150A2   =   0x26,
    CTRL_REG4_MPL3150A2   =   0x29,
    CTRL_REG5_MPL3150A2   =   0x2A
};

void* MPL3150A2_Poll_Device(void *arg);
void MPL3150A2_InterruptPinConfigure(void);
int MPL3150A2_Interrupt_Configuration(void);
int MPL3150A2_Set_System_Mode(unsigned char iSysMode);
int MPL3150A2_Read_Pressure(float *pressure);
int MPL3150A2_Read_Temperature(float *pTemperature);
int MPL3150A2_Read_DeviceId(unsigned char *pDeviceId);
int MPL3150A2_Configure_Window_Register(void);
int MPL3150A2_Read_Max_Pressure(float *pMaxPressure);
int MPL3150A2_Read_Max_Temperature(float *pMaxTemperature);
int MPL3150A2_Read_Min_Pressure(float *pMinPressure);
int MPL3150A2_Read_Min_Temperature(float *pMinTemperature);
float MPL3150A2_Calculate_Fraction_Part(char iData);

#endif //ONBOARDSENSOR_MPL3150A2_H
