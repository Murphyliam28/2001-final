#include "claw.h"

Claw::Claw(void){
    // Map original degree values to microseconds: map(deg, 0, 180, 1000, 2000)
    openUs  = 1000;  // was write(70)
    closeUs = 1300;  // was write(115)
    currentAngle = 70;
}

void Claw::Initialize(){
    // NOTE: chassis.InititalizeChassis() must be called before this to set up Timer4!
    // Also note: pin 6 shares OCR4D with pin 12 -- do NOT use both simultaneously.
    // Pin 6 also shares with the buzzer; cut the buzzer trace if needed.
    clawServo.attach();
    Open();
}

void Claw::Open(){
    clawServo.writeUs(openUs);
    currentAngle = 70;
}

void Claw::Close(){
    clawServo.writeUs(closeUs);
    currentAngle = 115;
}

int Claw::GetPosition() const{
    return currentAngle;
}
