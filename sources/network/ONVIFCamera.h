#ifndef _ONVIF_CAMERA
#define _ONVIF_CAMERA

#include <QtCore>

class ONVIFService
{
public:
    QString NameSpace;
    QString XAddress;
};

class ONVIFCamera
{
public:
    QString Manufacturer;
    QString Model;
    QString FirmwareVersion;
    QString SerialNumber;
    QString HardwareId;

    QString Hardware;
    QString UTCTimestamp;
    QString DeviceName;

    QString IpAddress;
    quint32 ServicePort;
    QString ServiceUser;
    QString Password;
    QString FriendlyName;

    QList<ONVIFService> ServiceList;
    QStringList ProbeList;
    QString DefaultServiceUri;

    QString uniqueToken()
    {
        QString token = IpAddress + "/";
        token += QVariant(ServicePort).toString() + "/";
        token += ServiceUser + "/";
        token += Password + "/";
        token += DeviceName + "/";
        return token;
    }

};

#endif
