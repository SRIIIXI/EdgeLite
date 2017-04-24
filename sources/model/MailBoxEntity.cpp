#include "MailBoxEntity.h"

MailBoxEntity* mailBoxEntityPtr = nullptr;

MailBoxEntity::MailBoxEntity(QSqlDatabase *db, QString tabname) : DataEntity(db, tabname)
{
    mailBoxEntityPtr = this;

    _InsertSQL = "insert into MailBox (EMailId, MessageId, Subject, FromDisplayName, FromUser, ToList, CCList, BCCList, MailDirection, MailStatus, MessageTimeStamp) values ('EMailId__', 'MessageId__', 'Subject__', 'FromDisplayName__','FromUser__', 'ToList__', 'CCList__', 'BCCList__', 'MailDirection__', 'MailStatus__', 'MessageTimeStamp__')";

    _UpdateSQL = "update MailBox set MessageId = 'MessageId__', Subject = 'Subject__', FromDisplayName = 'FromDisplayName__', FromUser = 'FromUser__', ToList = 'ToList__', CCList = 'CCList__', BCCList = 'BCCList__', MailDirection = 'MailDirection__', MailStatus = 'MailStatus__', MessageTimeStamp = 'MessageTimeStamp__' where EMailId = 'EMailId__'";
}

MailBoxEntity::~MailBoxEntity()
{
}

bool MailBoxEntity::create(QSqlRecord record)
{
    return true;
}

bool MailBoxEntity::update(QSqlRecord record)
{
    return true;
}

bool MailBoxEntity::createMailBoxTable()
{
    QString sqlString = "create table MailBox \
            (	\
            EMailId varchar(32) not null, \
            MessageId varchar(256) not null, \
            Subject varchar(128), \
            FromDisplayName varchar(64), \
            FromUser varchar(64), \
            ToList varchar(1024), \
            CCList varchar(1024), \
            BCCList varchar(1024), \
            MailDirection char(1) not null, \
            MailStatus char(1) not null, \
            MessageTimeStamp varchar(32), \
            primary key(EmailId, MessageId) \
            )";

    QSqlQuery qryresult;

    try
    {
        _Database->transaction();

        qryresult = _Database->exec(sqlString);

        _Database->commit();
    }
    catch(QException e)
    {
       _Database->rollback();
       QString errString = _Database->lastError().text();
       return false;
    }

    return true;

}

bool MailBoxEntity::createMail(MailHeader &rec, QString useremail)
{
    if(!rec.toList().contains(useremail))
    {
        return false;
    }

    QSqlQuery qryresult;
    QString err;

    QString sql;

    sql = _InsertSQL;

    if(rec.direction() == "I")
    {
        sql = sql.replace("EMailId__", useremail);
    }
    else
    {
        sql = sql.replace("EMailId__", rec.from());
    }

    sql = sql.replace("MessageId__", rec.messageId());
    sql = sql.replace("Subject__", rec.subject());
    sql = sql.replace("FromDisplayName__", rec.fromDisplayName());
    sql = sql.replace("FromUser__", rec.from());
    sql = sql.replace("ToList__", rec.to());
    sql = sql.replace("BCCList__", rec.bcc());
    sql = sql.replace("CCList__", rec.cc());
    sql = sql.replace("MailDirection__", rec.direction());
    sql = sql.replace("MailStatus__", rec.status());
    sql = sql.replace("MessageTimeStamp__", rec.timeStamp());

    try
    {
        _Database->transaction();

        qryresult = _Database->exec(sql);

        _Database->commit();
    }
    catch(QException e)
    {
       _Database->rollback();
       err = _Database->lastError().text();
       return false;
    }

    return true;
}

bool MailBoxEntity::updateMail(MailHeader &rec)
{
    QSqlQuery qryresult;
    QString err;

    QString sql;

    sql = _UpdateSQL;

    if(rec.direction() == "I")
    {
        sql = sql.replace("EMailId__", rec.toList().at(0));
    }
    else
    {
        sql = sql.replace("EMailId__", rec.from());
    }

    sql = sql.replace("MessageId__", rec.messageId());
    sql = sql.replace("Subject__", rec.subject());
    sql = sql.replace("FromDisplayName__", rec.fromDisplayName());
    sql = sql.replace("FromUser__", rec.from());
    sql = sql.replace("ToList__", rec.to());
    sql = sql.replace("BCCList__", rec.bcc());
    sql = sql.replace("CCList__", rec.cc());
    sql = sql.replace("MailDirection__", rec.direction());
    sql = sql.replace("MailStatus__", rec.status());
    sql = sql.replace("MessageTimeStamp__", rec.timeStamp());

    try
    {
        _Database->transaction();

        qryresult = _Database->exec(sql);

        _Database->commit();
    }
    catch(QException e)
    {
       _Database->rollback();
       err = _Database->lastError().text();
       return false;
    }

    return true;
}

bool MailBoxEntity::allMails(QList<MailHeader> &list)
{
    QList<QSqlRecord> *reclist = getAllRecords();

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        MailHeader struc;

        struc.setMessageId(rec.value(1).toString());
        struc.setSubject(rec.value(2).toString());
        struc.setFromDisplayName(rec.value(3).toString());
        struc.setFrom(rec.value(4).toString());
        struc.addtoToList(rec.value(5).toString());
        struc.addtoCcList(rec.value(6).toString());
        struc.addtoBccList(rec.value(7).toString());
        struc.setDirection(rec.value(8).toString());
        struc.setStatus(rec.value(9).toString());
        struc.setTimeStamp(rec.value(10).toString());

        list.append(struc);
    }

    return true;
}

bool MailBoxEntity::selectedMails(QList<MailHeader> &list, QString keyname, QVariant value)
{
    QList<QSqlRecord> *reclist = getSelectedRecords(keyname, value);


    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        MailHeader struc;

        struc.setMessageId(rec.value(1).toString());
        struc.setSubject(rec.value(2).toString());
        struc.setFromDisplayName(rec.value(3).toString());
        struc.setFrom(rec.value(4).toString());
        struc.addtoToList(rec.value(5).toString());
        struc.addtoCcList(rec.value(6).toString());
        struc.addtoBccList(rec.value(7).toString());
        struc.setDirection(rec.value(8).toString());
        struc.setStatus(rec.value(9).toString());
        struc.setTimeStamp(rec.value(10).toString());

        list.append(struc);
    }

    return true;

}

bool MailBoxEntity::removeMail(QString keyname, QVariant value)
{
    return remove(keyname, value);
}

