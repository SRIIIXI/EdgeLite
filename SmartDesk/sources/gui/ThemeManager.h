#ifndef _THEME_MANAGER
#define _THEME_MANAGER

#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <QPalette>
#include <QIcon>
#include <QPixmap>
#include <QAction>
#include <QBitmap>
#include <QPainter>

class ThemeManager
{
public:
    ThemeManager();
    virtual ~ThemeManager();

    QString preferredFont();
    void initFontPreference();
    void applyLightTheme(QApplication *ptr);
    void applyDarkTheme(QApplication *ptr);
    QPalette palette();

    bool roundel(QString imgfile, int sz, QPixmap &destpix);
    bool roundel(QPixmap origpix, int sz, QPixmap &destpix);

private:
    QString _PreferredFont;
    QPalette _ApplicationPalette;
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
