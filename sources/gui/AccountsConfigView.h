#ifndef _ACCOUNTS__CONFIG_VIEW
#define _ACCOUNTS__CONFIG_VIEW

#include <QtWidgets>
#include <QFrame>
#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QPixmap>
#include <QMessageBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QComboBox>
#include <QToolBar>
#include "AccountEntity.h"
#include "ImapClient.h"
#include "SmtpClient.h"
#include "ThemeManager.h"

class AccountsConfigView : public QWidget
{
    Q_OBJECT
public:
    AccountsConfigView(QWidget* parent = nullptr);
    void createProfileList();

signals:
    void accountAdded(QString emailId);
    void accountUpdated(QString emailId);
    void accountRemoved(QString emailId);

private slots:
    void accountSelected(QTreeWidgetItem *item, int column);

    void resetFields();
    void removeEntry();
    void saveEntry();

    void eventSmtpAuthenticated();
    void eventSmtpAuthenticationFailure(QString uname, QString pass);
    void eventImapAuthenticated();
    void eventImapAuthenticationFailure(QString uname, QString pass);

private:
    void createAccountContent();

    QGridLayout   _MainLayout;
    QTreeWidget   _ProfileListView;
    QToolBar      _Toolbar;

    QAction *addAct;
    QAction *removeAct;
    QAction *saveAct;
    QAction *progressAct;

    QLabel mailInGrplbl;
    QLabel mailInServlbl;
    QLabel mailInPortlbl;
    QLabel mailInUserlbl;
    QLabel mailInPasslbl;
    QLabel mailInSecrlbl;
    QLabel mailInProtolbl;
    QLabel namelbl;

    QLineEdit mailInServctl;
    QLineEdit mailInPortctl;
    QLineEdit mailInUserctl;
    QLineEdit mailInPassctl;
    QComboBox mailInSecrctl;
    QComboBox mailInProtoctl;
    QLineEdit namectl;

    QLabel mailOutGrplbl;
    QLabel mailOutServlbl;
    QLabel mailOutPortlbl;
    QLabel mailOutUserlbl;
    QLabel mailOutPasslbl;
    QLabel mailOutSecrlbl;
    QLabel mailOutHelolbl;
    QLabel emaillbl;

    QLineEdit mailOutServctl;
    QLineEdit mailOutPortctl;
    QLineEdit mailOutUserctl;
    QLineEdit mailOutPassctl;
    QComboBox mailOutSecrctl;
    QComboBox mailOutHeloctl;
    QLineEdit emailctl;

    Account _CurrentRecord;

    ImapClient* imapPtr;
    SmtpClient* smtpPtr;

    IndicatorCircular _Indicator;

    QLabel errorlbl;
    QTreeWidgetItem* _CurrentItem;
};

#endif
