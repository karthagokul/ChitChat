#include "simulatorthread.h"
#include "clientconnection.h"
#include "sysutils.h"
#include <QDebug>
#include <QCoreApplication>
#include <QDateTime>

SimulatorThread::SimulatorThread(QObject *aParent):QThread(aParent)
{
    mCon=new ClientConnection(aParent);
    connect(mCon,SIGNAL(newMessage(QString,QString)),this,SLOT(onNewMessage(QString,QString)),Qt::DirectConnection);
    connect(mCon,SIGNAL(error(QString)),this,SLOT(onError(QString)),Qt::DirectConnection);
    //mCon->setServer("192.168.178.66",8080); //Later on from command line parsing .
    mCon->setServer("127.0.0.1",8080); //Later on from command line parsing .
    mCon->setUserName(SysUtils::generateRandomName());
    mCon->connectNow();
   // mTimer=new QTimer(this);
    //connect(mTimer,SIGNAL(timeout()),this,SLOT(onEchoMessage()));
    //mTimer->start(4000);
    //mCon->start();
}

void SimulatorThread::onEchoMessage()
{
    qsrand(static_cast<quint64>(QTime::currentTime().msecsSinceStartOfDay()));
    mMutex.lock();
    QString data=mFortunes.at(qrand() % mFortunes.size());
    mCon->send(data);
    runMe=true;
    mMutex.unlock();
}

SimulatorThread::~SimulatorThread()
{
   mCon->disconnectFromServer();
   mMutex.lock();
   runMe=false;
   mMutex.unlock();
   mCon->terminate();
}

void SimulatorThread::onNewMessage(QString message,QString sender)
{
    qDebug()<<"["<<mCon->name()<<"]->"<<"["<<sender<<"] : "<<message;
}

void SimulatorThread::run()
{
    while(runMe)
    {
        sleep(10);
        onEchoMessage();
    }
}

void SimulatorThread::onError(QString aMessage)
{
    qCritical()<<aMessage;
    //mTimer->stop();
}
