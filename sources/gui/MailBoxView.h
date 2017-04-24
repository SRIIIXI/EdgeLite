#ifndef _MAILBOX_VIEW
#define _MAILBOX_VIEW

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QtCore>
#include <QToolBar>
#include <QLineEdit>
#include "MailView.h"
#include "ImapClient.h"
#include "Mail.h"

/*
class MailBoxView : public QTreeWidget
{
    Q_OBJECT
public:
    MailBoxView(QWidget *parent = nullptr);
    ~MailBoxView();
    void initialize(MailView* ptr);

    void moveToNext();
    void moveToPrevious();

    void deleteCurrent();
    void flagCurrent();

    MailHeader currentMailHeader();
    MailBody currentMailBody();

signals:
    void loadingStarted();
    void loadingFinished(long num);
    void firstMailSelected();
    void lastMailSelected();
    void enableNavigation();

public slots:
    void eventLoadDirectory(ImapClient* ptr, QString uname, QString dirname, long mcount, long nextuid);
    void eventMailSelected(QTreeWidgetItem *item, int column);

    void eventHeaderReceived(QString uname, QString dirname, long msgno, MailHeader emlhdr);
    void eventBodyReceived(QString uname, QString dirname, long msgno, MailBody emlbody);
    void eventMailFlagged(long msgno);
    void eventMailDeleted(long msgno);
    void eventHeaderNotFound(QString uname, QString dirname, long msgno);
    void eventBodyNotFound(QString uname, QString dirname, long msgno);
    void eventMarkedSeen(QString uname, QString dirname, long msgno);

private:
    QString _CurDir;
    QString _CurMailId;
    long _Count;
    long _NextUid;
    ImapClient* _ImapClientPtr;
    MailView* _MailViewPtr;
    long    _PendingHeaderFetch;
    long _CurrentMessageNo;
    long _LastMessageNo;
    MailHeader _CurrentHeader;
    MailBody   _CurrentBody;
    QString _CurrentMessageId;
    QTreeWidgetItem* _CurrentItem;
    QMap<int, MailHeader> _NewMailMap;
    QMap<QString, MailHeader> _OldMailMap;
};
*/

class MailBoxView : public QWidget
{
    Q_OBJECT
public:
    MailBoxView(QWidget* parent = nullptr);
    ~MailBoxView();
    void initialize(MailView *ptr);

    void moveToNext();
    void moveToPrevious();

    void deleteCurrent();
    void flagCurrent();

    MailHeader currentMailHeader();
    MailBody currentMailBody();

    void clear();

signals:
    void loadingStarted();
    void loadingFinished(long num);
    void firstMailSelected();
    void lastMailSelected();
    void enableNavigation();

public slots:
    void eventLoadDirectory(ImapClient* ptr, QString uname, QString dirname, long mcount, long nextuid);
    void eventMailSelected(QListWidgetItem *item);

    void eventHeaderReceived(QString uname, QString dirname, long msgno, MailHeader emlhdr);
    void eventBodyReceived(QString uname, QString dirname, long msgno, MailBody emlbody);
    void eventMailFlagged(long msgno);
    void eventMailDeleted(long msgno);
    void eventHeaderNotFound(QString uname, QString dirname, long msgno);
    void eventBodyNotFound(QString uname, QString dirname, long msgno);
    void eventMarkedSeen(QString uname, QString dirname, long msgno);

private:
    QVBoxLayout _MainLayout;
    QLineEdit  _SearchBox;
    QToolBar    _Toolbar;
    QListWidget _MailList;

    QString _CurDir;
    QString _CurMailId;
    long _Count;
    long _NextUid;
    ImapClient* _ImapClientPtr;
    MailView* _MailViewPtr;
    long    _PendingHeaderFetch;
    long _CurrentMessageNo;
    long _LastMessageNo;
    MailHeader _CurrentHeader;
    MailBody   _CurrentBody;
    QString _CurrentMessageId;
    QListWidgetItem* _CurrentItem;
    QMap<int, MailHeader> _NewMailMap;
    QMap<QString, MailHeader> _OldMailMap;

};

#endif // DIRECTORYVIEW_H
