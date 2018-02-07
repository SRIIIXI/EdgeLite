#include "LiveStreamView.h"
#include "ThemeManager.h"
#include "CustomWidgets.h"
#include "Tracks.h"
#include "MainWindow.h"

int PoolSize = 5;

LiveStreamView::LiveStreamView(QWidget *parent) : QWidget(parent)
{
    //_StoragePtr = nullptr;
    _SetCount = 0;
    _RecordCount = 0;
    _PendingCount = 0;

    QStringList reclst;
    reclst << "Camera"  << "Filename" << "Date and Time" << "Size";
    _RecordingList.setFrameStyle(QFrame::NoFrame);
    _RecordingList.setHeaderLabels(reclst);
    _RecordingList.setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    _RecordingList.setColumnWidth(0, 250);
    _RecordingList.setColumnWidth(1, 250);
    _RecordingList.setColumnWidth(2, 200);

    _Layout.addWidget(new HorizontalLine());

    setupCamSelectGroup();

    _Layout.addWidget(new HorizontalLine());

    setupFilterGroup();

    _Layout.addWidget(new HorizontalLine());

    setupSearchGroup();

    _Layout.addWidget(new HorizontalLine());

    _Layout.addWidget(&_RecordingList, Qt::AlignLeft | Qt::AlignTop);

    _Layout.addWidget(new HorizontalLine());

    setupExportGroup();

    _Layout.addWidget(new HorizontalLine());

    _Layout.setMargin(0);
    _Layout.setSpacing(0);

    setLayout(&_Layout);

    for(int ctr = 0; ctr < 5; ctr++)
    {
        QObject::connect(&_ConnectionPool[ctr], SIGNAL(finished(QNetworkReply*)), this, SLOT(eventFinished(QNetworkReply*)));
        QObject::connect(&_ConnectionPool[ctr], SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)), this, SLOT(eventAuthenticationRequired(QNetworkReply*, QAuthenticator*)));
    }
}

void LiveStreamView::setupCamSelectGroup()
{
    _cmbxCamList.setMaximumWidth(250);
    _lblCam.setMaximumWidth(100);
    _lblCam.setText("Camera List");
    _CamSelectLayout.addWidget(&_lblCam,               Qt::AlignLeft);
    _CamSelectLayout.addWidget(&_cmbxCamList,          Qt::AlignLeft);
    _CamSelectLayout.addStretch();
    _CamSelectLayout.setSpacing(5);
    _CamSelectLayout.setMargin(5);
    _Layout.addLayout(&_CamSelectLayout);

    QObject::connect(&_cmbxCamList, SIGNAL(currentIndexChanged(int)), this, SLOT(eventCameraSelected(int)));
}

void LiveStreamView::setupFilterGroup()
{
    _dtStartDate.setCalendarPopup(true);
    _dtEndDate.setCalendarPopup(true);

    _lblDate.setText("Date Range");
    _lblTimeRange1.setText("Time Range #1");
    _lblTimeRange2.setText("Time Range #2");
    _lblTimeRange3.setText("Time Range #3");
    _lblTimeRange4.setText("Time Range #4");

    QDateTime dt = QDateTime::currentDateTime();
    _dtEndDate.setDateTime(dt);
    dt = dt.addDays(-7);
    _dtStartDate.setDateTime(dt);

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

    _SearchLayout.setMargin(5);
    _SearchLayout.setSpacing(5);
    _Layout.addLayout(&_SearchLayout);
}

void LiveStreamView::setupSearchGroup()
{
    _chkSkip999.setText("Skip Corrupt Files");
    _cmdSearch.setText("Search");
    _cmdSearch.setMaximumWidth(75);

    _SearchBar.addStretch();
    _SearchBar.addWidget(&_chkSkip999,  Qt::AlignRight);
    _SearchBar.addWidget(&_cmdSearch,   Qt::AlignRight);
    _SearchBar.setMargin(5);
    _SearchBar.setSpacing(5);

    _Layout.addLayout(&_SearchBar);
    connect(&_cmdSearch, &QPushButton::clicked, this, &LiveStreamView::searchRecordings);
}

void LiveStreamView::setupExportGroup()
{
    _cmdExport.setMaximumWidth(75);
    _cmdExport.setText("Export");
    _cmdSelectAll.setMaximumWidth(75);
    _cmdSelectAll.setText("Select All");
    _cmdDeSelectAll.setMaximumWidth(75);
    _cmdDeSelectAll.setText("De Select All");
    _cmdClearAll.setMaximumWidth(75);
    _cmdClearAll.setText("Clear All");

    _ExportLayout.addStretch();
    _ExportLayout.addWidget(&_cmdExport, Qt::AlignRight);
    _ExportLayout.addWidget(&_cmdSelectAll, Qt::AlignRight);
    _ExportLayout.addWidget(&_cmdDeSelectAll, Qt::AlignRight);
    _ExportLayout.addWidget(&_cmdClearAll, Qt::AlignRight);
    _ExportLayout.setMargin(5);
    _ExportLayout.setSpacing(5);
    _Layout.addLayout(&_ExportLayout);
    connect(&_cmdExport, &QPushButton::clicked, this, &LiveStreamView::exportRecordings);
    connect(&_cmdSelectAll, &QPushButton::clicked, this, &LiveStreamView::selectAllRecordings);
    connect(&_cmdDeSelectAll, &QPushButton::clicked, this, &LiveStreamView::deSelectAllRecordings);
    connect(&_cmdClearAll, &QPushButton::clicked, this, &LiveStreamView::clearAllRecordings);
}


void LiveStreamView::refreshCameraList()
{
    _cmbxCamList.clear();

    QList<QString> camlist = tracksPtr->getCameraNames();

    if(camlist.count() > 0)
    {
        _cmbxCamList.addItems(camlist);
    }

    if(_cmbxCamList.count() >= 1)
    {
        eventCameraSelected(0);
    }
}

void LiveStreamView::eventCameraSelected(int index)
{
    _CurrentCamName = _cmbxCamList.itemText(index);
    _CurrentCam = tracksPtr->getCamera(_CurrentCamName);
    QDateTime dt = QDateTime::currentDateTime();
    _dtEndDate.setDateTime(dt);
    dt = dt.addDays(-7);
    _dtStartDate.setDateTime(dt);

    _tmStartTimeRange1.setTime(QTime(00, 00, 00));
    _tmEndTimeRange1.setTime(QTime(05, 59, 59));

    _tmStartTimeRange2.setTime(QTime(06, 00, 00));
    _tmEndTimeRange2.setTime(QTime(11, 59, 59));

    _tmStartTimeRange3.setTime(QTime(12, 00, 00));
    _tmEndTimeRange3.setTime(QTime(17, 59, 59));

    _tmStartTimeRange4.setTime(QTime(18, 00, 00));
    _tmEndTimeRange4.setTime(QTime(23, 59, 59));
}

void LiveStreamView::searchRecordings()
{
    disableAll();

    _SetCount = 0;
    _RecordCount = 0;
    _PendingCount = 0;

    mainWindowPtr->setStatusMessage("Searching recordings ...");
    mainWindowPtr->setProgressMessage("Trying to connect ...");

//    _StoragePtr = new StorageInterface();

//    _StoragePtr->setServer(_CurrentCam.IpAddress);
//    _StoragePtr->setPort(_CurrentCam.AlternatePort);
//    _StoragePtr->setUser(_CurrentCam.ServiceUser);
//    _StoragePtr->setPassword(_CurrentCam.Password);
//    _StoragePtr->setStorageDirectory(ipbumPtr->recordingsDirectory());

//    connect(_StoragePtr, &StorageInterface::storageDetectionSuceeded, this, &LiveStreamView::eventStorageDetectionSuceeded);
//    connect(_StoragePtr, &StorageInterface::rootDirectoryReceived, this, &LiveStreamView::eventRootDirectoryReceived);
//    connect(_StoragePtr, &StorageInterface::recordsDirectoryReceived, this, &LiveStreamView::eventRecordsDirectoryReceived);
//    connect(_StoragePtr, &StorageInterface::recordingsReceived, this, &LiveStreamView::eventRecordingsReceived);
//    connect(_StoragePtr, &StorageInterface::storageDetectionFailed, this, &LiveStreamView::eventStorageDetectionFailed);

//    _StoragePtr->getAllRecordingSets();
}

void LiveStreamView::selectAllRecordings()
{
    disableAll();

    for(int i = 0; i < _RecordingList.topLevelItemCount(); i++ )
    {
      _RecordingList.topLevelItem(i)->setCheckState(0, Qt::Checked);
    }

    enableAll();
}

void LiveStreamView::deSelectAllRecordings()
{
    _SetCount = 0;
    _RecordCount = 0;
    _PendingCount = 0;

    disableAll();

    for(int i = 0; i < _RecordingList.topLevelItemCount(); i++ )
    {
      _RecordingList.topLevelItem(i)->setCheckState(0, Qt::Unchecked);
    }

    enableAll();
}

void LiveStreamView::clearAllRecordings()
{
    _SetCount = 0;
    _RecordCount = 0;
    _PendingCount = 0;

    disableAll();

    _RecordingList.clear();

    enableAll();
}

//void LiveStreamView::eventRootDirectoryReceived(QList<StorageDirectory> rootdlist)
//{
//    QDate dtstart = _dtStartDate.date();
//    QDate dtend = _dtEndDate.date();

//    mainWindowPtr->setProgressMessage("Received root directory");

////    _RootDirList = rootdlist;
////    foreach (StorageDirectory rstg, _RootDirList)
////    {
////        QDate dt = QDate::fromString(rstg.DirectoryName, "yyyyMMdd");

////        if(dt >= dtstart && dt <= dtend)
////        {
////            _StoragePtr->getDailyRecordingSets(rstg);
////        }
////    }
//}

//void LiveStreamView::eventRecordsDirectoryReceived(QList<DailyRecordingSet> recdlist)
//{
//    mainWindowPtr->setProgressMessage("Received recording directory");

////    _RecDirList = recdlist;

////    _SetCount += _RecDirList.count();

////    foreach (DailyRecordingSet dstg, _RecDirList)
////    {
////         _StoragePtr->getRecordingSets(dstg);
////    }

//}

//void LiveStreamView::eventRecordingsReceived(QList<RecordingSet> recdlist)
//{
//    mainWindowPtr->setProgressMessage("Receiving recording list ...");

//    _RecSet = recdlist;

//    _SetCount--;

//    foreach (RecordingSet recstg, _RecSet)
//    {
//        if(_chkSkip999.checkState() == Qt::Checked
//                && recstg.Name.contains("_9999")
//                && recstg.Name.contains("9999.264") )
//        {
//            continue;
//        }


//        int matchscore = 0;

//        if(recstg.TimeStamp.time() >= _tmStartTimeRange1.time() && recstg.TimeStamp.time() <= _tmEndTimeRange1.time())
//        {
//            matchscore++;
//        }

//        if(recstg.TimeStamp.time() >= _tmStartTimeRange2.time() && recstg.TimeStamp.time() <= _tmEndTimeRange2.time())
//        {
//            matchscore++;
//        }

//        if(recstg.TimeStamp.time() >= _tmStartTimeRange3.time() && recstg.TimeStamp.time() <= _tmEndTimeRange3.time())
//        {
//            matchscore++;
//        }

//        if(recstg.TimeStamp.time() >= _tmStartTimeRange4.time() && recstg.TimeStamp.time() <= _tmEndTimeRange4.time())
//        {
//            matchscore++;
//        }

//        if(matchscore > 0)
//        {
//            QString remotefile = recstg.ParentDirectory + "/" + recstg.Name;
//            QTreeWidgetItem *recItem = new QTreeWidgetItem(&_RecordingList);
//            recItem->setText(0, _CurrentCamName);
//            recItem->setText(1, recstg.Name);
//            recItem->setText(2, recstg.TimeStamp.toString("ddd, dd-MMM-yyyy hh:mm:ss"));
//            recItem->setText(3, recstg.Size);
//            recItem->setData(1, Qt::UserRole, QVariant(remotefile));
//            recItem->setCheckState(0, Qt::Unchecked);

//            _RecordCount++;
//        }
//    }

//    if(_SetCount <= 0)
//    {
//        mainWindowPtr->clearStatusBar();

//        if(_RecordCount <= 0)
//        {
//            QString msginfo = "No recordings found at\nhttp://";
//            msginfo += _StoragePtr->server() + ":";
//            msginfo += QVariant(_StoragePtr->port()).toString() + "/\n";
//            msginfo += " for the provided date and time range";

//            QMessageBox::information(nullptr, ipbumPtr->applicationName(), msginfo, QMessageBox::Ok);

//        }

//        cleanUpStorageInterface();
//    }
//}

//void LiveStreamView::eventStorageDetectionFailed(StorageInterface* cam)
//{
//    mainWindowPtr->clearStatusBar();

//    QString msginfo = "No camera found at\nhttp://";
//    msginfo += cam->server() + ":";
//    msginfo += QVariant(cam->port()).toString() + "/";

//    QMessageBox::information(nullptr, ipbumPtr->applicationName(), msginfo, QMessageBox::Ok);

//    cleanUpStorageInterface();
//}

void LiveStreamView::cleanUpStorageInterface()
{
//    disconnect(_StoragePtr, &StorageInterface::storageDetectionSuceeded, this, &LiveStreamView::eventStorageDetectionSuceeded);
//    disconnect(_StoragePtr, &StorageInterface::rootDirectoryReceived, this, &LiveStreamView::eventRootDirectoryReceived);
//    disconnect(_StoragePtr, &StorageInterface::recordsDirectoryReceived, this, &LiveStreamView::eventRecordsDirectoryReceived);
//    disconnect(_StoragePtr, &StorageInterface::recordingsReceived, this, &LiveStreamView::eventRecordingsReceived);
//    disconnect(_StoragePtr, &StorageInterface::storageDetectionFailed, this, &LiveStreamView::eventStorageDetectionFailed);
//    _StoragePtr->deleteLater();

    enableAll();
}

//void LiveStreamView::eventStorageDetectionSuceeded(StorageInterface* cam)
//{

//}

void LiveStreamView::exportRecordings()
{
    disableAll();

    _FetchList.clear();

    _PendingCount = 0;
    _FetchedCount = 0;

    for(int i = 0; i < _RecordingList.topLevelItemCount(); i++ )
    {
        if(_RecordingList.topLevelItem(i)->checkState(0) == Qt::Checked)
        {
            _PendingCount++;
            _FetchList.append(_RecordingList.topLevelItem(i)->data(1, Qt::UserRole).toString());
        }
    }

    if(_FetchList.count() < 1)
    {
        enableAll();
        return;
    }

    QString stat;
    stat = "";
    stat += "Selected ";
    stat += QVariant(_PendingCount).toString();
    stat += " recordings";

    mainWindowPtr->setStatusMessage(stat);

    stat = "";
    stat += "Downloaded ";
    stat += QVariant(_FetchedCount).toString();
    stat += " of ";
    stat += QVariant(_PendingCount).toString();
    stat += " recordings";

    mainWindowPtr->setProgressMessage(stat);

    int ct = 5;

    if(_FetchList.count() < ct)
    {
        ct = _FetchList.count();
    }

    for(int k = 0; k < ct; k++)
    {
        QString str = _FetchList.takeFirst();
        QUrl url(str);

        QNetworkRequest request(url);
        _ConnectionPool[k].get(request);
    }
}

void LiveStreamView::eventFinished(QNetworkReply *reply)
{
    _FetchedCount++;

    QStringList ls = reply->request().url().toString().split('/', QString::SkipEmptyParts);

    QString dirpart = tracksPtr->recordingsDirectory() + "/";

    dirpart += getStoragePath(reply->request().url().toString());

    QDir dir(dirpart);

    bool dircreated = false;

    if(!dir.exists())
    {
        if(dir.mkpath(dirpart))
        {
            dircreated = true;
        }
    }
    else
    {
       dircreated = true;
    }

    if(dircreated)
    {
        QString flname = dirpart + ls.at(ls.count() - 1);

        QFile fl(flname);

        if(fl.open(QIODevice::WriteOnly))
        {
            fl.write(reply->readAll());
            fl.flush();
            fl.close();
        }
    }

    QString stat;
    stat = "";
    stat += "Downloaded ";
    stat += QVariant(_FetchedCount).toString();
    stat += " of ";
    stat += QVariant(_PendingCount).toString();
    stat += " files";

    mainWindowPtr->setProgressMessage(stat);

    if(_PendingCount > _FetchedCount)
    {
        if(_FetchList.count() > 0)
        {
            QString str = _FetchList.takeFirst();
            QUrl url(str);

            QNetworkRequest request(url);
            reply->manager()->get(request);
        }
    }
    else
    {
        mainWindowPtr->clearStatusBar();

        enableAll();
    }
}

void LiveStreamView::eventAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    authenticator->setUser(_CurrentCam.ServiceUser);
    authenticator->setPassword(_CurrentCam.Password);
}

QString LiveStreamView::getStoragePath(QString str)
{
    QUrl url(str);

    QStringList tlist = str.split('/', QString::SkipEmptyParts);

    QStringList endp = tlist.at(1).split(':', QString::SkipEmptyParts);

    QString camname = tracksPtr->cameraNameByEndpoint(endp.at(0), QVariant(endp.at(1)).toUInt());

    QString dirpath = "";
    dirpath += camname + "/";
    dirpath += tlist.at(tlist.count() - 3) + "/";

    return dirpath;
}

void LiveStreamView::enableAll()
{
    _cmdClearAll.setEnabled(true);
    _cmdDeSelectAll.setEnabled(true);
    _cmdSelectAll.setEnabled(true);
    _cmdExport.setEnabled(true);
    _chkSkip999.setEnabled(true);
    _cmdSearch.setEnabled(true);
    _cmbxCamList.setEnabled(true);
}

void LiveStreamView::disableAll()
{
    _cmdClearAll.setEnabled(false);
    _cmdDeSelectAll.setEnabled(false);
    _cmdSelectAll.setEnabled(false);
    _cmdExport.setEnabled(false);
    _chkSkip999.setEnabled(false);
    _cmdSearch.setEnabled(false);
    _cmbxCamList.setEnabled(false);
}
