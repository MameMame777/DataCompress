#include <iostream>
#include "../include/bmpbase.h" // Adjust the path to the actual location of bmpbase.h

int main() {
  // Load a BMP file
  const std::string filename = "Mandrill"; // Replace with your BMP file path
  const std::string filepath = filename+".bmp"; // Replace with your BMP file path

  // output bmp file name corresiponding to the input bmp file name
  const std::string outfilepath = filename + "_out.bmp";
  BmpBase bmp;
  if (!bmp.load(filepath)) {
      std::cerr << "Failed to load BMP file: " << filepath << std::endl;
      return 1;
  }
  std::cout << "BMP file loaded successfully." << std::endl;
  #ifdef DEBUG
    bmp.DebugHeaders(); // Debug the headers to see the loaded information
  #endif
  bmp.saveBMP(outfilepath); // Save the BMP file 
  
  return 0;
}