#include "CamNameDialog.h"

CamNameDialog::CamNameDialog()
{
    initialize();
}

CamNameDialog::CamNameDialog(QWidget * parent)
{
    setParent(parent);
    initialize();
}

void CamNameDialog::initialize()
{
    setMaximumWidth(290);
    setMaximumHeight(160);

    setMinimumWidth(290);
    setMinimumHeight(160);

    lblCamName.setText("IP Cam Friendly Name");
    lblCamSDPort.setText("IP Cam SD Card Access Port");
    cmdOk.setText("OK");

    txtCamName.resize(180,24);
    txtCamSDPort.resize(180,24);

    dialogLayout.addWidget(&lblCamName,0,0);
    dialogLayout.addWidget(&txtCamName,1,0,1,3);

    dialogLayout.addWidget(&lblCamSDPort,3,0);
    dialogLayout.addWidget(&txtCamSDPort,4,0,1,3);

    dialogLayout.addWidget(&cmdOk,6,2);

    setLayout(&dialogLayout);
    setWindowIcon(QIcon(":/images/ipbum_small.png"));

    //
    connect(&cmdOk,SIGNAL(clicked()),this,SLOT(dialogClose()));
}

CamNameDialog::~CamNameDialog()
{
}

void CamNameDialog::dialogClose()
{
    this->close();
}

QString CamNameDialog::getCamFriendlyName()
{
    return txtCamName.text();
}

QString CamNameDialog::getCamSDCardPort()
{
    return txtCamSDPort.text();
}

void CamNameDialog::setCamFriendlyName(QString str)
{
    txtCamName.setText(str);
}
void CamNameDialog::setCamSDCardPort(QString str)
{
    txtCamSDPort.setText(str);
}
