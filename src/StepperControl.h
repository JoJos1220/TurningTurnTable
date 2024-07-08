/***************************TurningTurnTable Project by Jojo1220************************************
  Filename:     StepperControl.h
  Purpose:      StepperControl.h File of TurningTurnTable Project containing whole Stepper Interfacing functions
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

#ifndef STEPPERCONTROL_H
#define STEPPERCONTROL_H

/*-----------------------------------------------------------------------------
 * Including of Libary Direcotries
 * ----------------------------------------------------------------------------
*/
#include <AccelStepper.h>
#include "CustomDebugging.h"
#include "TurningTurnTablePARAMETERS.h"

/*-----------------------------------------------------------------------------
 * Setup Global Constants
 * ----------------------------------------------------------------------------
*/
// Definition of Stepper State Machine
#define RSPD    01  // RUN Stepper with Constant Speed
#define RJSTR   02  // RUN Stepper with Acceleration/Deacceleration Phase
#define STOPPED 03  // Stopped

/*-----------------------------------------------------------------------------
 * Setup custom StepperControl() -- Class
 * ----------------------------------------------------------------------------
*/
class StepperControl{
    public:
        StepperControl(int Pin1, int Pin2, int Pin3, int Pin4);
        StepperControl(int Pin1, int Pin2, int Pin3, int Pin4, float maxSpeed, float Acceleration);
        ~StepperControl();
        bool isStepperRunning(void);
        long stepperPosition(void);

        // Movement Function
        bool runToPosition(long absolut, bool processActive);
        void motorStop(void);

        // Setting Motor Parameters to µC
        void setMaxSpeed(float speed);
        float getMaxSpeed(void);
        void setAcceleration(float acceleration);
        float getAcceleration(void);

    private:
        void _initStepper(int Pin1, int Pin2, int Pin3, int Pin4);
        AccelStepper* _Stepper;
        float _maxSpeed = 0.0;
        float _Acceleration = 0.0;
        int _state = RJSTR;
        unsigned long __msec = millis();
        long _positionNEW = 0;
};

#endif //StepperControl.h