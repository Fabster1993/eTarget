#include "DeviceHandler.h"
#include <QLowEnergyController>
#include "DeviceDiscoveryAgent.h"

const QString positionServiceUuid = "{1bc5d5a5-0200-b49a-e111-010000000000}";
const QString xPositionUuid = "{1bc5d5a5-0200-36ac-e111-010000000000}";
const QString yPositionUuid = "{1bc5d5a5-0200-36ac-e111-01000000E000}";

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
    if(state == QLowEnergyService::ServiceDiscovered)
    {
        QList<QLowEnergyCharacteristic> characteristics = positionService->characteristics();
        if(characteristics.length() < 2)
        {
            qDebug() << "There are less than 2 characteristics in service";
            return;
        }
        for(int index = 0; index < characteristics.length(); ++index)
        {
            qDebug() << characteristics.at(index).properties();
        }
//        xPositionDescriptor = characteristics.at(0).descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
//        if (xPositionDescriptor.isValid())
//        {
//            qDebug() << "X Position Descriptor not valid";
//        }
//        qDebug() << xPositionDescriptor.uuid();
//        positionService->writeDescriptor(xPositionDescriptor, QByteArray::fromHex("0100"));
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

void DeviceHandler::onNewDataReceived(const QLowEnergyCharacteristic& c, const QByteArray& value)
{
    qDebug() << "Received data: " << value;
}
