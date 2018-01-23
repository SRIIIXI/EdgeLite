#include "DownloadManager.h"
#include "ServiceInterface.h"

static DownloadManager *downloadManagerPtr = NULL;

DownloadManager::DownloadManager(QObject *parent) : QObject(parent)
{
    downloadedCount = 0;
    totalCount = 0;
    connect(&manager,SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),SLOT(authenticationRequired(QNetworkReply*,QAuthenticator*)));
    connect(&manager,SIGNAL(proxyAuthenticationRequired(QNetworkProxy,QAuthenticator*)),this,SLOT(proxyAuthenticationRequired(QNetworkProxy,QAuthenticator*)));

}

DownloadManager* DownloadManager::GetInstance()
{
    if(downloadManagerPtr == NULL)
    {
        downloadManagerPtr = new DownloadManager();
    }

    return downloadManagerPtr;
}

void DownloadManager::reset()
{
    downloadedCount = 0;
    totalCount = 0;
    pendinglist.clear();
}

void DownloadManager::disableProxy()
{
    proxy.setType(QNetworkProxy::NoProxy);
    proxy.setHostName("");
    proxy.setPort(-1);
    proxy.setUser("");
    proxy.setPassword("");
    manager.setProxy(proxy);
}

void DownloadManager::setProxy(QString host, int port, QString userid, QString passwd)
{
    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName(host);
    proxy.setPort(port);
    proxy.setUser(userid);
    proxy.setPassword(passwd);
    manager.setProxy(proxy);
}

void DownloadManager::setUser(QString str)
{
    ServiceUser = str;
}

void DownloadManager::setPassword(QString str)
{
    Password = str;
}

void DownloadManager::setDownloadDirectory(QString str)
{
    DownloadDirectory = str;
}

void DownloadManager::append(const QStringList &urlList)
{
    foreach (QString url, urlList)
        append(QUrl::fromEncoded(url.toLocal8Bit()));

    if (downloadQueue.isEmpty())
        QTimer::singleShot(0, this, SIGNAL(finished()));
}

void DownloadManager::append(const QUrl &url)
{
    if (downloadQueue.isEmpty())
        QTimer::singleShot(0, this, SLOT(startNextDownload()));

    downloadQueue.enqueue(url);

    QString upath = url.toString();
    upath = upath.remove("http://");
    upath = upath.remove(url.host());

    pendinglist.append(upath);

    ++totalCount;
}

void DownloadManager::startNextDownload()
{
    if (downloadQueue.isEmpty())
    {
        emit finished();
        return;
    }

    output.clear();

    QUrl url = downloadQueue.dequeue();

    QNetworkRequest request(url);
    currentDownload = manager.get(request);

    connect(currentDownload, SIGNAL(finished()),
            SLOT(downloadFinished()));
    connect(currentDownload, SIGNAL(readyRead()),
            SLOT(downloadReadyRead()));

    downloadTime.start();
}

void DownloadManager::downloadFinished()
{
    if (currentDownload->error())
    {
        // download failed
        qDebug() << "Failed: " << qPrintable(currentDownload->errorString());
    }
    else
    {
        ++downloadedCount;

        QStringList ls = currentDownload->url().toString().split('/', QString::SkipEmptyParts);

        QString dirpart = DownloadDirectory + "/";

        dirpart += "/";

        QDir dir(dirpart);

        if(!dir.exists())
        {
            if(dir.mkpath(dirpart))
            {
                QString flname = dirpart + ls.at(ls.count() - 1);

                QFile fl(flname);

                if(fl.open(QIODevice::WriteOnly))
                {
                    fl.write(currentDownload->readAll());
                    fl.flush();
                    fl.close();
                }
            }
        }

        QString urlResource = currentDownload->url().toString();
        urlResource = urlResource.remove("http://");
        urlResource = urlResource.remove(currentDownload->url().host());

        pendinglist.removeOne(urlResource);
        emit completed(output, urlResource);
    }

    currentDownload->deleteLater();
    startNextDownload();
}

void DownloadManager::downloadReadyRead()
{
    output.append(currentDownload->readAll());
}

void DownloadManager::authenticationRequired(QNetworkReply* reply,QAuthenticator* ath)
{
    ath->setUser(ServiceUser);
    ath->setPassword(Password);
}

void DownloadManager::proxyAuthenticationRequired(const QNetworkProxy &proxyRef,QAuthenticator* auth)
{
    auth->setUser(proxy.user());
    auth->setPassword(proxy.password());

    QNetworkProxy p(proxyRef);
    p.setUser(proxy.user());
    p.setPassword(proxy.password());
    QNetworkProxy::setApplicationProxy(p);
}

bool DownloadManager::imageIsLoading(QString url)
{
    return pendinglist.contains(url);
}

