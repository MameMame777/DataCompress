#include <vector>
#include <iostream>
#include <cassert>
#include <cstdint>

void testWyleEncodingDecoding() {
    // Test values
    std::vector<int> testValues = {0,0,0,1,3,7,0xF,0xff,0x1F,0x3F,0x7F,0xFF,0x1FF,0x3FF,0x7FF,0xFFF,0x1FFF,0x3FFF,0x7FFF,0xFFFF};

    // Buffer to simulate bit-level output
    std::vector<uint8_t> compressedData;
    int bits = 0;
    int bdata = 0;

    // Lambda to simulate fputBit
    auto fputBit = [&](int bit) {
        bdata = (bdata << 1) | bit;
        bits++;
        if (bits == 8) {
            compressedData.push_back(bdata);
            bits = 0;
            bdata = 0;
        }
    };

    // Lambda to flush remaining bits
    auto flushBit = [&]() {
        if (bits > 0) {
            bdata <<= (8 - bits); // Left-align remaining bits
            compressedData.push_back(bdata);
            bits = 0;
            bdata = 0;
        }
    };

    // Lambda to encode using Wyle encoding
    auto outputWyleCode = [&](int value) {
        int bit;
        int x =value;
        int length = 0;

        // Calculate the header length
        int temp = x / 4;
        while (temp > 0) {
            temp /= 2;
            length++;
        }

        // Output the header
        for (int i = 0; i < length; i++) {
            fputBit(1);
        }
        fputBit(0);

        // Output the value
        length += 2;
        for (int i = 0; i < length; i++) {
            fputBit(x & 1);
            x >>= 1;
        }
    };

    // Encode all test values
    for (int value : testValues) {
        outputWyleCode(value);
    }
    flushBit(); // Flush remaining bits

    // Simulate bit-level input for decoding
    int inputIndex = 0;
    int inputBits = 0;
    int inputData = 0;

    // Lambda to simulate fgetBit
    auto fgetBit = [&]() -> int {
      //inputBits = 0;
      //inputData = 0;
      if (inputBits == 0) {
          inputData = compressedData[inputIndex++];
          inputBits = 8;
      }
      int val = (inputData >> 7) & 1;
      inputData = (inputData << 1) & 0xFF;
      inputBits--;
      return val;
    };

    // Lambda to decode using Wyle decoding
    auto readHUFFMANCode = [&]() -> int {
    int headsize = 0;
    while (fgetBit() == 1) {
        headsize++;
    }
    headsize += 2;

    int answer = 0;
    for (int i = 0; i < headsize; i++) {
        int bit = fgetBit() << i;
        answer |= bit;
    }
    return answer;
    };

    // Decode all values and verify
    for (int originalValue : testValues) {
        int decodedValue = readHUFFMANCode(); // Adjust for encoding
        std::cout << "Original: " << originalValue << ", Decoded: " << decodedValue << std::endl;
        assert(originalValue == decodedValue); // Ensure values match
    }

    std::cout << "All tests passed!" << std::endl;
}

int main() {
    testWyleEncodingDecoding();
    return 0;
}