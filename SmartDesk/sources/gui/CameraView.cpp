#include "CameraView.h"
#include "ThemeManager.h"
#include "CustomWidgets.h"
#include "IPCamDesktop.h"
#include "MainWindow.h"

CameraView::CameraView(QWidget *parent) : QWidget(parent)
{
    _lblIpAddress.setText("IP Address");
    _lblONVIFPort.setText("ONVIF Port");
    _lblUser.setText("Username");
    _lblPass.setText("Password");
    _cmdProbe.setText("Probe Device");
    _txtPass.setEchoMode(QLineEdit::Password);

    _lblIsActive.setText("Is Active");
    _lblLastAccessed.setText("Last Accessed");
    _lblDirTimestamp.setText("Directory Last Accessed");
    _lblFileTimestamp.setText("Recording Last Accessed");
    _lblRequireDownload.setText("Download Required");

    _lblCamName.setText("Camera Name");
    _lblSDCardPort.setText("SD Card Port");
    _cmdSaveCamera.setText("Save Camera");
    _cmdRemove.setText("Remove");
    _cmdRemoveAll.setText("Remove All");

    _lblSlotStart1.setText("Slot 1 Start");
    _lblSlotStart2.setText("Slot 2 Start");
    _lblSlotStart3.setText("Slot 3 Start");
    _lblSlotStart4.setText("Slot 4 Start");
    _lblSlotEnd1.setText("Slot 1 End");
    _lblSlotEnd2.setText("Slot 2 End");
    _lblSlotEnd3.setText("Slot 3 End");
    _lblSlotEnd4.setText("Slot 4 End");

    _txtSDCardPort.setMaximumWidth(45);

    _ControleBar.addWidget(&_lblIpAddress);
    _ControleBar.addWidget(&_txtIpAddress);
    _ControleBar.addWidget(&_lblONVIFPort);
    _ControleBar.addWidget(&_txtONVIFPort);

    _ControleBar.addWidget(&_lblUser);
    _ControleBar.addWidget(&_txtUser);
    _ControleBar.addWidget(&_lblPass);
    _ControleBar.addWidget(&_txtPass);

    _ControleBar.addWidget(&_cmdProbe);
    _ControleBar.setMargin(5);
    _ControleBar.setSpacing(5);

    _InfoArea.addWidget(&_lblIsActive, 0, 0);
    _InfoArea.addWidget(&_txtIsActive, 0, 1);
    _InfoArea.addWidget(&_lblLastAccessed, 0, 2);
    _InfoArea.addWidget(&_txtLastAccessed, 0, 3);
    _InfoArea.addWidget(&_lblDirTimestamp, 0, 4);
    _InfoArea.addWidget(&_txtDirTimestamp, 0, 5);
    _InfoArea.addWidget(&_lblFileTimestamp, 0, 6);
    _InfoArea.addWidget(&_txtFileTimestamp, 0, 7);

    _InfoArea.addWidget(&_lblRequireDownload, 1, 0);
    _InfoArea.addWidget(&_ChkRequiredDownload, 1, 1);

    _TimeSlotLayout.addWidget(&_lblSlotStart1,3,0);
    _TimeSlotLayout.addWidget(&_lblSlotEnd1,3,2);
    _TimeSlotLayout.addWidget(&_lblSlotStart2,4,0);
    _TimeSlotLayout.addWidget(&_lblSlotEnd2,4,2);
    _TimeSlotLayout.addWidget(&_lblSlotStart3,5,0);
    _TimeSlotLayout.addWidget(&_lblSlotEnd3,5,2);
    _TimeSlotLayout.addWidget(&_lblSlotStart4,6,0);
    _TimeSlotLayout.addWidget(&_lblSlotEnd4,6,2);

    _TimeSlotLayout.addWidget(&_EdSlotStart1,3,1);
    _TimeSlotLayout.addWidget(&_EdSlotEnd1,3,3);
    _TimeSlotLayout.addWidget(&_EdSlotStart2,4,1);
    _TimeSlotLayout.addWidget(&_EdSlotEnd2,4,3);
    _TimeSlotLayout.addWidget(&_EdSlotStart3,5,1);
    _TimeSlotLayout.addWidget(&_EdSlotEnd3,5,3);
    _TimeSlotLayout.addWidget(&_EdSlotStart4,6,1);
    _TimeSlotLayout.addWidget(&_EdSlotEnd4,6,3);

    _InfoArea.setMargin(5);
    _InfoArea.setSpacing(5);

    //DEFAULTS --
    //_txtIpAddress.setText("127.0.0.1");
    //_txtPort.setText("10000");
    _txtIpAddress.setText("122.160.173.121");
    _txtONVIFPort.setText("8083");
    _txtUser.setText("admin");
    _txtPass.setText("admin");
    //DEFAULTS --

    _ListOptLayout.addWidget(&_lblCamName);
    _ListOptLayout.addWidget(&_txtCamName);
    _ListOptLayout.addWidget(&_lblSDCardPort);
    _ListOptLayout.addWidget(&_txtSDCardPort);
    _ListOptLayout.addWidget(&_cmdSaveCamera);
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
    _Layout.addLayout(&_TimeSlotLayout);
    _Layout.addWidget(new HorizontalLine());

    _CameraList.setFrameStyle(QFrame::NoFrame);
    _CameraList.setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);

    _Layout.addStretch();

    connect(&_cmdProbe, &QPushButton::clicked, this, &CameraView::eventStartdeviceProbe);
    connect(&_CameraList, &QListWidget::itemClicked, this, &CameraView::cameraSelected);

    connect(&_cmdSaveCamera, &QPushButton::clicked, this, &CameraView::eventSaveChanges);
    connect(&_cmdRemove, &QPushButton::clicked, this, &CameraView::eventRemove);
    connect(&_cmdRemoveAll, &QPushButton::clicked, this, &CameraView::eventRemoveAll);

    _Layout.setMargin(0);
    _Layout.setSpacing(0);

    setLayout(&_Layout);

    setMinimumWidth(500);
}

CameraView::~CameraView()
{
}

void CameraView::eventStartdeviceProbe()
{
    if(QVariant(_txtONVIFPort.text()).toInt() < 0 || QVariant(_txtONVIFPort.text()).toInt() > 65535)
    {
        QMessageBox msgBox;
        msgBox.setText("Please provide a port number between 0 and 65535");
        msgBox.exec();
    }

    QHostAddress address(_txtIpAddress.text());

    if (QAbstractSocket::IPv4Protocol != address.protocol())
    {
        if (QAbstractSocket::IPv6Protocol != address.protocol())
        {
            QMessageBox msgBox;
            msgBox.setText("Please provide a valid IP address");
            msgBox.exec();
        }
    }

    mainWindowPtr->setStatusMessage("Searching camera ...");
    mainWindowPtr->setProgressMessage("Starting probe");

    /*
    _CurrentProbe = new ServiceInterface();

    connect(_CurrentProbe, &ServiceInterface::cameraFound, this, &NetworkView::eventCameraFound);
    connect(_CurrentProbe, &ServiceInterface::probeFailed, this, &NetworkView::eventProbeFailed);
    connect(_CurrentProbe, &ServiceInterface::deviceInformationReceived, this, &NetworkView::eventDeviceInformationReceived);
    connect(_CurrentProbe, &ServiceInterface::serviceDetectionFailed, this, &NetworkView::eventServiceDetectionFailed);
    connect(this, &NetworkView::probeTimedOut, this, &NetworkView::eventProbeTimedOut);

    _CurrentProbe->Camera.IpAddress = _txtIpAddress.text();
    _CurrentProbe->Camera.ServicePort = _txtPort.text();
    _CurrentProbe->Camera.AlternatePort = _txtPort.text();
    _CurrentProbe->Camera.ServiceUser = _txtUser.text();
    _CurrentProbe->Camera.Password = _txtPass.text();
    _CurrentProbe->setTemplateDirectory(applicationPtr->templateDirectory());

    // UDP Probe disabled
    //_CurrentProbe->probeHost(_CurrentProbe->Camera.IpAddress, _CurrentProbe->Camera.ServicePort);
    //QTimer::singleShot(5000, this, SIGNAL(probeTimedOut()));
    // UDP Probe disabled

    // Direct connection
    _CurrentProbe->getDeviceInformation();
    // Direct connection
    */

    _cmdProbe.setEnabled(false);
    _cmdSaveCamera.setEnabled(false);
    _cmdRemove.setEnabled(false);
    _cmdRemoveAll.setEnabled(false);
}

void CameraView::eventCameraNotFound(Camera &cam)
{
    mainWindowPtr->clearStatusBar();

    QString msginfo = "No camera found at\nhttp://";
    msginfo += cam.ip_address + ":";
    msginfo += QVariant(cam.onvif_port).toString() + "/";

    QMessageBox::information(nullptr, applicationPtr->applicationName(), msginfo, QMessageBox::Ok);

    cleanUp();
}


void CameraView::eventCameraFound(Camera &cam)
{
    mainWindowPtr->setProgressMessage("Discovery success");
}

void CameraView::cameraSelected(QListWidgetItem *item)
{
    clearCameraInfo();

    QString camid = item->data(Qt::UserRole+1).toString();

    Camera cam = applicationPtr->getCameraById(camid);

    displayCameraInfo(cam);
}

void CameraView::displayCameraInfo(Camera &cam)
{
    //_ServiceListView.clear();
    //_txtManufacturer.setText(cam.Manufacturer);
    //_txtModel.setText(cam.Model);
    //_txtFirmwareVersion.setText(cam.FirmwareVersion);
    //_txtSerialNumber.setText(cam.SerialNumber);
    //_txtHardwareId.setText(cam.HardwareId);
    //_txtCameraName.setText(cam.DeviceName);
    //_txtHardware.setText(cam.Hardware);
    //_txLastAccessTime.setText(cam.LastAccessed);
    //_txtCamFriendlyName.setText(cam.CameraName);
    //_txtAlternatePort.setText(cam.AlternatePort);

    //_txtIpAddress.setText(cam.IpAddress);
    //_txtPort.setText(cam.ServicePort);
    //_txtUser.setText(cam.ServiceUser);
    //_txtPass.setText(cam.Password);
}

void CameraView::eventSaveChanges()
{
    if(_CameraList.selectedItems().count() > 0)
    {
        if(applicationPtr->hasCameraByName(_txtCamName.text()))
        {
            QMessageBox msgBox;
            msgBox.setText("The requested camera name already exists");
            msgBox.exec();
            return;
        }

        QListWidgetItem* camitem = _CameraList.selectedItems().at(0);
        QString currentcamid =   camitem->data(Qt::UserRole+1).toString();

        Camera cam = applicationPtr->getCameraById(currentcamid);

        cam.camera_name = _txtCamName.text();
        cam.sd_port = _txtSDCardPort.text();


        //    if(_ChkDownloadRecordings.checkState() == Qt::CheckState::Checked)
        //    {
        //        st.DownloadRequired = "Y";
        //    }
        //    else
        //    {
        //        st.DownloadRequired = "N";
        //    }

        //    st.SiteId = _txtSiteId.text();

        //    st.DownloadSchedule = "";
        //    st.DownloadSchedule += _EdSlotStart1.time().toString("hhmmss") + "-" + _EdSlotEnd1.time().toString("hhmmss") + ";";
        //    st.DownloadSchedule += _EdSlotStart2.time().toString("hhmmss") + "-" + _EdSlotEnd2.time().toString("hhmmss") + ";";
        //    st.DownloadSchedule += _EdSlotStart3.time().toString("hhmmss") + "-" + _EdSlotEnd3.time().toString("hhmmss") + ";";
        //    st.DownloadSchedule += _EdSlotStart4.time().toString("hhmmss") + "-" + _EdSlotEnd4.time().toString("hhmmss");

        applicationPtr->updateCamera(cam);
    }
}

void CameraView::eventRemove()
{
    clearCameraInfo();

    QList<QListWidgetItem*> items = _CameraList.selectedItems();

    foreach(QListWidgetItem * item, items)
    {
        QString camname = item->data(Qt::UserRole+1).toString();

        applicationPtr->removeCamera(camname);
    }
}

void CameraView::eventRemoveAll()
{
    clearCameraInfo();

     _CameraList.clear();

     applicationPtr->removeAllCameras();
}

void CameraView::clearCameraInfo()
{
    _txtIsActive.setText("");
    _txtLastAccessed.setText("");
    _txtDirTimestamp.setText("");
    _txtFileTimestamp.setText("");
    _txtCamName.setText("");
    _txtLastAccessed.setText("");
    _txtCamName.setText("");
    _txtSDCardPort.setText("");
}

void CameraView::cleanUp()
{
    //disconnect(_CurrentProbe, &ServiceInterface::cameraFound, this, &NetworkView::eventCameraFound);
    //disconnect(_CurrentProbe, &ServiceInterface::probeFailed, this, &NetworkView::eventProbeFailed);
    //disconnect(_CurrentProbe, &ServiceInterface::deviceInformationReceived, this, &NetworkView::eventDeviceInformationReceived);
    //disconnect(_CurrentProbe, &ServiceInterface::serviceDetectionFailed, this, &NetworkView::eventServiceDetectionFailed);
    //_CurrentProbe->deleteLater();

    _cmdProbe.setEnabled(true);
    _cmdSaveCamera.setEnabled(true);
    _cmdRemove.setEnabled(true);
    _cmdRemoveAll.setEnabled(true);
}

void CameraView::eventCameraAdded(Camera &cam)
{
    clearCameraInfo();

    QString desc = "ONVIF service at http://" + cam.ip_address.trimmed() + ":" + cam.onvif_port.trimmed() + "/";
    QListWidgetItem *camItem = new QListWidgetItem(&_CameraList);
    camItem->setData(Qt::DisplayRole, QVariant(cam.camera_name));
    camItem->setData(Qt::UserRole, QVariant(desc));
    camItem->setData(Qt::UserRole+1, QVariant(cam.camera_id));

    camItem->setSelected(true);

    displayCameraInfo(cam);
}

void CameraView::eventCameraEdited(Camera &cam)
{
//    QListWidgetItem *camitem = nullptr;

//    for(int row = 0; row < _CameraList.count(); row++)
//    {
//        camitem = _CameraList.item(row);

//        if(camitem->data(Qt::UserRole+1) == cam.CameraId)
//        {
//            camitem->setData(Qt::DisplayRole, QVariant(cam.CameraName));
//            _CameraList.repaint();
//            camitem->setSelected(true);
//            break;
//        }
//    }
}

void CameraView::eventCameraDeleted(Camera &cam)
{
//    QListWidgetItem *camitem = nullptr;

//    for(int row = 0; row < _CameraList.count(); row++)
//    {
//        camitem = _CameraList.item(row);

//        if(camitem->data(Qt::UserRole+1) == cam.CameraId)
//        {
//            camitem = _CameraList.takeItem(row);
//            delete camitem;
//            break;
//        }
//    }
}
