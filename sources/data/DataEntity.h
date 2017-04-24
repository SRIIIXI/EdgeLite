#ifndef DATA_ENTITY_H
#define DATA_ENTITY_H

#include <QObject>
#include <QtSql>

class DataEntity
{
public:
    DataEntity(QSqlDatabase* db, QString tabname);
    virtual ~DataEntity();
    void clear();
    QList<QSqlRecord>* getAllRecords();
    QList<QSqlRecord>* getSelectedRecords(QString keyname, QVariant value);
    bool remove(QString keyname, QVariant value);
    virtual bool create(QSqlRecord record)=0;
    virtual bool update(QSqlRecord record)=0;
    QVariant maximumValue(QString keyname);
    QVariant maximumValue(QString keyname, QString filter, QVariant filterval);
private:
    void substitueValue(QVariant &var, QString &str);
protected:
    QSqlDatabase*       _Database;
    QString             _TableName;
    QList<QSqlRecord>   _RecordList;
    QString _InsertSQL;
    QString _UpdateSQL;
};

#endif
