#include "discoveryhandler.h"
#include <QNetworkInterface>

DiscoveryManager::DiscoveryManager(QObject* aParent)
    :QObject(aParent)
{
    mSocket.bind(QHostAddress::AnyIPv4, SEARCH_PORT, QUdpSocket::ShareAddress);
    connect(&mSocket, SIGNAL(readyRead()),this, SLOT(processResults()));
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
    mSocket.writeDatagram(datagram, QHostAddress::Broadcast, SEARCH_PORT);
    return true;
}


bool DiscoveryManager::parseRequest(QString strToParse)
{
    bool success=false;
    if(strToParse==SEARCH_QUERY_STRING)
    {
        emit search();
        success=true;
    }
    if(strToParse.startsWith(SEARCH_RESULT_SUBSTRING))
    {
        strToParse=strToParse.remove(SEARCH_RESULT_SUBSTRING);
        QStringList ipDetails=strToParse.split(":");
        if(ipDetails.count()==2)
        {
            if(!ipDetails[0].isEmpty() && !ipDetails[1].isEmpty())
            {
                QString host=ipDetails[0];
                int port=ipDetails[1].toInt();
                emit serverinfo(host,port);
                success=true;
            }
        }
    }
    else
    {
        //Do Nothing
    }
    return success;
}


void DiscoveryManager::processResults()
{
    QByteArray datagram;
    while (mSocket.hasPendingDatagrams()) {
        datagram.resize(int(mSocket.pendingDatagramSize()));
        mSocket.readDatagram(datagram.data(), datagram.size());
    }
    QString strToParse=datagram.constData();
    if(!parseRequest(strToParse))
    {
        // qDebug()<<"Got a Discover Message,Ignoring!";
    }
}

void DiscoveryManager::searchMyServer()
{
    QByteArray datagram =SEARCH_QUERY_STRING;
    mSocket.writeDatagram(datagram, QHostAddress::Broadcast, SEARCH_PORT);
    return;
}
