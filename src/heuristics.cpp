#include <cmath>
#include <heuristics.hpp>

#define DICT_SIZE_LIMIT 512

namespace heuristic {

  static double mse(Block* b1, Block* b2);

  static double mse(Block* b1, Block* b2)
  {
    (void)b1;
    (void)b2;
    return 0.0;
  }

  // MATCH
  // TODO: fix this function, make it choose the largest block and correctly use tolerance
  void match::top_left_mse(Dictionary* dict, double tolerance, Image& image,
                           Growing_point* current_gp, size_t* common_block_idx,
                           Block** picked_block)
  {
    double best_match = 999.0;
    size_t best_i = 0;
    auto* gp_block = new Block(0, 0, std::vector<std::vector<uint8_t>>());

    for(size_t i = dict->size() - 1; i > 255; i--) {
      Block* dict_entry = (*dict)[i];
      if(gp_block->width != dict_entry->width ||
         gp_block->height != dict_entry->height) {
        // TODO: This part might not be that easy check in the future.
        //       Need to check if gp_block is not on top of the already sent part of image.
        delete gp_block;
        std::vector<std::vector<uint8_t>> pixels(
          std::vector<std::vector<uint8_t>>(
            dict_entry->width, std::vector<uint8_t>(dict_entry->height, 0)));
        for(size_t x = 0; x < dict_entry->width; x++) {
          for(size_t y = 0; y < dict_entry->height; y++) {
            pixels[x][y] = image.pixels[current_gp->x + x][current_gp->y + y];
          }
        }
        gp_block = new Block(dict_entry->width, dict_entry->height, pixels);
      }

      const double match = mse(gp_block, (*dict)[i]);
      if(match < tolerance) // Is this the right way to do this??
      {
        *common_block_idx = i;
        *picked_block = gp_block;
        return;
      }
      if(best_match > match) {
        best_match = match;
        best_i = i;
      }
    }
    *common_block_idx = best_i;
    *picked_block = gp_block;
  }

  // DICT INIT
  Dictionary* dict_init::range_0_to_255()
  {
    auto* dict = new Dictionary();
    for(uint16_t color = 0; color < 256; color++) {
      std::vector<std::vector<uint8_t>> pixel(
        std::vector<std::vector<uint8_t>>(1, std::vector<uint8_t>(1, 0)));
      pixel[0][0] = static_cast<uint8_t>(color);
      auto* block = new Block(1, 1, pixel);
      dict->insert(block);
    }

    return dict;
  }

  // GP UPDATE
  void gp_update::first_from_left(std::vector<Growing_point*>& growing_points,
                                  const size_t* size, Growing_point* cur_gp)
  {
    (void)growing_points;
    (void)size;
    (void)cur_gp;
  }

  // GROWING
  Growing_point* growing::wave(std::vector<Growing_point*>& gp_pool)
  {
    Growing_point* best = gp_pool[0];
    for(size_t i = 1; i < gp_pool.size(); i++) {
      Growing_point* cur = gp_pool[i];
      if(best->x + best->y > cur->x + cur->y) {
        best = cur;
      }
    }

    return best;
  }

  Growing_point* growing::diagonal(std::vector<Growing_point*>& gp_pool)
  {
    Growing_point* best = gp_pool[0];
    for(size_t i = 1; i < gp_pool.size(); i++) {
      Growing_point* cur = gp_pool[i];
      if(abs((int)best->x - (int)best->y) > abs((int)cur->x - (int)cur->y)) {
        best = cur;
      }
    }

    return best;
  }

  Growing_point* growing::lifo(std::vector<Growing_point*>& gp_pool)
  {
    return gp_pool[gp_pool.size() - 1];
  }

  // DICT UPDATE
  void dict_update::one_column(Dictionary* dict, Block* picked_block,
                               Growing_point* gp, Image& image)
  {
    if(gp->y == 0) {
      auto* b = new Block(picked_block->width, picked_block->height,
                          picked_block->pixels);
      dict->insert(b);
      return;
    }
    size_t w = picked_block->width;
    size_t h = picked_block->height + 1;

    std::vector<std::vector<uint8_t>> pixels(
      std::vector<std::vector<uint8_t>>(w, std::vector<uint8_t>(h, 0)));
    for(size_t x = 0; x < w; x++) {
      pixels[x][0] = image.pixels[gp->x + x][gp->y - 1];
      for(size_t y = 1; y < h; y++) {
        pixels[x][y] = picked_block->pixels[x][y - 1];
      }
    }
    auto* b = new Block(w, h, pixels);
    dict->insert(b);
  }

  void dict_update::one_row(Dictionary* dict, Block* picked_block,
                            Growing_point* gp, Image& image)
  {
    if(gp->x == 0) {
      auto* b = new Block(picked_block->width, picked_block->height,
                          picked_block->pixels);
      dict->insert(b);
      return;
    }
    size_t w = picked_block->width + 1;
    size_t h = picked_block->height;

    std::vector<std::vector<uint8_t>> pixels(
      std::vector<std::vector<uint8_t>>(w, std::vector<uint8_t>(h, 0)));
    for(size_t y = 0; y < w; y++) {
      pixels[0][y] = image.pixels[gp->x - 1][gp->y + y];
      for(size_t x = 1; x < h; x++) {
        pixels[x][y] = picked_block->pixels[x - 1][y];
      }
    }
    auto* b = new Block(w, h, pixels);
    dict->insert(b);
  }

  void dict_update::one_column_one_row(Dictionary* dict, Block* picked_block,
                                       Growing_point* gp, Image& image)
  {
    one_column(dict, picked_block, gp, image);
    one_row(dict, picked_block, gp, image);
  }

  // DICT DELETION
  void dict_deletion::fifo(Dictionary* dict)
  {
    while(dict->size() > DICT_SIZE_LIMIT) {
      dict->remove((*dict)[256]); // DON'T TOUCH 1x1 blocks
    }
  }
} // namespace heuristic
