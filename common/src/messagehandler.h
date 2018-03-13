#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QStringList>
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/* Various Types of Commands to Operate between Server and Client*/
#define COMMAND "command"
#define COMMAND_LOGON "logon"
#define COMMAND_ONLINE "online"
#define COMMAND_CHAT "chat"
#define COMMAND_MENTION "mention"
#define COMMAND_LOGOFF "logff"

#define VERSION "version"
#define BUDDIES "buddies"
#define MESSAGE "message"
#define SENDER "sender"

/*!
 * \brief The Message class
 */
class Message:public QObject
{
public:
    /*!
     * \brief The MessageType enum
     */
    enum MessageType{Invalid=1,Chat,Mention,Online,LogOn,LogOff};
private:
    MessageType mType;
    QStringList mBuddies;
    QString mSender;
    QString mMessage;
public:
    Message();
    Message(const Message &aMessage);
    Message(MessageType aType,QString aSender,QString aMessage,QStringList aBuddies);
    Message(const QByteArray &aData);
    QByteArray toByteArray() const;

private:
    bool jsonObjectToStringList(QJsonObject &aObject,QString aID,QStringList &aStringList);
    bool jsonObjectToString(QJsonObject &aObject,QString aID,QString &aString);

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
{command:"logon",sender:"Gokul"}

Buddies: FromServer
{command:"online","buddies": ["Thomas","Salvatore","Farhan"],"version": 1.0 }

Message Broadcast: Client to Server
{command:"chat",sender:"Gokul",message: "Welcome"}

Message to Specific Users
{command:"mention",sender:"Gokul",message: "Welcome","buddies":["Thomas","Salvatore","Farhan"]}

Message Broadcast: Client to Server
{command:"logoff",sender:"Gokul",message: "Bye"}

*/

#endif // MESSAGEHANDLER_H
