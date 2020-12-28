#include "TargetIndicator.h"

using namespace Business;

TargetIndicator::TargetIndicator(QObject *parent) : QObject(parent)
{

}

const StrikeInformation& TargetIndicator::getStrikeAt(int row) const
{
    return strikes.at(row);
}

int TargetIndicator::getStrikeCount() const
{
    return strikes.length();
}

void TargetIndicator::onStrikeDetected(const QPoint& position, const quint16 radius)
{
    StrikeInformation detectedStrike(position, radius);
    strikes.prepend(detectedStrike);
    emit dataChanged();
}
