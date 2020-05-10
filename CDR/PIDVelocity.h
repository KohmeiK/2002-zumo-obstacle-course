#include "MotorSpeedsStruct.h"
#include <Zumo32U4Motors.h>
#include <Zumo32U4Encoders.h>
Zumo32U4Motors motors;
Zumo32U4Encoders encoders;

volatile uint8_t readyToPID = 0;
volatile int16_t countsLeft = 0;
volatile int16_t countsRight = 0;

ISR(TIMER4_OVF_vect)
{
  //Capture a "snapshot" of the encoder counts for later processing
  countsLeft = encoders.getCountsLeft();
  countsRight = encoders.getCountsRight();

  readyToPID = 1;
}

class PIDVelocity {
  private:
    float Kp;
    float Ki;
    float Kd;
    int intergrealCap;
    float targetLeft = 0;
    float targetRight = 0;
    uint16_t prevNow;
    bool invertLeft;
  public:
    bool enable = true;

    PIDVelocity() {
      Kp = PV_Kp;
      Ki = PV_Ki;
      Kd = PV_Kd;
      intergrealCap = PV_IntergeralCap;
      invertLeft = InvertLeft;
    }

    void Init() {
      noInterrupts(); //disable interupts while we mess with the Timer4 registers

      //sets up timer 4
      TCCR4A = 0x00; //disable some functionality -- no need to worry about this
      TCCR4B = 0x0C; //sets the prescaler 2048
      TCCR4C = 0x04; //toggles pin 6 at one-half the timer frequency
      TCCR4D = 0x00; //normal mode

      OCR4C = 129;   //TOP goes in OCR4C
      TIMSK4 = 0x04; //enable overflow interrupt

      interrupts(); //re-enable interrupts
    }

    void setTargets(MotorSpeeds target) {
      targetLeft = target.left;
      targetRight = target.right;
    }

    void update() {
      if (readyToPID && enable) //timer flag set
      {
        uint16_t now = millis();

        //clear the timer flag
        readyToPID = 0;

        //for tracking previous counts
        static int16_t prevLeft = 0;
        static int16_t prevRight = 0;
        static int16_t prevLeftError = 0;
        static int16_t prevRightError = 0;

        //error sum
        static int16_t sumLeft = 0;
        static int16_t sumRight = 0;

        noInterrupts();
        int16_t speedLeft = countsLeft - prevLeft;
        int16_t speedRight = countsRight - prevRight;

        prevLeft = countsLeft;
        prevRight = countsRight;
        interrupts();

        int16_t errorLeft = targetLeft - speedLeft;
        sumLeft += errorLeft;
        int16_t errorRight = targetRight - speedRight;
        sumRight += errorRight;

        int timeDelta = now - prevNow;
        prevNow = now;

        float derLeft = (float)(errorLeft - prevLeftError) / timeDelta;
        float derRight = (float)(errorRight - prevRightError) / timeDelta;
        prevLeftError = errorLeft;
        prevRightError = errorRight;

        if (sumLeft > intergrealCap) sumLeft = intergrealCap;
        if (sumRight > intergrealCap) sumRight = intergrealCap;

        float effortLeft = Kp * errorLeft + Ki * sumLeft + Kd * derLeft;
        float effortRight = Kp * errorRight + Ki * sumRight + Kd * derRight;

        if (abs(speedLeft) < 2 && targetLeft == 0) sumLeft = effortLeft = 0;
        if (abs(speedRight) < 2 && targetRight == 0) sumRight = effortRight = 0;

        if (invertLeft) effortLeft = -effortLeft;

        motors.setSpeeds(effortLeft, effortRight);

        //Serial.println("target left: " + String(targetLeft) + "\ttarget right: " + String(targetRight) + "\teffort left: " + String(effortLeft) + "\teffort right: " + String(effortRight));
      }
    }
};
