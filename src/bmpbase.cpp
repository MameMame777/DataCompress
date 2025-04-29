#include "../include/bmpbase.h"
#include "../include/Globalconfig.h"

#include <iostream>

BmpBase::BmpBase() {
  // デフォルトコンストラクタの実装
  CreateNewImage(100, 100, 24); // デフォルトで幅100、高さ100、ビット深度24の画像を作成
}
BmpBase::BmpBase(int width, int height, int bitDepth) {
  CreateNewImage(width, height, bitDepth);
}
BmpBase::~BmpBase() {
    // destructor
}

//methods 
void BmpBase::FlipImage(std::vector<uint8_t>& pixelData, int width, int height, int bytesPerPixel, int flipType) {
  int rowSize = width * bytesPerPixel;

  if (flipType == GlobalConfig::vertically) {
      // flip vertically
      for (int y = 0; y < height / 2; ++y) {
          int topIndex = y * rowSize;
          int bottomIndex = (height - 1 - y) * rowSize;

          for (int x = 0; x < rowSize; ++x) {
              std::swap(pixelData[topIndex + x], pixelData[bottomIndex + x]);
          }
      }
  } else if (flipType == GlobalConfig::horizontally) {
      // flip horizontally
      for (int y = 0; y < height; ++y) {
          int rowStart = y * rowSize;
          for (int x = 0; x < width / 2; ++x) {
              for (int b = 0; b < bytesPerPixel; ++b) {
                  std::swap(pixelData[rowStart + x * bytesPerPixel + b],
                            pixelData[rowStart + (width - 1 - x) * bytesPerPixel + b]);
              }
          }
      }
  } else if (flipType == GlobalConfig::both) {
      // flip both vertically and horizontally
      for (int y = 0; y < height / 2; ++y) {
          int topIndex = y * rowSize;
          int bottomIndex = (height - 1 - y) * rowSize;

          for (int x = 0; x < width; ++x) {
              for (int b = 0; b < bytesPerPixel; ++b) {
                  std::swap(pixelData[topIndex + x * bytesPerPixel + b],
                            pixelData[bottomIndex + (width - 1 - x) * bytesPerPixel + b]);
              }
          }
      }

      // 奇数行の場合、中央行を左右反転
      if (height % 2 != 0) {
          int middleRowIndex = (height / 2) * rowSize;
          uint8_t* middleRow = &pixelData[middleRowIndex];

          for (int x = 0; x < width / 2; ++x) {
              for (int b = 0; b < bytesPerPixel; ++b) {
                  std::swap(middleRow[x * bytesPerPixel + b],
                            middleRow[(width - 1 - x) * bytesPerPixel + b]);
              }
          }
      }
  } else {
      throw std::invalid_argument("Invalid flipType value. Supported values are 0 (vertical), 1 (horizontal), or 2 (both).");
  }
}

bool BmpBase::saveBMP(const std::string& filePath) const {
  std::ofstream outFile(filePath, std::ios::binary);
  if (!outFile) {
      return false;
  }
  #ifdef DEBUG
  std::cout << "bfOffBits: " << fileHeader.bfOffBits << std::endl;
  std::cout << "biWidth: " << infoHeader.biWidth << ", biHeight: " << infoHeader.biHeight
            << ", biBitCount: " << infoHeader.biBitCount << std::endl;
  #endif
  
  outFile.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
  outFile.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));
  if (infoHeader.biBitCount == 8) {
    outFile.write(reinterpret_cast<const char*>(ColorPalette.data()), ColorPalette.size() * sizeof(BitmapColorPalette));
  }

  // Write the pixel data with padding
  size_t rowSize = ((infoHeader.biWidth * infoHeader.biBitCount + 31) / 32) * 4;
  size_t paddingSize = rowSize - (infoHeader.biWidth * (infoHeader.biBitCount / 8));
  std::vector<char> padding(paddingSize, 0);

  for (int y = infoHeader.biHeight - 1; y >= 0; --y) {
    outFile.write(reinterpret_cast<const char*>(&pixelData[y * rowSize]), rowSize - paddingSize);
    if (paddingSize > 0) {
        outFile.write(padding.data(), paddingSize);
    }
  }
  std::cout << "Writing BMP Header to File:" << std::endl;
  std::cout << "File Size: " << fileHeader.bfSize << std::endl;
  std::cout << "Data Offset: " << fileHeader.bfOffBits << std::endl;
  std::cout << "Width: " << infoHeader.biWidth << std::endl;
  std::cout << "Height: " << infoHeader.biHeight << std::endl;
  std::cout << "Bit Depth: " << infoHeader.biBitCount << std::endl;
  return outFile.good();
}

void BmpBase::CreateHeader(int width, int height, int bitDepth ){
  fileHeader.bfType = 0x4D42; // 'BM'
  fileHeader.bfSize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + pixelData.size();
  fileHeader.bfReserved1 = 0;
  fileHeader.bfReserved2 = 0;
  fileHeader.bfOffBits = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);

  infoHeader.biSize = sizeof(BitmapInfoHeader);
  infoHeader.biWidth = width;
  infoHeader.biHeight = height;
  infoHeader.biPlanes = 1;
  infoHeader.biBitCount = bitDepth;
  infoHeader.biCompression = 0; // BI_RGB
  infoHeader.biSizeImage = pixelData.size();
  infoHeader.biXPelsPerMeter = 0;
  infoHeader.biYPelsPerMeter = 0;
  infoHeader.biClrUsed = 0;
  infoHeader.biClrImportant = 0;
  size_t paletteSize = (bitDepth == 8) ? 256 * sizeof(BitmapColorPalette) : 0;
  fileHeader.bfOffBits = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + paletteSize;
  fileHeader.bfSize = fileHeader.bfOffBits + pixelData.size();
}

void BmpBase::DebugHeaders() const {
  std::cout << "File Header:" << std::endl;
  std::cout << "  bfType: " << std::hex << fileHeader.bfType << std::endl;
  std::cout << "  bfSize: " << std::dec << fileHeader.bfSize << std::endl;
  std::cout << "  bfOffBits: " << fileHeader.bfOffBits << std::endl;

  std::cout << "Info Header:" << std::endl;
  std::cout << "  biSize: " << infoHeader.biSize << std::endl;
  std::cout << "  biWidth: " << infoHeader.biWidth << std::endl;
  std::cout << "  biHeight: " << infoHeader.biHeight << std::endl;
  std::cout << "  biPlanes: " << infoHeader.biPlanes << std::endl;
  std::cout << "  biBitCount: " << infoHeader.biBitCount << std::endl;
  std::cout << "  biCompression: " << infoHeader.biCompression << std::endl;
  std::cout << "  biSizeImage: " << infoHeader.biSizeImage << std::endl;
  std::cout << "  biXPelsPerMeter: " << infoHeader.biXPelsPerMeter << std::endl;
  std::cout << "  biYPelsPerMeter: " << infoHeader.biYPelsPerMeter << std::endl;
  std::cout << "  biClrUsed: " << infoHeader.biClrUsed << std::endl;
  std::cout << "  biClrImportant: " << infoHeader.biClrImportant << std::endl;
  std::cout << "  pixelData.size(): "<<std::hex  << pixelData.size() << std::endl;
  
}
void BmpBase::DebugPixel() const {
  std::cout << "Pixel Data:" << std::endl;
  for (size_t i = 0; i < pixelData.size(); ++i) {
      std::cout << "  Pixel " << i << ": " << static_cast<int>(pixelData[i]) << std::endl;
  }
}
void BmpBase::CreatePixelData(int width, int height, int bitDepth ) {
  size_t rowSize = ((width * bitDepth + 31) / 32) * 4; // Row size must be a multiple of 4 bytes
  pixelData.resize(rowSize * height);
}

void BmpBase::CreateNewImage(int width, int height, int bitDepth )
{
  if (width <= 0 || height <= 0) {
      throw std::invalid_argument("Width and height must be positive integers.");
  }
  if (bitDepth != 24 && bitDepth != 32) {
      throw std::invalid_argument("Bit depth must be either 24 or 32.");
  }
  CreateHeader(width, height, bitDepth);
  CreatePixelData(width, height, bitDepth);
}
void BmpBase::setPixel(int x, int y, const Color& color) {
  size_t rowSize = ((infoHeader.biWidth * infoHeader.biBitCount + 31) / 32) * 4; // Row size must be a multiple of 4 bytes
  size_t pixelIndex = y * rowSize + x * (infoHeader.biBitCount / 8);

    if (infoHeader.biBitCount == 8) {
        // カラーパレットのインデックスを設定
        for (size_t i = 0; i < ColorPalette.size(); ++i) {
            if (ColorPalette[i].red == color.r && ColorPalette[i].green == color.g && ColorPalette[i].blue == color.b) {
                pixelData[pixelIndex] = static_cast<uint8_t>(i);
                return;
            }
        }
        throw std::runtime_error("Color not found in palette.");
    } else if (infoHeader.biBitCount == 24 || infoHeader.biBitCount == 32) {
        pixelData[pixelIndex] = color.b;
        pixelData[pixelIndex + 1] = color.g;
        pixelData[pixelIndex + 2] = color.r;
    }
}
BmpBase BmpBase::CheckerBoard(int width, int height, int bitDepth )
{
  BmpBase newImage(width, height, bitDepth);
  if (bitDepth == 8) {
    // カラーパレットを設定
    newImage.ColorPalette.resize(256);
    newImage.ColorPalette[0] = {0, 0, 0, 0};       // 黒
    newImage.ColorPalette[1] = {255, 255, 255, 0}; // 白
  }
  for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
          if ((x / 16 + y / 16) % 2 == 0) {
              newImage.setPixel(x, y, {255, 255, 255});
          } else {
              newImage.setPixel(x, y, {0, 0, 0});
          }
      }
  }
  return newImage;
}
//imprementation of load function
bool BmpBase::load(const std::string& filePath) {
  std::ifstream inFile(filePath, std::ios::binary);
  if (!inFile) {
      std::cerr << "Error: Unable to open file " << filePath << std::endl;
      return false;
  }

  // Read the file header
  inFile.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
  if (fileHeader.bfType != 0x4D42) { // 'BM'
      std::cerr << "Error: Not a valid BMP file." << std::endl;
      return false;
  }

  // Read the info header
  inFile.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
  if (infoHeader.biBitCount != 8 && infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32) {
      std::cerr << "Error: Unsupported bit depth (" << infoHeader.biBitCount << ")." << std::endl;
      return false;
  }


  if (infoHeader.biBitCount == 8) {
    size_t paletteSize = infoHeader.biClrUsed ? infoHeader.biClrUsed : 256;
    ColorPalette.resize(paletteSize);
    inFile.read(reinterpret_cast<char*>(ColorPalette.data()), paletteSize * sizeof(BitmapColorPalette));
    if (!inFile) {
        std::cerr << "Error: Failed to read color palette." << std::endl;
        return false;
    }
}
  // Calculate row size and pixel data size
  size_t rowSize = ((infoHeader.biWidth * infoHeader.biBitCount + 31) / 32) * 4;
  size_t pixelDataSize = rowSize * abs(infoHeader.biHeight);
  pixelData.resize(pixelDataSize);

  // Move the file pointer to the pixel data start position
  inFile.seekg(fileHeader.bfOffBits, std::ios::beg);

  // Read the pixel data
  if (!inFile.read(reinterpret_cast<char*>(pixelData.data()), pixelDataSize)) {
      std::cerr << "Error: Failed to read pixel data." << std::endl;
      return false;
  }

  // Reverse the rows to handle the bottom-up format
  std::vector<uint8_t> tempData = pixelData;
  for (int y = 0; y < abs(infoHeader.biHeight); ++y) {
      std::copy(
          tempData.begin() + y * rowSize,
          tempData.begin() + (y + 1) * rowSize,
          pixelData.begin() + (abs(infoHeader.biHeight) - 1 - y) * rowSize
      );
  }

  return true;
}