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
    bool isActive()
    {
        return mActive;
    }

signals:
    void stateChanged();

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
};

#endif // CLIENTCONNECTION_H
