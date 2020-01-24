#ifndef _RECORDING
#define _RECORDING

#include <QString>
#include <QDateTime>
#include <QChar>
#include <QMap>
#include <QVariant>
#include <QList>

typedef struct Recording
{
    QString	site_id;
    QString	request_id;
    QString	camera_id;
    QString	camera_file_uri;
    QString	storage_file_uri;
    QString	request_timestamp;
    QString	request_status;
    QString	file_size;
    QString	file_timestamp;
}Recording;

#endif
