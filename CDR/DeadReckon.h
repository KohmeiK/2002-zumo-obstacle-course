#include <Zumo32U4.h>
#include "MotorSpeedsStruct.h"

class DeadReckon{
  
  private:
  Zumo32U4Encoders enc; 
  MotorSpeeds targets;
  int targetDist;
  const double multiplier = DR_SpeedMutiplier;
  const float k_cmToEnc = 53.99325;
  
  public:
  
  //Constructor with encoders passed by reference
  DeadReckon(Zumo32U4Encoders encoders)
  {
   this->enc = encoders;
  }

  void setTarget(int distance_cm){
    //find target enc pos based on dist
    targetDist = encAvgCurrent() + (distance_cm * k_cmToEnc);       
  }

  MotorSpeeds startDrive(){
      targets.left = (80*multiplier); //Set constant drive speeds to 80/400 * multiplier
      targets.right = (80*multiplier);
      return targets;
    }

  bool isFinished(){
    return ((enc.getCountsLeft() >= targetDist) || (enc.getCountsRight() >= targetDist)); //Return true if either value has reached target
  }
  
  //Calculate Average Encoder Value
  float encAvgCurrent(){
    return ((enc.getCountsLeft() + enc.getCountsRight())/2); 
  }
};
