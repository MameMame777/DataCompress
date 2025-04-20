#include "bmpbase.h"
#include "BmpBaseDataProvider.h"
#include "RLEProcessor.h"
#include <iostream>
#include <vector>
#include <string>

void processImage(const std::string& inputFilePath, const std::string& compressedFilePath, const std::string& decompressedFilePath) {
  try {
      // load the image using BmpBaseDataProvider
      BmpBaseDataProvider provider(inputFilePath);

      // RLEProcessor を作成 (行方向の操作モードを指定)
      RLEProcessor processor(provider, TraversalMode::RowWise);

      // the vecotor to store compressed data
      std::vector<uint8_t> compressedData;

      // 圧縮処理
      std::cout << "Compressing image..." << std::endl;
      processor.compress(provider.getImageData(), compressedData);
      std::cout << "Compression completed. Compressed data size: " << compressedData.size() << " bytes" << std::endl;

      // 圧縮データをファイルに保存 (オプション)
      std::ofstream compressedFile(compressedFilePath, std::ios::binary);
      if (compressedFile) {
          compressedFile.write(reinterpret_cast<const char*>(compressedData.data()), compressedData.size());
          compressedFile.close();
          std::cout << "Compressed data saved to: " << compressedFilePath << std::endl;
      }

      // 復元データを格納するベクタ
      std::vector<uint8_t> decompressedData;

      // 復元処理
      std::cout << "Decompressing image..." << std::endl;
      processor.decompress(compressedData, decompressedData);
      std::cout << "Decompression completed. Decompressed data size: " << decompressedData.size() << " bytes" << std::endl;

      // 復元データを新しいBMP画像として保存
      BmpBase decompressedImage;
      decompressedImage.CreateNewImage(provider.getHeaderData()[18] | (provider.getHeaderData()[19] << 8),
                                       provider.getHeaderData()[22] | (provider.getHeaderData()[23] << 8),
                                       24); // ビット深度は24ビットを仮定
      // set pixel data
      decompressedImage.setPixelData(decompressedData);
      decompressedImage.saveBMP(decompressedFilePath);
      std::cout << "Decompressed image saved to: " << decompressedFilePath << std::endl;

  } catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << std::endl;
  }
}