#include "TargetIndicator.h"

using namespace Business;

TargetIndicator::TargetIndicator(QObject *parent) : QObject(parent)
{
    strikes.append(QPoint(100, 100));
    strikes.append(QPoint(-100, 100));
    strikes.append(QPoint(-100, -100));
    strikes.append(QPoint(100, -100));
}

QPoint TargetIndicator::getStrikeAt(int row) const
{
    return strikes.at(row);
}

int TargetIndicator::getStrikeCount() const
{
    return strikes.length();
}

void TargetIndicator::onStrikeDetected(const QPoint& point)
{
    strikes.append(point);
    emit dataChanged();
}
