#include "MailCompositionWindow.h"
#include "AddressBookWindow.h"

MailCompositionWindow* mailCompostionWindowPtr = nullptr;

MailCompositionWindow::MailCompositionWindow(QWidget *parent) : QWidget(parent)
{
    _Layout.addWidget(&_SendBtn, 0, 0, 1, 1);
    _Layout.addWidget(&_SendIndicator, 0, 1, 1, 1, Qt::AlignRight);

    _Layout.addWidget(&_ToLbl, 1, 0, 1, 1);
    _Layout.addWidget(&_ToTxt, 1, 1, 1, 1);

    _Layout.addWidget(&_CCLbl, 2, 0, 1, 1);
    _Layout.addWidget(&_CCTxt, 2, 1, 1, 1);

    _Layout.addWidget(&_BCCLbl, 3, 0, 1, 1);
    _Layout.addWidget(&_BCCTxt, 3, 1, 1,1);

    _Layout.addWidget(&_SubjectLbl, 4, 0, 1, 1);
    _Layout.addWidget(&_SubjectTxt, 4, 1, 1, 1);

    _Layout.addWidget(&_ProfileLbl, 5, 0, 1, 1);
    _Layout.addWidget(&_ProfileTxt, 5, 1, 1, 1);

    _Layout.addWidget(&_AttachBtn, 6, 0, 1, 1);
    _Layout.addWidget(&_FileList, 6, 1, 1, 1);

    _Layout.addWidget(&_Editor, 7, 0, 1, 2);


    _ToLbl.setText("To");
    _CCLbl.setText("Cc");
    _BCCLbl.setText("Bcc");
    _ProfileLbl.setText("Use Profile");
    _SendBtn.setText("Send");
    _AttachBtn.setText("Attach");

    _FileList.setFlow(QListWidget::LeftToRight);
    _FileList.setViewMode(QListView::ListMode);
    _FileList.setMovement(QListView::Static);
    _FileList.setMaximumHeight(48);
    _FileList.setMinimumHeight(48);
    _FileList.setSpacing(0);

    _ProfileTxt.setMaximumWidth(150);

    QString tl = "New mail";

    _MainLayout.addLayout(&_Layout, 0, 0);
    setLayout(&_MainLayout);

    setMinimumHeight(600);
    setMinimumWidth(800);

    QIcon icon(":/images/emailcompose.png");
    setWindowIcon(icon);
    setWindowTitle(tr("Compose Email"));

    accountEntityPtr->allAccounts(_ProfileList);

    for (int ctr = 0; ctr < _ProfileList.count(); ctr++ )
    {
        Account ps = _ProfileList.at(ctr);
        _ProfileTxt.addItem(ps.EMailId, ps.EMailId);
    }

    _SMTPClientPtr = nullptr;

    connect(&_ToLbl, &QAbstractButton::clicked, this, &MailCompositionWindow::eventSelectContacts);
    connect(&_CCLbl, &QAbstractButton::clicked, this, &MailCompositionWindow::eventSelectContacts);
    connect(&_BCCLbl, &QAbstractButton::clicked, this, &MailCompositionWindow::eventSelectContacts);

    connect(&_AttachBtn, &QAbstractButton::clicked, this, &MailCompositionWindow::eventAttach);
    connect(&_SendBtn, &QAbstractButton::clicked, this, &MailCompositionWindow::eventSend);

    mailCompostionWindowPtr = this;

//    adjustBackground(this);
//    adjustBackground(&_SubjectLbl);
//    adjustBackground(&_ProfileLbl);
//    adjustBackground(&_ToTxt);
//    adjustBackground(&_CCTxt);
//    adjustBackground(&_BCCTxt);
//    adjustBackground(&_SubjectTxt);
//    adjustBackground(&_FileList);
//    adjustBackground(&_Editor);
//    adjustBackground(&_SendIndicator);
 }


void MailCompositionWindow::eventSelectContacts()
{
    addressBookWindowPtr->setContactSelectionFlag(true);
    addressBookWindowPtr->show();
}

void MailCompositionWindow::eventProfileSelected()
{

}

void MailCompositionWindow::eventAuthenticated(QString uname)
{
    _Header.setFrom(_ProfileTxt.currentText());
    _Header.addtoToList(_ToTxt.text());
    _Header.addtoCcList(_CCTxt.text());
    _Header.addtoBccList(_BCCTxt.text());
    _Body.setMessage(_Editor.toPlainText());
    _Header.setSubject(_SubjectTxt.text());
    _Header.setDirection("O");

    _SMTPClientPtr->sendMail(_Header, _Body);
}

void MailCompositionWindow::eventMailDeliveryStatus(QString uname, QString msgid, bool state)
{
    enableInputs();

    disconnect(_SMTPClientPtr, &SmtpClient::authenticated, this, &MailCompositionWindow::eventAuthenticated);
    disconnect(_SMTPClientPtr, &SmtpClient::mailDeliveryStatus, this, &MailCompositionWindow::eventMailDeliveryStatus);
    _SMTPClientPtr->close();
    _SMTPClientPtr->deleteLater();
    _SMTPClientPtr = nullptr;

    if(!state)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error");
        msgBox.setText("Mail could not be sent\nServer returned the following message\n"+msgid);
        msgBox.exec();
    }

    close();
}

void MailCompositionWindow::eventSend()
{   

    if(_ToTxt.text().length() < 1)
    {
        return;
    }

    if(_SubjectTxt.text().length() < 1 || _Editor.toPlainText().length() < 1)
    {
        return;
    }

    Account prf = accountEntityPtr->cache()->value(_ProfileTxt.currentText());

    if(_SMTPClientPtr != nullptr)
    {
        disconnect(_SMTPClientPtr, &SmtpClient::authenticated, this, &MailCompositionWindow::eventAuthenticated);
        disconnect(_SMTPClientPtr, &SmtpClient::mailDeliveryStatus, this, &MailCompositionWindow::eventMailDeliveryStatus);
        _SMTPClientPtr->deleteLater();
        _SMTPClientPtr = nullptr;
    }

    _SMTPClientPtr = new SmtpClient();

    connect(_SMTPClientPtr, &SmtpClient::authenticated, this, &MailCompositionWindow::eventAuthenticated);
    connect(_SMTPClientPtr, &SmtpClient::mailDeliveryStatus, this, &MailCompositionWindow::eventMailDeliveryStatus);

    _SMTPClientPtr->setPublicIpAddress(mailClientPtr->publicIpAddress());

    bool usehandshake = true;

    if(prf.MailOutUseHandshake == "Y")
    {
       usehandshake = true;
    }
    else
    {
        usehandshake = false;
    }


    disableInputs();

    _SMTPClientPtr->connect(prf.MailOutServer, QVariant(prf.MailOutPort).toInt(), prf.MailOutUser, prf.MailOutPass, prf.MailOutSecurity.at(0).toLatin1(), usehandshake);
}

void MailCompositionWindow::eventAttach()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"),  QDir::homePath(), "All files (*.*)");

    if(!filename.isEmpty())
    {
        _Body.attachFile(filename);
        QFileInfo finfo(filename);
        QString str = finfo.fileName();
        _FileList.addItem(str);
    }
}

void MailCompositionWindow::disableInputs()
{
   _ToLbl.setEnabled(false);
   _CCLbl.setEnabled(false);
   _BCCLbl.setEnabled(false);
   _AttachBtn.setEnabled(false);
   _SendBtn.setEnabled(false);

   _ToTxt.setEnabled(false);
   _CCTxt.setEnabled(false);
   _BCCTxt.setEnabled(false);
   _SubjectTxt.setEnabled(false);
   _ProfileTxt.setEnabled(false);
   _FileList.setEnabled(false);
   _Editor.setEnabled(false);

   _SendIndicator.start();
}

void MailCompositionWindow::enableInputs()
{
    _ToLbl.setEnabled(true);
    _CCLbl.setEnabled(true);
    _BCCLbl.setEnabled(true);
    _AttachBtn.setEnabled(true);
    _SendBtn.setEnabled(true);

    _ToTxt.setEnabled(true);
    _CCTxt.setEnabled(true);
    _BCCTxt.setEnabled(true);
    _SubjectTxt.setEnabled(true);
    _ProfileTxt.setEnabled(true);
    _FileList.setEnabled(true);
    _Editor.setEnabled(true);

    _SendIndicator.stop();
}

void MailCompositionWindow::setMail(MailHeader emlhdr, MailBody emlbdy)
{
    _Header = emlhdr;
    _Body = emlbdy;

    _ToTxt.setText(_Header.toList().join(","));
    _CCTxt.setText(_Header.ccList().join(","));
    _BCCTxt.setText(_Header.bccList().join(","));
    _SubjectTxt.setText(_Header.subject());
    _Editor.setText(_Body.message());
}

void MailCompositionWindow::addtoToList(QString addr)
{
    QString str = _ToTxt.text() + "," + addr;
    if(str.at(0) == ',')
    {
        str = str.remove(0,1);
    }
    _ToTxt.setText(str);
}

void MailCompositionWindow::addtoCcList(QString addr)
{
    QString str = _CCTxt.text() + "," + addr;
    if(str.at(0) == ',')
    {
        str = str.remove(0,1);
    }
    _CCTxt.setText(str);
}

void MailCompositionWindow::addtoBccList(QString addr)
{
    QString str = _BCCTxt.text() + "," + addr;
    if(str.at(0) == ',')
    {
        str = str.remove(0,1);
    }
    _BCCTxt.setText(str);
}
