#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QVBoxLayout>
#include <QLabel>
#include <QtCore>
#include <QSplitter>
#include <QStatusBar>
#include <QTabWidget>
#include <QTabBar>
#include <QStackedWidget>

#include "AccountView.h"
#include "MailBoxView.h"
#include "ActionBuilder.h"
#include "ThemeManager.h"
#include "MailView.h"
#include "MailNewView.h"
#include "ContactsView.h"
#include "AccountsConfigView.h"
#include "SettingsView.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool initialize();
    void loadDirectory(ImapClient *ptr, QString uname, QString dirname, long mcount, long nextuid);

protected:
    void setupDirectoryView();
    void setupMailBoxView();
    void setupViewPane();

protected slots:
    void eventExit();
    void eventNewMail();
    void eventContacts();
    void eventAccounts();
    void eventSettings();

    void eventReply();
    void eventReplyAll();
    void eventForward();
    void eventDelete();
    void eventFlag();

    void eventNext();
    void eventPrevious();
    void eventSearch();
    void eventRefresh();

    void eventAboutQt();
    void eventApplication();

    void eventLoadingStarted();
    void eventLoadingFinished(long num);
    void eventFirstMailSelected();
    void eventLastMailSelected();
    void eventEnableNavigation();

    void eventProfileAdded(QString emailId);
    void eventProfileUpdated(QString emailId);
    void eventProfileRemoved(QString emailId);

private:
    QSplitter       _AppCentralWidget;

    AccountView    _AccountView;
    MailBoxView     _MailBoxView;

    QStackedWidget _ViewPane;

    MailView    _Mail;
    ContactsView _Contacts;
    MailNewView _NewMail;
    AccountsConfigView _Accounts;
    SettingsView _Settings;
};

extern MainWindow* mainWindowPtr;

#endif // MAINWINDOW_H