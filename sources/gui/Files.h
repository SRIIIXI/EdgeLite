#ifndef _FILES
#define _FILES

#include <QtCore>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QTextBrowser>
#include <QListWidget>
#include <QPushButton>

#include "Mail.h"
#include "ThemeManager.h"

class Files : public QWidget
{
    Q_OBJECT
public:
    Files(QWidget *parent = 0);
};

#endif
