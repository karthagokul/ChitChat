#include "serverapplication.h"
#include "server.h"
#include <QDebug>

#include <QSharedMemory>

//TODO : May be create a lock file to ensure its single existence
ServerApplication::ServerApplication(int argc,char **argv)
    :QCoreApplication(argc,argv)
{
    qSetMessagePattern("%{function}::%{type}->%{message}");
}
ServerApplication::~ServerApplication()
{
}

int ServerApplication::start()
{
    connect(this,SIGNAL(aboutToQuit()),this,SLOT(onExit()));

    mServerInstance=new Server(this);
    if(!mServerInstance->init())
    {
        qCritical()<<"Failed to initialise the server";
        return -1;
    }

    int result=QCoreApplication::exec();
    return result;
}

void ServerApplication::onExit()
{
    qDebug()<<"Exiting";
}
