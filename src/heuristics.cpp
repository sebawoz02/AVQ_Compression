#include <cmath>
#include <heuristics.hpp>

namespace heuristic {

  static double _mse(Block* b1, Block* b2);
  static double _max_se(Block* b1, Block* b2);
  static double _euclidean(Block* b1, Block* b2);
  static void mark_image(Image& image, size_t width, size_t height, size_t x,
                         size_t y);

  static double _mse(Block* b1, Block* b2)
  {
    double square_error = 0.0;
    size_t size = b1->width * b1->height;

    for(size_t i = 0; i < b1->width; ++i) {
      for(size_t j = 0; j < b1->height; ++j) {
        auto diff = static_cast<int16_t>(b1->pixels[i][j] - b2->pixels[i][j]);
        square_error += (diff * diff) / static_cast<double>(size);
      }
    }

    return square_error;
  }

  static double _max_se(Block* b1, Block* b2)
  {
    double max_square_error = 0.0;

    for(size_t i = 0; i < b1->width; ++i) {
      for(size_t j = 0; j < b1->height; ++j) {
        auto diff = static_cast<int16_t>(b1->pixels[i][j] - b2->pixels[i][j]);
        double se = diff * diff;
        if(se > max_square_error) {
          max_square_error = se;
        }
      }
    }

    return max_square_error;
  }

  static double _euclidean(Block* b1, Block* b2)
  {
    double euclidean_distance = 0.0;
    for(size_t i = 0; i < b1->width; ++i) {
      for(size_t j = 0; j < b1->height; ++j) {
        auto diff = static_cast<int16_t>(b1->pixels[i][j] - b2->pixels[i][j]);
        double square = diff * diff;
        euclidean_distance += square;
      }
    }

    return euclidean_distance;
  }

  static void mark_image(Image& image, size_t width, size_t height, size_t x,
                         size_t y)
  {
    const size_t limit_x = width + x;
    const size_t limit_y = height + y;
    for(size_t i = x; i < limit_x; i++) {
      for(size_t j = y; j < limit_y; j++) {
        image.encoded[i][j] = true;
      }
    }
  }

  static void top_left(double(match_func)(Block*, Block*), Dictionary& dict,
                       double tolerance, Image& image,
                       Growing_point* current_gp, size_t* common_block_idx,
                       Block** picked_block)
  {
    auto* gp_block = new Block(0, 0, std::vector<std::vector<uint8_t>>());

    for(size_t i = dict.size() - 1; i > 255; i--) {
      Block* dict_entry = dict[i];
      if(dict_entry->height + current_gp->y > image.height ||
         dict_entry->width + current_gp->x > image.width) {
        continue;
      }

      if(gp_block->width != dict_entry->width ||
         gp_block->height != dict_entry->height) {
        delete gp_block;
        std::vector<std::vector<uint8_t>> pixels(
          dict_entry->width, std::vector<uint8_t>(dict_entry->height, 0));
        for(size_t x = 0; x < dict_entry->width; x++) {
          for(size_t y = 0; y < dict_entry->height; y++) {
            pixels[x][y] = image.pixels[current_gp->x + x][current_gp->y + y];
          }
        }
        gp_block = new Block(dict_entry->width, dict_entry->height, pixels);
      }

      const double match = match_func(gp_block, dict[i]);
      double mean, variance;
      gp_block->mean_and_variance(mean, variance);
      double A = variance / mean;
      double _tolerance = (A <= 0.05)
                            ? (0.4 * tolerance)
                            : ((A <= 0.1) ? 0.6 * tolerance : tolerance);

      if(match <= _tolerance) {
        // The largest block that fits in tolerance
        *common_block_idx = i;
        *picked_block = gp_block;
        if(dict.deletion_mode == LRU) {
            dict.deletion_handler->update(dict.get_entry_at(i));
        }
        mark_image(image, gp_block->width, gp_block->height, current_gp->x,
                   current_gp->y);
        return;
      }
    }
    delete gp_block;
    std::vector<std::vector<uint8_t>> pixel(
      1, std::vector<uint8_t>(1, image.pixels[current_gp->x][current_gp->y]));

    gp_block = new Block(1, 1, pixel);

    *common_block_idx = image.pixels[current_gp->x][current_gp->y];
    *picked_block = gp_block;
    mark_image(image, gp_block->width, gp_block->height, current_gp->x,
               current_gp->y);
  }

  // MATCH
  void match::mse(Dictionary& dict, double tolerance, Image& image,
                  Growing_point* current_gp, size_t* common_block_idx,
                  Block** picked_block)
  {
    top_left(_mse, dict, tolerance, image, current_gp,
             common_block_idx, picked_block);
  }

  void match::max_se(Dictionary& dict, double tolerance, Image& image,
                     Growing_point* current_gp, size_t* common_block_idx,
                     Block** picked_block)
  {
    top_left(_max_se, dict, tolerance, image, current_gp,
             common_block_idx, picked_block);
  }

  void match::euclidean(Dictionary& dict, double tolerance, Image& image,
                        Growing_point* current_gp, size_t* common_block_idx,
                        Block** picked_block)
  {
    top_left(_euclidean, dict, tolerance, image, current_gp,
             common_block_idx, picked_block);
  }

  // DICT INIT
  Dictionary* dict_init::range_0_to_255()
  {
    auto* dict = new Dictionary();
    for(uint16_t color = 0; color < 256; color++) {
      std::vector<std::vector<uint8_t>> pixel(1, std::vector<uint8_t>(1, 0));
      pixel[0][0] = static_cast<uint8_t>(color);
      auto* block = new Block(1, 1, pixel);
      dict->insert(block);
    }

    return dict;
  }

  // GP UPDATE
  void gpp_update::first_from_left(Image& image, GP_pool& growing_points,
                                   GP_pool_entry* cur_gp)
  {
    growing_points.remove(cur_gp);
    growing_points.remove_obsolete(image);

    // LIMIT GPP SIZE TO 48 to speed up this process
    if(growing_points.size() > GPP_SIZE_LIMIT) {
      return;
    }

    size_t y = 0;
    while(y < image.height) {
      if(!image.encoded[0][y] && !growing_points.contains(0, y)) {
        growing_points.add(new Growing_point(0, y));
        return;
      }
      size_t x = 1;
      while(x < image.width && image.encoded[x][y]) {
        x++;
      }
      if(x != image.width && !growing_points.contains(x, y)) {
        growing_points.add(new Growing_point(x, y));
      }
      y++;
    }
  }

  // GROWING
  GP_pool_entry* growing::wave(GP_pool& gp_pool)
  {
    GP_pool_entry* best = gp_pool.first();
    GP_pool_entry* cur = best->next;
    while(cur != nullptr) {
      if(best->gp->x + best->gp->y > cur->gp->x + cur->gp->y) {
        best = cur;
      }
      cur = cur->next;
    }

    return best;
  }

  GP_pool_entry* growing::diagonal(GP_pool& gp_pool)
  {
    GP_pool_entry* best = gp_pool.first();
    GP_pool_entry* cur = best->next;
    while(cur != nullptr) {
      if(abs((int)best->gp->x - (int)best->gp->y) >
         abs((int)cur->gp->x - (int)cur->gp->y)) {
        best = cur;
      }
      cur = cur->next;
    }

    return best;
  }

  GP_pool_entry* growing::lifo(GP_pool& gp_pool)
  {
    return gp_pool.last();
  }

  // DICT UPDATE
  void dict_update::one_row(Dictionary& dict, Block* picked_block,
                            Growing_point* gp, Image& image)
  {
    if(gp->y == 0) {
      if(picked_block->width == 1 && picked_block->height == 1) {
        return;
      }

      auto* b = new Block(picked_block->width, picked_block->height,
                          picked_block->pixels);
      dict.insert(b);
      return;
    }

    size_t w = picked_block->width;
    size_t h = picked_block->height + 1;

    std::vector<std::vector<uint8_t>> pixels(w, std::vector<uint8_t>(h, 0));
    for(size_t x = 0; x < w; x++) {
      pixels[x][0] = image.pixels[gp->x + x][gp->y - 1];
      if(!image.encoded[gp->x + x][gp->y - 1]) {
        // Don't add not encoded part to dict
        return;
      }
      for(size_t y = 1; y < h; y++) {
        pixels[x][y] = picked_block->pixels[x][y - 1];
      }
    }
    auto* b = new Block(w, h, pixels);
    dict.insert(b);
  }

  void dict_update::one_column(Dictionary& dict, Block* picked_block,
                               Growing_point* gp, Image& image)
  {
    if(gp->x == 0) {
      if(picked_block->width == 1 && picked_block->height == 1) {
        return;
      }
      auto* b = new Block(picked_block->width, picked_block->height,
                          picked_block->pixels);
      dict.insert(b);
      return;
    }
    size_t w = picked_block->width + 1;
    size_t h = picked_block->height;

    std::vector<std::vector<uint8_t>> pixels(w, std::vector<uint8_t>(h, 0));
    for(size_t y = 0; y < h; y++) {
      pixels[0][y] = image.pixels[gp->x - 1][gp->y + y];
      if(!image.encoded[gp->x - 1][gp->y + y]) {
        // Don't add not encoded part to dict
        return;
      }
      for(size_t x = 1; x < w; x++) {
        pixels[x][y] = picked_block->pixels[x - 1][y];
      }
    }
    auto* b = new Block(w, h, pixels);
    dict.insert(b);
  }

  void dict_update::one_column_one_row(Dictionary& dict, Block* picked_block,
                                       Growing_point* gp, Image& image)
  {
    one_column(dict, picked_block, gp, image);
    one_row(dict, picked_block, gp, image);
  }

  // DICT DELETION
  void dict_deletion::deletion(Dictionary& dict)
  {
    while(dict.size() >= DICT_SIZE_LIMIT) {
      dict.delete_entry();
    }
  }
} // namespace heuristic
