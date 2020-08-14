#ifndef TARGETINDICATOR_H
#define TARGETINDICATOR_H

#include <QObject>
#include <QPoint>

namespace Business
{
    class TargetIndicator : public QObject
    {
        Q_OBJECT
    public:
        explicit TargetIndicator(QObject *parent = nullptr);
        QPoint getStrikeAt(int row) const;
        int getStrikeCount() const;

    signals:
        void dataChanged();

    public slots:
        void onStrikeDetected(const QPoint& point);
    private:
        QList<QPoint> strikes;
    };
}

#endif // TARGETINDICATOR_H
