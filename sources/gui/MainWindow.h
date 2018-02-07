#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QHBoxLayout>
#include <QLabel>
#include <QtCore>
#include <QStatusBar>
#include <QTabWidget>
#include <QProgressBar>
#include <QTabBar>
#include <QStackedWidget>
#include <QMap>
#include <QtXml/QDomDocument>

#include "SideBar.h"
#include "ThemeManager.h"
#include "ServiceInterface.h"
#include "Tracks.h"
#include "LiveStreamView.h"
#include "NetworkView.h"
#include "StatisticsView.h"
#include "ConfigurationView.h"
#include "About.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool initialize();
    void clearStatusBar();
    void setStatusMessage(QString str);
    void setProgressMessage(QString str);
protected:
    void setupDirectoryView();
    void setupViewPane();
    void resizeEvent(QResizeEvent *event) override;
protected slots:
    void eventOptionSelected(int index);

private:
    CentralWidget _AppCentralWidget;
    QStatusBar      _AppStatusBar;
    QLabel          _StatusMessage;
    QLabel    _ProgressIndicator;

    SideBar    _OptionsMenuView;
    QStackedWidget _ViewPane;
    VerticalLine _ViewSeparator;
    QMap<QString, ServiceInterface*> _CamList;

    LiveStreamView _Recordings;
    NetworkView _Network;
    ConfigurationView _Configuration;
    About _About;
};

extern MainWindow* mainWindowPtr;

#endif
