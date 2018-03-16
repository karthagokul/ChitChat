
#include "clientconnection.h"
#include "messagehandler.h"

ClientConnection::ClientConnection(QObject *parent) :
    QObject(parent),mSocket(new QTcpSocket(this)),mHostIp("127.0.0.1"),mPort(8080),mName(getRandomName()),mActive(false)
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
    mActive=true;
    Message loginmessage(Message::LogOn,mName,QString(),QStringList());
    mSocket->write(loginmessage.toByteArray());
    qDebug()<<loginmessage.toByteArray();
    emit stateChanged();
}

void ClientConnection::onDisconnected()
{
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
    mSocket->connectToHost(mHostIp,mPort);
}

void ClientConnection::onError(QAbstractSocket::SocketError aError)
{
    qDebug()<<aError;

    switch(aError)
    {
    case QAbstractSocket::ConnectionRefusedError:
        emit error("Connected Refused to "+mHostIp);
        break;
    case QAbstractSocket::RemoteHostClosedError:
        emit error("Server Went Down at "+mHostIp);
        break;
    default:
        emit error("Unknown Error");
        break;
    }

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
    mBuddies.clear();
    emit stateChanged();
    emit newMessage("Logged Out from Chat Server !","You");

}

void ClientConnection::onRead()
{
    Message readmessage(mSocket->readAll());
    switch (readmessage.type()) {
    case Message::Invalid:
        qDebug()<<"Unsupported Message Type";
        break;
    case Message::Online:
        mBuddies=readmessage.buddies();
        emit buddylist(readmessage.message());
        break;
    case Message::LogOff:
        //Log off Notification from Other Client, Lets update the latest buddies
        mBuddies=readmessage.buddies();
        emit buddylist(readmessage.message());
        break;
    case Message::Chat:
        if(readmessage.sender()==mName)
        {
            emit newMessage(readmessage.message(),"You");
        }
        else
        {
            emit newMessage(readmessage.message(),readmessage.sender());
        }
    default:
        break;
    }

}

void ClientConnection::send(QString &aData)
{
    if(!isActive())
    {
        qCritical()<<"No Active Connection to send Message";
        return;
    }
    Message newMessage(Message::Chat,mName,aData,QStringList());
    mSocket->write(newMessage.toByteArray());
}

ClientConnection::~ClientConnection()
{
    if(mSocket)
    {
        delete mSocket;
        mSocket=0;
    }
}

QString ClientConnection::getRandomName() const
{
   const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
   const int randomStringLength = 6; // assuming you want random strings of 12 characters

   QString randomString;
   for(int i=0; i<randomStringLength; ++i)
   {
       int index = qrand() % possibleCharacters.length();
       QChar nextChar = possibleCharacters.at(index);
       randomString.append(nextChar);
   }
   return QHostInfo::localHostName()+"#"+randomString;
}
