#include <Zumo32U4.h>

class IR{
  public:
  //IR(){}
  void Init(){
    // Enable pull-up resistors on all the sensor inputs.
    FastGPIO::Pin<SENSOR_LEFT>::setInputPulledUp();
    FastGPIO::Pin<SENSOR_RIGHT>::setInputPulledUp();
    FastGPIO::Pin<SENSOR_FRONT>::setInputPulledUp();
  }
  bool isDetecting(){
    //I have no idea how to do this
    //reference remoteControll sample program
  }
};
