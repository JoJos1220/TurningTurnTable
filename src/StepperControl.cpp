/***************************TurningTurnTable Project by Jojo1220************************************
  Filename:     StepperControl.cpp
  Purpose:      See StepperControl.h
  Autor:        Jojo1220
  Created:      05.06.2024
  Modified:     05.06.2024
  Lizenz:       CC BY-NC-ND 4.0
  Notification: See StepperControl.h
******************************************v1.0.0****************************************************/

#include "StepperControl.h"

/*-----------------------------------------------------------------------------
 * Class: StepperControl::StepperControl() Constructor Function (4Pins only)
 * ----------------------------------------------------------------------------
*/
StepperControl::StepperControl(int Pin1, int Pin2, int Pin3, int Pin4){
    _initStepper(Pin1, Pin2, Pin3, Pin4);
};

/*-----------------------------------------------------------------------------
 * Class: StepperControl::StepperControl() Constructor Function (4Pins + Speed&Acceleration Parameters)
 * ----------------------------------------------------------------------------
*/
StepperControl::StepperControl(int Pin1, int Pin2, int Pin3, int Pin4, float maxSpeed, float Acceleration){
    _initStepper(Pin1, Pin2, Pin3, Pin4);
    setMaxSpeed(maxSpeed);
    setAcceleration(Acceleration);
    DEBUG_SERIAL.print("[Stepper] Initialized! \tSpeed: ");
    DEBUG_SERIAL.print(maxSpeed);
    DEBUG_SERIAL.print("\tAcceleration: ");
    DEBUG_SERIAL.println(Acceleration);
}

/*-----------------------------------------------------------------------------
 * Class: StepperControl::~StepperControl() Destructor Function
 * ----------------------------------------------------------------------------
*/
StepperControl::~StepperControl(){
    _Stepper->~AccelStepper();
    delete _Stepper;
};

/*-----------------------------------------------------------------------------
 * Class: StepperControl::runToPosition() Run to Position
 * ----------------------------------------------------------------------------
*/
bool StepperControl::runToPosition(long absolut, bool processActive){

    // Checking, if loaded Position is unequal private Class Position
    // If so, enable Stepper Movement to new Position
    
    if(_positionNEW != absolut){
        _Stepper->enableOutputs();
        _Stepper->setCurrentPosition(0);
        _Stepper->moveTo(absolut);
        _positionNEW = absolut;
    }

    // State Machine: 
    //      First, Acceleration Phase
    //      Second, Constant Movement Phase
    //      Third, STOP Stepper Movement & Disable Outputs
    if((_positionNEW > stepperPosition() && _positionNEW >= 0) || (_positionNEW < stepperPosition() && _positionNEW < 0)){
        
        if((_Stepper->speed() >= _maxSpeed && _positionNEW >= 0) || (abs(_Stepper->speed()) >= _maxSpeed && _positionNEW <0)){
            _state = RSPD;
        }

        switch (_state){
            case RSPD:
                _Stepper->runSpeed();
                break;
            case RJSTR:
                _Stepper->run();
                break;
            default:
                break;
        }

        return false;
    }else{
        _state = RJSTR;
        _positionNEW = 0;

        if(!processActive){
            // Disable Stepper Outputs if NOT Process is active to avoid
            // Uncontrolled movement during Automatic Process and loss of Steps!
            _Stepper->disableOutputs();
        }
        return true;
    }
};

/*-----------------------------------------------------------------------------
 * Class: StepperControl::motorStop() Stop Stepper Movement and disable Output
 * ----------------------------------------------------------------------------
*/
void StepperControl::motorStop(void){
    _Stepper->stop();
    _Stepper->disableOutputs();   
}

/*-----------------------------------------------------------------------------
 * Class: StepperControl::isStepperRunning() Checking if Stepper is Running
 * ----------------------------------------------------------------------------
*/
bool StepperControl::isStepperRunning(void){
    return _Stepper->isRunning();
}

/*-----------------------------------------------------------------------------
 * Class: StepperControl::stepperPosition() Get current Stepper Position
 * ----------------------------------------------------------------------------
*/
long StepperControl::stepperPosition(void){
    return _Stepper->currentPosition();
}

/*-----------------------------------------------------------------------------
 * Class: StepperControl::setMaxSpeed() Set Max Stepper Speed
 * ----------------------------------------------------------------------------
*/
void StepperControl::setMaxSpeed(float speed){
    _Stepper->setMaxSpeed(speed);
    _maxSpeed = speed;
}

/*-----------------------------------------------------------------------------
 * Class: StepperControl::getMaxSpeed() Get current Max Speed
 * ----------------------------------------------------------------------------
*/
float StepperControl::getMaxSpeed(void){
    return _maxSpeed;
}

/*-----------------------------------------------------------------------------
 * Class: StepperControl::setAcceleration() set Stepper Acceleration
 * ----------------------------------------------------------------------------
*/
void StepperControl::setAcceleration(float acceleration){
    _Stepper->setAcceleration(acceleration);
    _Acceleration = acceleration;
}

/*-----------------------------------------------------------------------------
 * Class: StepperControl::getAcceleration() Get current Acceleration
 * ----------------------------------------------------------------------------
*/
float StepperControl::getAcceleration(void){
    return _Acceleration;
}

/*-----------------------------------------------------------------------------
 * Class: StepperControl::_initStepper() Stepper Motor Initialisation Function
 * ----------------------------------------------------------------------------
*/
void StepperControl::_initStepper(int Pin1, int Pin2, int Pin3, int Pin4){
    // Create NEW AccelStepper Class Object
    #if StepperOperation == 0
        _Stepper = new AccelStepper(AccelStepper::FULL4WIRE, Pin1, Pin2, Pin3, Pin4, false);
    #elif StepperOperation == 1
        _Stepper = new AccelStepper(AccelStepper::HALF4WIRE, Pin1, Pin2, Pin3, Pin4, false);
    #elif StepperOperation == 2
        _Stepper = new AccelStepper(AccelStepper::DRIVER, Pin2, Pin1);

        // Setup Driver Enable Pin
        _Stepper->setEnablePin(Pin3);
        _Stepper->setPinsInverted(false, false, true);

        // Setup Remaining Pin as Output and Set it permanent to LOW
        pinMode(Pin4, OUTPUT);
        digitalWrite(Pin4, LOW);

    #endif   
    
    // Stopp Stepper after Restart or after Initialisation
    _Stepper->stop();

    //Init Stepper Position to "0" after Initialization
    _Stepper->setCurrentPosition(0); 
    
    DEBUG_SERIAL.println("[Stepper] Init Done!");
}
