#include "TestStrikeInformation.h"
#include "Business/StrikeInformation.h"

void TestStrikeInformation::instatiateStrikeInformation_ScoreIsCalculatedCorrectly()
{
    QFETCH(QPoint, strikePoint);
    QFETCH(quint8, strikeScore);

    Business::StrikeInformation strikeInfo(strikePoint, 0);
    QCOMPARE(strikeInfo.getScore(), strikeScore);
}

void TestStrikeInformation::instatiateStrikeInformation_ScoreIsCalculatedCorrectly_data()
{
    QTest::addColumn<QPoint>("strikePoint");
    QTest::addColumn<quint8>("strikeScore");

    QTest::newRow("X: 10 Y: 0") << QPoint(10, 0) << quint8(9);
    QTest::newRow("X: 9 Y: 0") << QPoint(9, 0) << quint8(10);
    QTest::newRow("X: 0 Y: 20") << QPoint(0, 20) << quint8(8);
    QTest::newRow("X: -30 Y: 0") << QPoint(-30, 0) << quint8(7);
    QTest::newRow("X: 50 Y: 50") << QPoint(50, 50) << quint8(3);
}

int TestStrikeInformation::execute(int argc, char* argv[])
{
    return QTest::qExec(this, argc, argv);
}
