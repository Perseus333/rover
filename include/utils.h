// utils.hpp
// Helper functions

#ifndef UTILS_HPP
#define UTILS_HPP

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

// Custom implementation of map function
short map(short value, short initialLow, short initialHigh, short finalLow, short finalHigh);

// Converts angles from degrees to radians
float toRadians(short angleInDegrees);

#endif // UTILS_HPP