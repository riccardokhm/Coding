// Create a bit rotation function for 8-bit unsigned integers
#include <iostream>

using namespace std;

/// @brief Rotate the @param value to the left by @param shift bits.
/// @param value 
/// @param shift 
/// @return 
unsigned char lrotate(unsigned char value, int shift) {
    
    unsigned int t = value;


    for (int i = 0; i < shift; i++){
        t << 1;
    
        // if a bit is shifted out on the left, set the rightmost bit
        if (t & 256)
           t = t | 1;
    }
       
    return t;
    
}

unsigned char rrotate(unsigned char value, int shift) {
    
    unsigned int t = value;

    t = t << 8;

    for (int i = 0; i < shift; i++)
    {
        t >> 1;
        if(t & 128)
            t = t | 32768;
    }
    
    return t >> 8;
}