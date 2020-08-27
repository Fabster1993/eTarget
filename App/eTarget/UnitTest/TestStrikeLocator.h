#ifndef TESTSTRIKELOCATOR_H
#define TESTSTRIKELOCATOR_H

#include "TestClass.h"

class TestStrikeLocator : public TestClass
{
    Q_OBJECT
private slots:
    void calculatePosition();
public:
    int execute(int argc, char* argv[]);
};

#endif // TESTSTRIKELOCATOR_H
