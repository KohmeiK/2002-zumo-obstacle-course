#include <Wire.h>
#include <Zumo32U4.h>

LSM303 compass;
L3G gyro;

const bool debug = false;
class Filter{
  public:
  
  float k; //kappa
  float e; //epsilon
  unsigned long lastTime; //for detla T calcs

  float estimatedAngle = 0; //Theta Hat Prime

  //Bias stuff
  float lastXAcc;
  float accXoffset = 0;
  float gyroBias = 0;

  Filter(float k,float e){
    this->k = k;
    this->e = e;
  }
  
  void Init(){
    lastTime = millis();
    Wire.begin();
    if (!compass.init()){
      // Failed to detect the compass.
      ledRed(1);
      while(1){
        Serial.println(F("Failed to detect the compass."));
        delay(100);
      }
    }
  
    compass.enableDefault();
    compass.writeReg(LSM303::CTRL1, 0x67); //update refreshrate/sensitivty
    
    if (!gyro.init()){
      // Failed to detect the gyro.
      ledRed(1);
      while(1){
        Serial.println(F("Failed to detect gyro."));
        delay(100);
      }
    }
    gyro.enableDefault();
    gyro.writeReg(L3G::CTRL1, 0xBF); //update refreshrate/sensitivty
    gyro.writeReg(L3G::CTRL4, 0x10);
    
    if(debug){
      while(!Serial){} //wait for the Serial Monitor
      uint8_t ctrl2 = compass.readReg(LSM303::CTRL1);
      Serial.print("ACC->CTRL1 is: ");
      Serial.println(ctrl2, HEX);
      uint8_t ctrl1 = gyro.readReg(L3G::CTRL1);
      Serial.print("GYRO->CTRL1 is: ");
      Serial.println(ctrl1, HEX);
      uint8_t ctrl3 = gyro.readReg(L3G::CTRL4);
      Serial.print("GYRO->CTRL4 is: ");
      Serial.println(ctrl3, HEX);
    }
  }

  bool CalcAngle(float& est){

    //figure out is compass has new data
    bool retrunVal = false;
    uint8_t statusByte = compass.readReg(LSM303::STATUS_A);
    uint8_t statusBit = 0b00001000;
    
    if(statusByte & statusBit == 8){ //new data available
      unsigned long now = millis();
      retrunVal = true;
      compass.readAcc();
      gyro.read();

      //output this data to main file
      lastXAcc = compass.a.x;
      
      
      //(1)Prediction
      unsigned long deltaT = now - lastTime;
      float r = (gyro.g.y*17.5/57296); //convert to Rad/sec
      estimatedAngle += (deltaT*0.001)*(r-gyroBias); // Rad/sec / sec gives rad
      lastTime = now; 

      //(2)Observation
      float observedAngle = atan2(-(compass.a.x-accXoffset),compass.a.z);

      //(4) Update Bias
      gyroBias += e*(estimatedAngle-observedAngle);
      
      //(3)Correction
      estimatedAngle = k*estimatedAngle + (1.0-k)*observedAngle;
      est = estimatedAngle;

    }
    return retrunVal;
  }
  
};
