/***************************TurningTurnTable Project by Jojo1220************************************
  Filename:     PinControl.cpp
  Purpose:      See PinControl.h
  Autor:        Jojo1220
  Created:      05.06.2024
  Modified:     05.06.2024
  Lizenz:       CC BY-NC-ND 4.0
  Notification: See PinControl.h
******************************************v1.0.0****************************************************/

#include "PinControl.h"

/*-----------------------------------------------------------------------------
 * digitalFASTRead(): Function to read a GPIO Pin State as fast alternative to digitalRead()
 * ----------------------------------------------------------------------------
*/
int digitalFASTRead(uint8_t pin) {
  if (pin < 0 || pin >= 40) {
    // Pinnumber outside of valid area!
    return false;
  }

  uint32_t bit = digitalPinToBitMask(pin);
  uint32_t port = digitalPinToPort(pin);

  if (port == NOT_A_PIN) {
    // Invalid Pinnumber
    return false;
  }

  // Pointer on GPIO-Register
  volatile uint32_t *reg = portInputRegister(port);

  // Read Pin State
  bool state = ((*reg) & bit) != 0;

  return state;
}


/*-----------------------------------------------------------------------------
 * digitalFASTWrite(): Function to write a GPIO Pin State as fast alternative to digitalwrite()
 * ----------------------------------------------------------------------------
*/
void digitalFASTWrite(uint8_t pin, bool state){
  if (pin < 0 || pin >= 40) {
    // Pinnumber outside of valid area!
    return;
  }

  if(pin<32)
  {
    // Handle Pin 0-32
    if(state == HIGH){
      GPIO.out_w1ts = (1 << pin);
    }else{
      GPIO.out_w1tc = (1 << pin);
    }
  }else if(pin<40)
  {
    // Handle pins 32-39 if necessary
    if(state == HIGH){
      GPIO.out1_w1ts.val |= (1 << (pin - 32));
    }else{
      GPIO.out1_w1tc.val |= (1 << (pin - 32));
    }

  }else{
    // WRONG Pin
    return;
  }
}

/*-----------------------------------------------------------------------------
 * LED::LED(byte pin): Constructor Function of LED Class Instance
 * ----------------------------------------------------------------------------
*/
LED::LED(byte pin){
  _pin = pin;
}

/*-----------------------------------------------------------------------------
 * LED::~LED(): Destructor Function of LED Class Instance
 * ----------------------------------------------------------------------------
*/
LED::~LED(void){

}

/*-----------------------------------------------------------------------------
 * LED::setup(): Function to Setup LED Class Instance
 * ----------------------------------------------------------------------------
*/
void LED::setup(void){
  pinMode(_pin, OUTPUT);
}

/*-----------------------------------------------------------------------------
 * LED::writeOutput(bool _state): Function to Wirte LED State
 * ----------------------------------------------------------------------------
*/
void LED::writeOutput(bool _state) {
  digitalFASTWrite(_pin, _state);
}

/*-----------------------------------------------------------------------------
 * LED::invertLEDState(): Function to Invert LED State
 * ----------------------------------------------------------------------------
*/
void LED::invertLEDState(void) {
  digitalFASTWrite(_pin, !digitalFASTRead(_pin));
}

/*-----------------------------------------------------------------------------
 * LED::read(): Function to read LED State
 * ----------------------------------------------------------------------------
*/
bool LED::read(void) {
  return digitalFASTRead(_pin);
}



