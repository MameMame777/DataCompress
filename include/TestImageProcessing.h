#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <filesystem>

namespace WYLEProcessing {
  class TestImageProcessing {
    public:
        
        static bool compareImages(const std::string& beforePath, const std::string& afterPath) {
        }
        
        static bool compareFileSizes(const std::string& beforePath, const std::string& afterPath) {
        }
  
    private:
        static std::vector<uint8_t> readFile(const std::string& filePath) {
        }
  };
}