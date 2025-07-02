#include "../include/LZWProcessor.h"
#include "iostream"
namespace LZWProcessing {
  LZWProcessor::LZWProcessor(const DataProvider& provider)
    : width(provider.getHeaderData()[18] | (provider.getHeaderData()[19] << 8)),
      height(provider.getHeaderData()[22] | (provider.getHeaderData()[23] << 8)),
      imageData(provider.getImageData()) {}

  LZWProcessor::~LZWProcessor() {}

  int LZWProcessor::initDictionary(Word *dictionary, int dic_size, int characters) {
    int i;
    for (i = 0; i < characters; i++) {
      dictionary[i].charactor = i;
      dictionary[i].parent_code = -1;
    }
    for (i = characters; i < dic_size; i++) {
      dictionary[i].charactor = NOT_USED;
      dictionary[i].parent_code = -1;
    }
    return characters;
  }

  int LZWProcessor::searchWordFromDictionary(Word *dictionary, int charactor, int parent, int size) {
    int i;
    for (i = 0; i < size; i++) {
      if (dictionary[i].charactor == NOT_USED) continue;
      if (dictionary[i].parent_code != parent) continue;
      if (dictionary[i].charactor == charactor) return i;
    }
    return -1;
  }

  int LZWProcessor::addWordToDictionary(Word *dictionary, int parent, int c, int size) {
    dictionary[size].charactor = c;
    dictionary[size].parent_code = parent;
    return size + 1;
  }

  int LZWProcessor::outputOriginalData(std::vector<uint8_t>& output, int code, Word* dictionary, int size) {
    if (code >= size || code < 0) {
        throw std::out_of_range("Code is out of dictionary bounds.");
    }

    std::vector<int> buffer; // Temporary buffer to store expanded data
    int first_char = -1;

    // Traverse the dictionary to expand
    int current_node = code;
    while (current_node >= 0) {
        const Word& entry = dictionary[current_node];
        buffer.push_back(entry.charactor);
        current_node = entry.parent_code;

        // Record the first character
        if (first_char == -1) {
            first_char = entry.charactor;
        }
    }

    // Write the expanded data to `output` in reverse order
    for (auto it = buffer.rbegin(); it != buffer.rend(); ++it) {
        output.push_back(static_cast<uint8_t>(*it));
    }

    return first_char; // Return the first character of the expanded data
  }

  void LZWProcessor::prepare(const std::vector<uint8_t>& input) {
    // Implement preparation logic if needed
  }

  void LZWProcessor::compress(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
    if (input.empty()) {
      throw std::invalid_argument("Input data is empty.");
    }

    // Perform 16-bit processing internally
    std::vector<uint16_t> internal_output;
    compressInternal(input, internal_output);
    
    // Encode 16-bit results to 8-bit format
    encodeToUint8(internal_output, output);
  }

  // Internal 16-bit processing function
  void LZWProcessor::compressInternal(const std::vector<uint8_t>& input, std::vector<uint16_t>& output) {
    // Initialization
    int dic_size = 4096;      // Dictionary size
    int characters = 256;     // Number of characters
    int now_parent = -1;
    int now_dic_size;
    int code;

    // Initialize dictionary
    std::vector<Word> dictionary(dic_size);
    now_dic_size = initDictionary(dictionary.data(), dic_size, characters);

    // Compression process
    for (size_t i = 0; i < input.size(); ++i) {
      int c = input[i];
      code = searchWordFromDictionary(dictionary.data(), c, now_parent, now_dic_size);

      if (code < 0) {
        // If not found in dictionary, output current pattern and add new pattern
        if (now_parent >= 0) {
          output.push_back(static_cast<uint16_t>(now_parent)); // 16-bit output
        }
        if (now_dic_size < dic_size) {
          now_dic_size = addWordToDictionary(dictionary.data(), now_parent, c, now_dic_size);
        } else {
          // If dictionary is full, clear dictionary
          now_dic_size = initDictionary(dictionary.data(), dic_size, characters);
        }
        now_parent = c; // Start new pattern with current character
      } else {
        now_parent = code;
      }
    }

    // Output the last code
    if (now_parent >= 0) {
      output.push_back(static_cast<uint16_t>(now_parent)); // 16-bit output
    }

    std::cout << "Internal 16-bit compressed codes: " << output.size() << std::endl;
  }

  // Encode 16-bit data to 8-bit format
  void LZWProcessor::encodeToUint8(const std::vector<uint16_t>& input16, std::vector<uint8_t>& output8) {
    output8.clear();
    output8.reserve(input16.size() * 2); // Reserve maximum size

    for (uint16_t code : input16) {
      if (code <= 254) {
        // 0-254: Directly represent with 1 byte
        output8.push_back(static_cast<uint8_t>(code));
      } else {
        // 255 and above: Use escape sequence
        output8.push_back(255); // Escape marker
        output8.push_back(static_cast<uint8_t>((code >> 8) & 0xFF)); // High byte
        output8.push_back(static_cast<uint8_t>(code & 0xFF)); // Low byte
      }
    }

    std::cout << "Encoded to 8-bit format: " << output8.size() << " bytes" << std::endl;
  }

  void LZWProcessor::decompress(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
    if (input.empty()) {
        throw std::invalid_argument("Input data is empty.");
    }

    // Decode 8-bit input to 16-bit format
    std::vector<uint16_t> internal_input;
    decodeFromUint8(input, internal_input);
    
    // Perform internal 16-bit processing
    decompressInternal(internal_input, output);
  }

  // Decode 8-bit data to 16-bit format
  void LZWProcessor::decodeFromUint8(const std::vector<uint8_t>& input8, std::vector<uint16_t>& output16) {
    output16.clear();
    
    for (size_t i = 0; i < input8.size(); ++i) {
      uint8_t byte = input8[i];
      
      if (byte != 255) {
        // 0-254: Direct 1-byte value
        output16.push_back(static_cast<uint16_t>(byte));
      } else {
        // 255: Escape sequence, next 2 bytes are 16-bit value
        if (i + 2 < input8.size()) {
          uint16_t high = static_cast<uint16_t>(input8[i + 1]);
          uint16_t low = static_cast<uint16_t>(input8[i + 2]);
          uint16_t code = (high << 8) | low;
          output16.push_back(code);
          i += 2; // Skip 2 bytes
        }
      }
    }
    
    std::cout << "Decoded from 8-bit format: " << output16.size() << " codes" << std::endl;
  }

  // Internal 16-bit decompression processing function
  void LZWProcessor::decompressInternal(const std::vector<uint16_t>& input, std::vector<uint8_t>& output) {
    // Initialization
    int dic_size = 4096; // Dictionary size
    int charactors = 256; // Number of characters
    int now_parent = -1;
    int now_dic_size;
    int first_char;

    // Initialize dictionary
    std::vector<Word> dictionary(dic_size);
    now_dic_size = initDictionary(dictionary.data(), dic_size, charactors);

    // Decompression process
    for (size_t i = 0; i < input.size(); ++i) {
        int code = input[i]; // Directly use 16-bit code

        if (code == now_dic_size) { // For special code
            if (now_dic_size < dic_size) {
                now_dic_size = addWordToDictionary(dictionary.data(), now_parent, first_char, now_dic_size);
            } else {
                now_dic_size = initDictionary(dictionary.data(), dic_size, charactors);
            }
            now_parent = -1;
        }

        first_char = outputOriginalData(output, code, dictionary.data(), now_dic_size);
        if (now_parent >= 0) {
            if (now_dic_size < dic_size) {
                now_dic_size = addWordToDictionary(dictionary.data(), now_parent, first_char, now_dic_size);
            } else {
                now_dic_size = initDictionary(dictionary.data(), dic_size, charactors);
            }
        }
        now_parent = code;
    }
    
    std::cout << "Internal 16-bit decompression completed: " << output.size() << " bytes" << std::endl;
  }

  void LZWProcessor::finalize(const std::vector<uint8_t>& output) {
    // Implement post-processing if necessary
  }
}
