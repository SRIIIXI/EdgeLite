#include "MainWindow.h"

MainWindow* mainWindowPtr = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setMinimumHeight(768);
    setMinimumWidth(1024);
    mainWindowPtr = this;

    setWindowIcon(QIcon(":images/ipbum.png"));
}

MainWindow::~MainWindow()
{
}

bool MainWindow::initialize()
{
    _StatusMessage.setMinimumWidth(350);
    _ProgressIndicator.setMinimumWidth(width() - 370);
    _StatusMessage.setFrameStyle(QFrame::NoFrame);
    _ProgressIndicator.setFrameStyle(QFrame::NoFrame);

    setStatusBar(&_AppStatusBar);
    _AppStatusBar.insertWidget(0, &_StatusMessage);
    _AppStatusBar.insertWidget(1, &_ProgressIndicator);

    setupDirectoryView();
    _AppCentralWidget.addWidget(&_ViewSeparator);
    setupViewPane();

    setCentralWidget(&_AppCentralWidget);

    connect(&_OptionsMenuView, &SideBar::optionSelected, this, &MainWindow::eventOptionSelected);

    return true;
}

void MainWindow::setupDirectoryView()
{
    _OptionsMenuView.initialize();
    _AppCentralWidget.addWidget(&_OptionsMenuView);
}

void MainWindow::setupViewPane()
{
    _AppCentralWidget.addWidget(&_ViewPane);

    _ViewPane.addWidget(&_Camera);
    _ViewPane.addWidget(&_Recording);
    _ViewPane.addWidget(&_Configuration);
    _ViewPane.addWidget(&_License);
    _ViewPane.addWidget(&_About);

    if(applicationPtr->cameraList().count() > 0)
    {
        _ViewPane.setCurrentWidget(&_Recording);
    }
    else
    {
        _ViewPane.setCurrentWidget(&_Camera);
    }
}

void MainWindow::eventOptionSelected(int index)
{
    if(index == 6)
    {
        close();
    }

    switch (index)
    {
    case 1:
        _ViewPane.setCurrentWidget(&_Camera);
        break;
    case 2:
        _ViewPane.setCurrentWidget(&_Recording);
        break;
    case 3:
        _ViewPane.setCurrentWidget(&_Configuration);
        _Configuration.updateLicenseFile(applicationPtr->licenseFile());
        break;
    case 4:
        _ViewPane.setCurrentWidget(&_License);
        break;
    case 5:
        _ViewPane.setCurrentWidget(&_About);
        break;
    default:
        break;
    }
}

void MainWindow::setStatusMessage(QString str)
{
    _StatusMessage.setText(str);
}

void MainWindow::setProgressMessage(QString str)
{
    _ProgressIndicator.setText(str);
}

void MainWindow::clearStatusBar()
{
    _StatusMessage.setText("");
    _ProgressIndicator.setText("");
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    _ProgressIndicator.setMinimumWidth(width() - 370);
}

void MainWindow::updateRestServiceStatus(bool flag, Site st)
{
    _Configuration.updateRestServiceStatus(flag, st);
}

CameraView* MainWindow::cameras()
{
    return &_Camera;
}

RecordingView* MainWindow::recordings()
{
   return &_Recording;
}

void MainWindow::activateCameraView()
{
    _ViewPane.setCurrentWidget(&_Camera);
}

void MainWindow::activateConfigView()
{
    _ViewPane.setCurrentWidget(&_Configuration);
}

void MainWindow::enableSelection()
{
    _OptionsMenuView.enableSelection();
}

void MainWindow::disableSelection()
{
    _OptionsMenuView.disableSelection();
}
