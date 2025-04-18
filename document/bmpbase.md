# BMP Base Document

## BMP Base Test
Catch2 is used for unit testing in this project. The test cases are defined in the `test`
directory.

https://engineering.nature.global/entry/blog-fes-2022-catch2

# BMP File Format

The BMP file format, also known as Bitmap Image File or Device Independent Bitmap (DIB) file format,
is a raster graphics image file format used to store bitmap digital images. 
It is widely supported on Windows operating systems.

## Key Features
- **File Extension**: `.bmp` or `.dib`
- **Uncompressed**: BMP files are typically uncompressed, resulting in large file sizes.
- **Color Depth**: Supports various color depths, including 1-bit (monochrome), 4-bit, 8-bit, 16-bit, 24-bit, and 32-bit.
- **Header Information**: Contains metadata such as file size, dimensions, color depth, and compression type.

## Structure
1. **Bitmap File Header**: Contains general information about the file, such as file size and starting address of the pixel array. Typically, this header is 14 bytes in size.
2. **DIB Header**: Provides detailed information about the image, including width, height, color planes, and compression method. The size of this header can vary depending on the BMP version, but common sizes include 12 bytes (BITMAPCOREHEADER) or 40 bytes (BITMAPINFOHEADER).
3. **Color Table** (optional): Used for indexed color images to map pixel values to RGB colors. The size of the color table depends on the color depth of the image.
4. **Pixel Array**: Contains the actual image data, stored as a grid of pixels.

## Advantages
- Simple and easy to implement.
- High-quality images due to lack of compression.

## Disadvantages
- Large file size compared to compressed formats like JPEG or PNG.
- Limited support for advanced features like transparency.

## Common Uses
- Storing high-quality images for editing.
- Used in Windows applications for icons and graphics.

For more details, refer to the [BMP Specification](https://en.wikipedia.org/wiki/BMP_file_format).