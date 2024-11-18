#pragma once

#include <types/dictionary.hpp>
#include <types/gp_pool.hpp>
#include <types/growing_point.hpp>
#include <types/image.hpp>

namespace heuristic {

  ///> Match heuristic function type
  using mh_t = void (*)(Dictionary*, double, Image&, Growing_point*, size_t*,
                        Block**);

  ///> Dictionary Init heuristic function type
  using idh_t = Dictionary* (*)();

  ///> Growing Point Pool Update heuristic function type
  using gpuh_t = void (*)(Image&, GP_pool*, Growing_point*);

  ///> Growing heuristic function type
  using gh_t = Growing_point* (*)(GP_pool*);

  ///> Dictionary Update heuristic function type
  using duh_t = void (*)(Dictionary*, Block*, Growing_point*, Image&);

  ///> Dictionary entry deletion heuristic function type
  using dh_t = void (*)(Dictionary*);

  namespace match {
    void mse(Dictionary* dict, double tolerance, Image& image,
             Growing_point* current_gp, size_t* common_block_idx,
             Block** picked_block);
    void max_se(Dictionary* dict, double tolerance, Image& image,
                Growing_point* current_gp, size_t* common_block_idx,
                Block** picked_block);
    void euclidean(Dictionary* dict, double tolerance, Image& image,
                   Growing_point* current_gp, size_t* common_block_idx,
                   Block** picked_block);
  } // namespace match

  namespace dict_init {
    Dictionary* range_0_to_255();
  }

  namespace gpp_update {
    void first_from_left(Image& image, GP_pool* growing_points,
                         Growing_point* cur_gp);
  }

  namespace growing {
    Growing_point* wave(GP_pool* gp_pool);
    Growing_point* diagonal(GP_pool* gp_pool);
    Growing_point* lifo(GP_pool* gp_pool);
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