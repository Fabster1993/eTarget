#include "TargetIndicator.h"

using namespace Business;

TargetIndicator::TargetIndicator(QObject *parent) : QObject(parent)
{
    // temporary
    timer.start(500);
    connect(&timer, &QTimer::timeout, this,
    [this]()
    {
        QPoint position(generator.bounded(-50, 50), generator.bounded(-50, 50));
        onStrikeDetected(position, static_cast<quint16>(generator.bounded(0, 1000)));

    });
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
    strikes.append(detectedStrike);
    emit dataChanged();
}
