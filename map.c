#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
/* `stdint.h` should be replaced with `arduino.h`
As it's only purpose is to use uint8_t
which arduino already replaces with `byte`
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
    Position currentPos;
    short vehicleRotation;
    unsigned short obstacleDistance;
    short sensorRotation;
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
    TODO: Arduino shenaningans
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
    // If we need more space, we assign another bit for the `obstacles` list
    if (obstacleAmount >= obstacleCapacity || (obstacleCapacity < MAX_MEMORY)) {
        // Adding another bit doubles the obstacle capacity
        obstacleCapacity *= 2;
        // The memory that we need to reallocate is that of the size of the struct of Position
        // times the amount of elements that we have in the array
        // the size of Position should be 4 bytes (2 shorts; 2 bytes each) * obstacleCapacity
        Position* temp = realloc(obstacles, obstacleCapacity * sizeof(Position));
        if (!temp) {
            printf("Couldn't allocate memory");
            exit(1);
        } 
        obstacles = temp;
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

// Custom implementation of map() function from `arduino.h`
short map(short value, short initialLow, short initialHigh, short finalLow, short finalHigh) {
    short valueMapped = (short)((finalHigh - finalLow) * ((float)(value - initialLow) / (float)(initialHigh - initialLow)) + finalLow);
    return valueMapped;
}

// TEMPORARY - ChatGPT Code! For testing and debugging purposes
void updateObstacleMap(short updateAmount) {
    char grid[mapSideLength][mapSideLength];

    // Step 1: Fill grid with '.'
    for (int y = 0; y < mapSideLength; y++) {
        for (int x = 0; x < mapSideLength; x++) {
            grid[y][x] = '.';
        }
    }

    // Step 2: Map obstacles to the grid
    for (int i = 0; i < obstacleAmount; i++) {
        short mappedX = map(obstacles[i].x, -32766, 32766, 0, mapSideLength - 1);
        short mappedY = map(obstacles[i].y, -32766, 32766, 0, mapSideLength - 1);

        if ((mappedX >= 0) && (mappedX < mapSideLength) && (mappedY >= 0) && (mappedY < mapSideLength)) {
            grid[mappedY][mappedX] = '#';  // Place an obstacle
        }
    }

    grid[map(currentPos.y, -32766, 32766, 0, mapSideLength - 1)][map(currentPos.y, -32766, 32766, 0, mapSideLength - 1)] = 'X';

    // Step 3: Print the grid
    for (int y = 0; y < mapSideLength; y++) {
        for (int x = 0; x < mapSideLength; x++) {
            putchar(grid[y][x]);
            putchar(' ');  // Space for better visualization
        }
        putchar('\n');  // New line after each row
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