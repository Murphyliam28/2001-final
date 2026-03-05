#include "robot.h"
#include "BlueMotor.h"
#include "lift.h"

Lift::Lift(){
    
    stopCmd = 0;
    upCmd = -370;
    downCmd = 370;

    isMoving = false;
}


void Lift::Initialize(){
    motor.setup();
    motor.setEffort(0);
}

void Lift::Up(unsigned long durationMs){
    motor.setEffort(-400);
    moveStartTime = millis();
    moveDuration = durationMs;
    isMoving = true;
}

void Lift::Down(unsigned long durationMs){
        motor.setEffort(400);
        moveStartTime = millis();
        moveDuration = durationMs;
        isMoving = true;
}

void Lift::Update(void){
    if(isMoving){
        if(millis() - moveStartTime >= moveDuration){
            Stop();
        }
    }
}

void Lift::Stop(){
    motor.setEffort(0);
    isMoving = false;
}

bool Lift::IsBusy() const{
    return isMoving;
}