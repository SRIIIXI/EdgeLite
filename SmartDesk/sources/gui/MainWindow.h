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

#include "SideBar.h"
#include "ThemeManager.h"
#include "IPCamDesktop.h"
#include "LicenseView.h"
#include "CameraView.h"
#include "RecordingView.h"
#include "ConfigurationView.h"
#include "About.h"
#include "Site.h"

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
    void updateRestServiceStatus(bool flag, Site st);

    CameraView* cameras();
    RecordingView* recordings();

    void activateCameraView();
    void activateConfigView();

    void enableSelection();
    void disableSelection();

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

    CameraView _Camera;
    RecordingView   _Recording;
    LicenseView   _License;
    ConfigurationView _Configuration;
    About _About;
};

extern MainWindow* mainWindowPtr;

#endif
