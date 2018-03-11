#ifndef DISCOVERYHANDDLER_H
#define DISCOVERYHANDDLER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>

#define SEARCH_PORT 45454
#define SEARCH_RESULTS_PORT 45455
#define B_GROUP_ADDRESS "239.255.43.21"
#define SEARCH_QUERY_STRING "CHITCHAT_SERVER_SEARCH"
#define SEARCH_RESULT_SUBSTRING "CHITCHAT_SERVER_RESULTS"
const int tcp_server_port=8080; //This need to be moved to central place with server


/* Now only IPV4 support
 */
class DiscoveryManager:public QObject
{
    Q_OBJECT
public:
    DiscoveryManager(QObject* aParent=0);
    bool init();
    bool searchServer();
    //Need to be used by server only ,Dont expose this  guy, temporary
    bool sendMyIdentity();

private slots:
    void processRequest();
signals:
    void serverinfo(QString ip,int port);
    void search();
private:
    QUdpSocket mUdpSocket;
};


#endif // DISCOVERYHANDDLER_H
