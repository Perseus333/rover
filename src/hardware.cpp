// hardware.cpp
// Functions related to physical components


#include "Arduino.h"

// You can paste the code below this into the arduino setup and it will work right off the bat

// hardware.cpp
// Functions related to physical components

// You can paste the code below this into the arduino setup and it will work right off the bat

// Defining the pins
const byte ENABLE_RIGHT = 3;
const byte RIGHT_B = 4;
const byte RIGHT_F = 2;

const byte ENABLE_LEFT = 9;
const byte LEFT_B = 8;
const byte LEFT_F = 10;

const byte RIGHT_LED = 12;
const byte LEFT_LED = 13;

const byte TRIG = 6;
const byte ECHO = 5;

// Configures the servo
Servo sensorServo;

// Speed of sound - useful for calculating distances with HC-SR04
const float SPEED_OF_SOUND = 0.343f; // mm/s

char lastCommand = 'f';

// Reads a character and interprets it as a command

char readCommand() {
    char command = ' ';
    if (Serial.available() > 0) {
        command = Serial.read();
        Serial.print("Received command: ");
        Serial.println(command);
    }
    return command;
}

// Executes the instructions
void runCommand(char command, bool log = true) {
	// A switch statement is a more readable version
	// of the else if
	switch (command) {

	// forward
	case 'f':
		digitalWrite(LEFT_B, LOW);
		digitalWrite(LEFT_F, HIGH);
		digitalWrite(RIGHT_B, LOW);
		digitalWrite(RIGHT_F, HIGH);
		digitalWrite(LEFT_LED, HIGH);
		digitalWrite(RIGHT_LED, HIGH);
		break;

	// backwards
	case 'b':
		digitalWrite(LEFT_B, HIGH);
		digitalWrite(LEFT_F, LOW);
		digitalWrite(RIGHT_B, HIGH);
		digitalWrite(RIGHT_F, LOW);
		digitalWrite(LEFT_LED, LOW);
		digitalWrite(RIGHT_LED, LOW);
		break;

	// right
	case 'r':
		digitalWrite(LEFT_B, HIGH);
		digitalWrite(LEFT_F, LOW);
		digitalWrite(RIGHT_B, LOW);
		digitalWrite(RIGHT_F, HIGH);
		digitalWrite(LEFT_LED, LOW);
		digitalWrite(RIGHT_LED, HIGH);
		break;

	// left
	case 'l':
		digitalWrite(LEFT_B, LOW);
		digitalWrite(LEFT_F, HIGH);
		digitalWrite(RIGHT_B, HIGH);
		digitalWrite(RIGHT_F, LOW);
		digitalWrite(LEFT_LED, HIGH);
		digitalWrite(RIGHT_LED, LOW);
		break;
	}
  	if (log) {
  		lastCommand = command;
    }
}

int detectDistance() {
	long duration;
	int distance;

	// Trigger
	digitalWrite(TRIG, LOW);
	digitalWrite(TRIG, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIG, LOW);

	duration = pulseIn(ECHO, HIGH);

	distance = duration * SPEED_OF_SOUND / 2;

	// To avoid interferences from previous waves
	delayMicroseconds(100);

	return distance;
}

// Does a full swipe
// The sensor will be mounted on the servo
int scan() {
	int distances[SCANS_PER_SWIPE] = {};
  	int distanceSum = 0;
	sensorServo.write(180);
	Serial.print("d: ");
	for (int i = 0; i < SCANS_PER_SWIPE; i++) {
		distances[i] = detectDistance();
		distanceSum += distances[i];
		// Prints the distance to the obstacle;
		Serial.print(distances[i]);
      	Serial.print(", ");
	}
  	Serial.println("");
	delay_(2); // Remove the underscore ("_") when running
	// Returns to the original position
	sensorServo.write(0);
  	int avgDistance = (int)(distanceSum / SCANS_PER_SWIPE);
  	return avgDistance;
}

// setup and loop functions are only for demonstrative purposes

void setup() {
  
	// Define all DC Motor pins as OUTPUT
	pinMode(ENABLE_RIGHT, OUTPUT);
	pinMode(RIGHT_B, OUTPUT);
	pinMode(RIGHT_F, OUTPUT);
	pinMode(ENABLE_LEFT, OUTPUT);
	pinMode(LEFT_B, OUTPUT);
	pinMode(LEFT_F, OUTPUT);

	// Set all motors off at the beginning
	digitalWrite(RIGHT_B, LOW);
	digitalWrite(RIGHT_F, LOW);
	digitalWrite(LEFT_B, LOW);
	digitalWrite(LEFT_F, LOW);

	// Sets trigger pins for sensor
	pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

	// Always max speed
	analogWrite(ENABLE_LEFT, 255);
	analogWrite(ENABLE_RIGHT, 255);

	// Initializes the Serial Monitor (terminal)
	Serial.begin(9600);

	// Prints all available commands
	Serial.println("Available commands:");
	Serial.println("f: Forwards");
	Serial.println("b: Backwards");
	Serial.println("r: Turn right");
	Serial.println("l: Turn left");

	sensorServo.attach(11);

}

void loop() {
    int avgDistance = scan();
    if (avgDistance > 500) {
    	runCommand(lastCommand);
      	runCommand(readCommand());
    }
    else {
      	runCommand('b', false);
    }
}
