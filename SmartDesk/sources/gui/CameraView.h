#ifndef _CAMERA_VIEW
#define _CAMERA_VIEW

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
#include "CamNameDialog.h"
#include "Camera.h"

class CameraView : public QWidget
{
    Q_OBJECT
public:
    CameraView(QWidget* parent = nullptr);
    ~CameraView();

public slots:
    void eventCameraAdded(Camera &cam);
    void eventCameraEdited(Camera &cam);
    void eventCameraDeleted(Camera &cam);

private slots:
    void eventStartdeviceProbe();
    void eventSaveChanges();
    void eventRemove();
    void eventRemoveAll();

    void eventCameraFound(Camera &cam);
    void eventCameraNotFound(Camera &cam);

    void cameraSelected(QListWidgetItem *item);

private:
    void displayCameraInfo(Camera &cam);
    void clearCameraInfo();
    void cleanUp();

private:
    QVBoxLayout _Layout;

    //
    QHBoxLayout  _ControleBar;
    QLabel       _lblIpAddress;
    QLineEdit    _txtIpAddress;
    QLabel       _lblONVIFPort;
    QLineEdit    _txtONVIFPort;
    QLabel       _lblUser;
    QLineEdit    _txtUser;
    QLabel       _lblPass;
    QLineEdit    _txtPass;
    QPushButton  _cmdProbe;
    //

    //
    DataList _CameraList;
    //

    //
    QHBoxLayout _ListOptLayout;
    QLabel  _lblCamName;
    QLineEdit _txtCamName;
    QLabel  _lblSDCardPort;
    QLineEdit _txtSDCardPort;
    QPushButton _cmdSaveCamera;
    QPushButton _cmdRemove;
    QPushButton  _cmdRemoveAll;
    //

    //
    QGridLayout _InfoArea;

    QLabel _lblIsActive;
    QLabel _lblLastAccessed;
    QLabel _lblDirTimestamp;
    QLabel _lblFileTimestamp;
    QLabel _lblRequireDownload;

    QLabel _txtIsActive;
    QLabel _txtLastAccessed;
    QLabel _txtDirTimestamp;
    QLabel _txtFileTimestamp;
    QCheckBox _ChkRequiredDownload;

    QGridLayout    _TimeSlotLayout;
    QLabel         _lblSlotStart1;
    QLabel         _lblSlotStart2;
    QLabel         _lblSlotStart3;
    QLabel         _lblSlotStart4;
    QLabel         _lblSlotEnd1;
    QLabel         _lblSlotEnd2;
    QLabel         _lblSlotEnd3;
    QLabel         _lblSlotEnd4;

    QTimeEdit      _EdSlotStart1;
    QTimeEdit      _EdSlotStart2;
    QTimeEdit      _EdSlotStart3;
    QTimeEdit      _EdSlotStart4;
    QTimeEdit      _EdSlotEnd1;
    QTimeEdit      _EdSlotEnd2;
    QTimeEdit      _EdSlotEnd3;
    QTimeEdit      _EdSlotEnd4;
};

#endif // _NETWORK_VIEW
