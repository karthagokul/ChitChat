#include "sysutils.h"
#include <QHostInfo>
#include <QDateTime>

QString SysUtils::getUserName()
{
    QString name = qgetenv("USER");
    if (name.isEmpty())
        name = qgetenv("USERNAME");
    return name;
}

QString SysUtils::generateRandomName()
{
    qsrand(static_cast<quint64>(QTime::currentTime().msecsSinceStartOfDay()));
    //randomString Generation
    QStringList names;
    names<<  QString("John")
          <<  QString("Thomas")
           <<  QString("Salvatore")
            <<  QString("Ahmed")
             <<  QString("Williams")
              << QString("Adney")
              << QString("Lam");
    return names.at(qrand() % names.size());
}
