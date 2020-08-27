#ifndef TESTSTOCKMODEL_H
#define TESTSTOCKMODEL_H

#include "TestClass.h"

class TestDataParser : public TestClass
{
    Q_OBJECT
private slots:
    void parse_ValidData_ParserContainsAccordingPointObject();
    void parse_InvalidData_ParserReturnFalse();
public:
    int execute(int argc, char* argv[]);
};

#endif // TESTSTOCKMODEL_H
