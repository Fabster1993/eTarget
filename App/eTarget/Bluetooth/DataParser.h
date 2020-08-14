#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <QPoint>

namespace Bluetooth 
{
    class DataParser
    {
    public:
        DataParser();
        bool parse(const QString& data);
        QPoint getParseResult() const;
        
    private:
        QPoint parseResult;
    };
}

#endif // DATAPARSER_H
