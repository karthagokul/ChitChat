#include <QCoreApplication>
#include "simulatorthread.h"
#include <QDebug>

const int max_connections=3; //Later on may be from Command line.

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug()<<"Simulator is starting up with "<<max_connections;
    //qSetMessagePattern("%{function}::%{type}->%{message}");
    QList<SimulatorThread*> conns;
    for(int i=0;i<max_connections;i++)
    {
        SimulatorThread *s=new SimulatorThread(&a);
        s->start();
        conns<<s;
    }
    int val=a.exec();
    Q_UNUSED(val);
    for(int i=0;i<max_connections;i++)
    {
        conns[i]->exit(0);
    }
    return 0;
}
