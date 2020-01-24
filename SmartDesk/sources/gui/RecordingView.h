#ifndef _RECORDING_VIEW
#define _RECORDING_VIEW

#include <QMainWindow>
#include <QWidget>
#include <QToolBar>
#include <QtCore>
#include <QLabel>
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
#include "CustomWidgets.h"
#include "Recording.h"

class RecordingView : public QWidget
{
    Q_OBJECT
public:
    RecordingView(QWidget* parent = nullptr);
    void refreshDashboard();

public slots:
    void eventRecordingAdded(Recording &dnl);
    void eventRecordingEdited(Recording &dnl);
    void eventRecordingDeleted(Recording &dnl);

protected:
    void exportDashboard();

private:
    QTreeWidget _DashboardItems;

    QHBoxLayout _CommandLayout;
    QPushButton _cmdRefresh;
    QPushButton _cmdDExport;

    QVBoxLayout _Layout;
};

#endif
