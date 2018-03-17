#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QMap>
#include <QTcpServer>

#ifdef ENABLE_WEBSOCKETS
#include <QtWebSockets>
#endif

class ChatRoom;

const int tcp_port=8080;

#ifdef ENABLE_WEBSOCKETS
const int web_port=9090;
#endif

/*!
 * \brief The Server class
 */
class Server:public QObject
{
    Q_OBJECT
public:
    Server(QObject *aParent);
    /*!
     * \brief init
     * \return the status of init operation
     */
    bool init();

    /*!
     * \brief ~Server
     */
    virtual ~Server();

protected slots:
    /*!
     * \brief onNewTCPConnection
     */
    void onNewTCPConnection();
#ifdef ENABLE_WEBSOCKETS
    /*!
     * \brief onNewWebConnection
     */
    void onNewWebConnection();
#endif
private:
    ChatRoom *mChatRoom;
    QTcpServer *mSocketServer;
#ifdef ENABLE_WEBSOCKETS
    QWebSocketServer *mWebSocketServer;
#endif
};

#endif // SERVER_H
