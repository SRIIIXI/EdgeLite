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

    void setAppThemeDark(QApplication *ptr);
    void setAppThemeLight(QApplication *ptr);

    QString preferredFont();
    void initFontPreference();
    QPalette palette();
    ThemeType themeType();

    QString exit() { return _PixFileExit; }
    QString newEmail() { return _PixFileNewMail; }
    QString contacts() { return _PixFileContacts; }
    QString accounts() { return _PixFileAccouts; }
    QString settings() { return _PixFileSettings; }

    QString reply() { return _PixFileReply; }
    QString replyAll() { return _PixFileReplyAll; }
    QString forward() { return _PixFileForward; }
    QString deleteEmail() { return _PixFileDelete; }
    QString flagEmail() { return _PixFileFlag; }

    QString next() { return _PixFileNext; }
    QString previous() { return _PixFilePrevious; }
    QString search() { return _PixFileSearch; }
    QString refresh() { return _PixFileRefresh; }
    QString filter() { return _PixFileFilter; }
    QString unknown() { return _PixFileUnknown; }

private:
    void createPixmaps();

    QString _PreferredFont;
    QPalette _ApplicationPalette;
    ThemeType _ThemeType;

    QString _PixFileExit;
    QString _PixFileNewMail;
    QString _PixFileContacts;
    QString _PixFileAccouts;
    QString _PixFileSettings;

    QString _PixFileReply;
    QString _PixFileReplyAll;
    QString _PixFileForward;
    QString _PixFileDelete;
    QString _PixFileFlag;

    QString _PixFileNext;
    QString _PixFilePrevious;
    QString _PixFileSearch;
    QString _PixFileRefresh;
    QString _PixFileFilter;
    QString _PixFileUnknown;
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
