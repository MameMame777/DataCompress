#include "bmpbase.h"
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
  
  // Write the file header
  outFile.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));

  // Write the info header
  outFile.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));

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

  // Assuming 24-bit or 32-bit color depth
  if (infoHeader.biBitCount == 24 || infoHeader.biBitCount == 32) {
      pixelData[pixelIndex] = color.b;     // Blue
      pixelData[pixelIndex + 1] = color.g; // Green
      pixelData[pixelIndex + 2] = color.r; // Red
  }
}
BmpBase BmpBase::CheckerBoard(int width, int height, int bitDepth )
{
  BmpBase newImage(width, height, bitDepth);
  for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
          if ((x / 10 + y / 10) % 2 == 0) {
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
  if (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32) {
      std::cerr << "Error: Unsupported bit depth (" << infoHeader.biBitCount << ")." << std::endl;
      return false;
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