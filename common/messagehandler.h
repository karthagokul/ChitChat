#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QStringList>
#include <QObject>

#define ONLINE "online"
#define LOGON "logon"
#define MESSAGE "message"

enum MessageType {Invalid=1,Chat,Mention,Online,LogOn};

struct Message
{
    MessageType type;
    QStringList buddies;
    QString message;
};


class MessageHandler
{
public:
    Message parseMessageFromClient(const QByteArray &aData);
    Message parseMessageFromServer(const QByteArray &aData);
    QByteArray createMessage(const Message &aMessage);

};

#endif // MESSAGEHANDLER_H
