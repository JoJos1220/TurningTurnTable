/***************************TurningTurnTable Project by Jojo1220************************************
  Filename:     TurningTurnTablePARAMETERS.h
  Purpose:      TurningTurnTablePARAMETERS.h File of TurningTurnTable Project containing project specific parameters
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

#ifndef TURNINGTURNTABLEPARAMETERS_H
#define TURNINGTURNTABLEPARAMETERS_H

/*-----------------------------------------------------------------------------
 * Setup Global Constants
 * ----------------------------------------------------------------------------
*/

// OLED-Specific Project Parameters
#define MAX_SHOWN_MENU_Entrys 3                         // Max Shown Menu Items in OLED Screen

// Menu Definition
#define MENUMAIN 0                                      // Main Menu                                
    #define MENUAUTOMODE 10                             // Submenu to Setup Automatic Mode
    #define MENUHANDMODE 11                             // Submenu to Setup Hand Mode
    #define MENUSETTINGS 12                             // Submenu to Setup Parameters
        #define MENUACCELERATION 100                    // Submenu to Adjust the Stepper Acceleration   
            #define MENUACCSETTING 1000                 // Submenu to Adjust the Stepper Rotation "Real"-Acceleration-Value
        #define MENUSPEED 101                           // Menu to Adjust the Stepper Speed
            #define MENUSPEEDSETTING 1001               // Submenu to Adjust the Stepper Rotation "Real"-Speed-Value
        #define MENUAUTOMODESETTINGS 102                // Menu to set-up Automatic Mode Parameters
            #define MENUTESTSCREENSHOT 1002             // Submenu to Test Bluetooth Screenshot function
            #define MENUAUTOMODESTEPPERANGLE 1003       // Submenu to set Stepper Rotation Angle between Screenshot  
            #define MENUPICTUREDELAY 1004               // Submenu to set the Delay Time between Screenshot  
                #define MENUPICDELAYSETTING  1005       // Submenu to Adjust the Picutre Delay

// Stepper Specific Project Parameters
#define defaultMAXSPEEDSTEPPER 700.0                    // Default Motor Max Speed Parameter
#define MAXSTEPPERSPEED 2000.0                          // Default Motor Max Speed HIGH-Value
#define MINSTEPPERSPEED 50.0                            // Default Motor Min Speed LOW-Value
#define defaultACCELERATIONSTEPPER 300.0                // Default Motor Acceleration Parameter
#define MAXSTEPPERACCELERATION 3000.0                   // Default Motor Max Acceleration HIGH-Value
#define MINSTEPPERACCELERATION 10.0                     // Default Motor Min Acceleration LOW-Value
#define defaultPICTUREDELAY 5000                        // Default Delay after a Picture is Triggered by BLE
#define MAXDPICTUREDELAY 10000                          // Default Delay MAX Time
#define MINPICTUREDELAY 1000                            // Default delay MIN Time 

// Defining Stepper Operation Mode on your specific connection/operation Purpose
#define StepperOperation 2                              // 0 == FULL4WIRE; 1 == HALF4WIRE; 2 == DRIVER Stage with 16 MicroSteps

// Turning Parameters
#if StepperOperation == 2
    // Multiplying here, because of 16 Times of Steps within one turn in 1/16 Micro-Mode
    #define MECHGEARRATIO   16*6.1666667 
#elif StepperOperation == 1
    // Multiplying here, because of 2 Times of Steps within one turn in Half Mode
    #define MECHGEARRATIO   2*6.1666667                       // Defining Ratio of Mechanical Gear
#elif StepperOperation == 0
    #define MECHGEARRATIO   6.1666667                       // Defining Ratio of Mechanical Gear
#endif

#ifdef _28BYJ48	
    #define TURNFULL        (MECHGEARRATIO * (long)2048)    // Steps for a FULL Revolution -->> 1 Image
    #define TURNHALF        (MECHGEARRATIO * (long)1024)    // Steps for a HALF Revolution -->> 2 Images
    #define TURNQUART       (MECHGEARRATIO * (long)512)     // Steps for a QUARTER Revolution -->> 4 Images 
    #define TURNEIGHT       (MECHGEARRATIO * (long)256)     // Steps for a EIGHTS Revolution -->> 8 Images
    #define TURNSIXTE       (MECHGEARRATIO * (long)128)     // Steps for a Sixteenth Revolution -->> 16 Images
    #define TURNTHIRT       (MECHGEARRATIO * (long)64)      // Steps for a Thirty-Twos Revolution -->> 32 Images
    #define TURNSIXTY       (MECHGEARRATIO * (long)32)      // Steps for a Sixty-fourth Revolution -->> 64 Images
#endif

#ifdef _17HS08
    #define TURNFULL        (MECHGEARRATIO * (long)200)    // Steps for a FULL Revolution -->> 1 Image
    #define TURNHALF        (MECHGEARRATIO * (long)100)    // Steps for a HALF Revolution -->> 2 Images
    #define TURNQUART       (MECHGEARRATIO * (long)50)     // Steps for a QUARTER Revolution -->> 4 Images 
    #define TURNEIGHT       (MECHGEARRATIO * (long)25)     // Steps for a EIGHTS Revolution -->> 8 Images
    #define TURNSIXTE       (MECHGEARRATIO * (long)12.5)   // Steps for a Sixteenth Revolution -->> 16 Images
    #define TURNTHIRT       (MECHGEARRATIO * (long)6.25)   // Steps for a Thirty-Twos Revolution -->> 32 Images
    #define TURNSIXTY       (MECHGEARRATIO * (long)3.125)  // Steps for a Sixty-fourth Revolution -->> 64 Images
#endif

// Process Steps for Automatic Mode
#define STEPDONE        0                               // Process Step: Standby
#define STEPPICTURE     10                              // Process Step: Taking a Picture of Object
#define STEPPICBREAK    20                              // Process Step: Break after Sendig Picture CMD to device via BLE
#define STEPPERMOVE     30                              // Process Step: Moving Stepper
#define STEPPBREAK      40                              // Process Step: Break after Stepper-Rotation
#define STEPCHECK       50                              // Process Step: Check if further Rotation is necessary
#define STEPPSTOPJOB    99                              // Process Step: STOP Automatic-Procedure

#endif // TurningTurnTablePARAMETERS.h
