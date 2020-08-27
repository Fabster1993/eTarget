#ifndef TESTCLASS_H
#define TESTCLASS_H

#include <QtTest/QtTest>

class TestClass : public QObject
{
    Q_OBJECT
public:
    virtual int execute(int argc, char* argv[]) = 0;
};

#endif // TESTCLASS_H
