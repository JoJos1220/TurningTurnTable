/***************************TurningTurnTable Project by Jojo1220************************************
  Filename:     BLEIntegration.cpp
  Purpose:      See BLEIntegration.h
  Autor:        Jojo1220
  Created:      05.06.2024
  Modified:     05.06.2024
  Lizenz:       CC BY-NC-ND 4.0
  Notification: See BLEIntegration.h
******************************************v1.0.0****************************************************/

#include "BLEIntegration.h"

/*-----------------------------------------------------------------------------
 * BLE Specific Variable Initialization
 * ----------------------------------------------------------------------------
*/
BLEHIDDevice* hid;
BLECharacteristic* input;
BLECharacteristic* output;
BLECharacteristic* inputVolume;
BLECharacteristic* outputVolume;
bool connected = false;

/*-----------------------------------------------------------------------------
 * Class: MyCallbacks::onConnect() BLE Function
 * ----------------------------------------------------------------------------
*/
void MyCallbacks::onConnect(BLEServer* pServer){
    connected = true;
    BLE2902* desc = (BLE2902*)input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    desc->setNotifications(true);
    
    BLE2902* descv = (BLE2902*)inputVolume->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    descv->setNotifications(true);
} // MyCallbacks::onConnect()

/*-----------------------------------------------------------------------------
 * Class: MyCallbacks::onDisconnect() BLE Function
 * ----------------------------------------------------------------------------
*/
void MyCallbacks::onDisconnect(BLEServer* pServer){
    connected = false;
    BLE2902* desc = (BLE2902*)input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    desc->setNotifications(false);
    
    BLE2902* descv = (BLE2902*)inputVolume->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    descv->setNotifications(false);
} // MyCallbacks::onDisconnect()

/*-----------------------------------------------------------------------------
 * taskServer() BLE Handler Function
 * ----------------------------------------------------------------------------
*/
void taskServer(void*){
  BLEDevice::init("TurningTurnTable");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyCallbacks());

  hid = new BLEHIDDevice(pServer);
  inputVolume = hid->inputReport(1); // <-- input REPORTID from report map
  outputVolume = hid->outputReport(1); // <-- output REPORTID from report map

  input = hid->inputReport(2); // <-- input REPORTID from report map
  output = hid->outputReport(2); // <-- output REPORTID from report map

  std::string name = "Jojo1220";
  hid->manufacturer()->setValue(name);

  hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
  hid->hidInfo(0x00,0x02);

  hid->reportMap((uint8_t*)keyboardHidDescriptor, sizeof(keyboardHidDescriptor));
  hid->startServices();


  BLESecurity *pSecurity = new BLESecurity();
  pSecurity->setKeySize();
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);


  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->setAppearance(HID_KEYBOARD);
  pAdvertising->addServiceUUID(hid->hidService()->getUUID());
  pAdvertising->start();
  hid->setBatteryLevel(7);

  delay(portMAX_DELAY);
} // taskServer()
