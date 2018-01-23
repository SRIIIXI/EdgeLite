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

    _ViewPane.setCurrentIndex(0);
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

