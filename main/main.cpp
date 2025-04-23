#include <stdio.h>
#include "bmpbase.h"

int main() {
  BmpBase bmp;
  bmp.saveBMP("output.bmp"); // Save the BMP file
  printf("Hello, BMP World!\n");
  
}