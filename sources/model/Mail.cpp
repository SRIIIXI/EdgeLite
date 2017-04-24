#include "Mail.h"

MailBody::MailBody()
{

}

MailBody::~MailBody()
{

}

MailBody::MailBody(const MailBody &other)
{
    _Message = other._Message;
    _MessageId = other._MessageId;
    _AttachementList.append(other._AttachementList);
}

void MailBody::operator=(const MailBody& other)
{
    _Message = other._Message;
    _MessageId = other._MessageId;
    _AttachementList.append(other._AttachementList);
}

void MailBody::setMessage(QString message)
{
    _Message = message;
}

void MailBody::attachFile(QString filename)
{
    _AttachementList.append(filename);
}

void MailBody::setMessageId(QString msgid)
{
    _MessageId = msgid;
}

QString MailBody::message()
{
    return _Message;
}

QList<QString> MailBody::attachments()
{
    return _AttachementList;
}

QString MailBody::messageId()
{
    return _MessageId;
}

void MailBody::serialize(QString &str)
{
    str.clear();

    if(_AttachementList.count() < 1)
    {
        str += "\n" + _Message;
    }
    else
    {
        char linebuffer[1024] = {0};
        QString attachmentBoundary = "";
        QDateTime ts = QDateTime::currentDateTimeUtc();

        attachmentBoundary = "--";
        attachmentBoundary += ts.toString("yyyyMMddhhmmss");
        attachmentBoundary += ts.toString("yyyyMMddhhmmss");
        attachmentBoundary += "--";

        memset(linebuffer, 0, 1024);
        sprintf(linebuffer, "MIME-Version: 1.0\n");
        str += linebuffer;
        memset(linebuffer, 0, 1024);
        sprintf(linebuffer, "Content-Type:multipart/mixed;boundary=\"%s\"\n", attachmentBoundary.toStdString().c_str());
        str += linebuffer;

        memset(linebuffer, 0, 1024);
        sprintf(linebuffer, "--%s\n", attachmentBoundary.toStdString().c_str());
        str += linebuffer;

        str += "Content-type: text/plain; charset=iso-8859-1\n";
        str += "Content-Transfer-Encoding: quoted-printable\n";

        memset(linebuffer, 0, 1024);
        sprintf(linebuffer, "\n%s\n", _Message.toStdString().c_str());
        str += linebuffer;

        for(int idx = 0; idx < _AttachementList.count(); idx++)
        {
            QString fname = _AttachementList.at(idx);

            QFileInfo finfo(fname);

            if(!finfo.exists())
            {
                continue;
            }

            QFile fl(fname);

            if(!fl.open(QIODevice::ReadOnly))
            {
                continue;
            }

            QByteArray ba = fl.readAll();

            QString b64content = encodeBase64(ba);

            memset(linebuffer, 0, 1024);
            sprintf(linebuffer, "--%s\n", attachmentBoundary.toStdString().c_str());
            str += linebuffer;

            memset(linebuffer, 0, 1024);
            sprintf(linebuffer, "Content-Type:application/octet-stream;name=\"%s\"\n", fname.toStdString().c_str());
            str += linebuffer;

            memset(linebuffer, 0, 1024);
            sprintf(linebuffer, "Content-Transfer-Encoding:base64\n");
            str += linebuffer;

            memset(linebuffer, 0, 1024);
            sprintf(linebuffer, "Content-Disposition:attachment;filename=\"%s\"\n\n", fname.toStdString().c_str());
            str += linebuffer;

            str += b64content;
            str += "\n\n";

        }

        memset(linebuffer, 0, 1024);
        sprintf(linebuffer, "--%s--\n", attachmentBoundary.toStdString().c_str());
        str += linebuffer;
    }
}

void MailBody::deSerialize()
{
    parseAttachments();
    parseTexts();
}

bool MailBody::hasMimeData()
{
    _MimeData = _Message.split("\r\n");

    while(true)
    {
        if(_MimeData.at(_MimeData.count() - 1).length() < 1)
        {
            _MimeData.removeLast();
        }
        else
        {
            break;
        }
    }

    QString firstline = _MimeData.at(0);
    QString lastline = _MimeData.at(_MimeData.count() - 1);
    bool hasMimeBoundary = false;
    bool hasContentTypeTag = false;

    if(firstline+"--" == lastline)
    {
        hasMimeBoundary = true;
        if(_MimeData.at(1).toLower().contains("content-type"))
        {
           hasContentTypeTag = true;
        }
    }

    if(hasMimeBoundary && hasContentTypeTag)
    {
        return true;
    }

    return false;
}


void MailBody::parseAttachments()
{
    QStringList displaydata;

    int count = _MimeData.count();

    for(int idx = 0; idx < count; idx++)
    {
        QString line = _MimeData.at(idx);

        if(line.trimmed().toLower().contains("content-type:") && line.trimmed().toLower().contains("name="))
        {
            idx--;
            extractAttachment(idx);
            continue;
        }

        displaydata.append(line);
    }

    _MimeData.clear();
    _MimeData.append(displaydata);
}

void MailBody::extractAttachment(int &ctr)
{
    int startline = ctr;
    QString buffer = "";
    QString start = _MimeData.at(ctr);
    QString inlinecontentid;

    bool startaccumulating= false;
    MimeNode value;

    value.NodeType = MimeType::Attachment;

    ctr++;

    while(true)
    {
        buffer = _MimeData.at(ctr).trimmed();

        if(start == buffer || start+"--" == buffer)
        {
            break;
        }

        if(buffer.length() < 1)
        {
            if(startaccumulating == false)
            {
                startaccumulating = true;
            }
            else
            {
                startaccumulating = false;
            }
            ctr++;
            continue;
        }

        if(startaccumulating)
        {
            if(buffer.length() > 0)
            {
                value.Data += buffer;
            }

            if(buffer.contains("=="))
            {
                startaccumulating = false;
            }
        }
        else
        {
            if(buffer.toLower().contains("content-disposition") && buffer.toLower().contains("inline"))
            {
                value.NodeType = MimeType::InlineImage;
            }

            if(buffer.toLower().contains("content-id"))
            {
                int cpos = buffer.indexOf(':');
                buffer.remove(0, cpos+1);
                buffer = buffer.replace("<", "");
                buffer = buffer.replace(">", "");
                inlinecontentid = buffer.trimmed();
            }
        }

        ctr++;
    }

    QString name = _MimeData.at(startline+1);
    int pos = name.indexOf("name=");
    name = name.right(name.length() - pos - 6);

    if(value.NodeType == MimeType::Attachment)
    {
        value.NodeName = name.replace("\"", "");
    }
    else
    {
        value.NodeName = inlinecontentid;
    }

    _ParsedMimeData.append(value);
    _AttachementList.append(value.NodeName);
}

void MailBody::parseTexts()
{
    QStringList displaydata;

    int count = _MimeData.count();

    for(int idx = 0; idx < count; idx++)
    {
        QString line = _MimeData.at(idx);

        if(line.trimmed().toLower().contains("content-type:")
                && ( line.trimmed().toLower().contains("text/plain") || line.trimmed().toLower().contains("text/html")) )
        {
            idx--;
            extractText(idx);
            continue;
        }

        displaydata.append(line);
    }

    _MimeData.clear();
}

void MailBody::extractText(int &ctr)
{
    QString buffer = "";
    QString start = _MimeData.at(ctr);

    bool startaccumulating= false;
    MimeNode value;

    ctr++;

    while(true)
    {
        buffer = _MimeData.at(ctr).trimmed();

        if(start == buffer || start+"--" == buffer)
        {
            break;
        }

        if(!startaccumulating)
        {
            if(buffer.length() < 1)
            {
                startaccumulating = true;
                ctr++;
                continue;
            }
        }

        if(startaccumulating)
        {
            if(buffer.length() < 1)
            {
                value.Data += "\r\n";
            }
            else
            {
                if(buffer.at(buffer.count() -1) == '=' && value.TextEncoding == MailTextEncoding::QuotedPrintable)
                {
                    buffer.remove(buffer.count() -1, 1);
                }

                value.Data += buffer;
            }
        }
        else
        {
            if(buffer.toLower().contains("content-type") && buffer.toLower().contains("text/plain"))
            {
                value.NodeType = MimeType::PlainText;
            }

            if(buffer.toLower().contains("content-type") && buffer.toLower().contains("text/html"))
            {
                value.NodeType = MimeType::Html;
            }

            if(buffer.toLower().contains("content-transfer-encoding") && buffer.toLower().contains("base64"))
            {
                value.TextEncoding = MailTextEncoding::Base64;
            }

            if(buffer.toLower().contains("content-transfer-encoding") && buffer.toLower().contains("quoted-printable"))
            {
                value.TextEncoding = MailTextEncoding::QuotedPrintable;
            }
        }

        ctr++;
    }

    if(value.TextEncoding == MailTextEncoding::Base64)
    {
        value.Data = decodeBase64(value.Data);
    }

    _ParsedMimeData.append(value);
}

QList<MimeNode>* MailBody::mimeNodes()
{
    return &_ParsedMimeData;
}

QString MailBody::encodeBase64(QString string)
{
    QByteArray ba;
    ba.append(string);
    return ba.toBase64();
}

QString MailBody::encodeBase64(QByteArray buffer)
{
    return buffer.toBase64();
}

QString MailBody::decodeBase64(QString string)
{
    QByteArray ba;
    ba.append(string);
    return QByteArray::fromBase64(ba);
}

///////////////////////////////////////////////////////////////////////////

MailHeader::MailHeader()
{

}

MailHeader::~MailHeader()
{

}

MailHeader::MailHeader(const MailHeader &other)
{
    _Subject = other._Subject;
    _FromDisplayName = other._FromDisplayName;
    _From = other._From;
    _MessageId = other._MessageId;
    _TimeStamp = other._TimeStamp;
    _ToList = other._ToList;
    _CcList = other._CcList;
    _BccList = other._BccList;
    _Direction = other._Direction;
    _Status = other._Status;

}

void MailHeader::operator=(const MailHeader& other)
{
    _Subject = other._Subject;
    _FromDisplayName = other._FromDisplayName;
    _From = other._From;
    _MessageId = other._MessageId;
    _TimeStamp = other._TimeStamp;
    _ToList = other._ToList;
    _CcList = other._CcList;
    _BccList = other._BccList;
    _Direction = other._Direction;
    _Status = other._Status;
}

void MailHeader::addtoToList(QString receipient, bool overwrite)
{
    if(_ToList.length() < 1)
    {
        _ToList = receipient.trimmed();
    }
    else
    {
        if(overwrite)
        {
            _ToList = receipient;
        }
        else
        {
            _ToList += ",";
            _ToList += receipient.trimmed();
        }
    }
}

void MailHeader::addtoCcList(QString receipient, bool overwrite)
{
    if(_CcList.length() < 1)
    {
        _CcList = receipient.trimmed();
    }
    else
    {
        if(overwrite)
        {
            _CcList = receipient;
        }
        else
        {
            _CcList += ",";
            _CcList += receipient.trimmed();
        }
    }
}

void MailHeader::addtoBccList(QString receipient, bool overwrite)
{
    if(_BccList.length() < 1)
    {
        _BccList = receipient.trimmed();
    }
    else
    {
        if(overwrite)
        {
            _BccList = receipient;
        }
        else
        {
            _BccList += ",";
            _BccList += receipient.trimmed();
        }
    }
}

void MailHeader::setSubject(QString subject)
{
    _Subject = subject;
}

void MailHeader::setFrom(QString from)
{
    _From = from;
}

void MailHeader::setFromDisplayName(QString fromdisp)
{
    _FromDisplayName = fromdisp;
}

void MailHeader::setMessageId(QString msgid)
{
    _MessageId = msgid;
}

void MailHeader::setTimeStamp(QString dt)
{
    _TimeStamp = dt;
}

void MailHeader::setDirection(QString dir)
{
    _Direction = dir;
}

void MailHeader::setStatus(QString st)
{
    _Status = st;
}

QList<QString> MailHeader::toList()
{
    return _ToList.split(",", QString::SkipEmptyParts);
}

QList<QString> MailHeader::ccList()
{
    return _CcList.split(",", QString::SkipEmptyParts);
}

QList<QString> MailHeader::bccList()
{
    return _BccList.split(",", QString::SkipEmptyParts);
}

QString MailHeader::to()
{
    return _ToList;
}

QString MailHeader::cc()
{
    return _CcList;
}

QString MailHeader::bcc()
{
    return _BccList;
}

QString MailHeader::subject()
{
    return _Subject;
}

QString MailHeader::from()
{
    return _From;
}

QString MailHeader::fromDisplayName()
{
    return _FromDisplayName;
}

QString MailHeader::messageId()
{
    return _MessageId;
}

QString MailHeader::timeStamp()
{
    return _TimeStamp;
}

QString MailHeader::direction()
{
    return _Direction;
}

QString MailHeader::status()
{
    return _Status;
}

void MailHeader::serialize(QString &str, MailBody &body)
{
    str.clear();

    str += "To: ";
    str += _ToList;
    str += "\r\n";

    str += "From: ";
    str += _From;
    str += "\r\n";

    str += "Subject: ";
    str += _Subject;
    str += "\r\n";

    if(_CcList.count() > 0)
    {
        str += "cc: ";
        str += _CcList;
        str += "\r\n";
    }

    if(_BccList.count() > 0)
    {
        str += "bcc: ";
        str += _BccList;
        str += "\r\n";
    }


    QString bodystr;

    body.serialize(bodystr);

    str += bodystr;
}

void MailHeader::deSerialize()
{

}

void MailHeader::normalizeAddresses()
{
    QStringList pass1;
    QStringList pass2;
    QString str;
    QString line;
    int idx = 0;

    pass1.clear();
    pass2.clear();
    str = _From.replace('<', ',');
    str = _From.replace('>', ',');

    pass1 = _From.split(',', QString::SkipEmptyParts);

    for(idx = 0; idx < pass1.count(); idx++)
    {
        line = pass1.at(idx).trimmed();
        if(line.contains('@')  && line.at(0) != '"')
        {
            pass2.append(line);
        }
        else
        {
            _FromDisplayName = line.trimmed();
        }
    }

    _From = pass2.join(',');
    if(_FromDisplayName.length() < 1)
    {
        _FromDisplayName = _From;
    }

    utf8Transform(_FromDisplayName);
    _FromDisplayName = _FromDisplayName.replace("\"", "");

    pass1.clear();
    pass2.clear();
    str = _ToList.replace('<', ',');
    str = _ToList.replace('>', ',');

    pass1 = _ToList.split(',', QString::SkipEmptyParts);

    for(idx = 0; idx < pass1.count(); idx++)
    {
        line = pass1.at(idx).trimmed();
        if(line.contains('@')  && line.at(0) != '"')
        {
            pass2.append(line);
        }
    }

    _ToList = pass2.join(',');

    pass1.clear();
    pass2.clear();
    str = _CcList.replace('<', ',');
    str = _CcList.replace('>', ',');

    pass1 = _CcList.split(',', QString::SkipEmptyParts);

    for(idx = 0; idx < pass1.count(); idx++)
    {
        line = pass1.at(idx).trimmed();
        if(line.contains('@')  && line.at(0) != '"')
        {
            pass2.append(line);
        }
    }

    _CcList = pass2.join(',');

    pass1.clear();
    pass2.clear();
    str = _BccList.replace('<', ',');
    str = _BccList.replace('>', ',');

    pass1 = _BccList.split(',', QString::SkipEmptyParts);

    for(idx = 0; idx < pass1.count(); idx++)
    {
        line = pass1.at(idx).trimmed();
        if(line.contains('@')  && line.at(0) != '"')
        {
            pass2.append(line);
        }
    }

    _BccList = pass2.join(',');
}

void MailHeader::utf8Transform(QString &str)
{
    if(str.toLower().contains("utf-8"))
    {
        int utf8startpos = -1;
        int utf8endpos = -1;
        QChar enc = '\0';

        utf8startpos = str.toLower().indexOf("utf-8") - 2;
        utf8endpos = str.indexOf("?=");

        str = str.remove(utf8endpos, 2);
        str = str.remove(utf8startpos, 8);

        enc = str.at(utf8startpos);

        str = str.remove(utf8startpos, 2);

        if(enc.toUpper() == 'B')
        {
            str = decodeBase64(str);
        }
        else
        {
            str = str.replace("=2E", ".");
            str = str.replace("=3F", "?");
            str = str.replace("=2C", ",");
            str = str.replace("_", " ");
            str = str.replace("=20", " ");
        }
    }
}

QString MailHeader::encodeBase64(QString string)
{
    QByteArray ba;
    ba.append(string);
    return ba.toBase64();
}

QString MailHeader::encodeBase64(QByteArray buffer)
{
    return buffer.toBase64();
}

QString MailHeader::decodeBase64(QString string)
{
    QByteArray ba;
    ba.append(string);
    return QByteArray::fromBase64(ba);
}

///////////////////////////////////////////////////////////////////////////

