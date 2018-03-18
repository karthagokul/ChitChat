#ifndef SIMULATORAPP_H
#define SIMULATORAPP_H

#include <QCoreApplication>
#include <QTimer>
#include "simulatorthread.h"
#define MAX_CONN 10

class SimulatorApp:public QCoreApplication
{
    Q_OBJECT
public:
    SimulatorApp(int argc,char **argv);
    void start();
private slots:
    void onTimeOut();

private:
    QStringList mData;
    int mConns;
    QList<SimulatorThread*> mConnPool;
    QTimer *mTimer;
};

#endif // SIMULATORAPP_H
