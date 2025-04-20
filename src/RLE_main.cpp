#include <string>
#include "../include/RunLengh.h"

int main() {
  const std::string picturepath    = "../picture/";     // Replace with your BMP file path
  const std::string filename       = "checker";         // Replace with your BMP file path
  const std::string filename_bmp   = ".bmp";            // Replace with your BMP file path
  const std::string filename_rle   = "_compressed.rle"; // Replace with your BMP file path
  const std::string filename_decomp= "_decompressed.bmp"; // Replace with your BMP file path
  
  const std::string inputFilePath        = picturepath+filename+filename_bmp; // Replace with your BMP file path
  
  const std::string compressedFilePath   = picturepath+filename+filename_rle; // 圧縮データの保存先
  const std::string decompressedFilePath = picturepath+filename+filename_decomp; // 復元後のBMPファイル

  processImage(inputFilePath, compressedFilePath, decompressedFilePath);

  return 0;
}