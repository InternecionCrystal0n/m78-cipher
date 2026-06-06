/*
    ARDUINO VERSION OF THE M78 CIPHER. (MODIFIED FOR RECIEVER)
    IT IS AI GENERATED
    HOWEVER HUMAN MODIFICATIONS IS DONE FOR DEBUGGING
*/

#include <Arduino.h>

// --- SYSTEM MATRIX DIMENSIONS ---
const int K_ROWS = 4;
const int N_COLUMNS = 7;
const int RECEIVER_PIN = 8; // Connect your incoming signal wire to Digital Pin 8

// --- HARDCODED INVERSE PRIVATE KEYS ---
// For this standalone receiver test, these keys match the transmitter's decryption matrices
const uint8_t HARDCODED_P_INV[N_COLUMNS] = { 
    0b0010000, 0b1000000, 0b0000100, 0b0000001, 0b0100000, 0b0000010, 0b0001000 
};
const uint8_t HARDCODED_S_INV[K_ROWS] = { 
    0b1111, 0b0110, 0b0011, 0b1011 
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

// --- ARDUINO MODULE 2: DECODER CORE (PORTED LOGIC) ---

uint8_t removePermutation(uint8_t noisyCiphertext, const uint8_t* privatePInv) {
    uint8_t unshuffledBlock = 0;
    uint8_t inputData[1] = { static_cast<uint8_t>(noisyCiphertext & 0x7F) };
    matrixMultiply(inputData, privatePInv, &unshuffledBlock, 1, 7, 7);
    return unshuffledBlock;
}

uint8_t cleanRadioNoise(uint8_t unshuffledBlock) {
    bool b0 = (unshuffledBlock >> 6) & 1;
    bool b1 = (unshuffledBlock >> 5) & 1;
    bool b2 = (unshuffledBlock >> 4) & 1;
    bool b3 = (unshuffledBlock >> 3) & 1;
    bool b4 = (unshuffledBlock >> 2) & 1;
    bool b5 = (unshuffledBlock >> 1) & 1;
    bool b6 = unshuffledBlock & 1;

    uint8_t s0 = b4 ^ b0 ^ b1 ^ b2;
    uint8_t s1 = b5 ^ b1 ^ b2 ^ b3;
    uint8_t s2 = b6 ^ b0 ^ b1 ^ b3; // Fixed error math matching your G template

    uint8_t syndrome = (s2 << 2) | (s1 << 1) | s0;

    // QR-style error healing bitmask switches
    if (syndrome == 0b001)      unshuffledBlock ^= (1 << 4);
    else if (syndrome == 0b010) unshuffledBlock ^= (1 << 5);
    else if (syndrome == 0b100) unshuffledBlock ^= (1 << 6);
    else if (syndrome == 0b101) unshuffledBlock ^= (1 << 0);
    else if (syndrome == 0b111) unshuffledBlock ^= (1 << 1);
    else if (syndrome == 0b011) unshuffledBlock ^= (1 << 2);
    else if (syndrome == 0b110) unshuffledBlock ^= (1 << 3);

    return unshuffledBlock;
}

uint8_t removeScrambler(uint8_t cleanBlock, const uint8_t* privateSInv) {
    uint8_t unscrambledBlock = 0;
    uint8_t coreDataBits = (cleanBlock >> 3) & 0x0F;
    uint8_t inputData[1] = { static_cast<uint8_t>(coreDataBits) };
    matrixMultiply(inputData, privateSInv, &unscrambledBlock, 1, 4, 4);
    return unscrambledBlock;
}

// --- ARDUINO MODULE 3: RADIO BIT CAPTURE ENGINE ---

// Reads your physical input pin and samples the length of the wave pulse
bool readBit() {
    unsigned long duration = pulseIn(RECEIVER_PIN, HIGH, 50000); // 50ms timeout window
    if (duration > 200) {
        return 1; // High frequency carrier wave detected -> 1
    }
    delay(2); // Match transmitter timing if dead silent
    return 0; // Pure silence -> 0
}

// --- HARDWARE INITIALIZATION ---
void setup() {
    Serial.begin(9600);
    pinMode(RECEIVER_PIN, INPUT);
    Serial.println(F("[+] M78-Cipher Receiver Operational. Listening for data..."));
}

// --- MAIN RUNTIME CAPTURE LOOP ---
void loop() {
    // 1. SCAN MODE: Wait until we find the unique "1 1 0" Preamble Wakeup Beacon
    if (digitalRead(RECEIVER_PIN) == HIGH) {
        if (readBit() == 1 && readBit() == 1 && readBit() == 0) {
            
            Serial.println(F("[!] Preamble Detected. Capturing stream..."));
            
            uint8_t receivedHigh = 0;
            uint8_t receivedLow = 0;

            // 2. CAPTURE HIGH BLOCK (7 Bits)
            for (int i = 6; i >= 0; i--) {
                if (readBit()) {
                    receivedHigh |= (1 << i);
                }
            }

            // 3. CAPTURE LOW BLOCK (7 Bits)
            for (int i = 6; i >= 0; i--) {
                if (readBit()) {
                    receivedLow |= (1 << i);
                }
            }

            // 4. DECRYPTION EXECUTION PIPELINE
            // Step A: Undo column shuffling matrix
            uint8_t unshuffledHigh = removePermutation(receivedHigh, HARDCODED_P_INV);
            uint8_t unshuffledLow = removePermutation(receivedLow, HARDCODED_P_INV);

            // Step B: Pinpoint and wipe out the injected noise/airway drops
            uint8_t cleanHigh = cleanRadioNoise(unshuffledHigh);
            uint8_t cleanLow = cleanRadioNoise(unshuffledLow);

            // Step C: Strip final matrix scrambler layer
            uint8_t finalHigh = removeScrambler(cleanHigh, HARDCODED_S_INV);
            uint8_t finalLow = removeScrambler(cleanLow, HARDCODED_S_INV);

            // Recombine the two 4-bit nibbles back into a clean 8-bit letter text
            char decryptedChar = (finalHigh << 4) | finalLow;

            // 5. OUTPUT TO SERIAL MONITOR
            Serial.print(F("[=] Decrypted Character output: "));
            Serial.println(decryptedChar);
            Serial.println(F("----------------------------------------"));
        }
    }
}
