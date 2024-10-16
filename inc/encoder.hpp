#pragma once

#include <cstdint>
#include <fstream>
#include <heuristics.hpp>
#include <types/pixel.hpp>

namespace encoder {
  typedef struct Encoder {
    Encoder(std::ofstream* _out_file, heuristic::mh_t mh, heuristic::idh_t idh,
            heuristic::gpuh_t gpuh, heuristic::gh_t gh, heuristic::duh_t duh,
            heuristic::dh_t dh, double t)
      : out_file(_out_file), match_heur(mh), dict_init_heur(idh),
        growing_point_update_heur(gpuh), growing_heur(gh),
        dict_update_heur(duh), deletion_heur(dh), tolerance(t){};

    void encode(const std::vector<std::vector<Pixel>>& image);

  private:
    std::ofstream* out_file;
    heuristic::mh_t match_heur;
    heuristic::idh_t dict_init_heur;
    heuristic::gpuh_t growing_point_update_heur;
    heuristic::gh_t growing_heur;
    heuristic::duh_t dict_update_heur;
    heuristic::dh_t deletion_heur;

    double tolerance;

    void adaptive_vector_quantization(std::vector<std::vector<uint8_t>>& image);
    void find_common_block(Dictionary* dict,
                           std::vector<std::vector<uint8_t>>& image,
                           Growing_point* current_gp, size_t* common_block_idx,
                           Block** picked_block);
  } Encoder;
} // namespace encoder
