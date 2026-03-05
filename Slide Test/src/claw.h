#ifndef CLAW_H
#define CLAW_H

#include <Arduino.h>
#include "servo32u4.h"   // Use the 32U4-specific library, NOT <Servo.h>

class Claw
{
    private:
        // Pin 6 uses Timer4 -- chassis.InititalizeChassis() MUST be called first!
        Servo32U4Pin5 clawServo;

        // Microsecond equivalents of the original degree values
        uint16_t openUs;   // was 70 degrees
        uint16_t closeUs;  // was 115 degrees

        int currentAngle;

    public:
        Claw(void);

        void Initialize();

        void Open();
        void Close();

        int GetPosition() const;

        bool checkPosition();
};

#endif
