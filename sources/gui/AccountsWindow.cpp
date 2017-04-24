#include "AccountsWindow.h"
#include "MailClient.h"

AccountsWindow* accountsWindowPtr = nullptr;

AccountsWindow::AccountsWindow(QWidget* parent) : QWidget(parent)
{
    imapPtr = nullptr;
    smtpPtr = nullptr;

    QIcon icon(":/images/profiles.png");
    setWindowIcon(icon);

    QLayout *ptr = layout();
    if(ptr != nullptr)
    {
        delete ptr;
    }

    _CurrentRecord.EMailId = "";

    addAct = new QAction(QIcon(":images/add.png"), tr("Add Entry..."), this);
    removeAct = new QAction(QIcon(":images/remove.png"), tr("Remove Entry"), this);
    saveAct = new QAction(QIcon(":images/save.png"), tr("Save Entry..."), this);

    removeAct->setEnabled(false);
    saveAct->setEnabled(false);

    _Toolbar.addAction(addAct);
    _Toolbar.addAction(removeAct);
    _Toolbar.addAction(saveAct);
    progressAct = _Toolbar.addWidget(&_Indicator);
    progressAct->setVisible(true);

    _MainLayout.addWidget(&_Toolbar, 0, 0, 1, 4, Qt::AlignTop | Qt::AlignLeft);
    _MainLayout.addWidget(&_ProfileListView, 1, 0, 1, 4, Qt::AlignTop | Qt::AlignLeft);

    createProfileList();

    createAccountContent();

    _MainLayout.setContentsMargins(QMargins(15, 0, 15, 15));
    _MainLayout.setSpacing(5);
    setLayout(&_MainLayout);
    setWindowTitle(tr("Email Profiles"));

    QPalette pal = _Toolbar.palette();
    pal.setColor(QPalette::Background, Qt::white);
    _Toolbar.setAutoFillBackground(true);
    _Toolbar.setPalette(pal);
    _Toolbar.setIconSize(QSize(32,32));
    //_Toolbar.setStyleSheet("QToolBar { background: white; }");

    setMinimumHeight(415);
    setMaximumHeight(495);

    setMinimumWidth(480);
    setMaximumWidth(480);

    _ProfileListView.setMinimumHeight(100);
    _ProfileListView.setMaximumHeight(100);

    _ProfileListView.setMinimumWidth(width() - 30);
    _ProfileListView.setMaximumWidth(width() - 30);

    QStringList lst;

    lst << "Profile Name" << "Email Id" << " ";

    _ProfileListView.setSelectionMode(QAbstractItemView::SingleSelection);

     _ProfileListView.setHeaderLabels(lst);
     _ProfileListView.setColumnWidth(0,150);
     _ProfileListView.setColumnWidth(1,150);
     _ProfileListView.setColumnWidth(2,50);

    connect(addAct, &QAction::triggered, this, &AccountsWindow::resetFields);
    connect(removeAct, &QAction::triggered, this, &AccountsWindow::removeEntry);
    connect(saveAct, &QAction::triggered, this, &AccountsWindow::saveEntry);
    connect(&_ProfileListView, &QTreeWidget::itemClicked, this, &AccountsWindow::accountSelected);

    accountsWindowPtr = this;

    //adjustBackground(this);
}

void AccountsWindow::createProfileList()
{
    if(accountEntityPtr->cache()->count() < 1)
    {
        saveAct->setEnabled(true);
        return;
    }

    QMapIterator<QString, Account> iter(*accountEntityPtr->cache());

     while (iter.hasNext())
     {
         iter.next();
         Account ps = iter.value();
         QTreeWidgetItem *profileItem = new QTreeWidgetItem(&_ProfileListView);
         profileItem->setText(0, ps.AccountName);
         profileItem->setText(1, ps.EMailId);
         profileItem->setText(2, " ");
         profileItem->setData(0, Qt::UserRole, QVariant(ps.EMailId));
     }
}

void AccountsWindow::createAccountContent()
{
    mailInGrplbl.setText("Mail Incoming");
    mailInServlbl.setText("Server");
    mailInPortlbl.setText("Port");
    mailInUserlbl.setText("User");
    mailInPasslbl.setText("Password");
    mailInSecrlbl.setText("Encryption");
    mailInProtolbl.setText("Protocol");
    namelbl.setText("Profile Name");

    mailOutGrplbl.setText("Mail Outcoimg");
    mailOutServlbl.setText("Server");
    mailOutPortlbl.setText("Port");
    mailOutUserlbl.setText("User");
    mailOutPasslbl.setText("Password");
    mailOutSecrlbl.setText("Encryption");
    mailOutHelolbl.setText("Send EHLO");
    emaillbl.setText("Email Id");

    _MainLayout.addWidget(&mailInGrplbl, 2, 0);
    _MainLayout.addWidget(&mailInServlbl, 4, 0);
    _MainLayout.addWidget(&mailInPortlbl, 5, 0);
    _MainLayout.addWidget(&mailInUserlbl, 6, 0);
    _MainLayout.addWidget(&mailInPasslbl, 7, 0);
    _MainLayout.addWidget(&mailInSecrlbl, 8, 0);
    _MainLayout.addWidget(&mailInProtolbl, 9, 0);
    _MainLayout.addWidget(&namelbl, 10, 0);

    _MainLayout.addWidget(&mailInServctl, 4, 1);
    _MainLayout.addWidget(&mailInPortctl, 5, 1);
    _MainLayout.addWidget(&mailInUserctl, 6, 1);
    _MainLayout.addWidget(&mailInPassctl, 7, 1);
    _MainLayout.addWidget(&mailInSecrctl, 8, 1);
    _MainLayout.addWidget(&mailInProtoctl, 9, 1);
    _MainLayout.addWidget(&namectl, 10, 1);

    _MainLayout.addWidget(&mailOutGrplbl, 2, 2);
    _MainLayout.addWidget(&mailOutServlbl, 4, 2);
    _MainLayout.addWidget(&mailOutPortlbl, 5, 2);
    _MainLayout.addWidget(&mailOutUserlbl, 6, 2);
    _MainLayout.addWidget(&mailOutPasslbl, 7, 2);
    _MainLayout.addWidget(&mailOutSecrlbl, 8, 2);
    _MainLayout.addWidget(&mailOutHelolbl, 9, 2);
    _MainLayout.addWidget(&emaillbl, 10, 2);

    _MainLayout.addWidget(&mailOutServctl, 4, 3);
    _MainLayout.addWidget(&mailOutPortctl, 5, 3);
    _MainLayout.addWidget(&mailOutUserctl, 6, 3);
    _MainLayout.addWidget(&mailOutPassctl, 7, 3);
    _MainLayout.addWidget(&mailOutSecrctl, 8, 3);
    _MainLayout.addWidget(&mailOutHeloctl, 9, 3);
    _MainLayout.addWidget(&emailctl, 10, 3);

    mailInSecrctl.addItem("Auto");
    mailInSecrctl.addItem("SSL");
    mailInSecrctl.addItem("TLS");
    mailInSecrctl.addItem("None");

    mailOutSecrctl.addItem("Auto");
    mailOutSecrctl.addItem("SSL");
    mailOutSecrctl.addItem("TLS");
    mailOutSecrctl.addItem("None");

    mailOutHeloctl.addItem("Yes");
    mailOutHeloctl.addItem("No");

    mailInProtoctl.addItem("IMAP");
    mailInProtoctl.addItem("POP3");

    mailInPassctl.setEchoMode(QLineEdit::Password);
    mailOutPassctl.setEchoMode(QLineEdit::Password);

    _MainLayout.addWidget(&errorlbl, 12, 0, 1, 4);

    errorlbl.setMinimumHeight(32);
}

void AccountsWindow::accountSelected(QTreeWidgetItem *item, int column)
{
    _CurrentRecord.EMailId = item->data(0, Qt::UserRole).toString();
    _CurrentItem = item;

    if(accountEntityPtr->cache()->contains(_CurrentRecord.EMailId))
    {
        _CurrentRecord = accountEntityPtr->cache()->value(_CurrentRecord.EMailId);

        mailInServctl.setText(_CurrentRecord.MailInServer);
        mailInPortctl.setText(_CurrentRecord.MailInPort);
        mailInUserctl.setText(_CurrentRecord.MailInUser);
        mailInPassctl.setText(_CurrentRecord.MailInPass);
        namectl.setText(_CurrentRecord.AccountName);

        mailOutServctl.setText(_CurrentRecord.MailOutServer);
        mailOutPortctl.setText(_CurrentRecord.MailOutPort);
        mailOutUserctl.setText(_CurrentRecord.MailOutUser);
        mailOutPassctl.setText(_CurrentRecord.MailOutPass);
        emailctl.setText(_CurrentRecord.EMailId);

        if(_CurrentRecord.MailOutUseHandshake == "Y")
        {
            mailOutHeloctl.setCurrentIndex(0);
        }
        else
        {
            mailOutHeloctl.setCurrentIndex(1);
        }

        if(_CurrentRecord.MailInProtocol == "I")
        {
            mailInProtoctl.setCurrentIndex(0);
        }
        else
        {
            mailInProtoctl.setCurrentIndex(1);
        }

        char ich = _CurrentRecord.MailInSecurity.at(0).toLatin1();
        char och = _CurrentRecord.MailOutSecurity.at(0).toLatin1();

        if(ich == 'A')
            mailInSecrctl.setCurrentIndex(0);

        if(ich == 'S')
            mailInSecrctl.setCurrentIndex(1);

        if(ich == 'T')
            mailInSecrctl.setCurrentIndex(2);

        if(ich == 'N')
            mailInSecrctl.setCurrentIndex(3);

        if(och == 'A')
            mailOutSecrctl.setCurrentIndex(0);

        if(och == 'S')
            mailOutSecrctl.setCurrentIndex(1);

        if(och == 'T')
            mailOutSecrctl.setCurrentIndex(2);

        if(och == 'N')
            mailOutSecrctl.setCurrentIndex(3);

        removeAct->setEnabled(true);
        saveAct->setEnabled(true);
    }
}

void AccountsWindow::removeEntry()
{
    if(accountEntityPtr->cache()->contains(_CurrentRecord.EMailId))
    {
        QString prfremoved = _CurrentRecord.EMailId;

        resetFields();

        accountEntityPtr->removeAccount("EMailId", QVariant(prfremoved));

        _CurrentRecord.AccountName = "";
        _CurrentRecord.EMailId = "";

        _CurrentRecord.MailInUser = "";
        _CurrentRecord.MailInPass = "";
        _CurrentRecord.MailInServer = "";
        _CurrentRecord.MailInPort = "";
        _CurrentRecord.MailInSecurity = "";
        _CurrentRecord.MailInProtocol = "";

        _CurrentRecord.MailOutUser = "";
        _CurrentRecord.MailOutPass = "";
        _CurrentRecord.MailOutServer = "";
        _CurrentRecord.MailOutPort = "";
        _CurrentRecord.MailOutSecurity = "";
        _CurrentRecord.MailOutUseHandshake = "";

        int idx = _ProfileListView.indexOfTopLevelItem(_CurrentItem);

        _CurrentItem = _ProfileListView.takeTopLevelItem(idx);

        delete _CurrentItem;

        _CurrentItem = nullptr;

        emit accountRemoved(prfremoved);
    }
}

void AccountsWindow::resetFields()
{
    mailInServctl.setText("");
    mailInPortctl.setText("");
    mailInUserctl.setText("");
    mailInPassctl.setText("");
    namectl.setText("");

    mailOutServctl.setText("");
    mailOutPortctl.setText("");
    mailOutUserctl.setText("");
    mailOutPassctl.setText("");
    emailctl.setText("");

    mailOutHeloctl.setCurrentIndex(0);
    mailInProtoctl.setCurrentIndex(0);

    mailInSecrctl.setCurrentIndex(0);
    mailOutSecrctl.setCurrentIndex(0);

    _CurrentRecord.AccountName = "";
    _CurrentRecord.EMailId = "";

    _CurrentRecord.MailInUser = "";
    _CurrentRecord.MailInPass = "";
    _CurrentRecord.MailInServer = "";
    _CurrentRecord.MailInPort = "";
    _CurrentRecord.MailInSecurity = "";
    _CurrentRecord.MailInProtocol = "";

    _CurrentRecord.MailOutUser = "";
    _CurrentRecord.MailOutPass = "";
    _CurrentRecord.MailOutServer = "";
    _CurrentRecord.MailOutPort = "";
    _CurrentRecord.MailOutSecurity = "";
    _CurrentRecord.MailOutUseHandshake = "";
}

void AccountsWindow::saveEntry()
{
    errorlbl.setText("");
    progressAct->setVisible(true);
    _Indicator.start();

    _CurrentRecord.AccountName = namectl.text();
    _CurrentRecord.EMailId = emailctl.text();

    _CurrentRecord.MailInUser = mailInUserctl.text();
    _CurrentRecord.MailInPass = mailInPassctl.text();
    _CurrentRecord.MailInServer = mailInServctl.text();
    _CurrentRecord.MailInPort = mailInPortctl.text();
    _CurrentRecord.MailInSecurity = mailInSecrctl.currentText().at(0).toLatin1();
    _CurrentRecord.MailInProtocol = mailInProtoctl.currentText().at(0).toLatin1();

    _CurrentRecord.MailOutUser = mailOutUserctl.text();
    _CurrentRecord.MailOutPass = mailOutPassctl.text();
    _CurrentRecord.MailOutServer = mailOutServctl.text();
    _CurrentRecord.MailOutPort = mailOutPortctl.text();
    _CurrentRecord.MailOutSecurity = mailOutSecrctl.currentText().at(0).toLatin1();
    _CurrentRecord.MailOutUseHandshake = mailOutHeloctl.currentText().at(0).toLatin1();

    imapPtr = new ImapClient();

    connect(imapPtr, &ImapClient::authenticated, this, &AccountsWindow::eventImapAuthenticated);
    connect(imapPtr, &ImapClient::authenticationFailure, this, &AccountsWindow::eventImapAuthenticationFailure);

    imapPtr->connect(_CurrentRecord.MailInServer,
                     QVariant(_CurrentRecord.MailInPort).toInt(),
                     _CurrentRecord.MailInUser,
                     _CurrentRecord.MailInPass,
                     _CurrentRecord.MailInSecurity.at(0).toLatin1());
}

void AccountsWindow::eventImapAuthenticated()
{
    disconnect(imapPtr, &ImapClient::authenticated, this, &AccountsWindow::eventImapAuthenticated);
    disconnect(imapPtr, &ImapClient::authenticationFailure, this, &AccountsWindow::eventImapAuthenticationFailure);
    imapPtr->close();
    imapPtr->deleteLater();
    imapPtr = nullptr;

    smtpPtr = new SmtpClient();

    connect(smtpPtr, &SmtpClient::authenticated, this, &AccountsWindow::eventSmtpAuthenticated);
    connect(smtpPtr, &SmtpClient::authenticationFailure, this, &AccountsWindow::eventSmtpAuthenticationFailure);

    bool handshake = true;

    if(_CurrentRecord.MailOutUseHandshake == "Y")
    {
        handshake = true;
    }
    else
    {
        handshake = false;
    }

    smtpPtr->setPublicIpAddress(mailClientPtr->publicIpAddress());
    smtpPtr->connect(_CurrentRecord.MailOutServer,
                     QVariant(_CurrentRecord.MailOutPort).toInt(),
                     _CurrentRecord.MailOutUser,
                     _CurrentRecord.MailOutPass,
                     _CurrentRecord.MailOutSecurity.at(0).toLatin1(),
                     handshake);

}

void AccountsWindow::eventSmtpAuthenticated()
{
    disconnect(smtpPtr, &SmtpClient::authenticated, this, &AccountsWindow::eventSmtpAuthenticated);
    disconnect(smtpPtr, &SmtpClient::authenticationFailure, this, &AccountsWindow::eventSmtpAuthenticationFailure);
    smtpPtr->close();
    smtpPtr->deleteLater();
    smtpPtr = nullptr;

    if(accountEntityPtr->cache()->contains(_CurrentRecord.EMailId))
    {
        accountEntityPtr->updateAccount(_CurrentRecord);
        emit accountUpdated(_CurrentRecord.EMailId);
    }
    else
    {
        accountEntityPtr->createAccount(_CurrentRecord);
        emit accountAdded(_CurrentRecord.EMailId);

        QTreeWidgetItem *profileItem = new QTreeWidgetItem(&_ProfileListView);
        profileItem->setText(0, _CurrentRecord.AccountName);
        profileItem->setText(1, _CurrentRecord.EMailId);
        profileItem->setText(2, " ");
        profileItem->setData(0, Qt::UserRole, QVariant(_CurrentRecord.EMailId));

        removeAct->setEnabled(true);
    }

    _Indicator.stop();
    progressAct->setVisible(false);
    errorlbl.setText("");
}

void AccountsWindow::eventImapAuthenticationFailure(QString uname, QString pass)
{
    disconnect(imapPtr, &ImapClient::authenticated, this, &AccountsWindow::eventImapAuthenticated);
    disconnect(imapPtr, &ImapClient::authenticationFailure, this, &AccountsWindow::eventImapAuthenticationFailure);
    imapPtr->close();
    imapPtr->deleteLater();
    imapPtr = nullptr;

    errorlbl.setText("<font color='red'>Incoming mail setting are incorrect</font>");
    _Indicator.stop();
    progressAct->setVisible(false);
}

void AccountsWindow::eventSmtpAuthenticationFailure(QString uname, QString pass)
{
    disconnect(smtpPtr, &SmtpClient::authenticated, this, &AccountsWindow::eventSmtpAuthenticated);
    disconnect(smtpPtr, &SmtpClient::authenticationFailure, this, &AccountsWindow::eventSmtpAuthenticationFailure);
    smtpPtr->close();
    smtpPtr->deleteLater();
    smtpPtr = nullptr;
    errorlbl.setText("<font color='red'>Outgoing mail setting are incorrect</font>");
    _Indicator.stop();
    progressAct->setVisible(false);
}


