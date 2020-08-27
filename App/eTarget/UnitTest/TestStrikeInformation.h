#ifndef TESTSTRIKEINFORMATION_H
#define TESTSTRIKEINFORMATION_H

#include "TestClass.h"

class TestStrikeInformation : public TestClass
{
    Q_OBJECT
private slots:
    void instatiateStrikeInformation_ScoreIsCalculatedCorrectly();
    void instatiateStrikeInformation_ScoreIsCalculatedCorrectly_data();
public:
    int execute(int argc, char* argv[]);
};

#endif // TESTSTRIKEINFORMATION_H
