/***************************TurningTurnTable Project by Jojo1220************************************
  Filename:     TurningTurntable.h
  Purpose:      TurningTurntable.h File of TurningTurnTable Project containing project specific stuff, like Read/Write Preferences
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

#ifndef TURNINGTURNTABLE_H
#define TURNINGTURNTABLE_H

/*-----------------------------------------------------------------------------
 * Including of Libary Direcotries
 * ----------------------------------------------------------------------------
*/
#include <Preferences.h>

#include "CustomDebugging.h"
#include "TurningTurnTablePARAMETERS.h"


/*-----------------------------------------------------------------------------
 * Setup Custom Structs (Datatyps)
 * ----------------------------------------------------------------------------
*/
// Custom Struct for Remanent Project Specific Parameters
struct paramValues{
  long steppCount = 0;
  float maxStepperSpeed = 0.0;
  float maxStepperAccel = 0.0;
  unsigned long pictureDelay = 0;
};

/*-----------------------------------------------------------------------------
 * Setup Global Variables
 * ----------------------------------------------------------------------------
*/

// Project specific System Parameters
extern paramValues systemParameters;

// Preferences Object for Remanent adjustable Settings Variables
extern Preferences preferences;

// Preferences APP Name
extern const char *AppName;

// Definition of Project Specific Constant Key Char* for Preferences Access
extern const char* STEPCOUNT_KEY;
extern const char* MAXVEL_KEY;
extern const char* MAXACC_KEY;
extern const char* PICDELAY_KEY;


/*-----------------------------------------------------------------------------
 * Setup Custom Function Headers
 * ----------------------------------------------------------------------------
*/
void printParameters(paramValues _data);

void preferencesBegin(void);

paramValues readParameters(void);
paramValues writeParameters(paramValues _data);


#endif //TurningTurntable.h