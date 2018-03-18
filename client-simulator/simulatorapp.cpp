#include "simulatorapp.h"
#include <QDebug>
#include <QTime>
#include <QFile>
SimulatorApp::SimulatorApp(int argc,char **argv)
    :QCoreApplication(argc,argv),mConns(1)
{
    QFile file(":/quotes.txt");
    if(file.open(QIODevice::ReadOnly))
    {
        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            mData<<QString(line);
        }
    }

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
    qsrand(static_cast<quint64>(QTime::currentTime().msecsSinceStartOfDay()));
    qDebug()<<"Please hold , Simulation is on the way";
    for(int i=0;i<mConnPool.count();i++)
    {
        mConnPool[i]->terminate();
        mConnPool[i]->deleteLater();
    }
    mConnPool.clear();
    mConns=qrand() % MAX_CONN;
    for(int i=0;i<mConns;i++)
    {
        SimulatorThread *s=new SimulatorThread(this);
        s->setData(mData);
        s->start();
        mConnPool<<s;
    }

}
