// constants.hpp
// CONSTANTS

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

const float PI = 3.141592;
const float SPEED_OF_SOUND = 0.343f; // mm/s
const short FULL_ROTATION = 180; // in degrees

const short MAX_RESPONSE_TIME = (short)((1 << 15) * SPEED_OF_SOUND / 2); // Avoid short overflow
// `1 << 15` means that we shift the bit 1, 15 times. This results in the number 2^15

const short MAX_SHORT_NUM = (short)(1 << 16);
const short MIN_SHORT_NUM = (short)(MAX_SHORT_NUM * -1);

const int mapSideLength = 20; // For debugging purposes only

#endif // CONSTANTS_HPP