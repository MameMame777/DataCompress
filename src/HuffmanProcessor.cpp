#include "../include/HUFFMANProcessor.h"
#include "iostream"
namespace HUFFMANProcessing {
  HUFFMANProcessor::HUFFMANProcessor(const DataProvider& provider, TraversalMode mode)
    : traversalMode(mode),
      width(provider.getHeaderData()[18] | (provider.getHeaderData()[19] << 8)),
      height(provider.getHeaderData()[22] | (provider.getHeaderData()[23] << 8)),
      imageData(provider.getImageData()) {}

  HUFFMANProcessor::~HUFFMANProcessor() {}
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

  void HUFFMANProcessor::prepare(const std::vector<uint8_t>& input) {
    // Implement preparation logic if needed
  }

  void HUFFMANProcessor::compress(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
    output.clear();
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

  void HUFFMANProcessor::decompress(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
    if (traversalMode == TraversalMode::RowWise) {
      decompressRowWise(input, output);
    } else if (traversalMode == TraversalMode::ColumnWise) {
      decompressColumnWise(input, output);
    } else {
      throw std::logic_error("Unsupported traversal mode.");
    }

  }

  void HUFFMANProcessor::finalize(const std::vector<uint8_t>& output) {
    // Implement post-processing if necessary
  }

  void HUFFMANProcessor::compressRowWise(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
    // Step 1: Initialize variables
    HuffmanTree tree;
    int datasize = input.size();
    int buffer = 0;
    std::vector<int> histgram(N*2, 0);
    std::vector<int> histgram_for_compress(N*2, 0);
    
    // Step 2: Calculate histogram (frequency of each value)
    for (int i = 0; i < datasize; i++) {
        histgram[input[i]]++;
        histgram_for_compress[input[i]]++;
    }
    //saveHistogramToFile(histgram, "histogram_compress.txt");
    // Step 3: Build Huffman tree
    makeHuffmanTree(histgram.data(), N, tree);
    // Step 4: Initialize bit-level output
    int bits = 0;
    int bdata = 0;

    auto fputBit = [&](int bit) {
      static uint8_t bits = 0;
      static uint8_t buffer = 0;
      buffer = (buffer << 1) | bit;
      bits++;
      if (bits == 8) {
          output.push_back(buffer);
          bits = 0;
          buffer = 0;
      }
    };

    auto flushBit = [&]() {
        if (bits > 0) {
            buffer <<= (8 - bits); // 残りのビットを左詰め
            output.push_back(buffer);
            bits = 0;
            buffer = 0;
        }
    };
    auto absi = [](int x) -> int {
      return (x < 0) ? -x : x;
    };
    // Step 5: Output histogram using Wyle encoding
    auto outputWyleCode = [&](int value) {
        int bit;
        int x = absi(value);
        int length = 0;

        // Calculate the header length
        int temp = x / 4;
        while (temp > 0) {
            temp /= 2;
            length++;
        }

        // Output the header
        for (int i = 0; i < length; i++) {
            fputBit(1);
        }
        fputBit(0);

        // Output the value
        length += 2;
        for (int i = 0; i < length; i++) {
            fputBit(x & 1);
            x >>= 1;
        }
    };

    for (int i = 0; i < N; i++) {
        outputWyleCode(histgram_for_compress[i]);
    }

    // Step 6: Encode data using Huffman tree
    auto sgni = [](int x) -> int {
        return (x < 0) ? -1 : (x > 0 ? 1 : 0);
    };

    for (int i = 0; i < datasize; i++) {
        int value = input[i];
        int code[100]={0}; // Temporary storage for the Huffman code
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
    // Step 7: Flush remaining bits
    flushBit();
}

  void HUFFMANProcessor::compressColumnWise(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
    // Implement column-wise compression logic
  }

  void HUFFMANProcessor::decompressRowWise(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
    // Step 1: Initialize variables
    HuffmanTree tree;
    int datasize = width * height; // Assuming width and height are class members
    std::vector<int> histgram(N*2, 0);
    int bitDepth = 8; // Example: 8-bit BMP
    int inputIndex = 0;
    //print input 
    std::cout << "Input data: ";
    std::cout << "input size =  "<<std::dec <<input.size() << std::endl;
      int bits = 0;
      int bdata = 0;    
    auto fgetBit = [&]() -> int {
      if (bits == 0) {
          bdata = input[inputIndex++];
          bits = 8;
      }
      int val = (bdata >> 7) & 1;
      bdata = (bdata << 1) & 0xFF;
      bits--;
      return val;
    };

    // Step 3: Read histogram from input
    auto readHUFFMANCode = [&]() -> int {
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
    return answer;
    };

    
    for (int i = 0; i < N; i++) {
        histgram[i] = readHUFFMANCode() ;    
      }
    // Step 4: Reconstruct Huffman tree
    makeHuffmanTree(histgram.data(), N, tree);
    // Step 5: Decode data using Huffman tree
    auto getValue = [&]() -> int {
        int nowNode = tree.treesize - 1; // Start at the root of the tree
        while (nowNode >= N) { // While not a leaf node
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
  void HUFFMANProcessor::decompressColumnWise(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
    // Implement column-wise decompression logic
  }
}