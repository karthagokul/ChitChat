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
    enum Type{WebSocketClient,TCPSocketClient};
#else
    enum Type{TCPSocketClient};
#endif

public:
#ifdef ENABLE_WEBSOCKETS
    ChatSession(QWebSocket *aSocket,QObject *aParent = 0);
#endif
    ChatSession(QTcpSocket *aSocket, QObject *aParent = 0);
    virtual ~ChatSession();
    QString name()
    {
        return mName;
    }
    QString id()
    {
        return mId;
    }

    void run();
signals:
    void error(QTcpSocket::SocketError socketerror);
    void disconnecting(const QString&sessionId);
    void loggedin(const Message &aMessage,const QString&sessionId);
    void newmessage(const Message &aMessage,const QString&sessionId);

public slots:
    void readyRead();
    void disconnected();
    void write(QByteArray aData);
#ifdef ENABLE_WEBSOCKETS
    void onWebSocketRead(const QString &message);
#endif

private:
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
