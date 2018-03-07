#ifndef CHATROOM_H
#define CHATROOM_H

#include <QObject>
#include <QMap>
#include "messagehandler.h"
class ChatSession;

/*!
 * \brief The ChatRoom class
 */
class ChatRoom :public QObject
{
    Q_OBJECT
public:
    ChatRoom(QObject *aParent);
    void registerSession(ChatSession *session);
    void closeAllSessions();
private:
    QStringList getBuddies(const QString &sessionId);
protected slots:
    void onClientDisConnection(const QString&sessionId);
    void onLogon(const Message &aMessage,const QString&sessionId);
    void onMessageRequest(const Message &aMessage,const QString&sessionId);
private:
    void broadcastMessage(const Message &aMessage,const QString&sessionId=QString());
private:
    QMap<QString,ChatSession*> mOnlineClients;
};

#endif // CHATROOM_H
