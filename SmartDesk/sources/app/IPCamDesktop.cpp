#include "IPCamDesktop.h"
#include "MainWindow.h"

IPCamDesktop* applicationPtr = nullptr;

IPCamDesktop::IPCamDesktop(int argc, char *argv[]) : QApplication (argc, argv)
{
    applicationPtr = this;

    QPixmap pix(":images/splashscreen.png");

    appSplash.setPixmap(pix);

    appSplash.show();

    //Auto generate the filenames

    QString appnamestr = applicationName();
    QDir curpath(QDir::currentPath());
    curpath.cdUp();

    QString datalocation = curpath.path();

    _TemplateDir = datalocation + "/templates/";

    _ConfigFile = datalocation + "/config/";
    _ConfigFile += appnamestr + ".ini";

    _Settings.LicenseFile = datalocation + "/license/";
    _Settings.LicenseFile += appnamestr + ".lic";

    QSettings settings(_ConfigFile, QSettings::IniFormat);

    settings.beginGroup("WebService");
    _Settings.Host = settings.value("Host").toString();
    _Settings.Port = settings.value("Port").toString();
    _Settings.User = settings.value("User").toString();
    _Settings.Pass = settings.value("Pass").toString();
    settings.endGroup();

    QObject::connect(&_RestService, SIGNAL(finished(QNetworkReply*)), this, SLOT(eventFinished(QNetworkReply*)));
    QObject::connect(&_RestService, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)), this, SLOT(eventAuthenticationRequired(QNetworkReply*, QAuthenticator*)));

    foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
    {
        if (!(netInterface.flags() & QNetworkInterface::IsLoopBack))
        {
            _SiteId = netInterface.hardwareAddress();
            break;
        }
    }

    checkStartup(_Settings.Host, _Settings.Port, _Settings.User, _Settings.Pass);

    processEvents();

    ApplicationThemeManager.initFontPreference();
    ApplicationThemeManager.applyDarkTheme(this);
}

IPCamDesktop::~IPCamDesktop()
{
}

AppSettings* IPCamDesktop::settings()
{
    return &_Settings;
}

void IPCamDesktop::showMessage(QString str)
{
    appSplash.showMessage(str);
}

void IPCamDesktop::start()
{
    mainWindowPtr->show();
    appSplash.finish(mainWindowPtr);

    connect(this, &IPCamDesktop::cameraAdded, mainWindowPtr->cameras(), &CameraView::eventCameraAdded);
    connect(this, &IPCamDesktop::cameraEdited, mainWindowPtr->cameras(), &CameraView::eventCameraEdited);
    connect(this, &IPCamDesktop::cameraDeleted, mainWindowPtr->cameras(), &CameraView::eventCameraDeleted);

    connect(this, &IPCamDesktop::recordingAdded, mainWindowPtr->recordings(), &RecordingView::eventRecordingAdded);
    connect(this, &IPCamDesktop::recordingEdited, mainWindowPtr->recordings(), &RecordingView::eventRecordingEdited);
    connect(this, &IPCamDesktop::recordingDeleted, mainWindowPtr->recordings(), &RecordingView::eventRecordingDeleted);
}

QString& IPCamDesktop::configFile()
{
    return _ConfigFile;
}

QString& IPCamDesktop::templateDirectory()
{
    return _TemplateDir;
}

QString& IPCamDesktop::licenseFile()
{
    return _Settings.LicenseFile;
}

Camera IPCamDesktop::getCameraById(QString camid)
{
    return _CameraCollection.value(camid);
}

Camera IPCamDesktop::getCameraByName(QString camname)
{
    Camera cam;

    foreach(Camera camtemp, _CameraCollection.values())
    {
        if(camtemp.camera_name == camname)
        {
            return camtemp;
        }
    }

    return cam;
}

bool IPCamDesktop::hasCameraByName(QString camname)
{
    foreach(Camera camtemp, _CameraCollection.values())
    {
        if(camtemp.camera_name == camname)
        {
            return true;
        }
    }

    return false;
}

bool IPCamDesktop::hasCameraById(QString camid)
{
    foreach(Camera camtemp, _CameraCollection.values())
    {
        if(camtemp.camera_id == camid)
        {
            return true;
        }
    }

    return false;
}

QString IPCamDesktop::cameraNameByEndpoint(QString ipaddress, quint32 port)
{
    foreach(Camera cam, _CameraCollection.values())
    {
        if(cam.ip_address == ipaddress && cam.onvif_port == port)
        {
            return cam.camera_name;
        }

        if(cam.ip_address == ipaddress && cam.sd_port == port)
        {
            return cam.camera_name;
        }
    }

    return "";
}

QList<QString> IPCamDesktop::getCameraNames()
{
    return _CameraCollection.keys();
}

QList<Camera> IPCamDesktop::cameraList()
{
    return _CameraCollection.values();
}

bool IPCamDesktop::verifyLicense(QString &licensefile)
{
    _Settings.LicenseFile = licensefile;

    return verifyLicense();
}

bool IPCamDesktop::verifyLicense()
{
    QFile fl(_Settings.LicenseFile);

    bool macok = false;
    bool datok = false;

    if(fl.open(QIODevice::ReadOnly))
    {
        QByteArray ba  = fl.readAll();
        fl.close();

        QString str = QByteArray::fromBase64(ba);

        char arr[28] = {0};

        for(int ctr = str.length()-1, idx = 0; ctr > -1; ctr--, idx++)
        {
            arr[idx] = str.at(ctr).toLatin1();
        }

        str = arr;

        QStringList lst = str.split('-');

        QString dtstr = lst.at(0) + lst.at(2);
        QDate dt = QDate::fromString(dtstr, "yyyyMMdd");
        QString macstring =  lst.at(1);

        if(macstring == "ff:ff:ff:ff:ff:ff")
        {
            saveLicenseFile();
            return true;
        }

        foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
        {
            if (!(netInterface.flags() & QNetworkInterface::IsLoopBack))
            {
                QString hwdstr = netInterface.hardwareAddress();

                if(hwdstr == macstring)
                {
                    macok = true;
                    break;
                }
            }
        }

        if(dt >= QDate::currentDate())
        {
            datok = true;
        }

        if(datok && macok)
        {
            saveLicenseFile();
            return true;
        }
    }

    removeLicenseFile();
    return false;
}

void IPCamDesktop::saveLicenseFile()
{
    QSettings settings(_ConfigFile, QSettings::IniFormat);
    settings.beginGroup("License");
    settings.setValue("LicenceFile", _Settings.LicenseFile);
    settings.endGroup();
    settings.sync();
}

void IPCamDesktop::removeLicenseFile()
{
    _Settings.LicenseFile = "";
    QSettings settings(_ConfigFile, QSettings::IniFormat);
    settings.beginGroup("License");
    settings.setValue("LicenceFile", "");
    settings.endGroup();
    settings.sync();
}

void IPCamDesktop::saveAPIServiceInfo()
{
    QSettings settings(_ConfigFile, QSettings::IniFormat);
    settings.beginGroup("WebService");
    settings.setValue("Host", _Settings.Host);
    settings.setValue("Port", _Settings.Port);
    settings.setValue("User", _Settings.User);
    settings.setValue("Pass", _Settings.Pass);
    settings.endGroup();
    settings.sync();
}

void IPCamDesktop::eventAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    authenticator->setUser(_Settings.User);
    authenticator->setPassword(_Settings.Pass);
}

void IPCamDesktop::eventFinished(QNetworkReply *reply)
{
    QString str;

    QNetworkReply::NetworkError err = reply->error();
    str = reply->readAll();

    QString req_uri = reply->request().url().toString();

    QString request_str = "";

    if(req_uri.contains("camera"))
    {
        request_str = "camera";
    }

    if(req_uri.contains("recording"))
    {
        request_str = "recording";
    }

    if(req_uri.contains("site"))
    {
        request_str = "site";
    }

    if(req_uri.contains("ping"))
    {
        request_str = "ping";
    }

    if(req_uri.contains("detect"))
    {
        request_str = "detect";
    }

    if(req_uri.contains("snapshot"))
    {
        request_str = "snapshot";
    }

   if(err != QNetworkReply::NoError && request_str == "site")
   {
       Site st;
       startupFinished(st, false);
   }

    if(err == QNetworkReply::NoError || str.length() > 0)
    {
        QNetworkAccessManager::Operation op = reply->operation();

        QList<Camera> camlist;
        QList<Site> sitelist;
        QList<Recording> reclist;

        if(request_str == "camera" || request_str == "detect")
        {
            parseCameraList(camlist, str);
        }

        if(request_str == "recording")
        {
            parseRecordingList(reclist, str);
        }

        if(request_str == "site")
        {
            parseSiteList(sitelist, str);
        }

        try
        {
            switch(op)
            {
                case QNetworkAccessManager::GetOperation:
                case QNetworkAccessManager::PostOperation:
                {
                    if(request_str == "camera")
                    {
                        for(auto cam : camlist)
                        {
                            if(!_CameraCollection.contains(cam.camera_id))
                            {
                                _CameraCollection.insert(cam.camera_id, cam);
                                emit cameraAdded(cam);
                            }
                        }
                    }

                    if(request_str == "recording")
                    {
                        for(auto rec : reclist)
                        {
                            emit recordingAdded(rec);
                        }
                    }

                    if(request_str == "site")
                    {
                        Site ste = sitelist.at(0);
                        startupFinished(ste, true);
                    }

                    break;
                }
                case QNetworkAccessManager::PutOperation:
                {
                    if(request_str == "camera")
                    {
                        for(auto cam : camlist)
                        {
                            if(_CameraCollection.contains(cam.camera_id))
                            {
                                _CameraCollection[cam.camera_id] = cam;
                                emit cameraEdited(cam);
                            }
                        }
                    }

                    if(request_str == "recording")
                    {
                        for(auto rec : reclist)
                        {
                            emit recordingEdited(rec);
                        }
                    }

                    break;
                }
                case QNetworkAccessManager::DeleteOperation:
                {
                    if(request_str == "camera")
                    {
                        for(auto cam : camlist)
                        {
                            if(_CameraCollection.contains(cam.camera_id))
                            {
                                _CameraCollection.remove(cam.camera_id);
                                emit cameraDeleted(cam);
                            }
                        }
                    }

                    if(request_str == "recording")
                    {
                        for(auto rec : reclist)
                        {
                            emit recordingDeleted(rec);
                        }
                    }

                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        catch(QException ex)
        {
        }
    }
}

void IPCamDesktop::startupFinished(const Site &site, bool result)
{
    if(!result)
    {
        mainWindowPtr->activateConfigView();
        mainWindowPtr->updateRestServiceStatus(false, site);
        mainWindowPtr->disableSelection();
        QMessageBox msgBox;
        msgBox.setText("Could not connect to IPBuM Web Service. \nPlease specify a valid IP address and port number");
        msgBox.exec();
    }
    else
    {
        start();
        saveAPIServiceInfo();
        mainWindowPtr->updateRestServiceStatus(true, site);
        loadCameraList();
        loadRecordingList();
        mainWindowPtr->activateCameraView();
        mainWindowPtr->enableSelection();
    }
}

void IPCamDesktop::parseSiteList(QList<Site> &sitelist, QString &str)
{
    QStringList rows = str.split('\n', QString::SkipEmptyParts);

    rows.takeFirst();

    for(auto row : rows)
    {
        QStringList attributes = row.split(',', QString::SkipEmptyParts);

        if(attributes.count() == 9)
        {
            Site st;

            st.site_id = attributes.at(0).trimmed();
            st.ip_address = attributes.at(1).trimmed();
            st.port = attributes.at(2).trimmed();
            st.contact_first_name = attributes.at(3).trimmed();
            st.contact_last_name = attributes.at(4).trimmed();
            st.contact_phone_no = attributes.at(5).trimmed();
            st.contact_email_id = attributes.at(6).trimmed();
            st.recording_start = attributes.at(7).trimmed();
            st.recording_end = attributes.at(8).trimmed();

            sitelist.append(st);
        }
    }
}

void IPCamDesktop::parseCameraList(QList<Camera> &camlist, QString &str)
{
    QStringList rows = str.split('\n', QString::SkipEmptyParts);

    rows.takeFirst();

    for(auto row : rows)
    {
        QStringList attributes = row.split(',', QString::SkipEmptyParts);

        if(attributes.count() == 17)
        {
            Camera cam;

            cam.camera_id = attributes.at(0).trimmed();
            cam.camera_name = attributes.at(1).trimmed();
            cam.site_id = attributes.at(2).trimmed();
            cam.ip_address = attributes.at(3).trimmed();
            cam.onvif_port = attributes.at(4).trimmed();
            cam.sd_port = attributes.at(5).trimmed();
            cam.user_id = attributes.at(6).trimmed();
            cam.password = attributes.at(7).trimmed();
            cam.is_active = attributes.at(8).trimmed();
            cam.last_accessed = attributes.at(9).trimmed();
            cam.last_file_timestamp = attributes.at(10).trimmed();
            cam.last_dir_timestamp = attributes.at(11).trimmed();
            cam.download_required = attributes.at(12).trimmed();
            cam.slot_1 = attributes.at(13).trimmed();
            cam.slot_2 = attributes.at(14).trimmed();
            cam.slot_3 = attributes.at(15).trimmed();
            cam.slot_4 = attributes.at(16).trimmed();

            camlist.append(cam);
        }
    }
}

void IPCamDesktop::parseRecordingList(QList<Recording> &reclist, QString &str)
{
    QStringList rows = str.split('\n', QString::SkipEmptyParts);

    rows.takeFirst();

    for(auto row : rows)
    {
        QStringList attributes = row.split(',', QString::SkipEmptyParts);

        if(attributes.count() == 9)
        {
            Recording rec;

            rec.site_id = attributes.at(0).trimmed();
            rec.request_id = attributes.at(1).trimmed();
            rec.camera_id = attributes.at(2).trimmed();
            rec.camera_file_uri = attributes.at(3).trimmed();
            rec.storage_file_uri = attributes.at(4).trimmed();
            rec.request_timestamp = attributes.at(5).trimmed();
            rec.request_status = attributes.at(6).trimmed();
            rec.file_size = attributes.at(7).trimmed();
            rec.file_timestamp = attributes.at(8).trimmed();

            reclist.append(rec);
        }
    }
}

void IPCamDesktop::loadCameraList()
{
    QString url = "http://" + _Settings.Host + ":" +  _Settings.Port + "/camera/";
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/csv");

    QString concatenated = "IPCamDesktop:IPCamDesktop";
    QByteArray data = concatenated.toLocal8Bit().toBase64();
    QString headerData = "Basic " + data;
    request.setRawHeader("Authorization", headerData.toLocal8Bit());

    _RestService.get(request);
}

void IPCamDesktop::loadRecordingList()
{
    QString url = "http://" + _Settings.Host + ":" +  _Settings.Port + "/recording/";
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/csv");

    QString concatenated = "IPCamDesktop:IPCamDesktop";
    QByteArray data = concatenated.toLocal8Bit().toBase64();
    QString headerData = "Basic " + data;
    request.setRawHeader("Authorization", headerData.toLocal8Bit());

    _RestService.get(request);
}

void IPCamDesktop::checkStartup(QString ipaddress, QString port, QString user, QString pass)
{
    _Settings.Host = ipaddress;
    _Settings.Port = port;
    _Settings.User = user;
    _Settings.Pass = pass;

    QString resturi = "http://" + ipaddress + ":" + port + "/site";

    QNetworkRequest request;
    request.setUrl(QUrl(resturi));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/csv");

    QString concatenated = "IPCamDesktop:IPCamDesktop";
    QByteArray data = concatenated.toLocal8Bit().toBase64();
    QString headerData = "Basic " + data;
    request.setRawHeader("Authorization", headerData.toLocal8Bit());

    _RestService.get(request);
}

void IPCamDesktop::saveSiteConfiguration(Site st)
{
    QString csvData = "";

    csvData += "site_id,ip_address,port,contact_first_name,contact_last_name,contact_phone_no,contact_email_id,recording_start,recording_end\n";

    csvData += st.site_id + ",";
    csvData += st.ip_address + ",";
    csvData += st.port + ",";
    csvData += st.contact_first_name + ",";
    csvData += st.contact_last_name + ",";
    csvData += st.contact_phone_no + ",";
    csvData += st.contact_email_id + ",";
    csvData += st.recording_start + ",";
    csvData += st.recording_end + "\n";

    QString url = "http://" + _Settings.Host + ":" +  _Settings.Port + "/site/" + st.site_id;
    QByteArray buffer((const char*)csvData.toStdString().c_str(), csvData.toStdString().length());
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/csv");

    QString concatenated = "IPCamDesktop:IPCamDesktop";
    QByteArray data = concatenated.toLocal8Bit().toBase64();
    QString headerData = "Basic " + data;
    request.setRawHeader("Authorization", headerData.toLocal8Bit());

    _RestService.put(request, buffer);
}

void IPCamDesktop::updateRecording(Recording &dnlstruct)
{
    QString csvData;

    csvData += "site_id,request_id,camera_id,camera_file_uri,storage_file_uri,request_timestamp,request_status,file_size,file_timestamp\n";

    csvData += dnlstruct.site_id + ",";
    csvData += dnlstruct.request_id + ",";
    csvData += dnlstruct.camera_id + ",";
    csvData += dnlstruct.camera_file_uri + ",";
    csvData += dnlstruct.storage_file_uri + ",";
    csvData += dnlstruct.request_timestamp + ",";
    csvData += dnlstruct.request_status + ",";
    csvData += dnlstruct.file_size + ",";
    csvData += dnlstruct.file_timestamp + "\n";

    QString url = "http://" + _Settings.Host + ":" +  _Settings.Port + "/downloadrequest/";
    QByteArray buffer((const char*)csvData.toStdString().c_str(), csvData.toStdString().length());
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/csv");

    QString concatenated = "IPCamDesktop:IPCamDesktop";
    QByteArray data = concatenated.toLocal8Bit().toBase64();
    QString headerData = "Basic " + data;
    request.setRawHeader("Authorization", headerData.toLocal8Bit());

    _RestService.post(request, buffer);
}

bool IPCamDesktop::addCamera(Camera device)
{
    if(_CameraCollection.contains(device.camera_id) || hasCameraByName(device.camera_name))
    {
        return false;
    }

    QString csvData;

    csvData += "camera_id,camera_name,site_id,ip_address,onvif_port,sd_port,user_id,password,is_active,last_accessed,last_file_timestamp,last_dir_timestamp,download_required,slot_1,slot_2,slot_3,slot_4\n";

    csvData += device.camera_id + ",";
    csvData += device.camera_name + ",";
    csvData += device.site_id + ",";
    csvData += device.ip_address + ",";
    csvData += device.onvif_port + ",";
    csvData += device.sd_port + ",";
    csvData += device.user_id + ",";
    csvData += device.password + ",";
    csvData += device.is_active + ",";
    csvData += device.last_accessed + ",";
    csvData += device.last_file_timestamp + ",";
    csvData += device.last_dir_timestamp + ",";
    csvData += device.download_required + ",";
    csvData += device.slot_1 + ",";
    csvData += device.slot_2 + ",";
    csvData += device.slot_3 + ",";
    csvData += device.slot_4 + "\n";

    QString url = "http://" + _Settings.Host + ":" +  _Settings.Port + "/camera/";
    QByteArray buffer((const char*)csvData.toStdString().c_str(), csvData.toStdString().length());
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/csv");

    QString concatenated = "IPCamDesktop:IPCamDesktop";
    QByteArray data = concatenated.toLocal8Bit().toBase64();
    QString headerData = "Basic " + data;
    request.setRawHeader("Authorization", headerData.toLocal8Bit());

    _RestService.post(request, buffer);

    return true;
}

bool IPCamDesktop::updateCamera(Camera device)
{
    if(_CameraCollection.contains(device.camera_id))
    {
        QString csvData;

        csvData += "camera_id,camera_name,site_id,ip_address,onvif_port,sd_port,user_id,password,is_active,last_accessed,last_file_timestamp,last_dir_timestamp,download_required,slot_1,slot_2,slot_3,slot_4\n";

        csvData += device.camera_id + ",";
        csvData += device.camera_name + ",";
        csvData += device.site_id + ",";
        csvData += device.ip_address + ",";
        csvData += device.onvif_port + ",";
        csvData += device.sd_port + ",";
        csvData += device.user_id + ",";
        csvData += device.password + ",";
        csvData += device.is_active + ",";
        csvData += device.last_accessed + ",";
        csvData += device.last_file_timestamp + ",";
        csvData += device.last_dir_timestamp + ",";
        csvData += device.download_required + ",";
        csvData += device.slot_1 + ",";
        csvData += device.slot_2 + ",";
        csvData += device.slot_3 + ",";
        csvData += device.slot_4 + "\n";

        QString url = "http://" + _Settings.Host + ":" +  _Settings.Port + "/camera/" + device.camera_id;
        QByteArray buffer((const char*)csvData.toStdString().c_str(), csvData.toStdString().length());
        QNetworkRequest request;
        request.setUrl(QUrl(url));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "text/csv");

        QString concatenated = "IPCamDesktop:IPCamDesktop";
        QByteArray data = concatenated.toLocal8Bit().toBase64();
        QString headerData = "Basic " + data;
        request.setRawHeader("Authorization", headerData.toLocal8Bit());

        _RestService.put(request, buffer);

        return true;
    }

    return false;
}

void IPCamDesktop::removeCamera(QString camid)
{
    if(_CameraCollection.contains(camid))
    {
        QString url = "http://" + _Settings.Host + ":" +  _Settings.Port + "/camera/" + camid;
        QNetworkRequest request;
        request.setUrl(QUrl(url));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "text/csv");
        _RestService.deleteResource(request);
    }
}

void IPCamDesktop::removeAllCameras()
{
    QList<QString> camlist = _CameraCollection.keys();

    foreach (QString cam, camlist)
    {
        removeCamera(cam);
    }
}
