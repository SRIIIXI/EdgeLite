#include "ContactsView.h"
#include "ContactAddDialog.h"
#include "ContactEntity.h"
#include "ThemeManager.h"
#include "MailNewView.h"

ContactsView::ContactsView(QWidget *parent) : QWidget(parent)
{
    _CurrentItem = nullptr;
    _IsEdited = false;
    QStringList lst;

    lst << "Name" << "EMail Id";

    _ContactSelctionMode = true;

    addAct = new QAction(QIcon(ApplicationThemeManager.contactAdd()), tr("Add Entry..."), this);
    editAct = new QAction(QIcon(ApplicationThemeManager.contactEdit()), tr("Edit Entry..."), this);
    removeAct = new QAction(QIcon(ApplicationThemeManager.conactRemove()), tr("Remove Entry"), this);
    editAct->setEnabled(false);
    removeAct->setEnabled(false);

    _BtnSearch.setText("Search");
    _ContactListView.setHeaderLabels(lst);

    connect(addAct, &QAction::triggered, this, &ContactsView::addEntry);
    connect(editAct, &QAction::triggered, this, &ContactsView::editEntry);
    connect(removeAct, &QAction::triggered, this, &ContactsView::removeEntry);
    connect(&_ContactListView, &QTreeWidget::itemClicked, this, &ContactsView::contactSelected);

    connect(&_BtnSearch, &QAbstractButton::clicked, this, &ContactsView::search);
    connect(&_ToBtn, &QAbstractButton::clicked, this, &ContactsView::insertTo);
    connect(&_CCBtn, &QAbstractButton::clicked, this, &ContactsView::insertCc);
    connect(&_BCCBtn, &QAbstractButton::clicked, this, &ContactsView::insertBcc);

    //connect(contactAddDialogPtr, &ContactAddDialog::accepted, this, &ContactsView::saveEntry);

    toolbar.addAction(addAct);
    toolbar.addAction(editAct);
    toolbar.addAction(removeAct);

    _SearchLayout.addWidget(&_TxtSearch);
    _SearchLayout.addWidget(&_BtnSearch);

    _Layout.addWidget(&toolbar);
    _Layout.addLayout(&_SearchLayout);
    _Layout.addWidget(&_ContactListView);

    _ContactSelectionLayout.addWidget(&_ToBtn, 0, 0);
    _ContactSelectionLayout.addWidget(&_ToTxt, 0, 1);

    _ContactSelectionLayout.addWidget(&_CCBtn, 1, 0);
    _ContactSelectionLayout.addWidget(&_CCTxt, 1, 1);

    _ContactSelectionLayout.addWidget(&_BCCBtn, 2, 0);
    _ContactSelectionLayout.addWidget(&_BCCTxt, 2, 1);

    _ToBtn.setText("To");
    _CCBtn.setText("Cc");
    _BCCBtn.setText("Bcc");

    _Layout.addLayout(&_ContactSelectionLayout);

    _Layout.setMargin(5);

    setLayout(&_Layout);

    setWindowTitle(tr("Address Book"));

    QIcon icon(":/images/addressbook.png");
    setWindowIcon(icon);

    setMinimumHeight(480);
    setMinimumWidth(640);
    setMaximumHeight(480);
    setMaximumWidth(640);
}

void ContactsView::setContactSelectionFlag(bool flag)
{
    _ContactSelctionMode = flag;

    if(_ContactSelctionMode)
    {
        _ToBtn.setVisible(true);
        _CCBtn.setVisible(true);
        _BCCBtn.setVisible(true);
        _ToTxt.setVisible(true);
        _CCTxt.setVisible(true);
         _BCCTxt.setVisible(true);
    }
    else
    {
        _ToBtn.setVisible(false);
        _CCBtn.setVisible(false);
        _BCCBtn.setVisible(false);
        _ToTxt.setVisible(false);
        _CCTxt.setVisible(false);
         _BCCTxt.setVisible(false);
    }
}

void ContactsView::updateActions(const QItemSelection &selection)
{
    QModelIndexList indexes = selection.indexes();

    if (!indexes.isEmpty())
    {
        removeAct->setEnabled(true);
        editAct->setEnabled(true);
    }
    else
    {
        removeAct->setEnabled(false);
        editAct->setEnabled(false);
    }
}

void ContactsView::loadContacts()
{
    QList<Contact> list;

    contactEntityPtr->allContacts(list);

    for (int i=0; i < list.count(); ++i)
    {
        Contact rec = list.at(i);

        QTreeWidgetItem *profileItem = new QTreeWidgetItem(&_ContactListView);
        profileItem->setText(0, rec.LastName + ", " + rec.FirstName);
        profileItem->setText(1, rec.EmailId);
        profileItem->setData(1, Qt::UserRole, QVariant(rec.EmailId));
    }
}

void ContactsView::contactSelected(QTreeWidgetItem *item, int column)
{
    _CurrentItem = item;
    _CurrentEmail = item->data(1, Qt::UserRole).toString();
    editAct->setEnabled(true);
    removeAct->setEnabled(true);
}

void ContactsView::addEntry()
{
    _IsEdited = false;
    editAct->setEnabled(false);
    removeAct->setEnabled(false);
    //contactAddDialogPtr->resetFields();
    //contactAddDialogPtr->show();
}

void ContactsView::editEntry()
{
    _IsEdited = true;
    Contact rec = contactEntityPtr->cache()->value(_CurrentEmail);
    //contactAddDialogPtr->setRecord(rec);
    //contactAddDialogPtr->show();
}

void ContactsView::removeEntry()
{
    editAct->setEnabled(false);
    removeAct->setEnabled(false);

    if(contactEntityPtr->removeContact("EmailId", _CurrentEmail))
    {
        int idx = _ContactListView.indexOfTopLevelItem(_CurrentItem);
        _CurrentItem = _ContactListView.takeTopLevelItem(idx);

        delete _CurrentItem;

        _CurrentItem = nullptr;
    }
}

void ContactsView::saveEntry()
{
    Contact rec;
    //rec = contactAddDialogPtr->getRecord();

    if(_IsEdited)
    {
        contactEntityPtr->updateContact(rec);
       _CurrentItem->setText(0, rec.LastName + ", " + rec.FirstName);
    }
    else
    {
        contactEntityPtr->createContact(rec);
        QTreeWidgetItem *profileItem = new QTreeWidgetItem(&_ContactListView);
        profileItem->setText(0, rec.LastName + ", " + rec.FirstName);
        profileItem->setText(1, rec.EmailId);
        profileItem->setData(1, Qt::UserRole, QVariant(rec.EmailId));

    }
}

void ContactsView::insertTo()
{
    /*
    if(_CurrentItem != nullptr)
    {
        QString str = _ToTxt.text() + "," + _CurrentEmail;
        if(str.at(0) == ',')
        {
            str = str.remove(0,1);
        }
        _ToTxt.setText(str);

        if(mailCompostionWindowPtr->isVisible())
        {
            mailCompostionWindowPtr->addtoToList(_CurrentEmail);
        }
    }
    */
}

void ContactsView::insertCc()
{
    /*
    if(_CurrentItem != nullptr)
    {
        QString str = _CCTxt.text() + "," + _CurrentEmail;
        if(str.at(0) == ',')
        {
            str = str.remove(0,1);
        }
        _CCTxt.setText(str);

        if(mailCompostionWindowPtr->isVisible())
        {
            mailCompostionWindowPtr->addtoCcList(_CurrentEmail);
        }
    }
    */
}

void ContactsView::insertBcc()
{
    /*
    if(_CurrentItem != nullptr)
    {
        QString str = _BCCTxt.text() + "," + _CurrentEmail;
        if(str.at(0) == ',')
        {
            str = str.remove(0,1);
        }
        _BCCTxt.setText(str);

        if(mailCompostionWindowPtr->isVisible())
        {
            mailCompostionWindowPtr->addtoBccList(_CurrentEmail);
        }
    }
    */
}

void ContactsView::search()
{

}
