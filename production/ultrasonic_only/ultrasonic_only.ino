/*
  Title:        APSC 101 U-1 Module 5 Arduino Program with Ultrasonic Sensor with set height (Production)
  Author:       Shih-Ling Shen
  Date:         Feb 3rd, 2023
  Description:  This program relies on the ultrasonic sensor to determine when to grab and release the object.
  Operation:    The claw closes if it reaches below the set distance threshold and the previous state of the claw was open
                The claw opens if it reaches below the set distance threshold and the previous state of the claw was closed
 */

//Required Libraries
  #include <NewPing.h>      //NewPing library for ultrasonic sensor
  #include <Servo.h>        //servo library for servo motor

//Program Parameters
//  #define DEBUG                         //commenting this out will disable all debugging features, program efficiency will improve
  #define DISTANCE_THRESHOLD      30    //the distance detected to trigger a grab/release (in centimeters)
  #define ERROR_PREVENTION        20    //the distance additional to the DISTANCE_THRESHOLD to recognize the claw is raised (to prevent signal fluctuation problems)
  #define INITIAL_ANGLE           0     //the initial angle the servo motor is at. The twist angle will be adjust accordingly to this number (in degrees)
  #define TWIST_ANGLE             180   //the amount to twist to grasp the object (in degrees)
  #define PERFORMANCE_DELAY       30    //the time delay to wait between each data check
  #define CLAW_DELAY              3000  //the time delay before the claw opens or closes
  #define BLACKOUT                1000  //the claw will be unresponsive for this many milliseconds after opening/closing to prevent signal redundancy
  #define SIGNAL_SAMPLE_RATE      100   //the time delay (in ms) for the ultrasonic for sampling data (to prevent redundancy again)

//Define BOOL
  #define OPEN   0
  #define CLOSED 1

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

//Global Variables
  int claw_state = OPEN;   //recording claw grab state

//Function Prototypes
  void ClawDelay ();
  void WaitUntilRise ();
  void WaitUntilLower ();
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

  //Open Claw initially
  OpenClaw();
}

void loop() {
  //wait until the claw rises then lowers
  WaitUntilRise();
  delay(PERFORMANCE_DELAY);
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

  //delay the program to prevent signal redundancy
  delay(BLACKOUT);
}

//Delaying 
void ClawDelay () {
  //stores current time
  long time = millis();
  
  //delay the claw execution by CLAW_DELAY milliseconds
  while (millis() <= time + CLAW_DELAY) {
    delay(PERFORMANCE_DELAY);
  }

  #ifdef DEBUG
    Serial.println("Timeout");
  #endif
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
  
  #ifdef DEBUG
    Serial.println("Rise Detected");
  #endif
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
  
  #ifdef DEBUG
    Serial.println("Lower Detected");
  #endif
}

//retrieves the distance read in the ultrasonic sensor
//prints the distance through serial port if DEBUG is on
int GetUltrasonic () {
  delay(SIGNAL_SAMPLE_RATE);                //delay to prevent inaccurate/redundant readings

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
  #ifdef DEBUG
    Serial.println("Open");
  #endif
}

//Closes the claw
void CloseClaw () {
  myservo.write(INITIAL_ANGLE + TWIST_ANGLE);
  claw_state = CLOSED;

  #ifdef DEBUG
    Serial.println("Close");
  #endif
}