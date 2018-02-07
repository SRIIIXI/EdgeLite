#ifndef STORAGE_INTERFACE_H
#define STORAGE_INTERFACE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QtXml/QDomDocument>
#include <QException>
#include <QFile>
#include <QUdpSocket>
#include <QHostInfo>
#include <QNetworkAddressEntry>
#include <QTextDocument>
#include "RecordingStructures.h"

class StorageInterface : public QObject
{
    Q_OBJECT
public:
    StorageInterface(QObject* parent = nullptr);
    StorageInterface(QString server, quint32 port, QString user, QString password, QObject* parent = nullptr);

    void setServer(QString str);
    void setPort(quint32 num);
    void setUser(QString str);
    void setPassword(QString str);
    void setStorageDirectory(QString str);

    QString server();
    quint32 port();

    void getAllRecordingSets();
    void getDailyRecordingSets(StorageDirectory &stgdir);
    void getRecordingSets(DailyRecordingSet &stgdir);
    void detectStorage();

    QString storageUrl();

    static QString storagePath(QString str);

signals:
    void rootDirectoryReceived(QList<StorageDirectory> rootdlist);
    void recordsDirectoryReceived(QList<DailyRecordingSet> recdlist);
    void recordingsReceived(QList<RecordingSet> recdlist);
    void storageDetectionFailed(StorageInterface* cam);
    void storageDetectionSuceeded(StorageInterface* cam);

private slots:
    void eventFinished(QNetworkReply *reply);
    void eventAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);

private:
    void setUpEventHandlers();
    void processRootDirectory(QString &urlContent, QList<StorageDirectory> &dlist);
    void processDailyRecordSetDirectory(QString &urlContent, QList<DailyRecordingSet> &rlist, QString parentdir);
    void processRecordSetDirectory(QString &urlContent, QList<RecordingSet> &rlist, QString parentdir);


    QString _StorageDirectory;
    QNetworkAccessManager _NetAccess;
    QString _StorageUrl;
    QString IpAddress;
    quint32 ServicePort;
    QString ServiceUser;
    QString Password;

    QList<StorageDirectory> _StgDirList;
    QList<DailyRecordingSet> _DailyRecDirList;
    QList<RecordingSet> _Recordist;
};

#endif // STORAGE_INTERFACE_H
