# initial

## about classes

### BmpBase

BmpBase is a base class that provides foundational functionality for handling BMP image files. It includes methods for reading, writing, and manipulating BMP file headers and pixel data. This class serves as a utility to simplify operations on BMP images and is often extended by other classes for specific compression or processing tasks.

### RunLengh.cpp

RunLengh.cpp performs compression and decompression of BMP images using run-length encoding.  
The compressed image consists of the header file of the original image and the compressed data.

decompression is done by reading the compressed data and reconstructing the original image.
decompressioned header info is used to reconstruct the original image.

The decompressed image is the same as the original image.

#### header data structure

```cpp
    #pragma pack(push, 1)
    struct BitmapFileHeader {
        uint16_t bfType;        // 'BM' for bitmap files
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
```
####　LZW compression 

[aboutLZW.md](./document/aboutLZW.md)