//
// Created by mukeshp on 29-Aug-20.
//

#ifndef ONBOARDSENSOR_MMA8451Q_H
#define ONBOARDSENSOR_MMA8451Q_H

#define DEVICE_MMA8451Q		(0x38)

#define MMA8451Q_INT1_GPIO              163
#define MMA8451Q_INT1_GPIO_STRING       "gpio163" //CN2_56

#define MMA8451Q_INT2_GPIO               47
#define MMA8451Q_INT2_GPIO_STRING        "gpio47" //CN2_58

typedef enum
{
    MMA8451Q_MODE_STANDBY   =   0,
    MMA8451Q_MODE_ACTIVE	=   1,
    MMA8451Q_MODE_MAX       =	2
}MMA8451Q_Mode_e;

typedef enum
{
    MMA8451Q_SCALE_2G = 1,
    MMA8451Q_SCALE_4G = 2,
    MMA8451Q_SCALE_8G = 3,
    MMA8451Q_SCALE_MAX= 4,
}MMA8451Q_Scale_e;

typedef enum
{
    MMA8451Q_ACTIVE_800HZ 	    = 0,
    MMA8451Q_ACTIVE_400HZ 	    = 1,
    MMA8451Q_ACTIVE_200HZ 	    = 2,
    MMA8451Q_ACTIVE_100HZ 	    = 3,
    MMA8451Q_ACTIVE_50HZ 	    = 4,
    MMA8451Q_ACTIVE_12_5HZ	    = 5,
    MMA8451Q_ACTIVE_6_25HZ 	    = 6,
    MMA8451Q_ACTIVE_1_563HZ	    = 7,
    MMA8451Q_ACTIVE_MAX 		= 8,
}MMA8451Q_Active_DataRate_e;

typedef enum
{
    MMA8451Q_SLEEP_DATARATE_50HZ 		= 0,
    MMA8451Q_SLEEP_DATARATE_12_5HZ 	= 1,
    MMA8451Q_SLEEP_DATARATE_6_25HZ 	= 2,
    MMA8451Q_SLEEP_DATARATE_1_56HZ 	= 3,
    MMA8451Q_SLEEP_DATARATE_MAX 		= 4,
}MMA8451Q_Sleep_DataRate_e;

enum
{
    STATUS_F_STATUS = 0x00,
    OUT_X_MSB 			= 0x01,
    OUT_X_LSB 			= 0x02,
    OUT_Y_MSB 			= 0x03,
    OUT_Y_LSB 			= 0x04,
    OUT_Z_MSB 			= 0x05,
    OUT_Z_LSB 			= 0x06,
    UNUSED_1				=	0x07,
    UNUSED_2				=	0x08,
    F_SETUP					=	0x09,
    TRIG_CFG				= 0x0A,
    SYSMOD_MMA8451Q					=	0x0B,
    INT_SOURCE_MMA8451Q 			= 0x0C,
    WHO_AM_I_MMA8451Q 				= 0x0D,
    XYZ_DATA_CFG 		= 0x0E,
    HP_FILTER_CUTOFF = 0x0F,
    PL_STATUS 			= 0x10,
    PL_CFG 					= 0x11,
    PL_COUNT 				= 0x12,
    PL_BF_ZCOMP 		= 0x13,
    P_L_THS_REG 		= 0x14,
    FF_MT_CFG 			= 0x15,
    FF_MT_SRC 			= 0x16,
    FF_MT_THS 			= 0x17,
    FF_MT_COUNT 		= 0x18,
    UNUSED_3 = 0x19,
    UNUSED_4 = 0x1A,
    UNUSED_5 = 0x1B,
    UNUSED_6 = 0x1C,
    TRANSIENT_CFG = 0x1D,
    TRANSIENT_SCR = 0x1E,
    TRANSIENT_THS = 0x1F,
    TRANSIENT_COUNT = 0x20,
    PULSE_CFG	= 0x21,
    PULSE_SRC = 0x22,
    PULSE_THSX = 0x23,
    PULSE_THSY = 0x24,
    PULSE_THSZ = 0x25,
    PULSE_TMLT = 0x26,
    PULSE_LTCY = 0x27,
    PULSE_WIND = 0x28,
    ASLP_COUNT = 0x29,
    CTRL_REG1_MMA8451Q = 0x2A,
    CTRL_REG2_MMA8451Q = 0x2B,
    CTRL_REG3_MMA8451Q = 0x2C,
    CTRL_REG4_MMA8451Q = 0x2D,
    CTRL_REG5_MMA8451Q = 0x2E,
    OFF_X = 0x2F,
    OFF_Y = 0x30,
    OFF_Z = 0x31
};

void MMA8451Q_Reset(void);
void MMA8451Q_Init_Orientation(void);
void MMA8451Q_Init_FreeFallDetection(void);
void MMA8451Q_Mode_Select(unsigned char iMode);
void MMA8451Q_OutputData_Rate_Select(unsigned char iDataRate);
int MMA8451Q_Poll_Device(void);
void MMA8451Q_InterruptPinConfigure(void);
void MMA8451Q_GetOrientationData(void);
void MMA8451Q_GetFreeFallData(void);

#endif //ONBOARDSENSOR_MMA8451Q_H
