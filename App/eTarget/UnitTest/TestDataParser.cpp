#include "TestDataParser.h"
#include "../Bluetooth/DataParser.h"

void TestDataParser::parse_ValidData_ParserContainsAccordingPointObject()
{
//    Bluetooth::DataParser parser;
//    QString data("X123Y561");

//    QVERIFY(parser.parse(data));
//    QCOMPARE(parser.getParseResult(), QPoint(123, 561));
}

void TestDataParser::parse_InvalidData_ParserReturnFalse()
{
//    Bluetooth::DataParser parser;
//    QString data("X123561");

//    QVERIFY(!parser.parse(data));
}

int TestDataParser::execute(int argc, char* argv[])
{
    return QTest::qExec(this, argc, argv);
}
