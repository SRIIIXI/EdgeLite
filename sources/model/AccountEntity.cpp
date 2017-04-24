#include "AccountEntity.h"

AccountEntity* accountEntityPtr = nullptr;

AccountEntity::AccountEntity(QSqlDatabase *db, QString tabname) : DataEntity(db, tabname)
{
    accountEntityPtr = this;
    _InsertSQL = "insert into Accounts (AccountName, EMailId, MailInUser, MailInPass, MailInServer, MailInPort, MailInSecurity, MailInProtocol, MailOutUser, MailOutPass, MailOutServer, MailOutPort, MailOutSecurity, MailOutUseHandshake) values ('AccountName__', 'EMailId__', 'MailInUser__', 'MailInPass__', 'MailInServer__', MailInPort__, 'MailInSecurity__', 'MailInProtocol__', 'MailOutUser__', 'MailOutPass__', 'MailOutServer__', MailOutPort__, 'MailOutSecurity__', 'MailOutUseHandshake__')";
    _UpdateSQL = "update Accounts set  AccountName = 'AccountName__', MailInUser =  'MailInUser__', MailInPass = 'MailInPass__',  MailInServer = 'MailInServer__', MailInPort = MailInPort__, MailInSecurity = 'MailInSecurity__', MailInProtocol = 'MailInProtocol__', MailOutUser = 'MailOutUser__', MailOutPass = 'MailOutPass__', MailOutServer = 'MailOutServer__', MailOutPort = MailOutPort__, MailOutSecurity = 'MailOutSecurity__', MailOutUseHandshake = 'MailOutUseHandshake__' where EMailId = 'EMailId__'";
}

AccountEntity::~AccountEntity()
{
}

bool AccountEntity::create(QSqlRecord record)
{
    return true;
}

bool AccountEntity::update(QSqlRecord record)
{
    return true;
}

bool AccountEntity::createAccountTable()
{
    QString sqlString = "create table Accounts \
            (	\
            AccountName varchar(32) not null, \
            EMailId varchar(64) not null, \
            MailInUser varchar(32) not null, \
            MailInPass varchar(32) not null, \
            MailInServer varchar(32) not null, \
            MailInPort integer not null, \
            MailInSecurity char(1) not null, \
            MailInProtocol char(1) not null, \
            MailOutUser varchar(32) not null, \
            MailOutPass varchar(32) not null, \
            MailOutServer varchar(32) not null, \
            MailOutPort integer not null, \
            MailOutSecurity char(1) not null, \
            MailOutUseHandshake char(1) not null, \
            primary key(EMailId) \
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

bool AccountEntity::createAccount(const Account &rec)
{
    QSqlQuery qryresult;
    QString err;

    QString sql;

    sql = _InsertSQL;

    sql = sql.replace("AccountName__", rec.AccountName);
    sql = sql.replace("EMailId__", rec.EMailId);
    sql = sql.replace("MailInUser__", rec.MailInUser);
    sql = sql.replace("MailInPass__", rec.MailInPass);
    sql = sql.replace("MailInServer__", rec.MailInServer);
    sql = sql.replace("MailInPort__", rec.MailInPort);
    sql = sql.replace("MailInSecurity__", rec.MailInSecurity);
    sql = sql.replace("MailInProtocol__", rec.MailInProtocol);
    sql = sql.replace("MailOutUser__", rec.MailOutUser);
    sql = sql.replace("MailOutPass__", rec.MailOutPass);
    sql = sql.replace("MailOutServer__", rec.MailOutServer);
    sql = sql.replace("MailOutPort__", rec.MailOutPort);
    sql = sql.replace("MailOutSecurity__", rec.MailOutSecurity);
    sql = sql.replace("MailOutUseHandshake__", rec.MailOutUseHandshake);

    try
    {
        _Database->transaction();

        qryresult = _Database->exec(sql);

        _Database->commit();

        _Cache.insert(rec.EMailId, rec);
    }
    catch(QException e)
    {
       _Database->rollback();
       err = _Database->lastError().text();
       return false;
    }

    return true;
}

bool AccountEntity::updateAccount(const Account &rec)
{
    QSqlQuery qryresult;
    QString err;

    QString sql;

    sql = _UpdateSQL;

    sql = sql.replace("AccountName__", rec.AccountName);
    sql = sql.replace("EMailId__", rec.EMailId);
    sql = sql.replace("MailInUser__", rec.MailInUser);
    sql = sql.replace("MailInPass__", rec.MailInPass);
    sql = sql.replace("MailInServer__", rec.MailInServer);
    sql = sql.replace("MailInPort__", rec.MailInPort);
    sql = sql.replace("MailInSecurity__", rec.MailInSecurity);
    sql = sql.replace("MailInProtocol__", rec.MailInProtocol);
    sql = sql.replace("MailOutUser__", rec.MailOutUser);
    sql = sql.replace("MailOutPass__", rec.MailOutPass);
    sql = sql.replace("MailOutServer__", rec.MailOutServer);
    sql = sql.replace("MailOutPort__", rec.MailOutPort);
    sql = sql.replace("MailOutSecurity__", rec.MailOutSecurity);
    sql = sql.replace("MailOutUseHandshake__", rec.MailOutUseHandshake);

    try
    {
        _Database->transaction();

        qryresult = _Database->exec(sql);

        _Database->commit();

        _Cache[rec.EMailId] = rec;
    }
    catch(QException e)
    {
       _Database->rollback();
       err = _Database->lastError().text();
       return false;
    }

    return true;
}

bool AccountEntity::allAccounts(QList<Account> &list)
{
    _Cache.clear();

    QList<QSqlRecord> *reclist = getAllRecords();

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        Account struc;

        struc.AccountName = rec.value(0).toString();
        struc.EMailId = rec.value(1).toString();
        struc.MailInUser = rec.value(2).toString();
        struc.MailInPass = rec.value(3).toString();
        struc.MailInServer = rec.value(4).toString();
        struc.MailInPort = rec.value(5).toString();
        struc.MailInSecurity = rec.value(6).toString();
        struc.MailInProtocol = rec.value(7).toString();
        struc.MailOutUser = rec.value(8).toString();
        struc.MailOutPass = rec.value(9).toString();
        struc.MailOutServer = rec.value(10).toString();
        struc.MailOutPort = rec.value(11).toString();
        struc.MailOutSecurity = rec.value(12).toString();
        struc.MailOutUseHandshake = rec.value(13).toString();

        list.append(struc);

        _Cache.insert(struc.EMailId, struc);
    }

    return true;
}

bool AccountEntity::selectedAccounts(QList<Account> &list, QString keyname, QVariant value)
{
    QList<QSqlRecord> *reclist = getSelectedRecords(keyname, value);


    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        Account struc;

        struc.AccountName = rec.value(0).toString();
        struc.EMailId = rec.value(1).toString();
        struc.MailInUser = rec.value(2).toString();
        struc.MailInPass = rec.value(3).toString();
        struc.MailInServer = rec.value(4).toString();
        struc.MailInPort = rec.value(5).toString();
        struc.MailInSecurity = rec.value(6).toString();
        struc.MailInProtocol = rec.value(7).toString();
        struc.MailOutUser = rec.value(8).toString();
        struc.MailOutPass = rec.value(9).toString();
        struc.MailOutServer = rec.value(10).toString();
        struc.MailOutPort = rec.value(11).toString();
        struc.MailOutSecurity = rec.value(12).toString();
        struc.MailOutUseHandshake = rec.value(13).toString();

        list.append(struc);
    }

    return true;

}

bool AccountEntity::removeAccount(QString keyname, QVariant value)
{
    bool ret = false;
    ret = remove(keyname, value);

    if(ret)
    {
        _Cache.remove(value.toString());
    }

    return ret;
}

QMap<QString, Account>* AccountEntity::cache()
{
    if(_Cache.count() < 1)
    {
        QList<QSqlRecord> *reclist = getAllRecords();

        for(int ctr = 0; ctr < reclist->count(); ctr++)
        {
            QSqlRecord rec = reclist->at(ctr);

            Account struc;

            struc.AccountName = rec.value(0).toString();
            struc.EMailId = rec.value(1).toString();
            struc.MailInUser = rec.value(2).toString();
            struc.MailInPass = rec.value(3).toString();
            struc.MailInServer = rec.value(4).toString();
            struc.MailInPort = rec.value(5).toString();
            struc.MailInSecurity = rec.value(6).toString();
            struc.MailOutUser = rec.value(7).toString();
            struc.MailOutPass = rec.value(8).toString();
            struc.MailOutServer = rec.value(9).toString();
            struc.MailOutPort = rec.value(10).toString();
            struc.MailOutSecurity = rec.value(11).toString();
            struc.MailOutUseHandshake = rec.value(12).toString();

            _Cache.insert(struc.EMailId, struc);
        }

    }

    return &_Cache;
}

