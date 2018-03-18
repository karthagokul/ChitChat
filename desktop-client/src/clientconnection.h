#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QTcpSocket>
#include <QtNetwork>
#include <QThread>

class ClientConnection : public QThread
{
    Q_OBJECT
public:
    explicit ClientConnection(QObject *parent = 0);
    virtual ~ClientConnection();
    void setServer(QString aHostName,int aPort);
    void setUserName(QString aName);
    void run();
    void connectNow();
    void disconnectFromServer();
    void send(const QString &aData);
    void sendToSelected(const QString &aData,const QStringList &aSelectedBuddies);
    bool isActive()
    {
        return mActive;
    }

    QStringList buddies()
    {
        return mBuddies;
    }

    QString name()
    {
        return mName;
    }
signals:
    void stateChanged();
    void buddylist(QString aMessage);
    //need to change the below logic
    void newMessage(QString message,QString sender,bool emphasise=false);
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
