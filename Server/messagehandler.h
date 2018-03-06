#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QStringList>

enum MessageType {Chat,Mention,Status};

struct Message
{
    MessageType type;
    QStringList buddies;
    QString message;
};

class MessageHandler
{
public:
    Message createMessagefromNetworkData(const QByteArray &aData);
    QByteArray createNetworkDataFromMessage(const Message &aMessage);
};

#endif // MESSAGEHANDLER_H
