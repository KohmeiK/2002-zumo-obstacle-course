#include "MotorSpeedsStruct.h"
#include <Zumo32U4.h>;

//Zumo32U4Encoders encoders3; //im guessing we want to do this
//actaully I think we have to pass these encoders by ref
//I can't define them mutiple times in each file

class KinematicTurn{
  private:
    float currentAngle;
    float targetAngle;
  public:
    //KinematicTurn(){}
    void Init(){
      
    }

    bool isFinished(){
      //check if kenmatic based angle is close enough to target
    }

    void startTurn(int angle){
      //update targetAngle
    }

    MotorSpeeds calcSpeeds(){
      MotorSpeeds output;
      output.left = 0; //solve for these
      output.right = 0;
      return output;
    }
    
};

