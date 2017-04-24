#include "MainWindow.h"
#include "AddressBookWindow.h"
#include "MailCompositionWindow.h"
#include "AccountsWindow.h"
#include "SettingsWindow.h"

MainWindow* mainWindowPtr = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setMinimumHeight(768);
    setMinimumWidth(1024);
    mainWindowPtr = this;

    setWindowIcon(QIcon(":images/dark/mailclient.png"));
}

MainWindow::~MainWindow()
{
}

bool MainWindow::initialize()
{
    //Intialize UI elements
    //createActions();
    //populateMenubar(_AppMenuBar);
    //populateToolbar(_AppToolBar);
    //addToolBar(&_AppToolBar);
    //setMenuBar(&_AppMenuBar);

    setupDirectoryView();
    setupMailBoxView();
    setupViewPane();

    setCentralWidget(&_AppCentralWidget);

    _AppCentralWidget.setOrientation(Qt::Horizontal);
    _AppCentralWidget.setHandleWidth(1);
    _AppCentralWidget.setFrameStyle(QFrame::Plain);

    _Contacts.setContactSelectionFlag(false);
    _Contacts.loadContacts();

    //Wire up event handlers
//    connect(fileExit, &QAction::triggered, this, &MainWindow::eventExit);
//    connect(fileNewMail, &QAction::triggered, this, &MainWindow::eventNewMail);
//    connect(fileAddressBook, &QAction::triggered, this, &MainWindow::eventAddressBook);
//    connect(fileProfiles, &QAction::triggered, this, &MainWindow::eventProfiles);
//    connect(fileSettings, &QAction::triggered, this, &MainWindow::eventSettings);
//    connect(editReply, &QAction::triggered, this, &MainWindow::eventReply);
//    connect(editReplyAll, &QAction::triggered, this, &MainWindow::eventReplyAll);
//    connect(editForward, &QAction::triggered, this, &MainWindow::eventForward);
//    connect(editDelete, &QAction::triggered, this, &MainWindow::eventDelete);
//    connect(editFlag, &QAction::triggered, this, &MainWindow::eventFlag);
//    connect(viewNext, &QAction::triggered, this, &MainWindow::eventNext);
//    connect(viewPrevious, &QAction::triggered, this, &MainWindow::eventPrevious);
//    connect(viewSearch, &QAction::triggered, this, &MainWindow::eventSearch);
//    connect(viewRefresh, &QAction::triggered, this, &MainWindow::eventRefresh);
//    connect(helpAboutQt, &QAction::triggered, this, &MainWindow::eventAboutQt);
//    connect(helpAboutApplication, &QAction::triggered, this, &MainWindow::eventApplication);

    connect(&_AccountView, &AccountView::loadDirectory, &_MailBoxView, &MailBoxView::eventLoadDirectory);

    connect(&_MailBoxView, &MailBoxView::firstMailSelected, this, &MainWindow::eventFirstMailSelected);
    connect(&_MailBoxView, &MailBoxView::lastMailSelected, this, &MainWindow::eventLastMailSelected);
    connect(&_MailBoxView, &MailBoxView::loadingStarted, this, &MainWindow::eventLoadingStarted);
    connect(&_MailBoxView, &MailBoxView::loadingFinished, this, &MainWindow::eventLoadingFinished);
    connect(&_MailBoxView, &MailBoxView::enableNavigation, this, &MainWindow::eventEnableNavigation);

    connect(accountsWindowPtr, &AccountsWindow::accountAdded, this, &MainWindow::eventProfileAdded);
    connect(accountsWindowPtr, &AccountsWindow::accountUpdated, this, &MainWindow::eventProfileUpdated);
    connect(accountsWindowPtr, &AccountsWindow::accountRemoved, this, &MainWindow::eventProfileRemoved);

    //viewRefresh->setEnabled(false);

    if(accountEntityPtr->cache()->count() < 1)
    {
//        fileNewMail->setEnabled(false);
//        editReply->setEnabled(false);
//        editReplyAll->setEnabled(false);
//        editForward->setEnabled(false);
//        editDelete->setEnabled(false);
//        editFlag->setEnabled(false);
//        viewNext->setEnabled(false);
//        viewPrevious->setEnabled(false);
//        viewSearch->setEnabled(false);
//        viewRefresh->setEnabled(false);
    }

    return true;
}

void MainWindow::setupDirectoryView()
{
    _AccountView.initialize();
    _AppCentralWidget.addWidget(&_AccountView);
}

void MainWindow::setupMailBoxView()
{
    _AppCentralWidget.addWidget(&_MailBoxView);
    _MailBoxView.initialize(&_MailView);
}

void MainWindow::setupViewPane()
{
    _AppCentralWidget.addWidget(&_ViewPane);
    _ViewPane.addWidget(&_MailView);
    _ViewPane.setCurrentIndex(0);
}

void MainWindow::eventExit()
{
    close();
}

void MainWindow::eventNewMail()
{
    mailCompostionWindowPtr->show();
}

void MainWindow::eventAddressBook()
{
    addressBookWindowPtr->setContactSelectionFlag(false);
    addressBookWindowPtr->show();
}

void MainWindow::eventProfiles()
{
    accountsWindowPtr->show();
}

void MainWindow::eventSettings()
{
    settingsWindowPtr->show();
}

void MainWindow::eventReply()
{
    MailHeader emlhdr = _MailBoxView.currentMailHeader();

    QString str = "RE: " + emlhdr.subject();

    emlhdr.setSubject(str);

    str = emlhdr.toList().at(0);

    emlhdr.addtoToList(emlhdr.from(), true);

    emlhdr.setFrom(str);

    mailCompostionWindowPtr->setMail(emlhdr, _MailBoxView.currentMailBody());
    mailCompostionWindowPtr->show();
}

void MainWindow::eventReplyAll()
{
    MailHeader emlhdr = _MailBoxView.currentMailHeader();

    QString str = "RE: " + emlhdr.subject();

    emlhdr.setSubject(str);

    str = emlhdr.toList().at(0);

    emlhdr.addtoToList(emlhdr.from(), true);

    emlhdr.setFrom(str);

    mailCompostionWindowPtr->setMail(emlhdr, _MailBoxView.currentMailBody());
    mailCompostionWindowPtr->show();
}

void MainWindow::eventForward()
{
    MailHeader emlhdr = _MailBoxView.currentMailHeader();

    QString str = "FW: " + emlhdr.subject();

    emlhdr.setSubject(str);

    emlhdr.setFrom(str);

    mailCompostionWindowPtr->setMail(emlhdr, _MailBoxView.currentMailBody());
    mailCompostionWindowPtr->show();
}

void MainWindow::eventDelete()
{
    _MailBoxView.deleteCurrent();
}

void MainWindow::eventFlag()
{
    _MailBoxView.flagCurrent();
}

void MainWindow::eventNext()
{
    _MailBoxView.moveToNext();
}

void MainWindow::eventPrevious()
{
    _MailBoxView.moveToPrevious();
}

void MainWindow::eventSearch()
{

}

void MainWindow::eventRefresh()
{

}

void MainWindow::eventAboutQt()
{
    QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::eventApplication()
{
    QMessageBox::about(this, "About " + mailClientPtr->applicationName(), tr("Light, fast and secure email client"));
}

void MainWindow::eventLoadingStarted()
{
}

void MainWindow::eventLoadingFinished(long num)
{
    if(num < 3)
    {
//        viewPrevious->setEnabled(false);
//        viewNext->setEnabled(false);
    }
    else
    {
//        viewPrevious->setEnabled(true);
//        viewNext->setEnabled(true);
    }
}

void MainWindow::eventFirstMailSelected()
{
//    viewPrevious->setEnabled(false);
//    viewNext->setEnabled(true);
}

void MainWindow::eventLastMailSelected()
{
//    viewNext->setEnabled(false);
//    viewPrevious->setEnabled(true);
}

void MainWindow::eventEnableNavigation()
{
//    viewPrevious->setEnabled(true);
//    viewNext->setEnabled(true);
}

void MainWindow::eventProfileAdded(QString emailId)
{
    _AccountView.addAccount(emailId);

    if(accountEntityPtr->cache()->count() == 1)
    {
//        fileNewMail->setEnabled(true);
//        editReply->setEnabled(true);
//        editReplyAll->setEnabled(true);
//        editForward->setEnabled(true);
//        editDelete->setEnabled(true);
//        editFlag->setEnabled(true);
//        viewNext->setEnabled(true);
//        viewPrevious->setEnabled(true);
//        viewSearch->setEnabled(true);
        //viewRefresh->setEnabled(true);
    }
}

void MainWindow::eventProfileUpdated(QString emailId)
{
    _AccountView.updateAccount(emailId);
}

void MainWindow::eventProfileRemoved(QString emailId)
{
    if(accountEntityPtr->cache()->count() < 1)
    {
//        fileNewMail->setEnabled(false);
//        editReply->setEnabled(false);
//        editReplyAll->setEnabled(false);
//        editForward->setEnabled(false);
//        editDelete->setEnabled(false);
//        editFlag->setEnabled(false);
//        viewNext->setEnabled(false);
//        viewPrevious->setEnabled(false);
//        viewSearch->setEnabled(false);
//        viewRefresh->setEnabled(false);
    }

    _AccountView.removeAccount(emailId);
    _MailBoxView.clear();
    _MailView.clear();
}

void MainWindow::loadDirectory(ImapClient *ptr, QString uname, QString dirname, long mcount, long nextuid)
{

}
