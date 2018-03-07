#ifndef CHATROOM_H
#define CHATROOM_H

#include <QObject>
#include <QMap>
#include "messagehandler.h"
class ChatConnection;

/*!
 * \brief The ChatRoom class
 */
class ChatRoom :public QObject
{
    Q_OBJECT
public:
    ChatRoom(QObject *aParent);
    void createNewSession(const qintptr socketSessionId);
    void closeAllSessions();
private:
    QStringList getBuddies(const qintptr&sessionId);
protected slots:
    void onClientDisConnection(const qintptr&sessionId);
    void onLogon(const Message &aMessage,const qintptr&sessionId);
    void onMessageRequest(const Message &aMessage,const qintptr&sessionId);
private:
    void broadcastMessage(const Message &aMessage,const qintptr&sessionId=-1);
private:
    QMap<qintptr,ChatConnection*> mOnlineClients;
};

#endif // CHATROOM_H
