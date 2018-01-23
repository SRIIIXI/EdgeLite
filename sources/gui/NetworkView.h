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
#include "CustomWidgets.h"
#include "ServiceInterface.h"

class NetworkView : public QWidget
{
    Q_OBJECT
public:
    NetworkView(QWidget* parent = nullptr);
    ~NetworkView();

signals:
    void probeTimedOut();

public slots:
    void startNetworkScan();
    void startHostProbe();
    void eventProbeTimedOut();

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
    void displayCameraInfo(ServiceInterface *cam);

private:
    QVBoxLayout _Layout;
    QGridLayout _ControleBar;
    DataList _CameraList;

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
    QPushButton  _cmdClearAll;

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

    DataList _ServiceList;
    ServiceInterface* _CurrentProbe;
};

#endif // _NETWORK_VIEW
