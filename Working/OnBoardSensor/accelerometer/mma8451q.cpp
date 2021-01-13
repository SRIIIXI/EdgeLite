//
// Created by mukeshp on 29-Aug-20.
//
#include "mma8451q.h"
#include "I2c_Interface.h"
#include "OnBoardSensorCnfg.h"

extern OnBoardSensorCnfg_t gOnBoardSensorCnfg;

int MMA8451Q_Poll_Device(void)
{
    char gpio_irq[64] = {0, };
    int ret, irqfd = 0, i = 0;
    unsigned char tInt1Source = 0;
    int buf;
    fd_set fds;

    char tGpioPinStr[8] = {0, };
    unsigned char tGpioPin = 0;

    if(gOnBoardSensorCnfg.mInterruptPinSel == 1)
    {
        strcpy(tGpioPinStr, MMA8451Q_INT1_GPIO_STRING);
        tGpioPin = MMA8451Q_INT1_GPIO;
    }
    else if(gOnBoardSensorCnfg.mInterruptPinSel == 2)
    {
        strcpy(tGpioPinStr, MMA8451Q_INT2_GPIO_STRING);
        tGpioPin = MMA8451Q_INT2_GPIO;
    }

    FD_ZERO(&fds);

    snprintf(gpio_irq, sizeof(gpio_irq), "/sys/class/gpio/%s/value", tGpioPinStr);
    irqfd = open(gpio_irq, O_RDONLY, S_IREAD);
    if(irqfd == -1)
    {
        printf("Could not open IRQ %s at %s\n", MMA8451Q_INT2_GPIO_STRING, __func__ );
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

            //printf("IRQ detected %d\n", i);
            fflush(stdout);
            i++;

            /* The return value includes the actual GPIO register value */
            read(irqfd, &buf, sizeof(buf));
            lseek(irqfd, 0, SEEK_SET);

            I2C_ReadRegister(DEVICE_MMA8451Q, INT_SOURCE_MMA8451Q, (char*)&tInt1Source, 1);
            //printf("tInt1Source:0x%02X\n", tInt1Source);

            if(tInt1Source & 0x10)
            {
                MMA8451Q_GetOrientationData();
            }
            else if(tInt1Source & 0x04)
            {
                MMA8451Q_GetFreeFallData();
            }
        }

        /*Sleep, or  do any other processing here */
        //usleep(100000);
    }

    close(irqfd);
    return 0;
}

void MMA8451Q_InterruptPinConfigure(void)
{
    int tFdInt1 = -1;
    char tGpioIrq[256] = {0,};
    char tCommand[256] = {0,};
    char tGpioPinStr[8] = {0, };
    unsigned char tGpioPin = 0;

    if(gOnBoardSensorCnfg.mInterruptPinSel == 1)
    {
        strcpy(tGpioPinStr, MMA8451Q_INT1_GPIO_STRING);
        tGpioPin = MMA8451Q_INT1_GPIO;
    }
    else if(gOnBoardSensorCnfg.mInterruptPinSel == 2)
    {
        strcpy(tGpioPinStr, MMA8451Q_INT2_GPIO_STRING);
        tGpioPin = MMA8451Q_INT2_GPIO;
    }

    sprintf(tGpioIrq, "/sys/class/gpio/%s/direction", tGpioPinStr);
    tFdInt1 = open(tGpioIrq, O_RDWR);
    if (tFdInt1 < 0)
    {
        /* if not exported already than export it */
        sprintf(tCommand, "echo \"%d\" > /sys/class/gpio/export", tGpioPin);
        system(tCommand);
    }
    else
    {
        close(tFdInt1);
    }

    sprintf(tCommand, "echo \"in\" > /sys/class/gpio/%s/direction", tGpioPinStr);
    system(tCommand);

    sprintf(tCommand,"echo \"falling\" > /sys/class/gpio/%s/edge", tGpioPinStr);
    system(tCommand);

    return;
}

void MMA8451Q_Reset(void)
{
    unsigned char tDataBuf = 0;

    tDataBuf = 0x40;
    I2C_WriteRegister(DEVICE_MMA8451Q, CTRL_REG2_MMA8451Q, (char*)&tDataBuf, 1);

    sleep(3);
}

void MMA8451Q_Init_Orientation(void)
{
    unsigned char tDataBuf = 0;

    /* Step 1: Put the part into Standby Mode */
    MMA8451Q_Mode_Select(MMA8451Q_MODE_STANDBY);

    /* Step 2: Set the PL_EN bit in Register 0x11 PL_CFG.
     * This will enable the orientation detection functionality.
     * */
    I2C_ReadRegister(DEVICE_MMA8451Q, PL_CFG, (char*)&tDataBuf, 1);
    //printf("PL_CFG:0x%02X\n", tDataBuf);
    tDataBuf |= 0x40;
    I2C_WriteRegister(DEVICE_MMA8451Q, PL_CFG, (char*)&tDataBuf, 1);
    //printf("PL_CFG:0x%02X\n", tDataBuf);

    /* Step 3: Set the Back/Front Angle trip points and z-lockout angle trip point */
    I2C_ReadRegister(DEVICE_MMA8451Q, PL_BF_ZCOMP, (char*)&tDataBuf, 1);
    /* Back-Front transition: Z < 70° or Z > 290°
     * Front-Back transition: Z > 110° and Z < 250°
     * */
    tDataBuf &= 0x3F ;
    tDataBuf |= 2 << 6 ;
    I2C_WriteRegister(DEVICE_MMA8451Q, PL_BF_ZCOMP, (char*)&tDataBuf, 1);

    /* Step 4: Z-lockout threshold angles: 42 degree */
    I2C_ReadRegister(DEVICE_MMA8451Q, PL_BF_ZCOMP, (char*)&tDataBuf, 1);
    tDataBuf &= 0xF8;
    tDataBuf |= 0x07;
    I2C_WriteRegister(DEVICE_MMA8451Q, PL_BF_ZCOMP, (char*)&tDataBuf, 1);

    /* Step 5: Set the Trip Threshold Angle (portrait and landscape change angle) and
     * the Hysteresis Angle - To avoid frequently change at trip threshold point
     * */
    tDataBuf = 0;
    I2C_ReadRegister(DEVICE_MMA8451Q, P_L_THS_REG, (char*)&tDataBuf, 1);
    tDataBuf &= 0x07;
    tDataBuf |= 0x10 << 3; /* Trip thresold angle is 45 degree */
    I2C_WriteRegister(DEVICE_MMA8451Q, P_L_THS_REG, (char*)&tDataBuf, 1);

    /* Step 6: Set the Hysteresis angle */
    tDataBuf = 0;
    I2C_ReadRegister(DEVICE_MMA8451Q, P_L_THS_REG, (char*)&tDataBuf, 1);
    tDataBuf &= 0xF8;
    tDataBuf |= 0x07; /* Hysteresis angle with thresold trip angle is (+/-)24 degree */
    I2C_WriteRegister(DEVICE_MMA8451Q, P_L_THS_REG, (char*)&tDataBuf, 1);

    /* Step 7: Register 0x2D, Control Register 4 configures all embedded features for interrupt detection. */
    I2C_ReadRegister(DEVICE_MMA8451Q, CTRL_REG4_MMA8451Q, (char*)&tDataBuf, 1);
    tDataBuf &= 0xEF;
    tDataBuf |= 0x10;
    I2C_WriteRegister(DEVICE_MMA8451Q, CTRL_REG4_MMA8451Q, (char*)&tDataBuf, 1);

    /* Step 8: Register 0x2E is Control Register 5 which gives the option of routing the interrupt
     * to either INT1 or INT2
     * */
    I2C_ReadRegister(DEVICE_MMA8451Q, CTRL_REG5_MMA8451Q, (char*)&tDataBuf, 1);
    tDataBuf &= 0xEF;

    if(gOnBoardSensorCnfg.mInterruptPinSel == 1)
    {
        tDataBuf |= 0x10; //Selected INT1
    }

    I2C_WriteRegister(DEVICE_MMA8451Q, CTRL_REG5_MMA8451Q, (char*)&tDataBuf, 1);

    /* Step 9: Set the debounce counter in register 0x12 */
    tDataBuf = 50;
    I2C_WriteRegister(DEVICE_MMA8451Q, PL_COUNT, (char*)&tDataBuf, 1);

    /* Step 10: Set the data sample rate*/
    MMA8451Q_OutputData_Rate_Select(MMA8451Q_ACTIVE_100HZ);

    /* Step 11: Put the part into Active Mode */
    MMA8451Q_Mode_Select(MMA8451Q_MODE_ACTIVE);

    return;
}

void MMA8451Q_Init_FreeFallDetection(void)
{
    unsigned char   tDataBuf = 0;

    /* Step 1: Put the device into Standby Mode: Register 0x2A CTRL_REG1 */
    MMA8451Q_Mode_Select(MMA8451Q_MODE_STANDBY);

    /* Step 2: Configuration Register set for Freefall Detection enabling “AND” condition, OAE = 0,
     *  Enabling X,Y, Z and the Latch
     *  */
    tDataBuf = 0xB8;
    I2C_WriteRegister(DEVICE_MMA8451Q, FF_MT_CFG, (char*)&tDataBuf, 1);

    /* Step 3:Threshold Setting Value for the resulting acceleration < 0.2g.
     * Note: The step count is 0.063g/count,
     * 0.2g/0.063g = 3.17 counts so Round to 3 counts Freefall detection on X,Y,Z axis
     * when X AND Y AND Z < 0.2g
     * */
    tDataBuf = 0x04;
    I2C_WriteRegister(DEVICE_MMA8451Q, FF_MT_THS, (char*)&tDataBuf, 1);

    /* Step 4: Set the debounce counter to eliminate false positive readings for 100Hz sample rate
     * with a requirement of 320 ms timer, assuming Normal Mode.
            Note: 320 ms/10 ms (steps) = 32 counts
            Freefall detection Setting debounce timer */
    tDataBuf = 32;
    I2C_WriteRegister(DEVICE_MMA8451Q, FF_MT_COUNT, (char*)&tDataBuf, 1);

    /* Step 5: Register 0x2D, Control Register 4 configures all embedded features for interrupt detection. */
    I2C_ReadRegister(DEVICE_MMA8451Q, CTRL_REG4_MMA8451Q, (char*)&tDataBuf, 1);
    tDataBuf &= 0xFB;
    tDataBuf |= 0x04;
    I2C_WriteRegister(DEVICE_MMA8451Q, CTRL_REG4_MMA8451Q, (char*)&tDataBuf, 1);

    /*Step 6: Register 0x2E is Control Register 5 which gives the option of routing the interrupt to
     * either INT1 or INT2 Route the Motion/Freefall Interrupt Function to INT2 hardware pin (CTRL_REG5)
    */
    I2C_ReadRegister(DEVICE_MMA8451Q, CTRL_REG5_MMA8451Q, (char*)&tDataBuf, 1);
    tDataBuf &= 0xFB;

    if(gOnBoardSensorCnfg.mInterruptPinSel == 1)
    {
        tDataBuf |= 0x04; //Selected INT1
    }
    I2C_WriteRegister(DEVICE_MMA8451Q, CTRL_REG5_MMA8451Q, (char*)&tDataBuf, 1);

    /*Step 7: Set the data sample rate and put in active mode */
    MMA8451Q_OutputData_Rate_Select(MMA8451Q_ACTIVE_100HZ);

    /* Step 8: Put the part into Active Mode */
    MMA8451Q_Mode_Select(MMA8451Q_MODE_ACTIVE);

    return;
}

void MMA8451Q_Mode_Select(unsigned char iMode)
{
    unsigned char tDataBuf;
    int tfd;

    if(iMode >= MMA8451Q_MODE_MAX)
    {
        printf("Invalid mode - %d at %s\n", iMode, __func__);
        return;
    }

    /*
    ** Read current value of System Control 1 Register.
    ** Put sensor into Standby Mode by clearing the Active bit
    ** Return with previous value of System Control 1 Register.
    */

    I2C_ReadRegister(DEVICE_MMA8451Q, CTRL_REG1_MMA8451Q, (char*)&tDataBuf, 1);

    //printf("Control Register-1 = 0x%X\n", tDataBuf);

    if(MMA8451Q_MODE_STANDBY == iMode)
    {
        /*
        ** StandBy mode Clear the Active bit in CTRL Reg 1
        **/
        tDataBuf &=  (~0x01);
    }
    else if(MMA8451Q_MODE_ACTIVE == iMode)
    {
        /*
        ** Active mode Set the Active bit in CTRL Reg 1
        **/
        tDataBuf |=  0x01;
    }

    I2C_WriteRegister(DEVICE_MMA8451Q, CTRL_REG1_MMA8451Q, (char*)&tDataBuf, 1);

    I2C_ReadRegister(DEVICE_MMA8451Q, CTRL_REG1_MMA8451Q, (char*)&tDataBuf, 1);

    //printf("Control Register-1 = 0x%X\n", tDataBuf);

    I2C_Close(tfd);
}

void MMA8451Q_OutputData_Rate_Select(unsigned char iDataRate)
{
    unsigned char tDataBuf;

    if(iDataRate >= MMA8451Q_ACTIVE_MAX)
    {
        printf("Invalid Active mode data rate-%d at %s\n", iDataRate, __func__);
        return;
    }

    MMA8451Q_Mode_Select(MMA8451Q_MODE_STANDBY);

    I2C_ReadRegister(DEVICE_MMA8451Q, CTRL_REG1_MMA8451Q, (char*)&tDataBuf, 1);

    //printf("Control Reg1:0x%02X\n", tDataBuf);

    tDataBuf &= 0xC7;

    switch(iDataRate)
    {
        case MMA8451Q_ACTIVE_800HZ:
        {
            tDataBuf |= MMA8451Q_ACTIVE_800HZ<<3;
            break;
        }
        case MMA8451Q_ACTIVE_400HZ:
        {
            tDataBuf |= MMA8451Q_ACTIVE_400HZ<<3;
            break;
        }
        case MMA8451Q_ACTIVE_200HZ:
        {
            tDataBuf |= MMA8451Q_ACTIVE_200HZ<<3;
            break;
        }
        case MMA8451Q_ACTIVE_100HZ:
        {
            tDataBuf |= MMA8451Q_ACTIVE_100HZ<<3;
            break;
        }
        case MMA8451Q_ACTIVE_50HZ:
        {
            tDataBuf |= MMA8451Q_ACTIVE_50HZ<<3;
            break;
        }
        case MMA8451Q_ACTIVE_12_5HZ:
        {
            tDataBuf |= MMA8451Q_ACTIVE_12_5HZ<<3;
            break;
        }
        case MMA8451Q_ACTIVE_6_25HZ:
        {
            tDataBuf |= MMA8451Q_ACTIVE_6_25HZ<<3;
            break;
        }
        case MMA8451Q_ACTIVE_1_563HZ:
        {
            tDataBuf |= MMA8451Q_ACTIVE_1_563HZ<<3;
            break;
        }
        default:
        {
            break;
        }
    }

    //printf("tDataBuf:0x%02X\n", tDataBuf);
    I2C_WriteRegister(DEVICE_MMA8451Q, CTRL_REG1_MMA8451Q, (char*)&tDataBuf, 1);

    I2C_ReadRegister(DEVICE_MMA8451Q, CTRL_REG1_MMA8451Q, (char*)&tDataBuf, 1);
    //printf("CTRL_REG1 Register = 0x%X\n", tDataBuf);

    MMA8451Q_Mode_Select(MMA8451Q_MODE_ACTIVE);

    return;
}

void MMA8451Q_GetOrientationData(void)
{
    unsigned char   tDataBuf = 0;

    /* Perform an Action since Orientation Flag has been set.
     * Read the PL State from the Status Register, clear the interrupt
     * */
    I2C_ReadRegister(DEVICE_MMA8451Q, PL_STATUS, (char*)&tDataBuf, 1);

    /* Update Image on Display Screen based on the data stored */
    if(tDataBuf & 0x80)
    {
        if(tDataBuf & 0x40)
        {
            printf("Z-tilt lockout trip angle has been exceeded. Lockout has been detected\n");
        }

        switch ((tDataBuf & 0x06) >> 1)
        {
            case 0:
            {
                printf("PORTRAIT_UP\n");
                break;
            }
            case 1:
            {
                printf("PORTRAIT_DOWN\n");
                break;
            }
            case 2:
            {
                printf("LANDSCAPE_RIGHT\n");
                break;
            }
            case 3:
            {
                printf("LANDSCAPE_LEFT\n");
                break;
            }
            default:
            {
                printf("Invalid\n");
            }
        }

        if(tDataBuf & 0x01)
        {
            printf("BACK\n");
        }
        else
        {
            printf("FRONT\n");
        }
    }
    else
    {
        printf("No Changes in Portrait/Landscape orientation\n");
    }

    return;
}

void MMA8451Q_GetFreeFallData(void)
{
    unsigned char   tDataBuf = 0;

    printf("FreeFall Detection\n");

    /* Perform an Action since Free fall Flag has been set.
     * Read the FF_MT_SRC Register, clear the interrupt
     * */
    I2C_ReadRegister(DEVICE_MMA8451Q, FF_MT_SRC, (char*)&tDataBuf, 1);

    return;
}

