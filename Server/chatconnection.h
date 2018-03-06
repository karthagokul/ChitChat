#ifndef CHATCONNECTION_H
#define CHATCONNECTION_H

#include <QThread>
#include <QTcpSocket>

class ChatRoom;
class ChatConnection : public QThread
{
    Q_OBJECT
public:
    explicit ChatConnection(const ChatRoom *aRoom,qintptr aSocketID, QObject *aParent = 0);
    virtual ~ChatConnection();
    void run();

signals:
    void error(QTcpSocket::SocketError socketerror);
    void newMessage(const QString &aString,const qintptr&sessionId);
    void identityRecived(const QString &aIdentity);
    void disconnecting(const qintptr&sessionId);

public slots:
    void readyRead();
    void disconnected();

private:
    QString mName;
    QTcpSocket *mSocket;
    qintptr mSocketDescriptor;
    const ChatRoom *mChatRoom;
};

#endif // CHATCONNECTION_H
