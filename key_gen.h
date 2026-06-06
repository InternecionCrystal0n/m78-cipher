#ifndef KEYGEN_H
#define KEYGEN_H

#include <cstdint>

inline constexpr int K_ROWS = 4;
inline constexpr int N_COLUMNS = 7;

void generatePublicKey(const uint8_t* privateG, const uint8_t* privateP, const uint8_t* privateS, uint8_t* publicMatrix);

void generateRandomPermutation(uint8_t* privateP);

void generateRandomScrambler(uint8_t* privateS);

void generateRandomGenerator(uint8_t* privateG); // well this became invertScramblerMatrix's friend becuz ai generated it.

void invertPermutationMatrix(const uint8_t* matrixP, uint8_t* inverseP);

bool invertScramblerMatrix(const uint8_t* matrixS, uint8_t* inverseS); // THIS STANDS OUT.. why? its fucking ai generated TwT

#endif // KEYGEN_H

// THE ONLY HEADER FILE WITH SOME HUMAN MODFICIATIONSSS
