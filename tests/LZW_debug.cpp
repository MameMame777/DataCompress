#include <string>
#include <filesystem>
#include "../include/LZWEncoding.h"
#include "../include/TestImageProcessing.h"

int main() {
  Testing::TestImageProcessing TestImageProcessing;


  const std::string picturepathdebug  = "../picture/debug/";  // Replace with your BMP file path
  const std::string outputpathmono   = "../picture/output/debug/";  // Replace with your BMP file path
  const std::string filename_bmp   = ".bmp";            // Replace with your BMP file path
  const std::string filename_lzw   = "_compressed.lzw"; // Replace with your BMP file path
  const std::string filename_decomp= "_lzw_decompressed.bmp"; // Replace with your BMP file path  

  //std::string filename       = "debugdata1_8_grad";         // Replace with your BMP file path
  std::string filename       = "LENNA";         // Replace with your BMP file path
  

  const std::string inputFilePath        = picturepathdebug+filename+filename_bmp; // Replace with your BMP file path
  const std::string compressedFilePath   = outputpathmono+filename+filename_lzw; 
  const std::string decompressedFilePath = outputpathmono+filename+filename_decomp; 
  
  if (std::filesystem::exists(decompressedFilePath)) {
    std::filesystem::remove(decompressedFilePath);
    std::cout << "Deleted file: " << decompressedFilePath << std::endl;
  } 
  LZWProcessing::LZWprocessImage(inputFilePath, compressedFilePath, decompressedFilePath);
  TestImageProcessing.compareImages   (inputFilePath, decompressedFilePath);
  TestImageProcessing.compareFileSizes(inputFilePath, decompressedFilePath);

  return 0;
}