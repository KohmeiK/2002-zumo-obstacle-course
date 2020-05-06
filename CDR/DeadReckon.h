#include <Zumo32U4.h>

Zumo32U4Encoders encoders3; //im guessing we want to do this
//with the average reading of both encoders

class DeadReckon{
  int distance;
  public:
  //DeadReckon(){}
  //bool calcSpeeds(){}
  //I dont think we need this, we don't constantly update speed
  void startDrive(int distance){
    //find target enc pos based on dist
    //update target for PIDvelo
  }
  bool isFinished(){
    //return true when enc target is reached
  }
  
};

