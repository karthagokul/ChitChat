#ifndef SIMULATORAPP_H
#define SIMULATORAPP_H

#include <QCoreApplication>
#include <QTimer>
#include "simulatorthread.h"

class SimulatorApp:public QCoreApplication
{
    Q_OBJECT
public:
    SimulatorApp(int argc,char **argv);
    void setMaxConnections(int aNumber)
    {
        mConns=aNumber;
    }
    void start();
private slots:
    void onTimeOut();
private:
    int mConns;
    QList<SimulatorThread*> mConnPool;
    QTimer *mTimer;
};

#endif // SIMULATORAPP_H
