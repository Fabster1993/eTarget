/*
 * Bluetooth.h
 *
 *  Created on: 16.08.2020
 *      Author: Fabian Hegg
 */

#ifndef SRC_BLUETOOTH_H_
#define SRC_BLUETOOTH_H_

#include <stdint.h>

const int addressSize = 6;

class Bluetooth
{
public:
  Bluetooth();
  virtual ~Bluetooth();
  bool initialize();
  static void clientEventCallback(void* receivedData);
  bool registerPositionService();
  static bool setDeviceDiscoverable();
  bool updatePositionCharacteristics(uint32_t xPosition, uint32_t yPosition);
private:
  uint8_t address[addressSize];
  uint16_t positionServiceHandle;
  uint16_t positionCharacteristicHandle;
  static bool clientConnected;
};

#endif /* SRC_BLUETOOTH_H_ */
