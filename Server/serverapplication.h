#ifndef SERVERAPPLICATION_H
#define SERVERAPPLICATION_H

#include <QCoreApplication>
#include <QPointer>

class Server;
class ServerApplication:public QCoreApplication
{
    Q_OBJECT
public:
    ServerApplication(int argc,char **argv);
    int start();
protected slots:
    void onExit();

private:
    QPointer<Server> mServerInstance;
};

#endif // SERVERAPPLICATION_H
