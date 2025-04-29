#include <iostream>
#include "../include/bmpbase.h" // Adjust the path to the actual location of bmpbase.h
int main() {
  const int width = 256;  // Image width
  const int height = 256; // Image height
  const int bitDepth = 8; // Bit depth

  BmpBase bmp;

  // Call some methods of bmpbase to verify functionality
  bmp.CreateNewImage(width, height, bitDepth);       // Create a new image
  bmp.CreatePixelData(width, height, bitDepth);      // Create pixel data for the image
  bmp.CreateHeader(width, height, bitDepth);         // Create the header for the image
  bmp = bmp.CheckerBoard(width, height, bitDepth);   // Create a checkerboard pattern image
  std::cout << "BMP image created successfully." << std::endl;
  bmp.saveBMP("checker_8bit.bmp");                   // Save the BMP file
  std::cout << "BMP image saved as output.bmp." << std::endl;

  return 0;
}
