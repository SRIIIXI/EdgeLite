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
#include <QSql>
#include <QSqlDatabase>
#include <QSettings>
#include "ThemeManager.h"

class ConfigurationView : public QWidget
{
    Q_OBJECT
public:
    ConfigurationView(QWidget* parent = nullptr);
private slots:
    void OnDBConnect();
    void OnDownloadDirBrowse();
    void OnThemeChange();

private:
    void setupDataConnectionGroup();
    void setupDownloadDirGroup();
    void setupThemeSelectionGroup();

    QVBoxLayout     _Layout;

    // 1
    QGridLayout     _DataConnectLayout;
    QLabel          _DataConnectGroup;
    QLabel          _lblDataSourceName;
    QLabel          _lblDatabaseUsername;
    QLabel          _lblDatabasePassword;
    QLabel          _lblHostname;
    QLabel          _lblPortNum;
    QLabel          _lblConnectionStatus;

    QLineEdit       _txtDataSourceName;
    QLineEdit       _txtDatabaseUsername;
    QLineEdit       _txtDatabasePassword;
    QLineEdit       _txtHostname;
    QLineEdit       _txtPortNum;
    QLabel          _txtConnectionStatus;
    QPushButton     _btnConnect;
    // 1

    // 2
    QHBoxLayout    _DownloadDirLayout;
    QLabel         _lblDownloadDir;
    QLabel         _txtDownloadDir;
    QPushButton    _btnDownloadDir;
    // 2

    // 3
    QHBoxLayout    _ThemeLayout;
    QLabel         _lblTheme;
    QRadioButton   _btnLight;
    QRadioButton   _btnDark;
    QButtonGroup   _RadGroup;
    // 3
};

#endif
