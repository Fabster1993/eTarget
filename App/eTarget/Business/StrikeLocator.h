#ifndef STRIKELOCATOR_H
#define STRIKELOCATOR_H

#include <QPoint>

namespace Business
{
    class StrikeLocator
    {
    public:
        StrikeLocator();
        QPoint calculatePosition(const double timeSensorA, const double timeSensorB, const double timeSensorC, const double timeSensorD);
    private:
        double tA;
        double tB;
        double tC;
        double tD;
    };
}

#endif // STRIKELOCATOR_H
