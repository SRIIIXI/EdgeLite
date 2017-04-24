#include "ImapClient.h"

ImapClient::ImapClient()
{
    _CurrentType = "";
}

ImapClient::ImapClient(const QString &host, quint16 port, QString username, QString password, char sectype)
{
    if(sectype == 'A')
    {
        sectype = 'S';
    }

    _Host = host;
    _Port = port;
    _Username = username;
    _Password = password;
    _SecurityType = sectype;
    _CurrentType = "";

    detectKnowTypes(username);
}

ImapClient::~ImapClient()
{
    if(_Socket != nullptr)
    {
        if(_Socket->state() == QTcpSocket::ConnectedState)
        {
            _Socket->close();
            delete _Socket;
        }
    }
}

bool ImapClient::connect(const QString &host, quint16 port, QString username, QString password, char sectype)
{
    if(sectype == 'A')
    {
        sectype = 'S';
    }

    _Host = host;
    _Port = port;
    _Username = username;
    _Password = password;
    _SecurityType = sectype;
    _CurrentType = "";

    detectKnowTypes(username);

    if(_SecurityType == 'N')
    {
        _Socket = new QTcpSocket();
    }
    else
    {
        _Socket = new QSslSocket();

        if(_SecurityType == 'S')
        {
            ((QSslSocket*)_Socket)->setProtocol(QSsl::SecureProtocols);
        }

        if(_SecurityType == 'T')
        {
            ((QSslSocket*)_Socket)->setProtocol(QSsl::TlsV1_2OrLater);
        }
    }

    QObject::connect(_Socket, &QSslSocket::connected, this, &ImapClient::eventConnected);
    QObject::connect(_Socket, &QSslSocket::readyRead, this, &ImapClient::eventDataReceived);

    if(_SecurityType == 'N')
    {
        _Socket->connectToHost(_Host, _Port);
    }
    else
    {
        ((QSslSocket*)_Socket)->connectToHostEncrypted(_Host, _Port);
        ((QSslSocket*)_Socket)->ignoreSslErrors();
    }

    return true;
}

bool ImapClient::connect()
{
    if(_SecurityType == 'N')
    {
        _Socket = new QTcpSocket();
    }
    else
    {
        _Socket = new QSslSocket();

        if(_SecurityType == 'S')
        {
            ((QSslSocket*)_Socket)->setProtocol(QSsl::SecureProtocols);
        }

        if(_SecurityType == 'T')
        {
            ((QSslSocket*)_Socket)->setProtocol(QSsl::TlsV1_2OrLater);
        }
    }

    QObject::connect(_Socket, &QSslSocket::connected, this, &ImapClient::eventConnected);
    QObject::connect(_Socket, &QSslSocket::readyRead, this, &ImapClient::eventDataReceived);

    if(_SecurityType == 'N')
    {
        _Socket->connectToHost(_Host, _Port);
    }
    else
    {
        ((QSslSocket*)_Socket)->connectToHostEncrypted(_Host, _Port);
        ((QSslSocket*)_Socket)->ignoreSslErrors();
    }

    return true;
}

void ImapClient::close()
{
    _Socket->close();
}

void ImapClient::detectKnowTypes(QString &str)
{
    if(_CurrentType.length() > 0)
    {
        return;
    }

    if(str.toLower().contains("gmail"))
    {
        _CurrentType = "gmail";
    }
    else
    {
        if(str.toLower().contains("yahoo"))
        {
            _CurrentType = "yahoo";
        }
        else
        {
            if(str.toLower().contains("hotmail") || str.toLower().contains("live") || str.toLower().contains("outlook") || str.toLower().contains("msn"))
            {
                _CurrentType = "outlook";
            }
        }
    }

}

QString ImapClient::provider()
{
    return _CurrentType;
}

QString ImapClient::account()
{
    return _Username;
}

void ImapClient::setPublicIpAddress(QString str)
{
    _PublicIpAddress = str;
}

void ImapClient::clearBuffer()
{
    _Buffer.clear();
}

void ImapClient::sendCommand()
{
    int len = strlen(_CurrentCommand);
    _Socket->write(_CurrentCommand, len);
}

void ImapClient::markAsSeen(long msgno)
{
    _CurrentMailNo = msgno;
    memset(_CurrentCommand, 0, 128);
    sprintf(_CurrentCommand, "SN STORE %ld +FLAGS \\Seen\r\n", msgno);
    sendCommand();
}

void ImapClient::expunge()
{
    memset(_CurrentCommand, 0, 128);
    sprintf(_CurrentCommand, "EX EXPUNGE\r\n");
    sendCommand();
}

void ImapClient::deleteMessage(long msgno)
{
    _CurrentMailNo = msgno;
    memset(_CurrentCommand, 0, 128);
    sprintf(_CurrentCommand, "DL STORE %ld +FLAGS \\Deleted\r\n", msgno);
    sendCommand();
}

void ImapClient::flagMessage(long msgno, QString flag)
{
    _CurrentMailNo = msgno;
    memset(_CurrentCommand, 0, 128);
    sprintf(_CurrentCommand, "FL STORE %ld +FLAGS \\%s\r\n", msgno, flag.toStdString().c_str());
    sendCommand();
}

void ImapClient::getDirectoryList()
{
    QString temp = QString("LS LIST ") + QString("\"\"") + QString(" \"*\"") + QString("\r\n");
    memset(_CurrentCommand, 0, 128);
    sprintf(_CurrentCommand, "%s", temp.toStdString().c_str());
    sendCommand();
}

void ImapClient::authenticate()
{
    memset(_CurrentCommand, 0, 128);
    sprintf(_CurrentCommand, "LG LOGIN %s %s\r\n", _Username.toStdString().c_str(), _Password.toStdString().c_str());
    sendCommand();
}

void ImapClient::eventConnected()
{
    memset(_CurrentCommand, 0, 128);
    sprintf(_CurrentCommand, "CY CAPABILITY\r\n");
    sendCommand();
}

void ImapClient::getDirectory(QString dirname)
{
    _CurrentDirectory = dirname;
    memset(_CurrentCommand, 0, 128);
    sprintf(_CurrentCommand, "IN SELECT \"%s\"\r\n", dirname.toStdString().c_str());
    sendCommand();
}

void ImapClient::getMessageHeader(long msgno)
{
    _CurrentMailNo = msgno;
    memset(_CurrentCommand, 0, 128);
    sprintf(_CurrentCommand, "HD FETCH %ld (BODY[HEADER.FIELDS (MESSAGE-ID DATE FROM SUBJECT TO CC BCC FLAGS)])\r\n", _CurrentMailNo);
    sendCommand();
}

void ImapClient::getMessageBody(long msgno)
{
    _CurrentMailNo = msgno;
    memset(_CurrentCommand, 0, 128);
    sprintf(_CurrentCommand, "BD FETCH %ld (BODY[TEXT])\r\n", _CurrentMailNo);
    sendCommand();
}

void ImapClient::eventDataReceived()
{
    QByteArray buff;
    QStringList lines;
    QString lastline;

    if (_Socket->canReadLine())
    {
        buff = _Socket->readAll();

        _Buffer += buff.toStdString().c_str();

        if(!_Buffer.contains("\r\n"))
        {
            return;
        }

        lines = _Buffer.split("\r\n", QString::SkipEmptyParts);

        lastline = lines.at(lines.count()-1);

        if(lines.count() > 1)
        {
            lines.removeLast();
        }

        if(((lastline.contains("IN OK") && strstr(_CurrentCommand, "IN SELECT") != nullptr))
                || (lines.at(0).contains("* FLAGS") && strstr(_CurrentCommand, "IN SELECT") != nullptr))
        {
            int count = 0;
            int uidnext = 0;

            for(int ctr = 0; ctr < lines.count(); ctr++)
            {
                if(lines.at(ctr).toLower().contains("exists") || lines.at(ctr).toLower().contains("messages"))
                {
                    count = getNumber(lines.at(ctr).toStdString().c_str());
                }

                if(lines.at(ctr).toLower().contains("uidnext"))
                {
                    uidnext = getNumber(lines.at(ctr).toStdString().c_str());
                }
            }

            emit directoryReceived(_Username, _CurrentDirectory, count, uidnext);
            _CurrentDirectory.clear();
            _Buffer.clear();
            return;
        }

        if(lastline.contains("FL OK") && strstr(_CurrentCommand, "FL STORE") != nullptr)
        {
            _Buffer.clear();
            emit flagged(_CurrentMailNo);
            return;
        }

        if(lastline.contains("DL OK") && strstr(_CurrentCommand, "DL STORE") != nullptr)
        {
            _Buffer.clear();
            expunge();
            return;
        }

        if(lastline.contains("SN OK") && strstr(_CurrentCommand, "SN STORE") != nullptr)
        {
            _Buffer.clear();
            emit markedSeen(_Username, _CurrentDirectory, _CurrentMailNo);
            return;
        }

        if(lastline.contains("EX OK") && strstr(_CurrentCommand, "EX EXPUNGE") != nullptr)
        {
            _Buffer.clear();
            emit deleted(_CurrentMailNo);
            return;
        }

        if(lastline.contains("* OK"))
        {
            detectKnowTypes(lastline);
            _Buffer.clear();
            return;
        }

        if(lastline.contains("CY OK") && strstr(_CurrentCommand, "CY CAPABILITY") != nullptr)
        {
            _Buffer.clear();
            authenticate();
            return;
        }

        if(lastline.contains("LG OK") && strstr(_CurrentCommand, "LG LOGIN") != nullptr)
        {
            _Buffer.clear();
            emit authenticated(_Username);
            return;
        }

        if((lastline.contains("LG NO") || lines.at(0).contains("LG NO"))
                 && strstr(_CurrentCommand, "LG LOGIN") != nullptr)
        {
            _Buffer.clear();
            emit authenticationFailure(_Username, _Password);
            return;
        }

        if(lastline.contains("LS OK") && strstr(_CurrentCommand, "LS LIST") != nullptr)
        {
            QStringList dirlist;

            for(int ctr = 0; ctr < lines.count(); ctr++)
            {
                QString str =lines.at(ctr);

                if(_CurrentType == "gmail")
                {
                    normalizeGmailDirList(str);
                }
                else
                {
                    if(_CurrentType == "outlook")
                    {
                        normalizeOutlookDirList(str);
                    }
                    else
                    {
                        if(_CurrentType == "yahoo")
                        {
                            normalizeYahooDirList(str);
                        }
                        else
                        {
                            normalizeGenericDirList(str);
                        }
                    }
                }

                if(str.length() < 1)
                {
                    continue;
                }

                dirlist.append(str);
            }

            emit directoryListReceived(_Username, dirlist);
            _Buffer.clear();
            return;
        }

        if(lastline.contains("HD OK") && strstr(_CurrentCommand, "HD FETCH") != nullptr)
        {
            _Buffer.clear();

            QString numstr = lines.at(0);

            numstr = numstr.replace("* ", "");

            long num = 0;

            num = atol(numstr.toStdString().c_str());

            lines.removeFirst();

            int headerstep = 0;

            QString str;

            MailHeader ehdr;

            for(int ix = 0; ix < lines.count(); ix++ )
            {
                if(lines.at(ix).contains("Message-ID:") || lines.at(ix).contains("Message-Id:"))
                {
                    str = lines.at(ix);
                    str = str.replace("Message-ID:", "").trimmed();
                    str = str.replace("Message-Id:", "").trimmed();
                    ehdr.setMessageId(str);
                    headerstep = 0;
                    continue;
                }

                if(lines.at(ix).contains("Date:"))
                {
                    str = lines.at(ix);
                    str = str.replace("Date:", "").trimmed();
                    ehdr.setTimeStamp(str);
                    headerstep = 1;
                    continue;
                }

                if(lines.at(ix).contains("From:"))
                {
                    str = lines.at(ix);
                    str = str.replace("From:", "").trimmed();
                    ehdr.setFrom(str);
                    headerstep = 2;
                    continue;
                }

                if(lines.at(ix).contains("Subject:"))
                {
                    str = lines.at(ix);
                    str = str.replace("Subject:", "").trimmed();

                    if(str.toLower().contains("utf-8"))
                    {
                        utf8Transform(str);
                    }

                    ehdr.setSubject(str);
                    headerstep = 3;
                    continue;
                }

                if(lines.at(ix).contains("To:"))
                {
                    str = lines.at(ix);
                    str = str.replace("To:", "").trimmed();
                    ehdr.addtoToList(str);
                    headerstep = 4;
                    continue;
                }

                if(lines.at(ix).contains("CC:"))
                {
                    str = lines.at(ix);
                    str = str.replace("CC:", "").trimmed();
                    ehdr.addtoCcList(str);
                    headerstep = 5;
                    continue;
                }

                if(lines.at(ix).contains("BCC:"))
                {
                    str = lines.at(ix);
                    str = str.replace("BCC:", "").trimmed();
                    ehdr.addtoBccList(str);
                    headerstep = 6;
                    continue;
                }

                if(lines.at(ix) == ")")
                {
                     continue;
                }

                //Text line warped over

                switch(headerstep)
                {
                case 3:
                {
                    QString oldstr = ehdr.subject();

                    str = lines.at(ix).trimmed();

                    if(str.toLower().contains("utf-8"))
                    {
                        utf8Transform(str);
                    }

                    str = oldstr + str;

                    ehdr.setSubject(str);
                    break;
                }
                case 4:
                {
                    str = lines.at(ix).trimmed();
                    ehdr.addtoToList(str);
                    break;
                }
                case 5:
                {
                    str = lines.at(ix).trimmed();
                    ehdr.addtoCcList(str);
                    break;
                }
                case 6:
                {
                    str = lines.at(ix).trimmed();
                    ehdr.addtoBccList(str);
                    break;
                }
                default:
                    break;
                }
            }

            _Buffer.clear();

            ehdr.normalizeAddresses();

            emit headerReceived(_Username, _CurrentDirectory, num, ehdr);

            return;
        }

        if(lastline.contains("BD OK")  && strstr(_CurrentCommand, "BD FETCH") != nullptr)
        {
            int pos = 0;

            pos = _Buffer.indexOf("BD OK");

            pos = pos - 3;

            _Buffer = _Buffer.remove(pos, _Buffer.length() - pos);

            pos = _Buffer.indexOf("\r\n");

            pos = pos + 2;

            _Buffer = _Buffer.remove(0, pos);

            MailBody ebdy;
            ebdy.setMessage(_Buffer);

            emit bodyReceived(_Username, _CurrentDirectory, _CurrentMailNo, ebdy);

            _Buffer.clear();

            return;
        }

        if(lastline.contains("HD BAD")  && strstr(_CurrentCommand, "HD FETCH") != nullptr)
        {
            _Buffer.clear();

            emit headerNotFound(_Username, _CurrentDirectory, _CurrentMailNo);

            return;
        }

        if(lastline.contains("BD BAD") && strstr(_CurrentCommand, "BD FETCH") != nullptr)
        {
            _Buffer.clear();

            emit bodyNotFound(_Username, _CurrentDirectory, _CurrentMailNo);

            return;
        }
    }
}

int ImapClient::getNumber(const char* ptr)
{
    char buff[32] = {0};

    int ctr = 0;
    int idx = 0;

    for( ; ptr[ctr] != 0 && ctr < 31; ctr++)
    {
        if(isdigit(ptr[ctr]))
        {
            buff[idx] = ptr[ctr];
            idx++;
        }
    }

    return atoi(buff);
}


void ImapClient::normalizeGmailDirList(QString &str)
{
    if(str.contains("INBOX"))
    {
        str = "Inbox";
        return;
    }

    if(str.contains("Trash"))
    {
        str = "[Gmail]/Trash";
        return;
    }

    if(str.contains("Sent"))
    {
        str = "[Gmail]/Sent Mail";
        return;
    }

    if(str.contains("Drafts"))
    {
        str = "[Gmail]/Drafts";
        return;
    }

    if(str.contains("Spam"))
    {
        str = "[Gmail]/Spam";
        return;
    }

    if(str.contains("Important"))
    {
        str = "[Gmail]/Important";
        return;
    }

    if(str.contains("Starred"))
    {
        str = "[Gmail]/Starred";
        return;
    }

    str = "";
}

void ImapClient::normalizeOutlookDirList(QString &str)
{
    str = str.replace("* LIST ", "");
    str = str.replace("\".\"", "");
    str = str.replace("\"/\"", "");
    str = str.replace("\r\n", "");
    str = str.replace("(", "");
    str = str.replace(")", "");
    str = str.replace("\"", "");
    str = str.replace("\\", "");
    str = str.trimmed();
}

void ImapClient::normalizeGenericDirList(QString &str)
{
    if(str.contains("INBOX"))
    {
        str = "Inbox";
        return;
    }

    if(str.contains("Trash"))
    {
        str = "Trash";
        return;
    }

    if(str.contains("Sent"))
    {
        str = "Sent";
        return;
    }

    if(str.contains("Drafts"))
    {
        str = "Drafts";
        return;
    }

    if(str.contains("Junk"))
    {
        str = "Junk";
        return;
    }

    str = "";
}

void ImapClient::normalizeYahooDirList(QString &str)
{
    if(str.contains("Inbox"))
    {
        str = "Inbox";
        return;
    }

    if(str.contains("Junk"))
    {
        str = "Junk";
        return;
    }

    if(str.contains("Sent"))
    {
        str = "Sent";
        return;
    }

    if(str.contains("Drafts"))
    {
        str = "Drafts";
        return;
    }

    if(str.contains("Trash"))
    {
        str = "Trash";
        return;
    }

    if(str.contains("Archive"))
    {
        str = "Archive";
        return;
    }

    str = "";
}

QString ImapClient::decodeBase64(QString string)
{
    QByteArray ba;
    ba.append(string);
    return QByteArray::fromBase64(ba);
}

void ImapClient::utf8Transform(QString &str)
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
            //str = decodeBase64(str);
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
