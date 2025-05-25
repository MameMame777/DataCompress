#include <string>
#include <filesystem>
#include "../include/HuffmanEncoding.h"
#include "../include/TestImageProcessing.h"

int main() {
  Testing::TestImageProcessing TestImageProcessing;


  const std::string picturepathdebug  = "../picture/debug/";  // Replace with your BMP file path
  const std::string outputpathmono   = "../picture/output/debug/";  // Replace with your BMP file path
  const std::string filename_bmp   = ".bmp";            // Replace with your BMP file path
  const std::string filename_huffman   = "_compressed.huffman"; // Replace with your BMP file path
  const std::string filename_decomp= "_huffman_decompressed.bmp"; // Replace with your BMP file path  

  std::string filename       = "debugdata1_8_grad";         // Replace with your BMP file path
  const std::string inputFilePath        = picturepathdebug+filename+filename_bmp; // Replace with your BMP file path
  const std::string compressedFilePath   = outputpathmono+filename+filename_huffman; 
  const std::string decompressedFilePath = outputpathmono+filename+filename_decomp; 
  
  if (std::filesystem::exists(decompressedFilePath)) {
    std::filesystem::remove(decompressedFilePath);
    std::cout << "Deleted file: " << decompressedFilePath << std::endl;
  } 
  HUFFMANProcessing::HUFFMANprocessImage(inputFilePath, compressedFilePath, decompressedFilePath);
  TestImageProcessing.compareImages   (inputFilePath, decompressedFilePath);
  TestImageProcessing.compareFileSizes(inputFilePath, decompressedFilePath);

  return 0;
}