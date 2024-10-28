#pragma once

#include <fstream>
#include <heuristics.hpp>
#include <io_handler.hpp>
#include <types/pixel.hpp>

typedef struct Encoder {
  Encoder(IO_Handler* _io_handler, heuristic::mh_t mh, heuristic::idh_t idh,
          heuristic::gpuh_t gpuh, heuristic::gh_t gh, heuristic::duh_t duh,
          heuristic::dh_t dh, double t)
    : io_handler(_io_handler), match_heur(mh), dict_init_heur(idh),
      growing_point_update_heur(gpuh), growing_heur(gh), dict_update_heur(duh),
      deletion_heur(dh), tolerance(t){};

  void encode(const std::vector<std::vector<Pixel>>& image, size_t width,
              size_t height);

private:
  IO_Handler* io_handler;
  heuristic::mh_t match_heur;
  heuristic::idh_t dict_init_heur;
  heuristic::gpuh_t growing_point_update_heur;
  heuristic::gh_t growing_heur;
  heuristic::duh_t dict_update_heur;
  heuristic::dh_t deletion_heur;

  double tolerance;

  void adaptive_vector_quantization(Image image);
} Encoder;
