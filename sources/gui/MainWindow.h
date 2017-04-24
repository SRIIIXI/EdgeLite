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
#include "Calender.h"
#include "AddressBookWindow.h"
#include "Tasks.h"
#include "Files.h"

//class MainWindow : public QMainWindow, ActionBuilder
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
    void eventAddressBook();
    void eventProfiles();
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

    //QToolBar        _AppToolBar;

    QSplitter       _AppCentralWidget;

    AccountView    _AccountView;
    MailBoxView     _MailBoxView;
    MailView    _MailView;

    QStackedWidget _ViewPane;

    AddressBookWindow _Contacts;
    Calender _Calendar;
    Tasks _Pending;
    Files _Files;
};

extern MainWindow* mainWindowPtr;

#endif // MAINWINDOW_H
