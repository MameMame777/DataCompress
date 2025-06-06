#ifndef LZWENCODING_H
#define LZWENCODING_H

#include <string>
/**
 * @brief LZW Encoding and Decoding
 * 
 */
namespace LZWProcessing {
/**
* @brief Processes an image using LZW encoding.
* @param inputFilePath Path to the input image file.
* @param compressedFilePath Path to the output compressed file.
* @param decompressedFilePath Path to the output decompressed file.
* @details This function reads an image from the specified input file, compresses it using LZW encoding,
*/
  void LZWprocessImage(const std::string& inputFilePath, const std::string& compressedFilePath, const std::string& decompressedFilePath);

}
#endif // LZWENCODING_H