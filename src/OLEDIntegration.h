/***************************TurningTurnTable Project by Jojo1220************************************
  Filename:     OLEDIntegration.h
  Purpose:      OLEDIntegration.h File of TurningTurnTable Project containing Display Setup/Update functions
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

#ifndef OLEDINTEGRATION_H
#define OLEDINTEGRATION_H

/*-----------------------------------------------------------------------------
 * Including of Libary Direcotries
 * ----------------------------------------------------------------------------
*/

#include <U8g2lib.h>
#include "CustomDebugging.h"
#include "TurningTurnTablePARAMETERS.h"
#include "TurningTurntable.h"
#include "pins.h"

// Including Wire.h because of I2C-Oled Display!
// If other Display is used, include SPI Libary!
#ifdef U8X8_HAVE_HW_SPI
    #include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
    #include <Wire.h>
#endif


/*-----------------------------------------------------------------------------
 * Function Header Definition
 * ----------------------------------------------------------------------------
*/
void OLEDSetup(void);
void MenuSelector(int _value, bool* _spacingTriggerd, long* _StepperWay, bool* _StartAutoMode, bool* _hitBLEScreenshot, long* _way, bool* _StepperRunning, int* _InfiniteMode);
void OLEDMainMenu(int _selected, float* _parMaxSpeed, float* _parAccel);

void setMenuSelected(int _value);
int getMenuSelected(void);
int getMaxMenuCount(void);

/*-----------------------------------------------------------------------------
 * Setup Global Constants
 * ----------------------------------------------------------------------------
*/

// Placeholder Constant for dynamic Parameters
static const char* placeHolder = "___VALUE___";

// Main Menu String Configuration
//#define MENUMAIN 0
static const uint8_t NUM_MENUMAIN_ITEMS = 4;
static const char* menuMAIN_Header = ">>Main Menu<<";
static const char* menuMAIN_items[NUM_MENUMAIN_ITEMS] = {
    "Auto Mode",
    "Hand Mode",
    "Settings",
    "- - - - - - - - - -",
};

// Auto-Mode Settings Menu
//#define MENUAUTOMODE 10
static const uint8_t NUM_MENUAutoMode_ITEMS = 4;
static const char* menuAutoMode_Header = ">>Auto Mode<<";
static const char* menuAutoMode_items[NUM_MENUAutoMode_ITEMS] = {
    "START",
    "STOPP",
    "<< BACK",
    "- - - - - - - - - -"
};

// Hand-Mode Settings Menu
//#define MENUHANDMODE 11
static const uint8_t NUM_MENUHandMode_ITEMS = 10;
static const char* menuHandMode_Header = ">>Hand Mode<<";
static const char* menuHandMode_items[NUM_MENUHandMode_ITEMS] = {
    "+ INF",
    "+ 1/4 Turn",
    "+ 1/16 Turn",
    "+ 1/64 Turn",
    "- 1/64 Turn",
    "- 1/16 Turn",
    "- 1/4 Turn",
    "- INF",
    "<< BACK",
    "- - - - - - - - - -"
};

// Settings Menu String Configuration
//#define MENUSETTINGS 12
static const uint8_t NUM_MENUSETTINGS_ITEMS = 5;
static const char* menuSETTINGS_Header = ">>Settings<<";
static const char* menuSETTINGS_items[NUM_MENUSETTINGS_ITEMS] = {
    "Auto Mode",
    "Stepper Acceleration",
    "Stepper Max. Speed",
    "<< BACK",
    "- - - - - - - - - -"
};

// Stepper Acceleration String Configuration
//#define MENUACCELERATION 100
static const uint8_t NUM_MENUSTEPACC_ITEMS = 3;
static const char* menuAcceleration_Header = ">>Stepper Acc.<<";
extern const char* menuSTEPaCC_items[NUM_MENUSTEPACC_ITEMS];

// Others String Configuration
//#define MENUSPEED 101
static const uint8_t NUM_MENUSTEPVEL_ITEMS = 3;
static const char* menuSpeed_Header = ">>Stepper Vel.<<";
extern const char* menuSTEPvEL_items[NUM_MENUSTEPVEL_ITEMS];

// Automatic Mode Menu String Configuration
//#define MENUAUTOMODESETTINGS 102
static const uint8_t NUM_MENUAutoModeSettings_ITEMS = 5;
static const char* menuAutoModeSettings_Header = ">>Auto Settings<<";
static const char* menuAtuoModeSettings_items[NUM_MENUAutoModeSettings_ITEMS] = {
    "Stepper Angle",
    "Test Camera",
    "Pic. Delay",
    "<< BACK",
    "- - - - - - - - - -"
};

// Stepper Acceleration Setting String Configuration
//#define MENUACCSETTING 1000
static const uint8_t NUM_MENUAccSettings_ITEMS = 1;
static const char* menuAccSettings_Header = ">>Set Acc. Stepper<<";


// Stepper Speed Setting String Configuration
//#define MENUSPEEDSETTING 1001
static const uint8_t NUM_MENUVelSettings_ITEMS = 1;
static const char* menuVelSettings_Header = ">>Set Vel. Stepper<<";


// Test Screenshot Funciton
//#define MENUTESTSCREENSHOT 1002
static const uint8_t NUM_MENUTestBLEScreens_ITEMS = 3;
static const char* menuTestBLEScreens_Header = ">>Test BLE Screens.<<";
static const char* menuTestBLEScreens_items[NUM_MENUTestBLEScreens_ITEMS] = {
    "Test Screens.",
    "<< BACK",
    "- - - - - - - - - -"
};

// Stepper Angles Menu String Configuration
//#define MENUAUTOMODESTEPPERANGLE 1003
static const uint8_t NUM_MENUStepperAngle_ITEMS = 9;
static const char* menuStepperAngle_Header = ">>Stepper Angle<<";
static const char* menuStepperAngle_items[NUM_MENUStepperAngle_ITEMS] = {
    " 1: 360°",
    " 2; 180°",
    " 4: 90°",
    " 8: 45°",
    "16: 22,5°",
    "32: 11,25°",
    "64: 5,625",
    "<< BACK",
    "- - - - - - - - - -"
};

// Others String Configuration
//#define MENUPICTUREDELAY 1004
static const uint8_t NUM_MENUPICDEL_ITEMS = 3;
static const char* menuPicDelay_Header = ">>Pic. Delay<<";
extern const char* menuPicDelay_items[NUM_MENUPICDEL_ITEMS];

// Stepper Speed Setting String Configuration
//#define MENUPICDELAYSETTING 1005
static const uint8_t NUM_MENUPICTUREDELAY_ITEMS = 1;
static const char* menuPictureDelay_Header = ">>Set Pic. Delay<<";

#endif // OLEDIntegration.h
