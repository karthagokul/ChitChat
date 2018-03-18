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
    /*!
     * \brief ChatRoom
     * \param aParent
     */
    ChatRoom(QObject *aParent);
    /*!
     * \brief registerSession
     * \param session
     */
    void registerSession(ChatSession *session);
    /*!
     * \brief closeAllSessions
     */
    void closeAllSessions();
private:
    /*!
     * \brief getBuddies
     * \param sessionId
     * \return
     */
    QStringList getBuddies(const QString &sessionId=QString());
    /*!
     * \brief broadcastMessage
     * \param aMessage
     * \param sessionId
     */
    void broadcastMessage(const Message &aMessage,const QString&sessionId=QString());

    /*!
     * \brief sendPrivateMessage
     * \param aMessage
     * \param sessionId
     */
    void sendPrivateMessage(const Message &aMessage,const QString&sessionId=QString());

protected slots:
    /*!
     * \brief onClientDisConnection
     * \param sessionId
     */
    void onClientDisConnection(const QString&sessionId);
    /*!
     * \brief onLogon
     * \param aMessage
     * \param sessionId
     */
    void onLogon(const Message &aMessage,const QString&sessionId);
    /*!
     * \brief onMessageRequest
     * \param aMessage
     * \param sessionId
     */
    void onMessageRequest(const Message &aMessage,const QString&sessionId);

    /*!
     * \brief sendLogOffMessage Adding a little delay for synchronisation
     */
    void sendLogOffMessage();
private:
    QMap<QString,ChatSession*> mOnlineClients;
};

#endif // CHATROOM_H
