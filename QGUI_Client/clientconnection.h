#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QTcpSocket>
#include <QtNetwork>

class ClientConnection : public QObject
{
    Q_OBJECT
public:
    explicit ClientConnection(QObject *parent = 0);
    virtual ~ClientConnection();
    void setServer(QString aHostName,int aPort);
    void setUserName(QString aName);
    void stop();
    void start();
    void send(QString &aData);
    bool isActive()
    {
        return mActive;
    }

    QStringList buddies()
    {
        return mBuddies;
    }

private:
    QString getRandomName() const;

signals:
    void stateChanged();
    void buddylist();
    //need to change the below logic
    void newMessage(QString message,QString sender);
    void error(QString aMessage);

protected slots:
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError aError);
    void onRead();
private:
    QTcpSocket *mSocket;
    QString mName;
    QString mHostIp;
    int mPort;
    bool mActive;
    QStringList mBuddies;
};

#endif // CLIENTCONNECTION_H
