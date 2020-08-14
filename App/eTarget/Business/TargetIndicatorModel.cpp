#include "TargetIndicatorModel.h"
#include "TargetIndicator.h"

using namespace Business;

TargetIndicatorModel::TargetIndicatorModel(TargetIndicator& targetIndicator) :
    QAbstractListModel(),
    targetIndicator(targetIndicator)
{
    connect(&targetIndicator, &TargetIndicator::dataChanged, this,
    [this]()
    {
        beginInsertRows(QModelIndex(), this->targetIndicator.getStrikeCount() - 1, this->targetIndicator.getStrikeCount() - 1);
        endInsertRows();
    });
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

    switch(role)
    {
    case xPositionRole: return QVariant(targetIndicator.getStrikeAt(index.row()).x());
    case yPositionRole: return QVariant(targetIndicator.getStrikeAt(index.row()).y());
    case quadrantRole: return QVariant(pointToQuadrant(targetIndicator.getStrikeAt(index.row())));
    }
    return QVariant();
}

QHash<int, QByteArray> TargetIndicatorModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[xPositionRole] = "xPosition";
    names[yPositionRole] = "yPosition";
    names[quadrantRole] = "quadrant";
    return names;
}

int TargetIndicatorModel::pointToQuadrant(const QPoint& point) const
{
    if(point.x() >= 0 && point.y() >= 0)
        return 1;
    if(point.x() < 0 && point.y() >=0)
        return 2;
    if(point.x() < 0 && point.y() < 0)
        return 3;
    else // (point.x() >= 0 && point.y() < 0)
        return 4;
}
