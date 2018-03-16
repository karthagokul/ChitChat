#ifndef DISCOVERYHANDDLER_H
#define DISCOVERYHANDDLER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>

#define SEARCH_PORT 45454
#define SEARCH_RESULTS_PORT 45455
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
     * \brief init Function to initiate the discovery mechanism
     * \return
     */
    bool init();
    /*!
     * \brief searchServer A Client can use this function to initiate a search for the available server in the network
     * \return
     */
    bool searchServer();
    /*!
     * \brief sendMyIdentity function can be used to send the server identity up on a search request
     * \return
     */
    bool sendMyIdentity();

private slots:
    void processSearchRequest();
    void processResultRequest();
protected:
    bool parseRequest(QString data);
signals:
    /*!
     * \brief serverinfo provides the remote server ip and Port received
     * \param ip : Address
     * \param port : Port Number
     */
    void serverinfo(QString ip,int port);
    /*!
     * \brief The Signal Get Triggered Up on a Search Query
     */
    void search();
private:
    QUdpSocket mReceiveSocket,mSendSocket;
};


#endif // DISCOVERYHANDDLER_H
