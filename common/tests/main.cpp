
#include <QtTest/QtTest>
#include "discoveryhandler.h"
#include "messagehandler.h"

class UTDiscoveryManager:public DiscoveryManager
{
    Q_OBJECT
public slots:
    void testParseForSearch_EmptyString()
    {
        qDebug()<<Q_FUNC_INFO;
        QString data;
        QSignalSpy spy(this, SIGNAL (search()));
        DiscoveryManager::parseRequest(data);
        QVERIFY(spy.count()== 0);
    }

    void testParseForSearch_ValidString()
    {
        qDebug()<<Q_FUNC_INFO;
        QString data(SEARCH_QUERY_STRING);
        QSignalSpy spy(this, SIGNAL (search()));
        DiscoveryManager::parseRequest(data);
        QVERIFY(spy.count()== 1);
    }

    void testParseForSearch_InValidString()
    {
        qDebug()<<Q_FUNC_INFO;
        QString data("INVALID_STRING");
        QSignalSpy spy(this, SIGNAL (search()));
        DiscoveryManager::parseRequest(data);
        QVERIFY(spy.count()== 0);
    }

    void testParseForResults_EmptyString()
    {
        qDebug()<<Q_FUNC_INFO;
        QString data;
        QSignalSpy spy(this, SIGNAL (search()));
        DiscoveryManager::parseRequest(data);
        QVERIFY(spy.count()== 0);
    }

    void testParseForResults_ValidString()
    {
        qDebug()<<Q_FUNC_INFO;
        QString data(SEARCH_RESULT_SUBSTRING+QString("192.168.100.3:8080"));
        QSignalSpy spy(this, SIGNAL (serverinfo(QString,int)));
        DiscoveryManager::parseRequest(data);
        QVERIFY(spy.count()== 1);
    }

    void testParseForResults_InValidString()
    {
        qDebug()<<Q_FUNC_INFO;
        //case 0
        QString data(SEARCH_RESULT_SUBSTRING+QString("192.168.100.3#8080"));
        QSignalSpy spy(this, SIGNAL (serverinfo(QString,int)));
        DiscoveryManager::parseRequest(data);
        QVERIFY(spy.count()== 0);
        spy.clear();

        //case 1
        data=QString(SEARCH_RESULT_SUBSTRING+QString(":8080"));
        DiscoveryManager::parseRequest(data);
        QVERIFY(spy.count()== 0);
        spy.clear();

        //case 2
        data=QString(SEARCH_RESULT_SUBSTRING+QString("192.168.100.3:"));
        DiscoveryManager::parseRequest(data);
        QVERIFY(spy.count()== 0);
    }
};

class UTMessage:public Message
{
    Q_OBJECT
public slots:
    void testConstruction()
    {
        qDebug()<<Q_FUNC_INFO;
        QString data;
        //case 0
        //Empty String
        Message m0(data.toLocal8Bit());
        QVERIFY(m0.type()==Message::Invalid);

        //case 1
        //Empty Json String
        data=QString("{}");
        Message m1(data.toLocal8Bit());
        QVERIFY(m1.type()==Message::Invalid);

        //case 2
        //Valid String
        data=QString("{\"command\":\"logon\",\"sender\":\"Gokul\"}");
        Message m2(data.toLocal8Bit());
        QVERIFY(m2.type()==Message::LogOn);
        QVERIFY(m2.sender()==QString("Gokul"));

        //case 3
        //Valid String with a invalid message parameter
        data=QString("{\"command\":\"logon\",\"sender\":\"Gokul\",\"message\":\"hello\"}");
        Message m3(data.toLocal8Bit());
        QVERIFY(m3.type()==Message::LogOn);
        QVERIFY(m3.sender()==QString("Gokul"));
        QVERIFY(m3.message()==QString());

        //case 4
        //Valid Chat Case
        data=QString("{\"command\":\"chat\",\"sender\":\"Kartha\",\"message\":\"hello\"}");
        Message m4(data.toLocal8Bit());
        QVERIFY(m4.type()==Message::Chat);
        QVERIFY(m4.sender()==QString("Kartha"));
        QVERIFY(m4.message()==QString("hello"));

        //case 5
        //Valid Chat Case invalid Message
        data=QString("{\"command\":\"chat\",\"whois\":\"IceCreamMan\",\"message\":\"Do you Want Icecreams?\"}");
        Message m5(data.toLocal8Bit());
        QVERIFY(m5.type()==Message::Invalid);
    }
};

class UTCommon:public QObject
{
    Q_OBJECT
private slots:
    void runAll()
    {
        UTMessage m;
        m.testConstruction();

        UTDiscoveryManager d;
        d.testParseForSearch_EmptyString();
        d.testParseForSearch_InValidString();
        d.testParseForSearch_ValidString();
        d.testParseForResults_EmptyString();
        d.testParseForResults_InValidString();
        d.testParseForResults_ValidString();
    }
};

QTEST_MAIN(UTCommon)

#include "main.moc"
