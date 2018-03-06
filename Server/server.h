#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QMap>
#include <QTcpServer>

class ChatConnection;

const int server_port=8080;

/*!
 * \brief The Server class
 */
class Server:public QTcpServer
{
    Q_OBJECT
public:
    Server(QObject *aParent);
    /*!
     * \brief init
     * \return the status of init operation
     */
    bool init();

    /*!
     * \brief ~Server
     */
    virtual ~Server();

    /*!
     * \brief setHostAddress
     * \param aHostAddress
     * \return
     */
    bool setHostAddress(const QString &aHostAddress)
    {
        mHostAddress=aHostAddress;
        return true;
    }

    /*!
     * \brief setPortNumber
     * \param aPortNumber
     * \return
     */
    bool setPortNumber(const int &aPortNumber)
    {
        mPort=aPortNumber;
        return true;
    }

private:
    QMap<QString,ChatConnection*> mOnlineClients;
    QString mHostAddress;
    int mPort;

};

#endif // SERVER_H
