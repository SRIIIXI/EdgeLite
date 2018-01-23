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

void ThemeManager::applyLightTheme(QApplication *ptr)
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
    _ApplicationPalette.setColor(QPalette::Highlight, QColor(25, 69 , 102));
    _ApplicationPalette.setColor(QPalette::HighlightedText, Qt::white);
    ptr->setPalette(_ApplicationPalette);
    ptr->setStyleSheet("QToolTip { color: #000000; background-color: #2a82da; border: 1px solid black; }");
#endif
}

void ThemeManager::applyDarkTheme(QApplication *ptr)
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
    //_ApplicationPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    _ApplicationPalette.setColor(QPalette::Highlight, QColor(25, 69 , 102));
    _ApplicationPalette.setColor(QPalette::HighlightedText, Qt::black);
    ptr->setPalette(_ApplicationPalette);
    ptr->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
#endif

}

ThemeManager::~ThemeManager()
{

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
