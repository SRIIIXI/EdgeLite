#include "ActionBuilder.h"
#include "ThemeManager.h"

ActionBuilder::ActionBuilder()
{
}

void ActionBuilder::createActions()
{
    if(ApplicationThemeManager.themeType() == Dark)
    {
        fileExit = new QAction(QIcon(":images/dark/exit.png"), ("Exit"), nullptr);
        fileNewMail = new QAction(QIcon(":images/dark/emailcompose.png"), ("Compose ..."), nullptr);
        fileAddressBook = new QAction(QIcon(":images/dark/addressbook.png"), ("Contacts ..."), nullptr);
        fileProfiles = new QAction(QIcon(":images/dark/profiles.png"), ("Profiles ..."), nullptr);
        fileSettings = new QAction(QIcon(":images/dark/settings.png"), ("Settings ..."), nullptr);

        editReply = new QAction(QIcon(":images/dark/emailreply.png"), ("Reply"), nullptr);
        editReplyAll = new QAction(QIcon(":images/dark/emailreplyall.png"), ("Reply All"), nullptr);
        editForward = new QAction(QIcon(":images/dark/emailforward.png"), ("Forward"), nullptr);
        editDelete = new QAction(QIcon(":images/dark/emaildelete.png"), ("Delete"), nullptr);
        editFlag = new QAction(QIcon(":images/dark/emailflag.png"), ("Flag"), nullptr);

        viewNext = new QAction(QIcon(":images/dark/emailnext.png"), ("Next"), nullptr);
        viewPrevious = new QAction(QIcon(":images/dark/emailprevious.png"), ("Previous"), nullptr);
        viewSearch = new QAction(QIcon(":images/dark/emailsearch.png"), ("Search ..."), nullptr);
        viewRefresh = new QAction(QIcon(":images/dark/refresh.png"), ("Refresh"), nullptr);
        viewFilter = new QAction(QIcon(":images/dark/emailfilter.png"), ("Filter"), nullptr);
    }

    else
    {
        fileExit = new QAction(QIcon(":images/light/exit.png"), ("Exit"), nullptr);
        fileNewMail = new QAction(QIcon(":images/light/emailcompose.png"), ("Compose ..."), nullptr);
        fileAddressBook = new QAction(QIcon(":images/light/addressbook.png"), ("Contacts ..."), nullptr);
        fileProfiles = new QAction(QIcon(":images/light/profiles.png"), ("Profiles ..."), nullptr);
        fileSettings = new QAction(QIcon(":images/light/settings.png"), ("Settings ..."), nullptr);

        editReply = new QAction(QIcon(":images/light/emailreply.png"), ("Reply"), nullptr);
        editReplyAll = new QAction(QIcon(":images/light/emailreplyall.png"), ("Reply All"), nullptr);
        editForward = new QAction(QIcon(":images/light/emailforward.png"), ("Forward"), nullptr);
        editDelete = new QAction(QIcon(":images/light/emaildelete.png"), ("Delete"), nullptr);
        editFlag = new QAction(QIcon(":images/light/emailflag.png"), ("Flag"), nullptr);

        viewNext = new QAction(QIcon(":images/light/emailnext.png"), ("Next"), nullptr);
        viewPrevious = new QAction(QIcon(":images/light/emailprevious.png"), ("Previous"), nullptr);
        viewSearch = new QAction(QIcon(":images/light/emailsearch.png"), ("Search ..."), nullptr);
        viewRefresh = new QAction(QIcon(":images/light/refresh.png"), ("Refresh"), nullptr);
        viewFilter = new QAction(QIcon(":images/light/emailfilter.png"), ("Filter"), nullptr);
    }

    helpAboutQt = new QAction(QIcon(":images/dark/qtlogo.png"), ("About Qt"), NULL);
    helpAboutApplication = new QAction(QIcon(":images/mailclient.png"), ("About Application"), NULL);
}

void ActionBuilder::populateToolbar(QToolBar &tbar)
{
    _FilterInput.setMaximumWidth(250);

    tbar.addAction(fileNewMail);

    tbar.addSeparator();

    tbar.addAction(editReply);
    tbar.addAction(editReplyAll);
    tbar.addAction(editForward);

    tbar.addSeparator();

    tbar.addWidget(&_FilterInput);
    tbar.addAction(viewFilter);

    tbar.setMovable(false);
    tbar.setToolButtonStyle(Qt::ToolButtonIconOnly);

    QPalette pal = tbar.palette();
    pal.setColor(QPalette::Background, Qt::white);
    tbar.setAutoFillBackground(true);
    tbar.setPalette(pal);
    tbar.setIconSize(QSize(32,32));
    tbar.setStyleSheet("QToolBar { background: white; }");
}

void ActionBuilder::populateMenubar(QMenuBar &mbar)
{
    fileMenu.setTitle("File");
    fileMenu.addAction(fileNewMail);
    fileMenu.addAction(fileAddressBook);
    fileMenu.addAction(fileProfiles);
    fileMenu.addSeparator();
    fileMenu.addAction(fileSettings);
    fileMenu.addSeparator();
    fileMenu.addAction(fileExit);

    editMenu.setTitle("Edit");
    editMenu.addAction(editReply);
    editMenu.addAction(editReplyAll);
    editMenu.addAction(editForward);
    editMenu.addSeparator();
    editMenu.addAction(editDelete);
    editMenu.addAction(editFlag);

    viewMenu.setTitle("View");
    viewMenu.addAction(viewNext);
    viewMenu.addAction(viewPrevious);
    viewMenu.addSeparator();
    viewMenu.addAction(viewSearch);
    viewMenu.addSeparator();
    viewMenu.addAction(viewRefresh);

    helpMenu.setTitle("Help");
    helpMenu.addAction(helpAboutQt);
    helpMenu.addAction(helpAboutApplication);

    mbar.addMenu(&fileMenu);
    mbar.addMenu(&editMenu);
    mbar.addMenu(&viewMenu);
    mbar.addMenu(&helpMenu);
}
