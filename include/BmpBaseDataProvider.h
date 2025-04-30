#ifndef BMPBASEDATAPROVIDER_H
#define BMPBASEDATAPROVIDER_H

#include "DataProvider.h"
#include "bmpbase.h"
class BmpBaseDataProvider : public DataProvider {
private:
    BmpBase bmpBase; 

public:
    explicit BmpBaseDataProvider(const std::string& filePath) {
        if (!bmpBase.load(filePath)) {
            throw std::runtime_error("Failed to load BMP file: " + filePath);
        }
    }
    const std::vector<uint8_t>& getImageData() const override {
        return bmpBase.getPixelData(); // BmpBaseのピクセルデータを返す
    }
    const std::vector<uint8_t>& getHeaderData() const override {
        static std::vector<uint8_t> headerData;
        headerData.clear();
        //Convert  BitmapFileHeader to byte array 
        const auto& fileHeader = bmpBase.getFileHeader();
        headerData.insert(headerData.end(), reinterpret_cast<const uint8_t*>(&fileHeader), 
                            reinterpret_cast<const uint8_t*>(&fileHeader) + sizeof(fileHeader));
        //Convert BitmapInfoHeader to byte array
        const auto& infoHeader = bmpBase.getInfoHeader();
        headerData.insert(headerData.end(), reinterpret_cast<const uint8_t*>(&infoHeader), 
                            reinterpret_cast<const uint8_t*>(&infoHeader) + sizeof(infoHeader));
        //Convert ColorPalette to byte array
        if (bmpBase.getInfoHeader().biBitCount == 8) {
            const auto& colorPalette = bmpBase.getColorPalette();
            headerData.insert(headerData.end(), reinterpret_cast<const uint8_t*>(colorPalette.data()), 
                              reinterpret_cast<const uint8_t*>(colorPalette.data()) + colorPalette.size() * sizeof(BitmapColorPalette));
        }
        return headerData;
    }
};

#endif // BMPBASEDATAPROVIDER_H