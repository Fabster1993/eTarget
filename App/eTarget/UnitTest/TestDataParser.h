#ifndef TESTSTOCKMODEL_H
#define TESTSTOCKMODEL_H

#include <QtTest/QtTest>

class TestDataParser : public QObject
{
    Q_OBJECT
private slots:
    void parse_ValidData_ParserContainsAccordingPointObject();
    void parse_InvalidData_ParserReturnFalse();


};

#endif // TESTSTOCKMODEL_H
