#include "MotorSpeedsStruct.h"
#include "Params.h"

class WallFollow {
  private:
    int targetDist;
    int lastReading;
    uint16_t prevTime;
    uint16_t lastUpdateTime;
    int updateInterval = WallTrackUpdateInterval;

    int pinNum;
    int baseSpeed;

    float errorSum = 0;
    float Kp;
    float Kd;
    float Ki;
    float lastError = 0;
    float maxErrorSum = 0;

    bool leftWall = WF_isWallLeft;

    const int steeringMax = WF_MaxSteer;
  public:
    WallFollow() {
      pinNum = WF_SensorPort;
      Kp = WF_Kp;
      Kd = WF_Kd;
      Ki = WF_Ki;
      targetDist = WF_Distance;
      maxErrorSum = WF_MaxErrorSum;
      
      this->baseSpeed = WF_BaseSpeed;
    }
    void Init() {
      pinMode(pinNum, INPUT); //INPUT for IR sensor
    }
    void setTarget(int distCM) {
      targetDist = distCM;
    }
    MotorSpeeds calcSpeeds() {

      uint16_t currentUpdateTime = millis();
      if (currentUpdateTime - lastUpdateTime > updateInterval) 
        {
        lastUpdateTime = currentUpdateTime;
        
        //Conversion function from Analog Val to distance from LAB1
        float distanceCm = pow((analogRead(pinNum) / 2206.0), (-1 / 0.738));

//        Serial.println(distanceCm);
  
        //Serial.println(distanceCm);
        float error = targetDist - distanceCm;
  
        errorSum += error;

        if (errorSum > maxErrorSum) errorSum = maxErrorSum;
        if (errorSum < -maxErrorSum) errorSum = -maxErrorSum;
  
        uint16_t now = millis();
        uint16_t deltaTime = now - prevTime;
        prevTime = now;
  
        //Scale down deltaTime so D value is not crazy high
        float dir = (error - lastError) / (0.01 * deltaTime);
  
        lastError = error;
        float steering = Kp * error + Kd * dir + Ki * errorSum;
  
        if (steering > steeringMax) steering = steeringMax;
        else if (steering < -steeringMax) steering = -steeringMax;

        //Apply Deadband Compensation
        float localWFDeadband = WF_Deadband; //super weird, won't compile without this
        if(steering < localWFDeadband && steering > -localWFDeadband) steering = 0;
        
        if (leftWall) steering = -steering;
  
        MotorSpeeds output;
        output.left = baseSpeed - steering;
        output.right = baseSpeed + steering;

        //Serial.println
        return output;
      }
   }
};
