#include "math_engine.h"
#include "encoder.h"
#include <random>
#include <cstdint>

uint8_t encryptBlock(uint8_t dataNibble, const uint8_t* publicKeyMatrix) {
    uint8_t cipherBlock = 0;
    uint8_t inputData[1] = { static_cast<uint8_t>(dataNibble & 0x0F) };
    matrixMultiply(inputData, publicKeyMatrix, &cipherBlock, 1, 4, 7);
    return cipherBlock;
}


// AI told me to use newer modern random engine so i did. but i still made ittt :3
uint8_t injectNoise(uint8_t pureCiphertext) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 6);

    int rndPos = dis(gen);
    uint8_t noiseMask = 1 << rndPos;
    return pureCiphertext ^ noiseMask;
}
