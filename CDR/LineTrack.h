#include <Zumo32U4.h>
#include "MotorSpeedsStruct.h"


class LineTrack{
  private:
    Zumo32U4LineSensors lineSensors;
    #define NUM_SENSORS 3
    uint16_t lineSensorValues[NUM_SENSORS];
    bool useEmitters = true;
    int threshold; 
  public:
    LineTrack(int thresh){
      threshold = thresh;
    }
    void Init(){
      lineSensors.initThreeSensors();
    }
    bool lineDetected(){
      //read with our without emitter (from sample program)
      lineSensors.read(lineSensorValues, useEmitters ? QTR_EMITTERS_ON : QTR_EMITTERS_OFF);
      int average = (lineSensorValues[0]+lineSensorValues[1]+lineSensorValues[2])/3;
      return(average < threshold);
    }
    MotorSpeeds calcSpeeds(){
      MotorSpeeds output;
      output.left = 0; //solve for these
      output.right = 0;
      return output;
    }
  

  
};

