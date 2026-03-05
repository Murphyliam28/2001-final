#ifndef SLIDE_H
#define SLIDE_H

#include <Arduino.h>
#include "servo32u4.h"   // Use the 32U4-specific library, NOT <Servo.h>

class Slide
{
    private:
        Servo32U4Pin12 slideServo;  // Pin 12 uses Timer3 -- no Chassis dependency

        // Target angles for a positional servo (0-180 degrees).
        // Tune these to match your physical slide endpoints.
        uint8_t extendedAngle;   // angle when slide is fully extended
        uint8_t retractedAngle;  // angle when slide is fully retracted

        // Estimated time (ms) for the servo to travel end-to-end.
        // Used by IsBusy() since positional servos have no position feedback.
        unsigned long travelTimeMs;

        bool isMoving;
        unsigned long moveStartTime;

    public:
        Slide(void);

        void Initialize();

        // Command the servo to the extended or retracted position.
        // The optional travelMs override lets callers supply a custom
        // travel-time estimate for IsBusy(); pass 0 to use the default.
        void StartExtend(unsigned long travelMs = 0);
        void StartRetract(unsigned long travelMs = 0);

        // Park the servo at the retracted (home) position.
        void Stop();

        // Poll this every loop iteration to clear the busy flag once the
        // estimated travel time has elapsed.
        void Update();

        // Returns true while the servo is estimated to still be moving.
        bool IsBusy() const;
};
#endif