#ifndef BMPBASE_H
#define BMPBASE_H

#include <string>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <fstream>
#include "bmpheader.h"
#include <iostream>
struct Color {
    uint8_t r, g, b;
};

class BmpBase {
public:
    // Constructor and Destructor
    BmpBase(); // default constructor
    BmpBase(int width, int height, int bitDepth );
    ~BmpBase();

    //bmp image flipper
    void FlipImage(std::vector<uint8_t>& pixelData, int width, int height, int bytesPerPixel,int flipType=0);
    
    // bmp acceassor
    // get file header
    const BitmapFileHeader& getFileHeader() const {
        return fileHeader;
    }
    // get info header
    const BitmapInfoHeader& getInfoHeader() const {
        return infoHeader;
    }
    // get color palette
    const std::vector<BitmapColorPalette>& getColorPalette() const {
        return ColorPalette;
    }
    // get pixel data
    const std::vector<uint8_t>& getPixelData() const {
        return pixelData;
    }

    //Generate BMP file header and info header
    void CreateNewImage(int width, int height, int bitDepth = 24);
    void CreateHeader(int width, int height, int bitDepth = 24);
    void DebugHeaders() const;
    void DebugPixel()  const;
    void CreatePixelData(int width, int height, int bitDepth = 24);

    BmpBase CheckerBoard(int width=100, int height=100, int bitDepth = 24); 
    bool load(const std::string& filePath);
    bool saveBMP(const std::string& filePath) const;

    // pixel control
    void setPixel(int x, int y, const Color& color);
    Color getPixel(int x, int y) const;
    void setPixelData(const std::vector<uint8_t>& data) {
        pixelData = data;
    }
    void flipHorizontal();
    void flipVertical();
    void resize(int newWidth, int newHeight);
    BmpBase crop(int x, int y, int width, int height) const;


private:
    std::vector<uint8_t> pixelData;
    std::vector<BitmapColorPalette> ColorPalette;
    bool validateHeader() const;
    size_t calculateRowSize() const;
    size_t calculatePixelArraySize() const;

    // メモリ管理
    void allocatePixelData();
};

#endif // BMPBASE_H