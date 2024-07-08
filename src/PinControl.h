/***************************TurningTurnTable Project by Jojo1220************************************
  Filename:     PinControl.h
  Purpose:      PinControl.h File of TurningTurnTable Project containing alternative function for controlling pins.
                Functions like a digitalFASTread()/digitalFASTwrite() Function, compared to the digitalRead()/digitalWrite(), is implemented
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

#ifndef PINCONTROL_H
#define PINCONTROL_H

/*-----------------------------------------------------------------------------
 * Including of Libary Direcotries
 * ----------------------------------------------------------------------------
*/

#include <Arduino.h>

/*-----------------------------------------------------------------------------
 * Function Header Definition
 * ----------------------------------------------------------------------------
*/

int digitalFASTRead(uint8_t pin);
void digitalFASTWrite(uint8_t pin, bool state);

/*-----------------------------------------------------------------------------
 * Class Definition
 * ----------------------------------------------------------------------------
*/

/* Class Definition of LED */
class LED {
  public:
    LED(byte pin);
    ~LED(void);

    void setup(void);
    void writeOutput(bool _state);
    void invertLEDState(void);
    bool read(void);

  private:
    byte _pin;
};

#endif // PinControl.h