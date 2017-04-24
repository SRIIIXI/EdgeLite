#include "AccountView.h"
#include "ItemDelegates.h"
#include "ThemeManager.h"

AccountView::AccountView(QWidget *parent) : QWidget(parent)
{
    _NewMailLabel.setText("New mail");
    _NewMailLabel.setImageFile(":images/light/newemail.png");

    _AccountsLabel.setText("Accounts");
    _AccountsLabel.setImageFile(":images/light/account.png");

    _DirectoryLabel.setText("Folders");
    _DirectoryLabel.setImageFile(":images/light/folder.png");

    _AccountList.setItemDelegate(new AccountListItemDelegate(&_AccountList));

    _DirectoryList.setItemDelegate(new DirectoryListItemDelegate(&_DirectoryList));

    _AccountList.setFrameStyle(QFrame::NoFrame);
    _DirectoryList.setFrameStyle(QFrame::NoFrame);

    _Toolbar.setOrientation(Qt::Horizontal);
    _Toolbar.addAction(ApplicationThemeManager.addressBook());
    _Toolbar.addAction(ApplicationThemeManager.accounts());
    _Toolbar.addAction(ApplicationThemeManager.settings());

    _MainLayout.addWidget(&_NewMailLabel, Qt::AlignLeft | Qt::AlignTop);
    _MainLayout.addWidget(&_AccountsLabel, Qt::AlignLeft | Qt::AlignTop);
    _MainLayout.addWidget(&_AccountList, Qt::AlignLeft | Qt::AlignTop);
    _MainLayout.addWidget(&_DirectoryLabel, Qt::AlignLeft | Qt::AlignTop);
    _MainLayout.addWidget(&_DirectoryList, Qt::AlignLeft | Qt::AlignTop);
    _MainLayout.addStretch(0);
    _MainLayout.addWidget(&_Toolbar, Qt::AlignLeft | Qt::AlignTop);

    connect(&_AccountList, &QListWidget::itemClicked, this, &AccountView::eventAccountSelected);
    connect(&_DirectoryList, &QListWidget::itemClicked, this, &AccountView::eventDirectorySelected);

    _MainLayout.setMargin(0);
    _MainLayout.setSpacing(0);

    setLayout(&_MainLayout);

    setMinimumWidth(250);
    setMaximumWidth(250);
}

AccountView::~AccountView()
{

}

void AccountView::initialize()
{
    _ProfilesLoaded = 0;

    QMapIterator<QString, Account> iter(*accountEntityPtr->cache());

     while (iter.hasNext())
     {
         iter.next();
         Account pf = iter.value();
         addAccount(pf);
     }
}

void AccountView::eventAuthenticated(QString uname)
{
    if(_MailboxMap.contains(uname))
    {
        ImapClient* imapptr = _MailboxMap.value(uname);

        if(imapptr != nullptr)
        {
            imapptr->getDirectoryList();
        }
    }
}

void AccountView::eventdirectoryListReceived(QString uname, QStringList slist)
{
    slist.sort();

    QString inboxstring = "";

    int tctr = 0;

    for(tctr = 0; tctr < slist.count(); tctr++)
    {
        if(slist.at(tctr).toLower().contains("inbox"))
        {
           inboxstring = slist.at(tctr);
           break;
        }
    }

    if(inboxstring.length() > 1)
    {
        slist.removeAt(tctr);
        slist.insert(0, inboxstring);
    }

    _ProfilesLoaded++;

    _DirectoryMap.insert(uname, slist);

    if(_ProfilesLoaded == _MailboxMap.count())
    {
        QListWidgetItem *firstaccountitem = _AccountList.item(0);

        if(firstaccountitem != nullptr)
        {
            firstaccountitem->setSelected(true);
            eventAccountSelected(firstaccountitem);
        }
    }
}

void AccountView::eventdirectoryReceived(QString uname, QString dirname, long count, long nextuid)
{
    if(_MailboxMap.contains(uname))
    {
        ImapClient* imapptr = _MailboxMap.value(uname);

        if(imapptr != nullptr)
        {
            emit loadDirectory(imapptr, uname, dirname, count, nextuid);
        }
    }
}

void AccountView::eventAccountSelected(QListWidgetItem *item)
{
    _DirectoryList.clear();

    QString emailid = QVariant(item->data(Qt::UserRole)).toString();

    if(!_DirectoryMap.contains(emailid))
    {
        return;
    }

    QStringList slist = _DirectoryMap.value(emailid);

    foreach(QString str, slist)
    {
        QListWidgetItem *item = new QListWidgetItem(&_DirectoryList);
        item->setText(str);
        item->setData(Qt::DisplayRole, QVariant(str));
        item->setData(Qt::UserRole, QVariant(str));
        item->setData(Qt::UserRole+1, QVariant(emailid));
    }
}


void AccountView::eventDirectorySelected(QListWidgetItem *item)
{
    QString dirname = item->data(Qt::UserRole).toString();
    QString emailid = item->data(Qt::UserRole+1).toString();

    if(_MailboxMap.contains(emailid))
    {
        ImapClient* imapptr = _MailboxMap.value(emailid);

        if(imapptr != nullptr)
        {
            imapptr->getDirectory(dirname);
        }
    }
}

void AccountView::addAccount(Account &pf)
{
    QListWidgetItem *item = new QListWidgetItem(&_AccountList);
    item->setData(Qt::DisplayRole, QVariant(pf.AccountName));
    item->setData(Qt::UserRole, QVariant(pf.EMailId));

    ImapClient* imapptr = new ImapClient(pf.MailInServer, QVariant(pf.MailInPort).toInt(), pf.MailInUser, pf.MailInPass, pf.MailInSecurity.at(0).toLatin1());

    QString provider = imapptr->provider();

    if(provider.toLower().contains("gmail"))
    {
        item->setData(Qt::DecorationRole, QVariant(QPixmap(":images/Google.png").scaled(48, 48)));
    }
    else
    {
        if(provider.toLower().contains("outlook"))
        {
            item->setData(Qt::DecorationRole, QVariant(QPixmap(":images/Outlook.png").scaled(48, 48)));
        }
        else
        {
            if(provider.toLower().contains("yahoo"))
            {
                item->setData(Qt::DecorationRole, QVariant(QPixmap(":images/Yahoo.png").scaled(48, 48)));
            }
            else
            {
               item->setData(Qt::DecorationRole, QVariant(QPixmap(":images/EmailGeneric.png").scaled(48, 48)));
            }
        }
    }

    connect(imapptr, &ImapClient::authenticated, this, &AccountView::eventAuthenticated);
    connect(imapptr, &ImapClient::directoryListReceived, this, &AccountView::eventdirectoryListReceived);
    connect(imapptr, &ImapClient::directoryReceived, this, &AccountView::eventdirectoryReceived);

    _MailboxMap.insert(pf.EMailId, imapptr);

    imapptr->connect();
}


void AccountView::addAccount(QString emailId)
{
    Account pf = accountEntityPtr->cache()->value(emailId);
    addAccount(pf);
}

void AccountView::removeAccount(QString emailId)
{
    /*
    QTreeWidgetItem *item = nullptr;
    int i = 0;

    for( i = 0; i < topLevelItemCount(); ++i )
    {
       item = topLevelItem( i );

       QString udata = item->data(0, Qt::UserRole).toString();

       if(udata == emailId)
       {
           break;
       }
    }

    if(item != nullptr)
    {
        item = takeTopLevelItem(i);
        delete item;

        ImapClient* imapptr = _MailboxMap.value(emailId);
        _MailboxMap.remove(emailId);

        if(imapptr != nullptr)
        {
            imapptr->close();
            imapptr->deleteLater();
        }
    }
    */
}

void AccountView::updateAccount(QString emailId)
{

}

