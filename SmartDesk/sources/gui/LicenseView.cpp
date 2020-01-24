#include "LicenseView.h"

LicenseView::LicenseView(QWidget *parent)
    : QWidget(parent)
{
    _lblMac.setText("MAC Address of the licensed host");
    _lblDate.setText("License expiry date");
    _cmdGenLic.setText("Generate License File");
    _cmdGenMasterLic.setText("Generate Master License File");

    _dtDate.setCalendarPopup(true);
    QDateTime dt = QDateTime::currentDateTime();
    dt = dt.addYears(1);
    _dtDate.setDateTime(dt);

//   _AppCentralWidget.addWidget(&_lblMac);
//   _AppCentralWidget.addWidget(&_mcAddr);
//   _AppCentralWidget.addWidget(&_lblDate);
//   _AppCentralWidget.addWidget(&_dtDate);
//   _AppCentralWidget.addWidget(&_cmdGenLic);
//   _AppCentralWidget.addWidget(&_cmdGenMasterLic);
//    _Layout.addWidget(&_AppCentralWidget);

    _ed1.setMaximumWidth(20);
    _ed2.setMaximumWidth(20);
    _ed3.setMaximumWidth(20);
    _ed4.setMaximumWidth(20);
    _ed5.setMaximumWidth(20);
    _ed6.setMaximumWidth(20);

    _ed1.setInputMask("NN");
    _ed2.setInputMask("NN");
    _ed3.setInputMask("NN");
    _ed4.setInputMask("NN");
    _ed5.setInputMask("NN");
    _ed6.setInputMask("NN");

    _ed1.setText("00");
    _ed2.setText("00");
    _ed3.setText("00");
    _ed4.setText("00");
    _ed5.setText("00");
    _ed6.setText("00");

    _MacAddressLayout.addWidget(&_ed1);
    _MacAddressLayout.addWidget(&_ed2);
    _MacAddressLayout.addWidget(&_ed3);
    _MacAddressLayout.addWidget(&_ed4);
    _MacAddressLayout.addWidget(&_ed5);
    _MacAddressLayout.addWidget(&_ed6);
    _MacAddressLayout.addStretch();

    _MacAddressLayout.setMargin(0);
    _MacAddressLayout.setSpacing(5);


    _Layout.addWidget(&_lblMac);
    _Layout.addLayout(&_MacAddressLayout);
    _Layout.addWidget(&_lblDate);
    _Layout.addWidget(&_dtDate);
    _Layout.addWidget(&_cmdGenLic);
    _Layout.addWidget(&_cmdGenMasterLic);

    setLayout(&_Layout);

    connect(&_cmdGenLic, &QPushButton::clicked, this, &LicenseView::generateLicense);
    connect(&_cmdGenMasterLic, &QPushButton::clicked, this, &LicenseView::generateMasterLicense);
}

LicenseView::~LicenseView()
{

}

void LicenseView::generateLicense()
{
    QString mstr = getMacString();
    QString dstr = _dtDate.date().toString("yyyy/MM/dd");

    if(mstr.length() < 17)
    {
        QMessageBox::critical(this, "Error", "Invalid MAC Address", QMessageBox::Ok);
        return;
    }

    if(_dtDate.date() < QDate::currentDate())
    {
        QMessageBox::critical(this, "Error", "Select a date in future", QMessageBox::Ok);
        return;
    }

    generateLicenseString(mstr, dstr);
}

void LicenseView::generateMasterLicense()
{
    generateLicenseString("ff:ff:ff:ff:ff:ff", "9999/12/31");
}

void LicenseView::generateLicenseString(QString macstring, QString datestring)
{
    QStringList lst = datestring.split('/');

    QString str = lst.at(0) + "-" + macstring + "-" + lst.at(1) + lst.at(2);

    char arr[28] = {0};

    for(int ctr = str.length()-1, idx = 0; ctr > -1; ctr--, idx++)
    {
        arr[idx] = str.at(ctr).toLatin1();
    }

    QByteArray ba(arr, strlen(arr));

    QString encoded = ba.toBase64();

    QByteArray buffer = encoded.toUtf8();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As..."), QDir::homePath(), "License file (*.lic)");

    if (!fileName.isEmpty())
    {
        QFile fl(fileName);

        if(fl.open(QIODevice::WriteOnly|QIODevice::Truncate))
        {
            fl.write(buffer);
            fl.flush();
            fl.close();
        }
        else
        {
            QMessageBox::critical(this, "Error", "Could not write file\nPlease select another location", QMessageBox::Ok);
        }
    }
}

QString LicenseView::getMacString()
{
   return QString(_ed1.text() + ":" + _ed2.text() + ":" + _ed3.text() + ":" + _ed4.text() + ":" + _ed5.text() + ":" + _ed6.text());
}

