#ifndef CHATCONNECTION_H
#define CHATCONNECTION_H

#include <QThread>

/*!
 * \brief The ChatConnection class
 */
class ChatConnection:public QThread
{
public:
    ChatConnection();
};

#endif // CHATCONNECTION_H
