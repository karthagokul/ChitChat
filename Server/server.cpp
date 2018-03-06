#include "server.h"
#include "chatconnection.h"
#include <QHostAddress>
#include <QNetworkInterface>

Server::Server(QObject *aParent)
    :QTcpServer(aParent)
{

}

Server::~Server()
{
    qDebug()<<"Closing Sockets";
}

bool Server::init()
{
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&  ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    qDebug()<<"Using IP :"<<ipAddress;

    return true;
}
