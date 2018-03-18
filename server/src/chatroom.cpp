#include "chatroom.h"
#include "chatsession.h"
#include "messagehandler.h"
#include <QTimer>

ChatRoom::ChatRoom(QObject *aParent)
    :QObject(aParent)
{
    qRegisterMetaType< Message >("Message");
    qRegisterMetaType< qintptr >("qintptr");
}

void ChatRoom::onLogon(const Message &aMessage,const QString&sessionId)
{
    Message onlinelist(Message::Online,QString(),aMessage.sender()+QString(" Logged in!"),getBuddies(sessionId));
    broadcastMessage(onlinelist,QString());
}

void  ChatRoom::broadcastMessage(const Message &aMessage,const QString&sessionId)
{
    QMapIterator<QString,ChatSession*> i(mOnlineClients);
    while (i.hasNext()) {
        i.next();
       // if(i.key()!=sessionId)
        {
            i.value()->write(aMessage.toByteArray());
        }
    }
}
void ChatRoom::sendPrivateMessage(const Message &aMessage,const QString&sessionId)
{
    //qDebug()<<"Private Message";
    QMapIterator<QString,ChatSession*> i(mOnlineClients);
    while (i.hasNext()) {
        i.next();
        if(i.value()->name() ==aMessage.sender() || aMessage.buddies().contains(i.value()->name()))
        {
            //qDebug()<<"Sending to "<<i.value()->name();
            i.value()->write(aMessage.toByteArray());
        }
    }
}
void ChatRoom::onMessageRequest(const Message &aMessage,const QString&sessionId)
{
    Q_UNUSED(sessionId);
   // qDebug()<<aMessage.buddies();
    if(aMessage.type()==Message::Mention)
    {
        sendPrivateMessage(aMessage,QString());
    }
    else
    {
        broadcastMessage(aMessage,QString());
    }

}

QStringList ChatRoom::getBuddies(const QString&sessionId)
{
    Q_UNUSED(sessionId);
    QStringList buddies;
    QMapIterator<QString,ChatSession*> i(mOnlineClients);
    while (i.hasNext()) {
        i.next();
        //We do not need to send own id to him
        //disabled for development
        //if(i.key()!=socketSessionId)
        {
            buddies<<i.value()->name();
        }
    }
    //qDebug()<<"Buddies"<<buddies;
    return buddies;
}

void ChatRoom::closeAllSessions()
{

}

void ChatRoom::onClientDisConnection(const QString&sessionId)
{
    //qDebug()<<"Disconncted";
    //Lets Remove the Online Client from Chatroom
    QString signOffUser;
    QMapIterator<QString,ChatSession*> i(mOnlineClients);
    while (i.hasNext()) {
        i.next();
        if(i.key()==sessionId)
        {
            signOffUser=i.value()->name();
            mOnlineClients.remove(i.key());
        }
    }

    Message logOffMessage(Message::LogOff,QString(),signOffUser+QString(" Left the Room"),\
                          getBuddies());
    broadcastMessage(logOffMessage);

    //Litle delay to Fix List of buddies bug .
    QTimer::singleShot(300, this, SLOT(sendLogOffMessage()));
}
void ChatRoom::sendLogOffMessage()
{
    Message onLineMessage(Message::Online,QString(),QString(),getBuddies());
    broadcastMessage(onLineMessage);
}

void ChatRoom::registerSession(ChatSession *session)
{
    ChatSession *newClient = session;
    // connect signal/slot
    // once a thread is not needed, it will be beleted later
    connect(newClient, SIGNAL(finished()), newClient, SLOT(deleteLater()));
    connect(newClient,SIGNAL(disconnecting(QString)),this,SLOT(onClientDisConnection(QString)));
    connect(newClient,SIGNAL(loggedin(Message,QString)),this,SLOT(onLogon(Message,QString)));
    connect(newClient,SIGNAL(newmessage(Message,QString)),this,SLOT(onMessageRequest(Message,QString)));
    //Lets Insert to Online map
    mOnlineClients.insert(newClient->id(),newClient);
    //qDebug()<<"Total Clients Online"<<mOnlineClients.count();
    newClient->start();
}
