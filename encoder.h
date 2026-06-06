#ifndef ENCODER_H
#define ENCODER_H

#include <cstdint>

// Multiplies your 4-bit data chunk by the 4x7 Public Matrix Key
uint8_t encryptBlock(uint8_t dataNibble, const uint8_t* publicKeyMatrix);

// Intentionally flips a random bit between position 0 and 6 to create the security shield
uint8_t injectNoise(uint8_t pureCiphertext);

#endif // ENCODER_H
