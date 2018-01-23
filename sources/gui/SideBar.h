#ifndef _ACCOUNT_VIEW
#define _ACCOUNT_VIEW

#include <QWidget>
#include <QtCore>
#include <QVBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLabel>
#include <QMap>
#include <QPainter>
#include <QPushButton>
#include <QToolBar>
#include "CustomWidgets.h"

class SideBar : public QWidget
{
    Q_OBJECT
public:
    SideBar(QWidget* parent = nullptr);
    virtual ~SideBar();

    void initialize();

signals:
    void optionSelected(int index);

private slots:
    void eventOptiontSelected(QListWidgetItem *item);

private:
    QVBoxLayout _MainLayout;
    OptionList _MenuOptios;
};

#endif
