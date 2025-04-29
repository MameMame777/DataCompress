#include "../include/bmpbase.h"
#include "../include/Globalconfig.h"
#include "../include/BmpBaseDataProvider.h"
#include "../include/RLEProcessor.h"
#include <iostream>
#include <vector>
#include <string>



void RLEprocessImage(const std::string& inputFilePath, const std::string& compressedFilePath, const std::string& decompressedFilePath) {

  try {
    // load the image using BmpBaseDataProvider
    BmpBase bmpBase;
    BmpBaseDataProvider provider(inputFilePath);
    RLEProcessor processor(provider, TraversalMode::RowWise);

    int width = provider.getHeaderData()[18] | (provider.getHeaderData()[19] << 8);
    int height = provider.getHeaderData()[22] | (provider.getHeaderData()[23] << 8);
    int bitCount = provider.getHeaderData()[28] | (provider.getHeaderData()[29] << 8);
    int bytesPerPixel = bitCount / 8; // ビット深度をバイト数に変換
    // the vecotor to store compressed data
    std::vector<uint8_t> compressedData;
    auto headerData = provider.getHeaderData();
    std::cout << std::dec << std::endl;
    // compression process
    std::cout << "Compressing image..." << std::endl;
    processor.compress(provider.getImageData(), compressedData);
    std::cout << "Compression completed. Compressed data size: " << compressedData.size() << " bytes" << std::endl;
    // add header data to compressed data
    std::vector<uint8_t> compressedWithHeader = headerData;
    compressedWithHeader.insert(compressedWithHeader.end(), compressedData.begin(), compressedData.end());

    // Store  Header and compressed data  to compressedFile
    std::ofstream compressedFile(compressedFilePath, std::ios::binary);
    if (compressedFile) {
        // compressedWithHeader を保存
        compressedFile.write(reinterpret_cast<const char*>(compressedWithHeader.data()), compressedWithHeader.size());
        compressedFile.close();
        std::cout << "Compressed data with header saved to: " << compressedFilePath << std::endl;
    }

    // vector to store decompressed data
    std::vector<uint8_t> decompressedData;

    // decompression process
    std::cout << "Decompressing image..." << std::endl;
    processor.decompress(compressedData, decompressedData);
    bmpBase.FlipImage(decompressedData, width, height, bytesPerPixel,GlobalConfig::vertically);
    std::cout << "Decompression completed. Decompressed data size: " << decompressedData.size() << " bytes" << std::endl;

    // 復元データを新しいBMP画像として保存
    std::ofstream decompressedFile(decompressedFilePath, std::ios::binary);
    if (decompressedFile) {
      // ヘッダーを書き込む
      auto headerData = provider.getHeaderData();
      decompressedFile.write(reinterpret_cast<const char*>(headerData.data()), headerData.size());

      // ピクセルデータを書き込む
      decompressedFile.write(reinterpret_cast<const char*>(decompressedData.data()), decompressedData.size());
      decompressedFile.close();
      std::cout << "Decompressed image saved to: " << decompressedFilePath << std::endl;
      //show compression ratio 
      int originalSize = provider.getHeaderData()[34] | (provider.getHeaderData()[35] << 8) |
      (provider.getHeaderData()[36] << 16) | (provider.getHeaderData()[37] << 24);

      // biSizeImage が 0 の場合、幅、高さ、ビット深度を使用して計算
      if (originalSize == 0) {
        originalSize = width * height * bytesPerPixel;
      }
      int compressedSize = compressedData.size();
      double compressionRatio = (static_cast<double>(compressedSize) / originalSize) * 100.0;
      std::cout << "Original Size: " << originalSize << " bytes" << std::endl;
      std::cout << "Compressed Size: " << compressedSize << " bytes" << std::endl;
      std::cout << "Compression Ratio: " << compressionRatio << " %" << std::endl;
    }
  } catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << std::endl;
  }
}