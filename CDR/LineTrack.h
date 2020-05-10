#include <Zumo32U4.h>
#include "MotorSpeedsStruct.h"
#include "Queue.h"
Queue<int> queue(100);

class LineTrack{
  private:
    Zumo32U4LineSensors lineSensors;
    #define NUM_SENSORS 5
    uint16_t lineSensorValues[NUM_SENSORS];
    bool useEmitters = true;
    int threshold; 
    const int baseSpeed = 6;
    int maxEffort = 50;
    float Kp = 0.02;
    float Ki = 0;
    float Kd = 0.008;
    bool whiteLine = true;
    
  public:
    LineTrack(int thresh){
      threshold = thresh;
    }
    void Init(){
      lineSensors.initFiveSensors();
    }
    bool lineDetected(){
      //read without emitter (from sample program)
      lineSensors.read(lineSensorValues, useEmitters ? QTR_EMITTERS_ON : QTR_EMITTERS_OFF);
      int average = (lineSensorValues[0]+lineSensorValues[1]+lineSensorValues[2] +
      lineSensorValues[3] + lineSensorValues[4])/NUM_SENSORS;
      return(average < threshold);
    }
    MotorSpeeds calcSpeeds(){
      MotorSpeeds output;
      lineSensors.read(lineSensorValues, useEmitters ? QTR_EMITTERS_ON : QTR_EMITTERS_OFF);
      static unsigned lastTime;
      unsigned long now = millis();
      int innerError = (5*lineSensorValues[1])+(lineSensorValues[2])-(3*lineSensorValues[3]);
//    int outerError = (lineSensorValues[0]-lineSensorValues[4])*2;
      static double aveSum = 0;
      if(queue.count() >= 3){
//        Serial.print("POP: ");
//        Serial.println((int)queue.peek());
        aveSum -= (int)queue.pop();
      }
      
      aveSum += innerError;
//      Serial.print("PUSH: ");
//      Serial.println(innerError);
      queue.push(innerError);

      float Error = (float)aveSum-1050;
      
      Serial.println(Error);
//      Serial.print('\t');
//      Serial.println(innerError);
      
      
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
//
////      Serial.print(lineSensorValues[0]);
//      Serial.print('\t'); 
//      Serial.print(lineSensorValues[1]);
//      Serial.print('\t');
////      Serial.print(lineSensorValues[2]);
////      Serial.print('\t');
//      Serial.println(lineSensorValues[3]);
////      Serial.print('\t');
//      Serial.println(lineSensorValues[4]);
//      
      return output;
    }
  

  
};

