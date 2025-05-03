#include "../include/WyleProcessor.h"
namespace WYLEProcessing {
  WYLEProcessor::WYLEProcessor(const DataProvider& provider, TraversalMode mode)
      : traversalMode(mode),
        width(provider.getHeaderData()[18] | (provider.getHeaderData()[19] << 8)),
        height(provider.getHeaderData()[22] | (provider.getHeaderData()[23] << 8)),
        imageData(provider.getImageData()) {}

  WYLEProcessor::~WYLEProcessor() {

  }
  void WYLEProcessor::prepare(const std::vector<uint8_t>& input) {

  }

  void WYLEProcessor::compress(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
    if (input.empty()) {
        throw std::invalid_argument("Input data is empty.");
    }
    if (traversalMode == TraversalMode::RowWise) {
        WYLEProcessor::compressRowWise(input, output);
    } else if (traversalMode == TraversalMode::ColumnWise) {
        WYLEProcessor::compressColumnWise(input, output);
    } else {
        throw std::logic_error("Unsupported traversal mode.");
    }
  }

  void WYLEProcessor::decompress(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
    if (traversalMode == TraversalMode::RowWise) {
        decompressRowWise(input, output);
    } else if (traversalMode == TraversalMode::ColumnWise) {
        decompressColumnWise(input, output);
    } else {
        throw std::logic_error("Unsupported traversal mode.");
    }
  }

  void WYLEProcessor::finalize(const std::vector<uint8_t>& output) {
    // Implement post-processing if necessary
  }


  void WYLEProcessor::compressRowWise(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {

    // Variable to store data at the bit level
    int bits = 0;
    int bdata = 0;

    // Lambda function to output bits
    auto fputBit = [&](int bit) {
        bdata = (bdata << 1) | bit;
        bits++;
        if (bits >= 8) {
            output.push_back(static_cast<uint8_t>(bdata));
            bits = 0;
            bdata = 0;
        }
    };

    // Lambda function to flush remaining bits in the buffer
    auto flushBit = [&]() {
        while (bits > 0 && bits < 8) {
            fputBit(0);
        }
    };

    // Encoding process
    for (size_t i = 0; i < input.size(); i++) {
        int value = input[i] + 1; // WYLE encoding starts from 1, so add 1
        int x = abs(value - 1);  // Calculate absolute value
        int length = 0;

        // Calculate the length of the header part
        int temp = x / 4;
        while (temp > 0) {
            temp /= 2;
            length++;
        }

        // Output the header part
        for (int j = 0; j < length; j++) {
            fputBit(1); // Output 1
        }
        fputBit(0); // Output 0 at the end

        // Output the data part
        length += 2;
        for (int j = 0; j < length; j++) {
            fputBit(x & 1); // Output the least significant bit of x
            x >>= 1;        // Right shift x
        }
    }

    // Flush remaining bits in the buffer
    flushBit();
}

void WYLEProcessor::compressColumnWise(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {

}

void WYLEProcessor::decompressRowWise(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
  // Variables for reading data at the bit level
  int bits = 0;
  int bdata = 0;
  size_t byteIndex = 0;

  // Lambda function to retrieve a bit
  auto fgetBit = [&]() -> int {
    if (bits == 0) { // Load the next byte
      if (byteIndex >= input.size()) {
        throw std::out_of_range("Unexpected end of input data.");
      }
      bdata = input[byteIndex++];
      bits = 8;
    }
    int bit = (bdata >> 7) & 1; // Get the most significant bit
    bdata <<= 1;                // Left shift to prepare for the next bit
    bits--;
    return bit;
  };

  // Decoding process
  while (byteIndex < input.size()) {
    // Read the header part
    int length = 0;
    while (fgetBit() == 1) { // Increment length as long as 1 continues
      length++;
    }

    // Read the data part
    int x = 0;
    for (int i = 0; i < length + 2; i++) {
      x |= (fgetBit() << i); // Reconstruct bits from right to left
    }

    // Calculate the original value and output
    int value = x + 1; // Restore the value by subtracting the added 1 during encoding
    output.push_back(static_cast<uint8_t>(value));
  }
}

void WYLEProcessor::decompressColumnWise(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
    

}

}