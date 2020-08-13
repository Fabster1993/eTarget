#ifndef BLUETOOTHINTERFACE_H
#define BLUETOOTHINTERFACE_H

#include <QObject>
#include <QBluetoothDeviceInfo>

class QBluetoothDeviceDiscoveryAgent;

namespace Bluetooth
{
    class DeviceDiscoveryAgent : public QObject
    {
        Q_OBJECT
    public:
        explicit DeviceDiscoveryAgent(QObject *parent = nullptr);
        virtual ~DeviceDiscoveryAgent();
        void start();

    signals:
        void deviceOfInterestFound(const QBluetoothDeviceInfo&);
        void cannotFindDeviceOfInterest();

    public slots:
    private:
        QBluetoothDeviceDiscoveryAgent* deviceDiscoveryAgent;
        QBluetoothDeviceInfo deviceOfInterest;
        bool deviceFound;
    private slots:
        void onDeviceDiscovered(const QBluetoothDeviceInfo& device);
        void onScanFinished();
    };
}

#endif // BLUETOOTHINTERFACE_H
