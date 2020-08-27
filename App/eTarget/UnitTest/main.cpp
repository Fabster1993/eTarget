#include "TestDataParser.h"
#include "TestStrikeLocator.h"
#include "TestStrikeInformation.h"

int main(int argc, char *argv[])
{
    quint16 status = 0;
    QList<TestClass*> testObjects;

    testObjects.append(new TestDataParser());
    testObjects.append(new TestStrikeLocator());
    testObjects.append(new TestStrikeInformation());

    foreach(auto testObject, testObjects)
    {
        status |= testObject->execute(argc, argv);
    }

    return status;
}
