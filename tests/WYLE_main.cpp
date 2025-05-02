#include <string>
#include "../include/WyleEncoding.h"
#include "../include/TestImageProcessing.h"

int main() {
  Testing::TestImageProcessing TestImageProcessing;


  const std::string picturepath    = "../picture/color/"; // Replace with your BMP file path
  const std::string picturepathmono= "../picture/mono/";  // Replace with your BMP file path
  const std::string outputpath     = "../picture/output/";  // Replace with your BMP file path
  

  const std::string filename       = "checker";         // Replace with your BMP file path
  const std::string filename_bmp   = ".bmp";            // Replace with your BMP file path
  const std::string filename_wyle   = "_compressed.wyle"; // Replace with your BMP file path
  const std::string filename_decomp= "_wyle_decompressed.bmp"; // Replace with your BMP file path
  
  const std::string inputFilePath        = picturepathmono+filename+filename_bmp; // Replace with your BMP file path
  
  const std::string compressedFilePath   = outputpath+filename+filename_wyle; 
  const std::string decompressedFilePath = outputpath+filename+filename_decomp; 

  WYLEProcessing::WYLEprocessImage(inputFilePath, compressedFilePath, decompressedFilePath);

  TestImageProcessing.compareImages   (inputFilePath, decompressedFilePath);
  TestImageProcessing.compareFileSizes(inputFilePath, decompressedFilePath);
  return 0;
}