//
// Created by mukeshp on 31-Jul-20.
//
#include "OnBoardSensorCnfg.h"
#include <errno.h>

OnBoardSensorCnfg_t gOnBoardSensorCnfg;

int OnBoardSensor_ConfigurationRead(void)
{
    dictionary *tpIni = NULL;
    char *tpIniName = NULL;
    const char *tpName = NULL;
    char *tmpPtr = NULL;
    int tRetCode = -1;
    char tCnfgFileName[512] = {0,};
    char tag[256] = {0,};

    sprintf(tCnfgFileName, "%s/%s", ON_BOARD_SENSOR_CNFG_FILE_PATH, ON_BOARD_SENSOR_CNFG_FILE);

    tpIniName = tCnfgFileName;

    errno = 0;
    tpIni = iniparser_load(tpIniName);
    printf("%s\n\n", strerror(errno));

    if (tpIni == NULL)
    {
        printf("Error in opening file %s\n", tpIniName);
        return tRetCode;
    }

    do
    {
        tpName = iniparser_getstring((const dictionary *) tpIni, "Accelerometer:Enable", (const char *) tmpPtr);
        if (tpName == NULL)
        {
            printf("iniparser_getstring Failed For getting Accelerometer:Enable\n");
        }
        else
        {
            gOnBoardSensorCnfg.mIsAccelerometerEnable = atoi(tpName);
            printf("ACCELEROMETER : %s\n", GET_ENABLE_DISABLE_STR(gOnBoardSensorCnfg.mIsAccelerometerEnable));
        }

        if(gOnBoardSensorCnfg.mIsAccelerometerEnable == 1)
        {
            tpName = iniparser_getstring((const dictionary *) tpIni, "Accelerometer:OrientationEnable", (const char *) tmpPtr);
            if (tpName == NULL)
            {
                printf("iniparser_getstring Failed For getting Accelerometer:OrientationEnable\n");
            }
            else
            {
                gOnBoardSensorCnfg.mIsOrientationEnable = atoi(tpName);
                printf("ACCELEROMETER ORIENTATION: %s\n", GET_ENABLE_DISABLE_STR(gOnBoardSensorCnfg.mIsOrientationEnable));
            }

            tpName = iniparser_getstring((const dictionary *) tpIni, "Accelerometer:FreeFallEnable", (const char *) tmpPtr);
            if (tpName == NULL)
            {
                printf("iniparser_getstring Failed For getting Accelerometer:FreeFallEnable\n");
            }
            else
            {
                gOnBoardSensorCnfg.mIsFreeFallEnable = atoi(tpName);
                printf("ACCELEROMETER FREEFALL: %s\n", GET_ENABLE_DISABLE_STR(gOnBoardSensorCnfg.mIsFreeFallEnable) );
            }

            tpName = iniparser_getstring((const dictionary *) tpIni, "Accelerometer:InterruptOn", (const char *) tmpPtr);
            if (tpName == NULL)
            {
                printf("iniparser_getstring Failed For getting Accelerometer:InterruptOn\n");
            }
            else
            {
                gOnBoardSensorCnfg.mInterruptPinSel = atoi(tpName);
                printf("ACCELEROMETER INTERRUPT: %s\n", GET_INTERRUPT_STR(gOnBoardSensorCnfg.mInterruptPinSel) );
            }
        }

        tpName = iniparser_getstring((const dictionary *) tpIni, "Pressure:Enable", (const char *) tmpPtr);
        if (tpName == NULL)
        {
            printf("iniparser_getstring Failed For getting Pressure:Enable\n");
        }
        else
        {
            gOnBoardSensorCnfg.mIsPressureSensorEnable = atoi(tpName);
            printf("PRESSURE SENSOR: %s\n", GET_ENABLE_DISABLE_STR(gOnBoardSensorCnfg.mIsPressureSensorEnable));
        }

        if(gOnBoardSensorCnfg.mIsPressureSensorEnable == 1)
        {
            tpName = iniparser_getstring((const dictionary *) tpIni, "Pressure:TemperatureEnable", (const char *) tmpPtr);
            if (tpName == NULL)
            {
                printf("iniparser_getstring Failed For getting Pressure:TemperatureEnable\n");
            }
            else
            {
                gOnBoardSensorCnfg.mIsPressureSensorTemperatureEnable = atoi(tpName);
                printf("PRESSURE SENSOR TEMPERATURE: %s\n", GET_ENABLE_DISABLE_STR(gOnBoardSensorCnfg.mIsPressureSensorTemperatureEnable));
            }

            tpName = iniparser_getstring((const dictionary *) tpIni, "Pressure:PressureReadingDelta", (const char *) tmpPtr);
            if (tpName == NULL)
            {
                printf("iniparser_getstring Failed For getting Pressure:PressureReadingDelta\n");
            }
            else
            {
                gOnBoardSensorCnfg.mPressureChangeDelta = atoi(tpName);
                printf("PRESSURE CHANGE DELTA: %d\n", gOnBoardSensorCnfg.mPressureChangeDelta);
            }

            if(gOnBoardSensorCnfg.mIsPressureSensorTemperatureEnable == 1)
            {
                tpName = iniparser_getstring((const dictionary *) tpIni, "Pressure:TemperatureReadingDelta",
                                             (const char *) tmpPtr);
                if (tpName == NULL) {
                    printf("iniparser_getstring Failed For getting Pressure:TemperatureReadingDelta\n");
                } else {
                    gOnBoardSensorCnfg.mTemperatureChangeDelta = atoi(tpName);
                    printf("TEMPERATURE CHANGE DELTA: %d\n", gOnBoardSensorCnfg.mTemperatureChangeDelta);
                }
            }
        }

        tpName = iniparser_getstring((const dictionary *) tpIni, "Humidity:Enable", (const char *) tmpPtr);
        if (tpName == NULL)
        {
            printf("iniparser_getstring Failed For getting Humidity:Enable\n");
        }
        else
        {
            gOnBoardSensorCnfg.mIsHumiditySensorEnable = atoi(tpName);
            printf("HUMIDITY SENSOR: %s\n", GET_ENABLE_DISABLE_STR(gOnBoardSensorCnfg.mIsHumiditySensorEnable));
        }

        if(gOnBoardSensorCnfg.mIsHumiditySensorEnable == 1)
        {
            tpName = iniparser_getstring((const dictionary *) tpIni, "Humidity:TemperatureEnable", (const char *) tmpPtr);
            if (tpName == NULL)
            {
                printf("iniparser_getstring Failed For getting Humidity:TemperatureEnable\n");
            }
            else
            {
                gOnBoardSensorCnfg.mIsHumiditySensorTemperatureEnable = atoi(tpName);
                printf("HUMIDITY SENSOR TEMPERATURE: %s\n", GET_ENABLE_DISABLE_STR(gOnBoardSensorCnfg.mIsHumiditySensorTemperatureEnable));
            }
        }

        tRetCode = 0;
    } while (0);

    iniparser_freedict(tpIni);

    printf("\n\n\n");

    return tRetCode;
}

int WriteToIniFile(dictionary *ini , char *config_file_name)
{
    FILE *Ini_Fp = NULL;

    if((Ini_Fp = fopen(config_file_name, "w+")) == NULL )
    {
        printf("Ini file not present\n");
        return -1;
    }

    iniparser_dump_ini(ini, Ini_Fp);

    if(Ini_Fp != NULL)
    {
        fclose(Ini_Fp);
    }

    return 0;
}

int OnBoardSensor_ConfigurationWrite(const char *pTag, const char *pValue)
{
    dictionary  *ini  = NULL;
    char    *ini_name = NULL ;
    char    config_file_name[100] = {0 ,} ;
    int  	ret = -1;
    int 	rc = -1;
    char 	tag_dc[100] 	= {0,};
    char 	value_dc[100] 	= {0,};

    if((NULL == pTag) || (NULL == pValue))
    {
        printf("Error ptag:%p or pValue:%p at %s\n", __func__ );
        return -1;
    }

    sprintf(config_file_name,"%s/%s",ON_BOARD_SENSOR_CNFG_FILE_PATH, ON_BOARD_SENSOR_CNFG_FILE);
    ini_name = config_file_name ;

    ini = iniparser_load(ini_name);
    if(ini == NULL)
    {
        printf("Error in opening file %s", ini_name);
        return -1;
    }

    printf("tag:%s  value:%s\n", pTag, pValue);

    do {
        ret = iniparser_set(ini, pTag, pValue);
        if (ret != 0)
        {
            printf("Error iniparser_set  to write dc value");
            rc = -1;
            break;
        }

        ret = WriteToIniFile(ini, config_file_name);
        if (ret != 0)
        {
            printf("Error at to write dc value in ini file");
            rc = -1;
            break;
        }

        rc = 0;
    }while (0);

    iniparser_freedict(ini);
    return rc;
}


