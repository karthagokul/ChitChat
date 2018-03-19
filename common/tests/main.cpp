
#include <QtTest/QtTest>
#include "messagehandler.h"

class UTMessage:public Message
{
    Q_OBJECT
public slots:
    void testInvalidParameter()
    {
          QString data;
          data=QString("{\"command\":\"logon\",\"sender\":\"Gokul\",\"message\":\"hello\"}");
          Message m3(data.toLocal8Bit());
          QVERIFY(m3.type()==Message::LogOn);
          QVERIFY(m3.sender()==QString("Gokul"));
          QVERIFY(m3.message()==QString());
    }

    void testValidChatCase()
    {
          QString data;
          data=QString("{\"command\":\"chat\",\"sender\":\"Kartha\",\"message\":\"hello\"}");
          Message m4(data.toLocal8Bit());
          QVERIFY(m4.type()==Message::Chat);
          QVERIFY(m4.sender()==QString("Kartha"));
          QVERIFY(m4.message()==QString("hello"));
    }

    void testToByteArrayChat()
    {
         Message newMessage(Message::Chat,"Gokul","Hi",QStringList());
         QString result("{\n    \"command\": \"chat\",\n    \"message\": \"Hi\",\n    \"sender\": \"Gokul\"\n}\n");
         QVERIFY(result.toLocal8Bit()==newMessage.toByteArray());
    }

    void testEverythingElse()
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
        m.testEverythingElse();
        m.testInvalidParameter();
        m.testValidChatCase();
        m.testToByteArrayChat();
    }
};

QTEST_MAIN(UTCommon)

#include "main.moc"
