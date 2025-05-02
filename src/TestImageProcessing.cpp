#include "../include/TestImageProcessing.h"

namespace Testing {
 
  bool TestImageProcessing::compareImages(const std::string& beforePath, const std::string& afterPath) {
  std::vector<uint8_t> beforeData = TestImageProcessing::readFile(beforePath);
  std::vector<uint8_t> afterData = TestImageProcessing::readFile(afterPath);

    if (beforeData.size() != afterData.size()) {
        std::cerr << "*** File sizes are different: "
                  << beforeData.size() << " vs " << afterData.size() << std::endl;
        return false;
    }
    for (size_t i = 0; i < beforeData.size(); ++i) {
        if (beforeData[i] != afterData[i]) {
            std::cerr << "(x_x)Difference found at byte " << i << ": "
                      << static_cast<int>(beforeData[i]) << " vs "
                      << static_cast<int>(afterData[i]) << std::endl;
            return false;
        }
    }
    std::cout << "Images are identical. (^_^)b" << std::endl;
    return true;
  }

  bool TestImageProcessing::compareFileSizes(const std::string& beforePath, const std::string& afterPath) {
    auto beforeSize = std::filesystem::file_size(beforePath);
    auto afterSize = std::filesystem::file_size(afterPath);
  
    if (beforeSize != afterSize) {
        std::cerr << "***(x_x) File sizes are different: "
                  << beforeSize << " bytes vs " << afterSize << " bytes" << std::endl;
        return false;
    }
  
    std::cout << "File sizes are identical: " << beforeSize << " bytes. (^_^)b" << std::endl;
    return true;
  }

  std::vector<uint8_t> TestImageProcessing::readFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }
    return std::vector<uint8_t>(std::istreambuf_iterator<char>(file), {});
  }
}