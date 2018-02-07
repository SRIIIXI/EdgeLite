#include "Tracks.h"
#include "MainWindow.h"

Tracks* tracksPtr = nullptr;

Tracks::Tracks(int argc, char *argv[]) : QApplication (argc, argv)
{
    tracksPtr = this;
    _Settings.Style = "L";
    QPixmap pix(":images/splashscreen.png");
    appSplash.setPixmap(pix);

    appSplash.show();

    //Auto generate the filenames
    QString appnamestr = applicationName();
    QString curpath = QDir::currentPath();
    QString datalocation = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)+ "/" + appnamestr;   
    _TemplateDir = datalocation + "/templates/";
    _Settings.RecordingsDirectory = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    _ConfigFile = datalocation + "/config/";
    _ConfigFile += appnamestr + ".ini";

    QDir dir(datalocation);

    if(!dir.exists(datalocation))
    {
        dir.mkpath(datalocation);
    }
    else
    {
        if(!dir.exists(_TemplateDir))
        {
            dir.mkpath(_TemplateDir);
            copyRecursively(curpath + "/templates/", _TemplateDir);
        }

        if(!dir.exists(_Settings.RecordingsDirectory))
        {
            dir.mkpath(_Settings.RecordingsDirectory);
        }

        if(!dir.exists(QString(datalocation + "/config/")))
        {
            dir.mkpath(QString(datalocation + "/config/"));
        }
        else
        {
            QFile file(_ConfigFile);

            if(file.exists())
            {
                QSettings settings(_ConfigFile, QSettings::IniFormat);

                settings.beginGroup("Database");
                _Settings.DatasourceName = settings.value("DatasourceName").toString();
                _Settings.DatabaseHost = settings.value("DatabaseHost").toString();
                _Settings.DatabasePort = settings.value("DatabasePort").toString();
                _Settings.DatabaseUser = settings.value("DatabaseUser").toString();
                _Settings.DatabasePass = settings.value("DatabasePass").toString();
                settings.endGroup();

                settings.beginGroup("Recordings");
                _Settings.RecordingsDirectory = settings.value("RecordingsDirectory").toString();
                settings.endGroup();

                settings.beginGroup("Style");
                _Settings.Style = settings.value("Style").toString();
                settings.endGroup();

                settings.beginGroup("Cameras");
                QStringList keys = settings.childKeys();
                settings.endGroup();

                retrieveCameras(keys, settings);
            }
        }
    }

    processEvents();

    ApplicationThemeManager.initFontPreference();

    if(_Settings.Style == "D")
    {
        ApplicationThemeManager.applyDarkTheme(this);
    }
    else
    {
        ApplicationThemeManager.applyLightTheme(this);
    }
}

Tracks::~Tracks()
{
}

AppSettings* Tracks::settings()
{
    return &_Settings;
}

void Tracks::showMessage(QString str)
{
    appSplash.showMessage(str);
}

void Tracks::start()
{    mainWindowPtr->show();
    appSplash.finish(mainWindowPtr);
}

QString& Tracks::configFile()
{
    return _ConfigFile;
}

QString& Tracks::templateDirectory()
{
    return _TemplateDir;
}

QString& Tracks::recordingsDirectory()
{
    return _Settings.RecordingsDirectory;
}

void Tracks::setRecordingsDirectory(QString str)
{
    _Settings.RecordingsDirectory = str;
}

bool Tracks::addCamera(ONVIFCamera device)
{
    if(_CameraCollection.contains(device.FriendlyName))
    {
        return false;
    }

    _CameraCollection.insert(device.FriendlyName, device);

    QSettings settings(_ConfigFile, QSettings::IniFormat);
    settings.beginGroup("Cameras");
    settings.setValue(device.FriendlyName, device.uniqueToken());
    settings.endGroup();

    settings.beginGroup(device.FriendlyName);

    settings.setValue("Manufacturer", device.Manufacturer);
    settings.setValue("Model", device.Model);
    settings.setValue("FirmwareVersion", device.FirmwareVersion);
    settings.setValue("SerialNumber", device.SerialNumber);
    settings.setValue("HardwareId", device.HardwareId);
    settings.setValue("Hardware", device.Hardware);
    settings.setValue("UTCTimestamp", device.UTCTimestamp);
    settings.setValue("DeviceName", device.DeviceName);
    settings.setValue("IpAddress", device.IpAddress);
    settings.setValue("ServicePort", device.ServicePort);
    settings.setValue("ServiceUser", device.ServiceUser);
    settings.setValue("Password", device.Password);
    settings.setValue("DefaultServiceUri", device.DefaultServiceUri);

    QString svrlist;
    foreach(ONVIFService oserv, device.ServiceList)
    {
        svrlist += oserv.NameSpace + ";" + oserv.XAddress + "!";
    }

    settings.setValue("ServiceList", svrlist);

    settings.endGroup();

    settings.sync();

    return true;
}

void Tracks::removeCamera(QString camname)
{
    if(_CameraCollection.contains(camname))
    {
        QString iniKey = _CameraCollection.value(camname).FriendlyName;
        QList<ONVIFService> slist = _CameraCollection.value(camname).ServiceList;

        _CameraCollection.remove(camname);

        QSettings settings(_ConfigFile, QSettings::IniFormat);

        settings.beginGroup("Cameras");
        settings.remove(iniKey);
        settings.endGroup();

        settings.beginGroup(iniKey);
        settings.remove(""); //removes the group, and all it keys
        settings.endGroup();

        settings.sync();
    }
}

void Tracks::removeAllCameras()
{
    QList<QString> camlist = _CameraCollection.keys();

    foreach (QString cam, camlist)
    {
        removeCamera(cam);
    }
}

ONVIFCamera Tracks::getCamera(QString camname)
{
    return _CameraCollection.value(camname);
}

QList<QString> Tracks::getCameraNames()
{
    return _CameraCollection.keys();
}

QList<ONVIFCamera> Tracks::cameraList()
{
    return _CameraCollection.values();
}

QString Tracks::cameraNameByEndpoint(QString ipaddress, quint32 port)
{
    foreach(ONVIFCamera cam, _CameraCollection.values())
    {
        if(cam.IpAddress == ipaddress && cam.ServicePort == port)
        {
            return cam.FriendlyName;
        }
    }

    return "";
}

bool Tracks::copyRecursively(const QString &srcFilePath,  const QString &tgtFilePath)
{
    QDir srcdir(srcFilePath);

    QFileInfoList entries = srcdir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

    QString strorig = "";
    QString strdest = "";
    foreach(QFileInfo finfo, entries)
    {
        strorig = finfo.absoluteFilePath();
        strdest = tgtFilePath + finfo.fileName();
        QFile::copy(strorig, strdest);
    }

    return true;
}

void Tracks::saveDatabaseInfo()
{
    QSettings settings(_ConfigFile, QSettings::IniFormat);
    settings.beginGroup("Database");
    settings.setValue("DatasourceName", tracksPtr->settings()->DatasourceName);
    settings.setValue("DatabaseHost", tracksPtr->settings()->DatabaseHost);
    settings.setValue("DatabasePort", tracksPtr->settings()->DatabasePort);
    settings.setValue("DatabaseUser", tracksPtr->settings()->DatabaseUser);
    settings.setValue("DatabasePass", tracksPtr->settings()->DatabasePass);
    settings.endGroup();
    settings.sync();
}

void Tracks::saveDownloadLocation()
{
    QSettings settings(_ConfigFile, QSettings::IniFormat);
    settings.beginGroup("Recordings");
    settings.setValue("RecordingsDirectory", tracksPtr->settings()->RecordingsDirectory);
    settings.endGroup();
    settings.sync();
}

void Tracks::saveStyleOption()
{
    QSettings settings(_ConfigFile, QSettings::IniFormat);
    settings.beginGroup("Style");
    settings.setValue("Style", tracksPtr->settings()->Style);
    settings.endGroup();
    settings.sync();
}

void Tracks::retrieveCameras(QList<QString> &camlist, QSettings &settings)
{
    foreach(QString camname, camlist)
    {
        ONVIFCamera device;

        settings.beginGroup(camname);

        device.FriendlyName = camname;
        device.Manufacturer = settings.value("Manufacturer").toString();
        device.Model = settings.value("Model").toString();
        device.FirmwareVersion = settings.value("FirmwareVersion").toString();
        device.SerialNumber = settings.value("SerialNumber").toString();
        device.HardwareId = settings.value("HardwareId").toString();
        device.Hardware = settings.value("Hardware").toString();
        device.UTCTimestamp = settings.value("UTCTimestamp").toString();
        device.DeviceName = settings.value("DeviceName").toString();
        device.IpAddress = settings.value("IpAddress").toString();
        device.ServicePort = settings.value("ServicePort").toUInt();
        device.ServiceUser = settings.value("ServiceUser").toString();
        device.Password = settings.value("Password").toString();
        device.DefaultServiceUri = settings.value("DefaultServiceUri").toString();

        QStringList svrlist = settings.value("ServiceList").toString().split('!', QString::SkipEmptyParts);

        foreach(QString svrstr, svrlist)
        {
            QStringList tokens = svrstr.split(';', QString::SkipEmptyParts);
            ONVIFService srv;
            srv.NameSpace = tokens.at(0);
            srv.XAddress = tokens.at(1);
            device.ServiceList.append(srv);
        }

        _CameraCollection.insert(camname, device);

        settings.endGroup();
    }
}
