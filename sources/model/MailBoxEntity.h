#ifndef MAIL_BOX_H
#define MAIL_BOX_H

#include <QObject>
#include <QtSql>
#include "DataEntity.h"
#include "Mail.h"

class MailBoxEntity : protected DataEntity
{
public:
    MailBoxEntity(QSqlDatabase* db, QString tabname);
    virtual ~MailBoxEntity();
    bool createMailBoxTable();
    bool createMail(MailHeader &rec, QString useremail);
    bool updateMail(MailHeader &rec);
    bool allMails(QList<MailHeader> &list);
    bool selectedMails(QList<MailHeader> &list, QString keyname, QVariant value);
    bool removeMail(QString keyname, QVariant value);
private:
    bool create(QSqlRecord record);
    bool update(QSqlRecord record);
};

extern MailBoxEntity* mailBoxEntityPtr;

#endif
