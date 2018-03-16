#ifndef ChatSession_H
#define ChatSession_H

#include <QThread>
#include <QTcpSocket>
#ifdef ENABLE_WEBSOCKETS
#include <QtWebSockets>
#endif
#include "messagehandler.h"

/*!
 * \brief The ChatSession class
 */
class ChatSession : public QThread
{
    Q_OBJECT
#ifdef ENABLE_WEBSOCKETS
    /*!
     * \brief The Type enum
     */
    enum Type{WebSocketClient,TCPSocketClient};
#else
    /*!
     * \brief The Type enum
     */
    enum Type{TCPSocketClient};
#endif

public:
#ifdef ENABLE_WEBSOCKETS
    /*!
     * \brief ChatSession Constrctor
     * \param aSocket Web Socket Descriptor
     * \param aParent
     */
    ChatSession(QWebSocket *aSocket,QObject *aParent = 0);
 #endif
    /*!
     * \brief ChatSession Constrctor
     * \param aSocket TCP socket descriptor
     * \param aParent
     */
    ChatSession(QTcpSocket *aSocket, QObject *aParent = 0);

    virtual ~ChatSession();
    /*!
     * \brief name
     * \return session user name
     */
    QString name()
    {
        return mName;
    }
    /*!
     * \brief id
     * \return session internal id
     */
    QString id()
    {
        return mId;
    }
    //Over Ridden
    void run();
signals:
    /*!
     * \brief error Triggers on Socket Error
     * \param socketerror
     */
    void error(QTcpSocket::SocketError socketerror);
    /*!
     * \brief disconnecting Signal when session get disconnected
     * \param sessionId
     */
    void disconnecting(const QString&sessionId);
    /*!
     * \brief loggedin Signal triggers when a user logs in
     * \param aMessage
     * \param sessionId
     */
    void loggedin(const Message &aMessage,const QString&sessionId);
    /*!
     * \brief newmessage When a new message comes in the signal triggers
     * \param aMessage
     * \param sessionId
     */
    void newmessage(const Message &aMessage,const QString&sessionId);

public slots:
    /*!
     * \brief readyRead
     */
    void readyRead();
    /*!
     * \brief disconnected
     */
    void disconnected();
    /*!
     * \brief write
     * \param aData
     */
    void write(QByteArray aData);
#ifdef ENABLE_WEBSOCKETS
    /*!
     * \brief onWebSocketRead
     * \param message
     */
    void onWebSocketRead(const QString &message);
#endif

private:
    /*!
     * \brief onClientRead
     * \param data
     */
    void onClientRead(QByteArray data);
private:
    QString mName;
    QString mId;
    QTcpSocket *mSocket;
#ifdef ENABLE_WEBSOCKETS
    QWebSocket *mWebSocket;
#endif
    Type mType;
};

#endif // ChatSession_H
