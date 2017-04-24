#include "SmtpClient.h"

const int SMTP_STEP_SEND_AT = 100;
const int SMTP_STEP_SEND_FR = 101;
const int SMTP_STEP_SEND_TO = 102;
const int SMTP_STEP_SEND_DC = 103;
const int SMTP_STEP_SEND_DT = 104;
const int SMTP_STEP_SEND_FN = 105;

SmtpClient::SmtpClient()
{
    _SendStep = SMTP_STEP_SEND_AT;
    _Socket = nullptr;
    _RcPtCount = 0;
}

SmtpClient::SmtpClient(const QString &host, quint16 port, QString username, QString password, char sectype, bool sendhelo)
{
    if(sectype == 'A')
    {
        sectype = 'S';
    }

    _SendStep = SMTP_STEP_SEND_AT;
    _Host = host;
    _Port = port;
    _Username = username;
    _Password = password;
    _SecurityType = sectype;
    _SendHelo = sendhelo;
    _Socket = nullptr;
    _RcPtCount = 0;
}

SmtpClient::~SmtpClient()
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

QString SmtpClient::encodeBase64(QString string)
{
    QByteArray ba;
    ba.append(string);
    return ba.toBase64();
}

QString SmtpClient::decodeBase64(QString string)
{
    QByteArray ba;
    ba.append(string);
    return QByteArray::fromBase64(ba);
}

bool SmtpClient::connect(const QString &host, quint16 port, QString username, QString password, char sectype, bool sendhelo)
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
    _SendHelo = sendhelo;
    _RcPtCount = 0;

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

    QObject::connect(_Socket, &QSslSocket::connected, this, &SmtpClient::eventConnected);
    QObject::connect(_Socket, &QSslSocket::readyRead, this, &SmtpClient::eventDataReceived);

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

bool SmtpClient::connect()
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

    QObject::connect(_Socket, &QSslSocket::connected, this, &SmtpClient::eventConnected);
    QObject::connect(_Socket, &QSslSocket::readyRead, this, &SmtpClient::eventDataReceived);

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

void SmtpClient::close()
{
    _Socket->close();
}

void SmtpClient::setPublicIpAddress(QString str)
{
    _PublicIpAddress = str;
}

QString SmtpClient::account()
{
    return _Username;
}

bool SmtpClient::sendMail(MailHeader &ehdr, MailBody &ebdy)
{
    _EmlHdr = ehdr;
    _EmlBdy = ebdy;
    _SendStep = SMTP_STEP_SEND_FR;
    continuetransmission();

    return true;
}

void SmtpClient::eventConnected()
{
    char buff[128] = {0};

    if(_SendHelo)
    {
        sprintf(buff, "EHLO %s\r\n", _PublicIpAddress.toStdString().c_str());
    }
    else
    {
        sprintf(buff, "AUTH LOGIN\r\n");
    }

    int len = strlen(buff);
    _Socket->write(buff, len);
}

void SmtpClient::authenticate()
{
    char buff[128] = {0};

    sprintf(buff, "AUTH LOGIN\r\n");
    int len = strlen(buff);

    _Socket->write(buff, len);
}

void SmtpClient::eventDataReceived()
{
    if (_Socket->canReadLine())
    {
        QByteArray buff = _Socket->readAll();
        _Buffer += buff.toStdString().c_str();

        if(!_Buffer.contains("\r\n"))
        {
            return;
        }

        QStringList lines = _Buffer.split("\r\n", QString::SkipEmptyParts);
        QString lastline = lines.at(lines.count()-1);

        if(lines.count() > 1)
        {
            QString dummy = lines.takeLast();
        }

        if(lastline.contains("EHLO BAD ") || lastline.contains("AUTH BAD "))
        {
            _Buffer.clear();
            emit authenticationFailure(_Username, _Password);
            return;
        }

        //Authentication challenge
        if(lastline.contains("334 "))
        {
            QString authrequest = lastline.replace("334 ", "");
            authrequest = decodeBase64(authrequest);
            QString authreply;
            QByteArray base64buff;

            //Server is asking for username
            if(authrequest.contains("Username"))
            {
                authreply = encodeBase64(_Username);
            }

            //Server is asking for password
            if(authrequest.contains("Password"))
            {
                authreply = encodeBase64(_Password);
            }

            authreply += "\r\n";
            base64buff.append(authreply);
            _Socket->write(base64buff);
            _Buffer.clear();
            return;
        }

        //Welcome message
        if(lastline.contains("220 "))
        {
            _Buffer.clear();
            return;
        }

        //Authentication successfull
        if(lastline.contains("235 "))
        {
            _Buffer.clear();
            emit authenticated(_Username);
            //Nothing to do next
            return;
        }

        //Generic OK response
        if(lastline.contains("250 "))
        {
            _Buffer.clear();
            //Mail delivered successfully
            if(lastline.contains("250 2.0.0"))
            {
                emit mailDeliveryStatus(_Username, lastline, true);
            }
            else
            {
                if(_SendStep == SMTP_STEP_SEND_AT)
                {
                    authenticate();
                }
                else
                {
                    //FROM command from client has been verified by server
                    if(lastline.contains("250 2.1.0"))
                    {
                        _SendStep++;
                        continuetransmission();
                    }
                    //RCPT TO command from client has been verified by server
                    if(lastline.contains("250 2.1.5"))
                    {
                        //Multiple RCPT TO can be sent
                        //So we will keep on decrementing the count
                        _RcPtCount--;
                        //Once we reach zero this means all RCPT TO has been okay
                        //We can send the data now
                        if(_RcPtCount < 1)
                        {
                            _SendStep++;
                            continuetransmission();
                        }
                    }
                }
            }
            return;
        }

        //Continue sending data
        if(lastline.contains("354 "))
        {
            _Buffer.clear();
            _SendStep++;
            continuetransmission();
            return;
        }

        //Mail delivered and connection will close
        if(lastline.contains("221 "))
        {
            _Buffer.clear();
            emit mailDeliveryStatus(_Username, lastline, true);
            return;
        }

        //Authentication failure
        if(lastline.contains("535 5.7.8"))
        {
            _Buffer.clear();
            emit authenticationFailure(_Username, _Password);
            return;
        }

        if(lastline.contains("553 "))
        {
            _Buffer.clear();
            emit mailDeliveryStatus(_Username, lastline, false);
            return;
        }

        int err = atoi(lastline.toStdString().c_str());

        //Anything above 420 indicates an error
        if(err > 420)
        {
            qDebug() << "Error " << endl;
        }
        else
        {
            qDebug() << "Unhandled response " << endl;
        }

        qDebug() << _Buffer << endl;
        _Buffer.clear();
    }
}

void SmtpClient::continuetransmission()
{
    QByteArray ba;
    QString str;
    bool cansend = true;

    switch(_SendStep)
    {
        case SMTP_STEP_SEND_FR:
        {
            str = "MAIL FROM: <" + _EmlHdr.from() + ">\r\n";
            break;
        }

        case SMTP_STEP_SEND_TO:
        {
            QStringList rcptlist;
            rcptlist.append(_EmlHdr.toList());
            rcptlist.append(_EmlHdr.ccList());
            rcptlist.append(_EmlHdr.bccList());
            for(int idx = 0; idx < rcptlist.count(); idx++)
            {
                str += "RCPT TO: <" + rcptlist.at(idx) + ">\r\n";
                _RcPtCount++;
            }
            break;
        }

        case SMTP_STEP_SEND_DC:
        {
            str = "DATA\r\n";
            break;
        }

        case SMTP_STEP_SEND_DT:
        {
            _EmlHdr.serialize(str, _EmlBdy);
            str += "\r\n";
            break;
        }

        case SMTP_STEP_SEND_FN:
        {
            cansend = false;
            emit mailDeliveryStatus(_Username, "OK", true);
            break;
        }

        default:
        {
            cansend = false;
        }
    }

    if(cansend)
    {
        ba.append(str);
        _Socket->write(ba);
        // If we are at the message body transmission stage, then we have to indicate that we are done
        if(_SendStep == SMTP_STEP_SEND_DT)
        {
            str.clear();
            str = "\r\n.\r\n";
            QByteArray end;
            end.append(str);
            _Socket->write(end);
        }
    }
    return;
}
