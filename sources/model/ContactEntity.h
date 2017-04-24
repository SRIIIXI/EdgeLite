#ifndef CONTACT_H
#define CONTACT_H

#include <QObject>
#include <QtSql>
#include "DataEntity.h"

struct Contact
{
    QString ContactId;

    QString FirstName;
    QString MiddleName;
    QString LastName;
    QString NickName;
    QString EmailId;

    QString AddressLine1;
    QString AddressLine2;
    QString City;
    QString State;
    QString Country;
    QString ZipCode;

    QString WorkPhone;
    QString HomePhone;
    QString Fax;
    QString MobileNo;
    QString WebSite;

    QString Anniversary;
    QString Notes;
    QString Photo;
};

class ContactEntity : protected DataEntity
{
public:
    ContactEntity(QSqlDatabase* db, QString tabname);
    virtual ~ContactEntity();
    bool createContactTable();
    bool createContact(const Contact &rec);
    bool updateContact(const Contact &rec);
    bool allContacts(QList<Contact> &list);
    bool selectedContacts(QList<Contact> &list, QString keyname, QVariant value);
    bool removeContact(QString keyname, QVariant value);
    QMap<QString, Contact>* cache();
private:
    bool create(QSqlRecord record);
    bool update(QSqlRecord record);
    QMap<QString, Contact> _Cache;
};

extern ContactEntity* contactEntityPtr;

#endif
