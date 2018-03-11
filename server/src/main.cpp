#include <QCoreApplication>
#include "serverapplication.h"

int main(int argc, char *argv[])
{
    ServerApplication a(argc, argv);
    return a.start();
}
