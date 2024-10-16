#include <cmath>
#include <encoder.hpp>
#include <types/growing_point.hpp>
#include <output_writer.hpp>

namespace encoder {
  void Encoder::encode(const std::vector<std::vector<Pixel>>& image)
  {
      // TODO: use new Image struct instead of std::vector<std::vector<uint8_t>>
    const size_t width = image.size();
    const size_t height = image.size();
    std::vector<std::vector<uint8_t>> red(std::vector<std::vector<uint8_t>>(
      width, std::vector<uint8_t>(height, 0)));
    std::vector<std::vector<uint8_t>> green(std::vector<std::vector<uint8_t>>(
      width, std::vector<uint8_t>(height, 0)));
    std::vector<std::vector<uint8_t>> blue(std::vector<std::vector<uint8_t>>(
      width, std::vector<uint8_t>(height, 0)));

    for(size_t i = 0; i < width; i++) {
      for(size_t j = 0; j < height; j++) {
        red[i][j] = image[i][j].red;
        green[i][j] = image[i][j].green;
        blue[i][j] = image[i][j].blue;
      }
    }

    adaptive_vector_quantization(red);
    adaptive_vector_quantization(green);
    adaptive_vector_quantization(blue);
  }

  void Encoder::adaptive_vector_quantization(
    std::vector<std::vector<uint8_t>>& image)
  {
    output_writer::OutWriter out_writer(*out_file);

    // Add pixel [0, 0] to growing points pool
    auto* init_gp = new Growing_point(0, 0);
    std::vector<Growing_point*> gp_pool{};
    gp_pool.push_back(init_gp);

    size_t gpp_size = 1;

    // Init dictionary
    Dictionary* dict = dict_init_heur();

    // While growing points pool has more elements
    while(gpp_size > 0) {
      // Determine next growing point
      Growing_point* gp = growing_heur(gp_pool);
      gpp_size--; // Current gp removed from growing points pool

      size_t common_block_idx;
      Block* picked_block;
      find_common_block(dict, image, gp, &common_block_idx, &picked_block);

      auto bits_to_transmit =
        static_cast<int8_t>(std::floor(log2(static_cast<double>(dict->size))));

      // Write common_block_idx to file on log2(dict.size) bits
      for(int8_t i = bits_to_transmit - 1; i >= 0; i--) {
        bool bit = (common_block_idx >> i) & 1;
        out_writer.write(bit);
      }

      // Update dict
      dict_update_heur(dict, picked_block, gp, image);
      delete picked_block;
      // Check if dictionary is full and if so use deletion heuristic
      deletion_heur(dict);
      // Update growing points pool
      growing_point_update_heur(gp_pool, &gpp_size, gp);

      delete gp;
    }

    delete dict;
  }

  // TODO: fix this function, make it choose the largest block and correctly use tolerance
  void Encoder::find_common_block(Dictionary* dict,
                                  std::vector<std::vector<uint8_t>>& image,
                                  Growing_point* current_gp,
                                  size_t* common_block_idx,
                                  Block** picked_block)
  {
    double best_match = 999.0;
    size_t best_i = 0;
    auto* gp_block = new Block(0, 0, std::vector<std::vector<uint8_t>>());

    for(size_t i = dict->size - 1; i > 255; i--) {
      Block* dict_entry = dict->entries[i];
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
            pixels[x][y] = image[current_gp->x + x][current_gp->y + y];
          }
        }
        gp_block = new Block(dict_entry->width, dict_entry->height, pixels);
      }

      const double match = match_heur(gp_block, dict->entries[i]);
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
} // namespace encoder
