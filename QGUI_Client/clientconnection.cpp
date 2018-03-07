#include "clientconnection.h"
#include "messagehandler.h"

ClientConnection::ClientConnection(QObject *parent) :
    QObject(parent),mSocket(new QTcpSocket(this)),mHostIp("127.0.0.1"),mPort(8080),mActive(false),mName(getRandomName())
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
    Message loginmessage(Message::LogOn,mName,QString(),QStringList());
    mSocket->write(loginmessage.toByteArray());
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
    qDebug()<<"Reading";
    //QByteArray dataBytes = mSocket->readAll();
    //QString data=QString::fromStdString(dataBytes.toStdString());
    //qDebug()  << " Server Says: " << data;
    Message readmessage(mSocket->readAll());
    switch (readmessage.type()) {
    case Message::Invalid:
        qDebug()<<"Unsupported Message Type";
        break;
    case Message::Online:
        qDebug()<<"Got Buddies: " <<readmessage.buddies()<<":"<<readmessage.message();
        mBuddies=readmessage.buddies();
        emit buddylist(readmessage.message());
        break;
    case Message::LogOff:
        qDebug()<<"Got Buddies: " <<readmessage.buddies()<<":"<<readmessage.message();
        mBuddies=readmessage.buddies();
        emit buddylist(readmessage.message());
        break;
    case Message::Chat:
        qDebug()<<readmessage.sender()<<":"<<readmessage.message();
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
    qDebug()<<"Sending";
    //ToDo , Check if there are user mention, if so , do act
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
