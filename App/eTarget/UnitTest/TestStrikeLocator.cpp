#include "TestStrikeLocator.h"
#include "Business/StrikeLocator.h"
#include <QElapsedTimer>

void TestStrikeLocator::calculatePosition()
{
    Business::StrikeLocator strikeLocator;
    QElapsedTimer timer;
    timer.start();
    strikeLocator.calculatePosition(18.541, 0, 18.541, 30.00);
    strikeLocator.calculatePosition(9, 0, 9, 15);
    strikeLocator.calculatePosition(37, 0, 37, 60);
    strikeLocator.calculatePosition(0, 18.541, 30, 18.541);
    qDebug() << "Calculation took: " << timer.elapsed() << "ms";
}

int TestStrikeLocator::execute(int argc, char* argv[])
{
    return QTest::qExec(this, argc, argv);
}
