#ifndef _THEME_MANAGER
#define _THEME_MANAGER

#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <QPalette>
#include <QIcon>
#include <QPixmap>
#include <QAction>

typedef enum ThemeType
{
    Light,
    Dark
}ThemeType;

class ThemeManager
{
public:
    ThemeManager();
    virtual ~ThemeManager();

    void adjustBackground(QWidget* ptr);
    void setAppThemeDark(QApplication *ptr);
    void setAppThemeLight(QApplication *ptr);

    QString preferredFont();
    void initFontPreference();
    QPalette palette();
    ThemeType themeType();

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


private:
    void createActions();
    void createPixmaps();

    QString _PreferredFont;
    QPalette _ApplicationPalette;
    ThemeType _ThemeType;

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

    QPixmap* _PixNewEmail;
    QPixmap* _PixAccount;
    QPixmap* _PixFolder;
};

extern ThemeManager ApplicationThemeManager;

class IndicatorCircular : public QLabel
{
    Q_OBJECT
public:
    IndicatorCircular();

    void start();
    void stop();
private:
    QMovie _Movie;
};

class IndicatorBar : public QLabel
{
    Q_OBJECT
public:
    IndicatorBar();

    void start();
    void stop();
private:
    QMovie _Movie;
};

#endif
