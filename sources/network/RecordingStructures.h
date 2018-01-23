#ifndef _RECORDING_STRUCTURES
#define _RECORDING_STRUCTURES

#include <QtCore>

const int DirRoot = 0;
const int DirDailySet = 1;
const int DirRecords = 2;

typedef struct RecordingSet
{
    QString Size;
    QString Name;
    QDateTime TimeStamp;
    QString ParentDirectory;
}RecordingSet;

typedef struct DailyRecordingSet
{
    QString DirectoryName;
    QString SetName;
    QDateTime TimeStamp;
    QString ParentDirectory;
}DailyRecordingSet;

typedef struct StorageDirectory
{
    QString DirectoryName;
    QDateTime TimeStamp;
}StorageDirectory;

#endif
