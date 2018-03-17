#ifndef SIMULATORTHREAD_H
#define SIMULATORTHREAD_H

#include <QThread>

class ClientConnection;
class SimulatorThread:public QThread
{
    Q_OBJECT
private:
    ClientConnection *mCon;
public:
    SimulatorThread(QObject *aParent);
    virtual ~SimulatorThread();
protected:
    void run();
protected slots:
    void onNewMessage(QString message,QString sender);
    void onError(QString aMessage);
};
#endif // SIMULATORTHREAD_H
