[![tests](https://github.com/sebawoz02/AVQ_Compression/actions/workflows/tests.yaml/badge.svg?branch=main)](https://github.com/sebawoz02/AVQ_Compression/actions/workflows/tests.yaml)
[![g++](https://github.com/sebawoz02/AVQ_Compression/actions/workflows/g++.yaml/badge.svg?branch=main)](https://github.com/sebawoz02/AVQ_Compression/actions/workflows/g++.yaml)
[![clang](https://github.com/sebawoz02/AVQ_Compression/actions/workflows/clang.yaml/badge.svg?branch=main)](https://github.com/sebawoz02/AVQ_Compression/actions/workflows/clang.yaml)

# Adaptive Vector Quantization (AVQ) Compression

This project implements a compression and decompression system using **Adaptive Vector Quantization (AVQ)**. It includes tools for encoding and decoding images, leveraging adaptive heuristics to optimize compression while maintaining high-quality reconstruction. The project is written in modern C++ (C++20) and includes utilities for testing and analysis.

## Features

- **Encoding and Decoding**:
    - Efficient image compression using AVQ.
    - Multiple heuristics for optimizing encoding and dictionary updates.
- **Heuristics**:
    - Matching Heuristics: Maximum Square Error, Mean Square Error (MSE), Euclidean distance.
    - Growing Heuristics: Wave, Diagonal, LIFO.
    - Dictionary Update and Deletion Strategies.
- **Testing and Analysis**:
    - Comprehensive test suite for core components.
    - Statistical analysis tools for evaluating compression quality (e.g., PSNR, SSIM).

---

## Installation

### Prerequisites
- C++ compiler supporting **C++20** (e.g., GCC 10+).
- **CMake** version 3.22 or higher.
- Optional: Image processing library for input/output (not specified in provided files).

### Build Instructions
1. Clone the repository:
   ```bash
   git clone https://github.com/sebawoz02/AVQ_compression.git
   cd AVQ_compression
2. Create a build directory and configure the project with CMake: 
   ```bash
   mkdir build
   cd build
   cmake ..
3. Build the project:
   ```bash
   make

This will produce the following executables:
- avq: Main compression/decompression tool.
- analyzer: Tool for analyzing compression quality and generating statistics.
- test: Test suite for verifying the implementation.

--- 

## Usage

### Command-Line Arguments

The avq tool accepts the following arguments:
```bash
Usage: ./avq <input_file> <output_file> [-e/-d] [<options>]
Options:
  --help          Display this help message.
  -e              Encode mode.
  -d              Decode mode.
  -t <value>      Match tolerance (default: 0.0).
  -mh <value>     Match heuristic:
                  0 - Maximum Square Error (default),
                  1 - Mean Square Error,
                  2 - Euclidean distance.
  -gh <value>     Growing heuristic:
                  0 - Wave (default),
                  1 - Diagonal,
                  2 - LIFO.
  -duh <value>    Dictionary update heuristic:
                  0 - One Column One Row (default),
                  1 - One Row,
                  2 - One Column.
  -dh <value>     Dictionary deletion heuristic:
                  0 - FIFO (default),
                  1 - LRU.
```
### Examples

Encoding an image:
```bash
./avq input.tga compressed.avq -e -t 0.1 -mh 1 -gh 2
```
Decoding an image:
```bash
./avq compressed.avq output.tga -d
```

### Directory structure:
```plain
.
├── app/
│   ├── avq/                # Main application source
│   ├── analyzer/           # Analysis tool source
├── inc/                    # Header files
├── src/                    # Core implementation files
│   ├── types/              # Core types (Dictionary, Image, etc.)
│   ├── heuristics.cpp      # Heuristics implementations
│   ├── encoder.cpp         # Encoding logic
│   ├── decoder.cpp         # Decoding logic
│   ├── io_handler.cpp      # Input/Output handling
│   ├── arg_parser.cpp      # Argument parsing
├── test/                   # Unit tests and avq tests results
├── CMakeLists.txt          # Build configuration
├── README.md               # Project description
```