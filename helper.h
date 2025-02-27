// Helper functions
#define HELPER
#ifndef helper

// CONSTANTS
const float PI = 3.141592;
const float SPEED_OF_SOUND = 0.343f; // mm/s
const short FULL_ROTATION = 180; // in degrees

const short MAX_RESPONSE_TIME = (short)((1 << 15) * SPEED_OF_SOUND / 2); // Avoid short overflow
// `1 << 15` means that we shift the bit 1, 15 times. This results in the number 2^15

const short MAX_SHORT_NUM = (short)(1 << 16);
const short MIN_SHORT_NUM = (short)(MAX_SHORT_NUM * -1);

const int mapSideLength = 20; // For debugging purposes only


float toRadians(short angleInDegrees) {
    float angleInRadians = angleInDegrees * PI / 180;
    return angleInRadians;
}

// Custom implementation of map() function from `arduino.h`
short map(short value, short initialLow, short initialHigh, short finalLow, short finalHigh) {
    short valueMapped = (short)((finalHigh - finalLow) * ((float)(value - initialLow) / (float)(initialHigh - initialLow)) + finalLow);
    return valueMapped;
}

#endif // helper.h