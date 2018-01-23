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
    _Settings.DownloadDirectory = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
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

        if(!dir.exists(_Settings.DownloadDirectory))
        {
            dir.mkpath(_Settings.DownloadDirectory);
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

                settings.beginGroup("Downloads");
                _Settings.DownloadDirectory = settings.value("DownloadDirectory").toString();
                settings.endGroup();

                settings.beginGroup("Style");
                _Settings.Style = settings.value("Style").toString();
                settings.endGroup();

                settings.beginGroup("Cameras");

                QStringList keys = settings.childKeys();

                foreach(QString keystr, keys)
                {
                    QStringList tokens = settings.value(keystr).toString().split('/');

                    ServiceInterface* dev = new ServiceInterface();

                    dev->DeviceName = keystr;
                    dev->IpAddress = tokens.at(0);
                    dev->ServicePort = QVariant(tokens.at(1)).toUInt();
                    dev->ServiceUser = tokens.at(2);
                    dev->Password = tokens.at(3);

                    addCamera(dev);
                }

                settings.endGroup();
            }
        }
    }

    processEvents();

    ApplicationThemeManager.initFontPreference();

    if(_Settings.Style == "L")
    {
        ApplicationThemeManager.applyLightTheme(this);
    }
    else
    {
        ApplicationThemeManager.applyDarkTheme(this);
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
    return _Settings.DownloadDirectory;
}

void Tracks::setRecordingsDirectory(QString str)
{
    _Settings.DownloadDirectory = str;
}

bool Tracks::addCamera(ServiceInterface *device)
{
    if(_CameraCollection.contains(device->CameraName))
    {
        return false;
    }

    _CameraCollection.insert(device->CameraName, device);

    return true;
}

void Tracks::removeCamera(QString camname)
{
    if(_CameraCollection.contains(camname))
    {
        _CameraCollection.remove(camname);
    }
}

ServiceInterface *Tracks::getServiceInterface(QString camname)
{
    if(_CameraCollection.contains(camname))
    {
        ServiceInterface* refobj = _CameraCollection.value(camname);

        if(refobj != nullptr)
        {
            return refobj;
        }
    }

    return nullptr;
}

QList<QString> Tracks::cameraCollection()
{
    return _CameraCollection.keys();
}

QList<ServiceInterface *> Tracks::devices()
{
    return _CameraCollection.values();
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

