#ifndef _IPBUM_H
#define _IPBUM_H

#include <QtCore>
#include <QObject>
#include <QStringBuilder>
#include <QUrlQuery>
#include <QList>
#include <QPair>
#include <QMap>
#include <QHostAddress>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QApplication>
#include <QNetworkAccessManager>
#include <QAuthenticator>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSplashScreen>
#include "AppSettings.h"
#include "Recording.h"
#include "Camera.h"
#include "Site.h"

class IPCamDesktop : public QApplication
{
    Q_OBJECT
public:
    explicit IPCamDesktop(int argc, char *argv[]);
    virtual ~IPCamDesktop();

    void start();
    void showMessage(QString str);
    void checkStartup(QString ipaddress, QString port, QString user, QString pass);

    QString& configFile();
    QString& templateDirectory();
    QString& licenseFile();
    QString activeIpAddress();

    bool addCamera(Camera device);
    bool updateCamera(Camera device);
    void removeCamera(QString camid);
    void removeAllCameras();
    bool hasCameraByName(QString camname);
    bool hasCameraById(QString camid);

    Camera getCameraById(QString camid);
    Camera getCameraByName(QString camname);
    QList<Camera> cameraList();
    QList<QString> getCameraNames();
    QString cameraNameByEndpoint(QString ipaddress, quint32 port);

    AppSettings* settings();

    bool verifyLicense(QString &licensefile);
    bool verifyLicense();
    void saveLicenseFile();
    void removeLicenseFile();
    void saveAPIServiceInfo();

    void updateRecording(Recording &dnlstruct);
    void loadCameraList();
    void loadRecordingList();

    void saveSiteConfiguration(Site st);

signals:
    void cameraAdded(Camera &cam);
    void cameraEdited(Camera &cam);
    void cameraDeleted(Camera &cam);
    void cameraReceived(Camera &cam);

    void recordingAdded(Recording &dnl);
    void recordingEdited(Recording &dnl);
    void recordingDeleted(Recording &dnl);
    void recordingReceived(Recording &dnl);

    void siteAdded(Site &st);
    void siteEdited(Site &st);
    void siteDeleted(Site &st);
    void siteReceived(Site &st);


private slots:
    void eventAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);
    void eventFinished(QNetworkReply *reply);

private:
    void cameraListReceived(QStringList &objarray, QNetworkAccessManager::Operation op);
    void cameraReceived(QStringList &objarray, QNetworkAccessManager::Operation op);

    void downloadRequestListReceived(QStringList &objarray, QNetworkAccessManager::Operation op);
    void recordingReceived(QStringList &objarray, QNetworkAccessManager::Operation op);

    void parseSiteList(QList<Site> &sitelist, QString &str);
    void parseCameraList(QList<Camera> &camlist, QString &str);
    void parseRecordingList(QList<Recording> &reclist, QString &str);

    void startupFinished(const Site &site, bool result);

    QString _TemplateDir, _ConfigFile, _SiteId;
    QSplashScreen appSplash;

    QMap<QString, Camera> _CameraCollection;
    AppSettings _Settings;

    QNetworkAccessManager _RestService;
};

extern IPCamDesktop* applicationPtr;

#endif
