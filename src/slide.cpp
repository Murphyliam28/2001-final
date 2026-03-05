#include "slide.h"

Slide::Slide(void){
    // Positional servo target angles (degrees, 0-180).
    // Tune extendedAngle and retractedAngle to your physical slide endpoints.
    extendedAngle  = 180;  // fully extended position
    retractedAngle = 0;    // fully retracted / home position

    // Default estimated travel time from one end to the other.
    // A typical 60 RPM servo needs ~500 ms for a 180-degree sweep; adjust as needed.
    travelTimeMs = 500;

    isMoving = false;
}

void Slide::Initialize(){
    slideServo.attach();   // Servo32U4Pin12::attach() takes no arguments
    Stop();                // park at retracted position on startup
}

// Helper: convert a 0-180 degree angle to a 1000-2000 us pulse width
static uint16_t angleToUs(uint8_t angle){
    return (uint16_t)map(angle, 0, 180, 1000, 2000);
}

void Slide::StartExtend(unsigned long travelMs){
    slideServo.writeUs(angleToUs(extendedAngle));
    moveStartTime = millis();
    // Use caller-supplied estimate if provided, otherwise keep the stored default
    // (travelMs == 0 means "use default")
    if(travelMs > 0) travelTimeMs = travelMs;
    isMoving = true;
}

void Slide::StartRetract(unsigned long travelMs){
    slideServo.writeUs(angleToUs(retractedAngle));
    moveStartTime = millis();
    if(travelMs > 0) travelTimeMs = travelMs;
    isMoving = true;
}

void Slide::Stop(){
    // Park at the retracted (home) angle; positional servos hold position automatically.
    slideServo.writeUs(angleToUs(retractedAngle));
    isMoving = false;
}

void Slide::Update(){
    if(isMoving){
        if(millis() - moveStartTime >= travelTimeMs){
            isMoving = false;   // servo has had enough time to reach its target
        }
    }
}

bool Slide::IsBusy() const{
    return isMoving;
}