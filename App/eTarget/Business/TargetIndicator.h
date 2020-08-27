#ifndef TARGETINDICATOR_H
#define TARGETINDICATOR_H

#include <QObject>
#include <QPoint>
#include "StrikeInformation.h"

#include <QTimer> // temporary
#include <QRandomGenerator> // temporary

namespace Business
{
    class TargetIndicator : public QObject
    {
        Q_OBJECT
    public:
        explicit TargetIndicator(QObject *parent = nullptr);
        const StrikeInformation& getStrikeAt(int row) const;
        int getStrikeCount() const;

    signals:
        void dataChanged();

    public slots:
        void onStrikeDetected(const QPoint& position, const quint16 radius);
    private:
        QList<StrikeInformation> strikes;

        QTimer timer; // temporary
        QRandomGenerator generator; // temporary
    };
}

#endif // TARGETINDICATOR_H
