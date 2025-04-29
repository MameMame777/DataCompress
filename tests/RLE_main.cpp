#include <string>
#include "../include/RunLength.h"

int main() {
  const std::string picturepath    = "../picture/color/"; // Replace with your BMP file path
  const std::string picturepathmono= "../picture/mono/";  // Replace with your BMP file path
  const std::string outputpath     = "../picture/output/";  // Replace with your BMP file path
  

  const std::string filename       = "checker";         // Replace with your BMP file path
  const std::string filename_bmp   = ".bmp";            // Replace with your BMP file path
  const std::string filename_rle   = "_compressed.rle"; // Replace with your BMP file path
  const std::string filename_decomp= "_decompressed.bmp"; // Replace with your BMP file path
  
  const std::string inputFilePath        = picturepathmono+filename+filename_bmp; // Replace with your BMP file path
  
  const std::string compressedFilePath   = outputpath+filename+filename_rle; // 圧縮データの保存先
  const std::string decompressedFilePath = outputpath+filename+filename_decomp; // 復元後のBMPファイル

  RLEprocessImage(inputFilePath, compressedFilePath, decompressedFilePath);

  return 0;
}