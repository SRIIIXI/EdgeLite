#include "ConfigurationView.h"
#include "CustomWidgets.h"
#include "IPCamDesktop.h"
#include "ThemeManager.h"
#include "AppSettings.h"

ConfigurationView::ConfigurationView(QWidget* parent) : QWidget(parent)
{
    _Layout.addWidget(new HorizontalLine());

    setupServiceAndSiteGroup();

    _Layout.addWidget(new HorizontalLine());

    setupLicenseFileGroup();

    _Layout.addWidget(new HorizontalLine());

    _Layout.addStretch();

    _Layout.addWidget(new HorizontalLine());

    _Layout.setMargin(0);
    _Layout.setSpacing(0);

    setLayout(&_Layout);

    ApplicationThemeManager.applyLightTheme(applicationPtr);
}

void ConfigurationView::setupServiceAndSiteGroup()
{
    _txtRestServicePassword.setEchoMode(QLineEdit::Password);

    _RestServiceConnectGroup.setText("Site Settings");
    _lblRestServiceUsername.setText("Username");
    _lblRestServicePassword.setText("Password");
    _lblHostname.setText("Hostname");
    _lblPortNum.setText("Port");
    _lblConnectionStatus.setText("Connection Status");
    _btnConnect.setMaximumWidth(75);
    _btnConnect.setMinimumWidth(75);
    _btnConnect.setText("&Connect");

    _btnSave.setMaximumWidth(75);
    _btnSave.setMinimumWidth(75);
    _btnSave.setText("&Save");

    _lblSiteId.setText("Site Id");
    _txtConnectionStatus.setText("Not Connected");

    _lblContactFirstName.setText("Contact Firstname");
    _lblContactLastName.setText("Contact Lastname");
    _lblEmailId.setText("Contact EmailId");
    _lblContactPhoneNo.setText("Contact PhoneNo");

    _lblRecStart.setText("Recording download starts");
    _lblRecEnd.setText("Recording download ends");

    _RestServiceConnectLayout.addWidget(&_RestServiceConnectGroup,1,0);
    _RestServiceConnectLayout.addWidget(&_lblRestServiceUsername,3,0);
    _RestServiceConnectLayout.addWidget(&_lblRestServicePassword,3,2);
    _RestServiceConnectLayout.addWidget(&_lblHostname,4,0);
    _RestServiceConnectLayout.addWidget(&_lblPortNum,4,2);

    _RestServiceConnectLayout.addWidget(&_txtRestServiceUsername,3,1);
    _RestServiceConnectLayout.addWidget(&_txtRestServicePassword,3,3);
    _RestServiceConnectLayout.addWidget(&_txtRestServiceHostname,4,1);
    _RestServiceConnectLayout.addWidget(&_txtRestServicePortNum,4,3);

    _RestServiceConnectLayout.addWidget(&_btnConnect,5 ,3 , Qt::AlignRight);

    _RestServiceConnectLayout.addWidget(&_lblConnectionStatus,6,0);
    _RestServiceConnectLayout.addWidget(&_txtConnectionStatus,6,1);

    _RestServiceConnectLayout.addWidget(&_lblSiteId, 6, 2);
    _RestServiceConnectLayout.addWidget(&_txtSiteId, 6 , 3);

    _RestServiceConnectLayout.addWidget(&_lblContactFirstName, 7, 0);
    _RestServiceConnectLayout.addWidget(&_txtContactFirstName, 7 , 1);

    _RestServiceConnectLayout.addWidget(&_lblContactLastName, 7, 2);
    _RestServiceConnectLayout.addWidget(&_txtContactLastName, 7 , 3);

    _RestServiceConnectLayout.addWidget(&_lblEmailId, 8, 0);
    _RestServiceConnectLayout.addWidget(&_txtEmailId, 8 , 1);

    _RestServiceConnectLayout.addWidget(&_lblContactPhoneNo, 8, 2);
    _RestServiceConnectLayout.addWidget(&_txtContactPhoneNo, 8 , 3);

    _RestServiceConnectLayout.addWidget(&_lblRecStart, 9, 1);
    _RestServiceConnectLayout.addWidget(&_txtRecStart, 10 , 1);

    _RestServiceConnectLayout.addWidget(&_lblRecEnd, 9, 3);
    _RestServiceConnectLayout.addWidget(&_txtRecEnd, 10 , 3);

    _RestServiceConnectLayout.addWidget(&_btnSave,11 ,3 , Qt::AlignRight);


    _RestServiceConnectLayout.setMargin(5);
    _RestServiceConnectLayout.setSpacing(5);
    _Layout.addLayout(&_RestServiceConnectLayout);
    connect(&_btnConnect,SIGNAL(clicked()),this,SLOT(OnRestServiceConnect()));
    connect(&_btnSave,SIGNAL(clicked()),this,SLOT(OnSiteSave()));

    _txtRestServiceHostname.setText(applicationPtr->settings()->Host);
    _txtRestServicePortNum.setText(applicationPtr->settings()->Port);
    _txtRestServiceUsername.setText(applicationPtr->settings()->User);
    _txtRestServicePassword.setText(applicationPtr->settings()->Pass);
}

void ConfigurationView::setupLicenseFileGroup()
{
    _lblLicenseFile.setMaximumWidth(105);
    _lblLicenseFile.setMinimumWidth(105);
    _lblLicenseFile.setText("License File");
    _btnLicenseFile.setMaximumWidth(75);
    _btnLicenseFile.setMinimumWidth(75);
    _btnLicenseFile.setText("&Import...");

    _LicenseFileLayout.addWidget(&_lblLicenseFile);
    _LicenseFileLayout.addWidget(&_txtLicenseFile);
    _LicenseFileLayout.addWidget(&_btnLicenseFile);
    _LicenseFileLayout.setMargin(5);
    _Layout.addLayout(&_LicenseFileLayout);
    connect(&_btnLicenseFile,SIGNAL(clicked()),this,SLOT(OnLicenseImport()));

     _txtLicenseFile.setText(applicationPtr->settings()->LicenseFile);
}

void ConfigurationView::OnRestServiceConnect()
{
    applicationPtr->checkStartup(_txtRestServiceHostname.text(), _txtRestServicePortNum.text(), _txtRestServiceUsername.text(), _txtRestServicePassword.text());
}

void ConfigurationView::OnLicenseImport()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),  QDir::homePath(), "License file (*.lic)");

    if (!fileName.isEmpty())
    {
        if(applicationPtr->verifyLicense(fileName))
        {
            _txtLicenseFile.setText(fileName);
            applicationPtr->saveLicenseFile();
        }
        else
        {
            QMessageBox msg;
            msg.setText("Invalid license file\nEither the license date has expired\nOr the license is bound to another computer hardware");
            msg.exec();
        }
    }
}

void ConfigurationView::updateLicenseFile(QString str)
{
    _txtLicenseFile.setText(str);
}

void ConfigurationView::updateRestServiceStatus(bool flag, Site st)
{
    if(flag)
    {
        _btnConnect.setEnabled(true);
        _txtConnectionStatus.setText("Connected");
        _txtSiteId.setText(st.site_id);

        _txtContactFirstName.setText(st.contact_first_name);
        _txtContactLastName.setText(st.contact_last_name);
        _txtEmailId.setText(st.contact_email_id);
        _txtContactPhoneNo.setText(st.contact_phone_no);
    }
    else
    {
        _txtConnectionStatus.setText("Not Connected");
    }
}

void ConfigurationView::OnSiteSave()
{
    Site st;
    applicationPtr->saveSiteConfiguration(st);
}
