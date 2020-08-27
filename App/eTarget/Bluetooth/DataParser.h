#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <QPoint>
#include <QPair>

namespace Bluetooth 
{
    class DataParser
    {
    public:
        DataParser();
        bool parse(const QString& data);
        QPair<QPoint, quint16> getParseResult() const;
        
    private:
        QPair<QPoint, quint16> parseResult;
    };
}

#endif // DATAPARSER_H
