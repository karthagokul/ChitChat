#include "simulatorthread.h"
#include "clientconnection.h"
#include "sysutils.h"
#include <QDebug>
#include <QCoreApplication>

SimulatorThread::SimulatorThread(QObject *aParent):QThread(aParent)
  ,mEchoMessage(true)
{
    mCon=new ClientConnection(aParent);
    connect(mCon,SIGNAL(newMessage(QString,QString)),this,SLOT(onNewMessage(QString,QString)),Qt::DirectConnection);
    connect(mCon,SIGNAL(error(QString)),this,SLOT(onError(QString)),Qt::DirectConnection);
    mCon->setServer("127.0.0.1",8080); //Later on from command line parsing .
    mCon->setUserName(SysUtils::generateRandomName());
    mCon->start();
    if(mEchoMessage)
    {
        //randomString Generation
        mFortunes << tr("You've been leading a dog's life. Stay off the furniture.")
                 << tr("You've got to think about tomorrow.")
                 << tr("You will be surprised by a loud noise.")
                 << tr("You will feel hungry again in another hour.")
                 << tr("You might have mail.")
                 << tr("You cannot kill time without injuring eternity.")
                 << tr("Computers are not intelligent. They only think they are.");

        QTimer *timer=new QTimer(this);
        connect(timer,SIGNAL(timeout()),this,SLOT(onEchoMessage()));
        timer->start(6*1000);
    }
}

void SimulatorThread::onEchoMessage()
{
    mMutex.lock();
    QString data=mFortunes.at(qrand() % mFortunes.size());
    mCon->send(data);
    mMutex.unlock();
}

SimulatorThread::~SimulatorThread()
{
    //qDebug()<<"closing";
}
void SimulatorThread::onNewMessage(QString message,QString sender)
{
    qDebug()<<"["<<mCon->name()<<"]->"<<"["<<sender<<"] : "<<message;
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
