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

#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4Encoders encoders1;//Need to figure out if we want to define here as static, pass by reference, etc...

enum ROBOT_STATE {STOPPED, WAITING, WALL_FOLLOWING, TURN, LINE_FOLLOWING, DRIVE_RAMP, DRIVE_FORWARD};
ROBOT_STATE state = LINE_FOLLOWING;
ROBOT_STATE nextState = STOPPED;

const bool IRFrontOnly = true;//If true IRChecker will only use front sensor, if false all 3 will be utilized
const int DeadReckonDistance_cm = 20; //Actually set when we start testing
Button ButtonC(17);
EventTimer timer;
LineTrack lineTracker(1000);
KinematicTurn turn;
WallFollow wallFollower(30, 30, 0, .7, -.5, 1);
IR remoteChecker(IRFrontOnly);
RampDriver rampDriver;
DeadReckon simpleDriver(encoders1);
PIDVelocity pid(0,1,0.03,500);

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
  
  targetSpeeds.left = 0;
  targetSpeeds.right = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
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
  pid.setTargets(targetSpeeds);
  pid.update();
  
}
