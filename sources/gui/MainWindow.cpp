#include "MainWindow.h"
#include "ContactsView.h"
#include "MailNewView.h"
#include "AccountsConfigView.h"
#include "SettingsView.h"

MainWindow* mainWindowPtr = nullptr;

ContactsView* contactsWindow = nullptr;

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
    setupDirectoryView();
    _AppCentralWidget.addWidget(&_Line1);
    setupMailBoxView();
    _AppCentralWidget.addWidget(&_Line2);
    setupViewPane();

    setCentralWidget(&_AppCentralWidget);

    _Contacts.setContactSelectionFlag(false);
    _Contacts.loadContacts();

    //Wire up event handlers
    connect(&_AccountView, &AccountView::newEmailClicked, this, &MainWindow::eventNewMail);
    connect(&_AccountView, &AccountView::contactsClicked, this, &MainWindow::eventContacts);
    connect(&_AccountView, &AccountView::accountsClicked, this, &MainWindow::eventAccounts);
    connect(&_AccountView, &AccountView::settingsClicked, this, &MainWindow::eventSettings);

    //    connect(fileExit, &QAction::triggered, this, &MainWindow::eventExit);
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

    connect(&_Accounts, &AccountsConfigView::accountAdded, this, &MainWindow::eventProfileAdded);
    connect(&_Accounts, &AccountsConfigView::accountUpdated, this, &MainWindow::eventProfileUpdated);
    connect(&_Accounts, &AccountsConfigView::accountRemoved, this, &MainWindow::eventProfileRemoved);

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
    _MailBoxView.initialize(&_Mail);
}

void MainWindow::setupViewPane()
{
    _AppCentralWidget.addWidget(&_ViewPane);

    _ViewPane.addWidget(&_Mail);
    _ViewPane.addWidget(&_Contacts);
    _ViewPane.addWidget(&_NewMail);
    _ViewPane.addWidget(&_Accounts);
    _ViewPane.addWidget(&_Settings);

    _Contacts.loadContacts();

    //_ViewPane.setCurrentIndex(0);
    _ViewPane.setCurrentIndex(1);
}

void MainWindow::eventExit()
{
    close();
}

void MainWindow::eventContacts()
{
    _Contacts.setContactSelectionFlag(false);
    _ViewPane.setCurrentIndex(1);

    if(contactsWindow == nullptr)
    {
        contactsWindow = new ContactsView();
    }

    contactsWindow->setContactSelectionFlag(false);
    contactsWindow->show();
}

void MainWindow::eventNewMail()
{
    _ViewPane.setCurrentIndex(2);
}

void MainWindow::eventAccounts()
{
    _ViewPane.setCurrentIndex(3);
}

void MainWindow::eventSettings()
{
    _ViewPane.setCurrentIndex(4);
}

void MainWindow::eventReply()
{
    MailHeader emlhdr = _MailBoxView.currentMailHeader();

    QString str = "RE: " + emlhdr.subject();

    emlhdr.setSubject(str);

    str = emlhdr.toList().at(0);

    emlhdr.addtoToList(emlhdr.from(), true);

    emlhdr.setFrom(str);

    _NewMail.setMail(emlhdr, _MailBoxView.currentMailBody());
    _ViewPane.setCurrentIndex(2);
}

void MainWindow::eventReplyAll()
{
    MailHeader emlhdr = _MailBoxView.currentMailHeader();

    QString str = "RE: " + emlhdr.subject();

    emlhdr.setSubject(str);

    str = emlhdr.toList().at(0);

    emlhdr.addtoToList(emlhdr.from(), true);

    emlhdr.setFrom(str);

    _NewMail.setMail(emlhdr, _MailBoxView.currentMailBody());
    _ViewPane.setCurrentIndex(2);
}

void MainWindow::eventForward()
{
    MailHeader emlhdr = _MailBoxView.currentMailHeader();

    QString str = "FW: " + emlhdr.subject();

    emlhdr.setSubject(str);

    emlhdr.setFrom(str);

    _NewMail.setMail(emlhdr, _MailBoxView.currentMailBody());
    _ViewPane.setCurrentIndex(2);
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

void MainWindow::eventLoadingStarted()
{
}

void MainWindow::eventLoadingFinished(long num)
{
    _ViewPane.setCurrentIndex(0);
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
    _Mail.clear();
}

void MainWindow::loadDirectory(ImapClient *ptr, QString uname, QString dirname, long mcount, long nextuid)
{

}
