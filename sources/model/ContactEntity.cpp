#include "ContactEntity.h"

ContactEntity* contactEntityPtr = nullptr;

ContactEntity::ContactEntity(QSqlDatabase *db, QString tabname) : DataEntity(db, tabname)
{
    contactEntityPtr = this;

    _InsertSQL = "insert into Contacts (ContactId, FirstName, MiddleName, LastName, EmailId, WebSite, AddressLine1, AddressLine2, City, State, Country, ZipCode, WorkPhone, HomePhone, Fax, MobileNo, NickName, Anniversary, Notes, Photo) values ('ContactId__', 'FirstName__', 'MiddleName__', 'LastName__', 'EmailId__', 'WebSite__', 'AddressLine1__', 'AddressLine2__', 'City__', 'State__', 'Country__', 'ZipCode__', 'WorkPhone__', 'HomePhone__', 'Fax__', 'MobileNo__', 'NickName__', 'Anniversary__', 'Notes__', 'Photo__')";

    _UpdateSQL = "update Contacts set  FirstName = 'FirstName__',  MiddleName = 'MiddleName__', LastName = 'LastName__', EmailId = 'EmailId__', WebSite = 'WebSite__', AddressLine1 = 'AddressLine1__', AddressLine2 = 'AddressLine2__', City = 'City__', State = 'State__', Country = 'Country__', ZipCode = 'ZipCode__', WorkPhone = 'WorkPhone__', HomePhone = 'HomePhone__', Fax = 'Fax__', MobileNo = 'MobileNo__', NickName = 'NickName__', Anniversary = 'Anniversary__', Notes = 'Notes__', Photo = 'Photo__' where ContactId = 'ContactId__'";
}

ContactEntity::~ContactEntity()
{
}

bool ContactEntity::create(QSqlRecord record)
{
    return true;
}

bool ContactEntity::update(QSqlRecord record)
{
    return true;
}

bool ContactEntity::createContactTable()
{
    QString sqlString = "create table Contacts \
            (	\
            ContactId varchar(32) not null, \
            FirstName varchar(32) not null, \
            MiddleName varchar(32), \
            LastName varchar(32), \
            EmailId varchar(32) not null, \
            WebSite varchar(32), \
            AddressLine1 varchar(32), \
            AddressLine2 varchar(32), \
            City varchar(32), \
            State varchar(32), \
            Country varchar(32), \
            ZipCode varchar(32), \
            WorkPhone varchar(32), \
            HomePhone varchar(32), \
            Fax varchar(32), \
            MobileNo varchar(32), \
            NickName varchar(32), \
            Anniversary datetime, \
            Notes varchar(128), \
            Photo varchar(1024), \
            primary key(ContactId) \
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

bool ContactEntity::createContact(const Contact &rec)
{
    QSqlQuery qryresult;
    QString err;

    uint32_t contactid = maximumValue("ContactId").toUInt();
    contactid++;
    QVariant contactidstr(contactid);

    QString sql;

    sql = _InsertSQL;

    sql = sql.replace("ContactId__", contactidstr.toString());
    sql = sql.replace("FirstName__", rec.FirstName);
    sql = sql.replace("MiddleName__", rec.MiddleName);
    sql = sql.replace("LastName__", rec.LastName);
    sql = sql.replace("EmailId__", rec.EmailId);
    sql = sql.replace("WebSite__", rec.WebSite);
    sql = sql.replace("AddressLine1__", rec.AddressLine1);
    sql = sql.replace("AddressLine2__", rec.AddressLine2);
    sql = sql.replace("City__", rec.City);
    sql = sql.replace("State__", rec.State);
    sql = sql.replace("Country__", rec.Country);
    sql = sql.replace("ZipCode__", rec.ZipCode);
    sql = sql.replace("WorkPhone__", rec.WorkPhone);
    sql = sql.replace("HomePhone__", rec.HomePhone);
    sql = sql.replace("Fax__", rec.Fax);
    sql = sql.replace("MobileNo__", rec.MobileNo);
    sql = sql.replace("NickName__", rec.NickName);
    sql = sql.replace("Anniversary__", rec.Anniversary);
    sql = sql.replace("Notes__", rec.Notes);
    sql = sql.replace("Photo__", rec.Photo);

    try
    {
        _Database->transaction();

        qryresult = _Database->exec(sql);

        _Database->commit();

        _Cache.insert(rec.EmailId, rec);
    }
    catch(QException e)
    {
       _Database->rollback();
       err = _Database->lastError().text();
       return false;
    }

    return true;
}

bool ContactEntity::updateContact(const Contact &rec)
{
    QSqlQuery qryresult;
    QString err;

    QString sql;

    sql = _UpdateSQL;

    sql = sql.replace("ContactId__", rec.ContactId);
    sql = sql.replace("FirstName__", rec.FirstName);
    sql = sql.replace("MiddleName__", rec.MiddleName);
    sql = sql.replace("LastName__", rec.LastName);
    sql = sql.replace("EmailId__", rec.EmailId);
    sql = sql.replace("WebSite__", rec.WebSite);
    sql = sql.replace("AddressLine1__", rec.AddressLine1);
    sql = sql.replace("AddressLine2__", rec.AddressLine2);
    sql = sql.replace("City__", rec.City);
    sql = sql.replace("State__", rec.State);
    sql = sql.replace("Country__", rec.Country);
    sql = sql.replace("ZipCode__", rec.ZipCode);
    sql = sql.replace("WorkPhone__", rec.WorkPhone);
    sql = sql.replace("HomePhone__", rec.HomePhone);
    sql = sql.replace("Fax__", rec.Fax);
    sql = sql.replace("MobileNo__", rec.MobileNo);
    sql = sql.replace("NickName__", rec.NickName);
    sql = sql.replace("Anniversary__", rec.Anniversary);
    sql = sql.replace("Notes__", rec.Notes);
    sql = sql.replace("Photo__", rec.Photo);

    try
    {
        _Database->transaction();

        qryresult = _Database->exec(sql);

        _Database->commit();

        _Cache[rec.EmailId] = rec;
    }
    catch(QException e)
    {
       _Database->rollback();
       err = _Database->lastError().text();
       return false;
    }

    return true;
}

bool ContactEntity::allContacts(QList<Contact> &list)
{
    QList<QSqlRecord> *reclist = getAllRecords();

    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        Contact struc;

        struc.ContactId = rec.value(0).toString();
        struc.FirstName = rec.value(1).toString();
        struc.MiddleName = rec.value(2).toString();
        struc.LastName = rec.value(3).toString();
        struc.EmailId = rec.value(4).toString();
        struc.WebSite = rec.value(5).toString();
        struc.AddressLine1 = rec.value(6).toString();
        struc.AddressLine2 = rec.value(7).toString();
        struc.City = rec.value(8).toString();
        struc.State = rec.value(9).toString();
        struc.Country = rec.value(10).toString();
        struc.ZipCode = rec.value(11).toString();
        struc.WorkPhone = rec.value(12).toString();
        struc.HomePhone = rec.value(13).toString();
        struc.Fax = rec.value(14).toString();
        struc.MobileNo = rec.value(15).toString();
        struc.NickName = rec.value(16).toString();
        struc.Anniversary = rec.value(17).toString();
        struc.Notes = rec.value(18).toString();
        struc.Photo = rec.value(19).toString();

        list.append(struc);

        _Cache.insert(struc.EmailId, struc);

    }

    return true;
}

bool ContactEntity::selectedContacts(QList<Contact> &list, QString keyname, QVariant value)
{
    QList<QSqlRecord> *reclist = getSelectedRecords(keyname, value);


    for(int ctr = 0; ctr < reclist->count(); ctr++)
    {
        QSqlRecord rec = reclist->at(ctr);

        Contact struc;

        struc.ContactId = rec.value(0).toString();
        struc.FirstName = rec.value(1).toString();
        struc.MiddleName = rec.value(2).toString();
        struc.LastName = rec.value(3).toString();
        struc.EmailId = rec.value(4).toString();
        struc.WebSite = rec.value(5).toString();
        struc.AddressLine1 = rec.value(6).toString();
        struc.AddressLine2 = rec.value(7).toString();
        struc.City = rec.value(8).toString();
        struc.State = rec.value(9).toString();
        struc.Country = rec.value(10).toString();
        struc.ZipCode = rec.value(11).toString();
        struc.WorkPhone = rec.value(12).toString();
        struc.HomePhone = rec.value(13).toString();
        struc.Fax = rec.value(14).toString();
        struc.MobileNo = rec.value(15).toString();
        struc.NickName = rec.value(16).toString();
        struc.Anniversary = rec.value(17).toString();
        struc.Notes = rec.value(18).toString();
        struc.Photo = rec.value(19).toString();

        list.append(struc);
    }

    return true;

}

bool ContactEntity::removeContact(QString keyname, QVariant value)
{
    bool ret = false;
    ret = remove(keyname, value);

    if(ret)
    {
        _Cache.remove(value.toString());
    }

    return ret;
}

QMap<QString, Contact>* ContactEntity::cache()
{
    if(_Cache.count() < 1)
    {
        QList<QSqlRecord> *reclist = getAllRecords();

        for(int ctr = 0; ctr < reclist->count(); ctr++)
        {
            QSqlRecord rec = reclist->at(ctr);

            Contact struc;

            struc.ContactId = rec.value(0).toString();
            struc.FirstName = rec.value(1).toString();
            struc.MiddleName = rec.value(2).toString();
            struc.LastName = rec.value(3).toString();
            struc.EmailId = rec.value(4).toString();
            struc.WebSite = rec.value(5).toString();
            struc.AddressLine1 = rec.value(6).toString();
            struc.AddressLine2 = rec.value(7).toString();
            struc.City = rec.value(8).toString();
            struc.State = rec.value(9).toString();
            struc.Country = rec.value(10).toString();
            struc.ZipCode = rec.value(11).toString();
            struc.WorkPhone = rec.value(12).toString();
            struc.HomePhone = rec.value(13).toString();
            struc.Fax = rec.value(14).toString();
            struc.MobileNo = rec.value(15).toString();
            struc.NickName = rec.value(16).toString();
            struc.Anniversary = rec.value(17).toString();
            struc.Notes = rec.value(18).toString();
            struc.Photo = rec.value(19).toString();

            _Cache.insert(struc.EmailId, struc);
        }

    }

    return &_Cache;
}

