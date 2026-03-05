#pragma once

#include "chassis.h"
#include "claw.h"
#include "lift.h"
#include "slide.h"

class Robot
{
public:
    /**
     * robotState is used to track the current task of the robot. You will add new states as 
     * the term progresses.
     */
    enum ROBOT_STATE 
    {
        ROBOT_IDLE,
        START,
        ROBOT_DRIVE_TO_POINT,
        PREPARE_PICKUP,
        EXTEND_TO_BLOCK,
        CLOSE_CLAW,
        OPEN_CLAW,
        RETRACT_SLIDE,
        UPPIES,
        MORE_UPPIES,
        UPPIES_MAX,
        EXTEND_TO_PLACE,
        PLACE_BLOCK,
        GET_DOWN
    };
    ROBOT_STATE robotState = ROBOT_IDLE;


protected:
    /* Define the chassis*/
    Chassis chassis;
    BlueMotor motor;
    Slide slide;
    Claw claw;
    Lift lift;

    int blockIndex = 0;

    // For managing key presses
    String keyString;

    /**
     * For tracking current pose and the destination.
     */
    Pose currPose;
    Pose destPose;
    
public:
    Robot(void) {keyString.reserve(10);}
    void InitializeRobot(void);
    void RobotLoop(void);

protected:
    /* State changes */    
    void EnterIdleState(void);
    void EnterState(ROBOT_STATE s);

    // /* Navigation methods.*/
    void UpdatePose(const Twist& u);
    void SetDestination(const Pose& desti);
    void DriveToPoint(void);
    bool CheckReachedDestination(void);
    void HandleDestination(void);

    void Turn(bool right);
    void StopDrive();
    void DriveForward();

    // /* Lift methods.*/
    void Up(int stage);
    void Down(void);
    
    // /*Claw methods.*/
    void Open(void);
    void Close(void);

protected:
    static const int NUM_WAYPOINTS = 4;
    Pose waypoints[NUM_WAYPOINTS] = {
        {0, 20, 0},
        {10, 40, 0},
        {35, 40, 0},
        {45, 0, 0}
    };

    int waypointIndex = 0;
};
