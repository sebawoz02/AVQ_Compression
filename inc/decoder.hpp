#pragma once

#include <heuristics.hpp>
#include <io_handler.hpp>

/**
 * @struct Decoder
 * @brief A decoder structure that stores all the necessary data and
 *        displays functions needed in the decompression process
 */
typedef struct Decoder {
  Decoder(IO_Handler* _io_handler, heuristic::idh_t idh, heuristic::gpuh_t gpuh,
          heuristic::gh_t gh, heuristic::duh_t duh, Deletion_Mode dh)
    : io_handler(_io_handler), dict_init_heur(idh),
      growing_point_update_heur(gpuh), growing_heur(gh), dict_update_heur(duh),
      deletion_heur(dh){};

  /**
  * @brief Decodes a compressed image into a 2D matrix of pixels.
  *
  * @param width The width of the output image in pixels.
  * @param height The height of the output image in pixels.
  * @return std::vector<std::vector<Pixel>> A 2D vector representing the decoded image,
  *                                         where each element is a Pixel object.
  *
  * @details
  * This function performs the decoding process for an image that has been
  * compressed using the Adaptive Vector Quantization (AVQ) algorithm.
  * It reconstructs the original image based on the compressed data, returning
  * the result as a 2D matrix of Pixel objects. The width and height parameters
  * specify the dimensions of the output image, which must correspond to the
  * original dimensions of the image before compression.
  *
  */
  std::vector<std::vector<Pixel>> decode(size_t width, size_t height);

private:
  IO_Handler* io_handler;                       ///> Input/Output handler
  heuristic::idh_t dict_init_heur;              ///> Dictionary Init heuristic
  heuristic::gpuh_t growing_point_update_heur;  ///> Growing Point Pool Update heuristic
  heuristic::gh_t growing_heur;                 ///> Growing heuristic
  heuristic::duh_t dict_update_heur;            ///> Dictionary Update heuristic
  Deletion_Mode deletion_heur;                  ///> Dictionary entry deletion heuristic

  Image adaptive_vector_quantization(size_t width, size_t height);
} Decoder;
