#include "Button.h"
#include "DeadReckon.h"
#include "EventTimer.h"
#include "IR.h"
#include "KinematicTurn.h"
#include "LineTrack.h"
#include "PIDVelocity.h"
#include "WallFollow.h"
#include "MotorSpeedsStruct.h"
#include "RampDriver.h"
#include "Params.h"

#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Encoders encoders1;//Need to figure out if we want to define here as static, pass by reference, etc...
Zumo32U4LCD lcd;

enum ROBOT_STATE {STOPPED, WAITING, WALL_FOLLOWING, TURN, LINE_FOLLOWING, DRIVE_RAMP, DRIVE_FORWARD};
ROBOT_STATE state = STOPPED;
ROBOT_STATE nextState = STOPPED;

const int DeadReckonDistance_cm = DR_DistanceCM; //Actually set when we start testing
Button ButtonC;
EventTimer timer;
EventTimer displayTimer;
LineTrack lineTracker;
KinematicTurn turn;
WallFollow wallFollower;
IR remoteChecker;
RampDriver rampDriver;
DeadReckon simpleDriver(encoders1);
PIDVelocity pid;

MotorSpeeds targetSpeeds;

void setup() {
  Serial.begin(115200);

  ButtonC.Init();
  remoteChecker.Init();
  turn.Init();
  lineTracker.Init();
  pid.Init();
  rampDriver.Init();
  wallFollower.Init();
  displayTimer.Start(200);
  
  targetSpeeds.left = 0;
  targetSpeeds.right = 0;
}

void loop() {
  
  switch (state)
  {
    case STOPPED:
      targetSpeeds.left = 0;
      targetSpeeds.right = 0;
      if (ButtonC.CheckButtonPress())
      {
        state = WAITING;
        timer.Start(1000);
      }
      break;
    case WAITING:
      targetSpeeds.left = 0;
      targetSpeeds.right = 0;
      if (timer.isExpired())
      {
        state = WALL_FOLLOWING;
      }
      break;
    case WALL_FOLLOWING:
      if (lineTracker.lineDetected())
      {
        state = TURN;
        nextState = LINE_FOLLOWING;
        turn.startTurn(90);
        break;
      }
      targetSpeeds=wallFollower.calcSpeeds();
      break;
    case TURN:
      if (turn.isFinished())
      {
        state = nextState;
        break;
      }
      targetSpeeds=turn.calcSpeeds();
      break;
    case LINE_FOLLOWING:
      if (remoteChecker.isDetecting())
      {
        state = TURN;
        nextState = DRIVE_RAMP;
        turn.startTurn(-90);
        break;
      }
      targetSpeeds=lineTracker.calcSpeeds();
      break;
    case DRIVE_RAMP:
      if (rampDriver.isFinished())
      {
        state = DRIVE_FORWARD;
        simpleDriver.setTarget(DeadReckonDistance_cm);
        targetSpeeds = simpleDriver.startDrive();
        break;
      }
      targetSpeeds=rampDriver.calcSpeeds();
      break;
    case DRIVE_FORWARD:
      if (simpleDriver.isFinished())
      {
        state = TURN;
        nextState = STOPPED;
        turn.startTurn(360);
        break;
      }
      break;
  }
  if(displayTimer.isExpired()){
    lcd.clear();
    switch(state){
      case STOPPED: lcd.print("Stopped"); lcd.gotoXY(0,1); lcd.print("Press C"); break;
      case WAITING: lcd.print("Waiting"); lcd.gotoXY(0,1); lcd.print("1 sec");break;
      case WALL_FOLLOWING: lcd.print("Wall"); lcd.gotoXY(0,1); lcd.print("Follow"); break;
      case TURN: lcd.print("Kinematc"); lcd.gotoXY(0,1); lcd.print("Turn"); break;
      case LINE_FOLLOWING: lcd.print("Line"); lcd.gotoXY(0,1); lcd.print("Follow"); break;
      case DRIVE_RAMP: lcd.print("Drive"); lcd.gotoXY(0,1); lcd.print("Ramp"); break;
      case DRIVE_FORWARD: lcd.print("Drive"); lcd.gotoXY(0,1); lcd.print("Forwards"); break;
    }
    displayTimer.Start(200);
  }
  pid.setTargets(targetSpeeds);
  pid.update();

  
  
}
