#pragma once

#include <types/args.hpp>

namespace arg_parser {
  /**
   * @brief Parses command-line arguments and configures program parameters.
   *
   * @param argc Number of command-line arguments.
   * @param argv Array of strings representing the command-line arguments.
   * @return Args A structure containing the configured program parameters.
   *
   * @details
   * This function parses the command-line arguments and sets the appropriate values
   * in the `Args` structure. If invalid arguments are provided or required options are missing,
   * the function displays help or error messages. The `exit` field in the returned `Args`
   * structure is set to `true` in such cases.
   *
   * Supported arguments:
   * - `--help` or `-h`: Displays usage instructions.
   * - `-e`: Sets the encoding mode.
   * - `-d`: Sets the decoding mode.
   * - `-t <value>`: Sets the compression tolerance (geq 0.0, default: 0.0).
   * - `-mh <value>`: Selects the match heuristic:
   *     - `0`: Maximum Square Error (default),
   *     - `1`: Mean Square Error (MSE),
   *     - `2`: Euclidean distance.
   * - `-gh <value>`: Selects the growing heuristic:
   *     - `0`: Wave (default),
   *     - `1`: Diagonal,
   *     - `2`: LIFO.
   * - `-duh <value>`: Selects the dictionary update heuristic:
   *     - `0`: One Column One Row (default),
   *     - `1`: One Row,
   *     - `2`: One Column.
   * - `-dh <value>`: Selects the dictionary deletion heuristic:
   *     - `0`: FIFO (default),
   *     - `1`: LRU.
   *
   * @note If no mode (`-e` or `-d`) is specified or if the arguments are invalid,
   *       the program will display appropriate error messages and terminate.
   */
  Args parse(size_t argc, char** argv);
} // namespace arg_parser