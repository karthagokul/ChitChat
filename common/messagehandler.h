#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QStringList>
#include <QObject>

/* Various Types of Commands to Operate between Server and Client*/
#define COMMAND "command"
#define COMMAND_LOGON "logon"
#define COMMAND_ONLINE "online"
#define COMMAND_CHAT "chat"
#define COMMAND_MENTION "mention"
#define COMMAND_LOGOFF "logff"

#define VERSION "version"
#define ONLINE "online"
#define MESSAGE "message"
#define SENDER "sender"

class Message
{
public:
    enum MessageType{Invalid=1,Chat,Mention,Online,LogOn,LogOff};
private:
    MessageType mType;
    QStringList mBuddies;
    QString mSender;
    QString mMessage;
    Message();
    Message(const Message &aMessage);
public:
    Message(MessageType aType,QString aSender,QString aMessage,QStringList aBuddies);
    Message(const QByteArray &aData);
    QByteArray toByteArray();

public:
    QStringList buddies() const
    {
        return mBuddies;
    }
    MessageType type() const
    {
        return mType;
    }
    QString message() const
    {
        return mMessage;
    }
    QString sender() const
    {
        return mSender;
    }
};


/*
 * Gokul's ChitChat Protocol
Login: FromClient
{command:"logon","version": 1.0 }

Buddies: FromServer
{command:"online","online": ["Thomas","Salvatore","Farhan"],"version": 1.0 }

Message Broadcast: Client to Server
{command:"chat",sender:"Gokul",message: "Welcome"}

Message to Specific Users
{command:"mention",sender:"Gokul",message: "Welcome","mention":["Thomas","Salvatore","Farhan"]}

Message Broadcast: Client to Server
{command:"logoff",sender:"Gokul",message: "Bye"}

*/


#endif // MESSAGEHANDLER_H
