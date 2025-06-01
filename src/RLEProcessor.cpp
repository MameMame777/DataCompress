#include "../include/RLEProcessor.h"
namespace RLEProcessing {
    RLEProcessor::RLEProcessor(const DataProvider& provider, TraversalMode mode)
        : traversalMode(mode),
          width(provider.getHeaderData()[18] | (provider.getHeaderData()[19] << 8)),
          height(provider.getHeaderData()[22] | (provider.getHeaderData()[23] << 8)),
          imageData(provider.getImageData()) {}

    void RLEProcessor::prepare(const std::vector<uint8_t>& input) {
        // 必要に応じて準備処理を実装
    }

    void RLEProcessor::compress(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
        if (input.empty()) {
            throw std::invalid_argument("Input data is empty.");
        }
        if (traversalMode == TraversalMode::RowWise) {
            compressRowWise(input, output);
        } else if (traversalMode == TraversalMode::ColumnWise) {
            compressColumnWise(input, output);
        } else {
            throw std::logic_error("Unsupported traversal mode.");
        }
    }

    void RLEProcessor::decompress(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
        if (input.size() % 2 != 0) {
            throw std::invalid_argument("Invalid RLE compressed data.");
        }
        if (traversalMode == TraversalMode::RowWise) {
            decompressRowWise(input, output);
        } else if (traversalMode == TraversalMode::ColumnWise) {
            decompressColumnWise(input, output);
        } else {
            throw std::logic_error("Unsupported traversal mode.");
        }
    }

    void RLEProcessor::finalize(const std::vector<uint8_t>& output) {
        // 必要に応じて後処理を実装
    }

    void RLEProcessor::compressRowWise(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
        size_t i = 0;
        while (i < input.size()) {
            uint8_t value = input[i];
            size_t count = 1;

            while (i + count < input.size() && input[i + count] == value && count < 255) {
                ++count;
            }

            output.push_back(static_cast<uint8_t>(count));
            output.push_back(value);

            i += count;
        }
    }

    void RLEProcessor::compressColumnWise(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
        for (int col = 0; col < width; ++col) {
            int row = 0;
            while (row < height) {
                uint8_t value = input[row * width + col];
                size_t count = 1;

                while (row + count < height && input[(row + count) * width + col] == value && count < 255) {
                    ++count;
                }

                output.push_back(static_cast<uint8_t>(count));
                output.push_back(value);

                row += count;
            }
        }
    }

    void RLEProcessor::decompressRowWise(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
        for (size_t i = 0; i < input.size(); i += 2) {
            uint8_t count = input[i];
            uint8_t value = input[i + 1];

            output.insert(output.end(), count, value);
        }
    }

    void RLEProcessor::decompressColumnWise(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
        output.resize(width * height, 0);

        size_t index = 0;
        for (int col = 0; col < width; ++col) {
            int row = 0;
            while (row < height) {
                if (index >= input.size()) {
                    throw std::invalid_argument("Invalid RLE compressed data.");
                }

                uint8_t count = input[index++];
                uint8_t value = input[index++];

                for (int k = 0; k < count; ++k) {
                    if (row >= height) {
                        throw std::invalid_argument("Decompressed data exceeds expected size.");
                    }
                    output[row * width + col] = value;
                    ++row;
                }
            }
        }
    }
}