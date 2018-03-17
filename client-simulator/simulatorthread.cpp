#include "simulatorthread.h"
#include "clientconnection.h"
#include "sysutils.h"
#include <QDebug>
#include <QCoreApplication>

SimulatorThread::SimulatorThread(QObject *aParent):QThread(aParent)
{
    mCon=new ClientConnection(aParent);
    connect(mCon,SIGNAL(newMessage(QString,QString)),this,SLOT(onNewMessage(QString,QString)),Qt::DirectConnection);
    connect(mCon,SIGNAL(error(QString)),this,SLOT(onError(QString)),Qt::DirectConnection);
    mCon->setServer("127.0.0.1",8080); //Later on from command line parsing .
    mCon->setUserName(SysUtils::generateRandomName());
    mCon->start();
}
SimulatorThread::~SimulatorThread()
{
   //qDebug()<<"closing";
}
void SimulatorThread::onNewMessage(QString message,QString sender)
{
     qDebug()<<"["<<sender<<"] : "<<message;
}
void SimulatorThread::run()
{
    exec();
}
void SimulatorThread::onError(QString aMessage)
{
    qCritical()<<aMessage;
    qApp->quit();
}
