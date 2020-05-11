#include "MotorSpeedsStruct.h"
#include "Params.h"
#include <Zumo32U4.h>

class KinematicTurn{
  private:
    double targetAngle;
    uint16_t lastTime;
    double wheelDiameter = WheelDiameter
    double wheelBase = WheelBase;
    double turningSpeed = TurningSpeed
    int leftPos = 0;
    int rightPos = 0;
    double distPerTick = DistPerTick;
    int updateInterval = UpdateInterval;
    Zumo32U4Encoders encodersTurn;
    MotorSpeeds output;
    float currentAngle = 0;
  public:
    //KinematicTurn(){}
    void Init(){
    }

    bool isFinished(){
      bool finished = abs(currentAngle) >= abs(targetAngle);
      if (finished)
      {
        encodersTurn.getCountsAndResetLeft();
        encodersTurn.getCountsAndResetRight();
      }
      return finished;
    }

    void startTurn(int angle){
      //update targetAngle
      targetAngle = angle;
      lastTime = millis();
      currentAngle = 0;
      leftPos = 0;
      rightPos = 0;
      encodersTurn.getCountsAndResetLeft();
      encodersTurn.getCountsAndResetRight();
      //Serial.println("starting turn to " + String(targetAngle) + " from current angle " + String(currentAngle));
    }

    MotorSpeeds calcSpeeds(){  
      calculateAngle();
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
      if (currTime - lastTime > updateInterval) 
      {
        double timeChange = float(currTime - lastTime) / 1000.;
        lastTime = currTime;
        int currPosRight = encodersTurn.getCountsRight();
        int currPosLeft = encodersTurn.getCountsLeft();
        double speedRight = double(currPosRight - rightPos) * distPerTick / timeChange; // in/sec
        double speedLeft = double(currPosLeft - leftPos) * distPerTick / timeChange; // in/sec
        double angularVelocity = (speedRight - speedLeft) / wheelBase * 180 / PI; // deg/sec
        //Serial.println("prev Angle" + String(currentAngle));
        currentAngle += angularVelocity * timeChange; //degrees
        leftPos = currPosLeft;
        rightPos = currPosRight;
        //Serial.println("next Angle" + String(currentAngle));
      }
      return currentAngle;
    }
};
