#ifndef DISCOVERYHANDDLER_H
#define DISCOVERYHANDDLER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>

#define SEARCH_PORT 45555
#define SEARCH_QUERY_STRING "CHITCHAT_SERVER_SEARCH"
#define SEARCH_RESULT_SUBSTRING "CHITCHAT_SERVER_RESULTS"
const int tcp_server_port=8080; //This need to be moved to central place with server, For now its Okay!

/*!
 * \brief The Class Handles Search and Resolving the Servers available in the Network . It uses Multi
 * Casting Mechanism to discover a Server (very like UPNP), Server sends his IP and Port to connect
 * When there is a Search Query.
 * This Feature can be enabled/disabled through a pro file configuration.
 * \todo it supports only IPV4 and the SendMyIdentity is Exposed to Both Client and Server . Should Retrict to Server only
 */
class DiscoveryManager:public QObject
{
    Q_OBJECT
public:
    DiscoveryManager(QObject* aParent=0);
    /*!
     * \brief sendMyIdentity function can be used to send the server identity up on a search request
     * \return
     */
    bool sendMyIdentity();

    /*!
     * \brief search
     */
    void searchMyServer();

signals:
    /*!
     * \brief serverinfo provides the remote server ip and Port received
     * \param ip : Address
     * \param port : Port Number
     */
    void serverinfo(QString ip,int port);
    void search();
private slots:
    void processResults();
private:
    bool parseRequest(QString strToParse);
private:
    QUdpSocket mSocket,mSocketSend;
};

#endif // DISCOVERYHANDDLER_H
