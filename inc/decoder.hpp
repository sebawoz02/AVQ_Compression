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
