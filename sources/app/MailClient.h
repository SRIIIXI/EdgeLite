#ifndef MAIL_CLIENT
#define MAIL_CLIENT

#include <QtCore>
#include <QObject>
#include <QStringBuilder>
#include <QUrlQuery>
#include <QDebug>
#include <QList>
#include <QPair>
#include <QHostAddress>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMutex>
#include <QSplashScreen>
#include "ImapClient.h"
#include "SmtpClient.h"
#include "Mail.h"

class MailClient : public QApplication
{
    Q_OBJECT
public:
    explicit MailClient(int argc, char *argv[]);
    virtual ~MailClient();
    bool addressResolved();
    QString publicIpAddress();
    void startOffline();
    void startOnline();
    bool isOffline();
    void showMessage(QString str);

private slots:
    void addressResolutionCompleted(QNetworkReply* reply);

private:
    void resolveAddress();
    QString _IpAddress;
    bool    _AddressResolved;
    QNetworkAccessManager* _NetAccess;
    QMutex _Mx;
    bool _IsOffline;
    QSplashScreen appSplash;
};

extern MailClient* mailClientPtr;

#endif
