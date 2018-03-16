#include "discoveryhandler.h"
#include <QNetworkInterface>

DiscoveryManager::DiscoveryManager(QObject* aParent)
    :QObject(aParent)
{
}

bool DiscoveryManager::searchServer()
{
    QByteArray datagram =SEARCH_QUERY_STRING;
    mClientSocket.writeDatagram(datagram, QHostAddress::Broadcast, SEARCH_PORT);
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
    mServerSocket.writeDatagram(datagram, QHostAddress::Broadcast, SEARCH_RESULTS_PORT);
    return true;
}

bool DiscoveryManager::init(bool isSearchClient)
{
    mSearchClient=isSearchClient;
    if(isSearchClient)
    {
        mClientSocket.bind(QHostAddress::AnyIPv4, SEARCH_RESULTS_PORT, QUdpSocket::ShareAddress);
        connect(&mClientSocket, SIGNAL(readyRead()),this, SLOT(processResultRequest()));
    }
    else
    {
        mServerSocket.bind(QHostAddress::AnyIPv4, SEARCH_PORT, QUdpSocket::ShareAddress);
        //mReceiveSocket.joinMulticastGroup(QHostAddress(QStringLiteral(B_GROUP_ADDRESS)));
        connect(&mServerSocket, SIGNAL(readyRead()),this, SLOT(processSearchRequest()));
    }
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
void DiscoveryManager::processResultRequest()
{
    qDebug()<<"Reading";
    QByteArray datagram;

    // using QUdpSocket::readDatagram (API since Qt 4)
    while (mClientSocket.hasPendingDatagrams()) {
        datagram.resize(int(mClientSocket.pendingDatagramSize()));
        mClientSocket.readDatagram(datagram.data(), datagram.size());
    }
    QString strToParse=datagram.constData();
    if(!parseRequest(strToParse))
    {
        qDebug()<<"Got a Discover Message,Ignoring!";
    }

}

void DiscoveryManager::processSearchRequest()
{
    qDebug()<<"got search";
   QByteArray datagram;

    // using QUdpSocket::readDatagram (API since Qt 4)
    while (mServerSocket.hasPendingDatagrams()) {
        datagram.resize(int(mServerSocket.pendingDatagramSize()));
        mServerSocket.readDatagram(datagram.data(), datagram.size());
    }
    QString strToParse=datagram.constData();
    if(!parseRequest(strToParse))
    {
        // qDebug()<<"Got a Discover Message,Ignoring!";
    }
}
