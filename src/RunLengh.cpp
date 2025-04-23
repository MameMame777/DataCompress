#include "bmpbase.h"
#include "BmpBaseDataProvider.h"
#include "RLEProcessor.h"
#include <iostream>
#include <vector>
#include <string>

void flipImage(std::vector<uint8_t>& pixelData, int width, int height, int bytesPerPixel) {
  int rowSize = width * bytesPerPixel;

  // 上下反転
  for (int y = 0; y < height / 2; ++y) {
      int topIndex = y * rowSize;
      int bottomIndex = (height - 1 - y) * rowSize;

      for (int x = 0; x < rowSize; ++x) {
          std::swap(pixelData[topIndex + x], pixelData[bottomIndex + x]);
      }
  }

  // 左右反転
  for (int y = 0; y < height; ++y) {
      int rowStart = y * rowSize;
      for (int x = 0; x < width / 2; ++x) {
          for (int b = 0; b < bytesPerPixel; ++b) {
              std::swap(pixelData[rowStart + x * bytesPerPixel + b],
                        pixelData[rowStart + (width - 1 - x) * bytesPerPixel + b]);
          }
      }
  }
}

void RLEprocessImage(const std::string& inputFilePath, const std::string& compressedFilePath, const std::string& decompressedFilePath) {

  try {
      // load the image using BmpBaseDataProvider
    BmpBaseDataProvider provider(inputFilePath);

      // RLEProcessor を作成 (行方向の操作モードを指定)
    RLEProcessor processor(provider, TraversalMode::RowWise);
    // ヘッダー情報から幅、高さ、ビット深度を取得
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
    flipImage(decompressedData, width, height, bytesPerPixel);
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