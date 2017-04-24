#ifndef _SMTP_CLIENT
#define _SMTP_CLIENT

#include <QObject>
#include <QSslSocket>
#include <QTcpSocket>
#include "SecurityTypes.h"
#include "Mail.h"

class SmtpClient : public QObject
{
    Q_OBJECT
public:
    SmtpClient();
    SmtpClient(const QString &host, quint16 port, QString username, QString password, char sectype, bool sendhelo = false);
    ~SmtpClient();
    bool connect(const QString &host, quint16 port, QString username, QString password, char sectype, bool sendhelo = false);
    bool connect();
    void setPublicIpAddress(QString str);
    bool sendMail(MailHeader &ehdr, MailBody &ebdy);
    void close();
    QString account();

signals:
    void mailDeliveryStatus(QString uname, QString messageId, bool state);
    void authenticated(QString uname);
    void authenticationFailure(QString uname, QString pass);

private slots:
    void eventConnected();
    void eventDataReceived();

private:
    void continuetransmission();
    void authenticate();
    QString encodeBase64(QString string);
    QString decodeBase64(QString string);

private:
    QTcpSocket *_Socket;
    QString _Host;
    QString _Username;
    QString _Password;
    quint16 _Port;
    QString _Buffer;
    int _SendStep;
    MailHeader _EmlHdr;
    MailBody _EmlBdy;
    QString _PublicIpAddress;
    bool _SendHelo;
    char _SecurityType;
    int _RcPtCount;
};

#endif
