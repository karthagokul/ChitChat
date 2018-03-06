#ifndef CHATCONNECTION_H
#define CHATCONNECTION_H

#include <QThread>
#include <QTcpSocket>
#include "messagehandler.h"

class ChatRoom;
class ChatConnection : public QThread
{
    Q_OBJECT
public:
    explicit ChatConnection(qintptr aSocketID, QObject *aParent = 0);
    virtual ~ChatConnection();
    QString name()
    {
        return mName;
    }

    void run();

signals:
    void error(QTcpSocket::SocketError socketerror);
    void disconnecting(const qintptr&sessionId);
    void loggedin(const Message &aMessage,const qintptr&sessionId);
    void newmessage(const Message &aMessage,const qintptr&sessionId);

public slots:
    void readyRead();
    void disconnected();
    void write(QByteArray aData);

private:
    QString mName;
    QTcpSocket *mSocket;
    qintptr mSocketDescriptor;
};

#endif // CHATCONNECTION_H
