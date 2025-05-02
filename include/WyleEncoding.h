#ifndef WYLEENCODING_H
#define WYLEENCODING_H

#include <string>
namespace WYLEProcessing {
  // processImage 関数の宣言
  void WYLEprocessImage(const std::string& inputFilePath, const std::string& compressedFilePath, const std::string& decompressedFilePath);
}
#endif // WYLEENCODING_H