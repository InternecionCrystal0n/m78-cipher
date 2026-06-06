/*
    ARDUINO VERSION OF THE M78 CIPHER TRANSMITTER.
    IT IS AI GENERATED
    HOWEVER HUMAN MODIFICATIONS IS DONE FOR DEBUGGING
*/

#include <Arduino.h>

// --- SYSTEM MATRIX DIMENSIONS ---
const int K_ROWS = 4;
const int N_COLUMNS = 7;
const int ANTENNA_PIN = 9; // Connect your 15-30cm copper wire here

// --- CORE MATHEMATICAL BEDROCK (G) ---
const uint8_t PRIVATE_G[4] = {
    0b1000110,
    0b0100101,
    0b0010011,
    0b0001111
};

// --- ARDUINO MODULE 1: MATH ENGINE ---

void matrixMultiply(const uint8_t* matrixA, const uint8_t* matrixB, uint8_t* resultMatrix, int rA, int cA, int cB) {
    for (int r = 0; r < rA; r++) {
        resultMatrix[r] = 0; 
        for (int c = 0; c < cB; c++) {
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

uint8_t vectorMatrixMultiply(uint8_t messageVector, const uint8_t* matrix, int rows, int columns) {
    uint8_t resultVector = 0;
    for (int c = 0; c < columns; c++) {
        bool bitSum = 0;
        for (int r = 0; r < rows; r++) {
            bool msgBit = (messageVector >> (rows - 1 - r)) & 1;
            bool matrixBit = (matrix[r] >> (columns - 1 - c)) & 1;
            bitSum ^= (msgBit & matrixBit);
        }
        if (bitSum) {
            resultVector |= (1 << (columns - 1 - c));
        }
    }
    return resultVector;
}

// --- ARDUINO MODULE 2: KEY GENERATOR (PORTED LABS) ---

void generateRandomPermutation(uint8_t* privateP) {
    int columns[7] = {0, 1, 2, 3, 4, 5, 6};

    // Lightweight AVR Shuffling (Fisher-Yates Algorithm)
    // Replaces heavy desktop std::shuffle seamlessly
    for (int i = 6; i > 0; i--) {
        int j = random(0, i + 1); // Native Arduino true hardware-seeded random
        int temp = columns[i];
        columns[i] = columns[j];
        columns[j] = temp;
    }

    // Your exact bit-painting loop
    for (int r = 0; r < 7; r++) {
        privateP[r] = 0;
        privateP[r] |= (1 << (6 - columns[r]));
    }
}

void generateRandomScrambler(uint8_t* privateS) {
    uint8_t L[4] = {0};
    uint8_t U[4] = {0};

    // Your exact Lower Matrix generation loop
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (r == c) {
                L[r] |= (1 << (3 - c)); 
            } else if (r > c) {
                if (random(0, 2) == 1) { // Upgraded to native Arduino random()
                    L[r] |= (1 << (3 - c));
                }
            }
        }
    }

    // Your exact Upper Matrix generation loop
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (r == c) {
                U[r] |= (1 << (3 - c));
            } else if (r < c) {
                if (random(0, 2) == 1) {
                    U[r] |= (1 << (3 - c));
                }
            }
        }
    }

    matrixMultiply(L, U, privateS, 4, 4, 4);
}

// Blends your dynamic private keys into the public key matrix
void buildPublicKey(const uint8_t* privateS, const uint8_t* privateP, uint8_t* publicMatrix) {
    uint8_t intermediate_S_times_G[4] = {0};
    matrixMultiply(privateS, PRIVATE_G, intermediate_S_times_G, 4, 4, 7);
    matrixMultiply(intermediate_S_times_G, privateP, publicMatrix, 4, 7, 7);
}

// --- ARDUINO MODULE 3: SENDER / TRANSMITTER LOOP ---

uint8_t encryptBlock(uint8_t dataNibble, const uint8_t* publicKeyMatrix) {
    uint8_t inputData = dataNibble & 0x0F;
    return vectorMatrixMultiply(inputData, publicKeyMatrix, 4, 7);
}

uint8_t injectNoise(uint8_t pureCiphertext) {
    int rndPos = random(0, 7);
    uint8_t noiseMask = 1 << rndPos;
    return pureCiphertext ^ noiseMask;
}

// Generates raw radio carrier pulses over your physical antenna wire
void transmitBit(bool bit) {
    if (bit == 1) {
        // Toggle Digital Pin 9 at maximum hardware velocity
        for (int i = 0; i < 400; i++) {
            PINB = _BV(PB1); // Blistering fast direct AVR hardware bit-flip for Pin 9
            delayMicroseconds(1); 
        }
    } else {
        // Keep transmitter off to output a binary 0 (On-Off Keying modulation)
        digitalWrite(ANTENNA_PIN, LOW);
        delay(2); 
    }
}

// --- HARDWARE INITIALIZATION ---
void setup() {
    Serial.begin(9600);
    pinMode(ANTENNA_PIN, OUTPUT);

    // Seed the hardware random registers using noise from an unconnected analog pin
    randomSeed(analogRead(A0));
    
    Serial.println(F("[+] M78-Cipher Arduino Boot Complete. Generating dynamic keys..."));
}

// --- MAIN RUNTIME BROADCAST LOOP ---
void loop() {
    uint8_t privateS[4] = {0};
    uint8_t privateP[7] = {0};
    uint8_t publicKey[4] = {0};

    // 1. Run your custom logic to forge fresh keys dynamically
    generateRandomScrambler(privateS);
    generateRandomPermutation(privateP);
    buildPublicKey(privateS, privateP, publicKey);

    // 2. Data to send (Example character 'H' -> 0b01001000)
    char characterToSend = 'H';
    uint8_t highNibble = (characterToSend >> 4) & 0x0F; // 0b0100
    uint8_t lowNibble = characterToSend & 0x0F;        // 0b1000

    // 3. Encrypt and add your quantum-resistant error noise
    uint8_t secureBlockHigh = injectNoise(encryptBlock(highNibble, publicKey));
    uint8_t secureBlockLow = injectNoise(encryptBlock(lowNibble, publicKey));

    // 4. Send Preamble Wakeup Beacon (1 1 0) so the Mac knows data is hitting the airwaves
    transmitBit(1); transmitBit(1); transmitBit(0);

    // 5. Broadcast High Block (7 bits)
    for (int i = 6; i >= 0; i--) {
        transmitBit((secureBlockHigh >> i) & 1);
    }

    // 6. Broadcast Low Block (7 bits)
    for (int i = 6; i >= 0; i--) {
        transmitBit((secureBlockLow >> i) & 1);
    }

    Serial.println(F("[*] Encrypted character data pulsed into the room airspace."));
    delay(3000); // Chill for 3 seconds before shuffling fresh keys and doing it again!
}
