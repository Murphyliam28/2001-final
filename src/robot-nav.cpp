/**
 * robot-nav.cpp is where you should put navigation routines.
 */

#include "robot.h"

void Robot::UpdatePose(const Twist& twist)
{
    /**
     * TODO: Add your FK algorithm to update currPose here.
     */

     
     double theta = currPose.theta + twist.omega;
     double theta_star = (currPose.theta + theta)/2;

     double x = currPose.x + twist.u*cos(theta_star);
     double y = currPose.y + twist.u*sin(theta_star);

#ifdef __NAV_DEBUG__
    TeleplotPrint("x", currPose.x);
    TeleplotPrint("y", currPose.y);
    TeleplotPrint("theta", currPose.theta);
#endif

    currPose.x = x;
    currPose.y = y;
    currPose.theta = theta;
}

/**
 * Sets a destination in the lab frame.
 */
void Robot::SetDestination(const Pose& dest)
{
    /**
     * TODO: Turn on LED, as well.
     */
    Serial.print("Setting dest to: ");
    Serial.print(dest.x);
    Serial.print(", ");
    Serial.print(dest.y);
    Serial.print('\n');

    destPose = dest;
    robotState = ROBOT_DRIVE_TO_POINT;
}

bool Robot::CheckReachedDestination(void)
{
    bool retVal = false;
    /**
     * TODO: Add code to check if you've reached destination here.
     */

    if ((abs((currPose.x-destPose.x)) <= 5) && abs((currPose.y-destPose.y) <= 5)){
        retVal = true;
    }


    return retVal;
}

void Robot::DriveToPoint(void)
{
    if(robotState == ROBOT_DRIVE_TO_POINT)
    {
        /**
         * TODO: Add your IK algorithm here. 
         */

        Serial.println(waypointIndex);
        double K_P = 6.0;
        double K_P_theta = 180;
        double error_x = destPose.x - currPose.x;
        double error_y = destPose.y - currPose.y;
        double error_theta = (atan2(error_y, error_x) - currPose.theta);
        if(error_theta >= PI){
            error_theta -= 2.0*PI;
        } else if (error_theta <= -PI) {
            error_theta += 2.0*PI;
        }
        error_theta *= K_P_theta; 
        double error_distance = sqrt(pow(error_x, 2.0) + pow(error_y,2.0));
        if(error_distance > 10){
            error_distance = 10;
        }
        error_distance *= K_P;

#ifdef __NAV_DEBUG__
        // Print useful stuff here.
#endif

        chassis.SetMotorEfforts((error_distance-error_theta), (error_distance+error_theta));
    }
}

//int poseState = 0;
/**

Pose pose1 = {10.0, 10.0, 0.0};
Pose pose2 = {10.0, 20.0, 0.0};
Pose pose3 = {30.0, 50.0, 0.0};
Pose pose4 = {12.0, 15.0, 0.0};



Pose coords[] = {pose1, pose2, pose3, pose4};

Pose getTargetPose(){
    return coords[poseState];
}

*/

void Robot::HandleDestination(void)
{
    /**
     * TODO: Stop and change state. Turn off LED.
     */
    chassis.Stop();

    // turn off the led

    if(waypointIndex < NUM_WAYPOINTS){
    
        //turn on led

        SetDestination(waypoints[waypointIndex]);

        waypointIndex++;
    }else{
        EnterIdleState();
    }
}




void Robot::StopDrive(){
    chassis.SetMotorEfforts(0,0);
}

void Robot::DriveForward(){
    chassis.SetMotorEfforts(100, 100);
}

/**
 * Things to do
 * 
 * Pick up off 2nd shelf then place on top
 * Pick up off floor and put on first shelf
 * Drive around the pole and end on other side
 * 
 * 
 * 
 */