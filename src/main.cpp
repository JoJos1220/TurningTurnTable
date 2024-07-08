/***************************TurningTurnTable Project by Jojo1220************************************
  Filename:     main.cpp
  Purpose:      main.cpp File of TurningTurnTable Project containing setup/loop Function
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

/*-----------------------------------------------------------------------------
 * Including of Libary Direcotries
 * ----------------------------------------------------------------------------
*/
#include <Arduino.h>
#include <RotaryEncoder.h>

#include "TurningTurnTablePARAMETERS.h"
#include "TurningTurntable.h"
#include "pins.h"
#include "StepperControl.h"
#include "BLEIntegration.h"
#include "OLEDIntegration.h"
#include "PinControl.h"

/*-----------------------------------------------------------------------------
 * Function Headers
 * ----------------------------------------------------------------------------
*/
void toggleScreenshot(void);
void runStepper(bool _autoModeActive);
uint8_t updateSteps(long _stepps);
void updateOLEDTask(void* _value);
void automaticMode(void);
void EncoderAcceleration(unsigned long _ms, long* _newPos, int Dir);

/*-----------------------------------------------------------------------------
 * Setup Global Variables
 * ----------------------------------------------------------------------------
*/

TaskHandle_t oledTaskHandler;                 // OLED Update Task Handler
TaskHandle_t serverTaskHandler;               // Bluetooth Update Task Handler

long newPosition = 0;                         // Direct Return Value of Encoder
long moduloValue = 0;                         // Manipulated Value based on selected Menu
bool encoderButtonPressed = false;
bool StepperRunning = false;
bool HitPicture = false;

int infiniteMode = 0;                         // Infinite Mode Variable

int Dir_old = 0;                              // Rotary Encoder Direction

bool StartAutoMode = false;

uint8_t stepAutoMode = STEPDONE;              // Step Variable for switch/case in Automatic Mode
uint8_t CountStepsAuto = 0;                   // Counting amounts of Step in Automatic Mode

long way = (long)0;

const long roundingWindow = 1;                // Rounding Window constant value

unsigned long AsyncBLEBreak = 0;

const unsigned long autoModeDelay = 5000;
unsigned long AsyncAutoModeBreak = 0;

const unsigned long debounceDelay = 70;       // Debounce-Time for Push Buttons [ms]
unsigned long lastDebounceTime = 0;           // Timestamp last valid Push Button state

constexpr float m = 30;                       // the maximum acceleration is 10 times.

constexpr float longCutoff = 50;              // at 200ms or slower, there should be no acceleration. (factor 1)

constexpr float shortCutoff = 5;              // at 5 ms, we want to have maximum acceleration (factor m)

// To derive the calc. constants, compute as follows:
// On an x(ms) - y(factor) plane resolve a linear formular factor(ms) = a * ms + b;
// where  f(4)=10 and f(200)=1

constexpr float a = (m - 1) / (shortCutoff - longCutoff);
constexpr float b = 1 - longCutoff * a;

#ifdef _28BYJ48
  StepperControl myStepper(IN1, IN2, IN3, IN4); // Stepper Motor Setup
#elif _17HS08
  StepperControl myStepper(IN1, IN3, IN2, IN4); // Stepper Motor Setup
#else
  #error "No Stepper model selected!"
#endif

RotaryEncoder encoder(encoderPinB, encoderPinA, RotaryEncoder::LatchMode::FOUR3); // Encoder Setup

LED BoardLED(ONBoardLED); // Onboard LED

/*-----------------------------------------------------------------------------
 * Global Setup() Function Call - Running ONCE after Startup
 * ----------------------------------------------------------------------------
*/
void setup() {
  DEBUG_SERIAL.begin(115200);
  DEBUG_SERIAL.println("\n");
  DEBUG_SERIAL.println("--- BOOT ---");
  DEBUG_SERIAL.println("Turning-TurnTable by Jojo1220");
  DEBUG_SERIAL_INFORMATION.println(ARDUINO);
  DEBUG_SERIAL_INFORMATION.println(__FILE__);
  DEBUG_SERIAL_INFORMATION.print("---------------------------------------");
  DEBUG_SERIAL_INFORMATION.print("\nESP Chip ID: ");
  uint32_t id = 0;
  for(int i=0; i<17; i=i+8) {
    id |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  DEBUG_SERIAL_INFORMATION.printf("%08X", id);
  DEBUG_SERIAL_INFORMATION.print("\t Chip Revision: ");
  DEBUG_SERIAL_INFORMATION.print(ESP.getChipRevision());
  DEBUG_SERIAL_INFORMATION.print("\t SDK Version: ");
  DEBUG_SERIAL_INFORMATION.print(ESP.getSdkVersion());
  DEBUG_SERIAL_INFORMATION.println("\n---------------------------------------");
  
  // Setup Hardware Pinmode
  BoardLED.setup();
  pinMode(encoderPinSW, INPUT_PULLUP);

  //Read Out Preferences Data from NVS
  systemParameters = readParameters();

  // Writing NVS Called Parameters Back to Class Instance
  myStepper.setAcceleration(systemParameters.maxStepperAccel);
  myStepper.setMaxSpeed(systemParameters.maxStepperSpeed);

  // Setup OLED Display with Startup-Screen
  OLEDSetup();

  //keyboard_report.reportId = 0x02;
  //consumer_Report.reportId = 0x01;
  xTaskCreatePinnedToCore(updateOLEDTask, "OLEDUpdate", 30000, &moduloValue, 5, &oledTaskHandler, 0);
  xTaskCreatePinnedToCore(taskServer, "BLEServer", 20000, NULL, 6, &serverTaskHandler, 1);

  DEBUG_SERIAL.println("--- setup() - DONE ---");
} //setup()

/*-----------------------------------------------------------------------------
 * Global Loop() Function Call
 * ----------------------------------------------------------------------------
*/
void loop()
{
  // Encoder "Working" - Function Call with FASTRead Operation
  encoder.tick(digitalFASTRead(encoderPinB), digitalFASTRead(encoderPinA));
  
  // Encoder Position/Direction Read-Back
  long newPos = encoder.getPosition();
  int newDir = (int)encoder.getDirection();

  // Check Automatic Mode of Turning Turntable is Triggered/Active or not
  automaticMode();

  // Calling Steper-Run-Function often to avoid stocking Stepper
  runStepper(StartAutoMode);

  // Check, if Screenshot is Polled by Automatic Mode or by Menu
  if (HitPicture && connected && (millis() > AsyncBLEBreak + systemParameters.pictureDelay))
  {

    HitPicture = false;
    AsyncBLEBreak = millis();

    toggleScreenshot();

  }

  // Update Display due to Encoder's new Position
  if (newPosition != newPos)
  {
    newPosition = newPos;

    // Get delay Time between Rotation for Acceleration Mode of Encoder
    unsigned long ms = encoder.getMillisBetweenRotations();

    // Limiting Encoder Position ONLY in MAX Menu Items and if no Parameter has to be set
    // Except during Parameter SETUP Mode
    if ((newPosition > 0) || (getMenuSelected() == MENUACCSETTING) || (getMenuSelected() == MENUSPEEDSETTING) || (getMenuSelected() == MENUPICDELAYSETTING)){
      if((getMenuSelected() != MENUACCSETTING) && (getMenuSelected() != MENUSPEEDSETTING) && (getMenuSelected() != MENUPICDELAYSETTING)){
        moduloValue = newPosition % getMaxMenuCount();
      }else{

        // Get Encoder Direction and Set POS/NEG Value to Encoder + Calculation value
        if((Dir_old != newDir) && (newDir != 0)){
          if(newDir < 0){
            encoder.setPosition(-1);
            newPosition = -1;
            //DEBUG_Serial.println("Change NEG Direction!");
          }else if(newDir > 0){
            encoder.setPosition(1);
            newPosition = 1;
            //DEBUG_Serial.println("Change POS Direction!");
          }
          Dir_old = newDir;          
        }
        // Set Position only to +/- 1 to avoid incremental +/- calculation
        if(newPosition > 1){
          encoder.setPosition(1);
          newPosition = 1;
        }else if(newPosition < -1){
          encoder.setPosition(-1);
          newPosition = -1;
        }

        // Encoder Value Manipulation based on +/-1 and direct
        // Value Overload to function.
        long tempNewPosition = newPosition;
        EncoderAcceleration(ms, &tempNewPosition, newDir);        

        moduloValue = tempNewPosition;

      }
    }
    else{
      moduloValue = 0;
      newPosition = 0;
      encoder.setPosition(0);
    }
    //DEBUG_Serial.println(moduloValue);

    vTaskResume(oledTaskHandler);
  }

  // Get Button State of Encoder by FASTRead Function
  bool CurrentButtonState = !digitalFASTRead(encoderPinSW);

  // Debouncing and Check Button State of Encoder Button if Changed
  if ((CurrentButtonState != encoderButtonPressed) && ((millis() - lastDebounceTime) > debounceDelay))
  {
    // Update Debounce Time and Button State
    lastDebounceTime = millis();
    encoderButtonPressed = CurrentButtonState;
    //DEBUG_Serial.println("Encoder Button pressed!");

    if(encoderButtonPressed){
      bool spacingTriggerd = false;

      // Calling Menu Update Function and Interact with Pointer-Variables and Main Loop Functions
      MenuSelector(moduloValue, &spacingTriggerd, &systemParameters.steppCount, &StartAutoMode, &HitPicture, &way, &StepperRunning, &infiniteMode);

      if (!spacingTriggerd) 
      {
        // If not Menu Spacing have been triggerd, update OLED Screen and Execute Menu Item
        //DEBUG_Serial.println("Another Task Resum ;)");
        newPosition = 0;
        moduloValue = 0;
        encoder.setPosition(0);
        vTaskResume(oledTaskHandler);
      };
    }
  }

} // loop()

/*-----------------------------------------------------------------------------
 * toggleScreenshot Function: Triggers a connected device for making a Screenshot
 * ----------------------------------------------------------------------------
*/
void toggleScreenshot(void){
  static bool volDirUp = true;

  inputKeyboard_t a{};
  a.Key = random(0x02,0x27);
  //   a.reportId = 0x02;
  input->setValue((uint8_t*)&a,sizeof(a));
  input->notify();

  input->setValue((uint8_t*)(&keyboard_report), sizeof(keyboard_report));
  input->notify();

  inputConsumer_t b{};
  //   b.reportId = 0x01;

  DEBUG_SERIAL.println("[BLE] CHEEEEESSSSSEEEEEE");
  b.ConsumerControl = volDirUp ? 0xE9 : 0xEA;
  volDirUp = volDirUp ? false : true;
  inputVolume->setValue((uint8_t*)&b,sizeof(b));
  inputVolume->notify();   

  inputVolume->setValue((uint8_t*)&consumer_Report,sizeof(consumer_Report));
  inputVolume->notify();  
} // toggleScreenshot()

/*-----------------------------------------------------------------------------
 * Update Steptyp to turn specific angle for Screenshot
 * ----------------------------------------------------------------------------
*/
uint8_t updateSteps(long _stepps)
{
  if (((TURNFULL - roundingWindow) <= _stepps) && (_stepps <= (TURNFULL + roundingWindow))){
    return (uint8_t)1;
  }else if (((TURNHALF - roundingWindow) <= _stepps) && (_stepps <= (TURNHALF + roundingWindow))){
    return (uint8_t)2;
  }else if (((TURNQUART - roundingWindow) <= _stepps) && (_stepps <= (TURNQUART + roundingWindow))){
    return (uint8_t)4;
  }else if (((TURNEIGHT - roundingWindow) <= _stepps) && (_stepps <= (TURNEIGHT + roundingWindow))){
    return (uint8_t)8;
  }else if (((TURNSIXTE - roundingWindow) <= _stepps) && (_stepps <= (TURNSIXTE + roundingWindow))){
    return (uint8_t)16;
  }else if (((TURNTHIRT - roundingWindow) <= _stepps) && (_stepps <= (TURNTHIRT + roundingWindow))){
    return (uint8_t)32;
  }else if (((TURNSIXTY - roundingWindow) <= _stepps) && (_stepps <= (TURNSIXTY + roundingWindow))){
    return (uint8_t)64;
  }else{
    return 0;
  };
} // updateSteps()

/*-----------------------------------------------------------------------------
 * runStepper(): Run Stepper to Position -->> Called in LOOP function
 * ----------------------------------------------------------------------------
*/
void runStepper(bool _autoModeActive){
  
  // Check if Stepper Parameter maxStepperAcceleration has change during runtime
  if(myStepper.getAcceleration() != systemParameters.maxStepperAccel){
    myStepper.setAcceleration(systemParameters.maxStepperAccel);
  }

  // Check if Stepper Parameter maxStepperSpeed has change during runtime
  if(myStepper.getMaxSpeed() != systemParameters.maxStepperSpeed){
    myStepper.setMaxSpeed(systemParameters.maxStepperSpeed);
  }

  // Infinite Turning in positive or negative direction
  // with constant parameters to "speed-up" things
  if(infiniteMode == 0){
    // Do Nothing/STOP Infinite Mode
    //DEBUG_SERIAL.println("0");
  }else if((infiniteMode == 1) && StepperRunning){
    // Run Stepper Positive to Infinite
    myStepper.setMaxSpeed(defaultMAXSPEEDSTEPPER);
    myStepper.setAcceleration(defaultACCELERATIONSTEPPER);
    way = 2147483647;
    StepperRunning = false;
    //DEBUG_SERIAL.println("1");
  }else if((infiniteMode == 2) && StepperRunning){
    // Run Stepper Negative to Infinite
    myStepper.setMaxSpeed(defaultMAXSPEEDSTEPPER);
    myStepper.setAcceleration(defaultACCELERATIONSTEPPER);
    way = -2147483647;
    StepperRunning = false;
    //DEBUG_SERIAL.println("2");
  }else if(infiniteMode == 3){
    // Reset Values
    way = 0;
    StepperRunning = false;
    infiniteMode = 0;
  }

  if (!StepperRunning){
      StepperRunning = myStepper.runToPosition(way, _autoModeActive); 
  }   

} // runStepper()

/*-----------------------------------------------------------------------------
 * updateOLEDTask(): Taks Function to Update OLED if a change was done
 * ----------------------------------------------------------------------------
*/
void updateOLEDTask(void* _value){
  while(1){
    long* moduloValuePtr = (long*)_value; // Casten Sie den Parameter _value zum Typ int*
    long _moduloValue = *moduloValuePtr;  // Holen Sie den Wert von moduloValue aus dem übergebenen Pointer
 
    OLEDMainMenu(_moduloValue, &systemParameters.maxStepperSpeed, &systemParameters.maxStepperAccel);
    vTaskSuspend(NULL);
  }
} // updateOLEDTask()

/*-----------------------------------------------------------------------------
 * automaticMode(): Automatic Mode for Turntable + Picture Mode
 * ----------------------------------------------------------------------------
*/
void automaticMode(void)
{
  uint8_t _maxStepps = updateSteps(systemParameters.steppCount);
  // If Started:
  switch (stepAutoMode)
  {
  case STEPDONE:
    if (StartAutoMode)
    {
      // Starting StateFlow Machine Here
      stepAutoMode = STEPPICTURE;
      CountStepsAuto = 0;
      DEBUG_SERIAL.println("[AUTO] -Step 10- Auto Mode Started");
    }
    break;

  case STEPPICTURE:
    // Start with a Picture
    HitPicture = true;
    AsyncAutoModeBreak = millis();
    stepAutoMode = STEPPICBREAK;
    BoardLED.writeOutput(HIGH);
    DEBUG_SERIAL.println("[AUTO] -Step 20- Take Picture");
    if (!StartAutoMode)
    {
      stepAutoMode = STEPPSTOPJOB;
    }
    break;

  case STEPPICBREAK:
    // Waiting Until Picture Delay is done
    if ((millis() > AsyncAutoModeBreak + autoModeDelay))
    {
      stepAutoMode = STEPPERMOVE;
      BoardLED.writeOutput(LOW);
      DEBUG_SERIAL.println("[AUTO] -Step 30- Picture Break");
    }
    if (!StartAutoMode)
    {
      stepAutoMode = STEPPSTOPJOB;
    }
    break;

  case STEPPERMOVE:
    // Moving Stepper Forward to Position
    way = systemParameters.steppCount;
    StepperRunning = false;
    AsyncAutoModeBreak = millis();
    stepAutoMode = STEPPBREAK;
    DEBUG_SERIAL.println("[AUTO] -Step 40- Move Stepper");
    if (!StartAutoMode)
    {
      stepAutoMode = STEPPSTOPJOB;
    }
    break;

  case STEPPBREAK:
    // Stepper Break after Movement
    if ((millis() > AsyncAutoModeBreak + autoModeDelay))
    {
      if (!myStepper.isStepperRunning() || myStepper.stepperPosition() >= way)
      {
        CountStepsAuto++;
        stepAutoMode = STEPCHECK;
        DEBUG_SERIAL.println("[AUTO] -Step 50- Stepper in Position");
      }
    }
    if (!StartAutoMode)
    {
      stepAutoMode = STEPPSTOPJOB;
    }
    break;

  case STEPCHECK:
    // checking if Rotation is done, or repeat is necessary
    if (CountStepsAuto >= _maxStepps)
    {
      DEBUG_SERIAL.print("[AUTO] Max Counts: ");
      DEBUG_SERIAL.print(_maxStepps);
      DEBUG_SERIAL.print("\t Current Counts: ");
      DEBUG_SERIAL.println(CountStepsAuto);
      DEBUG_SERIAL.println("[AUTO] -Step 60- Checking Rotation");

      CountStepsAuto = 0;
      StartAutoMode = false;
      stepAutoMode = STEPDONE;

      //Deactivate Stepper here
      myStepper.motorStop();
    }
    else
    {
      stepAutoMode = STEPPICTURE;
      DEBUG_SERIAL.println("[AUTO] -Step 60- Checking Rotation");
    }
    if (!StartAutoMode)
    {
      stepAutoMode = STEPPSTOPJOB;
    }
    break;

  case STEPPSTOPJOB:
    // Stopp Mode Condition
    CountStepsAuto = 0;
    StartAutoMode = false;
    stepAutoMode = STEPDONE;
    BoardLED.writeOutput(LOW);  // Reset Board LED
    //Deactivate Stepper here
    myStepper.motorStop();
    DEBUG_SERIAL.println("[AUTO] -Step 99- Stopp AUTO Mode");
    break;

  default:
    stepAutoMode = STEPPSTOPJOB; // Do Nothing here
    break;
  }
} // automaticMode()

/*-----------------------------------------------------------------------------
 * EncoderAcceleration(): Acceleration Calculation to speed up movement of change parameters
 * ----------------------------------------------------------------------------
*/
void EncoderAcceleration(unsigned long _ms, long* _newPos, int Dir){
  if (_ms < longCutoff) {
    // do some acceleration using factors a and b

    // limit to maximum acceleration
    if (_ms < shortCutoff) {
      _ms = shortCutoff;
    }

    *_newPos = *_newPos + round((Dir*(a * _ms + b)));
  }
} // EncoderAcceleration()