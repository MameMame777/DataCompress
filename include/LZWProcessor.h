#ifndef LZWPROCESSOR_H
#define LZWPROCESSOR_H

#include "ImageProcessor.h"
#include "BmpBaseDataProvider.h"

#include <vector>
#include <cstdint>
#include <stdexcept>
namespace LZWProcessing {
/**
 * @brief Specifies the traversal mode for processing image data.
 * 
 * This enumeration defines the modes in which the image data can be traversed
 * during compression or decompression.
 */  


  #define NOT_USED -1
  /**
   * @brief LZW_WORD_STRUCT. 
   */
  typedef struct LZW_WORD_STRUCT
  {
    int charactor;
    int parent_code;
  }Word;

/**
 * @brief Processes an image using LZW encoding.
 * @details This class provides methods to compress and decompress image data using LZW encoding.
 */
class LZWProcessor : public ImageProcessor {
public:
    static int initDictionary          (Word *dictionary,int dic_size,int charactors);
    static int searchWordFromDictionary(Word *dictionary,int charactor,int parent,int size);
    static int addWordToDictionary     (Word *dictionary,int parent,int c,int size);
    static int outputOriginalData      (std::vector<uint8_t>& output, int code, Word* dictionary, int size) ;
  /**
   * @brief Constructs a LZWProcessor object.
   * @param provider The data provider for the image data.
   * @param mode The traversal mode for processing the image data (default is RowWise).
   */
  explicit LZWProcessor(const DataProvider& provider );
  ~LZWProcessor() override;
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
    int width;
    int height;
    std::vector<uint8_t> imageData; 

};
}
#endif // LZWPROCESSOR_H