#ifndef LIFT_H
#define LIFT_H

#include <Arduino.h>
#include "BlueMotor.h"

class Lift
{
    public:

        int targetStages[3] = {-4039, -4765, -6035};

        double bottom = 0;


    private: 

        BlueMotor motor;

        int stopCmd;
        int upCmd;
        int downCmd;

        bool isMoving;
        unsigned long moveStartTime;
        unsigned long moveDuration;

    public:

        Lift(void);

        void Initialize(void);

        void Up(unsigned long durationMs);

        void Down(unsigned long durationMs);

        void Update(void);

        void Stop(void);

        bool IsBusy() const;
};

#endif