#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
/* `stdint.h` should be replaced with `arduino.h`
As it's only purpose is to use uint8_t
which arduino already replaces with `byte`
This also replaces the need for the map function
*/
#include <stdint.h>

/*
Integer Data Types
Efficient memory management is crucial in microcontrollers

0 to 255: byte (1 byte)
0 to 64000: unsigned short (2 bytes)
-32000 to 32000: short (2 bytes)
angles and rotations: short (2 bytes)

*/
const float PI = 3.141592;
const float SPEED_OF_SOUND = 0.343f; // mm/s
const short FULL_ROTATION = 180; // in degrees
const uint8_t SCANS_PER_SWIPE = 32; // Tweak for more accuracy
const unsigned short MAX_MEMORY = 1020; // Tweak for memory consuption (up to 2^16)
const uint8_t INITIAL_CAPACITY = 255; // 2^8-1; max values for 1 byte
const short MAX_RESPONSE_TIME = (short)((1 << 15) * SPEED_OF_SOUND / 2); // Avoid short overflow
// `1 << 15` means that we shift the bit 1, 15 times. This results in the number 2^15

const int mapSideLength = 100; // For debugging purposes only

typedef struct DataPacket DataPacket;
typedef struct Position Position;

// Custom data types are a bit redundant but can be added

// ALL DISTANCES IN MILLIMETRES (mm)
struct Position {
    // short allows a radius of 32m of distance representation.
    // The course will be much shorter, and this is half
    // as memory heavy as regular `int`
    short x;
    short y;
};

// ALL ANGLES IN DEGREES (º)
struct DataPacket {
    // Add the config
};

// Initializing global variables
Position currentPos;
Position* obstacles = NULL;
unsigned short obstacleAmount = 0;
short vehicleRotation = 0;
unsigned short obstacleCapacity = INITIAL_CAPACITY;

// Distance should be in mm
short getDistance() {
    /*
    TODO: Arduino shenaningans :3
    */
    
    //Temporary code - randomizes it
    long responseTime = rand() % MAX_RESPONSE_TIME;
    
    // Temp Code for testing max values
    // long responseTime = MAX_RESPONSE_TIME;

    short distance; 
    if (responseTime > MAX_RESPONSE_TIME) {
        // sets distance to be 2^15 which is the max number of a `short` type
        distance = (1 << 15);
    }
    else {
        distance = (short)(responseTime / SPEED_OF_SOUND) * 2;
    }

    return distance;
}

float toRadians(short angleInDegrees) {
    float angleInRadians = angleInDegrees * PI / 180;
    return angleInRadians;
}

// TEMPORARY CODE! -- DO NOT USE!
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
    // Add later
    obstacleAmount++;
}

void scanEnvironment() {
    // Add later
}

// Custom implementation of map() function from `arduino.h` because I was to lazy to import header files in windows (it sucks)
// The day C comes with a package manager, then I'll consider it
short map(short value, short initialLow, short initialHigh, short finalLow, short finalHigh) {
    short valueMapped = (short)((finalHigh - finalLow) * ((float)(value - initialLow) / (float)(initialHigh - initialLow)) + finalLow);
    return valueMapped;
}

void updateObstacleMap(short updateAmount) {
    // Add later
}

// Should probably return something
void pathFind() {
    // Add later
}

void moveVehicle() {
    // Add later
}

// The setup function in arduino
void setup() {

    currentPos.x = 0;
    currentPos.y = 0;
    
    // Allocate memory for obstacles
    obstacles = malloc(INITIAL_CAPACITY * sizeof(Position));

    // TEMPORARY - To initialize the time for "random" numbers for `getDistance()`
    srand(clock());
}

// The loop function in arduino
void loop() {
    scanEnvironment();
    updateObstacleMap(SCANS_PER_SWIPE);
    pathFind();
    moveVehicle();
}

// The `main()` function, should be excluded when porting it to Arduino,
// it's just here for testing purposes
int main() {

    setup();

    // same as `while true`, but without having to include stdbool.h
    for (;;) {
        loop();
        // waits for user input (testing purposes only)
        char ch = getchar();
        if (ch) {} // To not raise compiler warning for unused variable
    }
    
    return 0;
}
