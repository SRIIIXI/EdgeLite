#include "MailStorage.h"

QString MailStorage::_DataDirectory = "";

MailStorage::MailStorage()
{
    if(_DataDirectory.length() < 1)
    {
        QString appnamestr = QCoreApplication::applicationName();
        QString datalocation = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)+ "/" + appnamestr;

        _DataDirectory = datalocation + "/mails/";

        QDir dir(_DataDirectory);

        if(!dir.exists())
        {
            dir.mkdir(_DataDirectory);
        }
    }
}

MailStorage::~MailStorage()
{

}

bool MailStorage::storeMail(MailBody &email)
{
    //Uncompressed data stored in plain text file
    QString fname = _DataDirectory;
    fname += email.messageId();
    fname = fname.replace("<", "");
    fname = fname.replace(">", "");
    fname += ".eml";

    QFile fl(fname);

    if(fl.open(QIODevice::WriteOnly))
    {
        QByteArray ba;
        ba.append(email.message());
        fl.write(ba);
        fl.flush();
        fl.close();
        return true;
    }

    return false;
}

bool MailStorage::retrieveMail(QString messageid, MailBody &mail)
{
    //Uncompressed data stored in plain text file

    QString fname = _DataDirectory;
    fname += messageid;
    fname = fname.replace("<", "");
    fname = fname.replace(">", "");
    fname += ".eml";

    QFile fl(fname);

    if(fl.open(QIODevice::ReadOnly))
    {
        QByteArray ba;
        ba = fl.readAll();
        mail.setMessage(ba.toStdString().c_str());
        fl.close();
        return true;
    }

    return false;
}
