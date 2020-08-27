#ifndef STRIKEINFORMATION_H
#define STRIKEINFORMATION_H

#include <QPoint>

namespace Business
{
    class StrikeInformation
    {
    public:
        StrikeInformation(const QPoint position, const quint16 radius);
        QPoint getPosition() const;
        quint16 getRadius() const;
        quint8 getScore() const;

    private:
        quint8 calculateScore();

        QPoint position;
        quint16 radius;
        quint8 score;
    };
}

#endif // STRIKEINFORMATION_H
