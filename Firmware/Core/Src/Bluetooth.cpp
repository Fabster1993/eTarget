/*
 * Bluetooth.cpp
 *
 *  Created on: 16.08.2020
 *      Author: Fabian Hegg
 */
#include "Bluetooth.h"
#include "bluenrg_def.h"
#include "bluenrg_gap.h"
#include "sm.h"
#include "bluenrg_gatt_server.h"
#include "bluenrg_hal_aci.h"
#include "hci_const.h"
#include "bluenrg_gatt_aci.h"
#include "bluenrg_aci_const.h"
#include <stdio.h>

#include "hci.h"
#include "bluenrg_gap_aci.h"
#include "hci_le.h"

#define DEVICE_NAME 'e','T','a','r','g','e','t'

bool Bluetooth::clientConnected = false;
const char deviceName[] = "eTarget";
const uint32_t devicePin = 123456;
const uint8_t positionServiceUuid[]= {0x00,0x00,0x00,0x00,0x00,0x01,0x11,0xe1,0x9a,0xb4,0x00,0x02,0xa5,0xd5,0xc5,0x1b};
const uint8_t positionCharacteristicUuid[] = {0x00,0x00,0x00,0x00,0x00,0x01,0x11,0xe1,0xac,0x36,0x00,0x02,0xa5,0xd5,0xc5,0x1b};
const uint8_t positionMessageLength = 11;


Bluetooth::Bluetooth()
{
  // TODO Auto-generated constructor stub

}

Bluetooth::~Bluetooth()
{
  // TODO Auto-generated destructor stub
}

bool Bluetooth::initialize()
{
  uint8_t  addressLengthOut;
  uint16_t serviceHandle, deviceNameCharacteristicHandle, appearanceCharacteristicHandle;

  hci_init(clientEventCallback, NULL);
  hci_reset();
  HAL_Delay(100);
  if(aci_hal_read_config_data(CONFIG_DATA_RANDOM_ADDRESS, addressSize, &addressLengthOut, address) != BLE_STATUS_SUCCESS)
    return false;
  if(aci_gatt_init() != BLE_STATUS_SUCCESS)
    return false;
  if(aci_gap_init_IDB05A1(GAP_PERIPHERAL_ROLE_IDB05A1, 0, 0x07, &serviceHandle, &deviceNameCharacteristicHandle, &appearanceCharacteristicHandle) != BLE_STATUS_SUCCESS)
    return false;
  if(aci_gatt_update_char_value(serviceHandle, deviceNameCharacteristicHandle, 0, strlen(deviceName), (uint8_t *)deviceName) != BLE_STATUS_SUCCESS)
    return false;
  if(aci_gap_set_auth_requirement(MITM_PROTECTION_REQUIRED, OOB_AUTH_DATA_ABSENT, NULL, 7, 16, USE_FIXED_PIN_FOR_PAIRING, devicePin, BONDING)!= BLE_STATUS_SUCCESS)
    return false;

  if(!registerPositionService())
    return false;

  if(!setDeviceDiscoverable())
    return false;

  return true;
}

void Bluetooth::clientEventCallback(void* receivedData)
{
  hci_uart_pckt* uartPackage = static_cast<hci_uart_pckt*>(receivedData);
  hci_event_pckt* eventPackage = (hci_event_pckt*)uartPackage->data;

   if(uartPackage->type != HCI_EVENT_PKT)
     return;

   switch(eventPackage->evt)
   {
   case EVT_DISCONN_COMPLETE:
     { // Client disconnected
       setDeviceDiscoverable();
       clientConnected = false;
     }
     break;
   case EVT_LE_META_EVENT:
     {
       evt_le_meta_event* evt = (evt_le_meta_event*)eventPackage->data;
       switch(evt->subevent){
       case EVT_LE_CONN_COMPLETE:
         {
           // Client connected
           clientConnected = true;
         }
         break;
       }
     }
     break;
   }
}

bool Bluetooth::registerPositionService()
{
  if(aci_gatt_add_serv(UUID_TYPE_128, positionServiceUuid, PRIMARY_SERVICE, 3 + 1, &positionServiceHandle) != BLE_STATUS_SUCCESS)
    return false;

  if(aci_gatt_add_char(positionServiceHandle, UUID_TYPE_128, positionCharacteristicUuid, positionMessageLength, CHAR_PROP_NOTIFY,
                       ATTR_PERMISSION_NONE, GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, 16, 0, &positionCharacteristicHandle) != BLE_STATUS_SUCCESS)
    return false;

  return true;
}

bool Bluetooth::setDeviceDiscoverable()
{
  const char localName[] = {AD_TYPE_COMPLETE_LOCAL_NAME, DEVICE_NAME};
  hci_le_set_scan_resp_data(0, NULL);
  if (aci_gap_set_discoverable(ADV_IND, (1000*1000)/625,(1200*1000)/625, STATIC_RANDOM_ADDR, NO_WHITE_LIST_USE,
     sizeof(localName), localName, 0, NULL, 0, 0) != BLE_STATUS_SUCCESS)
    return false;

  return true;
}

bool Bluetooth::updatePositionCharacteristics(int32_t xPosition, int32_t yPosition)
{
  uint8_t message[positionMessageLength];
  sprintf((char*)message,"X%dY%d", (unsigned)xPosition, (unsigned)yPosition);
  if(aci_gatt_update_char_value(positionServiceHandle, positionCharacteristicHandle, 0, positionMessageLength, message) != BLE_STATUS_SUCCESS)
    return false;

  return true;
}
