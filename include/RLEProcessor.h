#ifndef RLEPROCESSOR_H
#define RLEPROCESSOR_H

#include "ImageProcessor.h"
#include "BmpBaseDataProvider.h"

#include <vector>
#include <cstdint>
#include <stdexcept>

enum class TraversalMode {
    RowWise,
    ColumnWise
};

class RLEProcessor : public ImageProcessor {
public:
    explicit RLEProcessor(const DataProvider& provider, TraversalMode mode = TraversalMode::RowWise);

    void prepare(const std::vector<uint8_t>& input) override;
    void compress(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) override;
    void decompress(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) override;
    void finalize(const std::vector<uint8_t>& output) override;

private:
    TraversalMode traversalMode;
    int width;  // image width
    int height; // image height
    const std::vector<uint8_t>& imageData; // pixel data

    void compressRowWise(const std::vector<uint8_t>& input, std::vector<uint8_t>& output);
    void compressColumnWise(const std::vector<uint8_t>& input, std::vector<uint8_t>& output);
    void decompressRowWise(const std::vector<uint8_t>& input, std::vector<uint8_t>& output);
    void decompressColumnWise(const std::vector<uint8_t>& input, std::vector<uint8_t>& output);
};

#endif // RLEPROCESSOR_H