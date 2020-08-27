#include "StrikeInformation.h"
#include <qmath.h>
#include <QDebug>

using namespace Business;

StrikeInformation::StrikeInformation(const QPoint point, const quint16 radius) :
    position(point),
    radius(radius)
{
    score = calculateScore();
}

quint8 StrikeInformation::getScore() const
{
    return score;
}

quint8 StrikeInformation::calculateScore()
{
    double relativeRadius = qSqrt(qPow(position.x(), 2) + qPow(position.y(), 2));
    if(relativeRadius > 100)
        qWarning() << "Relative radius should not exceed 100 but is " << relativeRadius;
    return static_cast<quint8>(qCeil(10 - relativeRadius / 10));
}

quint16 StrikeInformation::getRadius() const
{
    return radius;
}

QPoint StrikeInformation::getPosition() const
{
    return position;
}
