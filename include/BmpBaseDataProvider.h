#ifndef BMPBASEDATAPROVIDER_H
#define BMPBASEDATAPROVIDER_H

#include "DataProvider.h"
#include "bmpbase.h"

class BmpBaseDataProvider : public DataProvider {
private:
    BmpBase bmpBase; // BmpBaseインスタンス

public:
    // コンストラクタ: ファイルパスから画像をロード
    explicit BmpBaseDataProvider(const std::string& filePath) {
        if (!bmpBase.load(filePath)) {
            throw std::runtime_error("Failed to load BMP file: " + filePath);
        }
    }

    // 画像データを取得
    const std::vector<uint8_t>& getImageData() const override {
        return bmpBase.getPixelData(); // BmpBaseのピクセルデータを返す
    }

    // ヘッダデータを取得
    const std::vector<uint8_t>& getHeaderData() const override {
        static std::vector<uint8_t> headerData;
        headerData.clear();

        // BitmapFileHeaderをバイト列に変換して追加
        const auto& fileHeader = bmpBase.getFileHeader();
        headerData.insert(headerData.end(), reinterpret_cast<const uint8_t*>(&fileHeader), 
                          reinterpret_cast<const uint8_t*>(&fileHeader) + sizeof(fileHeader));

        // BitmapInfoHeaderをバイト列に変換して追加
        const auto& infoHeader = bmpBase.getInfoHeader();
        headerData.insert(headerData.end(), reinterpret_cast<const uint8_t*>(&infoHeader), 
                          reinterpret_cast<const uint8_t*>(&infoHeader) + sizeof(infoHeader));

        return headerData;
    }
};

#endif // BMPBASEDATAPROVIDER_H