/***************************TurningTurnTable Project by Jojo1220************************************
  Filename:     pins.h
  Purpose:      pins.h File of TurningTurnTable Project containing Hardware Pin's Configuration
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
#ifndef PINS_H
#define PINS_H

/*-----------------------------------------------------------------------------
 * Setup Global Constants
 * ----------------------------------------------------------------------------
*/

#ifdef ESP32dev_Board
/*-----------------------------------------------------------------------------
 * Functionl Pin Definition for ESP32 Developement Board 
 * ----------------------------------------------------------------------------
*/
  // System Specific Pin Definition
  #define ONBoardLED 2

  // Pin Definition for OLED
  #define SDA 21
  #define SCL 22

  // Pin Definition for Stepper Motor Driver ULN2003
  // Watch out! IN2 and IN3 has to be inverted in Software Logic due to used Libary!
  // Control is done by: (IN1; IN3; IN2; IN4) for 28BYJ48 Stepper Motor
  // (IN1; IN2; IN3; IN4) for 17HS08 NEMA Stepper Motor -->> Setup is accordingly changed like that!
  // (IN1==DIRECTION; IN2==STEP; IN3==ENABLE; IN4==NOT USED) for DRIVER Stage Setup -->> Setup is accordingly changed like that!
  #define IN1 26
  #define IN2 33
  #define IN3 25
  #define IN4 32

  // Pin Definition for Encoder
  #define encoderPinA 34      // CLK or Clock Pin
  #define encoderPinB 14      // DT or Data Pin

  #define encoderPinSW 27     // SW or Push Button Pin

#endif //ESP32dev_Board


#ifdef ESP32dev_ARDUINO
/*-----------------------------------------------------------------------------
 * Pin Definition for ESP32-"Arduino"
 * ----------------------------------------------------------------------------
*/
  // System Specific Pin Definition
  #define ONBoardLED 18

  // Pin Definition for OLED
  #define SDA 14
  #define SCL 27

  // Pin Definition for Stepper Motor Driver ULN2003
  // Watch out! IN2 and IN3 has to be inverted in Software Logic due to used Libary!
  // Control is done by: (IN1; IN3; IN2; IN4) for 28BYJ48 Stepper Motor
  // (IN1; IN2; IN3; IN4) for 17HS08 NEMA Stepper Motor -->> Setup is accordingly changed like that!
  // (IN1==DIRECTION; IN2==STEP; IN3==ENABLE; IN4==NOT USED) for DRIVER Stage Setup -->> Setup is accordingly changed like that!
  #define IN1 26
  #define IN2 17
  #define IN3 25
  #define IN4 16

  // Pin Definition for Encoder
  #define encoderPinA 5       // CLK or Clock Pin
  #define encoderPinB 19      // DT or Data Pin

  #define encoderPinSW 23     // SW or Push Button Pin

#endif // ESP32dev_ARDUINO

#endif //pins.h

