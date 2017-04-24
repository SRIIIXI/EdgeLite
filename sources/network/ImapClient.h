#ifndef _IMAP_CLIENT
#define _IMAP_CLIENT

#include <QObject>
#include <QSslSocket>
#include <QTcpSocket>
#include "SecurityTypes.h"
#include "Mail.h"

class ImapClient : public QObject
{
    Q_OBJECT

public:
    ImapClient();
    ImapClient(const QString &host, quint16 port, QString username, QString password, char sectype);
    ~ImapClient();

    bool connect(const QString &host, quint16 port, QString username, QString password, char sectype);
    bool connect();
    void setPublicIpAddress(QString str);
    void close();
    void getDirectoryList();
    void getDirectory(QString dirname);
    QString provider();
    QString account();
    void getMessageHeader(long msgno);
    void getMessageBody(long msgno);
    void clearBuffer();
    void deleteMessage(long msgno);
    void flagMessage(long msgno, QString flag);
    void markAsSeen(long msgno);

signals:
    void authenticated(QString uname);
    void authenticationFailure(QString uname, QString pass);
    void directoryListReceived(QString uname, QStringList slist);
    void directoryReceived(QString uname, QString dirname, long count, long nextuid);
    void headerNotFound(QString uname, QString dirname, long msgno);
    void bodyNotFound(QString uname, QString dirname, long msgno);

    void headerReceived(QString uname, QString dirname, long msgno, MailHeader emlhdr);
    void bodyReceived(QString uname, QString dirname, long msgno, MailBody emlbdy);
    void flagged(long msgno);
    void deleted(long msgno);
    void markedSeen(QString uname, QString dirname, long msgno);

private slots:
    void eventConnected();
    void eventDataReceived();

private:
    void authenticate();
    void expunge();

    void normalizeGmailDirList(QString &str);
    void normalizeOutlookDirList(QString &str);
    void normalizeGenericDirList(QString &str);
    void normalizeYahooDirList(QString &str);

    void detectKnowTypes(QString &str);
    int getNumber(const char *ptr);
    void sendCommand();
    QString decodeBase64(QString string);
    void utf8Transform(QString &str);

    QTcpSocket *_Socket;
    QString _Host;
    QString _Username;
    QString _Password;
    quint16 _Port;
    QString _Buffer;
    char _SecurityType;
    QString _PublicIpAddress;
    QString _CurrentType;
    QString _CurrentDirectory;
    long _CurrentMailNo;
    char _CurrentCommand[128];
};

#endif
