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

    std::vector<int> buffer; // 展開されたデータを一時的に格納するバッファ
    int first_char = -1;

    // 辞書をたどって展開
    int current_node = code;
    while (current_node >= 0) {
        const Word& entry = dictionary[current_node];
        buffer.push_back(entry.charactor);
        current_node = entry.parent_code;

        // 最初の文字を記録
        if (first_char == -1) {
            first_char = entry.charactor;
        }
    }

    // 展開されたデータを逆順で `output` に書き込む
    for (auto it = buffer.rbegin(); it != buffer.rend(); ++it) {
        output.push_back(static_cast<uint8_t>(*it));
    }

    return first_char; // 展開されたデータの最初の文字を返す
  }

  void LZWProcessor::prepare(const std::vector<uint8_t>& input) {
    // Implement preparation logic if needed
  }

  void LZWProcessor::compress(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
    if (input.empty()) {
      throw std::invalid_argument("Input data is empty.");
    }

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
        // If not found in dictionary, update dictionary and output code
        output.push_back(static_cast<uint8_t>(now_parent));
        if (now_dic_size < dic_size) {
          now_dic_size = addWordToDictionary(dictionary.data(), now_parent, c, now_dic_size);
        } else {
          // If dictionary is full, clear dictionary
          now_dic_size = initDictionary(dictionary.data(), dic_size, characters);
        }
        now_parent = -1;
        --i; // Re-process current character
      } else {
        now_parent = code;
      }
    }

    // Output the last code
    if (now_parent >= 0) {
      output.push_back(static_cast<uint8_t>(now_parent));
    }
  }

  void LZWProcessor::decompress(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
    if (input.empty()) {
        throw std::invalid_argument("Input data is empty.");
    }

    // 初期化
    int dic_size = 4096; // 辞書サイズ
    int charactors = 256; // 文字の種類
    int now_parent = -1;
    int now_dic_size;
    int first_char;

    // 辞書の初期化
    std::vector<Word> dictionary(dic_size);
    now_dic_size = initDictionary(dictionary.data(), dic_size, charactors);

    // デコード処理
    for (size_t i = 0; i < input.size(); ++i) {
        int code = input[i];

        if (code == now_dic_size) { // 特殊コードの場合
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
  }

  void LZWProcessor::finalize(const std::vector<uint8_t>& output) {
    // Implement post-processing if necessary
  }
}
