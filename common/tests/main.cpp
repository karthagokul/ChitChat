
#include <QtTest/QtTest>
#include "discoveryhandler.h"

class UTDiscoveryManager:public DiscoveryManager
{
    Q_OBJECT
 private slots:
    void testParseForSearch()
    {
        QString data;
        QSignalSpy spy(this, SIGNAL (search()));
        DiscoveryManager::parseRequest(data);
        QCOMPARE (spy.count(), 0);
        spy.clear();
        data=SEARCH_QUERY_STRING;
        DiscoveryManager::parseRequest(data);
        QCOMPARE (spy.count(), 1);
    }

    void testParseForResults()
    {
        QString data;
        QSignalSpy spy(this, SIGNAL (search()));
        DiscoveryManager::parseRequest(data);
        QCOMPARE (spy.count(), 0);
        spy.clear();
        data=SEARCH_QUERY_STRING;
        DiscoveryManager::parseRequest(data);
        QCOMPARE (spy.count(), 1);
    }

};



QTEST_MAIN(UTDiscoveryManager)

#include "main.moc"
