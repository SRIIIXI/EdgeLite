#ifndef _TRACKS_H
#define _TRACKS_H

#include <QtCore>
#include <QObject>
#include <QStringBuilder>
#include <QUrlQuery>
#include <QDebug>
#include <QList>
#include <QPair>
#include <QMap>
#include <QHostAddress>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMutex>
#include <QSplashScreen>
#include "ServiceInterface.h"
#include "AppSettings.h"

class Tracks : public QApplication
{
    Q_OBJECT
public:
    explicit Tracks(int argc, char *argv[]);
    virtual ~Tracks();

    void start();
    void showMessage(QString str);

    QString& configFile();
    QString& templateDirectory();
    QString& recordingsDirectory();
    void setRecordingsDirectory(QString str);

    bool addCamera(ONVIFCamera device);
    void removeCamera(QString camname);
    void removeAllCameras();
    ONVIFCamera getCamera(QString camname);
    QList<QString> getCameraNames();
    QList<ONVIFCamera> cameraList();
    AppSettings* settings();
    QString cameraNameByEndpoint(QString ipaddress, quint32 port);

    void saveDatabaseInfo();
    void saveDownloadLocation();
    void saveStyleOption();
private:
    bool copyRecursively(const QString &srcFilePath,  const QString &tgtFilePath);
    void retrieveCameras(QList<QString> &camlist, QSettings &settings);

    QString _TemplateDir, _ConfigFile;
    QSplashScreen appSplash;

    QMap<QString, ONVIFCamera> _CameraCollection;
    AppSettings _Settings;
};

extern Tracks* tracksPtr;

#endif
