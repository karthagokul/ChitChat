#include "sysutils.h"
#include <QHostInfo>

QString SysUtils::getUserName()
{
    QString name = qgetenv("USER");
    if (name.isEmpty())
        name = qgetenv("USERNAME");
    //Still fail condition
    if(name.isEmpty()) //unable to get the username from Environment , Get a Random Name
    {
        name=SysUtils::generateRandomName();
    }
    return name;
}

QString SysUtils::generateRandomName()
{
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    const int randomStringLength = 6; // assuming you want random strings of 12 characters

    QString randomString;
    for(int i=0; i<randomStringLength; ++i)
    {
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
     return randomString;
}
