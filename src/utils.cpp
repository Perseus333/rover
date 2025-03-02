// utils.cpp
// Helper functions

#include "..\include\constants.hpp"

float toRadians(short angleInDegrees) {
    float angleInRadians = angleInDegrees * PI / 180;
    return angleInRadians;
}

// Custom implementation of map() function from `arduino.h`
short map(short value, short initialLow, short initialHigh, short finalLow, short finalHigh) {
    short valueMapped = (short)((finalHigh - finalLow) * ((float)(value - initialLow) / (float)(initialHigh - initialLow)) + finalLow);
    return valueMapped;
}