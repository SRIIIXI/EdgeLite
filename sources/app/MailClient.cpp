#include "MailClient.h"
#include "MainWindow.h"

MailClient* mailClientPtr = nullptr;

MailClient::MailClient(int argc, char *argv[]) : QApplication (argc, argv)
{
    _IsOffline = false;
    _AddressResolved = false;
    mailClientPtr = this;
    QPixmap pix(":images/splashscreen.png");
    appSplash.setPixmap(pix);

    appSplash.show();
    processEvents();
}

MailClient::~MailClient()
{
}

void MailClient::showMessage(QString str)
{
    appSplash.showMessage(str);
}

void MailClient::startOnline()
{
    _IsOffline = true;
    resolveAddress();
    appSplash.showMessage("Querying public IP address");
}

void MailClient::startOffline()
{
    _IsOffline = false;
    mainWindowPtr->show();
    appSplash.finish(mainWindowPtr);
}

bool MailClient::isOffline()
{
    return _IsOffline;
}

void MailClient::addressResolutionCompleted(QNetworkReply* reply)
{
    _IpAddress = reply->readAll();
    mainWindowPtr->show();
    appSplash.finish(mainWindowPtr);
}

void MailClient::resolveAddress()
{
    _NetAccess = new QNetworkAccessManager();
    connect(_NetAccess, SIGNAL(finished(QNetworkReply*)), this, SLOT(addressResolutionCompleted(QNetworkReply*)));
    QString str = "https://api.ipify.org/";
    QUrl url(str);
    QNetworkReply* reply = _NetAccess->get(QNetworkRequest(url));
}

bool MailClient::addressResolved()
{
    return _AddressResolved;
}

QString MailClient::publicIpAddress()
{
    return _IpAddress;
}
