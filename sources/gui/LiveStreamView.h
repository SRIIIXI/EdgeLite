#ifndef _LIVE_STREAM_VIEW
#define _LIVE_STREAM_VIEW

#include <QMainWindow>
#include <QDateEdit>
#include <QTimeEdit>
#include <QWidget>
#include <QToolBar>
#include <QtCore>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTreeWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QAction>
#include <QFileDialog>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QAuthenticator>
//#include "StorageInterface.h"
//#include "RecordingStructures.h"
#include "ONVIFCamera.h"

extern int PoolSize;

class LiveStreamView : public QWidget
{
    Q_OBJECT
public:
    LiveStreamView(QWidget* parent = nullptr);
    void refreshCameraList();

private slots:
    void eventCameraSelected(int index);
    void exportRecordings();
    void searchRecordings();
    void selectAllRecordings();
    void deSelectAllRecordings();
    void clearAllRecordings();

//    void eventRootDirectoryReceived(QList<StorageDirectory> rootdlist);
//    void eventRecordsDirectoryReceived(QList<DailyRecordingSet> recdlist);
//    void eventRecordingsReceived(QList<RecordingSet> recdlist);
//    void eventStorageDetectionFailed(StorageInterface* cam);
//    void eventStorageDetectionSuceeded(StorageInterface* cam);

    void eventFinished(QNetworkReply *reply);
    void eventAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);

private:
    void setupCamSelectGroup();
    void setupFilterGroup();
    void setupSearchGroup();
    void setupExportGroup();

    void cleanUpStorageInterface();
    QString getStoragePath(QString str);

    void enableAll();
    void disableAll();

    QVBoxLayout _Layout;

    // 1
    QHBoxLayout _CamSelectLayout;
    QLabel _lblCam;
    QComboBox _cmbxCamList;
    // 1

    // 2
    QGridLayout _SearchLayout;

    QLabel _lblDate;
    QDateEdit _dtStartDate;
    QDateEdit _dtEndDate;

    QLabel _lblTimeRange1;
    QTimeEdit _tmStartTimeRange1;
    QTimeEdit _tmEndTimeRange1;

    QLabel _lblTimeRange2;
    QTimeEdit _tmStartTimeRange2;
    QTimeEdit _tmEndTimeRange2;

    QLabel _lblTimeRange3;
    QTimeEdit _tmStartTimeRange3;
    QTimeEdit _tmEndTimeRange3;

    QLabel _lblTimeRange4;
    QTimeEdit _tmStartTimeRange4;
    QTimeEdit _tmEndTimeRange4;
    // 2

    // 3
    QHBoxLayout  _SearchBar;
    QCheckBox   _chkSkip999;
    QPushButton _cmdSearch;
    // 3

    // 4
    QTreeWidget _RecordingList;
    // 4

    // 5
    QHBoxLayout _ExportLayout;
    QPushButton _cmdSelectAll;
    QPushButton _cmdDeSelectAll;
    QPushButton _cmdClearAll;
    QPushButton _cmdExport;
    // 5

    int _SetCount;
    int _RecordCount;
    int _PendingCount;
    int _FetchedCount;

    QString _CurrentCamName;
    ONVIFCamera _CurrentCam;
    QStringList _FetchList;

//    StorageInterface* _StoragePtr;
//    QList<StorageDirectory> _RootDirList;
//    QList<DailyRecordingSet> _RecDirList;
//    QList<RecordingSet> _RecSet;

    QNetworkAccessManager _ConnectionPool[5];
};


#endif
