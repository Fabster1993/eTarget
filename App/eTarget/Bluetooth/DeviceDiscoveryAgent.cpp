#include "DeviceDiscoveryAgent.h"
#include <QBluetoothDeviceDiscoveryAgent>
#include <QDebug>

using namespace Bluetooth;

DeviceDiscoveryAgent::DeviceDiscoveryAgent(QObject *parent) :
    QObject(parent),
    deviceDiscoveryAgent(new QBluetoothDeviceDiscoveryAgent()),
    deviceFound(false)
{
    connect(deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &DeviceDiscoveryAgent::onScanFinished);
    connect(deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::canceled, this, &DeviceDiscoveryAgent::onScanFinished);
    connect(deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &DeviceDiscoveryAgent::onDeviceDiscovered);
}

DeviceDiscoveryAgent::~DeviceDiscoveryAgent()
{
    delete deviceDiscoveryAgent;
    deviceDiscoveryAgent = nullptr;
}

void DeviceDiscoveryAgent::start()
{
    deviceFound = false;
    deviceDiscoveryAgent->setLowEnergyDiscoveryTimeout(5000);
    deviceDiscoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
    qDebug() << "Start scanning Bluetooth devices";
}

void DeviceDiscoveryAgent::onDeviceDiscovered(const QBluetoothDeviceInfo& device)
{
    // If device is LowEnergy-device, add it to the list
    if (device.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration)
    {
        if(device.name() == "eTarget")
        {
            deviceOfInterest = device;
            deviceFound = true;
            qDebug() << "eTarget found with address" << device.address();
            emit deviceOfInterestFound(device);
        }
    }
}

void DeviceDiscoveryAgent::onScanFinished()
{
    if(!deviceFound)
    {
        emit cannotFindDeviceOfInterest();
        qDebug() << "Scan finished. eTarget was not found";
        start();
    }
}
