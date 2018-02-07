#include "NetworkView.h"
#include "ThemeManager.h"
#include "CustomWidgets.h"
#include "Tracks.h"
#include "MainWindow.h"

NetworkView::NetworkView(QWidget *parent) : QWidget(parent)
{
    _lblIpAddress.setText("IP Address");
    _lblPort.setText("Port");
    _lblUser.setText("Username");
    _lblPass.setText("Password");
    _cmdProbe.setText("Probe Host");
    _cmdScan.setText("Scan Network");
    _txtPass.setEchoMode(QLineEdit::Password);

    lblManufacturer.setText("Manufacturer");
    lblModel.setText("Model");
    lblFirmwareVersion.setText("Firmware Version");
    lblSerialNumber.setText("Serial Number");
    lblHardwareId.setText("Hardware Id");
    lblCameraName.setText("Device Name");
    lblHardware.setText("Hardware");
    lblUTCTime.setText("Date Time");

    _lblCamFriendlyName.setText("Friendly Name");
    _cmdSaveName.setText("Save");
    _cmdRemove.setText("Remove");
    _cmdRemoveAll.setText("Remove All");

    _ControleBar.addWidget(&_lblIpAddress);
    _ControleBar.addWidget(&_txtIpAddress);
    _ControleBar.addWidget(&_lblPort);
    _ControleBar.addWidget(&_txtPort);

    _ControleBar.addWidget(&_lblUser);
    _ControleBar.addWidget(&_txtUser);
    _ControleBar.addWidget(&_lblPass);
    _ControleBar.addWidget(&_txtPass);

    _ControleBar.addWidget(&_cmdProbe);
    //_ControleBar.addWidget(new VerticalLine(28));
    //_ControleBar.addWidget(&_cmdScan);
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
    _txtIpAddress.setText("192.168.1.170");
    _txtPort.setText("8000");
    _txtUser.setText("admin");
    _txtPass.setText("admin");
    //TEMP --

    _ListOptLayout.addWidget(&_lblCamFriendlyName);
    _ListOptLayout.addWidget(&_txtCamFriendlyName);
    _ListOptLayout.addWidget(&_cmdSaveName);
    _ListOptLayout.addStretch();
    _ListOptLayout.addWidget(&_cmdRemove);
    _ListOptLayout.addWidget(&_cmdRemoveAll);
    _ListOptLayout.setMargin(5);
    _ListOptLayout.setSpacing(5);

    _Layout.addWidget(new HorizontalLine());
    _Layout.addLayout(&_ControleBar);
    _Layout.addWidget(new HorizontalLine());
    _Layout.addWidget(&_CameraList, Qt::AlignLeft | Qt::AlignTop);
    _Layout.addWidget(new HorizontalLine());
    _Layout.addLayout(&_ListOptLayout);
    _Layout.addWidget(new HorizontalLine());
    _Layout.addLayout(&_InfoArea);
    _Layout.addWidget(new HorizontalLine());
    _Layout.addWidget(&_ServiceList, Qt::AlignLeft | Qt::AlignTop);
    _Layout.addWidget(new HorizontalLine());

    _CameraList.setFrameStyle(QFrame::NoFrame);
    _CameraList.setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);

    _ServiceList.setFrameStyle(QFrame::NoFrame);
    _ServiceList.setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);

    _Layout.addStretch();

    connect(&_cmdScan, &QPushButton::clicked, this, &NetworkView::startNetworkScan);
    connect(&_cmdProbe, &QPushButton::clicked, this, &NetworkView::startHostProbe);
    connect(&_CameraList, &QListWidget::itemClicked, this, &NetworkView::cameraSelected);

    connect(&_cmdSaveName, &QPushButton::clicked, this, &NetworkView::eventSaveName);
    connect(&_cmdRemove, &QPushButton::clicked, this, &NetworkView::eventRemove);
    connect(&_cmdRemoveAll, &QPushButton::clicked, this, &NetworkView::eventRemoveAll);

    _Layout.setMargin(0);
    _Layout.setSpacing(0);

    setLayout(&_Layout);

    setMinimumWidth(500);
}

NetworkView::~NetworkView()
{
}

void NetworkView::loadCameraList()
{
    QList<ONVIFCamera> clist = tracksPtr->cameraList();

    foreach(ONVIFCamera cam, clist)
    {
        QString desc = "ONVIF service at " + cam.DefaultServiceUri;
        QListWidgetItem *camItem = new QListWidgetItem(&_CameraList);
        camItem->setData(Qt::DisplayRole, QVariant(cam.FriendlyName));
        camItem->setData(Qt::UserRole, QVariant(desc));
        camItem->setData(Qt::UserRole+1, QVariant(cam.FriendlyName));

        camItem->setSelected(true);

        displayCameraInfo(&cam);
    }
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
    mainWindowPtr->setStatusMessage("Searching camera ...");
    mainWindowPtr->setProgressMessage("Starting probe");

    _CurrentProbe = new ServiceInterface();

    connect(_CurrentProbe, &ServiceInterface::cameraFound, this, &NetworkView::eventCameraFound);
    connect(_CurrentProbe, &ServiceInterface::probeFailed, this, &NetworkView::eventProbeFailed);
    connect(_CurrentProbe, &ServiceInterface::deviceInformationReceived, this, &NetworkView::eventDeviceInformationReceived);
    connect(_CurrentProbe, &ServiceInterface::servicesReceived, this, &NetworkView::eventServicesReceived);
    connect(_CurrentProbe, &ServiceInterface::dateTimeReceived, this, &NetworkView::eventDateTimeReceived);
    connect(_CurrentProbe, &ServiceInterface::capabilitiesReceived, this, &NetworkView::eventCapabilitiesReceived);
    connect(_CurrentProbe, &ServiceInterface::serviceDetectionFailed, this, &NetworkView::eventServiceDetectionFailed);
    connect(this, &NetworkView::probeTimedOut, this, &NetworkView::eventProbeTimedOut);

    _CurrentProbe->Camera.IpAddress = _txtIpAddress.text();
    _CurrentProbe->Camera.ServicePort = QVariant(_txtPort.text()).toUInt();
    _CurrentProbe->Camera.ServiceUser = _txtUser.text();
    _CurrentProbe->Camera.Password = _txtPass.text();
    _CurrentProbe->setTemplateDirectory(tracksPtr->templateDirectory());


    // UDP Probe disabled
    //_CurrentProbe->probeHost(_CurrentProbe->Camera.IpAddress, _CurrentProbe->Camera.ServicePort);
    //QTimer::singleShot(5000, this, SIGNAL(probeTimedOut()));
    // UDP Probe disabled

    // Direct connection
    _CurrentProbe->getDeviceInformation();
    // Direct connection

    _cmdProbe.setEnabled(false);
    _cmdScan.setEnabled(false);
    _cmdSaveName.setEnabled(false);
    _cmdRemove.setEnabled(false);
    _cmdRemoveAll.setEnabled(false);
}

void NetworkView::eventProbeFailed(ServiceInterface* cam)
{
    mainWindowPtr->setProgressMessage("WS Discovery off ... trying getDeviceInformation");
    cam->setPath(cam->Camera.DefaultServiceUri);
    cam->getDeviceInformation();
}

void NetworkView::eventProbeTimedOut()
{
    eventProbeFailed(_CurrentProbe);
}

void NetworkView::eventCameraFound(ServiceInterface* cam)
{
    mainWindowPtr->setProgressMessage("WS Discovery success ... trying getDeviceInformation");
    cam->getDeviceInformation();
}

void NetworkView::eventDeviceInformationReceived(ServiceInterface *cam)
{
    mainWindowPtr->setProgressMessage("Trying getServices");
    cam->getServices();
}

void NetworkView::eventServicesReceived(ServiceInterface* cam)
{
    mainWindowPtr->setProgressMessage("Trying getCapabilities");
    cam->getCapabilities();
}

void NetworkView::eventCapabilitiesReceived(ServiceInterface* cam)
{
    mainWindowPtr->setProgressMessage("Trying getSystemDateTime");
    cam->getSystemDateAndTime();
}

void NetworkView::eventDateTimeReceived(ServiceInterface *cam)
{
    mainWindowPtr->setProgressMessage("Probe complete");

    if(tracksPtr->addCamera(cam->Camera))
    {
        QString desc = "ONVIF service at " + cam->Camera.DefaultServiceUri;
        QListWidgetItem *camItem = new QListWidgetItem(&_CameraList);
        camItem->setData(Qt::DisplayRole, QVariant(cam->Camera.FriendlyName));
        camItem->setData(Qt::UserRole, QVariant(desc));
        camItem->setData(Qt::UserRole+1, QVariant(cam->Camera.FriendlyName));

        camItem->setSelected(true);

        displayCameraInfo(&cam->Camera);
    }

    cleanUp();

    mainWindowPtr->clearStatusBar();
}

void NetworkView::eventServiceDetectionFailed(ServiceInterface *cam)
{
    mainWindowPtr->clearStatusBar();

    QString msginfo = "No camera found at\nhttp://";
    msginfo += cam->Camera.IpAddress + ":";
    msginfo += QVariant(cam->Camera.ServicePort).toString() + "/";

    QMessageBox::information(nullptr, tracksPtr->applicationName(), msginfo, QMessageBox::Ok);

    cleanUp();
}

void NetworkView::cameraSelected(QListWidgetItem *item)
{
    clearCameraInfo();

    QString camname = item->data(Qt::UserRole+1).toString();

    ONVIFCamera cam = tracksPtr->getCamera(camname);

    displayCameraInfo(&cam);
}

void NetworkView::displayCameraInfo(ONVIFCamera *cam)
{
    if(cam == nullptr)
    {
        return;
    }

    _ServiceList.clear();
    txtManufacturer.setText(cam->Manufacturer);
    txtModel.setText(cam->Model);
    txtFirmwareVersion.setText(cam->FirmwareVersion);
    txtSerialNumber.setText(cam->SerialNumber);
    txtHardwareId.setText(cam->HardwareId);
    txtCameraName.setText(cam->DeviceName);
    txtHardware.setText(cam->Hardware);
    txtUTCTime.setText(cam->UTCTimestamp);
    _txtCamFriendlyName.setText(cam->FriendlyName);

    for(int idx = 0; idx < cam->ServiceList.count(); idx++)
    {
        ONVIFService serv = cam->ServiceList.at(idx);
        QListWidgetItem *servItem = new QListWidgetItem(&_ServiceList);
        servItem->setData(Qt::DisplayRole, QVariant(serv.XAddress));
        servItem->setData(Qt::UserRole, QVariant(serv.NameSpace));
    }
}

void NetworkView::eventSaveName()
{
    if(_CameraList.selectedItems().count() > 0)
    {
        QListWidgetItem* camitem = _CameraList.selectedItems().at(0);
        QString currentname =   camitem->data(Qt::UserRole+1).toString();

        ONVIFCamera cam = tracksPtr->getCamera(currentname);

        cam.FriendlyName = _txtCamFriendlyName.text();

        camitem->setData(Qt::DisplayRole, QVariant(cam.FriendlyName));
        camitem->setData(Qt::UserRole+1, QVariant(cam.FriendlyName));
        _CameraList.repaint();
        camitem->setSelected(true);

        tracksPtr->removeCamera(currentname);

        tracksPtr->addCamera(cam);
    }
}

void NetworkView::eventRemove()
{
    clearCameraInfo();

    QList<QListWidgetItem*> items = _CameraList.selectedItems();

    foreach(QListWidgetItem * item, items)
    {
        QString camname = item->data(Qt::UserRole+1).toString();

        tracksPtr->removeCamera(camname);

        delete _CameraList.takeItem(_CameraList.row(item));
    }
}

void NetworkView::eventRemoveAll()
{
    clearCameraInfo();

     _CameraList.clear();

     tracksPtr->removeAllCameras();
}

void NetworkView::clearCameraInfo()
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
}

void NetworkView::cleanUp()
{
    disconnect(_CurrentProbe, &ServiceInterface::cameraFound, this, &NetworkView::eventCameraFound);
    disconnect(_CurrentProbe, &ServiceInterface::probeFailed, this, &NetworkView::eventProbeFailed);
    disconnect(_CurrentProbe, &ServiceInterface::deviceInformationReceived, this, &NetworkView::eventDeviceInformationReceived);
    disconnect(_CurrentProbe, &ServiceInterface::servicesReceived, this, &NetworkView::eventServicesReceived);
    disconnect(_CurrentProbe, &ServiceInterface::dateTimeReceived, this, &NetworkView::eventDateTimeReceived);
    disconnect(_CurrentProbe, &ServiceInterface::capabilitiesReceived, this, &NetworkView::eventCapabilitiesReceived);
    disconnect(_CurrentProbe, &ServiceInterface::serviceDetectionFailed, this, &NetworkView::eventServiceDetectionFailed);
    _CurrentProbe->deleteLater();

    _cmdProbe.setEnabled(true);
    _cmdScan.setEnabled(true);
    _cmdSaveName.setEnabled(true);
    _cmdRemove.setEnabled(true);
    _cmdRemoveAll.setEnabled(true);
}

