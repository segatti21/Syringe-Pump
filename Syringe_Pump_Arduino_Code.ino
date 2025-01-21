#include <AccelStepper.h>


// Pin definitions
#define stepPin 3 // gray
#define directionPin 2 // orange
#define redLED 5 // red
#define greenLED 6 // green
#define blueLED 7 // blue
#define onOffButton 10 // pink




// Stepper motor setup
AccelStepper stepper(AccelStepper::DRIVER, stepPin, directionPin);


// Variables
bool onOffPressed = false; // Tracks the state of the system (on/off)


float flowRate = 0.5; // mL/min
float syringeDiameter = 19; //20 mL syringe
float syringeRadius =  syringeDiameter / 2;
const float leadScrewLead = 8.0; // Lead screw lead in mm/rev
const float stepsPerRevolution = 200.0; // Steps per revolution
const float microsteps = 16.0; // Microstepping factor
float stepsPerSecond = 0.0; // Calculated steps per second
float volumePerStep;


void setup() {
 // Initialize pins
 pinMode(redLED, OUTPUT);
 pinMode(greenLED, OUTPUT);
 pinMode(blueLED, OUTPUT);
 pinMode(onOffButton, INPUT_PULLUP);


 // Stepper motor setup
 stepper.setMaxSpeed(1000);  // Maximum speed
 stepper.setAcceleration(500); // Acceleration


 // Default LED status (blue for stopped)
 updateLEDs();
}


void loop() {


 float volumePerStep = (PI * pow(syringeRadius, 2) * leadScrewLead) / (stepsPerRevolution * microsteps * 1000);
 stepsPerSecond = flowRate / volumePerStep / 60;




 // Check and handle the on/off button state
 static bool previousState = HIGH;
 bool currentState = digitalRead(onOffButton);
 if (previousState == HIGH && currentState == LOW) {
   onOffPressed = !onOffPressed; // Toggle state
   updateLEDs(); // Update LED based on state
 }
 previousState = currentState;


 // Control the motor based on the current state
 if (onOffPressed) {
   stepper.setSpeed(stepsPerSecond); // Set motor speed (500 steps/sec)
   stepper.runSpeed();    // Run the motor
 } else {
   stepper.stop();        // Stop the motor
 }
}


void updateLEDs() {
 if (onOffPressed) {
   // Green LED ON for "ON" state
   digitalWrite(redLED, LOW);
   digitalWrite(greenLED, HIGH);
   digitalWrite(blueLED, LOW); // Blue OFF
 } else {
   // Blue LED ON for "OFF" state
   digitalWrite(redLED, LOW);
   digitalWrite(greenLED, LOW);
   digitalWrite(blueLED, HIGH); // Blue ON
 }
}
