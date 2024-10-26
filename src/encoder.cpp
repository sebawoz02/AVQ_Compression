#include <cmath>
#include <encoder.hpp>
#include <output_writer.hpp>
// #include <iostream>
#include <types/growing_point.hpp>

namespace encoder {
  void Encoder::encode(const std::vector<std::vector<Pixel>>& image, size_t width, size_t height)
  {
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

    adaptive_vector_quantization(Image(red, width, height));
    adaptive_vector_quantization(Image(green, width, height));
    adaptive_vector_quantization(Image(blue, width, height));
  }

  void Encoder::adaptive_vector_quantization(Image image)
  {
    output_writer::OutWriter out_writer(*out_file);

    // Add pixel [0, 0] to growing points pool
    auto* init_gp = new Growing_point(0, 0);
    auto* gp_pool = new GP_pool();
    gp_pool->add(init_gp);

    size_t gpp_size = 1;

    // Init dictionary
    Dictionary* dict = dict_init_heur();

    // While growing points pool has more elements
    while(gpp_size > 0) {
      // Determine next growing point
      Growing_point* gp = growing_heur(gp_pool);

      size_t common_block_idx;
      Block* picked_block;
      match_heur(dict, tolerance, image, gp, &common_block_idx, &picked_block);

      auto bits_to_transmit = static_cast<int8_t>(
        std::ceil(log2(static_cast<double>(dict->size()))));

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

      /*
        std::cout << "gpp_size: " << gpp_size << std::endl;
        std::cout << "common_block_idx: " << common_block_idx << std::endl;
        std::cout << "dict.size(): " << dict->size() << std::endl;
        std::cout << "gp.x: " << gp->x << std::endl;
        std::cout << "gp.y: " << gp->y << std::endl;

        std::cout << "dict[common_block_idx] :" << std::endl;
        std::cout << "[ ";
        Block* tmp = (*dict)[common_block_idx];
        for(size_t i = 0; i < tmp->width; i++)
        {
            std::cout << "[ ";
            for(size_t j = 0; j < tmp->height; j++)
            {
                std::cout << (int)tmp->pixels[i][j] << ", ";
            }
            std::cout << "], ";
        }
        std::cout << "]" << std::endl;

        std::cout << "======================" << std::endl;
        */
      // Update growing points pool
      growing_point_update_heur(image, gp_pool, &gpp_size, gp);
    }

    delete dict;
    delete gp_pool;
  }

} // namespace encoder
