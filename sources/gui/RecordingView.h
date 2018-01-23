#ifndef _RECORDING_VIEW
#define _RECORDING_VIEW

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
#include "ServiceInterface.h"
#include "RecordingStructures.h"

class RecordingView : public QWidget
{
    Q_OBJECT
public:
    RecordingView(QWidget* parent = nullptr);
    void refreshCameraList();

private slots:
    void eventCameraSelected(int index);
    void exportRecordings();
    void searchRecordings();
    void selectAllRecordings();
    void clearAllRecordings();

//    void eventRootDirectoryReceived(QList<StorageDirectory> rootdlist);
//    void eventRecordsDirectoryReceived(QList<DailyRecordingSet> recdlist);
//    void eventRecordingsReceived(QList<RecordingSet> recdlist);

private:
    void setupCamSelectGroup();
    void setupSearchGroup();
    void setupExportGroup();

    QString _CurrentCam;
    ServiceInterface* _ServicePtr;

    QList<StorageDirectory> _RootDirList;
    QList<DailyRecordingSet> _RecDirList;
    QList<RecordingSet> _RecSet;

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

    QCheckBox   _chkSkip999;
    QPushButton _cmdSearch;
    // 2

    QTreeWidget _RecordingList;

    // 3
    QHBoxLayout _ExportLayout;
    QPushButton _cmdSelectAll;
    QPushButton _cmdClearAll;
    QPushButton _cmdExport;
    // 3
};

#endif
