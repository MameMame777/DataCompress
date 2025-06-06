#include <string>
#include <filesystem>
#include "../include/HuffmanEncoding.h"
#include "../include/TestImageProcessing.h"

int main() {
  Testing::TestImageProcessing TestImageProcessing;

  const std::string picturepathcolor = "../picture/color/"; // Replace with your BMP file path
  const std::string picturepathmono  = "../picture/mono/";  // Replace with your BMP file path
  const std::string outputpathmono   = "../picture/output/mono/";  // Replace with your BMP file path
  const std::string outputpathcolor   = "../picture/output/color/";  // Replace with your BMP file path
  
  const std::string filename_bmp   = ".bmp";            // Replace with your BMP file path
  const std::string filename_HUFFMAN   = "_compressed.HUFFMAN"; // Replace with your BMP file path
  const std::string filename_decomp= "_HUFFMAN_decompressed.bmp"; // Replace with your BMP file path  

  for (const auto& pair : TestImageProcessing.MonoToString) {
    
    std::string filename       =  pair.second;         // Replace with your BMP file path
    const std::string inputFilePath        = picturepathmono+filename+filename_bmp; // Replace with your BMP file path
    const std::string compressedFilePath   = outputpathmono+filename+filename_HUFFMAN; 
    const std::string decompressedFilePath = outputpathmono+filename+filename_decomp; 
  
    if (std::filesystem::exists(decompressedFilePath)) {
      std::filesystem::remove(decompressedFilePath);
      std::cout << "Deleted file: " << decompressedFilePath << std::endl;
    } 
    HUFFMANProcessing::HUFFMANprocessImage(inputFilePath, compressedFilePath, decompressedFilePath);
    TestImageProcessing.compareImages   (inputFilePath, decompressedFilePath);
    TestImageProcessing.compareFileSizes(inputFilePath, decompressedFilePath);
  }

  /*
  for (const auto& pair : TestImageProcessing.ColorToString) {
    
    std::string filename       =  pair.second;         // Replace with your BMP file path
    const std::string inputFilePath        = picturepathcolor+filename+filename_bmp; // Replace with your BMP file path
    const std::string compressedFilePath   = outputpathcolor+filename+filename_HUFFMAN; 
    const std::string decompressedFilePath = outputpathcolor+filename+filename_decomp; 
  
    if (std::filesystem::exists(decompressedFilePath)) {
      std::filesystem::remove(decompressedFilePath);
      std::cout << "Deleted file: " << decompressedFilePath << std::endl;
    } 
    HUFFMANProcessing::HUFFMANprocessImage(inputFilePath, compressedFilePath, decompressedFilePath);
    TestImageProcessing.compareImages   (inputFilePath, decompressedFilePath);
    TestImageProcessing.compareFileSizes(inputFilePath, decompressedFilePath);
  }
    */
  return 0;
}