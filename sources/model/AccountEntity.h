#ifndef _ACCOUNT
#define _ACCOUNT

#include <QObject>
#include <QtSql>
#include <QList>
#include <QMap>
#include "DataEntity.h"

struct Account
{
    QString AccountName;
    QString EMailId;

    QString MailInUser;
    QString MailInPass;
    QString MailInServer;
    QString MailInPort;
    QString MailInSecurity;
    QString MailInProtocol;

    QString MailOutUser;
    QString MailOutPass;
    QString MailOutServer;
    QString MailOutPort;
    QString MailOutSecurity;
    QString MailOutUseHandshake;
};

class AccountEntity : protected DataEntity
{
public:
    AccountEntity(QSqlDatabase* db, QString tabname);
    virtual ~AccountEntity();
    bool createAccountTable();
    bool createAccount(const Account &rec);
    bool updateAccount(const Account &rec);
    bool allAccounts(QList<Account> &list);
    bool selectedAccounts(QList<Account> &list, QString keyname, QVariant value);
    bool removeAccount(QString keyname, QVariant value);
    QMap<QString, Account>* cache();
private:
    bool create(QSqlRecord record);
    bool update(QSqlRecord record);
    QMap<QString, Account> _Cache;
};

extern AccountEntity* accountEntityPtr;

#endif
