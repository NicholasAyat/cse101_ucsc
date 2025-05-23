# Program 3: Byte Pair Encoding (BPE) Tokenizer

## Overview
This program implements a character-level Byte Pair Encoding (BPE) tokenizer, which is a subword tokenization method commonly used in modern language models. The implementation includes training BPE merges from a text corpus, building a subword vocabulary, and tokenizing new input sentences using a greedy longest-match strategy.

## Features
- Character-level tokenization with end-of-word markers
- BPE training with frequency-based pair merging
- Vocabulary building with unique token IDs
- Greedy longest-match tokenization
- Unknown character handling

## Files
- `bpe.c` - Main implementation file
- `Dictionary.c/h` - Dictionary implementation
- `HashTable.c/h` - Hash table implementation
- `List.c/h` - List implementation
- `makefile` - Build configuration
- `corpus.txt` - Example training corpus
- `test.in` - Example test input
- `test.out` - Expected output

## Building
```bash
make
```

## Usage
```bash
./prog3 corpus.txt < test.in
```

## Output Format
1. BPE Training Process:
   ```
   Iteration 1: merging 'l' + 'o'
   Iteration 2: merging 'lo' + 'w'
   ...
   ```

2. Vocabulary:
   ```
   token1: id1
   token2: id2
   ...
   ```

3. Tokenization:
   ```
   Word 'word': [token1 -> id1] [token2 -> id2] ...
   ```

## Example
Input:
```
low lower newest widest xxlow
```

Output:
```
Word 'low': [low</w> -> 0] 
Word 'lower': [lower</w> -> 1] 
Word 'newest': [ne -> 2] [w -> 3] [est</w> -> 4] 
Word 'widest': [w -> 3] [i -> 5] [d -> 6] [est</w> -> 4] 
Word 'xxlow': [UNK(x)] [UNK(x)] [low</w> -> 0] 
``` 