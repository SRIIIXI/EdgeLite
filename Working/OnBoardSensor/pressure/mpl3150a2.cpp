//
// Created by mukeshp on 31-Aug-20.
//
#include <pthread.h>
#include "I2c_Interface.h"
#include "mpl3150a2.h"
#include "OnBoardSensorCnfg.h"

extern OnBoardSensorCnfg_t gOnBoardSensorCnfg;

void* MPL3150A2_Poll_Device(void *arg)
{
    char gpio_irq[64] = {0, };
    int ret, irqfd = 0, i = 0;
    unsigned char tInt1Source = 0;
    int buf;
    int tRetVal = 0;
    fd_set fds;
    unsigned char tDeviceId = 0;
    char temp = 0x04;

    pthread_detach(pthread_self());

    I2C_WriteRegister(DEVICE_MPL3150A2, CTRL_REG1_MPL3150A2, &temp, 1);
    sleep(1);

    MPL3150A2_Read_DeviceId(&tDeviceId);

    MPL3150A2_Set_System_Mode(SYSMOD_ACTIVE);

    MPL3150A2_Configure_Window_Register();
    MPL3150A2_Interrupt_Configuration();

    FD_ZERO(&fds);

    snprintf(gpio_irq, sizeof(gpio_irq), "/sys/class/gpio/%s/value", MPL3150A2_INT_GPIO_STRING);
    irqfd = open(gpio_irq, O_RDONLY, S_IREAD);
    if(irqfd == -1)
    {
        printf("Could not open IRQ %s at %s\n", MPL3150A2_INT_GPIO_STRING, __func__ );
        printf("Make sure the GPIO is already exported\n");
    }

    while(1)
    {
        FD_SET(irqfd, &fds);

        /* See if the IRQ has any data available to read */
        ret = select(irqfd + 1, NULL, NULL, &fds, NULL);

        if(FD_ISSET(irqfd, &fds))
        {
            /* Remove the file description from set */
            FD_CLR(irqfd, &fds);

            //printf("Pressure IRQ detected %d\n", i);
            fflush(stdout);
            i++;

            /* The return value includes the actual GPIO register value */
            read(irqfd, &buf, sizeof(buf));
            lseek(irqfd, 0, SEEK_SET);

            tRetVal = I2C_ReadRegister(DEVICE_MPL3150A2, INT_SOURCE_MPL3150A2, (char*)&tInt1Source , 1);
            if(0 != tRetVal)
            {
                printf("Error to read INT_SOURCE register at %s\n", __func__ );
            }

            //printf("tInt1Source:0x%02X\n", tInt1Source);

            //Processing here.
            /* Pressure and/or Temperature data is ready for read */
            if(tInt1Source & 0x80)
            {
                float tPressure, temperature;

                MPL3150A2_Read_Pressure(&tPressure);
                MPL3150A2_Read_Temperature(&temperature);
            }

            if(tInt1Source & 0x01)
            {
                float temperature;

                MPL3150A2_Read_Temperature(&temperature);
            }

            if(tInt1Source & 0x02)
            {
                float tPressure;

                MPL3150A2_Read_Pressure(&tPressure);
            }
        }

        /*Sleep, or  do any other processing here */
        usleep(100000);
    }

    close(irqfd);

    pthread_exit(NULL);
}

void MPL3150A2_InterruptPinConfigure(void)
{
    int tFdInt1 = -1;
    char tGpioIrq[256] = {0,};
    char tCommand[256] = {0,};

    sprintf(tGpioIrq, "/sys/class/gpio/%s/direction", MPL3150A2_INT_GPIO_STRING);
    tFdInt1 = open(tGpioIrq, O_RDWR);
    if (tFdInt1 < 0)
    {
        /* if not exported already than export it */
        sprintf(tCommand, "echo \"%d\" > /sys/class/gpio/export", MPL3150A2_INT_GPIO);
        system(tCommand);
    }
    else
    {
        close(tFdInt1);
    }

    sprintf(tCommand, "echo \"in\" > /sys/class/gpio/%s/direction", MPL3150A2_INT_GPIO_STRING);
    system(tCommand);

    sprintf(tCommand,"echo \"falling\" > /sys/class/gpio/%s/edge", MPL3150A2_INT_GPIO_STRING);
    system(tCommand);

    return;
}

int MPL3150A2_Interrupt_Configuration(void)
{
    int tRetVal;
    unsigned char tData = 0;

    tData = 0x02;
    if(gOnBoardSensorCnfg.mIsPressureSensorTemperatureEnable)
    {
        tData = 0x03;
    }

    /* interrupt enable register */
    /* enable Data Ready, pressure change and temperature change interrupt */
    tRetVal = I2C_WriteRegister(DEVICE_MPL3150A2, CTRL_REG4_MPL3150A2, (char*)&tData , 1);
    if(0 != tRetVal)
    {
        printf("Error to read CTRL_REG4 register at %s\n", __func__ );
        return -1;
    }

    /* interrupt configuration register */
    /* pressure change and temperature change interrupt on INT1 pin */
    tRetVal = I2C_WriteRegister(DEVICE_MPL3150A2, CTRL_REG5_MPL3150A2, (char*)&tData , 1);
    if(0 != tRetVal)
    {
        printf("Error to read CTRL_REG5 register at %s\n", __func__ );
        return -1;
    }

    return 0;
}

int MPL3150A2_Set_System_Mode(unsigned char iSysMode)
{
    unsigned char tSysMode = 0;

    if(iSysMode > SYSMOD_ACTIVE)
    {
        printf("Invalid system mode(%d) at %s\n", iSysMode, __func__ );
        return -1;
    }

    //I2C_ReadRegister(DEVICE_MPL3150A2, CTRL_REG1_MPL3150A2, (char*)&tSysMode , 1);
    //printf("tSysmode:0x%02X\n", tSysMode);

    tSysMode |= SYSMOD_ACTIVE;
    I2C_WriteRegister(DEVICE_MPL3150A2, CTRL_REG1_MPL3150A2, (char*)&tSysMode, 1);

    //I2C_ReadRegister(DEVICE_MPL3150A2, CTRL_REG1_MPL3150A2, (char*)&tSysMode , 1);
    //printf("tSysmode:0x%02X\n", tSysMode);

    return 0;
}

int MPL3150A2_Configure_Window_Register(void)
{
    int tRetVal;
    unsigned char tData = 0 ;
    unsigned char temperatureChangeDelta = gOnBoardSensorCnfg.mTemperatureChangeDelta;
    unsigned char tPressureChangeDelta = gOnBoardSensorCnfg.mPressureChangeDelta;

    /* Set Pressure window register */
    tData = tPressureChangeDelta;//0x00;//0x01;
    tRetVal = I2C_WriteRegister(DEVICE_MPL3150A2, P_WND_LSB, (char*)&tData , 1);
    if(0 != tRetVal)
    {
        printf("Error to read P_WND_LSB register at %s\n", __func__ );
        return -1;
    }

    tData = 0x00;
    tRetVal = I2C_WriteRegister(DEVICE_MPL3150A2, P_WND_MSB, (char*)&tData , 1);
    if(0 != tRetVal)
    {
        printf("Error to read P_WND_MSB register at %s\n", __func__ );
        return -1;
    }

    if(gOnBoardSensorCnfg.mIsPressureSensorTemperatureEnable == 1)
    {
        /* Set temperature window register */
        tData = temperatureChangeDelta;//0x00;//0x01;
        tRetVal = I2C_WriteRegister(DEVICE_MPL3150A2, T_WIN, (char *) &tData, 1);
        if (0 != tRetVal) {
            printf("Error to read T_WIN register at %s\n", __func__);
            return -1;
        }
    }
    return 0;
}

int MPL3150A2_Read_DeviceId(unsigned char *pDeviceId)
{
    unsigned char tDeviceId = 0;
    int tRetVal = 0;

    tRetVal = I2C_ReadRegister(DEVICE_MPL3150A2, WHO_AM_I_MPL3150A2, (char*)&tDeviceId , 1);
    if(0 != tRetVal)
    {
        printf("Error to read WHO_AM_I_MPL3150A2 register at %s\n", __func__ );
        return -1;
    }

    (*pDeviceId) = tDeviceId;

    printf("Device ID:%02X\n", (*pDeviceId));
    return 0;
}

int MPL3150A2_Read_Pressure(float *pressure)
{
    float tPressure = 0;
    unsigned char tData[3] = {0, };
    unsigned long int tIntegerPart = 0;
    unsigned char tFractionPart = 0;
    int tRetVal = 0;

    tRetVal = I2C_ReadRegister(DEVICE_MPL3150A2, OUT_P_MSB, (char*)&tData[0] , 1);
    if(0 != tRetVal)
    {
        printf("Error to read OUT_P_MSB register at %s\n", __func__ );
        return -1;
    }

    tRetVal = I2C_ReadRegister(DEVICE_MPL3150A2, OUT_P_CSB, (char*)&tData[1] , 1);
    if(0 != tRetVal)
    {
        printf("Error to read OUT_P_CSB register at %s\n", __func__ );
        return -1;
    }

    tRetVal = I2C_ReadRegister(DEVICE_MPL3150A2, OUT_P_LSB, (char*)&tData[2] , 1);
    if(0 != tRetVal)
    {
        printf("Error to read OUT_P_LSB register at %s\n", __func__ );
        return -1;
    }

    //printf("Pressure -> tData[0]:0x%02X, tData[1]:0x%02X, tData[2]:0x%02X\n", tData[0], tData[1], tData[2]);

    tIntegerPart = tData[0];
    //printf("tIntegerPart:0x%02X\n", tIntegerPart);

    tIntegerPart = (tIntegerPart << 8) | tData[1] ;
    //printf("tIntegerPart:0x%02X\n", tIntegerPart);

    tIntegerPart = (tIntegerPart << 8) | tData[2] ;
    //printf("tIntegerPart:0x%02X\n", tIntegerPart);

    tIntegerPart = (tIntegerPart >> 6);
    //printf("tIntegerPart:0x%02X\n", tIntegerPart);

    tFractionPart = (tData[2] & 0x30) >> 4;
    //printf("tFractionPart:0x%02X\n", tFractionPart);

    tPressure = (float)tIntegerPart + (float)MPL3150A2_Calculate_Fraction_Part(tFractionPart);

    (*pressure) = tPressure;

    printf("MPL3150A2: Pressure:%.2f Pa\n", (*pressure));

    return 0;
}

int MPL3150A2_Read_Temperature(float *pTemperature)
{
    float temperature = 0;
    unsigned char tData[2] = {0, };
    unsigned char tIntegerPart = 0;
    unsigned char tFractionPart = 0;
    int tRetVal = 0;

    tRetVal = I2C_ReadRegister(DEVICE_MPL3150A2, OUT_T_MSB, (char*)&tData[0] , 1);
    if(0 != tRetVal)
    {
        printf("Error to read OUT_T_MSB register at %s\n", __func__ );
        return -1;
    }

    tRetVal = I2C_ReadRegister(DEVICE_MPL3150A2, OUT_T_LSB, (char*)&tData[1] , 1);
    if(0 != tRetVal)
    {
        printf("Error to read OUT_T_LSB register at %s\n", __func__ );
        return -1;
    }

    //printf("Temperature -> tData[0]:0x%02X, tData[1]:0x%02X\n", tData[0], tData[1]);

    tIntegerPart = tData[0];
    tFractionPart = ((tData[1] & 0xF0) >> 4);

    temperature = tIntegerPart + (float)MPL3150A2_Calculate_Fraction_Part(tFractionPart);
    (*pTemperature) = temperature;

    printf("MPL3150A2: Temperature:%.2f\n", (*pTemperature));
    return 0;
}

float MPL3150A2_Calculate_Fraction_Part(char iData)
{
    float tFractionPart = 0;

    iData &= 0x0F;

    if(iData & 0x08)
    {
        tFractionPart += 0.5;
    }

    if(iData & 0x04)
    {
        tFractionPart += 0.25;
    }

    if(iData & 0x02)
    {
        tFractionPart += 0.125;
    }

    if(iData & 0x01)
    {
        tFractionPart += 0.0625;
    }

    return (tFractionPart);
}

int MPL3150A2_Read_Max_Pressure(float *pMaxPressure)
{
    float tPressure = 0;
    unsigned char tData[3] = {0, };
    unsigned long int tIntegerPart = 0;
    unsigned char tFractionPart = 0;
    int tRetVal = 0;

    tRetVal = I2C_ReadRegister(DEVICE_MPL3150A2, P_MAX_MSB, (char*)&tData[0] , 1);
    if(0 != tRetVal)
    {
        printf("Error to read P_MAX_MSB register at %s\n", __func__ );
        return -1;
    }

    tRetVal = I2C_ReadRegister(DEVICE_MPL3150A2, P_MAX_CSB, (char*)&tData[1] , 1);
    if(0 != tRetVal)
    {
        printf("Error to read P_MAX_CSB register at %s\n", __func__ );
        return -1;
    }

    tRetVal = I2C_ReadRegister(DEVICE_MPL3150A2, P_MAX_LSB, (char*)&tData[2] , 1);
    if(0 != tRetVal)
    {
        printf("Error to read P_MAX_LSB register at %s\n", __func__ );
        return -1;
    }

    tIntegerPart = tData[2];
    tIntegerPart = (tIntegerPart << 8) | tData[1] ;
    tIntegerPart = (tIntegerPart << 2) | ((tData[2] & 0xC0) >> 6);

    tFractionPart = (tData[2] & 0x30) >> 4;

    tPressure = (float)tIntegerPart + ((float)tFractionPart/10) ;

    (*pMaxPressure) = tPressure;

    printf("Pressure:%f, ttPressure:%f\n", (*pMaxPressure), tPressure);

    return 0;
}

int MPL3150A2_Read_Max_Temperature(float *pMaxTemperature)
{
    float temperature = 0;
    unsigned char tData[2] = {0, };
    unsigned char tIntegerPart = 0;
    unsigned char tFractionPart = 0;
    int tRetVal = 0;

    tRetVal = I2C_ReadRegister(DEVICE_MPL3150A2, T_MAX_MSB, (char*)&tData[0] , 1);
    if(0 != tRetVal)
    {
        printf("Error to read T_MAX_MSB register at %s\n", __func__ );
        return -1;
    }

    tRetVal = I2C_ReadRegister(DEVICE_MPL3150A2, T_MAX_LSB, (char*)&tData[1] , 1);
    if(0 != tRetVal)
    {
        printf("Error to read T_MAX_LSB register at %s\n", __func__ );
        return -1;
    }

    tIntegerPart = tData[0];
    tFractionPart = ((tData[1] & 0xF0) >> 4);

    temperature = (float)tIntegerPart + ((float)tFractionPart/100);
    (*pMaxTemperature) = temperature;

    return 0;
}

int MPL3150A2_Read_Min_Pressure(float *pMinPressure)
{
    float tPressure = 0;
    unsigned char tData[3] = {0, };
    unsigned long int tIntegerPart = 0;
    unsigned char tFractionPart = 0;
    int tRetVal = 0;

    tRetVal = I2C_ReadRegister(DEVICE_MPL3150A2, P_MIN_MSB, (char*)&tData[0] , 1);
    if(0 != tRetVal)
    {
        printf("Error to read P_MIN_MSB register at %s\n", __func__ );
        return -1;
    }

    tRetVal = I2C_ReadRegister(DEVICE_MPL3150A2, P_MIN_CSB, (char*)&tData[1] , 1);
    if(0 != tRetVal)
    {
        printf("Error to read P_MIN_CSB register at %s\n", __func__ );
        return -1;
    }

    tRetVal = I2C_ReadRegister(DEVICE_MPL3150A2, P_MIN_LSB, (char*)&tData[2] , 1);
    if(0 != tRetVal)
    {
        printf("Error to read P_MIN_LSB register at %s\n", __func__ );
        return -1;
    }

    tIntegerPart = tData[2];
    tIntegerPart = (tIntegerPart << 8) | tData[1] ;
    tIntegerPart = (tIntegerPart << 2) | ((tData[2] & 0xC0) >> 6);

    tFractionPart = (tData[2] & 0x30) >> 4;

    tPressure = (float)tIntegerPart + ((float)tFractionPart/10) ;

    (*pMinPressure) = tPressure;

    printf("Pressure:%f, ttPressure:%f\n", (*pMinPressure), tPressure);

    return 0;
}

int MPL3150A2_Read_Min_Temperature(float *pMinTemperature)
{
    float temperature = 0;
    unsigned char tData[2] = {0, };
    unsigned char tIntegerPart = 0;
    unsigned char tFractionPart = 0;
    int tRetVal = 0;

    tRetVal = I2C_ReadRegister(DEVICE_MPL3150A2, T_MIN_MSB, (char*)&tData[0] , 1);
    if(0 != tRetVal)
    {
        printf("Error to read T_MIN_MSB register at %s\n", __func__ );
        return -1;
    }

    tRetVal = I2C_ReadRegister(DEVICE_MPL3150A2, T_MIN_LSB, (char*)&tData[1] , 1);
    if(0 != tRetVal)
    {
        printf("Error to read T_MIN_LSB register at %s\n", __func__ );
        return -1;
    }

    tIntegerPart = tData[0];
    tFractionPart = ((tData[1] & 0xF0) >> 4);

    temperature = (float)tIntegerPart + ((float)tFractionPart/100);
    (*pMinTemperature) = temperature;

    return 0;
}

