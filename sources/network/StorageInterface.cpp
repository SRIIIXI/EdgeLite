#include "StorageInterface.h"

StorageInterface::StorageInterface(QObject *parent)
    : QObject(parent)
{
    setUpEventHandlers();
}

StorageInterface::StorageInterface(QString server, quint32 port, QString user, QString password, QObject* parent)
    : QObject(parent)
{
    IpAddress = server;
    ServicePort = port;
    ServiceUser = user;
    Password = password;
    setUpEventHandlers();
}

void StorageInterface::setServer(QString str)
{
    IpAddress = str;
}

void StorageInterface::setPort(quint32 num)
{
    ServicePort = num;
}

void StorageInterface::setUser(QString str)
{
    ServiceUser = str;
}

void StorageInterface::setPassword(QString str)
{
    Password = str;
}

void StorageInterface::setStorageDirectory(QString str)
{
    _StorageDirectory = str;
}

QString StorageInterface::storageUrl()
{
    return _StorageUrl;
}

void StorageInterface::setUpEventHandlers()
{
    QObject::connect(&_NetAccess, SIGNAL(finished(QNetworkReply*)), this, SLOT(eventFinished(QNetworkReply*)));
    QObject::connect(&_NetAccess, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)), this, SLOT(eventAuthenticationRequired(QNetworkReply*, QAuthenticator*)));
}

void StorageInterface::detectStorage()
{
    _StorageUrl = "http://" + IpAddress + ":" + QVariant(ServicePort).toString() + "/sd/";

    QNetworkRequest request;
    request.setUrl(QUrl(_StorageUrl));
    _NetAccess.head(request);
}

void StorageInterface::getAllRecordingSets()
{
    _StorageUrl = "http://" + IpAddress + ":" + QVariant(ServicePort).toString() + "/sd/";

    QNetworkRequest request;
    request.setUrl(QUrl(_StorageUrl));
    request.setAttribute(QNetworkRequest::User, QVariant(DirRoot));
    _NetAccess.get(request);
}

void StorageInterface::getDailyRecordingSets(StorageDirectory &stgdir)
{
    QString dirurl = "http://" + IpAddress + ":" + QVariant(ServicePort).toString() + "/sd/";
    dirurl += stgdir.DirectoryName + "/";

    QNetworkRequest request;
    request.setUrl(QUrl(dirurl));
    request.setAttribute(QNetworkRequest::User, QVariant(DirDailySet));
    _NetAccess.get(request);
}

void StorageInterface::getRecordingSets(DailyRecordingSet &stgdir)
{
    QString dirurl = "http://" + IpAddress + ":" + QVariant(ServicePort).toString() + "/sd/";
    dirurl += stgdir.DirectoryName + "/";
    dirurl += stgdir.SetName + "/";

    QNetworkRequest request;
    request.setUrl(QUrl(dirurl));
    request.setAttribute(QNetworkRequest::User, QVariant(DirRecords));
    _NetAccess.get(request);
}

void StorageInterface::eventAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    authenticator->setUser(ServiceUser);
    authenticator->setPassword(Password);
}

void StorageInterface::eventFinished(QNetworkReply *reply)
{
    QNetworkReply::NetworkError err = reply->error();

    if(err == QNetworkReply::NoError || err == QNetworkReply::HostNotFoundError)
    {           
        if(reply->operation() == QNetworkAccessManager::HeadOperation)
        {
            emit storageDetectionSuceeded(this);

            return;
        }

        QString str = reply->readAll();

        if(reply->request().attribute(QNetworkRequest::User, QVariant(DirRoot)) == DirRoot)
        {
            _StgDirList.clear();

            processRootDirectory(str, _StgDirList);

            emit rootDirectoryReceived(_StgDirList);

            return;
        }

        if(reply->request().attribute(QNetworkRequest::User, QVariant(DirRoot)) == DirDailySet)
        {
            _DailyRecDirList.clear();

            processDailyRecordSetDirectory(str, _DailyRecDirList, reply->request().url().toString());

            emit recordsDirectoryReceived(_DailyRecDirList);

            return;
        }

        if(reply->request().attribute(QNetworkRequest::User, QVariant(DirRoot)) == DirRecords)
        {
            _Recordist.clear();

            processRecordSetDirectory(str, _Recordist, reply->request().url().toString());

            emit recordingsReceived(_Recordist);

            return;
        }
    }
    else
    {
        if(reply->operation() == QNetworkAccessManager::HeadOperation || reply->request().attribute(QNetworkRequest::User, QVariant(DirRoot)) == DirRoot)
        {
            emit storageDetectionFailed(this);
        }
    }
}

void StorageInterface::processRootDirectory(QString &urlContent, QList<StorageDirectory> &dlist)
{
    urlContent = urlContent.remove("&nbsp");
    urlContent = urlContent.remove("</tr>");
    urlContent = urlContent.remove("</td>");
    urlContent = urlContent.remove("[DIRECTORY]");
    urlContent = urlContent.remove("<td>");
    urlContent = urlContent.remove("<tr>");
    urlContent = urlContent.remove("<a href=\"/sd/");
    urlContent = urlContent.remove("</a");
    urlContent = urlContent.remove("</table></body></html>");
    urlContent = urlContent.remove("<table>");
    urlContent = urlContent.remove("<body>");
    urlContent = urlContent.remove("<html>");
    urlContent = urlContent.remove("<th>");
    urlContent = urlContent.remove("</th>");
    urlContent = urlContent.remove("<a>");
    urlContent = urlContent.remove("\">");
    urlContent = urlContent.remove(';');
    urlContent = urlContent.remove('>');

    int pos = urlContent.indexOf("--") + 2;
    urlContent = urlContent.remove(0, pos);

    QStringList dirs = QString(urlContent).split('\n', QString::SkipEmptyParts);

    for(int x = 0; x < dirs.count(); x++)
    {
        StorageDirectory stg;

        QStringList tokens = dirs.at(x).split('/', QString::SkipEmptyParts);

        stg.TimeStamp = QDateTime::fromString(tokens.at(2)+":00", "dd-MMM-yyyy hh:mm:ss");
        stg.DirectoryName = tokens.at(1);

        dlist.append(stg);
    }
}

void StorageInterface::processDailyRecordSetDirectory(QString &urlContent, QList<DailyRecordingSet> &rlist, QString parentdir)
{
    urlContent = urlContent.remove("&nbsp");
    urlContent = urlContent.remove("</tr>");
    urlContent = urlContent.remove("</td>");
    urlContent = urlContent.remove("[DIRECTORY]");
    urlContent = urlContent.remove("<td>");
    urlContent = urlContent.remove("<tr>");
    urlContent = urlContent.remove("<a href=\"/sd/");
    urlContent = urlContent.remove("</a");
    urlContent = urlContent.remove("</table></body></html>");
    urlContent = urlContent.remove("<table>");
    urlContent = urlContent.remove("<body>");
    urlContent = urlContent.remove("<html>");
    urlContent = urlContent.remove("<th>");
    urlContent = urlContent.remove("</th>");
    urlContent = urlContent.remove("<a>");
    urlContent = urlContent.remove("\">");
    urlContent = urlContent.remove(';');
    urlContent = urlContent.remove('>');

    int pos = urlContent.indexOf("--") + 2;
    urlContent = urlContent.remove(0, pos);

    QStringList recs = QString(urlContent).split('\n', QString::SkipEmptyParts);

    for(int x = 0; x < recs.count(); x++)
    {
        if(recs.at(x).contains(".db"))
        {
            recs.removeAt(x);
        }
    }

    for(int x = 0; x < recs.count(); x++)
    {
        DailyRecordingSet set;

        QStringList tokens = recs.at(x).split('/', QString::SkipEmptyParts);

        set.TimeStamp = QDateTime::fromString(tokens.at(3)+":00", "dd-MMM-yyyy hh:mm:ss");
        set.DirectoryName = tokens.at(0);
        set.SetName = tokens.at(1);
        set.ParentDirectory = parentdir;

        rlist.append(set);
    }
}

void StorageInterface::processRecordSetDirectory(QString &urlContent, QList<RecordingSet> &clist, QString parentdir)
{
    urlContent = urlContent.remove("&nbsp");
    urlContent = urlContent.remove("</tr>");
    urlContent = urlContent.remove("</td>");
    urlContent = urlContent.remove("[DIRECTORY]");
    urlContent = urlContent.remove("<td>");
    urlContent = urlContent.remove("<tr>");
    urlContent = urlContent.remove("<a href=\"/sd/");
    urlContent = urlContent.remove("</a");
    urlContent = urlContent.remove("</table></body></html>");
    urlContent = urlContent.remove("<table>");
    urlContent = urlContent.remove("<body>");
    urlContent = urlContent.remove("<html>");
    urlContent = urlContent.remove("<th>");
    urlContent = urlContent.remove("</th>");
    urlContent = urlContent.remove("<a>");
    urlContent = urlContent.remove('>');

    int pos = 0;

    pos = urlContent.indexOf("--") + 2;
    urlContent = urlContent.remove(0, pos);

    urlContent = urlContent.replace('"', '/');
    urlContent = urlContent.replace(';', '/');

    pos = urlContent.indexOf("-//-") + 4;
    urlContent = urlContent.remove(0, pos);

    QStringList recfiles = QString(urlContent).split('\n', QString::SkipEmptyParts);

    for(int x = 0; x < recfiles.count(); x++)
    {
        RecordingSet rcs;

        QStringList tokens = recfiles.at(x).split('/', QString::SkipEmptyParts);

        rcs.TimeStamp = QDateTime::fromString(tokens.at(4)+":00", "dd-MMM-yyyy hh:mm:ss");
        rcs.Name = tokens.at(2);
        rcs.Size = tokens.at(5);
        rcs.ParentDirectory = parentdir;

        clist.append(rcs);
    }

}

QString StorageInterface::storagePath(QString str)
{
    QUrl url(str);

    QStringList tlist = str.split('/', QString::SkipEmptyParts);

    QString dirpath = "";
    dirpath += url.host() + "-";
    dirpath += QVariant(url.port()).toString() + "/";
    dirpath += tlist.at(tlist.count() - 3) + "/";

    return dirpath;
}

QString StorageInterface::server()
{
    return IpAddress;
}

quint32 StorageInterface::port()
{
    return ServicePort;
}
