#include "chatroom.h"
#include "chatconnection.h"
#include "messagehandler.h"


ChatRoom::ChatRoom(QObject *aParent)
    :QObject(aParent)
{
    qRegisterMetaType< Message >("Message");
    qRegisterMetaType< qintptr >("qintptr");
}

void ChatRoom::onLogon(const Message &aMessage,const qintptr&sessionId)
{
    qDebug()<<"Got Logon ";
    Message onlinelist;
    onlinelist.type=Online;
    onlinelist.buddies=getBuddies(sessionId);
    broadcastMessage(onlinelist,-1);
}

void  ChatRoom::broadcastMessage(const Message &aMessage,const qintptr&sessionId)
{
    MessageHandler mHandler;
    QMapIterator<qintptr,ChatConnection*> i(mOnlineClients);
    while (i.hasNext()) {
        i.next();
        if(i.key()!=sessionId)
        {
            i.value()->write(mHandler.createMessage(aMessage));
        }
    }
}

void ChatRoom::onMessageRequest(const Message &aMessage,const qintptr&sessionId)
{
    broadcastMessage(aMessage,-1);
}

QStringList ChatRoom::getBuddies(const qintptr&sessionId)
{
    QStringList buddies;
    QMapIterator<qintptr,ChatConnection*> i(mOnlineClients);
    while (i.hasNext()) {
        i.next();
        //We do not need to send own id to him
        //disabled for development
        //if(i.key()!=socketSessionId)
        {
            buddies<<i.value()->name();
        }
    }
    qDebug()<<"Online Buddies"<<buddies;

    /*
     *

    */

    return buddies;
}

void ChatRoom::closeAllSessions()
{

}

void ChatRoom::onClientDisConnection(const qintptr&sessionId)
{
    //qDebug()<<"total clients"<<mOnlineClients.count();
    qDebug()<<sessionId<<" left Chatroom";
    //Lets Remove the Online Client from Chatroom
    QString signOffUser;
    QMapIterator<qintptr,ChatConnection*> i(mOnlineClients);
    while (i.hasNext()) {
        i.next();
        if(i.key()==sessionId)
        {
            qDebug()<<"Found one";
            signOffUser=i.value()->name();
            mOnlineClients.remove(i.key());
        }
    }

    //InformingOthers
    //Send a Custom Message which is visible on chat window
    Message signOffNotification;
    signOffNotification.type=Chat;
    signOffNotification.sender="Server Bot";
    signOffNotification.message=signOffUser+"Left Room !";
    qDebug()<<signOffUser+"Left Room !";
    broadcastMessage(signOffNotification,-1);

    //Update the client list
    Message onlinelist;
    onlinelist.type=Online;
    onlinelist.buddies=getBuddies(sessionId);
    broadcastMessage(onlinelist,-1);






    //qDebug()<<"total clients"<<mOnlineClients.count();
}

void ChatRoom::createNewSession(const qintptr socketSessionId)
{
    // Every new connection will be run in a newly created thread
    ChatConnection *newClient = new ChatConnection(socketSessionId, this);
    // connect signal/slot
    // once a thread is not needed, it will be beleted later
    connect(newClient, SIGNAL(finished()), newClient, SLOT(deleteLater()));
    connect(newClient,SIGNAL(disconnecting(qintptr)),this,SLOT(onClientDisConnection(qintptr)));
    connect(newClient,SIGNAL(loggedin(Message,qintptr)),this,SLOT(onLogon(Message,qintptr)));
    connect(newClient,SIGNAL(newmessage(Message,qintptr)),this,SLOT(onMessageRequest(Message,qintptr)));
    //Lets Insert to Online map
    mOnlineClients.insert(socketSessionId,newClient);
    newClient->start();

}
