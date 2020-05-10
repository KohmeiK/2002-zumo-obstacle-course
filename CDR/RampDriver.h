#include "MotorSpeedsStruct.h"
#include "Filter.h"
#include "Params.h"
#include <Zumo32U4.h>;

class RampDriver{
  private:
    float rampAngleDEG = RD_RampAngleDEG;
    float flatTolerance = RD_FlatAngleToleranceDEG;
    int baseSpeed = RD_BaseSpeed;
    float currentAngleRAD;
    float currentAngleDEG;
    bool climbedBefore = false;
    Filter filter = Filter(0.97,0.03);
  public:
    //RampDriver(){}
    void Init(){
      filter.Init();
    }

    bool isFinished(){
      return(abs(currentAngleDEG) < flatTolerance && climbedBefore);
    }

    MotorSpeeds calcSpeeds(){
      if(filter.CalcAngle(currentAngleRAD)){
        currentAngleDEG = currentAngleRAD * 180.0/3.14152;
//        Serial.println(currentAngleDEG);
        if(currentAngleDEG <= rampAngleDEG) climbedBefore = true;
      }
      MotorSpeeds output;
      output.left = baseSpeed;
      output.right = baseSpeed;
      return output;
    }
    
    
};
