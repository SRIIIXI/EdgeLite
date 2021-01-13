//
// Created by mukeshp on 5/19/2020.
//

#ifndef DEVELOPEMENT_I2C_INTERFACE_H
#define DEVELOPEMENT_I2C_INTERFACE_H

#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define I2C_DEVICE_NAME     "/dev/i2c-1"

int I2C_Open(const char *pDeviceName);
int I2C_Write(int iFd, char *pBuf, unsigned int iBufLen);
int I2C_Read(int iFd, char *pBuf, unsigned int iBufLen);
void I2C_Close(int iFd);

int I2C_WriteRegister(char iSlaveAddr, char iRegisterAddr, char *pDataBuf, unsigned int iDataBufLen);
//int I2C_ReadRegister(char iSlaveAddr, char iRegisterAddr, char *pDataBuf, unsigned int iDataBufLen);
int I2C_ReadRegister(char iSlaveAddr, char iRegisterAddr, char *pDataBuf, unsigned int iDataBufLen, char iFlag=1);
#endif //DEVELOPEMENT_I2C_INTERFACE_H
