#include "TestStrikeLocator.h"
#include "Business/StrikeLocator.h"
#include <QElapsedTimer>

void TestStrikeLocator::calculatePosition()
{
    Business::StrikeLocator strikeLocator;
    QElapsedTimer timer;
    timer.start();
    strikeLocator.calculatePosition(0, 18, 29);
    qDebug() << timer.elapsed();
}

int TestStrikeLocator::execute(int argc, char* argv[])
{
    return QTest::qExec(this, argc, argv);
}
