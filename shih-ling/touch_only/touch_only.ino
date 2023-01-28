/*
  Title:        APSC 101 U-1 Module 5 Arduino Program with Touch Sensor
  Author:       Shih-Ling Shen
  Date:         Jan 28th, 2023
  Description:  This program relies on the touch sensor to determine when to grab and release the object.
  Operation:    The claw closes/opens immediately if the touch sensor is triggered (if open, claw closes; if closed, claw opens).
                There is a cool down period between operation (can be manually adjusted).
 */

//Required Libraries
  #include <Servo.h>        //servo library for servo motor

//Program Parameters
  #define DEBUG                         //commenting this out will disable all debugging features, program efficiency will improve
  #define DELAY                   1000  //the amount of time to disable the program (reduce signal redundancy) (in milliseconds)
  #define INITIAL_ANGLE           105   //the initial angle the servo motor is at. The twist angle will be adjust accordingly to this number (in degrees)
  #define TWIST_ANGLE             -90   //the amount to twist to grasp the object (in degrees)
  #define PERFORMANCE_DELAY       30    //the time delay to wait between each data check

//Define BOOL
  #define OPEN   0
  #define CLOSED 1

//Servo Pin definitions
  #define SERVO 9
  Servo myservo;            //create servo object to control servo motor

//Switch Pin definitions
  #define SWITCH 8

//Global Variables
  int claw_state = OPEN;   //recording claw grab state

//Function Prototypes
  void WaitForTouch ();
  void OpenClaw ();
  void CloseClaw ();

void setup() {
  #ifdef DEBUG
    //Serial Debug Setup
    Serial.begin(9600);
  #endif

  //Servo Motor Setup
  myservo.attach(SERVO);

  //Switch Setup
  pinMode(SWITCH, INPUT);
}

void loop() {
  WaitForTouch();

  //if claw was open, close; if claw was closed, open
  if (claw_state == OPEN) {
    CloseClaw();
  }
  else {
    OpenClaw();
  }
  
  delay(DELAY);
}

//waits until the touch sensor is triggered
void WaitForTouch () {
  while (digitalRead(SWITCH) == LOW) {
    delay(PERFORMANCE_DELAY);
  }
}

//Opens the claw
void OpenClaw () {
  myservo.write(INITIAL_ANGLE);
  claw_state = OPEN;
}

//Closes the claw
void CloseClaw () {
  myservo.write(INITIAL_ANGLE + TWIST_ANGLE);
  claw_state = CLOSED;
}