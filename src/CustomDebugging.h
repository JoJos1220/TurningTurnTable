/***************************TurningTurnTable Project by Jojo1220************************************
  Filename:     CustomDebugging.h
  Purpose:      CutomDebugging.h File for Debugging of TurningTurnTable Project
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

#ifndef CUSTOMDEBUGGING_H
#define CUSTOMDEBUGGING_H

/*-----------------------------------------------------------------------------
 * Including of Libary Direcotries
 * ----------------------------------------------------------------------------
*/

#include <Arduino.h>


/*-----------------------------------------------------------------------------
 * Debugging Serial Declaration
 * ----------------------------------------------------------------------------
*/

// Defining DEBUG Output Level for Console
#ifdef SERIAL_OUTPUT_DEBUGGING
  #define DEBUG_SERIAL Serial
#else
  #define DEBUG_SERIAL if(0) Serial
#endif

// Defining INFORMATION Output Level for Console
#ifdef SERIAL_OUTPUT_INFORMATION
  #define DEBUG_SERIAL_INFORMATION Serial
#else
  #define DEBUG_SERIAL_INFORMATION if(0) Serial
#endif

// Several More Debug Levels are Possible e.g.
// #define DEBUG_SERIAL_Information if(SERIAL_OUTPUT_DEBUGGING)Serial

/*-----------------------------------------------------------------------------
 * Function Header Declaration
 * ----------------------------------------------------------------------------
*/


#endif // CustomDebugging.h