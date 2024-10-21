#pragma once

#include <types/dictionary.hpp>
#include <types/growing_point.hpp>
#include <types/image.hpp>

namespace heuristic {
  using mh_t = void (*)(Dictionary*, double, Image&, Growing_point*, size_t*,
                        Block**);

  using idh_t = Dictionary* (*)();

  using gpuh_t = void (*)(std::vector<Growing_point*>&, size_t*,
                          Growing_point*);

  using gh_t = Growing_point* (*)(std::vector<Growing_point*>&);

  using duh_t = void (*)(Dictionary*, Block*, Growing_point*, Image&);

  using dh_t = void (*)(Dictionary*);

  namespace match {
    void top_left_mse(Dictionary* dict, double tolerance, Image& image,
                      Growing_point* current_gp, size_t* common_block_idx,
                      Block** picked_block);
  }

  namespace dict_init {
    Dictionary* range_0_to_255();
  }

  namespace gp_update {
    void first_from_left(std::vector<Growing_point*>& growing_points,
                         const size_t* size, Growing_point* cur_gp);
  }

  namespace growing {
    Growing_point* wave(std::vector<Growing_point*>& gp_pool);
    Growing_point* diagonal(std::vector<Growing_point*>& gp_pool);
    Growing_point* lifo(std::vector<Growing_point*>& gp_pool);
    // circular coverage
  } // namespace growing

  namespace dict_update {
    void one_column(Dictionary* dict, Block* picked_block, Growing_point* gp,
                    Image& image);
    void one_row(Dictionary* dict, Block* picked_block, Growing_point* gp,
                 Image& image);
    void one_column_one_row(Dictionary* dict, Block* picked_block,
                            Growing_point* gp, Image& image);
  } // namespace dict_update

  namespace dict_deletion {
    void fifo(Dictionary* dict);
  }
} // namespace heuristic