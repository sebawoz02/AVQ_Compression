#include <cmath>
#include <encoder.hpp>
#include <io_handler.hpp>
#include <types/growing_point.hpp>

void Encoder::encode(const std::vector<std::vector<Pixel>>& image, size_t width,
                     size_t height)
{
  std::vector<std::vector<uint8_t>> red(
    std::vector<std::vector<uint8_t>>(width, std::vector<uint8_t>(height, 0)));
  std::vector<std::vector<uint8_t>> green(
    std::vector<std::vector<uint8_t>>(width, std::vector<uint8_t>(height, 0)));
  std::vector<std::vector<uint8_t>> blue(
    std::vector<std::vector<uint8_t>>(width, std::vector<uint8_t>(height, 0)));

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
  // Add pixel [0, 0] to growing points pool
  auto* init_gp = new Growing_point(0, 0);
  auto* gp_pool = new GP_pool();
  gp_pool->add(init_gp);

  // Init dictionary
  Dictionary* dict = dict_init_heur();

  // While growing points pool has more elements
  while(gp_pool->size() > 0) {
    // Determine next growing point
    Growing_point* gp = growing_heur(gp_pool);

    size_t common_block_idx;
    Block* picked_block;
    match_heur(dict, tolerance, image, gp, &common_block_idx, &picked_block);

    auto bits_to_transmit =
      static_cast<int8_t>(std::ceil(log2(static_cast<double>(dict->size()))));

    // Write common_block_idx to file on log2(dict.size) bits
    for(int8_t i = bits_to_transmit - 1; i >= 0; i--) {
      bool bit = (common_block_idx >> i) & 1;
      io_handler->write(bit);
    }
    // Update dict
    dict_update_heur(dict, picked_block, gp, image);
    delete picked_block;
    // Check if dictionary is full and if so use deletion heuristic
    deletion_heur(dict);
    // Update growing points pool
    growing_point_update_heur(image, gp_pool, gp);
  }

  delete dict;
  delete gp_pool;
}
