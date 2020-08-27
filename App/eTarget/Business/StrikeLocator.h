#ifndef STRIKELOCATOR_H
#define STRIKELOCATOR_H

#include <QPoint>

namespace Business
{
    class StrikeLocator
    {
    public:
        StrikeLocator();
        inline double function(double Px, double Py, double t0);
        QPoint calculatePosition(const int timeSensorA, const int timeSensorB, const int timeSensorC);
    private:
        double tA;
        double tB;
        double tC;
    };
}

#endif // STRIKELOCATOR_H
