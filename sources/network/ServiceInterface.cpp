#include "ServiceInterface.h"

ServiceInterface::ServiceInterface(QObject *parent)
    : QObject(parent)
{
    setUpEventHandlers();
}

ServiceInterface::ServiceInterface(QString server, quint32 port, QString user, QString password, QObject* parent)
    : QObject(parent)
{
    IpAddress = server;
    ServicePort = port;
    ServiceUser = user;
    Password = password;
    setUpEventHandlers();
}

void ServiceInterface::setServer(QString str)
{
    IpAddress = str;
}

void ServiceInterface::setPort(quint32 num)
{
    ServicePort = num;
}

void ServiceInterface::setUser(QString str)
{
    ServiceUser = str;
}

void ServiceInterface::setPassword(QString str)
{
    Password = str;
}

void ServiceInterface::setTemplateDirectory(QString str)
{
    _TemplateDirectory = str;
}

void ServiceInterface::setPath(QString str)
{
    _ResourcePath = str;
}

QString ServiceInterface::name()
{
    return DeviceName;
}

void ServiceInterface::setUpEventHandlers()
{
    QObject::connect(&_NetAccess, SIGNAL(finished(QNetworkReply*)), this, SLOT(eventFinished(QNetworkReply*)));
    QObject::connect(&_NetAccess, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)), this, SLOT(eventAuthenticationRequired(QNetworkReply*, QAuthenticator*)));
    QObject::connect(&_DatagramSocket, SIGNAL(readyRead()), this, SLOT(probeMatchReceived()));
}

void ServiceInterface::getSystemDateAndTime()
{
    QFile file(_TemplateDirectory + "/" + "Device.GetSystemDateAndTime.xml");

    if(file.open(QIODevice::ReadOnly))
    {
        sendRequest(&file);
    }
}

void ServiceInterface::getServices()
{
    QFile file(_TemplateDirectory + "/" + "Device.GetServices.xml");

    if(file.open(QIODevice::ReadOnly))
    {
        sendRequest(&file);
    }
}

void ServiceInterface::getCapabilities()
{
    QFile file(_TemplateDirectory + "/" + "Device.GetCapabilities.xml");

    if(file.open(QIODevice::ReadOnly))
    {
        sendRequest(&file);
    }
}

void ServiceInterface::getDeviceInformation()
{
    QFile file(_TemplateDirectory + "/" + "Device.GetDeviceInformation.xml");

    if(file.open(QIODevice::ReadOnly))
    {
        sendRequest(&file);
    }
}

void ServiceInterface::getRecordingSearchResult()
{
    QFile file(_TemplateDirectory + "/" + "Search.GetRecordingSearchResult.xml");

    if(file.open(QIODevice::ReadOnly))
    {
        sendRequest(&file);
    }
}

void ServiceInterface::getRecordings()
{
    QFile file(_TemplateDirectory + "/" + "Recording.GetRecordings.xml");

    if(file.open(QIODevice::ReadOnly))
    {
        sendRequest(&file);
    }
}

void ServiceInterface::sendRequest(QFile *file)
{
    QByteArray buffer = file->readAll();
    QDomDocument dom;

    if(dom.setContent(buffer))
    {
        if(_BaseUri.length() < 1)
        {
            _BaseUri = "http://" + IpAddress + ":" + QVariant(ServicePort).toString() + "/";
        }

        QString url = _BaseUri;

        QNetworkRequest request;
        request.setUrl(QUrl(url));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml");
        _NetAccess.post(request, dom.toByteArray());
    }
}

void ServiceInterface::eventAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    authenticator->setUser(ServiceUser);
    authenticator->setPassword(Password);
}

void ServiceInterface::eventFinished(QNetworkReply *reply)
{
    bool reqfailflag = false;

    QNetworkReply::NetworkError err = reply->error();

    if(err == QNetworkReply::NoError || err == QNetworkReply::HostNotFoundError)
    {
        QString str = reply->readAll();

        QDomDocument doc;
        try
        {
            if(doc.setContent(str))
            {
                QDomElement docElem = doc.documentElement();

                QDomNode firstnode = docElem.firstChild();

                while(!firstnode.isNull())
                {
                  QDomElement firstelement = firstnode.toElement();

                  if(!firstelement.isNull())
                  {
                      if(firstelement.tagName().trimmed().contains("SOAP-ENV:Body") || firstelement.tagName().trimmed().contains(":Body"))
                      {
                            parseResponseBody(firstelement);
                      }
                  }

                  firstnode = firstnode.nextSibling();
                }
            }
            else
            {
                reqfailflag = true;
            }
        }
        catch(QException ex)
        {
            reqfailflag = true;
        }
    }
    else
    {
        reqfailflag = true;
    }

    if(reqfailflag)
    {
        emit serviceDetectionFailed(this);
    }
}

void ServiceInterface::parseResponseBody(QDomElement &domelem)
{
    QDomNode firstchild = domelem.firstChild();

    while(!firstchild.isNull())
    {
      QDomElement firstelement = firstchild.toElement();

      if(!firstelement.isNull())
      {
          if(firstelement.tagName().trimmed().contains("GetSystemDateAndTimeResponse"))
          {
              QDomNode datanode = firstelement.firstChild();

              while(!datanode.isNull())
              {
                  QDomElement dataelem = datanode.toElement();

                  if(!dataelem.isNull())
                  {
                      if(dataelem.tagName().trimmed().contains("SystemDateAndTime"))
                      {
                          QDomNode tsnode = dataelem.firstChild();

                          while(!tsnode.isNull())
                          {
                              QDomElement tselem = tsnode.toElement();

                              if(!tselem.isNull())
                              {
                                  if(tselem.tagName().trimmed().contains("UTCDateTime"))
                                  {
                                      QString yytag = tselem.tagName().replace("UTCDateTime", "Year");
                                      QString MMtag = tselem.tagName().replace("UTCDateTime", "Month");
                                      QString ddtag = tselem.tagName().replace("UTCDateTime", "Day");
                                      QString hhtag = tselem.tagName().replace("UTCDateTime", "Hour");
                                      QString mmtag = tselem.tagName().replace("UTCDateTime", "Minute");
                                      QString sstag = tselem.tagName().replace("UTCDateTime", "Second");

                                      if(tselem.elementsByTagName(yytag).count() > 0)
                                      {
                                          UTCTimestamp += tselem.elementsByTagName(yytag).at(0).toElement().text() + "/";
                                      }
                                      if(tselem.elementsByTagName(MMtag).count() > 0)
                                      {
                                          UTCTimestamp += tselem.elementsByTagName(MMtag).at(0).toElement().text() + "/";
                                      }
                                      if(tselem.elementsByTagName(ddtag).count() > 0)
                                      {
                                          UTCTimestamp += tselem.elementsByTagName(ddtag).at(0).toElement().text() + " ";
                                      }
                                      if(tselem.elementsByTagName(hhtag).count() > 0)
                                      {
                                          UTCTimestamp += tselem.elementsByTagName(hhtag).at(0).toElement().text() + ":";
                                      }
                                      if(tselem.elementsByTagName(mmtag).count() > 0)
                                      {
                                          UTCTimestamp += tselem.elementsByTagName(mmtag).at(0).toElement().text() + ":";
                                      }
                                      if(tselem.elementsByTagName(sstag).count() > 0)
                                      {
                                          UTCTimestamp += tselem.elementsByTagName(sstag).at(0).toElement().text() + " UTC";
                                      }
                                  }
                              }

                              tsnode = tsnode.nextSibling();
                          }
                      }
                  }

                  datanode = datanode.nextSibling();
              }

              emit dateTimeReceived(this);

              return;
          }

          if(firstelement.tagName().trimmed().contains("GetDeviceInformationResponse"))
          {
              QDomNode datanode = firstelement.firstChild();

              while(!datanode.isNull())
              {
                  QDomElement dataelem = datanode.toElement();

                  if(!dataelem.isNull())
                  {
                      if(dataelem.tagName().trimmed().contains("Manufacturer"))
                      {
                          Manufacturer = dataelem.text();
                      }

                      if(dataelem.tagName().trimmed().contains("Model"))
                      {
                          Model = dataelem.text();
                      }

                      if(dataelem.tagName().trimmed().contains("FirmwareVersion"))
                      {
                          FirmwareVersion = dataelem.text();
                      }

                      if(dataelem.tagName().trimmed().contains("SerialNumber"))
                      {
                          SerialNumber = dataelem.text();
                      }

                      if(dataelem.tagName().trimmed().contains("HardwareId"))
                      {
                          HardwareId = dataelem.text();
                      }
                  }

                  datanode = datanode.nextSibling();
              }

              if(DeviceName.length() < 1)
              {
                  CameraName = DeviceName = Manufacturer + "-" + Model;
              }

              if(Hardware.length() < 1)
              {
                  Hardware = HardwareId;
              }

              if(DefaultServiceUri.length() < 1)
              {
                  DefaultServiceUri = "http://" + IpAddress + ":" + QVariant(ServicePort).toString() + "/onvif/device_service";
              }

              emit deviceInformationReceived(this);

              return;
          }

          if(firstelement.tagName().trimmed().contains("GetCapabilitiesResponse"))
          {
          }

          if(firstelement.tagName().trimmed().contains("GetServicesResponse"))
          {
              QDomNode datanode = firstelement.firstChild();

              while(!datanode.isNull())
              {
                  QDomElement dataelem = datanode.toElement();

                  if(!dataelem.isNull())
                  {
                      if(dataelem.tagName().trimmed().contains("Service"))
                      {
                          ONVIFService onvifserv;

                          QString nstag = dataelem.tagName().replace("Service", "Namespace");
                          QString xatag = dataelem.tagName().replace("Service", "XAddr");

                          if(dataelem.elementsByTagName(nstag).count() > 0)
                          {
                              onvifserv.NameSpace = dataelem.elementsByTagName(nstag).at(0).toElement().text();
                          }

                          if(dataelem.elementsByTagName(xatag).count() > 0)
                          {
                              onvifserv.XAddress = dataelem.elementsByTagName(xatag).at(0).toElement().text();
                          }

                          ServiceList.append(onvifserv);
                      }
                  }

                  datanode = datanode.nextSibling();
              }

              emit servicesReceived(this);

              return;
          }
      }

      firstchild = firstchild.nextSibling();
    }
}

// UDP Probe

void ServiceInterface::probeNetwork()
{
    QString hostip;

    QStringList iplist;

    QList<QNetworkInterface> ifs = QNetworkInterface::allInterfaces();

    for (int ni = 0; ni < ifs.size(); ni++)
    {
        QNetworkInterface ninf = ifs[ni];

        if(!ninf.humanReadableName().toLower().contains("ethernet") && !ninf.humanReadableName().toLower().contains("wi-fi"))
        {
            continue;
        }

        QList<QNetworkAddressEntry> addrs = ninf.addressEntries();

        for(int idx = 0; idx < addrs.size(); idx++)
        {
            QNetworkAddressEntry ent = addrs[idx];

            if(ent.ip().protocol() != QAbstractSocket::IPv4Protocol || ent.ip().isLoopback())
            {
                continue;
            }

            iplist.append(ent.ip().toString());
        }
    }

    QList<QHostAddress> list = QHostInfo::fromName(QHostInfo::localHostName()).addresses();

    for (int i = 0; i < list.size(); i++)
    {
        QHostAddress addr = list[i];

        if(addr.protocol() != QAbstractSocket::IPv4Protocol || addr.isLoopback())
        {
            continue;
        }

        QString ip = list[i].toString();

        if(iplist.contains(ip))
        {
            hostip = ip;
        }
    }

    QFile file(_TemplateDirectory + "/" + "Discovery.Probe.xml");

    QStringList ipfragments = hostip.split('.', QString::SkipEmptyParts);

    ipfragments.removeLast();

    QString destipbase;
    destipbase += ipfragments.at(0);
    destipbase += "." ;
    destipbase += ipfragments.at(1);
    destipbase += "." ;
    destipbase += ipfragments.at(2);
    destipbase += "." ;

    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray datagram = file.readAll();
        QDomDocument dom;

        if(dom.setContent(datagram))
        {
            for(unsigned int ipnum = 1; ipnum < 256; ipnum++)
            {
                QString destip = destipbase + QVariant(ipnum).toString();
                QHostAddress destaddress(destip);

                _DatagramSocket.writeDatagram(datagram, destaddress, 80);
                _DatagramSocket.writeDatagram(datagram, destaddress, 8080);
                //for(unsigned int portnum = 1; portnum <= 65535; portnum++)
                //{
                //    _DatagramSocket.writeDatagram(datagram, destaddress, portnum);
                //}
            }
        }
    }
}

void ServiceInterface::probeHost(QString ipaddress, quint32 port)
{
    QFile file(_TemplateDirectory + "/" + "Discovery.Probe.xml");

    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray datagram = file.readAll();
        QDomDocument dom;

        if(dom.setContent(datagram))
        {
            QHostAddress destaddress(ipaddress);
            _DatagramSocket.writeDatagram(datagram, destaddress, port);
        }
    }
}

void ServiceInterface::probeMatchReceived()
{
    QHostAddress sender;
    quint16 senderPort;
    QByteArray datagram;

    do {
        datagram.resize(_DatagramSocket.pendingDatagramSize());
        _DatagramSocket.readDatagram(datagram.data(), datagram.size(),&sender, &senderPort);
    } while (_DatagramSocket.hasPendingDatagrams());

    QDomDocument doc;

    if(doc.setContent(datagram))
    {
        QDomElement docElem = doc.documentElement();

        QDomNode firstnode = docElem.firstChild();

        while(!firstnode.isNull())
        {
          QDomElement firstelement = firstnode.toElement();

          if(!firstelement.isNull())
          {
              if(firstelement.tagName().trimmed().contains("SOAP-ENV:Body"))
              {
                    parseProbeResponseBody(firstelement);
              }
          }

          firstnode = firstnode.nextSibling();
        }
    }
    else
    {
        qDebug() << "Bad XML";
        emit probeFailed(this);
    }
}

void ServiceInterface::parseProbeResponseBody(QDomElement &domelem)
{
      QDomNode n = domelem.firstChild();

      while(!n.isNull())
      {
          QDomElement e = n.toElement();

          if(!e.isNull())
          {
              if(e.tagName().trimmed().contains("ProbeMatches"))
              {
                  parseProbedMatchList(e);
              }
          }

          n = n.nextSibling();
      }
}

void ServiceInterface::parseProbedMatchList(QDomElement &domelem)
{
    QDomNode n = domelem.firstChild();

    while(!n.isNull())
    {
        QDomElement e = n.toElement();

        if(!e.isNull())
        {
            if(e.tagName().trimmed().contains("ProbeMatch"))
            {
                parseProbedMatch(e);
            }
        }

        n = n.nextSibling();
    }
}

void ServiceInterface::parseProbedMatch(QDomElement &domelem)
{
    QDomNode n = domelem.firstChild();

    while(!n.isNull())
    {
        QDomElement e = n.toElement();

        if(!e.isNull())
        {
            if(e.tagName().trimmed().contains("XAddrs"))
            {
                parseXAddress(e);
            }

            if(e.tagName().trimmed().contains("Scopes"))
            {
                parseScopeForName(e);
            }
        }

        n = n.nextSibling();
    }

    emit cameraFound(this);
}

void ServiceInterface::parseXAddress(QDomElement &domelem)
{
    QUrl url(domelem.text());
    IpAddress = url.host();
    ServicePort = url.port();
    DefaultServiceUri = url.path();
}

void ServiceInterface::parseScopeForName(QDomElement &domelem)
{
    QStringList list;
    QString str = domelem.text();
    list = str.split(' ', QString::SkipEmptyParts);
    ProbeList.append(list);

    foreach(QString srvstr, list)
    {
        srvstr = srvstr.replace("onvif://www.onvif.org/", "");
        QStringList nlist = srvstr.split('/', QString::SkipEmptyParts);

        if(nlist.count() == 2)
        {
            if(nlist.at(0).toLower() == "name")
            {
                CameraName = DeviceName = nlist.at(1);
            }

            if(nlist.at(0).toLower() == "hardware")
            {
                Hardware = nlist.at(1);
            }
        }
    }
}

QString ServiceInterface::storagePath(QString url)
{
    return "";

}
