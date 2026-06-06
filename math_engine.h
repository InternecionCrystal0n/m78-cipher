#ifndef MATH_ENGINE_H
#define MATH_ENGINE_H

#include <vector>
#include <cstdint>

uint8_t vectorMatrixMultiply(uint8_t msgVec, const uint8_t* matrix, int rows, int columns);

void matrixMultiply(const uint8_t* matrixA, const uint8_t* matrixB, uint8_t* resultMatrix, int rA, int cA, int cB);


#endif // MATH_ENGINE_H