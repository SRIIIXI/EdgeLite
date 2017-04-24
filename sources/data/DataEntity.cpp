#include "DataEntity.h"

DataEntity::DataEntity(QSqlDatabase *db, QString tabname)
{
    _Database = db;
    _TableName = tabname;
}

DataEntity::~DataEntity()
{
}

void DataEntity::clear()
{
    _RecordList.clear();
}

QList<QSqlRecord>* DataEntity::getAllRecords()
{
    _RecordList.clear();

    QString querystring = "select * from " + _TableName;

    QSqlQuery query = _Database->exec(querystring);

    while(query.next())
    {
        _RecordList.append(query.record());
    }

    return &_RecordList;
}

QList<QSqlRecord>* DataEntity::getSelectedRecords(QString keyname, QVariant value)
{
    _RecordList.clear();

    QString valsub = "";
    substitueValue(value, valsub);

    QString querystring = "select * from " + _TableName + " where " + keyname + " = " + valsub;

    QSqlQuery query = _Database->exec(querystring);

    while(query.next())
    {
        _RecordList.append(query.record());
    }

    return &_RecordList;
}

bool DataEntity::remove(QString keyname, QVariant value)
{
    QSqlQuery qryresult;
    QString err;
    QString valsub = "";

    substitueValue(value, valsub);

    QString querystring = "delete from " + _TableName + " where " + keyname + " = " + valsub;

    try
    {
        _Database->transaction();

        qryresult = _Database->exec(querystring);

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

QVariant DataEntity::maximumValue(QString keyname)
{
    QVariant retval;

    QString sqlstring = "select distinct _key_ from _table_ order by _key_  desc limit 1";
    sqlstring = sqlstring.replace("_table_", _TableName);
    sqlstring = sqlstring.replace("_key_", keyname);

    QSqlQuery query = _Database->exec(sqlstring);

    if(query.next())
    {
        retval = query.record().value(0);
    }

    return retval;
}

QVariant DataEntity::maximumValue(QString keyname, QString filter, QVariant filterval)
{
    QVariant retval;

    QString valsub = "";
    substitueValue(filterval, valsub);

    QString sqlstring = "select _key_ from _table_ where _filter_ = _filterval_ order by _key_ desc limit 1";
    sqlstring = sqlstring.replace("_table_", _TableName);
    sqlstring = sqlstring.replace("_key_", keyname);
    sqlstring = sqlstring.replace("_filter_", filter);
    sqlstring = sqlstring.replace("_filterval_", valsub);

    QSqlQuery query = _Database->exec(sqlstring);

    if(query.next())
    {
        retval = query.record().value(0);
    }

    return retval;
}

void DataEntity::substitueValue(QVariant &var, QString &str)
{
    switch(var.type())
    {
        case QVariant::Char:
        case QVariant::String:
        {
            str = "'" + var.toString() + "'";
        }
        break;

        case QVariant::Double:
        case QVariant::Int:
        case QVariant::LongLong:
        case QVariant::UInt:
        case QVariant::ULongLong:
        {
            str = var.toString();
        }
        break;

        default:
        {
        }
        break;
    }
}

