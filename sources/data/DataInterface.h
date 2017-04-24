#ifndef DATA_INTERFACE_H
#define DATA_INTERFACE_H

#include <QObject>
#include <QtSql>
#include <QStringList>
#include <QException>

class DataInterface
{
public:
    DataInterface();
    virtual ~DataInterface();
    void setParameters(QString dbfilename, QString dbname);
    bool exists();
    void setParameters(QSqlDriver::DbmsType dbtype, QString server, QString dbname, QString username, QString password, quint16 port);
    bool createFile(const QString &dbfilename);
    bool open();
    bool isOpen();
    void close();
    QSqlDatabase* getDatabase();
    bool contains(const QString &tabname);
private:
    QSqlDatabase            _Database;
    QString                 _Dbfilename;
    QString                 _Dbname;
    QString                 _Server;
    QString                 _Username;
    QString                 _Password;
    quint16                 _Port;
    QSqlDriver::DbmsType    _DbType;
    QStringList*            _TableList;
};

extern DataInterface* dataInterfacePtr;

#endif
