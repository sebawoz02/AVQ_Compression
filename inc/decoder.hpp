#pragma once

#include <heuristics.hpp>
#include <io_handler.hpp>

typedef struct Decoder {
  Decoder(IO_Handler* _io_handler, heuristic::idh_t idh, heuristic::gpuh_t gpuh,
          heuristic::gh_t gh, heuristic::duh_t duh, heuristic::dh_t dh)
    : io_handler(_io_handler), dict_init_heur(idh),
      growing_point_update_heur(gpuh), growing_heur(gh), dict_update_heur(duh),
      deletion_heur(dh){};

  std::vector<std::vector<Pixel>> decode(size_t width, size_t height);

private:
  IO_Handler* io_handler;
  heuristic::idh_t dict_init_heur;
  heuristic::gpuh_t growing_point_update_heur;
  heuristic::gh_t growing_heur;
  heuristic::duh_t dict_update_heur;
  heuristic::dh_t deletion_heur;

  Image adaptive_vector_quantization(size_t width, size_t height);
} Decoder;
