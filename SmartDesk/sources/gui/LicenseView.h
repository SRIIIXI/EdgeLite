#ifndef _LICENSE_VIEW
#define _LICENSE_VIEW

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QDateEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>
#include "CustomWidgets.h"

class LicenseView : public QWidget
{
    Q_OBJECT

public:
    LicenseView(QWidget *parent = 0);
    ~LicenseView();

private slots:
    void generateLicense();
    void generateMasterLicense();

    void generateLicenseString(QString macstring, QString datestring);
    QString getMacString();

private:
    CentralWidget _AppCentralWidget;

    QVBoxLayout _Layout;

    QLabel _lblMac;
    QLabel _lblDate;
    QDateEdit _dtDate;

    QPushButton _cmdGenLic;
    QPushButton _cmdGenMasterLic;

    QHBoxLayout _MacAddressLayout;
    QLineEdit _ed1, _ed2, _ed3, _ed4, _ed5, _ed6;
};

#endif // MAINWINDOW_H
