#include <iostream>
#include "../include/bmpbase.h" // Adjust the path to the actual location of bmpbase.h

int main() {
  const int width=256; 
  const int height=256;
  const int bitDepth=24;

  BmpBase bmp;

  // Call some methods of bmpbase to verify functionality
  bmp.CreateNewImage(width, height, bitDepth); // Create a new image with width 200, height 200, and bit depth 24
  bmp.CreatePixelData(width, height, bitDepth); // Create pixel data for the image
  bmp.CreateHeader(width, height, bitDepth); // Create the header for the image
  bmp = bmp.CheckerBoard(width, height, bitDepth); // Create a checkerboard pattern image
  std::cout << "BMP image created successfully." << std::endl;
  bmp.saveBMP("checker.bmp"); // Save the BMP file
  std::cout << "BMP image saved as output.bmp." << std::endl;

  return 0;
}