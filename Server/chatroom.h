#ifndef CHATROOM_H
#define CHATROOM_H

#include <QObject>
#include <QMap>

class ChatConnection;

class ChatRoom :public QObject
{
    Q_OBJECT
public:
    ChatRoom(QObject *aParent);
    void createNewSession(qintptr socketSessionId);
    void closeAllSessions();

protected slots:
    void onClientDisConnection(const qintptr&sessionId);
private:
    QMap<qintptr,ChatConnection*> mOnlineClients;
};

#endif // CHATROOM_H
