#include "DeviceHandler.h"
#include <QLowEnergyController>
#include "DeviceDiscoveryAgent.h"

const QString positionServiceUuid = "{1bc5d5a5-0200-b49a-e111-010000000000}";
const QString PositionUuid = "{1bc5d5a5-0200-36ac-e111-010000000000}";

using namespace Bluetooth;

DeviceHandler::DeviceHandler(QObject *parent) : QObject(parent),
    controller(nullptr),
    positionService(nullptr),
    discoveryAgent(new DeviceDiscoveryAgent())
{
    connect(discoveryAgent, &DeviceDiscoveryAgent::deviceOfInterestFound, this, &DeviceHandler::onDeviceOfInterestFound);
}

DeviceHandler::~DeviceHandler()
{
    delete discoveryAgent;
    discoveryAgent = nullptr;
}

void DeviceHandler::tryToConnect()
{
    discoveryAgent->start();
}

void DeviceHandler::onDeviceOfInterestFound(const QBluetoothDeviceInfo& deviceInfo)
{
    qDebug() << "Is cached: " << deviceInfo.isCached();
    controller = QLowEnergyController::createCentral(deviceInfo, dynamic_cast<QObject*>(this));
    connect(controller, &QLowEnergyController::serviceDiscovered, this, &DeviceHandler::onServiceDiscovered);
    connect(controller, &QLowEnergyController::connected, this, &DeviceHandler::onDeviceConnected);
    connect(controller, static_cast<void (QLowEnergyController::*)(QLowEnergyController::Error)>(&QLowEnergyController::error), this, &DeviceHandler::onErrorOccured);
    connect(controller, &QLowEnergyController::disconnected, this, &DeviceHandler::onDeviceDisconnected);
    qDebug() << "Try to connect to device";
    controller->connectToDevice();
}

void DeviceHandler::onServiceDiscovered(const QBluetoothUuid& gatt)
{
    qDebug() << gatt;
    if(gatt.toString() == positionServiceUuid)
    {
        qDebug() << "Position service found";
        positionService = controller->createServiceObject(QBluetoothUuid(positionServiceUuid), dynamic_cast<QObject*>(this));
        connect(positionService, &QLowEnergyService::characteristicChanged, this, &DeviceHandler::onNewDataReceived);
        connect(positionService, &QLowEnergyService::stateChanged, this, &DeviceHandler::onServiceStateChanged);
        connect(positionService, static_cast<void (QLowEnergyService::*)(QLowEnergyService::ServiceError)>(&QLowEnergyService::error), this, &DeviceHandler::onServiceError);
        positionService->discoverDetails();
    }
}

void DeviceHandler::onServiceStateChanged(QLowEnergyService::ServiceState state)
{
    qDebug() << state;
    if (state == QLowEnergyService::ServiceDiscovered)
    {
        const QLowEnergyCharacteristic hrChar = positionService->characteristic(QBluetoothUuid(PositionUuid));
        if (!hrChar.isValid())
        {
            qDebug() << "invalid";
        }
        PositionDescriptor = hrChar.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
        if (PositionDescriptor.isValid())
            positionService->writeDescriptor(PositionDescriptor, QByteArray::fromHex("0100"));
        else
        {
            qDebug() << "invalid descriptor";
        }
    }
}

void DeviceHandler::onDeviceConnected()
{
    qDebug() << "Device connected";
    controller->discoverServices();
}

void DeviceHandler::onDeviceDisconnected()
{
    qDebug() << "Device disconnected";
    delete controller;
    controller = nullptr;

    delete positionService;
    positionService = nullptr;
}

void DeviceHandler::onErrorOccured(QLowEnergyController::Error error)
{
    if(error == QLowEnergyController::ConnectionError)
    {
        qDebug() << "Could not connect to device";
    }
    else
    {
        qDebug() << "Unknown error: " << error;
    }
}

void DeviceHandler::onServiceError(QLowEnergyService::ServiceError newError)
{
    qDebug() << newError;
}

void DeviceHandler::onNewDataReceived(const QLowEnergyCharacteristic& characteristic, const QByteArray& value)
{
    if(characteristic.uuid().toString() == PositionUuid)
    {
        qDebug() << "X Position:" << QString::fromStdString(value.toStdString());
    }
    qDebug() << "Received data: " << value.length();
}
