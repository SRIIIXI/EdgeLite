#ifndef _MAIL_STORAGE
#define _MAIL_STORAGE

#include <QtCore>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QCoreApplication>
#include "Mail.h"

class MailStorage
{
public:
    MailStorage();
    ~MailStorage();
    bool storeMail(MailBody &email);
    bool retrieveMail(QString messageid, MailBody &mail);
private:
    static QString _DataDirectory;
};

#endif
