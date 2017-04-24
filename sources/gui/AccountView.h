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
#include "ImageLabel.h"
#include "ImapClient.h"
#include "AccountEntity.h"

class AccountView : public QWidget
{
    Q_OBJECT
public:
    AccountView(QWidget* parent = nullptr);
    virtual ~AccountView();

    void initialize();

    void addAccount(QString emailId);
    void removeAccount(QString emailId);
    void updateAccount(QString emailId);

signals:
    void loadDirectory(ImapClient* ptr, QString uname, QString dirname, long count, long nextuid);

private slots:
    void eventAuthenticated(QString uname);
    void eventdirectoryListReceived(QString uname, QStringList slist);
    void eventdirectoryReceived(QString uname, QString dirname, long count, long nextuid);
    void eventDirectorySelected(QListWidgetItem *item);
    void eventAccountSelected(QListWidgetItem *item);

private:
    void addAccount(Account &pf);

    QVBoxLayout _MainLayout;

    ImageLabel _NewMailLabel;
    ImageLabel  _AccountsLabel;
    QListWidget _AccountList;
    QListWidget _DirectoryList;
    ImageLabel  _DirectoryLabel;
    QToolBar    _Toolbar;

    QMap<QString, ImapClient*> _MailboxMap;
    QMap<QString, QStringList> _DirectoryMap;
    int _ProfilesLoaded;
};

#endif
