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
#include <QSettings>
#include <QNetworkAccessManager>
#include "ThemeManager.h"
#include "Site.h"

class ConfigurationView : public QWidget
{
    Q_OBJECT
public:
    ConfigurationView(QWidget* parent = nullptr);
    void updateLicenseFile(QString str);
    void updateRestServiceStatus(bool flag, Site st);

private slots:
    void OnRestServiceConnect();
    void OnLicenseImport();
    void OnSiteSave();

private:
    void setupServiceAndSiteGroup();
    void setupLicenseFileGroup();

    QVBoxLayout     _Layout;

    // 1
    QGridLayout     _RestServiceConnectLayout;
    QLabel          _RestServiceConnectGroup;

    QLabel          _lblRestServiceUsername;
    QLabel          _lblRestServicePassword;
    QLabel          _lblHostname;
    QLabel          _lblPortNum;

    QLabel          _lblConnectionStatus;

    QLineEdit       _txtRestServiceUsername;
    QLineEdit       _txtRestServicePassword;
    QLineEdit       _txtRestServiceHostname;
    QLineEdit       _txtRestServicePortNum;

    QLabel          _txtConnectionStatus;
    QPushButton     _btnConnect;
    // 1

    // 2
    QLabel         _lblSiteId;
    QLineEdit      _txtSiteId;

    QLabel         _lblContactFirstName;
    QLineEdit      _txtContactFirstName;

    QLabel         _lblContactLastName;
    QLineEdit      _txtContactLastName;

    QLabel         _lblEmailId;
    QLineEdit      _txtEmailId;

    QLabel         _lblContactPhoneNo;
    QLineEdit      _txtContactPhoneNo;

    QLabel         _lblRecStart;
    QCalendarWidget _txtRecStart;

    QLabel         _lblRecEnd;
    QCalendarWidget _txtRecEnd;
    // 2

    QPushButton     _btnSave;

    // 4
    QHBoxLayout    _LicenseFileLayout;
    QLabel         _lblLicenseFile;
    QLabel         _txtLicenseFile;
    QPushButton    _btnLicenseFile;
    // 4
};

#endif
