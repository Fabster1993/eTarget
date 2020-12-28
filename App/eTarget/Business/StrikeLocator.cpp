#include "StrikeLocator.h"
#include <math.h>
#include <stdlib.h>

#include <QDebug>
#include <QTimer>

using namespace Business;

const QPoint SensorA(30, 0);
const QPoint SensorB(0, 30);
const QPoint SensorC(-30, 0);
const QPoint SensorD(0, -30);
const double Ax = SensorA.x();
const double By = SensorB.y();
const double Cx = SensorC.x();
const double Dy = SensorD.y();
const int maxC = 5000;

StrikeLocator::StrikeLocator()
{

}

QPoint StrikeLocator::calculatePosition(const double timeSensorA, const double timeSensorB, const double timeSensorC, const double timeSensorD)
{
    tA = timeSensorA;
    tB = timeSensorB;
    tC = timeSensorC;
    tD = timeSensorD;

    double nominator = (2 * (Ax * (By * (tC - tD) + Dy * (tB - tC)) - (By * (tA - tD) - Dy * (tA - tB)) * Cx));

    double pxk = (Ax * Ax * (By * (tC - tD) + Dy * (tB - tC)) - By * By * Dy * (tA - tC) - By * (Cx * Cx * (tA - tD) - Dy * Dy * (tA -tC)) + Cx * Cx * Dy * (tA - tB)) /
                 nominator;

    double pxc = ((By * (tA - tD) * (tC - tD) + Dy * (tA - tB) * (tB -tC)) * (tC - tA)) /
                 nominator;

    double pyk = (Ax * Ax * Cx * (tB - tD) + Ax * (By * By * (tC - tD) - Cx * Cx * (tB - tD) + Dy * Dy * (tB - tC)) - (By * By * (tA - tD) - Dy * Dy * (tA - tB)) * Cx) /
                 nominator;
    double pyc = (Ax * (tB - tC) * (tC - tD) + Cx * (tA * tA - tA * (tB + tD) + tB * tD)) * (tD - tB) /
                 nominator;

    double t0c = (Ax * (By * (tC * tC - tD * tD) + Dy * (tB * tB - tC * tC)) - (By * (tA * tA - tD * tD) - Dy * (tA * tA - tB * tB)) * Cx) /
                 nominator;

    double t0k = ((Ax - Cx) * (Ax * Cx - By * Dy) * (By - Dy)) /
                 nominator;

    double finalC = 0;
    double error = 100000;

    for(int index = 1; index < maxC; ++index)
    {
        double c = static_cast<double>(index) / 1000.0;
        double currentError = std::abs((((pxk + pxc * c * c) - Ax) * ((pxk + pxc * c * c) - Ax) + (pyk + pyc * c * c) * (pyk + pyc * c * c)) - ((c * (tA - (t0c + t0k / (c * c)))) * (c * (tA - (t0c + t0k / (c * c))))));
        if(currentError < error)
        {
            error = currentError;
            finalC = c;
        }
    }

    double c = finalC / 2;
    qDebug() << "C: " << c;

    double Px = pxk + pxc * c * c;
    double Py = pyk + pyc * c * c;

    qDebug() << "Px: " << Px << " Py: " << Py;

    return QPoint();
}
