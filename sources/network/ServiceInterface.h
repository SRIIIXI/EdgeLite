#ifndef SERVICE_INTERFACE_H
#define SERVICE_INTERFACE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QtXml/QDomDocument>
#include <QException>
#include <QFile>
#include <QUdpSocket>
#include <QHostInfo>
#include <QNetworkAddressEntry>
#include "ONVIFCamera.h"

class ServiceInterface : public QObject
{
    Q_OBJECT
public:
    ServiceInterface(QObject* parent = nullptr);
    ServiceInterface(QString server, quint32 port, QString user, QString password, QObject* parent = nullptr);

    void setServer(QString str);
    void setPort(quint32 num);
    void setUser(QString str);
    void setPassword(QString str);
    void setTemplateDirectory(QString str);
    void setPath(QString str);

    void probeNetwork();
    void probeHost(QString ipaddress, quint32 port);

    void getSystemDateAndTime();
    void getServices();
    void getCapabilities();
    void getDeviceInformation();
    void getRecordingSearchResult();
    void getRecordings();

    QString name();
    ONVIFCamera Camera;

signals:
    void servicesReceived(ServiceInterface* cam);
    void capabilitiesReceived(ServiceInterface* cam);
    void deviceInformationReceived(ServiceInterface* cam);
    void dateTimeReceived(ServiceInterface* cam);

    void recordingsReceived(ServiceInterface* cam);
    void recordingResultsReceived(ServiceInterface* cam);

    void cameraFound(ServiceInterface* cam);
    void serviceDetectionFailed(ServiceInterface* cam);
    void probeFailed(ServiceInterface* cam);

private slots:
    void eventFinished(QNetworkReply *reply);
    void eventAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);
    void probeMatchReceived();

private:
    void parseProbeResponseBody(QDomElement &domelem);
    void parseProbedMatchList(QDomElement &domelem);
    void parseProbedMatch(QDomElement &domelem);
    void parseXAddress(QDomElement &domelem);
    void parseScopeForName(QDomElement &domelem);

    void parseResponseBody(QDomElement &domelem);
    void sendRequest(QFile *file);
    void setUpEventHandlers();

    QString _BaseUri;
    QString _TemplateDirectory;
    QString _ResourcePath;

    QNetworkAccessManager _NetAccess;
    QUdpSocket _DatagramSocket;
};

#endif // ONVIFMANAGER_H
