// map.c
// The main file for the rover

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
/* `stdint.h` should be replaced with `arduino.h`
As it's only purpose is to use the uint8_t datatype
which arduino already replaces with `byte`
*/
#include <stdint.h>

// The necessary hardware libraries
#include "Arduino.h"

// Include the custom libraries (header files)
#include "utils.h"
#include "constants.h"
#include "hardware.h"
#include "pathfinding.h"

/*
Integer Data Types
Efficient memory management is crucial in microcontrollers

0 to 255: byte (1 byte)
0 to 64000: unsigned short (2 bytes)
-32000 to 32000: short (2 bytes)
angles and rotations: short (2 bytes)
*/

// Change this values by specifying them on the make command
// Fallback for Makefile, usually this values aren't read
#ifndef SCANS_PER_SWIPE
#define SCANS_PER_SWIPE 10 // Increase for more accuracy but lower speeds
#endif

#ifndef MAX_MEMORY
#define MAX_MEMORY 1020 // Affects memory usage
#endif

#ifndef INTIAL_CAPACITY
#define INITIAL_CAPACITY 255 // 2^8-1; max values for 1 byte
#endif

// Initializing global variables
Position currentPos;
Position* obstacles = NULL;
unsigned short obstacleAmount = 0;
short vehicleRotation = 0;
unsigned short obstacleCapacity = INITIAL_CAPACITY;
// Sets all the elements in the grid as . for better visibility
char grid[mapSideLength][mapSideLength] = {'.'};

// Distance should be in mm
short getDistance() {
    /*
    TODO: Arduino shenaningans
    */
    
    //Temporary code - randomizes it
    long responseTime = rand() % MAX_RESPONSE_TIME;
    
    // Temp Code for testing max values
    // long responseTime = MAX_RESPONSE_TIME;

    short distance; 
    if (responseTime > MAX_RESPONSE_TIME) {
        // sets distance to be 2^15 which is the max number of a `short` type
        distance = (1 << 15) - 1;
    }
    else {
        distance = (short)(responseTime / SPEED_OF_SOUND) * 2;
    }

    return distance;
}

Position generateObstacle(DataPacket data) {

    short overallRotation = data.sensorRotation + data.vehicleRotation;
    short xDistance = (short)(cos(toRadians(overallRotation)) * data.obstacleDistance);
    short yDistance = (short)(sin(toRadians(overallRotation)) * data.obstacleDistance);
    
    Position obstaclePos;
    obstaclePos.x = xDistance + data.currentPos.x;
    obstaclePos.y = yDistance + data.currentPos.y;

    return obstaclePos;
}

void appendObstacle(Position obstacle) {
    // If we need more space, we assign another bit (double it) for the `obstacles` list
    if (obstacleAmount >= obstacleCapacity && obstacleCapacity < MAX_MEMORY) {

        // Only doubles the memory if it it won't exceed the `MAX_MEMORY`
        if (obstacleCapacity*2 < MAX_MEMORY) {
            obstacleCapacity *= 2;
        }
        // The memory that we need to reallocate is that of the size of the struct of Position
        // times the amount of elements that we have in the array
        // the size of Position should be 4 bytes (2 shorts; 2 bytes each) * obstacleCapacity
        Position* temp = (Position*)realloc(obstacles, obstacleCapacity * sizeof(Position));
        if (!temp) {
            printf("Couldn't allocate memory");
            // Blink arduino LED rapidly or something to signal that there is an error
        } 
        // Only update the obstacles if there hasn't been an error with the memory
        else {
            obstacles = temp;
        }
    }
    
    // Replaces old objects instead of overflowing
    obstacleAmount = obstacleAmount % obstacleCapacity;

    obstacles[obstacleAmount].x = obstacle.x;
    obstacles[obstacleAmount].y = obstacle.y;

    obstacleAmount++;
}

void scanEnvironment() {
    // Angles are in degrees (0-360º)
    const short ANGLE_STEP = FULL_ROTATION / SCANS_PER_SWIPE;
    
    for (uint8_t i = 1; i <= SCANS_PER_SWIPE; i++) {
        
        short obstacleDistance = getDistance();
        short sensorRotation = ANGLE_STEP * i;
        
        DataPacket positionFeedback;
        positionFeedback.currentPos = currentPos;
        positionFeedback.obstacleDistance = obstacleDistance;
        positionFeedback.sensorRotation = sensorRotation;
        positionFeedback.vehicleRotation = vehicleRotation;
        
        Position obstaclePosition = generateObstacle(positionFeedback);
        appendObstacle(obstaclePosition);
    }
}

// Updates the obstacle map with the obstacles that were detected
void updateObstacleDisplay(short updateAmount) {

    for (int newObstacle = 0; newObstacle < updateAmount; newObstacle++) {
        short mappedX = map(obstacles[newObstacle].x, -32766, 32766, 0, mapSideLength - 1);
        short mappedY = map(obstacles[newObstacle].y, -32766, 32766, 0, mapSideLength - 1);

        if ((mappedX >= 0) && (mappedX < mapSideLength) && (mappedY >= 0) && (mappedY < mapSideLength)) {
            grid[mappedY][mappedX] = '#';
        }
    }

    //      initial value, min initial,   max initial, min target, max target 
    grid[map(currentPos.y, MIN_SHORT_NUM, MAX_SHORT_NUM, 0,    mapSideLength - 1)]
        [map(currentPos.x, MIN_SHORT_NUM, MAX_SHORT_NUM, 0,    mapSideLength - 1)] = 'X';

    for (int y = 0; y < mapSideLength; y++) {
        for (int x = 0; x < mapSideLength; x++) {
            putchar(grid[y][x]);
            putchar(' ');
        }
        putchar('\n');
    }
}

// Should probably return something
void pathFind() {
    // Add later
}

void moveVehicle() {
    // Add later
}

// The setup function in arduino
void setup_() {

    currentPos.x = 0;
    currentPos.y = 0;
    
    // Allocate memory for obstacles
    obstacles = (Position*)malloc(INITIAL_CAPACITY * sizeof(Position));

    // TEMPORARY - To initialize the time for "random" numbers for `getDistance()`
    srand(clock());
}

// The loop function in arduino
void loop_() {
    scanEnvironment();
    updateObstacleDisplay(SCANS_PER_SWIPE);
    pathFind();
    moveVehicle();
}

// The `main()` function, should be excluded when porting it to Arduino,
// it's just here for testing purposes
int main() {

    setup_();

    // same as `while true`, but without having to include stdbool.h
    for (;;) {
        loop_();
        // waits for user input (testing purposes only)
        char ch = getchar();
        if (ch) {} // To not raise compiler warning for unused variable
    }
    
    return 0;
}