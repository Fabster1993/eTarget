#include "TargetIndicatorModel.h"
#include "TargetIndicator.h"

using namespace Business;

TargetIndicatorModel::TargetIndicatorModel(TargetIndicator& targetIndicator) :
    QAbstractListModel(),
    targetIndicator(targetIndicator)
{
    connect(&targetIndicator, &TargetIndicator::dataChanged, this, &TargetIndicatorModel::onStrikeAdded);
}

int TargetIndicatorModel::rowCount(const QModelIndex& parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return targetIndicator.getStrikeCount();
}

QVariant TargetIndicatorModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const StrikeInformation& strike = targetIndicator.getStrikeAt(index.row());
    switch(role)
    {
    case xPositionRole: return QVariant(targetIndicator.getStrikeAt(index.row()).getPosition().x());
    case yPositionRole: return QVariant(strike.getPosition().y());
    case scoreRole: return QVariant(strike.getScore());
    case radiusRole: return QVariant(strike.getRadius());
    }
    return QVariant();
}

QHash<int, QByteArray> TargetIndicatorModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[xPositionRole] = "xPosition";
    names[yPositionRole] = "yPosition";
    names[scoreRole] = "score";
    names[radiusRole] = "radius";
    return names;
}

void TargetIndicatorModel::onStrikeAdded()
{
    const int row = 0;
    beginInsertRows(QModelIndex(), row, row);
    endInsertRows();
    emit strikeAdded(this->targetIndicator.getStrikeAt(row).getPosition().x(), this->targetIndicator.getStrikeAt(row).getPosition().y());
}
