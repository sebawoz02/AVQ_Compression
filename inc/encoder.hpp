#pragma once

#include <fstream>
#include <heuristics.hpp>
#include <io_handler.hpp>
#include <types/pixel.hpp>

/**
 * @struct Encoder
 * @brief A encoder structure that stores all the necessary data and
 *        displays functions needed in the compression process.
 */
typedef struct Encoder {
  Encoder(IO_Handler* _io_handler, heuristic::mh_t mh, heuristic::idh_t idh,
          heuristic::gpuh_t gpuh, heuristic::gh_t gh, heuristic::duh_t duh,
          Deletion_Mode dh, double t)
    : io_handler(_io_handler), match_heur(mh), dict_init_heur(idh),
      growing_point_update_heur(gpuh), growing_heur(gh), dict_update_heur(duh),
      deletion_heur(dh), tolerance(t){};

  /**
  * @brief Encodes an image using the Adaptive Vector Quantization (AVQ) algorithm.
  *
  * @param image The input image to be encoded, represented as an `Image` object.
  *
  * @details
  * This function performs the encoding process on the provided image. The Adaptive Vector
  * Quantization (AVQ) algorithm compresses the image data by dividing it into smaller blocks
  * and mapping them to representative vectors in a dictionary. The resulting compressed
  * representation reduces the storage requirements while preserving visual quality based on
  * the chosen compression parameters.
  *
  */
  void encode(Image image);

private:
  IO_Handler* io_handler;                       ///> Input/Output handler
  heuristic::mh_t match_heur;                   ///> Match heuristic
  heuristic::idh_t dict_init_heur;              ///> Dictionary Init heuristic
  heuristic::gpuh_t growing_point_update_heur;  ///> Growing Point Pool Update heuristic
  heuristic::gh_t growing_heur;                 ///> Growing heuristic
  heuristic::duh_t dict_update_heur;            ///> Dictionary Update heuristic
  Deletion_Mode deletion_heur;                  ///> Dictionary entry deletion heuristic

  double tolerance;                             ///> Information loss toleration
} Encoder;
