#include <QCoreApplication>
#include "simulatorthread.h"

const int max_connections=10; //Later on may be from Command line.

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
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
