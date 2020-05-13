#include "MotorSpeedsStruct.h"
#include "Params.h"
#include <Zumo32U4.h>

class KinematicTurn{
  private:
    double targetAngle; //angle to be turned relative to starting angle
    uint16_t lastTime; //time of last run, used to calculate change in time from one position to the next
    double wheelDiameter = WheelDiameter //vertical height of tread
    double wheelBase = WheelBase; //center to center tread distance
    double turningSpeed = TurningSpeed //speed at which the wheels should turn
    int leftPos = 0; //last known left wheel position
    int rightPos = 0; //last known right wheel position
    double distPerTick = DistPerTick; //linear distance robot travels per encoder tick
    int updateInterval = KinematicTurnUpdateInterval; //how often the position and wheel speeds should be updated
    Zumo32U4Encoders encodersTurn; //encoder object for turning
    MotorSpeeds output; //object to store left and right target speeds
    float currentAngle = 0; //the current angle of the robot
  public:
    //KinematicTurn(){}
    void Init(){
    }
    
    //checks if the robot has finished turning
    bool isFinished(){
      bool finished = abs(currentAngle) >= abs(targetAngle); //checks if the robot has exceeded the target position
      if (finished) //if the turn is complete, reset the encoders and return true
      {
        encodersTurn.getCountsAndResetLeft();
        encodersTurn.getCountsAndResetRight();
      }
      return finished;
    }

    //sets the target angle, resets the encoders, and sets the starting time to begin turning
    void startTurn(int angle){ 
      targetAngle = angle;
      lastTime = millis();
      currentAngle = 0;
      leftPos = 0;
      rightPos = 0;
      encodersTurn.getCountsAndResetLeft();
      encodersTurn.getCountsAndResetRight();
      //Serial.println("starting turn to " + String(targetAngle) + " from current angle " + String(currentAngle));
    }

    //motor direction determined by positive or negative turning angle. Speed determined from params.h
    MotorSpeeds calcSpeeds(){  
      calculateAngle(); //updates current angle
      if (targetAngle > 0) 
      {
        output.left = -turningSpeed;
        output.right = turningSpeed;
      }
      else 
      {
        output.left = turningSpeed;
        output.right = -turningSpeed;
      }
      return output;
    }
    double calculateAngle() {
      uint16_t currTime = millis();
      if (currTime - lastTime > updateInterval) //if it is time to update the position
      {
        double timeChange = float(currTime - lastTime) / 1000.;//calculate the change in time
        lastTime = currTime;
        int currPosRight = encodersTurn.getCountsRight(); //get the new positions
        int currPosLeft = encodersTurn.getCountsLeft();
        double speedRight = double(currPosRight - rightPos) * distPerTick / timeChange; //calculate the linear speed of the right wheel in units of in/sec
        double speedLeft = double(currPosLeft - leftPos) * distPerTick / timeChange; //calculate the linear speed of the left wheel in units of in/sec
        double angularVelocity = (speedRight - speedLeft) / wheelBase * 180 / PI; //calculate the angular speed of the robot in units of deg/sec
        //Serial.println("prev Angle" + String(currentAngle));
        currentAngle += angularVelocity * timeChange; //Update the current angle by adding the angular velocity * change in time
        leftPos = currPosLeft; //store new left motor encoder position
        rightPos = currPosRight; //store new left motor encoder position
        //Serial.println("next Angle" + String(currentAngle));
      }
      return currentAngle;
    }
};
