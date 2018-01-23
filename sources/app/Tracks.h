#ifndef _TRACKS
#define _TRACKS

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

    bool addCamera(ServiceInterface* device);
    void removeCamera(QString camname);
    ServiceInterface* getServiceInterface(QString camname);
    QList<QString> cameraCollection();
    QList<ServiceInterface*> devices();
    AppSettings* settings();

private:
    bool copyRecursively(const QString &srcFilePath,  const QString &tgtFilePath);
    QString _TemplateDir, _ConfigFile;
    QSplashScreen appSplash;

    QMap<QString, ServiceInterface*> _CameraCollection;
    AppSettings _Settings;
};

extern Tracks* tracksPtr;

#endif
