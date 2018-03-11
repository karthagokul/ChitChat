#include "discoveryhandler.h"
#include <QNetworkInterface>

DiscoveryManager::DiscoveryManager(QObject* aParent)
    :QObject(aParent)
{
}

bool DiscoveryManager::searchServer()
{
    QByteArray datagram =SEARCH_QUERY_STRING;
    mUdpSocket.writeDatagram(datagram, QHostAddress(QStringLiteral(B_GROUP_ADDRESS)), SEARCH_PORT);
    return true;
}

bool DiscoveryManager::sendMyIdentity()
{
    QString ip;
    //Should be improvised
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
            if(!address.toString().isEmpty())
            {
               ip=address.toString();
            }
    }
    if(ip.isEmpty())
    {
        return false;
    }

    QByteArray datagram =QString(SEARCH_RESULT_SUBSTRING+ip+QString(":")+QString::number(tcp_server_port)).toLocal8Bit();
    mUdpSocket.writeDatagram(datagram, QHostAddress(QStringLiteral(B_GROUP_ADDRESS)), SEARCH_PORT);
    return true;
}

bool DiscoveryManager::init()
{
    mUdpSocket.bind(QHostAddress::AnyIPv4, SEARCH_PORT, QUdpSocket::ShareAddress);
    mUdpSocket.joinMulticastGroup(QHostAddress(QStringLiteral(B_GROUP_ADDRESS)));
    connect(&mUdpSocket, SIGNAL(readyRead()),this, SLOT(processRequest()));
    return true;
}

void DiscoveryManager::processRequest()
{
    QByteArray datagram;

    // using QUdpSocket::readDatagram (API since Qt 4)
    while (mUdpSocket.hasPendingDatagrams()) {
        datagram.resize(int(mUdpSocket.pendingDatagramSize()));
        mUdpSocket.readDatagram(datagram.data(), datagram.size());
        qDebug()<<(tr("Received IPv4 datagram: \"%1\"")
                   .arg(datagram.constData()));
    }
    QString strToParse=datagram.constData();
    if(strToParse==SEARCH_QUERY_STRING)
    {
        qDebug()<<"Someone is Searching me";
        emit search();
       // DiscoverySender sender;
      //  if(!sender.sendIdentity())
      //  {
      //     qCritical()<<"Unable to respond to Search Request";
       // }
    }
    if(strToParse.startsWith(SEARCH_RESULT_SUBSTRING))
    {
        qDebug()<<"Got a Search Result";
        strToParse=strToParse.remove(SEARCH_RESULT_SUBSTRING);
        QStringList ipDetails=strToParse.split(":");
        if(ipDetails.count()==2)
        {
            QString host=ipDetails[0];
            int port=ipDetails[1].toInt();
            emit serverinfo(host,port);
        }
    }
    else
    {
        //Do Nothing
    }
}
