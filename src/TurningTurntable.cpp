/***************************TurningTurnTable Project by Jojo1220************************************
  Filename:     TurningTurntable.cpp
  Purpose:      See TurningTurntable.h
  Autor:        Jojo1220
  Created:      05.06.2024
  Modified:     05.06.2024
  Lizenz:       CC BY-NC-ND 4.0
  Notification: See TurningTurntable.h
******************************************v1.0.0****************************************************/

#include  "TurningTurntable.h"

/*-----------------------------------------------------------------------------
 * Setup Global Variables
 * ----------------------------------------------------------------------------
*/

// Procject Specific System Parameters Instance
paramValues systemParameters;

// preferences instance
Preferences preferences;

// Preferences App Name
const char *AppName = "mAp";

// key constance or Access key:values within Preferences
const char* STEPCOUNT_KEY = "StepCount";
const char* MAXVEL_KEY = "MaxVel";
const char* MAXACC_KEY = "MaxAcc";
const char* PICDELAY_KEY = "PicDel";

/*-----------------------------------------------------------------------------
 * readParameters() from Preferences function
 * ----------------------------------------------------------------------------
*/
paramValues readParameters(void){

  paramValues _data;

  preferencesBegin();

  // Read Stored Project Specific Settings in Preferneces to _data struct
  _data.steppCount = preferences.getLong(STEPCOUNT_KEY);
  _data.maxStepperSpeed = preferences.getFloat(MAXVEL_KEY); 
  _data.maxStepperAccel = preferences.getFloat(MAXACC_KEY);
  _data.pictureDelay = preferences.getULong(PICDELAY_KEY);
 
  preferences.end();

  // Checking by default brightness Level "0" if so, factory settings has to be defined.
  if(_data.steppCount == 0){
    DEBUG_SERIAL.println("[Prefs] not Found! Load Factory Settings!");
    _data = writeParameters(_data);
  }
  
  printParameters(_data);
  return _data;
}

/*-----------------------------------------------------------------------------
 * writeParameters() to Preferences function
 * ----------------------------------------------------------------------------
*/
paramValues writeParameters(paramValues _data){
  // Setting Default Variables to _data Struct
  _data.steppCount = TURNSIXTE;
  _data.maxStepperSpeed = defaultMAXSPEEDSTEPPER;
  _data.maxStepperAccel = defaultACCELERATIONSTEPPER;
  _data.pictureDelay = defaultPICTUREDELAY;

  // Wrting Default Values Back to preferences
  preferencesBegin();

  preferences.putLong(STEPCOUNT_KEY, _data.steppCount);
  preferences.putFloat(MAXVEL_KEY, _data.maxStepperSpeed);
  preferences.putFloat(MAXACC_KEY, _data.maxStepperAccel);
  preferences.putULong(PICDELAY_KEY, _data.pictureDelay);
    
  preferences.end();
  
  DEBUG_SERIAL.println("[Prefs] Factory Reset - Done!");

  return _data;
};

/*-----------------------------------------------------------------------------
 * printParameters() Parameters print to Console function
 * ----------------------------------------------------------------------------
*/
void printParameters(paramValues _data){
  DEBUG_SERIAL.print("-------------------------------------------------");
  DEBUG_SERIAL.print("\nSettings:\tStepps Count during Auto: ");
  DEBUG_SERIAL.print(_data.steppCount);
  DEBUG_SERIAL.print("\tStepper max. Speed: ");
  DEBUG_SERIAL.print(_data.maxStepperSpeed);
  DEBUG_SERIAL.print("\tStepper max. Acceleration: ");
  DEBUG_SERIAL.print(_data.maxStepperAccel);
  DEBUG_SERIAL.print("\tPicture Delay Time: ");
  DEBUG_SERIAL.print(_data.pictureDelay);
  DEBUG_SERIAL.println("\n-------------------------------------------------");
}

/*-----------------------------------------------------------------------------
 * preferencesBegin() Function with Error Handling
 * ----------------------------------------------------------------------------
*/
void preferencesBegin(void){
  // Max Attempts for Opening Preferences
  uint8_t maxAttempts = 10;

  for(uint8_t attempt = 1; attempt <= maxAttempts; attempt++) {
    if(preferences.begin(AppName, false)) {
        // Successfull Initialization of Preferences
        break;  // Break-up-Loop
    }else{
      for(unsigned long start = millis(); millis() - start < 200;) {
        yield(); //short yield() back to ESP Firmware
      }       
      DEBUG_SERIAL.println("[Prefs] Begin!");
    }

    // If Preferences begin is not possible -->> RESTART!
    if (attempt == maxAttempts) {
      DEBUG_SERIAL.println("[Prefs] Setup Error Restart!");
      #ifndef NATIVE_ENVIRONMENT
        ESP.restart();
      #endif
    }
  }
}

