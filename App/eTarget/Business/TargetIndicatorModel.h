#ifndef TARGETINDICATORMODEL_H
#define TARGETINDICATORMODEL_H

#include <QAbstractListModel>

namespace Business
{
    class TargetIndicator;

    class TargetIndicatorModel : public QAbstractListModel
    {
        Q_OBJECT
    public:
        enum
        {
            xPositionRole = Qt::UserRole,
            yPositionRole,
            quadrantRole
        };
        explicit TargetIndicatorModel(TargetIndicator& targetIndicator);

    signals:

    public slots:
    public:
        int rowCount(const QModelIndex& parent) const;
        QVariant data(const QModelIndex& index, int role) const;
        QHash<int, QByteArray> roleNames() const;
    private:
        TargetIndicator& targetIndicator;
        int pointToQuadrant(const QPoint& point) const;
    };
}

#endif // TARGETINDICATORMODEL_H
