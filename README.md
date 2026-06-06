# M-78 Cipher

M78 Cipher is an asymmetric, post quantum secure cryptographic prototype based on the mathematical foundations of **McEliece Cryptosystem** (1978)
Tradional PublicKey Algorithms (RSA or ECC) which can be broken by Shor's Algorithm on quantum computers, M78Cipher relies on the NP-Hard problem of decoding a randomized linear block code over $GF(2)$.
This code includes the simulation of the code currently with validated $(7, 4)$ Hamming matrix configuration paired with automated syndrone error correction

Expect a main.cpp to be replaced with easy to use functions in ver 0.3.0 and main.cpp simulations to be moved to an cmd integration of this in cmd.cpp in ver 0.4.0

# System Architecture

The entire core engine operates over Galois Field 2 ($GF(2)$) arithmetic, swapping traditional arithmetic for bitwise `AND` (multiplication) and bitwise `XOR` (modulo-2 addition).

## Encryption:
The Plain Text String
        │
        ▼
Split into 4-Bit Nibbles
        │
        ▼ 
Encryption Block: Multiplies data by 4x7 Public Key Matrix (G_pub)
        │
        ▼
Inject Noise: Randomly injects a 1-bit flip into the cipher block
        │
        ▼
Encrypted Data 

## Decryption:

Encrypted Data
        │
        ▼
Remove Permutation: Unshuffles bits back to native seats via P_INV
        │
        ▼
Clean Radio Noise: Dyanmic H Matrix captures syndrome & corrects flipped bit
        │
        ▼
Remove scrambler: Strips linear transformation via S_INV
        │
        ▼
Decrypted Data = The Plain Text String

(FOR SIMULATION OF THE ARDUINO - To Point Of Encrypted Data is Arduino Transmitter)
Encrypted Data
        │
        ▼
Virtual Airwaves Vector Stream: Transmits via 7-bit dynamic array (END OF ARDUINO TRANSMITTER)
        │
        ▼
Remove Permutation: Unshuffles bits back to native seats via P_INV (ARDUINO RECIEVER RECIEVES ENCRYPTED DATA)
        │
        ▼
Clean Radio Noise: Dyanmic H Matrix captures syndrome & corrects flipped bit
        │
        ▼
Remove scrambler: Strips linear transformation via S_INV
        │
        ▼
Decrypted Data = The Plain Text String

### KEY GENERATION

Every single runtime session generates fully different variables:
1. **Private Matrix G:** A dynamic $(7,4)$ systematic linear code generator matrix.
2. **Private Scrambler S:** A secret, invertible $4 \times 4$ transformation matrix generated via LU decomposition.
3. **Private Permutation P:** A secret $7 \times 7$ orthogonal bit-shuffling matrix.
4. **Public Key Matrix ($G_{pub}$):** Computed seamlessly as $G_{pub} = S \times G \times P$.


### A little problem that took a while to solve. Noise.

For a McEliece syndrome decoder to function with absolute precision, the Parity-Check matrix ($H$) must contain completely unique, non-zero row signatures. Standard random key generators frequently roll invalid linear code spaces, leading to ambiguous decoding states.
M78-Cipher features a custom runtime **validation loop** inside the key generator. If a rolled matrix introduces duplicate tracking vectors, the engine rejects it and automatically rerolls a new matrix until it locks down a mathematically pristine configuration. This guarantees $100\%$ noise cancellation on every boot.


# Compiling and Running

Compile the prototype on MacOS, Windows, or Linux using any modern compiler supporting C++17:

```bash
clang++ -std=c++17 main.cpp math_engine.cpp key_gen.cpp encoder.cpp decoder.cpp -o m78_sim
```
### Execute the Simulation:
```bash
./m78_sim
```

## Metadata and Credits

**Author** : InternecionCrystal0n
**Current Version** : `0.1.0 - Prototype`
**Theoretical Foundations** : Based on the McEliece Cryptosystem invented by Robert McEliece in 1978.
**Development Insights** : Built with human architectural concepts, assisted by AI for targeted pointer debugging, bitwise endianness sorting, and header configurations. Fixed a bunch of bugs to ensure complete keyboard layout compatibility.
**AI USAGE**: The code uses AI for debugging of the noise and speed up of research only.
**LICENSE**: MIT LICENSE