#include <QCoreApplication>
#include "serverapplication.h"
#include <QDebug>
#include <QSharedMemory>

int main(int argc, char *argv[])
{
    ServerApplication a(argc, argv);

    //Locking Mechanism to Ensure only One Instance of the Application.
    const char* MEM_KEY = "56";
    QSharedMemory sharedMem(MEM_KEY);
    if( !sharedMem.create( 512, QSharedMemory::ReadWrite) )
    {
      qDebug()<<"An Instance is already running , You are allowed to run only one Instance, Exiting !";
      exit(0);
    }

    //Looking good, Lets start!
    int val= a.start();
    Q_UNUSED(val);
    return 0;
}
