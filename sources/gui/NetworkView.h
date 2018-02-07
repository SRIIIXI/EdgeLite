#ifndef _NETWORK_VIEW
#define _NETWORK_VIEW

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QtCore>
#include <QToolBar>
#include <QLineEdit>
#include <QFileDialog>
#include <QVariant>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QRadioButton>
#include <QPushButton>
#include <QMessageBox>
#include "CustomWidgets.h"
#include "ServiceInterface.h"

class NetworkView : public QWidget
{
    Q_OBJECT
public:
    NetworkView(QWidget* parent = nullptr);
    ~NetworkView();
    void loadCameraList();

signals:
    void probeTimedOut();

public slots:
    void startNetworkScan();
    void startHostProbe();
    void eventProbeTimedOut();
    void eventSaveName();
    void eventRemove();
    void eventRemoveAll();

    void eventCameraFound(ServiceInterface* cam);
    void eventProbeFailed(ServiceInterface* cam);

    void eventDeviceInformationReceived(ServiceInterface* cam);
    void eventServicesReceived(ServiceInterface *cam);
    void eventDateTimeReceived(ServiceInterface* cam);
    void eventCapabilitiesReceived(ServiceInterface *cam);
    void eventServiceDetectionFailed(ServiceInterface *cam);

private slots:
    void cameraSelected(QListWidgetItem *item);

private:
    void displayCameraInfo(ONVIFCamera *cam);
    void clearCameraInfo();
    void cleanUp();

private:
    QVBoxLayout _Layout;

    //
    QHBoxLayout  _ControleBar;
    QLabel       _lblIpAddress;
    QLineEdit    _txtIpAddress;
    QLabel       _lblPort;
    QLineEdit    _txtPort;
    QLabel       _lblUser;
    QLineEdit    _txtUser;
    QLabel       _lblPass;
    QLineEdit    _txtPass;
    QPushButton  _cmdProbe;
    QPushButton  _cmdScan;
    //

    //
    DataList _CameraList;
    //

    //
    QHBoxLayout _ListOptLayout;
    QLabel  _lblCamFriendlyName;
    QLineEdit _txtCamFriendlyName;
    QPushButton _cmdSaveName;
    QPushButton _cmdRemove;
    QPushButton  _cmdRemoveAll;
    //

    //
    QGridLayout _InfoArea;
    QLabel lblManufacturer;
    QLabel lblModel;
    QLabel lblFirmwareVersion;
    QLabel lblSerialNumber;
    QLabel lblHardwareId;
    QLabel lblCameraName;
    QLabel lblHardware;
    QLabel lblUTCTime;

    QLabel txtManufacturer;
    QLabel txtModel;
    QLabel txtFirmwareVersion;
    QLabel txtSerialNumber;
    QLabel txtHardwareId;
    QLabel txtCameraName;
    QLabel txtHardware;
    QLabel txtUTCTime;
    //

    //
    DataList _ServiceList;
    //

    ServiceInterface* _CurrentProbe;
};

#endif // _NETWORK_VIEW
