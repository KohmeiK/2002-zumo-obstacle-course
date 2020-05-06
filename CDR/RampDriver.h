#include "MotorSpeedsStruct.h"
#include <Zumo32U4.h>;

class RampDriver{
  private:
    float rampAngle;
    float currentAngle;
    bool climbedBefore = false;
  public:
    //RampDriver(){}
    void Init(){
      
    }

    bool isFinished(){
      //check if the robot has climbed the ramp
    }

    MotorSpeeds calcSpeeds(){
      MotorSpeeds output;
      output.left = 0; //solve for these
      output.right = 0;
      return output;
    }
    
};
