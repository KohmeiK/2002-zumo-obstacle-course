#include <Zumo32U4.h>
#include "MotorSpeedsStruct.h"
#include "Queue.h"
Queue<int> queue(100);

class LineTrack{
  private:
    //EDIT THESE TO TUNE:

    //(super important)put your zumo on a uniform colored surface, get the avearge and plug in
    const int SENSOR_IMBALANCE_OFFSET = 280; 
    //is your line brighter (whiter) than your surface?
    bool whiteLine = true;
    //PID constants
    float Kp = 0.03;
    float Ki = 0;
    float Kd = 0.024;

    //This is for line detection only - not line tracking (passed in from constructor)
    int threshold; 
    
    //(less important)plot your error, if its very noisy increase this value (up to 10 or 15 only) 
    const int NUMBER_OF_READINGS_TO_AVE = 3;
  
    Zumo32U4LineSensors lineSensors;
    #define NUM_SENSORS 5
    uint16_t lineSensorValues[NUM_SENSORS];
    bool useEmitters = true;
    const int baseSpeed = 6; //Depending on the curvyness of line you could increase
    int maxEffort = 30; //maybe increase if you have super sharp turns
    
  public:
    LineTrack(int thresh){
      threshold = thresh;
    }
    void Init(){
      lineSensors.initFiveSensors();
    }
    bool lineDetected(){
      lineSensors.read(lineSensorValues, useEmitters ? QTR_EMITTERS_ON : QTR_EMITTERS_OFF);
      int average = (lineSensorValues[0]+lineSensorValues[1]+lineSensorValues[2] +
      lineSensorValues[3] + lineSensorValues[4])/NUM_SENSORS;
      return(average < threshold);
    }
    MotorSpeeds calcSpeeds(){
      MotorSpeeds output;
      static unsigned lastTime;
      unsigned long now = millis();
      static double aveSum = 0;
      
      lineSensors.read(lineSensorValues, useEmitters ? QTR_EMITTERS_ON : QTR_EMITTERS_OFF);
      int innerError = (5*lineSensorValues[1])+(lineSensorValues[2])-(3*lineSensorValues[3]);

      if(queue.count() >= NUMBER_OF_READINGS_TO_AVE){
        aveSum -= (int)queue.pop();
      }
      aveSum += innerError;
      queue.push(innerError);

      float Error = ((float)aveSum/(float)NUMBER_OF_READINGS_TO_AVE)-SENSOR_IMBALANCE_OFFSET;

//      uncomment and open serial plotter
      Serial.println(Error); 
      
      static int sumError = 0;
      sumError += Error;
      static int lastError;
      int deltaT = now - lastTime;
      int derError = Error-lastError;

      float diff = Kp*(float)Error + Ki*(float)sumError + Kd*(float)derError;

      if(diff > maxEffort) diff = maxEffort;
      else if(diff < -maxEffort) diff = -maxEffort;

      if(whiteLine) diff = -diff;
      
      output.left = baseSpeed - (diff);
      output.right = baseSpeed + (diff);

      lastTime = now;
      lastError = Error;

      //Need more debug? Uncomment these
//      Serial.print(lineSensorValues[0]);
//      Serial.print('\t'); 
//      Serial.print(lineSensorValues[1]);
//      Serial.print('\t');
//      Serial.print(lineSensorValues[2]);
//      Serial.print('\t');
//      Serial.println(lineSensorValues[3]);
//      Serial.print('\t');
//      Serial.println(lineSensorValues[4]);
      return output;
    }
 
};

