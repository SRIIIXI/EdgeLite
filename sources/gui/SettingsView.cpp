#include "SettingsView.h"
#include "ThemeManager.h"

SettingsView::SettingsView(QWidget *parent) :
    QWidget(parent)
{
    setMinimumWidth(480);

    _lblGeneralSettings.setText("General Settings");
    _lblAutomark.setText("Automatically messages as read");
    _lblPlainTextOnly.setText("Show HTML content as plain text");
    _lblSysTrayIcon.setText("Show system tray icon");
    _txtAutomarkDelay.setMaximumWidth(40);

    _lblMailInSettings.setText("Incoming Mail Settings");
    _lblMailInIdleTimeout.setText("Idle timeout");
    _lblMailInRefreshInterval.setText("Refresh interval");
    _txtIdleTimeout.setMaximumWidth(40);
    _txtRefreshInterval.setMaximumWidth(40);

    _lblMailOutSettings.setText("Outgoing Mail Settings");
    _lblSaveOutgoing.setText("Save outgoing email");
    _lblSendBURL.setText("Use SMTP BURL");

    _lblOfflineSettings.setText("Offline Usage Settings");
    _rdbNoCache.setText("Don't store emails");
    _rdbAlwaysCache.setText("Always store emails");
    _rdbLimitedCached.setText("Store emails for days");
    _txtCacheDuration.setMaximumWidth(40);

    _btnSave.setText("Save");
    _btnCancel.setText("Cancel");

    _btnSave.setMaximumWidth(75);
    _btnCancel.setMaximumWidth(75);

    _btnSave.setMinimumWidth(75);
    _btnCancel.setMinimumWidth(75);

    _lytCommands.setAlignment(&_btnCancel, Qt::AlignRight);
    _lytCommands.setAlignment(&_btnSave, Qt::AlignRight);

    QFont ft = _lblGeneralSettings.font();
    ft.setBold(true);

    _lblGeneralSettings.setFont(ft);
    _lblMailInSettings.setFont(ft);
    _lblMailOutSettings.setFont(ft);
    _lblOfflineSettings.setFont(ft);

    _ControlGrid.addWidget(&_lblGeneralSettings,1,1);
    _ControlGrid.addWidget(&_lblAutomark,2,1);
    _ControlGrid.addWidget(&_txtAutomarkDelay,2,3, Qt::AlignRight);
    _ControlGrid.addWidget(&_lblPlainTextOnly,3,1);
    _ControlGrid.addWidget(&_lblSysTrayIcon,4,1);

    _ControlGrid.addWidget(&_lblMailInSettings,7,1);
    _ControlGrid.addWidget(&_lblMailInIdleTimeout,8,1);
    _ControlGrid.addWidget(&_txtIdleTimeout,8,3, Qt::AlignRight);
    _ControlGrid.addWidget(&_lblMailInRefreshInterval,9,1);
    _ControlGrid.addWidget(&_txtRefreshInterval,9,3, Qt::AlignRight);

    _ControlGrid.addWidget(&_lblMailOutSettings,12,1);
    _ControlGrid.addWidget(&_lblSaveOutgoing,13,1);
    _ControlGrid.addWidget(&_lblSendBURL, 14, 1);

    _ControlGrid.addWidget(&_lblOfflineSettings,17,1);
    _ControlGrid.addWidget(&_rdbNoCache,18,1);
    _ControlGrid.addWidget(&_rdbAlwaysCache,19,1);
    _ControlGrid.addWidget(&_rdbLimitedCached,20,1);
    _ControlGrid.addWidget(&_txtCacheDuration,20,3, Qt::AlignRight);

    _lytCommands.addWidget(&_btnSave);
    _lytCommands.addWidget(&_btnCancel);
    _ControlGrid.addLayout(&_lytCommands,23,3,1,1, Qt::AlignRight);

    setLayout(&_ControlGrid);

    connect(&_btnCancel,SIGNAL(clicked()),this,SLOT(close()));
    connect(&_btnSave,SIGNAL(clicked()),this,SLOT(eventSave()));

    setWindowIcon(QIcon(":/images/settings.png"));
    setWindowTitle(tr("Settings"));
}

void SettingsView::eventSave()
{
//    appConfiguration.beginGroup("ODBCConfiguration");
//    appConfiguration.setValue("DataSourceName", _txtDataSourceName.text());
//    appConfiguration.setValue("DatabaseUser", _txtDatabaseUsername.text());
//    appConfiguration.setValue("DatabasePassword", _txtDatabasePassword.text());
//    appConfiguration.endGroup();

//    appConfiguration.beginGroup("ProxyConfiguration");
//    appConfiguration.setValue("UseProxy", _chkProxyRequired.isChecked());
//    appConfiguration.setValue("ProxyServer", _txtProxyServer.text());
//    appConfiguration.setValue("ProxyPort", _txtProxyPort.text());
//    appConfiguration.setValue("ProxyUser", _txtProxyUsername.text());
//    appConfiguration.setValue("ProxyPassword", _txtProxyPassword.text());
//    appConfiguration.endGroup();

//    appConfiguration.beginGroup("LocaleSettings");
//    appConfiguration.setValue("LanguageFile", _lblLanguageFileName.text());
//    appConfiguration.endGroup();

//    appConfiguration.beginGroup("GISConfiguration");
//    appConfiguration.setValue("MapProvider", selectedMapProvider());
//    appConfiguration.setValue("UseCache", _chkCacheRequired.isChecked());
//    appConfiguration.setValue("HomeLongitude", _txtLon.text());
//    appConfiguration.setValue("HomeLatitude", _txtLat.text());
//    appConfiguration.endGroup();
}

void SettingsView::loadConfiguration()
{
//    appConfiguration.beginGroup("ODBCConfiguration");
//    _txtDataSourceName.setText(appConfiguration.GetValue("DataSourceName").toString());
//    _txtDatabaseUsername.setText(appConfiguration.GetValue("DatabaseUser").toString());
//    _txtDatabasePassword.setText(appConfiguration.GetValue("DatabasePassword").toString());
//    appConfiguration.endGroup();

//    appConfiguration.beginGroup("ProxyConfiguration");
//    if(appConfiguration.GetValue("UseProxy").toBool())
//    {
//        _chkProxyRequired.setCheckState(Qt::Checked);
//    }
//    else
//    {
//        _chkProxyRequired.setCheckState(Qt::Unchecked);
//    }
//    _txtProxyServer.setText(appConfiguration.GetValue("ProxyServer").toString());
//    _txtProxyPort.setText(appConfiguration.GetValue("ProxyPort").toString());
//    _txtProxyUsername.setText(appConfiguration.GetValue("ProxyUser").toString());
//    _txtProxyPassword.setText(appConfiguration.GetValue("ProxyPassword").toString());
//    appConfiguration.endGroup();

//    appConfiguration.beginGroup("LocaleSettings");
//    _lblLanguageFileName.setText(appConfiguration.GetValue("LanguageFile").toString());
//    appConfiguration.endGroup();

//    appConfiguration.beginGroup("GISConfiguration");
//    if(appConfiguration.GetValue("UseCache").toBool())
//    {
//        _chkCacheRequired.setCheckState(Qt::Checked);
//    }
//    else
//    {
//        _chkCacheRequired.setCheckState(Qt::Unchecked);
//    }

//    QString mprov = appConfiguration.GetValue("MapProvider").toString();

//    if(mprov == "OSM")
//    {
//        _cmbMapProvider.setCurrentIndex(0);
//    }

//    if(mprov == "Yahoo")
//    {
//        _cmbMapProvider.setCurrentIndex(1);
//    }

//    if(mprov == "Google")
//    {
//        _cmbMapProvider.setCurrentIndex(2);
//    }
//    _txtLon.setText(appConfiguration.value("HomeLongitude", "").toString().trimmed());
//    _txtLat.setText(appConfiguration.value("HomeLatitude", "").toString().trimmed());
//    appConfiguration.endGroup();
}

