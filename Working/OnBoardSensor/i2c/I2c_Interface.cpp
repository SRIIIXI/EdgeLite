//
// Created by mukeshp on 5/19/2020.
//
#include "I2c_Interface.h"

static inline __s32 i2c_smbus_access(int file, char read_write, __u8 command,
                                     int size, union i2c_smbus_data *data)
{
    struct i2c_smbus_ioctl_data args;

    args.read_write = read_write;
    args.command = command;
    args.size = size;
    args.data = data;
    return ioctl(file,I2C_SMBUS,&args);
}


static inline __s32 i2c_smbus_read_byte_data(int file, __u8 command)
{
    union i2c_smbus_data data;
    if (i2c_smbus_access(file,I2C_SMBUS_READ,command,
                         I2C_SMBUS_BYTE_DATA,&data))
        return -1;
    else
        return 0x0FF & data.byte;
}

int I2C_WriteRegister(char iSlaveAddr, char iRegisterAddr, char *pDataBuf, unsigned int iDataBufLen)
{
    int tFd = -1;
    int tIdx = 0;
    int tRetVal = -1;
    char tBuffer[16] = {0,};

    //printf("iRegisterAddr:0x%02X, pDataBuf[0]:0x%02X, iDataBufLen:%d\n", iRegisterAddr, pDataBuf[0], iDataBufLen);

    tBuffer[tIdx++] = iRegisterAddr;

    for( int temp = 0; temp < iDataBufLen ; ++temp)
    {
        tBuffer[tIdx++] = pDataBuf[temp];
    }

    tBuffer[tIdx] = '\0';

    #if 0
    for(int i=0; i< (iDataBufLen+1) ; ++i)
    {
        printf("tBuffer[%d]:0x%X, ", i, tBuffer[i]);
    }
    printf("\n");
    #endif

    tFd = I2C_Open(I2C_DEVICE_NAME);
    if(tFd < 0)
    {
        printf("Error at I2c_Device open (%s) at %s\n", I2C_DEVICE_NAME, __func__ );
        return -1;
    }

    do
    {
        if (ioctl(tFd, I2C_SLAVE, iSlaveAddr>>1) < 0) {
            printf("Error to set slave address at %s\n", __func__);
            break;
        }

        if ((iDataBufLen+1) != I2C_Write(tFd, tBuffer, iDataBufLen+1)) {
            printf("Error to write register address at %s\n", __func__);
            break;
        }

        /*Success return value */
        tRetVal = 0;

    }while (0);

    I2C_Close(tFd);

    return tRetVal;
}

int I2C_ReadRegister(char iSlaveAddr, char iRegisterAddr, char *pDataBuf, unsigned int iDataBufLen, char iFlag)
{
    int tFd = -1;
    int tRetVal = -1;

    tFd = I2C_Open(I2C_DEVICE_NAME);
    if(tFd < 0)
    {
        printf("Error at I2c_Device open(%s) at %s\n", I2C_DEVICE_NAME, __func__ );
        return -1;
    }

    do
    {
        //printf("iSlaveAddr>>1:0x%x\n", iSlaveAddr>>1);
        if (ioctl(tFd, I2C_SLAVE, iSlaveAddr>>1) < 0) {
            printf("Error to set slave address at %s\n", __func__);
            break;
        }

        if(iFlag == 0) {
            //printf("iRegisterAddr:0x%x\n", iRegisterAddr);
            if (1 != I2C_Write(tFd, &iRegisterAddr, 1)) {
                printf("Error to write register address at %s\n", __func__);
                break;
            }

            sleep(1);

            if (iDataBufLen != I2C_Read(tFd, pDataBuf, iDataBufLen)) {
                printf("Error to read register address at %s\n", __func__);
                break;
            }
        }
        else
        {
            pDataBuf[0] = i2c_smbus_read_byte_data(tFd, iRegisterAddr);
        }
        /*Success return value */
        tRetVal = 0;

    }while (0);

    I2C_Close(tFd);

    return tRetVal;
}

int I2C_Open(const char *pDeviceName)
{
    int tFd = -1 ;

    tFd = open (pDeviceName, O_RDWR);
    if (tFd < 0)
    {
        printf("Error to open %s device at I2C_Open\n", pDeviceName);
        return -1;
    }

    return tFd;
}

int I2C_Write(int iFd, char *pBuf, unsigned int iBufLen)
{
    int tWrittenBytes = -1;

    errno = 0;
    tWrittenBytes = write(iFd, pBuf, iBufLen);
    if(tWrittenBytes < 0)
    {
        /* ERROR HANDLING: i2c transaction failed */
        printf("Error at I2C_Write(%s)\n", strerror(errno));
    }
    else
    {
        //printf("Success at I2C_Write\n");
    }

    return tWrittenBytes;
}

int I2C_Read(int iFd, char *pBuf, unsigned int iBufLen)
{
    int tReadBytes = -1;

    /* Using I2C Read */
    tReadBytes = read(iFd, pBuf, iBufLen);
    if(tReadBytes < 0)
    {
        /* ERROR HANDLING: i2c transaction failed */
        printf("Error at I2C_Read\n");
    }
    else
    {
        /* buf[0] contains the read byte */
        //printf("pBuf[0]:%x\n", pBuf[0]);
        //printf("I2C Read success \n");
    }

    return tReadBytes;
}

void I2C_Close(int iFd)
{
    close(iFd);
    //printf("I2C Close executed\n");
}
