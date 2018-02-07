#include "MainWindow.h"

MainWindow* mainWindowPtr = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setMinimumHeight(768);
    setMinimumWidth(1024);
    mainWindowPtr = this;

    setWindowIcon(QIcon(":images/tracks.png"));
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

    _ViewPane.addWidget(&_Network);
    _ViewPane.addWidget(&_Recordings);
    _ViewPane.addWidget(&_Configuration);
    _ViewPane.addWidget(&_About);

    if(tracksPtr->cameraList().count() > 0)
    {
        _ViewPane.setCurrentWidget(&_Recordings);
        _Recordings.refreshCameraList();
    }
    else
    {
        _ViewPane.setCurrentIndex(0);
    }
}

void MainWindow::eventOptionSelected(int index)
{
    if(index == 5)
    {
        close();
    }

    switch (index)
    {
    case 1:
        _ViewPane.setCurrentWidget(&_Network);
        _Network.loadCameraList();
        break;
    case 2:
        _ViewPane.setCurrentWidget(&_Recordings);
        _Recordings.refreshCameraList();
        break;
    case 3:
        _ViewPane.setCurrentWidget(&_Configuration);
        break;
    case 4:
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

