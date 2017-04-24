#ifndef _ACTION_BUILDER
#define _ACTION_BUILDER

#include <QtCore>
#include <QIcon>
#include <QAction>
#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QPalette>
#include <QLabel>
#include <QLineEdit>
#include "ThemeManager.h"

class ActionBuilder
{
public:
    ActionBuilder();
    void createActions();
    void populateToolbar(QToolBar &tbar);
    void populateMenubar(QMenuBar &mbar);

    QAction* exit() { return fileExit; }
    QAction* newEmail() { return fileNewMail; }
    QAction* addressBook() { return fileAddressBook; }
    QAction* accounts() { return fileProfiles; }
    QAction* settings() { return fileSettings; }

    QAction* reply() { return editReply; }
    QAction* replyAll() { return editReplyAll; }
    QAction* forward() { return editForward; }
    QAction* deleteEmail() { return editDelete; }
    QAction* flagEmail() { return editFlag; }

    QAction* next() { return viewNext; }
    QAction* previous() { return viewPrevious; }
    QAction* search() { return viewSearch; }
    QAction* refresh() { return viewRefresh; }
    QAction* filter() { return viewFilter; }

    QAction* aboutQt() { return helpAboutQt; }
    QAction* aboutApplication() { return helpAboutApplication; }


protected:
    QAction* fileExit;
    QAction* fileNewMail;
    QAction* fileAddressBook;
    QAction* fileProfiles;
    QAction* fileSettings;

    QAction* editReply;
    QAction* editReplyAll;
    QAction* editForward;
    QAction* editDelete;
    QAction* editFlag;

    QAction* viewNext;
    QAction* viewPrevious;
    QAction* viewSearch;
    QAction* viewRefresh;
    QAction* viewFilter;

    QAction* helpAboutQt;
    QAction* helpAboutApplication;

    QLineEdit _FilterInput;
    QLabel    _ProfilePicture;

    QMenu fileMenu;
    QMenu editMenu;
    QMenu viewMenu;
    QMenu helpMenu;
};

#endif
