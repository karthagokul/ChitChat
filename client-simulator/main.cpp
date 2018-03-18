#include "simulatorapp.h"

#include <QDebug>
#include <QTimer>

int main(int argc, char *argv[])
{
    SimulatorApp a(argc, argv);
    a.setMaxConnections(1);
    a.start();
    int val=a.exec();
    Q_UNUSED(val);

    return 0;
}
