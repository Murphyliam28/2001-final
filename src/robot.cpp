#include "robot.h"
#include "Romi32U4MotorTemplate.h"

void Robot::InitializeRobot(void)
{
    // FIX: chassis MUST be initialized before claw (pin 12 needs Timer4 from Chassis)
    chassis.InititalizeChassis();
    
    slide = Slide();
    slide.Initialize();

    claw = Claw();
    claw.Initialize();

    lift.Initialize();

    delay(1500);
    waypointIndex =0;
    SetDestination(waypoints[waypointIndex]);

    motor.setEffort(150);
    robotState = START;
}

void Robot::EnterIdleState(void)
{
    chassis.Stop();

    Serial.println("-> IDLE");
    robotState = ROBOT_IDLE;
}

unsigned long stateStartMs = 0;

void Robot::EnterState(ROBOT_STATE s)
{
    robotState = s;
    stateStartMs = millis();
    Serial.println(s);
}

/**
 * The main loop for your robot. Process both synchronous events (motor control),
 * and asynchronous events (distance readings, etc.).
*/
void Robot::RobotLoop(void) 
{
    // FIX: Update() must be called every loop iteration for timed slide moves
    slide.Update();
    lift.Update();
    Twist velocity;
    if(robotState == ROBOT_DRIVE_TO_POINT){
        if (chassis.ChassisLoop(velocity))
        {
            UpdatePose(velocity);
        }
    }

    switch(robotState)
    {
        case ROBOT_IDLE:
            break;

        case START:
            claw.Open();
            slide.Stop();
            lift.Stop();
            EnterState(PREPARE_PICKUP);
            break;

        case PREPARE_PICKUP:
            claw.Open();
            EnterState(EXTEND_TO_BLOCK);
            break;

        case EXTEND_TO_BLOCK:
            if(!lift.IsBusy()){
                slide.StartExtend(1000);
                EnterState(CLOSE_CLAW);
            }
            break;

        case CLOSE_CLAW:
            if(!slide.IsBusy()){
                claw.Close();
                robotState = RETRACT_SLIDE;
                stateStartMs = millis(); // start settle timer
                EnterState(RETRACT_SLIDE);
            }
            break;

    // FIX: Uncommented RETRACT_SLIDE so the slide actually gets commanded
        case RETRACT_SLIDE:
            if(millis() - stateStartMs < 250) break;

            if(!slide.IsBusy()){
                slide.StartRetract(1500);
                if(blockIndex == 0) EnterState(UPPIES);
                else EnterState(UPPIES_MAX);
            }
            break;

        case UPPIES:
            if(!slide.IsBusy()){
                lift.Up(2500); 
                EnterState(EXTEND_TO_PLACE);
            }
            break;

        case MORE_UPPIES:
            if(!slide.IsBusy()){
                lift.Up(650);
                EnterState(EXTEND_TO_BLOCK);
            }
            break;

        case UPPIES_MAX:
            if(!slide.IsBusy()){
                lift.Up(700);
                EnterState(EXTEND_TO_PLACE);
            }
            break;
        
        case EXTEND_TO_PLACE:
            if(!lift.IsBusy()){
                slide.StartExtend(1500);
                stateStartMs = millis();
                EnterState(PLACE_BLOCK);
            }
            break;

        case PLACE_BLOCK:

            if(slide.IsBusy()) break;
            
            if(millis() - stateStartMs < 250) break;
            claw.Open();
            slide.StartRetract(1200);

            
            if(blockIndex == 0){
                blockIndex++;
                EnterState(MORE_UPPIES);
            }
            else{
                EnterState(GET_DOWN);
            }
            
            break;

        case GET_DOWN:
            if(!slide.IsBusy()){
                lift.Down(2500);
                EnterState(ROBOT_DRIVE_TO_POINT);
            }
            break;

    

        case ROBOT_DRIVE_TO_POINT:
            DriveToPoint();
            if(CheckReachedDestination()) HandleDestination();
            break;

    }
}