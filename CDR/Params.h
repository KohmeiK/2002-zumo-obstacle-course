/**
 * Please DO NOT commit your version of this!
 * Once you pull this file once, add it to your .gitignore
 * this way github will ignore any changes you make to this file so
 * that we won't mess with each others tunings.
 * (I guess you can commit if you add params for Kiematic turning)
 */

/**
 * LINE TRACKER
 * Offset ->    Value to compensate for difference between left and right
 *              line sensor inconsisentcy To tune, place robot on unform
 *              colored surface, plot the Error variable and plug in the
 *              average value of that here.
 * isWhiteLine -> If your line brighter (smaller light sensor value) than            
 *                your floor? If so make this true.
 * Kp, Ki, Kd -> Line tracking turning PID constants.             
 * LineThreshold -> Average the values of all sensors on the line,
 *                  Average the values of the 5 sensors on the floor.
 *                  Take the average of the 2 values and set it to this.
 */
 #define LT_Offset 280;
 #define LT_isWhiteLine false;
 #define LT_Kp 0.1;
 #define LT_Ki 0;
 #define LT_Kd 0;
 #define LT_LineThreshold 300;

//BUTTON PinNum -> Pin number for start button (17 is btnC)
 #define BT_PinNum 17;

/**
 * DEAD REAKONING
 * DistanceCM -> Distance in CM for deadreakoning to travel for.
 *               Adjust to make sure zumo does not fall off the platform
 *               when doing the final 360
 * SpeedMutipler -> Default speed is 40.               
 */
 #define DR_DistanceCM 20;
 #define DR_SpeedMutiplier 0.7;

//IR DETECTOR useFrontOnly -> keep this true to use all 5 line sensors
 #define IR_useFrontOnly true;

/**
 * WALL FOLLOWING
 * SensorPort -> Port Number for IR sensor. Make sure it has an ADC
 *               and is not used by other sensors.
 * Distance -> Distance in CM from the wall to follow the line at.             
 *             (Min 10, Max 70)
 * BaseSpeed -> Base speed for forward movemnent. Steering added          
 *              and subtracted from this.
 * Kp, Ki, Kd -> PID for steering to remain at the right distnace             
 *               from the wall.
 * isWallLeft -> Are you trying to follow a wall that is to the left
 *               of the robot? If so make this true.
 */
 #define WF_SensorPort A0;
 #define WF_Distance 25;
 #define WF_BaseSpeed 10;
 #define WF_Kp 1.0 ;
 #define WF_Ki 0;
 #define WF_Kd -0.025;
 #define WF_isWallLeft false;
 #define WallTrackUpdateInterval 25;//ms
 #define WF_MaxSteer 5;
 #define WF_Deadband 1.25;

 /**
  * PID VELOCITY
  * Kp, Ki, Kd -> PID to maintain target velocity
  * IntergeralCap -> Intergeral cap to avoid intergeral windup.
  * InvertLeft -> reverse direction of left drive motor
  */
 #define PV_Kp 10;
 #define PV_Ki 1;
 #define PV_Kd 0.03;
 #define PV_IntergeralCap 500;
 #define InvertLeft false;

/**
 * RAMP DIRVER
 * FlatAngleToleranceDEG -> If your final platofrm is not exactly
 *                       level, increase this angle.
 * RampAnlgeDEG 0-> If must be tiled at least this much to be                     
 *                  considered to be climbing. Set it slighly less
 *                  than your actual ramp angle.
 * BaseSpeed -> Basespeed for when appoaching and climbing the ramp.               
 */
 #define RD_FlatAngleToleranceDEG 2;
 #define RD_RampAngleDEG -5; //negative if you are climbing headfirst
 #define RD_BaseSpeed 15;

/**
 * Kinematic Turn
 * 
 * WheelBase -> effective wheel base of your robot. 
 *            May need to be adjusted to get correct turning angle 
 *            depending on driving surface due to slip
 * TurningSpeed -> Speed at which the robot turns
 * WheelDiameter -> tread height
 * DistPerTick -> distance traveled per tick of encoder
 * UpdateInterval -> time interval over which to update position and speeds
 */
 #define WheelBase 4.25;// inch
 #define TurningSpeed 15.;
 #define WheelDiameter 1.5;// inch
 #define DistPerTick .00772;// inch/tick
 #define KinematicTurnUpdateInterval 20;//ms
 

 #define FirstTurnMultiplier -1;
 #define SeccondTurnMultiplier 1;
