#include "DataParser.h"
#include <QStringList>
#include <QDebug>

using namespace Bluetooth;

DataParser::DataParser() 
{

}

bool DataParser::parse(const QString& data)
{
    QStringList list = data.split(QLatin1Char('Y'));
    if(list.length() != 2)
    {
        qWarning() << "Parsing not possible due to invalid data";
        return false;
    }
    QString xPosition = list.first().remove("X");
    parseResult.first.setX(xPosition.toInt());
    QString yPosition = list.at(1);
    parseResult.first.setY(yPosition.toInt());
    return true;
}

QPair<QPoint, quint16> DataParser::getParseResult() const
{
    return parseResult;
}
