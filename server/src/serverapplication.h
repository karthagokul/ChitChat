#ifndef SERVERAPPLICATION_H
#define SERVERAPPLICATION_H

#include <QCoreApplication>
#include <QPointer>

class Server;

/*!
 * \brief The ServerApplication class
 */
class ServerApplication:public QCoreApplication
{
    Q_OBJECT
public:
    /*!
     * \brief ServerApplication Constructor
     * \param argc Command line args
     * \param argv Command line args
     */
    ServerApplication(int argc,char **argv);
    ~ServerApplication();
    /*!
     * \brief start Application Server Starts
     * \return Exit Value
     */
    int start();
protected slots:
    /*!
     * \brief onExit
     */
    void onExit();

private:
    QPointer<Server> mServerInstance;
};

#endif // SERVERAPPLICATION_H
