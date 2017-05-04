#ifndef _MAIL_NEW_VIEW
#define _MAIL_NEW_VIEW

#include <QtCore>
#include <QFileDialog>
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QMovie>

#include "MailClient.h"
#include "SmtpClient.h"
#include "AccountEntity.h"
#include "Mail.h"
#include "ThemeManager.h"

class MailNewView : public QWidget
{
    Q_OBJECT
public:
    MailNewView(QWidget *parent = 0);
    void setMail(MailHeader emlhdr, MailBody emlbdy);

    void addtoToList(QString addr);
    void addtoCcList(QString addr);
    void addtoBccList(QString addr);

private slots:
    void eventSelectContacts();
    void eventSend();
    void eventProfileSelected();
    void eventAuthenticated(QString uname);
    void eventAttach();
    void eventMailDeliveryStatus(QString uname, QString msgid, bool state);

private:
    void disableInputs();
    void enableInputs();

    QGridLayout _Layout;
    QGridLayout _MainLayout;

    QPushButton _ToLbl;
    QPushButton _CCLbl;
    QPushButton _BCCLbl;
    QLabel _SubjectLbl;
    QLabel _ProfileLbl;
    QPushButton _AttachBtn;

    QLineEdit _ToTxt;
    QLineEdit _CCTxt;
    QLineEdit _BCCTxt;
    QLineEdit _SubjectTxt;
    QComboBox _ProfileTxt;
    QListWidget _FileList;

    QPushButton _SendBtn;
    QTextEdit   _Editor;

    IndicatorBar _SendIndicator;

    SmtpClient* _SMTPClientPtr;
    QList<Account> _ProfileList;
    MailHeader    _Header;
    MailBody      _Body;
};

#endif
