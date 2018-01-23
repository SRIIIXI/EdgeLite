#include "SideBar.h"
#include "ThemeManager.h"

SideBar::SideBar(QWidget *parent) : QWidget(parent), _MenuOptios(true)
{
    _MenuOptios.setFrameStyle(QFrame::NoFrame);

    _MainLayout.addWidget(new HorizontalLine(), Qt::AlignLeft | Qt::AlignTop);
    _MainLayout.addWidget(&_MenuOptios, Qt::AlignLeft | Qt::AlignTop);
    _MainLayout.addStretch(0);
    _MainLayout.addWidget(new HorizontalLine(), Qt::AlignLeft | Qt::AlignTop);

    connect(&_MenuOptios, &QListWidget::itemClicked, this, &SideBar::eventOptiontSelected);

    _MainLayout.setMargin(0);
    _MainLayout.setSpacing(0);

    setLayout(&_MainLayout);

    setMinimumWidth(205);
    setMaximumWidth(205);
}

SideBar::~SideBar()
{

}

void SideBar::initialize()
{
    QListWidgetItem *item1 = new QListWidgetItem(&_MenuOptios);
    item1->setData(Qt::DisplayRole, QVariant("Network"));
    item1->setData(Qt::UserRole, QVariant("Discover available cameras"));
    item1->setData(Qt::UserRole+1, QVariant((int)1));

    QListWidgetItem *item2 = new QListWidgetItem(&_MenuOptios);
    item2->setData(Qt::DisplayRole, QVariant("Camera"));
    item2->setData(Qt::UserRole, QVariant("Manage the active camera"));
    item2->setData(Qt::UserRole+1, QVariant((int)2));

    QListWidgetItem *item3 = new QListWidgetItem(&_MenuOptios);
    item3->setData(Qt::DisplayRole, QVariant("Configuration"));
    item3->setData(Qt::UserRole, QVariant("Application Configuration"));
    item3->setData(Qt::UserRole+1, QVariant((int)3));

    QListWidgetItem *item4 = new QListWidgetItem(&_MenuOptios);
    item4->setData(Qt::DisplayRole, QVariant("About"));
    item4->setData(Qt::UserRole, QVariant("About IPBuM"));
    item4->setData(Qt::UserRole+1, QVariant((int)4));

    QListWidgetItem *item5 = new QListWidgetItem(&_MenuOptios);
    item5->setData(Qt::DisplayRole, QVariant("Exit"));
    item5->setData(Qt::UserRole, QVariant("Exit the application"));
    item5->setData(Qt::UserRole+1, QVariant((int)5));
}

void SideBar::eventOptiontSelected(QListWidgetItem *item)
{
    int index = item->data(Qt::UserRole+1).toInt();
    emit optionSelected(index);
}
