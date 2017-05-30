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
    createPixmaps();
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
    ptr->setStyleSheet("QToolTip { color: #000000; background-color: #2a82da; border: 1px solid black; }");
#endif
    _ThemeType = Light;
    createPixmaps();
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

void ThemeManager::createPixmaps()
{
    if(_ThemeType == Dark)
    {
        _PixFileExit = ":images/dark/exit.png";
        _PixFileNewMail = ":images/dark/emailcompose.png";
        _PixFileContacts = ":images/dark/addressbook.png";
        _PixFileAccouts = ":images/dark/profiles.png";
        _PixFileSettings = ":images/dark/settings.png";

        _PixFileReply = ":images/dark/emailreply.png";
        _PixFileReplyAll = ":images/dark/emailreplyall.png";
        _PixFileForward = ":images/dark/emailforward.png";
        _PixFileDelete = ":images/dark/emaildelete.png";
        _PixFileFlag = ":images/dark/emailflag.png";

        _PixFileNext = ":images/dark/emailnext.png";
        _PixFilePrevious = ":images/dark/emailprevious.png";
        _PixFileSearch = ":images/dark/emailsearch.png";
        _PixFileRefresh = ":images/dark/refresh.png";
        _PixFileFilter = ":images/dark/emailfilter.png";
        _PixFileUnknown = ":images/dark/unk.png";

        _PixFileContactAdd = ":images/dark/contactadd.png";
        _PixFileContactEdit = ":images/dark/contactedit.png";
        _PixFileContactDelete = ":images/dark/contactdelete.png";
    }

    else
    {
        _PixFileExit = ":images/light/exit.png";
        _PixFileNewMail = ":images/light/emailcompose.png";
        _PixFileContacts = ":images/light/addressbook.png";
        _PixFileAccouts = ":images/light/profiles.png";
        _PixFileSettings = ":images/light/settings.png";

        _PixFileReply = ":images/light/emailreply.png";
        _PixFileReplyAll = ":images/light/emailreplyall.png";
        _PixFileForward = ":images/light/emailforward.png";
        _PixFileDelete = ":images/light/emaildelete.png";
        _PixFileFlag = ":images/light/emailflag.png";

        _PixFileNext = ":images/light/emailnext.png";
        _PixFilePrevious = ":images/light/emailprevious.png";
        _PixFileSearch = ":images/light/emailsearch.png";
        _PixFileRefresh = ":images/light/refresh.png";
        _PixFileFilter = ":images/light/emailfilter.png";
        _PixFileUnknown = ":images/light/unk.png";

        _PixFileContactAdd = ":images/light/contactadd.png";
        _PixFileContactEdit = ":images/light/contactedit.png";
        _PixFileContactDelete = ":images/light/contactdelete.png";
    }
}

bool ThemeManager::roundel(QString imgfile, int sz, QPixmap &destpix)
{
    destpix.load(imgfile);

    if (destpix.isNull())
    {
       qFatal("Failed to load.");
       return false;
    }

    destpix = destpix.scaled(sz, sz);

    // Draw the mask.
    QBitmap  mask(destpix.size());
    QPainter painter(&mask);
    mask.fill(Qt::white);
    painter.setBrush(Qt::black);
    painter.drawEllipse(QPoint(mask.width()/2, mask.height()/2), sz, sz);

    // Draw the final image.
    destpix.setMask(mask);

    return true;
}
bool ThemeManager::roundel(QPixmap origpix, int sz, QPixmap &destpix)
{
    destpix = origpix;

    if (destpix.isNull())
    {
       qFatal("Failed to load.");
       return false;
    }

    // Draw the mask.
    QBitmap  mask(destpix.size());
    QPainter painter(&mask);
    mask.fill(Qt::white);
    painter.setBrush(Qt::black);
    painter.drawEllipse(QPoint(mask.width()/2, mask.height()/2), sz, sz);

    // Draw the final image.
    destpix.setMask(mask);

    return true;
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
