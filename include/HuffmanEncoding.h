#ifndef HUFFMANENCODING_H
#define HUFFMANENCODING_H

#include <string>
/**
 * @brief Huffman Encoding and Decoding
 * 
 */
namespace HUFFMANProcessing {
/**
* @brief Processes an image using Huffman encoding.
* @param inputFilePath Path to the input image file.
* @param compressedFilePath Path to the output compressed file.
* @param decompressedFilePath Path to the output decompressed file.
* @details This function reads an image from the specified input file, compresses it using Huffman encoding,
*/
  void HUFFMANprocessImage(const std::string& inputFilePath, const std::string& compressedFilePath, const std::string& decompressedFilePath);

}
#endif // HUFFMANENCODING_H