# About LZW

LZW (Lempel-Ziv-Welch) is a lossless data compression algorithm 
that is widely used in various file formats, such as GIF and TIFF. 
It works by replacing repeated sequences of data with shorter codes, 
effectively reducing the size of the data without losing any information.

## Key Features

- **Lossless Compression**: LZW compresses data without losing any information, making it suitable for text and image files.
- **Dictionary-Based**: It builds a dictionary of sequences found in the data, allowing for efficient encoding and decoding.
- **Widely Supported**: Many software applications and libraries support LZW compression, making it easy to implement.

## Procedure of Compression and Decompression in LZW

### Compression Procedure

1. **Initialize the Dictionary**:

   - Start with a dictionary containing all possible single-character sequences (e.g., ASCII characters).

2. **Read Input Data**:
   - Read the input data one character at a time.

3. **Find the Longest Match**:
   - Search the dictionary for the longest sequence of characters that matches the current input.

4. **Output the Code**:
   - Output the dictionary code corresponding to the matched sequence.

5. **Update the Dictionary**:
   - Add the new sequence (matched sequence + next character) to the dictionary.

6. **Repeat**:
   - Continue reading the input data and repeat the process until the entire input is processed.

---

### Decompression Procedure

1. **Initialize the Dictionary**:

   - Start with a dictionary containing all possible single-character sequences (same as in compression).

2. **Read Compressed Data**:
   - Read the compressed data one code at a time.

3. **Decode the Code**:
   - Use the dictionary to decode the code into the corresponding sequence of characters.

4. **Update the Dictionary**:
   - Add the new sequence (previous sequence + first character of the current sequence) to the dictionary.

5. **Output the Sequence**:
   - Output the decoded sequence.

6. **Repeat**:
   - Continue reading the compressed data and repeat the process until the entire compressed data is processed.

---

### Summary

- **Compression**: Reads input data, finds matches in the dictionary, outputs codes, and updates the dictionary.
- **Decompression**: Reads codes, decodes them using the dictionary, outputs sequences, and updates the dictionary.

Both processes rely on the same dictionary-building mechanism, ensuring that the compressed data can be accurately decompressed back to its original form.