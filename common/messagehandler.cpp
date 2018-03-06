#include "messagehandler.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

/*
 *

 Login Server
 {
  "logon":[],
  "version": 1.0
 }
Buddy Information
{
  "online": [
    "Thomas",
    "Salvatore",
    "Farhan"
  ],
  "version": 1.0
}

Message from Server to all clients
{
   buddy:"Gokul"
   message: "Welcome"
}

Message to Server from Client
{
   message: "Welcome"
}

{
   buddy: "Thomas"
   message: "Welcome"
}

*/


Message MessageHandler::parseMessageFromClient(const QByteArray &aData)
{
    Message m;
    QJsonDocument d = QJsonDocument::fromBinaryData(aData);
    if(d.isNull())
    {
        m.type=Invalid;
        return m;
    }
    QJsonObject dataObject = d.object();
    if(dataObject.contains(LOGON))
    {
        m.type=LogOn;
        if(dataObject.contains(MESSAGE))
        {
            m.message=dataObject[MESSAGE].toString();
        }
        else
        {
            qCritical()<<"Wrong Format";
        }
    }
    else
    {
         m.type=Chat;
         if(dataObject.contains(MESSAGE))
         {
             m.message=dataObject[MESSAGE].toString();
             if(dataObject.contains(SENDER))
             {
                m.sender=dataObject[SENDER].toString();
             }
             else
             {
                 qCritical()<<"Invalid Protocol:Message Must be send from a user";
                 m.type=Invalid;
             }
             if(dataObject.contains(BUDDIES))
             {
                 //we need to change type to mention
                 //This message is for few specific ones
             }
             else
             {
                 //Broadcast
             }
         }
         else
         {
             qCritical()<<"Wrong Format";
             m.type=Invalid;
         }
    }
    return m;
}

Message MessageHandler::parseMessageFromServer(const QByteArray &aData)
{
    Message m;
    QJsonDocument d = QJsonDocument::fromBinaryData(aData);
    if(d.isNull())
    {
        m.type=Invalid;
        return m;
    }
    QJsonObject dataObject = d.object();
    if(dataObject.contains(ONLINE))
    {
        //qDebug()<<"Got Online Status";
        QJsonArray onlinearay=dataObject[ONLINE].toArray();
        QVariantList buddies=onlinearay.toVariantList();
        for (QVariantList::iterator j = buddies.begin(); j != buddies.end(); j++)
        {
            //qDebug() << "iterating through QVariantList ";
            //qDebug() << (*j).toString(); // Print QVariant
            m.buddies<< (*j).toString();
        }
        m.type=Online;
    }
    else if(dataObject.contains(MESSAGE))
    {
        m.type=Chat;
        m.message=dataObject[MESSAGE].toString();
        if(dataObject.contains(SENDER))
        {
           m.sender=dataObject[SENDER].toString();
        }
        else
        {
            qCritical()<<"Invalid Protocol:Message Must be send from a user";
            m.type=Invalid;
        }
    }
    return m;
}

QByteArray MessageHandler::createMessage(const Message &aMessage)
{
    QJsonDocument d = QJsonDocument::fromJson(QString("{}").toUtf8());
    if(d.isNull())
    {
       qCritical()<<"System Error , Unable to create Message";
       return QByteArray();
    }
    QJsonObject rootobj;
    if(aMessage.type==LogOn)
    {
        rootobj[LOGON]=QJsonArray();
        rootobj[MESSAGE]=aMessage.message;

    }
    else if(aMessage.type==Online)
    {
        rootobj[ONLINE]=QJsonArray::fromStringList(aMessage.buddies);
    }
    else if(aMessage.type==Chat)
    {
        rootobj[MESSAGE]=aMessage.message;
        rootobj[SENDER]=aMessage.sender;
    }
    d.setObject(rootobj);
    return d.toBinaryData();
}
