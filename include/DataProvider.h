#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include <vector>
#include <cstdint>

class DataProvider {
public:
    virtual ~DataProvider() = default;
    // Get image data
    virtual const std::vector<uint8_t>& getImageData() const = 0;
    // Get header data
    virtual const std::vector<uint8_t>& getHeaderData() const = 0;
};

#endif // DATAPROVIDER_H