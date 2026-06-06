#include "math_engine.h"

uint8_t vectorMatrixMultiply(uint8_t msgVec, const uint8_t* matrix, int rows, int columns) {
    uint8_t resultVec = 0;
    
    for (int c=0; c < columns; ++c) {
        bool bitsum = 0;

        for (int r=0; r < rows; ++r) {

            bool msgBit = (msgVec >> (rows - 1 - r)) & 1;
            bool matrixBit = (matrix[r] >> (columns - 1 - c)) & 1;

            bitsum ^= msgBit & matrixBit;
        }

        if (bitsum) {
            resultVec |= (1 << (columns - 1 - c));
        }
    }

    return resultVec;
}

void matrixMultiply(const uint8_t* matrixA, const uint8_t* matrixB, uint8_t* resultMatrix, int rA, int cA, int cB) {
    for (int r=0; r < rA; r++) {
        resultMatrix[r] = 0;
        for (int c=0; c < cB; c++) {
            bool bitSum = 0;
            for (int k = 0; k < cA; k++) {
                bool bitA = (matrixA[r] >> (cA - 1 - k)) & 1;
                bool bitB = (matrixB[k] >> (cB - 1 - c)) & 1;
                bitSum ^= (bitA & bitB);
            }
            if (bitSum) {
                resultMatrix[r] |= (1 << (cB - 1 - c));
            }
        }
    }
}
