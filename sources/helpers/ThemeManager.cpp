#include "ThemeManager.h"
#include <QPalette>
#include <QStyleFactory>
#include <QApplication>
#include <QFontDatabase>
#include <QFontInfo>
#include <QFont>

ThemeManager ApplicationThemeManager;

ThemeManager::ThemeManager()
{
}

ThemeManager::~ThemeManager()
{

}

void ThemeManager::adjustBackground(QWidget* ptr)
{
    QString str = ptr->metaObject()->className();

    QPalette pal = ptr->palette();
    pal.setColor(QPalette::Background, Qt::white);
    ptr->setAutoFillBackground(true);
    ptr->setPalette(pal);
    ptr->setStyleSheet(str + "{background: white;}");
}

void ThemeManager::setAppThemeDark(QApplication* ptr)
{
#define FUSION_THEME
#ifdef FUSION_THEME
    ptr->setStyle(QStyleFactory::create("Fusion"));
    _ApplicationPalette.setColor(QPalette::Window, QColor(53,53,53));
    _ApplicationPalette.setColor(QPalette::WindowText, Qt::white);
    _ApplicationPalette.setColor(QPalette::Base, QColor(25,25,25));
    _ApplicationPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    _ApplicationPalette.setColor(QPalette::ToolTipBase, Qt::white);
    _ApplicationPalette.setColor(QPalette::ToolTipText, Qt::white);
    _ApplicationPalette.setColor(QPalette::Text, Qt::white);
    _ApplicationPalette.setColor(QPalette::Button, QColor(53,53,53));
    _ApplicationPalette.setColor(QPalette::ButtonText, Qt::white);
    _ApplicationPalette.setColor(QPalette::BrightText, Qt::red);
    _ApplicationPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    _ApplicationPalette.setColor(QPalette::Highlight, QColor(15, 122 , 253));
    _ApplicationPalette.setColor(QPalette::HighlightedText, Qt::black);
    ptr->setPalette(_ApplicationPalette);
    ptr->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
#endif
    _ThemeType = Dark;
    createActions();
}

void ThemeManager::setAppThemeLight(QApplication* ptr)
{
#define FUSION_THEME
#ifdef FUSION_THEME
    ptr->setStyle(QStyleFactory::create("Fusion"));
    _ApplicationPalette.setColor(QPalette::Window, QColor(255,255,255));
    _ApplicationPalette.setColor(QPalette::WindowText, Qt::black);
    _ApplicationPalette.setColor(QPalette::Base, QColor(255,255,255));
    _ApplicationPalette.setColor(QPalette::AlternateBase, QColor(235,235,235));
    _ApplicationPalette.setColor(QPalette::ToolTipBase, Qt::black);
    _ApplicationPalette.setColor(QPalette::ToolTipText, Qt::black);
    _ApplicationPalette.setColor(QPalette::Text, Qt::black);
    _ApplicationPalette.setColor(QPalette::Button, QColor(255,255,255));
    _ApplicationPalette.setColor(QPalette::ButtonText, Qt::black);
    _ApplicationPalette.setColor(QPalette::BrightText, Qt::red);
    _ApplicationPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    _ApplicationPalette.setColor(QPalette::Highlight, QColor(15, 122 , 253));
    _ApplicationPalette.setColor(QPalette::HighlightedText, Qt::white);
    ptr->setPalette(_ApplicationPalette);
    ptr->setStyleSheet("QToolTip { color: #000000; background-white: #2a82da; border: 1px solid black; }");
#endif
    _ThemeType = Light;
    createActions();
}

QString ThemeManager::preferredFont()
{
    return _PreferredFont;
}

void ThemeManager::initFontPreference()
{
    QFontDatabase fontdb;

    QStringList fontlist = fontdb.families();

    foreach(QString str, fontlist)
    {
        if(str.toLower().contains("tahoma"))
        {
            _PreferredFont = str;
        }
    }

    if(_PreferredFont.length() < 1)
    {
        _PreferredFont = fontlist.at(0);
    }
}

QPalette ThemeManager::palette()
{
    return _ApplicationPalette;
}

ThemeType ThemeManager::themeType()
{
    return _ThemeType;
}

void ThemeManager::createActions()
{
    if(_ThemeType == Dark)
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
}

void ThemeManager::createPixmaps()
{

}

IndicatorCircular::IndicatorCircular()
{
    _Movie.setFileName(":images/cirularprogress.gif");
    setMovie(&_Movie);
    setVisible(false);
}

void IndicatorCircular::start()
{
    setVisible(true);
    show();
    _Movie.start();
}

void IndicatorCircular::stop()
{
    setVisible(false);
    hide();
    _Movie.stop();
}

IndicatorBar::IndicatorBar()
{
    _Movie.setFileName(":images/barprogress.gif");
    setMovie(&_Movie);
    setVisible(false);
}

void IndicatorBar::start()
{
    setVisible(true);
    show();
    _Movie.start();}

void IndicatorBar::stop()
{
    setVisible(false);
    hide();
    _Movie.stop();
}
