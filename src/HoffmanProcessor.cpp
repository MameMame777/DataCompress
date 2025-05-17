#include "../include/HoffmanProcessor.h"
#include "iostream"
namespace HOFFMANProcessing {
  HOFFMANProcessor::HOFFMANProcessor(const DataProvider& provider, TraversalMode mode)
    : traversalMode(mode),
      width(provider.getHeaderData()[18] | (provider.getHeaderData()[19] << 8)),
      height(provider.getHeaderData()[22] | (provider.getHeaderData()[23] << 8)),
      imageData(provider.getImageData()) {}

  HOFFMANProcessor::~HOFFMANProcessor() {}

  void makeHuffmanTree(int* histgram, int n, HuffmanTree& tree) {
    int treesize = n;
    int d1, d2;

    // Initialize tree
    for (int i = 0; i < 2 * n; i++) {
      tree.left_node[i] = 0;
      tree.right_node[i] = 0;
      tree.parent[i] = 0;
    }

    while (true) {
      // Find two minimum frequencies
      d1 = -1;
      d2 = -1;
      int min1 = NODATA, min2 = NODATA;

      for (int i = 0; i < treesize; i++) {
        if (histgram[i] < min1) {
          min2 = min1;
          d2 = d1;
          min1 = histgram[i];
          d1 = i;
        } else if (histgram[i] < min2) {
          min2 = histgram[i];
          d2 = i;
        }
      }


    // Check if valid nodes were found
    if (d1 == -1 || d2 == -1) {
        break;
    }

    // Check if d1 and d2 are within bounds
    if (d1 < 0 || d1 >= 2 * n || d2 < 0 || d2 >= 2 * n) {
        throw std::logic_error("d1 or d2 is out of bounds.");
    }

    // Check if d1 and d2 are not the same
    if (d1 == d2) {
        throw std::logic_error("d1 and d2 must not be the same.");
    }

      // Create new node
      tree.left_node[treesize] = d1;
      tree.right_node[treesize] = d2;
      tree.parent[d1] = treesize;
      tree.parent[d2] = -treesize;

      histgram[treesize] = histgram[d1] + histgram[d2];
      histgram[d1] = NODATA;
      histgram[d2] = NODATA;

      treesize++;
    }

    tree.treesize = treesize;
  }

  void HOFFMANProcessor::prepare(const std::vector<uint8_t>& input) {
    // Implement preparation logic if needed
  }

  void HOFFMANProcessor::compress(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
    if (input.empty()) {
      throw std::invalid_argument("Input data is empty.");
    }
    if (traversalMode == TraversalMode::RowWise) {
      compressRowWise(input, output);
    } else if (traversalMode == TraversalMode::ColumnWise) {
      compressColumnWise(input, output);
    } else {
      throw std::logic_error("Unsupported traversal mode.");
    }
  }

  void HOFFMANProcessor::decompress(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
    if (traversalMode == TraversalMode::RowWise) {
      decompressRowWise(input, output);
    } else if (traversalMode == TraversalMode::ColumnWise) {
      decompressColumnWise(input, output);
    } else {
      throw std::logic_error("Unsupported traversal mode.");
    }
  }

  void HOFFMANProcessor::finalize(const std::vector<uint8_t>& output) {
    // Implement post-processing if necessary
  }

  void HOFFMANProcessor::compressRowWise(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
    // Step 1: Initialize variables
    HuffmanTree tree;
    int datasize = input.size();
    std::vector<int> histgram(datasize, 0); // サイズ datasize の配列を 0 で初期化
    // Step 2: Calculate histogram (frequency of each value)
    for (int i = 0; i < datasize; i++) {
      histgram[input[i]]++;
    }

    // Step 3: Build Huffman tree
    makeHuffmanTree(histgram.data(), 256, tree);

    // Step 4: Initialize bit-level output
    int bits = 0;
    int bdata = 0;

    auto fputBit = [&](int bit) {
      bdata = (bdata << 1) | bit;
      bits++;
      if (bits >= 8) {
        output.push_back(static_cast<uint8_t>(bdata));
        bits = 0;
        bdata = 0;
      }
    };

    auto flushBit = [&]() {
      while (bits > 0 && bits < 8) {
        fputBit(0);
      }
    };

    // Step 5: Encode data using Huffman tree
    auto sgni = [](int x) -> int {
      return (x < 0) ? -1 : (x > 0 ? 1 : 0);
    };
    auto absi = [](int x) -> int {
      return (x < 0) ? -x : x;
    };

    for (int i = 0; i < datasize; i++) {
      int value = input[i];
      int code[100]; // Temporary storage for the Huffman code
      int c = 0;
      int nowNode = value;

      // Traverse the Huffman tree to get the code
      while (true) {
        int selectedBranch = sgni(tree.parent[nowNode]);
        int nextNode = absi(tree.parent[nowNode]);
        if (selectedBranch < 0) {
          code[c++] = 1;
        } else {
          code[c++] = 0;
        }
        if (nextNode == tree.treesize - 1) break; // Reached the root
        nowNode = nextNode;
      }

      // Output the code in reverse order
      for (int j = c - 1; j >= 0; j--) {
        fputBit(code[j]);
      }
    }

    // Step 6: Flush remaining bits
    flushBit();
  }

  void HOFFMANProcessor::compressColumnWise(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
    // Implement column-wise compression logic
  }

  void HOFFMANProcessor::decompressRowWise(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
    // Step 1: Initialize variables
    HuffmanTree tree;
    int datasize = width * height; // Assuming width and height are class members
    std::vector<int> histgram(datasize, 0); // サイズ datasize の配列を 0 で初期化

    // Step 2: Read histogram from input (Hoffman decoding)
    int inputIndex = 0;
    auto fgetBit = [&]() -> int {
      static int bits = 0;
      static int bdata = 0;

      if (bits == 0) {
        if (inputIndex >= input.size()) {
          throw std::runtime_error("Unexpected end of input data");
        }
        bdata = input[inputIndex++];
        bits = 8;
      }

      int val = (bdata >> 7) & 1;
      bdata = (bdata << 1) & 0xFF;
      bits--;
      return val;
    };

    auto readHoffmanCode = [&]() -> int {
      int headsize = 0;
      while (fgetBit() == 1) {
        headsize++;
      }
      headsize += 2;

      int answer = 0;
      for (int i = 0; i < headsize; i++) {
        int bit = fgetBit() << i;
        answer |= bit;
      }
      return answer + 1;
    };

    for (int i = 0; i < 256; i++) {
      histgram[i] = readHoffmanCode() - 1;
    }

    // Step 3: Reconstruct Huffman tree
    makeHuffmanTree(histgram.data(), 256, tree);

    // Step 4: Decode data using Huffman tree
    auto getValue = [&]() -> int {
      int nowNode = tree.treesize - 1; // Start at the root of the tree
      while (nowNode >= 256) { // While not a leaf node
        int bit = fgetBit();
        if (bit == 0) {
          nowNode = tree.left_node[nowNode];
        } else {
          nowNode = tree.right_node[nowNode];
        }
      }
      return nowNode; // Leaf node reached, return the value
    };

    for (int i = 0; i < datasize; i++) {
      int value = getValue();
      output.push_back(static_cast<uint8_t>(value));
    }
  }

  void HOFFMANProcessor::decompressColumnWise(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
    // Implement column-wise decompression logic
  }
}