#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <vector>
#include <cstdint>
class ImageProcessor {
  //implementation has to be in the derived class
  public:
      virtual ~ImageProcessor() = default;
      void processCompression(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
          prepare(input);
          compress(input, output);
          finalize(output);
      }
  
      void processDecompression(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
          prepare(input);
          decompress(input, output);
          finalize(output);
      }
  
  protected:
      // Pure virtual functions to be implemented by derived classes
      // This function is called before compression/decompression
      virtual void prepare(const std::vector<uint8_t>& input) = 0;
      // This function is called for compression/decompression
      virtual void compress(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) = 0;
      virtual void decompress(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) = 0;
      // This function is called after compression/decompression
      // It can be used to finalize the output, e.g., writing to a file or performing additional processing
      virtual void finalize(const std::vector<uint8_t>& output) = 0;
  };
#endif // IMAGEPROCESSOR_H