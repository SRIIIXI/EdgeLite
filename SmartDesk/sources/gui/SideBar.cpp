#include "SideBar.h"
#include "ThemeManager.h"

SideBar::SideBar(QWidget *parent) : QWidget(parent), _MenuOptios(true)
{
    _SelectEnabled = true;

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
    item1->setData(Qt::DisplayRole, QVariant("Cameras"));
    item1->setData(Qt::UserRole, QVariant("Discover available cameras"));
    item1->setData(Qt::UserRole+1, QVariant((int)1));

    QListWidgetItem *item2 = new QListWidgetItem(&_MenuOptios);
    item2->setData(Qt::DisplayRole, QVariant("Recordings"));
    item2->setData(Qt::UserRole, QVariant("Recording list overview"));
    item2->setData(Qt::UserRole+1, QVariant((int)2));

    QListWidgetItem *item3 = new QListWidgetItem(&_MenuOptios);
    item3->setData(Qt::DisplayRole, QVariant("Configuration"));
    item3->setData(Qt::UserRole, QVariant("Application Configuration"));
    item3->setData(Qt::UserRole+1, QVariant((int)3));

    QListWidgetItem *item4 = new QListWidgetItem(&_MenuOptios);
    item4->setData(Qt::DisplayRole, QVariant("Licenses"));
    item4->setData(Qt::UserRole, QVariant("License Generation"));
    item4->setData(Qt::UserRole+1, QVariant((int)4));

    QListWidgetItem *item5 = new QListWidgetItem(&_MenuOptios);
    item5->setData(Qt::DisplayRole, QVariant("About"));
    item5->setData(Qt::UserRole, QVariant("About IPBuM"));
    item5->setData(Qt::UserRole+1, QVariant((int)5));

    QListWidgetItem *item6 = new QListWidgetItem(&_MenuOptios);
    item6->setData(Qt::DisplayRole, QVariant("Exit"));
    item6->setData(Qt::UserRole, QVariant("Exit the application"));
    item6->setData(Qt::UserRole+1, QVariant((int)6));
}

void SideBar::eventOptiontSelected(QListWidgetItem *item)
{
    int index = item->data(Qt::UserRole+1).toInt();

    if(_SelectEnabled)
    {
        emit optionSelected(index);
    }
    else
    {
        if(index == 6)
        {
            emit optionSelected(index);
        }
    }
}

void SideBar::enableSelection()
{
   _SelectEnabled = true;
}

void SideBar::disableSelection()
{
    _SelectEnabled = false;
}
