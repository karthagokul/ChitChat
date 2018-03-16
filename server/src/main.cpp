#include <QCoreApplication>
#include "serverapplication.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    ServerApplication a(argc, argv);
    int val= a.start();
    qDebug()<<val;
    return 0;
}
