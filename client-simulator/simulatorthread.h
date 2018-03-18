#ifndef SIMULATORTHREAD_H
#define SIMULATORTHREAD_H

#include <QThread>
#include <QStringList>
#include <QMutex>
#include <QTimer>

class ClientConnection;
class SimulatorThread:public QThread
{
    Q_OBJECT
private:
    ClientConnection *mCon;
    QStringList mFortunes;
    QMutex mMutex;
    //QTimer *mTimer;
    bool runMe;
public:
    SimulatorThread(QObject *aParent);
    virtual ~SimulatorThread();
    void setData(QStringList aForturnes)
    {
        mFortunes=aForturnes;
    }

protected:
    void run();
protected slots:
    void onNewMessage(QString message,QString sender);
    void onError(QString aMessage);
    void onEchoMessage();
};
#endif // SIMULATORTHREAD_H
