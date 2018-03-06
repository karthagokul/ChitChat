#include "clientconnection.h"
#include "messagehandler.h"

ClientConnection::ClientConnection(QObject *parent) :
    QObject(parent),mSocket(new QTcpSocket(this)),mHostIp("127.0.0.1"),mPort(8080),mActive(false),mName(QHostInfo::localHostName())
{
    connect(mSocket,SIGNAL(readyRead()),this,SLOT(onRead()));
    connect(mSocket,SIGNAL(connected()),this,SLOT(onConnected()));
    connect(mSocket,SIGNAL(disconnected()),this,SLOT(onDisconnected()));
    connect(mSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onError(QAbstractSocket::SocketError)));
}

void ClientConnection::setServer(QString aHostName,int aPort)
{
    mHostIp=aHostName;
    mPort=aPort;
}

void ClientConnection::setUserName(QString aName)
{
    mName=aName;
}

void ClientConnection::onConnected()
{
    qDebug()<<"Connected";
    mActive=true;

    Message loginmessage;
    loginmessage.type=LogOn;
    loginmessage.message=mName;
    MessageHandler  m;
    QByteArray array=m.createMessage(loginmessage);
    mSocket->write(array);
    emit stateChanged();
}

void ClientConnection::onDisconnected()
{
    qDebug()<<"Disconnected";
    mActive=false;
    emit stateChanged();
}

void ClientConnection::start()
{
    if(isActive())
    {
        qDebug()<<"Already Have an Active Session";
        return;
    }
    qDebug()<<"Connecting to "<<mHostIp<<":"<<mPort;
    mSocket->connectToHost(mHostIp,mPort);
}

void ClientConnection::onError(QAbstractSocket::SocketError aError)
{
    qDebug()<<aError;

    emit stateChanged();
}

void ClientConnection::stop()
{
    if(!isActive())
    {
        qDebug()<<"Do not have Active Session";
        return;
    }
    mSocket->disconnectFromHost();
    mSocket->close();
}

void ClientConnection::onRead()
{
    //QByteArray dataBytes = mSocket->readAll();
    //QString data=QString::fromStdString(dataBytes.toStdString());
    //qDebug()  << " Server Says: " << data;
    MessageHandler mH;
    Message readmessage=mH.parseMessageFromServer(mSocket->readAll());
    switch (readmessage.type) {
    case Invalid:
        qDebug()<<"Unsupported Message Type";
        break;
    case Online:
        qDebug()<<"Got Buddies: " <<readmessage.buddies;
        break;
    default:
        break;
    }

}

ClientConnection::~ClientConnection()
{
    if(mSocket)
    {
        delete mSocket;
        mSocket=0;
    }
}

