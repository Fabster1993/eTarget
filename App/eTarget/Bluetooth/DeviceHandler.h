#ifndef SERVICEDISCOVERYAGENT_H
#define SERVICEDISCOVERYAGENT_H

#include <QObject>
#include <QLowEnergyController>

class QBluetoothDeviceInfo;
class QBluetoothUuid;

namespace Bluetooth
{
    class DeviceDiscoveryAgent;
    class DataParser;

    class DeviceHandler : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(bool connected READ getConnected WRITE setConnected NOTIFY connectedChanged)
    public:
        explicit DeviceHandler(QObject *parent = nullptr);
        virtual ~DeviceHandler();
        bool getConnected() const { return connected; }
        void setConnected(bool connected);

    private slots:
        void onDeviceOfInterestFound(const QBluetoothDeviceInfo& deviceInfo);
        void onServiceDiscovered(const QBluetoothUuid& gatt);
        void onServiceStateChanged(QLowEnergyService::ServiceState state);
        void onControllerStateChanged(QLowEnergyController::ControllerState state);
        void onDeviceConnected();
        void onDeviceDisconnected();
        void onControllerError(QLowEnergyController::Error error);
        void onServiceError(QLowEnergyService::ServiceError newError);
        void onNewDataReceived(const QLowEnergyCharacteristic &characteristic, const QByteArray &value);

    signals:
        void connectedChanged();
        void strikeDetected(const QPoint& point);

    public slots:
        void tryToConnect();
    private:
        QLowEnergyController* controller;
        QLowEnergyService* positionService;
        QLowEnergyDescriptor PositionDescriptor;
        DeviceDiscoveryAgent* discoveryAgent;
        DataParser* dataParser;
        bool connected;
    };
}

#endif // SERVICEDISCOVERYAGENT_H
