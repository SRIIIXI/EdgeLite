#include "RecordingView.h"
#include "ThemeManager.h"
#include "IPCamDesktop.h"

RecordingView::RecordingView(QWidget *parent) : QWidget(parent)
{
    _cmdDExport.setText("Export");
    _cmdRefresh.setText("Refresh");
    _cmdDExport.setMaximumWidth(75);
    _cmdRefresh.setMaximumWidth(75);

    _CommandLayout.setMargin(5);
    _CommandLayout.addStretch();
    _CommandLayout.addWidget(&_cmdRefresh);
    _CommandLayout.addWidget(&_cmdDExport);

    _DashboardItems.setFrameStyle(QFrame::NoFrame);
    _DashboardItems.setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    QStringList reclst;
    reclst << "Camera"  << "Filename" << "Date and Time" << "Status" << "Size";
    _DashboardItems.setHeaderLabels(reclst);
    _DashboardItems.setColumnWidth(1, 300);

    _Layout.addWidget(new HorizontalLine());
    _Layout.addLayout(&_CommandLayout);
    _Layout.addWidget(new HorizontalLine());
    _Layout.addWidget(&_DashboardItems);
    _Layout.addWidget(new HorizontalLine());

    _Layout.setMargin(0);

    setLayout(&_Layout);

    connect(&_cmdRefresh, &QPushButton::clicked, this, &RecordingView::refreshDashboard);
    connect(&_cmdDExport, &QPushButton::clicked, this, &RecordingView::exportDashboard);
}

void RecordingView::refreshDashboard()
{
    _DashboardItems.clear();
    applicationPtr->loadRecordingList();
}

void RecordingView::eventRecordingAdded(Recording &dnl)
{
    QString camname = applicationPtr->getCameraById(dnl.camera_id).camera_name;

    QList<QTreeWidgetItem *> lst =  _DashboardItems.findItems(camname, Qt::MatchExactly, 0);

    if(lst.count() < 1)
    {
        QTreeWidgetItem *camItem = new QTreeWidgetItem(&_DashboardItems);
        camItem->setText(0, camname);
        QTreeWidgetItem *schedItem = new QTreeWidgetItem(camItem);
        QStringList toks = dnl.camera_file_uri.split('/',QString::SkipEmptyParts);
        schedItem->setText(1, toks.at(toks.count()-1));
        schedItem->setText(2, dnl.request_timestamp.toLower().replace("t", " "));

        if(dnl.request_status.toLower() == "c")
        {
            schedItem->setText(3, "Downloaded");
        }

        if(dnl.request_status.toLower() == "q")
        {
            schedItem->setText(3, "Queued");
        }

        if(dnl.request_status.toLower() == "e")
        {
            schedItem->setText(3, "Error");
        }

        if(dnl.request_status.toLower() == "r")
        {
            schedItem->setText(3, "Playlist Only");
        }

        schedItem->setText(4, dnl.file_size);
    }
    else
    {
        foreach (QTreeWidgetItem* item, lst)
        {
            QTreeWidgetItem *schedItem = new QTreeWidgetItem(item);
            QStringList toks = dnl.camera_file_uri.split('/',QString::SkipEmptyParts);
            schedItem->setText(1, toks.at(toks.count()-1));
            schedItem->setText(2, dnl.request_timestamp.toLower().replace("t", " "));

            if(dnl.request_status.toLower() == "c")
            {
                schedItem->setText(3, "Downloaded");
            }

            if(dnl.request_status.toLower() == "q")
            {
                schedItem->setText(3, "Queued");
            }

            if(dnl.request_status.toLower() == "e")
            {
                schedItem->setText(3, "Error");
            }

            if(dnl.request_status.toLower() == "r")
            {
                schedItem->setText(3, "Playlist Only");
            }

            schedItem->setText(4, dnl.file_size);
        }
    }
}

void RecordingView::eventRecordingEdited(Recording &dnl)
{

}

void RecordingView::eventRecordingDeleted(Recording &dnl)
{

}

void RecordingView::exportDashboard()
{

}
