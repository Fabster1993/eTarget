#ifndef SERVICEDISCOVERYAGENT_H
#define SERVICEDISCOVERYAGENT_H

#include <QObject>
#include <QLowEnergyController>

class QBluetoothDeviceInfo;
class QBluetoothUuid;

namespace Bluetooth
{
    class DeviceDiscoveryAgent;

    class DeviceHandler : public QObject
    {
        Q_OBJECT
    public:
        explicit DeviceHandler(QObject *parent = nullptr);
        virtual ~DeviceHandler();
        void tryToConnect();

    private slots:
        void onDeviceOfInterestFound(const QBluetoothDeviceInfo& deviceInfo);
        void onServiceDiscovered(const QBluetoothUuid& gatt);
        void onServiceStateChanged(QLowEnergyService::ServiceState state);
        void onDeviceConnected();
        void onDeviceDisconnected();
        void onErrorOccured(QLowEnergyController::Error error);
        void onServiceError(QLowEnergyService::ServiceError newError);
        void onNewDataReceived(const QLowEnergyCharacteristic &c, const QByteArray &value);

    signals:

    public slots:
    private:
        QLowEnergyController* controller;
        QLowEnergyService* positionService;
        QLowEnergyDescriptor xPositionDescriptor;
        QLowEnergyDescriptor yPositionDescriptor;
        DeviceDiscoveryAgent* discoveryAgent;
    };
}

#endif // SERVICEDISCOVERYAGENT_H
