#include <Zumo32U4.h>

class IR
{
  private:
  bool frontOnly = true;
      
  public:  
  IR(bool useFrontOnly)
  {
    frontOnly = useFrontOnly;
  }
    
  void Init()
  {
    if(frontOnly)
    {
      // Enable pull-up for front sensor input only
      FastGPIO::Pin<SENSOR_FRONT>::setInputPulledUp();
    }
    else
    {
      // Enable pull-up resistors on all the sensor inputs.
      FastGPIO::Pin<SENSOR_LEFT>::setInputPulledUp();
      FastGPIO::Pin<SENSOR_RIGHT>::setInputPulledUp();
      FastGPIO::Pin<SENSOR_FRONT>::setInputPulledUp();
    }
  }
    
  bool isDetecting(){
    //If any of the sensors are tripped then return true.
        if(frontOnly) return !FastGPIO::Pin<SENSOR_FRONT>::isInputHigh();
        else return !FastGPIO::Pin<SENSOR_LEFT>::isInputHigh() ||
                    !FastGPIO::Pin<SENSOR_RIGHT>::isInputHigh() ||
                    !FastGPIO::Pin<SENSOR_FRONT>::isInputHigh();
  }
};
