#include "NetworkView.h"
#include "ThemeManager.h"
#include "CustomWidgets.h"
#include "Tracks.h"

NetworkView::NetworkView(QWidget *parent) : QWidget(parent)
{
    _lblIpAddress.setText("IP Address");
    _lblPort.setText("Port");
    _lblUser.setText("Username");
    _lblPass.setText("Password");
    _cmdProbe.setText("Probe Host");
    _cmdClearAll.setText("Clear All");
    _cmdScan.setText("Scan Network");

    lblManufacturer.setText("Manufacturer");
    lblModel.setText("Model");
    lblFirmwareVersion.setText("Firmware Version");
    lblSerialNumber.setText("Serial Number");
    lblHardwareId.setText("Hardware Id");
    lblCameraName.setText("Camera Name");
    lblHardware.setText("Hardware");
    lblUTCTime.setText("Date Time");

    _ControleBar.addWidget(&_lblIpAddress, 0, 0);
    _ControleBar.addWidget(&_txtIpAddress, 0, 1);
    _ControleBar.addWidget(&_lblPort, 0, 2);
    _ControleBar.addWidget(&_txtPort, 0, 3);

    _ControleBar.addWidget(&_lblUser, 0, 4);
    _ControleBar.addWidget(&_txtUser, 0, 5);
    _ControleBar.addWidget(&_lblPass, 0, 6);
    _ControleBar.addWidget(&_txtPass, 0, 7);

    _ControleBar.addWidget(&_cmdProbe, 0, 8);
    _ControleBar.addWidget(new VerticalLine(28), 0, 9);
    _ControleBar.addWidget(&_cmdScan, 0, 10);
    _ControleBar.addWidget(new VerticalLine(28), 0, 11);
    _ControleBar.addWidget(&_cmdClearAll, 0, 12);
    _ControleBar.setMargin(5);
    _ControleBar.setSpacing(5);

    _InfoArea.addWidget(&lblManufacturer, 0, 0);
    _InfoArea.addWidget(&txtManufacturer, 0, 1);
    _InfoArea.addWidget(&lblModel, 0, 2);
    _InfoArea.addWidget(&txtModel, 0, 3);
    _InfoArea.addWidget(&lblFirmwareVersion, 0, 4);
    _InfoArea.addWidget(&txtFirmwareVersion, 0, 5);
    _InfoArea.addWidget(&lblSerialNumber, 0, 6);
    _InfoArea.addWidget(&txtSerialNumber, 0, 7);

    _InfoArea.addWidget(&lblHardwareId, 1, 0);
    _InfoArea.addWidget(&txtHardwareId, 1, 1);
    _InfoArea.addWidget(&lblCameraName, 1, 2);
    _InfoArea.addWidget(&txtCameraName, 1, 3);
    _InfoArea.addWidget(&lblHardware, 1, 4);
    _InfoArea.addWidget(&txtHardware, 1, 5);
    _InfoArea.addWidget(&lblUTCTime, 1, 6);
    _InfoArea.addWidget(&txtUTCTime, 1, 7);

    _InfoArea.setMargin(5);
    _InfoArea.setSpacing(5);

    //TEMP --
    _txtIpAddress.setText("122.160.173.121");
    _txtPort.setText("8081");
    _txtUser.setText("admin");
    _txtPass.setText("admin");
    //TEMP --

    _Layout.addWidget(new HorizontalLine());
    _Layout.addLayout(&_ControleBar);
    _Layout.addWidget(new HorizontalLine());
    _Layout.addWidget(&_CameraList, Qt::AlignLeft | Qt::AlignTop);
    _Layout.addWidget(new HorizontalLine());
    _Layout.addLayout(&_InfoArea);
    _Layout.addWidget(new HorizontalLine());
    _Layout.addWidget(&_ServiceList, Qt::AlignLeft | Qt::AlignTop);

    _CameraList.setFrameStyle(QFrame::NoFrame);
    _CameraList.setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);

    _ServiceList.setFrameStyle(QFrame::NoFrame);
    _ServiceList.setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);

    _Layout.addStretch();

    connect(&_cmdScan, &QPushButton::clicked, this, &NetworkView::startNetworkScan);
    connect(&_cmdProbe, &QPushButton::clicked, this, &NetworkView::startHostProbe);
    connect(&_CameraList, &QListWidget::itemClicked, this, &NetworkView::cameraSelected);

    _Layout.setMargin(0);
    _Layout.setSpacing(0);

    setLayout(&_Layout);

    setMinimumWidth(500);
}

NetworkView::~NetworkView()
{
}

void NetworkView::startNetworkScan()
{
//    ServiceInterface* networkScanner = new ServiceInterface();
//    connect(networkScanner, &ServiceInterface::cameraFound, this, &NetworkView::eventCameraFound);
//    connect(networkScanner, &ServiceInterface::serviceDetectionFailed, this, &NetworkView::eventServiceDetectionFailed);
//    networkScanner->setTemplateDirectory(ipbumPtr->templateDirectory());
//    networkScanner->probeNetwork();
}

void NetworkView::startHostProbe()
{
    _CurrentProbe = new ServiceInterface();

    connect(_CurrentProbe, &ServiceInterface::cameraFound, this, &NetworkView::eventCameraFound);
    connect(_CurrentProbe, &ServiceInterface::probeFailed, this, &NetworkView::eventProbeFailed);
    connect(_CurrentProbe, &ServiceInterface::serviceDetectionFailed, this, &NetworkView::eventServiceDetectionFailed);
    connect(_CurrentProbe, &ServiceInterface::deviceInformationReceived, this, &NetworkView::eventDeviceInformationReceived);
    connect(_CurrentProbe, &ServiceInterface::servicesReceived, this, &NetworkView::eventServicesReceived);
    connect(_CurrentProbe, &ServiceInterface::dateTimeReceived, this, &NetworkView::eventDateTimeReceived);
    connect(_CurrentProbe, &ServiceInterface::capabilitiesReceived, this, &NetworkView::eventCapabilitiesReceived);
    connect(_CurrentProbe, &ServiceInterface::serviceDetectionFailed, this, &NetworkView::eventServiceDetectionFailed);
    connect(this, &NetworkView::probeTimedOut, this, &NetworkView::eventProbeTimedOut);

    _CurrentProbe->IpAddress = _txtIpAddress.text();
    _CurrentProbe->ServicePort = QVariant(_txtPort.text()).toUInt();
    _CurrentProbe->ServiceUser = _txtUser.text();
    _CurrentProbe->Password = _txtPass.text();
    _CurrentProbe->setTemplateDirectory(tracksPtr->templateDirectory());

    _CurrentProbe->probeHost(_CurrentProbe->IpAddress, _CurrentProbe->ServicePort);
    QTimer::singleShot(1000, this, SIGNAL(probeTimedOut()));
}

void NetworkView::eventProbeFailed(ServiceInterface* cam)
{
    cam->setPath(cam->DefaultServiceUri);
    cam->getDeviceInformation();
}

void NetworkView::eventProbeTimedOut()
{
    eventProbeFailed(_CurrentProbe);
}

void NetworkView::eventCameraFound(ServiceInterface* cam)
{
    cam->getDeviceInformation();
}

void NetworkView::eventDeviceInformationReceived(ServiceInterface *cam)
{
    cam->getServices();
}

void NetworkView::eventServicesReceived(ServiceInterface* cam)
{
    cam->getSystemDateAndTime();
}

void NetworkView::eventDateTimeReceived(ServiceInterface *cam)
{
    if(tracksPtr->addCamera(cam))
    {
        QString desc = "ONVIF service at " + cam->DefaultServiceUri;
        QListWidgetItem *camItem = new QListWidgetItem(&_CameraList);
        camItem->setData(Qt::DisplayRole, QVariant(cam->DeviceName));
        camItem->setData(Qt::UserRole, QVariant(desc));
        camItem->setData(Qt::UserRole+1, QVariant(cam->DeviceName));

        camItem->setSelected(true);

        displayCameraInfo(cam);
    }
    // _CurrentCameraProbe->getCapabilities();
}

void NetworkView::eventCapabilitiesReceived(ServiceInterface* cam)
{

}

void NetworkView::eventServiceDetectionFailed(ServiceInterface *cam)
{
}

void NetworkView::cameraSelected(QListWidgetItem *item)
{
    _ServiceList.clear();
    txtManufacturer.setText("");
    txtModel.setText("");
    txtFirmwareVersion.setText("");
    txtSerialNumber.setText("");
    txtHardwareId.setText("");
    txtCameraName.setText("");
    txtHardware.setText("");
    txtUTCTime.setText("");

    QString camname = item->data(Qt::UserRole+1).toString();

    ServiceInterface* mgr = tracksPtr->getServiceInterface(camname);

    if(mgr != nullptr)
    {
        displayCameraInfo(mgr);
    }
}

void NetworkView::displayCameraInfo(ServiceInterface* cam)
{
    if(cam == nullptr)
    {
        return;
    }

    txtManufacturer.setText(cam->Manufacturer);
    txtModel.setText(cam->Model);
    txtFirmwareVersion.setText(cam->FirmwareVersion);
    txtSerialNumber.setText(cam->SerialNumber);
    txtHardwareId.setText(cam->HardwareId);
    txtCameraName.setText(cam->CameraName);
    txtHardware.setText(cam->Hardware);
    txtUTCTime.setText(cam->UTCTimestamp);

    for(int idx = 0; idx < cam->ServiceList.count(); idx++)
    {
        ONVIFService serv = cam->ServiceList.at(idx);
        QListWidgetItem *servItem = new QListWidgetItem(&_ServiceList);
        servItem->setData(Qt::DisplayRole, QVariant(serv.XAddress));
        servItem->setData(Qt::UserRole, QVariant(serv.NameSpace));
    }
}


