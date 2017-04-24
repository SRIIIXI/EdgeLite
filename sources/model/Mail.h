#ifndef _MAIL
#define _MAIL

#include <QtCore>

typedef enum MailTextEncoding
{
    QuotedPrintable,
    Base64
}MailTextEncoding;

typedef enum MimeType
{
    PlainText,
    Html,
    InlineImage,
    Attachment
}MimeType;

class MimeNode
{
public:
    QString NodeName;
    QString Data;
    MimeType NodeType;
    MailTextEncoding TextEncoding;
};

class MailBody
{
public:

    MailBody();
    MailBody(const MailBody &other);
    ~MailBody();

    void operator=(const MailBody& other);

    void setMessage(QString message);
    void setMessageId(QString msgid);

    void attachFile(QString filename);

    QString message();
    QList<QString> attachments();
    QString messageId();

    void serialize(QString &str);
    void deSerialize();

    bool hasMimeData();

    QList<MimeNode>* mimeNodes();

private:
    void parseAttachments();
    void parseTexts();
    void extractAttachment(int &ctr);
    void extractText(int &ctr);

    QString encodeBase64(QString string);
    QString encodeBase64(QByteArray buffer);
    QString decodeBase64(QString string);

    QList<QString> _AttachementList;
    QString _Message;
    QString _MessageId;

    QStringList _MimeData;
    QList<MimeNode> _ParsedMimeData;

};

/*------------------------------------------------------*/

class MailHeader
{
public:

    MailHeader();
    MailHeader(const MailHeader &other);
    ~MailHeader();

    void operator=(const MailHeader& other);

    void addtoToList(QString receipient, bool overwrite = false);
    void addtoCcList(QString receipient, bool overwrite = false);
    void addtoBccList(QString receipient, bool overwrite = false);

    void setSubject(QString subject);
    void setFrom(QString from);
    void setFromDisplayName(QString fromdisp);
    void setMessageId(QString msgid);
    void setTimeStamp(QString dt);
    void setDirection(QString dir);
    void setStatus(QString st);

    QList<QString> toList();
    QList<QString> ccList();
    QList<QString> bccList();

    QString to();
    QString cc();
    QString bcc();

    QString subject();
    QString from();
    QString fromDisplayName();
    QString messageId();
    QString timeStamp();

    QString direction();
    QString status();

    void serialize(QString &str, MailBody &body);
    void deSerialize();

    void normalizeAddresses();

private:
    void utf8Transform(QString &str);
    QString encodeBase64(QString string);
    QString encodeBase64(QByteArray buffer);
    QString decodeBase64(QString string);

    QString _Subject;
    QString _From;
    QString _FromDisplayName;
    QString _ToList;
    QString _CcList;
    QString _BccList;
    QString _MessageId;
    QString _TimeStamp;
    QString _Direction;
    QString _Status;
};

#endif
