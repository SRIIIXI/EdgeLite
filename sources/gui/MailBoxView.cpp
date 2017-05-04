#include "MailBoxView.h"
#include "MailView.h"
#include "MailBoxEntity.h"
#include "Mail.h"
#include "MailStorage.h"
#include "ItemDelegates.h"

MailBoxView::MailBoxView(QWidget *parent) : QWidget(parent)
{
    _Toolbar.setOrientation(Qt::Horizontal);
    _Toolbar.addWidget(&_SearchBox);

    _SearchAction = new QAction(QIcon(ApplicationThemeManager.search()), "Search", nullptr);
    _RefreshActon = new QAction(QIcon(ApplicationThemeManager.refresh()), "Refresh", nullptr);

    _Toolbar.addAction(_SearchAction);
    _Toolbar.addAction(_RefreshActon);

    _MainLayout.addWidget(&_Toolbar, Qt::AlignLeft | Qt::AlignTop);
    _MainLayout.addWidget(&_MailList, Qt::AlignLeft | Qt::AlignTop);
    _MailList.setItemDelegate(new MailListItemDelegate(&_MailList));

    _MainLayout.addStretch(0);

    connect(&_MailList, &QListWidget::itemClicked, this, &MailBoxView::eventMailSelected);

    _MainLayout.setMargin(0);
    _MainLayout.setSpacing(0);

    setLayout(&_MainLayout);

    setMinimumWidth(500);
    setMaximumWidth(500);

    _ImapClientPtr = nullptr;
    _MailViewPtr = nullptr;
    _CurrentItem = nullptr;
}

MailBoxView::~MailBoxView()
{
}

void MailBoxView::initialize(MailView *ptr)
{
    _MailViewPtr = ptr;
}

void MailBoxView::eventLoadDirectory(ImapClient *ptr, QString uname, QString dirname, long mcount, long nextuid)
{
    _ImapClientPtr = ptr;
    _CurDir = dirname;
    _CurMailId = uname;
    _Count = mcount;
    _NextUid = nextuid;

    _MailList.clear();

    if(_MailViewPtr != nullptr)
    {
        _MailViewPtr->clear();
    }

    QList<MailHeader> hdrlist;

    mailBoxEntityPtr->selectedMails(hdrlist, "EMailId", uname);

    for (int idx = 0; idx < hdrlist.count(); idx++)
    {
        MailHeader hdr = hdrlist.at(idx);
        _OldMailMap.insert(hdr.messageId(), hdr);

        QListWidgetItem *mailItem = new QListWidgetItem();
        mailItem->setData(Qt::DisplayRole, QVariant(hdr.fromDisplayName()));
        mailItem->setData(Qt::UserRole, QVariant(hdr.messageId()));
        mailItem->setData(Qt::UserRole+1, QVariant(hdr.subject()));
        mailItem->setData(Qt::UserRole+2, QVariant(hdr.timeStamp()));
        mailItem->setData(Qt::UserRole+3, QVariant(false));
        _MailList.insertItem(0, mailItem);
    }

    //Do we have a IMAP client already assigned

    if(_ImapClientPtr != nullptr)
    {
        disconnect(_ImapClientPtr, &ImapClient::headerReceived, this, &MailBoxView::eventHeaderReceived);
        disconnect(_ImapClientPtr, &ImapClient::bodyReceived, this, &MailBoxView::eventBodyReceived);
        disconnect(_ImapClientPtr, &ImapClient::flagged, this, &MailBoxView::eventMailFlagged);
        disconnect(_ImapClientPtr, &ImapClient::deleted, this, &MailBoxView::eventMailDeleted);
        disconnect(_ImapClientPtr, &ImapClient::headerNotFound, this, &MailBoxView::eventHeaderNotFound);
        disconnect(_ImapClientPtr, &ImapClient::bodyNotFound, this, &MailBoxView::eventBodyNotFound);
        disconnect(_ImapClientPtr, &ImapClient::markedSeen, this, &MailBoxView::eventMarkedSeen);
    }

    connect(_ImapClientPtr, &ImapClient::headerReceived, this, &MailBoxView::eventHeaderReceived);
    connect(_ImapClientPtr, &ImapClient::bodyReceived, this, &MailBoxView::eventBodyReceived);
    connect(_ImapClientPtr, &ImapClient::flagged, this, &MailBoxView::eventMailFlagged);
    connect(_ImapClientPtr, &ImapClient::deleted, this, &MailBoxView::eventMailDeleted);
    connect(_ImapClientPtr, &ImapClient::headerNotFound, this, &MailBoxView::eventHeaderNotFound);
    connect(_ImapClientPtr, &ImapClient::bodyNotFound, this, &MailBoxView::eventBodyNotFound);
    connect(_ImapClientPtr, &ImapClient::markedSeen, this, &MailBoxView::eventMarkedSeen);

    if(_Count < 1)
    {
        return;
    }

    _PendingHeaderFetch = 1;

    if(_PendingHeaderFetch < _Count+1)
    {

        _ImapClientPtr->clearBuffer();
        _ImapClientPtr->getMessageHeader(_PendingHeaderFetch);

        emit loadingStarted();
    }
}

void MailBoxView::eventMailSelected(QListWidgetItem *item)
{
    _CurrentItem = item;
    _CurrentMessageNo = _CurrentItem->data(Qt::UserRole + 4).toInt();

    if(_NewMailMap.contains(_CurrentMessageNo))
    {
        if(_MailViewPtr != nullptr)
        {
            _CurrentHeader = _NewMailMap.value(_CurrentMessageNo);
            emit loadingStarted();
            _ImapClientPtr->getMessageBody(_CurrentMessageNo);
        }

        if(_CurrentMessageNo == 1)
        {
            _LastMessageNo = _CurrentMessageNo;
            emit firstMailSelected();
            return;
        }

        if(_CurrentMessageNo == _Count)
        {
            _LastMessageNo = _CurrentMessageNo;
            emit lastMailSelected();
            return;
        }

        if(_LastMessageNo == 1 || _LastMessageNo == _Count)
        {
            emit enableNavigation();
        }

        _LastMessageNo = _CurrentMessageNo;
    }
    else
    {
        _CurrentMessageId = _CurrentItem->data(Qt::UserRole).toString();
        MailStorage mc;
        mc.retrieveMail(_CurrentMessageId, _CurrentBody);
        _CurrentHeader = _OldMailMap.value(_CurrentMessageId);
        _MailViewPtr->setMail(_CurrentHeader, _CurrentBody);
    }
}

void MailBoxView::eventHeaderReceived(QString uname, QString dirname, long msgno, MailHeader emlhdr)
{
    _CurrentMessageId = emlhdr.messageId();

    if(msgno < 1)
    {
        return;
    }

    if(_CurDir.toLower().contains("inbox"))
    {
        emlhdr.setDirection("I");
    }

    if(_CurDir.toLower().contains("sent"))
    {
        emlhdr.setDirection("O");
    }

    if(!_OldMailMap.contains(emlhdr.messageId()))
    {
        QListWidgetItem *mailItem = new QListWidgetItem();
        mailItem->setData(Qt::DisplayRole, QVariant(emlhdr.fromDisplayName()));
        mailItem->setData(Qt::UserRole, QVariant(emlhdr.messageId()));
        mailItem->setData(Qt::UserRole+1, QVariant(emlhdr.subject()));
        mailItem->setData(Qt::UserRole+2, QVariant(emlhdr.timeStamp()));
        mailItem->setData(Qt::UserRole+3, QVariant(true));
        mailItem->setData(Qt::UserRole+4, QVariant((int)msgno));
        _MailList.insertItem(0, mailItem);

        _NewMailMap.insert(msgno, emlhdr);
    }

    _PendingHeaderFetch++;

    if(_PendingHeaderFetch < _Count+1)
    {
        _ImapClientPtr->clearBuffer();
        _ImapClientPtr->getMessageHeader(_PendingHeaderFetch);
    }
    else
    {
        emit loadingFinished(_Count);
    }
}

void MailBoxView::eventBodyReceived(QString uname, QString dirname, long msgno, MailBody emlbody)
{
    MailHeader eml = _NewMailMap.value(msgno);

    mailBoxEntityPtr->createMail(eml, uname);

    MailStorage mc;
    emlbody.setMessageId(eml.messageId());
    mc.storeMail(emlbody);
    emit loadingFinished(msgno);

    _ImapClientPtr->markAsSeen(_CurrentMessageNo);

    _NewMailMap.remove(_CurrentMessageNo);

    _CurrentBody = emlbody;

    _MailViewPtr->setMail(_CurrentHeader, _CurrentBody);
}

void MailBoxView::eventHeaderNotFound(QString uname, QString dirname, long msgno)
{
    /*
    _PendingHeaderFetch--;

    if(_PendingHeaderFetch > 0)
    {
        _ImapClientPtr->clearBuffer();
        _ImapClientPtr->getMessageHeader(_PendingHeaderFetch);
    }
    else
    {
        emit loadingFinished(_Count);
    }
    */
}

void MailBoxView::eventBodyNotFound(QString uname, QString dirname, long msgno)
{
    emit loadingFinished(_Count);
}

void MailBoxView::eventMailFlagged(long msgno)
{

}

void MailBoxView::eventMailDeleted(long msgno)
{
    /*
    int idx = indexOfTopLevelItem(_CurrentItem);
    _CurrentItem = takeTopLevelItem(idx);
    delete _CurrentItem;
    _CurrentItem = nullptr;
    _MailViewPtr->clear();
    */
}

void MailBoxView::eventMarkedSeen(QString uname, QString dirname, long msgno)
{

}

void MailBoxView::deleteCurrent()
{
    /*
    if(_ImapClientPtr != nullptr)
    {
        _ImapClientPtr->clearBuffer();
        _ImapClientPtr->deleteMessage(_CurrentMessageNo);
    }

    _NewMailMap.remove(_CurrentMessageNo);
    _CurrentMessageNo = 0;
    */
}

void MailBoxView::flagCurrent()
{
    /*
    if(_ImapClientPtr != nullptr)
    {
        _ImapClientPtr->clearBuffer();
        _ImapClientPtr->flagMessage(_CurrentMessageNo, "\\Flagged");
    }
    */
}

void MailBoxView::moveToNext()
{
    /*
    if(_CurrentMessageNo + 1 > _Count)
    {
        return;
    }

    _CurrentMessageNo++;

    if(_ImapClientPtr != nullptr)
    {
        emit loadingStarted();
        _ImapClientPtr->clearBuffer();
        _ImapClientPtr->getMessageBody(_CurrentMessageNo);
    }
    */
}

void MailBoxView::moveToPrevious()
{
    /*
    if(_CurrentMessageNo - 1 < 1)
    {
        return;
    }

    _CurrentMessageNo--;

    if(_ImapClientPtr != nullptr)
    {
        emit loadingStarted();
        _ImapClientPtr->clearBuffer();
        _ImapClientPtr->getMessageBody(_CurrentMessageNo);
    }
    */
}

MailHeader MailBoxView::currentMailHeader()
{
    return _CurrentHeader;
}

MailBody MailBoxView::currentMailBody()
{
    return _CurrentBody;
}

void MailBoxView::clear()
{
    _MailList.clear();
}



