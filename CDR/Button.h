#include "Params.h"

class Button{
  private:
    enum State {Stable, Unstable};
    uint8_t pos; //Previous button state for Stable State
    uint8_t tempPos; //Previous button state for unstable State
    unsigned long nextTime; //Timer end time storage
  public:
    uint8_t pinNumber; //Button pin
    int timerDuration; //Duration of debouce timer
    State state; //State for main state machine
    
    Button(){
      state = Stable;
      pinNumber = BT_PinNum;
      timerDuration = 10; //hard coded debouce time
    }
    
    void Init(){
      pinMode(pinNumber,INPUT_PULLUP);
      nextTime = millis(); //just to avoid unintlialized next time value
    }

    bool CheckButtonPress(){
        bool returnVal = false; //value to return at the end
        unsigned long now = millis();
        uint8_t currButton = digitalRead(pinNumber); //read button only once
        switch(state){
          
          case(Stable):
            if(pos != currButton){ //When there is a change in the button
              state = Unstable; 
              tempPos = currButton; //store this as the base state for unstable state
              nextTime = now + timerDuration; //start timer
            }
            break;
            
          case(Unstable):
            if(tempPos != currButton){ //if the button changes again in the ustable state
              nextTime = now + timerDuration; //reset timer
              tempPos = currButton; //update prev position
            }
            else if(now > nextTime){ //timer has run out
              returnVal = (pos == HIGH && tempPos == LOW); //only true if rising edge
              pos = currButton; //update ending button state for stable state
              state = Stable;
            }
            break;
        }
        return returnVal;
    }

};


