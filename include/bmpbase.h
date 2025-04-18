#ifndef BMPBASE_H
#define BMPBASE_H

#include <string>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <fstream>
struct Color {
    uint8_t r, g, b;
};

class BmpBase {
public:
    // Constructor and Destructor
    BmpBase(); // デフォルトコンストラクタ
    BmpBase(int width, int height, int bitDepth = 24);
    ~BmpBase();


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

    void flipHorizontal();
    void flipVertical();
    void resize(int newWidth, int newHeight);
    BmpBase crop(int x, int y, int width, int height) const;


private:
    //
    #pragma pack(push, 1)
    struct BitmapFileHeader {
        uint16_t bfType;        // 'BM'
        uint32_t bfSize;        // size of the file in bytes
        uint16_t bfReserved1;   // reserved; must be 0
        uint16_t bfReserved2;   // reserved; must be 0
        uint32_t bfOffBits;     // offset to start of pixel data
    } fileHeader;
    #pragma pack(pop)
    struct BitmapInfoHeader {
        uint32_t biSize;          // size of this header (40 bytes)
        int32_t biWidth;          // width of the bitmap in pixels
        int32_t biHeight;         // height of the bitmap in pixels
        uint16_t biPlanes;        // number of color planes (must be 1)
        uint16_t biBitCount;      // bits per pixel (1, 4, 8, or 24)
        uint32_t biCompression;   // compression type (0 = none, 1 = RLE-8, 2 = RLE-4)
        uint32_t biSizeImage;     // size of the pixel data (can be 0 for uncompressed bitmaps)
        int32_t biXPelsPerMeter;  // horizontal resolution (pixels per meter)
        int32_t biYPelsPerMeter;  // vertical resolution (pixels per meter)
        uint32_t biClrUsed;       // number of colors in the color palette (0 = default)
        uint32_t biClrImportant;  // number of important colors (0 = all colors are important)
    } infoHeader;

    // ピクセルデータ
    std::vector<uint8_t> pixelData;

    // 内部ヘルパーメソッド
    bool validateHeader() const;
    size_t calculateRowSize() const;
    size_t calculatePixelArraySize() const;

    // メモリ管理
    void allocatePixelData();
};

#endif // BMPBASE_H