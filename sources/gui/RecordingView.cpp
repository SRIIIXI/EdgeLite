#include "RecordingView.h"
#include "ThemeManager.h"
#include "CustomWidgets.h"
#include "Tracks.h"
#include "DownloadManager.h"

RecordingView::RecordingView(QWidget *parent) : QWidget(parent)
{
    _ServicePtr = nullptr;

    QStringList reclst;
    reclst << "Camera"  << "Filename" << "Date and Time" << "Size";
    _RecordingList.setFrameStyle(QFrame::NoFrame);
    _RecordingList.setHeaderLabels(reclst);
    _RecordingList.setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);

    _Layout.addWidget(new HorizontalLine());

    setupCamSelectGroup();

    _Layout.addWidget(new HorizontalLine());

    setupSearchGroup();

    _Layout.addWidget(new HorizontalLine());

    _Layout.addWidget(&_RecordingList, Qt::AlignLeft | Qt::AlignTop);

    _Layout.addWidget(new HorizontalLine());

    setupExportGroup();

    _Layout.setMargin(0);
    _Layout.setSpacing(0);

    setLayout(&_Layout);
}

void RecordingView::setupCamSelectGroup()
{
    _lblCam.setText("Camera List");
    _CamSelectLayout.addWidget(&_lblCam,               Qt::AlignLeft);
    _CamSelectLayout.addWidget(&_cmbxCamList,          Qt::AlignLeft);
    _CamSelectLayout.addSpacing(10);
    _CamSelectLayout.addStretch();
    _CamSelectLayout.setMargin(5);
    _Layout.addLayout(&_CamSelectLayout);

    QObject::connect(&_cmbxCamList, SIGNAL(currentIndexChanged(int)), this, SLOT(eventCameraSelected(int)));
}

void RecordingView::setupSearchGroup()
{
    _dtStartDate.setCalendarPopup(true);
    _dtEndDate.setCalendarPopup(true);

    _lblDate.setText("Date Range");
    _lblTimeRange1.setText("Time Range #1");
    _lblTimeRange2.setText("Time Range #2");
    _lblTimeRange3.setText("Time Range #3");
    _lblTimeRange4.setText("Time Range #4");


    _chkSkip999.setText("Skip Corrupt Files");
    _cmdSearch.setText("Search");

    _SearchLayout.addWidget(&_lblDate,      0, 0, Qt::AlignLeft);
    _SearchLayout.addWidget(&_dtStartDate,  0, 2, Qt::AlignLeft);
    _SearchLayout.addWidget(&_dtEndDate,    0, 3, Qt::AlignLeft);

    _SearchLayout.addWidget(&_lblTimeRange1,      1, 1, Qt::AlignLeft);
    _SearchLayout.addWidget(&_tmStartTimeRange1,  1, 2, Qt::AlignLeft);
    _SearchLayout.addWidget(&_tmEndTimeRange1,    1, 3, Qt::AlignLeft);

    _SearchLayout.addWidget(&_lblTimeRange2,      2, 1, Qt::AlignLeft);
    _SearchLayout.addWidget(&_tmStartTimeRange2,  2, 2, Qt::AlignLeft);
    _SearchLayout.addWidget(&_tmEndTimeRange2,    2, 3, Qt::AlignLeft);

    _SearchLayout.addWidget(&_lblTimeRange3,      3, 1, Qt::AlignLeft);
    _SearchLayout.addWidget(&_tmStartTimeRange3,  3, 2, Qt::AlignLeft);
    _SearchLayout.addWidget(&_tmEndTimeRange3,    3, 3, Qt::AlignLeft);

    _SearchLayout.addWidget(&_lblTimeRange4,      4, 1, Qt::AlignLeft);
    _SearchLayout.addWidget(&_tmStartTimeRange4,  4, 2, Qt::AlignLeft);
    _SearchLayout.addWidget(&_tmEndTimeRange4,    4, 3, Qt::AlignLeft);

    _SearchLayout.addWidget(&_chkSkip999,   5, 2, Qt::AlignLeft);
    _SearchLayout.addWidget(&_cmdSearch,    5, 3, Qt::AlignLeft);

    _SearchLayout.setMargin(5);
    _SearchLayout.setSpacing(5);
    _Layout.addLayout(&_SearchLayout);
    connect(&_cmdSearch, &QPushButton::clicked, this, &RecordingView::searchRecordings);
}

void RecordingView::setupExportGroup()
{
    _cmdExport.setMaximumWidth(75);
    _cmdExport.setText("Export");
    _cmdSelectAll.setMaximumWidth(75);
    _cmdSelectAll.setText("Select All");
    _cmdClearAll.setMaximumWidth(75);
    _cmdClearAll.setText("Clear All");

    _ExportLayout.addStretch();
    _ExportLayout.addWidget(&_cmdExport, Qt::AlignRight);
    _ExportLayout.addWidget(&_cmdSelectAll, Qt::AlignRight);
    _ExportLayout.addWidget(&_cmdClearAll, Qt::AlignRight);
    _ExportLayout.setMargin(5);
    _ExportLayout.setSpacing(5);
    _Layout.addLayout(&_ExportLayout);
    connect(&_cmdExport, &QPushButton::clicked, this, &RecordingView::exportRecordings);
    connect(&_cmdSelectAll, &QPushButton::clicked, this, &RecordingView::selectAllRecordings);
    connect(&_cmdClearAll, &QPushButton::clicked, this, &RecordingView::clearAllRecordings);
}


void RecordingView::refreshCameraList()
{
    _cmbxCamList.clear();

    QList<QString> camlist = tracksPtr->cameraCollection();

    if(camlist.count() > 0)
    {
        _cmbxCamList.addItems(camlist);
    }

    if(_cmbxCamList.count() >= 1)
    {
        eventCameraSelected(0);
    }
}

void RecordingView::eventCameraSelected(int index)
{
    _CurrentCam = _cmbxCamList.itemText(index);
}

void RecordingView::exportRecordings()
{
    _cmdClearAll.setEnabled(false);
    _cmdSelectAll.setEnabled(false);
    _cmdExport.setEnabled(false);

    DownloadManager::GetInstance()->reset();
    DownloadManager::GetInstance()->setDownloadDirectory(tracksPtr->recordingsDirectory());

    for(int i = 0; i < _RecordingList.topLevelItemCount(); i++ )
    {
        if(_RecordingList.topLevelItem(i)->checkState(0) == Qt::Checked)
        {
            DownloadManager::GetInstance()->append(_RecordingList.topLevelItem(i)->data(1, Qt::UserRole).toString());
        }
    }

    _cmdClearAll.setEnabled(true);
    _cmdSelectAll.setEnabled(true);
    _cmdExport.setEnabled(true);
}

void RecordingView::searchRecordings()
{
    _ServicePtr = nullptr;

    _ServicePtr = tracksPtr->getServiceInterface(_CurrentCam);

    if(_ServicePtr != nullptr)
    {
        _ServicePtr->getRecordings();
    }
}

void RecordingView::selectAllRecordings()
{
    _cmdClearAll.setEnabled(false);
    _cmdSelectAll.setEnabled(false);
    _cmdExport.setEnabled(false);

    for(int i = 0; i < _RecordingList.topLevelItemCount(); i++ )
    {
      _RecordingList.topLevelItem(i)->setCheckState(0, Qt::Checked);
    }

    _cmdClearAll.setEnabled(true);
    _cmdSelectAll.setEnabled(true);
    _cmdExport.setEnabled(true);
}

void RecordingView::clearAllRecordings()
{
    _cmdClearAll.setEnabled(false);
    _cmdSelectAll.setEnabled(false);
    _cmdExport.setEnabled(false);

    for(int i = 0; i < _RecordingList.topLevelItemCount(); i++ )
    {
      _RecordingList.topLevelItem(i)->setCheckState(0, Qt::Unchecked);
    }

    _cmdClearAll.setEnabled(true);
    _cmdSelectAll.setEnabled(true);
    _cmdExport.setEnabled(true);
}

//void RecordingView::eventRootDirectoryReceived(QList<StorageDirectory> rootdlist)
//{
//    _RootDirList = rootdlist;
//    foreach (StorageDirectory rstg, _RootDirList)
//    {
//         _StoragePtr->getDailyRecordingSets(rstg);
//    }
//}

//void RecordingView::eventRecordsDirectoryReceived(QList<DailyRecordingSet> recdlist)
//{
//    _RecDirList = recdlist;

//    foreach (DailyRecordingSet dstg, _RecDirList)
//    {
//         _StoragePtr->getRecordingSets(dstg);
//    }
//}

//void RecordingView::eventRecordingsReceived(QList<RecordingSet> recdlist)
//{
//    _RecSet = recdlist;

//    foreach (RecordingSet recstg, _RecSet)
//    {
//        if(recstg.TimeStamp >= _dtStartDate.dateTime() && recstg.TimeStamp <= _dtEndDate.dateTime())
//        {
//            QString remotefile = recstg.ParentDirectory + "/" + recstg.Name;
//            QTreeWidgetItem *recItem = new QTreeWidgetItem(&_RecordingList);
//            recItem->setText(0, _StoragePtr->name());
//            recItem->setText(1, recstg.Name);
//            recItem->setText(2, recstg.TimeStamp.toString("ddd, dd-MMM-yyyy hh:mm:ss"));
//            recItem->setText(3, recstg.Size);
//            recItem->setData(1, Qt::UserRole, QVariant(remotefile));
//            recItem->setCheckState(0, Qt::Unchecked);
//        }
//    }
//}
