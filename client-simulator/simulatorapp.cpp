#include "simulatorapp.h"
#include <QDebug>

SimulatorApp::SimulatorApp(int argc,char **argv)
    :QCoreApplication(argc,argv),mConns(1)
{
    mTimer=new QTimer(this);
    connect(mTimer,SIGNAL(timeout()),this,SLOT(onTimeOut()));
}

void SimulatorApp::start()
{
    //So here Simulator will initialise and destroy connections in every 60 Seconds
    mTimer->start(60*1000);
    //for now one time call slot
    onTimeOut();
}

void SimulatorApp::onTimeOut()
{
    qDebug()<<"TimeOut";
    for(int i=0;i<mConnPool.count();i++)
    {
        mConnPool[i]->terminate();
        mConnPool[i]->deleteLater();
    }
    mConnPool.clear();
    for(int i=0;i<mConns;i++)
    {
        SimulatorThread *s=new SimulatorThread(this);
        s->start();
        mConnPool<<s;
    }

}
