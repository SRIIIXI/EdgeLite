#include "ConfigurationView.h"
#include "CustomWidgets.h"
#include "Tracks.h"
#include "ThemeManager.h"
#include "AppSettings.h"

ConfigurationView::ConfigurationView(QWidget* parent) : QWidget(parent)
{
    _Layout.addWidget(new HorizontalLine());

    setupDataConnectionGroup();

    _Layout.addWidget(new HorizontalLine());

    setupDownloadDirGroup();

    _Layout.addWidget(new HorizontalLine());

    setupThemeSelectionGroup();

    _Layout.addWidget(new HorizontalLine());

    _Layout.addStretch();

    _Layout.addWidget(new HorizontalLine());

    _Layout.setMargin(0);
    _Layout.setSpacing(0);

    setLayout(&_Layout);
}

void ConfigurationView::setupDataConnectionGroup()
{
    _txtDataSourceName.setMaximumWidth(175);
    _txtDatabaseUsername.setMaximumWidth(175);
    _txtDatabasePassword.setMaximumWidth(175);
    _txtHostname.setMaximumWidth(175);
    _txtPortNum.setMaximumWidth(175);
    _txtDatabasePassword.setEchoMode(QLineEdit::Password);

    _DataConnectGroup.setText("MySQL Connection Settings");
    _lblDataSourceName.setText("Datasource");
    _lblDatabaseUsername.setText("Username");
    _lblDatabasePassword.setText("Password");
    _lblHostname.setText("Hostname");
    _lblPortNum.setText("Port");
    _lblConnectionStatus.setText("Connection Status");
    _txtConnectionStatus.setText("Not Connected");
    _btnConnect.setMaximumWidth(75);
    _btnConnect.setMinimumWidth(75);
    _btnConnect.setText("&Connect");

    _DataConnectLayout.addWidget(&_DataConnectGroup,1,0);
    _DataConnectLayout.addWidget(&_lblDataSourceName,2,0);
    _DataConnectLayout.addWidget(&_lblDatabaseUsername,3,0);
    _DataConnectLayout.addWidget(&_lblDatabasePassword,3,2);
    _DataConnectLayout.addWidget(&_lblHostname,4,0);
    _DataConnectLayout.addWidget(&_lblPortNum,4,2);
    _DataConnectLayout.addWidget(&_lblConnectionStatus,5,0);

    _DataConnectLayout.addWidget(&_txtDataSourceName,2,1);
    _DataConnectLayout.addWidget(&_txtDatabaseUsername,3,1);
    _DataConnectLayout.addWidget(&_txtDatabasePassword,3,3);
    _DataConnectLayout.addWidget(&_txtHostname,4,1);
    _DataConnectLayout.addWidget(&_txtPortNum,4,3);
    _DataConnectLayout.addWidget(&_txtConnectionStatus,5,1);

    _DataConnectLayout.addWidget(&_btnConnect,5,3, Qt::AlignRight);
    _DataConnectLayout.setMargin(5);
    _DataConnectLayout.setSpacing(5);
    _Layout.addLayout(&_DataConnectLayout);
    connect(&_btnConnect,SIGNAL(clicked()),this,SLOT(OnDBConnect()));

     _txtDataSourceName.setText(tracksPtr->settings()->DatasourceName);
     _txtHostname.setText(tracksPtr->settings()->DatabaseHost);
     _txtPortNum.setText(tracksPtr->settings()->DatabasePort);
     _txtDatabaseUsername.setText(tracksPtr->settings()->DatabaseUser);
     _txtDatabasePassword.setText(tracksPtr->settings()->DatabasePass);

}

void ConfigurationView::setupDownloadDirGroup()
{
    _lblDownloadDir.setMaximumWidth(105);
    _lblDownloadDir.setMinimumWidth(105);
    _lblDownloadDir.setText("Download Directory");
    _btnDownloadDir.setMaximumWidth(75);
    _btnDownloadDir.setMinimumWidth(75);
    _btnDownloadDir.setText("&Browse...");

    _DownloadDirLayout.addWidget(&_lblDownloadDir);
    _DownloadDirLayout.addWidget(&_txtDownloadDir);
    _DownloadDirLayout.addWidget(&_btnDownloadDir);
    _DownloadDirLayout.setMargin(5);
    _Layout.addLayout(&_DownloadDirLayout);
    connect(&_btnDownloadDir,SIGNAL(clicked()),this,SLOT(OnDownloadDirBrowse()));

     _txtDownloadDir.setText(tracksPtr->settings()->RecordingsDirectory);
}

void ConfigurationView::setupThemeSelectionGroup()
{
    _RadGroup.addButton(&_btnLight);
    _RadGroup.addButton(&_btnDark);
    _lblTheme.setText("Application Theme");
    _btnLight.setText("Light");
    _btnDark.setText("Dark");
    _ThemeLayout.addWidget(&_lblTheme);
    _ThemeLayout.addWidget(&_btnLight);
    _ThemeLayout.addWidget(&_btnDark);
    _ThemeLayout.addStretch();
    _ThemeLayout.setMargin(5);
    _ThemeLayout.setSpacing(5);
    _Layout.addLayout(&_ThemeLayout);
    connect(&_btnLight,SIGNAL(clicked()),this,SLOT(OnThemeChange()));
    connect(&_btnDark,SIGNAL(clicked()),this,SLOT(OnThemeChange()));

    if(tracksPtr->settings()->Style == "L")
    {
       _btnLight.setChecked(true);
    }
    else
    {
        _btnDark.setChecked(true);
    }
}

void ConfigurationView::OnDBConnect()
{
    QSqlDatabase _Database = QSqlDatabase::addDatabase("QMYSQL");
    _Database.setDatabaseName(_txtDataSourceName.text());
    _Database.setUserName(_txtDatabaseUsername.text());
    _Database.setPassword(_txtDatabasePassword.text());
    _Database.setPort(QVariant(_txtPortNum.text()).toInt());
    _Database.setHostName(_txtHostname.text());

     if (!_Database.open())
     {
         _txtConnectionStatus.setText("Connection failed");
     }
     else
     {
        _txtConnectionStatus.setText("Connection suceeded");
        _Database.close();
        tracksPtr->settings()->DatasourceName = _txtDataSourceName.text();
        tracksPtr->settings()->DatabaseHost = _txtHostname.text();
        tracksPtr->settings()->DatabasePort = _txtPortNum.text();
        tracksPtr->settings()->DatabaseUser = _txtDatabaseUsername.text();
        tracksPtr->settings()->DatabasePass = _txtDatabasePassword.text();
        tracksPtr->saveDatabaseInfo();
     }
}

void ConfigurationView::OnDownloadDirBrowse()
{
    QString dirName = QFileDialog::getExistingDirectory(this, "Download Directory");

    if (!dirName.isEmpty())
    {
        _txtDownloadDir.setText(dirName);
        tracksPtr->settings()->RecordingsDirectory = _txtDownloadDir.text();
        tracksPtr->saveDownloadLocation();
    }
}

void ConfigurationView::OnThemeChange()
{
    if(_RadGroup.checkedButton()->text().toLower() == "light")
    {
        ApplicationThemeManager.applyLightTheme(tracksPtr);
    }

    if(_RadGroup.checkedButton()->text().toLower() == "dark")
    {
        ApplicationThemeManager.applyDarkTheme(tracksPtr);
    }

    if(_RadGroup.checkedButton()->text().toLower() == "light")
    {
        tracksPtr->settings()->Style = "L";
    }

    if(_RadGroup.checkedButton()->text().toLower() == "dark")
    {
        tracksPtr->settings()->Style = "D";
    }

    tracksPtr->saveStyleOption();
}
