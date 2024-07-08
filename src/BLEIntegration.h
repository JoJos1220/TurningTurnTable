/***************************TurningTurnTable Project by Jojo1220************************************
  Filename:     BLEIntegration.h
  Purpose:      BLEIntegration.h File for Bluetooth Low Energie functions of TurningTurnTable Project
  Autor:        Jojo1220
  Created:      05.06.2024
  Modified:     05.06.2024
  Lizenz:       CC BY-NC-ND 4.0
  Notification: Requieres the following Libarys:
          Dependency Graph
          |-- AccelStepper @ 1.64.0
          |-- U8g2 @ 2.35.19
          |-- RotaryEncoder @ 1.5.3
          |-- ESP32 BLE Arduino @ 2.0.0
          |-- SPI @ 2.0.0
          |-- Wire @ 2.0.0
          |-- Preferences @ 2.0.0
******************************************v1.0.0****************************************************/

#ifndef BLEINTEGRATION_H
#define BLEINTEGRATION_H

/*-----------------------------------------------------------------------------
 * Including of Libary Direcotries
 * ----------------------------------------------------------------------------
*/

#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <BLEHIDDevice.h>
#include <HIDTypes.h>
#include <HIDKeyboardTypes.h>

/*-----------------------------------------------------------------------------
 * Function Header Definition
 * ----------------------------------------------------------------------------
*/
void taskServer(void*);

/*-----------------------------------------------------------------------------
 * Global Structur/ Type Definition
 * ----------------------------------------------------------------------------
*/
typedef struct
{
  // uint8_t  reportId;                               // Report ID = 0x02 (2)
  // Collection: CA:ConsumerControl
  uint16_t ConsumerControl;                           // Value = 0 to 572
} inputConsumer_t;
  
typedef struct
{
// uint8_t  reportId;                                 // Report ID = 0x02 (2)
  // Collection: CA:Keyboard
  uint8_t  KB_KeyboardKeyboardLeftControl  : 1;       // Usage 0x000700E0: Keyboard Left Control, Value = 0 to 1
  uint8_t  KB_KeyboardKeyboardLeftShift    : 1;       // Usage 0x000700E1: Keyboard Left Shift, Value = 0 to 1
  uint8_t  KB_KeyboardKeyboardLeftAlt      : 1;       // Usage 0x000700E2: Keyboard Left Alt, Value = 0 to 1
  uint8_t  KB_KeyboardKeyboardLeftGui      : 1;       // Usage 0x000700E3: Keyboard Left GUI, Value = 0 to 1
  uint8_t  KB_KeyboardKeyboardRightControl : 1;       // Usage 0x000700E4: Keyboard Right Control, Value = 0 to 1
  uint8_t  KB_KeyboardKeyboardRightShift   : 1;       // Usage 0x000700E5: Keyboard Right Shift, Value = 0 to 1
  uint8_t  KB_KeyboardKeyboardRightAlt     : 1;       // Usage 0x000700E6: Keyboard Right Alt, Value = 0 to 1
  uint8_t  KB_KeyboardKeyboardRightGui     : 1;       // Usage 0x000700E7: Keyboard Right GUI, Value = 0 to 1
  uint8_t  Key;                                       // Value = 0 to 101
} inputKeyboard_t;

/*-----------------------------------------------------------------------------
 * Global Variable Definition
 * ----------------------------------------------------------------------------
*/
static uint8_t idleRate;           /* in 4 ms units */

static inputConsumer_t consumer_Report{};
static inputKeyboard_t keyboard_report{}; // sent to PC

extern BLEHIDDevice* hid;
extern BLECharacteristic* input;
extern BLECharacteristic* output;
extern BLECharacteristic* inputVolume;
extern BLECharacteristic* outputVolume;
extern bool connected;

const uint8_t keyboardHidDescriptor[] = {
  0x05, 0x0c,                    //     USAGE_PAGE (Consumer Devices)
  0x09, 0x01,                    //     USAGE (Consumer Control)
  0xa1, 0x01,                    //     COLLECTION (Application)
  0x85, 0x01,                    //     REPORT_ID (1)
  0x19, 0x00,                    //     USAGE_MINIMUM (Unassigned)
  0x2a, 0x3c, 0x02,              //     USAGE_MAXIMUM (AC Format)
  0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
  0x26, 0x3c, 0x02,              //     LOGICAL_MAXIMUM (572)
  0x95, 0x01,                    //     REPORT_COUNT (1)
  0x75, 0x10,                    //     REPORT_SIZE (16)
  0x81, 0x00,                    //     INPUT (Data,Var,Abs)
  0xc0,                          //     END_COLLECTION
  0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
  0x09, 0x06,                    //     USAGE (Keyboard)
  0xa1, 0x01,                    //     COLLECTION (Application)
  0x85, 0x02,                    //     REPORT_ID (2)
  0x05, 0x07,                    //     USAGE_PAGE (Keyboard)
  0x19, 0xe0,                    //     USAGE_MINIMUM (Keyboard LeftControl)
  0x29, 0xe7,                    //     USAGE_MAXIMUM (Keyboard Right GUI)
  0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
  0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
  0x75, 0x01,                    //     REPORT_SIZE (1)
  0x95, 0x08,                    //     REPORT_COUNT (8)
  0x81, 0x02,                    //     INPUT (Data,Var,Abs)
  0x95, 0x01,                    //     REPORT_COUNT (1)
  0x75, 0x08,                    //     REPORT_SIZE (8)
  0x25, 0x65,                    //     LOGICAL_MAXIMUM (101)
  0x19, 0x00,                    //     USAGE_MINIMUM (Reserved (no event indicated))
  0x29, 0x65,                    //     USAGE_MAXIMUM (Keyboard Application)
  0x81, 0x00,                    //     INPUT (Data,Ary,Abs)
  0xc0                           //     END_COLLECTION
};

/*-----------------------------------------------------------------------------
 * Custom Class MyCallbacks() Definition
 * ----------------------------------------------------------------------------
*/
class MyCallbacks : public BLEServerCallbacks {
  public:
    void onConnect(BLEServer* pServer);
    void onDisconnect(BLEServer* pServer);
};

#endif // BLEIntegration.h