#ifndef HOFFMANPROCESSOR_H
#define HOFFMANPROCESSOR_H

#include "ImageProcessor.h"
#include "BmpBaseDataProvider.h"

#include <vector>
#include <cstdint>
#include <stdexcept>
namespace HOFFMANProcessing {
/**
 * @brief Specifies the traversal mode for processing image data.
 * 
 * This enumeration defines the modes in which the image data can be traversed
 * during compression or decompression.
 */  
enum class TraversalMode {
  /**
   * @brief Process the image row by row.
   * 
   * In this mode, the image data is traversed and processed row by row.
   */
  RowWise,
  /**
   * @brief Process the image column by column.
   * 
   * In this mode, the image data is traversed and processed column by column.
   */
  ColumnWise

};

  constexpr int N = 256; 
  constexpr int NODATA = 2100000000; // Arbitrary value to indicate no data
  /**
   * @brief Huffman Tree structure. 
   */
  typedef struct HUFFTREE {
	  int treesize;
	  int left_node[2*N],right_node[2*N];
	  int parent[2*N];
  } HuffmanTree;
/**
 * @brief Processes an image using Huffman encoding.
 * @details This class provides methods to compress and decompress image data using Huffman encoding.
 */
class HOFFMANProcessor : public ImageProcessor {
public:
  /**
   * @brief Constructs a HOFFMANProcessor object.
   * @param provider The data provider for the image data.
   * @param mode The traversal mode for processing the image data (default is RowWise).
   */
  explicit HOFFMANProcessor(const DataProvider& provider, TraversalMode mode = TraversalMode::RowWise);
  ~HOFFMANProcessor() override;
  /**
   * @brief  Prepares the image data for processing.but not used in this class.
   * @param input The input image data. 
   */
  void prepare(const std::vector<uint8_t>& input) override;
  /**
   * @brief Compresses the image data.
   * @param input The input image data.
   * @param output The output compressed data.
   */
  void compress(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) override;
  /**
   * @brief Decompresses the image data.
   * @param input The input compressed data.
   * @param output The output decompressed data.
   */ 
  void decompress(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) override;
  /**
   * @brief Finalizes the output data after processing.　but not used in this class.
   * @param output The output data to be finalized.
   */
  void finalize(const std::vector<uint8_t>& output) override;

private:
  TraversalMode traversalMode;
  int width;  // image width
  int height; // image height
  const std::vector<uint8_t>& imageData; // pixel data
  /** @brief Compresses the image data in row-wise order.
   *  @param input The input image data.
   *  @param output The output compressed data.
   */
  void compressRowWise(const std::vector<uint8_t>& input, std::vector<uint8_t>& output);
  /** @brief Compresses the image data in column-wise order.
   * @param input The input image data.
   * @param output The output compressed data.
   */
  void compressColumnWise(const std::vector<uint8_t>& input, std::vector<uint8_t>& output);
  /** @brief deCompresses the image data in column-wise order.
   *  @param input The input image data.
   *  @param output The output compressed data.
   */
  void decompressRowWise(const std::vector<uint8_t>& input, std::vector<uint8_t>& output);
  /** @brief deCompresses the image data in column-wise order.
   *  @param input The input image data.
   *  @param output The output compressed data.
   */
  void decompressColumnWise(const std::vector<uint8_t>& input, std::vector<uint8_t>& output);
};
}
#endif // HOFFMANPROCESSOR_H