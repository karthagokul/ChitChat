#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QMap>
#include <QTcpServer>

class ChatRoom;

const int server_port=8080;

/*!
 * \brief The Server class
 */
class Server:public QTcpServer
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

    void incomingConnection(qintptr socketDescriptor);

private:
    ChatRoom *mChatRoom;
};

#endif // SERVER_H
