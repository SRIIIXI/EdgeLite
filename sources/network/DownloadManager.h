#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QFile>
#include <QObject>
#include <QQueue>
#include <QTime>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QAuthenticator>
#include <QNetworkProxy>
#include <QAbstractNetworkCache>
#include <QNetworkDiskCache>
#include <QFileInfo>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>
#include <QStringList>
#include <QTimer>

class DownloadManager: public QObject
{
    Q_OBJECT
private:
    DownloadManager(QObject *parent = 0);
public:
    void setProxy(QString host, int port, QString userid, QString passwd);
    void disableProxy();
    void setUser(QString str);
    void setPassword(QString str);
    void setDownloadDirectory(QString str);
    void append(const QUrl &url);
    void append(const QStringList &urlList);
    static DownloadManager* GetInstance();
    bool imageIsLoading(QString url);
    void reset();
signals:
    void completed(QByteArray pm, QString urlresource);
    void finished();

private slots:
    void startNextDownload();
    void downloadFinished();
    void downloadReadyRead();

    void authenticationRequired(QNetworkReply* reply,QAuthenticator* ath);
    void proxyAuthenticationRequired(const QNetworkProxy &proxyRef,QAuthenticator* auth);

private:
    QNetworkAccessManager manager;
    QNetworkProxy proxy;
    QQueue<QUrl> downloadQueue;
    QNetworkReply *currentDownload;
    QByteArray output;
    QTime downloadTime;
    QStringList pendinglist;
    int downloadedCount;
    int totalCount;

    QString ServiceUser;
    QString Password;
    QString DownloadDirectory;
};


#endif
