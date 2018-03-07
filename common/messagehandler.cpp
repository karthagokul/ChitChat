#include "messagehandler.h"

#include <QDebug>

Message::Message(MessageType aType,QString aSender,QString aMessage,QStringList aBuddies)
    :mType(aType),mBuddies(aBuddies),mSender(aSender),mMessage(aMessage)
{
}

Message::Message()
{
    mType=Message::Invalid;
    mBuddies=QStringList();
    mMessage=QString();
    mSender=QString();
}

Message::Message(const Message &aMessage)
{
    this->mType=aMessage.type();
    this->mMessage=aMessage.message();
    this->mSender=aMessage.sender();
    this->mBuddies=aMessage.buddies();
}

bool Message::jsonObjectToStringList(QJsonObject &aObject,QString aID,QStringList &aStringList)
{
    if(!aObject.contains(aID))
    {
        return false;
    }
    //Clear Existing List If Any
    aStringList.clear();
    QVariantList varList=aObject[aID].toArray().toVariantList();
    for (QVariantList::iterator j = varList.begin(); j != varList.end(); j++)
    {
        aStringList<< (*j).toString();
    }
    return true;
}

bool Message::jsonObjectToString(QJsonObject &aObject,QString aID,QString &aString)
{
    if(!aObject.contains(aID))
    {
        return false;
    }
    aString=aObject[aID].toString();
    return true;
}

Message::Message(const QByteArray &aData)
{
    QJsonDocument d = QJsonDocument::fromBinaryData(aData);
    if(d.isNull())
    {
        qCritical()<<"Unable to Parse the Protocol";
        mType=Invalid;
    }
    QJsonObject dataObject = d.object();
    QString command;
    if(!jsonObjectToString(dataObject,COMMAND,command))
    {
        qCritical()<<"Unable to Parse the Protocol";
        mType=Invalid;
    }

    if(command==COMMAND_LOGON)
    {
        mType=LogOn;
        if(!jsonObjectToString(dataObject,SENDER,mSender))
        {
            qCritical()<<COMMAND_CHAT<<":Parse Error";
            mType=Invalid;
        }
    }
    else if(command==COMMAND_CHAT)
    {
        mType=Chat;
        if(!jsonObjectToString(dataObject,MESSAGE,mMessage)||\
                !jsonObjectToString(dataObject,SENDER,mSender))
        {
            qCritical()<<COMMAND_CHAT<<":Parse Error";
            mType=Invalid;
        }
    }
    else if(command==COMMAND_LOGOFF)
    {
        mType=LogOff;
        if(!jsonObjectToStringList(dataObject,BUDDIES,mBuddies))
        {
            qCritical()<<COMMAND_LOGOFF<<":Parse Error";
            mType=Invalid;
        }
        jsonObjectToString(dataObject,MESSAGE,mMessage);
    }
    else if(command==COMMAND_MENTION)
    {
        mType=Mention;
        if(!jsonObjectToStringList(dataObject,BUDDIES,mBuddies)|| \
                !jsonObjectToString(dataObject,MESSAGE,mMessage)||\
                !jsonObjectToString(dataObject,SENDER,mSender))
        {
            qCritical()<<COMMAND_MENTION<<":Parse Error";
            mType=Invalid;
        }
    }
    else if(command==COMMAND_ONLINE)
    {
        mType=Online;
        if(!jsonObjectToStringList(dataObject,BUDDIES,mBuddies))
        {
            qCritical()<<COMMAND_ONLINE<<":Parse Error";
            mType=Invalid;
        }
        jsonObjectToString(dataObject,MESSAGE,mMessage);
    }
    else
    {
        qCritical()<<"Unknown Protocol";
        mType=Invalid;
    }
}

QByteArray Message::toByteArray() const
{
    QJsonDocument d = QJsonDocument::fromJson(QString("{}").toUtf8());
    if(d.isNull())
    {
        qCritical()<<"System Error , Unable to create Message";
        return QByteArray();
    }
    QJsonObject rootobj;
    switch(mType)
    {
    case LogOn:
        rootobj[COMMAND]=COMMAND_LOGON;
        rootobj[SENDER]=mSender;
        rootobj[MESSAGE]=mMessage;
        break;
    case Chat:
        rootobj[COMMAND]=COMMAND_CHAT;
        rootobj[MESSAGE]=mMessage;
        rootobj[SENDER]=mSender;
        break;
    case Mention:
        rootobj[COMMAND]=COMMAND_MENTION;
        rootobj[MESSAGE]=mMessage;
        rootobj[SENDER]=mSender;
        rootobj[BUDDIES]=QJsonArray::fromStringList(mBuddies);
        break;
    case Online:
        rootobj[COMMAND]=COMMAND_ONLINE;
        rootobj[MESSAGE]=mMessage; //Optional Message , like a reason
        rootobj[SENDER]=mSender;
        rootobj[BUDDIES]=QJsonArray::fromStringList(mBuddies);
        break;
    case LogOff:
        rootobj[COMMAND]=COMMAND_LOGOFF;
        rootobj[SENDER]=mSender;
        rootobj[MESSAGE]=mMessage;
        rootobj[BUDDIES]=QJsonArray::fromStringList(mBuddies);
        break;
    default: //Whatever else should be rejected
        //Empty case
        qCritical()<<"Invalid Protocol! You should not see this message !";
        return QByteArray();
        break;
    }
    d.setObject(rootobj);
    return d.toBinaryData();
}
