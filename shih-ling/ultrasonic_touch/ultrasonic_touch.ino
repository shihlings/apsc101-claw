/*
  Title:        APSC 101 U-1 Module 5 Arduino Program with Ultrasonic Sensor and Touch Sensor
  Author:       Shih-Ling Shen
  Date:         Jan 28th, 2023
  Description:  This program relies on the ultrasonic sensor and the touch sensor to determine when to grab and release the object.
  Operation:    The claw reads the ultrasonic sensor values in the meantime. If the claw reaches below set distance threshold,
                the claw will close/open after a set delay (if open, claw closes; if closed, claw opens).
                The claw closes/opens immediately if the touch sensor is triggered when the ultrasonic reading
                is below the set distnace threshold (if open, claw closes; if closed, claw opens).
 */

//Required Libraries
  #include <NewPing.h>      //NewPing library for ultrasonic sensor
  #include <Servo.h>        //servo library for servo motor

//Program Parameters
  #define DEBUG                         //commenting this out will disable all debugging features, program efficiency will improve
  #define DISTANCE_THRESHOLD      30    //the distance detected to trigger a grab/release (in centimeters)
  #define ERROR_PREVENTION        10    //the distance additional to the DISTANCE_THRESHOLD to recognize the claw is raised (to prevent signal fluctuation problems)
  #define INITIAL_ANGLE           0     //the initial angle the servo motor is at. The twist angle will be adjust accordingly to this number (in degrees)
  #define TWIST_ANGLE             180   //the amount to twist to grasp the object (in degrees)
  #define PERFORMANCE_DELAY       30    //the time delay to wait between each data check
  #define CLAW_DELAY              3000  //the time delay before the claw opens or closes

//Define BOOL
  #define OPEN   0
  #define CLOSED 1
  #define TRUE   1
  #define FALSE  0

//Ultrasonic definitions
  #define ULTRASONIC_GND      10
  #define ULTRASONIC_ECHO     11
  #define ULTRASONIC_TRIGGER  12
  #define ULTRASONIC_VCC      13
  #define MAX_DISTANCE        200                                   //maximum distance set to 200 cm
  NewPing sonar(ULTRASONIC_TRIGGER, ULTRASONIC_ECHO, MAX_DISTANCE); //initialize NewPing

//Servo Pin definitions
  #define SERVO 9
  Servo myservo;            //create servo object to control servo motor

//Switch Pin definitions
  #define SWITCH 8

//Global Variables
  int claw_state = OPEN;   //recording claw grab state

//Function Prototypes
  void ClawDelay ();
  void WaitUntilRise ();
  void WaitUntilLower ();
  int GetTouch ();
  int GetUltrasonic ();
  void OpenClaw ();
  void CloseClaw ();

void setup() {
  #ifdef DEBUG
    //Serial Debug Setup
    Serial.begin(9600);
  #endif
  
  //Ultrasonic Setup
  pinMode(ULTRASONIC_ECHO, INPUT);  
  pinMode(ULTRASONIC_TRIGGER, OUTPUT);
  pinMode(ULTRASONIC_GND, OUTPUT);
  pinMode(ULTRASONIC_VCC, OUTPUT);
  digitalWrite(ULTRASONIC_GND,LOW);
  digitalWrite(ULTRASONIC_VCC, HIGH);

  //Servo Motor Setup
  myservo.attach(SERVO);

  //Switch Setup
  pinMode(SWITCH, INPUT);

  //Open Claw initially
  OpenClaw();
}

void loop() {
  //wait until the claw rises then lowers
  WaitUntilRise();
  WaitUntilLower();

  //delays the execution of open/close to give time for positioning
  ClawDelay();

  //if claw was open, close; if claw was closed, open
  if (claw_state == OPEN) {
    CloseClaw();
  }
  else {
    OpenClaw();
  }
}

//Delaying 
void ClawDelay () {
  //stores current time
  long time = millis();
  
  //delay the claw execution by CLAW_DELAY milliseconds
  //if it touches the ground at any point, the delay breaks and closes the claw immediately
  while (millis() <= time + CLAW_DELAY && (GetTouch() == FALSE)) {
    delay(PERFORMANCE_DELAY);
  }
}

//wait until the claw is risen (no function just delay)
void WaitUntilRise () {
  int distance = GetUltrasonic();
  long last_check = 0;

  //waits until the distance detected is greater than the threshold
  while (distance <= DISTANCE_THRESHOLD + ERROR_PREVENTION) {
    //only check every PERFORMANCE_DELAY milliseconds to improve performance
    if (millis() >= last_check + PERFORMANCE_DELAY) {
      distance = GetUltrasonic();   //retrieve distance data
      last_check = millis();        //stores the time of the last check
    }
  }
}

//wait until the claw is lowered (no function just delay)
void WaitUntilLower () {
  int distance = GetUltrasonic();
  long last_check = 0;
  
  //waits until the distance detected is lower than the threshold
  while (distance > DISTANCE_THRESHOLD) {
    //only check every PERFORMANCE_DELAY milliseconds to improve performance
    if (millis() >= last_check + PERFORMANCE_DELAY) {
      distance = GetUltrasonic();   //retrieve distance data
      last_check = millis();        //stores the time of the last check
    }
  }
}

//detects if the touch sensor is triggered
//TRUE if triggered, FALSE if not triggered
int GetTouch () {
  return (digitalRead(SWITCH) == HIGH);
}

//retrieves the distance read in the ultrasonic sensor
//prints the distance through serial port if DEBUG is on
int GetUltrasonic () {
  int distance = sonar.ping_cm();           //read the ultrasonic sensor, and store it in a variable

  #ifdef DEBUG
    Serial.print("Ping: ");                 //print “Ping:" on the computer display
    Serial.print(distance);                 //print the value of the variable next
    Serial.println("cm");                   //print "cm" after that, then go to next lin
  #endif

  return distance;                          //returns the distance read to the main program
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