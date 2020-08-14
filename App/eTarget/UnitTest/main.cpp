#include "TestDataParser.h"

int main(int argc, char *argv[])
{
    quint16 status = 0;
    QList<QObject*> testObjects;

    testObjects.append(new TestDataParser());

    foreach(QObject* testObject, testObjects)
    {
        status |= QTest::qExec(testObject, argc, argv);
    }

    return status;
}
