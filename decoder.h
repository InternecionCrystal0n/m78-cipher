#ifndef DECODER_H
#define DECODER_H

#include <cstdint>

// Step 1: Takes the noisy ciphertext and un-shuffles the bit seats using P_INV
uint8_t removePermutation(uint8_t noisyCiphertext, const uint8_t* privatePInv);

// Step 2: Uses QR-style syndrome decoding to isolate the flipped noise bit and fix it
uint8_t cleanRadioNoise(uint8_t unshuffledBlock, const uint8_t* privateG);

// Step 3: Multiplies the cleaned block by S_INV to remove the final scrambler mask
uint8_t removeScrambler(uint8_t cleanBlock, const uint8_t* privateSInv);

#endif // DECODER_H

// AI GENERATED CUZ ITS A HEADER :3